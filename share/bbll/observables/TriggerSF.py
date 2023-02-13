from QFramework import INFO, TQObservable
from ROOT import TString
from CAFbbll import TriggerSF
from CAFbbll.bbll_analyze import build_weight_sys_observable_names


def addObservables(config):
    INFO("Adding TriggerSF observable")

    obs_names = build_weight_sys_observable_names(
        "TriggerSF", lambda x: "MUON_EFF_Trig" in x or "EL_EFF_Trigger" in x, config
    )

    for name in obs_names:
        obs = TriggerSF(TString(name))
        if not TQObservable.addObservable(obs):
            return False
    return True
