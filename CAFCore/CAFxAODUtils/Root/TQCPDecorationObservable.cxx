#include "CAFxAODUtils/TQCPDecorationObservable.h"
#include <limits>
#include <sstream>
#include <stdexcept>

#include "QFramework/TQSample.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"
/*@observable: [TQCPDecorationObservable] The TQCPDecorationObservable can be used to read the value of decorations of xAOD::IParticle objects, possibly nested inside xAOD::CompositeParticle objects.
  It is automatically created when encountering expressions like 'CP?D:nameOfContainer[index]:nameOfDecoration'. The '?' needs to indicate the correct data type of the decoration to be read. Supported types are type/? = float/F, double/D, char/C, int/I, unsigned int/U, unsigned short/US, unsinged long long/ULL. Full examples: 'CPCD:Event$(cand)[0]:PassLooseJetEventCleaning', 'CPUSD:Event$(cand)[0].part(0):author'
*/

templateClassImp(TQCPDecorationObservable<T>)

//______________________________________________________________________________________________

template <class T>
TQCPDecorationObservable<T>::TQCPDecorationObservable(){
  // default constructor
  DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

template <class T>
TQCPDecorationObservable<T>::~TQCPDecorationObservable(){
  // default destructor
  DEBUGclass("destructor called");
}

//______________________________________________________________________________________________

template <class T>
double TQCPDecorationObservable<T>::getValue() const {
  // value retrieval function, called on every event for every cut and histogram
  DEBUGclass("entering function");
  this->retrieve(this->mContainer, this->fContainerName.Data());
  
  if(this->mContainer->size() <= this->fContainerIndex){
    ERRORclass("unable to retrieve entry %d from container %s. This Observable's expression is '%s' (active expression: '%s')",this->fContainerIndex,this->fContainerName.Data(),this->getExpression().Data(), this->getActiveExpression().Data());
    return 0;
  }
  
  const xAOD::CompositeParticle* p = this->mContainer->at(this->fContainerIndex);

  const size_t nLevels = fParticleIndices.size();
  if(nLevels > 0){
    // we loop over the particle indices to derefence the correct number of levels
    // we only need to loop over n-1 levels because the last one is not necessarily a composite particle
    const size_t n = nLevels-1;
    for(size_t i=0; i<n; ++i){
      // retrieve composite particle child
      p = this->fUseOtherParts[i] ? p->otherCompPart(fParticleIndices[i]) : p->compPart(fParticleIndices[i]);
#ifndef _DEBUG_
      if(!p){
        std::stringstream ss;
        ss << "for event " << this->getCurrentEntry();
        ss << ": unable to cast to xAOD::CompositeParticle: " << this->fContainerName << "[" << this->fContainerIndex << "]";
        for(size_t j=0; j<i; ++j){
          ss << (this->fUseOtherParts[j]? "->otherPart(":"->part(") << this->fParticleIndices[j] << ")";
        }
        throw std::runtime_error(ss.str());
      }
#endif
    }
    DEBUGclass("reading aux-data (non-zero level)");
    // we now have the last level of composite particle in our hand
    // we get the part that is referenced by the index and return its auxdata member
    const xAOD::IParticle* pp = this->fUseOtherParts[n] ? p->otherPart(fParticleIndices[n]) : p->part(fParticleIndices[n]);
    if(!this->fDecoAccess->isAvailable(*pp)){
      return std::numeric_limits<double>::quiet_NaN();
    }
    const double retval = (*(this->fDecoAccess))(*pp);
    DEBUGclass("returning (non-zero level)");
    return retval;
  } else {
    // if the level is 0, we just get the aux data member from the composite particle itself
    DEBUGclass("reading aux-data (zero-level)");
    if(!this->fDecoAccess->isAvailable(*p)){
      return std::numeric_limits<double>::quiet_NaN();
    }
    const double retval = (*(this->fDecoAccess))(*p);
    DEBUGclass("returning (zero-level)");
    return retval;
  }
}
//______________________________________________________________________________________________

template <class T>
TQCPDecorationObservable<T>::TQCPDecorationObservable(const TString& expression):
TQEventObservable(expression)
{
  // constructor with expression argument
  DEBUGclass("constructor called with '%s'",expression.Data());
  this->setExpression(expression);
  this->SetName(TQObservable::makeObservableName(this->getExpression()));
}

//______________________________________________________________________________________________

template <class T>
const TString& TQCPDecorationObservable<T>::getExpression() const {
  return this->fExpression;
}

//______________________________________________________________________________________________

template <class T>
TString TQCPDecorationObservable<T>::getActiveExpression() const {
  // retrieve the expression associated with this observable
  if(this->fCachedExpression.IsNull()){
    this->fCachedExpression = TString::Format("%s:%s[%d]",TQCPDecorationObservable<T>::getExpressionPrefix().Data(),this->fContainerName.Data(),(int)(this->fContainerIndex));
    for(size_t i=0; i<this->fParticleIndices.size(); ++i){
      this->fCachedExpression.Append(this->fUseOtherParts[i] ? TString::Format(".otherPart(%d)",(int)(this->fParticleIndices[i])) : TString::Format(".part(%d)",(int)(this->fParticleIndices[i])));
    }
    this->fCachedExpression.Append(":");
    this->fCachedExpression.Append(this->fDecoration);
  }
  return this->fCachedExpression;
}

//______________________________________________________________________________________________

template <class T>
bool TQCPDecorationObservable<T>::hasExpression() const {
  // check if this observable type knows expressions
  return true;
}

//______________________________________________________________________________________________

template <class T>
bool TQCPDecorationObservable<T>::initializeSelf(){
  // initialize self - compile container name, construct accessor
  if(!this->parseExpression(TQObservable::compileExpression(this->fExpression,this->fSample))){
    return false;
  }
  this->fDecoAccess = new SG::AuxElement::Accessor<T>(this->fDecoration.Data());
  return true;
}

//______________________________________________________________________________________________

template <class T>
bool TQCPDecorationObservable<T>::finalizeSelf(){
  // finalize self - delete accessor
  if(this->fDecoAccess){
    delete this->fDecoAccess;
    this->fDecoAccess = 0;
  }
  this->clearParsedExpression();
  return true;
}

//______________________________________________________________________________________________

template <class T>
void TQCPDecorationObservable<T>::clearParsedExpression(){
  // clear the current expression
  this->fContainerName.Clear();
  this->fDecoration.Clear();
  this->fContainerIndex = 0;
  this->fParticleIndices.clear();
  this->fUseOtherParts.clear();
  this->fCachedExpression.Clear();
}

//______________________________________________________________________________________________

template <class T>
void TQCPDecorationObservable<T>::setExpression(const TString& expr){
  this->fExpression = expr;
}

//______________________________________________________________________________________________

template <class T>
bool TQCPDecorationObservable<T>::parseExpression(const TString& expr){
  TString input(expr);
  // set the expression to a given string
  this->clearParsedExpression();
  bool ok = TQEventObservable::parseParticleReference(input,this->fContainerName,this->fContainerIndex,this->fParticleIndices,this->fUseOtherParts,":");
  if(!ok){
    ERRORclass("unable to parse expression '%s'",(int)(this->fParticleIndices.size()),expr.Data());
    return false;
  }
  if(TQStringUtils::removeLeading(input,":") != 1){
    ERRORclass("missing auxdata declaration in expression",expr.Data());
    return false;
  }
  this->fDecoration = input;
  this->getExpression();
  return true;
}

#include "CAFxAODUtils/Observables.h"

PREFIX_ALL_TEMPLATES(TQCPDecorationObservable,CP,D)

DEFINE_TEMPLATE_OBSERVABLE_FACTORY(TQCPDecorationObservable,T,TString expression){
  // a factory for this observable type
  if(TQStringUtils::removeLeadingText(expression,TQCPDecorationObservable<T>::getExpressionPrefix()+":")){
    return new TQCPDecorationObservable<T>(expression);
  }
  return NULL;
}

INSTANTIATE_ALL_TEMPLATES(TQCPDecorationObservable)
