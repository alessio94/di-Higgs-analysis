//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef IMPACT_CALCULATOR
#define IMPACT_CALCULATOR

#include "TSStatisticsCalculator.h"


class TSImpactCalculator : public TSStatisticsCalculator {
public:
	const TString fFolderName = "Impacts";
	
protected:
  
  virtual void info(TString message) override;

	TQFolder* fit(TQTaggable* options, RooAbsPdf * pdf, RooDataSet * data, const RooArgSet* pois, const RooArgSet * nuis, const TString& name, const TString& pname/*, bool invert*/);
	
public:

  void storeImpacts(TQFolder* result,const TString& parname, TQFolder* nominal,TQFolder* up,TQFolder* down);
  
  TSImpactCalculator(RooWorkspace * ws, TQFolder* snapshots);

	bool runPreFit(TQTaggable* options, const RooAbsCollection* pois, TQFolder* fitresult);
	
  virtual TQFolder * runCalculation(TQFolder * options = 0) override;
  
  virtual ~TSImpactCalculator();
  
  ClassDefOverride(TSImpactCalculator, 0);
  
};

#endif
