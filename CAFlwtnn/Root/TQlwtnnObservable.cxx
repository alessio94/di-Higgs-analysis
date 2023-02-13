#include "CAFlwtnn/TQlwtnnObservable.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQTaggable.h"
#include "QFramework/TQPathManager.h"
#include "QFramework/TQSample.h"
//#include <limits>
#include "lwtnn/NNLayerConfig.hh"
#include "lwtnn/LightweightNeuralNetwork.hh"
#include "lwtnn/LightweightGraph.hh"
#include "lwtnn/parse_json.hh"
//#include "lwtnn/Stack.hh" 
#include <fstream> 
#include <iostream>
#include <iterator>



// uncomment the following line to enable debug printouts
// #define _DEBUG_
// you can perform debug printouts with statements like this
// DEBUG("error number %d occurred",someInteger);

// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

ClassImp(TQlwtnnObservable)

// initialize static member variables
std::map<std::string, lwt::LightweightNeuralNetwork*> TQlwtnnObservable::fNetworkCache;
std::map<std::string, lwt::LightweightGraph*> TQlwtnnObservable::fGraphCache;
std::map<std::string, std::string> TQlwtnnObservable::fInputNodeNameCache;
std::map<std::string, std::map<std::string, TString>> TQlwtnnObservable::fObservableExpressionCache;

//______________________________________________________________________________________________

TQlwtnnObservable::TQlwtnnObservable(){
  // default constructor
  DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

TQlwtnnObservable::~TQlwtnnObservable(){
  // default destructor
  DEBUGclass("destructor called");
}

//______________________________________________________________________________________________

TObjArray* TQlwtnnObservable::getBranchNames() const {
  // retrieve the list of branch names for this observable
  // ownership of the list belongs to the caller of the function
  DEBUGclass("retrieving branch names");
  TObjArray* bnames = new TObjArray();
  for(const std::pair<std::string,TQObservable*>& kv : this->fObservables){
    TQObservable* obs = kv.second;
    if(!obs) throw std::runtime_error(TString::Format("encountered invalid sub-observable corresponding key '%s'!",kv.first.c_str()).Data());
    DEBUGclass("retrieving branches of observable '%s' of class '%s'",obs->getExpression().Data(),obs->ClassName());
    TCollection* c = obs->getBranchNames();
    if(c){
      c->SetOwner(false);
      bnames->AddAll(c);
      delete c;
    }
  }
  DEBUGclass("returning");
  return bnames;
}

//______________________________________________________________________________________________

Long64_t TQlwtnnObservable::getCurrentEntry() const {
  // retrieve the current entry from the tree
  if(this->fObservables.size() == 0) return -1;
  for (const std::pair<std::string,TQObservable*> kv : this->fObservables) {
    if (kv.second->getCurrentEntry() >= 0) return kv.second->getCurrentEntry();
  }
  return -1;
}

//______________________________________________________________________________________________

int TQlwtnnObservable::getNevaluations() const {
  return this->fOutputNames.size();
}

//______________________________________________________________________________________________

void TQlwtnnObservable::evalSequential(std::map<std::string,double>& output) const {
  
  std::map<std::string, double> features;
  for (const std::pair<std::string,TQObservable*>& input: this->fObservables) {
    features[input.first] = input.second->getValue();
  }
  output = this->fNetwork->compute(features);
  return;
}

//______________________________________________________________________________________________

void TQlwtnnObservable::evalFunctional(std::map<std::string,double>& output) const {
  std::map<std::string, double> features;
  for (const std::pair<std::string,TQObservable*>& input: this->fObservables) {
    features[input.first] = input.second->getValue();
  }
  std::map<std::string, std::map<std::string, double>> wrappedFeatures;
  wrappedFeatures[this->fInputNodeName] = features;
  output = this->fGraph->compute(wrappedFeatures);
  return;
}


//______________________________________________________________________________________________

double TQlwtnnObservable::getValue() const {
  //forward to the implementation capable of handling multiple values.
  if (this->getObservableType() != TQObservable::ObservableType::vector) return this->getValueAt(0);
  else throw std::runtime_error(TString::Format( "Caught attempt to perform scalar evaluation on vector valued instance of TQlwtnnObservable with expression '%s'",this->getExpression().Data() ).Data());
}


double TQlwtnnObservable::getValueAt(int i) const {
  // value retrieval function, called on every event for every cut and histogram
  DEBUGclass("entering function");

  const Long64_t entry = getCurrentEntry();
  if (entry != fCachedEntry) {  // new event, calculate NN once
    fCachedValue.clear();
    if (this->fSequential) {
      //evaluate a "Sequential API" model
      this->evalSequential(fCachedValue);
    } else {
      //evaluate a "Functional API" model
      this->evalFunctional(fCachedValue);
    }
    fCachedEntry = entry;
  }

  const double retval = fCachedValue[this->fOutputNames[i]];
  return retval;
}

//______________________________________________________________________________________________

bool TQlwtnnObservable::initializeSequential(const std::string& fileName) {
  std::string cache_name(getName());
  DEBUGclass("Cache name = '%s'", cache_name.c_str());
  if (fNetworkCache.find(cache_name) == fNetworkCache.end()) {  // key not in map
    DEBUGclass("Creating new instance of neural network");
    //initialization in case the Sequential API style json is used
    std::ifstream netFile(fileName);

    lwt::JSONConfig netDef = lwt::parse_json(netFile);
    for (lwt::Input& input : netDef.inputs) { //explicitly non-const, as we'll possibly shorten the input keys!
      //parse input names into observable names and keys
      //expected syntax:
      //key=observableExpression
      //if no observableExpression is found, use the key itself
      TString key, obsExpr;
      obsExpr = TString(input.name);
      //move part before "=" to after
      TQStringUtils::readUpTo(obsExpr,key,"=","","\"\"''");
      TQStringUtils::removeLeading(obsExpr,"=");
      if (obsExpr.Length()<1) {
        obsExpr = key; //if no dedicated obsExpr is specified, fall back to key itself
      }
      //write back key to input definition, i.e., removing the obsExpr to make keys shorter
      input.name=key; //implicit conversion TString->std::string
      // we cannot cache the initialized observables because they might depend on
      // sample folder specific tags, so we just store the observable expression
      // without replacing the tags and initialize the observables for each instance later
      fObservableExpressionCache[cache_name][key.Data()] = obsExpr;
    }
    //after inputs were pre-parsed, construct the net from them
    fNetwork = new lwt::LightweightNeuralNetwork(netDef.inputs,netDef.layers,netDef.outputs);
    fNetworkCache.insert({cache_name, fNetwork});
  } else {
    DEBUGclass("Using existing instance of neural network");
    fNetwork = fNetworkCache.at(cache_name);
  }

  return initializeObservables(cache_name);
}

//______________________________________________________________________________________________

bool TQlwtnnObservable::initializeObservables(const std::string& cache_name) {
  std::shared_ptr<TQTaggable> aliases = TQTaggable::getGlobalTaggable("aliases");
  for (auto const& obsData : fObservableExpressionCache[cache_name] ) {
    std::string key = obsData.first;
    TString obsExpr = obsData.second;
    obsExpr = aliases->replaceInTextRecursive(obsExpr);

    TQObservable* inputObs = TQObservable::getObservable(obsExpr,this->fSample);
    if (!inputObs->initialize(this->fSample)) return false;
    this->fObservables[key] = inputObs;
  }

  return true;
}

//______________________________________________________________________________________________

bool TQlwtnnObservable::initializeFunctional(const std::string& fileName) {
  std::string cache_name(getName());
  DEBUGclass("Graph cache name = '%s'", cache_name.c_str());
  if (fGraphCache.find(cache_name) == fGraphCache.end()) {  // key not in map
    DEBUGclass("Creating new instance of graph");

    //initialization in case the Sequential API style json is used
    std::ifstream netFile(fileName);

    lwt::GraphConfig netDef = lwt::parse_json_graph(netFile);
    std::shared_ptr<TQTaggable> aliases = TQTaggable::getGlobalTaggable("aliases");
    if (netDef.inputs.size() != 1) {
      throw std::runtime_error(TString::Format("Failed to load functional lwtnn definition from file '%s': unsupported number of input nodes (must be 1 node withany number of variables)",fileName.c_str()).Data());
    }

    this->fInputNodeName = netDef.inputs[0].name;
    fInputNodeNameCache.insert({cache_name, fInputNodeName});
    for (lwt::Input& input : netDef.inputs[0].variables) { //explicitly non-const, as we'll possibly shorten the input keys!
      //parse input names into observable names and keys
      //expected syntax:
      //key=observableExpression
      //if no observableExpression is found, use the key itself
      TString key, obsExpr;
      obsExpr = TString(input.name);
      //move part before "=" to after
      TQStringUtils::readUpTo(obsExpr,key,"=","","\"\"''");
      TQStringUtils::removeLeading(obsExpr,"=");
      if (obsExpr.Length()<1) {
        obsExpr = key; //if no dedicated obsExpr is specified, fall back to key itself
      }
      //write back key to input definition, i.e., removing the obsExpr to make keys shorter
      input.name=key; //implicit conversion TString->std::string
      // we cannot cache the initialized observables because they might depend on
      // sample folder specific tags, so we just store the observable expression
      // without replacing the tags and initialize the observables for each instance later
      fObservableExpressionCache[cache_name][key.Data()] = obsExpr;
    }

    //after inputs were pre-parsed, construct the net from them
    fGraph = new lwt::LightweightGraph(netDef);
    fGraphCache.insert({cache_name, fGraph});
  } else {
    DEBUGclass("Using existing instance of graph");
    fInputNodeName = fInputNodeNameCache.at(cache_name);
    fGraph = fGraphCache.at(cache_name);
  }

  return initializeObservables(cache_name);
}



//______________________________________________________________________________________________

bool TQlwtnnObservable::initializeSelf(){
  // get your saved JSON file as an std::istream object
  std::string resolvedFileName = TQPathManager::getPathManager()->findFileFromEnvVar(this->fNetworkFileName,"CAFANALYSISSHARE");
  if (this->fSequential) {
    return this->initializeSequential(resolvedFileName);
  }
  return this->initializeFunctional(resolvedFileName);
}

//______________________________________________________________________________________________

bool TQlwtnnObservable::finalizeSelf(){
  // finalize this observable on a sample/tree
  DEBUGclass("finalizing");
  // here, you should undo anything that you did while initializing
  /* example block for TTreeFormula method:
  delete this->fFormula;
  this->fFormula = NULL;
  */
  for (const std::pair<std::string,TQObservable*>& input: this->fObservables) {
    if (!input.second->finalize()) return false;
  }
  std::string network_cache_name(getName());
  if (fNetworkCache.find(network_cache_name) != fNetworkCache.end()) {  // key in map
    fNetworkCache.erase(network_cache_name);
    delete fNetwork;
    fNetwork = nullptr;
  }

  if (fGraphCache.find(network_cache_name) != fGraphCache.end()) {  // key in map
    fGraphCache.erase(network_cache_name);
    delete fGraph;
    fGraph = nullptr;
  }

  if (fInputNodeNameCache.find(network_cache_name) != fInputNodeNameCache.end()) {  // key in map
    fInputNodeNameCache.erase(network_cache_name);
    fInputNodeName = "";
  }

  this->fObservables.clear();

  return true;
}

//______________________________________________________________________________________________

void TQlwtnnObservable::setOutputs(const std::vector<std::string> outputNames) {
  // Set a list of output names from the network to expose
  this->fOutputNames = outputNames;
}

void TQlwtnnObservable::setOutputs(const std::string& outputNames_) {
  this->setOutputs(TString(outputNames_));
}

void TQlwtnnObservable::setOutputs(const TString& outputNames_) {
  // Set a list of output names from the network to expose
  // This version expects a comma separated list of names,
  // optionally enclosed by {} (resembling a C++ initializer list)
  TString outputNames = outputNames_;
  TQStringUtils::trim(outputNames); //remove white spaces
  TString block = "";
  TQStringUtils::readBlock(outputNames,block,"()"); //in principle we'd like to use {} here, but TQObservable already replaces {} brackets with () ones...
  if (block.Length()<1) { //block wasn't enclosed, so let's use the original part directly
    block = outputNames;
  }  
  std::vector<TString> asTStrings = TQStringUtils::tokenizeVector(block, ",", true/*, "()[]{}"*/);
  this->fOutputNames.clear();
  for (const TString& tstr: asTStrings) {
    this->fOutputNames.push_back(tstr.Data());
  }
}

//______________________________________________________________________________________________

void TQlwtnnObservable::parseArguments(TString arguments) {
  // input is arguments string without surrounding parentheses
  TString fileName = "";
  TQStringUtils::readUpTo(arguments,fileName,",");
  TQStringUtils::removeLeading(arguments,",");
  TQStringUtils::trim(arguments);
  if (arguments.Length() < 1) {
    throw std::runtime_error(TString::Format("Unable to parse expression for lwtnn Observable: too few arguments (did you forget to specify the output names to expose?). Arguments were: '%s,%s'",fileName.Data(),arguments.Data()).Data());
  }
  this->fNetworkFileName = fileName;
  this->setOutputs(arguments);
  this->SetName("lwtnn("+fileName+", "+arguments+")");
}

//______________________________________________________________________________________________

TQlwtnnObservable::TQlwtnnObservable(const TString& arguments):
TQObservable(arguments)
{
    // constructor with name argument
    DEBUGclass("constructor called with '%s'",arguments.Data());
    parseArguments(arguments);
}



DEFINE_OBSERVABLE_FACTORY(TQlwtnnObservable,TString expression){
  // try to create an instance of this observable from the given expression
  TQStringUtils::trim(expression);
  if(TQStringUtils::removeLeadingText(expression,"lwtnn")) {
    bool sequential = false;
    if(TQStringUtils::removeLeadingText(expression,"Seq")) {
      //create instance configured for sequential API style inputs (using LightWeightNeuralNetwork)
      // else: create instance configured for functional API style inputs (using LightWeightGraph)
      sequential = true;
    }

    TQStringUtils::trim(expression);
    TString arguments = "";
    TQStringUtils::readBlock(expression,arguments);
    TQlwtnnObservable* obs = new TQlwtnnObservable(arguments);
    obs->setSequential(sequential);
    return obs;

  }
  return nullptr;
}

bool TQObservableFactory::setupCAFlwtnn() {
  TQObservable::manager.registerFactory(TQlwtnnObservable::getFactory(),true);
  return true;
}

namespace {
//dummy value to cause execution of above method uppon loading
const bool _TQObservableFactory__setupCAFlwtnn = TQObservableFactory::setupCAFlwtnn();
}
