//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef MODEL_BUILDER_H
#define MODEL_BUILDER_H

#include "SFramework/TSModelFactory.h"
#include "SFramework/TSSystematicsManager.h"
#include "QFramework/TQSampleFolder.h"
#include "QFramework/TQTaggable.h"

class TSModelBuilder : public TSSystematicsManager {

protected:

  int errorCount = 0;
  int maxErrorCount = 1000;
  int warnCount = 0;
  int maxWarnCount = 1000;
  
  TQSampleFolder * fDefaultSampleFolder;

  virtual Bool_t createChannelDefinitions(TQFolder * config, TQFolder * model);
  virtual Bool_t createChannelDefinition(TQFolder * config, TQFolder * model, TString name);



  virtual std::vector<int> getRemapping(TQFolder * channelConfig, TQSampleFolder * refSamples,TString refPath, TString refHistogram, TQTaggable * refHistogramOptions, Int_t remapX, Int_t remapY, Int_t &dim, Bool_t remapSlices);
  virtual std::vector<int> getRemappingOptimizedSgnf(TQFolder * channelConfig, TQSampleFolder * refSamples, TString sigPath, TString bkgPath, TString histname, TQTaggable * histogramOptions, Int_t &dim);


  virtual std::vector<int> getMergeBins(TQFolder * config, TQFolder * channelConfig, std::map<TQFolder*,TH1*> histograms, TString varname, Bool_t isNominal, Int_t &dim); // remco
  virtual TQSampleFolder * getSampleFolder(TQFolder * config);

  virtual Bool_t collectAllHistograms(TQFolder * config, TQFolder * model);
  virtual Bool_t collectHistograms(TQFolder * config, TQFolder * model, TQFolder* variation);
  virtual Bool_t collectHistograms(TQFolder * config, TQFolder * model, TQSampleFolder * samples, TQFolder* variation);
  
  void info(TString message);
  void error(TString message);
  void warn(TString message);

public:

  TSModelBuilder();

  static void applyEdits(TQFolder* edit, TQFolder* model);
  static void applyStyle(TQFolder* model, const TString& samplename, TH1* hist);
  
  virtual void setDefaultSampleFolder(TQSampleFolder * sampleFolder);
  virtual TQSampleFolder * getDefaultSampleFolder();

  virtual TQFolder * createDefinition(TQFolder * config);
  virtual Bool_t finalizeModel(TQFolder * model, TQFolder * config);
  virtual TQFolder * buildModel(TQFolder * config);
  void purgeVariation(TQFolder* model, const TString& name, Bool_t notify = false);

  Bool_t parseConversion(TString def, Bool_t &alongX,
                                 Bool_t &includeUnderflowX, Bool_t &includeOverflowX,
                                 Bool_t &includeUnderflowY, Bool_t &includeOverflowY);
  TH1* processHistogram(TQFolder* config, TQFolder* sampleDef, const TH1* histo);


  
  virtual ~TSModelBuilder();
		
  ClassDefOverride(TSModelBuilder, 0);

};

#endif
