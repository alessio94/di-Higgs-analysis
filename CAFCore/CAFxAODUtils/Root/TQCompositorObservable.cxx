#include "CAFxAODUtils/TQCompositorObservable.h"

#include "QFramework/TQToken.h"
#include "QFramework/TQSample.h"

#include <limits>

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// you can perform debug printouts with statements like this
// DEBUG("error number %d occurred",someInteger);

// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include "xAODParticleEvent/CompositeParticleContainer.h"

/*@observable: [TQCompositorObservable] The TQCompositorObservable allows to dynamically build a TLorentzVector object from a list of individual xAOD::IParticle's kinematics and retrieve pt, eta, phi or m of the so constructed TLorentzVector. Syntax: 'CP(nameOfContainer[indexInContainer], nameOfOtherContainer[indexInOtherContainer], ...).pt()' (or .m(), .phi(), .eta())

*/

ClassImp(TQCompositorObservable)

//______________________________________________________________________________________________

TQCompositorObservable::TQCompositorObservable(){
  // default constructor
  DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

TQCompositorObservable::~TQCompositorObservable(){
  // default destructor
  DEBUGclass("destructor called");
} 

//______________________________________________________________________________________________

bool TQCompositorObservable::computeValue() const {
  // add the 4-momenta of the components
  this->fCompositeParticle.SetPxPyPzE(0,0,0,0);
  for(size_t i=0; i<this->fContainerNames.size(); ++i){
    const xAOD::CompositeParticleContainer* container = NULL;
    this->retrieve(container, this->fContainerNames[i].Data());
    const xAOD::CompositeParticle* cp = container->at(this->fContainerIndices[i]);
    const size_t nLevels = this->fParticleIndices[i].size();
    if(nLevels > 0){
      // we loop over the particle indices to derefence the correct number of levels
      // we only need to loop over n-1 levels because the last one is not necessarily a composite particle
      const size_t n = nLevels-1;
      for(size_t j=0; j<n; ++j){
      // retrieve composite particle child
        cp = cp->compPart(this->fParticleIndices[i][j]); 
#ifndef _DEBUG_
        if(!cp){
          std::stringstream ss;
          ss << "for event " << this->getCurrentEntry();
          ss << ": unable to cast to xAOD::CompositeParticle: " << this->fContainerNames[i] << "[" << this->fContainerIndices[i] << "]";
          for(size_t k=0; k<j; ++k){
            ss << "->part(" << this->fParticleIndices[i][k] << ")";
          }
          throw std::runtime_error(ss.str());
        }
#endif
      }
    }
    const xAOD::IParticle* p = cp->part(nLevels-1);
    this->fCompositeParticle += p->p4();
  }
  return true;
}

//______________________________________________________________________________________________

double TQCompositorObservable::getValue() const {
  // value retrieval function, called on every event for every cut and histogram
  DEBUGclass("entering function");

  this->computeValue();

  switch(this->f4Quantity){
  case MASS:
    return this->fCompositeParticle.M();
  case PHI:
    return this->fCompositeParticle.Phi();
  case ETA:
    return this->fCompositeParticle.Eta();
  case PT:
    return this->fCompositeParticle.Pt();
  }

  DEBUGclass("returning");
  return 0;
}
//______________________________________________________________________________________________

TQCompositorObservable::TQCompositorObservable(const TString& expression):
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

const TString& TQCompositorObservable::getExpression() const {
  // retrieve the expression associated with this observable
  return this->fExpression;
}

//______________________________________________________________________________________________

bool TQCompositorObservable::hasExpression() const {
  // check if this observable type knows expressions
  return true;
}

//______________________________________________________________________________________________

void TQCompositorObservable::setExpression(const TString& expr){
  // set the expression to a given string
  this->fExpression = expr;
}
//______________________________________________________________________________________________

bool TQCompositorObservable::parseExpression(const TString& expr){
  // parse the expression
  // CP(someObject,someOtherObject,...).method()
  TString expression(expr);
  TString buffer;
  TQStringUtils::readUpTo(expression,buffer,"(");
  buffer.Clear();
  TQStringUtils::readBlock(expression,buffer,"()");
  while(!buffer.IsNull()){
    TString next;
    TQStringUtils::readUpTo(buffer,next,",");
    TQStringUtils::removeLeading(buffer,",");
    // next now contains the expression we want to parse
    TString containerName;
    unsigned int containerIndex;
    std::vector<unsigned int> particleIndices;
    TQEventObservable::parseParticleReference(next,containerName,containerIndex,particleIndices);
    this->fContainerNames.push_back(containerName);
    this->fContainerIndices.push_back(containerIndex);
    this->fParticleIndices.push_back(particleIndices);
  }
  expression.ToLower();
  if(expression.Contains("m")){
    this->f4Quantity = MASS;
  } else if(expression.Contains("pt")){
    this->f4Quantity = PT;
  } else if(expression.Contains("eta")){
    this->f4Quantity = ETA;
  } else if(expression.Contains("phi")){
    this->f4Quantity = PHI;
  } else {
    return false;
  }
  return true;
}

//______________________________________________________________________________________________

void TQCompositorObservable::clearParsedExpression(){
  // clear the current expression
  this->fContainerNames.clear();
  this->fContainerIndices.clear();
  this->fParticleIndices.clear();
}

//______________________________________________________________________________________________

TString TQCompositorObservable::getActiveExpression() const {
  // retrieve the expression associated with this incarnation
  return "";
}

//______________________________________________________________________________________________

bool TQCompositorObservable::initializeSelf(){
  // initialize self
  if(!this->parseExpression(TQObservable::compileExpression(this->fExpression,this->fSample))){
    this->clearParsedExpression();
    return false;
  }
  return true;
}

//______________________________________________________________________________________________

bool TQCompositorObservable::finalizeSelf(){
  // finalize self
  this->clearParsedExpression();
  return true;
}

//______________________________________________________________________________________________

DEFINE_OBSERVABLE_FACTORY(TQCompositorObservable,TString expression){
  // a factory for this observable type
  if(TQStringUtils::removeLeadingText(expression,"CP:") || expression.BeginsWith("CP(")  || expression.BeginsWith("CompositeParticle")){
    return new TQCompositorObservable(expression);
  }
  return NULL;
}

                          
