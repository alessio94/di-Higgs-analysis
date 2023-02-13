//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef FITTER
#define FITTER

#include "RooWorkspace.h"

#include "TSStatisticsCalculator.h"


class TSFitter : public TSStatisticsCalculator {

public:

  TSFitter();
  TSFitter(RooWorkspace * ws);

  virtual TQFolder * runCalculation(TQFolder * options = 0) override;

  virtual ~TSFitter();
		
  ClassDefOverride(TSFitter, 0);

};

#endif
