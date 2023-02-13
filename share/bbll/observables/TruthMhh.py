from QFramework import INFO, TQObservable
from CAFbbll import TruthMhh


def addObservables():
    obs = TruthMhh("TruthMhh")
    if not TQObservable.addObservable(obs):
        INFO("failed to add TruthMhh observable")
        return False
    return True
