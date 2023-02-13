#include "CAFxAODUtils/TQToolObservable.h"
#include "QFramework/TQIterator.h"
#include <limits>
#include <stdexcept>

#include "AsgTools/AsgTool.h"
#include "AsgTools/ToolStore.h"
#include "xAODBase/IParticle.h"
#include "xAODBase/IParticleContainer.h"

#include "QFramework/TQSample.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// you can perform debug printouts with statements like this
// DEBUG("error number %d occurred",someInteger);

// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

/*@observable: [TQToolObservable] The TQToolObservable is a generic wrapper around DualUseUtils (tools which are designed to work in AnalysisBase and Athena environments). [TODO: add more meaningful documentation/explanation and usage here]
*/

ClassImp(TQToolObservable)

//______________________________________________________________________________________________

TQToolObservable::TQToolObservable(){
  // default constructor
  DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

TQToolObservable::~TQToolObservable(){
  // default destructor
  DEBUGclass("destructor called");
  this->finalizeSelf();
} 


//______________________________________________________________________________________________

bool TQToolObservable::registerTool(IObservableTool* tool){
  // make sure that this tool is known to the tool store
  if(!tool){
    return false;
  }
  asg::IAsgTool* otherTool = asg::ToolStore::get(tool->name());
  if(otherTool){
    if(tool == otherTool){
      return true;
    } else {
      return false;
    }
  } else {
    return asg::ToolStore::put(tool).isSuccess();
  }
}

//______________________________________________________________________________________________

IObservableTool* TQToolObservable::getTool(const char* name){
  asg::IAsgTool* tool = asg::ToolStore::get(name);
  return dynamic_cast<IObservableTool*>(tool);
}

//______________________________________________________________________________________________

double TQToolObservable::getValue() const {
  // value retrieval function, called on every event for every cut and histogram
  DEBUGclass("entering function");
  const xAOD::IParticleContainer* cont = 0;
  this->retrieve(cont,this->fContainerName.Data());
  if(cont->size() <= (size_t)this->fContainerIndex){
    throw(std::runtime_error(TString::Format("ERROR in TQToolObservable '%s': unable to access index '%d' of container '%s' in event '%d'",this->GetName(),this->fContainerIndex,this->fContainerName.Data(),(int)(this->getCurrentEntry())).Data()));
  }
  const xAOD::IParticle* part = cont->at(this->fContainerIndex);
  return this->fTool->evaluate(part);
}

//______________________________________________________________________________________________

TQToolObservable::TQToolObservable(const TString& expression):
TQEventObservable(expression)
{
  // constructor with expression argument
  DEBUGclass("constructor called with '%s'",expression.Data());
  this->SetName(TQObservable::makeObservableName(expression));
  this->setExpression(expression);
}

//______________________________________________________________________________________________

const TString& TQToolObservable::getExpression() const {
  // retrieve the expression associated with this observable
  return this->fExpression;
}

//______________________________________________________________________________________________

bool TQToolObservable::hasExpression() const {
  // check if this observable type knows expressions
  return true;
}

//______________________________________________________________________________________________

void TQToolObservable::setExpression(const TString& expr){
  // set the expression to a given string
  this->fExpression = expr;
}

//______________________________________________________________________________________________

bool TQToolObservable::parseExpression(const TString& expr){
  // parse the expression
  this->clearParsedExpression();
  TString expression(expr);
  TQStringUtils::readToken(expression,this->fToolName,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890/:");
  TString block;
  if(expression.Length() < 3){
    throw(std::runtime_error(TString::Format("unable to parse expressionf or tool '%s' - empty argument block!",this->GetName()).Data()));
  }
  TQStringUtils::readBlock(expression,block,"()");
  bool ok = TQEventObservable::parseParticleReference(block,this->fContainerName,this->fContainerIndex,this->fParticleIndices,",");
  this->importTags(block);
  return ok;
}

//______________________________________________________________________________________________

void TQToolObservable::clearParsedExpression(){
  // clear the current expression
  this->fToolName.Clear();
  this->fContainerName.Clear();
  this->fContainerIndex = 0;
  this->fParticleIndices.clear();
}

//______________________________________________________________________________________________

TString TQToolObservable::getActiveExpression() const {
  // retrieve the expression associated with this incarnation
  std::stringstream ss;
  ss << "Tool:";
  ss << this->fToolName;
  TQTaggable* self = const_cast<TQToolObservable*>(this);
  TString tagString = self->exportTagsAsString();
  if (tagString.Length()>0) tagString.Prepend(","); //if an observable does not have additional arguments we may not insert a ',' as this will prevent the TQObservable::Manager from matching the expressions.
  ss << "(" << this->fContainerName << "[" << this->fContainerIndex << "]" << tagString << ")";
  return TQStringUtils::minimize(ss.str().c_str());
}

//______________________________________________________________________________________________

bool TQToolObservable::initializeSelf(){
  // initialize self - compile container name, construct accessor
  TString expr(TQObservable::compileExpression(this->fExpression,this->fSample,false));
  if(!this->parseExpression(expr)){
    ERRORclass("unable to initialize '%s'",this->GetName());
    return false;
  }
  this->fTool = asg::ToolStore::get<IObservableTool>(this->fToolName.Data());
  if(!fTool){
    throw(std::runtime_error(TString::Format("unable to retieve tool '%s'",this->fToolName.Data()).Data()));
  }
  asg::AsgTool* asgTool = dynamic_cast<asg::AsgTool*>(this->fTool);
  if(!asgTool){
    throw(std::runtime_error("retrieved a tool that is not an AsgTool - don't know what to do with it..."));
  }
  TQIterator itr(this->getListOfKeys(),true);
  PropertyMgr* mgr = asgTool->getPropertyMgr();
  if (!mgr) {
    throw(std::runtime_error("failed to retrieve property manager from AsgTool"));
  }
  while(itr.hasNext()){
    TString key(itr.readNext()->GetName());
    const Property* p = mgr->getProperty(key.Data());
    if (!p) {
      throw(std::runtime_error(TString::Format("failed to get property from property manager via key '%s'",key.Data()).Data()));
    }
    switch(p->type()){
    case Property::BOOL:
      asgTool->setProperty(key.Data(),(bool)(this->getTagBoolDefault(key,false))).ignore(); break;
    case Property::INT:
      asgTool->setProperty(key.Data(),(int)(this->getTagIntegerDefault(key,0))).ignore(); break;
    case Property::FLOAT:
      asgTool->setProperty(key.Data(),(float)(this->getTagDoubleDefault(key,0.))).ignore(); break;
    case Property::DOUBLE:
      asgTool->setProperty(key.Data(),(double)(this->getTagDoubleDefault(key,0.))).ignore(); break;
    case Property::STRING:
      asgTool->setProperty(key.Data(),this->getTagStringDefault(key,"").Data()).ignore(); break;
    default:
      ERRORclass("property-setting for type '%s' is not supported yet!",p->typeName().c_str());
    }
  }
  if(fTool->initialize().isFailure()){
    throw(std::runtime_error("unable to initialize tool '"+this->fTool->name()+"'!"));
  }
  return true;
}
 
//______________________________________________________________________________________________

bool TQToolObservable::finalizeSelf(){
  // finalize self
  this->clearParsedExpression();
// for now, IAsgTool doesn't have a finalize method. hence this block stays commented for the time being.
//  if(this->fTool){
//    if(this->fTool->finalize().isFailure()){
//      ERRORclass("tool '%s' failed to finalize...",this->fTool->name().c_str());
//    }
//  }
  this->fTool = NULL;
  return true;
}

//______________________________________________________________________________________________

DEFINE_OBSERVABLE_FACTORY(TQToolObservable,TString expression){
  // a factory for this observable type
  if(TQStringUtils::removeLeadingText(expression,"Tool:")){
    return new TQToolObservable(expression);
  }
  return NULL;
}

