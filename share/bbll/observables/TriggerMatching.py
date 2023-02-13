from QFramework import INFO, TQObservable
from ROOT import TString
from CAFbbll import TriggerMatching


def addObservables():
    INFO("Adding TriggerMatching observable")
    obs = TriggerMatching(TString("TriggerMatching"))
    if not TQObservable.addObservable(obs):
        return False
    return True
