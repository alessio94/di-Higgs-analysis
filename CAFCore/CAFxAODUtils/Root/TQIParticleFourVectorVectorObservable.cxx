#include "CAFxAODUtils/TQIParticleFourVectorVectorObservable.h"
#include <limits>

#include "QFramework/TQSample.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// you can perform debug printouts with statements like this
// DEBUG("error number %d occurred",someInteger);

// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

/*@observable: [TQIParticleFourVectorVectorObservable] The TQIParticleFourVectorVectorObservable allows to access basic four-vector components of all xAOD::IParticle instances in a container. It is automatically created when encountering expressions of the form 'P4V:nameOfContainer.pt()'. Available quantities are pt(), eta(), phi(), m(), e(). In contrast to the TQIParticleFourVectorObservable (only one 'Vector'!) this observable is a VectorObservable as it provides (possibly) more than one value per event, specifically one per IParticle in the specified container.
*/


ClassImp(TQIParticleFourVectorVectorObservable)

//______________________________________________________________________________________________

TQIParticleFourVectorVectorObservable::TQIParticleFourVectorVectorObservable(){
  // default constructor
  DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

TQIParticleFourVectorVectorObservable::~TQIParticleFourVectorVectorObservable(){
  // default destructor
  DEBUGclass("destructor called");
}

//______________________________________________________________________________________________

double TQIParticleFourVectorVectorObservable::getValue() const {
    throw std::runtime_error("Caught attempt to evaluate vector observable of type `TQIParticleFourVectorVectorObservable` in scalar context!");
    return -999;
}

//______________________________________________________________________________________________

bool TQIParticleFourVectorVectorObservable::makeCache() const {
  if (this->getCurrentEntry() == this->fCachedEntry) {
    return true;  // nothing to do here, already created cache
  }
  this->retrieve(this->m_container, fContainerName.Data());

  if (!m_container) {
    ERRORclass("Failed to retrieve container '%s'", fContainerName.Data());
    return false;
  }
  this->fCache.clear();  // reset old cache
  // create new cache
  for (const xAOD::IParticle * p : *m_container) {
    double val = -999;
    switch (this->fQuantity) {
    case PT:
      val = p->pt();
      break;
    case ETA:
      val = p->eta();
      break;
    case PHI:
      val = p->phi();
      break;
    case M:
      val = p->m();
      break;
    case E:
      val = p->e();
      break;
    default:
      val = -999;
    }
    this->fCache.push_back(val);
  }
  this->fCachedEntry = this->getCurrentEntry();
  return true;
}

//______________________________________________________________________________________________

double TQIParticleFourVectorVectorObservable::getValueAt(int index) const {
  // value retrieval function, called on every event for every cut and histogram
  DEBUGclass("entering function");

  if (!this->makeCache()) {
    ERRORclass("Failed to create cache in 'TQIParticleFourVectorVectorObservable' observable with container %s!", fContainerName.Data());
    return std::numeric_limits<double>::quiet_NaN();
  }
  if (index >= (int)this->fCache.size()) {
    throw std::runtime_error("Caught attempt to evaluate TQIParticleFourVectorVectorObservable observable out of bounds!");
    return -999;
  }

  return this->fCache.at(index);
}

//______________________________________________________________________________________________

int TQIParticleFourVectorVectorObservable::getNevaluations() const {
  this->makeCache();
  return this->fCache.size();
}

//______________________________________________________________________________________________

TQIParticleFourVectorVectorObservable::TQIParticleFourVectorVectorObservable(const TString& expression):
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

const TString& TQIParticleFourVectorVectorObservable::getExpression() const {
  // retrieve the expression associated with this observable
  return this->fExpression;
}

//______________________________________________________________________________________________

bool TQIParticleFourVectorVectorObservable::hasExpression() const {
  // check if this observable type knows expressions
  return true;
}

//______________________________________________________________________________________________

void TQIParticleFourVectorVectorObservable::setExpression(const TString& expr){
  // set the expression to a given string
  this->fExpression = expr;
}

//______________________________________________________________________________________________

bool TQIParticleFourVectorVectorObservable::parseContainerName(TString& expression, TString& containerName) {
  TString origExpr = expression;
  if (!TQStringUtils::readToken(expression, containerName, TQStringUtils::alphanumvarext)) {
    ERRORclass("unable to read container name from expression '%s'", origExpr.Data());
    return false;
  }
  return true;
}

bool TQIParticleFourVectorVectorObservable::parseExpression(const TString& expr){
  // parse the expression
  TString expression (expr);
  if(!TQIParticleFourVectorVectorObservable::parseContainerName(expression, this->fContainerName)) {
    return false;
  }
  expression.ToLower();
  if(expression.Contains("pt")) {
    this->fQuantity = PT;
  }
  else if(expression.Contains("eta")) {
    this->fQuantity = ETA;
  }
  else if(expression.Contains("phi")) {
    this->fQuantity = PHI;
  }
  else if(expression.Contains("m")) {
    this->fQuantity = M;
  }
  else if(expression.Contains("e")) {
    this->fQuantity = E;
  }
  else {
    return false;
  }
  return true;
}

//______________________________________________________________________________________________

void TQIParticleFourVectorVectorObservable::clearParsedExpression(){
  // clear the current expression
  this->fContainerName.Clear();
  this->fQuantity = UNKNOWN;
}

//______________________________________________________________________________________________

TString TQIParticleFourVectorVectorObservable::getActiveExpression() const {
  // retrieve the expression associated with this incarnation
  return "";
}

//______________________________________________________________________________________________

bool TQIParticleFourVectorVectorObservable::initializeSelf(){
  // initialize self - compile container name, construct accessor
  if(!this->parseExpression(TQObservable::compileExpression(this->fExpression, this->fSample))){
    return false;
  }
  return true;
}

//______________________________________________________________________________________________

bool TQIParticleFourVectorVectorObservable::finalizeSelf(){
  // finalize self
  this->clearParsedExpression();
  this->fCache.clear();
  return true;
}


//______________________________________________________________________________________________

DEFINE_OBSERVABLE_FACTORY(TQIParticleFourVectorVectorObservable,TString expression){
  // a factory for this observable type
  if(TQStringUtils::removeLeadingText(expression,"P4V:")){
    return new TQIParticleFourVectorVectorObservable(expression);
  }
  return NULL;
}
