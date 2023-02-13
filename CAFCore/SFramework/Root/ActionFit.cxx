#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "SFramework/TSStatisticsManager.h"

#include "SFramework/TSFitter.h"

/*<cafdoc name=Fit>
  Fit
  ===========================
  
  Usage:
  ---------------
  ```
  +Fit.myFit {
    # The name of the object to be written can be set as the suffix of
    #  the action name, or with a `name` tag.
    +Test{
      <blinded=false>
      <dataset="obsData">
      <fit.logToFile="./signif_observed.log">
      <fit.runHesse = true, fit.verbose=true>
    }
  }
  ```
</cafdoc> */

namespace TSBaseActions {

  class ActionFit : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("no such workspace available: '%s'",config->GetName()));
        return false;
      }
      
      TSFitter fitter(workspace);

      TString name = config->getTagStringDefault("~.label",config->getBase()->GetName());
      if(name.IsNull()) name = "Fit";
      
      manager->info(TString::Format("Running  '%s' on workspace '%s'",name.Data(),workspace->GetName()));
      TQFolder* fitresult = fitter.runCalculation(config);
      if (fitresult){
	fitresult->SetName(name);
	TQFolder * result = results()->getFolder(TString::Format("%s/Fit+",config->GetName()));
	result->addFolder(fitresult);
      } else {
        manager->error(TString::Format("%s: Fit failed",config->GetName()));
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ActionFit(),"Fit");
  }
}

