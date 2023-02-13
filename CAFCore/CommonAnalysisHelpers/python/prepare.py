import QFramework
from . import common
from CommonAnalysisHelpers import defaultConfig

"""this submodule contains helper functions to handle the initialzation of analysis sample folders"""

def DefaultArgumentParser():
    """create a pre-configured argument parser with some default options tailored for a 'prepare' step"""
    parser = common.createBasicArgumentParser("create a new sample folder, adding data and monte carlo samples")
    return parser

#######################################################################################################################

def addMonteCarlo(config, samples, pathToXSP=defaultConfig.pathToXSP, XSPName=defaultConfig.XSPName, XsecUnit=defaultConfig.XsecUnit):
    """add a list of Monte Carlo samples to a sample folder using a cross section parser"""
    from ROOT import TString
    #Get a list of sub-configs for each specified subcampaign. If no 
    #subcampaigns are listed in config the regular config object 
    #is the only element in the returned list
    listOfCampaignConfigs, hasCampaigns = common.getCampaignSubConfigList(config)
    for campConfig in listOfCampaignConfigs:
      campSuffix = ("_"+campConfig.GetName()) if hasCampaigns else "" #if no campaign split present no suffix is needed
      if hasCampaigns: QFramework.INFO("Adding MC for campaign '{:s}'".format(campConfig.GetName()))
      # retrieve a XSecParser
      parser = common.getXSecParser(campConfig, samples, pathToXSP, XSPName+campSuffix)
      
      #add campaign name for repalcements in path pattern (relates to mapping read below)
      campaignPlaceHolder = campConfig.getTagStringDefault("~campaignPlaceholder","campaign")
      #if no campaigns were specified, the campaign name is an empty string
      parser.setTagString(campaignPlaceHolder, campConfig.GetName() if hasCampaigns else "")
      #tell the XSP to export this special tag to all samples it creates
      parser.addTagKeyToExport(campaignPlaceHolder)
      
      # set the XSec unit
      unit = QFramework.TQXSecParser.unitName(QFramework.TQXSecParser.unit(campConfig.getTagStringDefault("~XsecUnit",XsecUnit))).Data()
      QFramework.INFO("parsing cross-section files (assuming unit: {:s})".format(unit))
      parser.setTagString("xSectionUnit",unit)
   
      # parse the listed XSec files, adding the entries into the XSecParser
      # and returning the number of files processed
      nfiles = readXSecFiles(campConfig, parser)

      # read the "mapping", i.e. the paths to which the samples are mapped in the folder structure
      readMapping(campConfig, parser)

      # apply whitelisting of DSIDs
      applyWhitelist(campConfig, parser) 

      # read in SumOfWeightsPerEvent information, if provided
      readSumOfWeights(campConfig, parser)

      # let the TQXSectionParser do it's magic and create a folder structure
      if not parser.isGood() or nfiles < 1: 
          QFramework.BREAK("no valid cross section files listed. please use the '{:s}' option to schedule cross section files".format("XsecFiles"))
      else:
          # TODO: how do we want to label "priority" nowadays?
          # add only samples that have a mapping and a priority
          QFramework.START("l.","adding mapped and whitelisted Monte Carlo samples to the sample folder")
          if parser.addAllSamples(True,True) > 0:
              QFramework.END(QFramework.TQMessageStream.OK)
          else:
              QFramework.END(QFramework.TQMessageStream.FAIL)

      # save the updated XSec parser in the sample folder
      common.storeXSecParser(parser, samples, pathToXSP)

    return

#######################################################################################################################

def readXSecFiles(config, parser):
    """inform the XSecParser about all of the MC DSIDs listed in the XSec files provided by the config"""
    from ROOT import TString

    # retrieve the filenames
    nfiles = 0
    XsecFiles = config.getTagVString("~XsecFiles")

    # read them in sequentially
    for obj in XsecFiles:
        # TODO: this is potentially confusing - do we even use the option of setting a tree name to the parser when reading the XSec file names?
        # do we ever use e.g. XSec/XS_13TeV_bkg.csv:CollectionTree?
        fileName = TString()
        treeName = TString()
        QFramework.TQFolder.parseLocation(obj,fileName,treeName)
        if not treeName.IsNull():
            parser.setTagString("treeName",treeName)
        file_abs = TString(common.findConfigPath(fileName))
        QFramework.START("r",fileName)
        if fileName.EndsWith(".csv"):
            if parser.readCSVfile(file_abs,config.getTagStringDefault("XsecColSep",",")):
                QFramework.END(QFramework.TQMessageStream.OK)
                nfiles = nfiles+1
            else:
                QFramework.END(QFramework.TQMessageStream.FAIL)
        elif fileName.EndsWith(".txt"):
            if parser.readTSVfile(file_abs,config.getTagStringDefault("XsecColSep"," \t")):
                QFramework.END(QFramework.TQMessageStream.OK)
                nfiles = nfiles+1
            else:
                QFramework.END(QFramework.TQMessageStream.FAIL)

    return nfiles

#######################################################################################################################

def readMapping(config, parser):
    """inform the XSecParser about where each DSID should live in the sample folder
    using either a dedicated mapping file or the 'path' column from the XSec files"""

    mapped = False
    for XsecMap in config.getTagVStandardString("~XsecMap"):
        # one (or several) map files given - read them
        XsecMap_abs = common.findConfigPath(XsecMap)
        QFramework.START("l.","reading path mapping from map file '{:s}'".format(XsecMap))
        if parser.readMapping(XsecMap_abs) > 0:
            # for bookkeeping reasons, we might want to save the paths in the parsed table
            parser.writeMappingToColumn("path")
            QFramework.END(QFramework.TQMessageStream.OK)
            mapped = True
        else:
            QFramework.END(QFramework.TQMessageStream.FAIL)
    if not mapped:
        # no map file given, taking from 'path' column of Xsec-Table
        QFramework.START("l.","reading path mapping from X-section file column 'path'")
        if parser.readMappingFromColumn("path") > 0:
            QFramework.END(QFramework.TQMessageStream.OK)
        else:
            QFramework.END(QFramework.TQMessageStream.FAIL)

    return

#######################################################################################################################

def applyWhitelist(config, parser):
    """inform the XSecParser about which DSIDs should actually be added to the sample folder
    using either a dedicated whitelist file or the DSID 'priority' provided by the config and XSec files"""
    from ROOT import TString
    from ctypes import c_int

    # set and apply the sample priorities
    priorityColumn = TString("")
    # first, we enable all samples
    parser.enableAllSamples()
    if config.getTagString("~XsecPriorityColumn",priorityColumn):
        # if a priority column was set, use the contents as follows:
        #p = ROOT.Long(1)
        p = c_int(1)
        if config.getTagInteger("~XsecPriorityMax",p):
            #  disable all samples that have a priority greater than XsecPriorityMax
            parser.disableSamplesWithPriorityGreaterThan(priorityColumn,p.value)
        if config.getTagInteger("~XsecPriorityMin",p):
            #  disable all samples that have a priority smaller than XsecPriorityMin
            parser.disableSamplesWithPriorityLessThan(priorityColumn,p.value)
    whitelist = TString("")
    if config.getTagString("~XsecWhitelist",whitelist):
        # applying sample whitelist
        whitelist_abs = TString(common.findConfigPath(whitelist))
        QFramework.INFO("applying sample whitelist from file '{:s}'".format(whitelist.Data()))
        parser.applyWhitelist(whitelist_abs)

    return

#######################################################################################################################

def readSumOfWeights(config, parser):
    """inform the XSecParser about sumOfWeights if an appropriate CSV file is supplied"""
    from ROOT import TString

    sumOfWeightsFile = TString()
    if config.getTagString("~averageweights.filename",sumOfWeightsFile):
        sumOfWeightsFile_abs = common.findConfigPath(sumOfWeightsFile)
        QFramework.START("l.","reading normalization information from file '{:s}'".format(sumOfWeightsFile))
        sumOfWeights = QFramework.TQTable("WeightFileReader")
        if sumOfWeights.readCSVfile(sumOfWeightsFile_abs,",") and parser.readColumn(sumOfWeights,config.getTagStringDefault("~averageweights.colname","Nominal"),"SampleID")>0:
            parser.setEntry(0,parser.getNcols()-1,config.getTagStringDefault("~averageWeightName","averageWeight"))
            QFramework.END(QFramework.TQMessageStream.OK)
        else:
            QFramework.END(QFramework.TQMessageStream.FAIL)

    return
