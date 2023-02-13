#include "RooSimultaneous.h"
#include "RooStats/ProofConfig.h"
#include "RooStats/ToyMCSampler.h"
#include "RooRealVar.h"
#include "RooRandom.h"

#include <map>

//#define _DEBUG_
#include "QFramework/TQLibrary.h"

#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSUtils.h"
#include "SFramework/TSStatisticsManager.h"

/*<cafdoc name=MakeToyDatasets>
  MakeToyDatasets
  ===========================
  
  Make some toy datasets. They will have the names `toyData_%d`, where
 `%d` will be the zero-padded index of the toy.

  For each toy, a snapshot `SnSh_AllVars_toyData_%d` will be saved in
  the workspace as well.

  Usage:
  ---------------
  ```
  +MakeToyDatasets {  
    +Morphing {
      <snapshot="SnSh_AllVars_Nominal">
      <rollNPs=true>
      <toys=100>
    }
  }
  ```

  Advanced Usage:
  ---------------

  In order to fit the toy datasets, a construction like the following
  one can be used.

  ```
  $for(i,0,100){
    +Fit.toy_$(i) {
      +Morphing{
        <dataset="toyData_$(i)">
      }
    }
  }

  ```

</cafdoc> */

namespace TSBaseActions {

class MakeToyDatasets : public TSStatisticsManager::Action {

  std::map<RooRealVar*,RooRealVar*> mapParameters(RooArgSet& nuis, const RooArgSet& globs) const {
    std::map<RooRealVar*,RooRealVar*> retval;
    RooFIter paramIter(nuis.fwdIterator()) ;
    RooRealVar* np = NULL;
    while((np=(RooRealVar*)paramIter.next())) {
      TString npname(np->GetName());
      npname.Prepend("nom_");
      RooFIter globiter(globs.fwdIterator()) ;
      RooRealVar* glob = NULL;
      while((glob=(RooRealVar*)globiter.next())) {
        TString globname(glob->GetName());
        if(npname.BeginsWith(globname)){
          retval[np] = glob;
          break;
        }
      }
    }
    return retval;
  }


    bool execute(TQFolder * config) const override {
      
      RooRandom::randomGenerator()->SetSeed(config->getTagIntegerDefault("seed",TQUtils::getCurrentTime()));
      
      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("unable to make toy dataset for '%s': no such workspace!",config->GetName()));
        return false;
      }
      TString namePrefix = config->getTagStringDefault("name","toyData_");
      int ntoys = config->getTagIntegerDefault("toys",1);
      if(ntoys<1){
        manager->error(TString::Format("cannot generate number of toys '%d', needs to be positive integer!",ntoys));
        return false;
      }

      TString mcName = config->getTagStringDefault("modelConfig","ModelConfig");
      TString snapshot = config->getTagStringDefault("snapshot","SnSh_AllVars_Nominal");
      RooStats::ModelConfig* mc = dynamic_cast<RooStats::ModelConfig*>(workspace->obj(mcName));
      if(!mc){
        manager->error(TString::Format("unable to retrieve ModelConfig '%s' from workspace '%s'!",mcName.Data(),workspace->GetName()));
        return false;
      }
      
      RooArgSet pois;
      std::vector<TString> poifilters = config->getTagVString("POIs");
      if(poifilters.size() == 0) poifilters.push_back("*");
      for(auto poi:*(mc->GetParametersOfInterest())){
        for(const auto&filter: poifilters){
          if(TQStringUtils::matches(poi->GetName(),filter)){
            pois.add(*poi);
          }
        }
      }
      RooArgSet nuis;
      std::vector<TString> npfilters = config->getTagVString("NPs");
      if(npfilters.size() == 0) npfilters.push_back("*");
      for(auto np:*(mc->GetNuisanceParameters())){
        for(const auto&filter: npfilters){
          if(TQStringUtils::matches(np->GetName(),filter)){
            nuis.add(*np);
          }
        }
      }
      RooArgSet globs (*(mc->GetGlobalObservables()));
      RooArgSet obs   (*(mc->GetObservables()));
      RooAbsPdf* pdf = mc->GetPdf();

      obs.add(workspace->allCats());

      //      RooStats::ProofConfig pc(*workspace, 1, "workers=1", false);
      RooStats::ToyMCSampler sampler;
      sampler.SetGenerateBinned(true);
      #ifdef _DEBUG_
      pdf->Print();
      obs.Print();
      pois.Print();
      #endif
      sampler.SetPdf(*pdf);
      double nexp = pdf->expectedEvents(obs);
      sampler.SetObservables(obs);
      //      sampler.SetParametersForTestStat(pois);
      //      sampler.SetProofConfig(&pc);

      RooArgSet allVars;
      bool rollNPs = config->getTagBoolDefault("rollNPs",true);
      if(rollNPs){
        manager->info(TString::Format("adding %d nuisance parameters to the list of random variables!",nuis.getSize()));
        //        sampler.SetPriorNuisance(simPdf);
        sampler.SetNuisanceParameters(nuis);
	//        sampler.SetGlobalObservables(globs);  
        allVars.add(nuis);
      } else {
        manager->info("only using data stat for toys!");
      }
      bool rollPOIs = config->getTagBoolDefault("rollPOIs",true);
      if(rollPOIs){
        manager->info(TString::Format("adding %d parameters of interest to the list of random variables!",pois.getSize()));
        allVars.add(pois);
      } else {
        manager->info("only using SM signal for toys!");
      }      
      
//      if(!sampler.CheckConfig()){
//        manager->error("ToyMCSampler configuration invalid!");
//        return false;
//      }
      
      std::vector<RooAbsData*> toys;
      std::map<RooRealVar*,RooRealVar*> npglobs = mapParameters(nuis,workspace->allVars());
      manager->info(TString::Format("found global observables for %d of %d nuisance parameters",(int)(npglobs.size()),(int)(nuis.getSize())));
      bool takeSnapshots = config->getTagBoolDefault("takeSnapshots",allVars.getSize() > 0);
      for(int i=0; i<ntoys; ++i){
        if(!workspace->loadSnapshot(snapshot)){
          manager->error(TString::Format("unable to load snapshot '%s'",snapshot.Data()));
          return false;
        }
        TString name(namePrefix+TQStringUtils::padNumber(i,log10(ntoys-1)+1));
	//	RooAbsData* toydata = allVars.size() > 0 ? sampler.GenerateToyData(allVars) : (RooAbsData*)(pdf->generateBinned(obs,RooFit::Extended()));
	RooAbsData* toydata = sampler.GenerateToyData(allVars);
        #ifdef _DEBUG_
        allVars.Print();
        #endif
        toydata->SetName(name);
        
        #ifdef _DEBUG_
        std::cout << nexp << " " << toydata->sumEntries() << std::endl;
        #endif
        manager->info(TString::Format("generated toy %s with %g events (%.1f sigma), %d random variables",name.Data(),toydata->sumEntries(),(toydata->sumEntries()-nexp)/sqrt(nexp),allVars.getSize()));
        
        if(takeSnapshots){
          for(const auto&p:npglobs){
            p.first->setVal(p.second->getVal());
          }
          workspace->saveSnapshot(TString::Format("SnSh_AllVars_%s",name.Data()).Data(),allVars);
        }
        
        toys.push_back(toydata);
      }

      workspace->loadSnapshot(snapshot);

      for(auto toy:toys){
        workspace->import(*toy);
      }
      
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new MakeToyDatasets(),"MakeToyDatasets");
  }
}
