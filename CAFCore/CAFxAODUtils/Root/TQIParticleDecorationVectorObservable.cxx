#include "CAFxAODUtils/TQIParticleDecorationVectorObservable.h"
#include <limits>
#include <sstream>
#include <stdexcept>
#include <algorithm>

#include "QFramework/TQSample.h"

//#define _DEBUG_
#include "QFramework/TQLibrary.h"
#include "xAODBase/IParticleContainer.h"


/*@observable: [TQIParticleDecorationVectorObservable] The TQIParticleDecorationVectorObservable can be used to read the value of decorations of all xAOD::IParticle objects in one container. It is therefore a VectorObservable with number of evaluations equal to the size of the container.
  It is automatically created when encountering expressions like 'IP?VD:nameOfContainer:nameOfDecorationOnIndividualObjects'. The '?' needs to indicate the correct data type of the decoration to be read. Supported types are type/? = float/F, double/D, char/C, int/I, unsigned int/U, unsigned short/US, unsinged long long/ULL. Full example: 'IPIVD:Electrons[0]:truthType'. If the index specified (in the example '0') is less than 1 the entire container is used, otherwise only elements of the container up to the specified index are considered.
*/


templateClassImp(TQIParticleDecorationVectorObservable<Taux>)

//______________________________________________________________________________________________

template <class Taux>
TQIParticleDecorationVectorObservable<Taux>::TQIParticleDecorationVectorObservable(){
  // default constructor
  DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

template <class Taux>
TQIParticleDecorationVectorObservable<Taux>::~TQIParticleDecorationVectorObservable(){
  // default destructor
  DEBUGclass("destructor called");
}

//______________________________________________________________________________________________
template <class Taux>
int TQIParticleDecorationVectorObservable<Taux>::getNevaluations() const {
  //Caching:
  if (this->fCachedEntry != this->getCurrentEntry()) {
    this->retrieve(this->mContainer, this->fContainerName.Data());
    this->fCachedEntry = this->getCurrentEntry();
  }
  if (this->fContainerMaxIndex<1) return this->mContainer->size();
  else return std::min(this->fContainerMaxIndex+1,(unsigned int)this->mContainer->size());
}


//______________________________________________________________________________________________

template <class Taux>
double TQIParticleDecorationVectorObservable<Taux>::getValueAt(int index) const {
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
  if (this->fCachedEntry != this->getCurrentEntry()) {
    this->retrieve(this->mContainer, this->fContainerName.Data());
    this->fCachedEntry = this->getCurrentEntry();
  }
  
  if(this->mContainer->size() <= (size_t)index){
    ERRORclass("unable to retrieve entry %d from container %s. This Observable's expression is '%s' (active expression: '%s')", index, this->fContainerName.Data(), this->getExpression().Data(), this->getActiveExpression().Data());
    return 0;
  }
  if (this->fContainerMaxIndex>0 && this->fContainerMaxIndex < (unsigned int)index) {
    ERRORclass("Found attempt to access index larger than specified max index '%d'. Use myContainer[0] to set no hard limit on the maximum index",this->fContainerMaxIndex);
    return 0;
  }
  
  
  DEBUGclass("reading aux-data (object-level)");
  const xAOD::IParticle* pp = this->mContainer->at(index);
  if (!fDecoAccess->isAvailable(*pp)){
    ERRORclass("Decoration access is not available for deciration named '%s'",this->fDecoration.Data());
    return std::numeric_limits<double>::quiet_NaN();
  } else {
    const double retval = (*(this->fDecoAccess))(*pp);
    return retval;
  }
}

//______________________________________________________________________________________________

template <class Taux>
double TQIParticleDecorationVectorObservable<Taux>::getValue() const 
{throw std::runtime_error("getValue called on vector observable. This is ambiguous, please use getValueAt(int)"); return -999.;}

//______________________________________________________________________________________________

template <class Taux>
TQIParticleDecorationVectorObservable<Taux>::TQIParticleDecorationVectorObservable(const TString& expression):
TQEventObservable(expression)
{
  // constructor with expression argument
  DEBUGclass("constructor called with '%s'",expression.Data());
  this->setExpression(expression);
  this->SetName(TQObservable::makeObservableName(this->getExpression()));
}

//______________________________________________________________________________________________

template <class Taux>
const TString& TQIParticleDecorationVectorObservable<Taux>::getExpression() const {
  return this->fExpression;
}

//______________________________________________________________________________________________

template <class Taux>
TString TQIParticleDecorationVectorObservable<Taux>::getActiveExpression() const {
  // retrieve the expression associated with this observable
  if(this->fCachedExpression.IsNull()){
    this->fCachedExpression = TString::Format("%s:%s",TQIParticleDecorationVectorObservable<Taux>::getExpressionPrefix().Data(),this->fContainerName.Data());
    this->fCachedExpression.Append(":");
    this->fCachedExpression.Append(this->fDecoration);
  }
  return this->fCachedExpression;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQIParticleDecorationVectorObservable<Taux>::hasExpression() const {
  // check if this observable type knows expressions
  return true;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQIParticleDecorationVectorObservable<Taux>::initializeSelf(){
  // initialize self - compile container name, construct accessor
  if(!this->parseExpression(TQObservable::compileExpression(this->fExpression,this->fSample))){
    return false;
  }
  this->fDecoAccess = new SG::AuxElement::Accessor<Taux>(this->fDecoration.Data());
  return true;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQIParticleDecorationVectorObservable<Taux>::finalizeSelf(){
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
void TQIParticleDecorationVectorObservable<Taux>::clearParsedExpression(){
  // clear the current expression
  this->fContainerName.Clear();
  this->fDecoration.Clear();
  this->fCachedExpression.Clear();
}

//______________________________________________________________________________________________

template <class Taux>
void TQIParticleDecorationVectorObservable<Taux>::setExpression(const TString& expr){
  this->fExpression = expr;
}

//______________________________________________________________________________________________

template <class Taux>
bool TQIParticleDecorationVectorObservable<Taux>::parseExpression(const TString& expr){
  TString input(expr);
  // set the expression to a given string
  this->clearParsedExpression();

  if(!TQEventObservable::parseParticleReference(input,this->fContainerName,this->fContainerMaxIndex)){
    ERRORclass("unable to read particle reference from expression '%s'",expr.Data());
    return false;
  }
  if(TQStringUtils::removeLeading(input,":") != 1){
    ERRORclass("missing auxdata declaration in expression",expr.Data());
    return false;
  }
  this->fDecoration = input;
  DEBUGclass("Initialized TQIParticleDecorationVectorObservable with : %s[%d]:%s",this->fContainerName.Data(),this->fContainerMaxIndex,this->fDecoration.Data());
  return true;
}

#include "CAFxAODUtils/Observables.h"

PREFIX_ALL_TEMPLATES(TQIParticleDecorationVectorObservable,IP,VD)

DEFINE_TEMPLATE_OBSERVABLE_FACTORY(TQIParticleDecorationVectorObservable,Taux,TString expression){
  // a factory for this observable type
  if(TQStringUtils::removeLeadingText(expression,TQIParticleDecorationVectorObservable<Taux>::getExpressionPrefix()+":")){
    return new TQIParticleDecorationVectorObservable<Taux>(expression);
  }
  return NULL;
}

INSTANTIATE_ALL_TEMPLATES(TQIParticleDecorationVectorObservable)
