#ifndef __TWONEUTRINOSCAN__
#define __TWONEUTRINOSCAN__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

#include "TFile.h"
#include "TH2F.h"

class TwoNeutrinoScan
    : public BBLLContainerRetrieval
    , public TQEventObservable {
private:
    enum TwoNeutrinoScanMode { UNKNOWN, WEIGHT };
    TwoNeutrinoScanMode m_mode = UNKNOWN;
    const double DEFAULT_VALUE = -1e10;

    TFile* m_ref_file;                   //!
    TH1F* m_h1d_mtlep_ref;               //!
    TH1F* m_h1d_dExy_ref;                //!
    TH1F* m_h1d_prob_mlb_vs_dmlb_ref[2]; //!
    TH2F* m_h2d_veto_eta_bbll_ref;       //!
    TH2F* m_h2d_veto_dR_lnu_ref;         //!
    TH2F* m_h2d_veto_lb_association_ref; //!

protected:
    static TLorentzVector to_gev(const TLorentzVector& v);
    std::vector<int> Run2vSMReconstruction(std::vector<TLorentzVector>& v_leptons, std::vector<TLorentzVector>& v_jets,
        TLorentzVector& nu1, TLorentzVector& nu2, const float& met, const float& metPhi, float& max_weight) const;
    TLorentzVector ReturnNeutrino4Vector(const TLorentzVector& lepton, const float& eta, const float& phi) const;

    bool virtual initializeSelf() override;
    bool virtual finalizeSelf() override;

public:
    TwoNeutrinoScan();
    TwoNeutrinoScan(const TString& name);
    virtual ~TwoNeutrinoScan();

    virtual double getValue() const override;

    void set_met_collection(const std::string& jet_collection);

    ClassDefOverride(TwoNeutrinoScan, 1);
};
#endif // not __TWONEUTRINOSCAN__
