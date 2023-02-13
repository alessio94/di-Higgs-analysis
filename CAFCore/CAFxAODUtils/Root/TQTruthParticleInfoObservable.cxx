#include "CAFxAODUtils/TQTruthParticleInfoObservable.h"
#include <limits>

#include "QFramework/TQSample.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// you can perform debug printouts with statements like this
// DEBUG("error number %d occurred",someInteger);

// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

/*@observable: [TQTruthParticleInfoObservable] The TQTruthParticleInfoObservable provides a way to access various information from TruthParticle instances. It is automatically created uppon encountering expressions of the form 'TRUTH:containerName[indexInContainer].infoToObtain()' where 'infoToObtain' can be one of the following: 'hasprodvtx', 'hasdecayvtx', 'nparents', 'nchildren', 'abspdgid', 'pdgid', 'status', 'charge'.
*/

ClassImp(TQTruthParticleInfoObservable)

//______________________________________________________________________________________________

TQTruthParticleInfoObservable::TQTruthParticleInfoObservable(){
  // default constructor
  DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

TQTruthParticleInfoObservable::~TQTruthParticleInfoObservable(){
  // default destructor
  DEBUGclass("destructor called");
}

//______________________________________________________________________________________________

double TQTruthParticleInfoObservable::getValue() const {
  // value retrieval function, called on every event for every cut and histogram
  DEBUGclass("entering function");
  this->retrieve(this->mContainer, fContainerName.Data());

  if( fIndex >= mContainer->size()){
    DEBUGclass("provided index %i not within container range %f!",fIndex,mContainer->size());
    return 0;
  }

  const TruthParticle* p = this->mContainer->at(fIndex);

  switch(this->fQuantity){
  case HASPRODVTX:
    return p->hasProdVtx();
  case HASDECAYVTX:
    return p->hasDecayVtx();
  case NPARENTS:
    return p->nParents();
  case NCHILDREN:
    return p->nChildren();
  case PDGID:
    return p->pdgId();
  case ABSPDGID:
    return p->absPdgId();
  case STATUS:
    return p->status();
  case CHARGE:
    return p->charge();
  default:
    return 0;
  }
}
//______________________________________________________________________________________________

TQTruthParticleInfoObservable::TQTruthParticleInfoObservable(const TString& expression):
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

const TString& TQTruthParticleInfoObservable::getExpression() const {
  // retrieve the expression associated with this observable
  return this->fExpression;
}

//______________________________________________________________________________________________

bool TQTruthParticleInfoObservable::hasExpression() const {
  // check if this observable type knows expressions
  return true;
}

//______________________________________________________________________________________________

void TQTruthParticleInfoObservable::setExpression(const TString& expr){
  // set the expression to a given string
  this->fExpression = expr;
}
//______________________________________________________________________________________________

bool TQTruthParticleInfoObservable::parseExpression(const TString& expr){
  // parse the expression
  TString expression (expr);
  if(!TQEventObservable::parseParticleReference(expression,this->fContainerName,this->fIndex)){
    return false;
  }
  expression.ToLower();
  if(expression.Contains("hasprodvtx")){
    this->fQuantity = HASPRODVTX;
  }
  else if(expression.Contains("hasdecayvtx")){
    this->fQuantity = HASDECAYVTX;
  }
  else if(expression.Contains("nparents")){
    this->fQuantity = NPARENTS;
  }
  else if(expression.Contains("nchildren")){
    this->fQuantity = NCHILDREN;
  }
  else if(expression.Contains("abspdgid")){  // this has to go before pdgid
    this->fQuantity = ABSPDGID;
  }
  else if(expression.Contains("pdgid")){
    this->fQuantity = PDGID;
  }
  else if(expression.Contains("status")){
    this->fQuantity = STATUS;
  }
  else if(expression.Contains("charge")){
    this->fQuantity = CHARGE;
  }
  else{
    return false;
  }
  return true;
}

//______________________________________________________________________________________________

void TQTruthParticleInfoObservable::clearParsedExpression(){
  // clear the current expression
  this->fContainerName.Clear();
  this->fIndex = 0;
  this->fQuantity = UNKNOWN;
}

//______________________________________________________________________________________________

TString TQTruthParticleInfoObservable::getActiveExpression() const {
  // retrieve the expression associated with this incarnation
  return "";
}

//______________________________________________________________________________________________

bool TQTruthParticleInfoObservable::initializeSelf(){
  // initialize self - compile container name, construct accessor
  if(!this->parseExpression(TQObservable::compileExpression(this->fExpression,this->fSample))){
    return false;
  }
  return true;
}

//______________________________________________________________________________________________

bool TQTruthParticleInfoObservable::finalizeSelf(){
  // finalize self - delete accessor
  this->clearParsedExpression();
  return true;
}


//______________________________________________________________________________________________

DEFINE_OBSERVABLE_FACTORY(TQTruthParticleInfoObservable,TString expression){
  // a factory for this observable type
  if(TQStringUtils::removeLeadingText(expression,"TRUTH:")){
    return new TQTruthParticleInfoObservable(expression);
  }
  return NULL;
}


