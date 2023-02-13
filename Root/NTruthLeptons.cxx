#include "CAFbbll/NTruthLeptons.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#ifdef _DEBUG_
#include <iomanip> // std::setw
#endif

NTruthLeptons::NTruthLeptons() { DEBUGclass("Default constructor called"); }

NTruthLeptons::~NTruthLeptons() { DEBUGclass("Destructor called"); }

NTruthLeptons::NTruthLeptons(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());
}

double NTruthLeptons::getValue() const {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    bool isMC = false;
    fSample->getTag("~isMC", isMC);
    if (!isMC) {
        return -1;
    }

    const xAOD::TruthParticleContainer* truth_particles = nullptr;
    if (!get_xaod_truth_particle_container(fEvent, truth_particles)) {
        return false;
    }

#ifdef _DEBUG_
    std::cout << "Contents of TruthParticles container" << std::endl;
    for (const xAOD::TruthParticle* truth_particle : *truth_particles) {
        std::cout << "pt=" << std::setw(8) << truth_particle->pt() << "  eta=" << std::setw(10) << truth_particle->eta()
                  << "  phi=" << std::setw(10) << truth_particle->phi() << "  type=" << fabs(truth_particle->pdgId())
                  << "  status=" << truth_particle->status()
                  << "  isWdecayFromTTbar=" << bool(truth_particle->auxdataConst<char>("isWdecayFromTtbar"))
                  << std::endl;
    }
    std::cout << std::endl;
#endif // _DEBUG_

    int count = 0;
    for (const xAOD::TruthParticle* truth_particle : *truth_particles) {
        int pdgId = fabs(truth_particle->pdgId());
        if ((pdgId == 11 || pdgId == 13) && truth_particle->status() == 1) {
            count++;
        }
    }

#ifdef _DEBUG_
    std::cout << "NTruthLeptons=" << count << std::endl;
#endif

    return count;
}
