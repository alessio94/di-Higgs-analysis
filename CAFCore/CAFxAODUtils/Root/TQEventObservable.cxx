// please don't do this
// currently required to retrieve the entry number from xAOD::TEvent
#define protected public
#include "xAODRootAccess/TActiveStore.h"
#include "xAODRootAccess/TEvent.h"
#include "AsgTools/SgTEvent.h"
#include "CAFxAODUtils/TQEventObservable.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQToken.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

#include <iostream>

ClassImp(TQEventObservable)

////////////////////////////////////////////////////////////////////////////////////////////////
//
/* @observable: [TQEventObservable]
   The TQEventObservable is a (still abstract) specialization of the abstract
   class TQObservable. It provides derived classes with an automated mechanism
   to access the Event.
*/
////////////////////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________________________

TQEventObservable::TQEventObservable() : 
TQObservable()
{
  // default constructor
}

//______________________________________________________________________________________________

TQEventObservable::TQEventObservable(const TString& expression) :
  TQObservable(expression)
{
  // constructor with expression argument
}

//______________________________________________________________________________________________

bool TQEventObservable::initialize(TQSample * sample){
  // initialize this observable, obtain an event token
  if(this->fIsInitialized) return true;
  /* we can't do anything if we already own a tree token */
  if (this->fEventToken || this->fEvent) return false;

  /* the sample to use has to be valid */
  if(!sample) return false;

  /* try to get a tree token */
  this->fEventToken = sample->getEventToken();
  if (!this->fEventToken) return false;

  this->fSample = sample;
  this->fEventToken->setOwner(this);
  xAOD::TEvent* event = static_cast<xAOD::TEvent*>(this->fEventToken->getContent());
  DEBUGclass("retrieved token %x for xAOD::TEvent in '%s'",this->fEvent,this->GetName());

  if(!event){
    ERRORclass("unable to retrieve xAOD::TEvent object!");
    return false;
  }
  this->fEvent = new asg::SgTEvent(event,xAOD::TActiveStore::store());
  
  this->fIsInitialized = this->initializeSelf();

  if(!this->fIsInitialized){
    ERRORclass("error initializing '%s', returning token again",this->GetName());
    if(this->fEventToken && fSample) this->fSample->returnToken(fEventToken);
    this->fEventToken = 0;
    this->fSample = 0;
    delete this->fEvent;
    this->fEvent = 0;
  }

  return this->fIsInitialized;
}


//______________________________________________________________________________________________

bool TQEventObservable::finalize() {
  // finalize this observable, return the event token
  DEBUGclass("finalizing");
  if(!this->fIsInitialized){
    return true;
  }
  if (!this->fEventToken) return false;
  if (!this->fSample) return false;
  if (!this->fEvent) return false;
  DEBUGclass("properly initialized, start finalization");
 
  bool ok = this->finalizeSelf();
  delete this->fEvent;
  this->fIsInitialized = !(this->fSample->returnToken(this->fEventToken) && ok);
  if(!(this->fIsInitialized)){
    DEBUGclass("tokens successfully returned for '%s'",this->GetName());
  } else {
    ERRORclass("error returning tokens for '%s'",this->GetName());
  }
 
  this->fEventToken = 0;
  this->fSample = 0;
  this->fEvent = 0;
 
  return (!this->fIsInitialized);
}


//______________________________________________________________________________________________

TQEventObservable::~TQEventObservable() {
  // standard destructor
}

//______________________________________________________________________________________________

void TQEventObservable::print() const {
  // print the contents of this observable and its associated branches
  std::cout << TQStringUtils::makeBoldYellow(this->getExpression()) << std::endl;
}

//______________________________________________________________________________________________

Long64_t TQEventObservable::getCurrentEntry() const {
  // retrieve the current entry from the TEvent
  return this->fEvent->event()->m_entry;
}

//______________________________________________________________________________________________

bool TQEventObservable::initializeSelf(){
  // initialize self -- empty by default
  return true;
}

//______________________________________________________________________________________________

bool TQEventObservable::finalizeSelf(){
  // finalize self -- empty by default
  return true;
}

//______________________________________________________________________________________________

TObjArray* TQEventObservable::getBranchNames() const {
  // return branch names -- not applicable for event observables
  return NULL;
}

//______________________________________________________________________________________________

bool TQEventObservable::parseParticleReference(TString& expression, TString& containerName, unsigned int& containerIndex){
  TString origExpr = expression;
  if(!TQStringUtils::readToken(expression,containerName,TQStringUtils::alphanumvarext)){
    ERRORclass("unable to read container name from expression '%s'",origExpr.Data());
    return false;
  }
  TString buf;
  if(!TQStringUtils::readBlock(expression,buf,"[]")){
    ERRORclass("unable to read container index from expression '%s'",origExpr.Data());
    return false;
  }
  if(!TQStringUtils::isInteger(buf)){
    ERRORclass("invalid container index in expression '%s'",origExpr.Data());
    return false;
  }	
	
  containerIndex = buf.Atoi();
	
	return true;
}


//______________________________________________________________________________________________
bool TQEventObservable::parseParticleReference(TString& expression, TString& containerName, unsigned int& containerIndex, std::vector<unsigned int>& particleIndices, const TString& stopatchars){
  std::vector<bool> dummy;
  TString origExpr = expression;
  bool retval =  parseParticleReference(expression, containerName, containerIndex, particleIndices, dummy, stopatchars);
  for (size_t i=0; i<dummy.size(); i++) {
    if (dummy.at(i)) {
      ERRORclass("It seems you tried reading information from an 'otherParts' constituent using an observable which does not support this! The problematic expression is '%s'",origExpr.Data());
      return false;
    }
  }
  
  return retval;
}


bool TQEventObservable::parseParticleReference(TString& expression, TString& containerName, unsigned int& containerIndex, std::vector<unsigned int>& particleIndices, std::vector<bool>& useOtherParts, const TString& stopatchars){
  TString origExpr = expression;
	if(!parseParticleReference(expression,containerName,containerIndex)){
		return false;
	}
	
  TString buf;
  while(!(expression.IsNull() || stopatchars.Contains(expression(0,1)))){
    buf.Clear();
    if(!TQStringUtils::removeLeading(expression,".->")){
      ERRORclass("unable to parse dereferenciation operator from expression '%s'",origExpr.Data());
      return false;
    }
    if(TQStringUtils::removeLeadingText(expression,"part")) { useOtherParts.push_back(false); }
    else if (TQStringUtils::removeLeadingText(expression,"otherPart")) { useOtherParts.push_back(true); }
    else {
      ERRORclass("unable to parse call to CompositeParticle::part(int) or CompositeParticle::otherPart(int) in '%s'",origExpr.Data());
      return false;
    }
    if(!TQStringUtils::readBlock(expression,buf,"()")){
      ERRORclass("unable to parse function call braces in call to CompositeParticle::part(int) in '%s'",origExpr.Data());
      return false;
    }
    if(!TQStringUtils::isInteger(buf)){
      ERRORclass("unable to parse integar argument in call to CompositeParticle::part(int) in '%s'",origExpr.Data());
      return false;
    }
    int idx = buf.Atoi();
    particleIndices.push_back(idx);
  }
  return true;
}
