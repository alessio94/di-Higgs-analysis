#include "CAFbbll/MCWeight.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include "xAODEventInfo/EventInfo.h"

MCWeight::MCWeight() { DEBUGclass("Default constructor called"); }

MCWeight::~MCWeight() { DEBUGclass("Destructor called"); }

MCWeight::MCWeight(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());

    TString input = name;

    TString obs_name;
    if (!TQStringUtils::readToken(input, obs_name, TQStringUtils::alphanumvarext)) {
        ERRORclass("The name of the MCWeight has to have the following scheme: 'MCWeight:MCName:Index'");
        BREAK("Error in creating MCWeight observable");
    }
    DEBUGclass("Observable name is %s", obs_name.Data());

    if (!input.IsNull()) { // there is a ":variation" after the observable name
        // read mc name
        if (TQStringUtils::removeLeading(input, ":") != 1) {
            if (input.IsNull()) {
                ERRORclass("Missing mc name declaration in expression '%s'", name.Data());
                BREAK("Error in creating MCWeight observable");
            }
        }

        TString mc_name;
        if (!TQStringUtils::readToken(input, mc_name, TQStringUtils::alphanumvarext)) {
            ERRORclass("The name of the MCWeight has to have the following scheme: 'MCWeight:MCName:Index'");
            BREAK("Error in creating MCWeight observable");
        }
        DEBUGclass("MC name is %s", mc_name.Data());
        m_mc_name = mc_name.Data();
        DEBUGclass("Using mc name %s", m_mc_name.c_str());
        // read variation
        if (TQStringUtils::removeLeading(input, ":") != 1) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating MCWeight observable");
        }
        if (input.IsNull()) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating MCWeight observable");
        }
        m_weight_index = std::atoi(input.Data());
        DEBUGclass("Using weight index %d", m_weight_index);
    } else {
        DEBUGclass("No systematic weight variation specified.");
    }
}

bool MCWeight::initializeSelf() {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }
    int dsid;
    fSample->getTag("~dsid", dsid);

    // only run on systematics on required samples
    // this allows us to run on samples which don't have these weights in the same job

    if (m_mc_name == "SignalPy8" && !((dsid >= 600465 && dsid <= 600470) || dsid == 600863 || dsid == 600898)) {
        m_weight_index = -1;
    }

    // bbtautau hadhad, bbtautau lephad, 4b, bbtautau 2l, bbWW 2l, bbZZ 2l2v, bbyy, bbZZ 2l2q, bbWW 2l (kl10), bbtautau
    // 2l (kl10), bbZZ 2l2v (kl=10), bbZZ 2l2q (kl=10)
    const std::set<int> signal_hw7_dsids = { 600023, 600029, 600043, 600045, 600047, 600049, 600051, 600761, 600046,
        600048, 600050, 600772 };
    if (m_mc_name == "SignalHw7" && signal_hw7_dsids.find(dsid) == signal_hw7_dsids.end()) {
        m_weight_index = -1;
    }

    if (m_mc_name == "SignalVBF" && !(dsid >= 503016 && dsid <= 503051)) {
        m_weight_index = -1;
    }

    // VBF bbZZ2l22q SM, VBF bbZZ2l2q (cvv=0) and VBF bbZZ2l2q (kl=10)}
    if (m_mc_name == "SignalVBFHw7" && !(dsid == 512131 || dsid == 512132 || dsid == 512133)) {
        m_weight_index = -1;
    }

    // Zee (main), Zee/Zmm verylowmass, Zee/Zmm EWK
    if (m_mc_name == "Zee"
        && !((dsid >= 364114 && dsid <= 364127) || (dsid >= 364358 && dsid <= 364363)
            || (dsid >= 308092 && dsid <= 308094))) {
        m_weight_index = -1;
    }

    if (m_mc_name == "Zmm" && !(dsid >= 364100 && dsid <= 364113)) {
        m_weight_index = -1;
    }

    if (m_mc_name == "TTBar" && !(dsid >= 410470 && dsid <= 410472)) {
        m_weight_index = -1;
    }

    const std::set<int> stop_dsids = { 410648, 410649 };
    if (m_mc_name == "STop" && stop_dsids.find(dsid) == stop_dsids.end()) {
        m_weight_index = -1;
    }

    return true;
}

double MCWeight::getValue() const {
    bool isMC = false;
    fSample->getTag("~isMC", isMC);
    if (!isMC) {
        DEBUGclass("This is a data event");
        return 1;
    }

    if (m_weight_index < 0) {
        return 0;
    }

    DEBUGclass("This is a MC event");

    // get event info container
    const xAOD::EventInfo* event_info = nullptr;
    if (!get_xaod_eventinfo(fEvent, "Nominal", event_info)) {
        return false;
    }

    float weight = event_info->auxdataConst<std::vector<float>>("MCEventWeightSys").at(m_weight_index);

    DEBUGclass("[weight_index=%d] weight=%f", m_weight_index, weight);
    return weight;
}
