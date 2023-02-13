# -*- coding: utf-8 -*-

import QFramework
import ROOT


def parseSystematicsList(version, config):
    vars_path = ROOT.TString()
    if not config.getTagString(version, vars_path):
        return list()

    vars_full_path = QFramework.TQPathManager.findFileFromEnvVar(vars_path, "CAFANALYSISSHARE")
    vars = list()
    with open(vars_full_path) as vars_file:
        for line in vars_file.readlines():
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            content = line.split(",")
            if len(content) != 2:
                QFramework.BREAK("Could not parse line '{:s} in file {:s}".format(line, vars_full_path))
            sys_name, sys_type = content
            sys_name = sys_name.strip()
            sys_type = sys_type.strip()
            if sys_type == "onesided":
                vars.append(sys_name + "__1up")
            elif sys_type == "twosided":
                vars.append(sys_name + "__1up")
                vars.append(sys_name + "__1down")
            else:
                QFramework.BREAK("Unknown systematic type {:s} for {:s}".format(sys_type, sys_name))
    return vars


def sampleFolderHasTagValueSomewhereBool(samples, tagname, value, default):
    for sample in samples.getListOfSamples():
        if sample.getTagBoolDefault(tagname, default) == value:
            return True
    return False


def prepareSystematics(config, samples):
    """Prepare the systematic handling"""

    CLI = config.getFolder("CLI+")
    # flag indicating to run a robust analysis
    robust = CLI.getTagBoolDefault("robust", False)
    # flag indicating to run a dummy analysis
    dummy = CLI.getTagBoolDefault("dummy", False)

    if not robust and not dummy:
        subfolders = samples.getListOfSampleFolders("?")
        doNominal = config.getTagBoolDefault("sysDoNominal", True)
        channels = config.getTagVStandardString("channels")
        mcasvchannels = set([c for c in channels])
        for c in channels:
            for sf in subfolders:
                if sf.getTagBoolDefault("isData", False):
                    # we're in a data SampleFolder
                    # just set some nominal tags for this channel subfolder
                    f = sf.getSampleFolder(c)
                    if not f:
                        continue
                    f.setTagString(".mcasv.channel", f.getTagStringDefault("channel", ""))
                    continue
                # we're in a MC SampleFolder
                f = sf.getSampleFolder(c)
                if not f:
                    QFramework.WARN(
                        "unable to retrieve sample folder '{:s}' from '{:s}'. ".format(c, sf.GetName())
                        + "Is this expected from your path restriction?"
                    )
                    sf.printContents()
                    # if this is not a valid folder, don't do anything with it,
                    # you'll get a null pointer exception otherwise!
                    continue

                p4_vars = parseSystematicsList("sysP4List", config)
                for p4var in p4_vars:
                    # run AFII systematics only on AFII samples
                    if "_AFII" in p4var and sampleFolderHasTagValueSomewhereBool(f, "~isAFII", False, False):
                        continue
                    if "_AF2" in p4var and sampleFolderHasTagValueSomewhereBool(f, "~isAFII", False, False):
                        continue

                    # run MC16 systematics only on fullsim samples
                    if "_MC16" in p4var and sampleFolderHasTagValueSomewhereBool(f, "~isAFII", True, False):
                        continue

                    # for each p4 varematic, copy the channel folder
                    newname = c + "_" + p4var
                    newf = f.copy(newname)
                    if not newf:
                        QFramework.BREAK("Unable to copy folder {:s} to new name {:s}".format(f.GetName(), newname))
                    sf.addFolder(newf)
                    # set the appropriate tags
                    newf.setTagString(".mcasv.channel", newname)
                    newf.setTagString("p4Variation", p4var)
                    if "JET_JERPD" in p4var:
                        newf.setTagInteger("JERWeight", -1)
                    mcasvchannels.add(newname)

                # if no nominal analysis was requested, we can remove the nominal channels
                if not doNominal:
                    f.detachFromBase()
                else:
                    f.setTagString(".mcasv.channel", f.GetName())

    # Add some nominal top level tags, even if systematics aren't being added
    samples.setTagString("p4Variation", "Nominal")

    # possibly print how the folder looks like now
    if config.getTagBoolDefault("showChannels", False):
        QFramework.INFO("After taking care of channel and systematics setup, your sample folder looks like this:")
        samples.printContents("r2dt")

    # save the whole collection of channels (including now systematics)
    # for access later when creating the MCASV
    runtime = config.getFolder("runtime+")
    for i, channel in enumerate(mcasvchannels):
        if "ROOT" in str(type(channel)):
            QFramework.WARN(
                "Please make sure that every mcasvchannel is only booked once. "
                + "The python set doesn't help when adding c++ ROOT instances ({:s}).".format(type(channel))
            )
        runtime.setTagString("mcasvchannels." + str(i), channel)

    return


def build_weight_sys_observable_names(nominal_name, selection, config):
    weight_variations = [nominal_name]
    for weight_var in parseSystematicsList("sysWeightList", config):
        if selection(weight_var):
            weight_variations.append(nominal_name + ":" + weight_var)
    return weight_variations
