#include "CAFbbll/TriggerMatching.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

// xAOD
#include "xAODBase/IParticleContainer.h"
#include "xAODEventInfo/EventInfo.h"

// CxAODTools
#include "CxAODTools/CommonProperties.h"

TriggerMatching::TriggerMatching() { DEBUGclass("Default constructor called"); }

TriggerMatching::~TriggerMatching() { DEBUGclass("Destructor called"); }

TriggerMatching::TriggerMatching(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());
}

int TriggerMatching::year_to_trigger_id(bool is2015, bool is2016, bool is2017, bool is2018) {
    if (is2015) {
        return 0;
    }
    if (is2016) {
        return 1;
    }
    if (is2017) {
        return 2;
    }
    if (is2018) {
        return 3;
    }
    return -100;
}

double TriggerMatching::getValue() const {
    // logic for assigning a trigger to an event
    // 1. single Lepton Triggers
    //  - check leading lepton
    //  - check subleading lepton
    // 2. dilepton trigger
    //  - if different flavour: check asymetric trigger
    //  - check symmetric trigger

    // this observable returns a code which is calculated as follows
    // code = <trigger code> + <year>
    //
    // The trigger code is set based on which trigger is used
    // 10 -> single electron trigger (leading lepton triggered)
    // 15 -> single electron trigger (subleading lepton triggered)
    // 20 -> single muon trigger (leading lepton triggered)
    // 25 -> single muontrigger (subleading lepton triggered)
    // 30 -> asymmetric em trigger
    // 35 -> asymmetric me trigger
    // 40 -> symmetric ee trigger
    // 45 -> symmetric mm trigger
    // 50 -> symmetric em trigger
    // 55 -> symmetric me trigger
    //
    // The <year> variable is set based on the year the data was taken:
    // 2015 -> 0
    // 2016 -> 1
    // 2017 -> 2
    // 2018 -> 3
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    bool isMC = false;
    fSample->getTag("~isMC", isMC);
    std::string variation = fSample->getTagStandardStringDefault("~p4Variation", "Nominal");

    const double GeV = 1000;

    const xAOD::EventInfo* event_info = nullptr;
    if (!get_xaod_eventinfo(fEvent, variation, event_info)) {
        return false;
    }

    const xAOD::IParticleContainer* leptons = nullptr;
    if (!get_bbll_lepton_container(fEvent, variation, leptons)) {
        return false;
    }

    unsigned int run_number;
    if (isMC) {
        run_number = event_info->auxdataConst<unsigned int>("RandomRunNumber");
    } else {
        run_number = event_info->auxdataConst<unsigned int>("runNumber");
    }

    bool is2015 = (266904 <= run_number && run_number <= 284484);
    bool is2016 = (296939 <= run_number && run_number <= 311481);
    bool is2017 = (324320 <= run_number && run_number <= 341649);
    bool is2018 = (348197 <= run_number && run_number <= 364485);

    // accidental prescale of HLT_2e17_lhvloose_nod0_L12EM15VHI in runs B5-B8 in 2017
    bool is2017prescale = (326834 <= run_number && run_number <= 328393);

    if (!(is2015 || is2016 || is2017 || is2018)) {
        return 0;
    }

    if (leptons->size() != 2) {
        DEBUGclass("Encountered %d leptons, but expected exactly 2!", leptons->size());
        return 0;
    }

    const xAOD::IParticle* lepA = leptons->at(0);
    const xAOD::IParticle* lepB = leptons->at(1);

    // check for single lepton triggers

    // single electron trigger, 2015, e-lead >= 25 GeV
    if (is2015 && lepA->type() == xAOD::Type::Electron && lepA->pt() >= 25 * GeV
        && ((Props::passHLT_e24_lhmedium_L1EM20VH.get(event_info) && Props::matchHLT_e24_lhmedium_L1EM20VH.get(lepA))
            || (Props::passHLT_e60_lhmedium.get(event_info) && Props::matchHLT_e60_lhmedium.get(lepA))
            || (Props::passHLT_e120_lhloose.get(event_info) && Props::matchHLT_e120_lhloose.get(lepA)))) {
        return 10 + year_to_trigger_id(is2015, is2016, is2017, is2018);
    }
    if (is2015 && lepB->type() == xAOD::Type::Electron && lepB->pt() >= 25 * GeV
        && ((Props::passHLT_e24_lhmedium_L1EM20VH.get(event_info) && Props::matchHLT_e24_lhmedium_L1EM20VH.get(lepB))
            || (Props::passHLT_e60_lhmedium.get(event_info) && Props::matchHLT_e60_lhmedium.get(lepB))
            || (Props::passHLT_e120_lhloose.get(event_info) && Props::matchHLT_e120_lhloose.get(lepB)))) {
        return 15 + year_to_trigger_id(is2015, is2016, is2017, is2018);
    }

    // single electron trigger, 2016-2018, e-lead >= 27 GeV
    if ((is2016 || is2017 || is2018) && lepA->type() == xAOD::Type::Electron && lepA->pt() >= 27 * GeV
        && ((Props::passHLT_e26_lhtight_nod0_ivarloose.get(event_info)
                && Props::matchHLT_e26_lhtight_nod0_ivarloose.get(lepA))
            || (Props::passHLT_e60_lhmedium_nod0.get(event_info) && Props::matchHLT_e60_lhmedium_nod0.get(lepA))
            || (Props::passHLT_e140_lhloose_nod0.get(event_info) && Props::matchHLT_e140_lhloose_nod0.get(lepA)))) {
        return 10 + year_to_trigger_id(is2015, is2016, is2017, is2018);
    }
    if ((is2016 || is2017 || is2018) && lepB->type() == xAOD::Type::Electron && lepB->pt() >= 27 * GeV
        && ((Props::passHLT_e26_lhtight_nod0_ivarloose.get(event_info)
                && Props::matchHLT_e26_lhtight_nod0_ivarloose.get(lepB))
            || (Props::passHLT_e60_lhmedium_nod0.get(event_info) && Props::matchHLT_e60_lhmedium_nod0.get(lepB))
            || (Props::passHLT_e140_lhloose_nod0.get(event_info) && Props::matchHLT_e140_lhloose_nod0.get(lepB)))) {
        return 15 + year_to_trigger_id(is2015, is2016, is2017, is2018);
    }

    // single muon trigger, 2015, mu-lead >= 21 GeV
    if (is2015 && lepA->type() == xAOD::Type::Muon && lepA->pt() >= 21 * GeV
        && ((Props::passHLT_mu20_iloose_L1MU15.get(event_info) && Props::matchHLT_mu20_iloose_L1MU15.get(lepA))
            || (Props::passHLT_mu50.get(event_info) && Props::matchHLT_mu50.get(lepA)))) {
        return 20 + year_to_trigger_id(is2015, is2016, is2017, is2018);
    }
    if (is2015 && lepB->type() == xAOD::Type::Muon && lepB->pt() >= 21 * GeV
        && ((Props::passHLT_mu20_iloose_L1MU15.get(event_info) && Props::matchHLT_mu20_iloose_L1MU15.get(lepB))
            || (Props::passHLT_mu50.get(event_info) && Props::matchHLT_mu50.get(lepB)))) {
        return 25 + year_to_trigger_id(is2015, is2016, is2017, is2018);
    }

    // single muon trigger, 2016-2018, mu-lead >= 28 GeV
    if ((is2016 || is2017 || is2018) && lepA->type() == xAOD::Type::Muon && lepA->pt() >= 28 * GeV
        && ((Props::passHLT_mu26_ivarmedium.get(event_info) && Props::matchHLT_mu26_ivarmedium.get(lepA))
            || (Props::passHLT_mu50.get(event_info) && Props::matchHLT_mu50.get(lepA)))) {
        return 20 + year_to_trigger_id(is2015, is2016, is2017, is2018);
    }
    if ((is2016 || is2017 || is2018) && lepB->type() == xAOD::Type::Muon && lepB->pt() >= 28 * GeV
        && ((Props::passHLT_mu26_ivarmedium.get(event_info) && Props::matchHLT_mu26_ivarmedium.get(lepB))
            || (Props::passHLT_mu50.get(event_info) && Props::matchHLT_mu50.get(lepB)))) {
        return 25 + year_to_trigger_id(is2015, is2016, is2017, is2018);
    }

    // check for asymetric dilepton triggers

    // em
    if (lepA->type() == xAOD::Type::Electron && lepB->type() == xAOD::Type::Muon) {
        // 2015, no asymmetric em trigger
        // 2016, e >= 27GeV, m >= 9GeV
        if (is2016 && lepA->pt() >= 27 * GeV && lepB->pt() >= 9 * GeV
            && Props::passHLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1.get(event_info)
            && Props::matchHLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1.get(lepA)
            && Props::matchHLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1.get(lepB)) {
            return 30 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
        // 2017-2018, e >= 27GeV, m >= 9GeV
        if ((is2017 || is2018) && lepA->pt() >= 27 * GeV && lepB->pt() >= 9 * GeV
            && Props::passHLT_e26_lhmedium_nod0_mu8noL1.get(event_info)
            && Props::matchHLT_e26_lhmedium_nod0_mu8noL1.get(lepA)
            && Props::matchHLT_e26_lhmedium_nod0_mu8noL1.get(lepB)) {
            return 30 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
    }
    // me
    else if (lepA->type() == xAOD::Type::Muon && lepB->type() == xAOD::Type::Electron) {
        // 2015, e >= 9GeV, m >= 26GeV
        if (is2015 && lepA->pt() >= 26 * GeV && lepB->pt() >= 9 * GeV && Props::passHLT_e7_lhmedium_mu24.get(event_info)
            && Props::matchHLT_e7_lhmedium_mu24.get(lepA) && Props::matchHLT_e7_lhmedium_mu24.get(lepB)) {
            return 35 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
        // 2016-2018, e >= 9GeV, m >= 26GeV
        else if ((is2016 || is2017 || is2018) && lepA->pt() >= 26 * GeV && lepB->pt() >= 9 * GeV
            && Props::passHLT_e7_lhmedium_nod0_mu24.get(event_info) && Props::matchHLT_e7_lhmedium_nod0_mu24.get(lepA)
            && Props::matchHLT_e7_lhmedium_nod0_mu24.get(lepB)) {
            return 35 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
    }

    // check for dilepton triggers

    // ee
    if (lepA->type() == xAOD::Type::Electron && lepB->type() == xAOD::Type::Electron) {
        // 2015, e-lead >= 13GeV, e-sub >= 13GeV
        if (is2015 && lepA->pt() >= 13 * GeV && lepB->pt() >= 13 * GeV
            && Props::passHLT_2e12_lhloose_L12EM10VH.get(event_info) && Props::matchHLT_2e12_lhloose_L12EM10VH.get(lepA)
            && Props::matchHLT_2e12_lhloose_L12EM10VH.get(lepB)) {
            return 40 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
        // 2016, e-lead >= 18GeV, e-sub >= 18GeV
        if (is2016 && lepA->pt() >= 18 * GeV && lepB->pt() >= 18 * GeV
            && Props::passHLT_2e17_lhvloose_nod0.get(event_info) && Props::matchHLT_2e17_lhvloose_nod0.get(lepA)
            && Props::matchHLT_2e17_lhvloose_nod0.get(lepB)) {
            return 40 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
        // 2017-2018 (no accidental prescale), e-lead >= 18Gev, e-sub >= 18GeV
        if (((is2017 || is2018) && !is2017prescale) && lepA->pt() >= 18 * GeV && lepB->pt() >= 18 * GeV
            && ((Props::passHLT_2e17_lhvloose_nod0_L12EM15VHI.get(event_info)
                    && Props::matchHLT_2e17_lhvloose_nod0_L12EM15VHI.get(lepA)
                    && Props::matchHLT_2e17_lhvloose_nod0_L12EM15VHI.get(lepB))
                || (Props::passHLT_2e24_lhvloose_nod0.get(event_info) && Props::matchHLT_2e24_lhvloose_nod0.get(lepA)
                    && Props::matchHLT_2e24_lhvloose_nod0.get(lepB)))) {
            return 40 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
        // 2017 (accidental prescale), e-lead >= 25Gev, e-sub >= 25GeV
        if (is2017prescale && lepA->pt() >= 25 * GeV && lepB->pt() >= 25 * GeV
            && Props::passHLT_2e24_lhvloose_nod0.get(event_info) && Props::matchHLT_2e24_lhvloose_nod0.get(lepA)
            && Props::matchHLT_2e24_lhvloose_nod0.get(lepB)) {
            return 40 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
    }

    // mm
    else if (lepA->type() == xAOD::Type::Muon && lepB->type() == xAOD::Type::Muon) {
        // 2015 m-lead >= 19Gev, m-sub >= 10GeV
        if (is2015
            && ((lepA->pt() >= 19 * GeV && lepB->pt() >= 10 * GeV)
                || (lepB->pt() >= 19 * GeV && lepA->pt() >= 10 * GeV))
            && Props::passHLT_mu18_mu8noL1.get(event_info) && Props::matchHLT_mu18_mu8noL1.get(lepA)
            && Props::matchHLT_mu18_mu8noL1.get(lepB)) {
            return 45 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
        // 2016-2018 m-lead >= 24Gev, m-sub >= 10GeV
        if ((is2016 || is2017 || is2018)
            && ((lepA->pt() >= 24 * GeV && lepB->pt() >= 10 * GeV)
                || (lepB->pt() >= 24 * GeV && lepA->pt() >= 10 * GeV))
            && Props::passHLT_mu22_mu8noL1.get(event_info) && Props::matchHLT_mu22_mu8noL1.get(lepA)
            && Props::matchHLT_mu22_mu8noL1.get(lepB)) {
            return 45 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
    }
    // em
    else if (lepA->type() == xAOD::Type::Electron && lepB->type() == xAOD::Type::Muon) {
        // 2015e >= 18GeV, m >= 15GeV
        if (is2015 && lepA->pt() >= 18 * GeV && lepB->pt() >= 15 * GeV
            && Props::passHLT_e17_lhloose_mu14.get(event_info) && Props::matchHLT_e17_lhloose_mu14.get(lepA)
            && Props::matchHLT_e17_lhloose_mu14.get(lepB)) {
            return 50 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
        // 2016-2018, e >= 18GeV, m >= 15GeV
        if ((is2016 || is2017 || is2018) && lepA->pt() >= 18 * GeV && lepB->pt() >= 15 * GeV
            && Props::passHLT_e17_lhloose_nod0_mu14.get(event_info) && Props::matchHLT_e17_lhloose_nod0_mu14.get(lepA)
            && Props::matchHLT_e17_lhloose_nod0_mu14.get(lepB)) {
            return 50 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
    }
    // me
    else if (lepA->type() == xAOD::Type::Muon && lepB->type() == xAOD::Type::Electron) {
        // 2015, e >= 18GeV, m >= 15GeV
        if (is2015 && lepB->pt() >= 18 * GeV && lepA->pt() >= 15 * GeV
            && Props::passHLT_e17_lhloose_mu14.get(event_info) && Props::matchHLT_e17_lhloose_mu14.get(lepB)
            && Props::matchHLT_e17_lhloose_mu14.get(lepA)) {
            return 55 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
        // 2016-2018, e >= 18GeV, m >= 15GeV
        else if ((is2016 || is2017 || is2018) && lepB->pt() >= 18 * GeV && lepA->pt() >= 15 * GeV
            && Props::passHLT_e17_lhloose_nod0_mu14.get(event_info) && Props::matchHLT_e17_lhloose_nod0_mu14.get(lepB)
            && Props::matchHLT_e17_lhloose_nod0_mu14.get(lepA)) {
            return 55 + year_to_trigger_id(is2015, is2016, is2017, is2018);
        }
    }

    // if event doesn't match with any trigger return 0
    return 0;
}
