//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQEventFlaggingAnalysisJob__
#define __TQEventFlaggingAnalysisJob__

#include "QFramework/TQAnalysisJob.h"
#include "TObject.h"
#include "QFramework/TQCounter.h"

class TQSample;
class TQToken;
namespace xAOD{
  class TEvent;
}

class TQEventFlaggingAnalysisJob : public TQAnalysisJob {

protected:

  
  void init();

  TQToken* fEventToken; //!
  xAOD::TEvent* fEvent; //!  
	bool fFlagValue;
	TString fFlagName;
	TString fActiveFlagName; //name of the flag after initialization, with possible tags replaced

public:

  TQEventFlaggingAnalysisJob();
  TQEventFlaggingAnalysisJob(TString name_, bool flag);

  inline bool getFlagValue() const {return fFlagValue;}
  inline void setFlagValue(bool flag) {this->fFlagValue = flag;}
  inline TString getFlagName() const {return fFlagName;}
  inline void setFlagName(const TString& name) {this->fFlagName = name;}
  //bool initializeSampleFolder(TQSampleFolder* sf) override;
  //bool finalizeSampleFolder (TQSampleFolder* sf) override;
  //using TQAnalysisJob::getBranchNames;

  virtual bool initializeSelf() override;
  virtual bool finalizeSelf() override;
  virtual bool execute(double weight) override;
 
  virtual ~TQEventFlaggingAnalysisJob();
 
  ClassDefOverride(TQEventFlaggingAnalysisJob, 1);

};

#endif
