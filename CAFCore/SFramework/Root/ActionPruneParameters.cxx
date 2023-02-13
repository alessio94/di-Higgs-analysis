#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQTable.h"
#include "QFramework/TQHistogramUtils.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSUtils.h"
#include "SFramework/TSHistogramExporter.h"

#include "RooStats/ModelConfig.h"

#include "RooCategory.h"
#include "RooBinning.h"
#include "RooPoisson.h"
#include "RooDataSet.h"
#include "RooRealSumPdf.h"
#include "RooProdPdf.h"
#include "RooArgSet.h"
#include "RooProduct.h"
#include "RooRealVar.h"
#include "RooRealIntegral.h"
#include "RooFitResult.h"
#include "RooAddition.h"
#include "RooSimultaneous.h"

#include "TFile.h"
#include "TMath.h"

#include "RooCmdConfig.h"

#include <map>
#include <utility>

/*<cafdoc name=PruneParameters>
  PruneParameters
  ===========================
  
  Usage:
  ---------------
  ```
  +PruneParameters {
    +Morphing {
      <threshold = 0.01>
      <filter={"*stat_uncert*"}>
    }
  }
  ```

</cafdoc> */

namespace {
  double getGammaUncertainty(RooRealVar* gamma){
    TIterator* itr = gamma->clientIterator();
    TObject* obj = NULL;;
    while((obj = itr->Next())){
      RooProduct* prod = dynamic_cast<RooProduct*>(obj);
      if(!prod) continue;
      gamma->setVal(1.);
      double val = prod->getVal();
      return sqrt(val)/val;
    }
    return -1;
  }
}


namespace TSBaseActions {
    
  class PruneParameters : public TSStatisticsManager::Action {

    bool editWS(TQFolder * config, RooWorkspace* ws, RooStats::ModelConfig* mc, RooSimultaneous* pdf) const {    
      const RooArgSet* nps (mc->GetNuisanceParameters());
      RooArgSet allVars;
      if(nps)
        allVars.add(*nps);
        
      TString snapshot = config->getTagStringDefault("snapshot","SnSh_AllVars_Nominal");

      if(!ws->loadSnapshot(snapshot)){
        manager->error(TString::Format("unable to load snapshot '%s'",snapshot.Data()));
        return false;
      } else {
        manager->info(TString::Format("using snapshot '%s'",snapshot.Data()));
      }
      TSUtils::setParameterValues(&allVars,config,"setPars",true);
      TSUtils::setParametersConstant(&allVars,config->getTagVString("constPars"),true);
      manager->info(TString::Format("merging parameters based on snapshot '%s'",snapshot.Data()));                      
      
      std::vector<TSHistogramExporter::Region*> regions = TSHistogramExporter::makeRegions(pdf,mc->GetObservables(),ws->allData());
      if(regions.size() < 1) {
        manager->error("unable to construct regions!");
        return false;
      }

      std::vector<TString> filter = config->getTagVString("filter");
      if(filter.size() == 0) filter.push_back("*");
      
      double threshold = config->getTagDoubleDefault("threshold",1e-4);

      std::vector<TString> remove;
      RooAbsArg* obj;
      RooFIter itr(allVars.fwdIterator());
      while((obj = itr.next())){
        RooRealVar* np = dynamic_cast<RooRealVar*>(obj);
        bool skip = true;
        for(auto f:filter){
          if(TQStringUtils::matchesFilter(np->GetName(),f)) skip=false;
        }
        if(skip) continue;
               
        double cval = np->getVal();            
        double eup = np->getErrorHi();
        double edn = np->getErrorLo();
        
        // this is needed because the gamma parameters don't typicall have correct prefit uncertainties assigned
        if(TMath::AreEqualRel(eup,-1.0,1e-6) && TMath::AreEqualRel(edn,1.0,1e-6) && TQStringUtils::matches(np->GetName(),"gamma_stat_*")){
          double val = ::getGammaUncertainty(np);
          if(val > 0){
            eup = val;
            edn = -val;
          }
        }
          
        double max = 0;

        for(const auto& region:regions){
          region->select();
          RooAbsPdf* pdf = region->getPdf();
          if(!pdf) {
            manager->warn("unable to get region pdf!");
            continue;
          }
          RooArgList observables(region->getObservables());
          RooRealVar* observable = dynamic_cast<RooRealVar*>(observables.first());
          if(!observable){
            manager->warn("unable to get region observable!");
            continue;
          }
            
          TH1* nomhist = pdf->createHistogram("nominal",*observable);
          nomhist->SetDirectory(NULL);
          np->setVal(cval+fabs(eup));
          TH1* uphist = pdf->createHistogram("up",*observable);
          uphist->SetDirectory(NULL);
          np->setVal(cval-fabs(edn));
          TH1* dnhist = pdf->createHistogram("down",*observable);
          dnhist->SetDirectory(NULL);
          np->setVal(cval);
            
          for(int i=0; i<nomhist->GetNbinsX(); ++i){
            double nomval = nomhist->GetBinContent(i+1);
            double upval = uphist->GetBinContent(i+1);
            double dnval = dnhist->GetBinContent(i+1);
            max = std::max(max,fabs(upval/nomval-1));
            max = std::max(max,fabs(dnval/nomval-1));
          }
          delete nomhist;
          delete uphist;
          delete dnhist;
        }
        
        manager->info(TString::Format("measuring impact of parameter '%s', maximum is %g",np->GetName(),max));
        if(max < threshold) remove.push_back(np->GetName());
      }
      
      TQTaggable edits;
      edits.setTagList("removeNPs",remove);
      edits.setTagList("constPars",remove);
      TSUtils::applySettings(&edits,ws->allVars(),mc);
      RooWorkspace* newWs = TSUtils::makeCleanWorkspace(ws,ws->GetName(), "ModelConfig", false /*don't copy snapshots*/);
      workspaces()->Remove(ws);
      workspaces()->addObject(newWs);
      delete ws;
      TSUtils::takeSnapshot(newWs, config->getTagStringDefault("saveSnapshot",snapshot), nullptr, manager); //not passing an explicit config here (3rd argument) = use default varaible sets to include in the snapshot
      
      return true;
    }

    bool execute(TQFolder * config) const override {
      if(!config){
        throw std::runtime_error("received NULL pointer as config object!");
      }
      
      RooWorkspace * ws = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!ws){
        manager->error(TString::Format("unable to load workspace '%s': no such workspace!",config->GetName()));
        return false;
      }

      TString mcname = config->getTagStringDefault("modelConfig","ModelConfig");
      RooStats::ModelConfig* mc = dynamic_cast<RooStats::ModelConfig*>(ws->obj(mcname.Data()));
      if(!mc){
        manager->error(TString::Format("unable to load ModelConfig from workspace '%s'!",config->GetName()));
        return false;
      }
			
      RooSimultaneous* pdf = dynamic_cast<RooSimultaneous*>(mc->GetPdf());
      if(!pdf){
        manager->error("ModelConfig does not have a PDF attached!");
        return false;
      }
      
      if(!editWS(config,ws,mc,pdf)){
        manager->error(TString::Format("error editing workspace '%s'!",config->GetName()));
        return false;
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new PruneParameters(),"PruneParameters");
  }
}
