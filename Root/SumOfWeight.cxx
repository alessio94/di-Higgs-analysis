#include "CAFbbll/SumOfWeight.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"
#include "QFramework/TQUtils.h"

#include "xAODEventInfo/EventInfo.h"

#include "TFile.h"

// initialize static member variables
std::map<std::string, std::set<std::string>> SumOfWeight::s_weight_variation_initialization_list = { { "Default",
    { "Nominal" } } };
std::map<std::string, bool> SumOfWeight::s_initialized;
std::map<std::string, std::map<std::string, double>> SumOfWeight::s_sum_of_weights;

SumOfWeight::SumOfWeight() { DEBUGclass("Default constructor called"); }

SumOfWeight::~SumOfWeight() { DEBUGclass("Destructor called"); }

SumOfWeight::SumOfWeight(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());

    TString input = name;

    TString obs_name;
    if (!TQStringUtils::readToken(input, obs_name, TQStringUtils::alphanumvarext)) {
        ERRORclass("The name of the SumOfWeight has to have the following scheme: 'SumOfWeight:MCName:Variation'");
        BREAK("Error in creating SumOfWeight observable");
    }
    DEBUGclass("Observable name is %s", obs_name.Data());

    if (!input.IsNull()) { // there is a ":variation" after the observable name
        // read mc name
        if (TQStringUtils::removeLeading(input, ":") != 1) {
            if (input.IsNull()) {
                ERRORclass("Missing mc name declaration in expression '%s'", name.Data());
                BREAK("Error in creating SumOfWeight observable");
            }
        }

        TString mc_name;
        if (!TQStringUtils::readToken(input, mc_name, TQStringUtils::alphanumvarext)) {
            ERRORclass("The name of the SumOfWeight has to have the following scheme: 'SumOfWeight:MCName:Variation'");
            BREAK("Error in creating SumOfWeight observable");
        }
        m_mc_name = mc_name.Data();
        DEBUGclass("Using mc name %s", m_mc_name.c_str());
        // read variation
        if (TQStringUtils::removeLeading(input, ":") != 1) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating SumOfWeight observable");
        }
        if (input.IsNull()) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating SumOfWeight observable");
        }
        m_weight_variation = input.Data();
        DEBUGclass("Using weight variation %s", m_weight_variation.c_str());
    } else {
        DEBUGclass("No systematic weight variation specified.");
    }
}

void SumOfWeight::addWeightToInitializationList(const std::string& sample, const std::string& variation) {
    s_weight_variation_initialization_list[sample].emplace(variation);
    s_initialized[sample] = false;
}

void replace(std::string& s, const std::string& s_find, const std::string& s_rep) {
    auto idx = s.find(s_find);
    while (idx != std::string::npos) {
        s.replace(idx, s_find.length(), s_rep);
        idx = s.find(s_find);
    }
}

bool SumOfWeight::initializeSumOfWeights(
    const std::string& sample, const std::string& default_sumw_hist, const TString& filename) {
    if (s_initialized[sample]) {
        return true;
    }
    DEBUGclass("Called initializeSumOfWeights with sample=%s", sample.c_str());

    if (s_weight_variation_initialization_list.find(sample) == s_weight_variation_initialization_list.end()) {
        ERRORclass("Could not find variations in initializer list for process type '%s'", sample.c_str());
        return false;
    }

    TFile* sample_file = TQUtils::openFile(filename);
    if (!sample_file) {
        ERRORclass("Failed to access sample file '%s'", filename.Data());
        return false;
    }

    for (const std::string& variation : s_weight_variation_initialization_list[sample]) {
        std::string hist_name;

        DEBUGclass("Parsing variation '%s'", variation.c_str());
        DEBUGclass("sample = %s", sample.c_str());
        // name of histogram is specified
        if (variation.rfind("Name_") == 0) {
            hist_name = variation;
            hist_name.erase(0, sizeof("Name_") - 1);
        }
        // FSR
        else if (variation == "FSR_Up") {
            hist_name = "LHE3Weight_isr:muRfac=10_fsr:muRfac=20";
        } else if (variation == "FSR_Down") {
            hist_name = "LHE3Weight_isr:muRfac=10_fsr:muRfac=05";
        }
        // scale variation
        else if (variation.rfind("Scale_") == 0) {
            if (sample == "SignalPy8" || sample == "SignalVBF" || sample == "SignalVBFHw7") {
                hist_name = variation;
                replace(hist_name, "Scale", "AllExecutedEvents_NonNominalMCWeight");
            } else if (sample == "SignalHw7") {
                hist_name = variation;
                hist_name.erase(0, sizeof("Scale_") - 1);
                replace(hist_name, "muR_", "renscfact=");
                replace(hist_name, "_muF_", "facscfact=");
                hist_name = "LHE3Weight_" + hist_name;
            } else if (sample == "Zee" || sample == "Zmm") {
                hist_name = variation;
                hist_name.erase(0, sizeof("Scale_") - 1);
                replace(hist_name, "muR_", "MUR");
                replace(hist_name, "muF_", "MUF");
                replace(hist_name, "10", "1");
                replace(hist_name, "20", "2");
                hist_name = "LHE3Weight_" + hist_name + "_PDF261000";
            } else if (sample == "TTBar" || sample == "STop") {
                hist_name = variation;
                hist_name.erase(0, sizeof("Scale_") - 1);
                replace(hist_name, "muR_", "muR=");
                replace(hist_name, "_muF_", ",muF=");
                hist_name = "LHE3Weight_" + hist_name;
            } else {
                BREAKclass("Scale variations not configured for process class '%s'", sample.c_str());
            }
        }
        // PDF
        else if (variation.rfind("PDF_") == 0) {
            if (sample == "SignalPy8" || sample == "SignalVBF" || sample == "SignalVBFHw7") {
                hist_name = variation;
                replace(hist_name, "PDF", "AllExecutedEvents_NonNominalMCWeight");
            } else if (sample == "SignalHw7") {
                hist_name = variation;
                hist_name.erase(0, sizeof("PDF_") - 1);
                hist_name = "LHE3Weight_lhapdf=" + hist_name;
            } else if (sample == "Zee" || sample == "Zmm") {
                hist_name = variation;
                hist_name.erase(3, 1); // erase underscore after "PDF"
                hist_name = "LHE3Weight_MUR1_MUF1_" + hist_name;
            } else if (sample == "TTBar" || sample == "STop") {
                hist_name = variation;
                hist_name.erase(0, sizeof("PDF_") - 1);
                hist_name = "LHE3Weight_PDFset=" + hist_name;
            } else {
                BREAKclass("PDF variations not configured for process class '%s'", sample.c_str());
            }
        } else {
            if (variation != "Nominal") {
                WARNclass("Unkown systematic name '%s'. Will use default histogram.", variation.c_str());
            }

            hist_name = default_sumw_hist;
        }

        DEBUGclass("[%s] variation: %s", sample.c_str(), variation.c_str());
        DEBUGclass("[%s] hist_name: %s", sample.c_str(), hist_name.c_str());

        TH1D* h_sumofweight = (TH1D*)(sample_file->Get(hist_name.c_str()));
        if (!h_sumofweight) {
            ERRORclass("Could not retrieve histogram '%s'!", hist_name.c_str());
            sample_file->Close();
            return false;
        }
        s_sum_of_weights[sample][variation] = h_sumofweight->GetBinContent(2);
    }

    sample_file->Close();
#ifdef _DEBUG_
    DEBUGclass("Summary for process type '%s'", sample.c_str());
    for (const auto& sumw_info : s_sum_of_weights[sample]) {
        DEBUGclass("[%s] %s: %f", sample.c_str(), sumw_info.first.c_str(), sumw_info.second);
    }
#endif

    s_initialized[sample] = true;

    return true;
}

bool SumOfWeight::initializeSelf() {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }
    int dsid;
    fSample->getTag("~dsid", dsid);
    std::string default_sumw_hist;
    default_sumw_hist = fSample->getTagStandardStringDefault("~nominalSumWHist", "nominal");

    // only run on systematics on required samples
    // this allows us to run on samples which don't have these weights in the same job

    bool is_valid_variation = false;

    if (m_mc_name == "SignalPy8" && ((dsid >= 600465 && dsid <= 600470) || dsid == 600863 || dsid == 600898)) {
        is_valid_variation = true;
    }

    // bbtautau hadhad, bbtautau lephad, 4b, bbtautau 2l, bbWW 2l, bbZZ 2l, bbyy, bbZZ 2l2q, bbWW 2l (kl10),
    // bbtautau 2l (kl10), bbZZ 2l2v (kl=10), bbZZ 2l2q (kl=10)
    const std::set<int> signal_dsids = { 600023, 600029, 600043, 600045, 600047, 600049, 600051, 600761, 600046, 600048,
        600050, 600772 };
    if (m_mc_name == "SignalHw7" && signal_dsids.find(dsid) != signal_dsids.end()) {
        is_valid_variation = true;
    }

    if (m_mc_name == "SignalVBF" && (dsid >= 503016 && dsid <= 503051)) {
        is_valid_variation = true;
    }

    // VBF bbZZ2l22q SM, VBF bbZZ2l2q (cvv=0) and VBF bbZZ2l2q (kl=10)
    if (m_mc_name == "SignalVBFHw7" && (dsid == 512131 || dsid == 512132 || dsid == 512133)) {
        is_valid_variation = true;
    }

    // Zee (main), Zee/Zmm verylowmass, Zee/Zmm EWK
    if (m_mc_name == "Zee"
        && ((dsid >= 364114 && dsid <= 364127) || (dsid >= 364358 && dsid <= 364363)
            || (dsid >= 308092 && dsid <= 308094))) {
        is_valid_variation = true;
    }

    if (m_mc_name == "Zmm" && (dsid >= 364100 && dsid <= 364113)) {
        is_valid_variation = true;
    }

    if (m_mc_name == "TTBar" && (dsid >= 410470 && dsid <= 410472)) {
        is_valid_variation = true;
    }

    const std::set<int> stop_dsids = { 410644, 410645, 410658, 410659 };
    if (m_mc_name == "STop" && stop_dsids.find(dsid) != stop_dsids.end()) {
        is_valid_variation = true;
    }

    if (is_valid_variation) {
        if (!initializeSumOfWeights(m_mc_name, default_sumw_hist, fSample->getFilename())) {
            return false;
        }
    } else {
        if (!initializeSumOfWeights("Default", default_sumw_hist, fSample->getFilename())) {
            return false;
        }
    }

    if (is_valid_variation) {
        m_sum_of_weight = s_sum_of_weights[m_mc_name][m_weight_variation];
    } else {
        m_sum_of_weight = s_sum_of_weights["Default"]["Nominal"];
    }

    return true;
}

bool SumOfWeight::finalizeSelf() {
    s_sum_of_weights.clear();
    s_initialized.clear();
    return true;
}

double SumOfWeight::getValue() const {
    bool isMC = false;
    fSample->getTag("~isMC", isMC);
    if (!isMC) {
        DEBUGclass("This is a data event");
        return 1;
    }

    DEBUGclass("[%s] %f", m_weight_variation.c_str(), m_sum_of_weight);
    return m_sum_of_weight;
}
