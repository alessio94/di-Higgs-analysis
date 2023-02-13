#include "QFramework/TQUniqueCut.h"
#include "QFramework/TQStringUtils.h"
#include <algorithm>

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQUniqueCut:
//
// This class is a special version of the TQCompiledCut.
// The intention of this class is to provide functionality to remove
// overlap between a set of samples.
// 
// This cut is most sensibly used as the root of a hierarchy of TQCompiledCuts.
// As such, it will take care that every event will only be considered once.
// All RunNumbers and EventNumbers are stored and compared with to achieve this,
// any events recognized as duplicates will be rejected.
// Please note that this procedure is very demanding in terms of CPU and RAM,
// and might easily lead to severe problems when analysing large sample sets.
//
// In such cases, please consider removing the overlap on nTuple code level.
// 
////////////////////////////////////////////////////////////////////////////////////////////////

bool TQUniqueCut::printOnVeto = false;

ClassImp(TQUniqueCut)

// this is necessary because std::is_sorted is defined in c++11
// in order for this code to compile with older compilers as well,
// this function needs to be provided
#if __cplusplus >= 201103L
#define IS_SORTED(first,last) std::is_sorted(first,last)
#else 
template<class ForwardIt>
bool is_sorted(ForwardIt first, ForwardIt last){
  if (first != last) {
    ForwardIt next = first;
    while (++next != last) {
      if (*next < *first)
        return ( next == last );
      first = next;
    }
  }
  return true;
}
#define IS_SORTED(first,last) is_sorted(first,last);
#endif

bool TQUniqueCut::isMergeable() const {
  // returns false, since TQUniqueCuts are never mergeable
  return false;
}

bool TQUniqueCut::setBranchNames(const TString& runBranch, const TString& evtBranch){
  // set the branch names used for run number and event number
  this->eventNumberBranch = evtBranch;
  this->runNumberBranch = runBranch;
  return true;
}

void TQUniqueCut::initUniqueCut(){
  // initialize this instance of TQUniqueCut
  this->SetName("UniqueCut");
  this->enabled = true;
}

TQUniqueCut::TQUniqueCut() :
  runNumberBranch("run"),
  eventNumberBranch("event")
{
  // default constructor of the TQUniqueCut
  this->initUniqueCut();
}

TQUniqueCut::TQUniqueCut(const TString& name) :
  runNumberBranch("run"),
  eventNumberBranch("event")
{
  // default constructor of the TQUniqueCut
  this->initUniqueCut();
  this->SetName(name);
}

TQUniqueCut::TQUniqueCut(const TString& runBranch, const TString& evtBranch) :
  runNumberBranch(runBranch),
  eventNumberBranch(evtBranch)
{
  // constructor of the TQUniqueCut class, taking branch names for run and event number
  this->initUniqueCut();
}

TQUniqueCut::TQUniqueCut(const TString& name, const TString& runBranch, const TString& evtBranch) :
  runNumberBranch(runBranch),
  eventNumberBranch(evtBranch)
{
  // constructor of the TQUniqueCut class, taking a name as well as branch names for run and event number
  this->initUniqueCut();
  this->SetName(name);
}

const TString& TQUniqueCut::getRunNumberExpression() const {
  return this->runNumberBranch;
}

const TString& TQUniqueCut::getEventNumberExpression() const {
  return this->eventNumberBranch;
}

void TQUniqueCut::clear(){
  // clear all saved information
  this->runNumbers.clear();
  this->eventNumbers.clear();
}

void TQUniqueCut::setActive(bool active){
  // activate/deactivate this instance
  this->enabled = active;
}

TObjArray* TQUniqueCut::getOwnBranches(){
  // add the branches required by this instance of TQUniqueCut to the internal list of branches
  TObjArray* bNames = new TObjArray();
  TObjArray* rnb = this->runNumberObservable ? this->runNumberObservable->getBranchNames() : nullptr;
  TObjArray* enb = this->eventNumberObservable ? this->eventNumberObservable->getBranchNames() : nullptr;
  
  if(rnb){
        bNames -> AddAll(rnb);
        rnb->SetOwner(false);
        delete rnb;
  }
  
  if(enb){
        bNames -> AddAll(enb);
        enb->SetOwner(false);
        delete enb;
  }
  //bNames->Add(new TObjString(this->runNumberBranch));
  //bNames->Add(new TObjString(this->eventNumberBranch));
  return bNames;
}


bool TQUniqueCut::checkUnique(std::set<long>& entries, long newEntry){
  // check if an entry is new to a set
  // if so, add it at the appropriate place and return true
  // else return false
  DEBUGclass("entering function");
  if (entries.count(newEntry)) return false;
  entries.insert(newEntry);
  return true;
}

long TQUniqueCut::getIndex(std::vector<long>& entries, long entry){
  // retrieve the index of an entry in a sorted vector
  // if the entry is not in the vector yet, add it appropriately
  // return the index of the element
  DEBUGclass("attempting to find index of %d",entry);
  if(entries.size() < 1){
    DEBUGclass("adding %d as first entry",entry);
    entries.insert(entries.begin(),entry);
    return 0;
  }
  long idx = entries.size() -1;
  DEBUGclass("checking entry %d against %d",entry,entries[idx]);
  if(entry > entries[idx]){
    DEBUGclass("adding entry %d at %d",entry,idx);
    entries.push_back(entry);
    return idx+1;
  }
  DEBUGclass("searching for entry %d",entry);
  while(idx >= 0 && entry < entries[idx]){
    idx--;
  }
  DEBUGclass("stopping search at index %d",idx);
  if(idx < 0 || entry > entries[idx]){
    long newidx = 1+idx;
    DEBUGclass("did not find entry %d, inserting  at %d",entry,newidx);
    entries.insert(entries.begin()+newidx,entry);
    return newidx;
  }
  DEBUGclass("found %d==%d at %d",entry,entries[idx],idx);
  return idx;
}


bool TQUniqueCut::passed(bool doPrint) const {
  // checks the run and event numbers of the current event
  // returns true if they are unique, false if this combination 
  // has already been encountered before
  if(!this->enabled) {
    if (doPrint) {
      INFOclass("[TreeIndex: %ld] UniqueCut '%s' passed (cut not enabled)", this->fTree?this->fTree->GetReadEntry():-1, this->GetName());
    }
    return true;
  }
  long event = this->eventNumberObservable->getValue();
  long run = this->runNumberObservable->getValue();
  
  DEBUGclass("checking unique event %ld : %ld",run,event);
  
  long runIdx = getIndex(runNumbers,run);
  if(runNumbers.size() > eventNumbers.size()){
    DEBUGclass("extending eventnumber list");
    auto b = eventNumbers.begin();
    std::set<long> newelem;
    eventNumbers.insert(b+runIdx,newelem);
  }
  #ifdef _DEBUG_
  if(runIdx >= (long)(eventNumbers.size())){
    throw std::runtime_error("event number list has insufficient length!");
  } else {
    DEBUGclass("event number list length %ld is suffucient for run index %ld",(long)(eventNumbers.size()),runIdx);
  }
  #endif
  
  DEBUGclass("checking unique for run %ld at %ld (%ld events)",runNumbers[runIdx],runIdx,eventNumbers[runIdx].size());
  const bool unique = TQUniqueCut::checkUnique(eventNumbers[runIdx],event);
  DEBUGclass("returning %d",unique);
  if(!unique){
    if (doPrint || TQUniqueCut::printOnVeto) {
      INFOclass("[TreeIndex: %ld] UniqueCut failed: event known: runNumber %ld : eventNumber %ld", this->fTree?this->fTree->GetReadEntry():-1, run, event);
    } else {
      DEBUGclass("[TreeIndex: %ld] UniqueCut failed: event known: runNumber %ld : eventNumber %ld", this->fTree?this->fTree->GetReadEntry():-1, run, event);
    }
    #ifdef _DEBUG_
    //this->printLists();
    #endif
  } else {
    if (doPrint) {
      INFOclass("[TreeIndex: %ld] UniqueCut passed: new event: runNumber %ld : eventNumber %ld",this->fTree?this->fTree->GetReadEntry():-1, run,event);
    } else {
      DEBUGclass("[TreeIndex: %ld] UniqueCut passed: new event: runNumber %ld : eventNumber %ld",this->fTree?this->fTree->GetReadEntry():-1, run,event);
    }
    
  }
  return unique;
}

bool TQUniqueCut::initializeObservables() {
  // initialize the observables required by this TQUniqueCut
  this->eventNumberObservable = TQObservable::getObservable(this->eventNumberBranch,this->fSample);
  this->runNumberObservable = TQObservable::getObservable(this->runNumberBranch,this->fSample);

  if (!runNumberObservable || !eventNumberObservable) return false;
  bool ok = true;
  if (!runNumberObservable ->initialize(this->fSample)) {
    ERRORclass("Failed to initialize runNumberObservable obtained from expression '%s' in TQUniqueCut '%s' for sample '%s'",this->runNumberBranch.Data(), this->GetName(), this->fSample->getPath().Data()); 
    ok = false;
  }
  
  if (!eventNumberObservable->initialize(this->fSample)){
    ok = false;
  }

  if (!ok) {
    this->finalizeObservables();
    return false;
  }
  return true;
}

bool TQUniqueCut::finalizeObservables() {
  // finalize the observables required by this TQUniqueCut
  if (runNumberObservable && eventNumberObservable){
    return runNumberObservable ->finalize() && eventNumberObservable ->finalize();
  }
  if(runNumberObservable) runNumberObservable ->finalize();
  if(eventNumberObservable) eventNumberObservable ->finalize();
  return false;
}


void TQUniqueCut::printLists() const {
  // print the internal lists of run and event numbers
  // WARNING: these lists can be excessively long!
  for (size_t i=0; i<runNumbers.size(); i++) {
    std::cout << "===" << runNumbers[i] << "===" << std::endl;
    size_t length = eventNumbers[i].size();
    for (const long& en : eventNumbers[i]) {
      if (--length) {
        std::cout << en << ", ";
      } else {
        std::cout<< en << std::endl; //last entry
      }
    }
  }
}

bool TQUniqueCut::initializeSelfSampleFolder(TQSampleFolder* sf){
  //@tag:[resetUniqueCut] If this boolean sample folder tag is set to true the run and event numbers of this cut are cleared upon initialization and finalization of the unique cut.
  if(sf->getTagBoolDefault("resetUniqueCut",false)) this->clear();
  return true;
}

bool TQUniqueCut::finalizeSelfSampleFolder(TQSampleFolder* sf){
  if(sf->getTagBoolDefault("resetUniqueCut",false)) this->clear();
  return true;                                        
}
