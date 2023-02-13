//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef LIKELIHOOD_SCANNER
#define LIKELIHOOD_SCANNER

#include "RooWorkspace.h"

#include "TSStatisticsCalculator.h"

class TSLikelihoodScanner : public TSStatisticsCalculator {

 protected:

  typedef std::map<TString,double> Point; // EXCLUDE
  virtual void info(TString message) override;
  void generatePoints(const std::vector<TQFolder*>& params, std::vector<Point>& points, const Point& thispoint, size_t idx=0);
  TString stringify(const Point& p);
  
public:
  
  static int nPoints(TQFolder* config);
  
  TSLikelihoodScanner(RooWorkspace * ws, TQFolder* snapshots);
  
  std::map<TString,TQFolder*> collectParameters(TQFolder* options);

  virtual TQFolder * runCalculation(TQFolder * options = NULL) override;

  TQFolder* runPreFit(TQFolder * options);
  std::map<Point,TQFolder*> runScan(TQFolder * options, TQFolder* uncondResult);
  TQFolder * finalizeScan(TQFolder * options, std::map<Point,TQFolder*>& result);
  
  virtual ~TSLikelihoodScanner();
  
  ClassDefOverride(TSLikelihoodScanner, 0);
  
};

#endif
