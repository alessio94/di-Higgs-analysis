#!/bin/env python

import glob
import numpy as np
import os

from SubmissionHelpers import submit
from SubmissionHelpers.task import task

PATTERNS = [
    "sig_X_X_nonres_bbWW",
    "sig_X_X_nonres_bbtautau",
    "sig_X_X_nonres_bbZZ",
    "sys_X_X_sig_nonres_herwig7_bbWW",
    "sys_X_X_sig_nonres_herwig7_bbtautau",
    "sys_X_X_sig_nonres_herwig7_bbZZ",
]


def create_tasks(args, setup, process_pattern, output_dir):

    ntuple_pattern = "{ntuple_dir}/*{process_pattern}*root".format(
        ntuple_dir=args.ntuple_dir, process_pattern=process_pattern
    )

    tasks = list()
    for ntuple_file_path in sorted(glob.glob(ntuple_pattern)):
        ntuple_file_path_full = os.path.abspath(ntuple_file_path)
        ntuple_file_name = os.path.basename(ntuple_file_path).replace(".root", "")

        kl_min = -15
        kl_max = 20
        kl_stepsize = 0.2
        kl_num = kl_min
        last_task = None
        while kl_num < kl_max:
            kl_str = "{:.1f}".format(kl_num)
            kl_format = kl_str.replace("-", "n").replace(".", "p")

            if kl_format == "1p0":
                kl_num += kl_stepsize
                continue

            # fix zero being -0.0
            if kl_format == "n0p0":
                kl_num = 0
                kl_str = "0.0"
                kl_format = "0p0"

            task_id = "postprocess_kl_{}_{}".format(ntuple_file_name, kl_format)
            task_log = "logs/postprocessing_kl/{}.log".format(task_id)

            output_file = os.path.join(output_dir, task_id + ".isDone")

            postprocess_command = "scripts/systematics/postprocessing_kl_singlejob.sh {} {} {}".format(
                ntuple_file_path_full, output_file, kl_format
            )

            command_list = [
                "mkdir -p " + output_dir,
                'echo "{}"'.format(postprocess_command),
                "time {}".format(postprocess_command),
            ]

            if last_task:
                dependencies = [last_task]
            else:
                dependencies = None

            this_task = task(
                task_id,
                command_list,
                args=args,
                time=args.time,
                memory=args.memory,
                outputs=[output_file],
                setup=setup,
                logFile=task_log,
                errFile=task_log,
                dependencies=dependencies,
            )
            tasks.append(this_task)
            last_task = this_task
            kl_num += kl_stepsize

    return tasks


def main(args):
    output_dir = "batchOutput/postprocessing_kl/" + args.ntuple_dir.split("/")[-1]
    tasks = []

    # get commands for setting up environment
    setup = submit.getSetupCommand(args)
    setupPath = os.environ["CAFANALYSISSETUP"]
    if setupPath:
        setup.append("source " + setupPath)

        for process_pattern in PATTERNS:
            tasks += create_tasks(args, setup, process_pattern, output_dir)

    ctrl = submit.guessSubmissionController()
    ctrl.submitTasks(args, tasks)
    print("Done")


if __name__ == "__main__":
    PARSER = submit.MinimalArgumentParser()
    PARSER.add_argument(
        "--ntuple-dir", type=str, default="data/ntup/unmerged/systematics", help="Folder which contains the ntuples"
    )
    PARSER.set_defaults(memory=1024 * 4, time=120)

    import ROOT

    # ignore command line arguments since ROOT is very greedy here (and tends to choke from it!)
    ROOT.PyConfig.IgnoreCommandLineOptions = True

    # use the argument parser to read the command line arguments and config options from the config file
    # from os.path import splitext
    ARGS = PARSER.parse_args()

    # call the main function
    main(ARGS)
