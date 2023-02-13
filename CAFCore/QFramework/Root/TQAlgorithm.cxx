#include "QFramework/TQAlgorithm.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQSampleFolder.h"
#include "QFramework/TQObservable.h"
#include "TTree.h"

#include "QFramework/TQLibrary.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQAlgorithm
//
// The TQAlgorithm class is a hook in order to precompute arbitrary
// event-based quantities and cache their values. The entire list of
// algorithms is executed on every event.
//
// every algorithm should implement the initialize, finalize and execute and cleanup methods.
// initialize will be called at the beginning of every sample and will take the sample as an argument
// finalize, execute and cleanup will not receive arguments and are called
// at the end of each sample (finalize), before each event (execute) and after each 
// event (cleanup).
// All four functions should return true in case of success, false in case of failure
// failure of any algorithm will cause the event loop to halt,
// additional error handling may be done by raising exceptions.
//
// All input/output managing is entirely up to the user and may be
// done via filling the TStore, creating decorations to objects in
// the TEvent or any other available operation.
//
// Please note that for use of TQAlgorithms with the TQMultiChannelAnalysisSampleVisitor
// must be streamable, i.e., correctly work with the TObject::Clone() method. This implies
// that all transient members (typically marked with the '//!' suffix in the header file)
// must be set in the 'TQAlgorithm::initialize(TQSample* s)' method! This also applies to 
// TQAlgorithms which are supposedly singletons (i.e. returning 'true' in TQAlgorithm::isSingleton())
// for which 'TQAlgorithm::initializeSingleton(const std::vector<TQSample*>& samples)' should 
// take the place of 'initialize(TQSample* s)'.
// If an implementation of an algorithm is marked as a singleton but should still be useable
// with SCASV as well as MCASV (or similar classes) BOTH initialization methods must be
// overridden!
//
////////////////////////////////////////////////////////////////////////////////////////////////

bool TQAlgorithm::Manager::addAlgorithm(TQAlgorithm* newAlgorithm){
  // add a new cache
  // the function takes ownership of the object, you shall not delete it!
  for(size_t i=0; i<this->gAlgorithmList.size(); ++i){
    if(TQStringUtils::equal(gAlgorithmList[i]->GetName(),newAlgorithm->GetName())){
      return false;
    }
  }
  this->gAlgorithmList.push_back(newAlgorithm);
  return true;
}

void TQAlgorithm::Manager::clearAlgorithms(){
  // clear the list of caches, deleting every one of them
  for(size_t i=0; i< this->gAlgorithmList.size(); ++i){
    delete this->gAlgorithmList[i];
  }
  this->gAlgorithmList.clear();
}

void TQAlgorithm::Manager::clearClonedAlgorithms() {
  // clear the list (of lists) of active caches, deleting every one of them
  for (auto& origAlgAndSubMapPair: this->gAlgorithmStore) {
    for (auto& stringAndClonedAlgPair : origAlgAndSubMapPair.second) {
      delete stringAndClonedAlgPair.second;
    }
    origAlgAndSubMapPair.second.clear();
  }
  this->gAlgorithmStore.clear();
}

const std::vector<TQAlgorithm*>& TQAlgorithm::Manager::getAlgorithms(){
  // retrieve a const reference to the list of all caches
  return this->gAlgorithmList;
}


void TQAlgorithm::Manager::printAlgorithms() const {
	// print the list of all currently registered algorithms
  std::cout<<"Registered algorithms:"<<std::endl;
  for (TQAlgorithm* const& alg : gAlgorithmList) {
    std::cout<<"  "<<(alg?alg->GetName():"NULL")<<std::endl;
  }
}

void TQAlgorithm::Manager::printActiveAlgorithms() const {
	// print the list of all currently active algorithms
  std::cout<<"Active algorithms:"<<std::endl;
  for (TQAlgorithm* const& alg : gActiveAlgorithms) {
    std::cout<<"  "<<(alg?alg->GetName():"NULL")<<std::endl;
  }
}
/*
bool TQAlgorithm::Manager::cloneAlgorithms(int n) {
  // create n clones of each cache unless the cache is a singleton (in which case only one clone is created)
  if (this->gClonedAlgorithmList.size() != 0) {
    WARNfunc("The list of cloned algorithms is not empty. Will implicitly call TQAlgorithm::Manager::clearClonedAlgorithms() before creating a fresh set of clones! Please consider performing this call explicitly before requesting a new set of clones to be created.");
    this->clearClonedAlgorithms();
  }
  for (const auto& origAlg: this->gAlgorithmList) {
    this->gClonedAlgorithmList.push_back(std::vector<TQAlgorithm*>());
    const size_t index = this->gClonedAlgorithmList.size()-1;
    if (origAlg->isSingleton()) { //single clone if algorithm should run once per event
      this->gClonedAlgorithmList[index].push_back( static_cast<TQAlgorithm*>( origAlg->Clone() ) );
    } else {
      for (int i=0;i<n;++i) { //n clones if n instances of the algorithm should run per event (typically n channels)
        this->gClonedAlgorithmList[index].push_back( static_cast<TQAlgorithm*>( origAlg->Clone() ) );
      }
    }
  }
  return true;
  
}
*/

TQAlgorithm* TQAlgorithm::getClone() const {
  // retrieve a clone of this algorithm
  return static_cast<TQAlgorithm*>( this->Clone() );
}

bool TQAlgorithm::Manager::initializeAlgorithms(TQSample*s){
  // initialize all algorithms
  for(size_t i=0; i<this->gAlgorithmList.size(); ++i){
    if(!this->gAlgorithmList[i]->initialize(s)) return false;
  }
  return true;
}

bool TQAlgorithm::Manager::initializeAlgorithms(TQSampleFolder*s){
  // initialize all algorithms
  for(size_t i=0; i<this->gAlgorithmList.size(); ++i){
    if(!this->gAlgorithmList[i]->initializeSampleFolder(s)) return false;
  }
  return true;
}

void TQAlgorithm::Manager::resetActiveAlgorithms() {
	// clear the list of currently active algorithms
  gActiveAlgorithms.clear();
}

namespace {
  template<class T> bool initAlg(TQAlgorithm* alg, T* obj);
  template<> bool initAlg<TQSample>(TQAlgorithm* alg, TQSample* obj){
    return alg->initialize(obj);
  }
  template<> bool initAlg<TQSampleFolder>(TQAlgorithm* alg, TQSampleFolder* obj){
    return alg->initializeSampleFolder(obj);
  }
  template<class T> bool initAlgSingleton(TQAlgorithm* alg, std::vector<T*>& obj);  
  template<> bool initAlgSingleton<TQSample>(TQAlgorithm* alg, std::vector<TQSample*>& obj){
    return alg->initializeSingleton(obj);
  }
  template<> bool initAlgSingleton<TQSampleFolder>(TQAlgorithm* alg, std::vector<TQSampleFolder*>& obj){
    return alg->initializeSampleFolderSingleton(obj);
  }
  template<class T> bool finAlg(TQAlgorithm* alg, T* obj);
  template<> bool finAlg<TQSample>(TQAlgorithm* alg, TQSample*/*obj*/){
    return alg->finalize();
  }
  template<> bool finAlg<TQSampleFolder>(TQAlgorithm* alg, TQSampleFolder* obj){
    return alg->finalizeSampleFolder(obj);
  }
  template<class T> bool finAlgSingleton(TQAlgorithm* alg, std::vector<T*>& obj);  
  template<> bool finAlgSingleton<TQSample>(TQAlgorithm* alg, std::vector<TQSample*>&/* obj*/){
    return alg->finalizeSingleton();
  }
  template<> bool finAlgSingleton<TQSampleFolder>(TQAlgorithm* alg, std::vector<TQSampleFolder*>& obj){
    return alg->finalizeSampleFolderSingleton(obj);
  }  
}

bool TQAlgorithm::Manager::isActiveAlgorithm(const TQAlgorithm* alg) const {
  for (const TQAlgorithm* ref : this->gActiveAlgorithms) {
    if (ref==alg) return true;
  }
  return false;
}

template <class T> std::map<TQAlgorithm*,T*> TQAlgorithm::Manager::getAlgorithmToSampleFolderMap(const std::vector<T*>& samples, const TString& tagKey) {
  
  std::map<TQAlgorithm*, T*> algMap;
  for (auto* s: samples) {
    TString key;
    if (!s->getTagString(tagKey,key)) { continue; } //TODO: is this the right treatment? we simply ignore SampleFolders which are, e.g., not yet split in channels?
    //note that skipping here implies that we will not finalize algs on such SampleFolders...
    for (const auto& origToChannels : this->gAlgorithmStore) {
      if (origToChannels.second.count(key)>0) {
        algMap[origToChannels.second.at(key)] = s;
      }
    }
  }
  return algMap;
}

template std::map<TQAlgorithm*,TQSample*> TQAlgorithm::Manager::getAlgorithmToSampleFolderMap<TQSample>(const std::vector<TQSample*> & samples, const TString& tagKey);
template std::map<TQAlgorithm*,TQSampleFolder*> TQAlgorithm::Manager::getAlgorithmToSampleFolderMap<TQSampleFolder>(const std::vector<TQSampleFolder*> & samples, const TString& tagKey);


template<class T> bool TQAlgorithm::Manager::finalizeClonedAlgorithms(std::vector<T*> & samples, const TString& tagKey){
  // finalize all algorithms (multi-channel variant)
  
  //step 1: finalize all singleton algorithms (which are not cloned)
  for (TQAlgorithm*const & alg : gActiveAlgorithms) {
    if(alg->isSingleton()){
      finAlgSingleton(alg,samples);
    } else {
      continue;
      //for(auto s:samples){
      //  finAlg(alg,s); //This is wrong!  each algo should only be finalized for the suitable sample(Folder)
      //}
    }
  }
  //step 2: finalize all cloned algoriths. For this we need to on-the-fly create the correct mapping of Algorithms to Sample(Folder)s
  std::map<TQAlgorithm*,T*> algMap = this->getAlgorithmToSampleFolderMap(samples, tagKey);
  for (const auto& algSF: algMap) {
    if (algSF.first->isSingleton()) continue; //just to make sure we don't accidentially include a singleton alg here...
    finAlg(algSF.first,algSF.second);
  }
  //TODO: return a more meaningful value here
  return true;
}

template bool TQAlgorithm::Manager::finalizeClonedAlgorithms<TQSample>(std::vector<TQSample*> & samples, const TString& tagKey);
template bool TQAlgorithm::Manager::finalizeClonedAlgorithms<TQSampleFolder>(std::vector<TQSampleFolder*> & samples, const TString& tagKey);

template<class T> bool TQAlgorithm::Manager::initializeClonedAlgorithms(std::vector<T*> & samples, const TString& tagKey, bool switchObservableSets){
  // initialize all already cloned or to-be-cloned algorithms (multi-channel variant)
  // creates clones of original algorithms if needed.
  this->resetActiveAlgorithms();
  
  for (TQAlgorithm* & origAlg: this->gAlgorithmList) {
    if (!origAlg) continue;
    if (origAlg->isSingleton()) {
      //the simple case, we just add it to the active algorithms and initialize it on all (active) samples at once
      if (!initAlgSingleton(origAlg,samples)) {
        throw std::runtime_error(TString::Format("Failed to initialize algorithm '%s' as singleton",origAlg->GetName()).Data());
        return false;
      }
      gActiveAlgorithms.push_back(origAlg);
      continue;//we're done with this algorithm for now
    }
    
    if (gAlgorithmStore.count(origAlg) == 0) {//there are no clones at all yet for this algorithm (and it's not a singleton)
      std::map<TString,TQAlgorithm*> algMap;
      for ( auto s  : samples) { //we had nothing before so we need to create the whole structure for each sample we are provided with
        TString key;
        if (!s->getTagString(tagKey,key)) {//cannot identify channel -> error (wouldn't know where to store it)
          throw std::runtime_error(TString::Format("Could not obtain channel identifier from sample '%s' using key '%s'",s->getPath().Data(), tagKey.Data()).Data());
          return false;
        }
        //make sure the algorithm uses the correct (active) observable set if needed. The "key" is the channel. Note that lazy evaluation prevents the switch from happening here if not requested
        if(switchObservableSets && !TQObservable::manager.setActiveSet(key)){
          TQObservable::manager.cloneActiveSet(key);
        }

        //clone origAlg, initialize clone and add to map and active algorithms
        TQAlgorithm* clone = origAlg->getClone();
        bool initSuccess = initAlg(clone,s);
        algMap[key] = clone; 
        gActiveAlgorithms.push_back(clone); //ownership belongs to algMap!
        if (!initSuccess) {
          ERRORclass("Failed to initialize algorithm '%s' on Sample(Folder)",origAlg->GetName(), s->getPath().Data());
          return false;
        }
      }
      //add algMap to gAlgorithmStore 
      gAlgorithmStore[origAlg] = algMap;
    } else {//there are already at least some clones of this algorithm, let's re-use them where possible
      std::map<TString,TQAlgorithm*>& algMap = gAlgorithmStore[origAlg]; 
      for ( auto s : samples) {
        TString key;
        if (!s->getTagString(tagKey,key)) {//cannot identify channel -> error (wouldn't know where to look for/store clones of the algorithm)
          throw std::runtime_error(TString::Format("Could not obtain channel identifier from sample '%s' using key '%s'",s->getPath().Data(), tagKey.Data()).Data());
          return false;
        }
        //make sure the algorithm uses the correct (active) observable set if needed. The "key" is the channel. Note that lazy evaluation prevents the switch from happening here if not requested
        if(switchObservableSets && !TQObservable::manager.setActiveSet(key)){
          TQObservable::manager.cloneActiveSet(key);
        }
        
        bool initSuccess = false;
        if (algMap.count(key) == 0) { //for this particular channel no clone exists yet
          TQAlgorithm* clone = origAlg->getClone();
          initSuccess = initAlg(clone,s);
          algMap[key] = clone; 
          gActiveAlgorithms.push_back(clone); //ownership belongs to algMap!
        } else { //there is already a clone for this channel, we use it
          TQAlgorithm* clone = algMap[key];
          initSuccess = initAlg(clone,s);
          gActiveAlgorithms.push_back(clone);
        }
        if (!initSuccess) {
            ERRORclass("Failed to initialize algorithm '%s' on Sample(Folder)",origAlg->GetName(), s->getPath().Data());
            return false;
        }
      }
      
    }
    
  }
  
  /*
  for(const std::vector<TQAlgorithm*>& clones : this->gClonedAlgorithmList){
    if (clones.size()==1  && clones[0]->isSingleton()) {
      if (!clones[0]->initializeSingleton(samples)) return false; //initialize singleton algorithm with list of all samples
    } else if (clones.size()==samples.size()) {
      //everything should be fine now, let's actually initialize the algorithms
      for (size_t i=0;i<clones.size();++i) {
        if (!clones[i]->initialize(samples[i]) ) return false;
      }
    } else { //we neither have a singleton algorithm nor does the number of clones match the number of samples -> ERROR
      ERRORclass("Failed to initialize cloned algorithms: number of clones differs from number of TQSamples provided!");
      return false;
    }
  }
  */
  return true;
} //end initializeClonedAlgorithms

template bool TQAlgorithm::Manager::initializeClonedAlgorithms<TQSample>(std::vector<TQSample*> & samples, const TString& tagKey, bool switchObservableSets);
template bool TQAlgorithm::Manager::initializeClonedAlgorithms<TQSampleFolder>(std::vector<TQSampleFolder*> & samples, const TString& tagKey, bool switchObservableSets);


bool TQAlgorithm::Manager::finalizeAlgorithms(){
  // finalize all algorithms
    for (const auto& alg : gAlgorithmList) {
      if (!alg->finalize()) return false;
    }
  return true;
}

bool TQAlgorithm::Manager::finalizeAlgorithmsSampleFolder(TQSampleFolder* sf){
  // finalize all algorithms
  for (const auto& alg : gAlgorithmList) {
    if (!alg->finalizeSampleFolder(sf)) return false;
    }
  return true;
}

bool TQAlgorithm::Manager::executeAlgorithms(){
  // execute all algorithms
  //for(size_t i=0; i<this->gAlgorithmList.size(); ++i){
    for (const auto& alg : gAlgorithmList) {
      if (!alg->execute()) return false;
    }
    //if(! this->gAlgorithmList[i]->execute()) return false;
  return true;
}

bool TQAlgorithm::Manager::executeClonedAlgorithms(){
  // execute all algorithms (multi-channel variant)
  for (TQAlgorithm* const & alg : gActiveAlgorithms) {
    if (!alg->execute()) return false;
  }
  return true;
}

bool TQAlgorithm::Manager::cleanupAlgorithms(){
  // cleanup all algorithms (multi channel variant)
  // post event method
    for (TQAlgorithm* const & alg : gAlgorithmList) {
      if (!alg->cleanup()) return false;
  }
  return true;
}

bool TQAlgorithm::Manager::cleanupClonedAlgorithms(){
  // cleanup all algorithms (multi channel variant)
  // post event method
  // actually a stupid name
  for ( TQAlgorithm* const & alg : gActiveAlgorithms) {
      if (!alg->cleanup()) return false;
    }
  return true;
}

//========================================================
//========================================================

bool TQAlgorithm::isSingleton() const {
  // return true if this algorithm is a singleton, false otherwise
  return false;
}

bool TQAlgorithm::finalizeSingleton(){
  // default implementation: call "old-style" finalize() method
  this->finalize();
  return true;  
}

bool TQAlgorithm::finalizeSampleFolderSingleton(const std::vector<TQSampleFolder*>& /*samples*/) { 
  // default implementation: do nothing
  return true;  
}

bool TQAlgorithm::initializeSingleton(const std::vector<TQSample*>& /*samples*/) { //parameter not used in baseClass implementation
  // default implementations simply report an error, it is the responsibility of the author of the derived class to implement at least one of these
  ERRORclass("initialization with a list of samples is not implemented for TQAlgorithm with name '%s'!", this->GetName());
  return false;
}

bool TQAlgorithm::initialize(TQSample* /*s*/) { //parameter not used in baseClass implementation
  // default implementations simply report an error, it is the responsibility of the author of the derived class to implement at least one of these
  ERRORclass("initialization with single TQSample is not implemented for TQAlgorithm with name '%s'!",this->GetName());
  return false;
}

bool TQAlgorithm::initializeSampleFolderSingleton(const std::vector<TQSampleFolder*>& /*samples*/) {
  // default implementation: do nothing
  return true;  
}
bool TQAlgorithm::initializeSampleFolder(TQSampleFolder* /*s*/) {
  // default implementation: do nothing
  return true;  
}
bool TQAlgorithm::finalizeSampleFolder(TQSampleFolder* /*sf*/){
  // default implementation: do nothing
  return true;  
}
