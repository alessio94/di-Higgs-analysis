#include "CAFbbll/BBLLObjectSelectionAlgorithm.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_

// CAFCore
#include "QFramework/TQLibrary.h"

// ASG
#include "AsgMessaging/StatusCode.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODRootAccess/TActiveStore.h"

// CxAODTools
#include "CxAODTools/CommonProperties.h"

BBLLObjectSelectionAlgorithm::BBLLObjectSelectionAlgorithm() {
    DEBUGclass("Constructor called");

    // fail job if a status code is not checked
    StatusCode::enableFailure();
}

BBLLObjectSelectionAlgorithm::~BBLLObjectSelectionAlgorithm() { }

void BBLLObjectSelectionAlgorithm::set_electron_id(const std::string& id) {
    m_electron_id = "is" + id + "LH";
    INFOclass("Set electron ID to '%s'", m_electron_id.c_str());
}

void BBLLObjectSelectionAlgorithm::set_electron_iso(const std::string& iso) {
    m_electron_iso = "is" + iso + "Iso";
    INFOclass("Set electron ISO to '%s'", m_electron_iso.c_str());
}

void BBLLObjectSelectionAlgorithm::set_muon_iso(const std::string& iso) {
    m_muon_iso = "is" + iso + "Iso";
    INFOclass("Set muon ISO to '%s'", m_muon_iso.c_str());
}

void BBLLObjectSelectionAlgorithm::set_jet_collection(const std::string& name, const std::string& btaggin_version) {
    std::string jet_container_name = "AntiKt4" + name + "Jets_BTagging" + btaggin_version;
    std::string met_container_name = "MET_Reference_AntiKt4" + name;

    set_jet_container_name(jet_container_name);
    set_met_container_name(met_container_name);
    INFOclass("Set jet container name to '%s'", jet_container_name.c_str());
    INFOclass("Set met container name to '%s'", met_container_name.c_str());
}

void BBLLObjectSelectionAlgorithm::set_btagging_cdi_file(const std::string& cdi_file) {
    m_btagging_cdi_file = cdi_file;
    INFOclass("Set b-tagging CDI file to '%s'", cdi_file.c_str());
}

void BBLLObjectSelectionAlgorithm::add_variation(const std::string& variation) { m_variations.insert(variation); }

bool BBLLObjectSelectionAlgorithm::initializeSingleton(const std::vector<TQSample*>& samples) {
    // initialize the algorithm
    DEBUGclass("initalize called");

    if (samples.size() < 1) {
        ERRORclass("Cannot initialize BBLLObjectSelectionAlgorithm with zero samples!");
        return false;
    }

    // we get one sample per channel, but since all samples are the same we can just use the first one, because these
    // are the same events
    TQSample* s = samples[0];
    if (!s) {
        ERRORclass("Recieved null sample!");
        return false;
    }

    // however, we need to extract all variations from the samples
    for (TQSample* sample : samples) {
        TString p4_variation;
        sample->getTag("~p4Variation", p4_variation);
        std::string p4_variation_string(p4_variation.Data());
        add_variation(p4_variation_string);
    }

    if (m_sample == s) { // already initialized for this sample
        return true;
    }
    m_sample = s;

    m_event_token = s->getEventToken();
    if (!m_event_token) {
        INFOclass("Could not get event token, maybe the file is empty!");
        return true; // return true here so that we just skip empty files
    }

    m_event = (xAOD::TEvent*)(m_event_token->getContent());
    if (!m_event) {
        ERRORclass("Could not retrieve TEvent!");
        m_sample->returnToken(m_event_token);
        return false;
    }

    m_store = xAOD::TActiveStore::store();
    if (!m_store) {
        ERRORclass("Unable to access active store, please create a TStore object and set it active using "
                   "'TStore::setActive()'!");
        return false;
    }

    m_overlapRegAcc = new OverlapRegisterAccessor(OverlapRegisterAccessor::READ);
    if (!m_overlapRegAcc) {
        ERRORclass("Could not initalize OverlapRegisterAccessor!");
        return false;
    }

    if (m_btagging_cdi_file.empty()) {
        ERRORclass("b-tagging CDI file not set!");
        return false;
    }
    m_btagtool_default =
        asg::AnaToolHandle<IBTaggingSelectionTool>("BTaggingSelectionTool/BTaggingSelectionTool_default");
    m_btagtool_default.setProperty("OutputLevel", MSG::WARNING).ignore(); // disable info messages
    m_btagtool_default.setProperty("FlvTagCutDefinitionsFileName", m_btagging_cdi_file).ignore();
    m_btagtool_default.setProperty("JetAuthor", jet_container_name()).ignore();
    m_btagtool_default.setProperty("TaggerName", "DL1r").ignore();
    m_btagtool_default.setProperty("OperatingPoint", "FixedCutBEff_77").ignore();
    m_btagtool_default.setProperty("MinPt", 20000).ignore();
    if (!m_btagtool_default.retrieve().isSuccess()) {
        ERRORclass("Could not initalize BTaggingSelectionTool (default)!");
        return false;
    }

    DEBUGclass("Successfully initialized");
    return true;
}

bool BBLLObjectSelectionAlgorithm::finalize() {
    // finalize the algorithm
    DEBUGclass("finalize called");

    m_sample->returnToken(m_event_token);
    m_event_token = nullptr;
    m_event = nullptr;
    m_sample = nullptr;
    m_store = nullptr;
    m_overlapRegAcc = nullptr;

    return true;
}

bool BBLLObjectSelectionAlgorithm::execute() {
    // execute the algorithm
    DEBUGclass("execute called");

    // load the overlap register
    if (!m_overlapRegAcc->loadRegister(m_event).isSuccess()) {
        ERRORclass("Could not load OverlapRegister!");
        return false;
    }

    bool isMC = false;
    m_sample->getTag("~isMC", isMC);

    for (std::string variation : m_variations) {
        DEBUGclass("Running on systematic variation '%s'", variation.c_str());

        if (!isMC && variation != "Nominal") {
            continue;
        }

        const xAOD::ElectronContainer* electrons = nullptr;
        if (!get_xaod_electron_container(*m_event, variation, electrons)) {
            return false;
        }

        const xAOD::MuonContainer* muons = nullptr;
        if (!get_xaod_muon_container(*m_event, variation, muons)) {
            return false;
        }

        const xAOD::JetContainer* jets = nullptr;
        if (!get_xaod_jet_container(*m_event, variation, jets)) {
            return false;
        }

        OverlapRegisterAccessor::Containers containers;
        containers.variation = variation;
        containers.electrons = electrons;
        containers.muons = muons;
        containers.jets = jets;
        if (!m_overlapRegAcc->decorateObjects(containers)) {
            ERRORclass("Could not decorate objects with overlap register decorators!");
            return false;
        }

        if (!select_leptons(*electrons, *muons, variation)) {
            ERRORclass("Could not select leptons!");
            return false;
        }

        if (!select_jets(*jets, variation)) {
            ERRORclass("Could not select jets!");
            return false;
        }

#ifdef _DEBUG_
        const xAOD::EventInfo* event_info = nullptr;
        if (!get_xaod_eventinfo(*m_event, variation, event_info)) {
            return false;
        }
        bool isMC = false;
        m_sample->getTag("~isMC", isMC);

        if (isMC) {
            std::cout << "runNumber=" << event_info->auxdataConst<unsigned int>("RandomRunNumber")
                      << ", eventNumber=" << event_info->auxdataConst<unsigned long long int>("eventNumber")
                      << std::endl;
        } else {
            std::cout << "runNumber=" << event_info->auxdataConst<unsigned int>("runNumber")
                      << ", eventNumber=" << event_info->auxdataConst<unsigned long long int>("eventNumber")
                      << std::endl;
        }
        std::cout << std::endl;
#endif
    }

    return true;
}

bool BBLLObjectSelectionAlgorithm::select_leptons(
    const xAOD::ElectronContainer& electrons, const xAOD::MuonContainer& muons, const std::string& variation) {

    std::vector<const xAOD::Electron*> selected_electrons;
    std::vector<const xAOD::Muon*> selected_muons;
    std::vector<const xAOD::IParticle*> selected_leptons;

    if (m_electron_id.empty()) {
        BREAKclass("Electron ID not set!");
        return false;
    }

    if (m_electron_iso.empty()) {
        BREAKclass("Electron ISO not set!");
        return false;
    }

    if (m_muon_iso.empty()) {
        BREAKclass("Muon ISO not set!");
        return false;
    }

    for (const xAOD::Electron* electron : electrons) {
        if (!Props::passOR.get(electron)) {
            continue;
        }
        bool pass_id = electron->auxdataConst<int>(m_electron_id);
        bool pass_iso = electron->auxdataConst<int>(m_electron_iso);
        if (pass_id && pass_iso) {
            electron->auxdecor<int>("CAF_leptype") = 0;
            selected_electrons.push_back(electron);
            selected_leptons.push_back(electron);
        }
    }

    for (const xAOD::Muon* muon : muons) {
        if (!Props::passOR.get(muon)) {
            continue;
        }
        if (muon->auxdataConst<int>(m_muon_iso)) {
            muon->auxdecor<int>("CAF_leptype") = 1;
            selected_muons.push_back(muon);
            selected_leptons.push_back(muon);
        }
    }

#ifdef _DEBUG_
    print_particle_vector_info(selected_electrons, "electrons");
    print_particle_vector_info(selected_muons, "muons");
    print_particle_vector_info(selected_leptons, "leptons");
#endif

    save_particles_to_store<xAOD::ElectronContainer>(selected_electrons, bbll_electron_container_name() + variation);
    save_particles_to_store<xAOD::MuonContainer>(selected_muons, bbll_muon_container_name() + variation);
    save_particles_to_store<xAOD::IParticleContainer>(selected_leptons, bbll_lepton_container_name() + variation);

    return true;
}

bool BBLLObjectSelectionAlgorithm::select_jets(const xAOD::JetContainer& jets, const std::string& variation) {
    std::vector<const xAOD::Jet*> selected_jets_all;
    std::vector<const xAOD::Jet*> selected_jets_other;
    std::vector<const xAOD::Jet*> selected_jets_forward;
    std::vector<const xAOD::Jet*> selected_jets_central;
    std::vector<const xAOD::Jet*> selected_bjets;

    for (const xAOD::Jet* jet : jets) {
        if (!Props::passOR.get(jet)) {
            continue;
        }

        selected_jets_all.push_back(jet);

        if (fabs(jet->eta()) < 2.5) {
            selected_jets_central.push_back(jet);
        } else {
            selected_jets_forward.push_back(jet);
        }

        double jet_pt = jet->pt();
        double jet_eta = jet->eta();
        double jet_dl1r_pb = jet->auxdataConst<double>("DL1r_pb");
        double jet_dl1r_pc = jet->auxdataConst<double>("DL1r_pc");
        double jet_dl1r_pu = jet->auxdataConst<double>("DL1r_pu");

        double tagweight = 0;
        m_btagtool_default->getTaggerWeight(jet_dl1r_pb, jet_dl1r_pc, jet_dl1r_pu, tagweight).ignore();
        bool isBJet_DL1r_default = m_btagtool_default->accept(jet_pt, jet_eta, tagweight);
        jet->auxdecor<int>("CAFisBJet_DL1r_FixedCutBEff_77") = isBJet_DL1r_default;
        jet->auxdecor<float>("CAFisBJet_DL1r_FixedCutBEff_77_tagweight") = tagweight;

        if (isBJet_DL1r_default) {
            selected_bjets.push_back(jet);
        } else {
            selected_jets_other.push_back(jet);
        }
    }

#ifdef _DEBUG_
    print_particle_vector_info(selected_jets_all, "all jets");
    print_particle_vector_info(selected_jets_other, "other jets");
    print_particle_vector_info(selected_jets_forward, "forwad jets");
    print_particle_vector_info(selected_jets_central, "central jets");
    print_particle_vector_info(selected_bjets, "bjets");
#endif

    save_particles_to_store<xAOD::JetContainer>(selected_jets_all, bbll_alljet_container_name() + variation);
    save_particles_to_store<xAOD::JetContainer>(selected_jets_other, bbll_otherjet_container_name() + variation);
    save_particles_to_store<xAOD::JetContainer>(selected_jets_central, bbll_centraljet_container_name() + variation);
    save_particles_to_store<xAOD::JetContainer>(selected_jets_forward, bbll_forwardjet_container_name() + variation);
    save_particles_to_store<xAOD::JetContainer>(selected_bjets, bbll_bjet_container_name() + variation);

    return true;
}

bool BBLLObjectSelectionAlgorithm::cleanup() {
    // cleanup the cache of the algorithm after an event
    DEBUGclass("cleanup called");

    m_store->clear();
    return true;
}

template <typename Particle> bool BBLLObjectSelectionAlgorithm::compare_pt(Particle a, Particle b) {
    return (a->pt() > b->pt());
}

template <typename Container>
void BBLLObjectSelectionAlgorithm::save_particles_to_store(
    std::vector<const typename Container::base_value_type*>& particles, std::string container_name) {
    // create container to store selected truth particles
    // for this a ConstDataVector should be used, which is basically only a list of pointers, which makes this very
    // efficient for more details see:
    // https://atlassoftwaredocs.web.cern.ch/ABtutorial/basic_xaod_modify/#constdatavector-advancedoptional
    auto particleContainer = std::make_unique<ConstDataVector<Container>>(SG::VIEW_ELEMENTS);

    // sort particle vector by pt
    std::sort(particles.begin(), particles.end(), compare_pt<const typename Container::base_value_type*>);

    // put particles into container
    for (const typename Container::base_value_type* particle : particles) {
        particleContainer->push_back(particle);
    }

    // save selected particle container to TStore
    if (!m_store->record(particleContainer.release(), container_name.c_str()).isSuccess()) {
        throw std::runtime_error("Error recording selected particles");
    }
}

template <typename Particle> void BBLLObjectSelectionAlgorithm::print_particle_info(Particle particle) {
    std::cout << "pt=" << particle->pt() << std::endl;
}

template <typename Particle>
void BBLLObjectSelectionAlgorithm::print_particle_vector_info(std::vector<Particle> particles, std::string name) {
    std::cout << "================================" << std::endl;
    std::cout << "Information for " << name << std::endl;
    std::cout << "Number of particles: " << particles.size() << std::endl;
    for (Particle particle : particles) {
        print_particle_info(particle);
    }
}
