#include "CAFbbll/TruthHiggsDecayInfo.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include "xAODEventInfo/EventInfo.h"

TruthHiggsDecayInfo::TruthHiggsDecayInfo() { DEBUGclass("Default constructor called"); }

TruthHiggsDecayInfo::~TruthHiggsDecayInfo() { DEBUGclass("Destructor called"); }

TruthHiggsDecayInfo::TruthHiggsDecayInfo(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());

    TString input = name;

    TString obs_name;
    if (!TQStringUtils::readToken(input, obs_name, TQStringUtils::alphanumvarext)) {
        ERRORclass("The name of the TruthHiggsDecayInfo has to have the following scheme: "
                   "'TruthHiggsDecayInfo:HiggsIndex:ChildIndex'");
        BREAK("Error in creating TruthHiggsDecayInfo observable");
    }
    DEBUGclass("Observable name is %s", obs_name.Data());

    if (input.IsNull()) { // there is a ":variation" after the observable name
        ERRORclass("The name of the TruthHiggsDecayInfo has to have the following scheme: "
                   "'TruthHiggsDecayInfo:HiggsIndex:ChildIndex'");
        BREAK("Error in creating TruthHiggsDecayInfo observable");
    }

    // read higgs index
    if (TQStringUtils::removeLeading(input, ":") != 1) {
        if (input.IsNull()) {
            ERRORclass("Missing mc name declaration in expression '%s'", name.Data());
            BREAK("Error in creating TruthHiggsDecayInfo observable");
        }
    }
    TString higgs_index;
    if (!TQStringUtils::readToken(input, higgs_index, TQStringUtils::numerals)) {
        ERRORclass("The name of the TruthHiggsDecayInfo has to have the following scheme: "
                   "'TruthHiggsDecayInfo:HiggsIndex:ChildIndex'");
        BREAK("Error in creating TruthHiggsDecayInfo observable");
    }
    m_higgs_index = higgs_index.Atoi();
    DEBUGclass("Using higgs index %d", m_higgs_index);

    // read child index
    if (TQStringUtils::removeLeading(input, ":") != 1) {
        ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
        BREAK("Error in creating TruthHiggsDecayInfo observable");
    }
    if (input.IsNull()) {
        ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
        BREAK("Error in creating TruthHiggsDecayInfo observable");
    }
    if (!input.IsDec()) {
        ERRORclass("The name of the TruthHiggsDecayInfo has to have the following scheme: "
                   "'TruthHiggsDecayInfo:HiggsIndex:ChildIndex'");
        BREAK("Error in creating TruthHiggsDecayInfo observable");
    }
    m_child_index = input.Atoi();
    DEBUGclass("Using child index %d", m_child_index);
}

bool TruthHiggsDecayInfo::initializeSelf() {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    if (m_higgs_index >= 0 && m_child_index >= 0) {
        TString deco_name = TString::Format("TruthHiggs%dChild%dPdgId", m_higgs_index, m_child_index);
        m_decorator = new SG::AuxElement::Accessor<int>(deco_name.Data());
    }

    return true;
}

bool TruthHiggsDecayInfo::finalizeSelf() {
    if (m_decorator) {
        delete m_decorator;
        m_decorator = nullptr;
    }

    return true;
}

double TruthHiggsDecayInfo::getValue() const {
    if (m_higgs_index < 0 && m_child_index < 0) {
        return -1;
    }

    bool isMC = false;
    fSample->getTag("~isMC", isMC);
    if (!isMC) {
        DEBUGclass("This is a data event");
        return -1;
    }

    // get event info container
    const xAOD::EventInfo* event_info = nullptr;
    if (!get_xaod_eventinfo(fEvent, "Nominal", event_info)) {
        return false;
    }

    if (!m_decorator->isAvailable(*event_info)) {
        return -1;
    }

    int higgs_decay_info = abs((*m_decorator)(*event_info));

    DEBUGclass("higgs %d child %d: %d", m_higgs_index, m_child_index, higgs_decay_info);
    return higgs_decay_info;
}
