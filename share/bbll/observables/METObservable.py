from QFramework import INFO, TQObservable
from CAFbbll import METObservable


def addObservables(config):
    modes = [
        "sumet",
        "met",
        "metx",
        "mety",
        "metphi",
        "soft_sumet",
        "soft_met",
        "soft_metx",
        "soft_mety",
        "metrho",
        "metsig",
        "metsig_soft",
        "metsig_hard",
    ]
    jet_collection = config.getTagStringDefault("jets.collection", "EMTopo").Data()
    for mode in modes:
        obs_name = "MET:" + mode
        obs = METObservable(obs_name)
        obs.set_met_collection(jet_collection)
        if not TQObservable.addObservable(obs, obs_name):
            INFO("Failed to add {} Observable".format(obs_name))
            return False
    return True
