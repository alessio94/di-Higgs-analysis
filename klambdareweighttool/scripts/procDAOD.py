import optparse
parser = optparse.OptionParser()
parser.add_option('-i', '--inFileName',           dest="infileName",         default=None, help="Run in loop mode")
parser.add_option('-o', '--outFileName',          dest="outfileName",        default=None, help="Run in loop mode")
parser.add_option('-n', '--nevents',              dest="nevents",           default=None, help="Run in loop mode")
parser.add_option('-l', '--runLocal',              dest="runLocal",         action="store_true", default=False, help="Run in loop mode")
parser.add_option('--massMin',                    dest="massMin",           default=None, help="Run in loop mode")
parser.add_option('--massMax',                    dest="massMax",           default=None, help="Run in loop mode")
o, a = parser.parse_args()


import ROOT
from array import array


import os
if int(os.environ.get('ROOTCORE_RELEASE_SERIES', 0)) < 25:
    print "loading packages"
    ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")

sh_all = ROOT.SH.SampleHandler()
if o.runLocal:
    fileList=[]
    fileList.append(o.infileName)
elif False:
    print "\t\tAdding samples using scanRucio"
    #sh_all.setMetaString( "nc_tree", "XhhMiniNtuple")
    ROOT.SH.scanRucio(sh_all, o.infileName)
    print "\t%d different dataset(s) found", len(sh_all)
    sh_all.printContent()

    fileList = sh_all.at(0).makeFileList()

else:
    from rucioMakeList import rucioMakeList
    import random
    tempfname = o.infileName+str(random.random())+"temp.txt"
    rucioMakeList(o.infileName,tempfname,"DAOD")
    ROOT.SH.readFileList(sh_all,o.infileName,tempfname)
    os.system("rm "+tempfname)

    print "\t%d different dataset(s) found", len(sh_all)
    sh_all.printContent()

    fileList = sh_all.at(0).makeFileList()
    print fileList

nBins = 1000
xMin = float(o.massMin)
xMax = float(o.massMax)

hmhh   = ROOT.TH1F("mhh", "mhh;m_{hh};Entries", nBins,   xMin ,xMax)
hmhh.Sumw2()
hmhh_w = ROOT.TH1F("mhh_w", "mhh;m_{hh};Entries", nBins, xMin ,xMax)
hmhh_w.Sumw2()

hmhh_20GeV_w = ROOT.TH1F("mhh_20GeV_w", "mhh_20GeV_w;m_{hh};Entries", 34, 250 ,930)
hmhh_20GeV_w.Sumw2()

#
# Make output ntuple
# 
outFile = ROOT.TFile(o.outfileName,"RECREATE")
outTree = ROOT.TTree("hhTree","hhTree")


#RunNumber = array( 'i', [ 0 ] )

mcEventWeight = array('f', [0.0] )
hpt       = ROOT.std.vector('float')()
heta      = ROOT.std.vector('float')()
hphi      = ROOT.std.vector('float')()
he        = ROOT.std.vector('float')()



outTree.Branch( 'mcEventWeight', mcEventWeight, 'mcEventWeight/F' )
outTree.Branch( 'hpt',       hpt  )
outTree.Branch( 'heta',      heta  )
outTree.Branch( 'hphi',      hphi  )
outTree.Branch( 'he',        he  )



iEvent = 0
print len(fileList)

for i in fileList:
    if o.nevents and (iEvent > int(o.nevents)):
        break

    print i
    inFile = ROOT.TFile.Open( i)
    #inFile.ls()
    
    t = ROOT.xAOD.MakeTransientTree( inFile )

    # Print some information:
    print( "Number of input events: %s" % t.GetEntries() )
    nEventThisFile = t.GetEntries()

    for entry in xrange( 0,nEventThisFile): # let's only run over the first 100 events for this example
        t.GetEntry( entry )    

        iEvent +=1             

        if iEvent %10000 == 0:
            print "Processed .... ",iEvent,"Events"
        if o.nevents and (iEvent > int(o.nevents)):
            break

        #print( "Processing run #%i, event #%i" % ( t.EventInfo.runNumber(), t.EventInfo.eventNumber() ) )
        #print( "Number of truth: %i" % len( t.TruthParticles ) )
        # loop over electron collection
        nHiggs  = 0
        h1 = None
        h2 = None

        hpt  .clear()
        heta .clear()
        hphi .clear()
        he   .clear()

        for i in xrange( t.TruthParticles.size()):
            trp = t.TruthParticles.at(i)
            if not trp.pdgId()  == 25: continue

            #if not trp.status() == 22: continue
            if not trp.nChildren() == 2: continue

            hpt  .push_back(trp.pt()*0.001)
            heta .push_back(trp.eta()     ) 
            hphi .push_back(trp.phi()     ) 
            he   .push_back(trp.e()*0.001  ) 

            if not h1:
                h1 = ROOT.TLorentzVector()
                h1.SetPtEtaPhiE(trp.pt()*0.001,
                                trp.eta(),
                                trp.phi(),
                                trp.e()*0.001)



            else:
                h2 = ROOT.TLorentzVector()
                h2.SetPtEtaPhiE(trp.pt()*0.001,
                                trp.eta(),    
                                trp.phi(),    
                                trp.e()*0.001)




            nHiggs += 1

        if not nHiggs == 2: print "ERROR nHiggs",nHiggs

                
        if not h1 or not h2:
            print "ERROR no h1 and h2"
            import sys
            sys.exit(-1)


        mhh = (h1+h2).M()

        weight = t.EventInfo.mcEventWeight()

        mcEventWeight[0] = weight

        #if t.EventInfo.mcEventWeights().size():
        #    print "\t",t.EventInfo.mcEventWeights().at(0)

        hmhh  .Fill(mhh, 1.0) 
        hmhh_w.Fill(mhh, weight) 
        hmhh_20GeV_w.Fill(mhh, weight) 

        outTree.Fill()


    # clear transient trees to avoid crash at end of job
    ROOT.xAOD.ClearTransientTrees()
    inFile.Close()


outFile.cd()
hmhh.Write()
hmhh_w.Write()
hmhh_20GeV_w.Write()
outTree.Write()

