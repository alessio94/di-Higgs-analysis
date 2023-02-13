//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TS_SIGNIFICANCEEVALUTATOR__
#define __TS_SIGNIFICANCEEVALUTATOR__

#include "QFramework/TQSignificanceEvaluator.h"
#include "QFramework/TQGridScanner.h"
#include "TSStatisticsManager.h"
#include "QFramework/TQFolder.h"

class TSCLSignificanceEvaluator : public TQSignificanceEvaluator {
 public:
  void keepTMP(TString tmpfilenamebase="histograms_");
  
  void printMode();
  void exportSampleFolder(bool doexport = true);
  void importSampleFolder(bool doimport = true);
  void exportSignificance(bool doexport = true);
  void importSignificance(bool doimport = true);
  void setSampleFolderFile(const TString& filename);
  void setSignificanceFile(const TString& filename);
  void setExternalEvaluation(const TString& command, bool allowRecycling = false);
  void setInternalEvaluation(bool eval = true);
  void deactivate();
  
  TQSampleFolder* getWorkingFolder(TString path = "");
  Int_t cleanWorkingFolder();
  TQSampleFolder* getBaseFolder(TString path = "");
  TQFolder* getConfigFolder(TString path = "");
  void printPaths();
  virtual double getLuminosity(TString folderName="info",TString tagName="luminosity") override;
  virtual double evaluate() override;
  void addVariation(TQSampleFolder* sf, TString varname);
  void setInitialization(TClass* initClass);
  virtual bool initialize(TQGridScanner* scanner) override;
  bool initialize(const TString& importfilename = "", const TString& exportfilename = "");
  bool configure(const TString& configfilename, const TString& modelname = "");
  TSCLSignificanceEvaluator(TQSampleFolder* sf=NULL, TString targetVar="", TString name="CL");
  TSCLSignificanceEvaluator(TList* folders, TString targetVar="", TString name="CL", TString folderKeyTag=".key");
  TSCLSignificanceEvaluator(TString targetVar, TString name="CL");
  
  virtual double getSignificance(size_t iregion) override;
  virtual double getSignificance2(size_t iregion) override;
  virtual std::vector<double> evaluateMultiple() override;

 protected:
  TQSampleFolder* baseFolder;       //!   
  TQSampleFolder* workingFolder;    //!
  TQFolder* config;		    //!
  TString tmpFileName;		    //!
  TString tmpFileNameBase;	    //!
  TString modelName;		    //!
  TString discriminantVariable;	    //!
  TSStatisticsManager* engine;	    //!
  std::vector<TString> regionsets;   //!
  std::vector<TString> locations;   //!
  std::vector<TString> histNames;  //!
  std::vector<TString> variations;  //!
  TString sampleFolderFileName;	    //!
  TString significanceFileName;	    //!
  TString evaluationCommand;	    //!
  bool internalEvaluation;          //!
  bool recyclingAllowed;          //!
  bool supportRegionSetHandling;   //!

  Int_t nPreparedRegionSets; //!

 public:

  bool checkCutoffs();
  void setRegionSetHandling(bool val);
  virtual bool hasNativeRegionSetHandling() override;
  virtual bool prepareNextRegionSet(const TString& suffix = "") override;
  virtual bool isPrepared() override;
  
  enum Mode { IMPORT, EXPORT, NONE };
  
 protected:
  Mode sampleFolderMode;
  Mode significanceMode;
  Int_t prepareFromGridScanner(const TString& /*suffix = ""*/);
  
  
  ClassDefOverride(TSCLSignificanceEvaluator,1)
};

#endif
