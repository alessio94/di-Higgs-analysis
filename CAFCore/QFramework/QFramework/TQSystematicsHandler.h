//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQSystematicsHandler__
#define __TQSystematicsHandler__

#include "QFramework/TQFolder.h"
#include "QFramework/TQTable.h"
#include "QFramework/TQSystematicsManager.h"
#include "QFramework/TQSampleDataReader.h"

class TQSystematicsHandler : public TQSystematicsManager, public TQFolder {
protected:
  TQFolder* _config;
  std::map<TString,TQSampleFolder*> _inputCache;

  void collectVariation(TQFolder* var);
  void exportObjects(TQFolder* cut,TQFolder* target, bool includeNormalizationInShape = true);
  void collectHistograms(TQSampleDataReader* rd, TQFolder* variation);
  void collectCounters(TQSampleDataReader* rd, TQFolder* variation);

  virtual bool writeFolderHook(TDirectory* dir, const TString&, int, bool) override;    
  
public:

  TQSampleFolder* getSampleFolder(TQFolder* var);
  TQSampleFolder* getSampleFolder(const TString& name);  
  
  TQSystematicsHandler(const TString& name);  
  TQSystematicsHandler(const TString& name, TQFolder* cfg);
  ~TQSystematicsHandler();

  TQFolder* config();
  
  TQFolder* addCut(const TString& id);
  TQFolder* addSystematic(const TString& id, const TString& tags);
  TQFolder* addSystematic(const TString& id, const TString& upvar, const TString& dnvar);
  TQFolder* addVariation(const TString& id, const TString& tags);
  TQFolder* addSample(const TString& id, const TString& path);

  void printSystematics();

  void collect();
  void compute();  

  TQFolder* exportSystematics(const TString& sample, bool includeNormalizationInShape = true);

  std::vector<TQFolder*> getRanking(const TString& sample, const TString& cutname);
  TQTable* getTable(const TString& sample, const TString& cutname);

  ClassDefOverride(TQSystematicsHandler,0) // helper class to prepare systematic variations for plotting

};

#endif
