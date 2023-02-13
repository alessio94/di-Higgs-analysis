#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "SFramework/TSStatisticsManager.h"

#include "SFramework/TSLimitCalculator.h"

/*<cafdoc name=CalculateLimit>
  CalculateLimit
  ===========================
  
  Perform a hypothesis test using the asymptotic-formulae approach
  (https://arxiv.org/abs/1007.1727), calculating the significiance of
  a discovery.

  The result of the calculation will be stored in the results folder
  and printed to the console.

  Usage:
  ---------------
  ```
  +CalculateLimit.myCalc {
    # The name of the object to be written can be set as the suffix of
    #  the action name, or with a `name` tag.
    +Test{
      +asimov {
        <dataset="asimovData">
        <oneSided=true>
        <useCLs=true>
        <confidenceLevel=.9>
        <nPoints=50>
        <min=0.>
        <max=6.>
        # define the null hypothesis
        +Parameters/H0 {
          <mu=0>
        }
      }    
    }
  }
  ```
</cafdoc> */

namespace TSBaseActions {

  class ActionCalculateLimit : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      TString wsname = config->getTagStringDefault("workspace",config->GetName());
      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(wsname));
      if(!workspace){
        manager->error(TString::Format("no such workspace available: '%s'",config->GetName()));
        return false;
      }
      
      TString resultname = config->getTagStringDefault("result",config->GetName());
      TQFolder * result = results()->getFolder(TString::Format("%s+",resultname.Data()));
      
      TSLimitCalculator lc(workspace);
      
      TQFolderIterator itr(config->getListOfFolders("?"));
      while(itr.hasNext()){
        TQFolder* cfg = itr.readNext();
        manager->info(TString::Format("Calculating limit '%s' on workspace '%s'",cfg->GetName(),config->GetName()));
        
        TString name = cfg->getTagStringDefault("name",cfg->GetName());
        
        TQFolder* limit = lc.runCalculation(cfg);
        if (limit){
          limit->SetName(name);
          result->getFolder("Limits+")->addObject(limit);
        } else {
          manager->error(TString::Format("%s: Limit calculation failed",config->GetName()));
        }
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ActionCalculateLimit(),"CalculateLimit");
  }
}

