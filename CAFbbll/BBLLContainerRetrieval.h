#ifndef __BBLLCONTAINERRETRIEVAL__
#define __BBLLCONTAINERRETRIEVAL__

#include <string>

#include "AsgTools/SgTEvent.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODTruth/TruthParticleContainer.h"

class BBLLContainerRetrieval {
private:
    // xAOD containers
    const std::string m_electron_container_name = "Electrons";
    const std::string m_muon_container_name = "Muons";
    std::string m_jet_container_name = "AntiKt4EMPFlowJets_BTagging201903";
    std::string m_met_container_name = "MET_Reference_AntiKt4EMPFlow";
    const std::string m_eventinfo_container_name = "EventInfo";
    // Right now no systematic variations are supported for the MMC
    const std::string m_mmc_container_name = "MMC___Nominal";
    const std::string m_truth_particle_container_name = "TruthParticles___Nominal";
    const std::string m_truth_met_container_name = "MET_Truth";

    // bbll containers
    const std::string m_bbll_electron_container_name = "SelectedElectrons___";
    const std::string m_bbll_muon_container_name = "SelectedMuons___";
    const std::string m_bbll_lepton_container_name = "SelectedLeptons___";
    const std::string m_bbll_alljet_container_name = "SelectedJetsAll___";
    const std::string m_bbll_otherjet_container_name = "SelectedJetsOther___";
    const std::string m_bbll_centraljet_container_name = "SelectedJetsCentral___";
    const std::string m_bbll_forwardjet_container_name = "SelectedJetsForward___";
    const std::string m_bbll_bjet_container_name = "SelectedBJets___";

protected:
    // xAOD containers
    const std::string& jet_container_name() const { return m_jet_container_name; }
    const std::string& met_container_name() const { return m_met_container_name; }
    void set_jet_container_name(const std::string& name) { m_jet_container_name = name; }
    void set_met_container_name(const std::string& name) { m_met_container_name = name; }

    // bbll containers
    const std::string& bbll_electron_container_name() const { return m_bbll_electron_container_name; }
    const std::string& bbll_muon_container_name() const { return m_bbll_muon_container_name; }
    const std::string& bbll_lepton_container_name() const { return m_bbll_lepton_container_name; }
    const std::string& bbll_alljet_container_name() const { return m_bbll_alljet_container_name; }
    const std::string& bbll_otherjet_container_name() const { return m_bbll_otherjet_container_name; }
    const std::string& bbll_centraljet_container_name() const { return m_bbll_centraljet_container_name; }
    const std::string& bbll_forwardjet_container_name() const { return m_bbll_forwardjet_container_name; }
    const std::string& bbll_bjet_container_name() const { return m_bbll_bjet_container_name; }

    // getters for algorithm code
    bool get_xaod_electron_container(
        xAOD::TEvent&, const std::string& variation, const xAOD::ElectronContainer*&) const;
    bool get_xaod_muon_container(xAOD::TEvent&, const std::string& variation, const xAOD::MuonContainer*&) const;
    bool get_xaod_jet_container(xAOD::TEvent&, const std::string& variation, const xAOD::JetContainer*&) const;
    bool get_xaod_met_container(xAOD::TEvent&, const std::string& variation, const xAOD::MissingETContainer*&) const;
    bool get_xaod_eventinfo(xAOD::TEvent&, const std::string& variation, const xAOD::EventInfo*&) const;
    bool get_xaod_mmc_container(xAOD::TEvent&, const std::string& variation, const xAOD::EventInfo*&) const;
    bool get_xaod_truth_particle_container(xAOD::TEvent&, const xAOD::TruthParticleContainer*&) const;
    bool get_xaod_truth_met_container(
        xAOD::TEvent&, const std::string& variation, const xAOD::MissingETContainer*&) const;

    // getters for observable code
    bool get_xaod_met_container(asg::SgTEvent*, const std::string& variation, const xAOD::MissingETContainer*&) const;
    bool get_xaod_eventinfo(asg::SgTEvent*, const std::string& variation, const xAOD::EventInfo*&) const;
    bool get_xaod_mmc_container(asg::SgTEvent*, const std::string& variation, const xAOD::EventInfo*&) const;
    bool get_xaod_truth_particle_container(asg::SgTEvent*, const xAOD::TruthParticleContainer*&) const;
    bool get_xaod_truth_met_container(
        asg::SgTEvent*, const std::string& variation, const xAOD::MissingETContainer*&) const;
    bool get_bbll_electron_container(
        asg::SgTEvent*, const std::string& variation, const xAOD::ElectronContainer*&) const;
    bool get_bbll_muon_container(asg::SgTEvent*, const std::string& variation, const xAOD::MuonContainer*&) const;
    bool get_bbll_lepton_container(
        asg::SgTEvent*, const std::string& variation, const xAOD::IParticleContainer*&) const;
    bool get_bbll_alljet_container(asg::SgTEvent*, const std::string& variation, const xAOD::JetContainer*&) const;
    bool get_bbll_otherjet_container(asg::SgTEvent*, const std::string& variation, const xAOD::JetContainer*&) const;
    bool get_bbll_forwardjet_container(asg::SgTEvent*, const std::string& variation, const xAOD::JetContainer*&) const;
    bool get_bbll_centraljet_container(asg::SgTEvent*, const std::string& variation, const xAOD::JetContainer*&) const;
    bool get_bbll_bjet_container(asg::SgTEvent*, const std::string& variation, const xAOD::JetContainer*&) const;

public:
    BBLLContainerRetrieval() {};
    virtual ~BBLLContainerRetrieval() {};

    static bool is_electron_systematic(const std::string& variation);
    static bool is_muon_systematic(const std::string& variation);
    static bool is_jet_systematic(const std::string& variation);
    static bool is_met_systematic(const std::string& variation);
    static bool is_truth_met_systematic(const std::string& variation);
    static bool is_eventinfo_systematic(const std::string& variation);
};

#endif // not __BBLLCONTAINERRETRIEVAL__
