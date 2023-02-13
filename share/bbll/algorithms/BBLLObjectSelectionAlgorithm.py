from QFramework import BREAK
from CAFbbll import BBLLObjectSelectionAlgorithm, BBLLBTaggingStudyAlgorithm, BBLLBJetCalibStudyAlgorithm
from CAFbbll.bbll_analyze import parseSystematicsList

OBJECT_SELECTION_TYPES = ["Default", "BTagStudy", "BCalibStudy"]
ELECTRON_ID_WPS = ["Medium", "Tight"]
ELECTRON_ISO_WPS = ["FixedCutLoose", "Gradient"]
MUON_ISO_WPS = ["Loose_VarRad", "PflowLoose_FixedRad", "PflowLoose_VarRad", "PflowTight_FixedRad", "PflowTight_VarRad"]


def addAlgorithms(visitor, config):
    object_selection_type = config.getTagStringDefault("object_selection", "Default").Data()
    if object_selection_type not in OBJECT_SELECTION_TYPES:
        BREAK("Object selection '{}' not supported right now!".format(object_selection_type))
    if object_selection_type == "Default":
        bbll_object_selection = BBLLObjectSelectionAlgorithm()
        bbll_object_selection.SetName("BBLLObjectSelectionAlgorithm")
    elif object_selection_type == "BTagStudy":
        bbll_object_selection = BBLLBTaggingStudyAlgorithm()
        bbll_object_selection.SetName("BBLLBTaggingStudyAlgorithm")
    elif object_selection_type == "BCalibStudy":
        bbll_object_selection = BBLLBJetCalibStudyAlgorithm()
        bbll_object_selection.SetName("BBLLBJetCalibStudyAlgorithm")

    electron_id = config.getTagStringDefault("electrons.Id", "Medium").Data()
    electron_iso = config.getTagStringDefault("electrons.Iso", "FixedCutLoose").Data()
    muon_iso = config.getTagStringDefault("muons.Iso", "PflowLoose_VarRad").Data()
    if electron_id not in ELECTRON_ID_WPS:
        BREAK("Electron ID working point '{}' is not supported right now!".format(electron_id))
    if electron_iso not in ELECTRON_ISO_WPS:
        BREAK("Electron ISO working point '{}' is not supported right now!".format(electron_iso))
    if muon_iso not in MUON_ISO_WPS:
        BREAK("Muon ISO working point '{}' is not supported right now!".format(muon_iso))

    bbll_object_selection.set_electron_id(electron_id)
    bbll_object_selection.set_electron_iso(electron_iso)
    bbll_object_selection.set_muon_iso(muon_iso)

    jet_collection = config.getTagStringDefault("jets.collection", "EMPFlow").Data()
    jet_btagging_version = config.getTagStringDefault("jets.btagging_version", "201903").Data()
    bbll_object_selection.set_jet_collection(jet_collection, jet_btagging_version)

    btagging_cdi_file = config.getTagStringDefault("jets.btagging_cdi_file", "").Data()
    if not btagging_cdi_file:
        BREAK("Please set a btagging CDI file!")
    bbll_object_selection.set_btagging_cdi_file(btagging_cdi_file)

    for weight_sys in parseSystematicsList("sysWeightList", config):
        if "MUON_EFF_Trig" in weight_sys:
            # only affects event info container
            continue
        if "FT_EFF" in weight_sys:
            # b-tagging variations are done only in CAF
            continue
        bbll_object_selection.add_variation(weight_sys)

    visitor.addAlgorithm(bbll_object_selection)
    return True
