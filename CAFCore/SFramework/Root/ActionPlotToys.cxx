#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"

#include "TCanvas.h"

/*<cafdoc name=PlotToys>
  PlotToys
  ===========================
  
  This action plots the distribution of some toy datasets based on
  some value, for example, the postfit signa-strength or the discovery
  significance.
  
  Usage:
  ---------------
  ```
  +PlotToys.Z0 {
    +HWWRun2GGF {
      <input="Significance/toy*">
      <outputFile="workspaces/run2-ggf-$(fitLabel)/toys_z0.pdf">
      <histogram="TH1F('hist','',20,2,6)">
      <tag="Z0*">
      <label="Z_{0}">
    }
  }
  
  +PlotToys.mu {
    +HWWRun2GGF {
      <input="Significance/toy*">
      <outputFile="workspaces/run2-ggf-$(fitLabel)/toys_mu.pdf">
      <histogram="TH1F('hist','',20,0.05,2.05)">
      <tag="muGGF*val">
      <label="#mu">
    }
  }
  ```

  The value plotted on the horizontal axis is chosen using the tag `tag`.

</cafdoc> */


namespace TSBaseActions {
  
  class PlotToys : public TSStatisticsManager::Action {
    
    bool execute(TQFolder * config) const override {
      
      TQFolder * results = manager->getResults();
      if (!config) {
        return false;
      }
      
      TQFolder* thisresult = results->getFolder(config->GetName());
      if(!thisresult){
        manager->error(TString::Format("unable to obtain result '%s'",config->GetName()));
        return false;
      }

      TString pattern = config->getTagStringDefault("inputs","Significance/toy*");
      TString tag = config->getTagStringDefault("tag","Z0*");
      TString histdef = config->getTagStringDefault("histogram","TH1F('hist','',10,0,10)");

      TString outputFile = config->getTagStringDefault("outputFile", "plot.pdf");
      TString drawOpt = config->getTagStringDefault("drawOpt", "HIST");
      
      TH1* hist = TQHistogramUtils::createHistogram(histdef);
      hist->GetYaxis()->SetTitle("toys");
      hist->GetXaxis()->SetTitle(config->getTagStringDefault("label",tag));
      if(!hist){
        manager->error(TString::Format("unable to create histogram '%s'",histdef.Data()));
        return false;
      }
      TQHistogramUtils::applyStyle(hist,config);
      TList* toys = thisresult->getListOfFolders(pattern);
      if(!toys || toys->GetEntries()==0){
        manager->error(TString::Format("no toys found matching expression '%s'",pattern.Data()));
        return false;
      }
      TQFolderIterator itr(toys,true);
      while(itr.hasNext()){
        TQFolder* toy = itr.readNext();
        if(!toy) continue;
        
        TList* tags = toy->getListOfKeys(tag);
        if(tags){
          tags->SetOwner(true);
        }
        if(!tags || tags->GetEntries() == 0){
          manager->warn(TString::Format("toy '%s' seems to be missing a tag matching '%s'",toy->GetName(),tag.Data()));
          if(tags) delete tags;
          continue;
        }
        TQIterator tagitr(tags,true);
        while(tagitr.hasNext()){
          TObject* tag = tagitr.readNext();
          if(!tag) continue;
          double val;
          if(!toy->getTagDouble(tag->GetName(),val)){
            manager->error(TString::Format("tag '%s' is not of type double!",tag->GetName()));
            continue;
          }
          hist->Fill(val);
        }
      }
      
      TCanvas * c = new TCanvas("distribution");

      hist->Draw(drawOpt);

      if (c) {
        TString outpath = TQPathManager::getPathManager()->getTargetPath(outputFile).c_str();
        TQUtils::ensureDirectoryForFile(outpath);
        c->SaveAs(outpath);
        delete c;
      } 

      return c;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new PlotToys(),"PlotToys");
  }
}
