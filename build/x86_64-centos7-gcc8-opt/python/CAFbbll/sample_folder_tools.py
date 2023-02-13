# -*- coding: utf-8 -*-

import collections
import ctypes
import logging


SampleInfo = collections.namedtuple("SampleInfo", ["path", "dsid", "campaign", "sumw", "norm", "is_fastsim"])


def get_sumw_from_samples(base_samples, path, campaigns=None):
    if not campaigns:
        campaigns = list()

    results = dict()
    for sample_folder in sorted(base_samples.getListOfSampleFolders(path)):
        logging.debug("Getting samples from %s", sample_folder.getPath())
        # get DSID level sample folders
        dsid_folders = sample_folder.getListOfSamples()
        dsid_folders_filtered = [s for s in dsid_folders if "CxAOD.root" not in s.getPath().Data()]
        # loop over contents of each DSID
        for dsid_folder in sorted(dsid_folders_filtered):
            # if multisample the sum-of-weight is stored in tag ".init.sumOfWeightsTotal"
            # if single sample the sum-of-weight is stored in tag ".init.sumOfWeights"
            sumw = dsid_folder.getTagDefault(
                ".init.sumOfWeightsTotal", dsid_folder.getTagDefault(".init.sumOfWeights", 0.0)
            )
            is_fastsim = dsid_folder.getTagBoolDefault("~isAFII", False)
            path = dsid_folder.getPath().Data()
            campaign = dsid_folder.getTagStandardStringDefault("campaign", "none")
            dsid = ctypes.c_int(0)
            dsid_folder.getTagInteger("dsid", dsid)
            dsid_string = str(dsid.value)
            norm = dsid_folder.getNormalisation()
            sample_info = SampleInfo(path, dsid_string, campaign, sumw, norm, is_fastsim)
            if dsid_string not in results:
                results[dsid_string] = dict()
            results[dsid_string][campaign] = sample_info
            logging.debug(sample_info)

    # calulate total sum-of-weights from all campaigns
    for dsid in sorted(results.keys()):
        total_sumw = 0
        for campaign in campaigns:
            try:  # it can happen that a sample is not there for all campaigns
                total_sumw += results[dsid][campaign].sumw
            except KeyError:
                logging.warning("Could not find sum-of-weights for dsid '%s' and campaign '%s'", dsid, campaign)
                pass
        results[dsid]["all"] = total_sumw

    return results


def split_sample_path(path, level=0):
    """Split sample folder paths with + into single contributions

    Example: split_sample_path('/A/[B+C]+/D/[E+F]/[G+H]') will return the following list:
             ['/A/B', '/A/C', '/D/E/G', '/D/E/H', '/D/F/G', '/D/F/H']
    """
    indend = "    " * level
    logging.debug(indend + "Splitting '%s'", path)
    # first divide top-level additions (i.e. "A+B")
    bracket_count = 0
    objects = list()
    last_object_start = 0
    for i, char in enumerate(path):
        if char == "[":
            bracket_count += 1
        if char == "]":
            bracket_count -= 1

        if char == "+" and bracket_count == 0:
            objects.append(path[last_object_start:i])
            last_object_start = i + 1
    # add last object
    objects.append(path[last_object_start:])
    logging.debug(indend + "Found objects: %s", objects)

    # split objects by first [... + ...]
    items = list()
    for obj in objects:
        if "[" not in obj:
            logging.debug(indend + "No need to further split object '%s'", obj)
            items.append(obj)
        else:
            logging.debug(indend + "Splitting object '%s'", obj)
            bracket_count = 0
            found_bracket = False
            start_idx = -1
            for i, char in enumerate(obj):
                if char == "[":
                    if not found_bracket:
                        start_idx = i
                        found_bracket = True
                        logging.debug(indend + "Found opening bracket at position %d", start_idx)
                    bracket_count += 1
                if char == "]":
                    bracket_count -= 1
                # got first "[" and matching "]"
                if found_bracket and bracket_count == 0:
                    stop_idx = i
                    logging.debug(indend + "Found closing bracket at position %d", stop_idx)
                    before = obj[:start_idx]
                    after = obj[stop_idx + 1 :]
                    inside = obj[start_idx + 1 : stop_idx]
                    logging.debug(indend + "Before: '%s'", before)
                    logging.debug(indend + "Inside: '%s'", inside)
                    logging.debug(indend + "After: '%s'", after)
                    for element in split_sample_path(inside, level + 1):  # split items in first [...]
                        new_path = before + element + after
                        logging.debug(indend + "Building new path '%s'", new_path)
                        items += split_sample_path(new_path, level + 1)  # split rest of path
                    break

    return items
