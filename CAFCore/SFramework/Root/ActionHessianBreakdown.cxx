#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "SFramework/TSStatisticsManager.h"

/*<cafdoc name=HessianBreakdown>
  HessianBreakdown
  ===========================
  
  The ranking of nuisance parameters can be measured in various ways,
  see `ActionCalculateImpacts` and `ActionCalculateBreakdowns` for
  more details. However, all of these options are very expensive in
  terms of computing-power.

  This action provides a shortcut and allows to make breakdowns very
  quickly, as it estimates the breakdown of systematic uncertainties
  solely based on the correlation matrix. It can thus be used to
  generate a breakdown from any fit results that includes a
  hesse-matrix, covariance-matrix or correlation-matrix and does not
  require running any additional fits.

  However, this technique assumes symmetric, gaussian uncertainties on
  all parameters, and so is often rather inaccurate in realistic
  scenarios.

  Usage:
  ---------------
  ```

  +HessianBreakdown.observed {
    +combined{
      <fitresult="Significance/FitResults/obsData_muhat">
      <parameter="mu">
      <target="UncertaintyBreakdown/HessianBreakdownData/mu">
    }
  }

  ```

  This action adds a section to the result under the path specified as
  `target` that contains all the information needed to make a
  breakdown plot using the `PlotResults` action.

</cafdoc> */


namespace TSBaseActions {

  class HessianBreakdown : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      TQFolder* result = results()->getFolder(config->GetName());
      if(!result){
        manager->error(TString::Format("no such result available: '%s'",config->GetName()));
        return false;
      }

      TString path;
      if(!config->getTagString("fitresult",path)){
        manager->error("you have to give a path via the 'fitresult' option!");
        return false;
      }
      
      TQFolder* fit = result->getFolder(path);
      if(!fit){
        manager->error(TString::Format("unable to retrieve fitresult '%s'",path.Data()));
        return false;
      } 

      TString hessename = config->getTagStringDefault("object","hesse");
      TH2* hesse = dynamic_cast<TH2*>(fit->getObject(hessename));
      
      if(!hesse){
        manager->error(TString::Format("no hesse matrix '%s' found in '%s'",hessename.Data(),path.Data()));
        return false;
      }
     
      
      TString parameter;
      if(!config->getTagString("parameter",parameter)){
        manager->error("you have to give a parameter name via the 'parameter' option!");
        return false;
      }

      TAxis* axis = hesse->GetXaxis();
      const int np = axis->GetNbins();
      const int poi = axis->FindBin(parameter);

      if(poi > np){
        manager->error(TString::Format("invalid POI: no parameter '%s' found in '%s/%s'",parameter.Data(),path.Data(),hessename.Data()));
        return false;
      } 
      
      manager->info(TString::Format("reading parameter '%s' @ bin %d from hessian matrix at '%s/%s'",parameter.Data(),poi,path.Data(),hessename.Data()));
      
      TString targetpath = config->getTagStringDefault("target",TQFolder::concatPaths("UncertaintyBreakdown","Breakdown",parameter))+"+";
      manager->info(TString::Format("writing breakdown to '%s'",targetpath.Data()));
      TQFolder* target = result->getFolder(targetpath);
      target->setTagBool("IsHessian",true);

      bool verbose = false;

      bool doRel = config->getTagBoolDefault("relative",true);
      
      for(int i=0; i<np; ++i){
        TString name(axis->GetBinLabel(i+1));
        if(name.IsNull()) continue;
        int bincov = hesse->GetBin(i+1,poi);
        int binpoi = hesse->GetBin(poi,poi);
        int binnp = hesse->GetBin(i+1,i+1);
        double valcov = fabs(hesse->GetBinContent(bincov));
        double valpoi = fabs(hesse->GetBinContent(binpoi));
        double valnp = fabs(hesse->GetBinContent(binnp));
        TQFolder* f_breakdown = target->getFolder(TQFolder::makeValidIdentifier(name)+"+");
        double value = valcov/sqrt(valnp);
        if(doRel) value /= sqrt(valpoi);
        if(!f_breakdown){
          manager->error(TString::Format("unable to create entry for parameter '%s' at bin index '%d'/'%d'",name.Data(),i+1,np));
          continue;
        }
        if(verbose) manager->info(TString::Format("setting parameter '%s' impact to '%g' (cov=%g, vpar=%g, vpoi=%g)",name.Data(),value,valcov,valnp,valpoi));

        f_breakdown->setTagDouble("Avg", value);
        f_breakdown->setTagDouble("Low", value);
        f_breakdown->setTagDouble("High", value);
        f_breakdown->setTagDouble("Span", 2*value);
        f_breakdown->setTagDouble("Avg_Rel", value);
        f_breakdown->setTagDouble("Low_Rel", value);
        f_breakdown->setTagDouble("High_Rel", value);
        f_breakdown->setTagDouble("Span_Rel", 2*value);
      }
      
      return true;
    }

  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new HessianBreakdown(),"HessianBreakdown");
  }
}


