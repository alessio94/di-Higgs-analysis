//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQAnalysisAlgorithm__
#define __TQAnalysisAlgorithm__

#include "TString.h"
#include "QFramework/TQAlgorithm.h"

#include "QFramework/TQCut.h"
#include "TObject.h"
#include "QFramework/TQCounter.h"

class TQAnalysisAlgorithm : public TQAlgorithm {
protected:
  TQCut* fCut = NULL;
  bool fUseWeights;
  TQSample * fSample; //!



public:

  TQAnalysisAlgorithm(const TString& name, TQCut* myCut);

  virtual void setBaseCut(TQCut* myCut);

  virtual bool initialize(TQSample* s) override;
  virtual bool finalize() override;

  virtual bool initializeSampleFolder(TQSampleFolder* s) override;
  virtual bool finalizeSampleFolder(TQSampleFolder* sf) override;
  
  virtual bool execute() override;
  virtual bool cleanup() override;

  virtual TQAlgorithm * getClone() const override;

  virtual TObjArray * getListOfBranches() override;

  ClassDefOverride(TQAnalysisAlgorithm,0)
};

#endif
