#include "CAFbbll/TruthPthh.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

TruthPthh::TruthPthh() { DEBUGclass("Default constructor called"); }

TruthPthh::~TruthPthh() { DEBUGclass("Destructor called"); }

TruthPthh::TruthPthh(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());
}

double TruthPthh::getValue() const {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    bool isSignal = false;
    fSample->getTag("~isSig", isSignal);
    if (!isSignal) {
        return -1;
    }

    const xAOD::TruthParticleContainer* truth_particles = nullptr;
    if (!get_xaod_truth_particle_container(fEvent, truth_particles)) {
        return false;
    }

    std::vector<const xAOD::TruthParticle*> truth_higgs_bosons;
    for (const xAOD::TruthParticle* truth_particle : *truth_particles) {
        int pdgId = fabs(truth_particle->pdgId());
        if (pdgId == 25) {
            truth_higgs_bosons.emplace_back(truth_particle);
        }
    }

    double truth_pthh = 0;
    if (truth_higgs_bosons.size() == 2) {
        const xAOD::TruthParticle* h0 = truth_higgs_bosons.at(0);
        const xAOD::TruthParticle* h1 = truth_higgs_bosons.at(1);
        truth_pthh = (h0->p4() + h1->p4()).Pt();
    } else {
        DEBUGclass("Event has %d and not 2 truth higgs bosons", truth_higgs_bosons.size());
    }

#ifdef _DEBUG_
    std::cout << "truth pt_hh = " << truth_pthh << std::endl;
#endif

    return truth_pthh;
}
