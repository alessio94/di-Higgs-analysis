from QFramework import INFO, TQObservable
from ROOT import TString
from CAFbbll import MCWeight


def addObservables():
    INFO("Adding MCWeight observable")

    # add base obs
    obs_name = "MCWeight"
    obs = MCWeight(TString(obs_name))
    if not TQObservable.addObservable(obs):
        return False

    indices = dict()

    signal_py8_indices = list()
    signal_py8_indices += list(range(2, 8))  # scale
    signal_py8_indices += list(range(14, 46))  # pdf internal
    signal_py8_indices += [11, 12]  # pdf external
    indices["SignalPy8"] = signal_py8_indices

    signal_hw7_indices = list()
    signal_hw7_indices += list(range(40, 46))  # scale
    signal_hw7_indices += list(range(8, 40))  # pdf internal
    signal_hw7_indices += [2, 3]  # pdf external
    indices["SignalHw7"] = signal_hw7_indices

    indices["SignalVBF"] = list(range(1, 147))  # scale + PDF

    indices["SignalVBFHw7"] = list(range(1, 147))  # scale + PDF

    zll_indices = list(range(4, 11))  # scale (Zee and Zmm)
    zee_indices = list(range(11, 115))  # pdf (only Zee, NNPDF replicas, alpha_s, external sets)
    zmm_indices = list(range(11, 13))  # pdf (only Zmm, external sets)
    indices["Zee"] = zll_indices + zee_indices
    indices["Zmm"] = zll_indices + zmm_indices

    ttbar_indices = list()
    ttbar_indices += list(range(1, 7))  # scale
    ttbar_indices += [11] + list(range(115, 145))  # pdf
    ttbar_indices += [12, 13]  # alpha_s
    ttbar_indices += [193, 194, 198, 199]  # isr, fsr
    indices["TTBar"] = ttbar_indices

    stop_indices = list()
    stop_indices += list(range(1, 7))  # scale
    stop_indices += [11] + list(range(112, 142))  # pdf
    stop_indices += [9, 10]  # alpha_s
    stop_indices += [142, 143, 147, 148]  # isr, fsr
    indices["STop"] = stop_indices

    for mc_name, index_list in indices.items():
        for index in index_list:
            obs_name = "MCWeight:{}:{}".format(mc_name, index)
            obs = MCWeight(TString(obs_name))
            if not TQObservable.addObservable(obs):
                return False

    return True
