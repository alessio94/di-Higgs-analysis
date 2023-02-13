//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQAlgorithm__
#define __TQAlgorithm__

#include "TQNamedTaggable.h"
#include <map>

class TQSample;
class TQSampleFolder;

class TQAlgorithm : public TQNamedTaggable {
public:
  class Manager { // nested
  protected:
    std::vector<TQAlgorithm*> gAlgorithmList; //list of algorithm "templates", will be cloned for multiple channels
//    std::vector<std::vector<TQAlgorithm*>> gClonedAlgorithmList;
    std::vector<TQAlgorithm*> gActiveAlgorithms; //list of algs activated for current set of samples (MCASV)
    std::map<TQAlgorithm*,std::map<TString,TQAlgorithm*>> gAlgorithmStore; //list of all algs ever cloned including currently not active ones. First index is the original algorithm from which the channel specific ones are cloned. string is the name of the channel
    
    template <class T> std::map<TQAlgorithm*,T*> getAlgorithmToSampleFolderMap(const std::vector<T*>& samples, const TString& tagKey);
    
  public:
    bool addAlgorithm(TQAlgorithm* newAlgorithm);
    //bool cloneAlgorithms(int n);
    void clearAlgorithms();
    void clearClonedAlgorithms();
    void resetActiveAlgorithms();
    const std::vector<TQAlgorithm*>& getAlgorithms();

    void printAlgorithms() const;
    void printActiveAlgorithms() const;
    bool isActiveAlgorithm(const TQAlgorithm* alg) const;
    bool initializeAlgorithms(TQSample*s);
    bool initializeAlgorithms(TQSampleFolder*s);
    template <class T> bool initializeClonedAlgorithms(std::vector<T*>& samples, const TString& tagKey, bool switchObservableSets = false);
    template <class T> bool finalizeClonedAlgorithms(std::vector<T*>& samples, const TString& tagKey);
    bool finalizeAlgorithms();
    bool finalizeAlgorithmsSampleFolder(TQSampleFolder* sf);    
    bool executeAlgorithms();
    bool executeClonedAlgorithms();
    bool cleanupAlgorithms();
    bool cleanupClonedAlgorithms();
        
  };
  
  virtual bool initializeSingleton(const std::vector<TQSample*>& samples); //at least one of the initialization methods must be overridden!
  virtual bool finalizeSingleton();
  virtual bool finalizeSampleFolderSingleton(const std::vector<TQSampleFolder*>& samples);
  virtual bool initializeSampleFolderSingleton(const std::vector<TQSampleFolder*>& samples);  
  virtual bool initializeSampleFolder(TQSampleFolder* s);
  virtual bool finalizeSampleFolder(TQSampleFolder* sf);  
  virtual bool initialize(TQSample* s);
  virtual bool finalize() = 0;
  virtual bool execute() = 0; //called at the beginning of each event (i.e. before the first cut)

  virtual TQAlgorithm* getClone() const;
  
  virtual bool cleanup() = 0; //called at the end of each event (i.e. after all cuts)
  
  virtual bool isSingleton() const; //can be used to indicate that an algorithm should not be cloned when running multiple channels at once
  
  virtual TObjArray * getListOfBranches() { return nullptr;} // if your algorithm internally uses Observables (even indirectly!), you should override this function and return a list ob branch names these Observables need to be enabled! (for reference, see the same-named function in TQCut!)

  ClassDefOverride(TQAlgorithm,0) // algorithm to be run on an event before the event loop reads it

};
#endif
