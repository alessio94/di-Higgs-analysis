from QFramework import INFO, TQObservable
from CAFbbll import TruthPthh


def addObservables():
    obs = TruthPthh("TruthPthh")
    if not TQObservable.addObservable(obs):
        INFO("failed to add TruthPthh observable")
        return False
    return True
