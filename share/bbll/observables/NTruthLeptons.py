from QFramework import INFO, TQObservable
from CAFbbll import NTruthLeptons


def addObservables():
    obs = NTruthLeptons("NTruthLeptons")
    if not TQObservable.addObservable(obs):
        INFO("failed to add NTruthLeptons observable")
        return False
    return True
