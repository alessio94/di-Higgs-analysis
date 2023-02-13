#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "SFramework/TSStatisticsManager.h"

#include "SFramework/TSSignificanceCalculator.h"
#include "SFramework/TSBreakdownCalculator.h"
#include "SFramework/TSImpactCalculator.h"
#include "SFramework/TSFitter.h"

/*<cafdoc name=RunStatistics>
  RunStatistics
  ===========================
  
  This is a legacy action that is deprecated and should no longer be
  used. Please consider using a more specialized action for the
  statistical task you want to perform:
  * Likelihood scan
  * breakdown calculation
  * significance calculation
  * ...

</cafdoc> */

namespace TSBaseActions {

  class RunStatistics : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("no such workspace available: '%s'",config->GetName()));
        return false;
      }
      
      TQFolder * result = results()->getFolder(TString::Format("%s+",config->GetName()));
      
      manager->info(TString::Format("Running statistical analysis on workspace '%s'",config->GetName()));
      
      TQFolderIterator itrFits(config->getListOfFolders("?"), "Fit, Fit.*", true);
      while (itrFits.hasNext()) {
        TQFolder * fitConfig = itrFits.readNext();
        TString name = fitConfig->GetName();
        if (!TQStringUtils::removeLeadingText(name, "Fit.")) {
          name = ".default";
        }
        TSFitter fitter(workspace);
        if (!result->addObject(fitter.runCalculation(fitConfig),TString::Format("::%s",name.Data()))){
          manager->error(TString::Format("%s: Fitting failed", config->GetName()));
        }
      }

      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new RunStatistics(),"RunStatistics");
  }
}


    
