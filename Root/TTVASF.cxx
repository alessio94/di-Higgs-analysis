#include "CAFbbll/TTVASF.h"
#include <limits>

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include "xAODBase/IParticleContainer.h"

TTVASF::TTVASF() { DEBUGclass("Default constructor called"); }

TTVASF::~TTVASF() { DEBUGclass("Destructor called"); }

TTVASF::TTVASF(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());
}

double TTVASF::getValue() const {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    bool isMC = false;
    fSample->getTag("~isMC", isMC);

    if (isMC) {
        DEBUGclass("This is a MC event");

        std::string variation = fSample->getTagStandardStringDefault("~p4Variation", "Nominal");

        const xAOD::IParticleContainer* leptons = nullptr;
        if (!get_bbll_lepton_container(fEvent, variation, leptons)) {
            return false;
        }

        float weight = 1;
        for (const xAOD::IParticle* lepton : *leptons) {
            if (lepton->type() == xAOD::Type::Muon) {
                weight *= lepton->auxdataConst<float>("TTVAEffSF");
            }
        }

        DEBUGclass("[%s] weight=%f", variation.c_str(), weight);

        return weight;
    } else {
        DEBUGclass("This is a data event");
        return 1.0;
    }
}
