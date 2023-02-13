#include "CAFbbll/NWTauDecays.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

// STL
#include <algorithm>

// xAOD
#include "xAODBase/IParticleContainer.h"
#include "xAODTruth/TruthParticleContainer.h"

NWTauDecays::NWTauDecays() { DEBUGclass("Default constructor called"); }

NWTauDecays::~NWTauDecays() { DEBUGclass("Destructor called"); }

NWTauDecays::NWTauDecays(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());
}

double NWTauDecays::getValue() const {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    const xAOD::IParticleContainer* leptons = nullptr;
    if (!get_bbll_lepton_container(fEvent, "Nominal", leptons)) {
        return false;
    }

    const xAOD::TruthParticleContainer* truth_particles = nullptr;
    if (!get_xaod_truth_particle_container(fEvent, truth_particles)) {
        return false;
    }

    int nWTauDecays = 0;
    std::vector<const xAOD::TruthParticle*> matched_truth_particles;
    for (const xAOD::IParticle* lepton : *leptons) {
        // electrons have pdg id 11, muons pdg id 13
        // charge is -1 for electrons/muons
        int reco_pdg_id = (lepton->type() == xAOD::Type::Electron ? -11 : -13) * lepton->auxdataConst<float>("charge");
        for (const xAOD::TruthParticle* truth_particle : *truth_particles) {
            if (reco_pdg_id == truth_particle->pdgId()) {              // check if particle type and charge match
                if (lepton->p4().DeltaR(truth_particle->p4()) < 0.4) { // check for deltaR < 0.4 overlap
                    // check if particle is not already used for another truth matching
                    if (std::find(matched_truth_particles.begin(), matched_truth_particles.end(), truth_particle)
                        == matched_truth_particles.end()) {
                        matched_truth_particles.emplace_back(truth_particle);
                        if (bool(truth_particle->auxdataConst<char>("isLeptonFromTauDecay"))) {
                            nWTauDecays += 1;
                        }
                    } else {
                        INFOclass("Truth particle was already used for truth matching!");
                    }
                }
            }
        }
    }
    return nWTauDecays;
}
