#ifndef __COMBINEDQUANTITIES__
#define __COMBINEDQUANTITIES__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class CombinedQuantities
    : public BBLLContainerRetrieval
    , public TQEventObservable {
private:
    enum CQMode {
        UNKNOWN,
        MT_LEP0_MET,
        MT_LEP1_MET,
        MT_BJET0_MET,
        MT_BJET1_MET,
        MCT,
        MCOLL,
        MCOLL_X0,
        MCOLL_X1,
        DELTAR_BL_MIN,
        DELTAR_BL_MAX,
        DELTAR_L0_CLOSESTJET,
        DELTAR_L1_CLOSESTJET,
        DELTAETA_L0_CLOSESTJET,
        DELTAETA_L1_CLOSESTJET,
        DELTAPHI_L0_CLOSESTJET,
        DELTAPHI_L1_CLOSESTJET,
        M_BBLL,
        PT_BBLL_SCALAR,
        PT_BBLL_VECTOR,
        M_BBLLMET,
        PT_BBLLMET_SCALAR,
        PT_BBLLMET_VECTOR,
        M_HH,
        DPHI_HH,
        MET_PHI_CENTRALITY,
        MET_REL,
        DPHI_MET_LL,
        DPHI_MET_LL_BB,
        DPT_MET_LL,
        DPT_MET_LL_BB,
        SUM_MET_PTLL,
        HT2,
        HT2R,
        HT,
        SPHERICITY,
        MT2_BB,
        VBF_MAX_MJJ,
        VBF_MAX_DELTA_ETA_JJ,
        TRUTH_MLL,
        TRUTH_MBB,
        TRUTH_MET,
        TRUTH_MLLMET
    };
    CQMode m_mode = UNKNOWN;
    const double DEFAULT_VALUE = -1e10;

protected:
    bool use_lepton_container = false;
    bool use_bjet_container = false;
    bool use_otherjets_container = false;
    bool use_met_container = false;
    bool use_mmc_container = false;
    bool use_truth_particle_container = false;
    bool use_truth_met_container = false;
    static bool calculate_collinear_mass(const TLorentzVector& k1, const TLorentzVector& k2, double met_etx,
        double met_ety, double& mass, Double_t& x1, Double_t& x2);
    static double calculate_sphericity(const std::vector<TLorentzVector>& vects);
    static TLorentzVector to_gev(const TLorentzVector& v);

public:
    CombinedQuantities();
    CombinedQuantities(const TString& name);
    virtual ~CombinedQuantities();

    virtual double getValue() const override;

    void set_met_collection(const std::string& jet_collection);

    ClassDefOverride(CombinedQuantities, 1);
};
#endif // not __COMBINEDQUANTITIES__
