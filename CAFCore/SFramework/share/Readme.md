================================================================================================
1. Introduction
================================================================================================

This is a software package for statistical analyses (in the following referred to as "statistics
code") using the HWWAnalysisCode core library (libQFramework.so) and HistFactory. Even though
the statistics code has been developed with the H->WW->lvlv analysis in mind it has been
structured in a quite general way and should be applicable to any other analysis as well.

The statistics code implements the following steps of the workflow:

- Building statistical models from input histograms or cutflow counter (which have to be provided
  in HWWAnalysisCode-like sample folders)

- Creating RooFit workspaces from statistical models

- Running various statistical analyses on RooFit workspaces

- Visualizing the results

In the context of the statistics code a statistical model is a complete description of an
analysis including information on all relevant channels (= regions, e.g. signal regions, control
regions) and all relevant signal and background samples, definitions of normalization factors,
responses to systematic variations, as well as corresponding input histograms. A statistical
model is represented by the root instance of the TQFolder class storing the aforesaid
information.

The statistics code is configured via a configuration file using the syntax defined in the
context of the text interface of the TQFolder class. The configuration file determines both the
tasks to be executed by the code as well as the corresponding parameters (see section "Tasks").


================================================================================================
2. Getting started
================================================================================================

In order to run the statistics code one needs:

- at least ROOT 5.34/03

- a compiled version of the HWWAnalysisCode core library (libQFramework.so)

  You will get it by checking out "svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/
    HiggsPhys/HSG3/WWDileptonAnalysisCode/HWWAnalysisCode/trunk" and typing "make". Please note:
  you better do a "make clean" before if you did a "svn update" since the last "make".

- a compiled version this statistics code (runHWWStats)

  You will get it by checking out "svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/
    HiggsPhys/HSG3/WWDileptonAnalysisCode/HWWAnalysisCode/trunk/analysis/Statistics" and
  typing "make". Please note: you better do a "make clean" before if you did a "svn update"
  since the last "make".

- a configuration file

  This is a file you usually have to prepare in order to tell the code what to do.

- an analysis ROOT file (ROOT file with an instance of TQSampleFolder) with an appropriate set
  of input histograms or counter

  This is another file you have to prepare usually using the analysis code (e.g.
  "analysis/HWWlvlv_2012").


The configuration file determines the tasks to be executed by the statistics code as well as the
corresponding parameters. After having prepared all inputs listed above the code is executed with

$Console> ./runHWWStats myConfigFile.txt

Depending on the configuration you will find the outputs in the corresponding directories.


There are inputs (configuration file and histogram inputs) for a simple example (more examples
to come) in the examples/ directory. This example can be run by typing in your console:

$Console> ./runHWWStats example/config_runChain_SimpleExample.txt

(more documentation on this example can be found in the corresponding configuration file.)


================================================================================================
Tasks
================================================================================================

Different tasks of the statistics code are invoked by corresponding top-level blocks in the
configuration file. Top-level blocks of currently available taks are:

- "CreateModels"

  Creates one or more (similar) models from input histograms. Specifics of the model are defined
  in the "ModelConfiguration" block within the top-level block. Example:

    +CreateModels.MyModel {
        +ModelConfiguration {
            #-- put your model configuration here --#
            # (for details on the ModelConfiguration see
            #  the documentation of the TSModelBuilder class)
        }
    }


- "ImportModels"

  Imports one or more models from external ROOT files.


- "ExportModels"

  Exports available models to external ROOT or TXT files (text files will use the text interface
  provided by the TQFolder class)


- "CreateWorkspaces"

  Creates workspaces from known models


- "ImportWorkspaces"

  Imports workspaces from external ROOT files. Example:

    +ImportWorkspaces {
		#-- list file and key names of workspaces to load as tag "inputfiles" --#
        <inputFiles = "workspace_*.root:*">
    }


- "RunStatistics"

  Runs statistical analyses on available workspaces. Currently available sub-tasks:

  - "Significance"

  - "UncertaintyBreakdown"

  - "ErrorProfile"


- "ExportWorkspaces"

  Exports available workspaces to external ROOT files


- "ExportResults"

  Exports results of statistical analyses to external ROOT or TXT files (text files will use the
  text interface provided by the TQFolder class). Example:

    +ExportResults {
		#-- list filenames to write results to as tag "outputFile" --#
        <outputFile = "result_$(Name).txt">
    }


Independent of the order of occurence in the config file the tasks are executed in the following order:

 1) CreateModels
 2) ImportModels
 3) ExportModels
 4) PlotHistograms
 5) CreateWorkspaces
 6) ImportWorkspaces
 7) RunStatistics
 8) ExportWorkspaces
 9) ImportResults
10) PlotResults
11) ExportResults


================================================================================================
Class overview
================================================================================================

- TSCLSignificanceEvaluator
- TSErrorProfile
- TSFitter
- TSHWWUtils
- TSModelBuilder
- TSModelFactory
- TSMultiModelBuilder
- TSSignificanceCalculator
- TSStatisticsCalculator
- TSStatisticsManager
- TSStatisticsPlotter
- TSUncertaintyBreakdown
