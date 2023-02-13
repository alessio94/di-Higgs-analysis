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
#include "RooRealIntegral.h"
#include "RooProduct.h"
#include "RooRealVar.h"
#include "RooFitResult.h"
#include "RooAddition.h"
#include "RooSimultaneous.h"

#include "TFile.h"

#include "RooCmdConfig.h"

#include <map>
#include <utility>


namespace TSBaseActions {
    
  class PrintSystematicsEffects : public TSStatisticsManager::Action {
    int getNonNormalizingClientsRecursive(RooAbsArg* server, std::vector<RooAbsArg*> & clients) const {
      //fills the provided vector with clients
      if (!server) return 0;
      TIterator* clItr = server->clientIterator();
      TObject* cl;
      int nAdded = 0;
      while ( (cl=clItr->Next()) ) {
        if ( cl->InheritsFrom(RooAbsPdf::Class()) ) {
          RooAbsPdf* clPdf = static_cast<RooAbsPdf*>(cl);
          double expected = clPdf->expectedEvents(nullptr); 
          if ( expected != 0 ) { //expectedEvents returns 0 in base implementation and non-zero if there is an extended term
            //std::cout<<"Expecting "<<expected<<" events for pdf:"<<std::endl;
            //clPdf->Print();
            clients.push_back(clPdf);
            nAdded++;
          } else {
            nAdded += getNonNormalizingClientsRecursive(clPdf, clients);
          }
        } else { //not a pdf
          //std::cout<<"Not a pdf: "<<cl->GetName()<<std::endl;
          //nAdded += getNonNormalizingClientsRecursive(dynamic_cast<RooAbsArg*>(cl), clients);
          clients.push_back(dynamic_cast<RooAbsArg*>(cl));
          nAdded++;
        }
      }
      return nAdded;
    }
    
    
    bool execute(TQFolder * config) const override {
      if(!config){
        throw std::runtime_error("received NULL pointer as config object!");
      }
      
      RooWorkspace * ws = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->getTagStringDefault("workspace",config->GetName())));
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
			TString whitelist = config->getTagStringDefault("whitelist","");
			TString blacklist = config->getTagStringDefault("blacklist","");
			bool nominalAtZero = config->getTagBoolDefault("nominalAtZero",false);
			bool showSameSign = config->getTagBoolDefault("showSameSign",false);
			double variationDefault = config->getTagDoubleDefault("var",1.0);
      double varUp = fabs(config->getTagDoubleDefault("varUp",variationDefault));
      double varDown = config->getTagDoubleDefault("varDown",variationDefault);
       
      RooSimultaneous* pdf = dynamic_cast<RooSimultaneous*>(mc->GetPdf());
      if(!pdf){
        manager->error("ModelConfig does not have a RooSimultaneous PDF attached!");
        return false;
      }
      
      
      
      const RooArgSet* nps (mc->GetNuisanceParameters());
      //const RooArgSet* pois(mc->GetParametersOfInterest());
      //RooArgSet allVars;
      //allVars.add(*pois);
      //allVars.add(*nps);
      RooFIter npItr(nps->fwdIterator());
      RooAbsArg* np;
      
      
      while((np = npItr.next())) {
        RooRealVar* npReal = dynamic_cast<RooRealVar*>(np);
        if (!npReal) {
          manager->error(TString::Format("NP '%s' is not a RooRealVar or derived type...",np->GetName()).Data());
          continue;
        }
        if ( TQStringUtils::matchesFilter(np->GetName(),blacklist,",") && !TQStringUtils::matchesFilter(np->GetName(),whitelist,",") ) continue;
        std::cout<<"Matching parameter to investigate: "<<std::endl;
        np->Print();
        std::vector<RooAbsArg*> yieldPars;
        getNonNormalizingClientsRecursive(np, yieldPars);
        
        double npNom  = npReal->getVal();
        //double errUp   = npReal->getErrorHi();
        //double errDown = npReal->getErrorLo();
        
       
        std::cout<<"--------------------------------------------------"<<std::endl;
        std::cout<<"For NP "<<np->GetName()<<"("<<npNom<<"+"<<varUp<<"/-"<<varDown<<"):"<<std::endl;
        for ( RooAbsArg* param : yieldPars ) {
          if (!param) continue;
          if (!param->InheritsFrom(RooAbsReal::Class())) continue;
          RooAbsReal* realParam =static_cast<RooAbsReal*>(param);
          npReal->setVal(nominalAtZero?0.:npNom);
          double nominal = realParam->getVal();
          if (fabs(nominal) < std::numeric_limits<double>::epsilon() ) {
            manager->warn(TString::Format("For parameter '%s' the nominal yield evaluated to zero. Did you request to have the NPs set to zero for nominal? This might not be suitable for normalization factors!",realParam->GetName()).Data());
            continue;
          }
          npReal->setVal((nominalAtZero?0.:npNom)+varUp);
          double high = realParam->getVal();
          npReal->setVal((nominalAtZero?0.:npNom)-varDown);
          double low = realParam->getVal();
          double highRel= (high-nominal)/nominal;
          double lowRel = (low-nominal)/nominal;
          if (!showSameSign && highRel*lowRel>0) continue; //skip same-signed effects, typically these are constraint terms (e.g. gaussians where every displacement away from nominal causes a reduction)
          std::cout<<"Effect on "<<realParam->GetName()<<":  "<<lowRel<<"/"<<highRel<<std::endl;
        }
        npReal->setVal(npNom); //reset to original value
        std::cout<<"--------------------------------------------------"<<std::endl;
        
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new PrintSystematicsEffects(),"PrintSystematicsEffects");
  }
}
