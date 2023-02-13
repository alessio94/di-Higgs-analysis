from CommonAnalysisHelpers import defaultConfig

try:
    from QFramework import ERROR,INFO,BREAK,WARN
except ImportError:
    def ERROR(arg):
        print(arg)
    def BREAK(arg):
        print(arg)
    def INFO(arg):
        print(arg)
    def WARN(arg):
        print(arg)


"""This submodule contains common helper functions to handle QFramework objects, especially sample folders"""

def createEmptyArgumentParser(description):
    """Create an empty argument parser"""
    from argparse import ArgumentParser
    parser = ArgumentParser(description=description, conflict_handler='resolve') # conflict_handler: allow to overwrite existing options with the same name
    return parser

#######################################################################################################################

def configureBasicArgumentParser(parser):
    """Pre-configure an argument parser with some default options"""
    parser.add_argument("config",type=str,nargs="+",metavar="CONFIGFILE.CFG",help="configuration file to be read")
    parser.add_argument("--options",dest="options",type=str,nargs="+",default=[],metavar="key=value",help="changes to be made to the configuration")
    parser.add_argument("--print-config",dest="printConfig",action='store_true',help="print the final configuration")
    parser.add_argument("--write-config-as-cfg",dest="writeConfigCfg",type=str,default="",metavar="file.cfg",help="write the final configuration to some file (in standard CFG format)")
    parser.add_argument("--write-config-as-tqfolder",dest="writeConfigTQFolder",type=str,default="",metavar="file.txt",help="write the final configuration to some file (in TQFolder format)")
    parser.add_argument('--snowflakeThresh', type=int, default=-1, help='If specified, a runtime error is raised at the end of the job if a status message of type ERROR or worse has been detected. The provided integer is the maximum number of WARNING messages allowed without triggering a runtime error.')

#######################################################################################################################

def createBasicArgumentParser(description):
    """Create a pre-configured argument parser with some default options"""
    parser = createEmptyArgumentParser(description)
    configureBasicArgumentParser(parser)
    return parser

#######################################################################################################################

def createConfigParser(presetTags=None):
    """Create a blank python ConfigParser object"""
    # this is for python3
    try: from configparser import ConfigParser
    except ImportError: pass
    # this is for python2
    try: from ConfigParser import ConfigParser
    except ImportError: pass
    # check whether any of the two worked
    try:
        # instantiate the config parser
        parser = ConfigParser()
        # tell it to preserve capitalization of keys
        parser.optionxform = str
    except NameError:
        BREAK("unable to access python module for ConfigParser!")
    return parser

#######################################################################################################################

def setTagsOnConfigParser(parser,tags,section):
    """Copy all tags on a TQTaggable object to a python ConfigParser object under a given section"""
    if not parser.has_section(section):
        parser.add_section(section)
    for key in [ obj.GetName() for obj in tags.getListOfKeys() ]:
        parser.set(section,key,tags.getTagStandardStringDefault(key))

#######################################################################################################################

def getTagsFromConfigParser(parser,tags,section):
    """Copy all options from a ConfigParser under a given section to an instance of TQTaggable"""
    if not parser.has_section(section):
        raise RuntimeError("section '{:s}' unavailable!".format(section))
    for (key, val) in parser.items(section):
        tags.setTagAuto(key,parser.get(section,key))

#######################################################################################################################

def readConfigOptionsRecursively(configObject, readconfig, section, parentConfigFiles = []):
    """
    Parses (list of) config files recursively. Filenames specified in the
    "config" section with the option "include" will also be parsed. Each file
    appends its options to readconfig, which is a TQFolder.
    """
    import ROOT
    # if configObject is a list of config files, loop over all of them
    if isinstance(configObject, (list,ROOT.vector('string'),ROOT.vector('TString'))):
        for cfgfile in configObject:
            if isinstance(cfgfile, (list,ROOT.vector('string'),ROOT.vector('TString'))):
                BREAK("Don't use nested lists in common.readConfigOptionsRecursively()")
            readConfigOptionsRecursively(cfgfile, readconfig, section, list(parentConfigFiles))
        return

    # configObject is a config file, parse it
    cfgfile = configObject.strip()
    INFO("Reading configuration for '{:s}' from file '{:s}'".format(section,cfgfile))
    if cfgfile in parentConfigFiles:
        parentConfigFiles.append(cfgfile)
        BREAK("Your include options go in a circle. Exiting to avoid infinite loop: '{:s}'".format(str(parentConfigFiles)))

    cfgpath = findConfigPath(cfgfile)
    if cfgpath == "":
        BREAK("Please specify a valid config file.")
    filesToInclude = []

    try:
        parser = createConfigParser()
        parsed = parser.read(cfgpath)
        getTagsFromConfigParser(parser, readconfig.getFolder(section+"+"), section)
        if parser.has_option("config", 'include'):
            filesToInclude = parser.get("config",'include').split(",")
        INFO("Successfully parsed as .cfg file")
    except:
        if cfgfile.endswith(".cfg"):
            BREAK("Could not parse as .cfg file, despite this being the file extention. "
                             "Have you forgotten to include the [{:s}] section name? "
                             "Do the key:value pairs follow the proper syntax? See the following link "
                             "to see how to write a .cfg file parsable by python"
                             " - https://wiki.python.org/moin/ConfigParserExamples".format(section))
        if cfgfile.endswith(".txt"):
            BREAK("Are you trying to read a TQFolder? That's a good idea, but not supported at the moment. The previous implementation overwrote all tags, but we don't want to do that anymore.")
            # todo: this used to work and would be nice to have. But now we don't want to overwrite tags (which the following implmentation does). Also, we need to think about what to do if <tag.0> is defined and someone tries to write <tag>? Make sure that we don't double-define this key.
            # if readconfig.importFromTextFile(cfgpath):
            #     INFO("Successfully parsed as .txt file with TQFolder syntax")
        else:
            BREAK("Could not parse configuration from file '{:s}'".format(cfgfile))

    # parse all files that are included by this config file
    if len(filesToInclude) > 0:
        parentConfigFiles.append(cfgfile)
        readConfigOptionsRecursively(filesToInclude, readconfig, section, parentConfigFiles)
    return

#######################################################################################################################

def getConfigOptions(args, section =  None, configFiles = None, asSubConfig = False):
    """Read a set of parsed arguments, load the corresponding config files, apply the arguments and return it as a TQFolder config object.
    Configurations can be given as UNIX CFG files or as TQFolder text files. If the same options are set by multiple files, order of precedence is:
    first CFG files, from back to front, then TQFolder files, from back to front. configFiles defaults to the list of file paths given by args.config.
    'asSubConfig' argument is for internal use only.
    """
    import QFramework
    import ROOT

    if not section:
        section = QFramework.TQLibrary.getApplicationName().Data()

    if configFiles is None:
      configFiles = args.config

    readconfig = QFramework.TQFolder.newFolder("__read__")
    readconfig.setGlobalOverwrite(False)
    readConfigOptionsRecursively(configFiles, readconfig, section)
    readconfig.setGlobalOverwrite(True)

    config = readconfig.getFolder(section)
    if not config:
        BREAK("Anticipated there being a [{:s}] section in the config file but couldn't find one"
                         " -- are you sure you are reading the correct config file?".format(section))

    for change in args.options:
        # apply changes passed via the command line
        parts = change.split("=")
        if len(parts) != 2:
            BREAK("unable to apply change '{:s}' to configuration, arguments need to follow 'key=value' syntax!".format(change))

        # remove both the normal and the vector version of the tag which is replaced
        config.removeTag(parts[0])  # normal version
        tagsToRemove = config.getListOfKeys(parts[0] + ".*")
        if not (tagsToRemove== None):  # before changing this line, please read https://root.cern.ch/phpBB3/viewtopic.php?t=8602
            for objStr in tagsToRemove:
                key = objStr.String()
                dotPos = QFramework.TQStringUtils.rfind(key, ".")
                if (dotPos != ROOT.kNPOS) and (dotPos + 1 < len(key.Data())) and key.Data()[dotPos+1:].isdigit() : #identify keys for vector tags
                    config.removeTag(key)
        # set the new tag
        config.setTagAuto(parts[0],parts[1])


    #read the path to one or more config files with definitions for (sub)campaigns
    campaignsConfig = config.getTagVStandardString("campaignsConfig")
    #read the list of (sub)campaign labels that should be considered
    campaigns = config.getTagVStandardString("campaigns")
    for camp in campaigns:
      if len(camp) < 1 : continue
      campCfg = getConfigOptions(args, section=camp, configFiles=campaignsConfig, asSubConfig=True)
      INFO("Loaded options for campaign '{:s}'".format(camp))
      config.addFolder(campCfg)

    if not asSubConfig: #only do this for top level configs

      # store the options from the CLI in the config folder
      storeCLI(config,args)

      if hasattr(args,'printConfig') and args.printConfig:
          # print the configuration to console
          INFO("configuration options given are:")
          config.printContents("rdt")
      if hasattr(args,'writeConfigTQFolder') and args.writeConfigTQFolder:
          # write out the configuration as a TQFolder text file
          if not QFramework.TQUtils.ensureDirectoryForFile(args.writeConfigTQFolder):
              BREAK("unable to write to '{:s}'!".format(args.writeConfigTQFolder))
          readconfig.exportToTextFile(args.writeConfigTQFolder)
      if hasattr(args,'writeConfigCfg') and args.writeConfigCfg:
          # write out the configuration as a CFG file
          parser = createConfigParser()
          setTagsOnConfigParser(parser,config,section)
          if not QFramework.TQUtils.ensureDirectoryForFile(args.writeConfigCfg):
              BREAK("unable to write to '{:s}'!".format(args.writeConfigCfg))
          with open(args.writeConfigCfg,"wt") as outfile:
              outfile.write("# -*- mode: config -*-\n")
              parser.write(outfile)

      # apply some global options
      linewidth = config.getTagIntegerDefault("consoleWidth",-1)
      if linewidth > 0:
          QFramework.TQLibrary.setConsoleWidth(linewidth)

      if hasattr(args,'snowflakeThresh') and args.snowflakeThresh >=0:
          QFramework.TQLibrary.getQLibrary().setSnowflakeMode(True, args.snowflakeThresh)

    config.detachFromBase()
    QFramework.TQTaggable.getGlobalTaggable("aliases").importTagsWithoutPrefix(config,"aliases.")
    #store tags from master config
    QFramework.TQTaggable.getGlobalTaggable("master").importTags(config)
    QFramework.TQTaggable.getGlobalTaggable("master").removeTags("aliases.*") #aliases are already stored as dedicated globalTaggable
    #create globalTaggables for subcampaings. Import tags from master config first such that overwriting tags from the subcampaign take precedence
    campFolders = config.getListOfFolders("?")
    if campFolders:
        for campFolder in campFolders:
            campTaggable = QFramework.TQTaggable.getGlobalTaggable(campFolder.GetName())
            campTaggable.importTags(QFramework.TQTaggable.getGlobalTaggable("master"))
            campTaggable.importTags(campFolder)

    return config

#######################################################################################################################

def getCampaignSubConfigList(mainConfig):
  """creates a list of sub-configs from a main config object for individual
     processing campaigns. The list of campaign names is taken from the
     (vector) tag "campaigns" of the mainConfig object. If this tag is not set
     a list containing only the mainConfig object is returned. If campaign names
     are found a list of subfolders of mainConfig with names matching the
     campaign names is returned. The list is not returned alone but as a tuple
     with an additional element indicating if sub-configs were extracted or not
     Usage:
     listOfConfigs, hasCampaigns = getCampaignSubConfigList(mainConfig)

     Raises a RuntimeError if campaign names are specified but no subfolder of
     mainConfig matches any of the specified names.
  """
  campaignNames = mainConfig.getTagVStandardString("campaigns")
  if len(campaignNames) == 0:
    #if there are no sub campaigns defined we return the mainConfig. For consistency it is wrapped in a list
    return ([mainConfig], False) #indicate that there are no subconfigs
  campaignConfigs = []
  for cName in campaignNames:
    camp = mainConfig.getFolder(cName)
    if not camp:
      ERROR("could not find subconfig for campaign '{:s}' - is the campaign name wrong or are you missing a corresponding config section?".format(cName))
    else:
      campaignConfigs.append(camp)
  if len(campaignConfigs) < 1 :
    raise RuntimeError("Not a single campaign name listed matched any potential subconfig - please check your master configuration and campaign definitions")
  return (campaignConfigs, True) # indicate that we have subconfigs


#######################################################################################################################

def patchFolder(filenames, samples):
    """apply a list of TQFolder-syntax text files to a folder"""
    from ROOT import TString
    errMsg = TString()
    patches = []
    for filename in filenames:
        filename_abs = findConfigPath(filename)
        patches.append((filename_abs,filename))
    for patch_abs, patch in patches:
        if not samples.importFromTextFile(patch_abs,errMsg):
            ERROR("error importing patch tags from '{:s}': {:s}".format(patch,errMsg))
        else:
            INFO("successfully imported patch tags from '{:s}'".format(patch))
    return

#######################################################################################################################

def patchSampleFolder(filenames, samples):
    """apply a set of TQFolder-syntax text files to a sample folder"""
    patchFolder(filenames,samples)
    return

#######################################################################################################################

def createSampleFolder(config, pathToConfig=defaultConfig.pathToConfig):
    """create a sample folder based on a configuration

       config options:
         - sampleFolderName: name of the sample folder to be created
         - luminosity: luminosity that the sample folder should contain
         - luminosityUnit: unit in which this luminosity is given (default: defaultConfig.luminosityUnit)
         - energy: center-of-mass collision energy for the contents of the sample folder
         - energyUnit: unit in which this energy is given (default: defaultConfig.energyUnit)
         - InputVersion: useful for any additional logic which needs to be conditional on the version of inputs used
       Please note that if you intend to use/process multiple campaigns no luminosity
       should be specified in the base master config!
    """
    import QFramework
    campConfigs, hasCampaigns = getCampaignSubConfigList(config)
    # retrieve the intended name
    sfname = config.getTagStandardStringDefault("sampleFolderName",defaultConfig.sampleFolderName)
    # create the sample folder with this name
    samples = QFramework.TQSampleFolder.newSampleFolder(sfname)
    if not samples:
        BREAK("unable to allocate sample folder '{:s}' - invalid name?".format(sfname))
    # set a couple of parameters
    if (not hasCampaigns) and config.hasTagDouble("luminosity"):
        samples.setTagDouble("luminosity",config.getTagDoubleDefault("luminosity",0.))
        samples.setTagString("luminosityUnit",config.getTagStringDefault("luminosityUnit",defaultConfig.luminosityUnit))
    else:
        if hasCampaigns:
            hasLumi = False
            for camp in campConfigs:
                if camp.hasTagDouble("luminosity"):
                    samples.setTagDouble("luminosity."+camp.GetName(),camp.getTagDoubleDefault("luminosity",0.))
                    hasLumi = True
            if hasLumi: samples.setTagString("luminosityUnit",config.getTagStringDefault("~luminosityUnit",defaultConfig.luminosityUnit))
        if config.hasTagDouble("luminosity"):
            WARN("It appears you are trying to process one or more subcampaigns, in such cases defining a global luminosity is a bad idea!")
    if config.hasTagDouble("energy"):
        samples.setTagDouble("energy",config.getTagDoubleDefault("energy",0.))
        samples.setTagString("energyUnit",config.getTagStringDefault("energyUnit",defaultConfig.energyUnit))
    if config.hasTagInteger("InputVersion"):
        samples.setTagInteger("InputVersion",config.getTagIntegerDefault("InputVersion",1))

    # store the config in the sample folder
    storeConfig(config,samples,pathToConfig)

    # return the folder
    return samples

#######################################################################################################################

def reduceSampleFolder(config, samples, condition=1):
    """remove all empty and unneeded parts of a sample folder"""
    import QFramework
    purge = QFramework.TQSamplePurger("purgatory")
    purge.setCondition(condition)
    samples.visitMe(purge)
    if not samples.getListOfSampleFolders("?"):
        BREAK("sample folder empty after purge - something is wrong!")
    return

#######################################################################################################################

def getDictOfChannels(config):
    """retrieve the channels which are to be considered for visualizing the results.
    channels will be a dictionary holding the channels specified by the user,
    each of which will be a dictionary with the properties of that channel
    """
    import QFramework
    rawchannels = [ c for c in config.getTagVStandardString("channels") ]
    channeltab = QFramework.TQTable()
    channeltabfile = config.getTagStringDefault("channelConfig","common/config/channels/channels.txt")
    channeltabfile_abs = findConfigPath(channeltabfile)
    channeltab.readTSVfile(channeltabfile_abs)
    channelNameColumn = channeltab.findColumn("name",False)
    channelPathColumn = channeltab.findColumn("path",False)
    channelLatexColumn = channeltab.findColumn("latex",False)
    channelDataPathColumn = channeltab.findColumn("datapath",False) #optional, needed when trying to plot systematic variations with data
    if max([channelNameColumn,channelPathColumn,channelLatexColumn])<0:
        QFramework.WARN("failed to find one of the following columns in the channel definition file '{:s}': name, path, latex, or datapath".format(channeltabfile))

    channelsDict = {}
    variationsuffix = config.getTagStandardStringDefault("variation","")
    if variationsuffix:
        print("Has variation suffix: {:s}".format(variationsuffix))
        variationsuffix = "_" + variationsuffix
    for i in range(1,channeltab.getNrows()):
        channelnames =  str(channeltab.getEntry(i,channelNameColumn,"verbatim")).split(",")
        cpath = channeltab.getEntry(i,channelPathColumn,"verbatim")
        cdatapath = cpath if channelDataPathColumn<0 else channeltab.getEntry(i,channelDataPathColumn,"verbatim")
        cpath.ReplaceAll("$(variation)",variationsuffix)
        cdatapath.ReplaceAll("$(variation)",variationsuffix)
        channelpath = cpath.Data()
        channeldatapath = cdatapath.Data()
        channelpretty = channeltab.getEntryLaTeX(i,channelLatexColumn).Data()

        # TODO: should probably warn the user righ away if one of the raw channels specified is not in the sample folder provided
        #        but that wouldn't go this far in the loop - maybe create a separate function which checks the channels dict as input?
        for craw in rawchannels:
            if craw in channelnames or craw is channelpath:
                channelsDict[str(channelnames[0])] = {
                    "names" : channelnames, # can be a list of multiple strings which mean the same thing, i.e. [emme,meem,df]
                    "path" :  channelpath,
                    "datapath" :  channeldatapath,
                    "latex" : channelpretty,
                    "roottex" : QFramework.TQStringUtils.convertLaTeX2ROOTTeX(channelpretty)
                }

    if not channelsDict:
        QFramework.BREAK("the channels {:s} you listed are not contained in '{:s}'".format(",".join(rawchannels),channeltabfile.Data()))

    return channelsDict

#######################################################################################################################

def exportHistograms(config, samples):
    """export histograms to a plain root file

    config options:
      - exportHistograms.path: path to the plain root file to export to
      - exportHistograms.fileName: file name of the plain root file to export. Path and file name are split in two options for easier batch processing
      - exportHistograms.processes: process definition file as used for visualize (plotter, cutflowprinter, ...)
      - exportHistograms.channels: comma separated list of channel names, individual names are defined in the channelConfig file
      - exportHistograms.channelConfig: path to config file defining channels (identical to visualize channel config file)
    """
    import QFramework
    import ROOT
    exportConfig = QFramework.TQTaggable()
    exportConfig.importTagsWithoutPrefix(config, "exportHistograms.")
    if exportConfig.getNTags()<1:
        #nothing to do here...
        return
    if not ( ( exportConfig.hasTag("path") or exportConfig.hasTag("fileName") ) and exportConfig.hasTag("processes") and ( exportConfig.hasTag("channels") or exportConfig.hasTag("channels.0") ) and exportConfig.hasTag("channelConfig") ):
        exportConfig.printTags()
        QFramework.WARN( "Skipping exporting of histograms despite at least one 'exportHistograms.*' option being present. Please ensure all required settings are present in your configuration:\n"
                        + "\t - 'path' or 'fileName' (or both) to export the histograms to\n"
                        + "\t - 'processes' files defining the processes to export (same as visualize plotting processes file) \n"
                        + "\t - 'channelConfig' channel definition file (analogous to channel config for visualize)  \n"
                        + "\t - 'channels' comma separated list of channel names defined in the channelConfig file \n"
                        + "\t All of these options need to be prefixed by 'exportHistograms.'")
        return

    channelDict = getDictOfChannels(exportConfig)
    dummyPlotter = QFramework.TQDefaultPlotter(samples) #just a dummy instance to use the process parsing caabilities

    dummyPlotter.importProcessesFromFile(exportConfig.getTagStandardStringDefault("processes",""))
    if not dummyPlotter.nProcesses():
        QFramework.WARN( "Skipping exporting of histograms: no valid processes were defined (was the correct processes file referenced in the config?" )

    processNamesAndPaths = [(name.String().Data(), dummyPlotter.getProcessPath(name.String()).Data()) for name in dummyPlotter.getListOfProcessNames() ]

    listOfHistogramNames = samples.getListOfHistogramNames()
    if not listOfHistogramNames:
        QFramework.ERROR( "Failed to retrieve list of histogram names, cannot export..." )
        return
    cutAndHistNames = [ x.String().Data().split("/") for x in listOfHistogramNames] #TList with TObjStrings, hence, access bare string via String() -> Data()

    outpath = exportConfig.getTagStandardStringDefault("path","")
    fileName = exportConfig.getTagStandardStringDefault("fileName","histograms_$(jobID).root")
    import os
    fullFileName = samples.replaceInTextRecursive(os.path.join(outpath, fileName))
    resolvedFileName = QFramework.TQPathManager.getPathManager().getTargetPath(fullFileName)
    QFramework.TQUtils.ensureDirectoryForFile(resolvedFileName)
    outfile = ROOT.TFile.Open(resolvedFileName,"RECREATE")
    if not outfile:
        QFramework.ERROR( "Failed to open file '{:s}' to export histograms to".format(resolvedFileName) )
        return

    for c in channelDict:
        channelTags = QFramework.TQTaggable()
        channelTags.setTag("channel",channelDict[c]["path"])
        channelTags.setTag("datachannel",channelDict[c]["datapath"])
        channelDir = outfile.mkdir(c)
        channelDir.cd()
        for procName,procPath in processNamesAndPaths:
            procPath = channelTags.replaceInTextRecursive(procPath).Data()
            procDir = channelDir.Get(procName)
            if not procDir:
                procDir = channelDir.mkdir(procName)
            procDir.cd()
            for cut,hist in cutAndHistNames:
                cutDir = procDir.Get(cut)
                if not cutDir:
                    cutDir = procDir.mkdir(cut)
                cutDir.cd()
                histObj = samples.getHistogram(procPath,os.path.join(cut,hist))
                if not histObj:
                    continue
                histObj.SetName(hist)
                histObj.SetDirectory(cutDir)

    outfile.Write()
    outfile.Close()
    return


#######################################################################################################################

def writeSampleFolder(config, samples, keepInMemory=False, quiet=False, testOnly=False):
    """write a sample folder to disk

      config options:
        - outputFile: path to the output file (required)
        - writeSplitSampleFolder: integer number identifying at which
           level the sample folder should be split into separate objects to
           make reading/writing more efficient (deafult:defaultConfig.writeSplitLevel)
    """
    from ROOT import TString
    import QFramework
    CLI = config.getFolder("CLI+")
    # retrieve config options
    if not config.hasTagString("outputFile"):
        BREAK("config option 'outputFile' is required!")
    if CLI.getTagBoolDefault("debug",False):
        outfilename = TString("debug.root")
    else:
        outfilename = config.getTagStandardStringDefault("outputFile","")
        # only if not in debug mode we optionally export histograms to a plain root file
        if not testOnly:
            exportHistograms(config,samples)

    outfilename = QFramework.TQPathManager.getPathManager().getTargetPath(outfilename)
    splitlevel = config.getTagIntegerDefault("writeSplitSampleFolder",defaultConfig.writeSplitLevel)

    if config.getTagBoolDefault("reduceOutput"):
        tag = ".generalize."+QFramework.TQLibrary.getApplicationName()
        samples.generalizeObjects(tag)
        if samples.getNElements(False) > 0:
            reduceSampleFolder(samples,QFramework.TQSamplePurger.ANALYZED)

    # record memory (for debugging)
    QFramework.TQLibrary.recordMemory()
    # start writing the sample folder, making sure that the directory exists
    if not quiet:
        INFO("writing output sample folder '{:s}' to '{:s}'...".format(samples.GetName(),outfilename))
        QFramework.START("l.","Write outcome:")
    ok = QFramework.TQUtils.ensureDirectoryForFile(outfilename) and (samples.writeToFile(outfilename,True,splitlevel,keepInMemory) > 0)
    from os.path import isfile
    if ok and not isfile(outfilename):
        ok = False
    if ok:
        if not quiet: QFramework.END(QFramework.TQMessageStream.OK)
        return True
    else:
        if not quiet: QFramework.END(QFramework.TQMessageStream.FAIL)
        return False

#######################################################################################################################

def getXSecParser(config, samples, pathToXSP=defaultConfig.pathToXSP, XSPName=defaultConfig.XSPName):
    """return the XSecParser if one already exists in the sample folder path provided.
    If not found, create and return a new channel aware instance"""

    import QFramework
    XSPFolder = samples.getFolder(pathToXSP+"+")

    if XSPFolder.hasObject(XSPName):
        # XSecParser found
        parser = samples.getFolder(pathToXSP).getObject(XSPName)
        # need to re-assign the sample folder to the XSecParser
        # in case this is the first time it's accessed during the run
        # SF pointer member to XSecParser isn't streamed by ROOT (isn't stored on disk)
        parser.setSampleFolder(samples)
        return parser
    else:
        # start by initializing a TQXSecParser
        parser = QFramework.TQXSecParser(samples)
        parser.SetName(XSPName)

        # add path variants to the parser (announce the channels)
        # each path variant should reflect a set of path replacements, i.e.
        # a path variant "channel='electron'" will replace all occurences of
        #  "$(channel)" with "electron" in the input paths
        # for this simple template analysis, we assume that each "channel"
        # consists of only one replacement of the type $(channel) => X
        # the placeholder string (here 'channel') can be configured
        channels = config.getTagVStandardString("~channels")
        channelPlaceHolder = config.getTagStringDefault("~channelPlaceholder","channel")
        if len(channels) < 1:
            BREAK("please specify at least one channel via 'channels: ee,mm,...'")
        # TODO: concat was modified such that it is now a templated method, a bit more work is needed now to expose this to python...
        #INFO("using channels: {:s}".format(QFramework.TQStringUtils.concat(channels)))
        INFO("using channels:")
        for c in channels:
            INFO(c)
            parser.addPathVariant(channelPlaceHolder,c)
    return parser

#######################################################################################################################

def storeCuts(cuts, samples, pathToCuts=defaultConfig.pathToCuts):
    """store the given cuts in the sample folder"""
    cutsFolder = samples.getFolder(pathToCuts+"+")
    if not cuts.dumpToFolder(cutsFolder):
        ERROR("unable to attach cuts to info folder")

#######################################################################################################################

def storeXSecParser(parser, samples, pathToXSP=defaultConfig.pathToXSP):
    """store the given XSec parser in the sample folder.
    Do nothing if an object with the same name is found"""
    XSPFolder = samples.getFolder(pathToXSP+"+")
    # if an object of the same name is not found in the path provided
    if not XSPFolder.hasObject(parser.getName()):
        # try storing the parser and print an error if failed
        if not XSPFolder.addObject(parser):
            ERROR("cannot add Xsection parser to sample folder")

    return

#######################################################################################################################

def storeCLI(config,args):
    """store the CLI options in the config folder under /CLI"""
    CLIFolder = config.getFolder("CLI+")
    for (key,value) in vars(args).items():
        # TODO: better way to set here?
        CLIFolder.setTagAuto(key,str(value))

#######################################################################################################################

def storeConfig(config,samples,pathToConfig=defaultConfig.pathToConfig):
    """store a copy of the given configuration in the sample folder under /[pathToConfig]/[ApplicationName]"""
    # create the config folder
    configFolder = samples.getFolder(pathToConfig+"+")
    # TODO: now saving config directly in sf instead of a copy - is this fine?
    #       allows runtime tags to be set and saved to the config in the sf
    #       downside is that config saved in theory may not reflect exactly what input config looked like
    # create a copy of the config
    #storedConfig = config.copy(QFramework.TQLibrary.getApplicationName())
    # set some info tags for bookkeeping
    config.setInfoTags()
    # add the copy of the config to the sample folder
    configFolder.addFolder(config)

#######################################################################################################################

def loadSampleFolder(config, doStoreConfig=True):
    """load a sample folder from disk

       config options:
         - inputFile: path to the input file (required)
         - sampleFolderName: name of the sample folder to be loaded
         - jobID: will be set as tag on the sample folder
         - InputVersion: will be set as tag on the sample folder
    """
    import QFramework
    CLI = config.getFolder("CLI+")
    # flag indicating to run a dummy analysis
    dummy = CLI.getTagBoolDefault("dummy",False)
    if dummy:
        samples = QFramework.TQSampleFolder("test")
        # store the config options
        if doStoreConfig: storeConfig(config,samples)
        # return the result
        return samples

    # retrieve config options
    if not config.hasTagString("inputFile"):
        BREAK("config option 'inputFile' is required!")
    sfinfilename = config.getTagStandardStringDefault("inputFile","")
    sfinfilename = QFramework.TQPathManager.getPathManager().getTargetPath(sfinfilename)
    sfname = config.getTagStandardStringDefault("sampleFolderName",defaultConfig.sampleFolderName)
    # load the sample folder
    sf = sfinfilename+":"+sfname
    INFO("loading sample folder '{:s}'".format(sf))
    samples = QFramework.TQSampleFolder.loadSampleFolder(sf)
    if not samples:
        BREAK("unable to load sample folder '{:s}' - please check input path".format(sf))
    # store jobID on the base sample folder if available
    # useful for batch submission
    if CLI.hasTagString("jobID"):
        jobID = CLI.getTagStandardStringDefault("jobID")
        samples.setTagString("jobID",jobID)
    # store the config options
    if doStoreConfig: storeConfig(config,samples)
    if config.hasTagInteger("InputVersion"):
        inputVersion = config.getTagIntegerDefault("InputVersion", 1)
        if samples.hasTagInteger("InputVersion") and not inputVersion == samples.getTagIntegerDefault("InputVersion", -1):
            BREAK("Detected incompatible tags 'InputVersion'. The sample folder has the tag InputVersion={:d} an the config InputVersion={:d}.".format(samples.getTagIntegerDefault("InputVersion", -1), inputVersion))
        samples.setTagInteger("InputVersion", inputVersion)
    # return the result
    return samples

#######################################################################################################################

def testWriteSampleFolder(config, samples):
    """test writing a sample folder to disk to check if names are valid and target directory is writeable"""
    import QFramework
    from ROOT import TString
    CLI = config.getFolder("CLI+")
    # retrieve config options
    if not config.hasTagString("outputFile"):
        BREAK("config option 'outputFile' is required!")
    if CLI.getTagBoolDefault("debug",False):
        outfname = TString("debug.root")
    else:
        outfname = QFramework.TQPathManager.getPathManager().getTargetPath(config.getTagStandardStringDefault("outputFile",""))
    # make a dummy copy that's lightweight to write        
    testSamples = QFramework.TQSampleFolder(samples.GetName()) 
    if not writeSampleFolder(config, testSamples, keepInMemory=False, quiet=True, testOnly=True):
        BREAK("unable to write to file '"+outfname+"'!")
    else:
        shellexec(["rm","-f",outfname])
    return

#######################################################################################################################

def printXSecParser(samples, pathToXSP=defaultConfig.pathToXSP, XSPName=defaultConfig.XSPName):
    """print the contents of the XSecParser stored in the sample folder provided"""
    import QFramework
    XSPFolder = samples.getFolder(pathToXSP+"+")

    if not XSPFolder.hasObject(XSPName):
        WARN("requested to print XSec parser from sample folder, but none was found under path '{:s}' and name '{:s}'".format(pathToXSP,XSPName))
        return
    else:
        XSPFolder.getObject(XSPName).printPlain()
        return

#######################################################################################################################

def printExecutionStatistics(config):
    """print and write some statistics from the code execution"""
    # create some very basic plots showing how much memory this run used as a function of time
    # more points can be added by calling TQLibrary::recordMemory(color) (or python equivalent)
    # at any point where color can be any valid TColor number.
    import QFramework
    import ROOT
    if (config.hasTagString("memoryGraph")):
        ROOT.gROOT.SetBatch(True)
        memGraph = QFramework.TQLibrary.getMemoryGraph()
        memFileName = config.getTagStringDefault("memoryGraph","memoryUsage.pdf")
        QFramework.TQStringUtils.ensureTrailingText(memFileName,".pdf")
        memCanvas = QFramework.TQHistogramUtils.applyATLASstyle(memGraph,"Internal",0.2,0.9,0.9,"timestamp","rss [byte]")
        memCanvas.SaveAs(memFileName.Data())
    if (config.hasTagString("memoryGraphDifferential")):
        ROOT.gROOT.SetBatch(True)
        memGraph = QFramework.TQLibrary.getMemoryGraph(True)
        memFileName = config.getTagStringDefault("memoryGraphDifferential","memoryUsageDifferential.pdf")
        QFramework.TQStringUtils.ensureTrailingText(memFileName,".pdf")
        memCanvas = QFramework.TQHistogramUtils.applyATLASstyle(memGraph,"Internal",0.2,0.9,0.9,"timestamp","#Delta rss [byte]")
        memCanvas.SaveAs(memFileName.Data())

    #Let the user know how heavy this run was:
    import QFramework
    rawRunTime = QFramework.TQLibrary.getQLibrary().getTimeSinceConstruction()
    runTime = QFramework.TQUtils.formatTimeDifference(rawRunTime)
    INFO("This analysis run used up to {:.2f} MiB of memory (RSS) and took {:s}.".format(QFramework.TQUtils.getPeakRSS()/1024./1024., runTime.Data()))
    return

#######################################################################################################################

def printExecutionSummary(config):
    import QFramework
    printExecutionStatistics(config)
    QFramework.TQLibrary.printGlobAlertMessageCount()

#######################################################################################################################

def printUnreadKeys(config):
    """print the config keys which have not been read"""
    import QFramework
    if config.hasUnreadKeys("!.*"):
        WARN("the following config keys were not read: "+(",".join([ key.GetName() for key in  config.getListOfUnreadKeys("!.*") ])))
    return

#######################################################################################################################

def findFormatKeywords(string):
    """Parses a string of the form you would see with old-style Python formatting
        e.g. '%(key1)s goes here, then $(key2)s' % {key1: 'string1', key2: 'string2'}
        Returns all the formatting keywords, e.g. 'key1' and 'key2' above
    """
    import re
    inputsRegex = re.compile(r"\%\([\w-]+\)s")
    matches = inputsRegex.findall(string)
    # strips the leading '%(' and trailing ')s' from the format code
    keywords = [formatCode[2:-2] for formatCode in matches]
    return keywords


#######################################################################################################################

try:
    isinstance("", basestring)
    def isstr(s):
        return isinstance(s, basestring)
except NameError:
    def isstr(s):
        return isinstance(s, str)

#######################################################################################################################

def concat(pieces,delim=" "):
    """concatenate a list of strings to a single string"""
    if isstr(pieces):
        return pieces
    else:
        return delim.join(map(str,pieces))


#######################################################################################################################

def shellexec(command,inputs=[],verbose=False,allowErrors=True):
    """execute a command by invoking it in a shell"""
    if verbose:
        print(concat(command))
        for inputline in inputs:
            if len(inputline) > 0: print("> "+concat(inputline))
            pass
        pass
    command = concat(command).split()
    stdinlines = [ concat(pieces) for pieces in inputs if len(pieces) > 0]
    s = concat(stdinlines,"\n")

    from subprocess import Popen, PIPE
    p = Popen(command, stdout=PIPE, stdin=PIPE, stderr=PIPE)
    b = s.encode('ascii')
    stdout_data, stderr_data = p.communicate(input=b)
    if len(stderr_data) > 0:
        if allowErrors and verbose:
            ERROR("Error from binary '"+concat(command)+"': "+concat(stderr_data))
        if not allowErrors:
            BREAK("Error from binary '"+concat(command)+"': "+concat(stderr_data))
    return stdout_data

#######################################################################################################################

def mkdir(path):
    """call 'mkdir -p' on the given path"""
    shellexec(["mkdir","-p",path])

#######################################################################################################################

def findConfigPath(filename, errorMessageIfNotFound=True, ignoreExecutionDirectory=False):
    # find the config file "filename"
    # if the environment variable isn't defined, just look locally.
    # filenames with TQFolder syntax e.g. folder.root:objectinside>>::label are supported
    # in these cases, only folder.root will be passed to the TQPathManager
    # and the absolute path including :objectinside>>::label will be returned
    import QFramework
    if filename == "":
        return ""
    import re
    filenamebase = re.findall(r"[-/\w._]+", str(filename))[0]
    if ignoreExecutionDirectory:
        filePath = QFramework.TQPathManager.findFileFromEnvVarWithoutExecDir(filenamebase, "CAFANALYSISSHARE", errorMessageIfNotFound)
    else:
        filePath = QFramework.TQPathManager.findFileFromEnvVar(filenamebase, "CAFANALYSISSHARE", errorMessageIfNotFound)
    if filePath == "" and errorMessageIfNotFound and QFramework.TQUtils.readEnvVarValue("CAFANALYSISSHARE") == "":
        WARN("Did you forget to source the file \"setup/setupAnalysis.sh\"?")

    return filePath+str(filename)[len(filenamebase):]
#######################################################################################################################

def findMultipleConfigPathsFromList(filenameList, errorMessageIfNotFound=True):
    # find config files from a list of filenames
    for i, filename in enumerate(filenameList):
        filenameList[i] = findConfigPath(filenameList[i], errorMessageIfNotFound)
    # remove any empty elements
#    filePathList = [x for x in filenameList if x != ""]
    return filenameList

#######################################################################################################################

def findMultipleConfigPathsFromString(filenames, errorMessageIfNotFound=True):
    # find config files from a string of filenames, separated by ","
    import QFramework
    filenameList = QFramework.TQStringUtils.split(filenames,",");
    filePathList = findMultipleConfigPathsFromList(filenameList, errorMessageIfNotFound)
    filePathString = ""
    for i, pathname in enumerate(filePathList):
        filePathString += pathname
        if (i+1 != len(filePathList)):
            filePathString += ","
    return filePathString

#######################################################################################################################

def isFolderInPathAtLevel(name, pathselect, level):
    """
    Checks if folder with name 'name' is present in path 'pathselect'
    at hierarchy level 'level. Currently used for checking if name of channel/campaign
    matches the one in pathselect.
    Wilcarded folder with '?' will always evaluate to true.
    """
    from ROOT import TString
    import QFramework
    foundMatch = False
    folderName_hold = TString("")
    folderName = TString("")
    if len(pathselect) > 0:
          for i, p in enumerate(pathselect):
              QFramework.TQStringUtils.removeLeading(p, "/") # remove potential leading '/'
              QFramework.TQStringUtils.removeTrailing(p, "/") # remove potential trailing '/'
              folderNameList = str(p).split("/")
              if level >= len(folderNameList):
                  # If level is larger than the length of the given path,
                  # always evaluate true
                  foundMatch = True
              else:
                  folderName = TString(folderNameList[level])
              if i > 0:
                  if not folderName.EqualTo(folderName_hold):
                      QFramework.BREAK("You specified multiple restricted sample paths '{:s}' that have not matching folders at "
                                       "folder hierarchy level {:d} "  \
                                       "(Possibly this is the level of the channel or campaign specifications). "
                                       "This is currently not possible, please specify paths with the same folders at this level per "
                                       "job".format(", ".join([str(p) for p in pathselect]), level))
              folderName_hold = folderName
              if TString(name).EqualTo(folderName) or folderName.EqualTo("?"):
                  foundMatch = True
    return foundMatch

#######################################################################################################################

def getRestrictedPaths(config):
    """
    Returns the restricted path if given in config, otherwise returns None
    """
    from ROOT import TString
    CLI = config.getFolder("CLI+")
    pathselect = CLI.getTagVString("pathselect")
    isRestrictedPath = False if len(pathselect) == 0 or ( len(pathselect) == 1 and TString(pathselect[0]).IsNull() ) else True
    if isRestrictedPath:
        return pathselect, isRestrictedPath
    else:
        return None, isRestrictedPath

#######################################################################################################################

def getMatchingBasePaths(restrictedPaths, basePaths):
    """
    Compares whether the restricted path matches the basepath and
    returns a list of all matching elements
    """
    from ROOT import TString
    from ROOT import vector
    from QFramework import TQStringUtils
    if not isinstance(restrictedPaths, (list, vector('TString'), vector('string'))): restrictedPaths = [restrictedPaths]
    if not isinstance(basePaths, (list, vector('TString'), vector('string'))): basePaths = [basePaths]
    matchingPaths = []
    for path in restrictedPaths:
        path = TString(path)
        TQStringUtils.removeLeading(path,"/")
        TQStringUtils.removeTrailing(path,"/")
        for basePath in basePaths:
            basePath = TString(basePath)
            TQStringUtils.removeLeading(basePath,"/")
            TQStringUtils.removeTrailing(basePath,"/")
            depth = len(TQStringUtils.split(basePath, "/"))
            # python does not care if index <IND> is out of range when given like [0:<IND>]
            pathToCompareWith = TString("/".join( [p for p in str(path).split("/")[0:depth] ] ))
            basePath.ReplaceAll("?", "*") # to use the following TQStringUtils function
            if TQStringUtils.matches(pathToCompareWith, basePath):
                matchingPaths.append(path)
    return matchingPaths

##################################################################################################################

def isPresentVariationNameHead(config, variationType, variationName):
    """
    Helper function to catch errors when specifying variation names in observables snippets.
    Checks whether identifier for variation 'variationName' is present in variation list.
    Returns true if no variation list is given
    """
    from ROOT import TString
    import QFramework
    nameMatch = False
    fileWithVariations = TString()
    if config.getTagString(variationType + "List", fileWithVariations):
        filePath = QFramework.TQPathManager.findFileFromEnvVar(fileWithVariations, "CAFANALYSISSHARE")
        with open(filePath) as varFile:
            varList_ = varFile.readlines()
            varList = [x.strip() for x in varList_]
            for variations in varList:
                #remove/skip comment lines:
                if variations == "" or variations[0]=="#": continue
                for var in variations.split(","): # splits up and down variation
                    var = var.strip()
                    if not var or len(var)<1: continue
                    if var.startswith(variationName):
                        nameMatch = True
    else:
        # no file with variations given (not running with variations) -> return true
        nameMatch = True
    if not nameMatch:
        WARN("You defined the variation identifier '{:s}' which is "\
             "not present in your given variation list. Is this intended?".format(variationName))
    return nameMatch

##################################################################################################################

def mergeFilesQuery(args, config):
    """
    Printout possible merge command at the end of submission out of convenience when all jobs have succeeded.
    Additionally provide the option to directly merge the files when '--merge' argument is given.
    """
    import QFramework
    import os

    options = getConfigOptions(args, section = "merge", configFiles = args.mergeConfig)

    # retrieve options from the merge config file
    mergeOptions = ""
    if options.hasTag("output"):
      mergeOptions = "--output {:s} ".format(options.getTagStringDefault("output", "merged.root"))
    else:
      # we can retrieve an output name from the master config
      # since their is a query before actually performing the merging
      # the following default name is fine...
      outputFile = config.getTagStringDefault("outputFile", "<yourDesiredOutputName>")
      outputFile = QFramework.TQPathManager.getPathManager().getTargetPath(outputFile)
      mergeOptions = "--output {:s} ".format(outputFile)
    if options.hasTag("name"):
      mergeOptions = mergeOptions + "--name {:s} ".format(options.getTagStringDefault("name", "mergedsf"))
    if options.hasTag("sfname"):
      mergeOptions = mergeOptions + "--sfname {:s} ".format(options.getTagStringDefault("sfname", "samples"))
    if not args.executable.split(".")[0] == "initialize": # for initialize we don't need the following merge options
        if options.hasTag("downmerge"):
            mergeOptions = mergeOptions + "--downmerge {:s} ".format(options.getTagStringDefault("downmerge", "generalize"))
        for p in options.getTagVString("patch"):
            p = findConfigPath(p)
            mergeOptions = mergeOptions + "--patch {:s} ".format(p)
        if options.hasTag("sum"):
            mergeOptions = mergeOptions + "-{:s} ".format(options.getTagStringDefault("sum", "Sum"))
        if options.hasTag("depth"):
          mergeOptions = mergeOptions + "--depth {:d} ".format(options.getTagIntegerDefault("depth", 2))

    mergeCmdTemplate = "tqmerge -t {executable} {unmergedFiles} {mergeoptions}"
    executableName = args.executable.split(".")[0]
    pathToUnmergedFiles = "{:s}/unmerged_{:s}/*.root ".format(args.output, args.identifier)
    pathToUnmergedFiles = QFramework.TQPathManager.getPathManager().getTargetPath(pathToUnmergedFiles)
    mergeCmd = mergeCmdTemplate.format(executable=executableName,
                                       unmergedFiles=pathToUnmergedFiles,
                                       mergeoptions=mergeOptions)

    # Final printout and query if files should be merged
    QFramework.INFO(QFramework.TQStringUtils.makeBoldWhite("All your batch jobs seem to have succeeded and " \
                                                           "have the status 'done'."))
    if args.merge:
      answer = raw_input("Do you want to merge your files with the following command? " \
                         # "(configured with '{:s}'".format(args.mergeConfig) + ")?\n" \
                         "\n" + QFramework.TQStringUtils.makeBoldGreen(mergeCmd) + "" \
                         "\n(y/n) ")
      if answer.lower() == "y" or answer.lower() == "yes":
        QFramework.INFO("Merging files...")
        os.system(mergeCmd)
    else:
      QFramework.INFO(QFramework.TQStringUtils.makeBoldWhite("You can merge them e.g. with:"))
      QFramework.INFO(QFramework.TQStringUtils.makeBoldGreen(mergeCmd))
