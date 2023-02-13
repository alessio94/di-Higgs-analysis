#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSImpactCalculator.h"
/*<cafdoc name=CalculateImpacts>
  CalculateImpacts
  ===========================
  
  This action performs an impact calculation, computing the impacts
  of individual systematic uncertainty parameters on your POI.
  
  There are two variations of the computation, the "normal" and the
  "inverted" one.
    * In the "normal" way, three fits are run for each systematic
      uncertainty: a fully unconditional one with all NPs floating,
      and two that are partially conditional on this uncertainty (with
      the respective NPs fixed to their nominal values plus or minus 1
      sigma). Then, the difference between the post-fit values of the
      POI between the two fits are computed.
    * The "inverted" calculation works the same way, but fixes all the
      other NPs to the nominal values instead of letting them float.

  Usage:
  ---------------
  ```

  +CalculateImpacts {
	  +Morphing {
      # compute impacts for the fit to asimov
      +asimov { 
        <datasetName="asimovData_SM">
        <invert=false>
      }   
      +asimovinvert {
        <datasetName="asimovData_SM">
        <invert=true>
      }   

      @ ? {
        # the "singles" will compute the breakdown for each individual NP matching the filter
        <singles.SYS = "alpha_ATLAS_*">
        <singles.THEO = "alpha_theo*">                                                                                                            
        <singles.MCSTAT = "gamma_*">
        <singles.NORM = "ATLAS_norm*">
        <singles.FAKE = "HWW_FakeFactor_*"> 
      }
      <fit.startingStrategy=1,fit.printLevel=1,fit.numRetry=3, fit.offset=true, fit.verbose=1> @ ?;
    }
  }
  ```

  As this performs a series of fits, it can take quite long to
  run. Hence, there is a batch-parallelized version of this action,
  `WriteCalculateImpacts`.

  A different method of assessing the effects of different
  systematical uncertainties also exists, see `CalculateBreakdown`.

</cafdoc> */
namespace TSBaseActions {

  class ActionCalculateImpacts : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("no such workspace available: '%s'",config->GetName()));
        return false;
      }
      
      TQFolder * result = results()->getFolder(TString::Format("%s+",config->GetName()));

      TSImpactCalculator im(workspace,result->getFolder("Snapshots+"));

      TQFolderIterator itr(config->getListOfFolders("?"));
      while(itr.hasNext()){
        TQFolder* cfg = itr.readNext();
        manager->info(TString::Format("Calculating impacts '%s' on workspace '%s'",cfg->GetName(),config->GetName()));
          
        TQFolder* impacts = im.runCalculation(cfg);
        if (impacts){
          result->getFolder("Impacts+")->addObject(impacts);
        } else {
          manager->error(TString::Format("%s: Impact calculation failed",config->GetName()));
        }
        //@tag: [trimResult] If this tag is set to true only the breakdown results are stored but not the full fit results (correlation/covariance matrices, initial and final parameter values, ...)
        if (cfg->getTagBoolDefault("~trimResult",false)) {
          result->deleteObject(TString::Format("Impacts/%s/FitResults!",cfg->GetName() )); //clean-up to reduce the size of individual result files, e.g., in case of a batch breakdown
        }
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ActionCalculateImpacts(),"CalculateImpacts");
  }
}


    
