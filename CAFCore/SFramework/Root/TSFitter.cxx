#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooSimultaneous.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

#include "QFramework/TQStringUtils.h"
#include "QFramework/TQIterator.h"

#include "SFramework/TSFitter.h"

ClassImp(TSFitter)


//__________________________________________________________________________________|___________

TSFitter::TSFitter() : TSStatisticsCalculator("TSFitter") {
}


//__________________________________________________________________________________|___________

TSFitter::TSFitter(RooWorkspace * ws) : TSStatisticsCalculator("TSFitter",ws) {
  DEBUGclass("creating fitter");
}


//__________________________________________________________________________________|___________

TSFitter::~TSFitter() {
}


//__________________________________________________________________________________|___________

TQFolder * TSFitter::runCalculation(TQFolder * options) {
  DEBUGclass("starting calculation");
  if (!fWorkspace || !fModelConfig || !options) {
    return NULL;
  }

  // dataset name
  TString datasetName(options->getTagStringDefault("dataset","asimovData"));

  TQTaggable fitOptions;
  options->exportTags(&fitOptions, "", "fit.*");
  fitOptions.renameTags("fit.", "");
  fitOptions.setTagString("id",options->GetName());
  
  // load nominal snapshot to start from
  TString snapshotName = fitOptions.getTagStringDefault("initSnapshot","SnSh_AllVars_Nominal");
  if(!fWorkspace->loadSnapshot(snapshotName)){
    error(TString::Format("unable to load snapshot '%s'",snapshotName.Data()));
    return NULL;
  }

  /* get the Point Of Interest, PDF, and data */
  RooAbsPdf *	pdf		= fModelConfig->GetPdf();
  RooDataSet *	data		= (RooDataSet*)fWorkspace->data(datasetName.Data());
  RooArgSet nuis(*fModelConfig->GetNuisanceParameters());
  RooArgSet pois(*fModelConfig->GetParametersOfInterest());  

  // stop if dataset could not be found
  if (!data) {
    error(TString::Format("runCalculation(): Could not find dataset '%s'. Stopping ...", datasetName.Data()));
    return NULL;
  }
  
  // restrict data and PDF to a certain category (channel)
  TString category; 
  if (options->getTagString("category",category)){
    if (!pdf->InheritsFrom(RooSimultaneous::Class())) {
      error("runCalculation(): PDF is not a RooSimultaneous. Stopping ...");
      return NULL;
    }
    
    // get PDF of single category
    RooAbsPdf * pdftmp = ((RooSimultaneous*)pdf)->getPdf(category.Data());
    if (!pdftmp) {
      error(TString::Format("runCalculation(): Could not find PDF of "
                            "category '%s'. Stopping ...", category.Data()));
      return NULL;
    }
    
    // get data of single category
    RooAbsData * datatmp = data->reduce(TString::Format("channelCat==channelCat::%s", category.Data()).Data());
    if (!datatmp) {
      error(TString::Format("runCalculation(): Could not find data of "
                            "category '%s'. Stopping ...", category.Data()));
      return NULL;
    }
    
    pdf = pdftmp;
    data = (RooDataSet*)datatmp;
    info(TString::Format("Running fit on category '%s' and dataset '%s' ...", category.Data(), datasetName.Data()));
  } else {
    info(TString::Format("Running fit on dataset '%s' ...", datasetName.Data()));
  }

  RooArgSet allVars;
  allVars.add(nuis);
  allVars.add(pois);
  
  // the results folder
  DEBUGclass("starting fit");
  TQFolder * result = fitPdfToData(pdf, data, &nuis, &fitOptions);
  fWorkspace->saveSnapshot(TString::Format("SnSh_AllVars_Unconditional_%s", datasetName.Data()),allVars);
  return result;
}
