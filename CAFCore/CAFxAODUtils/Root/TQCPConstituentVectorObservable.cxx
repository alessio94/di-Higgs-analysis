#include "CAFxAODUtils/TQCPConstituentVectorObservable.h"
#include <limits>
#include <sstream>
#include <stdexcept>

#include "TString.h"
#include "QFramework/TQTaggable.h"
// #define _DEBUG_
#include "QFramework/TQLibrary.h"
/*@observable: [TQCPConstituentVectorObservable] The TQCPConstituentVectorObservable is a VectorObservable providing the values from a selected method of a list of IParticles. The list of IParticles is obtained via a (possibly nested) CompositeParticle's "part" and/or "otherPart" method (by iterating over the valid arguments for these methods). Expressions for this Observable should read like 'Event$(cand)[0].part(1).otherPart(2).part(3).part().pt()' . The first segment specified the container to retrieve from the xAOD, the subsequent "part(i)" or "otherPart(i)" allow to go deeper in case of nested CompositeParticles. The last two segments specify the list of IParticles to finally consider and the method to call on them. For the second to last segment valid strings are "part()", "otherPart()", and "anyPart()" (the last one concatenating entries from part and otherPart in this order). The last segment specifies the method and can be in principle any simple IParticle method, although explicit support needs to be implemented. At this point the following methods are available: pt(), eta(), phi(), m(), e(), rapidity(), type() . A minimal example (without nested CompositePaticles) is: "Event$(cand)[0].part().pt()". To create a VectorObservable of, e.g., pTs from objects of only a particular type (e.g. jets), combine this Observable with the TQFilterObservable!
*/

ClassImp(TQCPConstituentVectorObservable)

//______________________________________________________________________________________________

TQCPConstituentVectorObservable::TQCPConstituentVectorObservable(){
  // default constructor
  DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

TQCPConstituentVectorObservable::~TQCPConstituentVectorObservable(){
  // default destructor
  DEBUGclass("destructor called");
}

//______________________________________________________________________________________________

double TQCPConstituentVectorObservable::getValue() const {
    throw std::runtime_error("Caught attempt to evaluate vector observable of type `TQIParticleFourVectorVectorObservable` in scalar context!");
    return -999;
}

//______________________________________________________________________________________________

const xAOD::CompositeParticle* TQCPConstituentVectorObservable::getLastCompositeParticle() const {
  const xAOD::CompositeParticleContainer* baseContainer;
  if(!this->fEvent->retrieve(baseContainer, this->fContainerName.Data()).isSuccess()){
    ERRORclass("failed to retrieve CompositeParticleContainer!");
    return nullptr;
  }
  const xAOD::CompositeParticle* cp = baseContainer->at(this->fContainerIndex);
  const size_t nLevels = fParticleIndices.size();
  for(size_t i=0; i<nLevels; ++i){
    cp = this->fUseOtherParts[i] ? cp->otherCompPart(fParticleIndices[i]) : cp->compPart(fParticleIndices[i]);
    if(!cp){
      std::stringstream ss;
      ss << "for event " << this->getCurrentEntry();
      ss << ": unable to cast to xAOD::CompositeParticle: " << this->fContainerName << "[" << this->fContainerIndex << "]";
      for(size_t j=0; j<i; ++j){
        ss << (this->fUseOtherParts[j]? "->otherPart(":"->part(") << this->fParticleIndices[j] << ")";
      }
      throw std::runtime_error(ss.str());
    }
    
  }
  return cp; 
}

//______________________________________________________________________________________________

int TQCPConstituentVectorObservable::getNevaluations() const {
  const xAOD::CompositeParticle* cp = getLastCompositeParticle();
  switch (fIterateVariant) {
    case IterateVariant::PART:
      return cp->nParts();
    case IterateVariant::OTHERPART:
      return cp->nOtherParts();
    case IterateVariant::ANYPART:
      return cp->nParts() + cp->nOtherParts();
    case IterateVariant::UNKNOWN:
      break;
  }
  return -1;
}

//______________________________________________________________________________________________
double TQCPConstituentVectorObservable::getSingleValue(const xAOD::IParticle* part) const {
  switch (this->fMethod) {
    case Method::M:
      return part->m();
    case Method::PT:
      return part->pt();
    case Method::ETA:
      return part->eta();
    case Method::PHI:
      return part->phi();
    case Method::E:
      return part->e();
    case Method::RAPIDITY:
      return part->rapidity();
    case Method::TYPE:
      return part->type();
    //sadly the following are not member functions of IParticle...
    /*case Method::PX:
      return part->px();
    case Method::PY:
      return part->py();
    case Method::PZ:
      return part->pz();
    case Method::ET:
      return part->et();
    case Method::CHARGE:
      return part->charge(); 
    case Method::PDGID:
      return part->pdgId();*/
    case Method::UNKNOWN: //just to make the compiler happy
      break;
  }
  return std::numeric_limits<double>::quiet_NaN();
}

double TQCPConstituentVectorObservable::getValueAt(int index) const {
  // value retrieval function, called on every event for every cut and histogram
  DEBUGclass("entering function");
  const xAOD::CompositeParticle* cp = this->getLastCompositeParticle();
  const xAOD::IParticle* p = nullptr;
  switch (fIterateVariant) {
    case IterateVariant::PART:
      p = cp->part(index);
      break;
    case IterateVariant::OTHERPART:
      p = cp->otherPart(index);
      break;
    case IterateVariant::ANYPART:
      if ((size_t)index<cp->nParts()) {
        p = cp->part(index);
      } else {
        p = cp->otherPart(index - cp->nParts());
      }
      break;
    case IterateVariant::UNKNOWN:
      break;
  }
  if (!p) return std::numeric_limits<double>::quiet_NaN();
  return this->getSingleValue(p);
}
//______________________________________________________________________________________________

TQCPConstituentVectorObservable* TQCPConstituentVectorObservable::createInstance(const TString& expression_)
{
  // static method used to instantiate the Observable
  
  TString expression = expression_;
  TQCPConstituentVectorObservable* instance = nullptr;
  
  if(TQStringUtils::removeLeadingText(expression,"CPCV")){
    expression = TQStringUtils::trim(expression);//remove leading+trailing whitespaces
    TString enclosed;
    TQStringUtils::readBlock(expression,enclosed,"()");
    if (expression.Length()>0) {
      ERRORclass("Expression seems to not be properly terminated, found non-empty residual after closing parenthesis in expression 'CPCV(%s)%s'",enclosed.Data(),expression.Data());
      throw std::runtime_error("Failed to create CPConstituentVectorObservable instance, see preceeding errors for details!");
      return nullptr;
    }
    enclosed = TQStringUtils::trim(enclosed);
    if (enclosed.Length()==0) {
      ERRORclass("Found an empty expression enclosed in the parentheses of 'CPCV(%s)'",enclosed.Data());
      throw std::runtime_error("Failed to create CPConstituentVectorObservable instance, see preceeding errors for details!");
      return nullptr;
    }
    //the following is a bit overkill for now, but we may want to add more options later...
    TQTaggable tagReader;
    tagReader.importTagsWithPrefix(enclosed, "", false, false, "obj");
    TString objDef;
    if (!tagReader.getTagString("obj",objDef)) {
      ERRORclass("Could not determine container/object specification from expression '%s'",expression_.Data());
      throw std::runtime_error("Failed to create CPConstituentVectorObservable instance, see preceeding errors for details!");
      return nullptr;
    }
    
    instance = new TQCPConstituentVectorObservable();
    instance->setExpression("CPCV("+enclosed+")"); //we will later on re-parse the expression, for now just make sure it's in a reasonably consistent format
    instance->SetName(TQObservable::makeObservableName(instance->getExpression()));
    //instance->setObjectDef(objDef);
    //possibly apply further options...
    
  }
  
  return instance;
}

//______________________________________________________________________________________________

bool TQCPConstituentVectorObservable::parseObjectDef(const TString& objDef) {
  //this method does most of the configuration based on an object definition string
  //which should read something like EventEM[0].part(0).otherPart(1).part().pt()
  this->fParticleIndices.clear();
  this->fContainerIndex = -1;
  this->fContainerName = "";
  this->fUseOtherParts.clear();
  //std::vector<TString> tokens = TQStringUtils::tokenizeVector(objDef
  size_t methodDotPos = TQStringUtils::rfindFree(objDef, ".", "()[]{}", objDef.Length()-1);
  if (methodDotPos == (size_t)kNPOS) {
    throw std::runtime_error(TString::Format("Could not find \".\" supposed to be separating the container/object specification from the method name in '%s'",objDef.Data()).Data());
    return false;
  }
  size_t iterDotPos = TQStringUtils::rfindFree(objDef, ".", "()[]{}", methodDotPos-1);
  if (methodDotPos == (size_t)kNPOS) {
    throw std::runtime_error(TString::Format("Could not find \".\" supposed to be separating the container specification from the list to iterate over (one of: part, otherPart, anyPart) '%s'",objDef.Data()).Data());
    return false;
  }
  TString method = objDef(methodDotPos+1, objDef.Length()-1 - methodDotPos);
  TString iter = objDef(iterDotPos+1,methodDotPos-1-iterDotPos);
  TString container = objDef(0, iterDotPos);
  
  if (!this->setMethod(method)) {
    throw std::runtime_error(TString::Format("Unsupported method '%s', cannot initialize Observable with expressions '%s'",method.Data(), this->getExpression().Data()).Data());
    return false;
  }
  if (!this->setIterMode(iter)) {
    throw std::runtime_error(TString::Format("Unsupported iteration mode '%s' (should be 'part', 'otherPart', or 'anyPart'), cannot initialize Observable with expressions '%s'",iter.Data(), this->getExpression().Data()).Data());
    return false;
  }
  
  fParticleIndices.clear();
  fUseOtherParts.clear();
  //parse the container and additional part(x).part(y).part(z)... calls
  //all arguments are passed by reference and will be modified by this function
  if (!TQEventObservable::parseParticleReference(container, fContainerName, fContainerIndex, fParticleIndices, fUseOtherParts)) {
    return false;
  }
  
  return true;
}

//______________________________________________________________________________________________
bool isEq(const TString& first, const char* second) {  //small helper function
    return first.EqualTo(second,TString::kIgnoreCase);
}

bool TQCPConstituentVectorObservable::setMethod(TString& method) {
  TQStringUtils::removeTrailing(method,TQStringUtils::getBlanks()+"()");
  if(isEq(method,"m")){
    this->fMethod = Method::M;
  } else if(isEq(method,"pt")){
    this->fMethod = Method::PT;
  } else if(isEq(method,"eta")){
    this->fMethod = Method::ETA;
  } else if(isEq(method,"phi")){
    this->fMethod = Method::PHI;
  } else if(isEq(method,"e")){
    this->fMethod = Method::E;
  } else if(isEq(method,"rapidity")){
    this->fMethod = Method::RAPIDITY;
  } else if(isEq(method,"type")){
    this->fMethod = Method::TYPE;
    //sadly the following are not actually members of IParticle:
    /*
  } else if(isEq(method,"px")){
    this->fMethod = Method::PX;
  } else if(isEq(method,"py")){
    this->fMethod = Method::PY;
  } else if(isEq(method,"pz")){
    this->fMethod = Method::PZ;
  } else if(isEq(method,"charge")){
    this->fMethod = Method::CHARGE;
  } else if(isEq(method,"pdgId")){
    this->fMethod = Method::PDGID;
    */
  } else {  
    this->fMethod = Method::UNKNOWN;
    return false;
  }
  return true;
}

//______________________________________________________________________________________________

bool TQCPConstituentVectorObservable::setIterMode(TString& iter) {
  TQStringUtils::removeTrailing(iter,TQStringUtils::getBlanks()+"()");
  if(isEq(iter,"part")){
    this->fIterateVariant = IterateVariant::PART;
  } else if(isEq(iter,"otherPart")){
    this->fIterateVariant = IterateVariant::OTHERPART;
  } else if(isEq(iter,"anyPart")){
    this->fIterateVariant = IterateVariant::ANYPART;
  } else {  
    this->fIterateVariant = IterateVariant::UNKNOWN;
    return false;
  }
  return true;
}



TString TQCPConstituentVectorObservable::getMethodString(Method method) {
  switch (method) {
    case Method::M:
      return "m";
    case Method::PT:
      return "pt";
    case Method::ETA:
      return "eta";
    case Method::PHI:
      return "phi";
    case Method::E:
      return "e";
    case Method::RAPIDITY:
      return "rapidity";
    case Method::TYPE:
      return "type";
      //sadly the following are not member functions of IParticle...
      /*
    case Method::PX:
      return "px";
    case Method::PY:
      return "py";
    case Method::PZ:
      return "pz";
    case Method::ET:
      return "et";
    case Method::CHARGE:
      return "charge";  
    case Method::PDGID:
      return "pdgId";
      */
    case Method::UNKNOWN: //just to make the compiler happy
      break;
  }
  return "UNKNOWN";
}

TString TQCPConstituentVectorObservable::getIterateVariantString(IterateVariant iterVar){
  switch (iterVar) {
    case IterateVariant::PART:
      return "part";
    case IterateVariant::OTHERPART:
      return "otherPart";
    case IterateVariant::ANYPART:
      return "anyPart";
    case IterateVariant::UNKNOWN:
      break;  
  }
  return "UNKNOWN";
}

//______________________________________________________________________________________________

const TString& TQCPConstituentVectorObservable::getExpression() const {
  return this->fExpression;
}

//______________________________________________________________________________________________

TString TQCPConstituentVectorObservable::getActiveExpression() const {
  // retrieve the expression associated with this observable
  if(this->fCachedExpression.IsNull()){
    this->fCachedExpression = TString::Format("%s(%s[%d]","CPCV",this->fContainerName.Data(),(int)(this->fContainerIndex));
    for(size_t i=0; i<this->fParticleIndices.size(); ++i){
      this->fCachedExpression.Append(this->fUseOtherParts[i] ? TString::Format(".otherPart(%d)",(int)(this->fParticleIndices[i])) : TString::Format(".part(%d)",(int)(this->fParticleIndices[i])));
    }
    this->fCachedExpression.Append(".");
    
    this->fCachedExpression.Append(getIterateVariantString(this->fIterateVariant)+"().");
    this->fCachedExpression.Append(getMethodString(this->fMethod)+"())");
  }
  return this->fCachedExpression;
}

//______________________________________________________________________________________________

bool TQCPConstituentVectorObservable::hasExpression() const {
  // check if this observable type knows expressions
  return true;
}

//______________________________________________________________________________________________

bool TQCPConstituentVectorObservable::initializeSelf(){
  // initialize self - compile container name, construct accessor
  TString expression = this->fExpression;
  TQStringUtils::removeLeadingText(expression,"CPCV(");
  TQStringUtils::removeTrailingText(expression,")");
  //optional TODO: if more options are desired in the future, this should likely be
  // followed by parsing expression via TQTaggable to read further options

  //only compile the actual object string
  if (!this->parseObjectDef(TQObservable::compileExpression(expression,this->fSample))) {
    return false;
  }
  //this->fDecoAccess = new SG::AuxElement::Accessor<T>(this->fDecoration.Data());
  return true;
}

//______________________________________________________________________________________________

bool TQCPConstituentVectorObservable::finalizeSelf(){
  // finalize self
  this->fIterateVariant = IterateVariant::UNKNOWN;
  this->fMethod = Method::UNKNOWN;
  this->clearParsedExpression();
  
  return true;
}

//______________________________________________________________________________________________

void TQCPConstituentVectorObservable::clearParsedExpression(){
  // clear the current expression
  this->fContainerName.Clear();
  this->fContainerIndex = 0;
  this->fParticleIndices.clear();
  this->fUseOtherParts.clear();
  this->fCachedExpression.Clear();
}

//______________________________________________________________________________________________

void TQCPConstituentVectorObservable::setExpression(const TString& expr_){
  TString expr = TQStringUtils::trim(expr_);
  TQStringUtils::ensureLeadingText(expr,"CPCV(");
  TQStringUtils::ensureTrailingText(expr,")");
  this->fExpression = expr;
}

//______________________________________________________________________________________________





DEFINE_OBSERVABLE_FACTORY(TQCPConstituentVectorObservable,TString expression){
  // a factory for this observable type
  TQCPConstituentVectorObservable* instance = TQCPConstituentVectorObservable::createInstance(expression);
  return instance;
  
}



