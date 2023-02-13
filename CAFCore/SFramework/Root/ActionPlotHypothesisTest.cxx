#include <limits>

#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLine.h"
#include "TH1F.h"

#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSHypothesisTest.h"

#define inf std::numeric_limits<double>::infinity()

/*<cafdoc name=PlotHypothesisTest>
  PlotHypothesisTest
  ===========================
  
  This action plots the results of a hypothesis test. The test can be
  performed using the `ActionRunHypothesisTest`.
  
  Usage:
  ---------------
  ```
  +PlotHypothesisTest {
    +Morphing {
      +HiggsCP {
        <outputFile="hypotest.pdf">
        <nBins=20>
        +h0 {
          <name="even">
          <title="CP-even">
          <color=kBlue>
        }
        +h1 {
          <name="odd">
          <title="CP-odd">
          <color=kRed>
        }
      }
    }
  }
  ```

</cafdoc> */

namespace TSBaseActions {

  class ActionPlotHypothesisTest : public TSStatisticsManager::Action {

    std::vector<int> defaultColors = {kBlue,kRed};
    std::vector<int> defaultFillStyles = {3004,3005};

    bool isOK(int status, double minnll) const {
      return (status == 1 || status == 0) && TQUtils::isNum(minnll);
    }

    bool execute(TQFolder * config) const override {

      gStyle->SetOptTitle(false);
      gStyle->SetOptStat(false);

      TQFolder * results = manager->getResults();
      if (!config) {
    		return 0;
    	}
      
      int nOK=0;

      TQFolderIterator itr(config->getListOfFolders("?"));
      while(itr.hasNext()){
        TQFolder* plotconfig = itr.readNext();
        if(!plotconfig) continue;

        TString outputFile = plotconfig->getTagStringDefault("outputFile", "plot.pdf");
        
        int nbinsOrig = plotconfig->getTagIntegerDefault("nBins",20);
        
        TString sourcePath = plotconfig->getTagStringDefault("source",plotconfig->GetName());
        TString path = TQFolder::concatPaths(config->GetName(),"HypothesisTests",sourcePath);
        TQFolder* source = results->getFolder(path);
        if(!source){
          manager->error(TString::Format("unable to access hypothesis test from path '%s'",path.Data()));
          continue;
        }

        TQFolder* h0 = plotconfig->getFolder("h0");
        if(!h0){
          manager->error(TString::Format("provide setup of default hypothesis via a 'h0' subfolder in '%s'",plotconfig->GetName()));
          continue;
        }
        TString h0name = h0->getTagStringDefault("name",h0->GetName());

        TQFolder* h1 = plotconfig->getFolder("h1");
        if(!h1){
          manager->error(TString::Format("provide setup of alternate hypothesis via a 'h1' subfolder in '%s'",plotconfig->GetName()));
          continue;
        }
        TString h1name = h1->getTagStringDefault("name",h1->GetName());
       
        TQFolderIterator toysets(source->getListOfFolders("toys/?"));
        std::map<TQFolder*,std::vector<double> > results;
        double minX = inf;
        double maxX = -inf;
        while(toysets.hasNext()){
          std::vector<double> vals;
          TQFolder* toyset = toysets.readNext();
          TQFolderIterator toys(toyset->getListOfFolders("?"));
          while(toys.hasNext()){
            TQFolder* toy = toys.readNext();
            int    h0_status = toy->getTagIntegerDefault(h0name+".status",-1);
            double h0_minnll = toy->getTagDoubleDefault (h0name+".minNll",0.);
            int    h1_status = toy->getTagIntegerDefault(h1name+".status",-1);
            double h1_minnll = toy->getTagDoubleDefault (h1name+".minNll",0.);
            if(isOK(h0_status,h0_minnll) && isOK(h1_status,h1_minnll)){
              double x = h0_minnll-h1_minnll;
              vals.push_back(x);
              minX = std::min(minX,x);
              maxX = std::max(maxX,x);
            } else {
              continue;
            }
          }
          results[toyset] = vals;
        }

        double xrangeOrig = maxX-minX;
        double bwidthOrig = xrangeOrig / nbinsOrig;

        double bwidth = TQUtils::roundAutoUp(bwidthOrig,1);
        int nbinsR = fabs(maxX)/bwidth;
        int nbinsL = fabs(minX)/bwidth;

        int nbins = nbinsR+nbinsL+1;
        double xl = -bwidth*(0.5+nbinsL);
        double xr =  bwidth*(0.5+nbinsR);

        double ymax = 0;
        std::vector<TH1*> histograms;
        TLegend* leg = new TLegend(0.75,0.75,0.95,0.95);
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);
        for(auto it:results){
          TQTaggable tags(it.first);
          if(it.first->getName()==h0->getName()){
            tags.importTags(h0);
          }
          if(it.first->getName()==h1->getName()){
            tags.importTags(h1);
          }
          
          TString title = tags.getTagStringDefault("title",it.first->GetName());
          int color = tags.getTagIntegerDefault("color",    defaultColors    [histograms.size()]);
          int style = tags.getTagIntegerDefault("fillStyle",defaultFillStyles[histograms.size()]);
          TH1* hist = new TH1F(it.first->GetName(),title,nbins,xl,xr);
          hist->SetDirectory(NULL);
          hist->SetLineColor(color);
          hist->SetFillColor(color);
          hist->SetFillStyle(style);
          hist->SetLineStyle(1);
          
          leg->AddEntry(hist,title,"f");
          for(auto v:it.second){
            hist->Fill(v);
          }

          ymax = std::max(TQHistogramUtils::getMax(hist),ymax);
          histograms.push_back(hist);
        }

        TCanvas * c = new TCanvas(plotconfig->GetName(),plotconfig->GetName());
        c->SetRightMargin(0.05);
        c->SetTopMargin(0.05);
        bool first = true;
        for(auto hist:histograms){
          if(first){
            hist->GetXaxis()->SetTitle(TString::Format("L(%s) - L(%s)",h0name.Data(),h1name.Data()));
            hist->GetYaxis()->SetTitle("# toys");
            hist->SetMaximum(1.05*ymax);
            hist->Draw("HIST");
          } else {
            hist->Draw("HISTSAME");
          }
          first = false;
        }

        std::vector<TLine*> lines;
        TQFolderIterator datasets(source->getListOfFolders("data/?"));
        while(datasets.hasNext()){
          TQFolder* data = datasets.readNext();
          int    h0_status = data->getTagIntegerDefault(h0name+".status",-1);
          double h0_minnll = data->getTagDoubleDefault (h0name+".minNll",0.);
          int    h1_status = data->getTagIntegerDefault(h1name+".status",-1);
          double h1_minnll = data->getTagDoubleDefault (h1name+".minNll",0.);
          if(isOK(h0_status,h0_minnll) && isOK(h1_status,h1_minnll)){
            double x = h0_minnll-h1_minnll;
            TLine* line = new TLine(x,0.,x,ymax);
            line->SetLineColor(kBlack);
            line->SetLineStyle(lines.size());
            line->Draw();
            leg->AddEntry(line,data->GetName(),"l");
            lines.push_back(line);
          } else {
            continue;
          }
        }
        leg->Draw();

        c->SaveAs(TQPathManager::getPathManager()->getTargetPath(outputFile).c_str());
        for(auto hist:histograms){
          delete hist;
        }
        for(auto line:lines){
          delete line;
        }
        delete leg;
        delete c;

        nOK++;

      }
      return nOK>0;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ActionPlotHypothesisTest(),"PlotHypothesisTest");
  }
}
