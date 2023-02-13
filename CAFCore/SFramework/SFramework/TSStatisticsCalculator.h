//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef STATISTICS_CALCULATOR
#define STATISTICS_CALCULATOR

#include "RooWorkspace.h"
#include "RooSimultaneous.h"
#include "RooStats/ModelConfig.h"
#include "RooRealVar.h"
#include "RooFitResult.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooArgList.h"
#include "RooNLLVar.h"

#include "SFramework/RooFitUtils.h"

#ifdef HAS_RooFitUtils
#include <RooFitUtils/ExtendedMinimizer.h>
#endif

#include "QFramework/TQTaggable.h"
#include "QFramework/TQFolder.h"

#ifdef HAS_RooFitUtils
class TSStatisticsCalculator : public RooFitUtils::ExtendedMinimizer {
#else
class TSStatisticsCalculator {
 protected:
  RooWorkspace * fWorkspace = NULL;
  RooAbsReal* fNll = NULL;
#endif
 protected:
  RooStats::ModelConfig * fModelConfig;
  TQFolder* fSnapshots = NULL;
  
protected:
  virtual void info(TString message);
  virtual void error(TString message);
  virtual void warn(TString message);

  void setParameterValues(TQTaggable* options, const TString& prefix);
  void setParameterErrors(TQTaggable* options, const TString& prefix);
	void setParametersConstFloat(const RooArgSet * params, TQTaggable * options);
	void setParametersConstFloat(const RooArgSet& params, TQTaggable * options);
	
  TQFolder * minimizeNll(const TString& options);
  TQFolder * minimizeNll(TQTaggable * options);

  void resetOffset();

  void setup(const TString& datasetName, TQTaggable * options = NULL);  
  void setup(RooAbsPdf * pdf, RooDataSet * data, const RooArgSet * constrain, TQTaggable * options);

  void clear();
  
  virtual TQFolder * fitPdfToData(RooAbsPdf * pdf, RooDataSet * data, const RooArgSet * constrain, TQTaggable * options = NULL);
  virtual TQFolder * fitPdfToData(RooAbsPdf * pdf, RooDataSet * data, const RooArgSet * constrain, const TString& options);
	
 public:

	RooWorkspace* getWorkspace();
	RooStats::ModelConfig* getModelConfig();
  RooAbsPdf* getPdf();
  RooSimultaneous* getSimPdf();
  RooCategory* getIndexCategory();
	const RooArgSet* getNuisanceParameters();
	const RooArgSet* getPOIs();
	
  static RooArgSet* makeMinosArgSet(RooWorkspace* ws, const std::vector<TString>& filters);  
  static void unfoldConstraints(RooArgSet &initial, RooArgSet &final, RooArgSet &obs, RooArgSet &nuis, Int_t &counter);

  bool loadSnapshot(TQTaggable * options);
	
  TSStatisticsCalculator(const char* className);
  TSStatisticsCalculator(const char* className,RooWorkspace * ws);
	TSStatisticsCalculator(const char* className,RooWorkspace * ws,TQFolder* snapshots);
  virtual ~TSStatisticsCalculator();

	RooArgSet* getPOIs(TQTaggable* options);
  Bool_t setWorkspace(RooWorkspace * ws);
 
  virtual TQFolder * runCalculation(TQFolder * options = 0) = 0;
  
  virtual TQFolder * fitPdfToData(const TString& datasetName, TQTaggable * options = NULL);
	
#ifdef HAS_RooFitUtils
  ClassDefOverride(TSStatisticsCalculator, 0);
#else
  ClassDef(TSStatisticsCalculator, 0);
#endif
};
 
#endif
