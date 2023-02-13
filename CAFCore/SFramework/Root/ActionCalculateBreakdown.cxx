#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "SFramework/TSStatisticsManager.h"

#include "SFramework/TSBreakdownCalculator.h"

/*<cafdoc name=CalculateBreakdown>
  CalculateBreakdown
  ===========================
  
  This action performs a breakdown calculation, computing a breakdown
  of systematical and statistical uncertainties on your POI by source of
  the uncertainty.
  
  There are two variations of the computation, the "normal" and the
  "inverted" one.
    * In the "normal" way, two fits are run for each systematic
      uncertainty: a fully unconditional one with all NPs floating, and one that is
      partially conditional on this uncertainty (with the respective
      NPs fixed to their nominal values). Then, a square-difference
      between the two fits of the uncertainty on your POI is computed.
    * The "inverted" calculation works the same way, but compares a
      fully conditional fit (with all NPs fixed to their nominal
      values) with a partially conditional one (with the NP in
      question floating and all other NPs fixed to their nominal
      values).

  Usage:
  ---------------
  ```

  +CalculateBreakdown {
	  +Morphing {
      # compute breakdown for the fit to asimov
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
        # the "group" will compute the breakdown for the entire group of NPs matching the filter
        <group.MCSTAT = "gamma_*"> 
        <group.SYS = "alpha_ATLAS_*", config.group.SYS.except = "ATLAS_norm*">    
        # since statistical uncertainties are not modelled explicitly, we compute them as the complement of all systematic unceratinies
        <group.STAT = "*", config.group.STAT.invert = true>                                                                                                 
      }
      <fit.startingStrategy=1,fit.printLevel=1,fit.numRetry=3, fit.offset=true, fit.verbose=1> @ ?;
    }
  }
  ```

  As this performs a series of fits, it can take quite long to
  run. Hence, there is a batch-parallelized version of this action,
  `WriteCalculateBreakdown` in ActionBatchCalculate.cxx.

  A different method of assessing the effects of different
  systematical uncertainties also exists, see `CalculateImpacts`.

</cafdoc> */
namespace TSBaseActions {

  class ActionCalculateBreakdown : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("no such workspace available: '%s'",config->GetName()));
        return false;
      }
      
      TQFolder * result = results()->getFolder(TString::Format("%s+",config->GetName()));

      TSBreakdownCalculator im(workspace,result->getFolder("Snapshots+"));

      TQFolderIterator itr(config->getListOfFolders("?"));
      while(itr.hasNext()){
        TQFolder* cfg = itr.readNext();
        manager->info(TString::Format("Calculating breakdown '%s' on workspace '%s'",cfg->GetName(),config->GetName()));
          
        TQFolder* breakdown = im.runCalculation(cfg);
        if (breakdown){
          result->getFolder("Breakdowns+")->addObject(breakdown);
        } else {
          manager->error(TString::Format("%s: Breakdown calculation failed",config->GetName()));
        }
        //@tag: [trimResult] If this tag is set to true only the breakdown results are stored but not the full fit results (correlation/covariance matrices, initial and final parameter values, ...)
        if (cfg->getTagBoolDefault("~trimResult",false)) {
          result->deleteObject(TString::Format("Breakdowns/%s/FitResults!",cfg->GetName() )); //clean-up to reduce the size of individual result files, e.g., in case of a batch breakdown
        }
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ActionCalculateBreakdown(),"CalculateBreakdown");
  }
}


    
