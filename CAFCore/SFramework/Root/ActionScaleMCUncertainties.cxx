#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQHistogramUtils.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSUtils.h"
#include "SFramework/TSHistogramExporter.h"

#include "RooStats/ModelConfig.h"

#include "RooCategory.h"
#include "RooBinning.h"
#include "RooCatType.h"
#include "RooDataSet.h"
#include "RooRealSumPdf.h"
#include "RooProdPdf.h"
#include "RooArgSet.h"
#include "RooProduct.h"
#include "RooRealVar.h"
#include "RooFitResult.h"
#include "RooAddition.h"
#include "RooSimultaneous.h"
#include "RooPoisson.h"
#include "RooGaussian.h"
#include "RooConstVar.h"

#include "TFile.h"

#include "RooCmdConfig.h"

#include <map>
#include <utility>

/*<cafdoc name=ScaleMCUncertainties>
  ScaleMCUncertainties
  ===========================

  This action scales all MC uncertainties in a workspace by an arbitrary factor.
  It assumes that these relevant parameters are named as they are named and
  structured as in case the workspace was created using HistFactory:
    - The names of the RooPoisson terms (Gaussian terms should work but have
      not been extensively tested yet!) match "gamma_stat_*"
    - Their parameters 'x' and 'mean' are composed as follows:
      - x : a RooRealVar parameter, nominal value 1/sigma^2
      - mean: a RooProduct containing a RooConstVar ('tau') that represents the
              scaling of another ("normalized") RooRealVar ('gamma') to match
              a poisson distribution.
  Note that this action will also modify all snapshots in the workspace! The
  values of all parameters matching "nom_gamma_stat*" are scaled by the same
  value as the modified parameters above if they can be associated with a
  RooPoisson constraint. This is needed to make sure when loading the snapshot
  to perform a fit the likelihood doesn't start at extreme values that would
  instantly cause the fit to fail (the LH parameters for the MC stat. NPs would
  typically be many sigma off their mean value!)

  Usage:
  ---------------
  ```
  +ScaleMCUncertainties{
    +NameOfWorkspace {
      #reduce MC stat. uncertainties as if 10 times more (raw) MC events were
      #available: sigma -> sigma/sqrt(10)
      <scale=10.0, invert=true, sqrt=true> #defautl values for 'invert' and 'sqrt'
      #other example:
      #increase MC stat. uncertainties by a factor 2 (sigma-> 2*sigma):
      #<scale = 2.0, invert=false, sqrt=false>
    }
  }
  ```


</cafdoc>*/


namespace {
  // hack to get access to RooPoisson::_CM
  template <typename RooPoissonTag>
  struct RooPoissonHackResult {
    typedef typename RooPoissonTag::thetype thetype;
    static thetype ptr;
  };

  template <typename RooPoissonTag>
  typename RooPoissonHackResult<RooPoissonTag>::thetype RooPoissonHackResult<RooPoissonTag>::ptr;

  template<typename RooPoissonTag, typename RooPoissonTag::thetype p>
  struct RooPoissonRob : RooPoissonHackResult<RooPoissonTag> {
    struct RooPoissonFiller {
      RooPoissonFiller() {RooPoissonHackResult<RooPoissonTag>::ptr = p;}
    };
    static RooPoissonFiller roopoissonfiller_obj;
  };

  template<typename RooPoissonTag, typename RooPoissonTag::thetype p>
  typename RooPoissonRob<RooPoissonTag, p>::RooPoissonFiller RooPoissonRob<RooPoissonTag, p>::roopoissonfiller_obj;

  //now expose some members of RooPoisson that we need to access
  struct RooPoissonMean { typedef RooRealProxy(RooPoisson::*thetype); };
  template class RooPoissonRob<RooPoissonMean, &RooPoisson::mean>;
  //now expose some members of RooPoisson that we need to access
  struct RooPoissonX { typedef RooRealProxy(RooPoisson::*thetype); };
  template class RooPoissonRob<RooPoissonX, &RooPoisson::x>;

  template <typename RooConstVarTag>
  struct RooConstVarHackResult {
    typedef typename RooConstVarTag::thetype thetype;
    static thetype ptr;
  };

  template <typename RooConstVarTag>
  typename RooConstVarHackResult<RooConstVarTag>::thetype RooConstVarHackResult<RooConstVarTag>::ptr;

  template<typename RooConstVarTag, typename RooConstVarTag::thetype p>
  struct RooConstVarRob : RooConstVarHackResult<RooConstVarTag> {
    struct RooConstVarFiller {
      RooConstVarFiller() {RooConstVarHackResult<RooConstVarTag>::ptr = p;}
    };
    static RooConstVarFiller rooconstvarfiller_obj;
  };

  template<typename RooConstVarTag, typename RooConstVarTag::thetype p>
  typename RooConstVarRob<RooConstVarTag, p>::RooConstVarFiller RooConstVarRob<RooConstVarTag, p>::rooconstvarfiller_obj;

  //now expose some members of RooConstVar that we need to access
#if ROOT_VERSION_CODE < ROOT_VERSION(6,23,0)
  struct RooConstVar_value { typedef Double_t(RooConstVar::*thetype); };
  template class RooConstVarRob<::RooConstVar_value, &RooConstVar::_value>;
#else
  struct RooConstVar_value { typedef Double_t(RooAbsReal::*thetype); };
  template class RooConstVarRob<::RooConstVar_value, &RooAbsReal::_value>;
#endif
}




namespace {

  RooConstVar* getTauCVParameter(RooPoisson* pois){
    const RooRealProxy& px = pois->*RooPoissonHackResult<RooPoissonMean>::ptr;
    RooProduct* prod = const_cast<RooProduct*>(dynamic_cast<const RooProduct*>(&(px.arg())));
    //prod->Print();
    RooArgList compList(prod->components());
    RooFIter citr(compList.fwdIterator());
    RooAbsArg* arg;
    while((arg = citr.next())){
      RooConstVar* tau = dynamic_cast<RooConstVar*>(arg);
      if(tau /*&& TQStringUtils::matches(tau->GetName(), "gamma_*_bin_*_tau_*")*/ ) return tau;
    }
    return NULL;
  }

  RooRealVar* getTauRVParameter(RooPoisson* pois){
    const RooRealProxy& px = pois->*RooPoissonHackResult<RooPoissonX>::ptr;
    RooRealVar* tauRV = const_cast<RooRealVar*>(dynamic_cast<const RooRealVar*>(&(px.arg())));
    return tauRV;
  }


  //this is a helper class to access some protected members of RooGaussians
  class RooGaussianHelper : public RooGaussian {
    public:
    static void scaleConstraint(const RooGaussian* gauss_, double scale) {
      RooGaussianHelper* gauss = (RooGaussianHelper*)gauss_;
      RooConstVar* sigma = const_cast<RooConstVar*>(dynamic_cast<const RooConstVar*>(&(gauss->sigma.arg())));
      double targetVal = sigma->getVal()*scale;
      sigma->*RooConstVarHackResult<::RooConstVar_value>::ptr = targetVal;
      sigma->setValueDirty();
      if (sigma->getVal() != targetVal) {
        std::cout<<"Something went wrong manipulating the gaussian sigma's value!"<<std::endl;
      }
    }

  };


}

namespace TSBaseActions {

  class scaleMCUncertainties : public TSStatisticsManager::Action {
    bool hasClientOfType(const RooAbsArg* par, TClass* type) const {
      TIterator* clItr = par->clientIterator();
      TObject* cl;
      while ( (cl=clItr->Next()) ) {
        if (cl && cl->InheritsFrom(type)) return true;
      }
      return false;
    }

    void scaleUncertainties(RooWorkspace* ws, double scaleSq = 1.) const{
      // scales the MC stat uncertainties (often refered to as gamma's).
      // the scaling value is expected to be the squared ratio by which
      // the stat. uncertainty should be scaled!
      // Note: we don't actually scale the gamma parameter as that one does
      // not represent the uncertainty but only the corresponding deviation
      // from the central value!
      // For more insight, try to understand this segment:
      // https://root.cern.ch/doc/v612/HistoToWorkspaceFactoryFast_8cxx_source.html#l02622
      if (!ws) return;

      if (scaleSq<=0.) {
        manager->error(TString::Format("Cannot scale stat. uncertainties by factor <= 0 !"));
        return; //nothing to do here
      }
      RooArgSet pdfs (ws->allPdfs());
      RooFIter itr(pdfs.fwdIterator());
      RooAbsArg* obj = NULL;;
      manager->info(TString::Format("Modifying Poisson terms for MC stat. uncertainties..."));
      while((obj = itr.next())){
        RooGaussian* gauss = dynamic_cast<RooGaussian*>(obj);
        RooPoisson* pois = dynamic_cast<RooPoisson*>(obj);
        if (gauss) {
          if(!TQStringUtils::matches(gauss->GetName(),"gamma_stat_*")) continue; //yes, here we still look for 'gamma' as that's how we call the combined RooProduct
          RooGaussianHelper::scaleConstraint(gauss, std::sqrt(scaleSq));
        }
        else if(pois) {
          if(!TQStringUtils::matches(pois->GetName(),"gamma_stat_*")) continue; //yes, here we still look for 'gamma' as that's how we call the combined RooProduct

          RooConstVar* tauCV = ::getTauCVParameter(pois);
          if (!tauCV) {
            manager->error(TString::Format("failed to get tauCV component of RooPoisson object '%s'!",pois->GetName()));
          } else {
            double targetVal = tauCV->getVal()/scaleSq;
            tauCV->*RooConstVarHackResult<::RooConstVar_value>::ptr = targetVal;
            tauCV->setValueDirty();
            if (tauCV->getVal() != targetVal) {
              std::cout<<"Something went wrong manipulating tauCV's value!"<<std::endl;
            }
          }
          RooRealVar* tauRV = ::getTauRVParameter(pois);
          if (!tauRV) {
            manager->error(TString::Format("failed to get tauRV component of RooPoisson object '%s'!",pois->GetName()));
          } else {
            tauRV->setVal(tauRV->getVal()/scaleSq);
            tauRV->setValueDirty();
          }
        } else continue; //not really needed, but for clarity: skip this parameter if it's neither a Poisson nor a Gaussian term
      }

      manager->info(TString::Format("Patching snapshots to match scaled MC stat. uncertainty..."));
      //remove all existing snapshots, if we scale the stat. uncert. even just by a msall factor the likelihood when starting at existing parameter values very, very quickly becomes "inf"/ 1e30!!
      RooLinkedList snapshots = TSUtils::getListOfSnapshots(ws);
      /* ws->*RooWSHackResult<RooWSsnapshots>::ptr; */
      TObject* snapObj = nullptr;
      RooLinkedListIter snapItr = snapshots.iterator();
      while(( snapObj = snapItr.Next() )) { //iterate through snapshots
        RooArgSet* snap = dynamic_cast<RooArgSet*>(snapObj);
        if (!snap) continue;
        RooLinkedListIter parIter = snap->iterator();
        TObject* parObj = nullptr;
        while(( parObj = parIter.Next() )) {
          if (!parObj) continue;
          if (TQStringUtils::matches(parObj->GetName(), "nom_gamma_stat_*")) {
            RooRealVar* par = dynamic_cast<RooRealVar*>(parObj);
            if (!par) continue;
            RooAbsArg* parInWS = ws->var(par->GetName());
            if (!parInWS) {
              manager->warn(TString::Format("Found parameter '%s' in snapshot '%s' but no equivalent var in workspace '%s'. Will not adjust its value!",par->GetName(),snap->GetName(),ws->GetName()).Data());
              continue;
            }
            if (this->hasClientOfType(parInWS,RooPoisson::Class())) {
              std::cout<<"Scaling snapshot parameter called "<<par->GetName()<<std::endl;
              par->setVal(par->getVal()/scaleSq);
            } else {
              std::cout<<"snapshot parameter "<<par->GetName()<<" does not serve a poissonian constraint"<<std::endl;
            }
          }
        }
      }


    }


    bool execute(TQFolder * config) const override {
      if(!config){
        throw std::runtime_error("received NULL pointer as config object!");
      }

      RooWorkspace * ws = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!ws){
        manager->error(TString::Format("unable to load workspace '%s': no such workspace!",config->GetName()));
        return false;
      }
      double scaleSq = config->getTagDoubleDefault("scale",1.);
      //@tag: [sqrt] scale by sqrt(scale) instead of 'scale'. Default: true
      if (!config->getTagBoolDefault("sqrt",true)) {
        //what we'll actually do is scale the "tau" parameter which is pow(sigma,-2). Hence, we only consider the squared scaling factor
        scaleSq = pow(scaleSq,2);
      }
      //@tag: [invert] scale by the inverse of the specified scaling factor (or sqrt thereof). Default: true
      if (scaleSq>0 &&config->getTagBoolDefault("invert",true)) {
        scaleSq = 1./scaleSq;
      }
      scaleUncertainties(ws,scaleSq);

      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new scaleMCUncertainties(),"ScaleMCUncertainties");
  }
}
