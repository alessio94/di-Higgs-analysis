from QFramework import INFO, TQObservable
from ROOT import TString
from CAFbbll import TTVASF


def addObservables(config):
    INFO("Adding TTVASF observable")
    obs = TTVASF(TString("TTVASF"))
    if not TQObservable.addObservable(obs):
        return False
    return True
