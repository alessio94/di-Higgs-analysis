#include "CAFxAODUtils/TQEventFlaggingAnalysisJob.h"
#include "QFramework/TQCut.h"
#include "QFramework/TQAnalysisJob.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQToken.h"

#include "xAODRootAccess/TEvent.h"
#include "xAODEventInfo/EventInfo.h"


// #define _DEBUG_
#include "QFramework/TQLibrary.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include <stdexcept>

using std::cout;
using std::endl;

ClassImp(TQEventFlaggingAnalysisJob)


//______________________________________________________________________________________________

TQEventFlaggingAnalysisJob::TQEventFlaggingAnalysisJob() : 
TQAnalysisJob("TQEventFlaggingAnalysisJob"),
fFlagValue(true),
fFlagName("passedSelection")

{

}


//______________________________________________________________________________________________

TQEventFlaggingAnalysisJob::TQEventFlaggingAnalysisJob(TString name_, bool flag) : 
  TQAnalysisJob(name_),
  fFlagValue(flag),
  fFlagName(name_)

{

}


//______________________________________________________________________________________________

bool TQEventFlaggingAnalysisJob::initializeSelf() {
  /* initialize this analysis job */
  if (!this->fSample) return false;
  this->fEventToken = this->fSample->getEventToken();
  if (!this->fEventToken) return false;
  this->fEventToken->setOwner(this);
  this->fEvent = static_cast<xAOD::TEvent*>(this->fEventToken->getContent());
  this->fActiveFlagName = this->fSample->replaceInTextRecursive(this->fFlagName,"~");
  
  return true;
}


//______________________________________________________________________________________________

bool TQEventFlaggingAnalysisJob::finalizeSelf() {
  /* finalize this analysis job */
  DEBUGclass("attempting to create .cutflow folder in sample '%s'",this->fSample->getPath().Data());
	if (this->fEventToken) {
	  this->fSample->returnToken(this->fEventToken); //token passed by reference to pointer, internally set to NULL
    this->fEvent = NULL;
	}
  return true;

}



//______________________________________________________________________________________________

bool TQEventFlaggingAnalysisJob::execute(double) {
  const xAOD::EventInfo* eventInfo = NULL;
  if(this->fEvent->retrieve(eventInfo, "EventInfo").isFailure()){
    throw std::runtime_error("unable to retrieve event info from TEvent");
    return false;
  }
  DEBUGclass("attempting to attach flag '%s'", fActiveFlagName.Data());  
  eventInfo->auxdecor<char>(this->fActiveFlagName.Data()) = this->fFlagValue;

  return true;

}


//______________________________________________________________________________________________

TQEventFlaggingAnalysisJob::~TQEventFlaggingAnalysisJob() {
  if (this->fSample && this->fEventToken) {
    this->fSample->returnToken(fEventToken); //passing pointer reference, fEventToken will be automatically set to NULL
    this->fEvent = NULL;
    this->fSample = NULL;
  }
}


