import optparse
parser = optparse.OptionParser()
parser.add_option('-i', '--inFileName',           dest="infileName",         default=None, help="Run in loop mode")
parser.add_option('-l', '--local',                dest="runLocal",          action="store_true",default=False, help="Run in loop mode")
parser.add_option('-o', '--outFileName',          dest="outfileName",        default=None, help="Run in loop mode")
parser.add_option('-n', '--nevents',              dest="nevents",           default=None, help="Run in loop mode")
o, a = parser.parse_args()


import ROOT
ROOT.gROOT.Macro( '$ROOTCOREDIR/scripts/load_packages.C' )

treeName = "CollectionTree" # default when making transient tree anyway

# Initialize the xAOD infrastructure: 
if(not ROOT.xAOD.Init().isSuccess()): print "Failed xAOD.Init()"


if not o.runLocal:
    import os
    #if int(os.environ.get('ROOTCORE_RELEASE_SERIES', 0)) < 25:
    # print "loading packages"
    # ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")
    
    sh_all = ROOT.SH.SampleHandler()
    if False:
        inFile = ROOT.TFile( o.infileName, "READ" )
    else:
        print "\t\tAdding samples using scanRucio"
        #sh_all.setMetaString( "nc_tree", "XhhMiniNtuple")
        ROOT.SH.scanRucio(sh_all, o.infileName)
    
    print "\t%d different dataset(s) found", len(sh_all)
    sh_all.printContent()
    
    #sh_all.setMetaString( "nc_tree", "XhhMiniNtuple")
    fileList = sh_all.at(0).makeFileList()
else:
    fileList=[]
    fileList.append(o.infileName)



hHiggsPt   = ROOT.TH1F("hPt", "hPt;Higgs p_T;Entries", 100, 0 ,500)
hHiggsPt.Sumw2()

hHiggsPt_l   = ROOT.TH1F("hPt_l", "hPt_l;Higgs p_T;Entries", 100, 0 ,1000)
hHiggsPt_l.Sumw2()


#hpt = []
#for i in range(4):
#    hpt.append(ROOT.TH1F("pt_b"+str(i), "pt_b"+str(i)+";pt_b"+str(i)+";Entries", 100, 0 ,250))
#


iEvent = 0


for i in fileList:
    print i
    inFile = ROOT.TFile.Open( i)

    #inFile.ls()
    #hhTree = inFile.Get("CollectionTree")
    #t = ROOT.xAOD.MakeTransientTree( inFile, "CollectionTree")


    t = ROOT.xAOD.MakeTransientTree( inFile )

    # Print some information:
    print( "Number of input events: %s" % t.GetEntries() )
    nEventThisFile = t.GetEntries()

    for entry in xrange( 0,nEventThisFile): # let's only run over the first 100 events for this example
        t.GetEntry( entry )

        #print t.EventInfo.mcEventWeight()

        #if t.EventInfo.mcEventWeights().size():
        #    print "\t",t.EventInfo.mcEventWeights().at(0)

        iEvent +=1             

        if iEvent %10000 == 0:
            print "Processed .... ",iEvent,"Events"
        if o.nevents and (iEvent > int(o.nevents)):
            break

        #print( "Processing run #%i, event #%i" % ( t.EventInfo.runNumber(), t.EventInfo.eventNumber() ) )
        #print( "Number of truth: %i" % len( t.TruthParticles ) )
        # loop over electron collection
        nHiggs  = 0
        for i in xrange( t.TruthParticles.size()):
            trp = t.TruthParticles.at(i)
            if not trp.pdgId()  == 25: continue
            if not trp.status() == 22: continue
            higgsPt = trp.pt()*0.001
            hHiggsPt.Fill(higgsPt)
            hHiggsPt_l.Fill(higgsPt)

            nHiggs += 1
        if not nHiggs == 1: print "ERROR nHiggs",nHiggs

    if o.nevents and (iEvent > int(o.nevents)):
        break

    # clear transient trees to avoid crash at end of job
    ROOT.xAOD.ClearTransientTrees()
    inFile.Close()

outFile = ROOT.TFile(o.outfileName,"RECREATE")
hHiggsPt.Write()
hHiggsPt_l.Write()
    

