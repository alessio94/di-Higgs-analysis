#include "CAFbbll/JvtSF.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include "xAODJet/JetContainer.h"

JvtSF::JvtSF() { DEBUGclass("Default constructor called"); }

JvtSF::~JvtSF() { DEBUGclass("Destructor called"); }

JvtSF::JvtSF(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());

    TString input = name;

    TString obs_name;
    if (!TQStringUtils::readToken(input, obs_name, TQStringUtils::alphanumvarext)) {
        ERRORclass("The name of the JvtSF has to have the following scheme: 'Name<:Variation>'");
        BREAK("Error in creating JvtSF observable");
    }
    DEBUGclass("Observable name is %s", obs_name.Data());

    if (!input.IsNull()) { // there is a ":variation" after the observable name
        if (TQStringUtils::removeLeading(input, ":") != 1) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating JvtSF observable");
        }

        if (input.IsNull()) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating JvtSF observable");
        }
        m_weight_variation = input.Data();
        DEBUGclass("Using weight variation %s", m_weight_variation.c_str());
    } else {
        DEBUGclass("No systematic weight variation specified.");
    }
}

bool JvtSF::initializeSelf() {
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

double JvtSF::getValue() const {
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

    const xAOD::JetContainer* forward_jets = nullptr;
    if (!get_bbll_forwardjet_container(fEvent, m_variation, forward_jets)) {
        return false;
    }

    float weight = 1.0;
    int i = 0;
    for (const xAOD::Jet* jet : *central_jets) {
        float jvt_sf = jet->auxdataConst<float>("JvtSF");
        DEBUGclass("[%s] central jet %d: jvt_sf=%f", m_variation.c_str(), i, jvt_sf);
        weight *= jvt_sf;
        ++i;
    }

    i = 0;
    for (const xAOD::Jet* jet : *forward_jets) {
        float fjvt_sf = jet->auxdataConst<float>("FJvtSF");
        DEBUGclass("[%s] forward jet %d: fjvt_sf=%f", m_variation.c_str(), i, fjvt_sf);
        weight *= fjvt_sf;
        ++i;
    }

    DEBUGclass("[%s] total jvt weight = %f", m_variation.c_str(), weight);
    return weight;
}