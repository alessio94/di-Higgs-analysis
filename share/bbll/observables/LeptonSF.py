from QFramework import INFO, TQObservable
from ROOT import TString
from CAFbbll import BBLLContainerRetrieval, LeptonSF
from CAFbbll.bbll_analyze import build_weight_sys_observable_names


def weight_sys_selection(variation):
    if "EL_EFF_Trigger" in variation:
        return False

    if "MUON_EFF_Trig" in variation:
        return False

    if BBLLContainerRetrieval.is_electron_systematic(variation):
        return True

    if BBLLContainerRetrieval.is_muon_systematic(variation):
        return True

    return False


def addObservables(config):
    INFO("Adding LeptonSF observable")

    obs_names = build_weight_sys_observable_names("LeptonSF", weight_sys_selection, config)

    for name in obs_names:
        lepton_sf = LeptonSF(TString(name))

        electron_id = config.getTagStringDefault("electrons.Id", "Medium").Data()
        electron_iso = config.getTagStringDefault("electrons.Iso", "FixedCutLoose").Data()
        muon_iso = config.getTagStringDefault("muons.Iso", "PflowLoose_VarRad").Data()
        # check if the settings are correct is done in bbll/algorithms/BBLLObjectSelectionAlgorithm.py

        lepton_sf.set_electron_id(electron_id)
        lepton_sf.set_electron_iso(electron_iso, electron_id)
        lepton_sf.set_muon_iso(muon_iso)

        if not TQObservable.addObservable(lepton_sf):
            return False
    return True
