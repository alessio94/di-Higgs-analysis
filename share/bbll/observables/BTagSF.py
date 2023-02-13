from QFramework import ERROR, TQObservable
from CAFbbll import BTagSF
from CAFbbll.bbll_analyze import build_weight_sys_observable_names


def addObservables(config):
    object_selection_type = config.getTagStringDefault("object_selection", "Default").Data()
    taggers = ["DL1r"]
    if object_selection_type == "BTagStudy":
        working_points = ["FixedCutBEff_70", "FixedCutBEff_77", "FixedCutBEff_85"]
    else:
        working_points = ["FixedCutBEff_77"]

    modes = list()
    for tagger in taggers:
        for working_point in working_points:
            modes.append("{tagger}:{working_point}".format(tagger=tagger, working_point=working_point))

    jet_collection = config.getTagStringDefault("jets.collection", "EMPFlow").Data()
    jet_btagging_version = config.getTagStringDefault("jets.btagging_version", "201903").Data()

    btagging_cdi_file = config.getTagStringDefault("jets.btagging_cdi_file", "").Data()
    if not btagging_cdi_file:
        ERROR("Please set a btagging CDI file!")
        return False

    for mode in modes:
        obs_name = "BTagSF:" + mode
        obs_variation_names = build_weight_sys_observable_names(obs_name, lambda x: "FT_EFF" in x, config)
        for obs_name in obs_variation_names:
            obs = BTagSF(obs_name)
            obs.set_jet_collection(jet_collection, jet_btagging_version)
            obs.set_btagging_cdi_file(btagging_cdi_file)
            if not TQObservable.addObservable(obs, obs_name):
                ERROR("Failed to add '{}' observable".format(obs_name))
                return False
    return True
