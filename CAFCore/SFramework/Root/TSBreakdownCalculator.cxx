#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <limits>

#include "SFramework/TSBreakdownCalculator.h"
#include "SFramework/TSUtils.h"

#include "TKey.h"
#include "TMath.h"
#include "Math/MinimizerOptions.h"
#include "RooNLLVar.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooRealVar.h"
#include "Math/ProbFuncMathCore.h"
#include "Math/QuantFuncMathCore.h"
#include "TStopwatch.h"

#include "QFramework/TQStringUtils.h"
#include "QFramework/TQIterator.h"

// #define _DEBUG_

#include "QFramework/TQLibrary.h"

ClassImp(TSBreakdownCalculator)

#define epsilon std::numeric_limits<double>::epsilon()
#define inf std::numeric_limits<double>::infinity()
#define nan std::numeric_limits<double>::quiet_NaN()

//__________________________________________________________________________________|___________

TSBreakdownCalculator::TSBreakdownCalculator(RooWorkspace * ws, TQFolder* snapshots) : TSStatisticsCalculator("TSBreakdownCalculator",ws,snapshots) {
}


//__________________________________________________________________________________|___________

TSBreakdownCalculator::~TSBreakdownCalculator() {
}


//__________________________________________________________________________________|___________

void TSBreakdownCalculator::info(TString message) {

  std::cout << "SFramework/TSBreakdownCalculator: " << message.Data() << std::endl;
}

//__________________________________________________________________________________|___________

void TSBreakdownCalculator::storeUncertainty(const RooRealVar* poi, TQFolder * uncond) {
  TString namePOI = poi->GetName();
  TQFolder * varFolder = uncond->getFolder(namePOI + "+");
  varFolder->setTagDouble("val",     poi->getVal());
  varFolder->setTagDouble("errHigh", fabs(poi->getAsymErrorHi()) > epsilon ? poi->getAsymErrorHi() :       poi->getError() );
  varFolder->setTagDouble("errLow",  fabs(poi->getAsymErrorLo()) > epsilon ? poi->getAsymErrorLo() : -fabs(poi->getError()) );
}

	
//__________________________________________________________________________________|___________

void TSBreakdownCalculator::storeUncertainties(const RooAbsCollection* pois, TQFolder * uncond) {
  // loop over POIs and store unconditional uncertainties
  TQIterator itrPOIs(pois->createIterator(), true);
  while (itrPOIs.hasNext()) {
    RooRealVar * var = dynamic_cast<RooRealVar*>(itrPOIs.readNext());
    storeUncertainty(var,uncond);
  }
  itrPOIs.reset();       
}

//__________________________________________________________________________________|___________

void TSBreakdownCalculator::compareUncertainties(const TString& name, const RooAbsCollection* pois, TQFolder* uncond, TQFolder * cond, TQFolder* target) {
  // loop over POIs and determine uncertainties
  TQIterator itrPOIs(pois->createIterator(), true);

  while (itrPOIs.hasNext()) {
    TString namePOI = itrPOIs.readNext()->GetName();
    int nOK;
    
    // get unconditional uncertainties
    Double_t val_uncond = 0.;
    Double_t errHi_uncond = 0.;
    Double_t errLo_uncond = 0.;
    nOK=2;
    if(uncond){
      TQFolder * poi_uncond = uncond->getFolder(namePOI);
      if (!poi_uncond) {
        warn(TString::Format("POI '%s' not contained in fit result '%s'",namePOI.Data(),uncond->GetName()));
        continue;
      }
      double err_uncond = poi_uncond->getTagDoubleDefault("err",0.);
      errHi_uncond = err_uncond;
      errLo_uncond = err_uncond;      
      if(!poi_uncond->getTagDouble("val", val_uncond)){
        error(TString::Format("unable to obtain value of '%s' from unconditional fit result!",namePOI.Data()));
      }
      if(!poi_uncond->getTagDouble("errHigh", errHi_uncond)){
        error(TString::Format("unable to obtain '+' error of '%s' from unconditional fit result!",namePOI.Data()));
        nOK--;
      }
      if(!poi_uncond->getTagDouble("errLow", errLo_uncond)){
        error(TString::Format("unable to obtain '-' error of '%s' from unconditional fit result!",namePOI.Data()));
        nOK--;
      }

      if(fabs(errHi_uncond) < epsilon){
        warn(TString::Format("'+' error of '%s' from unconditional fit result is zero!",namePOI.Data()));
        errHi_uncond = fabs(errLo_uncond);
        errLo_uncond = -fabs(errLo_uncond);        
        nOK--;
      }
      if(fabs(errLo_uncond) < epsilon){
        warn(TString::Format("'-' error of '%s' from unconditional fit result is zero!",namePOI.Data()));
        errHi_uncond = fabs(errLo_uncond);
        errLo_uncond = -fabs(errLo_uncond);        
        nOK--;
      }
			
#ifdef _DEBUG_
      std::cout<<"--------------------------------------------------------------\nTest printout UNcond:\n-----------------------------------------------"<<std::endl;
      poi_uncond->print("rdt");
#endif
    } 
    bool ok_uncond = (nOK>0);

    // get conditional uncertainties
    Double_t val_cond = 0.;
    Double_t errHi_cond = 0.;
    Double_t errLo_cond = 0.;
    nOK=2;
    if(cond){
      TQFolder * poi_cond = cond->getFolder(namePOI);
      if (!poi_cond) {
        warn(TString::Format("POI '%s' not contained in fit result '%s'",namePOI.Data(),cond->GetName()));        
        continue;
      }
      double err_cond = poi_cond->getTagDoubleDefault("err",0.);
      errHi_cond = err_cond;
      errLo_cond = err_cond;      
      if(!poi_cond->getTagDouble("val", val_cond)){
        error(TString::Format("unable to obtain value of '%s' from conditional fit result!",namePOI.Data()));
      }
      if(!poi_cond->getTagDouble("errHigh", errHi_cond)){
        error(TString::Format("unable to obtain '+' error of '%s' from conditional fit result!",namePOI.Data()));
        nOK--;
      }
      if(!poi_cond->getTagDouble("errLow", errLo_cond)){
        error(TString::Format("unable to obtain '-' error of '%s' from conditional fit result!",namePOI.Data()));
        nOK--;
      }

      if(fabs(errHi_cond) < epsilon){
        warn(TString::Format("'+' error of '%s' from conditional fit result is zero!",namePOI.Data()));
        errHi_cond = fabs(errLo_cond);
        errLo_cond = -fabs(errLo_cond);
        nOK--;
      }
      if(fabs(errLo_cond) < epsilon){
        warn(TString::Format("'-' error of '%s' from conditional fit result is zero!",namePOI.Data()));
        errHi_cond = fabs(errHi_cond);
        errLo_cond = -fabs(errHi_cond);        
        nOK--;
      }
#ifdef _DEBUG_
      std::cout<<"--------------------------------------------------------------\nTest printout cond:\n-----------------------------------------------"<<std::endl;
      poi_cond->print("rdt");
#endif
    }
    bool ok_cond = (nOK>0);
    
    
    // why do whe take the absolute value here?
    // in principle, the conditional should *never* be larger than the unconditional
    // however, for very small systematic, numeric fluctuations can twist the systematic in the "wrong" direction 
    // and cause the expression under the square root to be negative
    // by taking the absolute value, we ensure that the impact is always as least as big as the numerical fluctuations it is subject to
    Double_t high = TMath::Sqrt(std::abs(TMath::Power(errHi_uncond, 2.) - TMath::Power(errHi_cond, 2.)));
    Double_t low  = TMath::Sqrt(std::abs(TMath::Power(errLo_uncond, 2.) - TMath::Power(errLo_cond, 2.)));
    Double_t span = TMath::Sqrt(std::abs(TMath::Power(errHi_uncond - errLo_uncond, 2.) - TMath::Power(errHi_cond - errLo_cond, 2.)));

    TQFolder * f_breakdown = target->getFolder(TQFolder::concatPaths(namePOI, name) + "+");
    f_breakdown->setTagBool("IsHessian",false);
    f_breakdown->setTagBool("IsOK",ok_cond && ok_uncond);
    f_breakdown->setTagDouble("Avg", (low + high) / 2.);
    f_breakdown->setTagDouble("Low", -low);
    f_breakdown->setTagDouble("High", high);
    f_breakdown->setTagDouble("Span", span);
    f_breakdown->setTagDouble("Low_RelUnc", -low / fabs(errLo_uncond));
    f_breakdown->setTagDouble("High_RelUnc", high / fabs(errHi_uncond));
    f_breakdown->setTagDouble("Span_RelUnc", 0.5 * span / fabs( errHi_uncond - errLo_uncond ));    
    if(uncond){
      f_breakdown->setTagDouble("Avg_Rel", (low + high) / (2. * val_uncond) );
      f_breakdown->setTagDouble("Low_Rel", -low / val_uncond );
      f_breakdown->setTagDouble("High_Rel", high / val_uncond );
      f_breakdown->setTagDouble("Span_Rel", span / val_uncond );
    }
    f_breakdown->setTagString("comment",TString::Format("%s - %s",uncond ? uncond->GetName() : "NULL",cond ? cond->GetName() : "NULL"));
  }
}

//__________________________________________________________________________________|___________

bool TSBreakdownCalculator::runPreFit(TQTaggable* options, const RooAbsCollection* listOfPOIs, TQFolder* target){
  TQTaggable snapshotoptions;

  TString datasetName = options->getTagStringDefault("datasetName", "asimovData_1");
  snapshotoptions.importTagsWithoutPrefix(options,"fit.");
  snapshotoptions.setTagString("datasetName",datasetName);

  std::vector<TString> namesOfPOIs;
  TSUtils::getParameterNames(listOfPOIs,namesOfPOIs);
  TString poinames = TQStringUtils::concat(namesOfPOIs,",");

  std::vector<TString> namesOfNPs;
  TSUtils::getParameterNames(this->getNuisanceParameters(),namesOfNPs);
  TString npnames = TQStringUtils::concat(namesOfNPs,",");
	
  TString snapshot_uncond = options->getTagStringDefault("snapshot.unconditional","SnSh_AllVars_Unconditional_"+datasetName);
  TString snapshot_cond   = options->getTagStringDefault("snapshot.conditional",  "SnSh_AllVars_Conditional_"+datasetName);  
	
  snapshotoptions.setTagBool("runHesse", false);
  snapshotoptions.setTagString("POI", poinames);
  snapshotoptions.setTagBool("runMinos", true);
  snapshotoptions.setTagList("runMinosVars", namesOfPOIs);
	
  snapshotoptions.setTagString("id","unconditional");
  snapshotoptions.setTagString("snapshot",snapshot_uncond);
  snapshotoptions.setTagList("floatPars",namesOfNPs);
        
  snapshotoptions.setTagString("resultName","unconditional");
        
  if(!this->loadSnapshot(&snapshotoptions)){
    error("failed obtaining unconditional snapshot!");
    return false;
  }
  snapshotoptions.removeTags("floatPars.*");
  this->storeUncertainties(listOfPOIs,target->getFolder("unconditional+"));
	
	
  snapshotoptions.setTagString("id","conditional");
  snapshotoptions.setTagString("snapshot",snapshot_cond);
  snapshotoptions.setTagList("constPars",namesOfNPs);
  snapshotoptions.setTagString("resultName","conditional");
  snapshotoptions.setTagString("snapshot.nominal",snapshot_uncond);
  
  std::vector<TString> allPOIs;
  TSUtils::getParameterNames(fModelConfig->GetParametersOfInterest(),allPOIs);
  for(auto poi:allPOIs){
    RooRealVar* poiReal = dynamic_cast<RooRealVar*>(listOfPOIs->find(poi));
    if(poiReal){
      snapshotoptions.setTagDouble(TString::Format("presetParamError.%s",poi.Data()),poiReal->getError());
    }
  }
  
  if(!this->loadSnapshot(&snapshotoptions)){
    error("failed obtaining conditional snapshot!");
    return false;
  }
  snapshotoptions.removeTags("constPars.*");
  this->storeUncertainties(listOfPOIs,target->getFolder("conditional+"));

  return true;
}

//__________________________________________________________________________________|___________

TQFolder * TSBreakdownCalculator::runCalculation(TQFolder * config) {

  if (!fWorkspace || !fModelConfig || !config) {
    return NULL;
  }
  /* get the Point Of Interest, PDF, and data */
  TString datasetName = config->getTagStringDefault("datasetName", "asimovData_1");
  const RooArgSet * 	nuis		= this->getNuisanceParameters();
  RooArgSet *	listOfPOIs	= this->getPOIs(config);
  
  if (!listOfPOIs) {
    error("no POIs found!");
    return NULL;
  }

  RooArgSet nuisAndPOIs(*nuis,*listOfPOIs);
  // iterate over groups ("group.*") and determine individual uncertainties on POIs
  TSUtils::expandKeys(&nuisAndPOIs,config);
  TQIterator itrGroups(config->getListOfKeys("group.*"), true);
  
  std::vector<TString> namesOfPOIs;
  //Remove the parameters set as const from the POIs if there are any
  std::vector<TString> userPOIs = config->getTagVString("POI");
  if(userPOIs.size() == 0){
    TSUtils::getParameterNames(listOfPOIs,namesOfPOIs);
  } else {
    std::vector<TString> currentPOInames;
    TSUtils::getParameterNames(listOfPOIs,currentPOInames);
    for(auto name:currentPOInames){
      bool use = false;
      for(auto poi:userPOIs){
        if(TQStringUtils::matches(name,poi)){
          use = true;
        }
      }
      if(use) namesOfPOIs.push_back(name);
    }
  }
  RooArgSet newListOfPOIs;
  for(auto poiname:namesOfPOIs){
    newListOfPOIs.add(*(listOfPOIs->find(poiname.Data())));
  }
  TString poinames = TQStringUtils::concat(namesOfPOIs,",");
  delete listOfPOIs;
  
  // the folder to write the results to 
  TQFolder* retval = TQFolder::newFolder(config->GetName());
  if(!runPreFit(config,&newListOfPOIs,retval)){
    return NULL;
  }
  bool invert = config->getTagBoolDefault("invert",false);
  TString startingsnapshot = TString::Format("SnSh_AllVars_%s_%s",invert ? "Conditional" : "Unconditional",datasetName.Data());
  config->getTagString("snapshot",startingsnapshot);

  TQTaggable startingsnapshotoptions(config);
  startingsnapshotoptions.setTagString("snapshot",startingsnapshot);
  if(!this->loadSnapshot(&startingsnapshotoptions)){
    error("failed obtaining initial snapshot!");
    return NULL;
  }

  TQFolder* full_uncond = retval->getFolder("unconditional");
  TQFolder* full_cond   = retval->getFolder("conditional");

  TQFolder* fitresults = retval->getFolder("FitResults+");
  TQFolder* breakdown = retval->getFolder(this->fFolderName+"+");
  RooAbsPdf* pdf = fModelConfig->GetPdf();
  RooDataSet* data = dynamic_cast<RooDataSet*>(fWorkspace->data(datasetName));

  TQTaggable fitOptions;
  fitOptions.importTagsWithoutPrefix(config,"fit.");	

  fitOptions.setTagString("snapshot",startingsnapshot);
  fitOptions.setTagBool("runHesse", false);
  fitOptions.setTagBool("runMinos", true);

  TString constPars = fitOptions.getTagStringDefault("constPars","");
  fitOptions.removeTags("constPars.*");
  fitOptions.removeTags("constPars.except.*");

  std::vector<TString> namesOfNPs;
  TSUtils::getParameterNames(this->getNuisanceParameters(),namesOfNPs);
  TString npnames = TQStringUtils::concat(namesOfNPs,",");

  fitOptions.setTagList("runMinosVars", namesOfPOIs);
  // do not change paramteter values, we're re-using the snapshot from the unconditional fit.
  // Changing paramter values here increases the chances to converge to a different minimum causing all sorts of problems!
  fitOptions.removeTags("initParam.*");	
	
  while (itrGroups.hasNext()) {
    TString name = itrGroups.readNext()->GetName();
    TString filter;
    if(!config->getTagString(name, filter)){
      error(TString::Format("failed to retrieve filter for group '%s'",name.Data()));
      continue;
    }
    TString filterException = config->getTagStringDefault("config."+name+".except",""); //blacklist for parameters not to be included in this group

    std::vector<TString> nps = TSUtils::expandNames(this->getNuisanceParameters(),filter,filterException);

    bool invertCalculation = config->getTagBoolDefault("config."+name+".invert",false);
    TQStringUtils::removeLeadingText(name, "group.");
		
    fitOptions.setTagString("id",name);
    TString floatPars = fitOptions.getTagStringDefault("floatPars","");
    fitOptions.removeTags("floatPars.*");
    fitOptions.removeTags("floatPars.except.*");
    fitOptions.removeTags("constPars.*");
    fitOptions.removeTags("constPars.except.*");

    fitOptions.setTagInteger("verbose",0);    

    if(invert && !invertCalculation) {
      fitOptions.setTagList("floatPars",nps);
      info(TString::Format("Running fit '%s' with filter '%s' and filter exceptions '%s' (%lu matches)", name.Data(), filter.Data(), filterException.Data(),nps.size()));      
    } else {
      fitOptions.setTagList("constPars",nps);
      info(TString::Format("Running fit '%s' with inverted filter '%s' and filter exceptions '%s' (%lu matches)", name.Data(), filter.Data(), filterException.Data(),nps.size()));
    }
    
    TQFolder * result = fitPdfToData(pdf,data, nuis, &fitOptions);
    result->SetName(name);
    fitresults->addObject(result);
    TQFolder* cond = result->getFolder("floatParsFinal");
    if(invertCalculation){
      this->compareUncertainties(name,&newListOfPOIs,NULL,cond,breakdown);
    } else if(invert){
      this->compareUncertainties(name,&newListOfPOIs,full_cond,cond,breakdown);
    } else {
      this->compareUncertainties(name,&newListOfPOIs,cond,full_uncond,breakdown);
    }
    fitOptions.setTagString("constPars",constPars);
  }
    
  fitresults->sortByName();
  breakdown->sortByName();

  fWorkspace->loadSnapshot(startingsnapshot);
  
  return retval;
}
