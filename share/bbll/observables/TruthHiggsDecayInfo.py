from QFramework import INFO, TQObservable
from ROOT import TString
from CAFbbll import TruthHiggsDecayInfo


def addObservables():
    INFO("Adding TruthHiggsDecayInfo observable")

    for higgs_index, child_index in [(0, 0), (0, 1), (1, 0), (1, 1)]:
        obs_name = "TruthHiggsDecayInfo:{}:{}".format(higgs_index, child_index)
        obs = TruthHiggsDecayInfo(TString(obs_name))
        if not TQObservable.addObservable(obs):
            return False

    return True
