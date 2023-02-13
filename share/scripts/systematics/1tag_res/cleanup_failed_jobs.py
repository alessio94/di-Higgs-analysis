#!/usr/bin/env python

import argparse
import logging
import os

SAMPLE_FOLDER_PATH = "batchOutput"
NTUPLE_FOLDER_PATH = "data/ntup/unmerged/systematics_1tag_res"
FILE_SIZE_TRHESHOLD = 1024  # in bytes


def list_files_in_dir(path):
    return [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f))]


def delete_file(file_path):
    if os.path.isfile(file_path):
        logging.info("Deleting '%s'", file_path)
        os.remove(file_path)


def clean_up_files(ntuple_file_name, ntuple_file_path):
    # first transform ntuple file name from
    # bbll_systematics_p4_bkg_X_X_Zmm_main_bb.part163_TopCR.root
    # to sample folder file path
    # -> batchOutput/unmerged_bbll_systematics_p4/unmerged_bbll_systematics_p4_bkg_X_X_Zmm_main_bb.part163.root
    file_name_info = ntuple_file_name.split("_")
    sample_folder_prefix = "unmerged_" + "_".join(file_name_info[:3])
    sample_folder_file_name = "_".join([sample_folder_prefix] + file_name_info[3:-1]) + ".root"
    sample_folder_path = os.path.join(SAMPLE_FOLDER_PATH, sample_folder_prefix, sample_folder_file_name)
    delete_file(ntuple_file_path)
    delete_file(sample_folder_path)


def main(args):
    ntuple_dir_path = os.path.join(NTUPLE_FOLDER_PATH)
    for ntuple_file_name in list_files_in_dir(ntuple_dir_path):
        ntuple_file_path = os.path.join(ntuple_dir_path, ntuple_file_name)
        ntuple_file_size = os.path.getsize(ntuple_file_path)
        if ntuple_file_size < FILE_SIZE_TRHESHOLD:
            logging.info("Found file '%s' with size '%s'", ntuple_file_path, ntuple_file_size)
            clean_up_files(ntuple_file_name, ntuple_file_path)


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Clean up failed jobs for systematic ntuples")
    PARSER.add_argument("--debug", dest="debug", action="store_true", help="Print debug messages", default=False)
    ARGS = PARSER.parse_args()

    LOG_LEVEL = logging.DEBUG if ARGS.debug else logging.INFO
    logging.basicConfig(
        level=LOG_LEVEL, format="%(asctime)s - %(levelname)s - %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )

    main(ARGS)
