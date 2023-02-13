#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "SFramework/TSStatisticsManager.h"

#include "SFramework/TSSignificanceCalculator.h"

/*<cafdoc name=CalculateSignificance>
  CalculateSignificance
  ===========================
  
  Perform a hypothesis test using the asymptotic-formulae approach
  (https://arxiv.org/abs/1007.1727), calculating the significiance of
  a discovery.

  The result of the calculation will be stored in the results folder
  and printed to the console.

  Usage:
  ---------------
  ```
  +CalculateSignificance.myCalc {
    # The name of the object to be written can be set as the suffix of
    #  the action name, or with a `name` tag.
    +Test{
      +asimov {
        <dataset="asimovData">
        <fit.logToFile="./signif_asimov.log">
      }    
      +observed {
        # the "blinded=true" flag will calculate the result, but not print or store it
        <blinded=false>
        <dataset="obsData">
        <fit.logToFile="./signif_observed.log">
      }
      <fit.runHesse = true, fit.verbose=true> @ ?;
    }
  }
  ```
</cafdoc> */

namespace TSBaseActions {

  class ActionCalculateSignificance : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("no such workspace available: '%s'",config->GetName()));
        return false;
      }
      
      TQFolder * result = results()->getFolder(TString::Format("%s+",config->GetName()));

      TSSignificanceCalculator sc(workspace);

      TQFolderIterator itr(config->getListOfFolders("?"));
      while(itr.hasNext()){
        TQFolder* cfg = itr.readNext();
        manager->info(TString::Format("Calculating significance '%s' on workspace '%s'",cfg->GetName(),config->GetName()));
        
        TString name = cfg->getTagStringDefault("name",cfg->GetName());

        TQFolder* signif = sc.runCalculation(cfg);
        if (signif){
          signif->SetName(name);
          if (config->getTagBoolDefault("doConditional",true)) result->getFolder("Significance+")->addObject(signif);
          else result->getFolder("MainFit+")->addObject(signif);
        } else {
          manager->error(TString::Format("%s: Significance calculation failed",config->GetName()));
        }
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ActionCalculateSignificance(),"CalculateSignificance");
  }
}

