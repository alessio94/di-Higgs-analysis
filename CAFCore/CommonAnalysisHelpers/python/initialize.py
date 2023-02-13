import QFramework
from . import common
from CommonAnalysisHelpers import defaultConfig

"""this submodule contains helper functions to handle the initialzation of analysis sample folders"""

def DefaultArgumentParser():
    """create a pre-configured argument parser with some default options tailored for an 'initialize' step"""
    parser = common.createBasicArgumentParser("initialize a previously created sample folder with all meta-information required to execute the analysis")
    # add some extra arguments to parallelize the initialize step
    parser.add_argument('--restrict', metavar="PATHS", dest="pathselect", default="", help='restrict the analysis to a path set')
    parser.add_argument('--jobID', dest="jobID", type=str, default="initialize", help="identifier for the job executing this instance")
    return parser

#######################################################################################################################

def addData(config, samples, pathToXSP=defaultConfig.pathToXSP, XSPName=defaultConfig.XSPName, dataBasePaths = ["data"], isCampaignConfig=False):
    """add a list of Data samples to a sample folder using a cross section parser"""
    from ROOT import TString

    # get potential restricted sample paths
    if not config.Class() == QFramework.TQTaggable.Class():
        # Check the above as a temporary fix for initializing hww fakes (in hwwinitialize.py) in the old
        # HWWAnalysisCode setup with new CAFCore setup
        # old (new) referring to before (after) implementation of campaign support
        pathselect_tmp, isRestrictedPath = common.getRestrictedPaths(config)
    else:
        isRestrictedPath = False

    # check if potential restricted path is actually an MC sample path
    if isRestrictedPath:
        pathselect = common.getMatchingBasePaths(restrictedPaths = pathselect_tmp, basePaths = dataBasePaths)
        if not pathselect:
            QFramework.INFO("You run with the restrict option set to '{:s}', which is not a data sample path. " \
                            "Nothing to do in 'addData(...)', returning from function...".format(" ".join(map(str,pathselect_tmp))))
            return
        
    listOfCampaignConfigs, hasCampaigns = common.getCampaignSubConfigList(config)
    # special case when a campaigns config (subconfig) is already provided
    if isCampaignConfig: hasCampaigns = True
    
    # Loop over campaigns ---->>
    for campConfig in listOfCampaignConfigs:
        
      # if path is restricted, check if campaigns are
      # matching for the current loop, else skip
      if hasCampaigns and isRestrictedPath:
          campaignFolderLayer = config.getTagIntegerDefault("campaignFolderLayer", 2)
          if not common.isFolderInPathAtLevel(campConfig.GetName(), pathselect, campaignFolderLayer):
              QFramework.INFO("You specified a restricted path: '{:s}' which does not match " \
                              "the current campaign '{:s}' that is looped over. " \
                              "Skipping!".format(", ".join([str(p) for p in pathselect]),  campConfig.GetName()))
              continue
              
      campSuffix = ("_"+campConfig.GetName()) if hasCampaigns else "" #if no campaign split present no suffix is needed
      campaignName = "all"
      if hasCampaigns: 
        QFramework.INFO("Adding Data for campaign '{:s}'".format(campConfig.GetName()))
        campaignName = campConfig.GetName()
        
      # retrieve a XSecParser
      parser = common.getXSecParser(config, samples, pathToXSP, XSPName+campSuffix)
      
      campaignPlaceHolder = campConfig.getTagStringDefault("~campaignPlaceholder","campaign")
      # here, we assume that data samples will not be listed in the Xsection file
      # but will be kept in a separate directory with a dedicated filename pattern
      dataFileNamePattern = campConfig.getTagStringDefault("~dataFilePattern","*.root*")
      # we will put all the data samples to a separate folder given by the data folder pattern
      # this will allow for different channels in much the same manner as with Monte Carlo
      dataFolderPattern = campConfig.getTagStandardStringDefault("~dataFolderPattern","data/$(channel)/$({campaignPlaceHolder})".format(campaignPlaceHolder=campaignPlaceHolder))
      dataPaths = campConfig.getTagVStandardString("dataPaths") #explicitly NOT retrieving recursively. This setting is fundamental to the concept of a campaign
      dataFileList =list(campConfig.getTagVStandardString("dataFileLists")) or list(campConfig.getTagVStandardString("dataFileList"))
      channels = campConfig.getTagVStandardString("~channels")
      channelPlaceholder = campConfig.getTagStandardStringDefault("~channelPlaceholder","channel")
      tags = QFramework.TQTaggable()
      tags.setTagString(campaignPlaceHolder,campaignName)
      if len(dataPaths) > 0:
          # discover the data input files from the paths provided
          for directory in dataPaths:
              path = TString()
              treeName = TString()
              QFramework.TQFolder.parseLocation(directory,path,treeName)
              if not treeName:
                  treeName = "CollectionTree"
              #if the treeName is explicitly specified via a dedicated tag use that one (only the last fallback layer is what we just obtained as a treeName, i.e., either the :treeName suffix or, if no suffix it given, "CollectionTree")
              treeName = campConfig.getTagStandardStringDefault("~dataPathsTreeName",campConfig.getTagStandardStringDefault("~dataTreeName",campConfig.getTagStandardStringDefault("~treeName",treeName)))
              QFramework.INFO("adding data samples from path '{:s}' with file pattern '{:s}'".format(path.Data(),dataFileNamePattern.Data()))
              parser.setTagString("treeName",treeName)
              # add the data samples
              for c in channels:
                  # First, check if channel is matching the one in a potential restricted path, iff not, skip
                  if isRestrictedPath:
                      channelFolderLayer = config.getTagIntegerDefault("channelFolderLayer", 1)
                      if not common.isFolderInPathAtLevel(c, pathselect, channelFolderLayer):
                          QFramework.INFO("You specified a restricted path: '{:s}' which does not match " \
                                          "the current channel '{:s}' that is looped over. " \
                                          "Skipping!".format(", ".join([str(p) for p in pathselect]),  c))
                          continue
                  QFramework.START("r",c)
                  tags.setTagString(channelPlaceholder,c)
                  # add the samples indiscriminately
                  if parser.addAllSamplesFromPath(path,tags.replaceInText(dataFolderPattern),dataFileNamePattern,"./","generator='ATLAS', processinfo='p-p-collisions', isInitialized=true, {campaignPlaceHolder}='{campaignName}'".format(campaignPlaceHolder=campaignPlaceHolder,campaignName=campaignName) ) > 0:
                      QFramework.END(QFramework.TQMessageStream.OK)
                  else:
                      QFramework.END(QFramework.TQMessageStream.FAIL)
      elif dataFileList:
          for df in dataFileList:
              datafile = common.findConfigPath(df)
              # use a dedicated list to explicitly add the data files
              QFramework.INFO("adding data samples from file {:s}'".format(datafile))
              treeName = "CollectionTree"
              #if the treeName is explicitly specified via a dedicated tag use that one (only the last fallback layer is what we just obtained as a treeName, i.e., either the :treeName suffix or, if no suffix it given, "CollectionTree")
              treeName = campConfig.getTagStandardStringDefault("~dataFileListTreeName",campConfig.getTagStandardStringDefault("~dataTreeName",campConfig.getTagStandardStringDefault("~treeName",treeName)))
              
              parser.setTagString("treeName",treeName)
              for c in channels:
                  QFramework.START("r",c)
                  tags.setTagString(channelPlaceholder,c)
                  # add the samples indiscriminately
                  if parser.addAllSamplesFromList(datafile,tags.replaceInText(dataFolderPattern),"generator='ATLAS', processinfo='p-p-collisions', isInitialized=true, {campaignPlaceHolder}='{campaignName}'".format(campaignPlaceHolder=campaignPlaceHolder,campaignName=campaignName) ) > 0:
                      QFramework.END(QFramework.TQMessageStream.OK)
                  else:
                      QFramework.END(QFramework.TQMessageStream.FAIL)

      else:
          QFramework.WARN("no data path given - running over MC only!")

      # save the updated XSec parser in the sample folder
      common.storeXSecParser(parser, samples, pathToXSP)


    return


#######################################################################################################################

def initializeMonteCarlo(config, samples, pathToMCDirStruct=defaultConfig.pathToMCDirStruct,MCbasePaths=["sig", "bkg"],isCampaignConfig=False):
    """initialize all samples in the given sample folder with their corresponding file paths, tree names and sum-of-weights"""
    from ROOT import TString
    from ROOT.std import vector
        
    # get potential restricted sample paths
    if not config.Class() == QFramework.TQTaggable.Class():
        # Check the above as a temporary fix for initializing hww fakes (in hwwinitialize.py) in the old
        # HWWAnalysisCode setup with new CAFCore setup
        # old (new) referring to before (after) implementation of campaign support
        pathselect_tmp, isRestrictedPath = common.getRestrictedPaths(config)
        if not pathselect_tmp: # try to get pathselect from config directly (used in hww fake initialization)
            pathselect_tmp = config.getTagVString("pathselect")
            isRestrictedPath = False if len(pathselect_tmp) == 0 or ( len(pathselect_tmp) == 1 and TString(pathselect_tmp[0]).IsNull() ) else True
    else:
        isRestrictedPath = False

    # check if potential restricted path is actually an MC sample path
    if isRestrictedPath:
        pathselect = common.getMatchingBasePaths(restrictedPaths = pathselect_tmp, basePaths = MCbasePaths)
        if not pathselect:
            QFramework.INFO("You run with the restrict option set to '{:s}', which is not an MC sample path. " \
                            "Nothing to do in 'initializeMonteCarlo(...)', returning from function...".format(" ".join(map(str,pathselect_tmp))))
            return
    
    listOfCampaignConfigs, hasCampaigns = common.getCampaignSubConfigList(config)
    # special case when a campaigns config (subconfig) is already provided
    if isCampaignConfig: hasCampaigns = True
    
    # Loop over campaigns ---->>
    for campConfig in listOfCampaignConfigs:
        
      # if path is restricted, check if campaigns are
      # matching for the current loop, else skip
      if hasCampaigns and isRestrictedPath:
          campaignFolderLayer = config.getTagIntegerDefault("campaignFolderLayer", 2)
          if not common.isFolderInPathAtLevel(campConfig.GetName(), pathselect, campaignFolderLayer):
              QFramework.INFO("You specified a restricted path: '{:s}' which does not match " \
                              "the current campaign '{:s}' that is looped over. " \
                              "Skipping!".format(", ".join([str(p) for p in pathselect]),  campConfig.GetName()))
              continue
          
      campSuffix = ("_"+campConfig.GetName()) if hasCampaigns else "" #if no campaign split present no suffix is needed
     
      # retrieve some basic information from the config
      mcPaths = campConfig.getTagVStandardString("mcPaths")
      channels = campConfig.getTagVString("~channels")
      channelPlaceHolder = campConfig.getTagStringDefault("~channelPlaceholder","channel")
      campaignPlaceHolder = campConfig.getTagStringDefault("~campaignPlaceholder","campaign")
      
      # Retrieve the luminosity information, first looking directly in the sampleFolder
      # then using a value from the config as a fallback. In case we have subcampaigns a global 
      # luminosity setting does not make sense, we therefore explicitly veto this case
      if samples.hasTagDouble("luminosity") and hasCampaigns:
          QFramework.BREAK("Found a luminosity value on the (base) SampleFolder but configuration specifies (sub) campaigns. Luminosity must be specified in each (sub) campaign config section and NOT as a general luminosity on the (base) SampleFolder! (To specify a per-campaign luminosity via the SampleFolder use a tag of the format 'luminosity.nameOfCampaign' insteadof 'luminosity')")
      if config.hasTagDouble("luminosity") and hasCampaigns and not isCampaignConfig: #explicitly checking base config here
          QFramework.BREAK("Found a luminosity value in the base master config but configuration specifies (sub) campaigns. Luminosity must be specified in each (sub) campaign config section and NOT as a general luminosity on the base master config!")
      #default values:
      luminosity = 0.
      luminosityUnit = "pb"
      campaignName = ""
      lumiTagName = "luminosity" #this is the dynamic lumi tag (i.e. the one on the sampleFolder where the name depends on the campaign).
      if hasCampaigns:
          campaignName = campConfig.GetName()
          lumiTagName += "."+campConfig.GetName()
      #the ambiguous case first (lumi defined on both sampleFolder and config)
      if samples.hasTagDouble(lumiTagName) and campConfig.getTagDoubleDefault("~luminosity"):
          #get values from both sources, issue a warning if they are inconsistent
          lumiSF   = samples.getTagDoubleDefault(lumiTagName, luminosity)
          lumiConf = campConfig.getTagDoubleDefault("~luminosity", luminosity)
          if not (lumiSF == lumiConf):
              QFramework.WARN("Found inconsistent luminosity values from SampleFolder ({:.2f}) and master config ({:.2f}), will use the latter and overwrite the one on the SampleFolder!".format(lumiSF,lumiConf, " for campaign '{:s}'".format(campConfig.GetName()) if hasCampaigns else "" ))
              samples.setTagDouble(lumiTagName,lumiConf)
          luminosity = lumiConf
      elif samples.hasTagDouble(lumiTagName):#lumi is only on the SampleFolder, use it
          luminosity = samples.getTagDoubleDefault(lumiTagName,luminosity)
      elif campConfig.hasTag("~luminosity"):
          luminosity = campConfig.getTagDoubleDefault("~luminosity",luminosity)
      else: #no lumi found on neither SF nor config -> break!
          QFramework.BREAK("No luminosity specified, please provide a luminosity value in the master (campaign) config (key: 'luminosity') or via the SampleFolder (key: 'luminosity{:s}')!".format(".nameOfCampaign" if hasCampaigns else "") )
          
      #checking for inconsistencies inthe lumi unit is less relevant as an error here will be much more obvious to the user
      if campConfig.hasTagString("~luminosityUnit"):
          luminosityUnit = campConfig.getTagStandardStringDefault("~luminosityUnit","pb")
      elif samples.hasTagString("luminosityUnit"):
          luminosityUnit = samples.getTagStandardStringDefault("luminosityUnit","pb")
      else:
          QFramework.BREAK("No luminosity unit found in either the sample folder or master config. Please provide a luminosity unit to use.")

      luminosityInPb = luminosity / QFramework.TQXSecParser.unit(luminosityUnit)

      # setup a sample initializer to find the samples on the disk
      initializer = QFramework.TQSampleInitializer()
      initializer.setPrettyPrint(campConfig.getTagBoolDefault("~prettyPrint",True))
      initializer.setLineUpdates(campConfig.getTagBoolDefault("~lineUpdates",True)) 
      appname = QFramework.TQLibrary.getApplicationName().Data()           
      initializer.setVisitTraceID(appname)

      # read the average event weight / sumOfWeightsPerEvent from an external txt file
      initializer.setTagString("averageWeightName",campConfig.getTagStringDefault("~averageWeightName","averageWeight"))

      # If a dummy initializer is specified, then tell it to not actually initialize anything
      if campConfig.getTagBoolDefault("~initializer.dummy",False):
          initializer.setTagBool("initialize",False)

      # check if the list of input files was provided as a text file, or whether we need to run "ls" ourselves
      if not (campConfig.hasTagString("mcFileList") or len(campConfig.getTagVStandardString("mcFileLists")) >= 1) : #explicitly NOT getting recursively as files should be provided on a per-campaign basis
          # no file list provided, expecting now a path to search with
          initializer.setTagString("filenameSuffix",campConfig.getTagStringDefault("~mcFilenameSuffix","*.root*"))
          initializer.setTagString("filenamePrefix",campConfig.getTagStringDefault("~mcFilenamePrefix","*"))
          if len(mcPaths) == 0:
              QFramework.BREAK("no mc sample paths provided. please use the '{:s}' option to add one".format("mcPaths"))
      else:
          # input file list found
          mcPaths=[""]
          initializer.setTagString("filenameSuffix",campConfig.getTagStringDefault("~mcFilenameSuffix","*.root*"))
          initializer.setTagString("filenamePrefix",campConfig.getTagStringDefault("~mcFilenamePrefix","*"))

      # saving the cutbookkeepers can be enabled
      initializer.setTagBool("extractCounters", campConfig.getTagBoolDefault("~initializer.extractCutBookKeeperCounters", False))

      # by default, the initializer will use the GetEntries function of the tree
      # to learn about the number of entries. however, a different method is also
      # supported: usage of a count histogram, where the first bin will hold
      # the number of entries. this might be required depending on the sklimming.
      # we can safely enable it, since the tree entry count will be used as fallback.
      if campConfig.getTagBoolDefault("~initializer.useCountHistogram",False):
          initializer.setTagBool   ("useCountHistogram"  ,True)
          initializer.setTagBool   ("extractCounters", False)
          initializer.setTagString ("countHistogram.name",campConfig.getTagStringDefault("~initializer.countHistogramName","Count"))
          initializer.setTagInteger("countHistogram.bin" ,campConfig.getTagIntegerDefault("~initializer.countHistogramBinNumber",1))

      # as an alternative, the number of entries can be retrieved from a tree
      if campConfig.getTagBoolDefault("~initializer.useSummarytree",False):
          initializer.setTagBool  ("useSummarytree"        , True)
          initializer.setTagBool   ("extractCounters", False)
          initializer.setTagString("summaryTree.name"      , campConfig.getTagStringDefault("~initializer.summaryTree.name","summary"))
          initializer.setTagString("summaryTree.expression", campConfig.getTagStringDefault("~initializer.summaryTree.expression","initialSumW"))

      # reading of the cutbookkeepers can be disabled, if they are not present (for example when using a TRUTH xAOD)
      initializer.setTagBool("useCutBookKeeper", campConfig.getTagBoolDefault("~initializer.useCutBookKeeper",True))
      initializer.setTagString("xAODCutBookKeeperContainer", campConfig.getTagStringDefault("~initializer.xAODCutBookKeeperContainer","StreamAOD"))

      # set luminosity information - this will be used for scaling samples
      # according to their respective cross sections and luminosity
      # as compared to data
      QFramework.INFO("scaling{:s} to luminosity: {:g}/{:s} ({:g}/pb)".format(" "+campaignName if hasCampaigns else "",luminosity,luminosityUnit,luminosityInPb))
      initializer.setTagDouble("luminosity",luminosityInPb)

      # set the tree name pattern to be used in order to find the trees
      channelFolderPattern = campConfig.getTagStringDefault("~channelFolderPattern","?/$({:s})".format(channelPlaceHolder));

      # request creation of counters to include the initial nominal event counts
      initializer.setTagString("makeNominalCounter","initial")

      # Loop over mcPaths ---->>
      # call upon the initializer to do its magic
      for directory in mcPaths:
          mcFileList = list(campConfig.getTagVStandardString("mcFileLists")) or list(campConfig.getTagVStandardString("mcFileList")) # if campConfig.hasMatchingTag("mcFileLists") else
          if len(mcFileList) == 0: #explicitly NOT searching recursively, this is a per-campaign setting
              path = TString()
              treeName = TString()
              QFramework.TQFolder.parseLocation(directory,path,treeName);
              # if no tree is defined in path, set the default (might be overwritten afterwards if dedicated options are set)
              if not treeName:
                  treeName = "CollectionTree"
              #if the treeName is explicitly specified via a dedicated tag use that one (only the last fallback layer is what we just obtained as a treeName, i.e., either the :treeName suffix or, if no suffix it given, "CollectionTree")
              treeName = campConfig.getTagStandardStringDefault("~mcPathsTreeName",campConfig.getTagStandardStringDefault("~mcTreeName",campConfig.getTagStandardStringDefault("~treeName",treeName)))
              
              QFramework.INFO("initializing samples from path '{:s}' in tree '{:s}' - this may take a while".format(path.Data(),treeName))
              # read the directory from disk to enable the initializer to find the files
              QFramework.START("r","crawling filesystem")
              initializer.readDirectory(path)
              if initializer.getDirectory() and initializer.getDirectory().getNObjects() > 0:
                  QFramework.END(QFramework.TQMessageStream.OK)
              else:
                  QFramework.END(QFramework.TQMessageStream.FAIL)
                  QFramework.BREAK("unable to read inputs from path '{:s}'".format(path))
              initializer.setTagString("treeName",treeName)
          else:
            for filelist in mcFileList:
                mcFileList_abs = common.findConfigPath(filelist)
                QFramework.INFO("initializing samples from input file list '{:s}'".format(str(filelist)))
                treeName = "CollectionTree"
                #if the treeName is explicitly specified via a dedicated tag use that one (only the last fallback layer is what we just obtained as a treeName, i.e., either the :treeName suffix or, if no suffix it given, "CollectionTree")
                treeName = campConfig.getTagStandardStringDefault("~mcFileListTreeName",campConfig.getTagStandardStringDefault("~mcTreeName",campConfig.getTagStandardStringDefault("~treeName",treeName)))
                initializer.setTagString("treeName",treeName)
                if not initializer.readInputFilesList(mcFileList_abs, False, False):
                    QFramework.ERROR("unable to read '{:s}' - please check!".format(filelist))

          if campConfig.getTagBoolDefault("~verbose",False):
              initializer.setVerbose(True)

          # Loop over channels ---->>
          for c in channels:
              
              # First, check if channel is matching the one in a potential restricted path
              # If not, skip
              if isRestrictedPath:
                  channelFolderLayer = config.getTagIntegerDefault("channelFolderLayer", 1)
                  if not common.isFolderInPathAtLevel(c, pathselect, channelFolderLayer):
                      QFramework.INFO("You specified a restricted path: '{:s}' which does not match " \
                                      "the current channel '{:s}' that is looped over. " \
                                      "Skipping!".format(", ".join([str(p) for p in pathselect]),  c))
                      continue
              
              # since the trees are named differently for the different channels,
              # we need to announce the channel to the initializer
              chlong = TString(c)
              chlong.ReplaceAll("m","mu")
              initializer.setTagString(channelPlaceHolder,c)
              initializer.setTagString("channellong",chlong)
              # get the patterns which should get us all channel level sampleFolders...
              # channelFolderPattern is e.g. ?/$(channel)
              # channelFolder is e.g. ?/em
              channelFolder = initializer.replaceInText(channelFolderPattern)
              #...store the channel tag on the so identified folders...
              samples.setTagString(channelPlaceHolder,c,channelFolder)
              #...and get now all folders we should process which also are flagged for the current campaign
              channelTag = QFramework.TQValue.newInstance(channelPlaceHolder,c)
              requirements = vector('const TQValue*')() #creates an empty std::vector equivalent object
              requirements.push_back(channelTag)
              if hasCampaigns:  
                  campaignTag = QFramework.TQValue.newInstance(campaignPlaceHolder,campConfig.GetName())
                  requirements.push_back(campaignTag) #add campaign selection tag if needed
              selectedFolders = samples.getSampleFoldersWithTagsEquivalentTo(requirements) #
              
              # check if restrict option was used and select only matching folders out of selectedFolders
              if isRestrictedPath:
                  restrictedFolders = vector('TQSampleFolder*')()
                  for p in pathselect:
                      # p is e.g.: /sig/?/?/mh125/ggf
                      # expand selected path
                      restrictedPaths = samples.getFolderPaths(p)
                      for path in restrictedPaths:
                          # path is e.g.: /sig/em/c16a/mh125/ggf
                          QFramework.TQStringUtils.removeLeading(p, "/") # remove potential leading '/'
                          for f in selectedFolders:
                              # f.getPath() is e.g.: /sig/em/c16a/mh125/ggf/345324_s
                              if f.getPath().Contains(path):
                                  restrictedFolders.push_back(f)
                              
                  if restrictedFolders.size() != 0:
                      QFramework.INFO("Restrict option was specified, initializing samples only for " \
                                      "paths: {:s}".format(", ".join([str(f.getPath()) for f in restrictedFolders])))
                      selectedFolders = restrictedFolders
                  else:
                      QFramework.BREAK("Restrict option was specified to '{:s}' but no matching " \
                                      "sample folders were found! Please check your provided sample path. Exiting..."  \
                                      "".format(", ".join([str(p) for p in pathselect]), campConfig.GetName()))
                      
              QFramework.START("r",channelFolder)
              # actually initialize the samples
              if samples.visitSampleFolders(initializer, selectedFolders) >= config.getTagIntegerDefault("initializer.minSamples",1):
                  QFramework.END(QFramework.TQMessageStream.OK)
              else:
                  QFramework.END(QFramework.TQMessageStream.FAIL)
                  if config.getTagBoolDefault("printSamplesFailed",False):
                      initializer.getDirectory().printContents("rdt")

      # <<---- end loop over channels and mcPaths
      
      # save the directory structure in the sample folder
      storeMCDirStruct(initializer, samples, pathToMCDirStruct+campSuffix)

    # <<---- end loop over campaigns
    return

#######################################################################################################################

def storeMCDirStruct(initializer, samples, pathToMCDirStruct=defaultConfig.pathToMCDirStruct):
    """store the Monte Carlo directory structure contained in the given sample initializer in the sample folder.
    Do nothing if an object with the same name is found"""

    MCDirStruct = initializer.getDirectory()

    MCDirStructFolder = samples.getFolder(pathToMCDirStruct+"+")

    # if an object of the same name is not found in the path provided
    if not MCDirStructFolder.hasObject(MCDirStruct.getName()):
        # try storing the directory structure and print an error if failed
        if not MCDirStructFolder.addObject(MCDirStruct):
            QFramework.ERROR("cannot add monte carlo directory structure to sample folder")

    return
