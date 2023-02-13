#include "CAFbbll/LeptonFakeType.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include <algorithm>
#include <string>

LeptonFakeType::LeptonFakeType() { DEBUGclass("Default constructor called"); }

LeptonFakeType::~LeptonFakeType() { DEBUGclass("Destructor called"); }

LeptonFakeType::LeptonFakeType(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());

    TString input = name;

    TString obs_name;
    if (!TQStringUtils::readToken(input, obs_name, TQStringUtils::alphanumvarext)) {
        ERRORclass("The name of the LeptonFakeType has to have the following scheme: 'Name<:Variation>'");
        BREAK("Error in creating LeptonFakeType observable");
    }
    DEBUGclass("Observable name is %s", obs_name.Data());

    if (!input.IsNull()) { // there is a ":lepton_index" after the observable name
        if (TQStringUtils::removeLeading(input, ":") != 1) {
            ERRORclass("Missing lepton index definition in expression '%s'", name.Data());
            BREAK("Error in creating LeptonFakeType observable");
        }

        if (input.IsNull()) {
            ERRORclass("Missing lepton index definition in expression '%s'", name.Data());
            BREAK("Error in creating LeptonFakeType observable");
        }

        if (!input.IsDec()) {
            ERRORclass("Lepton index definition is not an integer in expression '%s'", name.Data());
            BREAK("Error in creating LeptonFakeType observable");
        }

        if (input.Atoi() < 0) {
            ERRORclass("Lepton index is negative!");
            BREAK("Error in creating LeptonFakeType observable");
        }
        m_lepton_index = input.Atoi();
        DEBUGclass("Using lepton index %d", m_lepton_index);
    } else {
        ERRORclass("Missing lepton index definition in expression '%s'", name.Data());
        BREAK("Error in creating LeptonFakeType observable");
    }
}

bool LeptonFakeType::initializeSelf() {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    m_variation = fSample->getTagStandardStringDefault("~p4Variation", "Nominal");

    return true;
}

double LeptonFakeType::getValue() const {
    bool isMC = false;
    fSample->getTag("~isMC", isMC);
    if (!isMC) {
        DEBUGclass("[%s] This is a data event", m_variation.c_str());
        return 1;
    }

    const xAOD::IParticleContainer* leptons = nullptr;
    if (!get_bbll_lepton_container(fEvent, m_variation, leptons)) {
        return false;
    }

    if (m_lepton_index >= leptons->size()) {
        return -1;
    }

    xAOD::IParticle const* lepton = leptons->at(m_lepton_index);

    // truth lepton classification based on Isolation and Fake Forum recommendations
    // https://indico.cern.ch/event/725960/contributions/2987219/attachments/1641430/2621432/TruthDef_April242018.pdf

    int T = lepton->auxdataConst<int>("MCTruthClassifierType");   // truthType
    int O = lepton->auxdataConst<int>("MCTruthClassifierOrigin"); // truthOrigin
    // only for electrons
    float charge = 0;           // reco charge
    int firstEgMotherPdgId = 0; // firstEgMotherPdgId
    int firstEgMotherT = -1;    // firstEgMotherTruthType
    int firstEgMotherO = -1;    // firstEgMotherTruthOrigin

    if (lepton->type() == xAOD::Type::Electron) {
        charge = lepton->auxdataConst<float>("charge");
        firstEgMotherPdgId = lepton->auxdataConst<int>("firstEgMotherPdgId");
        firstEgMotherT = lepton->auxdataConst<int>("firstEgMotherTruthType");
        firstEgMotherO = lepton->auxdataConst<int>("firstEgMotherTruthOrigin");
    }

    int lepton_type = -1;

    bool C1 = (T == 2) || (T == 4 && O == 5 && fabs(firstEgMotherPdgId) == 11)
        || (T == 4 && O == 7 && firstEgMotherT == 2
            && (firstEgMotherO == 10 || firstEgMotherO == 12 || firstEgMotherO == 13 || firstEgMotherO == 14
                || firstEgMotherO == 43)
            && fabs(firstEgMotherPdgId) == 11);

    if ((C1 && firstEgMotherPdgId * charge < 0) || (T == 4 && (O == 5 || O == 7) && firstEgMotherO == 40)
        || (T == 15 && O == 40)) { // prompt electron
        lepton_type = 0;
    } else if (C1 && firstEgMotherPdgId * charge > 0) { // charged flipped electron
        lepton_type = 1;
    } else if (T == 6 && (O == 10 || O == 12 || O == 13 || O == 14 || O == 15 || O == 22 || O == 43)) { // prompt muon
        lepton_type = 0;
    } else if ((T == 14 && O == 37)
        || (T == 4 && (O == 5 || O == 7) && firstEgMotherT == 14 && firstEgMotherO == 37)) { // prompt photon conversion
        lepton_type = 2;
    } else if ((T == 4 && O == 5 && firstEgMotherT == 16 && firstEgMotherO == 38)
        || (T == 16 && O == 38)) { // prompt photon conversion
        lepton_type = 2;
    } else if (T == 4 && (O == 6 || O == 23 || O == 24)) { // hadron decay (light flavor)
        lepton_type = 3;
    } else if (T == 4 && O == 5 && firstEgMotherT == 16
        && (firstEgMotherO == 42 || firstEgMotherO == 23 || firstEgMotherO == 24)) { // hadron decay (light flavor)
        lepton_type = 3;
    } else if (T == 4 && O == 7
        && ((firstEgMotherT == 4 && firstEgMotherO == 24)
            || (firstEgMotherT == 16 && firstEgMotherO == 42))) { // hadron decay (light flavor)
        lepton_type = 3;
    } else if (T == 16 && (O == 42 || O == 23)) { // hadron decay (light flavor)
        lepton_type = 3;
    } else if ((T == 8 && (O == 34 || 35 || 23 || 24)) || T == 17) { // hadron decay (light flavor)
        lepton_type = 3;
    } else if (((T == 3 || T == 15) && O == 8)
        || (T == 4 && O == 7 && firstEgMotherT == 3 && firstEgMotherO == 8)) { // muons reconstructed as e
        lepton_type = 4;
    } else if (T == 4 && O == 5 && firstEgMotherT == 15 && firstEgMotherO == 8) { // muons reconstructed as e
        lepton_type = 4;
    } else if (((T == 3 || T == 15) && O == 9) || (T == 4 && O == 5 && firstEgMotherT == 15 && firstEgMotherO == 9)
        || (T == 7 && O == 9)) { // heavy flavor / non-prompt tau
        lepton_type = 5;
    } else if (T == 3 && (O == 26 || O == 29 || O == 33)) { // heavy flavor / non-prompt b
        lepton_type = 6;
    } else if ((T == 16 && O == 26)
        || (T == 4 && O == 5 && firstEgMotherT == 16 && firstEgMotherO == 26)) { // heavy flavor / non-prompt b
        lepton_type = 6;
    } else if ((T == 6 || T == 7) && (O == 26 || O == 29 || O == 33)) { // heavy flavor / non-prompt b
        lepton_type = 6;
    } else if ((T == 3 && (O == 25 || O == 32 || O == 27)) || (T == 4 && O == 27)) { // heavy flavor / non-prompt c
        lepton_type = 7;
    } else if ((T == 16 && (O == 25 || O == 27))
        || (T == 4 && O == 5 && firstEgMotherT == 16
            && (firstEgMotherO == 25 || firstEgMotherO == 27))) { // heavy flavor / non-prompt c
        lepton_type = 7;
    } else if (T == 7 && (O == 25 || O == 32 || O == 27)) { // heavy flavor / non-prompt c
        lepton_type = 7;
    } else if ((T == 6 || T == 8) && (O == 27 /*|| originbkgLep==27*/)) { // heavy flavor / non-prompt c
        lepton_type = 7;
    } else { // other fake source
        lepton_type = 8;
    }

    DEBUGclass("[%s] lepton %d has type %d", m_variation.c_str(), m_lepton_index, lepton_type);
    return lepton_type;
}
