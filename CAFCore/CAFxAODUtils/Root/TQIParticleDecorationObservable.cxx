#include "CAFxAODUtils/TQIParticleDecorationObservable.h"
#include <limits>
#include <sstream>
#include <stdexcept>

#include "QFramework/TQSample.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

/*@observable: [TQIParticleDecorationObservable] The TQIParticleDecorationObservable can be used to read the value of decorations of xAOD::IParticle objects,
  It is automatically created when encountering expressions like 'IP?D:nameOfContainer[index]:nameOfDecoration'. The '?' needs to indicate the correct data type of the decoration to be read. Supported types are type/? = float/F, double/D, char/C, int/I, unsigned int/U, unsigned short/US, unsinged long long/ULL. Full example: 'IPCD:Event$(cand)[0]:PassLooseJetEventCleaning' (note: this particular example is untested. The functionality of this observable is most likely fully covered by the TQCompositeParticleDecorationObservable as well)
*/

templateClassImp(TQIParticleDecorationObservable<Taux>)

//______________________________________________________________________________________________

template <class Taux>
TQIParticleDecorationObservable<Taux>::TQIParticleDecorationObservable(){
  // default constructor
  DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

template <class Taux>
TQIParticleDecorationObservable<Taux>::~TQIParticleDecorationObservable(){
  // default destructor
  DEBUGclass("destructor called");
}

//______________________________________________________________________________________________


template <class Taux>
double TQIParticleDecorationObservable<Taux>::getValue() const {
  // value retrieval function, called on every event for every cut and histogram
  DEBUGclass("entering function");
  // the following protective if-block should NEVER evaluate true
  // for performance reasons, we disable it per preprocessor directive unless in debugging mode
  #ifdef _DEBUG_
  if(!this->fEvent){
    ERRORclass("failed to access xAOD::TEvent member!");
    abort();
    return 0;
  }
  #endif

  this->retrieve(this->mContainer, this->fContainerName.Data());

  if(this->mContainer->size() <= this->fContainerIndex){
    ERRORclass("unable to retrieve entry %d from container %s. This Observable's expression is '%s' (active expression: '%s')",this->fContainerIndex,this->fContainerName.Data(),this->getExpression().Data(), this->getActiveExpression().Data());
    return 0;
  }
  
  DEBUGclass("reading aux-data (object-level)");
  const xAOD::IParticle* pp = this->mContainer->at(this->fContainerIndex);
  if (!fDecoAccess->isAvailable(*pp)){
    return std::numeric_limits<double>::quiet_NaN();
  } else {
    const double retval = (*(this->fDecoAccess))(*pp);
    return retval;
  }
}
//______________________________________________________________________________________________

template <class Taux>
TQIParticleDecorationObservable<Taux>::TQIParticleDecorationObservable(const TString& expression):
TQEventObservable(expression)
{
  // constructor with expression argument
  DEBUGclass("constructor called with '%s'",expression.Data());
  this->setExpression(expression);
  this->SetName(TQObservable::makeObservableName(this->getExpression()));
}

//______________________________________________________________________________________________

template <class Taux>
const TString& TQIParticleDecorationObservable<Taux>::getExpression() const {
  return this->fExpression;
}

//______________________________________________________________________________________________

template <class Taux>
TString TQIParticleDecorationObservable<Taux>::getActiveExpression() const {
  // retrieve the expression associated with this observable
  if(this->fCachedExpression.IsNull()){
    this->fCachedExpression = TString::Format("%s:%s",TQIParticleDecorationObservable<Taux>::getExpressionPrefix().Data(),this->fContainerName.Data());
    this->fCachedExpression.Append(":");
    this->fCachedExpression.Append(this->fDecoration);
  }
  return this->fCachedExpression;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQIParticleDecorationObservable<Taux>::hasExpression() const {
  // check if this observable type knows expressions
  return true;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQIParticleDecorationObservable<Taux>::initializeSelf(){
  // initialize self - compile container name, construct accessor
  if(!this->parseExpression(TQObservable::compileExpression(this->fExpression,this->fSample))){
    return false;
  }
  this->fDecoAccess = new SG::AuxElement::Accessor<Taux>(this->fDecoration.Data());
  return true;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQIParticleDecorationObservable<Taux>::finalizeSelf(){
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
void TQIParticleDecorationObservable<Taux>::clearParsedExpression(){
  // clear the current expression
  this->fContainerName.Clear();
  this->fDecoration.Clear();
  this->fCachedExpression.Clear();
}

//______________________________________________________________________________________________

template <class Taux>
void TQIParticleDecorationObservable<Taux>::setExpression(const TString& expr){
  this->fExpression = expr;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQIParticleDecorationObservable<Taux>::parseExpression(const TString& expr){
  TString input(expr);
  // set the expression to a given string
  this->clearParsedExpression();

  if(!TQEventObservable::parseParticleReference(input,this->fContainerName,this->fContainerIndex)){
    ERRORclass("unable to read particle reference from expression '%s'",expr.Data());
    return false;
  }
  if(TQStringUtils::removeLeading(input,":") != 1){
    ERRORclass("missing auxdata declaration in expression",expr.Data());
    return false;
  }
  this->fDecoration = input;
  DEBUGclass("Initialized TQIParticleDecorationObservable with : %s[%d]:%s",this->fContainerName.Data(),this->fContainerIndex,this->fDecoration.Data());
  return true;
}

#include "CAFxAODUtils/Observables.h"

PREFIX_ALL_TEMPLATES(TQIParticleDecorationObservable,IP,D)

DEFINE_TEMPLATE_OBSERVABLE_FACTORY(TQIParticleDecorationObservable,Taux,TString expression){
  // a factory for this observable type
  if(TQStringUtils::removeLeadingText(expression,TQIParticleDecorationObservable<Taux>::getExpressionPrefix()+":")){
    return new TQIParticleDecorationObservable<Taux>(expression);
  }
  return NULL;
}

INSTANTIATE_ALL_TEMPLATES(TQIParticleDecorationObservable)
