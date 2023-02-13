#include "QFramework/TQFolder.h"
#include "SFramework/TSStatisticsManager.h"
#include "RooHistFunc.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooDataHist.h"
#include "RooWorkspace.h"
#include "TH1.h"


/*<cafdoc name=ModifyHistogramInWorkspace>
  ModifyHistogramInWorkspace
  ===========================
  
  Replace a histogram inside a `RooFit` workspace by 
  manipulating the RooDataHist inside a RooHistFunc
  
  Usage:
  ---------------
  ```
  +ModifyHistogramInWorkspace {
    +HWWVBF{
       <histFunc = "sigVBF_SR_HWWRun2VBF_Hist_alphanominal">
       TH1F("histogram","histogram",7,0,7) << 7@{0+-0, 36.61+-0.18, 41.41+-0.19, 20.92+-0.14, 23.12+-0.14, 21.11+-0.14, 20.00+-0.14, 39.27+-0.19, 0+-0};
       <histName = "histogram">
    }
  }
  ```
  The following information/tags are required
  * `histFunc`: name of the RooHistFunc to be modified in the workspace
  * histogram definition: Definition of histogram that should replace the existing one in the workspace
  * `histName`: name of above defined histogram

</cafdoc> */


namespace TSBaseActions {

  class ModifyHistogramInWorkspace : public TSStatisticsManager::Action {
    bool execute(TQFolder * config) const override {
      if(!config){
        throw std::runtime_error("received NULL pointer as config object!");
      }

      RooWorkspace * ws = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!ws){
        manager->error(TString::Format("unable to load workspace '%s': no such workspace!",config->GetName()));
        return false;
      }

      TString hfname;
      if(!config->getTagString("histFunc",hfname)){
        manager->error("no histFunc given!");
        return false;
      }

      TH1* hist = dynamic_cast<TH1*>(config->getObject(config->getTagStringDefault("histName","histogram")));
      if(!hist){
        manager->error("no replacement histogram given!");
        return false;
      }

      RooHistFunc* hf = dynamic_cast<RooHistFunc*>(ws->function(hfname.Data()));
      if(!hf){
        manager->error(TString::Format("no histFunc named '%s' found in workspace!",hfname.Data()));
        return false;
      }        

      RooDataHist* data = &(hf->dataHist());
      data->reset();

      RooArgSet observables(*data->get());
      if(observables.getSize() > 1){
        manager->error("not implemented for multi-dimensional histograms!");
        return false;
      }

      RooRealVar* obs = dynamic_cast<RooRealVar*>(observables.first());

      for(int i=0; i < hist->GetNbinsX(); ++i){
        obs->setVal(obs->getBinning().binCenter(i));
        data->add(observables,hist->GetBinContent(i+1));
      }

      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ModifyHistogramInWorkspace(),"ModifyHistogramInWorkspace");
  }
}
