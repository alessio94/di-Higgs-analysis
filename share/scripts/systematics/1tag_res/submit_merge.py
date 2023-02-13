#!/bin/env python

import os
from SubmissionHelpers import submit
from SubmissionHelpers.task import task

REGIONS = ["2tagSR1", "2tagSR2", "2tagTopCR", "2tagZllCR", "1tagSR1", "1tagSR2", "1tagTopCR"]
PROCESSES = [
    ("sig_nonres_bbww", "sig_X_X_nonres_bbWW"),
    ("sig_nonres_bbtautau", "sig_X_X_nonres_bbtautau"),
    ("sig_nonres_bbzz", "sig_X_X_nonres_bbZZ"),
    ("sig_nonres_herwig7_bbww", "sys_X_X_sig_nonres_herwig7_bbWW"),
    ("sig_nonres_herwig7_bbtautau", "sys_X_X_sig_nonres_herwig7_bbtautau"),
    ("sig_nonres_herwig7_bbzz", "sys_X_X_sig_nonres_herwig7_bbZZ"),
    ("sig_kl10_bbww", "sig_X_X_kl10_bbWW"),
    ("sig_kl10_bbtautau", "sig_X_X_kl10_bbtautau"),
    ("sig_kl10_bbzz", "sig_X_X_kl10_bbZZ"),
    ("sig_kl10_herwig7_bbww", "sys_X_X_sig_kl10_herwig7_bbWW"),
    ("sig_kl10_herwig7_bbtautau", "sys_X_X_sig_kl10_herwig7_bbtautau"),
    ("sig_kl10_herwig7_bbzz", "sys_X_X_sig_kl10_herwig7_bbZZ"),
    ("sig_res_251_bbww", "sig_X_X_res_251_bbWW"),
    ("sig_res_260_bbww", "sig_X_X_res_260_bbWW"),
    ("sig_res_300_bbww", "sig_X_X_res_300_bbWW"),
    ("sig_res_400_bbww", "sig_X_X_res_400_bbWW"),
    ("sig_res_500_bbww", "sig_X_X_res_500_bbWW"),
    ("sig_res_600_bbww", "sig_X_X_res_600_bbWW"),
    ("sig_res_800_bbww", "sig_X_X_res_800_bbWW"),
    ("sig_res_1000_bbww", "sig_X_X_res_1000_bbWW"),
    ("sig_res_251_bbzz", "sig_X_X_res_251_bbZZ"),
    ("sig_res_260_bbzz", "sig_X_X_res_260_bbZZ"),
    ("sig_res_300_bbzz", "sig_X_X_res_300_bbZZ"),
    ("sig_res_400_bbzz", "sig_X_X_res_400_bbZZ"),
    ("sig_res_500_bbzz", "sig_X_X_res_500_bbZZ"),
    ("sig_res_600_bbzz", "sig_X_X_res_600_bbZZ"),
    ("sig_res_800_bbzz", "sig_X_X_res_800_bbZZ"),
    ("sig_res_1000_bbzz", "sig_X_X_res_1000_bbZZ"),
    ("sig_res_251_bbtautau", "sig_X_X_res_251_bbtautau"),
    ("sig_res_260_bbtautau", "sig_X_X_res_260_bbtautau"),
    ("sig_res_300_bbtautau", "sig_X_X_res_300_bbtautau"),
    ("sig_res_400_bbtautau", "sig_X_X_res_400_bbtautau"),
    ("sig_res_500_bbtautau", "sig_X_X_res_500_bbtautau"),
    ("sig_res_600_bbtautau", "sig_X_X_res_600_bbtautau"),
    ("sig_res_800_bbtautau", "sig_X_X_res_800_bbtautau"),
    ("sig_res_1000_bbtautau", "sig_X_X_res_1000_bbtautau"),
    ("sig_vbf_bbww", "sig_X_X_vbf_novhh_l1cvv1cv1_bbWW"),
    ("sig_vbf_bbzz", "sig_X_X_vbf_novhh_l1cvv1cv1_bbZZ"),
    ("sig_vbf_bbtautau", "sig_X_X_vbf_novhh_l1cvv1cv1_bbtautau"),
    ("diboson", "bkg_X_X_diboson."),
    ("Zee_main_l", "bkg_X_X_Zee_X_main_l"),
    ("Zee_main_c", "bkg_X_X_Zee_X_main_c"),
    ("Zee_main_b", "bkg_X_X_Zee_X_main_b"),
    ("Zmm_main_l", "bkg_X_X_Zmm_X_main_l"),
    ("Zmm_main_c", "bkg_X_X_Zmm_X_main_c"),
    ("Zmm_main_b", "bkg_X_X_Zmm_X_main_b"),
    ("Zee_lowmass_lc", "bkg_X_X_Zee_X_lowmass_lc"),
    ("Zee_lowmass_bb", "bkg_X_X_Zee_X_lowmass_b"),
    ("Zmm_lowmass_lc", "bkg_X_X_Zmm_X_lowmass_lc"),
    ("Zmm_lowmass_bb", "bkg_X_X_Zmm_X_lowmass_b"),
    ("Zee_verylowmass", "bkg_X_X_Zee_X_verylowmass"),
    ("Zmm_verylowmass", "bkg_X_X_Zmm_X_verylowmass"),
    ("Zee_ew", "bkg_X_X_Zee_X_ew"),
    ("Zmm_ew", "bkg_X_X_Zmm_X_ew"),
    ("Ztt", "bkg_X_X_Ztt"),
    ("Zee_mgpy8_l", "sys_X_X_Zee_MGPy8_l"),
    ("Zee_mgpy8_c", "sys_X_X_Zee_MGPy8_c"),
    ("Zee_mgpy8_b", "sys_X_X_Zee_MGPy8_b"),
    ("Zmm_mgpy8_l", "sys_X_X_Zmm_MGPy8_l"),
    ("Zmm_mgpy8_c", "sys_X_X_Zmm_MGPy8_c"),
    ("Zmm_mgpy8_b", "sys_X_X_Zmm_MGPy8_b"),
    ("stop", "bkg_X_X_top_stop."),
    ("stop_fastsim", "sys_X_X_top_stop_fastsim"),
    ("stop_powheg_herwig", "sys_X_X_top_stop_PowhegHerwig7"),
    ("stop_amcatnlo_pythia8", "sys_X_X_top_stop_aMCatNLOPythia8"),
    ("stop_ds", "sys_X_X_top_stop_PowhegPythia8_DS"),
    ("ttbar0", "bkg_X_X_top_ttbar.part{,[0-5]}[0-9]_"),
    ("ttbar1", "bkg_X_X_top_ttbar.part[6-9][0-9]_"),
    ("ttbar2", "bkg_X_X_top_ttbar.part1[0-5][0-9]_"),
    ("ttbar3", "bkg_X_X_top_ttbar.part1[6-9][0-9]_"),
    ("ttbar4", "bkg_X_X_top_ttbar.part2[0-5][0-9]_"),
    ("ttbar5", "bkg_X_X_top_ttbar.part2[6-9][0-9]_"),
    ("ttbar6", "bkg_X_X_top_ttbar.part3[0-5][0-9]_"),
    ("ttbar7", "bkg_X_X_top_ttbar.part3[6-9][0-9]_"),
    ("ttbar8", "bkg_X_X_top_ttbar.part[4-9][0-9][0-9]_"),
    ("ttbar_fastsim", "sys_X_X_top_ttbar_fastsim"),
    ("ttbar_powheg_herwig", "sys_X_X_top_ttbar_PowhegHerwig7"),
    ("ttbar_amcatnlo_pythia8", "sys_X_X_top_ttbar_aMCatNLOPythia8"),
    # ("ttbar_hdamp", "sys_X_X_top_ttbar_hdamp517p5"),
    ("ttbarV", "bkg_X_X_top_ttbarV."),
    ("fakes_diboson", "bkg_X_X_fakes_diboson"),
    ("fakes_Wmunu", "bkg_X_X_fakes_Wmunu."),
    ("fakes_Wenu", "bkg_X_X_fakes_Wenu."),
    ("fakes_Wtaunu", "bkg_X_X_fakes_Wtaunu"),
    ("fakes_top_ttbar", "bkg_X_X_fakes_top_ttbar_"),
    ("fakes_top_stop", "bkg_X_X_fakes_top_stop."),
    ("higgs_tautau_ggf", "bkg_X_X_Higgs_tautau_ggf"),
    ("higgs_tautau_vbf", "bkg_X_X_Higgs_tautau_vbf"),
    ("higgs_tautau_vh", "bkg_X_X_Higgs_tautau_vh"),
    ("higgs_ww_ggf", "bkg_X_X_Higgs_WW_ggf"),
    ("higgs_ww_vbf", "bkg_X_X_Higgs_WW_vbf"),
    ("higgs_ww_wh", "bkg_X_X_Higgs_WW_wh"),
    ("higgs_zz_ggf", "bkg_X_X_Higgs_ZZ_ggf"),
    ("higgs_zz_vbf", "bkg_X_X_Higgs_ZZ_vbf"),
    ("higgs_bb_ggf", "bkg_X_X_Higgs_bb_ggf"),
    ("higgs_bb_zh", "bkg_X_X_Higgs_bb_zh"),
    ("higgs_vh_wh", "bkg_X_X_Higgs_vh_wh"),
    ("higgs_vh_zh", "bkg_X_X_Higgs_vh_zh"),
    ("higgs_tth", "bkg_X_X_Higgs_ttH"),
    ("data", "data_X_"),
]
ALTERNATIVE_SAMPLES = [
    "sig_nonres_herwig7_bbww",
    "sig_nonres_herwig7_bbtautau",
    "sig_nonres_herwig7_bbzz",
    "sig_kl10_herwig7_bbww",
    "sig_kl10_herwig7_bbtautau",
    "sig_kl10_herwig7_bbzz",
    "stop_fastsim",
    "stop_powheg_herwig",
    "stop_amcatnlo_pythia8",
    "stop_ds",
    "ttbar_fastsim",
    "ttbar_powheg_herwig",
    "ttbar_amcatnlo_pythia8",
    "ttbar_hdamp",
    "Zee_mgpy8_l",
    "Zee_mgpy8_c",
    "Zee_mgpy8_b",
    "Zmm_mgpy8_l",
    "Zmm_mgpy8_c",
    "Zmm_mgpy8_b",
]


def create_task(args, output_dir, process_id, input_dir, ntup_id, version, process_pattern, region, setup):
    output_file_appendix = ""
    if version == "p4":
        output_file_appendix = "_" + version
    output_file = "{}/{}{}.root".format(output_dir, process_id, output_file_appendix)

    if process_id in ALTERNATIVE_SAMPLES:
        version = "alternative"

    merge_command = "hadd -f {output_file} {input_dir}/{ntup_id}_{version}_{proc_pattern}*{region}*root"
    merge_command = merge_command.format(
        output_file=output_file,
        input_dir=input_dir,
        ntup_id=ntup_id,
        version=version,
        proc_pattern=process_pattern,
        region=region,
    )

    command_list = [
        "mkdir -p " + output_dir,
        'echo "{}"'.format(merge_command),
        "time {}".format(merge_command),
    ]

    task_id = "merge_1tag_res_{}_{}_{}".format(process_id, version, region)
    task_log = "logs/{}.log".format(task_id)

    this_task = task(
        task_id,
        command_list,
        args=args,
        time=1440,
        memory=500,
        outputs=[output_file],
        setup=setup,
        logFile=task_log,
        errFile=task_log,
    )

    return this_task


def main(args):
    input_dir = "data/ntup/unmerged/systematics_1tag_res"
    base_output_dir = "data/ntup/merged/systematics_1tag_res"
    tasks = []

    # get commands for setting up environment
    setup = submit.getSetupCommand(args)
    setupPath = os.environ["CAFANALYSISSETUP"]
    if setupPath:
        setup.append("source " + setupPath)

    for region in REGIONS:
        output_dir = os.path.join(base_output_dir, region)
        for process_id, process_pattern in PROCESSES:
            task_weight = create_task(
                args,
                output_dir,
                process_id,
                input_dir,
                "bbll_systematics_1tag_res",
                "weight",
                process_pattern,
                region,
                setup,
            )
            tasks.append(task_weight)

            if process_id != "data" and process_id not in ALTERNATIVE_SAMPLES:
                task_p4 = create_task(
                    args,
                    output_dir,
                    process_id,
                    input_dir,
                    "bbll_systematics_1tag_res",
                    "p4",
                    process_pattern,
                    region,
                    setup,
                )
                tasks.append(task_p4)

    ctrl = submit.guessSubmissionController()
    ctrl.submitTasks(args, tasks)
    print("Done")


if __name__ == "__main__":
    PARSER = submit.MinimalArgumentParser()

    import ROOT

    # ignore command line arguments since ROOT is very greedy here (and tends to choke from it!)
    ROOT.PyConfig.IgnoreCommandLineOptions = True

    # use the argument parser to read the command line arguments and config options from the config file
    # from os.path import splitext
    ARGS = PARSER.parse_args()

    # call the main function
    main(ARGS)
