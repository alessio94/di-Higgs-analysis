from QFramework import INFO, TQObservable
from ROOT import TString
from CAFbbll import BBLLContainerRetrieval, JvtSF
from CAFbbll.bbll_analyze import build_weight_sys_observable_names


def addObservables(config):
    INFO("Adding JvtSF observable")

    weight_sys_selection = lambda x: BBLLContainerRetrieval.is_jet_systematic(x)
    obs_names = build_weight_sys_observable_names("JvtSF", weight_sys_selection, config)

    for name in obs_names:
        obs = JvtSF(TString(name))
        if not TQObservable.addObservable(obs):
            return False
    return True
