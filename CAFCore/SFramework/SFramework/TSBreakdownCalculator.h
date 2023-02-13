//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef UNCERTAINTY_BREAKDOWN
#define UNCERTAINTY_BREAKDOWN

#include "TSStatisticsCalculator.h"


class TSBreakdownCalculator : public TSStatisticsCalculator {
public:
	const TString fFolderName = "Breakdown";
	
protected:
  
  virtual void info(TString message) override;
  
	void storeUncertainty(const RooRealVar* poi, TQFolder * uncond);
  void storeUncertainties(const RooAbsCollection* pois, TQFolder * uncond);
  void compareUncertainties(const TString& name, const RooAbsCollection* pois, TQFolder* uncond, TQFolder * cond, TQFolder* target);
  
public:
  
  TSBreakdownCalculator(RooWorkspace * ws, TQFolder* snapshots);

	bool runPreFit(TQTaggable* options, const RooAbsCollection* pois, TQFolder* fitresult);
	
  virtual TQFolder * runCalculation(TQFolder * options = 0) override;
  
  virtual ~TSBreakdownCalculator();
  
  ClassDefOverride(TSBreakdownCalculator, 0);
  
};

#endif
