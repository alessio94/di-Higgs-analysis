#include "CAFxAODUtils/TQIParticleFourVectorObservable.h"
#include <limits>

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// you can perform debug printouts with statements like this
// DEBUG("error number %d occurred",someInteger);

#include "QFramework/TQSample.h"

// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

/*@observable: [TQIParticleFourVectorObservable] The TQIParticleFourVectorObservable allows to access basic four-vector components of xAOD::IParticle objects. It is automatically created when encountering expressions of the form 'P4:nameOfContainer[indexInContainer].pt()'. Available quantities are pt(), eta(), phi(), m(), e().
*/

ClassImp(TQIParticleFourVectorObservable)

//______________________________________________________________________________________________

TQIParticleFourVectorObservable::TQIParticleFourVectorObservable(){
  // default constructor
  DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

TQIParticleFourVectorObservable::~TQIParticleFourVectorObservable(){
  // default destructor
  DEBUGclass("destructor called");
} 

//______________________________________________________________________________________________

double TQIParticleFourVectorObservable::getValue() const {
  // value retrieval function, called on every event for every cut and histogram
  DEBUGclass("entering function");
  this->retrieve(this->mContainer, fContainerName.Data());

  if( fIndex >= mContainer->size()){
        ERRORclass("unable to retrieve entry %d from container %s. This Observable's expression is '%s' (active expression: '%s')",this->fIndex,this->fContainerName.Data(),this->getExpression().Data(), this->getActiveExpression().Data());
    return 0;	
  }
  
  const IParticle* p = this->mContainer->at(fIndex);
  
  switch(this->fQuantity){
  case PT:
    return p->pt();
  case ETA:
    return p->eta();
  case PHI:
    return p->phi();
  case M:
    return p->m();
  case E:
    return p->e();
  default:
    return 0;
  }
}
//______________________________________________________________________________________________

TQIParticleFourVectorObservable::TQIParticleFourVectorObservable(const TString& expression):
TQEventObservable(expression)
{
  // constructor with expression argument
  DEBUGclass("constructor called with '%s'",expression.Data());
  // the predefined string member "expression" allows your observable to store an expression of your choice
  // this string will be the verbatim argument you passed to the constructor of your observable
  // you can use it to choose between different modes or pass configuration options to your observable
  this->SetName(TQObservable::makeObservableName(expression));
  this->setExpression(expression);
}

//______________________________________________________________________________________________

const TString& TQIParticleFourVectorObservable::getExpression() const {
  // retrieve the expression associated with this observable
  return this->fExpression;
}

//______________________________________________________________________________________________

bool TQIParticleFourVectorObservable::hasExpression() const {
  // check if this observable type knows expressions
  return true;
}

//______________________________________________________________________________________________

void TQIParticleFourVectorObservable::setExpression(const TString& expr){
  // set the expression to a given string
  this->fExpression = expr;
}
//______________________________________________________________________________________________

bool TQIParticleFourVectorObservable::parseExpression(const TString& expr){
  // parse the expression
  TString expression (expr);
  if(!TQEventObservable::parseParticleReference(expression,this->fContainerName,this->fIndex)){
    return false;
  }
  expression.ToLower();
  if(expression.Contains("pt")){
    this->fQuantity = PT;
  }
  else if(expression.Contains("eta")){
    this->fQuantity = ETA;
  }
  else if(expression.Contains("phi")){
    this->fQuantity = PHI;
  }
  else if(expression.Contains("m")){
    this->fQuantity = M;
  }
  else if(expression.Contains("e")){
    this->fQuantity = E;
  }
  else{
    return false;
  }
  return true;
}

//______________________________________________________________________________________________

void TQIParticleFourVectorObservable::clearParsedExpression(){
  // clear the current expression
  this->fContainerName.Clear();
  this->fIndex = 0;
  this->fQuantity = UNKNOWN;
}

//______________________________________________________________________________________________

TString TQIParticleFourVectorObservable::getActiveExpression() const {
  // retrieve the expression associated with this incarnation
  return "";
}

//______________________________________________________________________________________________

bool TQIParticleFourVectorObservable::initializeSelf(){
  // initialize self - compile container name, construct accessor
  if(!this->parseExpression(TQObservable::compileExpression(this->fExpression,this->fSample))){
    return false;
  }
  return true;
}
 
//______________________________________________________________________________________________

bool TQIParticleFourVectorObservable::finalizeSelf(){
  // finalize self - delete accessor
  this->clearParsedExpression();
  return true;
}


//______________________________________________________________________________________________

DEFINE_OBSERVABLE_FACTORY(TQIParticleFourVectorObservable,TString expression){
  // a factory for this observable type
  if(TQStringUtils::removeLeadingText(expression,"P4:")){
    return new TQIParticleFourVectorObservable(expression);
  }
  return NULL;
}

                          
