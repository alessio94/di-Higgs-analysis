# CAFbbll - Detailed documentation

This guide will help you to understand how the HH->bbll analysis is done with CAF (Common Analysis Framework) on a technical level.

## Useful links

### Tutorials

If you want to know more about how CAF works on a more general level please have a look at the following resources

 - [CAF & PxAOD Tutorial](https://indico.cern.ch/event/875315/) (only the CAF part)
 - [Getting started](https://atlas-caf-tutorial.web.cern.ch/node/2)

### Technical documentation and help

 - [Code search and documentation](http://atlas-caf.web.cern.ch/)
 - [Tag documentation](http://atlas-caf.web.cern.ch/tags.html)
 - [Observable documentation](http://atlas-caf.web.cern.ch/observables.html)
 - [GitLab repository](https://gitlab.cern.ch/atlas-caf/CAFCore)
 - Mailing list: [qframework-users@cern.ch](mailto:qframework-users@cern.ch)

## Introduction

The workflow in CAF is split into four steps. For each step there is a corresponding python script and master config file.
The four steps are:
 - `prepare`: create the sample folder structure
 - `initialize`: locate and inspect the input files
 - `analyze`: run the analysis, i.e. loop over all events, apply cuts, and fill histograms
 - `visualize`: create nice plots and cutflows

What the different steps do is explained below in detail.

One core concept of CAF is the so-called sample folder.
The idea is that the sample folder contains all relevant information from the previous step.
So the input of the `initialize` step is the output of the `prepare` step.
Inside the sample folder the different MC and data samples are stored in a tree similar to a tree in a file system.
After the analyze step the histograms and counters (objects to store event yields) are also stored in the sample folder.

Sometimes it makes sense to investigate the contents of the sample folder after a step.
This can be done with with an interactive ROOT shell. However, ROOT doesn't understand sample folders per default.
Because of this there is the `tqroot` wrapper, which loads the corresponding classes. You can use it the following way:

```
tqroot -sf <path/to/sample-folder.root>
# print top-level content
samples->print(":td")
# print a certain path in the sample folder
samples->print("sig/ee/c16a/nonres/bbtautau:td")
```

## Contents of this repository

If you look at the top level of this repository you see some folders. The important folders and their meaning are listed here.

 - CAFCore: core code of CAF
 - CAFbbll: header files for bbll analysis
 - CxAODTools: part of the CxAODFramework which is needed to read CxAODs
 - Root: source files for the bbll analysis
 - build: directory where you compile the code (can be moved somewhere else if you know what you are doing)
 - docs: documentation
 - python: python files which can be included in other python scripts
 - setup: files to set up the analysis
 - share: this contains all the config files and python scripts specific to the analysis
   - bbll: config files for the bbll analysis
   - scripts: python scripts for studies, etc
 - tools: general python scripts which are not specific to the analysis

## Prepare

For this and the following sections the path of config files will be given relative to the `share` directory.

The main config file for the prepare step is located at `bbll/config/master/prepare.cfg`.
You start the prepare step with
```bash
prepare.py bbll/config/master/prepare.cfg
```
The pepare step is building the initial sample folder structure (for signal and background processes)
and set the correct cross-seciton normalization.

The structure is defined in the so called map (`bbll/config/samples/maps/default.map`).
Here you map DSIDs (unique identifiers for each sample) to a path of the sample folder.
The map file looks like this:

```
450030 /sig/$(channel)/$(campaign)/nonres/bbtautau
364114 /bkg/$(channel)/$(campaign)/Zee/ll
364115 /bkg/$(channel)/$(campaign)/Zee/cc
364116 /bkg/$(channel)/$(campaign)/Zee/bb
```

There are two wildcards, one for the channel and one for the campaign.

Later on we will use the channel to split the events depending on the type of the leptons in the final state.
The channels will be `ee`, `mm`, `em`, `me`, where `e` stands for electron and `m` for muon.
Right now we don't need this splitting yet so in the `prepare` and `initialize` step there is only one "dummy" channel with the name `channel`.

The campaigns wildcard is for the different MC campaigns (MC16a, MC16d, MC16e) and different data-taking years.
The table below shows how the naming scheme for campaigns in CAF.
For more information about the different campaigns in ATLAS please have a look [here](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/AtlasProductionGroupMC16).

| CAF campaign | MC campaign | data campaign |
| ------------ | ----------- | ------------- |
| c16a         | MC16a       | data15+data16 |
| c16d         | MC16d       | data17        |
| c16e         | MC16d       | data18        |

The information about the normalization is taken from the cross section file (`bbll/config/samples/XSec/XS_13TeV.csv`).
For each sample the cross-section, filter efficiency and k-factor is listed here.

## Initialize

The main config file for the initialize step is located at `bbll/config/master/initialize.cfg`.
You start the initialize step with
```bash
initialize.py bbll/config/master/initialize.cfg
```
The initialize step takes care of finding MC and data samples and getting the *initial sum of weights* information for MC samples.

The input files are configured in the campaigns config file (`bbll/config/master/campaigns.cfg`) on a per-campaign level.
The default analysis is running over samples which are stored on a grid storage disk (`UNI-FREIBURG_LOCALGROUPDISK` or `CERN-PROD_PHYS-HDBS`).
Here we cannot just do a simple `ls` to find all files. Therefore, all the file paths are stored in input file lists (`bbll/config/samples/inputFileLists`).

If you want to run over local samples you can use the following options for the campaigns config: `mcPaths`, `mcFilenamePrefix`, `mcFilenameSuffix`, `dataPaths`.
The full path for MC is built the following way: `{mcPaths}{mcFilenamePrefix}{DSID}{mcFilenameSuffix}`. The standard values are `mcFilenamePrefix=*` and `mcFilenameSuffix=*.root*`.

The *initial sum of weights* can be retrieved for MC files from either a histogram or tree (this depends on the files you want to analyze).
In CxAODs this information is stored in a histogram called `MetaData_EventCount` in bin `4`.

After the initialize step a patch file (`bbll/config/patches/initialize-post-patch.txt`) is applied to the sample folder.
The following things are done here:
 - prepare the splitting of the nonallhad ttbar sample (see section [ttbar splitting](#ttbar-splitting) below)
 - replicate the "dummy" channel to have the four normal channels
 - set some useful tags (like `isMC`, `isData`, `isSF`, `isDF`)

## Analyze

The main config file for the analyze step is located at `bbll/config/master/analyze.cfg`.
You start the analyze step with
```bash
analyze.py bbll/config/master/analyze.cfg
```
The analyze step runs over all events, applies cuts and creates histograms and/or ntuples.

### Algorithms

While running over the events the container contents can be modified, object can be sorted, and new decorations can be added.
This can be done with algorithms.

For this analysis there is one algorithm used.
It consists of a c++ part and a python snippet where you load the algorithm into CAF during runtime.
The corresponding files are located at
 - header file: `../CAFbbll/BBLLObjectSelectionAlgorithm.h`
 - source file: `../Root/BBLLObjectSelectionAlgorithm.cxx`
 - python snippet: `bbll/algorithms/BBLLObjectSelectionAlgorithm.py`

The algorithm is doing the following things:
 - apply the CxAOD overlap register. This has something to do with how systematic variations of objects are stored internally in CxAODs. For more information have a look [here](https://gitlab.cern.ch/CxAODFramework/CxAODTools/blob/master/CxAODTools/OverlapRegisterAccessor.h).
 - apply b-tagging to the jets
 - apply some object definitions. Most object definitions are applied in the CxAODFramework, but some are (re)-applied here. This is also useful for object definition studies. Currently the following object definitions are applied:
   - electrons: identification and isolation working point
   - muons: isolation working point

## Visualize

## Special procedures

### ttbar splitting

## Other scripts

### Creating ntuples for HHComb overlay checks

Submit the batch jobs with
```bash
submit.py bbll/config/master/analyze-ntuples_hhcombcrosscheck.cfg --jobs bbll/config/jobLists/hhcombcrosscheck.txt --allowArgChanges --identifier hhcombcrosscheck --queue nemo_vm_atlsch
```
Merge the ntuples with
```bash
./scripts/merge_ntuple_hhcombcrosscheck.sh
```
