import optparse
parser = optparse.OptionParser()
parser.add_option('-i', '--inFileName',           dest="infileName",         default=None, help="Run in loop mode")
parser.add_option('-l', '--local',                dest="runLocal",          action="store_true",default=False, help="Run in loop mode")
parser.add_option('-o', '--outFileName',          dest="outfileName",        default=None, help="Run in loop mode")
parser.add_option('-n', '--nevents',              dest="nevents",           default=None, help="Run in loop mode")
o, a = parser.parse_args()


import ROOT

if not o.runLocal:
    import os
    if int(os.environ.get('ROOTCORE_RELEASE_SERIES', 0)) < 25:
        print "loading packages"
        ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")
    
    sh_all = ROOT.SH.SampleHandler()
    if False:
        inFile = ROOT.TFile( o.infileName, "READ" )
    else:
        print "\t\tAdding samples using scanRucio"
        #sh_all.setMetaString( "nc_tree", "XhhMiniNtuple")
        ROOT.SH.scanRucio(sh_all, o.infileName)
    
    print "\t%d different dataset(s) found", len(sh_all)
    sh_all.printContent()
    
    sh_all.setMetaString( "nc_tree", "XhhMiniNtuple")
    fileList = sh_all.at(0).makeFileList()
else:
    fileList=[]
    fileList.append(o.infileName)



hnTags   = ROOT.TH1F("nTags", "nTags;nTags;Entries", 10, -0.5 ,9.5)
hnTags.Sumw2()


hpt = []
for i in range(4):
    hpt.append(ROOT.TH1F("pt_b"+str(i), "pt_b"+str(i)+";pt_b"+str(i)+";Entries", 100, 0 ,250))



iEvent = 0


for i in fileList:
    print i
    inFile = ROOT.TFile.Open( i)
    hhTree = inFile.Get("XhhMiniNtuple")

    nEvents = hhTree.GetEntries()
    print nEvents

    for event in hhTree:
        # print event.weight_mhh
        iEvent +=1 
        if iEvent %10000 == 0:
            print "Processed .... ",iEvent,"Events"
        if o.nevents and (iEvent > int(o.nevents)):
            break
    

        nResolved =  len(event.resolvedJets_pt)
        nTags = 0
        b_pts = []
        for iJet in range(nResolved):
            #if event.resolvedJets_pt[iJet] < 40: continue
            if event.resolvedJets_MV2c10[iJet] < 0.8244: continue
            b_pts.append(event.resolvedJets_pt[iJet])
            nTags += 1

        hnTags.Fill(nTags,event.weight)
        

        if nTags < 4: continue
        
        weight = event.weight
        b_pts.sort(reverse=True)

        for bptItr in range(4):
            hpt[bptItr].Fill(b_pts[bptItr],weight)



outFile = ROOT.TFile(o.outfileName,"RECREATE")
for histPt in  hpt:
    histPt.Write()
    

hnTags.Write()
