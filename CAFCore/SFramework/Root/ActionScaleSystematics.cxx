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
#include "RooAbsReal.h"
#include "RooFitResult.h"
#include "RooAddition.h"
#include "RooSimultaneous.h"
#include "RooPoisson.h"
#include "RooConstVar.h"
#include "RooHistFunc.h"
#include "RooDataHist.h"
#include "RooStats/HistFactory/PiecewiseInterpolation.h"
#include "RooStats/HistFactory/FlexibleInterpVar.h"
#include "RooStats/HistFactory/ParamHistFunc.h"
#include "RooAbsDataStore.h"

#include "TFile.h"

#include "RooCmdConfig.h"

#include <map>
#include <utility>

/*<cafdoc name=ScaleSystematics>
  ScaleSystematics
  ===========================
  
  This action scales the effect of certain parameters (systematic uncertainties)
  in a workspace by an arbitrary factor.
  
  The Action scales the effect of all (matching) components of 
  `PiecewiseInterpolation` and `RooStats::HistFactory::FlexibleInterpVar`
  instances in a workspace/pdf.
  
  Note: This has been designed to work with workspaces like the HWW one when
  created using `SFramework`/`HistFactory`.
  
  
  Usage:
  ---------------
  ```
  +ScaleSystematics{
    +NameOfWorkspace {
      <pdf = "nameOfPdfToEdit"> #defaults to "simPdf"
      +someParameter{
        <name = "expression*ToMatch", scale = 0.5, invert=false, sqrt=false> 
        #scale the effects of parameter(s) with names matching the expression given as 
        #'name' by the given scaling factor. If 'invert' is true, yields
        #are scaled by 1/'scale' instead (default is false).
        #if 'sqrt' is true the sqrt of the scaling factor is used (or its 
        #inverse)
      }
      +otherSyst {
        <scale = 3, invert=true> 
        #if 'name' is not specified, the name of the folder is used (here:
        # "otherProcess")
      }
    }
  }
  ```
  The process name is matched against the name of the relevant `RooAbsArg`
  instances. If their names are not known, try printing a workspace in
  cling (`root` shell): `ws->Print("t")`

</cafdoc> */


namespace TSBaseActions {
    
  class ScaleSystematics : public TSStatisticsManager::Action {
    bool doApply(const char* name, const TString& filter, const TString& veto) const {
      return TQStringUtils::matchesFilter(name, filter,",") && !TQStringUtils::matchesFilter(name,veto,",");
    }
    
    
    void processPiecewiseInterpolation(const PiecewiseInterpolation* pw, const TString& filter, const TString& veto, const double scale) const {
      RooAbsArg* nominalAbs = TSUtils::getNominalMember(pw);
      if (!nominalAbs) return;
      RooListProxy* paramSet = TSUtils::getParamSetMember(pw);
      if (!paramSet) return;
      RooListProxy* highSet = TSUtils::getHighSetMember(pw);
      if (!highSet) return;
      RooListProxy* lowSet = TSUtils::getLowSetMember(pw);
      if (!lowSet) return;
      
      RooFIter paramIter(paramSet->fwdIterator()) ;
      RooFIter highIter(highSet->fwdIterator()) ;
      RooFIter lowIter(lowSet->fwdIterator()) ;
      
      RooAbsArg* parAbs = nullptr;
      RooAbsArg* highAbs = nullptr;
      RooAbsArg* lowAbs = nullptr;
      
      RooHistFunc* nominalHist = dynamic_cast<RooHistFunc*>(nominalAbs);
      
      while((parAbs=paramIter.next())) {
        lowAbs = lowIter.next() ;
        highAbs = highIter.next() ;
        if (!doApply(parAbs->GetName(), filter, veto)) continue; //only process if this parameter matches our filter expression
        std::cout<<"Scaling PiecewiseInterpolation component '"<<parAbs->GetName()<<"'"<<std::endl;
        //assumption: this construct only appears in use with RooHistFunc s, other cases would still need to be implemented
        if (nominalHist) {
          RooHistFunc* highHist = dynamic_cast<RooHistFunc*>(highAbs);
          RooHistFunc* lowHist = dynamic_cast<RooHistFunc*>(lowAbs);
          if (!highHist || !lowHist) { //this should never happen as it would mean that nominal and variations are different types of objects....
            throw std::runtime_error(TString::Format("Central value (nominal) '%s' is a RooHistFunc but high and/or low variations ('%s', '%s') are not... It seems your workspace has some severe issues!",nominalHist->GetName(), highAbs->GetName(), lowAbs->GetName()).Data());
          }
          TSUtils::scaleDifferenceToNominal(TSUtils::getDataHist(highHist),TSUtils::getDataHist(nominalHist),scale);
          TSUtils::scaleDifferenceToNominal(TSUtils::getDataHist(lowHist),TSUtils::getDataHist(nominalHist),scale);
        } else { //to support other data types, add treatment here (in addition to attempted dynamic_casts above):
          std::cout<<"Encountered unsuported type of PiecewiseInterpolation component: '"<<nominalAbs->GetName()<<"' is of type '"<<nominalAbs->IsA()->GetName()<<"' instance"<<std::endl;
        }
        
      }
    }
    
    void processFlexibleInterpVar(const RooStats::HistFactory::FlexibleInterpVar* interp, const TString& filter, const TString& veto, const double scale) const {
      if (!interp) return;
      Double_t nominal = TSUtils::getNominalMember(interp);
      std::vector<double>* high = TSUtils::getHighMember(interp);
      std::vector<double>* low = TSUtils::getLowMember(interp);
      RooListProxy* paramList = TSUtils::getParamListMember(interp);
      if (!high || !low || !paramList) {
        throw std::runtime_error(TString::Format("Some members required for processing of FlexibleInterpVar '%s' could not be retrieved, please check your workspace/pdf!",interp->GetName()).Data());
        return;
      }
      if (high->size() != low->size()) {
        throw std::runtime_error(TString::Format("Low and High variation vectors of FlexibleInterpVar '%s' are of different length, please check your workspace/pdf!",interp->GetName()).Data());
        return;
      }
      TIterator* paramItr = paramList->createIterator();
      RooAbsArg* param = nullptr;
      
      for (size_t i=0; i<high->size(); ++i) {
        if (! (param=(RooAbsArg*)paramItr->Next() ) ) {
          throw std::runtime_error(TString::Format("Length of Low/High variation vectors and corresponding parameter list of FlexibleInterpVar '%s' are inconsistent, please check your workspace/pdf!",interp->GetName()).Data());
          if (paramItr) delete paramItr;
          paramItr = nullptr;
          return;
        }
        //ignore this high/low combination if the parameter does not match our selection criteria
        if (!doApply(param->GetName(), filter, veto)) continue;
        
        std::cout<<"Scaling effect of FlexibleInterpVar parameter '"<<param->GetName()<<"' by factor "<<scale<<std::endl;
        //apply the actual scaling
        high->at(i) = nominal + scale * (high->at(i)-nominal);
        low->at(i) = nominal + scale * (low->at(i)-nominal); 
      }
      
      if (paramItr) delete paramItr;
      paramItr = nullptr;
      
    }
    
    bool execute(TQFolder * config) const override {
      if(!config){
        throw std::runtime_error("received NULL pointer as config object!");
      }
      //retrieve workspace
      RooWorkspace * ws = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!ws){
        manager->error(TString::Format("unable to load workspace '%s': no such workspace!",config->GetName()));
        return false;
      }
      //ws->obj("ModelConfig")->Print("t");
      //retrieve pdf to edit
      TString pdfName = config->getTagStringDefault("pdfName","simPdf");
      RooAbsPdf* pdf = ws->pdf(pdfName);
      if (!pdf){
        manager->error(TString::Format("unable to retrieve pdf '%s' from workspace '%s': no such pdf!",pdfName.Data(),config->GetName()));
        return false;
      }
      
      /*
      RooSimultaneous* simpdf = dynamic_cast<RooSimultaneous*>(pdf);
      if (!simpdf) {
        manager->error(TString::Format("unable to cast pdf '%s' to RooSumultaneous: your workspace '%s' seems to have a structure that is not (yet) supported!",pdfName.Data(),config->GetName()));
        return false;
      }
      std::vector<RooHistFunc*>* vhf = RooSimultaneousHelper::getAllDataHists(simpdf);
      */
      
      //this is a "manual" hack to get all nominal RooHistFuncs
      //std::set<const RooAbsArg*>* allHistFuncs = TSUtils::getContributionsRecursive(simpdf, RooHistFunc::Class(), false);
      std::set<const RooAbsArg*>* allPiecewise = TSUtils::getContributionsRecursive(pdf, PiecewiseInterpolation::Class(), false);
      for (const auto* pw : *allPiecewise) {
        if (!pw) continue;
        pw->Print();
      }
      std::set<const RooAbsArg*>* allFlexInterp = TSUtils::getContributionsRecursive(pdf, RooStats::HistFactory::FlexibleInterpVar::Class(), false);
      for (const auto* pw : *allFlexInterp) {
        if (!pw) continue;
        pw->Print();
      }
      
      
      TQFolderIterator operationsItr(config->getListOfFolders("?"),true); //retrieve the individual scaling operations we should perform
      while (operationsItr.hasNext()) {
        TQFolder* op = operationsItr.readNext();
        //read some basic options for this part of the payload
        //@tag: [name] Possibly comma separated list of (wildcarded) expressions the name of a systematic (nuissance parameter) needs to match for its effects to be modified. Defaults to name of the folder it is read from.
        TString pattern = op->getTagStringDefault("name",op->GetName());
        //@tag: [exclude] If the name of a systematic (nuissance parameter) matches this (possibly comma separated list of) expression(s) it is not modified even if it matches the 'name' pattern(s). Default: ''
        TString vetoPattern = op->getTagStringDefault("exclude","");
        //@tag: [scale] Value by which the effect of the selected systematics should be changed. Default: 1.0
        double scale = op->getTagDoubleDefault("scale",1.);
        //@tag: [invert] If set to true will multiply effects by '1/scale' instead of multiplying them by 'scale'
        if (scale>0 && op->getTagBoolDefault("invert",false)) {
          scale = 1./scale;
          std::cout<<"inverting scale factor to "<<scale<<std::endl;
        }
        //@tag: [sqrt] If set the effect of selected parameters is multiplied by 'sqrt(scale)' instead of 'scale' (can be used together with 'invert')
        if (op->getTagBoolDefault("sqrt",false)) {
          scale = sqrt(scale);
          std::cout<<"will scale by sqrt(scale) = "<<scale<<std::endl;
        }
        
        for (const RooAbsArg* pw_ : *allPiecewise) {
          if (!pw_) continue;
          const PiecewiseInterpolation* pw = dynamic_cast<const PiecewiseInterpolation*>(pw_);
          if (!pw) {
            std::cout<<"Skipping object '"<<pw_->GetName()<<"': not a PiecewiseInterpolation"<<std::endl;
            continue;
          }
          processPiecewiseInterpolation(pw, pattern, vetoPattern, scale);
        }
        for (const RooAbsArg* pw_ : *allFlexInterp) {
          if (!pw_) continue;
          const RooStats::HistFactory::FlexibleInterpVar* pw = dynamic_cast<const RooStats::HistFactory::FlexibleInterpVar*>(pw_);
          if (!pw) {
            std::cout<<"Skipping object '"<<pw_->GetName()<<"': not a FlexibleInterpVar"<<std::endl;
            continue;
          }
          processFlexibleInterpVar(pw, pattern, vetoPattern, scale);
        }
      }
      
      //delete allHistFuncs;
      //ws->loadSnapshot("SnSh_AllVars_Nominal");
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ScaleSystematics(),"ScaleSystematics");
  }
}
