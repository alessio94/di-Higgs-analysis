#include "CAFbbll/BBLLBJetCalibStudyAlgorithm.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// you can perform debug printouts with statements like this
// DEBUG("error number %d occurred",someInteger);
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#ifndef __CINT__
#include "xAODCore/ShallowCopy.h"
//#include "xAODJet/JetTypes.h"
#else
namespace xAOD {
    ShallowAuxContainer;
}
#endif

BBLLBJetCalibStudyAlgorithm::BBLLBJetCalibStudyAlgorithm()
    : BBLLObjectSelectionAlgorithm() { }

bool BBLLBJetCalibStudyAlgorithm::select_jets(const xAOD::JetContainer& jets, const std::string& variation) {
    if (!BBLLObjectSelectionAlgorithm::select_jets(jets, variation)) {
        return false;
    }

    xAOD::JetContainer* jets_muinjet = nullptr;
    xAOD::JetContainer* jets_muinjet_eloss = nullptr;
    xAOD::JetContainer* jets_muinjet_ptreco_old = nullptr;
    xAOD::JetContainer* jets_muinjet_ptreco_new = nullptr;

    std::vector<const xAOD::Jet*> selected_bjets_muinjet;
    std::vector<const xAOD::Jet*> selected_bjets_muinjet_eloss;
    std::vector<const xAOD::Jet*> selected_bjets_muinjet_ptreco_old;
    std::vector<const xAOD::Jet*> selected_bjets_muinjet_ptreco_new;

    // b-jet corrections - muon in jet, with e-loss correction
    auto shallowCopyJetsMuInJetELoss = xAOD::shallowCopyContainer(jets);
    jets_muinjet_eloss = shallowCopyJetsMuInJetELoss.first;

    for (xAOD::Jet* jet : *jets_muinjet_eloss) {
        if (jet->auxdataConst<int>("CAFisBJet_DL1r_FixedCutBEff_77")) {
            float onemu_eta = jet->auxdataConst<float>("OneMu_eta");
            float onemu_m = jet->auxdataConst<float>("OneMu_m");
            float onemu_phi = jet->auxdataConst<float>("OneMu_phi");
            float onemu_pt = jet->auxdataConst<float>("OneMu_pt");
            xAOD::JetFourMom_t onemu;
            onemu.SetCoordinates(onemu_pt, onemu_eta, onemu_phi, onemu_m);
            jet->setJetP4(jet->jetP4() + onemu);
            selected_bjets_muinjet_eloss.push_back(jet);
        }
    }

    // b-jet corrections - muon in jet, without e-loss correction
    auto shallowCopyJetsMuInJet = xAOD::shallowCopyContainer(jets);
    jets_muinjet = shallowCopyJetsMuInJet.first;

    for (xAOD::Jet* jet : *jets_muinjet) {
        if (jet->auxdataConst<int>("CAFisBJet_DL1r_FixedCutBEff_77")) {
            float onemu_eta = jet->auxdataConst<float>("OneMuNew_eta");
            float onemu_m = jet->auxdataConst<float>("OneMuNew_m");
            float onemu_phi = jet->auxdataConst<float>("OneMuNew_phi");
            float onemu_pt = jet->auxdataConst<float>("OneMuNew_pt");
            xAOD::JetFourMom_t onemu;
            onemu.SetCoordinates(onemu_pt, onemu_eta, onemu_phi, onemu_m);
            jet->setJetP4(jet->jetP4() + onemu);
            selected_bjets_muinjet.push_back(jet);
        }
    }

    // b-jet corrections - muon in jet, with e-loss correction, pt reco factor (old)
    auto shallowCopyJetsPtRecoOld = xAOD::shallowCopyContainer(jets);
    jets_muinjet_ptreco_old = shallowCopyJetsPtRecoOld.first;

    for (xAOD::Jet* jet : *jets_muinjet_ptreco_old) {
        if (jet->auxdataConst<int>("CAFisBJet_DL1r_FixedCutBEff_77")) {
            float onemu_eta = jet->auxdataConst<float>("OneMu_eta");
            float onemu_m = jet->auxdataConst<float>("OneMu_m");
            float onemu_phi = jet->auxdataConst<float>("OneMu_phi");
            float onemu_pt = jet->auxdataConst<float>("OneMu_pt");
            xAOD::JetFourMom_t onemu;
            onemu.SetCoordinates(onemu_pt, onemu_eta, onemu_phi, onemu_m);
            float pt_reco = jet->auxdataConst<float>("factorPtReco");
            jet->setJetP4((jet->jetP4() + onemu) * pt_reco);
            selected_bjets_muinjet_ptreco_old.push_back(jet);
        }
    }

    // b-jet corrections - muon in jet, with e-loss correction, pt reco factor (new)
    auto shallowCopyJetsPtRecoNew = xAOD::shallowCopyContainer(jets);
    jets_muinjet_ptreco_new = shallowCopyJetsPtRecoNew.first;

    for (xAOD::Jet* jet : *jets_muinjet_ptreco_new) {
        if (jet->auxdataConst<int>("CAFisBJet_DL1r_FixedCutBEff_77")) {
            float onemu_eta = jet->auxdataConst<float>("OneMu_eta");
            float onemu_m = jet->auxdataConst<float>("OneMu_m");
            float onemu_phi = jet->auxdataConst<float>("OneMu_phi");
            float onemu_pt = jet->auxdataConst<float>("OneMu_pt");
            xAOD::JetFourMom_t onemu;
            onemu.SetCoordinates(onemu_pt, onemu_eta, onemu_phi, onemu_m);
            float pt_reco = jet->auxdataConst<float>("factorPtRecoNew");
            jet->setJetP4((jet->jetP4() + onemu) * pt_reco);
            selected_bjets_muinjet_ptreco_new.push_back(jet);
        }
    }

    save_particles_to_store<xAOD::JetContainer>(selected_bjets_muinjet, "SelectedBJetsMuInJet___" + variation);
    save_particles_to_store<xAOD::JetContainer>(
        selected_bjets_muinjet_eloss, "SelectedBJetsMuInJetELoss___" + variation);
    save_particles_to_store<xAOD::JetContainer>(
        selected_bjets_muinjet_ptreco_old, "SelectedBJetsPtRecoOld___" + variation);
    save_particles_to_store<xAOD::JetContainer>(
        selected_bjets_muinjet_ptreco_new, "SelectedBJetsPtRecoNew___" + variation);

    return true;
}
