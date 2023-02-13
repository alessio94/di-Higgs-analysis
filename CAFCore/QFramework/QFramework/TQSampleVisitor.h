//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQSampleVisitor__
#define __TQSampleVisitor__

#include "TNamed.h"
#include "TQStringUtils.h"

class TQSample;
class TQSampleFolder;
class TQTaggable;

class TQSampleVisitor : public TNamed {

protected:

  int fSampleColWidth;
  bool fVerbose;
  bool fPrettyPrint;
  bool fUpdateLines;

  TString fVisitTraceID;
  TString fStatusLine; //store the last printed status line to allow updates from child classes (e.g. indicating event loop progress)

  static TString statusSKIPPED(bool pretty);
  static TString statusOK(bool pretty);
  static TString statusFAILED(bool pretty);
  static TString statusWARN(bool pretty);
  static TString statusSKIPPEDOK(bool pretty);
  static TString statusRUNNING(bool pretty);
  static TString statusPROGRESS(bool pretty,double fraction);


  virtual int visitFolder(TQSampleFolder * sampleFolder, TString& message);
  virtual int visitSample(TQSample * sample, TString& message);
  virtual int revisitSample(TQSample * sample, TString& message);
  virtual int revisitFolder(TQSampleFolder * sampleFolder, TString& message);


  TString printLine(TQSampleFolder* f, int level, bool isSample, const TString& bullet);
  void updateLine(const TString& line, const TString& message, int result, bool ignore=false, double progress = 0.);
  inline void leaveTrace(TQSampleFolder* sf, TString prefix, int result, const TString& message);
  int getStatusID(TQSampleFolder* sampleFolder, TString prefix);
  bool callInitialize(TQSampleFolder * sampleFolder);
  int callVisit(TQSampleFolder * sampleFolder, int level, bool requireSelectionTag = false);
  bool callFinalize();

public:

  enum visitSTATUS {
    visitIGNORE = 9,
    visitLISTONLY = 8,
    visitPROGRESS = 7,
    visitSKIPPEDOK = 6,

    visitSKIPPED = 0,
    visitOK = 1,
    visitWARN = 2,
    visitFAILED = 3
  };

  static TString getStatusString(int status, bool prettyPrint, double progress=0.);

  TQSampleVisitor(const TString& name = "vis");

  void setVerbose(bool verbose = true);
  void setPrettyPrint(bool pretty = true);
  void setLineUpdates(bool allow = true);

  /* called before the first element is visited */
  virtual int initialize(TQSampleFolder * sampleFolder, TString& message);
  /* called after last element was visited */
  virtual int finalize();

  int visit(TQSampleFolder * sampleFolder, bool requireSelectionTag = false);

  bool setVisitTraceID(TString id);
  TString getVisitTraceID() const ;
  const char* getVisitTraceIDConst() const;

  void stamp(TQTaggable* obj) const;
  bool checkVisit(TQTaggable* obj) const;
  bool checkVisitFriends(TQSampleFolder* obj) const;
  static bool checkRestrictionTag(TQSampleFolder* sf);

  static void updateFriends(TQSampleFolder* sf);

  virtual ~TQSampleVisitor();

  ClassDefOverride(TQSampleVisitor, 0); //QFramework class

};

#endif
