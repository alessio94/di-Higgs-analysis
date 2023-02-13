#!/bin/env python

import os
from SubmissionHelpers import submit
from SubmissionHelpers.task import task

REGIONS = ["SR1VBFVeto", "SR2", "TopCR", "ZllCR"]
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
    ("sig_vbf_bbww", "sig_X_X_vbf_novhh_l1cvv1cv1_bbWW"),
    ("sig_vbf_bbtautau", "sig_X_X_vbf_novhh_l1cvv1cv1_bbtautau"),
    ("sig_vbf_bbzz", "sig_X_X_vbf_novhh_l1cvv1cv1_bbZZ"),
    ("sig_vbf_bbww_kl0", "sig_X_X_vbf_novhh_l0cvv1cv1_bbWW"),
    ("sig_vbf_bbtautau_kl0", "sig_X_X_vbf_novhh_l0cvv1cv1_bbtautau"),
    ("sig_vbf_bbzz_kl0", "sig_X_X_vbf_novhh_l0cvv1cv1_bbZZ"),
    ("sig_vbf_bbww_kl2", "sig_X_X_vbf_novhh_l2cvv1cv1_bbWW"),
    ("sig_vbf_bbtautau_kl2", "sig_X_X_vbf_novhh_l2cvv1cv1_bbtautau"),
    ("sig_vbf_bbzz_kl2", "sig_X_X_vbf_novhh_l2cvv1cv1_bbZZ"),
    ("sig_vbf_bbww_kl10", "sig_X_X_vbf_novhh_l10cvv1cv1_bbWW"),
    ("sig_vbf_bbtautau_kl10", "sig_X_X_vbf_novhh_l10cvv1cv1_bbtautau"),
    ("sig_vbf_bbzz_kl10", "sig_X_X_vbf_novhh_l10cvv1cv1_bbZZ"),
    ("diboson", "bkg_X_X_prompt_diboson."),
    ("Zee_hf_main_l", "bkg_X_X_prompt_Zee_hf_main_l"),
    ("Zee_hf_main_c", "bkg_X_X_prompt_Zee_hf_main_c"),
    ("Zee_hf_main_b", "bkg_X_X_prompt_Zee_hf_main_b"),
    ("Zmm_hf_main_l", "bkg_X_X_prompt_Zmm_hf_main_l"),
    ("Zmm_hf_main_c", "bkg_X_X_prompt_Zmm_hf_main_c"),
    ("Zmm_hf_main_b", "bkg_X_X_prompt_Zmm_hf_main_b"),
    ("Zee_hf_lowmass_lc", "bkg_X_X_prompt_Zee_hf_lowmass_lc"),
    ("Zee_hf_lowmass_b", "bkg_X_X_prompt_Zee_hf_lowmass_b"),
    ("Zmm_hf_lowmass_lc", "bkg_X_X_prompt_Zmm_hf_lowmass_lc"),
    ("Zmm_hf_lowmass_b", "bkg_X_X_prompt_Zmm_hf_lowmass_b"),
    ("Zee_hf_verylowmass", "bkg_X_X_prompt_Zee_hf_verylowmass"),
    ("Zmm_hf_verylowmass", "bkg_X_X_prompt_Zmm_hf_verylowmass"),
    ("Zee_hf_ew", "bkg_X_X_prompt_Zee_hf_ew"),
    ("Zmm_hf_ew", "bkg_X_X_prompt_Zmm_hf_ew"),
    ("Zee_lf_main_l", "bkg_X_X_prompt_Zee_lf_main_l"),
    ("Zee_lf_main_c", "bkg_X_X_prompt_Zee_lf_main_c"),
    ("Zee_lf_main_b", "bkg_X_X_prompt_Zee_lf_main_b"),
    ("Zmm_lf_main_l", "bkg_X_X_prompt_Zmm_lf_main_l"),
    ("Zmm_lf_main_c", "bkg_X_X_prompt_Zmm_lf_main_c"),
    ("Zmm_lf_main_b", "bkg_X_X_prompt_Zmm_lf_main_b"),
    ("Zee_lf_lowmass_lc", "bkg_X_X_prompt_Zee_lf_lowmass_lc"),
    ("Zee_lf_lowmass_b", "bkg_X_X_prompt_Zee_lf_lowmass_b"),
    ("Zmm_lf_lowmass_lc", "bkg_X_X_prompt_Zmm_lf_lowmass_lc"),
    ("Zmm_lf_lowmass_b", "bkg_X_X_prompt_Zmm_lf_lowmass_b"),
    ("Zee_lf_verylowmass", "bkg_X_X_prompt_Zee_lf_verylowmass"),
    ("Zmm_lf_verylowmass", "bkg_X_X_prompt_Zmm_lf_verylowmass"),
    ("Zee_lf_ew", "bkg_X_X_prompt_Zee_lf_ew"),
    ("Zmm_lf_ew", "bkg_X_X_prompt_Zmm_lf_ew"),
    ("Ztt_hf", "bkg_X_X_prompt_Ztt_hf"),
    ("Ztt_lf", "bkg_X_X_prompt_Ztt_lf"),
    ("Zee_mgpy8_l", "sys_X_X_bkg_prompt_Zee_MGPy8_l"),
    ("Zee_mgpy8_c", "sys_X_X_bkg_prompt_Zee_MGPy8_c"),
    ("Zee_mgpy8_b", "sys_X_X_bkg_prompt_Zee_MGPy8_b"),
    ("Zmm_mgpy8_l", "sys_X_X_bkg_prompt_Zmm_MGPy8_l"),
    ("Zmm_mgpy8_c", "sys_X_X_bkg_prompt_Zmm_MGPy8_c"),
    ("Zmm_mgpy8_b", "sys_X_X_bkg_prompt_Zmm_MGPy8_b"),
    ("stop_wt", "bkg_X_X_prompt_top_stop_Wt"),
    ("stop_wt_fastsim", "sys_X_X_bkg_prompt_top_stop_fastsim"),
    ("stop_wt_powheg_herwig", "sys_X_X_bkg_prompt_top_stop_PowhegHerwig7"),
    ("stop_wt_amcatnlo_pythia8", "sys_X_X_bkg_prompt_top_stop_aMCatNLOPythia8"),
    ("stop_wt_ds", "sys_X_X_bkg_prompt_top_stop_PowhegPythia8_DS"),
    ("stop_schan", "bkg_X_X_prompt_top_stop_schan"),
    ("stop_tchan", "bkg_X_X_prompt_top_stop_tchan"),
    ("ttbar_0", "bkg_X_X_prompt_top_ttbar_dilepton.part{,[0-5]}[0-9]_"),
    ("ttbar_1", "bkg_X_X_prompt_top_ttbar_dilepton.part[6-9][0-9]_"),
    ("ttbar_2", "bkg_X_X_prompt_top_ttbar_dilepton.part1[0-5][0-9]_"),
    ("ttbar_3", "bkg_X_X_prompt_top_ttbar_dilepton.part1[6-9][0-9]_"),
    ("ttbar_4", "bkg_X_X_prompt_top_ttbar_dilepton.part2[0-5][0-9]_"),
    ("ttbar_5", "bkg_X_X_prompt_top_ttbar_dilepton.part2[6-9][0-9]_"),
    ("ttbar_6", "bkg_X_X_prompt_top_ttbar_dilepton.part3[0-5][0-9]_"),
    ("ttbar_7", "bkg_X_X_prompt_top_ttbar_dilepton.part3[6-9][0-9]_"),
    ("ttbar_8", "bkg_X_X_prompt_top_ttbar_dilepton.part[4-9][0-9][0-9]_"),
    ("ttbar_9", "bkg_X_X_prompt_top_ttbar_{nonallhad,allhad}"),
    ("ttbar_fastsim", "sys_X_X_bkg_prompt_top_ttbar_fastsim"),
    ("ttbar_powheg_herwig", "sys_X_X_bkg_prompt_top_ttbar_PowhegHerwig7"),
    ("ttbar_amcatnlo_pythia8", "sys_X_X_bkg_prompt_top_ttbar_aMCatNLOPythia8"),
    # ("ttbar_hdamp", "sys_X_X_bkg_prompt_top_ttbar_hdamp517p5"),
    ("ttbarV", "bkg_X_X_prompt_top_ttbarV"),
    ("Wmunu", "bkg_X_X_prompt_Wjets_Wmunu"),
    ("Wenu", "bkg_X_X_prompt_Wjets_Wenu"),
    ("Wtaunu", "bkg_X_X_prompt_Wjets_Wtaunu"),
    ("higgs_tautau_ggf", "bkg_X_X_prompt_Higgs_tautau_ggf"),
    ("higgs_tautau_vbf", "bkg_X_X_prompt_Higgs_tautau_vbf"),
    ("higgs_tautau_vh", "bkg_X_X_prompt_Higgs_tautau_vh"),
    ("higgs_ww_ggf", "bkg_X_X_prompt_Higgs_WW_ggf"),
    ("higgs_ww_vbf", "bkg_X_X_prompt_Higgs_WW_vbf"),
    ("higgs_ww_vh", "bkg_X_X_prompt_Higgs_WW_vh"),
    ("higgs_zz_ggf", "bkg_X_X_prompt_Higgs_ZZ_ggf"),
    ("higgs_zz_vbf", "bkg_X_X_prompt_Higgs_ZZ_vbf"),
    ("higgs_bb_ggf", "bkg_X_X_prompt_Higgs_bb_ggf"),
    ("higgs_bb_vbf", "bkg_X_X_prompt_Higgs_bb_vbf"),
    ("higgs_bb_vh", "bkg_X_X_prompt_Higgs_bb_vh"),
    ("higgs_cc_vh", "bkg_X_X_prompt_Higgs_cc_vh"),
    ("higgs_tth", "bkg_X_X_prompt_Higgs_ttH"),
    ("mcfakes_diboson", "bkg_X_X_nonprompt_diboson"),
    ("mcfakes_Zee", "bkg_X_X_nonprompt_Zee"),
    ("mcfakes_Zmm", "bkg_X_X_nonprompt_Zmm"),
    ("mcfakes_Ztt", "bkg_X_X_nonprompt_Ztt"),
    ("mcfakes_Wmunu", "bkg_X_X_nonprompt_Wjets_Wmunu"),
    ("mcfakes_Wenu", "bkg_X_X_nonprompt_Wjets_Wenu"),
    ("mcfakes_Wtaunu", "bkg_X_X_nonprompt_Wjets_Wtaunu"),
    ("mcfakes_ttbar", "bkg_X_X_nonprompt_top_ttbar_{dilepton,nonallhad,allhad}"),
    ("mcfakes_stop", "bkg_X_X_nonprompt_top_stop"),
    ("mcfakes_ttbarV", "bkg_X_X_nonprompt_top_ttbarV"),
    ("mcfakes_higgs", "bkg_X_X_nonprompt_Higgs"),
    ("ddFakes", "bkg_X_*_ddFakes_{data,mc_prompt}"),
    ("data", "data_X_"),
]
ALTERNATIVE_SAMPLES = [
    "sig_nonres_herwig7_bbww",
    "sig_nonres_herwig7_bbtautau",
    "sig_nonres_herwig7_bbzz",
    "sig_kl10_herwig7_bbww",
    "sig_kl10_herwig7_bbtautau",
    "sig_kl10_herwig7_bbzz",
    "stop_wt_fastsim",
    "stop_wt_powheg_herwig",
    "stop_wt_amcatnlo_pythia8",
    "stop_wt_ds",
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

    merge_command = "hadd -fk505 -O {output_file} {input_dir}/{ntup_id}_{version}_{proc_pattern}*{region}*root"
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

    task_id = "merge_{}_{}_{}".format(process_id, version, region)
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
    input_dir = "data/ntup/unmerged/systematics"
    base_output_dir = "data/ntup/merged/systematics"
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
                args, output_dir, process_id, input_dir, "bbll_systematics", "weight", process_pattern, region, setup
            )
            tasks.append(task_weight)

            if process_id != "data" and process_id != "ddFakes" and process_id not in ALTERNATIVE_SAMPLES:
                task_p4 = create_task(
                    args, output_dir, process_id, input_dir, "bbll_systematics", "p4", process_pattern, region, setup
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
