from QFramework import INFO, TQObservable
from ROOT import TString
from CAFbbll import LeptonFakeType


def addObservables():
    INFO("Adding LeptonFakeType observables")

    for lepton_index in [0, 1]:
        obs_name = "LeptonFakeType:{0}".format(lepton_index)
        obs = LeptonFakeType(TString(obs_name))
        if not TQObservable.addObservable(obs):
            return False
    return True
