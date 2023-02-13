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

#include "RooCmdConfig.h"

#include <map>
#include <utility>


/*<cafdoc name=ExtractParameterTable>
  ExtractParameterTable
  ===========================
  
  Make a table of all the parameters in the workspace for different
  snapshots. Intended to work well together with the MakeToyDataSets
  Action, which saves snapshots of all the toys.

  Internally, a TQTable is used to create both html and tex versions of the table.

  Usage:
  ---------------
  ```
  +ExtractParameterTable {
  	+Morphing{
      <output="paramvals", showErrors=false>
      <allNPs=true, allPOIs=true>
      $for(i,0,100){
       +SnSh_AllVars_toyData_$(i);
      }
    }
  }
  ```
</cafdoc> */

namespace TSBaseActions {
    
  class ExtractParameterTable : public TSStatisticsManager::Action {
  
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
			

      const RooArgSet* nps (mc->GetNuisanceParameters());
      const RooArgSet* pois(mc->GetParametersOfInterest());

      bool includeNPs = config->getTagBoolDefault("allNPs",false);
      bool includePOIs = config->getTagBoolDefault("allNPs",true);

      bool showErrors = config->getTagBoolDefault("showErrors",false);

      RooArgSet allVars;
      if(pois && includePOIs)
        allVars.add(*pois);
      if(nps && includeNPs)
        allVars.add(*nps);

      TString outbase;
      if(!config->getTagString("output",outbase)){
        manager->error("no 'output' given!");
        return false;
      }
      
      TQTable* table = new TQTable("table");

      TQFolderIterator itr(config->getListOfFolders("?"));
      int ok = 0;
      while(itr.hasNext()){
        TQFolder* f = itr.readNext();
        if(!f) continue;

        TString snapshot = f->getTagStringDefault("snapshot",f->GetName());

        if(!ws->loadSnapshot(snapshot)){
          manager->error(TString::Format("unable to load snapshot '%s'",snapshot.Data()));
          continue;
        } 

        table->setEntry(0,ok+1,snapshot);
    
        manager->info(TString::Format("  extracting parameter values from snapshot '%s'",snapshot.Data()));        

        int iNP=1;
        RooAbsArg* obj;
        RooFIter itr(allVars.fwdIterator());
        while((obj = itr.next())){
          RooRealVar* np = dynamic_cast<RooRealVar*>(obj);
          
          table->setEntry(iNP,0,np->GetName());
          if(showErrors)
            table->setEntryValueAndUncertainty(iNP,ok+1,np->getVal(),np->getError());
          else
            table->setEntryValue(iNP,ok+1,np->getVal());
          iNP++;
        }
        ok++;
      }

      manager->info("writing "+outbase);
      table->shrink();
      table->writeLaTeX(outbase+".tex");
      table->writeHTML(outbase+".html");

      return (ok>0);
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ExtractParameterTable(),"ExtractParameterTable");
  }
}
