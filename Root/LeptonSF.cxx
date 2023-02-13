#include "CAFbbll/LeptonSF.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include <algorithm>
#include <string>

LeptonSF::LeptonSF() { DEBUGclass("Default constructor called"); }

LeptonSF::~LeptonSF() { DEBUGclass("Destructor called"); }

LeptonSF::LeptonSF(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());

    TString input = name;

    TString obs_name;
    if (!TQStringUtils::readToken(input, obs_name, TQStringUtils::alphanumvarext)) {
        ERRORclass("The name of the LeptonSF has to have the following scheme: 'Name<:Variation>'");
        BREAK("Error in creating LeptonSF observable");
    }
    DEBUGclass("Observable name is %s", obs_name.Data());

    if (!input.IsNull()) { // there is a ":variation" after the observable name
        if (TQStringUtils::removeLeading(input, ":") != 1) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating LeptonSF observable");
        }

        if (input.IsNull()) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating LeptonSF observable");
        }
        m_weight_variation = input.Data();
        DEBUGclass("Using weight variation %s", m_weight_variation.c_str());
    } else {
        DEBUGclass("No systematic weight variation specified.");
    }
}

bool LeptonSF::initializeSelf() {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }
    m_p4_variation = fSample->getTagStandardStringDefault("~p4Variation", "Nominal");

    if (m_weight_variation.empty()) {
        m_variation = m_p4_variation;
    } else {
        if (m_p4_variation == "Nominal") {
            m_variation = m_weight_variation;
        } else {
            // skip this configuration as we don't want weight variations on top of p4 variations
            m_variation = "";
        }
    }

    return true;
}

double LeptonSF::getValue() const {
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

    const xAOD::IParticleContainer* leptons = nullptr;
    if (!get_bbll_lepton_container(fEvent, m_variation, leptons)) {
        return false;
    }

    float weight = 1;
    for (xAOD::IParticle const* lepton : *leptons) {
        if (lepton->type() == xAOD::Type::Electron) {
            weight *= lepton->auxdataConst<float>("effSFReco");               // reco
            weight *= lepton->auxdataConst<float>(m_electron_id_sf.c_str());  // id
            weight *= lepton->auxdataConst<float>(m_electron_iso_sf.c_str()); // iso
        } else if (lepton->type() == xAOD::Type::Muon) {
            weight *= lepton->auxdataConst<double>("mediumEffSF");        // id
            weight *= lepton->auxdataConst<float>(m_muon_iso_sf.c_str()); // iso
        }
    }

    DEBUGclass("[%s] weight=%f", m_variation.c_str(), weight);
    return weight;
}

void LeptonSF::set_electron_id(const std::string& id) {
    std::string id_lower = id;
    std::for_each(id_lower.begin(), id_lower.end(), [](char& c) { c = std::tolower(c); });
    m_electron_id_sf = "effSF" + id_lower + "LH";
    DEBUGclass("Set electron ID scale factor to '%s'", m_electron_id_sf.c_str());
}

void LeptonSF::set_electron_iso(const std::string& iso, const std::string& id) {
    m_electron_iso_sf = "effSFIso" + iso + id + "LH";
    DEBUGclass("Set electron ISO scale factor to '%s'", m_electron_iso_sf.c_str());
}

void LeptonSF::set_muon_iso(const std::string& iso) {
    m_muon_iso_sf = iso + "IsoSF";
    DEBUGclass("Set muon ISO scale factor to '%s'", m_muon_iso_sf.c_str());
}
