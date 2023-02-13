from QFramework import ERROR, INFO, TQObservable
from CAFbbll import DDFakeWeight

import os
import yaml


def parse_fake_config(config_file_path, dd_fake_weight):
    yaml_config = None
    with open(config_file_path, "r") as stream:
        try:
            yaml_config = yaml.safe_load(stream)
        except yaml.YAMLError as exc:
            ERROR(exc)
            return False

    for config_id, config_values in yaml_config.items():
        # validate yaml
        for key in ["variable", "binning", "values", "errors"]:
            if key not in config_values:
                ERROR("ddFake config incomplete, key '{:s}' is missing!".format(key))
                return False

        variable = config_values["variable"]
        selection = config_values.get("selection", "")
        binning = config_values["binning"]
        values = config_values["values"]
        errors = config_values["errors"]
        if "sys_errors" in config_values:
            sys_errors = config_values["sys_errors"]
        else:
            sys_errors = [0] * len(values)

        if len(binning) != len(values) + 1:
            ERROR("Inconsistent number of bin edges and weight values!")
            return False

        if len(values) != len(errors):
            ERROR("Inconsistent number weight values and errors!")
            return False

        if len(values) != len(sys_errors):
            ERROR("Inconsistent number weight values and sys_errors!")
            return False

        obs_config = dd_fake_weight.add_config(config_id, variable, selection)
        bin_edges = zip(binning[:-1], binning[1:])
        for (bin_edge_low, bin_edge_high), value, error, sys_error in zip(bin_edges, values, errors, sys_errors):
            obs_config.add_weight_bin(bin_edge_low, bin_edge_high, value, error, sys_error)

    return True


def addObservables(config):
    INFO("Adding DDFakeWeight observable")

    dd_fake_config = config.getTagStringDefault("ddFakes.config", "").Data()
    dd_fake_nbins = config.getTagIntegerDefault("ddFakes.nbins", -1)
    if not dd_fake_config or not os.path.exists(dd_fake_config):
        ERROR("You need to provide a valid ddFake config with the 'ddFakes.config' option!")
        return False
    if not dd_fake_nbins or dd_fake_nbins <= 0:
        ERROR("You need to set the number of fake bins for the FAKE_STAT uncertainties with the 'ddFake.nbins' option!")

    variations = list()
    variations.append(("Nominal", 0))
    variations += [("FAKE_STAT_UP_BIN_{}".format(i), i) for i in range(dd_fake_nbins)]
    variations += [("FAKE_STAT_DOWN_BIN_{}".format(i), i) for i in range(dd_fake_nbins)]
    variations += [("FAKE_GEN_UP", -1), ("FAKE_GEN_DOWN", -1)]
    for variation_name, variation_bin in variations:
        obs_name = "ddFakeWeight"
        if variation_name != "Nominal":
            obs_name += ":" + variation_name
        dd_fake_weight = DDFakeWeight(obs_name)
        if not parse_fake_config(dd_fake_config, dd_fake_weight):
            return False
        if variation_name != "Nominal":
            dd_fake_weight.set_variation_bin(variation_bin)

        if not TQObservable.addObservable(dd_fake_weight):
            return False
    return True
