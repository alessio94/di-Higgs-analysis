#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import subprocess
import shlex
import sys
import re

from CommonAnalysisHelpers import common


SRM_TO_DCAP_MATCH = re.compile(r"srm://([\w.-]+)(?::\d*)?.*\?SFN=(.*)")
SRM_TO_DCAP_SUB = r"dcap://\1\2"


def bashCommand(cmd):
    """Execute a bash command, returns the stdout output.

    Arguments:
        cmd: The command to execute.

    Returns:
        string: Output of the command.
    """
    command_tokenized = shlex.split(cmd)
    output, error = subprocess.Popen(command_tokenized, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
    return output, error


def checkRucio():
    """Check if rucio is set up."""
    _, error = bashCommand("which rucio")
    if "no rucio in" in error:
        QFramework.ERROR("Could not detect rucio in the path. Please set it up with 'lsetup rucio'!")
        return False
    return True


def checkProxy():
    _, error = bashCommand("voms-proxy-info")
    if "Proxy not found" in error:
        QFramework.ERROR("Could not detect valid proxy. Please set it up with 'voms-proxy-init -voms atlas'!")
        return False
    return True


def getFileList(dataset, rse, protocol, isData=False, numTry=1, maxRetries=3):
    rucio_command = "rucio list-file-replicas {} --rse {} --pfn --protocol {}".format(dataset, rse, protocol)
    output, error = bashCommand(rucio_command)
    if error:
        if numTry < maxRetries:
            numTry += 1
            QFramework.WARN(
                "Timeout while getting file list for dataset '{}'. Starting retry {}/{}".format(
                    dataset, numTry, maxRetries
                )
            )
            return getFileList(dataset, rse, protocol, isData, numTry, maxRetries)
        else:
            QFramework.WARN("Timeout while getting file list for dataset '{}'. Reached maximum number of retries.")
    file_list = output.strip().split("\n")
    if len(file_list) == 1 and not file_list[0]:  # there is always an empty line
        QFramework.WARN("Found 0 files for dataset '{}'".format(dataset))
        return list()  # return empty list
    else:
        QFramework.INFO("Found {:3d} file(s) for dataset '{}'".format(len(file_list), dataset))

    for i, file_path in enumerate(file_list):
        # convert srm to dcap path
        if protocol == "srm":
            file_list[i] = SRM_TO_DCAP_MATCH.sub(SRM_TO_DCAP_SUB, file_path)
        if not isData:
            file_list[i] = file_list[i] + " #" + dataset

    return file_list


def generateFileList(tag_samplelist, tag_filelist, campaign, rse, protocol, isData=False):
    sampleList_relpath = config.getTagStringDefault(tag_samplelist + "." + campaign, "")
    sampleList_abspath = common.findConfigPath(sampleList_relpath)

    output_content = list()
    with open(sampleList_abspath) as sampleList:
        for line in sampleList:
            line = line.strip()
            if line.startswith("#") or not line:  # ignore comments and empty lines
                continue
            else:
                output_content += getFileList(line, rse, protocol, isData)

    fileList_path = config.getTagStringDefault(tag_filelist + "." + campaign, "").Data()
    with open(fileList_path, "w") as fileList:
        for line in output_content:
            fileList.write(line + "\n")
    return fileList_path, len(output_content)


def main(config):

    rse = config.getTagStringDefault("storage.endpoint")
    if not rse:
        QFramework.BREAK("Pleace specify a storage endpoint with 'storage.endpoint'")
    protocol = config.getTagStringDefault("storage.protocol", "root")
    if protocol not in ["srm", "root", "https"]:
        QFramework.BREAK("Protocol '{}' is not supported!".format(protocol))

    campaigns = list(config.getTagVStandardString("campaignList"))

    mc_results = dict()
    data_results = dict()
    for campaign in campaigns:
        mc_results[campaign] = generateFileList("mcSampleList", "mcFileList", campaign, rse, protocol)
        data_results[campaign] = generateFileList(
            "dataSampleList", "dataFileList", campaign, rse, protocol, isData=True
        )

    # print summary
    for campaign in campaigns:
        mcFileList, nfiles_mc = mc_results[campaign]
        dataFileList, nfiles_data = data_results[campaign]

        QFramework.INFO("Campaign {}".format(campaign))
        QFramework.INFO("=" * len("Campaign {}".format(campaign)))

        if mcFileList:
            QFramework.INFO("Found {} files for MC".format(nfiles_mc))
            QFramework.INFO("Wrote MC file list to '{}'".format(mcFileList))
        else:
            QFramework.WARN("Errror occured while generating MC file list")

        if dataFileList:
            QFramework.INFO("Found {} files for data".format(nfiles_data))
            QFramework.INFO("Wrote data file list to '{}'".format(dataFileList))
        else:
            QFramework.WARN("Errror occured while generating data file list")
        QFramework.INFO("")


if __name__ == "__main__":
    parser = common.createBasicArgumentParser("Create a list of files stored on a grid storage")

    import QFramework
    import ROOT

    if not checkRucio() or not checkProxy():
        sys.exit(1)

    config = common.getConfigOptions(parser.parse_args(), "grid")
    ROOT.PyConfig.IgnoreCommandLineOptions = True
    main(config)
