from QFramework import INFO, TQObservable
from CAFbbll import NWTauDecays


def addObservables():
    obs = NWTauDecays("NWTauDecays")
    if not TQObservable.addObservable(obs):
        INFO("failed to add NWTauDecays observable")
        return False
    return True
