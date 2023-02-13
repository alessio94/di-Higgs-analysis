import os
import argparse
from SubmissionHelpers.submit import adjustArgs
from SubmissionHelpers.task import task


def makeSmartTaskList(
    args_,
    jobFile,
    config,
    templateCommand,
    maxSampleCount=-1,
    maxSampleSize=-1,
    setup=[],
    idOffset=0,
    inputs=[],
    outputs=[],
):
    """
    This is a copy of the makeSmartTaskList function in SubmissionHelpers/python/submit.py, but with
    getPathFriendsWildcarded(True) instead of getPathFriendsWildcarded(False).
    This change fixes some weird behavior when calling getPathFriendsWildcarded(False) where some parts of the sample
    folder file path are marked as wildcard even if they are not wildcarded in the joblist and initialize step.

    Original docstring:
    -------------------
    Creates a list of tasks based on a SampleFolder (determined from config argument, expected format is a TQFolder as obtained from common.getConfigOptions) taking boundary conditions from a job definition file (joblist) into account

    The payload of the created tasks is created from the templateCommand replacing
    occurances of the substrings '{restrict}' and '{identifier}' with the SampleFolder
    path(s) the job should be restricted to and a corresponding identifier which can
    be used in file names. Example:
    templateCommand = "analyze.py flatNtuple/config/master/analyze.cfg --retrict {restrict} --options outputFile:analyzed/toMerge/samples_unmerged_{identifier}.root"

    Additional replacements that can be used:
    -'{uniqueID}': a number which is unique within the list of tasks generated during a single call (starting at idOffset (=0 unless specified as argument to this function) and incremented for each subsequent task created)
    -'{options}' : if the args namespace has an attribute 'options' this placeholder is replaced by the concatenation of the options attribute of the args namespace. The individual elements are concatenated with single spaces ' '.

    The optional arguments 'inputs' and 'outputs' are expected to be lists of input (config) files each individual job reads and output files it is expected to produce. The same replacements as in the templateCommand case are applied (to each element of the lists).

    Optionally a list of setup instructions can be provided which will be set on each individual task. Also for the elements of the setup list above mentioned replacements are applied.

    The joblist file is expected to contain one SampleFolder path per line, tasks will be created within
    the domain of that path, possibly splitting into multiple tasks (TODO: use taskArrays instead) to
    respect the maxSampleCount and maxSampleSize arguments if they are > 0. maxSampleCount refers to a maximum number of
    input files, maxSampleSize to a maximum cumulated size of input files for a single task.

    The file size of a sample is read from tags '.init.filestamp.size' and (if the first is not found) '.xsp.fileSize', defaulting to 0.

    ATTENTION: this function assumes that whereever a wildcard is used in the job definition file, the corresponding parts of the SampleFolder are also marked as wildcarded with the tag 'wildcarded' set to true!

    """
    import QFramework
    import CommonAnalysisHelpers as CAH

    sf = CAH.common.loadSampleFolder(config)
    if not sf:
        QFramework.BREAK(
            "unable to load sample folder '{:s}' to create smart job list - please check input path".format(
                inputFileName
            )
        )
    # patch sample folder so that the same sample folder in the submitted job has the same format as the sample folder while creating the task list
    CAH.common.patchSampleFolder(config.getTagVStandardString("patches"), sf)
    # make a deep-ish copy of args_ (as it might be modified further down the line)
    args = argparse.Namespace(**vars(args_))

    retList = []
    allPathsWildcarded = (
        []
    )  # list to keep track of what contribution is already used somewhere to prevent double counting/running
    joblist = []
    with open(QFramework.TQPathManager.findFileFromEnvVar(jobFile, "CAFANALYSISSHARE")) as f:
        joblist_ = f.readlines()
        joblist = [x.strip() for x in joblist_]
    for jobElement in joblist:
        # remove/skip comment lines:
        if jobElement == "" or jobElement[0] == "#":
            continue

        modline = jobElement.lstrip("!")  # remove leading '!' character
        if len(modline) < len(jobElement):  # we actually removed at least one leading '!'
            # hence, we adjust the current (local copy of the) argparse namespace
            if not args.allowArgChanges:
                continue
            (key, val) = modline.split("=", 1)
            key = key.strip()
            val = val.strip()
            # we treat the following two cases differently as they are not part of the minimal argparser
            if key == "maxSampleCount":
                maxSampleCount = int(val)
            elif key == "maxSampleSize":
                maxSampleSize = float(val)
            else:
                adjustArgs(args, modline)
        else:  # not a modline, so consider it as a job definition
            # TODO: remove
            # (label,restrict,downmerge) = jobElement
            localPaths = [[]]  # sublists will contain all paths for one subjob

            sampleFolders = sf.getListOfSampleFolders(jobElement)
            if not sampleFolders:
                QFramework.BREAK("No matching sample folders found for expression '{:s}', exiting".format(jobElement))

            allSamplesForSubjob = []
            for thisSF in sampleFolders:
                allSamplesForSubjob = allSamplesForSubjob + [
                    x for x in thisSF.getListOfSamples() if not x.hasSubSamples()
                ]  # this could, in theory, contain duplicates, we'll resovle this later by converting the list to a set
            # allSamplesForSubjob = list(set(allSamplesForSubjob)) #remove duplicates. Note: we still need another check later when dealing with wildcarded paths
            # the following lines efficiently remove duplicates whilst preserving the order. This is important since otherwise the creation of tasklists is not fully deterministic!!!
            alreadyAddedTemp = set()
            alreadyAddedTemp_add = alreadyAddedTemp.add  # saves time w.r.t. resolution for each element
            allSamplesForSubjob = [
                x for x in allSamplesForSubjob if not (x in alreadyAddedTemp or alreadyAddedTemp_add(x))
            ]
            localCount = 0
            localSize = 0.0
            for sample in allSamplesForSubjob:
                if not sample:
                    continue
                pathWildcarded = sample.getPathFriendsWildcarded(True).Data().strip()
                if pathWildcarded in allPathsWildcarded:
                    continue  # check if the path is already used somewhere and skipp it if needed to prevent double counting
                sampleSize = sample.getTagDoubleDefault(
                    ".init.filestamp.size", sample.getTagDoubleDefault(".xsp.fileSize", 0.0)
                )
                if (maxSampleSize > 0 and localSize + sampleSize > maxSampleSize) or (
                    maxSampleCount > 0 and localCount + 1 > maxSampleCount
                ):  # check if we need to create a new batch of samples or we can add it to the current one
                    if (
                        len(localPaths[-1]) > 0
                    ):  # don't do anything if this is simply a single sample already exceeding the limit
                        localPaths.append([])
                        localSize = 0.0
                        localCount = 0

                allPathsWildcarded.append(pathWildcarded)  # we are using this path now so we add it to the used paths
                localPaths[-1].append(pathWildcarded)
                localSize += sampleSize
                localCount += 1

            label = (
                args.identifier + "_" + (jobElement.strip("/").replace("/", "_").replace("?", "X").replace(",", "_"))
            )
            # combine the sample paths into --restrict arguments
            nPart = 0

            try:
                options = args.options
            except:
                options = []
            options.append("prettyPrint=false")
            options.append("lineUpdates=false")

            optionstring = " ".join(options)

            for pList in localPaths:
                if len(pList) == 0:
                    continue
                restrict = ",".join(pList)

                if not isinstance(templateCommand, list):  # turn into a list if this is not the case yet
                    templateCommand = [templateCommand]
                partLabel = label + (".part" + str(nPart) if len(localPaths) > 1 else "")
                payload = [
                    x.format(identifier=partLabel, uniqueID=idOffset, restrict=restrict, options=optionstring)
                    for x in templateCommand
                ]  # note: this also ensures we have a copy of the payload list and don't run into troubles with the mutable nature of lists!
                theseOutputs = [
                    x.format(identifier=partLabel, uniqueID=idOffset, restrict=restrict, options=optionstring)
                    for x in outputs
                ]  # note: this also ensures we have a copy of the payload list and don't run into troubles with the mutable nature of lists!
                theseInputs = [
                    x.format(identifier=partLabel, uniqueID=idOffset, restrict=restrict, options=optionstring)
                    for x in inputs
                ]  # note: this also ensures we have a copy of the payload list and don't run into troubles with the mutable nature of lists!
                thisSetup = [
                    x.format(identifier=partLabel, uniqueID=idOffset, restrict=restrict, options=optionstring)
                    for x in setup
                ]  # note: this also ensures we have a copy of the payload list and don't run into troubles with the mutable nature of lists!
                logFile = os.path.join(args.logpath, partLabel + ".log")
                thisTask = task(
                    partLabel,
                    payload,
                    setup=thisSetup,
                    args=args,
                    inputs=theseInputs,
                    outputs=theseOutputs,
                    logFile=logFile,
                    errFile=logFile,
                )
                retList.append(thisTask)
                # perform some increments
                nPart += 1  # id within this (sub)set of tasks
                idOffset += 1  # global id for full list of tasks
    return retList
