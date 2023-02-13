#include "CAFbbll/TwoNeutrinoScan.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

// QFramework
#include "QFramework/TQPathManager.h"

// xAOD
#include "xAODBase/IParticleContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingETContainer.h"

TwoNeutrinoScan::TwoNeutrinoScan() { DEBUGclass("Default constructor called"); }

TwoNeutrinoScan::~TwoNeutrinoScan() { DEBUGclass("Destructor called"); }

double TwoNeutrinoScan::getValue() const {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    std::string variation = fSample->getTagStandardStringDefault("~p4Variation", "Nominal");

    // get leptons
    const xAOD::IParticleContainer* leptons = nullptr;
    if (!get_bbll_lepton_container(fEvent, variation, leptons)) {
        return false;
    }

    // retrieve b-jets
    const xAOD::JetContainer* b_jets = nullptr;
    if (!get_bbll_bjet_container(fEvent, variation, b_jets)) {
        return false;
    }

    if (leptons->size() < 2 || b_jets->size() < 2) {
        return DEFAULT_VALUE;
    }

    // retrieve MET
    const xAOD::MissingETContainer* missing_et_container = nullptr;
    const xAOD::MissingET* missing_et = nullptr;
    if (!get_xaod_met_container(fEvent, variation, missing_et_container)) {
        return false;
    }
    missing_et = missing_et_container->at(0);
    float met_met_gev = missing_et->met() / 1000;
    float met_phi = missing_et->phi();

    // input values
    std::vector<TLorentzVector> v_recoLeptons;
    std::vector<TLorentzVector> v_recoJets;
    v_recoLeptons.push_back(to_gev(leptons->at(0)->p4()));
    v_recoLeptons.push_back(to_gev(leptons->at(1)->p4()));
    v_recoJets.push_back(to_gev(b_jets->at(0)->p4()));
    v_recoJets.push_back(to_gev(b_jets->at(1)->p4()));

    // results will be filled into these variables
    std::vector<int> v_2vSM_recoIndices;
    float weight_2vSM(0.);
    TLorentzVector nu1, nu2;

    v_2vSM_recoIndices = Run2vSMReconstruction(v_recoLeptons, v_recoJets, nu1, nu2, met_met_gev, met_phi, weight_2vSM);

    if (m_mode == WEIGHT) {
        return weight_2vSM;
    }

    // this code should never be executed
    ERRORclass("Unknown mode for TwoNeutrinoScan observable!");
    return DEFAULT_VALUE;
}

//______________________________________________________________________________________________

TwoNeutrinoScan::TwoNeutrinoScan(const TString& name)
    : TQEventObservable(name) {
    // constructor with name argument
    DEBUGclass("constructor called with '%s'", name.Data());

    if (name == TString("2vScan:weight")) {
        m_mode = WEIGHT;
    } else {
        ERRORclass("Unkown mode for TwoNeutrinoScan observable: '%s'", name.Data());
        m_mode = UNKNOWN;
    }
}

//______________________________________________________________________________________________

void TwoNeutrinoScan::set_met_collection(const std::string& jet_collection) {
    std::string met_container_name = "MET_Reference_AntiKt4" + jet_collection;
    set_met_container_name(met_container_name);
    DEBUGclass("Set met container name to '%s'", met_container_name.c_str());
}

//______________________________________________________________________________________________

bool TwoNeutrinoScan::initializeSelf() {
    DEBUGclass("initializeSelf called");
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    std::string ref_file_path = "bbll/auxData/TwoNeutrinoScan/reco_weights_mc_reference_v2.root";
    std::string ref_file_path_resolved =
        TQPathManager::getPathManager()->findFileFromEnvVar(ref_file_path, "CAFANALYSISSHARE");
    DEBUGclass("Opening reference histo file for interpolation in reco algorithms...");
    m_ref_file = TFile::Open(ref_file_path_resolved.c_str(), "READ");
    if (!m_ref_file) {
        ERRORclass("Reference histogram file for interpolation is missing, exiting...");
        return false;
    }

    m_h1d_mtlep_ref = (TH1F*)m_ref_file->Get("h1d_mtlep");
    DEBUGclass("Retrieving leptonic top interpolation histo...");
    if (!m_h1d_mtlep_ref) {
        ERRORclass("Reference histogram h1d_mtlep is missing, exiting...");
        return false;
    }

    m_h1d_dExy_ref = (TH1F*)m_ref_file->Get("h1d_2vSM_dExy_2l");
    DEBUGclass("Retrieving ETmiss (x,y) interpolation histo...");
    if (!m_h1d_dExy_ref) {
        ERRORclass("Reference histogram h1d_2vSM_dExy_2l is missing, exiting...");
        return false;
    }

    m_h2d_veto_eta_bbll_ref = (TH2F*)m_ref_file->Get("h2d_2vSM_kinematic_constraint_1_veto_2l");
    DEBUGclass("Retrieving eta(bbll) veto histo...");
    if (!m_h2d_veto_eta_bbll_ref) {
        ERRORclass("Reference histogram h2d_2vSM_kinematic_constraint_1_veto_2l is missing, exiting...");
        return false;
    }

    m_h2d_veto_dR_lnu_ref = (TH2F*)m_ref_file->Get("h2d_2vSM_kinematic_constraint_2_veto_2l");
    DEBUGclass("Retrieving dR(l,v) veto histo...");
    if (!m_h2d_veto_dR_lnu_ref) {
        ERRORclass("Reference histogram h2d_2vSM_kinematic_constraint_2_veto_2l is missing, exiting...");
        return false;
    }

    m_h2d_veto_lb_association_ref = (TH2F*)m_ref_file->Get("h2d_2vSM_lb_association_veto_2l");
    DEBUGclass("Retrieving lb association veto histo...");
    if (!m_h2d_veto_lb_association_ref) {
        ERRORclass("Reference histogram h2d_2vSM_lb_association_veto_2l is missing, exiting...");
        return false;
    }

    m_h1d_prob_mlb_vs_dmlb_ref[0] = (TH1F*)m_ref_file->Get("h1d_2vSM_lb_association_same_hemisphere_prob_2l");
    DEBUGclass("Retrieving same hemisphere lb association probability vs d(mlb)...");
    if (!m_h1d_prob_mlb_vs_dmlb_ref[0]) {
        ERRORclass("Reference histogram h1d_2vSM_lb_association_same_hemisphere_prob_2l is missing, exiting...");
        return false;
    }

    m_h1d_prob_mlb_vs_dmlb_ref[1] = (TH1F*)m_ref_file->Get("h1d_2vSM_lb_association_opp_hemisphere_prob_2l");
    DEBUGclass("Retrieving opposite hemisphere lb association probability vs d(mlb)...");
    if (!m_h1d_prob_mlb_vs_dmlb_ref[1]) {
        ERRORclass("Reference histogram h1d_2vSM_lb_association_opp_hemisphere_prob_2l is missing, exiting...");
        return false;
    }

    return true;
}

//______________________________________________________________________________________________

bool TwoNeutrinoScan::finalizeSelf() {
    DEBUGclass("finalizeSelf called");

    if (m_h1d_mtlep_ref) {
        m_h1d_mtlep_ref = nullptr;
    }
    if (m_h1d_dExy_ref) {
        m_h1d_dExy_ref = nullptr;
    }
    if (m_h1d_prob_mlb_vs_dmlb_ref[0]) {
        m_h1d_prob_mlb_vs_dmlb_ref[0] = nullptr;
    }
    if (m_h1d_prob_mlb_vs_dmlb_ref[1]) {
        m_h1d_prob_mlb_vs_dmlb_ref[1] = nullptr;
    }
    if (m_h2d_veto_eta_bbll_ref) {
        m_h2d_veto_eta_bbll_ref = nullptr;
    }
    if (m_h2d_veto_dR_lnu_ref) {
        m_h2d_veto_dR_lnu_ref = nullptr;
    }
    if (m_h2d_veto_lb_association_ref) {
        m_h2d_veto_lb_association_ref = nullptr;
    }

    if (m_ref_file) {
        m_ref_file->Close();
        m_ref_file = nullptr;
    }

    return true;
}

//______________________________________________________________________________________________

TLorentzVector TwoNeutrinoScan::to_gev(const TLorentzVector& v) {
    TLorentzVector result;
    result.SetPtEtaPhiE(v.Pt() / 1000, v.Eta(), v.Phi(), v.E() / 1000);
    return result;
}

//______________________________________________________________________________________________

// original code from Tom McCarthy
// -> tmccarth<at>cern.ch / thomas.george.mccarthy<at>gmail.com
// https://indico.cern.ch/event/1020761/contributions/4318797/attachments/2225470/3774569/top_bkgd_suppression_with_2vSM_13042021.pdf
std::vector<int> TwoNeutrinoScan::Run2vSMReconstruction(std::vector<TLorentzVector>& v_leptons,
    std::vector<TLorentzVector>& v_jets, TLorentzVector& nu1, TLorentzVector& nu2, const float& met,
    const float& metPhi, float& max_weight) const {

    std::vector<int> indexVector;
    TLorentzVector l1, l2, b1, b2; // l1 is l(+), l2 is l(-), see output indexVector being filled in RunZReconstruction
    TLorentzVector this_nu1, this_nu2, top1, top2;
    float dphi_spacing(2. * TMath::Pi() / 10.); // 10 steps in phi from [0,2pi] for each step [MAKE FINER FOR BETTER
                                                // SIGNAL RECONSTRUCTION / COARSER FOR FASTER CPU TIME]
    float deta_spacing(10. / 10.);              // 10 steps in eta from [-5,5] for each step
    float met_x = met * TMath::Cos(metPhi);
    float met_y = met * TMath::Sin(metPhi);
    float this_2vSM_weight(-1.);
    float max_2vSM_weight(-1.);
    int l1_index(-1), l2_index(-1), b1_index(-1), b2_index(-1);
    int lb_association_max(1);
    int b1_index_final(-1);
    int b2_index_final(-1);

    l1 = v_leptons.at(0);
    l1_index = 0;
    l2 = v_leptons.at(1);
    l2_index = 1;

    b1 = v_jets.at(0);
    b1_index = 0;
    b2 = v_jets.at(1);
    b2_index = 1;

    float ht_ll(l1.Pt() + l2.Pt());
    int ht_ll_bin_for_dRlnu(m_h2d_veto_dR_lnu_ref->GetXaxis()->FindBin(ht_ll));
    int NbinsY_dR_lnu = m_h2d_veto_dR_lnu_ref->GetNbinsY();

    if (ht_ll_bin_for_dRlnu > m_h2d_veto_dR_lnu_ref->GetNbinsX())
        ht_ll_bin_for_dRlnu = m_h2d_veto_dR_lnu_ref->GetNbinsX();
    else if (ht_ll_bin_for_dRlnu < 1)
        ht_ll_bin_for_dRlnu = 1;

    int NbinsY_eta = m_h2d_veto_eta_bbll_ref->GetNbinsY();
    int yBin(0);

    int lb_veto(0);
    float eta_bbll(0.);
    int eta_bbll_bin(0);
    float ave_mlb_1(0.);
    float ave_mlb_2(0.);
    int hemisphere_index(0);

    eta_bbll = (b1 + b2 + l1 + l2).Eta();
    eta_bbll_bin = m_h2d_veto_eta_bbll_ref->GetXaxis()->FindBin(eta_bbll);
    ave_mlb_1 = (l1 + b1).Mag() + (l2 + b2).Mag();
    ave_mlb_2 = (l1 + b2).Mag() + (l2 + b1).Mag();
    hemisphere_index = b1.Eta() * b2.Eta() < 0 ? 1 : 0;

    if (eta_bbll_bin > m_h2d_veto_eta_bbll_ref->GetNbinsX())
        eta_bbll_bin = m_h2d_veto_eta_bbll_ref->GetNbinsX();
    else if (eta_bbll_bin < 1)
        eta_bbll_bin = 1;

    // NB: 90% EFFICIENCY IN NEXT LINE CAN BE ADJUSTED (trade-off S/B discrimination and tt reco efficiency)
    if (m_h1d_prob_mlb_vs_dmlb_ref[hemisphere_index]->Interpolate(0.5 * fabs(ave_mlb_1 - ave_mlb_2)) < 0.90)
        lb_veto = 0;
    else {
        if (ave_mlb_1 < ave_mlb_2)
            lb_veto = -1; // veto (l1b2),(l2b1) association
        else
            lb_veto = 1; // veto (l1b1),(l2b2) association
    }

    for (int ilb = 1; ilb >= -1; ilb -= 2) {
        // used to swap lb association
        // ilb = 1:   (l1b1,l2b2)
        // ilb = -1:  (l1b2,l2b1)
        if (ilb == lb_veto)
            continue; // requires either that the ave mlb or the charge info strongly disfavour that pairing (charge
                      // cutoff results in approx. 10% loss)
        for (float n1_eta = -5.; n1_eta <= 5.; n1_eta += deta_spacing) {
            yBin = m_h2d_veto_eta_bbll_ref->GetYaxis()->FindBin(n1_eta);
            if (yBin > NbinsY_eta)
                yBin -= 1;
            if (m_h2d_veto_eta_bbll_ref->GetBinContent(eta_bbll_bin, yBin) <= 0)
                continue;
            for (float n2_eta = -5.; n2_eta <= 5.; n2_eta += deta_spacing) {
                yBin = m_h2d_veto_eta_bbll_ref->GetYaxis()->FindBin(n2_eta);
                if (yBin > NbinsY_eta)
                    yBin -= 1;
                if (m_h2d_veto_eta_bbll_ref->GetBinContent(eta_bbll_bin, yBin) <= 0)
                    continue;
                for (float n1_phi = -TMath::Pi(); n1_phi < TMath::Pi(); n1_phi += dphi_spacing) {
                    this_nu1 = ReturnNeutrino4Vector(l1, n1_eta, n1_phi);
                    yBin = m_h2d_veto_dR_lnu_ref->GetYaxis()->FindBin(l1.DeltaR(this_nu1));
                    if (yBin > NbinsY_dR_lnu)
                        yBin -= 1;
                    if (m_h2d_veto_dR_lnu_ref->GetBinContent(ht_ll_bin_for_dRlnu, yBin) <= 0)
                        continue;
                    for (float n2_phi = -TMath::Pi(); n2_phi < TMath::Pi(); n2_phi += dphi_spacing) {
                        this_nu2 = ReturnNeutrino4Vector(l2, n2_eta, n2_phi);
                        yBin = m_h2d_veto_dR_lnu_ref->GetYaxis()->FindBin(l2.DeltaR(this_nu2));
                        if (yBin > NbinsY_dR_lnu)
                            yBin -= 1;
                        if (m_h2d_veto_dR_lnu_ref->GetBinContent(ht_ll_bin_for_dRlnu, yBin) <= 0)
                            continue;

                        if (ilb == 1) {
                            top1 = l1 + b1 + this_nu1;
                            top2 = l2 + b2 + this_nu2;
                        } else {
                            top1 = l1 + b2 + this_nu1;
                            top2 = l2 + b1 + this_nu2;
                        }
                        this_2vSM_weight = m_h1d_mtlep_ref->Interpolate(top1.Mag())
                            * m_h1d_mtlep_ref->Interpolate(top2.Mag()); // 2 x m(blv) terms
                        // 2 x met terms (dEx,dEy), NB: has additional factor 2 to weaken the impact of the dExy terms
                        // in the product (improved both S/B and signal efficiency using this based on a dedicated
                        // study)
                        this_2vSM_weight *= m_h1d_dExy_ref->Interpolate((this_nu1.Px() + this_nu2.Px() - met_x) / 2.);
                        this_2vSM_weight *= m_h1d_dExy_ref->Interpolate((this_nu1.Py() + this_nu2.Py() - met_y) / 2.);
                        if (this_2vSM_weight > max_2vSM_weight) {
                            max_2vSM_weight = this_2vSM_weight;
                            lb_association_max = ilb;
                            nu1 = this_nu1;
                            nu2 = this_nu2;
                            if (lb_association_max == 1) {
                                b1_index_final = b1_index;
                                b2_index_final = b2_index;
                            } else {
                                b1_index_final = b2_index;
                                b2_index_final = b1_index;
                            }
                        }
                    }
                }
            }
        }
    }

    max_weight = max_2vSM_weight;

    indexVector.push_back(l1_index);
    indexVector.push_back(b1_index_final);
    indexVector.push_back(l2_index);
    indexVector.push_back(b2_index_final);

    return indexVector; // has format 0: lepton index for l(+), 1: jet index for jet associated to b-quark, 2: lepton
                        // index for l(-), 3: jet index for jet associated to bbar-quark
}

//______________________________________________________________________________________________

TLorentzVector TwoNeutrinoScan::ReturnNeutrino4Vector(
    const TLorentzVector& lepton, const float& eta, const float& phi) const {
    // function returns the full 4-vector of the neutrino, since knowing eta, phi and the constraint that m(lv) = mW is
    // sufficient to evaluate the pT
    TLorentzVector lv;
    float denom = lepton.E() * TMath::CosH(eta) - lepton.Pz() * TMath::SinH(eta) - lepton.Px() * TMath::Cos(phi)
        - lepton.Py() * TMath::Sin(phi);
    float pT = denom > 0 ? 0.5 * (pow(80.385, 2) - pow(lepton.Mag(), 2)) / denom : 0.;
    lv.SetPtEtaPhiM(pT, eta, phi, 0.);
    return lv;
}
