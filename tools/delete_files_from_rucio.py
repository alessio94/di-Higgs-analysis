#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import argparse
import logging
import shlex
import subprocess


def bashCommand(cmd, combine_error=False):
    """Execute a bash command, returns the stdout output.

    Arguments:
        cmd: The command to execute.

    Returns:
        string: Output of the command.
    """
    command_tokenized = shlex.split(cmd)
    if not combine_error:
        output, error = subprocess.Popen(
            command_tokenized, stdout=subprocess.PIPE, stderr=subprocess.PIPE
        ).communicate()
        return output, error
    else:
        output = subprocess.Popen(command_tokenized, stdout=subprocess.PIPE, stderr=subprocess.STDOUT).communicate()


def checkRucio():
    """Check if rucio is set up."""
    _, error = bashCommand("which rucio")
    if "no rucio in" in error:
        logging.error("Could not detect rucio in the path. Please set it up with 'lsetup rucio'!")
        return False
    return True


def checkProxy():
    _, error = bashCommand("voms-proxy-info")
    if "Proxy not found" in error:
        logging.error("Could not detect valid proxy. Please set it up with 'voms-proxy-init -voms atlas'!")
        return False
    return True


def delete_datasets(datasets):
    """Delete a list of datasets from rucio."""
    logging.info("Deleting %d datasets", len(datasets))
    rucio_command = "rucio erase {}".format(" ".join(datasets))
    output = bashCommand(rucio_command, combine_error=True)
    if output:
        print(output)


def main(args):
    dataset_list = list()
    with open(args.datasets) as dataset_list_file:
        for line in dataset_list_file:
            line = line.strip()
            if line.startswith("#") or not line:  # ignore comments and empty lines
                continue
            else:
                dataset_list.append(line)
    delete_datasets(dataset_list)


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser(description="Delete files from rucio")
    PARSER.add_argument("datasets", metavar="DATASETS", type=str, help="A list of datasets to delete")
    ARGS = PARSER.parse_args()

    if not checkRucio() or not checkProxy():
        sys.exit(1)

    main(ARGS)
