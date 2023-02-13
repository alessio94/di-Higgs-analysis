# Systematics

## How to use the fit input ntuples

A reference implementation for TRexFitter can be found [here](https://gitlab.cern.ch/atlas-physics/HDBS/DiHiggs/bbll/TRExFitter/-/blob/bbll/config/bbll.config).

Instructions for running TRexFitter can be found in [this](https://gitlab.cern.ch/atlas-physics/HDBS/DiHiggs/bbll/TRExFitter/-/blob/bbll/README-bbll.md) documentation.

### Input files

The fit input ntuples are located at `/eos/atlas/atlascerngroupdisk/phys-hdbs/diHiggs/bbll/fit_input/`.

First, there are different directories which contain the ntuples for the different signal and control regions (`SR1VBFVeto`, `SR2`, `TopCR`, `ZllCR`).

Inside the directories there are multiple files for each process:
  - `<proc_filename>.root`: contains nominal events and weight systematics
  - `<proc_filename>_p4.root`: contains four-vector systematics (only for nominal MC samples, not for SSOS Fakes)

You can find an overview of the processes and corresponding file/tree names in the table below.

#### Nominal samples

| Process                         | file names                   | tree names                      | Comment                                       |
| ------------------------------- | ---------------------------- | ------------------------------- | --------------------------------------------- |
| ggF HH non-res bbWW (kl=1)      | `sig_nonres_bbww`            | `sig_nonres_bbww`               |                                               |
| ggF HH non-res bbtautau (kl=1)  | `sig_nonres_bbtautau`        | `sig_nonres_bbtautau`           |                                               |
| ggF HH non-res bbZZ (kl=1)      | `sig_nonres_bbzz`            | `sig_nonres_bbzz_{2l2v,2l2q}`   |                                               |
| ggF HH non-res bbWW (kl=10)     | `sig_kl10_bbww`              | `sig_kl10_bbww`                 |                                               |
| ggF HH non-res bbtautau (kl=10) | `sig_kl10_bbtautau`          | `sig_kl10_bbtautau`             |                                               |
| ggF HH non-res bbZZ (kl=10)     | `sig_kl10_bbzz`              | `sig_kl10_bbzz_{2l2v,2l2q`}     |                                               |
| VBF HH non-res bbWW             | `sig_vbf_bbww`               | `sig_vbf_l1cvv1cv1_bbWW`        |                                               |
| VBF HH non-res bbtautau         | `sig_vbf_bbtautau`           | `sig_vbf_l1cvv1cv1_bbtautau`    |                                               |
| VBF HH non-res bbZZ             | `sig_vbf_bbzz`               | `sig_vbf_l1cvv1cv1_bbZZ`        |                                               |
| diboson                         | `diboson`                    | `diboson`                       |                                               |
| Zee (mll > 40 GeV)              | `Zee_{hf,lf}_main_{l,c,b}`   | `Zee_{hf,lf}_main_{l,c,b}`      | only one (corresponding) tree per file        |
| Zmm (mll > 40 GeV)              | `Zmm_{hf,lf}_main_{l,c,b}`   | `Zmm_{hf,lf}_main_{l,c,b}`      | only one (corresponding) tree per file        |
| Zee (lowmass)                   | `Zee_{hf,lf}_lowmass_{lc,b}` | `Zee_{hf,lf}_lowmass_{lc,b}`    | only one (corresponding) tree per file        |
| Zmm (lowmass)                   | `Zmm_{hf,lf}_lowmass_{lc,b}` | `Zmm_{hf,lf}_lowmass_{lc,b}`    | only one (corresponding) tree per file        |
| Zee (verylowmass)               | `Zee_{hf,lf}_verylowmass`    | `Zee_{hf,lf}_verylowmass`       | only one (corresponding) tree per file        |
| Zmm (verylowmass)               | `Zmm_{hf,lf}_verylowmass`    | `Zmm_{hf,lf}_verylowmass`       | only one (corresponding) tree per file        |
| Zee (electroweak)               | `Zee_{hf,lf}_ew`             | `Zee_{hf,lf}_ew`                | only one (corresponding) tree per file        |
| Zmm (electroweak)               | `Zmm_{hf,lf}_ew`             | `Zmm_{hf,lf}_ew`                | only one (corresponding) tree per file        |
| Ztt                             | `Ztt_{hf,lf}`                | `Ztt_{hf,lf}`                   | only one (corresponding) tree per file        |
| single top (Wt)                 | `stop_wt`                    | `stop_wt`                       |                                               |
| single top (s-channel)          | `stop_schan`                 | `stop_schan`                    |                                               |
| single top (t-channel)          | `stop_tchan`                 | `stop_tchan`                    |                                               |
| ttbar                           | `ttbar{0-9}`                 | `ttbar`                         | sample split into multiple files              |
| ttbar+V                         | `ttbarV`                     | `ttbarV`                        |                                               |
| W+jets (Wenu)                   | `Wenu`                       | `Wenu`                          |                                               |
| W+jets (Wmunu)                  | `Wmunu`                      | `Wmunu`                         |                                               |
| W+jets (Wtaunu)                 | `Wtaunu`                     | `Wtaunu`                        |                                               |
| Higgs (ggF, tautau)             | `higgs_tautau_ggf`           | `higgs_tautau_ggf`              |                                               |
| Higgs (VBF, tautau)             | `higgs_tautau_vbf`           | `higgs_tautau_vbf`              |                                               |
| Higgs (VH, tautau)              | `higgs_tautau_vh`            | `higgs_tautau_vh`               |                                               |
| Higgs (ggF, WW)                 | `higgs_ww_ggf`               | `higgs_ww_ggf`                  |                                               |
| Higgs (VBF, WW)                 | `higgs_ww_vbf`               | `higgs_ww_vbf`                  |                                               |
| Higgs (VH, WW)                  | `higgs_ww_vh`                | `higgs_ww_vh`                   |                                               |
| Higgs (ggF, ZZ)                 | `higgs_zz_ggf`               | `higgs_zz_ggf`                  |                                               |
| Higgs (VBF, ZZ)                 | `higgs_zz_vbf`               | `higgs_zz_vbf`                  |                                               |
| Higgs (ggF, bb)                 | `higgs_bb_ggf`               | `higgs_bb_ggf`                  |                                               |
| Higgs (VBF, bb)                 | `higgs_bb_vbf`               | `higgs_bb_vbf`                  |                                               |
| Higgs (VH, bb)                  | `higgs_bb_vh`                | `higgs_bb_vh`                   |                                               |
| Higgs (VH, cc)                  | `higgs_cc_vh`                | `higgs_cc_vh`                   |                                               |
| Higgs (ttH)                     | `higgs_tth`                  | `higgs_tth`                     |                                               |
| MC Fakes (diboson)              | `mcfakes_diboson`            | `mcfakes_diboson`               |                                               |
| MC Fakes (Zee)                  | `mcfakes_Zee`                | `mcfakes_Zee`                   |                                               |
| MC Fakes (Zmm)                  | `mcfakes_Zmm`                | `mcfakes_Zmm`                   |                                               |
| MC Fakes (Ztt)                  | `mcfakes_Ztt`                | `mcfakes_Ztt`                   |                                               |
| MC Fakes (Wenu)                 | `mcfakes_Wenu`               | `mcfakes_Wenu`                  |                                               |
| MC Fakes (Wmunu)                | `mcfakes_Wmunu`              | `mcfakes_Wmunu`                 |                                               |
| MC Fakes (Wtaunu)               | `mcfakes_Wtaunu`             | `mcfakes_Wtaunu`                |                                               |
| MC Fakes (ttbar)                | `mcfakes_ttbar`              | `mcfakes_ttbar`                 |                                               |
| MC Fakes (stop)                 | `mcfakes_stop`               | `mcfakes_stop_{wt,schan,tchan}` |                                               |
| MC Fakes (ttbarV)               | `mcfakes_ttbarV`             | `mcfakes_ttbarV`                |                                               |
| MC Fakes (Higgs)                | `mcfakes_higgs`              | `mcfakes_higgs`                 |                                               |
| SSOS Fakes (data)               | `ddFakes`                    | `ddfakes_data`                  |                                               |
| SSOS Fakes (diboson)            | `ddFakes`                    | `ddfakes_diboson`               |                                               |
| SSOS Fakes (Zee)                | `ddFakes`                    | `ddfakes_Zee`                   |                                               |
| SSOS Fakes (Zmm)                | `ddFakes`                    | `ddfakes_Zmm`                   |                                               |
| SSOS Fakes (Ztt)                | `ddFakes`                    | `ddfakes_Ztt`                   |                                               |
| SSOS Fakes (Wenu)               | `ddFakes`                    | `ddfakes_Wenu`                  |                                               |
| SSOS Fakes (Wmunu)              | `ddFakes`                    | `ddfakes_Wmunu`                 |                                               |
| SSOS Fakes (Wtaunu)             | `ddFakes`                    | `ddfakes_Wtaunu`                |                                               |
| SSOS Fakes (ttbar)              | `ddFakes`                    | `ddfakes_top_ttbar`             |                                               |
| SSOS Fakes (stop)               | `ddFakes`                    | `ddfakes_top_stop`              |                                               |
| SSOS Fakes (ttbarV)             | `ddFakes`                    | `ddfakes_top_ttbarV`            |                                               |
| SSOS Fakes (Higgs)              | `ddFakes`                    | `ddfakes_higgs`                 |                                               |
| Data                            | `data`                       | `data`                          |                                               |

#### Alternative samples

These samples are used to estimate theory uncertainties for some some samples.
See the section below about systematics for more information.

| Process                         | file names                    | tree names                            | Comment                                                 |
| ------------------------------- | ----------------------------- | ------------------------------------- | ------------------------------------------------------- |
| ggF HH non-res bbWW (kl=1)      | `sig_nonres_herwig7_bbww`     | `sig_nonres_herwig7_bbww`             | Herwig 7                                                |
| ggF HH non-res bbtautau (kl=1)  | `sig_nonres_herwig7_bbtautau` | `sig_nonres_herwig7_bbtautau`         | Herwig 7                                                |
| ggF HH non-res bbZZ (kl=1)      | `sig_nonres_herwig7_bbzz`     | `sig_nonres_herwig7_bbzz_{2l2v,2l2q}` | Herwig 7                                                |
| ggF HH non-res bbWW (kl=10)     | `sig_kl10_herwig7_bbww`       | `sig_kl10_herwig7_bbww`               | Herwig 7                                                |
| ggF HH non-res bbtautau (kl=10) | `sig_kl10_herwig7_bbtautau`   | `sig_kl10_herwig7_bbtautau`           | Herwig 7                                                |
| ggF HH non-res bbZZ (kl=10)     | `sig_kl10_herwig7_bbzz`       | `sig_kl10_herwig7_bbzz_{2l2v,2l2q}`   | Herwig 7                                                |
| Zee (mll > 40 GeV)              | `Zee_mgpy8_{l,c,b}`           | `zee_mgpy8_{l,c,b}`                   | Madgraph+Pythia 8, only the corresponding tree per file |
| Zmm (mll > 40 GeV)              | `Zmm_mgpy8_{l,c,b}`           | `zmm_mgpy8_{l,c,b}`                   | Madgraph+Pythia 8, only the corresponding tree per file |
| single top (Wt)                 | `stop_wt_fastsim`             | `stop_wt_fastsim`                     | fast-sim                                                |
| single top (Wt)                 | `stop_wt_powheg_herwig`       | `stop_wt_powheg_herwig`               | Powheg+Herwig 7                                         |
| single top (Wt)                 | `stop_wt_amcatnlo_pythia8`    | `stop_wt_amcatnlo_pythia8`            | aMCatNLO+Pythia 8                                       |
| single top (Wt)                 | `stop_wt_ds`                  | `stop_wt_ds`                          | DS                                                      |
| ttbar                           | `ttbar_fastsim`               | `ttbar_fastsim`                       | fast-sim                                                |
| ttbar                           | `ttbar_powheg_herwig`         | `ttbar_powheg_herwig`                 | Powheg+Herwig 7                                         |
| ttbar                           | `ttbar_amcatnlo_pythia8`      | `ttbar_amcatnlo_pythia8`              | aMCatNLO+Pythia 8                                       |


### Branches

The branch `weight` contains all normalization, event weight, and scale factors already multiplied together.

The output of the neural networks are stored in the `nn_{signalregion}_{outputclass}` branches.
There are also versions trained on the kl=10 samples, indicated by `kl10` in the branch name.

Furthermore, branches for all input features for the NNs are included.

### Weight systematics

For weight systematics there are different branches in the `<proc>` tree which will be used to modify the `weight` branch.
Make sure to divide out the nominal value of the varied weight.
The nominal values for each component of the `weight` branch are stored in the `*_nom` branches.

For example, for the `ATLAS_EL_EFF_TRIG_TOTAL_1NPCOR_PLUS_UNCOR` systematic the new weight expression for the up variation
should be `weight * trigger_sf_el_eff_trigger_up / trigger_sf_nom`.

You can find the weight systematics for the bbll analysis in the table below.

| Name in workspace                           | branch for up variation                         | branch for down variation                         | branch for nominal value |
| ------------------------------------------- | ----------------------------------------------- | ------------------------------------------------- | ------------------------ |
| `ATLAS_EL_EFF_TRIG_TOTAL_1NPCOR_PLUS_UNCOR` | `trigger_sf_el_eff_trigger_up`                  | `trigger_sf_el_eff_trigger_down`                  | `trigger_sf_nom`         |
| `ATLAS_EL_EFF_RECO_TOTAL_1NPCOR_PLUS_UNCOR` | `lepton_sf_el_eff_reco_up`                      | `lepton_sf_el_eff_reco_down`                      | `lepton_sf_nom`          |
| `ATLAS_EL_EFF_ID_TOTAL_1NPCOR_PLUS_UNCOR`   | `lepton_sf_el_eff_id_up`                        | `lepton_sf_el_eff_id_down`                        | `lepton_sf_nom`          |
| `ATLAS_EL_EFF_ISO_TOTAL_1NPCOR_PLUS_UNCOR`  | `lepton_sf_el_eff_iso_up`                       | `lepton_sf_el_eff_iso_down`                       | `lepton_sf_nom`          |
| `ATLAS_MUON_EFF_TrigStatUncertainty`        | `trigger_sf_nom_muon_eff_trig_stat_up`          | `trigger_sf_nom_muon_eff_trig_stat_down`          | `trigger_sf_nom`         |
| `ATLAS_MUON_EFF_TrigSystUncertainty`        | `trigger_sf_nom_muon_eff_trig_sys_up`           | `trigger_sf_nom_muon_eff_trig_sys_down`           | `trigger_sf_nom`         |
| `ATLAS_MUON_EFF_RECO_STAT`                  | `lepton_sf_muon_eff_reco_stat_up`               | `lepton_sf_muon_eff_reco_stat_down`               | `trigger_sf_nom`         |
| `ATLAS_MUON_EFF_RECO_SYS`                   | `lepton_sf_muon_eff_reco_sys_up`                | `lepton_sf_muon_eff_reco_sys_down`                | `trigger_sf_nom`         |
| `ATLAS_MUON_EFF_RECO_STAT_LOWPT`            | `lepton_sf_muon_eff_reco_stat_lowpt_up`         | `lepton_sf_muon_eff_reco_stat_lowpt_down`         | `trigger_sf_nom`         |
| `ATLAS_MUON_EFF_RECO_SYS_LOWPT`             | `lepton_sf_muon_eff_reco_sys_lowpt_up`          | `lepton_sf_muon_eff_reco_sys_lowpt_down`          | `trigger_sf_nom`         |
| `ATLAS_MUON_EFF_ISO_STAT`                   | `lepton_sf_muon_eff_iso_stat_up`                | `lepton_sf_muon_eff_iso_stat_down`                | `trigger_sf_nom`         |
| `ATLAS_MUON_EFF_ISO_SYS`                    | `lepton_sf_muon_eff_iso_sys_up`                 | `lepton_sf_muon_eff_iso_sys_down`                 | `trigger_sf_nom`         |
| `ATLAS_JET_JvtEfficiency`                   | `jet_jvt_efficency_up`                          | `jet_jvt_efficency_down`                          | `jet_jvt_efficency_nom`  |
| `ATLAS_JET_fJvtEfficiency`                  | `jet_fjvt_efficency_up`                         | `jet_fjvt_efficency_down`                         | `jet_jvt_efficency_nom`  |
| `ATLAS_PU_PRW_DATASF`                       | `prw_datasf_up`                                 | `prw_datasf_down`                                 | `prw_nom`                |
| `ATLAS_FT_EFF_Eigen_B_0`                    | `btag_sf_eff_eigen_b_0_up`                      | `btag_sf_eff_eigen_b_0_down`                      | `btag_sf_nom`            |
| `ATLAS_FT_EFF_Eigen_B_1`                    | `btag_sf_eff_eigen_b_1_up`                      | `btag_sf_eff_eigen_b_1_down`                      | `btag_sf_nom`            |
| `ATLAS_FT_EFF_Eigen_B_2`                    | `btag_sf_eff_eigen_b_2_up`                      | `btag_sf_eff_eigen_b_2_down`                      | `btag_sf_nom`            |
| `ATLAS_FT_EFF_Eigen_C_0`                    | `btag_sf_eff_eigen_c_0_up`                      | `btag_sf_eff_eigen_c_0_down`                      | `btag_sf_nom`            |
| `ATLAS_FT_EFF_Eigen_C_1`                    | `btag_sf_eff_eigen_c_1_up`                      | `btag_sf_eff_eigen_c_1_down`                      | `btag_sf_nom`            |
| `ATLAS_FT_EFF_Eigen_C_2`                    | `btag_sf_eff_eigen_c_2_up`                      | `btag_sf_eff_eigen_c_2_down`                      | `btag_sf_nom`            |
| `ATLAS_FT_EFF_Eigen_C_3`                    | `btag_sf_eff_eigen_c_3_up`                      | `btag_sf_eff_eigen_c_3_down`                      | `btag_sf_nom`            |
| `ATLAS_FT_EFF_Eigen_Light_0`                | `btag_sf_eff_eigen_light_0_up`                  | `btag_sf_eff_eigen_light_0_down`                  | `btag_sf_nom`            |
| `ATLAS_FT_EFF_Eigen_Light_1`                | `btag_sf_eff_eigen_light_1_up`                  | `btag_sf_eff_eigen_light_1_down`                  | `btag_sf_nom`            |
| `ATLAS_FT_EFF_Eigen_Light_2`                | `btag_sf_eff_eigen_light_2_up`                  | `btag_sf_eff_eigen_light_2_down`                  | `btag_sf_nom`            |
| `ATLAS_FT_EFF_Eigen_Light_3`                | `btag_sf_eff_eigen_light_3_up`                  | `btag_sf_eff_eigen_light_3_down`                  | `btag_sf_nom`            |
| `ATLAS_FT_EFF_extrapolation`                | `btag_sf_eff_eigen_extrapolation_up`            | `btag_sf_eff_eigen_extrapolation_down`            | `btag_sf_nom`            |
| `ATLAS_FT_EFF_extrapolation_from_charm`     | `btag_sf_eff_eigen_extrapolation_from_charm_up` | `btag_sf_eff_eigen_extrapolation_from_charm_down` | `btag_sf_nom`            |

### Four-vector systematics

The four-vector systematics are stored in the `<proc>_p4.root` ntuples.

For each variation there is a tree with the name of the variation. The name of the tree consists of the process name and variation name: `<proc>_<variation>`.
This tree contains the output of the neural networks and the `weight` branch.

There is also a `<proc>_Nominal` tree in the `<proc>_p4.root` ntuples which you should use to compare with the systematic variations.
This ensures that you are not affected by some missing statistics in either the nominal or four-fector statistics
(these two files are created in two different batch jobs, so in case of some failing jobs there cut be a discrepancy).

For example, for the `ATLAS_MUON_ID` systematic use the tree `<proc>_MUON_ID__1up` for the up variation and compare to the
`<proc>_Nominal` tree (both trees are in `<proc>_p4.root`).

A list with all four-vector variations can be found in [this](https://gitlab.cern.ch/atlas-physics/HDBS/DiHiggs/bbll/CAFbbll/blob/master/share/bbll/auxData/systematicVariations/p4-variations.txt) config.

For the `JET_JER` systematics you need to combine the smeared MC (`_JET_JERMC_<variation>`) and smeared pseudo data (`_JET_JERPD_<variation>`) variations
by subtracting the pseudo data variation from the MC variation and adding this to the nominal variation (per bin):
```
<proc>_JET_JER_<variation>__1up   = <proc>_JET_JERMC_<variation>__1up   - <proc>_JET_JERPD_<variation>__1up   + <proc>_Nominal (per bin)
<proc>_JET_JER_<variation>__1down = <proc>_JET_JERMC_<variation>__1down - <proc>_JET_JERPD_<variation>__1down + <proc>_Nominal (per bin)
```

Variations ending with `MC16` are only present for fullsim samples. Variations ending with `AFII` or `AF2` are only present for fastsim samples.
Currently only the resonant signal (`sig_res_*`) samples and verylowmass Zll samples are fastsim samples.

### Uncertainties for SSOS Fakes

As the SSOS fake estimate is semi-datadriven the main uncertainties arise not from the normal ATLAS CP uncertainties but
from the calculation of the transfer factor (TF).

We consider systematics uncertainties from two sources:

1. Statistical error on the TF for each bin of the TF.
   These are available in the `fake_weight_stat_{up,down}_bin_{0-10}` branches.
   This yields 11 systematics uncertainties, as they need to be treated uncorrelated for each TF bin.
3. Different TFs due to using a different generator for the major MC contributions the TF caclulation.
   These are available in the `fake_weight_gen_{up,down}` branches.

Both uncertainties behave like normal weight uncertainties, so the weight branch needs to be multiplied with
the systematics weight and divided by the nominal value of the TF,
e.g. use `weight*fake_weight_stat_down_bin_0/fake_weight_nom`.

### Theory uncertainties

#### General remarks on weight theory systematics

Similar to the normal weight systematics for experimental uncertainties you need to modify the `weight` expression.

**Currently it is not recommended to vary the initial sum-of-weights!**
~~However, here you also need to vary the initial sum-of-weights normalization and not only the MC weight.~~
~~Because in the calculation of the final event weight the initial sum-of-weights value enters in the denominator,~~
~~you need to multiply with the nominal value and divide by the varied value.~~

~~For example, for one of the scale variations of the signal samples use `weight * mcweight_signal_scale_mur_05_murf_05 / mcweight_nom * sumofweight_nom / sumofweight_signal_scale_mur_05_muf_05`.~~

##### Scale uncertainties

For the scale variation we use the 7-point variation where you vary each of the scales between `[0.5, 1.0, 2.0]` and build all possible
combinations except of the cross-diagonal terms (`{0.5, 2.0}` and `{2.0, 0.5}`). The `{1.0, 1.0}` variation is the nominal one and you don't need to modify the `weight` branch.
Take the envelope of the seven variations to build the final uncertainty.

#### ggF signal theory uncertainties

For details see https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/DiHiggsCombination#Non_resonant_ggF

##### Alternative samples

There are alternative samples using Herwig 7 instead of Pythia 8 for the parton shower.
The alternative samples can be found in the `sig_nonres_herwig7_{bbww,bbtautau,bbzz}` files
with `sig_nonres_herwig7_{bbww,bbtautau,bbzz_2l2v,bbzz_2l2q}` as tree names.

##### Reweighting

The signal theory uncertainties which rely on reweighting are stored in the `sig_nonres_{bbww,bbtautau,bbzz}.root` ntuples.

See the general remarks on weight systematics for theory uncertainties [above](#general-remarks-on-weight-theory-systematics).

There are scale uncertainties for the signal samples, see [here](#scale-uncertainties) for more details.

For the PDF variation there are 30 internal eigen errors of PDF4LHC15 and two $`\alpha_s`$ (0.1165 and 0.1195) variations.
Combine the 30 internal PDF variations with the **Hessian method**:

```math
\Delta(\mathrm{PDF}) = \sqrt{\sum_i (X_i - X_0)^2}
```

Combine the two $`\alpha_s`$ variations by subtracting them from each other and dividing by two

```math
\Delta(\alpha_s) = (X_{0.1195} - X_{0.1165}) / 2
```

Then combine the the two into a PDF+$`\alpha_s`$ uncertainty by summing quadratically:

```math
\Delta(\mathrm{PDF}+\alpha_s) = \sqrt{\Delta(\mathrm{PDF})^2 + \Delta(\alpha_s)^2}
```

| Variation        | MC weight branch                         | initial sumw branch                         | Additional comments                            |
| ---------------- | ---------------------------------------- | ------------------------------------------- | ---------------------------------------------- |
| muR+muF scales   | `mcweight_signal_scale_*`                | `sumofweight_signal_scale_*`                | compute envelope of 6 variations + nominal     |
| PDF              | `mcweight_signal_pdf_internal_*`         | `sumofweight_signal_pdf_internal_*`         | combine with Hessian method                    |
| $`\alpha_s`$     | `mcweight_signal_pdf_alphas_{1195,1165}` | `sumofweight_signal_pdf_alphas_{1195,1165}` | combine by building difference and divide by 2 |

##### Cross-section uncertainties

See https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHWGHH?redirectedfrom=LHCPhysics.LHCHXSWGHH#Latest_recommendations_for_gluon

Assign a flat $`\pm 3\,\%`$ uncertainty on the ggF HH cross-section for PDF+$`\alpha_s`$ and
$`-23\,\% / +6\,\%`$ for scale+mtop.

#### VBF signal theory uncertainties

For details see https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/DiHiggsCombination#Non_resonant_VBF

##### Alternative samples

We plan on using alternative samples for a parton shower uncertainty replacing Pythia 8 with Herwig 7, however these samples are not available yet.

##### Reweighting

The signal theory uncertainties which rely on reweighting are stored in the `sig_vbf_{bbww,bbtautau,bbzz}.root` ntuples.

See the general remarks on weight systematics for theory uncertainties [above](#general-remarks-on-weight-theory-systematics).

There are scale uncertainties for the signal samples, see [here](#scale-uncertainties) for more details.

For the PDF variation there are 100 internal variations of NNPDF23LO.
Combine the variations $`X_i`$ by computing the standard deviation w.r.t the nominal variation $`X_0`$:

```math
\Delta X = \sqrt{\frac{1}{N}\sum_i (X_i - X_0)^2}
```

Since the nominal variation is already the mean of all replicas one just needs a factor $`\frac{1}{N}`$.

| Variation        | MC weight branch                         | initial sumw branch                         | Additional comments                            |
| ---------------- | ---------------------------------------- | ------------------------------------------- | ---------------------------------------------- |
| muR+muF scales   | `mcweight_signal_vbf_scale_*`            | `sumofweight_signal_vbf_scale_*`            | compute envelope of 6 variations + nominal     |
| PDF              | `mcweight_signal_vbf_pdf_*`              | `sumofweight_signal_vbf_pdf_*`              | combine with standard deviation                |

##### Cross-section uncertainties

See https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHWGHH?redirectedfrom=LHCPhysics.LHCHXSWGHH#HHjj_VBF

Assign a flat $`\pm 2.1\,\%`$ uncertainty on the VBF HH cross-section for PDF+$`\alpha_s`$ and
$`-0.04\,\% / +0.03\,\%`$ for scale+mtop.

#### Z+jets theory uncertainties

##### Alternative samples

There are alternative samples using Madgraph+Pythia8 instead of Sherpa for the generator and parton shower.
The alternative samples can be found in the `Z{ee,mm}_mgpy8_*` files.
These samples correspond to the main Zll samples, i.e. `Z{ee,mm}_{hf,lf}_main_*`.
In difference to the main Zll samples the alternative samples are not split into heavy flavor/light flavor.
To select only heavy flavor events apply the following selection on the alternative samples:
`(bjet_0_truthinfo == 5 || bjet_0_truthinfo == 4) && (bjet_1_truthinfo == 5 || bjet_1_truthinfo == 4)`.

##### Reweighting

The Z+jets theory uncertainties which rely on reweighting are stored in the `{Zee,Zmm}_{hf,lf}_*.root` ntuples.

See the general remarks on weight systematics for theory uncertainties [above](#general-remarks-on-weight-theory-systematics).

There are scale uncertainties for the Zll samples. In this case the `{1.0, 1.0}` variation is also available.
According to the [PmgWeakBosonProcesses TWiki](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/PmgWeakBosonProcesses#Process_specific_recipes)
this variation should also be used to calculate the uncertainty with respect to this uncertainty and not the nominal one, and then transfer the resulting uncertainty onto the prediction based on the nominal weight.

For the PDF uncertainties there are 100 NNPDF replicas.
Combine the variations $`X_i`$ by computing the standard deviation w.r.t the nominal variation $`X_0`$:

```math
\Delta(\mathrm{PDF}) = \sqrt{\frac{1}{N}\sum_i (X_i - X_0)^2}
```

Since the nominal variation is already the mean of all replicas one just needs a factor $`\frac{1}{N}`$.

Combine the two $`\alpha_s`$ variations by subtracting them from each other and dividing by two

```math
\Delta(\alpha_s) = (X_{0.1195} - X_{0.1165}) / 2
```

Then combine the the two into a PDF+$`\alpha_s`$ uncertainty by summing quadratically:

```math
\Delta(\mathrm{PDF}+\alpha_s) = \sqrt{\Delta(\mathrm{PDF})^2 + \Delta(\alpha_s)^2}
```

These variations are only available for the Zee samples (due to technical/historical reasons during the Zmm event generation).
The [PmgWeakBosonProcesses TWiki](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/PmgWeakBosonProcesses#Process_specific_recipes)
suggests that
> you can transfer the relative uncertainty that you get for Zee to Zmumu at the truth level (in the same fiducial phase-space).

The same needs to be done for the $`\alpha_s`$ variations.

There are also CKKW and QSF variations available.
These are added to the ntuples with the
[AddZJetsWeights.py](https://gitlab.cern.ch/atlas/athena/blob/21.2/PhysicsAnalysis/SUSYPhys/SUSYTools/scripts/AddZJetsWeights.py)
script from the SUSY group after the ntuples are produced by CAF.
For these variations you just need to multiply the additional weight to the nominal weight branch `weight`.

| Variation                | MC weight branch                     | initial sumw branch                     | Additional comments                                             |
| ------------------------ | ------------------------------------ | --------------------------------------- | --------------------------------------------------------------- |
| muR+muF scales           | `mcweight_{zee,zmm}_scale_*`         | `sumofweight_{zee,zmm}_scale_*`         | compute envelope of 7 variations w.r.t `{1.0, 1.0}` variation   |
| PDF                      | `mcweight_zee_pdf_*`                 | `sumofweight_zee_pdf_*`                 | combine with standard deviation, only available for Zee         |
| $`\alpha_s`$             | `mcweight_zee_alphas_*`              | `sumofweight_zee_alphas_*`              | combine with difference and divide by 2, only available for Zee |
| CKKW                     | `ckkw*_Weight`                       | not needed                              | just mulitply MC weight branch with `weight` branch             |
| QSF                      | `qsf*_Weight`                        | not needed                              | just mulitply MC weight branch with `weight` branch             |
| external PDF             | `mcweight_{zee,zmm}_{mmht2014,ct14}` | `sumofweight_{zee,zmm}_{mmht2014,ct14}` | compute envelope of the 2 variations + nominal                  |

#### ttbar theory uncertainties

##### Alternative samples

The alternative samples are stored in the `ttbar_<proc>.root` ntuples inside the `ttbar_<proc>` tree, where `<proc>` is
`fastsim`, `powheg_herwig`, `amcatnlo_pythia8`, or `hdamp`.

Because the alternative samples are fastsim you need to compare to the fastsim version of the nominal ttbar sample (`ttbar_fastsim`) and then
transfer the relative uncertainties onto the fullsim sample.

| Variation       | Sample description        | `<proc>`           |
| --------------- | ------------------------- | ------------------ |
| Nominal         | Powheg+Pythia 8 (fastsim) | `fastsim`          |
| Matrix  element | aMCatNLO+Pythia8          | `amcatnlo_pythia8` |
| Parton shower   | Powheg+Herwig7            | `powheg_herwig`    |

The `hdamp` alternative sample (varied hdamp parameter) is currently not used.

##### Reweighting

The ttbar theory uncertainties which rely on reweighting are stored in the `ttbar.root` ntuple.

See the general remarks on weight systematics for theory uncertainties above.

There are scale uncertainties for the ttbar samples, see [here](#scale-uncertainties) for more details.

For the PDF variation we use the PDF4LHC15 error set, which consists of 30 values (`*_ttbar_pdf_{1..30}`).
We also need the central PDF value of the PDF4LHC15 set (`*_ttbar_pdf_0`).
Compute the PDF error with the Hessian method, where $`X_0`$ is the central value and $`X_i`$ are the 30 variations of the error set.

```math
\Delta X = \sqrt{\sum_i (X_i - X_0)^2}
```

Combine the two $`\alpha_s`$ variations by subtracting them from each other and dividing by two

```math
\Delta X = (X_{0.119} - X_{0.117}) / 2
```

The relative variations of the PDF and $`\alpha_s`$ uncertainties should be combined quadratically:

```math
\delta^{\text{PDF}+\alpha_s} = \sqrt{{(\delta^{\text{PDF}})}^2 + {(\delta^{\alpha_s})}^2}
```

| Variation      | MC weight branch                  | initial sumw branch                  | Additional comments                        |
| -------------- | --------------------------------  | ------------------------------------ | ------------------------------------------ |
| ISR alpha_s    | `mcweight_ttbar_isr_*`            | `sumofweight_ttbar_isr_*`            |                                            |
| FSR            | `mcweight_ttbar_fsr_*`            | `sumofweight_ttbar_fsr_*`            |                                            |
| muR+muF scales | `mcweight_ttbar_scale_*`          | `sumofweight_ttbar_scale`            | compute envelope of 6 variations + nominal |
| PDF            | `mcweight_ttbar_pdf_*`            | `sumofweight_ttbar_pdf_*`            | use Hessian method to combine              |
| $`\alpha_s`$   | `mcweight_ttbar_alphas_{117,119}` | `sumofweight_ttbar_alphas_{117,119}` | combine with difference and divide by 2    |

#### single top (Wt) theory uncertainties

##### Alternative samples

The alternative samples are stored in the `stop_wt_<proc>.root` ntuples inside the `stop_wt_<proc>` tree, where `<proc>` is
`fastsim`, `powheg_herwig`, `amcatnlo_pythia8`, or `ds`.

Because the alternative samples (with exception of `ds`) are fastsim you need to compare to the fastsim version of the nominal single top sample (`stop_fastsim`) and then transfer the relative uncertainties onto the fullsimm sample.
The `ds` sample can be directly compared to the fullsim nominal samples (which uses the DR scheme).

| Variation          | Sample description        | `<proc>`           |
| ------------------ | ------------------------- | ------------------ |
| Nominal            | Powheg+Pythia 8 (fastsim) | `fastsim`          |
| Matrix  element    | aMCatNLO+Pythia8          | `amcatnlo_pythia8` |
| Parton shower      | Powheg+Herwig7            | `powheg_herwig`    |
| ttbar interference | Powheg+Pythia8 (DS scheme)| `ds`               |

##### Reweighting

Follow the instructions for the ttbar theory uncertainties, but replace `ttbar` in the branch names with `stop`.

## How to generate ntuples with systematics

Run the normal prepare and initialize steps mentioned in the main [README](../README.md).

Submit the systematic jobs with
```bash
./scripts/systematics/submit_systematics.sh
```

There are different configs for four-vector systematics and weight systematics, as well as alternative samples.
Check the configs referenced in the script for more details.

You can check the status of the jobs using the `--checkmissing` option for the script mentioned above.

The ntuples will be located at `data/ntup/systematics`.

It can happen that some jobs fail, but still produce an output file. Check for this by running the script
```bash
./scripts/systematics/cleanup_failed_jobs.py
```
This will remove the ntuple files and corresponding sample folders if the ntuple file is smaller an 1kB, as this usually indicates that something went wrong with the job.
Then run the submit script again to submit the failed jobs.

If the weights for the kappa-lambda reweighting of the ggF samples are needed, submit the postprocessing
as follows:
```bash
./scripts/systematics/submit_postprocessing_kl.py
```

This is done before merging, otherwise the jobs take too much time.
You can check the status of the jobs using the `--checkmissing` option for the script mentioned above.

After all jobs are completed you can merge the ntuples with the merge script.
This script will submit merge jobs to the batch system.

```bash
./scripts/systematics/submit_merge.py
```
Here again you can use the `--checkmissing` option to check the status of the jobs.

The merged ntuples will be located at `data/ntup/merged/systematics`.

Here you should also check for ntuple files smaller than 1kB. You can do this with
```bash
find data/ntup/merged/systematics/ -name "*root" -type 'f' -size 1k
```
Check the log files to understand why this didn't work.

You can delete these files with
```bash
find data/ntup/merged/systematics/ -name "*root" -type 'f' -size 1k -delete
```

Then run the postprocessing script to add the Z+jets CKKW and QSF weights:

```bash
./scripts/systematics/postprocessing.sh
```
