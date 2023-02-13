import optparse
parser = optparse.OptionParser()
parser.add_option('-i', '--inFileName',           dest="infileName",         default=None, help="Run in loop mode")
parser.add_option('-o', '--outFileName',          dest="outfileName",        default=None, help="Run in loop mode")
parser.add_option('-n', '--nevents',              dest="nevents",           default=None, help="Run in loop mode")
parser.add_option('--massMin',                    dest="massMin",           default=None, help="Run in loop mode")
parser.add_option('--massMax',                    dest="massMax",           default=None, help="Run in loop mode")
o, a = parser.parse_args()


import ROOT


import os
if int(os.environ.get('ROOTCORE_RELEASE_SERIES', 0)) < 25:
    print "loading packages"
    ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")

sh_all = ROOT.SH.SampleHandler()
if False:
    inFile = ROOT.TFile( o.infileName, "READ" )
elif False:
    print "\t\tAdding samples using scanRucio"
    #sh_all.setMetaString( "nc_tree", "XhhMiniNtuple")
    ROOT.SH.scanRucio(sh_all, o.infileName)
else:
    from rucioMakeList import rucioMakeList
    import random
    tempfname = o.infileName+str(random.random())+"temp.txt"
    rucioMakeList(o.infileName,tempfname)
    ROOT.SH.readFileList(sh_all,o.infileName,tempfname)
    os.system("rm "+tempfname)

print "\t%d different dataset(s) found", len(sh_all)
sh_all.printContent()

sh_all.setMetaString( "nc_tree", "XhhMiniNtuple")
fileList = sh_all.at(0).makeFileList()

nBins = 1000
xMin = float(o.massMin)
xMax = float(o.massMax)

hmhh   = ROOT.TH1F("mhh", "mhh;m_{hh};Entries", nBins,   xMin ,xMax)
hmhh.Sumw2()
hmhh_w = ROOT.TH1F("mhh_w", "mhh;m_{hh};Entries", nBins, xMin ,xMax)
hmhh_w.Sumw2()

hmhh_sel   = ROOT.TH1F("mhh_sel", "mhh;m_{hh};Entries", nBins, xMin ,xMax)
hmhh_sel.Sumw2()
hmhh_sel_w = ROOT.TH1F("mhh_w_sel", "mhh;m_{hh};Entries", nBins, xMin ,xMax)
hmhh_sel_w.Sumw2()


hnTags   = ROOT.TH1F("nTags", "nTags;nTags;Entries", 10, -0.5 ,9.5)
hnTags.Sumw2()


iEvent = 0


for i in fileList:
    print i
    inFile = ROOT.TFile.Open( i)
    inFile.ls()
    
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

        doPrintXSectionOnly = True
        if doPrintXSectionOnly:
            weight = event.weight
            print "XSection",o.infileName,event.weight," ",event.weight_xs
            break

    
        nTruth =  len(event.truth_pt)

        h1 = None
        h2 = None


        nResolved =  len(event.resolvedJets_pt)
        nTags = 0
        for iJet in range(nResolved):
            if event.resolvedJets_pt[iJet] < 40: continue
            if event.resolvedJets_MV2c10[iJet] < 0.8244: continue
            nTags += 1

        hnTags.Fill(nTags,event.weight)

        for i in range(nTruth):
            if not event.truth_pdgId[i] == 25:
                continue

            if not h1:
                h1 = ROOT.TLorentzVector()
                h1.SetPtEtaPhiE(event.truth_pt[i],
                                event.truth_eta[i],
                                event.truth_phi[i],
                                event.truth_E[i])
            else:
                h2 = ROOT.TLorentzVector()
                h2.SetPtEtaPhiE(event.truth_pt[i],
                                event.truth_eta[i],
                                event.truth_phi[i],
                                event.truth_E[i])
                
        if not h1 or not h2:
            print "ERROR no h1 and h2"
            import sys
            sys.exit(-1)

        mhh = (h1+h2).M()

        hmhh  .Fill(mhh, weight) 
        hmhh_w.Fill(mhh, weight*event.weight_mhh) 
    
        if nTags > 3:
            hmhh_sel  .Fill(mhh, weight) 
            hmhh_sel_w.Fill(mhh, weight*event.weight_mhh) 
    



outFile = ROOT.TFile(o.outfileName,"RECREATE")
hmhh.Write()
hmhh_w.Write()
hmhh_sel.Write()
hmhh_sel_w.Write()
hnTags.Write()
