#include "QFramework/TQFolder.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQIterator.h"
#include "SFramework/TSStatisticsManager.h"

#include "SFramework/TSContourScanner.h"

/*<cafdoc name=ScanContours>
  ScanContours
  ===========================
  
  This action performs a contour scan over a 2D space, employing the
  Minuit `contour` method.
  
  Usage:
  ---------------
  ```
  +ScanContours.cont2D{
    +test{
      <datasetName="modelData">
      <x="frac",y="sigma_g2">
      <n=100>
    }
  }

  ```

  As this performs a series of fits, it can take quite long to
  run. Since the `contour` method is iterative and cannot easily be
  parallelized, the parallelizable scan of the full parameter space
  using `WriteScanLikelihood` is usually more efficient.

</cafdoc> */

namespace TSBaseActions {

  class ActionScanContours : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("no such workspace available: '%s'",config->GetName()));
        return false;
      }
      
      TString scanname(config->getBase()->GetName());
      config->getTagString("name",scanname);
      TQFolder * result = results()->getFolder(config->getName()+"+");
      
      TSContourScanner lhs(workspace,result->getFolder("Snapshots+"));
      
      manager->info(TString::Format("Performing likelihood scan '%s' on workspace '%s'",config->GetName(),config->GetName()));
      TString logfile;
      if(config->getTagString("fit.logToFile",logfile)){
        TList* l = TQUtils::execute("rm "+logfile);
        delete l;
      }
      TQFolder* calc = lhs.runCalculation(config);
      if (calc){
        calc->SetName(scanname);
        result->getFolder("ContourScans+")->addObject(calc);
      } else {
        manager->error(TString::Format("%s: Scanning contours failed", config->GetName()));
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ActionScanContours(),"ScanContours");
  }
}

