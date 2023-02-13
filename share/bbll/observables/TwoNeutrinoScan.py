from QFramework import INFO, TQObservable
from CAFbbll import TwoNeutrinoScan


def addObservables(config):
    modes = ["2vScan:weight"]
    jet_collection = config.getTagStringDefault("jets.collection", "EMTopo").Data()
    for obs_name in modes:
        obs = TwoNeutrinoScan(obs_name)
        obs.set_met_collection(jet_collection)
        if not TQObservable.addObservable(obs, obs_name):
            INFO("failed to add '{}' Observable".format(obs_name))
            return False
    return True
