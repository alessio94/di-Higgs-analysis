from QFramework import ERROR, TQObservable
from CAFbbll import RegionID


def addObservables():
    obs = RegionID("RegionID")
    if not TQObservable.addObservable(obs):
        ERROR("failed to add RegionID observable")
        return False
    return True
