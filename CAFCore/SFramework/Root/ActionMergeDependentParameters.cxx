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
#include "RooCatType.h"
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

/*<cafdoc name=MergeDependentParameters>
  MergeDependentParameters
  ===========================
  
  This action will merge all dependent (non-independent) parameters
  that match the name filter in some workspace.

  This is useful to avoid non-invertible hesse-matrices, especially in
  scenarios where nuisance parameters are added explicitly (as for
  theory uncertainties) while at the same time applying some automatic
  bin merging, as this will merge any two parameters that affect only
  a single bin, if that bin is the same one.
  
  Usage:
  ---------------
  ```
  +MergeDependentParameters {
    +Morphing {
      <filter={"*stat_uncert*"}>
    }
  }
  ```

</cafdoc> */

namespace TSBaseActions {
    
  class MergeDependentParameters : public TSStatisticsManager::Action {

    RooWorkspace* editWS(TQFolder * config, RooWorkspace* ws, RooStats::ModelConfig* mc, RooSimultaneous* pdf) const {    
      const RooArgSet* nps (mc->GetNuisanceParameters());
      const RooArgSet* pois(mc->GetParametersOfInterest());
      RooArgSet allVars;
      if(pois)
        allVars.add(*pois);
      if(nps)
        allVars.add(*nps);
        

      TString snapshot = config->getTagStringDefault("snapshot","SnSh_AllVars_Nominal");

      if(!ws->loadSnapshot(snapshot)){
        manager->error(TString::Format("unable to load snapshot '%s'",snapshot.Data()));
        return NULL;
      } else {
        manager->info(TString::Format("using snapshot '%s'",snapshot.Data()));
      }
      TSUtils::setParameterValues(&allVars,config,"setPars",true);
      TSUtils::setParametersConstant(&allVars,config->getTagVString("constPars"),true);
      manager->info(TString::Format("merging parameters based on snapshot '%s'",snapshot.Data()));                      
      

      std::vector<TSHistogramExporter::Region*> regions = TSHistogramExporter::makeRegions(pdf,mc->GetObservables(),ws->allData());
      if(regions.size() < 1) {
        manager->error("unable to construct regions!");
        return NULL;
      }

      struct NPeffect {
        std::vector<double> up;
        std::vector<double> down;
      };

      std::map<RooRealVar*,NPeffect> effects;

      std::vector<TString> filter = config->getTagVString("filter");
      if(filter.size() == 0) filter.push_back("*");
      
      RooAbsArg* obj;
      RooFIter itr(allVars.fwdIterator());
      while((obj = itr.next())){
        NPeffect effect;
       
        RooRealVar* np = dynamic_cast<RooRealVar*>(obj);
        bool ok = false;
        for(auto f:filter){
          if(TQStringUtils::matchesFilter(np->GetName(),f)) ok=true;
        }
        if(!ok) continue;
        
        manager->info(TString::Format("measuring impact of parameter '%s'",np->GetName()));
        
        double cval = np->getVal();            
        double eup = np->getErrorHi();
        double edn = np->getErrorLo();
          
        for(const auto& region:regions){
          region->select();
          RooAbsPdf* pdf = region->getPdf();
          RooArgList observables(region->getObservables());
          RooRealVar* observable = dynamic_cast<RooRealVar*>(observables.first());
            
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
            effect.up.push_back(upval/nomval);
            effect.down.push_back(dnval/nomval);
          }
          delete nomhist;
          delete uphist;
          delete dnhist;
        }

        effects[np] = effect;
      }


      double epsilon = config->getTagDoubleDefault("epsilon",1e-4);
      
      std::set< std::set <RooRealVar*> > dependents;;
          
      for(const auto& c1:effects){
        std::set<RooRealVar*> deps;
        for(const auto& c2:effects){
          if(c1.first == c2.first){
            deps.insert(c2.first);
            continue;
          }
          
          bool equal = true;
          double ratioUp = 1.;
          double ratioDn = 1.;
          for(size_t i=0; i<c2.second.up.size();++i){
            if(TMath::AreEqualRel(c2.second.up[i],1,epsilon) && TMath::AreEqualRel(c1.second.up[i],1,epsilon)) continue;
            if     (ratioUp == 1.)                                                      ratioUp = c2.second.up[i] / c1.second.up[i];
            else if(!TMath::AreEqualRel(c2.second.up[i]/c1.second.up[i],ratioUp,epsilon)) equal=false;
            if     (ratioDn == 1.)                                                      ratioDn = c2.second.down[i] / c1.second.down[i];
            else if(!TMath::AreEqualRel(c2.second.down[i]/c1.second.down[i],ratioDn,epsilon)) equal=false;            
          }
          if(equal) deps.insert(c2.first);
        }
        if(deps.size() > 1) dependents.insert(deps);
      }
      
      int nset = 0;
      std::stringstream ss;
      std::map<std::string,bool> newpars;
      std::set<std::string> oldnps;
      ss << "EDIT::" << pdf->GetName() << "(" << pdf->GetName();
      for(const auto& s:dependents){
        ss << ",";
        nset++;
        TString base;
        bool isnp = false;
        for(const auto& p:s){
          bool np = mc->GetNuisanceParameters()->find(p->GetName());
          if(np) oldnps.insert(p->GetName());
          isnp = isnp || np;
          if(base.IsNull()){
            base=p->GetName();
          } else {
            base = base(0,TQStringUtils::compareHeads(base,p->GetName()));
          }
        }
        bool first=true;
        for(const auto& p:s){
          if(!first) ss << ",";
          first=false;
          std::stringstream s;
          s << base << "_mergedPar_" << nset;
          ss << p->GetName() << "=" << s.str();
          s << "[" << p->getVal() << "," << p->getMin() << "," << p->getMax() << "]";
          newpars[s.str()] = isnp;
        }
      }
      ss << ")";
      
      if(nset > 0){
        RooArgSet npset(*(mc->GetNuisanceParameters()));
        manager->info("editing workspace");
        manager->info(ss.str().c_str());
        for(const auto& s:newpars){
          RooAbsArg* newpar = ws->factory(s.first.c_str());
          if(!newpar){
            manager->error(TString::Format("unable to find parameter: %s",s.first.c_str()));
          } else {
            if(s.second){
              npset.add(*newpar);
            }
          }
        }
        for(const auto& name:oldnps){
          RooAbsArg* oldpar = ws->factory(name.c_str());
          if(!oldpar){
            manager->error(TString::Format("unable to find parameter: %s",name.c_str()));
          } else {
            npset.remove(*oldpar);
          }
        }
        mc->SetNuisanceParameters(npset);


        
        RooAbsArg* arg = ws->factory(ss.str().c_str());
        if(arg){
          arg->Print();
        } else {
          manager->error(TString::Format("unable to commit transaction: %s",ss.str().c_str()));
        }
        manager->info("cleaning up workspace");        
        RooWorkspace* newws = TSUtils::makeCleanWorkspace(ws,ws->GetName(),mc->GetName(),true,false);
        
        return newws;

      } else {
        manager->info("no dependency found!");
        return ws;
      }
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

      RooWorkspace* newws = editWS(config,ws,mc,pdf);
      if(newws){
        if(newws != ws){
          workspaces()->Remove(ws);
          workspaces()->addObject(newws);
          delete ws;
        }
      } else {
        manager->error(TString::Format("error editing workspace '%s'!",config->GetName()));
        return false;
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new MergeDependentParameters(),"MergeDependentParameters");
  }
}
