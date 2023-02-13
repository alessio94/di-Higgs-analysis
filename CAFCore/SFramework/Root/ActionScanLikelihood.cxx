#include "QFramework/TQFolder.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQIterator.h"
#include "SFramework/TSStatisticsManager.h"

#include "SFramework/TSLikelihoodScanner.h"

/*<cafdoc name=ScanLikelihood>
  ScanLikelihood
  ===========================
  
  This action performs a likelihood scan, looping over one or more
  parameters and evaluating a grid of points.
  
  
  Usage:
  ---------------
  ```
  +ScanLikelihood.mu{
  	+Test{
  		<fit.logToFile="./scan.log"> # write the fit logs to this file
  		<datasetName="asimovData"> # fit to asimov
  		+mu{ # scan parameter mu with 60 points in the range -5 to 5
  			<min=-5,max=5, nbins=60>
  		}
  	}
  }

  +ScanLikelihood.2D{
  	+Morphing{
  		<datasetName="asimovData_SM"> # fit to SM asimov
      <snapshot="SnSh_AllVars_Nominal"> # start from the nominal snapshot
      <saveFullSnapshots=true> # save full snapshots of every fit in the workspace
      <storeFullResults=true> # store the fit-results of all parameters for every point
      <crossCheckMinima=true> # check for inconsistencies in the Nll shape after fitting and refit points if need be
  		<fit.runMinos = true> # run Minos for every point
      <fit.runHesse = true> # run Hesse for every point
  		+AT{ # first parameter to scan: AT coupling, 20 points in the range 0 to 2
  			<min=0.,max=2, nbins=20>
  		}
  		+AL{ # second parameter to scan: AL coupling, 16 points in the range 0.8 to 1.2
  			<min=0.8,max=1.2, nbins=16>
  		}
  	}
  }

  ```

  As this performs a series of fits, it can take quite long to
  run. Hence, there is a batch-parallelized version of this action,
  `WriteScanLikelihood`.

</cafdoc> */


namespace TSBaseActions {

  class ActionScanLikelihood : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("no such workspace available: '%s'",config->GetName()));
        return false;
      }
      
      TString scanname(config->getBase()->GetName());
      config->getTagString("name",scanname);
      TQFolder * result = results()->getFolder(config->getName()+"+");
      
      TSLikelihoodScanner lhs(workspace,result->getFolder("Snapshots+"));
      
      manager->info(TString::Format("Performing likelihood scan '%s' on workspace '%s'",config->GetName(),config->GetName()));
      TString logfile;
      if(config->getTagString("fit.logToFile",logfile)){
        TList* l = TQUtils::execute("rm -f "+logfile);
        delete l;
      }
      TQFolder* calc = lhs.runCalculation(config);
			if (calc){
        calc->SetName(scanname);
        result->getFolder("LikelihoodScans+")->addObject(calc);
      } else {
				manager->error(TString::Format("%s: Scanning likelihood function failed", config->GetName()));
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ActionScanLikelihood(),"ScanLikelihood");
  }
}

