#!/bin/env python

import os
from SubmissionHelpers import submit
from SubmissionHelpers.task import task

REGIONS = ["SR1VBFVeto", "SR2", "TopCR", "ZllCR"]
PROCESSES = [
    ("sig_nonres_ggF", "sig_X_X_nonres_{bbWW,bbtautau,bbZZ}"),
    ("sig_kl10_ggF", "sig_X_X_kl10_{bbWW,bbtautau,bbZZ}"),
    ("sig_vbf", "sig_X_X_vbf_novhh_"),
    ("diboson", "bkg_X_X_prompt_diboson."),
    ("Zll", "bkg_X_X_prompt_{Zee,Zmm}"),
    ("Ztt", "bkg_X_X_prompt_Ztt"),
    ("stop", "bkg_X_X_prompt_top_stop"),
    ("ttbar", "bkg_X_X_prompt_top_ttbar_{dilepton,nonallhad,allhad}"),
    ("ttbarV", "bkg_X_X_prompt_top_ttbarV"),
    ("Wjets", "bkg_X_X_prompt_Wjets"),
    ("higgs", "bkg_X_X_prompt_Higgs"),
    ("mcfakes", "bkg_X_X_nonprompt"),
    ("ddFakes", "bkg_X_*_ddFakes_{data,mc_prompt}"),
    ("data", "data_X_"),
]


def create_task(args, output_dir, process_id, input_dir, ntup_id, process_pattern, region, setup):
    output_file = "{}/{}.root".format(output_dir, process_id)

    pattern = "{input_dir}/{ntup_id}_{proc_pattern}*{region}*root".format(
        input_dir=input_dir,
        ntup_id=ntup_id,
        proc_pattern=process_pattern,
        region=region,
    )
    check_command = (
        'if ! ls {pattern} > /dev/null 2>&1; then echo "pattern {pattern} does not match any files"; exit 1; fi'.format(
            pattern=pattern
        )
    )
    merge_command = "hadd -fk505 -O {output_file} {pattern}".format(output_file=output_file, pattern=pattern)
    command_list = [
        "mkdir -p " + output_dir,
        'echo "{}"'.format(check_command),
        "time {}".format(check_command),
        'echo "{}"'.format(merge_command),
        "time {}".format(merge_command),
    ]

    task_id = "merge_{}_{}_{}".format(ntup_id, process_id, region)
    task_log = "logs/merge_statonly/{}/{}_{}.log".format(ntup_id, process_id, region)

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
    )

    return this_task


def main(args):
    input_dir = args.ntuple_dir
    ntuple_id = input_dir.split("/")[-1]
    base_output_dir = "data/ntup/merged/" + ntuple_id
    tasks = []

    # get commands for setting up environment
    setup = submit.getSetupCommand(args)
    setupPath = os.environ["CAFANALYSISSETUP"]
    if setupPath:
        setup.append("source " + setupPath)

    for region in REGIONS:
        output_dir = os.path.join(base_output_dir, region)
        for process_id, process_pattern in PROCESSES:
            this_task = create_task(args, output_dir, process_id, input_dir, ntuple_id, process_pattern, region, setup)
            tasks.append(this_task)

    ctrl = submit.guessSubmissionController()
    ctrl.submitTasks(args, tasks)
    print("Done")


if __name__ == "__main__":
    PARSER = submit.MinimalArgumentParser()
    PARSER.add_argument(
        "--ntuple-dir", type=str, default="data/ntup/unmerged/stat-only", help="Folder which contains the ntuples"
    )
    PARSER.set_defaults(memory=512, time=60)

    import ROOT

    # ignore command line arguments since ROOT is very greedy here (and tends to choke from it!)
    ROOT.PyConfig.IgnoreCommandLineOptions = True

    # use the argument parser to read the command line arguments and config options from the config file
    # from os.path import splitext
    ARGS = PARSER.parse_args()

    # call the main function
    main(ARGS)
