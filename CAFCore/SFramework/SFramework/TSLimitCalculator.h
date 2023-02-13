//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef LIMIT_CALCULATOR
#define LIMIT_CALCULATOR

#include "RooWorkspace.h"

#include "TSStatisticsCalculator.h"


class TSLimitCalculator : public TSStatisticsCalculator {

 protected:

  virtual void info(TString message) override;
  void readSnapshot(TQFolder * config, RooArgSet& snsh);
 public:
  
  TSLimitCalculator();
  TSLimitCalculator(RooWorkspace * ws);
  
  virtual TQFolder * runCalculation(TQFolder * options = 0)  override;
  
  virtual ~TSLimitCalculator();
  
  ClassDefOverride(TSLimitCalculator, 0);
  
};

#endif
