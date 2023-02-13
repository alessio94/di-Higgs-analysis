import os

from QFramework import INFO, ERROR, TQObservable
from ROOT import TString
from CAFbbll import TTbarNNLOWeight


def addObservables():
    if "CAF_LIBRARIES" not in os.environ:
        ERROR("Could not fetch CAF_LIBRARIES environment variable")
        return False
    caf_lib_path = os.environ["CAF_LIBRARIES"].split(":")[-1]
    ttbar_data_path = os.path.join(caf_lib_path, "../data/TTbarNNLOReweighter")
    INFO("Adding TTbarNNLOWeight observable")
    obs = TTbarNNLOWeight(TString("TTbarNNLOWeight"))
    obs.set_data_path(ttbar_data_path)
    if not TQObservable.addObservable(obs):
        return False
    return True
