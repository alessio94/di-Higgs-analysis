#include "CAFxAODUtils/TQSGAuxElementDecorationObservable.h"
#include <limits>
#include <sstream>
#include <stdexcept>

#include "QFramework/TQSample.h"

//#define _DEBUG_
#include "QFramework/TQLibrary.h"


/*@observable: [TQSGAuxElementDecorationObservable] The TQSGAuxElementDecorationObservable can be used to read the value of decorations of SG::AuxElement objects (such as the EventInfo object),
  It is automatically created when encountering expressions like 'SGAux?Dec:nameOfAuxElement:nameOfDecoration'. The '?' needs to indicate the correct data type of the decoration to be read. Supported types are type/? = float/F, double/D, char/C, int/I, unsigned int/U, unsigned short/US, unsinged long long/ULL. Full example: 'SGAuxIDec:EventInfo:HTXS_Stage0_Category'
*/


templateClassImp(TQSGAuxElementDecorationObservable<Taux>)

//______________________________________________________________________________________________

template <class Taux>
TQSGAuxElementDecorationObservable<Taux>::TQSGAuxElementDecorationObservable(){
  // default constructor
  DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

template <class Taux>
TQSGAuxElementDecorationObservable<Taux>::~TQSGAuxElementDecorationObservable(){
  // default destructor
  DEBUGclass("destructor called");
}

//______________________________________________________________________________________________


template <class Taux>
double TQSGAuxElementDecorationObservable<Taux>::getValue() const {
  // value retrieval function, called on every event for every cut and histogram
  DEBUGclass("entering function");

  this->retrieve(this->mContainer, this->fContainerName.Data());

  DEBUGclass("reading aux-data (object-level)");
  if (mContainer->isAvailable< Taux >( fDecoration.Data() )) 
    return mContainer->auxdata< Taux > ( fDecoration.Data() );
  
  ERRORclass("%s:%s unavailable - returning NaN",this->fContainerName.Data(),this->fDecoration.Data());
  return std::numeric_limits<double>::quiet_NaN();
}
//______________________________________________________________________________________________

template <class Taux>
TQSGAuxElementDecorationObservable<Taux>::TQSGAuxElementDecorationObservable(const TString& expression):
TQEventObservable(expression)
{
  // constructor with expression argument
  DEBUGclass("constructor called with '%s'",expression.Data());
  this->setExpression(expression);
  this->SetName(TQObservable::makeObservableName(this->getExpression()));
}

//______________________________________________________________________________________________

template <class Taux>
const TString& TQSGAuxElementDecorationObservable<Taux>::getExpression() const {
  return this->fExpression;
}

//______________________________________________________________________________________________

template <class Taux>
TString TQSGAuxElementDecorationObservable<Taux>::getActiveExpression() const {
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
bool TQSGAuxElementDecorationObservable<Taux>::hasExpression() const {
  // check if this observable type knows expressions
  return true;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQSGAuxElementDecorationObservable<Taux>::initializeSelf(){
  // initialize self - compile container name, construct accessor
  if(!this->parseExpression(TQObservable::compileExpression(this->fExpression,this->fSample))){
    return false;
  }
  this->fDecoAccess = new SG::AuxElement::Accessor<Taux>(this->fDecoration.Data());
  return true;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQSGAuxElementDecorationObservable<Taux>::finalizeSelf(){
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
void TQSGAuxElementDecorationObservable<Taux>::clearParsedExpression(){
  // clear the current expression
  this->fContainerName.Clear();
  this->fDecoration.Clear();
  this->fCachedExpression.Clear();
}

//______________________________________________________________________________________________

template <class Taux>
void TQSGAuxElementDecorationObservable<Taux>::setExpression(const TString& expr){
  this->fExpression = expr;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQSGAuxElementDecorationObservable<Taux>::parseExpression(const TString& expr){
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
  DEBUGclass("Initialized TQSGAuxElementDecorationObservable with : %s, %s",this->fContainerName.Data(),this->fDecoration.Data());
  this->getExpression();
  return true;
}

#include "CAFxAODUtils/Observables.h"

PREFIX_ALL_TEMPLATES(TQSGAuxElementDecorationObservable,SGAux,Dec)

DEFINE_TEMPLATE_OBSERVABLE_FACTORY(TQSGAuxElementDecorationObservable,Taux,TString expression){
  // a factory for this observable type
  if(TQStringUtils::removeLeadingText(expression,TQSGAuxElementDecorationObservable<Taux>::getExpressionPrefix()+":")){
    return new TQSGAuxElementDecorationObservable<Taux>(expression);
  }
  return NULL;
}

INSTANTIATE_ALL_TEMPLATES(TQSGAuxElementDecorationObservable)
