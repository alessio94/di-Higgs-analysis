#include "CAFbbll/PUWeight.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include "xAODEventInfo/EventInfo.h"

PUWeight::PUWeight() { DEBUGclass("Default constructor called"); }

PUWeight::~PUWeight() { DEBUGclass("Destructor called"); }

PUWeight::PUWeight(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());

    TString input = name;

    TString obs_name;
    if (!TQStringUtils::readToken(input, obs_name, TQStringUtils::alphanumvarext)) {
        ERRORclass("The name of the PUWeight has to have the following scheme: 'Name<:Variation>'");
        BREAK("Error in creating PUWeight observable");
    }
    DEBUGclass("Observable name is %s", obs_name.Data());

    if (!input.IsNull()) { // there is a ":variation" after the observable name
        if (TQStringUtils::removeLeading(input, ":") != 1) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating PUWeight observable");
        }

        if (input.IsNull()) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating PUWeight observable");
        }
        m_weight_variation = input.Data();
        DEBUGclass("Using weight variation %s", m_weight_variation.c_str());
    } else {
        DEBUGclass("No systematic weight variation specified.");
    }
}

bool PUWeight::initializeSelf() {
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

double PUWeight::getValue() const {
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
    const xAOD::EventInfo* event_info = nullptr;
    if (!get_xaod_eventinfo(fEvent, m_variation, event_info)) {
        return false;
    }

    float weight_pileup = event_info->auxdataConst<float>("PileupReweight");
    DEBUGclass("weight_pileup=%f", weight_pileup);

    return weight_pileup;
}
