#include "CAFbbll/CombinedQuantities.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

// STL
#include <algorithm>
#include <iterator>

// ROOT
#include "TVectorD.h"

// xAOD
#include "xAODBase/IParticleContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingETContainer.h"

// other
#include "CalcGenericMT2/MT2_ROOT.h"

CombinedQuantities::CombinedQuantities() { DEBUGclass("Default constructor called"); }

CombinedQuantities::~CombinedQuantities() { DEBUGclass("Destructor called"); }

bool CombinedQuantities::calculate_collinear_mass(const TLorentzVector& k1, const TLorentzVector& k2, double met_etx,
    double met_ety, double& mass, Double_t& x1, Double_t& x2) {
    x1 = 0;
    x2 = 0;
    mass = 0;
    TMatrixD K(2, 2);

    K(0, 0) = k1.Px();
    K(0, 1) = k2.Px();
    K(1, 0) = k1.Py();
    K(1, 1) = k2.Py();

    if (K.Determinant() == 0) {
        return false;
    }
    TMatrixD Kinv = K.Invert();

    TMatrixD M(2, 1);
    M(0, 0) = met_etx;
    M(1, 0) = met_ety;

    TMatrixD X(2, 1);
    X = Kinv * M;

    double X1 = X(0, 0);
    double X2 = X(1, 0);

    x1 = 1. / (1. + X1);
    x2 = 1. / (1. + X2);

    TLorentzVector p1 = k1 * (1 / x1);
    TLorentzVector p2 = k2 * (1 / x2);
    mass = (p1 + p2).M();

    return true;
}

double CombinedQuantities::calculate_sphericity(const std::vector<TLorentzVector>& vects) {
    if (vects.size() == 0) {
        return 0;
    }

    TMatrixD S(3, 3);
    for (UInt_t i = 0; i < 3; ++i) {
        for (UInt_t j = 0; j < 3; ++j) {
            S[i][j] = 0;
            for (auto vect : vects) {
                S[i][j] += (vect.Vect()[i] * vect.Vect()[j]);
            }
        }
    }

    // normalize
    double norm = 0;
    for (auto vect : vects)
        norm += (vect.Vect().Mag2());
    S *= 1.0 / norm;

    TVectorD eigenValues;
    S.EigenVectors(eigenValues);

    std::vector<double> eigvals = { eigenValues[0], eigenValues[1], eigenValues[2] };
    std::sort(eigvals.begin(), eigvals.end());

    // getSphericity
    return (eigvals[0] + eigvals[1]) * 1.5;
}

TLorentzVector CombinedQuantities::to_gev(const TLorentzVector& v) {
    TLorentzVector result;
    result.SetPtEtaPhiE(v.Pt() / 1000, v.Eta(), v.Phi(), v.E() / 1000);
    return result;
}

double CombinedQuantities::getValue() const {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    std::string variation = fSample->getTagStandardStringDefault("~p4Variation", "Nominal");

    // get leptons
    const xAOD::IParticleContainer* leptons = nullptr;
    TLorentzVector lep0;
    TLorentzVector lep1;
    if (use_lepton_container) {
        if (!get_bbll_lepton_container(fEvent, variation, leptons)) {
            return false;
        }
        if (leptons->size() >= 1) {
            lep0 = leptons->at(0)->p4();
        }
        if (leptons->size() >= 2) {
            lep1 = leptons->at(1)->p4();
        }
    }

    // retrieve b-jets
    const xAOD::JetContainer* b_jets = nullptr;
    TLorentzVector bjet0;
    TLorentzVector bjet1;
    if (use_bjet_container) {
        if (!get_bbll_bjet_container(fEvent, variation, b_jets)) {
            return false;
        }
        if (b_jets->size() >= 1) {
            bjet0 = b_jets->at(0)->p4();
        }
        if (b_jets->size() >= 2) {
            bjet1 = b_jets->at(1)->p4();
        }
    }

    // retrieve other jets
    const xAOD::JetContainer* other_jets = nullptr;
    if (use_otherjets_container) {
        if (!get_bbll_otherjet_container(fEvent, variation, other_jets)) {
            return false;
        }
    }

    // retrieve MET
    const xAOD::MissingETContainer* missing_et_container = nullptr;
    const xAOD::MissingET* missing_et = nullptr;
    TLorentzVector met_vector;
    if (use_met_container) {
        if (!get_xaod_met_container(fEvent, variation, missing_et_container)) {
            return false;
        }
        missing_et = missing_et_container->at(0);
        // MET has per definition eta = 0 (perpendicular to beam axis)
        met_vector.SetPtEtaPhiE(missing_et->met(), 0, missing_et->phi(), missing_et->met());
    }

    // retrieve MMC
    xAOD::EventInfo const* mmc_container = nullptr;
    TLorentzVector mmc_vector;
    if (use_mmc_container) {
        if (!get_xaod_mmc_container(fEvent, variation, mmc_container)) {
            return false;
        }
        float pt = mmc_container->auxdataConst<float>("mmc_mlnu3p_4vect_pt");
        float eta = mmc_container->auxdataConst<float>("mmc_mlnu3p_4vect_eta");
        float phi = mmc_container->auxdataConst<float>("mmc_mlnu3p_4vect_phi");
        float e = mmc_container->auxdataConst<float>("mmc_mlnu3p_4vect_e");
        mmc_vector.SetPtEtaPhiE(pt, eta, phi, e);
    }

    // retrieve truth particle container
    const xAOD::TruthParticleContainer* truth_particles = nullptr;
    if (use_truth_particle_container) {
        if (!get_xaod_truth_particle_container(fEvent, truth_particles)) {
            return false;
        }
    }

    // retrieve truth met container
    const xAOD::MissingETContainer* truth_missing_et_container = nullptr;
    const xAOD::MissingET* truth_missing_et = nullptr;
    TLorentzVector truth_met_vector;
    if (use_truth_met_container) {
        if (!get_xaod_truth_met_container(fEvent, variation, truth_missing_et_container)) {
            return false;
        }
        truth_missing_et = truth_missing_et_container->at(0);
        // MET has per definition eta = 0 (perpendicular to beam axis)
        truth_met_vector.SetPtEtaPhiE(truth_missing_et->met(), 0, truth_missing_et->phi(), truth_missing_et->met());
    }

    // the transverse mass is defined in the high energy limit as
    // sqrt(2 * E(MET) * E(lep) * (1 - cos(DeltaPhi(lep, MET))))
    // E(lep) = pT(lep) in the high energy limit
    if (m_mode == MT_LEP0_MET) {
        return TMath::Sqrt(2 * missing_et->met() * lep0.Pt() * (1 - TMath::Cos(lep0.DeltaPhi(met_vector))));
    }
    if (m_mode == MT_LEP1_MET) {
        return TMath::Sqrt(2 * missing_et->met() * lep1.Pt() * (1 - TMath::Cos(lep1.DeltaPhi(met_vector))));
    }
    if (m_mode == MT_BJET0_MET) {
        return TMath::Sqrt(2 * missing_et->met() * bjet0.Pt() * (1 - TMath::Cos(bjet0.DeltaPhi(met_vector))));
    }
    if (m_mode == MT_BJET1_MET) {
        return TMath::Sqrt(2 * missing_et->met() * bjet1.Pt() * (1 - TMath::Cos(bjet1.DeltaPhi(met_vector))));
    }

    // contransversal mass
    if (m_mode == MCT) {
        if (b_jets->size() < 2) {
            DEBUGclass("There are less than two b-jets for MCT observable. Setting value to %g.", DEFAULT_VALUE);
            return DEFAULT_VALUE;
        } else {
            double sum_et = bjet0.Et() + bjet1.Et();
            double diff_pt = bjet0.Pt() - bjet1.Pt();
            return TMath::Sqrt(sum_et * sum_et - diff_pt * diff_pt);
        }
    }

    // collinear mass
    if (m_mode == MCOLL || m_mode == MCOLL_X0 || m_mode == MCOLL_X1) {
        double met_etx = missing_et->mpx();
        double met_ety = missing_et->mpy();
        double mcoll = 0;
        double x0 = 0;
        double x1 = 0;
        bool mcoll_success = CombinedQuantities::calculate_collinear_mass(lep0, lep1, met_etx, met_ety, mcoll, x0, x1);

        if (!mcoll_success) {
            INFOclass("Failed to calculate collinear approximation. Setting collinear mass and moment fractions to 0.");
        }

        if (m_mode == MCOLL) {
            return mcoll;
        } else if (m_mode == MCOLL_X0) {
            return x0;
        } else if (m_mode == MCOLL_X1) {
            return x1;
        }
    }

    // DeltaR of b-jets and leptons
    if (m_mode == DELTAR_BL_MIN || m_mode == DELTAR_BL_MAX) {
        std::vector<double> deltaRs;
        for (const xAOD::IParticle* lepton : *leptons) {
            for (const xAOD::Jet* bjet : *b_jets) {
                deltaRs.push_back(bjet->p4().DeltaR(lepton->p4()));
            }
        }
        if (m_mode == DELTAR_BL_MIN) {
            auto result = std::min_element(std::begin(deltaRs), std::end(deltaRs));
            if (std::end(deltaRs) != result) {
                return *result;
            } else {
                return DEFAULT_VALUE;
            }
        } else if (m_mode == DELTAR_BL_MAX) {
            auto result = std::max_element(std::begin(deltaRs), std::end(deltaRs));
            if (std::end(deltaRs) != result) {
                return *result;
            } else {
                return DEFAULT_VALUE;
            }
        }
    }

    // DeltaR/DeltaEta/DeltaPhi between lepton and closest jet
    if (m_mode == DELTAR_L0_CLOSESTJET || m_mode == DELTAETA_L0_CLOSESTJET || m_mode == DELTAPHI_L0_CLOSESTJET) {
        const xAOD::Jet* closest_jet = nullptr;
        float closest_jet_deltar = 1000;
        for (const xAOD::Jet* bjet : *b_jets) {
            float this_deltar = lep0.DeltaR(bjet->p4());
            if (this_deltar < closest_jet_deltar) {
                closest_jet = bjet;
                closest_jet_deltar = this_deltar;
            }
        }
        for (const xAOD::Jet* otherjet : *other_jets) {
            float this_deltar = lep0.DeltaR(otherjet->p4());
            if (this_deltar < closest_jet_deltar) {
                closest_jet = otherjet;
                closest_jet_deltar = this_deltar;
            }
        }

        if (m_mode == DELTAR_L0_CLOSESTJET) {
            return closest_jet_deltar;
        } else if (m_mode == DELTAETA_L0_CLOSESTJET) {
            return lep0.Eta() - closest_jet->eta();
        } else if (m_mode == DELTAPHI_L0_CLOSESTJET) {
            return lep0.DeltaPhi(closest_jet->p4());
        }
    }
    if (m_mode == DELTAR_L1_CLOSESTJET || m_mode == DELTAETA_L1_CLOSESTJET || m_mode == DELTAPHI_L1_CLOSESTJET) {
        const xAOD::Jet* closest_jet = nullptr;
        float closest_jet_deltar = 1000;
        for (const xAOD::Jet* bjet : *b_jets) {
            float this_deltar = lep1.DeltaR(bjet->p4());
            if (this_deltar < closest_jet_deltar) {
                closest_jet = bjet;
                closest_jet_deltar = this_deltar;
            }
        }
        for (const xAOD::Jet* otherjet : *other_jets) {
            float this_deltar = lep1.DeltaR(otherjet->p4());
            if (this_deltar < closest_jet_deltar) {
                closest_jet = otherjet;
                closest_jet_deltar = this_deltar;
            }
        }

        if (m_mode == DELTAR_L1_CLOSESTJET) {
            return closest_jet_deltar;
        } else if (m_mode == DELTAETA_L1_CLOSESTJET) {
            return lep1.Eta() - closest_jet->eta();
        } else if (m_mode == DELTAPHI_L1_CLOSESTJET) {
            return lep1.DeltaPhi(closest_jet->p4());
        }
    }

    // combine bb + ll + met
    if (m_mode == M_BBLL || m_mode == PT_BBLL_SCALAR || m_mode == PT_BBLL_VECTOR) {
        if (b_jets->size() < 1) {
            DEBUGclass("There is less than one b-jet for the BBLL observables. Setting value to %g.", DEFAULT_VALUE);
            return DEFAULT_VALUE;
        } else {
            if (m_mode == M_BBLL) {
                TLorentzVector p = lep0 + lep1 + bjet0 + bjet1;
                return p.M();
            } else if (m_mode == PT_BBLL_SCALAR) {
                return lep0.Pt() + lep1.Pt() + bjet0.Pt() + bjet1.Pt();
            } else if (m_mode == PT_BBLL_VECTOR) {
                TLorentzVector p = lep0 + lep1 + bjet0 + bjet1;
                return p.M();
            }
        }
    }

    // combine bb + ll + met
    if (m_mode == M_BBLLMET || m_mode == PT_BBLLMET_SCALAR || m_mode == PT_BBLLMET_VECTOR) {
        if (b_jets->size() < 1) {
            DEBUGclass("There is less than one b-jet for the BBLLMET observables. Setting value to %g.", DEFAULT_VALUE);
            return DEFAULT_VALUE;
        } else {
            if (m_mode == M_BBLLMET) {
                TLorentzVector p = lep0 + lep1 + bjet0 + bjet1 + met_vector;
                return p.M();
            } else if (m_mode == PT_BBLLMET_SCALAR) {
                return lep0.Pt() + lep1.Pt() + bjet0.Pt() + bjet1.Pt() + met_vector.Pt();
            } else if (m_mode == PT_BBLLMET_VECTOR) {
                TLorentzVector p = lep0 + lep1 + bjet0 + bjet1 + met_vector;
                return p.M();
            }
        }
    }

    // H1 = bb system and H2 = four vector from MMC
    if (m_mode == M_HH) {
        if (b_jets->size() < 1) {
            DEBUGclass("There is less than one b-jet for the MHH observable. Setting value to %g.", DEFAULT_VALUE);
            return DEFAULT_VALUE;
        }
        TLorentzVector bb_system = bjet0 + bjet1;
        double m_bb = bb_system.M();
        double m_tautau = mmc_vector.M();
        if (m_bb == 0 || m_tautau == 0 || std::isnan(m_tautau)) { // this can happen e.g. if the MMC algorithm fails
            return 0;
        }
        double scale_bb = 125000 / m_bb;      // the bb system is stored in MeV
        double scale_tautau = 125 / m_tautau; // the MMC mass is stored in GeV
        return (scale_bb * bb_system + scale_tautau * 1000 * mmc_vector).M();
    }

    // H1 = bb system and H2 = four vector from MMC
    if (m_mode == DPHI_HH) {
        if (b_jets->size() < 1) {
            DEBUGclass(
                "There is less than one b-jet for the DeltaPhi(H,H) observable. Setting value to %g.", DEFAULT_VALUE);
            return DEFAULT_VALUE;
        }
        TLorentzVector bb_system = bjet0 + bjet1;
        return bb_system.DeltaPhi(mmc_vector);
    }

    if (m_mode == MET_PHI_CENTRALITY) {
        double phi_l1 = lep0.Phi();
        double phi_l2 = lep1.Phi();
        double phi_met = missing_et->phi();
        if ((phi_l2 - phi_l1 != 0) && (phi_l2 - phi_l1 != 2 * TMath::Pi()) && (phi_l2 - phi_l1 != -2 * TMath::Pi())) {
            double c_phi_a = TMath::Sin(phi_met - phi_l1) / TMath::Sin(phi_l2 - phi_l1);
            double c_phi_b = TMath::Sin(phi_l2 - phi_met) / TMath::Sin(phi_l2 - phi_l1);
            return (c_phi_a + c_phi_b) / TMath::Sqrt(c_phi_a * c_phi_a + c_phi_b * c_phi_b) / TMath::Sqrt(2);
        } else {
            return -1e11;
        }
    }

    if (m_mode == MET_REL) {
        std::vector<double> deltaPhis;
        for (const xAOD::Jet* bjet : *b_jets) {
            deltaPhis.push_back(fabs(met_vector.DeltaPhi(bjet->p4())));
        }
        for (const xAOD::Jet* otherjet : *other_jets) {
            deltaPhis.push_back(fabs(met_vector.DeltaPhi(otherjet->p4())));
        }
        for (const xAOD::IParticle* lepton : *leptons) {
            deltaPhis.push_back(fabs(met_vector.DeltaPhi(lepton->p4())));
        }
        auto closest_delta_phi_l_j = std::min_element(std::begin(deltaPhis), std::end(deltaPhis));
        if (*closest_delta_phi_l_j >= TMath::Pi() / 2) {
            return missing_et->met();
        } else {
            return missing_et->met() * TMath::Sin(*closest_delta_phi_l_j);
        }
    }

    if (m_mode == DPHI_MET_LL) {
        return met_vector.DeltaPhi(lep0 + lep1);
    }
    if (m_mode == DPHI_MET_LL_BB) {
        if (b_jets->size() < 2) {
            DEBUGclass("There are less than two b-jets for the DeltaPhi(MET+ll,bb) observable. Setting value to %g.",
                DEFAULT_VALUE);
            return DEFAULT_VALUE;
        }
        return (met_vector + lep0 + lep1).DeltaPhi(bjet0 + bjet1);
    }
    if (m_mode == DPT_MET_LL) {
        return met_vector.Pt() - (lep0 + lep1).Pt();
    }
    if (m_mode == DPT_MET_LL_BB) {
        if (b_jets->size() < 2) {
            DEBUGclass("There are less than two b-jets for the DeltaPt(MET+ll,bb) observable. Setting value to %g.",
                DEFAULT_VALUE);
            return DEFAULT_VALUE;
        }
        return (met_vector + lep0 + lep1).Pt() - (bjet0 + bjet1).Pt();
    }

    if (m_mode == SUM_MET_PTLL) {
        return (met_vector + lep0 + lep1).Mag();
    }

    if (m_mode == HT2 || m_mode == HT2R) {
        TVector3 met_pt_vector = TVector3(met_vector.X(), met_vector.Y(), 0);
        TVector3 lep0_pt_vector = TVector3(lep0.X(), lep0.Y(), 0);
        TVector3 lep1_pt_vector = TVector3(lep1.X(), lep1.Y(), 0);
        TVector3 bjet0_pt_vector = TVector3(bjet0.X(), bjet0.Y(), 0);
        TVector3 bjet1_pt_vector = TVector3(bjet1.X(), bjet1.Y(), 0);
        double ht2 =
            (met_pt_vector + lep0_pt_vector + lep1_pt_vector).Mag() + (bjet0_pt_vector + bjet1_pt_vector).Mag();
        if (m_mode == HT2) {
            return ht2;
        } else {
            return ht2
                / (met_pt_vector.Mag() + lep0_pt_vector.Mag() + lep1_pt_vector.Mag() + bjet0_pt_vector.Mag()
                    + bjet1_pt_vector.Mag());
        }
    }

    if (m_mode == HT) {
        double ht = 0;

        // loop over all b-jets
        for (const xAOD::Jet* jet : *b_jets) {
            ht += jet->pt();
        }
        // loop over all non b-jets
        for (const xAOD::Jet* jet : *other_jets) {
            ht += jet->pt();
        }

        return ht;
    }

    if (m_mode == SPHERICITY) {
        std::vector<TLorentzVector> vects;
        vects.emplace_back(lep0);
        vects.emplace_back(lep1);
        vects.emplace_back(bjet0);
        vects.emplace_back(bjet1);
        for (const xAOD::Jet* jet : *other_jets) {
            vects.emplace_back(jet->p4());
        }
        return CombinedQuantities::calculate_sphericity(vects);
    }

    if (m_mode == MT2_BB) {
        if (b_jets->size() < 2) {
            DEBUGclass(
                "There are less than two b-jets for the M_T2^BB observable. Setting value to %g.", DEFAULT_VALUE);
            return DEFAULT_VALUE;
        }

        TLorentzVector bjet0_gev = to_gev(bjet0);
        TLorentzVector bjet1_gev = to_gev(bjet1);
        TLorentzVector met_vector_gev = to_gev(met_vector);

        ComputeMT2 mt2_calculator = ComputeMT2(bjet0_gev, bjet1_gev, met_vector_gev, 0, 0);
        return mt2_calculator.Compute();
    }

    if (m_mode == VBF_MAX_MJJ) {
        if (other_jets->size() < 2) {
            DEBUGclass("There are less than two non b-tagged jets for the VBF_MAX_MJJ observable. Setting value to %g.",
                DEFAULT_VALUE);
            return DEFAULT_VALUE;
        }

        float max_mjj = 0;
        for (const xAOD::Jet* other_jet_1 : *other_jets) {
            for (const xAOD::Jet* other_jet_2 : *other_jets) {
                if (other_jet_1 == other_jet_2) {
                    continue;
                }
                float this_mjj = (other_jet_1->p4() + other_jet_2->p4()).M();
                if (this_mjj > max_mjj) {
                    max_mjj = this_mjj;
                }
            }
        }
        return max_mjj;
    }

    if (m_mode == VBF_MAX_DELTA_ETA_JJ) {
        if (other_jets->size() < 2) {
            DEBUGclass("There are less than two non b-tagged jets for the VBF_MAX_DELTA_ETA_JJ observable. Setting "
                       "value to %g.",
                DEFAULT_VALUE);
            return DEFAULT_VALUE;
        }

        float max_delta_eta_jj = 0;
        for (const xAOD::Jet* other_jet_1 : *other_jets) {
            for (const xAOD::Jet* other_jet_2 : *other_jets) {
                if (other_jet_1 == other_jet_2) {
                    continue;
                }
                float this_delta_eta_jj = other_jet_1->eta() - other_jet_2->eta();
                if (this_delta_eta_jj > max_delta_eta_jj) {
                    max_delta_eta_jj = this_delta_eta_jj;
                }
            }
        }
        return max_delta_eta_jj;
    }

    if (m_mode == TRUTH_MLL || m_mode == TRUTH_MLLMET) {
        if (leptons->size() < 2) {
            return DEFAULT_VALUE;
        }
        auto l0 = leptons->at(0);
        auto l1 = leptons->at(1);
        float lep0_truth_pt = l0->auxdataConst<float>("TruthPt");
        float lep0_truth_eta = l0->auxdataConst<float>("TruthEta");
        float lep0_truth_phi = l0->auxdataConst<float>("TruthPhi");
        float lep0_truth_e = l0->auxdataConst<float>("TruthE");
        float lep1_truth_pt = l1->auxdataConst<float>("TruthPt");
        float lep1_truth_eta = l1->auxdataConst<float>("TruthEta");
        float lep1_truth_phi = l1->auxdataConst<float>("TruthPhi");
        float lep1_truth_e = l1->auxdataConst<float>("TruthE");
        TLorentzVector l0_truth_vector;
        TLorentzVector l1_truth_vector;
        l0_truth_vector.SetPtEtaPhiE(lep0_truth_pt, lep0_truth_eta, lep0_truth_phi, lep0_truth_e);
        l1_truth_vector.SetPtEtaPhiE(lep1_truth_pt, lep1_truth_eta, lep1_truth_phi, lep1_truth_e);

        if (m_mode == TRUTH_MLL) {
            return (l0_truth_vector + l1_truth_vector).M();
        }
        // TRUTH_MLLMET
        return (l0_truth_vector + l1_truth_vector + truth_met_vector).M();
    }

    if (m_mode == TRUTH_MBB) {
        std::vector<const xAOD::TruthParticle*> truth_bjets;
        for (const xAOD::TruthParticle* truth_particle : *truth_particles) {
            if (truth_particle->absPdgId() == 5) {
                truth_bjets.emplace_back(truth_particle);
            }
        }
        if (truth_bjets.size() < 2) {
            return DEFAULT_VALUE;
        }
        return (truth_bjets.at(0)->p4() + truth_bjets.at(1)->p4()).M();
    }

    if (m_mode == TRUTH_MET) {
        return truth_missing_et->met();
    }

    // this code should never be executed
    ERRORclass("Unknown mode for combined quantities observable!");
    return -1e11;
}

//______________________________________________________________________________________________

CombinedQuantities::CombinedQuantities(const TString& name)
    : TQEventObservable(name) {
    // constructor with name argument
    DEBUGclass("constructor called with '%s'", name.Data());

    if (name == TString("MT:lep0_MET")) {
        m_mode = MT_LEP0_MET;
        use_lepton_container = true;
        use_met_container = true;
    } else if (name == TString("MT:lep1_MET")) {
        m_mode = MT_LEP1_MET;
        use_lepton_container = true;
        use_met_container = true;
    } else if (name == TString("MT:bjet0_MET")) {
        m_mode = MT_BJET0_MET;
        use_bjet_container = true;
        use_met_container = true;
    } else if (name == TString("MT:bjet1_MET")) {
        m_mode = MT_BJET1_MET;
        use_bjet_container = true;
        use_met_container = true;
    } else if (name == TString("MCT")) {
        m_mode = MCT;
        use_bjet_container = true;
    } else if (name == TString("MCOLL:m")) {
        m_mode = MCOLL;
        use_lepton_container = true;
        use_met_container = true;
    } else if (name == TString("MCOLL:x0")) {
        m_mode = MCOLL_X0;
        use_lepton_container = true;
        use_met_container = true;
    } else if (name == TString("MCOLL:x1")) {
        m_mode = MCOLL_X1;
        use_lepton_container = true;
        use_met_container = true;
    } else if (name == TString("DELTAR_BL_MIN")) {
        m_mode = DELTAR_BL_MIN;
        use_lepton_container = true;
        use_bjet_container = true;
    } else if (name == TString("DELTAR_BL_MAX")) {
        m_mode = DELTAR_BL_MAX;
        use_lepton_container = true;
        use_bjet_container = true;
    } else if (name == TString("DELTAR:lep0_jclosest")) {
        m_mode = DELTAR_L0_CLOSESTJET;
        use_lepton_container = true;
        use_bjet_container = true;
        use_otherjets_container = true;
    } else if (name == TString("DELTAR:lep1_jclosest")) {
        m_mode = DELTAR_L1_CLOSESTJET;
        use_lepton_container = true;
        use_bjet_container = true;
        use_otherjets_container = true;
    } else if (name == TString("DELTAETA:lep0_jclosest")) {
        m_mode = DELTAETA_L0_CLOSESTJET;
        use_lepton_container = true;
        use_bjet_container = true;
        use_otherjets_container = true;
    } else if (name == TString("DELTAETA:lep1_jclosest")) {
        m_mode = DELTAETA_L1_CLOSESTJET;
        use_lepton_container = true;
        use_bjet_container = true;
        use_otherjets_container = true;
    } else if (name == TString("DELTAPHI:lep0_jclosest")) {
        m_mode = DELTAPHI_L0_CLOSESTJET;
        use_lepton_container = true;
        use_bjet_container = true;
        use_otherjets_container = true;
    } else if (name == TString("DELTAPHI:lep1_jclosest")) {
        m_mode = DELTAPHI_L1_CLOSESTJET;
        use_lepton_container = true;
        use_bjet_container = true;
        use_otherjets_container = true;
    } else if (name == TString("M_BBLL")) {
        m_mode = M_BBLL;
        use_lepton_container = true;
        use_bjet_container = true;
    } else if (name == TString("Pt_BBLL_Scalar")) {
        m_mode = PT_BBLL_SCALAR;
        use_lepton_container = true;
        use_bjet_container = true;
    } else if (name == TString("Pt_BBLL_Vector")) {
        m_mode = PT_BBLL_VECTOR;
        use_lepton_container = true;
        use_bjet_container = true;
    } else if (name == TString("M_BBLLMET")) {
        m_mode = M_BBLLMET;
        use_lepton_container = true;
        use_bjet_container = true;
        use_met_container = true;
    } else if (name == TString("Pt_BBLLMET_Scalar")) {
        m_mode = PT_BBLLMET_SCALAR;
        use_lepton_container = true;
        use_bjet_container = true;
        use_met_container = true;
    } else if (name == TString("Pt_BBLLMET_Vector")) {
        m_mode = PT_BBLLMET_VECTOR;
        use_lepton_container = true;
        use_bjet_container = true;
        use_met_container = true;
    } else if (name == TString("M_HH")) {
        m_mode = M_HH;
        use_bjet_container = true;
        use_mmc_container = true;
    } else if (name == TString("DPHI_HH")) {
        m_mode = DPHI_HH;
        use_bjet_container = true;
        use_mmc_container = true;
    } else if (name == TString("MET_PHI_CENTRALITY")) {
        m_mode = MET_PHI_CENTRALITY;
        use_lepton_container = true;
        use_met_container = true;
    } else if (name == TString("MET:rel")) {
        m_mode = MET_REL;
        use_lepton_container = true;
        use_bjet_container = true;
        use_otherjets_container = true;
        use_met_container = true;
    } else if (name == TString("DPHI_MET_LL")) {
        m_mode = DPHI_MET_LL;
        use_lepton_container = true;
        use_met_container = true;
    } else if (name == TString("DPHI_MET_LL_BB")) {
        m_mode = DPHI_MET_LL_BB;
        use_lepton_container = true;
        use_bjet_container = true;
        use_met_container = true;
    } else if (name == TString("DPT_MET_LL")) {
        m_mode = DPT_MET_LL;
        use_lepton_container = true;
        use_met_container = true;
    } else if (name == TString("DPT_MET_LL_BB")) {
        m_mode = DPT_MET_LL_BB;
        use_lepton_container = true;
        use_bjet_container = true;
        use_met_container = true;
    } else if (name == TString("SUM_MET_PTLL")) {
        m_mode = SUM_MET_PTLL;
        use_lepton_container = true;
        use_met_container = true;
    } else if (name == TString("HT2")) {
        m_mode = HT2;
        use_lepton_container = true;
        use_bjet_container = true;
        use_met_container = true;
    } else if (name == TString("HT2R")) {
        m_mode = HT2R;
        use_lepton_container = true;
        use_bjet_container = true;
        use_met_container = true;
    } else if (name == TString("HT")) {
        m_mode = HT;
        use_bjet_container = true;
        use_otherjets_container = true;
    } else if (name == TString("SPHERICITY")) {
        m_mode = SPHERICITY;
        use_lepton_container = true;
        use_bjet_container = true;
        use_otherjets_container = true;
    } else if (name == TString("MT2_BB")) {
        m_mode = MT2_BB;
        use_bjet_container = true;
        use_met_container = true;
    } else if (name == TString("VBF_MAX_MJJ")) {
        m_mode = VBF_MAX_MJJ;
        use_otherjets_container = true;
    } else if (name == TString("VBF_MAX_DELTA_ETA_JJ")) {
        m_mode = VBF_MAX_DELTA_ETA_JJ;
        use_otherjets_container = true;
    } else if (name == TString("TRUTH_MLL")) {
        m_mode = TRUTH_MLL;
        use_lepton_container = true;
    } else if (name == TString("TRUTH_MBB")) {
        m_mode = TRUTH_MBB;
        use_truth_particle_container = true;
    } else if (name == TString("TRUTH_MET")) {
        m_mode = TRUTH_MET;
        use_truth_met_container = true;
    } else if (name == TString("TRUTH_MLLMET")) {
        m_mode = TRUTH_MLLMET;
        use_lepton_container = true;
        use_truth_met_container = true;
    } else {
        ERRORclass("Unkown mode for combined quantities observable: '%s'", name.Data());
        m_mode = UNKNOWN;
    }
}

void CombinedQuantities::set_met_collection(const std::string& jet_collection) {
    std::string met_container_name = "MET_Reference_AntiKt4" + jet_collection;
    set_met_container_name(met_container_name);
    DEBUGclass("Set met container name to '%s'", met_container_name.c_str());
}
