//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef CONTOUR_SCANNER
#define CONTOUR_SCANNER

#include "RooWorkspace.h"

#include "TSStatisticsCalculator.h"

#include <map>

class TSContourScanner : public TSStatisticsCalculator {
 protected:
  
  virtual void info(TString message) override;

  int getVarIndex(RooArgList* list, RooRealVar* v);
  
  TQFolder * contour(TQTaggable* options, RooRealVar* var1, RooRealVar* var2, unsigned int npoints, const std::map<TString,double>& thresholds);
  
 public:
  
  TSContourScanner(RooWorkspace * ws, TQFolder* snapshots);
  
  virtual TQFolder * runCalculation(TQFolder * options = NULL) override;
  
  virtual ~TSContourScanner();
  
  ClassDefOverride(TSContourScanner, 0);
  
};

#endif
