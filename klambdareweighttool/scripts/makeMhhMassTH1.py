
import ROOT

def makeKFactors():

    inFile = open("scripts/mhh_all_Kfactors_20GeV.txt","r")

    hMhhWeight = ROOT.TH1F("mhh_kfactor","mhh_kfactor", 33, 270., 930.)

    iBin = 0

    for line in inFile:
        words = line.split()
        if not len(words): continue
        if line[0] == "#": continue
        
        kfactor    = float(words[1])
        kfactorErr = float(words[2])
        print iBin, kfactor, kfactorErr
    
        hMhhWeight.SetBinContent(iBin,kfactor) 
        hMhhWeight.SetBinError  (iBin,kfactorErr) 
        iBin +=1
    
    
    outFile = ROOT.TFile("SMhh_mhh_kfactor_new.root","RECREATE")
    hMhhWeight.Write()
    outFile.Close()
    
    for i in range(hMhhWeight.GetNbinsX()+1):
        print hMhhWeight.GetBinLowEdge(i)+hMhhWeight.GetBinWidth(i) ,"--->",hMhhWeight.GetBinContent(i)
    
    
    print hMhhWeight.GetBinContent(hMhhWeight.GetNbinsX()+1)




def makeRatio(target,source):
    if not len(target) == len(source):
        print "ERROR",len(target),len(source)
        return

    hratio = ROOT.TH1F("mhh_ratio","mhh", 33, 270., 930.)

    for itr, tData in enumerate(target):

        #print "mass",tData[0],"ratio",tData[1]/source[itr][1]
        
        hratio.SetBinContent(hratio.FindBin(tData[0]),tData[1]/source[itr][1])
        hratio.SetBinError  (hratio.FindBin(tData[0]),tData[1]/source[itr][1]*0.1)

    return hratio

def makeRatioHist(target,source):

    hSF = target.Clone()
    hSF.SetName("mhh_kfactor")

    for iBins in range(target.GetNbinsX()+1):
        if not source.GetBinContent(iBins): continue
        ratio = target.GetBinContent(iBins)/source.GetBinContent(iBins)
        print target.GetBinCenter(iBins),"ratio:",ratio
        hSF.SetBinContent(iBins, ratio)
        hSF.SetBinError  (iBins, 0)

    #
    # Set Overflow
    #
    iBinOverflow = target.GetNbinsX()+1
    ratio = target.GetBinContent(target.GetNbinsX())/source.GetBinContent(target.GetNbinsX())
    hSF.SetBinContent(iBinOverflow, ratio)
    hSF.SetBinError  (iBinOverflow, 0)
    return hSF



def makeMassPlots():

    outFile = ROOT.TFile("SMhh_mhh_raw_new.root","RECREATE")

    from hhTools import histFromFile

    hFull     = histFromFile("full",     "scripts/mhh_full_13TeV_20GeV.txt")
    hFTApprox = histFromFile("ftapprox", "scripts/mhh_ftapprox_13TeV_20GeV.txt")
    hBorn     = histFromFile("born",      "scripts/mhh_born_20GeV.txt")

    print hFull.Integral(),"vs",hFTApprox.Integral()
    #hFull.Scale(hFTApprox.Integral()/hFull.Integral())
    hFull.Scale(1.0/hFull.Integral())    
    hFTApprox.Scale(1.0/hFTApprox.Integral())    

    outFileSF = ROOT.TFile("SMhh_mhh_ReWeight.root","RECREATE")
    hWeights = makeRatioHist(hFull,hFTApprox)
    hWeights.Write()

    #print "TEST"
    #hWeights = makeRatioHist(hFull,hFTApprox)

    from hhTools import getData
    fullData    = getData("scripts/mhh_full_13TeV_20GeV.txt")
    FTAppoxData = getData("scripts/mhh_ftapprox_13TeV_20GeV.txt")
    hRatio = makeRatio(fullData,FTAppoxData)

    outFile.cd()
    hFull.Write()
    hFTApprox.Write()
    hBorn.Write()
    hRatio.Write()
    outFile.Close()


    


#makeKFactors()
makeMassPlots()
