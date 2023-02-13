from QFramework import INFO, TQObservable
from CAFbbll import CombinedQuantities


def addObservables(config):
    modes = [
        "MT:lep0_MET",
        "MT:lep1_MET",
        "MT:bjet0_MET",
        "MT:bjet1_MET",
        "MCT",
        "MCOLL:m",
        "MCOLL:x0",
        "MCOLL:x1",
        "DELTAR_BL_MIN",
        "DELTAR_BL_MAX",
        "DELTAR:lep0_jclosest",
        "DELTAR:lep1_jclosest",
        "DELTAETA:lep0_jclosest",
        "DELTAETA:lep1_jclosest",
        "DELTAPHI:lep0_jclosest",
        "DELTAPHI:lep1_jclosest",
        "M_BBLL",
        "Pt_BBLL_Scalar",
        "Pt_BBLL_Vector",
        "M_BBLLMET",
        "Pt_BBLLMET_Scalar",
        "Pt_BBLLMET_Vector",
        "M_HH",
        "DPHI_HH",
        "MET_PHI_CENTRALITY",
        "MET:rel",
        "DPHI_MET_LL",
        "DPHI_MET_LL_BB",
        "DPT_MET_LL",
        "DPT_MET_LL_BB",
        "SUM_MET_PTLL",
        "HT2",
        "HT2R",
        "HT",
        "SPHERICITY",
        "MT2_BB",
        "VBF_MAX_MJJ",
        "VBF_MAX_DELTA_ETA_JJ",
        "TRUTH_MLL",
        "TRUTH_MBB",
        "TRUTH_MET",
        "TRUTH_MLLMET",
    ]
    jet_collection = config.getTagStringDefault("jets.collection", "EMTopo").Data()
    for obs_name in modes:
        obs = CombinedQuantities(obs_name)
        obs.set_met_collection(jet_collection)
        if not TQObservable.addObservable(obs, obs_name):
            INFO("failed to add '{}' Observable".format(obs_name))
            return False
    return True
