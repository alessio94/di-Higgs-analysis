//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQUniqueCut__
#define __TQUniqueCut__

#include "QFramework/TQObservable.h"
#include "QFramework/TQCompiledCut.h"

class TQUniqueCut : public TQCompiledCut {
protected:
  TString runNumberBranch;
  TString eventNumberBranch;
  TQObservable* runNumberObservable; //!
  TQObservable* eventNumberObservable; //!

  mutable std::vector<long> runNumbers;
  mutable std::vector<std::set<long> >eventNumbers;

  bool enabled;

  void initUniqueCut();

  virtual bool initializeObservables() override;
  virtual bool finalizeObservables() override;
  virtual bool initializeSelfSampleFolder(TQSampleFolder* sf) override;
  virtual bool finalizeSelfSampleFolder(TQSampleFolder* sf) override;
  
  virtual TObjArray* getOwnBranches() override;

public:
  static bool printOnVeto;
  
  static bool checkUnique(std::set<long>& entries, long newEntry);
  static long getIndex(std::vector<long>& entries, long entry);

  void clear();
  void setActive(bool active);

  bool isMergeable() const override;
  bool setBranchNames(const TString& runBranch, const TString& evtBranch);
  TQUniqueCut();
  TQUniqueCut(const TString& name);
  TQUniqueCut(const TString& runBranch, const TString& evtBranch);
  TQUniqueCut(const TString& name, const TString& runBranch, const TString& evtBranch);
  
  const TString& getRunNumberExpression() const;
  const TString& getEventNumberExpression() const;
  
  virtual bool passed(bool doPrint) const override;
  
  void printLists() const;

  ClassDefOverride(TQUniqueCut, 0); // cut to remove duplicate events
};

 
#endif
