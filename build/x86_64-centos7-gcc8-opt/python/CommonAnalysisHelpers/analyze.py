import QFramework
import ROOT
from . import common
#load additional analyze relevant python classes
from .analyzeAux import *

"""this submodule contains helper functions for executing the analysis"""

def DefaultArgumentParser():
    """create a pre-configured argument parser with some default options tailored for an 'analyze' step"""
    parser = common.createBasicArgumentParser("execute your analysis on a readily initialized sample folder")
    # add some extra arguments which are helpful for this step of the analysis
    parser.add_argument('--restrict', metavar="PATHS", dest="pathselect", default="", help='restrict the analysis to a path set')
    parser.add_argument('--downmergeTo', metavar="DOWNMERGETO", dest="downmergeTo", default="", help='specify alternative downmerging targets to the paths listed at \'--restrict\'')
    parser.add_argument('--downmerge', action="store_const", const=True, default=False,
                            help='if the analysis is restricted to a path set, objects will be merged up to the paths specified in \'--restrict\'')
    parser.add_argument('--dummy', dest="dummy", action="store_const", const=True, default=False, help='run a dummy analysis (do not read data)')
    parser.add_argument('--debug', dest="debug", action="store_const", const=True, default=False,
                            help='run a debug analysis (same as "-c outputFile=debug.root maxEvents=100")')
    parser.add_argument('--robust', dest="robust", action="store_const", const=True, default=False,
                            help='run robustly over non-well-formed inputs. useful for debugging. does not support many vital analysis features.')
    parser.add_argument('--width', dest="width", type=int, default=0, help="console width for printouts")
    parser.add_argument('--jobID', dest="jobID", type=str, default="analyze", help="identifier for the job executing this instance")
    return parser

#######################################################################################################################

def loadCuts(config, aliases=QFramework.TQTaggable()):
    """load the cuts from one or more cut files"""

    # if the user has specified a templated cut file to use, use that instead of a regular cut file
    templatedCutsFilepath = str(config.getTagStringDefault("templatedCuts.templateFilepath", ""))
    templatedCutsFilepath_abs = str(common.findConfigPath(templatedCutsFilepath, False))
    if templatedCutsFilepath_abs:
        import os.path
        templateDir = os.path.dirname(templatedCutsFilepath_abs)
        templatedCutsInputsDir = str(config.getTagStringDefault("templatedCuts.inputsDir",templateDir + '../inputs/'))
        # TODO: probably there's a cleaner way to do this, but for now we pass in the relative path to the templated cut file
        #       but already the fully resolved path to the inputs dir (from the absolute path to the templated cut file)
        #       templated cut file again resolved internally
        #Note: when using the templated cuts the 'single entry-cut policy' still applies
        cutfolder = loadTemplatedCutFolder(templatedCutsFilepath, templatedCutsInputsDir)
        cutdefs = cutfolder.getFolder("?")
    else:
        # Use a regular cut file
        cutdefs = loadCutFolder(config)
    # retrieve the actual cut definitions from the folder
    #cutdefs = cutfolder.getFolder("?")
    #cutdefs.detachFromBase()

    # if no aliases directly provided,
    if aliases.getNTags() < 1:
        # see if there are any in the config
        aliases.importTagsWithoutPrefix(config,"cutParameters.")
        aliases.importTagsWithoutPrefix(config,"aliases.")
    # create compiled cuts
    cuts = QFramework.TQCut.importFromFolder(cutdefs,aliases)
    if not cuts:
        QFramework.BREAK("failed to load cuts from folder '{:s}' - please check input path".format(cutdefs.GetName()))

    # for "N-1" plots
    for cut in config.getTagVString("nullifyCuts"):
        c = cuts.getCut(cut)
        if c:
            QFramework.WARN("nullifying cut '{:s}'".format(cut))
            c.setCutExpression("1")
        else:
            QFramework.WARN("unable to find cut '{:s}' while trying to nullify".format(cut))

    return cuts

#######################################################################################################################

def loadCutFolder(config):
    """create and return a TQFolder from the cut definition files"""
    cutfolder = QFramework.TQFolder("__baseCut__") #not a name any user should use but still sort of readable
    cutfolder.setTagString(".cutExpression","1.") #pass all events, this is only a common handle for all actual (sub)cuts
    cutfolder.setTagBool(".skipJobs",True) #do not execute AnalysisJobs on this cut
    cutdeffiles = config.getTagVString("cuts")
    if len(cutdeffiles) < 1:
        QFramework.BREAK("no cuts given!")
    for cutdeffile in cutdeffiles:
        cutdeffile_abs = ROOT.TString(common.findConfigPath(cutdeffile))
        QFramework.INFO("loading cuts from '{:s}'".format(cutdeffile.Data()))
        ok = False
        if cutdeffile_abs.EndsWith(".py"):
          ok = loadPyCuts(config,cutfolder,cutdeffile_abs)
        else:
          ok = cutfolder.importFromTextFile(cutdeffile_abs)


        if not ok:
            QFramework.BREAK("unable to load cuts from '{:s}' ".format(cutdeffile.Data()))

    return cutfolder

#######################################################################################################################

def loadPyCuts(config, basecut, path):
    """import and call a python/PyCut based cut definition file."""

    abs_path = common.findConfigPath(path)

    # continue only if there was one match found
    if len(abs_path) == 0:
        QFramework.BREAK("No file '{:s}' found, please check your configuration!".format(abs_path))

    import imp
    import inspect
    import os

    module_name = os.path.basename(abs_path).rstrip(".py")
    try:
        addcuts = imp.load_source(module_name, abs_path)

        argspec = inspect.getargspec(addcuts.addCuts)
        if 'config' in argspec.args and 'baseCut' in argspec.args:
            added = addcuts.addCuts(baseCut = PyCut(basecut), config=config)
        elif len(argspec.args) == 2:
            added = addcuts.addCuts(config,PyCut(basecut))
        elif len(argspec.args) == 1:
            added = addcuts.addCuts(PyCut(basecut))
        else:
            QFramework.BREAK("unable to add cuts(s) from script '{:s}' - unknown arguments appeared: {:s}".format(abs_path, str(argspec.args)))
        if not added and not (added is None):
            QFramework.BREAK("The cut definition snippet '{:s}' indicated an error, please check potential errors reported above".format(abs_path))
    except IOError as error:
        QFramework.BREAK("unable to open file '{:s}' - please double-check!\n".format(abs_path)+"Message from python:\n"+str(error))
    except NameError as error:
        QFramework.BREAK("syntax error in cut snippet '{:s}' - please double-check!\n".format(abs_path)+"Message from python:\n"+str(error))
    except AttributeError as error:
        QFramework.BREAK("attribute error in cut definition '{:s}' - please double-check!\n".format(abs_path)+
                         "If the message from python below is\n'module' object has no attribute 'addCuts'\nplease make sure that the snippet has the function addCuts(baseCut) or addCuts(config,baseCut) defined.\n"
                         "Message from python:\n"+str(error))
    return True

#######################################################################################################################

def loadTemplatedCutFolder(filepath, inputsDir):
    """create and return a TQFolder out of a cuts template which references one or more input files
        This allows more modular cut files in allowing the user to attach cut sequences to different cuts
        in different analyses.
        See cuts/ZH/templatized/ for working examples of the configuration format expected by this function
        TODO: remove the need to reference ZH specific code
    """
    cutfolder = QFramework.TQFolder("cuts")

    filepath_abs = common.findConfigPath(filepath)

    with open(filepath_abs, 'r') as templateFile:
        template = templateFile.read()

    # Keywords found in the template file are used as the names of the files containing the inputs
    inputs = common.findFormatKeywords(template)

    # Reads the inputs and plugs them into the template
    inputDict = {}
    for input in inputs:
        with open(inputsDir+input+".dent", "r") as inputFile:
            inputDict[input] = inputFile.read()

    ok = cutfolder.importFromText(template % inputDict)
    if not ok:
        QFramework.BREAK("unable to load templated cuts from '{:s}' ".format(filepath.Data()))
    return cutfolder

#######################################################################################################################

def bookAnalysisJobs(config, cuts, aliases=QFramework.TQTaggable()):
    """book a set of analysis jobs from the corresponding config files"""

    # boolean variable to keep track of whether we are using cutbased or MVA
    mva = config.getTagVString("MVA")
    cutbased = (len(mva)==0)

    # if no aliases directly provided,
    if aliases.getNTags() < 1:
        # see if there are any in the config
        aliases.importTagsWithoutPrefix(config,"cutParameters.")
        aliases.importTagsWithoutPrefix(config,"aliases.")

    # TODO: modularize booking of each type of analysis job?

    #book cutflows
    if config.getTagBoolDefault("cutbased.makeCutflow",cutbased):
        QFramework.INFO("booking cutflow")
        cutflowjob = QFramework.TQCutflowAnalysisJob("cutflowJob")
        cuts.addAnalysisJob(cutflowjob,"*")
        cutbased = True

    # TODO: book xAOD skimming here? (worst case just implement in HWW)
    xAODdumpingConfig = QFramework.TQTaggable()
    dumpXAODs = (xAODdumpingConfig.importTagsWithoutPrefix(config,"xAODdumping.") > 0)
    if dumpXAODs :
        try:
            flaggingJob = ROOT.TQEventFlaggingAnalysisJob()
            if xAODdumpingConfig.hasTag("flagName"): flaggingJob.setFlagName(xAODdumpingConfig.getTagStringDefault("flagName",""))
            flaggingCuts = xAODdumpingConfig.getTagStringDefault("cuts","")
            print("Booking event flagging jobs at cuts: {:s}".format(flaggingCuts.Data()))
            cuts.addAnalysisJob(flaggingJob,flaggingCuts)
        except NameError:
            QFramework.ERROR("Cannot schedule xAOD dumping, required classes are not in your version of CAFCore. Please consider updating CAFCore")

    # add the event flagging for possible unfolding
    unfoldingConfig = QFramework.TQTaggable()
    unfolding = (unfoldingConfig.importTagsWithoutPrefix(config,"unfolding.") > 0)
    if unfolding :
        #add a suffix to the cut names for the flags. This is needed to prevent cross talk between channels!
        unfoldingFlagSuffix = config.getTagStandardStringDefault("~flagSuffix","_$(cand)")
        unfoldingCuts = unfoldingConfig.getTagVString("flagcuts")
        for cutName in unfoldingCuts:
            fullCutNames = cuts.getCutNames(cutName)
            for fullCutName in fullCutNames:
                unfoldingJob = ROOT.TQEventFlaggingAnalysisJob()
                flagName = fullCutName + unfoldingFlagSuffix
                unfoldingJob.setFlagName(flagName)
                cuts.addAnalysisJob(unfoldingJob,fullCutName)


    #book histograms (TH1, TH2, TH3, TProfiles,...)
    if ( config.hasTag("histograms.0") or config.hasTag("histograms") ) and config.getTagBoolDefault("makeHistograms",cutbased):
        QFramework.INFO("booking histograms")
        histofiles = config.getTagVString("histograms")
        histofiles = common.findMultipleConfigPathsFromList(histofiles)
        if QFramework.TQHistoMakerAnalysisJob.importJobsFromTextFiles(histofiles,cuts,aliases,"*",config.getTagBoolDefault("printHistograms",False)) > 0:
            print(QFramework.TQHistoMakerAnalysisJob.getErrorMessage())
            cutbased = True

    #book multi-dim histograms (THn based, i.e., for nDim>3)
    if ( config.hasTag("multidimHistograms") ) and config.getTagBoolDefault("makeHistograms",cutbased):
        QFramework.INFO("booking multidimensional histograms")
        histofiles = config.getTagVString("multidimHistograms")
        histofiles = common.findMultipleConfigPathsFromList(histofiles)
        if QFramework.TQTHnBaseMakerAnalysisJob.importJobsFromTextFiles(histofiles,cuts,aliases,"*",config.getTagBoolDefault("printHistograms",False)) > 0:
            print(QFramework.TQTHnBaseMakerAnalysisJob.getErrorMessage())
            cutbased = True

    #book graphs
    if ( config.hasTag("graphs.0") or config.hasTag("graphs") ) and config.getTagBoolDefault("makeGraphs",cutbased):
        QFramework.INFO("booking graphs")
        graphfiles = config.getTagVString("graphs")
        graphfiles = findMultipleConfigPathsFromList(graphfiles)
        if QFramework.TQGraphMakerAnalysisJob.importJobsFromTextFiles(graphfiles,cuts,aliases,"*",config.getTagBoolDefault("printGraphs",False)) > 0:
            print(QFramework.TQGraphMakerAnalysisJob.getErrorMessage())
            cutbased = True

    #book event lists
    if ( config.hasTag("eventlists.0") or config.hasTag("eventlists") ) and config.getTagBoolDefault("makeEventLists",cutbased):
        QFramework.INFO("booking eventlists")
        evtlistfiles = config.getTagVString("eventlists")
        evtlistfiles = common.findMultipleConfigPathsFromList(evtlistfiles)
        if QFramework.TQEventlistAnalysisJob.importJobsFromTextFiles(evtlistfiles,cuts,aliases,"*") > 0:
            print(QFramework.TQEventlistAnalysisJob.getErrorMessage())
            cutbased = True

    #book Ntuple dumping
    if ( config.hasTag("ntuples.0") or config.hasTag("ntuples") ) and config.getTagBoolDefault("dumpNtuples",cutbased):
        QFramework.INFO("preparing to dump ntuples")
        ntupfiles = config.getTagVString("ntuples")
        ntupfiles = common.findMultipleConfigPathsFromList(ntupfiles)
        if QFramework.TQNTupleDumperAnalysisJob.importJobsFromTextFiles(ntupfiles,cuts,aliases,"*",config.getTagBoolDefault("printNTuples",False)) > 0:
            print(QFramework.TQNTupleDumperAnalysisJob.getErrorMessage());
            cutbased = True

    runtime = config.getFolder("runtime+")
    runtime.setTagBool("cutbased", cutbased)

    return

#######################################################################################################################

def prepareSystematicsExample(config, samples):
    """prepare the systematic handling for your analysis"""
    QFramework.WARN("this is just an example implementation of the systematics handling")
    QFramework.WARN("you will need to tailor the systematics handling to fit the needs of your analysis!")

    CLI = config.getFolder("CLI+")
    # flag indicating to run a robust analysis
    robust = CLI.getTagBoolDefault("robust",False)
    # flag indicating to run a dummy analysis
    dummy = CLI.getTagBoolDefault("dummy",False)

    if not robust and not dummy:
        subfolders = samples.getListOfSampleFolders("?")
        doNominal = config.getTagBoolDefault("xAODdoNominal",True)
        channels = config.getTagVStandardString("channels")
        mcasvchannels = set([ c for c in channels ])
        for c in channels:
            for sf in subfolders:
                if sf.getTagBoolDefault("isData",False):
                    # we're in a data SampleFolder
                    # just set some nominal tags for this channel subfolder
                    f = sf.getSampleFolder(c)
                    if not f: continue
                    f.setTagString(".mcasv.channel",f.getTagStringDefault("channel",""))
                    f.setTagString("weightname","nominal")
                    f.setTagString("p4suffix","")
                    continue
                # we're in a MC SampleFolder
                f = sf.getSampleFolder(c)
                if not f:
                    QFramework.WARN("unable to retrieve sample folder '{:s}' from '{:s}'. Is this expected from your path restriction?".format(c,sf.GetName()))
                    sf.printContents()
                    continue #if this is not a valid folder, don't do anything with it, you'll get a null pointer exception otherwise!

                # right now, we are treating p4 systematics and sf systematics as uncorrelated
                # we do only look at the diagonal terms where one systematic is activated
                # crossover terms are neglected for now, hence we have two separate loops
                for p4syst in config.getTagVStandardString("xAODp4Systematics"):
                    # for each p4 systematic, copy the channel folder
                    if not p4syst or len(p4syst)<1: continue
                    newname=c+"_"+p4syst
                    newf = f.copy(newname)
                    if not newf:
                        QFramework.BREAK("unable to copy folder {:s} to new name {:s}".format(f.GetName(),newname))
                    sf.addFolder(newf)
                    # set the appropriate tags
                    candname = f.getTagStringDefault("~cand","")
                    if not candname:
                        QFramework.BREAK("didn't find 'cand' tag on sample folder '{:s}'".format(f.getPath()))
                    newf.setTagString("p4Variation",p4syst)
                    newf.setTagString("weightname","nominal")
                    newf.setTagString("cand",candname+"___"+p4syst)
                    newf.setTagString("cand.original",candname)
                    newf.setTagString(".mcasv.channel",newname)
                    newf.setTagString("p4suffix",ROOT.TString("___")+p4syst)
                    newf.setTagString("sysVariation",p4syst)
                    mcasvchannels.add(newname.Data())
                for sfsyst in config.getTagVString("xAODsfSystematics"):
                    # for each sf systematic, copy the channel folder
                    if not sfsyst or len(sfsyst)<1: continue
                    newname = c+"_"+sfsyst
                    newf = f.copy(newname)
                    if not newf:
                        QFramework.BREAK("unable to copy folder {:s} to new name {:s}".format(f.GetName(),newname))
                    sf.addFolder(newf)
                    # set the appropriate tags
                    newf.setTagString("sfVariation",sfsyst)
                    newf.setTagString("weightname",sfsyst)
                    newf.setTagString(".mcasv.channel",newname)
                    newf.setTagString("p4suffix","")
                    newf.setTagString("sysVariation",sfsyst)
                    mcasvchannels.add(newname.Data())

                # if no nominal analysis was requested, we can remove the nominal channels
                if not doNominal:
                    f.detachFromBase()
                else:
                    f.setTagString(".mcasv.channel",f.GetName())

    # Add some nominal top level tags, even if systematics aren't being added
    samples.setTagString("weightname","nominal")
    samples.setTagString("sfVariation","nominal")
    samples.setTagString("p4Variation","nominal")
    samples.setTagString("p4suffix","")
    samples.setTagString("sysVariation","nominal")

    # possibly print how the folder looks like now
    if config.getTagBoolDefault("showChannels",False):
        QFramework.INFO("after taking care of channel and systematics setup, your sample folder looks like this:")
        samples.printContents("r2dt")

    # save the whole collection of channels (including now systematics)
    # for access later when creating the MCASV
    runtime = config.getFolder("runtime+")
    for i, channel in enumerate(mcasvchannels):
        if "ROOT" in str(type(channel)):
            from QFramework import WARN
            QFramework.WARN("Please make sure that every mcasvchannel is only booked once. The python set doesn't help when adding c++ ROOT instances (" + str(type(channel)) + ".")
        runtime.setTagString("mcasvchannels."+str(i), channel)

    return

#######################################################################################################################

def loadObservables(config):
    """load a set of observables from python snippets"""
    import imp
    import inspect
    import os

    CLI = config.getFolder("CLI+")
    robust = CLI.getTagBoolDefault("robust",False)
    dummy = CLI.getTagBoolDefault("dummy",False)

    # set global property
    if config.getTagBoolDefault("evaluateSubObservablesLazily", False):
        QFramework.TQMultiObservable.gEvaluateSubObservablesLazily = True

    customobservables = False
    # for each snippet,
    for observablescript_TString in config.getTagVString("customObservables.snippets"):
        QFramework.TQStringUtils.removeLeadingBlanks(observablescript_TString)
        QFramework.TQStringUtils.removeTrailingBlanks(observablescript_TString)
        QFramework.TQStringUtils.removeTrailingText(observablescript_TString, ".py")
        observablescript = observablescript_TString.Data()
        if len(observablescript) == 0:
            QFramework.INFO("Skipping custom observable loading - no snippets provided")
            continue
        found_modules = []
        observablesDirs = config.getTagVStandardString("customObservables.directories")
        # search through the directories provided in the config
        for observablesPath in observablesDirs:
            module = QFramework.TQFolder.concatPaths(observablesPath, observablescript)+".py"
            module = common.findConfigPath(module, False, True)
            # findConfigPath returns "" if no module was found
            if len(module) > 0:
                # snippet was found in this directory -
                # add it's absolute path and the directory it was found in
                # to a list in tuple form
                found_modules.append((module,observablesPath))
        if len(found_modules) == 0:
            # check CommonAnalysisHelpers for an observable snippet as fall-back
            CAHObservablesDir = "CommonAnalysisHelpers/share/observables"
            observablesDirs.push_back(CAHObservablesDir)
            module = QFramework.TQFolder.concatPaths(CAHObservablesDir, observablescript) + ".py"
            module = QFramework.TQPathManager.findFileFromEnvVarWithoutExecDir(module, "CAFCOREDIR", False)
            if len(module) > 0:
                found_modules.append((module, CAHAlgorithmsDir))

        # continue only if there was one match found
        if len(found_modules) == 0:
            QFramework.BREAK("No module found for '{:s}' in the custom observable directories provided:\n{:s}\n".format(observablescript,', '.join(observablesDirs))+
                             "Please make sure that there exists a snippet by the name of '{:s}.py' available in one of them.\n".format(observablescript))
        elif len(found_modules) > 1:
            QFramework.BREAK("Ambiguity detected while resolving custom observable snippet location. Multiple modules found for {:s} in the custom observable directories provided:\n{:s}\n".format(observablescript,', '.join(observablesDirs))+
                             "Consider placing the {:s}.py snippet only in a common directory if it's used by more than one (sub)analysis.".format(observablescript))
        abs_path = found_modules[0][0]
        module_name = os.path.basename(abs_path).rstrip(".py")
        relative_path = QFramework.TQFolder.concatPaths(found_modules[0][1], observablescript)+".py"
        QFramework.START("l.","loading custom observable instances from the '{:s}' snippet".format(str(relative_path)))
        try:
            addobservables = imp.load_source(module_name, abs_path)

            argspec = inspect.getargspec(addobservables.addObservables)
            if 'config' in argspec.args:
                added = addobservables.addObservables(config=config)
            elif len(argspec.args) == 1:
                added = addobservables.addObservables(config)
            elif len(argspec.args) == 0:
                added = addobservables.addObservables()
            else:
                QFramework.BREAK("unable to add observable(s) from script '{:s}' - unknown arguments appeared: {:s}".format(abs_path, str(argspec.args)))
            if added:
                QFramework.END(QFramework.TQMessageStream.OK)
                customobservables = True
            else:
                QFramework.END(QFramework.TQMessageStream.FAIL)
                QFramework.BREAK("unable to properly setup custom observables from '{:s}'".format(abs_path))
        except IOError as error:
            QFramework.END(QFramework.TQMessageStream.FAIL)
            QFramework.BREAK("unable to open file '{:s}' - please double-check!\n".format(abs_path)+"Message from python:\n"+str(error))
        except NameError as error:
            QFramework.END(QFramework.TQMessageStream.FAIL)
            if not robust and not dummy:
                QFramework.BREAK("syntax error in observable snippet '{:s}' - please double-check!\n".format(abs_path)+"Message from python:\n"+str(error))
        except AttributeError as error:
            QFramework.END(QFramework.TQMessageStream.FAIL)
            if not robust and not dummy:
                QFramework.BREAK("attribute error in observable snippet '{:s}' - please double-check!\n".format(abs_path)+
                                 "If the message from python below is\n'module' object has no attribute 'addObservables'\nplease make sure that the snippet has the function addObservables() defined.\n"
                                 "Message from python:\n"+str(error))
    return customobservables

#######################################################################################################################

def createAnalysisSampleVisitor(config, cuts):
    """create a new sample visitor based on a configuration"""

    # TODO: warn user if this function is called but 'cutbased' is false? (or no jobs yet booked - must do this first if a cutbased analysis is desired)

    # read the channel definitions
    channels = config.getTagVString("channels")

    CLI = config.getFolder("CLI+")
    # flag indicating to run a robust analysis
    robust = CLI.getTagBoolDefault("robust",False)
    # flag indicating to run a dummy analysis
    dummy = CLI.getTagBoolDefault("dummy",False)

    if not config.getTagBoolDefault("useMultiChannelVisitor",False) or robust or dummy:
        # using regular analysis sample visitor (default)
        visitor = QFramework.TQAnalysisSampleVisitor()
        visitor.setVerbose(True)
        visitor.setBaseCut(cuts)
        visitor.setPrettyPrint(config.getTagBoolDefault("prettyPrint",True))
        visitor.setLineUpdates(config.getTagBoolDefault("lineUpdates",True))
        visitor.setTagDouble("progressInterval",config.getTagDoubleDefault("progressInterval",5.))
    else:
        # using fast MultiChannel analysis sample visitor
        visitor = QFramework.TQMultiChannelAnalysisSampleVisitor()
        visitor.setVerbose(True)
        visitor.setPrettyPrint(config.getTagBoolDefault("prettyPrint",True))
        visitor.setLineUpdates(config.getTagBoolDefault("lineUpdates",True))
        visitor.setTagDouble("progressInterval",config.getTagDoubleDefault("progressInterval",5.))

        runtime = config.getFolder("runtime+")
        # TODO: add some protection against not finding the mcasvchannels in the runtime config for whatever reason
        mcasvchannels = runtime.getTagVStandardString("mcasvchannels")

        # TODO: cutlist was defined in runAnalysis.py, but aparently not used
        #cutlist = []
        for channel in mcasvchannels:
            cut = cuts.getClone()
            #cutlist.append(cut)
            visitor.addChannel(channel,cut)
            # TODO: used previously just as a list for cloneObservablesSmart
            # mcvchannels.append(channel)
        if config.getTagBoolDefault("showChannels",False):
            visitor.printChannels()

        # TODO: SmartObservableCloning not yet migrated to CAFExample (or fully implemented?) - Initial author = Carsten
        #       Safe for this to go here? i.e. only for MCASV and before analysis algorithms are attached
        cloneObservablesSmart = False
        if config.getTagBoolDefault("reduceMCVObservables",False):
            try:
                from CAFExample.SmartObservableCloning import cloneSetSmart
                cloneObservablesSmart = True
            except ImportError:
                cloneObservablesSmart = False
                QFramework.ERROR("smart observable cloning unavailable, skipping")
        if  cloneObservablesSmart:
            for channel in mcasvchannels:
                QFramework.TQObservable.getManager().cloneActiveSet(channel)

    return visitor

#######################################################################################################################

def bookAlgorithms(config, visitor):
    """load a set of algorithms from python snippets and add them to the visitor"""
    import imp
    import os

    CLI = config.getFolder("CLI+")
    # flag indicating to run a robust analysis
    robust = CLI.getTagBoolDefault("robust",False)
    # flag indicating to run a dummy analysis
    dummy = CLI.getTagBoolDefault("dummy",False)

    # book any algorithms
    for algorithmscript_TString in config.getTagVString("algorithms.snippets"):
        QFramework.TQStringUtils.removeLeadingBlanks(algorithmscript_TString)
        QFramework.TQStringUtils.removeTrailingBlanks(algorithmscript_TString)
        QFramework.TQStringUtils.removeTrailingText(algorithmscript_TString, ".py")
        algorithmscript = algorithmscript_TString.Data()
        found_modules = []
        algorithmsDirs = config.getTagVStandardString("algorithms.directories")
        # search through the directories provided in the config
        for algorithmsPath in algorithmsDirs:
            module = QFramework.TQFolder.concatPaths(algorithmsPath, algorithmscript) + ".py"
            module = common.findConfigPath(module, False)
            # findConfigPath returns "" if no module was found
            if len(module) > 0:
                # snippet was found in this directory -
                # add its absolute path and the directory it was found in
                # to a list in tuple form
                found_modules.append((module, algorithmsPath))
        if len(found_modules) == 0:
            # check CommonAnalysisHelpers for an algorithm snippet as fall-back
            CAHAlgorithmsDir = "CommonAnalysisHelpers/share/algorithms"
            algorithmsDirs.push_back(CAHAlgorithmsDir)
            module = QFramework.TQFolder.concatPaths(CAHAlgorithmsDir, algorithmscript) + ".py"
            module = QFramework.TQPathManager.findFileFromEnvVarWithoutExecDir(module, "CAFCOREDIR")
            if len(module) > 0:
                found_modules.append((module, CAHAlgorithmsDir))
        print(len(found_modules))
        # continue only if there was one match found
        if len(found_modules) == 0:
            QFramework.BREAK("No module found for '{:s}' in the custom algorithm directories provided:\n{:s}\n".format(algorithmscript,', '.join(algorithmsDirs))+
                             "Please make sure that there exists a snippet by the name of '{:s}.py' available in one of them.\n".format(algorithmscript))
        elif len(found_modules) > 1:
            QFramework.BREAK("Ambiguity detected while resolving custom algorithm snippet location. Multiple modules found for {:s} in the custom algorithm directories provided:\n{:s}\n".format(algorithmscript,', '.join(algorithmsDirs))+
                             "Consider placing the {:s}.py snippet only in a common directory if it's used by more than one (sub)analysis.".format(algorithmscript))
        abs_path = found_modules[0][0]
        module_name = os.path.basename(abs_path).rstrip(".py")
        relative_path = QFramework.TQFolder.concatPaths(found_modules[0][1], algorithmscript)+".py"
        QFramework.START("l.","loading algorithms from '{:s}'".format(str(relative_path)))
        try:
            addalgorithms = imp.load_source(module_name, abs_path)
            added = addalgorithms.addAlgorithms(visitor,config)
            if added:
                QFramework.END(QFramework.TQMessageStream.OK)
            else:
                QFramework.END(QFramework.TQMessageStream.FAIL)
                QFramework.BREAK("unable to properly setup custom algorithms")
        except IOError as error:
            QFramework.END(QFramework.TQMessageStream.FAIL)
            QFramework.BREAK("unable to open file '{:s}' - please double-check!\n".format(abs_path)+"Message from python:\n"+str(error))
        except NameError as error:
            QFramework.END(QFramework.TQMessageStream.FAIL)
            if not robust and not dummy:
                QFramework.BREAK("syntax error in algorithm snippet '{:s}' - please double-check!\n".format(abs_path)+"Message from python:\n"+str(error))
        except AttributeError as error:
            QFramework.END(QFramework.TQMessageStream.FAIL)
            if not robust and not dummy:
                QFramework.BREAK("attribute error in algorithm snippet '{:s}' - please double-check!\n".format(abs_path)+
                                 "If the message from python below is\n'module' object has no attribute 'addAlgorithms'\nplease make sure that the snippet has the function addAlgorithms() defined.\n"
                                 "Message from python:\n"+str(error))

    # only try and do the xAOD skimming configuration below if we are running with the MCASV
    # since all channels should be considered at the same time (e.g. systematic variations)
    if isinstance(visitor,QFramework.TQMultiChannelAnalysisSampleVisitor):
        # TODO: these two lines are also done in bookAnalysisJobs
        xAODdumpingConfig = QFramework.TQTaggable()
        dumpXAODs = (xAODdumpingConfig.importTagsWithoutPrefix(config,"xAODdumping.") > 0)

        jobID = CLI.getTagStringDefault("jobID","analyze")

        #add xAODskimmingAlgorithm if requested (only for MCASV as we'd have event duplications otherwise!)
        #note: if we ever implement an option to limit the number of channels executed at the same time we must ensure this does not run in such a configuration!!!!
        if dumpXAODs:
            print("Setting up xAOD skimming Algorithm...")
            xAODskimmingAlg = ROOT.TQxAODskimmingAlgorithm()
            xAODskimmingAlg.SetName("xAODdumper")
            if xAODdumpingConfig.hasTag("flagName"): xAODskimmingAlg.setFlagName(xAODdumpingConfig.getTagStringDefault("flagName",""))
            xAODskimmingAlg.setOutputDir( xAODdumpingConfig.getTagStringDefault("outputDir","CAFxAODs") )
            xAODskimmingAlg.setFilePrefix(jobID+"_")
            if config.hasTag("nameTagName") : xAODskimmingAlg.setPrefix( config.getTagStringDefault( ROOT.TString("aliases.")+config.getTagStringDefault("nameTagName",""), "" ) )
            visitor.addAlgorithm( xAODskimmingAlg )

#######################################################################################################################

def executeAnalysis(config, samples, visitor):
    """execute the event loop to produce histograms, counters and other reduced data formats from your events"""
    # store cuts in "info" (re-created from TQCuts)
    # ROOT.xAOD.clearTransientTrees()
    #nEventsProcessed = 0

    CLI = config.getFolder("CLI+")

    # flag indicating to run analysis in debug mode
    debug = CLI.getTagBoolDefault("debug",False)
    # flag indicating to run a dummy analysis
    dummy = CLI.getTagBoolDefault("dummy",False)

    downmerge   = CLI.getTagBoolDefault("downmerge",False)
    downmergeTo = CLI.getTagStandardStringDefault("downmergeTo","")

    pathselect = CLI.getTagVStandardString("pathselect")

    if debug:
        maxEvents = 100
    else:
        maxEvents = config.getTagIntegerDefault("maxEvents",-1)

    # proceed with analysis
    appname = QFramework.TQLibrary.getApplicationName().Data()
    visitor.setVisitTraceID(appname)
    if maxEvents > 0:
        QFramework.WARN("setting maximum number of events per sample to {:d}".format(maxEvents))
        visitor.setMaxEvents(maxEvents)
    QFramework.TQLibrary.allowRedirection(False)
    timer = ROOT.TStopwatch()
    nsamples = 0
    if pathselect.size() > 0:
        paths = ROOT.TString(",".join(map(str,pathselect)))
    else:
        # Read in sample folder restrictions and convert to a single comma-
        # separated string, the same format as it would be passed in via CLI.
        # Can't use `join` since this is a vector<TString>
        # Can't read in the field as a single string with getTagString,
        # perhaps since it has commas
        paths = ""
        for path in config.getTagVString("restrict"):
            paths += path.Data() + ","
        paths = ROOT.TString(paths[:-1])
    if paths.Length() != 0:
        if not dummy:
            nsamples = samples.visitSampleFolders(visitor,paths)
            QFramework.TQLibrary.recordMemory()
            QFramework.TQObservable.clearAll()
            QFramework.TQLibrary.recordMemory()
            if downmerge or downmergeTo:
                downmergeTargets = downmergeTo
                if not downmergeTargets:
                    downmergeTargets = paths
                samples.setTag(".generalize.histograms",True,downmergeTargets)
                samples.setTag(".generalize.cutflow",True,downmergeTargets)
        else:
            QFramework.WARN("dummy run, skipping execution of cutbased analysis on paths '{:s}'".format(pathselect))
    else:
        if not dummy:
            nsamples = samples.visitMe(visitor)
            QFramework.TQLibrary.recordMemory()
        else:
            QFramework.WARN("dummy run, skipping execution of cutbased analysis on root sample folder")

    # TODO: put the rest of this in a separate function like for post processing?
    #       right now nsamples is returned but nothing is done with it
    if nsamples > 0:
        if downmerge or downmergeTo:
            samples.generalizeObjects(".generalize")
    timer.Stop()

    # TODO: put this section in its own function (with cuts available)
    #       just get cuts from visitor? (will need to provide a channel in the MCASV case I think)
    if config.getTagBoolDefault("checkRun",True):

        if dummy:
            allevents = QFramework.TQCounter("dummy",0,0,0)
        else:
            if isinstance(visitor,QFramework.TQAnalysisSampleVisitor):
                allevents = samples.getCounter(".",visitor.getBaseCut().GetName())
            elif isinstance(visitor,QFramework.TQMultiChannelAnalysisSampleVisitor):
                channels = config.getTagVString("channels")
                allevents = samples.getCounter(".",visitor.getBaseCut(channels[0]).GetName())

        if nsamples > 0:
            # debugging printout
            # TODO: make separate method?
            if config.getTagBoolDefault("printCounterValues",False):
                samples.printListOfCounters()
            printhists = config.getTagVString("printHistogramsASCII")
            for hist in printhists:
                h = samples.getHistogram(".",hist)
                if h:
                    QFramework.TQHistogramUtils.printHistogramASCII(h)
                else:
                    QFramework.ERROR("unable to access histogram '{:s}'".format(hist))

        else:
            QFramework.ERROR("execution of analysis finished but might have failed, no samples were visited successfully (they might simply be empty).")
            runtime = config.getFolder("runtime+")
            # store in runtime folder the fact that no samples were visited in the form of an error string
            analysisError = "execution of analysis finished but might have failed, no samples were visited successfully (they might simply be empty)."
            runtime.setTagString("analysisError", analysisError)
            #don't quit just now, but instead we'll write an alternative output file later which basically states "job didn't crash but there is a small chance something went wrong"
            #quit()

    #return nEventsProcessed
    return nsamples

#######################################################################################################################

def loadLibMVA(config):
    """load MVA library provided by the config"""
    mvalibpath = config.getTagStringDefault("libMVA","libTMVA.so")
    QFramework.START("l.","attempting to load MVA library from '{:s}'".format(mvalibpath.Data()))
    try:
        ROOT.gSystem.Load(mvalibpath.Data())
        QFramework.END(QFramework.TQMessageStream.OK)
        return True
    except:
        QFramework.END(QFramework.TQMessageStream.FAIL)
        QFramework.BREAK("unable to load MVA library - please specify the proper path via the 'libMVA' config option")
        return False

#######################################################################################################################

def trainMVA(config, samples, cuts):
    """train one or more TMVA methods on your events"""

    # cutbased set to true during runtime if running of a cutbased analysis has already been detected
    # i.e. originally cutbased = (len(mva)==0) from bookAnalysisJobs and is set to true if any analysis job is added to the provided cuts
    runtime = config.getFolder("runtime+")
    cutbased = runtime.getTagBoolDefault("cutbased", False)

    mvascriptnames = config.getTagVString("MVA")
    mvaOK = False
    if len(mvascriptnames)>0:
        for mvaconfig in mvascriptnames:
            mvascriptname = ROOT.TString()
            QFramework.TQStringUtils.readUpTo(mvaconfig,mvascriptname,"(")
            QFramework.TQStringUtils.removeLeadingText(mvaconfig,"(")
            QFramework.TQStringUtils.removeTrailingText(mvaconfig,")")
            path = QFramework.TQFolder.concatPaths("MVA",mvascriptname).Data() + ".py"
            if (mvaconfig):
                QFramework.INFO("now running analysis '{:s}' with options '{:s}'".format(mvascriptname,mvaconfig))
            else:
                QFramework.INFO("now running analysis '{:s}'".format(mvascriptname))
            allOK = True

            try:
                myMVA = imp.load_source("myMVA",path)
            except IOError:
                QFramework.CRITICAL("unable to open file '{:s}' - please double-check!".format(path))
                allOK = False

            if allOK:
                try:
                    QFramework.TQUtils.ensureDirectory("weights")
                    tqmva = QFramework.TQMVA(samples)
                    tqmva.setBaseCut(cuts)
                    tqmva.setName(mvascriptname)
                    tqmva.setTitle(mvascriptname)
                    tqmva.setAliases(aliases)
                    tqmva.importTags(mvaconfig,False)
                    timer = ROOT.TStopwatch()
                    if not dummy:
                        retval = myMVA.runMVA(tqmva)
                    else:
                        retval = True
                        QFramework.WARN("dummy run, skipping execution of MVA analysis '{:s}'".format(tqmva.GetName()))
                    timer.Stop()
                    if retval:
                        QFramework.INFO("analysis '{:s}' complete after {:.2f}s, output written to '{:s}'!".format(mvascriptname,timer.RealTime(),tqmva.getTagStringDefault("outputFileName","<unknown file>")))
                        mvaOK = True
                    else:
                        QFramework.WARN("analysis '{:s}' returned '{:s}' - please double-check!".format(mvascript,str(retval)))
                except Exception as ex:
                    template = "An exception of type '{0}' occured: {1!s}"
                    message = template.format(type(ex).__name__, ",".join(map(str,ex.args)))
                    QFramework.ERROR(message)
                    allOK = False

    elif not cutbased:
        appname = QFramework.TQLibrary.getApplicationName().Data()
        QFramework.ERROR("no analysis script given, please use 'MVA: myAnalysis' under the [{:s}] section to import and execute some python script 'MVA/myAnalysis.py'. it should contain a function 'runMVA(...)' that will receive a readily prepared sample folder at your disposal".format(appname))

    # store in runtime folder whether or not at least 1 MVA finished successfully
    runtime = config.getFolder("runtime+")
    runtime.setTagString("mvaOK", mvaOK)

    return
