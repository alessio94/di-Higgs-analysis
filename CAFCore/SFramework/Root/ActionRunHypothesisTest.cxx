#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "SFramework/TSStatisticsManager.h"

#include "SFramework/TSHypothesisTest.h"

/*<cafdoc name=RunHypothesisTest>
  RunHypothesisTest
  ===========================
  
  This action performs a hypothesis test.  
  
  Usage:
  ---------------
  ```
  +RunHypothesisTest {
    +Morphing {
      +HiggsCP {
        <nToys=200>
        <storeResults=true>
        <saveToys=true>
        <dataName="asimovData_1">
        +even {
          <title="CP-even">
          <pars.cosa=0.>
        }
        +odd {
          <title="CP-odd">
          <pars.cosa=1.>
        }
      }
    }
  }
  ```

</cafdoc> */

namespace TSBaseActions {

  class ActionRunHypothesisTest : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("no such workspace available: '%s'",config->GetName()));
        return false;
      }
      
      TQFolder * result = results()->getFolder(TString::Format("%s+",config->GetName()));

      TSHypothesisTest ht(workspace,result->getFolder("Snapshots+"));

      TQFolderIterator itr(config->getListOfFolders("?"));
      while(itr.hasNext()){
        TQFolder* cfg = itr.readNext();
        manager->info(TString::Format("Performing hypothesis test '%s' on workspace '%s'",cfg->GetName(),config->GetName()));
          
        TQFolder* hypotest = ht.runCalculation(cfg);
        if (hypotest){
          hypotest->SetName(cfg->GetName());
          result->getFolder("HypothesisTests+")->addObject(hypotest);
        } else {
          manager->error(TString::Format("%s: Hypothesis test failed",config->GetName()));
        }
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ActionRunHypothesisTest(),"RunHypothesisTest");
  }
}


    
