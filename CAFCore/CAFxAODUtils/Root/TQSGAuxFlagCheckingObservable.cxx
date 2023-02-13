#include "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h"
#include <limits>
#include <sstream>
#include <stdexcept>

#include "QFramework/TQSample.h"

//#define _DEBUG_
#include "QFramework/TQLibrary.h"


/*@observable: [TQSGAuxFlagCheckingObservable] The TQSGAuxFlagCheckingObservable can be used to check the existence of decorations of SG::AuxElement objects (such as the EventInfo object),
*/


templateClassImp(TQSGAuxFlagCheckingObservable<Taux>)

//______________________________________________________________________________________________

template <class Taux>
TQSGAuxFlagCheckingObservable<Taux>::TQSGAuxFlagCheckingObservable(){
  // default constructor
  DEBUGclass("default constructor called for FlagChecking");
}

//______________________________________________________________________________________________

template <class Taux>
TQSGAuxFlagCheckingObservable<Taux>::~TQSGAuxFlagCheckingObservable(){
  // default destructor
  DEBUGclass("destructor called for FlagChecking");
}

//______________________________________________________________________________________________


template <class Taux>
double TQSGAuxFlagCheckingObservable<Taux>::getValue() const {
  // value retrieval function, called on every event for every cut and histogram
  DEBUGclass("entering function for FlagChecking");

  this->retrieve(this->mContainer, this->fContainerName.Data());

  DEBUGclass("reading aux-data (object-level)");
  if (mContainer->isAvailable< Taux >( fDecoration.Data() )){
      DEBUGclass("reading aux-data returns 1");
      DEBUGclass(fDecoration.Data());
      return 1;
  }
  else{
      DEBUGclass("reading aux-data returns 0");
      DEBUGclass(fDecoration.Data());
      return 0;}
}

//______________________________________________________________________________________________//


template <class Taux>
TQSGAuxFlagCheckingObservable<Taux>::TQSGAuxFlagCheckingObservable(const TString& expression):
TQEventObservable(expression)
{
  // constructor with expression argument
  DEBUGclass("constructor called with '%s'",expression.Data());
  this->setExpression(expression);
  this->SetName(TQObservable::makeObservableName(this->getExpression()));
}

//______________________________________________________________________________________________

template <class Taux>
const TString& TQSGAuxFlagCheckingObservable<Taux>::getExpression() const {
  return this->fExpression;
}

//______________________________________________________________________________________________

template <class Taux>
TString TQSGAuxFlagCheckingObservable<Taux>::getActiveExpression() const {
  // retrieve the expression associated with this observable
  if(this->fCachedExpression.IsNull()){
    this->fCachedExpression = TString::Format("%s:%s",this->getExpressionPrefix().Data(),this->fContainerName.Data());
    this->fCachedExpression.Append(":");
    this->fCachedExpression.Append(this->fDecoration);
  }
  return this->fCachedExpression;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQSGAuxFlagCheckingObservable<Taux>::hasExpression() const {
  // check if this observable type knows expressions
  return true;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQSGAuxFlagCheckingObservable<Taux>::initializeSelf(){
  // initialize self - compile container name, construct accessor
  if(!this->parseExpression(TQObservable::compileExpression(this->fExpression,this->fSample))){
    return false;
  }
  DEBUGclass("CHECKING: %s",this->fDecoration.Data());
  this->fDecoAccess = new SG::AuxElement::Accessor<Taux>(this->fDecoration.Data());
  return true;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQSGAuxFlagCheckingObservable<Taux>::finalizeSelf(){
  // finalize self - delete accessor
  if(this->fDecoAccess){
    delete this->fDecoAccess;
    this->fDecoAccess = 0;
  }
  this->clearParsedExpression();
  return true;
}

//______________________________________________________________________________________________

template <class Taux>
void TQSGAuxFlagCheckingObservable<Taux>::clearParsedExpression(){
  // clear the current expression
  this->fContainerName.Clear();
  this->fDecoration.Clear();
  this->fCachedExpression.Clear();
}

//______________________________________________________________________________________________

template <class Taux>
void TQSGAuxFlagCheckingObservable<Taux>::setExpression(const TString& expr){
  this->fExpression = expr;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQSGAuxFlagCheckingObservable<Taux>::parseExpression(const TString& expr){
  TString input(expr);
  // set the expression to a given string
  this->clearParsedExpression();
  if(!TQStringUtils::readToken(input,this->fContainerName,TQStringUtils::alphanumvarext)){
    ERRORclass("unable to read container name from expression '%s'",input.Data());
    ERRORclass("unable to parse expression '%s'",expr.Data());
    return false;
  }
  if(TQStringUtils::removeLeading(input,":") != 1){
    ERRORclass("missing auxdata declaration in expression",expr.Data());
    return false;
  }
  this->fDecoration = input;
  DEBUGclass("Initialized TQSGAuxFlagCheckingObservable with : %s, %s",this->fContainerName.Data(),this->fDecoration.Data());
  this->getExpression();
  return true;
}

#include "CAFxAODUtils/Observables.h"

PREFIX_ALL_TEMPLATES(TQSGAuxFlagCheckingObservable,hasSGAux,Dec)

DEFINE_TEMPLATE_OBSERVABLE_FACTORY(TQSGAuxFlagCheckingObservable,Taux,TString expression){
  // a factory for this observable type
  if(TQStringUtils::removeLeadingText(expression,TQSGAuxFlagCheckingObservable<Taux>::getExpressionPrefix()+":")){
    return new TQSGAuxFlagCheckingObservable<Taux>(expression);
  }
  return NULL;
}

INSTANTIATE_ALL_TEMPLATES(TQSGAuxFlagCheckingObservable)
