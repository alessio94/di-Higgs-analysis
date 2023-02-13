//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef HYPOTHESIS_TEST
#define HYPOTHESIS_TEST

#include "TSStatisticsCalculator.h"

class TSHypothesisTest : public TSStatisticsCalculator {
public:
  
  class Hypothesis { // nested
  protected:
    std::string fName;
    std::map<std::string,double> fParameters;
  public:
    Hypothesis(const char* hname);
    Hypothesis(const char* hname, const char* pname, double val);
    Hypothesis(TQFolder* h);
    const char* getParameterNameString();    
    void addParameter(const char* pname, double val);    
    const char* name() const;
    void addParametersToList(const RooAbsCollection& allPars, RooAbsCollection& toList);
    void setParameters(RooAbsCollection& pars) const;
    void setParametersConstant(RooAbsCollection& pars, bool setConstant=true) const;
  };

protected:

  void runFit(TQFolder* results, RooDataSet* data, const TString& name, TQFolder* options);
  TQFolder* runFit(RooDataSet* data, TQFolder* options, RooAbsCollection& pois, const std::vector<const TSHypothesisTest::Hypothesis*>& hypotheses);

public: 
  
	TSHypothesisTest(RooWorkspace * ws, TQFolder* snapshots);
	
	virtual TQFolder * runCalculation(TQFolder * options = NULL) override;
  
	virtual ~TSHypothesisTest();
	
	ClassDefOverride(TSHypothesisTest, 0);
	
};

#endif
