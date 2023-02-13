#include "CAFbbll/BTagSF.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicVariation.h"
#include "xAODJet/JetContainer.h"

BTagSF::BTagSF() { DEBUGclass("Default constructor called"); }

BTagSF::~BTagSF() { DEBUGclass("Destructor called"); }

BTagSF::BTagSF(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());

    TString input = name;

    if (!TQStringUtils::removeLeadingText(input, "BTagSF:")) {
        ERRORclass("The name of the BTagSF observable has to have the following schema: "
                   "'BTagSF:Algorithm:WorkingPoint<:Variation>'");
        BREAK("Error in creating BTagSF observable");
    }

    if (!TQStringUtils::readToken(input, m_tagger, TQStringUtils::alphanumvarext)) {
        ERRORclass("Unable to read b-tagging algorithm from expression '%s'", input.Data());
        ERRORclass("Unable to parse expression '%s'", name.Data());
        BREAK("Error in creating BTagSF observable");
    }
    if (TQStringUtils::removeLeading(input, ":") != 1) {
        ERRORclass("Missing working point declaration in expression '%s'", name.Data());
        BREAK("Error in creating BTagSF observable");
    }

    if (!TQStringUtils::readToken(input, m_operating_point, TQStringUtils::alphanumvarext)) {
        ERRORclass("The name of the BTagSF observable has to have the following schema: "
                   "'BTagSF:Algorithm:WorkingPoint<:Variation>'");
        BREAK("Error in creating BTagSF observable");
    }
    DEBUGclass("Working point is %s", m_operating_point.Data());
    m_is_bjet_auxdata_string = ("CAFisBJet_" + m_tagger + "_" + m_operating_point).Data();

    if (!input.IsNull()) { // there is a ":variation" after the observable name
        if (TQStringUtils::removeLeading(input, ":") != 1) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating BTagSF observable");
        }

        if (input.IsNull()) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating BTagSF observable");
        }
        m_weight_variation = input.Data();
        DEBUGclass("Using weight variation %s", m_weight_variation.c_str());
    } else {
        DEBUGclass("No systematic weight variation specified.");
    }

    DEBUGclass("Initialized BTagSF observable with algorithm='%s' and working point '%s'", m_tagger.Data(),
        m_operating_point.Data());
    DEBUGclass("AuxDataString=%s", m_is_bjet_auxdata_string.c_str());
}

bool BTagSF::initializeSelf() {
    DEBUGclass("initializeSelf called");
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }
    m_p4_variation = fSample->getTagStandardStringDefault("~p4Variation", "Nominal");

    if (m_weight_variation.empty()) {
        // use Nominal configuration for all p4 variations
        m_variation = "Nominal";
    } else {
        if (m_p4_variation == "Nominal") {
            m_variation = m_weight_variation;
        } else {
            // skip this configuration as we don't want weight variations on top of p4 variations
            m_variation = "";
            return true;
        }
    }

    // initialize btagging SF tool
    std::string tool_name =
        ("BTaggingEfficiencyTool/BTagSF_" + m_tagger + "_" + m_operating_point + "_" + m_variation).Data();
    m_btag_sf_tool = asg::AnaToolHandle<IBTaggingEfficiencyTool>(tool_name);

    if (!m_btag_sf_tool.isUserConfigured()) {
        INFOclass("Configuring b-tagging tool '%s'", tool_name.c_str());

        m_btag_sf_tool.setProperty("OutputLevel", MSG::WARNING).ignore(); // disable info messages

        if (m_btag_sf_tool.setProperty("TaggerName", m_tagger.Data()) != StatusCode::SUCCESS) {
            ERRORclass("Could not set property 'TaggerName' to '%s'", m_tagger.Data());
            return false;
        }

        if (m_btag_sf_tool.setProperty("OperatingPoint", m_operating_point.Data()) != StatusCode::SUCCESS) {
            ERRORclass("Could not set property 'OperatingPoint' to '%s'", m_operating_point.Data());
            return false;
        }

        if (m_btag_sf_tool.setProperty("JetAuthor", jet_container_name()) != StatusCode::SUCCESS) {
            ERRORclass("Could not set property 'JetAuthor' to '%s'", jet_container_name().c_str());
            return false;
        }

        if (m_btagging_cdi_file.empty()) {
            ERRORclass("b-tagging CDI file not set!");
            return false;
        }
        if (m_btag_sf_tool.setProperty("ScaleFactorFileName", m_btagging_cdi_file) != StatusCode::SUCCESS) {
            ERRORclass("Could not set property 'ScaleFactorFileName' to '%s'", m_btagging_cdi_file.c_str());
            return false;
        }

        if (m_btag_sf_tool.setProperty("MinPt", 20000) != StatusCode::SUCCESS) {
            ERRORclass("Could not set property 'MinPt' to '20000'");
            return false;
        }

        if (m_btag_sf_tool.setProperty("EigenvectorReductionB", "Medium") != StatusCode::SUCCESS) {
            ERRORclass("Could not set property 'EigenvectorReductionB' to 'Medium'");
            return false;
        }

        if (m_btag_sf_tool.setProperty("EigenvectorReductionC", "Medium") != StatusCode::SUCCESS) {
            ERRORclass("Could not set property 'EigenvectorReductionC' to 'Medium'");
            return false;
        }

        if (m_btag_sf_tool.setProperty("EigenvectorReductionLight", "Medium") != StatusCode::SUCCESS) {
            ERRORclass("Could not set property 'EigenvectorReductionLight' to 'Medium'");
            return false;
        }
    }

    INFOclass("Initializing/retrieving tool %s", tool_name.c_str());
    if (m_btag_sf_tool.retrieve() != StatusCode::SUCCESS) {
        ERRORclass("Could not retrieve the tool '%s'", m_btag_sf_tool.name().c_str());
        return false;
    }

    bool print_allowed_variations = false;
    bool success = true;
    if (m_p4_variation == "Nominal" && !m_weight_variation.empty()) {
        CP::SystematicSet set;

        const std::map<CP::SystematicVariation, std::vector<std::string>> allowed_variations =
            m_btag_sf_tool->listSystematics();
        for (const auto& var : allowed_variations) {
            if (var.first.name() == m_weight_variation) {
                set.insert(var.first);
            }
        }

        if (set.empty()) {
            ERRORclass("Could not find systematic '%s' in list of supported sytematics", m_weight_variation.c_str());
            print_allowed_variations = true;
            success = false;
        } else {
            if (m_btag_sf_tool->applySystematicVariation(set) != StatusCode::SUCCESS) {
                ERRORclass("Could not set systematic to %s", m_weight_variation.c_str());
                return false;
            }
        }
    }

#ifdef _DEBUG_
    if (m_p4_variation == "Nominal" && m_weight_variation.empty()) {
        print_allowed_variations = true;
    }
#endif
    if (print_allowed_variations) {
        const std::map<CP::SystematicVariation, std::vector<std::string>> allowed_variations =
            m_btag_sf_tool->listSystematics();
        std::cout << "Allowed systematics variations for tool " << m_btag_sf_tool.name() << ":" << std::endl;
        for (auto var : allowed_variations) {
            std::cout << std::setw(40) << std::left << var.first.name() << ":";
            for (auto flv : var.second)
                std::cout << " " << flv;
            std::cout << std::endl;
        }
    }

    DEBUGclass("initializeSelf done");
    return success;
}

double BTagSF::getValue() const {
    bool isMC = false;
    fSample->getTag("~isMC", isMC);
    if (!isMC) {
        DEBUGclass("This is a data event");
        return 1;
    }

    if (m_variation.empty()) {
        // skip p4+weight sys at the same time
        return 0;
    }

    DEBUGclass("[%s] This is a MC event", m_variation.c_str());

    const xAOD::JetContainer* central_jets = nullptr;
    if (!get_bbll_centraljet_container(fEvent, m_variation, central_jets)) {
        return false;
    }

    float weight = 1.0;
    int i = 0;
    for (const xAOD::Jet* jet : *central_jets) { // b-tagging only works for jets with |eta| < 2.5
        float btag_sf = 1.0;
        // b-tagging only works for jets with pt > 20 GeV
        if (jet->pt() > 20 * 1000) {
            if (jet->auxdataConst<int>(m_is_bjet_auxdata_string)) {
                if (m_btag_sf_tool->getScaleFactor(*jet, btag_sf) != CP::CorrectionCode::Ok) {
                    WARNclass("[%s] There was an error retrieving the b-tagging efficiency scale factor (%s, %s) for "
                              "jet %d, pt=%f, eta=%f",
                        m_variation.c_str(), m_tagger.Data(), m_operating_point.Data(), i, jet->pt(), jet->eta());
                    btag_sf = 1;
                }
                DEBUGclass("[%s] jet %d (pt=%6.2f, eta=%1.2f, phi=%1.2f, e=%6.2f, label=%d): b-jet eff sf=%f",
                    m_variation.c_str(), i, jet->pt(), jet->eta(), jet->phi(), jet->e(),
                    jet->auxdataConst<int>("HadronConeExclTruthLabelID"), btag_sf);
            } else {
                if (m_btag_sf_tool->getInefficiencyScaleFactor(*jet, btag_sf) != CP::CorrectionCode::Ok) {
                    WARNclass("[%s] There was an error retrieving the b-tagging inefficiency scale factor (%s, %s) for "
                              "jet %d, pt=%f, eta=%f",
                        m_variation.c_str(), m_tagger.Data(), m_operating_point.Data(), i, jet->pt(), jet->eta());
                    btag_sf = 1;
                }
                DEBUGclass("[%s] jet %d (pt=%6.2f, eta=%1.2f, phi=%1.2f, e=%6.2f, label=%d): b-jet ineff sf=%f",
                    m_variation.c_str(), i, jet->pt(), jet->eta(), jet->phi(), jet->e(),
                    jet->auxdataConst<int>("HadronConeExclTruthLabelID"), btag_sf);
            }
        } else {
            DEBUGclass("[%s] jet %d has pt < 20 GeV => no b-jet SF available", m_variation.c_str(), i);
        }

        weight *= btag_sf;
        ++i;
    }

    DEBUGclass("[%s] total b-tag sf weight = %f", m_variation.c_str(), weight);

    return weight;
}

void BTagSF::set_jet_collection(std::string name, std::string btaggin_version) {
    std::string jet_container_name = "AntiKt4" + name + "Jets_BTagging" + btaggin_version;
    std::string met_container_name = "MET_Reference_AntiKt4" + name;

    set_jet_container_name(jet_container_name);
    set_met_container_name(met_container_name);
    DEBUGclass("Set jet container name to '%s'", jet_container_name.c_str());
    DEBUGclass("Set met container name to '%s'", met_container_name.c_str());
}

void BTagSF::set_btagging_cdi_file(std::string cdi_file) {
    m_btagging_cdi_file = cdi_file;
    DEBUGclass("Set b-tagging CDI file to '%s'", cdi_file.c_str());
}
