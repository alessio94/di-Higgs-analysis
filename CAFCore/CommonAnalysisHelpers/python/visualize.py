import QFramework
from . import common
from CommonAnalysisHelpers import defaultConfig

"""this submodule contains helper functions for visualizing the results of the analysis"""

def DefaultArgumentParser():
    """create a pre-configured argument parser with some default options tailored for a 'visualize' step"""
    parser = common.createBasicArgumentParser("visualize the results, making plots, cutflows and more from a sample folder on which you executed your analysis")
    return parser

#######################################################################################################################

def getOutputDir(config):
    """parse the name of the directory to which all of the results will be written"""
    outputdir = config.getTagStandardStringDefault("outputDir","results")
    outputdir.replace("$(variation)",config.getTagStandardStringDefault("variation","nominal"))
    return outputdir

#######################################################################################################################

def getDictOfChannels(config):
    """retrieve the channels which are to be considered for visualizing the results.
    channels will be a dictionary holding the channels specified by the user,
    each of which will be a dictionary with the properties of that channel
    """
    #moved to common, hence, for compatibility reasons simply forward the call
    return common.getDictOfChannels(config)
    
    rawchannels = [ c for c in config.getTagVStandardString("channels") ]
    channeltab = QFramework.TQTable()
    channeltabfile = config.getTagStringDefault("channelConfig","common/config/channels/channels.txt")
    channeltabfile_abs = common.findConfigPath(channeltabfile)
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

def getDictOfCampaigns(config):
    """retrieve the campaigns which are to be considered for visualizing the results.
    campaigns will be a dictionary holding the campaigns specified by the user,
    each of which will be a dictionary with the properties of that campaign
    """
    rawcampaigns = [ c for c in config.getTagVStandardString("visualizeCampaigns") ]
    campaigntab = QFramework.TQTable()
    campaigntabfile = config.getTagStringDefault("visualizeCampaignConfig","common/config/campaigns/campaigns.txt")
    campaigntabfile_abs = common.findConfigPath(campaigntabfile)
    campaigntab.readTSVfile(campaigntabfile_abs)
    campaignNameColumn = campaigntab.findColumn("name",False)
    campaignPathColumn = campaigntab.findColumn("path",False)
    campaignLatexColumn = campaigntab.findColumn("latex",False)
    campaignDataPathColumn = campaigntab.findColumn("datapath",False) #optional, needed when trying to plot systematic variations with data
    if max([campaignNameColumn,campaignPathColumn,campaignLatexColumn])<0:
        QFramework.WARN("failed to find one of the following columns in the campaign definition file '{:s}': name, path, latex, or datapath".format(campaigntabfile))

    campaignsDict = {}
    for i in range(1,campaigntab.getNrows()):
        campaignnames =  str(campaigntab.getEntry(i,campaignNameColumn,"verbatim")).split(",")
        cpath = campaigntab.getEntry(i,campaignPathColumn,"verbatim")
        cdatapath = cpath if campaignDataPathColumn<0 else campaigntab.getEntry(i,campaignDataPathColumn,"verbatim")
        campaignpath = cpath.Data()
        campaigndatapath = cdatapath.Data()
        campaignpretty = campaigntab.getEntryLaTeX(i,campaignLatexColumn).Data()

        # TODO: should probably warn the user righ away if one of the raw campaigns specified is not in the sample folder provided
        #        but that wouldn't go this far in the loop - maybe create a separate function which checks the campaigns dict as input?
        for craw in rawcampaigns:
            if craw in campaignnames or craw is campaignpath:
                campaignsDict[str(campaignnames[0])] = {
                    "names" : campaignnames, # can be a list of multiple strings which mean the same thing, i.e. [emme,meem,df]
                    "path" :  campaignpath,
                    "datapath" :  campaigndatapath,
                    "latex" : campaignpretty,
                    "roottex" : QFramework.TQStringUtils.convertLaTeX2ROOTTeX(campaignpretty)
                }

    if not campaignsDict:
        QFramework.INFO("No campaigns dictionary could be found. Maybe the campaigns you listed " \
                        "- which are '{:s}' -  are not contained in '{:s}'. Or did you forget to specify the desired campaigns with " \
                        "the tag 'visualizeCampaigns'? If you are simply running without different campaigns specified you " \
                        "can ignore this warning.".format(",".join(rawcampaigns),campaigntabfile.Data()))
        return {None: {}} # format of default campaignDict in other visualize functions

    return campaignsDict

#######################################################################################################################

def initializeSampleDataReader(config, samples):
    """initialize the object which is in charge of retrieving analysis results from a sample folder"""
    reader = QFramework.TQSampleDataReader(samples)
    if config.getTagBoolDefault("verbose",False):
        reader.setVerbose(5)

    return reader

#######################################################################################################################

def generateFakeData(config):
    """generate and store fake data from a given source"""
    from ROOT import TString
    fakedata = TString()
    channelPlaceholder = config.getTagStandardStringDefault("~channelPlaceholder","channel")
    campaignPlaceholder = config.getTagStandardStringDefault("~campaignPlaceholder","campaign")
    if config.getTagString("fakeData.source",fakedata):
        tags = QFramework.TQTaggable()
        campaigns = config.getTagVStandardString("fakeData.campaigns")
        if len(campaigns)<1:
          campaigns += [""]
        for ch in config.getTagVStandardString("fakeData.channels"):
          for camp in campaigns:
            tags.setTagString(channelPlaceholder,ch)
            tags.setTagString(campaignPlaceholder,camp)
            path = tags.replaceInText(fakedata)
            fakedatadir =  config.getTagStringDefault("fakeData.target", QFramework.TQFolder.concatPaths("data",ch,camp,"fake"))
            QFramework.WARN("generating fake data at '{:s}'".format(fakedatadir))
            reader.copyData(tags.replaceInText(fakedata),fakedatadir,"rerollPoisson=true")
    return

#######################################################################################################################

def generateNormalizationFactors(config, samples, campaignsDict={None: {}}):
    """generate the normalization factors"""
    from ROOT import TString
    if config.getTagBoolDefault("doNFs",False):
        nfconfigs = config.getTagVString("nfConfig")
        nfconfigs_abs = common.findMultipleConfigPathsFromList(nfconfigs)
        nfVariationFiles = config.getTagVString("nfVariations")
        nfVariationsFolder = QFramework.TQFolder.newFolder("nfVariations")
        for fileName in nfVariationFiles:
            fileName_abs = common.findConfigPath(fileName)
            nfVariationsFolder.importFromTextFile(fileName_abs)
        nfVariations = nfVariationsFolder.getListOfFolders()

        campaignTag = None
        # only need to check this if we are in campaigns mode
        if campaignsDict.keys()[0]:
            campaignName = TString()
            # if the following tag is set it will be used for specifying the
            # campaign, otherwise we try to get it from the campaignsDict
            if not config.getTagString("nfCampaign", campaignName):
                # take the last campaign that is specified but throw a warning
                # if there are more than one to choose from
                campaignName = campaignsDict.keys()[-1]
                if len(campaignsDict.keys()) > 1:
                    QFramework.WARN("You specified more than one campaign " \
                                    "for visualizing and also want to calculate normalization factors. " \
                                    "Please specify the tag 'nfCampaign' to explicitely set your desired campaign "
                                    " to derive normalization factors for. " \
                                    "For now, the campaign in your list with the name '{:s}' is chosen for "\
                                    "deriving the NFs.".format(campaignName))
            campDictName = ""
            for camp in campaignsDict.keys():
                try:
                    if campaignName in campaignsDict[camp]["names"]:
                        campDictName = camp
                except:
                    QFramework.WARN("Something odd happened. You have a setup with campaigns as well as "\
                                    "a campagin dictionary specified .This, however, does not have an entry called 'names' which "\
                                    "needs to be present to use the normalization feature.")
            # now create the TQTaggable that is parsed to the TQNFChainloader later
            if not campDictName in campaignsDict.keys():
                QFramework.INFO("Your campaigns dictionary:")
                print("\n".join("{}\t{}".format(k, v) for k, v in campaignsDict.items()))
                QFramework.BREAK("Your specified campaign name '{:s}' for deriving normalization " \
                                 "factors cannot be found in the campaigns dictionary which is created from " \
                                 "the tag 'visualizeCampaigns' and looks as printed above.".format(campaignName))
            else:
                campaignPath = campaignsDict[str(campDictName)]["path"]
            campaignTag = QFramework.TQTaggable()
            campaignTag.setTag(config.getTagStandardStringDefault("~campaignPlaceholder","campaign"), campaignPath)

        if (not nfVariations) or len(nfVariations)<1:
            cl = QFramework.TQNFChainloader(nfconfigs_abs,samples)
            cl.importTagsWithoutPrefix(config,"NF.")
            if campaignTag: cl.setCampaignTag(campaignTag)
            cl.initialize()
            cl.execute()
            cl.finalize()
        else:
            for variation in nfVariations:
                cl = QFramework.TQNFChainloader(nfconfigs_abs,samples)
                cl.importTagsWithoutPrefix(config,"NF.")
                cl.setVariationTags(variation)
                if campaignTag: cl.setCampaignTag(campaignTag)
                cl.initialize()
                cl.execute()
                cl.finalize()
    return

#######################################################################################################################

def createSummaryDocument(samples, outputdir, pathToConfig=defaultConfig.pathToConfig):
    """create a document which will summarize all of the results"""
    from getpass import getuser
    from time import strftime

    outputpath = QFramework.TQFolder.concatPaths(outputdir,"summary.tex").Data()
    outputpath = QFramework.TQPathManager.getPathManager().getLocalPath(outputpath)
    summary = open(outputpath,"w")

    summary.write("\\documentclass{article}\n")
    summary.write("\\usepackage[a4paper,margin=2cm]{geometry}\n")
    summary.write("\\title{Analysis Summary}\n")
    summary.write("\\author{generated with CAFCore/CommonAnalysisHelpers by user "+getuser()+"}\n")
    summary.write("\\date{"+strftime("%d-%m-%Y")+"}\n")
    summary.write("\\usepackage{adjustbox}\n")
    summary.write("\\usepackage{graphicx}\n")
    summary.write("\\usepackage[subpreambles]{standalone}\n")
    summary.write("\\usepackage{fancyhdr}\n")
    summary.write("\\pagestyle{fancy}\n")
    summary.write("\\setcounter{secnumdepth}{0}\n")
    summary.write("\\usepackage{tikz}\n")
    summary.write("\\usetikzlibrary{positioning,arrows}\n")
    summary.write("\\usepackage{hyperref}\n")
    summary.write("\\usepackage[open]{bookmark}\n")
    summary.write("\\begin{document}\n\\maketitle\n\\tableofcontents\n\n")
    summary.write("\\begin{itemize}\n")
    for folder in samples.getListOfFolders(QFramework.TQFolder.concatPaths(pathToConfig,"?").Data()):
        try:
            cli = folder.getFolder("CLI")
            configsString = cli.getTagStringDefault("config", "")
            optionsString = cli.getTagStringDefault("options", "")
            if configsString:
                configsString = str(configsString).replace("[","").replace("]","").replace("_", "\_")
            if optionsString:
                optionsString = str(optionsString).replace("[","").replace("]","").replace("_", "\_")                
        except:
            QFramework.WARN("Could not find configuration to display in preamble of summary tex file")
            configsString = "NA"
            options = "NA"
        summary.write("\\item \\textbf{"
                    +str(folder.getTagStringDefault(".createdBy","unknown script"))+"} was run "+
                    "on \\textbf{"+str(folder.getTagStringDefault(".creationDate","unknown date"))+"} "+
                    # TODO: update printout, SVN revision -> git hash
                    # "with SVN revision "+str(folder.getTagStringDefault(".libVersion","???"))+" "
                    "compiled with gcc "+str(folder.getTagStringDefault(".gccVersion","???"))+" "+
                    "under ROOT "+str(folder.getTagStringDefault(".rootVersion","???"))+" "+
                    "using the configuration \\textbf{"+configsString+"} " +
                    "with options \\textbf{"+optionsString+"} ")
    summary.write("\\end{itemize}\n")

    return summary

#######################################################################################################################

def createCutDiagram(config, samples, summary, outputdir, pathToCuts=defaultConfig.pathToCuts):
    """create a diagram which illustrates the cutflow and add a cut overview section to a summary file"""

    QFramework.INFO("creating cut diagram")

    cutfolder = samples.getFolder(QFramework.TQFolder.concatPaths(pathToCuts,"?").Data())
    cuts = QFramework.TQCut.importFromFolder(cutfolder)
    cuttextags = QFramework.TQTaggable("standalone=true")
    cuttextags.importTagsWithoutPrefix(config,"cutprinter.")
    outputpath = QFramework.TQFolder.concatPaths(outputdir,"cuts.tex")
    outputpath = QFramework.TQPathManager.getPathManager().getLocalPath(outputpath)
    cuts.writeDiagramToFile(outputpath,cuttextags)
    writesec(summary,"section","Cut Overview")
    outputpath = QFramework.TQFolder.concatPaths(outputdir,"cuts")
    #note: this output path is not an actual file, it's a path being written to the summary file
    #hence, don't use the following line:
    #outputpath = QFramework.TQPathManager.getPathManager().getLocalPath(outputpath)
    writeinput(summary,QFramework.TQPathManager.getPathManager().getTargetPath(outputpath, False))
    return

#######################################################################################################################

def printCutflows(config, reader, summary, channelsDict, outputdir, campaignsDict={None: {}}):
    """print a collection of analysis cutflows defined by the user in each of the requested formats"""
    # TODO: don't need to pass both samples and reader in necessarily
    from ROOT import TString
    from os.path import splitext,basename

    channelPlaceHolder = config.getTagStandardStringDefault("~channelPlaceholder","channel")
    dataChannelPlaceHolder = config.getTagStandardStringDefault("~dataChannelPlaceholder","dataChannel")

    campaignPlaceHolder = config.getTagStandardStringDefault("~campaignPlaceholder","campaign")
    dataCampaignPlaceHolder = config.getTagStandardStringDefault("~dataCampaignPlaceholder",campaignPlaceHolder) #recommended treatment is to use the same name for data and corresponding MC campaigns, hence, typically the placeholder can be identical. For channels the subfolder might contain a suffix for systematic variations, hence we default to different placeholders there.

    samples = reader.getSampleFolder()

    cutflowprinter = QFramework.TQCutflowPrinter(reader)
    cutflowprinter.importTagsWithoutPrefix(config,"cutflowprinter.")
    cutflowformats = config.getTagVString("cutflowFormats")
    for systematicsBand in config.getTagVString("systematicsBands"):
        systematicsBand_abs = common.findConfigPath(systematicsBand)
        QFramework.START("l.","loading systematics band from '{:s}' for cutflow printing".format(systematicsBand))
        if cutflowprinter.loadSystematics(systematicsBand_abs):
            QFramework.END(QFramework.TQMessageStream.OK)
        else:
            QFramework.END(QFramework.TQMessageStream.FAIL)
    if not cutflowformats or len(cutflowformats) < 1:
        cutflowformats = ["html","csv","tex"]
    if config.getTagBoolDefault("makeCutflows",False):
        if "tex" in cutflowformats: writesec(summary,"section","Cutflows")
        processfiles = config.getTagVStandardString("cutflowProcessFiles")
        cutfiles = config.getTagVStandardString("cutflowCutFiles")
        cutflowtags_ = QFramework.TQTaggable()
        cutflowtags_.setTagBool("ensureDirectory",True)
        cutflowtags_.importTagsWithoutPrefix(config,"cutflowprinter.")
        for ch in channelsDict.keys():
            for camp in campaignsDict.keys():

                cutflowtags = QFramework.TQTaggable(cutflowtags_) #make a copy to ensure there is no cross talk between channels!
                #first set the data variant as it might be identical to the regular path elements (or rather their placeholder) in which case the main "path" column should be the one to use (making datapath the less strictly required one of these two columns)
                cutflowtags.setTagString(dataChannelPlaceHolder,channelsDict[ch]["datapath"])
                cutflowtags.setTagString(channelPlaceHolder,channelsDict[ch]["path"])
                if camp:
                  cutflowtags.setTagString(dataCampaignPlaceHolder,campaignsDict[camp]["datapath"])
                  cutflowtags.setTagString(campaignPlaceHolder,campaignsDict[camp]["path"])
                luminosity = getLuminosityString(config, samples, campaignsDict[camp]["path"] if camp else "")

                # TODO: find a better way of setting a default textbf - in readAnalysis the 'title' variable was never really set properly
                chtitle="\\ensuremath{"+channelsDict[ch]["latex"]+"}" if ("\\" in channelsDict[ch]["latex"]) else channelsDict[ch]["latex"]
                camptitle=( "\\ensuremath{"+campaignsDict[camp]["latex"]+"}" if ("\\" in campaignsDict[camp]["latex"]) else campaignsDict[camp]["latex"] ) if camp else "" #make this an empty string if there's no actual campaign defined
                cutflowtags.setTagString("style.title.latex",
                                         config.getTagStringDefault("cutflowtitle",
                    "\\ensuremath{{\\sqrt{{s}}={:s}}}, \\ensuremath{{\\mathcal{{L}}={:s}}} ".format(samples.getTagStandardStringDefault("energy","(?)") + " " + samples.getTagStandardStringDefault("energyUnit","TeV"),luminosity) +  #add campaign information (e.g. data taking years) if we run in a subcampaign enabled mode (camp is not None):
                    ( (" (\\ensuremath{{ {:s} }})" if "\\" in campaignsDict[camp]["latex"] else " ({:s})" ).format(campaignsDict[camp]["latex"]) if camp else "" )
                ))
                cutflowtags.setTagString("style.title.html",
                    "&radic;<i>s</i>={:s}, <i>L</i>={:s} <b>{:s}</b>".format(samples.getTagStandardStringDefault("energy","(?)") + " " + samples.getTagStandardStringDefault("energyUnit","TeV"),QFramework.TQStringUtils.convertLaTeX2HTML(luminosity).Data(),QFramework.TQStringUtils.convertLaTeX2HTML(channelsDict[ch]["latex"]).Data()) +
                    ("({:s})".format( QFramework.TQStringUtils.convertLaTeX2HTML(campaignsDict[camp]["latex"]).Data() ) if camp else "")
                )

                for processfile in processfiles:
                    processfile_abs = common.findConfigPath(processfile)
                    processname = splitext(basename(processfile_abs))[0]
                    cutflowprinter.resetProcesses()
                    cutflowprinter.importProcessesFromFile(processfile_abs)
                    if not showData(samples, config): removeData(cutflowprinter, "cutflows")
                    tagProcessPaths(cutflowprinter,".generalize.visualize.cutflow",True)
                    for cutfile in cutfiles:
                        cutfile_abs = common.findConfigPath(cutfile)
                        cutname = splitext(basename(cutfile_abs))[0]
                        cutflowprinter.resetCuts()
                        cutflowprinter.importCutsFromFile(cutfile_abs)
                        QFramework.INFO("making cutflow '{:s}'/'{:s}' in {:s}-channel {:s}".format(processname,cutname,ch, "for campaign '"+camp+"'" if camp else ""))
                        cutflow = cutflowprinter.createTable(cutflowtags)
                        cutflow.importTags(cutflowtags)
                        filename = "{:s}/cutflows/{:s}{:s}-{:s}-{:s}" .format(outputdir,camp+"-" if camp else "",ch,processname,cutname)
                        for fmt in cutflowformats:
                            cutflowtags.setTagString("format",fmt)
                            filenamefmt = str(filename)+"."+str(fmt)
                            filepathfmt = QFramework.TQPathManager.getPathManager().getLocalPath(filenamefmt)
                            cutflow.write(filepathfmt,cutflowtags)
                        if "tex" in cutflowformats:
                            writesec(summary,"subsection","Cutflow "+processname+"/"+cutname+" for " + chtitle + " (" + camptitle + ")" )
                            #note: this output path is not an actual file, it's a path being
                            #written to the summary file hence, don't use the following line:
                            #filepath = QFramework.TQPathManager.getPathManager().getLocalPath(filename)
                            filepath=filename
                            writeinput(summary,QFramework.TQPathManager.getPathManager().getTargetPath(filepath, False))

    return

#######################################################################################################################

def printEventLists(config, reader, channelsDict, outputdir, campaignsDict={None: {}}):
    """print an event by event list of variables defined by the user"""
    from ROOT import TString

    samples = reader.getSampleFolder()
    campaignPlaceholder = config.getTagStandardStringDefault("~campaignPlaceholder","campaign")
    dataCampaignPlaceholder = config.getTagStandardStringDefault("~dataCampaignPlaceholder",campaignPlaceholder) #recommended treatment is to use the same name for data and corresponding MC campaigns, hence, typically the placeholder can be identical. For channels the subfolder might contain a suffix for systematic variations, hence we default to different placeholders there.

    eventlistprinter = QFramework.TQEventlistPrinter(reader)
    evtlists = reader.getListOfEventlistNames()
    if evtlists and hasattr(evtlists, '__iter__') and config.getTagBoolDefault("makeEventlists",True):
        processfiles = config.getTagVString("eventlistProcessFiles")
        eventlisttags_ = QFramework.TQTaggable()
        eventlisttags_.setTagBool("ensureDirectory",True)
        eventlisttags_.setTagBool("adjustColWidth",True)
        eventlisttags_.importTagsWithoutPrefix(config,"eventlistprinter.")
        for processfile in processfiles:
            processfile_abs = TString(common.findConfigPath(processfile))
            processname = TString(processfile_abs(processfile_abs.Last('/')+1,processfile_abs.Last('.')-processfile_abs.Last('/')-1))
            eventlistprinter.resetProcesses()
            eventlistprinter.importProcessesFromFile(processfile_abs)
            if not showData(samples, config): removeData(eventlistprinter, "eventlists")
        eventlistprinter.resetCuts()
        jobs = set()
        for evtlist in evtlists:
            name = TString(evtlist.GetName())
            cutname = QFramework.TQFolder.getPathHead(name)
            QFramework.INFO("setup eventlist printer for list'{:s}' and cut {:s}".format(name,cutname))
            eventlistprinter.addCut(cutname)
            jobs.add(name)
        for c in channelsDict.keys():
            for camp in campaignsDict.keys():
                eventlisttags = QFramework.TQTaggable(eventlisttags_) #make a copy to ensure there is no cross talk between channels!
                eventlisttags.setTagString("channel",channelsDict[c]["path"])
                eventlisttags.setTagString("datachannel",channelsDict[c]["datapath"])
                if camp:
                    eventlisttags.setTagString(campaignPlaceholder,campaignsDict[camp]["path"])
                    eventlisttags.setTagString(dataCampaignPlaceholder,campaignsDict[camp]["datapath"])


                for evtlist in jobs:
                    QFramework.INFO("making eventlists for '{:s}' in {:s}-channel {:s}".format(evtlist,c, ("and campaign '"+str(camp)+"'") if camp else "" ))
                    outputpath = QFramework.TQFolder.concatPaths(outputdir,"eventlists",c,camp if camp else "")
                    # Don't use TQPathManager here. It is used internally by TQEventlistPrinter.
                    eventlistprinter.writeEventlists(evtlist,outputpath,eventlisttags)
    return

#######################################################################################################################
def makePlots(config, reader, summary, channelsDict, outputdir, campaignsDict = {None: {}}, pathToCuts=defaultConfig.pathToCuts, pathToNormalization=defaultConfig.pathToNormalization):
    """make pretty versions of all the plots requested by the user by combining each of the defined processes"""
    from ROOT import TString

    samples = reader.getSampleFolder()
    cutfolder = samples.getFolder(QFramework.TQFolder.concatPaths(pathToCuts,"?").Data())
    campaignPlaceholder = config.getTagStandardStringDefault("~campaignPlaceholder","campaign")
    dataCampaignPlaceholder = config.getTagStandardStringDefault("~dataCampaignPlaceholder",campaignPlaceholder) #recommended treatment is to use the same name for data and corresponding MC campaigns, hence, typically the placeholder can be identical. For channels the subfolder might contain a suffix for systematic variations, hence we default to different placeholders there.

    histograms = getNames(reader.getListOfHistogramNames()) + [s.Data() for s in config.getTagVString("morePlots")]
    plotfilter = config.getTagVString("makePlots")
    summaryfilter = config.getTagVString("summaryCuts")
    cutname = ""
    plotcounter=0

    pathManager = QFramework.TQPathManager.getPathManager()
    if histograms and not len(plotfilter) < 1:
        QFramework.INFO("making plots")
        plotterClassName = config.getTagStandardStringDefault("plotter","TQDefaultPlotter")
        plotterClass = getattr(QFramework, plotterClassName)
        plotter = plotterClass(samples)
        plotter.setNormalizationInfo(pathToNormalization)
        for systematicsBand in config.getTagVString("systematicsBands"):
            systematicsBand_abs = common.findConfigPath(systematicsBand)
            QFramework.START("l.","loading systematics band from '{:s}' for plotting".format(systematicsBand))
            if plotter.loadSystematics(systematicsBand_abs):
                QFramework.END(QFramework.TQMessageStream.OK)
            else:
                QFramework.END(QFramework.TQMessageStream.FAIL)
        plottags = QFramework.TQTaggable()
        plottags.importTagsWithoutPrefix(config,"plotter.")
        if config.getTagBoolDefault("useEXIF",False):
            if QFramework.TQLibrary.enableEXIFsupport():
                QFramework.INFO("using EXIF")
            else:
                QFramework.WARN("unable to configure EXIF")
        if config.getTagBoolDefault("embedPDFfonts",False):
            if QFramework.TQLibrary.enablePDFfontEmbedding():
                QFramework.INFO("using embedFonts")
                plottags.setTagBool("embedfonts",True)
            else:
                QFramework.WARN("unable to configure font embedding")
        plotformats = config.getTagVStandardString("plotFormats")
        if len(plotformats) < 1:
            plotformats.push_back("pdf")
        if "pdf" in plotformats: writesec(summary,"section","Plots")
        plottags.setTagBool("ensureDirectory",True)
        histScheme = TString()
        if config.getTagString("histogramScheme",histScheme):
            if plotter.setScheme(histScheme):
                QFramework.INFO("using histogram scheme '{:s}' for plotting".format(histScheme))
            else:
                QFramework.ERROR("failed to use histogram scheme '{:s}' - unknown scheme".format(histScheme.Data()))
        histProcesses = TString()
        if config.getTagString("histogramProcesses",histProcesses):
            histProcesses_abs = TString(common.findConfigPath(histProcesses))
            plotter.clearProcesses()
            if plotter.importProcessesFromFile(histProcesses_abs):
                QFramework.INFO("reading histogram processes from  file '{:s}'".format(histProcesses.Data()))
            else:
                QFramework.ERROR("failed to read histogram processes from file '{:s}'".format(histProcesses.Data()))
        # cut/histogram-specific tags
        listOfPlotTags = []
        histPlotFile = TString()
        if config.getTagString("histogramPlots",histPlotFile):
            QFramework.INFO("reading histogram plots settings from  file '{:s}'".format(histPlotFile.Data()))
            histPlotFilePath = TString(common.findConfigPath(histPlotFile))
            listOfPlotTags = QFramework.TQUtils.getListOfTagsFromFile(histPlotFilePath)
            if not len(listOfPlotTags):
                QFramework.WARN("no histogram plots settings found in file '{:s}'".format(histPlotFile.Data()))
        if not showData(samples, config): removeData(plotter, "plots")
        tagProcessPaths(plotter,".generalize.visualize.histograms",True)
        plotchannels = []
        for c in channelsDict.keys():
            cpath = channelsDict[c]["path"]
            plottesttags = QFramework.TQTaggable()
            plottesttags.setTagString("channel",cpath)
            plottesttags.setTagString("datachannel",channelsDict[c]["datapath"])
            plottesttags.importTagsWithoutPrefix(config,"plotter.alias.")
            ok = True
            for camp in campaignsDict.keys():
                if camp:
                    plottesttags.setTagString(dataCampaignPlaceholder, campaignsDict[camp]["datapath"]) #we simply wildcard the campaign level for the testing purposes
                    plottesttags.setTagString(campaignPlaceholder, campaignsDict[camp]["path"]) #we simply wildcard the campaign level for the testing purposes
                for process in plotter.getListOfProcessNames():
                    rawpath = plotter.getProcessPath(process.GetName())
                    if rawpath.IsNull(): continue
                    path = plottesttags.replaceInText(rawpath).Data()
                    sflist = reader.getListOfSampleFolders(path)
                    if not sflist or sflist.GetEntries() < 1:
                        ok = False
                        break
                if ok: #at least one campaign needs to have the channel, i.e., if for one campaign in the loop things all contributing processes are present, enable the channel
                    break
            if ok:
                plotchannels.append(c)
            else:
                QFramework.WARN("scheduled channel '{:s}', but didn't find sample folder '{:s}' in sample folder -- skipping channel!".format(c,path))
        skipallplots = config.getTagBoolDefault("skipAllPlots",False)
        for h in sorted(histograms):
            makeplot = False
            makesummary = False
            for pf in plotfilter:
                if QFramework.TQStringUtils.matches(h,QFramework.TQStringUtils.trim(pf)):
                    makeplot = True
            if not makeplot:
                if not config.getTagBoolDefault("skipPlotsSilent",False):
                    QFramework.INFO("skipping histogram '{:s}'".format(h))
                continue
            plottagsLocal = QFramework.TQTaggable(plottags)
            thiscutname = TString("")
            distname = TString(h)
            QFramework.TQStringUtils.readUpTo(distname,thiscutname,"/","[]")
            QFramework.TQStringUtils.removeLeading(distname,"/")
            if not len(summaryfilter) < 1:
                for cn in summaryfilter:
                    if cn == thiscutname:
                        makesummary = True
            else:
                makesummary = True
            if thiscutname != cutname:
                cutname = thiscutname
                if makesummary == True:
                    thiscut = cutfolder.getFolder("*/"+thiscutname)
                    writesec(summary,"subsection",thiscut.getTagStringDefault(".title",cutname) if thiscut else cutname)

            contributions = reader.getParsedPathStrings(distname)
            for altPathStrings in contributions:
                for pathTermString in altPathStrings:
                    con = pathTermString.Data()
                    prefix = TString("plotter.{:s}.".format(con))
                    plottagsLocal.importTagsWithoutPrefix(config,prefix)

            # apply the cut/hist-specific tags
            applyPlotTags(h,listOfPlotTags,plottagsLocal)

            sanename = QFramework.TQFolder.makeValidIdentifier(h,"-").Data()
            for camp in campaignsDict.keys():
                luminosity = getLuminosityString(config, samples, campaignsDict[camp]["path"] if camp else "")
                for c in plotchannels:
                    if plotter.getSystematics(c):
                        plottagsLocal.setTagString("errors.showSys",c)
                    plottagsLocal.setTagString("input.channel",channelsDict[c]["path"])
                    plottagsLocal.setTagString("alias.datachannel",channelsDict[c]["datapath"]) #TODO: change to "input.datachannel" for consistency sake once a CAFCore version is established which handles this a bit smarter...
                    if camp:
                        plottagsLocal.setTagString("input."+dataCampaignPlaceholder, campaignsDict[camp]["datapath"])
                        plottagsLocal.setTagString("input."+campaignPlaceholder, campaignsDict[camp]["path"])

                    for plotformat in plotformats:
                        if plotformat == "tex":
                            if config.getTagBoolDefault("showPlotLabels",True):
                                plottagsLocal.setTagString("labels.0", TString("\\sqrt{s} = ")+samples.getTagStringDefault("energy","(?)") + TString("$ ") + samples.getTagStringDefault("energyUnit","TeV")+TString(", $\\int Ldt = ")+luminosity)
                                label = plottagsLocal.getTagStringDefault(".title",config.getTagStringDefault("plotlabel","$(channel) channel"))
                                label.ReplaceAll("$(channel)",channelsDict[c]["latex"])
                                if camp: label.ReplaceAll("$("+campaignPlaceholder+")", campaignsDict[camp]["latex"])
                                plottagsLocal.setTagString("labels.1", label)
                        else:
                            if config.getTagBoolDefault("showPlotLabels",True):
                                plottagsLocal.setTagString("labels.0", TString("#sqrt{s} = ")+samples.getTagStringDefault("energy","(?)") + TString(" ") + samples.getTagStringDefault("energyUnit","TeV")+TString(", #lower[-0.2]{#scale[0.6]{#int}} Ldt = ")+luminosity)
                                label = plottagsLocal.getTagStringDefault(".title",config.getTagStringDefault("plotlabel","$(channel) channel"))
                                label.ReplaceAll("$(channel)",channelsDict[c]["roottex"])
                                if camp: label.ReplaceAll("$("+campaignPlaceholder+")", campaignsDict[camp]["roottex"])
                                plottagsLocal.setTagString("labels.1", label)

                        # make plot-lin/log.pdf with global flags
                        if config.getTagBoolDefault("makeLinPlots", True):
                            plottagsLin = QFramework.TQTaggable(plottagsLocal)
                            plottagsLin.setTagBool("style.logScale",False)
                            histname = "{:s}/plots/{:s}{:s}-{:s}-lin.{:s}".format(outputdir,camp+"-" if camp else "",c,sanename,plotformat)
                            QFramework.INFO("Plotting histogram '{:s}'".format(histname));
                            if not skipallplots and not plotter.plotAndSaveAs(h, pathManager.getLocalPath(histname),plottagsLin):
                                QFramework.WARN("error plotting '{:s}'".format(histname))
                                pathManager.deleteLocalFile(histname)
                            elif plotformat == "pdf" and makesummary==True:
                                if not config.getTagBoolDefault("summaryLinearOnly",False):
                                    writegraphics(summary, pathManager.getTargetPath(histname, False),.49,False)
                                else:
                                    if ((plotcounter+1)%2):
                                        writegraphics(summary, pathManager.getTargetPath(histname, False),.49,False)
                                    else:
                                        writegraphics(summary, pathManager.getTargetPath(histname, False),.49,True)
                                    plotcounter += 1
                        if config.getTagBoolDefault("makeLogPlots", False):
                            plottagsLog = QFramework.TQTaggable(plottagsLocal)
                            plottagsLog.setTagBool("style.logScale",True)
                            histname = "{:s}/plots/{:s}{:s}-{:s}-log.{:s}".format(outputdir,camp+"-" if camp else "",c,sanename,plotformat)
                            QFramework.INFO("Plotting histogram '{:s}'".format(histname));
                            if not skipallplots and not plotter.plotAndSaveAs(h, pathManager.getLocalPath(histname),plottagsLog):
                                QFramework.WARN("error plotting '{:s}'".format(histname))
                                pathManager.deleteLocalFile(histname)
                            elif plotformat == "pdf" and makesummary==True:
                                if not config.getTagBoolDefault("summaryLinearOnly",False):
                                    writegraphics(summary, pathManager.getTargetPath(histname, False),.49,False)
                                else:
                                    if ((plotcounter+1)%2):
                                        writegraphics(summary,pathManager.getTargetPath(histname, False),.49,False)
                                    else:
                                        writegraphics(summary,pathManager.getTargetPath(histname, False),.49,True)

                        # make plot.pdf with histogram-specific flags
                        if not histPlotFile.IsNull():
                            histname = "{:s}/plots/{:s}{:s}-{:s}.{:s}".format(outputdir,camp+"-" if camp else "",c,sanename,plotformat)
                            QFramework.INFO("Plotting histogram '{:s}'".format(histname));
                            if not skipallplots and not plotter.plotAndSaveAs(h, pathManager.getLocalPath(histname),plottagsLocal):
                                QFramework.WARN("error plotting '{:s}'".format(histname))
                                pathManager.deleteLocalFile(histname)
                            elif plotformat == "pdf" and makesummary==True:
                                if not config.getTagBoolDefault("summaryLinearOnly",False):
                                    writegraphics(summary,pathManager.getTargetPath(histname, False),.49,False)
                                else:
                                    if ((plotcounter+1)%2):
                                        writegraphics(summary,pathManager.getTargetPath(histname, False),.49,False)
                                    else:
                                        writegraphics(summary,pathManager.getTargetPath(histname, False),.49,True)
                                    plotcounter += 1


    return

#######################################################################################################################

def makeComparisonPlots(config, reader, outputdir, pathToNormalization=defaultConfig.pathToNormalization):
    """make a selection of advanced plots to more easily compare specific processes defined by the user"""

    if config.hasTag("makeComparePlots"):
        plotformats = config.getTagVString("plotFormats")
        if len(plotformats) < 1:
            plotformats.push_back("pdf")
        comparePlotter = QFramework.TQCompPlotter(reader)
        comparePlotter.setNormalizationInfo(pathToNormalization)
        compFolders = QFramework.TQFolder()
        for fileName in config.getTagVString("makeComparePlots"):
            fileName_abs = common.findConfigPath(fileName)
            compFolders.importFromTextFile(fileName_abs)
        plottags = QFramework.TQTaggable()
        plottags.importTagsWithoutPrefix(config,"compPlotter.")
        plottags.importTagsWithoutPrefix(config,"plotter.")
        for folder in compFolders.getListOfFolders():
            folder.importTags(plottags,False)
            name = folder.getTagStringDefault("name",folder.GetName())
            sanename = QFramework.TQFolder.makeValidIdentifier(name,"-")
            for plotformat in plotformats:
                folder.setTagBool("style.logScale",False)
                histname = "{:s}/plots/{:s}-lin.{:s}".format(outputdir,sanename,plotformat)
                QFramework.INFO("Plotting histogram '{:s}'".format(histname));
                if not comparePlotter.plotAndSaveAs("irrelevant", pathManager.getLocalPath(histname),folder):
                    QFramework.WARN("error plotting '{:s}'".format(histname))
                    pathManager.deleteLocalFile(histname)
                folder.setTagBool("style.logScale",True)
                histname = "{:s}/plots/{:s}-log.{:s}".format(outputdir,sanename,plotformat)
                QFramework.INFO("Plotting histogram '{:s}'".format(histname));
                if not comparePlotter.plotAndSaveAs("irrelevant", pathManager.getLocalPath(histname),folder):
                    QFramework.WARN("error plotting '{:s}'".format(histname))
                    pathManager.deleteLocalFile(histname)

    return

#######################################################################################################################

def finalizeSummaryDocument(summary, config=None):
    """finalize a document which summarizes all of the results"""
    summary.write("\\end{document}")
    summary.close()
    if config:
        if config.hasTag("summaryCuts.0") or config.hasTag("summaryCuts"): #don't bother with printout if no summary is configured
            outputdir=config.getTagStringDefault("outputDir", "results")
            outputpath = QFramework.TQFolder.concatPaths(outputdir,"summary.tex").Data()
            outputpath = QFramework.TQPathManager.getPathManager().getTargetPath(outputpath, False)
            QFramework.INFO("For compiling the summary you can use the following command (run twice to include a table of contents):")
            QFramework.INFO("pdflatex -output-directory {outputdir} " \
                            "{outputpath}".format(outputdir="/".join(str(outputpath).split("/")[:-1]), outputpath=outputpath))
            QFramework.INFO("Note, to use the pdflatex command you might want to export your " \
                            "path environment variable to: 'export PATH=/afs/cern.ch/sw/XML/TL2016/bin/x86_64-linux:$PATH'")
    return

#######################################################################################################################

def getLuminosityString(config, samples, campaignPathElement = ""):
    """get the luminosity value as a well formatted string"""
    from ROOT import TString

    lumiValue = samples.getTagDoubleDefault("luminosity",-1.)
    if len(campaignPathElement)>0: #we have a setup with campaigns, this requires some magic to calculate the correct combined lumi
      lumiValue = 0.
      lumiTags = QFramework.TQTaggable()
      lumiTags.importTagsWithoutPrefix(samples,"luminosity.")
      keys = lumiTags.getListOfKeys()
      validChars = QFramework.TQFolder.getValidNameCharacters().Data() #all valid characters for tqfolder names
      if keys:
        for k in keys:
          k = k.GetName() # turn TObjectString into a regular string to work with
          keyPos = 0
          while keyPos >= 0:
            keyPos = campaignPathElement.find(k,keyPos)
            if keyPos<0: continue #key is not in the campaignPathElement
            if (keyPos==0) or not (campaignPathElement[keyPos-1] in validChars): #either at the very beginning of the string (e.g. single campaign) or an *invalid* character before (i.e. this is not a part of a longer folder name)
              if keyPos+len(k) >= len(campaignPathElement) or not (campaignPathElement[keyPos+len(k)] in validChars): #either at the very end of the campPathElement or the following character is not a valid TQFolder name character (e.g. a '+' or ']' )
                #-> we got a full match, add the lumi of this tag
                lumiValue += lumiTags.getTagDoubleDefault(k,0.)
                break #exit the while loop
      else:
        QFramework.BREAK("Failed to retrieve per-campaign luminosity information. Please ensure that on your base SampleFolder tags of the format 'luminosity.nameOfCorrespondingCampaign' are set with values corresponding to the luminosity of the campaign in units of pb (unless the unit is changed via the luminosityUnit option).")

    luminosity = TString("")
    lumiUnit = samples.getTagStringDefault("luminosityUnit","pb")
    displayLumiUnit = config.getTagStringDefault("displayLuminosityUnit","fb")
    if lumiValue < 0:
        luminosity = "(?)"
    else:
        luminosity = str(QFramework.TQStringUtils.formatSignificantDigits(lumiValue / QFramework.TQXSecParser.unit(lumiUnit) * QFramework.TQXSecParser.unit(displayLumiUnit),config.getTagIntegerDefault("lumiDigit",2))) + " "+str(displayLumiUnit)+"^{-1}"

    return luminosity


#######################################################################################################################

def tagProcessPaths(presenter,tag,value):
    """tag all processes explicitly used by some presenter with a certain tag"""
    import QFramework
    results = QFramework.TQTaggable("channel='?'")
    for process in presenter.getListOfProcessNames():
        path = results.replaceInText(presenter.getProcessPath(process.GetName()))
        folders = presenter.getReader().getListOfSampleFolders(path)
        if folders:
            for folder in folders:
                folder.setTag(tag,value)
                

#######################################################################################################################

def applyPlotTags(plotName, listOfPlotTags, localPlotTags):
    """apply a pre-loaded list of histogram-specific plotting tags (append new/overwrite existing)"""
    import QFramework
    for listOfPlotTags in listOfPlotTags:
        if QFramework.TQStringUtils.matches(plotName,listOfPlotTags.getTagStringDefault(".name","")): 
            localPlotTags.importTags(listOfPlotTags)

#######################################################################################################################

def getNames(l):
    """get a list of the names of the elements of a list"""
    retval = []
    if l:
        for e in l: retval.append(e.GetName())
    return retval

#######################################################################################################################

def writesec(f,h,t):
    """write a new section to a given summary file"""
    pretty = str(t).replace("_","\\_").replace("\\_{","_{")
    safe = pretty # good enough for now
    f.write("\\"+h+"["+safe+"]{"+pretty+"}\n\n\\centering\n\n")

#######################################################################################################################

def writeinput(f,x):
    """write input to a given summary file"""
    f.write("\\adjustbox{width=\\textwidth,height=\\textheight,keepaspectratio}{\\input{"+str(x)+"}}\n\n")

#######################################################################################################################

def writegraphics(f,x,w,nl):
    """write a graphic to a given summary file"""
    f.write("\\includegraphics[width={:f}\\textwidth]".format(w))
    f.write("{"+x+"}")
    if nl: f.write("\n\n")

#######################################################################################################################

def showData(samples, config):
    """return false only if showData config set to false or data folder can't be found"""
    return config.getTagBoolDefault("showData",True)

#######################################################################################################################

def removeData(presenter, presenterName):
    """remove the data from being presented"""
    QFramework.INFO("removing data from {:s}".format(presenterName))
    presenter.removeProcessesByName("*data*")
    presenter.removeProcessesByName("*Data*")
