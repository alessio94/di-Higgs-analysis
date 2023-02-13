#include "TH1.h"

#include "QFramework/TQFolder.h"

#include "SFramework/TSStatisticsManager.h"

/*<cafdoc name=ModifyHistogramInModel>
  ModifyHistogramInModel
  ===========================
  
  Replace a histogram inside a statistical model created with SFramework
  
  Usage:
  ---------------
  ```
  +ModifyHistogramInModel {
    +HWWVBF{
      <histPath = ".Histograms/Nominal/SR_VBF2J_MJJ_350_700_PTH_0_200/">
       TH1F("sigVBF","sigVBF",7,0,7) << 7@{0+-0, 36.61+-0.18, 41.41+-0.19, 20.92+-0.14, 23.12+-0.14, 21.11+-0.14, 20.00+-0.14, 39.27+-0.19, 0+-0};
       <histname = "sigVBF">
    }
  }
  ```
  The following information/tags are required
  * `histPath`: path in which the histogram that is to be modified is located in the model
  * histogram definition: Definition of histogram that should replace the existing one. The name of that histogram needs to match the sample name (here: sigVBF)
  * `histName`: name of above defined histogram

  The following tags are optional
  * `verbose`: it's recommended to set the verbose option to true to double-check the behavior of that action with the printouts

</cafdoc> */

namespace TSBaseActions {

  class ModifyHistogramInModel : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {
      if(!config){
        throw std::runtime_error("received NULL pointer as config object!");
      }

      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        return false;
      }

      // get histogram from model
      TString histname = config->getTagStringDefault("histName","histogram");

      // try to explicitely delete the histogram that is to be replaced to catch errors
      TString histPath = config->getTagStringDefault("histPath", "");
      if (config->getTagBoolDefault("verbose", true)) {
        TH1* oldHist = dynamic_cast<TH1F*>(model->getObject(TQFolder::concatPaths(histPath, histname)));
        manager->info("You are replacing the following histogram...");
        manager->info("Bin, hist content, error");
        for (int i=0; i< oldHist->GetNbinsX()+2; i++) {
          manager->info(TString::Format("%d, %f, %f", i, oldHist->GetBinContent(i), oldHist->GetBinError(i)));
        }
      }
        
      if (!model->deleteObject(TQFolder::concatPaths(histPath, histname))) {
        manager->error(TString::Format("Failed to delete histogram at path '%s' ", TQFolder::concatPaths(histPath, histname).Data()));
        return false;
      }

      TH1* newHist = dynamic_cast<TH1*>(config->getObject(histname));
      if(!newHist){
        manager->error("no replacement histogram given!");
        return false;
      }
      if (config->getTagBoolDefault("verbose", true)) {
        manager->info("...with a histogram with the following content");
        manager->info("Bin, hist content, error");
        for (int i=0; i< newHist->GetNbinsX()+2; i++) {
          manager->info(TString::Format("%d, %f, %f", i, newHist->GetBinContent(i), newHist->GetBinError(i)));
        }
      }
      
      // add new histogram
      if (!model->addObject(newHist, histPath)) {
        manager->error(TString::Format("Failed to add histogram at path '%s' ", histPath.Data()));
        return false;
      }

      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ModifyHistogramInModel(),"ModifyHistogramInModel");
  }
}
