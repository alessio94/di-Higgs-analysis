#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iterator>

#include "SFramework/TSImpactCalculator.h"
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
#include "QFramework/TQLibrary.h"

ClassImp(TSImpactCalculator)


//__________________________________________________________________________________|___________

TSImpactCalculator::TSImpactCalculator(RooWorkspace * ws, TQFolder* snapshots) : TSStatisticsCalculator("TSImpactCalculator",ws,snapshots) {
}


//__________________________________________________________________________________|___________

TSImpactCalculator::~TSImpactCalculator() {
}


//__________________________________________________________________________________|___________

void TSImpactCalculator::info(TString message) {

  std::cout << "SFramework/TSImpactCalculator: " << message.Data() << std::endl;
}


//__________________________________________________________________________________|___________

TQFolder* TSImpactCalculator::fit(TQTaggable* options, RooAbsPdf * pdf, RooDataSet * data, const RooArgSet* listOfPOIs, const RooArgSet * nuis, const TString& name, const TString& pname /*, bool invert*/) {
	// run a fit for a given setup
  options->setTagString("id",name);

	// take care of setting the "right" parameters constant/floating
	options->removeTags("constPars");
	options->removeTags("floatPars");
	options->setTagString("constPars",pname);
  std::vector<TString> poinames;
  TSUtils::getParameterNames(listOfPOIs,poinames);
  options->setTagString("floatPars",TQStringUtils::concat(poinames,","));
	info("running fit '"+name+"'");
	TQFolder * fitResult = fitPdfToData(pdf, data, nuis, options);
  fitResult->SetName(name);
  return fitResult;
}

//__________________________________________________________________________________|___________

void TSImpactCalculator::storeImpacts(TQFolder* result,const TString& parname, TQFolder* nominal,TQFolder* up,TQFolder* down){
	// store the impacts in a TQFolder
  if(!up){
    error("unable to retrieve fit results of up variation!");
    return;
  }
  if(!down){
    error("unable to retrieve fit results of down variation!");
    return;
  }
  TQFolderIterator itr(nominal->getListOfFolders("?"));
  while(itr.hasNext()){
    TQFolder * poi = itr.readNext();
    if (!poi) {
      continue;
    }
		TString poiname(poi->GetName());
		TQFolder* poiresult = result->getFolder(poiname+"+")->getFolder(parname+"+");
    double val_uncond = poi->getTagDoubleDefault("val", 0.);

    TQFolder* poi_up = up->getFolder(poiname);
    if(!poi_up){
      error("unable to retrieve up variation");
      return;
    }
    double val_up = poi_up->getTagDoubleDefault("val", 0.);

    TQFolder* poi_down = down->getFolder(poiname);
    if(!poi_down){
      error("unable to retrieve up variation");
      return;
    }
    double val_down = poi_down->getTagDoubleDefault("val", 0.);

    Double_t high = val_up - val_uncond;
    Double_t low  = val_down - val_uncond;
    Double_t span = std::abs(val_up - val_down);
    Double_t avg = 0.5 * (std::abs(val_up-val_uncond) + std::abs(val_down-val_uncond));
    double absval = std::abs(val_uncond);

    poiresult->setTagDouble("Avg", avg);
    poiresult->setTagDouble("Low", low);
    poiresult->setTagDouble("High", high);
    poiresult->setTagDouble("Span", span);
    poiresult->setTagDouble("Avg_Rel", avg / absval);
    poiresult->setTagDouble("Low_Rel", low / absval);
    poiresult->setTagDouble("High_Rel", high / absval);
    poiresult->setTagDouble("Span_Rel", span / absval);
  }
}

//__________________________________________________________________________________|___________

bool TSImpactCalculator::runPreFit(TQTaggable* options, const RooAbsCollection* listOfPOIs, TQFolder* target){
	TQTaggable snapshotoptions(options);

	TString datasetName = options->getTagStringDefault("datasetName", "asimovData_1");
	snapshotoptions.importTagsWithoutPrefix(options,"fit.");
  snapshotoptions.setTagString("datasetName",datasetName);

  // std::vector<TString> namesOfPOIs;
  // TSUtils::getParameterNames(listOfPOIs,namesOfPOIs);

  std::vector<TString> namesOfNPs;
  TSUtils::getParameterNames(this->getNuisanceParameters(),namesOfNPs);
  // TString npnames = TQStringUtils::concat(namesOfNPs,",");

  TString minosVars = options->getValuesOfTags("singles.*");

  TString snapshot_uncond = options->getTagStringDefault("snapshot.unconditional","SnSh_AllVars_Impact_Unconditional_"+datasetName);
  TString snapshot_cond   = options->getTagStringDefault("snapshot.conditional",  "SnSh_AllVars_Impact_Conditional_"  +datasetName);

	snapshotoptions.setTagBool("runHesse", false);
	snapshotoptions.setTagBool("runMinos", true);
	snapshotoptions.setTagList("runMinosVars", minosVars);

	snapshotoptions.setTagString("id","unconditional");
  snapshotoptions.setTagString("snapshot",snapshot_uncond);
  snapshotoptions.setTagList("floatPars",namesOfNPs);

  // snapshotoptions.setTagString("resultName","unconditional");

	if(!this->loadSnapshot(&snapshotoptions)){
		error("failed obtaining unconditional snapshot!");
		return false;
	}

	TQFolder* nominal = TSUtils::convertParameterList(listOfPOIs);
	nominal->SetName("nominal");
	target->addFolder(nominal);

	snapshotoptions.setTagString("id","conditional");
	snapshotoptions.setTagString("snapshot",snapshot_cond);
  snapshotoptions.removeTags("floatPars.*");
  // snapshotoptions.setTagString("floatPars",options->getTagStringDefault("floatPars",""));
	snapshotoptions.setTagList("constPars",namesOfNPs);
	snapshotoptions.setTagString("resultName","conditional");
	snapshotoptions.setTagString("snapshot.nominal",snapshot_uncond);
  this->setParametersConstFloat(fWorkspace->allVars(),&snapshotoptions);
  fWorkspace->saveSnapshot(snapshot_cond.Data(), fWorkspace->allVars());
 //  if(!this->loadSnapshot(&snapshotoptions)){
	// 	error("failed obtaining conditional snapshot!");
	// 	return false;
	// }
	return true;
}

//__________________________________________________________________________________|___________

TQFolder * TSImpactCalculator::runCalculation(TQFolder * config) {

  if (!fWorkspace || !fModelConfig || !config) {
    return NULL;
  }

  /* get the Point Of Interest, PDF, and data */
  TString datasetName = config->getTagStringDefault("datasetName", "asimovData_1");
  const RooArgSet * 	nuis		= this->getNuisanceParameters();
  RooArgSet *	listOfPOIs	= this->getPOIs(config);
  RooArgSet nuisAndPOIs(*nuis);
  nuisAndPOIs.add(*listOfPOIs);

  if (!listOfPOIs) {
    error("no POIs found!");
    return NULL;
  }

  // iterate over groups ("group.*") and determine individual uncertainties on POIs
  TSUtils::expandKeys(&nuisAndPOIs,config);
  TQIterator itrGroups(config->getListOfKeys("group.*"), true);

 TQFolder* retval = TQFolder::newFolder(config->GetName());
  if(!runPreFit(config,listOfPOIs,retval)){
    return NULL;
  }
  bool invert = config->getTagBoolDefault("invert",false);
  TString startingsnapshot = TString::Format("SnSh_AllVars_Impact_%s_%s",invert ? "Conditional" : "Unconditional",datasetName.Data());
  TQFolder* nominal = retval->getFolder("nominal");

  TQFolder* fitresults = retval->getFolder("FitResults+");
  TQFolder* impacts = retval->getFolder(this->fFolderName+"+");
  RooAbsPdf* pdf = fModelConfig->GetPdf();
  RooDataSet* data = dynamic_cast<RooDataSet*>(fWorkspace->data(datasetName));

  TQTaggable fitOptions;
  fitOptions.setTagBool("runHesse", false);
  fitOptions.setTagBool("runMinos", false);
  fitOptions.setTagBool("loadSnapshot", false);

  fitOptions.importTagsWithoutPrefix(config,"fit.");
  // do not change paramteter values, we're re-using the snapshot from the unconditional fit.
  // Changing paramter values here increases the chances to converge to a different minimum causing all sorts of problems!
  fitOptions.removeTags("initParam*");

  // iterate over groups ("group.*") and determine individual uncertainties on POIs
  while (itrGroups.hasNext()) {
    TString name = itrGroups.readNext()->GetName();
    TString filter;
    if(!config->getTagString(name, filter)) continue;
    TQStringUtils::removeLeadingText(name, "group.");
    if(filter.Contains("*")){
      error("wildcarded groups not supported: "+filter);
      continue;
    }

    RooRealVar* var = NULL;
    std::vector<RooRealVar*> params;
    RooFIter itr(nuisAndPOIs.fwdIterator());
    while((var = (RooRealVar*)itr.next())){
      if(TQStringUtils::matchesFilter(var->GetName(),filter)){
        params.push_back(var);
      }
    }

    if(params.size() == 0){
      warn(TString::Format("no variables found matching filter '%s'",filter.Data()));
      continue;
    }

    this->fWorkspace->loadSnapshot(startingsnapshot);
    for(auto var:params){
      const double nom = var->getVal();
      const double up = var->getErrorHi();
      var->setVal(nom+std::abs(up));
      std::cout<<"Set parameter "<< var->GetName()<<" to "<<nom+std::abs(up)<<std::endl;
    }

    TQFolder* fitresultUp = fit(&fitOptions, pdf, data, listOfPOIs, &nuisAndPOIs, name+"_Up", name /*, invert*/);
    fitresults->addObject(fitresultUp);

    this->fWorkspace->loadSnapshot(startingsnapshot);
    for(auto var:params){
      const double nom = var->getVal();
      const double down = var->getErrorLo();
      var->setVal(nom-std::abs(down));
      std::cout<<"Set parameter "<< var->GetName()<<" to "<<nom-std::abs(down)<<std::endl;
    }

    TQFolder* fitresultDown = fit(&fitOptions, pdf, data, listOfPOIs, &nuisAndPOIs, name+"_Down", name /*, invert*/);
    fitresults->addObject(fitresultDown);

    if(impacts && nominal && fitresultUp && fitresultDown){
      this->storeImpacts(impacts,name,
                         nominal,fitresultUp->getFolder("floatParsFinal"),fitresultDown->getFolder("floatParsFinal"));
    } else {
      error(TString::Format("error measuring impact for '%s'",name.Data()));
    }
  }

  delete listOfPOIs;



  impacts->sortByName();
  fitresults->sortByName();

  fWorkspace->loadSnapshot(startingsnapshot);

  return retval;
}
