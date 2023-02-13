from QFramework import INFO, TQObservable
from ROOT import TString
from CAFbbll import PUWeight
from CAFbbll.bbll_analyze import build_weight_sys_observable_names


def addObservables(config):
    INFO("Adding PUWeight observable")

    obs_names = build_weight_sys_observable_names("PUWeight", lambda x: "PRW_DATASF" in x, config)

    for name in obs_names:
        obs = PUWeight(TString(name))
        if not TQObservable.addObservable(obs):
            return False
    return True
