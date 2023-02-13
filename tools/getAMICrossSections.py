#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import copy
import difflib
import os
import sys

from CommonAnalysisHelpers import common

try:
    import pyAMI.client
    import pyAMI.atlas.api as AtlasAPI
except ImportError:
    print("Could not import pyAMI client. Please call 'lsetup pyami'")
    sys.exit(1)


def parsekFactorFile(kfactorFile_abspath):
    kfactors = dict()
    if not os.path.isfile(kfactorFile_abspath):
        QFramework.WARN("The k-factor file '%s' is not existing!")
        return kfactors
    QFramework.INFO("Parsing kfactor file")
    with open(kfactorFile_abspath) as kFactorFile:
        for line in kFactorFile:
            line = line.strip()
            if line.startswith("#") or not line:
                continue
            contents = [x for x in line.split(" ") if x]
            if len(contents) != 2:
                QFramework.WARN("Could not parse the line '{}'".format(line))
                continue
            dsid, kfactor = contents
            kfactors[dsid] = kfactor
    return kfactors


def parseOverwriteFile(overwriteFile_abspath):
    overwrites = dict()
    if not os.path.isfile(overwriteFile_abspath):
        QFramework.WARN("The custom overwrite file '%s' is not existing!")
        return dict()
    QFramework.INFO("Parsing custom overwrites file")
    with open(overwriteFile_abspath) as overwriteFile:
        for line in overwriteFile:
            line = line.strip()
            if line.startswith("#") or not line:
                continue
            contents = [x for x in line.split(" ") if x]
            if len(contents) != 4:
                QFramework.WARN("Could not parse the line '{}'".format(line))
                continue
            dsid, xsection, kfactor, filtereff = contents
            overwrites[dsid] = dict()
            overwrites[dsid]["crossSection"] = xsection
            overwrites[dsid]["kfactor"] = kfactor
            overwrites[dsid]["genFiltEff"] = filtereff
    return overwrites


def createIdentifier(output_member):
    if output_member["type"] != "sample":
        ValueError("Only sample type allowed for identifier creation!")
    sample_name = output_member["sample_tag"]
    identifier = str(output_member["id"])
    # sort suffix
    # it's important to sort all suffix according to there position in sample name
    suffix_index = [sample_name.find(suffix) for suffix in output_member["suffix"]]
    sort_index = sorted(range(len(suffix_index)), key=lambda k: suffix_index[k])
    # create identifier
    for index in sort_index:
        identifier += "*{}*".format(output_member["suffix"][index])
    return identifier


def getMinimizedSuffix(suffix, compared_sample_name):
    if len(suffix) < 2 or len(suffix) >= len(compared_sample_name):
        return suffix
    for i in range(len(suffix)):
        if not suffix[:i] in compared_sample_name:
            return suffix[:i]


def updateDuplicatedMember(output_collections, new_member, line_format, minimize_suffix=True):
    """Checks duplicated member in output_collections and returns updated output_collections.

    If two different member have same DSID, extra suffix will be added for filtering.

    """
    # check duplicated member
    no_duplication = True
    if new_member["type"] != "sample":
        no_duplication = True
    else:
        duplicated_entries = []
        checked_identifier = createIdentifier(new_member)
        for i, member in enumerate(output_collections):
            if member["type"] == "sample":
                identifier = createIdentifier(member)
                if identifier == checked_identifier:
                    no_duplication = False
                    duplicated_entries.append(i)

    # if not duplicated, directly add new member
    if no_duplication:
        output_collections.append(new_member)
    # if duplicated, add proper suffix and updated all members with same DSID
    else:
        new_sample_name = new_member["sample_tag"]
        new_sample_name = "_".join(new_sample_name.split("_")[:-1])  # remove p-tag, as they can be different
        for i in duplicated_entries:
            compared_member = output_collections[i]
            compared_sample_name = compared_member["sample_tag"]
            compared_sample_name = "_".join(
                compared_sample_name.split("_")[:-1]
            )  # remove p-tag, as they can be different
            # updates new member suffix
            diff_obj = difflib.ndiff(compared_sample_name, new_sample_name)
            suffix_list = []
            new_suffix = False
            new_suffix_content = ""
            for ele in diff_obj:
                if ele.startswith("+ "):
                    new_suffix = True
                    new_suffix_content += ele[2:]
                else:
                    if new_suffix:
                        new_suffix = False
                        if "_" + new_suffix_content in new_sample_name:
                            new_suffix_content = "_" + new_suffix_content
                        if minimize_suffix:
                            new_suffix_content = getMinimizedSuffix(new_suffix_content, compared_sample_name)
                        suffix_list.append(new_suffix_content)
                        new_suffix_content = ""
            new_suffix_list = copy.deepcopy(new_member["suffix"])
            for suffix in suffix_list:
                if suffix not in new_member["suffix"]:
                    new_suffix_list.append(suffix)
            new_member["suffix"] = new_suffix_list
            meta_info = new_member["meta"]
            meta_info[0] = createIdentifier(new_member)
            new_member["content"] = line_format.format(*meta_info)
            # also updates old duplicated mem suffix
            diff_obj = difflib.ndiff(compared_sample_name, new_sample_name)
            suffix_list = []
            new_suffix = False
            new_suffix_content = ""
            for ele in diff_obj:
                if ele.startswith("- "):
                    new_suffix = True
                    new_suffix_content += ele[2:]
                else:
                    if new_suffix:
                        new_suffix = False
                        if "_" + new_suffix_content in compared_sample_name:
                            new_suffix_content = "_" + new_suffix_content
                        if minimize_suffix:
                            new_suffix_content = getMinimizedSuffix(new_suffix_content, new_sample_name)
                        suffix_list.append(new_suffix_content)
                        new_suffix_content = ""
            new_suffix_list = copy.deepcopy(compared_member["suffix"])
            for suffix in suffix_list:
                if suffix not in output_collections[i]["suffix"]:
                    new_suffix_list.append(suffix)
            output_collections[i]["suffix"] = new_suffix_list
            meta_info = output_collections[i]["meta"]
            meta_info[0] = createIdentifier(output_collections[i])
            output_collections[i]["content"] = line_format.format(*meta_info)
        output_collections.append(new_member)


def main(config):
    client = pyAMI.client.Client("atlas")
    AtlasAPI.init()

    sampleList_relpath = config.getTagStringDefault("derivationsMC", "")
    sampleList_abspath = common.findConfigPath(sampleList_relpath)

    kfactorFile_relpath = config.getTagStringDefault("kFactorFile", "")
    if kfactorFile_relpath:
        kfactorFile_abspath = common.findConfigPath(kfactorFile_relpath)
        kfactors = parsekFactorFile(kfactorFile_abspath)
    else:
        kfactors = dict()

    overwriteFile_relpath = config.getTagStringDefault("overwriteFile", "")
    if overwriteFile_relpath:
        overwriteFile_abspath = common.findConfigPath(overwriteFile_relpath)
        overwrites = parseOverwriteFile(overwriteFile_abspath)
    else:
        overwrites = dict()

    info_keys = [
        "datasetNumber",
        "physicsShort",
        "crossSection",
        "kfactor",
        "genFiltEff",
        "generatorName",
        "conditionsTag",
    ]

    output_collections = list()
    comment_id = 0
    error_id = 0
    line_format = "{:<8}, {:<60}, {:<10}, {:<8}, {:<10}, {:<40}, {}"
    new_content = line_format.format(
        "SampleID",
        "process",
        "xsection(nb)",
        "kfactor",
        "filtereff",
        "generator",
        "simulation",
    )
    new_member = {"id": "header", "type": "header", "content": new_content}
    output_collections.append(new_member)
    with open(sampleList_abspath) as sampleFile:
        for line in sampleFile:
            line = line.strip()
            # copy comments and empty lines to output file
            if line.startswith("#") or not line:
                new_member = {
                    "id": "comment_{}".format(comment_id),
                    "type": "comment",
                    "content": line,
                }
                comment_id += 1
                output_collections.append(new_member)
            else:
                QFramework.INFO("Getting info for '{}'".format(line))
                try:
                    ami_info = AtlasAPI.get_dataset_info(client, line)
                    if len(ami_info) < 1:
                        QFramework.WARN("Could not find info for dataset '{}'".format(line))
                    ami_info = ami_info[0]
                    dsid = ami_info["datasetNumber"]
                    selected_info = list()
                    for info_key in info_keys:
                        if dsid in overwrites and info_key in [
                            "crossSection",
                            "kfactor",
                            "genFiltEff",
                        ]:
                            selected_info.append(overwrites[dsid][info_key])
                        else:
                            if info_key == "kfactor":
                                # get kfactor from list, default to 1 if no kfactor defined
                                selected_info.append(kfactors.get(dsid, "1"))
                            else:
                                selected_info.append(ami_info[info_key])
                    new_member = {
                        "id": selected_info[0],
                        "type": "sample",
                        "content": line_format.format(*selected_info),
                        "meta": selected_info,
                        "suffix": [],
                        "sample_tag": line.split("SUSY2.")[-1],
                    }
                    # output_collections.append(new_member)
                    updateDuplicatedMember(output_collections, new_member, line_format)
                except Exception as e:
                    QFramework.WARN("There was an error getting the info for dataset '{}': {}".format(line, e))
                    new_member = {
                        "id": "error_{}".format(error_id),
                        "type": "error",
                        "content": "# TODO: " + line,
                    }
                    error_id += 1
                    output_collections.append(new_member)

    xsec_file_path = config.getTagStringDefault("XsecFile", "XS.csv").Data()
    with open(xsec_file_path, "w") as xsec_file:
        for member in output_collections:
            xsec_file.write(member["content"] + "\n")

    QFramework.INFO("Wrote cross-section information to '{}'".format(xsec_file_path))


if __name__ == "__main__":
    parser = common.createBasicArgumentParser("create a cross-section file based on information from AMI")

    import QFramework
    import ROOT

    config = common.getConfigOptions(parser.parse_args(), "grid")
    ROOT.PyConfig.IgnoreCommandLineOptions = True
    main(config)
