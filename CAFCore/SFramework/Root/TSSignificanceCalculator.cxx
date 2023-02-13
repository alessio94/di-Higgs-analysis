#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iterator>

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

#include "SFramework/TSSignificanceCalculator.h"

#include "TKey.h"
#include "RooNLLVar.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooCategory.h"
#include "RooFitResult.h"
#include "Math/ProbFuncMathCore.h"
#include "Math/QuantFuncMathCore.h"
#include "TStopwatch.h"
#include "TMath.h"
#include "Math/MinimizerOptions.h"

#include "QFramework/TQStringUtils.h"
#include "QFramework/TQUtils.h"
#include "SFramework/TSUtils.h"
#include "QFramework/TQIterator.h"

ClassImp(TSSignificanceCalculator)


//__________________________________________________________________________________|___________

TSSignificanceCalculator::TSSignificanceCalculator() : TSStatisticsCalculator("TSSignificanceCalculator") {
}


//__________________________________________________________________________________|___________

TSSignificanceCalculator::TSSignificanceCalculator(RooWorkspace * ws) : TSStatisticsCalculator("TSSignificanceCalculator",ws) {
}


//__________________________________________________________________________________|___________

TSSignificanceCalculator::~TSSignificanceCalculator() {
}


//__________________________________________________________________________________|___________

void TSSignificanceCalculator::info(TString message) {

  std::cout << "SFramework/TSSignificanceCalculator: " << message.Data() << std::endl;
}

//__________________________________________________________________________________|___________

TQFolder* TSSignificanceCalculator::runFit(TQFolder* result, RooAbsPdf* pdf, RooDataSet* data, const TString& fitid,RooArgSet& pois, RooArgSet& nuis, bool conditional, TQTaggable* fitOptions, bool save){
  TString snapshotName = fitOptions->getTagStringDefault("initSnapshot","SnSh_AllVars_Nominal");
  if(!fWorkspace->loadSnapshot(snapshotName)){
    error(TString::Format("unable to load snapshot '%s'",snapshotName.Data()));
    return NULL;
  }
  
  fitOptions->setTagString("id",fitid);
  
  info(TString::Format("runCalculation(): Running %s fit on %s",(conditional ? "conditional" : "unconditional"), data->GetName()));
  TQFolder* fitResult = fitPdfToData(pdf, data, &nuis, fitOptions);

  // save status after conditional fit as snapshot
  if(save){
    result->addObject(fitResult, "FitResults+ ::"+fitid);
    RooArgSet nuisAndPois(pois,nuis);
    fWorkspace->saveSnapshot(TString::Format("SnSh_NuisPOI_%s_%s",(conditional ? "Conditional" : "Unconditional"),fitid.Data()), nuisAndPois);
  }
 
  return fitResult;
}

#define nan std::numeric_limits<double>::quiet_NaN()

//__________________________________________________________________________________|___________

void TSSignificanceCalculator::makeSummary(TQFolder* target, TQFolder* result_constrained, TQFolder* result_unconstrained, const TString& label, RooArgSet& pois, bool blinded){
	if (!result_constrained || !result_unconstrained) return;
  int status_constrained = result_constrained->getTagIntegerDefault("status", -1);
  int strategy_constrained = result_constrained->getTagIntegerDefault("strategy", -1);
  int status_unconstrained = result_unconstrained->getTagIntegerDefault("status", -1);
  int strategy_unconstrained = result_unconstrained->getTagIntegerDefault("strategy", -1);
  bool ok = true;
  double min_constrained   = result_constrained->getTagDoubleDefault("minNll",nan);
  double min_unconstrained = result_unconstrained->getTagDoubleDefault("minNll",nan);
  int ndim_constrained = result_constrained->getTagIntegerDefault("nDim",-1);
  int ndim_unconstrained = result_unconstrained->getTagIntegerDefault("nDim",-1);
  
  if(status_constrained < 0 || !TQUtils::isNum(min_constrained)){
    error(TString::Format("fit error: constrained fit failed with status '%d' using strategy '%d' in %d dimensions, minimum was '%g'",status_constrained,strategy_constrained,ndim_constrained,min_constrained));
    ok=false;
  }
  if(status_unconstrained < 0 || !TQUtils::isNum(min_unconstrained)){
    error(TString::Format("fit error: unconstrained fit failed with status '%d' using strategy '%d' in %d dimensions, minimum was '%g'",status_unconstrained,strategy_unconstrained,ndim_constrained,min_unconstrained));
    ok=false;
  }

  Double_t q0 = 2 * (min_constrained - min_unconstrained);
  TQFolder* floatParsFinal = result_unconstrained->getFolder("floatParsFinal");
  if(floatParsFinal){
    RooAbsArg* obj;
    RooFIter itr(pois.fwdIterator());
    while((obj = itr.next())){
      TQFolder* f = floatParsFinal->getFolder(obj->GetName());
      if(f && !blinded){
        double value = f->getTagDoubleDefault("val",0);
        double errLow = f->getTagDoubleDefault("errLow",0);
        double errHigh = f->getTagDoubleDefault("errHigh",0);
        target->setTagDouble(TString::Format("%s_%s_val",obj->GetName(),label.Data()),value);
        target->setTagDouble(TString::Format("%s_%s_errLow",obj->GetName(),label.Data()),errLow);
        target->setTagDouble(TString::Format("%s_%s_errHigh",obj->GetName(),label.Data()),errHigh);
        if(value<0) q0 = 0;
      }
    }
  } else {
    ok = false;
  }
  if(ok){
    if(q0 >= 0){
      Double_t Z0 = TMath::Sqrt(q0);
      if(!blinded){
        target->setTagDouble(TString::Format("Z0_%s",label.Data()), Z0);
        target->setTagDouble(TString::Format("p0_%s",label.Data()), 1 - ROOT::Math::gaussian_cdf(Z0));
        info(TString::Format("%s. significance Z0 = %.1f, nll was %.3f (constrained, ndim=%d) vs %.3f (unconstrained, ndim=%d)",label.Data(), Z0, min_constrained, ndim_constrained, min_unconstrained, ndim_unconstrained));
      } else {
        warn(TString::Format("%s. fit results retrieved successfully, suppressing printout due to active blinding!",label.Data()));
      }
    } else {
      error(TString::Format("fit error: Nll minimum for constrained fit better than for unconstrained: %.3f < %.3f",min_constrained,min_unconstrained));
      target->setTagDouble(TString::Format("minNll_%s_constrained",label.Data()), min_constrained);
      target->setTagDouble(TString::Format("minNll_%s_unconstrained",label.Data()), min_unconstrained);
    } 
  } else {
    target->setTagDouble(TString::Format("minNll_%s_constrained",label.Data()), min_constrained);
    target->setTagDouble(TString::Format("minNll_%s_unconstrained",label.Data()), min_unconstrained);
  }
}

//__________________________________________________________________________________|___________
                 
TQFolder * TSSignificanceCalculator::runCalculation(TQFolder * config) {
  DEBUGclass("starting calculation");
  if (!fWorkspace || !fModelConfig || !config) {
    return NULL;
  }
  
  // load nominal snapshot
  TString snapshotName = config->getTagStringDefault("fit.initSnapshot","SnSh_AllVars_Nominal");
  if(!fWorkspace->loadSnapshot(snapshotName)){
    error(TString::Format("unable to load snapshot '%s'",snapshotName.Data()));
    return NULL;
  }

  RooArgSet* pois = this->getPOIs(config);
  RooArgSet   	nuis(*fModelConfig->GetNuisanceParameters());
	RooAbsPdf *		pdf			= fModelConfig->GetPdf();
  TString dataName = config->getTagStringDefault("dataset","asimovData_1");
	RooDataSet *	data		= (RooDataSet*)fWorkspace->data(dataName);
  if (!data) {
    error(TString::Format("unable to obtain dataset '%s'!",dataName.Data()));
    return NULL;
  }


  Bool_t doBlinded			= config->getTagBoolDefault("blinded",false);
  Bool_t save	= config->getTagBoolDefault("save",true);

  TQTaggable fitOptions;
  fitOptions.importTagsWithoutPrefix(config,"fit.");
  fitOptions.setTagBool("reuseNll",config->getTagBoolDefault("fit.reuseNll",true));
  
  // the folder to write the results to 
  TQFolder * result = TQFolder::newFolder("Significance");
  
  TString name(config->getTagStringDefault("name",config->GetName()));
  TString pname(config->getTagStringDefault("parameter","mu"));
  
  TQFolder * fitResult_mu0	= NULL;
  TQFolder * fitResult_muhat	= NULL;
  
  std::vector<TString> constPars = fitOptions.getTagVString("constPars");
  std::vector<TString> floatPars = fitOptions.getTagVString("floatPars");
  std::vector<TString> floatParsCopy(floatPars);
  std::vector<TString> constParsCopy(constPars);
  std::vector<TString> poinames;
  TSUtils::getParameterNames(pois,floatParsCopy);
  TSUtils::getParameterNames(pois,constParsCopy);
  TSUtils::getParameterNames(pois,poinames);

  fitOptions.removeTags("floatPars.*");
  fitOptions.removeTags("constPars.*");
  fitOptions.setTagList("floatPars",floatParsCopy);
  fitOptions.setTagList("constPars",constPars);
  fitResult_muhat = runFit(result,pdf,data,TString::Format("%s_%shat",name.Data(),pname.Data()),  *pois,nuis,false,&fitOptions,save);
  
  if (config->getTagBoolDefault("doConditional",true)) {
    //some options are pointless for the conditional fit, so semi-hardcode them    
    fitOptions.removeTags("floatPars.*");
    fitOptions.removeTags("constPars.*");
    fitOptions.setTagList("floatPars",floatPars);
    fitOptions.setTagList("constPars",constParsCopy);
    for(const auto& poi:poinames){
      fitOptions.setTagDouble("presetParam."+poi, config->getTagDoubleDefault("conditionalParamValue", 0.));
    }
    fitOptions.setTagBool("runMinos",fitOptions.getTagBoolDefault("runMinos.conditional",false));
    fitResult_mu0   = runFit(result,pdf,data,TString::Format("%s_%s0"  ,name.Data(),pname.Data()),  *pois,nuis,true ,&fitOptions,save);
  }
  
  makeSummary(result,fitResult_mu0,fitResult_muhat,name.Data(),*pois,doBlinded);
  
  if(!save){
    delete fitResult_mu0;
    delete fitResult_muhat;
  }

  this->clear();  
  delete pois;
  
  return result;
}
