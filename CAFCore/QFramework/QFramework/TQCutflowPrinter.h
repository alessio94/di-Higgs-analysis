//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQCutflowPrinter__
#define __TQCutflowPrinter__

#include "QFramework/TQPresenter.h"
#include "QFramework/TQTable.h"

class TQCutflowPrinter : public TQPresenter {

protected:

  bool fVerbose = true;
  
  bool getScaleFactors (TQTaggable& tags, TQNamedTaggable* cut, TList* sfList, double& value, double& error, bool& applied, bool& equal, TString& info);
  bool getValues (TQTaggable& tags, TQNamedTaggable* cut, TQNamedTaggable* process, double& value, double& error, int& raw, TString& info, TString& defaultTitle, TList* sfList);
  bool getValues(TQTaggable& tags, TQNamedTaggable* cut, TQNamedTaggable* process, double& number, double& statError, double& expSysError, double& theoSysError, bool includeScaleUncertainty, int& raw, TString& info, TString& defaultTitle, TList* sfList);
  bool getScaleFactors (TQTaggable& tags, TQNamedTaggable* cut, TQNamedTaggable* process, double& value, double& error, bool& applied, bool& equal, TString& info);
  void getCounterValueAndStatError(
      const TString& processName,
      const TString& cutName,
      double& count,
      double& statErr,
      int& raw,
      TString& info,
      TQTaggable tags,
      TList* sfList
  );
  void getCounterValueAndErrors(
      const TString& processName,
      const TString& cutName,
      double& count,
      double& statErr,
      double& theoSysErr,
      int& raw,
      TString& info,
      TQTaggable tags,
      TList* sfList
  );
  TString makeCellContents (TQTaggable& tags, TQNamedTaggable* cut, TQNamedTaggable* process);
  TString makeSFCellContents(TQTaggable& tags, TQNamedTaggable* cut, TQNamedTaggable* process, TString& info);
 
public:

  TQCutflowPrinter();
  TQCutflowPrinter(TQSampleFolder * samples);
  TQCutflowPrinter(TQSampleDataReader * reader);

  bool readProcessesFromFile(TString fileName, TString channel);
  bool readCutsFromFile (TString fileName, bool addscline=false);

  void addCutflowCut (TString cutName, TString cutTitle, int sfPolicy=0);
  void addCutflowProcess(TString processName, TString processTitle);

  int sanitizeProcesses();
  int sanitizeCuts();

  void setVerbose(bool v);

  TQTable * createTable(const TString& tags = "");
  TQTable * createTable(TQTaggable* tags);
  TQTable * createTable(TQTaggable tags);
 
  void dumpProcesses(std::ostream& out);
  void dumpCuts(std::ostream& out);

  virtual ~TQCutflowPrinter();
 
  ClassDefOverride(TQCutflowPrinter, 0); // presenter class to print cutflows

};

#endif
