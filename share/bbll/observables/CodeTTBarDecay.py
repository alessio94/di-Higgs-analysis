from QFramework import INFO, TQObservable
from ROOT import TString
from CAFbbll import CodeTTBarDecay


def addObservables():
    INFO("Adding CodeTTBarDecay observable")
    obs = CodeTTBarDecay(TString("CodeTTBarDecay"))
    if not TQObservable.addObservable(obs):
        return False
    return True
