import ROOT
try:
    import ATLAS
except:
    pass

from   ROOTCore.Plotting      import plot_hists_wratio, plot_hists
from   ROOTCore.Utils         import make_legend

def getKFactors():

    inFile = open("scripts/mhh_all_Kfactors_20GeV.txt","r")

    kFactors = {}

    for line in inFile:
        words = line.split()
        if not len(words): continue
        if line[0] == "#": continue

        mass    = float(words[0])-10
        sf       = float(words[1])
        kFactors[mass] = sf

    return kFactors

from hhTools import getData
from hhTools import makeRatio
from hhTools import histFromFile


def applyKfactor(hWeighted, hUnWeighted, kFactors, norm):

    for i in range(hWeighted.GetNbinsX()+1):
        if hUnWeighted.GetBinCenter(i) not in kFactors: 
            print "Skipping",hUnWeighted.GetBinCenter(i)
            continue
        hWeighted.SetBinContent(i,hUnWeighted.GetBinContent(i)*kFactors[hUnWeighted.GetBinCenter(i)]*norm)
        print hUnWeighted.GetBinCenter(i),"--",hUnWeighted.GetBinContent(i),"-->",hUnWeighted.GetBinContent(i)*kFactors[hUnWeighted.GetBinCenter(i)],kFactors[hUnWeighted.GetBinCenter(i)]
    
    return

def makePlot(hists,name,colors=[ROOT.kBlack,ROOT.kBlack],log_y=1,rMin=0.5,rMax=1.5,max=3000,norm=False,showDenError=False,labels=None):
        thePlot = plot_hists_wratio([hists[0].Clone(),hists[1].Clone()],
                                    name,
                                    log_y = log_y,
                                    draw_options = ["PE","hist","hist"],
                                    line_colors = colors,
                                    y_title = "Entries",
                                    #bayesRatio = 1,
                                    #fill_colors = [ROOT.kBlack,ROOT.kRed],
                                    fill_style  = [ROOT.kBlack,0,0],
                                    rMin=rMin,
                                    rMax=rMax,
                                    max=max,
                                    showDenError = showDenError,
                                    )

        if labels:
            draw_options = ["PE","L"]
            leg = make_legend(hists = [hists[0],hists[1]],
                              labels = labels,
                              width = 0.30,
                              height= 0.05,
                              #x2=legend_limits['x2'],
                              #y2=legend_limits['y2'],
                              draw_options = draw_options
                              )


            thePlot['canvas'].cd()
            leg.Draw()


        thePlot['canvas'].Update()
        thePlot["canvas"].SaveAs(name+".pdf")


def main():

    #
    # Get input data
    #
    #inFile1= ROOT.TFile("data/DAODhh_4b.root","READ")
    inFile1= ROOT.TFile("TestHists.root","READ")

    hATLAS = inFile1.Get("mhh_20GeV")

    hATLAS_w = hATLAS.Clone()
    hATLAS_w.GetXaxis().SetTitle("m_{hh}")
    
    hATLAS_wNew = hATLAS.Clone()
    hATLAS_wNew.GetXaxis().SetTitle("m_{hh}")
    

    #
    #  Get Theory
    #
    hFull    = histFromFile("mhh_full","scripts/mhh_full_13TeV_20GeV.txt")
    hFTaprox  = histFromFile("mhh_ftaprox","scripts/mhh_ftapprox_13TeV_20GeV.txt")

    normFullToFT = hFTaprox.Integral()/hFull.Integral()
    print "Ratio is: ",normFullToFT
    
    #
    # Scale to lumi
    #
    relLumi = hATLAS.Integral()/hFTaprox.Integral()
    hFTaprox.Scale(relLumi)
    hFull   .Scale(relLumi)
    
    #
    # Get Kfactors
    #
    kFactorsOld = getKFactors()

    fullData    = getData("scripts/mhh_full_13TeV_20GeV.txt")
    FTAppoxData = getData("scripts/mhh_ftapprox_13TeV_20GeV.txt")
    kFactorsNew = makeRatio(fullData,FTAppoxData)

    applyKfactor(hATLAS_w, hATLAS, kFactorsOld, normFullToFT)
    applyKfactor(hATLAS_wNew, hATLAS, kFactorsNew, normFullToFT)    

    hATLAS_wNew.Scale(1./normFullToFT)

    
    print "was",    hATLAS.GetBinContent(hATLAS.FindBin(840))
    print "now",    hATLAS_wNew.GetBinContent(hATLAS_wNew.FindBin(840))
    print "ratio",  hATLAS_wNew.GetBinContent(hATLAS_wNew.FindBin(840))/hATLAS.GetBinContent(hATLAS.FindBin(840))
    print "ratioTh",hFull.GetBinContent(hFull.FindBin(840))/hFTaprox.GetBinContent(hFTaprox.FindBin(840))
    print "KFactor",kFactorsNew[840]

    makePlot([hATLAS,hFTaprox],"mhh_ATLAS_vs_FTapprox",labels=["ATLAS Nominal","FTapprox"])
    makePlot([hATLAS,hFull   ],"mhh_ATLAS_vs_Full",    labels=["ATLAS Nominal","NLO Full mT"])
    makePlot([hATLAS_w,hFull],"mhh_ATLASweighted_vs_Full",labels=["ATLAS Old Weighted","NLO Full mT"])#,colors=[ROOT.kBlue,ROOT.kBlue])

    makePlot([hATLAS_wNew,hFull],"mhh_ATLASreWeighted_vs_Full",labels=["ATLAS Weighting","NLO Full mT"])#,colors=[ROOT.kRed,ROOT.kBlue])    

    print "Input Norm",hATLAS.Integral()
    print "Output Norm",hATLAS_wNew.Integral()
    

    hPtFull    = histFromFile("pth_full","scripts/pth_full_13TeV_20GeV.txt",25,0,500)
    hPtFull.GetXaxis().SetTitle("p_{T}^{h}")
    hPtFull   .Scale(2*relLumi)
    hPtFTaprox  = histFromFile("pth_ftapprox","scripts/pth_ftapprox_13TeV_20GeV.txt",25,0,500)
    hPtFTaprox   .Scale(2*relLumi)

    makePlot([hPtFull,hPtFTaprox],"pth_FTapprox_vs_Full",labels=["NLO Full mT","FTapprox"],rMin=0.8,rMax=1.2,max=6000)


    hATLASPt = inFile1.Get("pth_20GeV")
    hATLASPt.GetXaxis().SetTitle("p_{T}^{h}")

    hATLASPt_rw = inFile1.Get("pth_20GeV_reWeight")
    hATLASPt_rw.GetXaxis().SetTitle("p_{T}^{h}")


    makePlot([hATLASPt_rw,hATLASPt],"pth_ATLASweighted_vs_ATLAS",labels=["ATLAS Weighted","ATLAS Nominal"],rMin=0.5,rMax=1.5,max=6000)

    makePlot([hATLASPt,hPtFTaprox],"pth_ATLAS_vs_FTaprox",labels=["ATLAS Nominal","FTapprox"]   ,rMin=0.5,rMax=1.5,max=6000)
    makePlot([hATLASPt,hPtFull]   ,"pth_ATLAS_vs_Full"   ,labels=["ATLAS Nominal","NLO Full mT"],rMin=0.5,rMax=1.5,max=6000)
    
    
    makePlot([hATLASPt_rw,hPtFTaprox],"pth_ATLASweighted_vs_FTaprox",labels=["ATLAS Weighted","FTapprox"]   ,rMin=0.5,rMax=1.5,max=6000)
    makePlot([hATLASPt_rw,hPtFull]   ,"pth_ATLASweighted_vs_Full"   ,labels=["ATLAS Weighted","NLO Full mT"],rMin=0.5,rMax=1.5,max=6000)


    #
    #
    #
    hATLAS_wNew.SetLineColor(ROOT.kRed+1)
    hATLAS_wNew.SetMarkerColor(ROOT.kRed+1)
    hFull.SetLineColor(ROOT.kBlue+1)
    hATLAS_w.SetLineColor(ROOT.kBlue+1)
    hATLAS_w.SetMarkerColor(ROOT.kBlue+1)


    can = ROOT.TCanvas()
    can.cd().SetLogy(1)
    hAxis = ROOT.TH1F("mhh","mhh;m_{hh};Entries",2,270,930)
    hAxis.GetYaxis().SetRangeUser(8e0,0.3e4)
    
    can.cd()
    hAxis.Draw()
    hFTaprox.Draw("same hist")
    hFull.Draw("same hist")
    hAxis.Draw("same axis")
    can.SaveAs("mhhComp_FTapprox_vs_Full.pdf")
    
    hAxis.Draw()
    hATLAS.Draw("same")
    hATLAS_w.Draw("same")
    hATLAS_wNew.Draw("same")
    can.SaveAs("mhhComp_ATLAS_comp.pdf")



if __name__ == "__main__":
    main()



