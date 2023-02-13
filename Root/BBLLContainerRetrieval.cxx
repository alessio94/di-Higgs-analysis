#include "CAFbbll/BBLLContainerRetrieval.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

bool string_starts_with(const std::string& string, const std::string& substring) { return string.find(substring) == 0; }

bool BBLLContainerRetrieval::is_electron_systematic(const std::string& variation) {
    // EG: kinematic variation
    // EL: weight variation
    return variation == "Nominal" || string_starts_with(variation, "EG") || string_starts_with(variation, "EL");
}

bool BBLLContainerRetrieval::is_muon_systematic(const std::string& variation) {
    return variation == "Nominal" || string_starts_with(variation, "MUON");
}

bool BBLLContainerRetrieval::is_jet_systematic(const std::string& variation) {
    return variation == "Nominal" || string_starts_with(variation, "JET");
}

bool BBLLContainerRetrieval::is_met_systematic(const std::string& variation) {
    // only kinematic variations
    return variation == "Nominal" || string_starts_with(variation, "MET") || string_starts_with(variation, "EG")
        || (string_starts_with(variation, "MUON") && !string_starts_with(variation, "MUON_EFF"))
        || (string_starts_with(variation, "JET") && !string_starts_with(variation, "JET_Jvt"));
}

bool BBLLContainerRetrieval::is_truth_met_systematic(const std::string& variation) {
    return variation == "Nominal" || string_starts_with(variation, "MET");
}

bool BBLLContainerRetrieval::is_eventinfo_systematic(const std::string& variation) {
    // only weight variations
    return variation == "Nominal" || string_starts_with(variation, "EL") || string_starts_with(variation, "MUON_EFF")
        || string_starts_with(variation, "JET_Jvt") || string_starts_with(variation, "PRW");
}

bool BBLLContainerRetrieval::get_xaod_electron_container(
    xAOD::TEvent& event, const std::string& variation, const xAOD::ElectronContainer*& container) const {
    std::string container_name;
    if (is_electron_systematic(variation)) {
        container_name = m_electron_container_name + "___" + variation;
    } else {
        container_name = m_electron_container_name + "___Nominal";
    }
    if (!event.retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve electron container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_xaod_muon_container(
    xAOD::TEvent& event, const std::string& variation, const xAOD::MuonContainer*& container) const {
    std::string container_name;
    if (is_muon_systematic(variation)) {
        container_name = m_muon_container_name + "___" + variation;
    } else {
        container_name = m_muon_container_name + "___Nominal";
    }
    if (!event.retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve muon container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_xaod_jet_container(
    xAOD::TEvent& event, const std::string& variation, const xAOD::JetContainer*& container) const {
    std::string container_name;
    if (is_jet_systematic(variation)) {
        container_name = m_jet_container_name + "___" + variation;
    } else {
        container_name = m_jet_container_name + "___Nominal";
    }
    if (!event.retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve jet container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_xaod_met_container(
    xAOD::TEvent& event, const std::string& variation, const xAOD::MissingETContainer*& container) const {
    std::string container_name;
    if (is_met_systematic(variation)) {
        container_name = m_met_container_name + "___" + variation;
    } else {
        container_name = m_met_container_name + "___Nominal";
    }
    if (!event.retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve met container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_xaod_eventinfo(
    xAOD::TEvent& event, const std::string& variation, const xAOD::EventInfo*& event_info) const {
    std::string container_name;
    if (is_eventinfo_systematic(variation)) {
        container_name = m_eventinfo_container_name + "___" + variation;
    } else {
        container_name = m_eventinfo_container_name + "___Nominal";
    }
    if (!event.retrieve(event_info, container_name).isSuccess()) {
        ERROR("Failed to retrieve event info '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_xaod_mmc_container(
    xAOD::TEvent& event, const std::string& /*variation*/, const xAOD::EventInfo*& mmc_container) const {
    if (!event.retrieve(mmc_container, m_mmc_container_name).isSuccess()) {
        ERROR("Failed to retrieve event info '%s'!", m_mmc_container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_xaod_truth_particle_container(
    xAOD::TEvent& event, const xAOD::TruthParticleContainer*& truth_particle_container) const {
    if (!event.retrieve(truth_particle_container, m_truth_particle_container_name).isSuccess()) {
        ERROR("Failed to retrieve event info '%s'!", m_truth_particle_container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_xaod_truth_met_container(
    xAOD::TEvent& event, const std::string& variation, const xAOD::MissingETContainer*& container) const {
    std::string container_name;
    if (is_truth_met_systematic(variation)) {
        container_name = m_truth_met_container_name + "___" + variation;
    } else {
        container_name = m_truth_met_container_name + "___Nominal";
    }
    if (!event.retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve truth met container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_xaod_met_container(
    asg::SgTEvent* event, const std::string& variation, const xAOD::MissingETContainer*& event_info) const {
    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }
    return get_xaod_met_container(*event->event(), variation, event_info);
}

bool BBLLContainerRetrieval::get_xaod_eventinfo(
    asg::SgTEvent* event, const std::string& variation, const xAOD::EventInfo*& event_info) const {
    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }
    return get_xaod_eventinfo(*event->event(), variation, event_info);
}

bool BBLLContainerRetrieval::get_xaod_mmc_container(
    asg::SgTEvent* event, const std::string& variation, const xAOD::EventInfo*& mmc_container) const {
    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }
    return get_xaod_mmc_container(*event->event(), variation, mmc_container);
}

bool BBLLContainerRetrieval::get_xaod_truth_particle_container(
    asg::SgTEvent* event, const xAOD::TruthParticleContainer*& truth_particle_container) const {
    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }
    return get_xaod_truth_particle_container(*event->event(), truth_particle_container);
}

bool BBLLContainerRetrieval::get_xaod_truth_met_container(
    asg::SgTEvent* event, const std::string& variation, const xAOD::MissingETContainer*& event_info) const {
    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }
    return get_xaod_met_container(*event->event(), variation, event_info);
}

bool BBLLContainerRetrieval::get_bbll_electron_container(
    asg::SgTEvent* event, const std::string& variation, const xAOD::ElectronContainer*& container) const {
    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }
    std::string container_name = bbll_electron_container_name() + variation;
    if (!event->retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve electron container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_bbll_muon_container(
    asg::SgTEvent* event, const std::string& variation, const xAOD::MuonContainer*& container) const {
    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }
    std::string container_name = bbll_muon_container_name() + variation;
    if (!event->retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve muon container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_bbll_lepton_container(
    asg::SgTEvent* event, const std::string& variation, const xAOD::IParticleContainer*& container) const {
    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }
    std::string container_name = bbll_lepton_container_name() + variation;
    if (!event->retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve lepton container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_bbll_alljet_container(
    asg::SgTEvent* event, const std::string& variation, const xAOD::JetContainer*& container) const {
    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }
    std::string container_name = bbll_alljet_container_name() + variation;
    if (!event->retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve alljet container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_bbll_otherjet_container(
    asg::SgTEvent* event, const std::string& variation, const xAOD::JetContainer*& container) const {

    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }
    std::string container_name = bbll_otherjet_container_name() + variation;
    if (!event->retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve otherjet container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_bbll_forwardjet_container(
    asg::SgTEvent* event, const std::string& variation, const xAOD::JetContainer*& container) const {

    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }
    std::string container_name = bbll_forwardjet_container_name() + variation;
    if (!event->retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve forwardjet container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_bbll_centraljet_container(
    asg::SgTEvent* event, const std::string& variation, const xAOD::JetContainer*& container) const {

    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }

    std::string container_name;
    if (variation.rfind("FT_EFF", 0) == 0) {
        // b-tagging efficiency systematics are done locally in CAFbbll
        // we only need to set the variation in the b-tagging scale factor tool in the BTagSF observable
        container_name = bbll_centraljet_container_name() + "Nominal";
    } else {
        container_name = bbll_centraljet_container_name() + variation;
    }
    if (!event->retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve centraljet container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}

bool BBLLContainerRetrieval::get_bbll_bjet_container(
    asg::SgTEvent* event, const std::string& variation, const xAOD::JetContainer*& container) const {

    if (!event) {
        ERROR("SgTEvent is a null pointer!");
        return false;
    }
    std::string container_name = bbll_bjet_container_name() + variation;
    if (!event->retrieve(container, container_name).isSuccess()) {
        ERROR("Failed to retrieve bjet container '%s'!", container_name.c_str());
        return false;
    }
    return true;
}
