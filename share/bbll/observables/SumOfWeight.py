from QFramework import INFO, TQObservable
from ROOT import TString
from CAFbbll import SumOfWeight


def addObservables():
    INFO("Adding SumOfWeight observable")

    # add base obs
    obs_name = "SumOfWeight"
    obs = SumOfWeight(TString(obs_name))
    if not TQObservable.addObservable(obs):
        return False

    variations = dict()
    mur_muf = [("05", "05"), ("05", "10"), ("10", "05"), ("10", "20"), ("20", "10"), ("20", "20")]

    signal_py8_variations = list()
    signal_py8_variations += ["Scale_{}".format(i) for i in range(2, 8)]  # scale
    signal_py8_variations += ["PDF_{}".format(i) for i in range(14, 46)]  # pdf internal
    signal_py8_variations += ["PDF_{}".format(i) for i in [11, 12]]  # pdf external
    variations["SignalPy8"] = signal_py8_variations

    signal_py8_variations = list()
    signal_py8_variations += ["Scale_muR_{}_muF_{}".format(mur, muf) for mur, muf in mur_muf]  # scale
    signal_py8_variations += ["PDF_{}".format(90400 + i) for i in range(1, 33)]  # pdf internal
    signal_py8_variations += ["PDF_{}".format(i) for i in [13100, 25200]]  # pdf external
    variations["SignalHw7"] = signal_py8_variations

    signal_vbf_variations = list()
    signal_vbf_variations += ["Scale_{}".format(i) for i in [1, 59, 81, 92, 114, 125, 136]]  # scale
    signal_vbf_variations += ["PDF_{}".format(i) for i in range(2, 147)]  # pdf
    variations["SignalVBF"] = signal_vbf_variations

    signal_vbf_variations = list()
    signal_vbf_variations += ["Scale_{}".format(i) for i in [1, 2, 4, 143, 145, 146]]  # scale
    signal_vbf_variations += ["PDF_{}".format(i) for i in range(2, 147)]  # pdf
    variations["SignalVBFHw7"] = signal_vbf_variations

    zll_variations = ["Nominal"]
    zll_variations += ["Scale_muR_{}_muF_{}".format(mur, muf) for mur, muf in mur_muf + [("10", "10")]]  # scale
    zll_variations += ["PDF_{}".format(i) for i in [25300, 13000]]  # pdf external
    zee_variations = ["PDF_{}".format(261000 + i) for i in range(1, 101)]  # pdf internal
    zee_variations += ["PDF_{}".format(i) for i in [269000, 270000]]  # pdf alpha_s
    variations["Zee"] = zll_variations + zee_variations
    variations["Zmm"] = zll_variations

    top_variations = list()
    top_variations += ["Name_LHE3Weight_Var3cUp", "Name_LHE3Weight_Var3cDown"]  # isr
    top_variations += ["FSR_Up", "FSR_Down"]  # isr
    top_variations += ["Scale_muR_{}_muF_{}".format(mur, muf) for mur, muf in mur_muf]  # scale
    top_variations += ["PDF_{}".format(90900 + i) for i in range(0, 31)]  # pdf
    top_variations += ["PDF_{}".format(i) for i in [265000, 266000]]  # alpha_s
    variations["TTBar"] = top_variations
    variations["STop"] = top_variations

    for mc_name, variation_list in variations.items():
        for variation in variation_list:
            obs_name = "SumOfWeight:{}:{}".format(mc_name, variation)
            SumOfWeight.addWeightToInitializationList(mc_name, variation)
            obs = SumOfWeight(TString(obs_name))
            if not TQObservable.addObservable(obs):
                return False

    return True
