#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import argparse
import logging
import shlex
import subprocess
import sys

try:
    import pyAMI.client
    import pyAMI.atlas.api as AtlasAPI
except ImportError:
    print("Could not import pyAMI client. Please call 'lsetup pyami'")
    sys.exit(1)


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


def checkProxy():
    _, error = bashCommand("voms-proxy-info")
    if "Proxy not found" in error:
        logging.error("Could not detect valid proxy. Please set it up with 'voms-proxy-init -voms atlas'!")
        return False
    return True


def read_sample_list(sample_list_path):
    samples = list()
    with open(sample_list_path) as sample_list:
        for line in sample_list:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            samples.append(line)
    return samples


def get_ami_info(client, sample):
    logging.info("Getting AMI info for dataset '%s'", sample)
    info_keys = ["datasetNumber", "genFiltEff", "physicsShort", "generatorName"]

    try:
        ami_info = AtlasAPI.get_dataset_info(client, sample)
        if len(ami_info) < 1:
            logging.warning("Could not find info for dataset '%s'", format(sample))
        ami_info = ami_info[0]

        selected_info = list()
        for info_key in info_keys:
            selected_info.append(ami_info[info_key])

        return selected_info

    except Exception as e:
        logging.warning("There was an error getting the info for dataset '%s': %s", sample, e)


def main(args):
    client = pyAMI.client.Client("atlas")
    AtlasAPI.init()

    sample_list = read_sample_list(args.input_file)

    line_format = "{:<8}, {:<10}, {:<60}, {:<60}"
    header = line_format.format("SampleID", "FilterEff", "Process", "Generator")
    output = list()
    output.append(header)
    for sample in sample_list:
        ami_info = get_ami_info(client, sample)
        if ami_info:
            output.append(line_format.format(*ami_info))

    with open(args.output_file, "w") as output_file:
        for line in output:
            print(line)
            output_file.write(line + "\n")


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser(description="Create cutflow table with fake breakdown")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input file with samples")
    PARSER.add_argument(
        "--output-file", type=str, default="gen_filter_eff.txt", help="Output file with generator filter efficiency"
    )
    PARSER.add_argument("--debug", dest="debug", action="store_true", help="Print debug messages", default=False)
    ARGS = PARSER.parse_args()

    LOG_LEVEL = logging.DEBUG if ARGS.debug else logging.INFO
    logging.basicConfig(
        level=LOG_LEVEL, format="%(asctime)s - %(levelname)s - %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )

    if not checkProxy():
        sys.exit(1)

    main(ARGS)
