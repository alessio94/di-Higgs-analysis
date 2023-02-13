import optparse
parser = optparse.OptionParser()
parser.add_option('-i', '--inFileName',           dest="infileName",         default=None, help="Run in loop mode")
parser.add_option('-o', '--outFileName',          dest="outfileName",        default=None, help="Run in loop mode")
parser.add_option('-n', '--nevents',              dest="nevents",           default=None, help="Run in loop mode")
o, a = parser.parse_args()


import ROOT
from array import array


nBins = 1000
xMin = float(250)
xMax = float(1500)

hmhh = ROOT.TH1F("mhh", "mhh;m_{hh};Entries", nBins, xMin ,xMax)
hmhh.Sumw2()

hmhh_20GeV = ROOT.TH1F("mhh_20GeV", "mhh_20GeV;m_{hh};Entries", 34, 250 ,930)
hmhh_20GeV.Sumw2()

hpth_20GeV = ROOT.TH1F("pth_20GeV", "mhh_20GeV;p_{T}^{h};Entries", 25, 0 ,500)
hpth_20GeV.Sumw2()


hmhh_20GeV_reWeight = ROOT.TH1F("mhh_20GeV_reWeight", "mhh_20GeV;m_{hh};Entries", 34, 250 ,930)
hmhh_20GeV_reWeight.Sumw2()

hpth_20GeV_reWeight = ROOT.TH1F("pth_20GeV_reWeight", "mhh_20GeV;p_{T}^{h};Entries", 25, 0 ,500)
hpth_20GeV_reWeight.Sumw2()


hmhh_20GeV_newWeight = ROOT.TH1F("mhh_20GeV_newWeight", "mhh_20GeV;m_{hh};Entries", 34, 250 ,930)
hmhh_20GeV_newWeight.Sumw2()

hpth_20GeV_newWeight = ROOT.TH1F("pth_20GeV_newWeight", "mhh_20GeV;p_{T}^{h};Entries", 25, 0 ,500)
hpth_20GeV_newWeight.Sumw2()



#
# Get k-factors
#
from hhTools import getData
from hhTools import getKFactors
from hhTools import gethhWeight
fullData    = getData("scripts/mhh_full_13TeV_20GeV.txt")
FTAppoxData = getData("scripts/mhh_ftapprox_13TeV_20GeV.txt")
kFactorUpperEdges, kFactorsNew = getKFactors(fullData,FTAppoxData)


m_mHHUpperBinEdge = ROOT.std.vector(float)()
m_kFactor         = ROOT.std.vector(float)()
m_kFactorErr      = ROOT.std.vector(float)()


def addMHHBin(upperMhhBinEdge, kFactor, kFactorError):
    print "Loading ",kFactor 
    m_mHHUpperBinEdge.push_back(upperMhhBinEdge)
    m_kFactor        .push_back(kFactor        )
    m_kFactorErr     .push_back(kFactorError   )



def loadFile(fileName,histName):
 
  inFile   = ROOT.TFile(fileName,"READ")
  mhh_hist = inFile.Get(histName)

  #
  # Underflow + all bins
  #
  for iBin in range(mhh_hist.GetNbinsX()+1):
      addMHHBin((mhh_hist.GetBinLowEdge(iBin)+mhh_hist.GetBinWidth(iBin)),  
                mhh_hist.GetBinContent(iBin),
                mhh_hist.GetBinError  (iBin))

  #
  # Overflow
  #
  addMHHBin(1e9,
	    mhh_hist.GetBinContent(mhh_hist.GetNbinsX()+1),
	    mhh_hist.GetBinError  (mhh_hist.GetNbinsX()+1))



  inFile.Close()
  return


def gethhWeightFromTool(mhh):
    mhh_in_GeV = mhh*0.001
    for  iBin in range(m_nMHHBins):
        if(mhh_in_GeV < m_mHHUpperBinEdge.at(iBin)):
            return m_kFactor.at(iBin)
        
    return m_kFactor.back()



loadFile("data/SMhh_mhh_ReWeight.root","mhh_kfactor")
m_nMHHBins = m_mHHUpperBinEdge.size()




#
# Make output ntuple
# 
outFile = ROOT.TFile(o.outfileName,"RECREATE")

#
# Vars
#
mcEventWeight = array('f', [0.0] )
hpt       = ROOT.std.vector('float')()
heta      = ROOT.std.vector('float')()
hphi      = ROOT.std.vector('float')()
he        = ROOT.std.vector('float')()




iEvent = 0


inFile = ROOT.TFile.Open( o.infileName)

hhTree = inFile.Get("hhTree")
hhTree.SetBranchAddress( 'mcEventWeight', mcEventWeight)
hhTree.SetBranchAddress( 'hpt',       hpt  )
hhTree.SetBranchAddress( 'heta',      heta  )
hhTree.SetBranchAddress( 'hphi',      hphi  )
hhTree.SetBranchAddress( 'he',        he  )



# Print some information:
print( "Number of input events: %s" % hhTree.GetEntries() )
nEventThisFile = hhTree.GetEntries()


for entry in xrange( 0,nEventThisFile): # let's only run over the first 100 events for this example
    hhTree.GetEntry( entry )    

    iEvent +=1             

    if iEvent %10000 == 0:
        print "Processed .... ",iEvent,"Events"
    if o.nevents and (iEvent > int(o.nevents)):
        break


    nHiggs  = 0

    higgsLV = []


    for iHiggs in xrange( hpt.size()):

        higgsLV.append(ROOT.TLorentzVector())
        higgsLV[-1].SetPtEtaPhiE(hpt .at(iHiggs),
                                 heta.at(iHiggs),
                                 hphi.at(iHiggs),
                                 he  .at(iHiggs))




    if not len(higgsLV) == 2: 
        print "ERROR nHiggs",nHiggs
        import sys
        sys.exit(-1)

    h1 = higgsLV[0]
    h2 = higgsLV[1]

    mhh = (h1+h2).M()

    weight = mcEventWeight[0]

    hmhh         .Fill(mhh, weight) 
    hmhh_20GeV   .Fill(mhh, weight) 
    hpth_20GeV   .Fill(h1.Pt(), weight)
    hpth_20GeV   .Fill(h2.Pt(), weight)

    hhWeight = gethhWeight(mhh,kFactorUpperEdges,kFactorsNew)

    hmhh_20GeV_reWeight .Fill(mhh,     weight*hhWeight) 
    hpth_20GeV_reWeight .Fill(h1.Pt(), weight*hhWeight)
    hpth_20GeV_reWeight .Fill(h2.Pt(), weight*hhWeight)

    hhWeightTool = gethhWeightFromTool((mhh*1000))

    hmhh_20GeV_newWeight .Fill(mhh,     weight*hhWeightTool) 
    hpth_20GeV_newWeight .Fill(h1.Pt(), weight*hhWeightTool)
    hpth_20GeV_newWeight .Fill(h2.Pt(), weight*hhWeightTool)



inFile.Close()


outFile.cd()
hmhh       .Write()
hmhh_20GeV .Write()
hpth_20GeV .Write()

hmhh_20GeV_reWeight .Write()
hpth_20GeV_reWeight .Write()

hmhh_20GeV_newWeight .Write()
hpth_20GeV_newWeight .Write()

