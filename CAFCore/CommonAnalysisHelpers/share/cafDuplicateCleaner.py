#!/usr/bin/env python2
from CommonAnalysisHelpers.analyze import PyCut
from CommonAnalysisHelpers import common,prepare,initialize,analyze
import os

def main(config):
    #create a minimal SampleFolder
    samples = QFramework.TQSampleFolder("samples")
    samples.setTagString(".mcasv.channel","all")
    dataSF = samples.getSampleFolder("data+")
    for inp in config.getTagVStandardString("inputs"):
      fileName,fileExtension = os.path.splitext(os.path.basename(inp))
      sampleName = QFramework.TQFolder.makeValidIdentifier(fileName)+"+"
      print("Creating sample named {:s}".format(sampleName.Data()))
      thisSample = dataSF.getSample(sampleName)
      thisSample.setTagString("dsid","data")
      thisSample.setTagString(".xsp.filepath",inp)
      thisSample.setTagString(".xsp.treename","CollectionTree")
    
    #create a single cut to apply the veto
    baseCutFolder = PyCut("allEvents",cut="1.")
    baseCutFolder + PyCut("duplicateVeto",cut="#UNIQUE(EventInfo.runNumber(),EventInfo.eventNumber())")
    #convert to actual TQCuts
    cuts = QFramework.TQCut.importFromFolder(baseCutFolder.folder)
    
    #add (hard coded) config options to trigger setup of xAOD skimming
    config.setTagString("xAODdumping.flagName","passDuplicateRemoval")
    config.setTagString("xAODdumping.cuts","duplicateVeto")
    config.setTagString("channels.0","all")
    config.setTagBool("useMultiChannelVisitor",True)
    #fake some CLI/runtime arguments
    config.importFromText("+CLI{<jobID=dupeClean>}")
    config.importFromText("+runtime{<mcasvchannels.0=all>}")
    
    #book AnalysisJobs (eventFlagging for xAOD skimming and by default also cutflow AnalyisJobs)
    analyze.bookAnalysisJobs(config,cuts)
    #create the sample visitor
    visitor = analyze.createAnalysisSampleVisitor(config,cuts)
    #book algorithms (xAOD skimming)
    if not args.checkOnly: analyze.bookAlgorithms(config,visitor)
    #reduce number of events for testing if requested
    if args.debug: visitor.setMaxEvents(10)
    #execute analysis
    print("Starting processing of samples...")
    analyze.executeAnalysis(config, samples, visitor)
    print("Processing done.")
    #Print some summary about how many events were vetoed
    allEvents    = samples.getCounter("data","allEvents").getCounter()
    uniqueEvents = samples.getCounter("data","duplicateVeto").getCounter()
    print("Duplicate removal summary:")
    print("--------------------------")
    print("events processed: {:d}".format(int(allEvents)))
    print("unique events   : {:d}".format(int(uniqueEvents)))
    print("unique fraction : {:.3f}".format(uniqueEvents/max(allEvents,1.)))
    print("removed fraction: {:.3f}".format(1.-uniqueEvents/max(allEvents,1.)))
    
    #safe mini-sampleFolder for the sake of having the cutflow outputs at hand:
    samples.writeToFile("duplicateRemovalSampleFolder.root",1,-1,True)
    
    #cleanup and stats printing
    ROOT.xAOD.ClearTransientTrees()
    common.printExecutionStatistics(config)
    
if __name__ == "__main__":

    # create a pre-configured argument parser
    parser = common.createEmptyArgumentParser("CAF xAOD Duplicate Event Cleaner")

    import QFramework
    import ROOT

    QFramework.TQUniqueCut.printOnVeto = True
    
    # ignore command line arguments since ROOT is very greedy here (and tends to choke from it!)
    ROOT.PyConfig.IgnoreCommandLineOptions = True
    
    
    parser.add_argument("input",type=str,nargs="+",default=[],metavar="INPUT",help="xAODs to be processed")
    parser.add_argument("--output", type=str, metavar="OUTPUT", help="directory to write outputs to")
    parser.add_argument("--checkOnly", action='store_true',default=False, help="do not perform the actual cleaning but only check for duplicated events")
    parser.add_argument("--debug", action='store_true',default=False, help="only run over a few test events per file for debugging purposes")
    args = parser.parse_args()
    config = QFramework.TQFolder()
    for index,val in enumerate(args.input):
      config.setTagString("inputs.{:d}".format(index),val)
    config.setTagString("xAODdumping.outputDir",args.output)
    # call the main function
    main(config)

