#include "CAFbbll/TruthMhh.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

TruthMhh::TruthMhh() { DEBUGclass("Default constructor called"); }

TruthMhh::~TruthMhh() { DEBUGclass("Destructor called"); }

TruthMhh::TruthMhh(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());
}

double TruthMhh::getValue() const {
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

    double truth_mhh = 0;
    if (truth_higgs_bosons.size() == 2) {
        const xAOD::TruthParticle* h0 = truth_higgs_bosons.at(0);
        const xAOD::TruthParticle* h1 = truth_higgs_bosons.at(1);
        truth_mhh = (h0->p4() + h1->p4()).M();
    } else {
        DEBUGclass("Event has %d and not 2 truth higgs bosons", truth_higgs_bosons.size());
    }

#ifdef _DEBUG_
    std::cout << "truth m_hh = " << truth_mhh << std::endl;
#endif

    return truth_mhh;
}
