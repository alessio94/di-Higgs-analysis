//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef SIGNIFICANCE_CALCULATOR
#define SIGNIFICANCE_CALCULATOR

#include "RooWorkspace.h"

#include "TSStatisticsCalculator.h"


class TSSignificanceCalculator : public TSStatisticsCalculator {

 protected:

  virtual void info(TString message) override;
  TQFolder* runFit(TQFolder* result, RooAbsPdf* pdf, RooDataSet* data, const TString& fitid,RooArgSet& pois, RooArgSet& nuis, bool conditional, TQTaggable* fitOptions,bool save);
  void makeSummary(TQFolder* target, TQFolder* result_constrained, TQFolder* result_unconstrained, const TString& label, RooArgSet& pois, bool blinded);
  
 public:
  
  TSSignificanceCalculator();
  TSSignificanceCalculator(RooWorkspace * ws);
  
  virtual TQFolder * runCalculation(TQFolder * options = 0)  override;
  
  virtual ~TSSignificanceCalculator();
  
  ClassDefOverride(TSSignificanceCalculator, 0);
  
};

#endif
