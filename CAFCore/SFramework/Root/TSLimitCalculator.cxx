#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iterator>

#include "RooNLLVar.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooStats/AsymptoticCalculator.h"
#include "RooStats/HypoTestInverter.h"
#include "RooStats/HypoTestPlot.h"
#include "RooStats/HypoTestInverterPlot.h"
#include "TCanvas.h"

#include "QFramework/TQPathManager.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQLibrary.h"

#include "SFramework/TSLimitCalculator.h"
#include "SFramework/TSUtils.h"

ClassImp(TSLimitCalculator)

//__________________________________________________________________________________|___________

TSLimitCalculator::TSLimitCalculator() : TSStatisticsCalculator("TSLimitCalculator") {
}


//__________________________________________________________________________________|___________

TSLimitCalculator::TSLimitCalculator(RooWorkspace * ws) : TSStatisticsCalculator("TSLimitCalculator",ws) {
}


//__________________________________________________________________________________|___________

TSLimitCalculator::~TSLimitCalculator() {
}


//__________________________________________________________________________________|___________

void TSLimitCalculator::info(TString message) {

  std::cout << "SFramework/TSLimitCalculator: " << message.Data() << std::endl;
}

//__________________________________________________________________________________|___________
                 
void TSLimitCalculator::readSnapshot(TQFolder * config, RooArgSet& snsh) {
  TQTaggableIterator itr(config->getListOfKeys());
  while(itr.hasNext()){
    TObject* tag = itr.readNext();
    if(!tag) continue;
    double val = config->getTagDoubleDefault(tag->GetName(),0.);
    RooRealVar* var = fWorkspace->var(tag->GetName());
    if(!var){
      warn(TString::Format("unable to find parameter '%s'",tag->GetName()));
      continue;
    }
    var->setVal(val);
    snsh.add(*var);
  }
}

//__________________________________________________________________________________|___________
                 
TQFolder * TSLimitCalculator::runCalculation(TQFolder * config) {

  if (!fWorkspace || !fModelConfig) {
    return NULL;
  }
  
  // load nominal snapshot
  TString snapshotName = config->getTagStringDefault("fit.initSnapshot","SnSh_AllVars_Nominal");
  if(!fWorkspace->loadSnapshot(snapshotName)){
    error(TString::Format("unable to load snapshot '%s'",snapshotName.Data()));
    return NULL;
  }

  RooArgSet   	nuis(*fModelConfig->GetNuisanceParameters());
  TString dataName = config->getTagStringDefault("dataset","asimovData_1");
	RooDataSet *	data		= (RooDataSet*)fWorkspace->data(dataName);
  if (!data) {
    error(TString::Format("unable to obtain dataset '%s'!",dataName.Data()));
    return NULL;
  }

  TQFolder* parameters = config->getFolder("Parameters");
  if(!parameters){
    error("no parameters given, please specifiy in 'Parameters'");
    return NULL;
  }
  
  RooStats::ModelConfig* bModel = fModelConfig->Clone();
  TSUtils::applySettings(parameters,fWorkspace->allVars(),bModel);
  TQFolder* h0 = parameters->getFolder("H0");
  if(h0){
    RooArgSet snapshot;
    this->readSnapshot(h0,snapshot);
    bModel->SetSnapshot(snapshot);
  } else {
    warn("no parameters given for Null Hypothesis, please specifiy in 'Parameters/H0'");
  }
  RooStats::ModelConfig* sbModel = fModelConfig->Clone();
  TSUtils::applySettings(parameters,fWorkspace->allVars(),sbModel);
  TQFolder* h1 = parameters->getFolder("H1");
  if(h1){
    RooArgSet snapshot;
    this->readSnapshot(h1,snapshot);
    sbModel->SetSnapshot(snapshot);
  } else {
    warn("no parameters given for Null Hypothesis, please specifiy in 'Parameters/H1'");
  }

  bool oneSided = config->getTagBoolDefault("oneSided",true);
  bool useCLs = config->getTagBoolDefault("useCLs",true);
  double confidenceLevel = config->getTagDoubleDefault("confidenceLevel",.9);
  int nPoints = config->getTagIntegerDefault("nPoints",50);
  double xmin = config->getTagDoubleDefault("min",0.);
  double xmax = config->getTagDoubleDefault("max",6.);  

  TString outfile;
  bool redirect=config->getTag("fit.logToFile",outfile);
  if(redirect){
    TString fname = config->replaceInText(outfile);
    fname = TQPathManager::getPathManager()->getTargetPath(fname);
    TQUtils::ensureDirectoryForFile(fname);
    info(TString::Format("writing log to '%s'",fname.Data()));
    TQLibrary::redirect(fname,true);
  }  

  RooStats::AsymptoticCalculator  asympCalc(*data, *bModel, *sbModel);
  asympCalc.SetOneSided(oneSided);
  RooStats::HypoTestInverter inverter(asympCalc);
  inverter.SetConfidenceLevel(confidenceLevel);
  inverter.UseCLs(useCLs);
 
  inverter.SetVerbose(false);
  inverter.SetFixedScan(nPoints,xmin,xmax); 

  RooStats::HypoTestInverterResult* htresult =  inverter.GetInterval();
  
  if(redirect){
    TQLibrary::restore();
  }

  TQFolder* result = new TQFolder("Limit");
  result->addFolder(config->copy(".config")); // bookkeeping

  result->setTagDouble("upper",htresult->UpperLimit());
  result->setTagDouble("lower",htresult->LowerLimit());

  result->setTagDouble("exp_upper_med",htresult->GetExpectedUpperLimit(0));
  result->setTagDouble("exp_upper_p1s",htresult->GetExpectedUpperLimit(+1));
  result->setTagDouble("exp_upper_m1s",htresult->GetExpectedUpperLimit(-1));
  result->setTagDouble("exp_upper_p2s",htresult->GetExpectedUpperLimit(+2));
  result->setTagDouble("exp_upper_m2s",htresult->GetExpectedUpperLimit(-2));
  result->setTagDouble("exp_lower_med",htresult->GetExpectedLowerLimit(0));
  result->setTagDouble("exp_lower_p1s",htresult->GetExpectedLowerLimit(+1));
  result->setTagDouble("exp_lower_m1s",htresult->GetExpectedLowerLimit(-1));
  result->setTagDouble("exp_lower_p2s",htresult->GetExpectedLowerLimit(+2));
  result->setTagDouble("exp_lower_m2s",htresult->GetExpectedLowerLimit(-2));

  for(int i=0; i<htresult->ArraySize(); ++i){
    result->setTagDouble(TString::Format("CLb.%d",          i),htresult->CLb          (i));
    result->setTagDouble(TString::Format("CLbError.%d",     i),htresult->CLbError     (i));
    result->setTagDouble(TString::Format("CLs.%d",          i),htresult->CLs          (i));
    result->setTagDouble(TString::Format("CLsError.%d",     i),htresult->CLsError     (i));
    result->setTagDouble(TString::Format("CLsplusb.%d",     i),htresult->CLsplusb     (i));
    result->setTagDouble(TString::Format("CLsplusbError.%d",i),htresult->CLsplusbError(i));
  }  

  // Make test scan plot
//  RooStats::HypoTestInverterPlot *plot = new RooStats::HypoTestInverterPlot("HTI_Result_Plot", "95% CLs Scan Result", htresult);
//
//  TCanvas *c1 = new TCanvas("Test Scan");
//  c1->SetLogy(false);
//  plot->Draw("CLb 2CL"); // plot all and Clb
//  c1->SaveAs("TestScan.pdf");

  delete sbModel;
  delete bModel;
  delete htresult;

  return result;
}
