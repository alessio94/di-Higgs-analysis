//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef HISTOGRAM_EXPORTER
#define HISTOGRAM_EXPORTER

#include "TString.h"

#include "RooAbsPdf.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooCategory.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooRealSumPdf.h"
#include "RooSimultaneous.h"
#include "RooFitResult.h"


#include "RooAbsOptTestStatistic.h"

#include "QFramework/TQFolder.h"
#include "QFramework/TQNamedTaggable.h"

#include "TMatrixDSym.h"
#include "TVectorD.h"

#include <list>
#include <map>
#include <vector>

class RooExpectedEvents : public RooAbsReal {
  RooAbsPdf& _pdf;
  const RooArgSet& _normSet;
public:
  RooExpectedEvents(const char* name, const char* title, RooAbsPdf& pdf, const RooArgSet& normSet);
  virtual ~RooExpectedEvents();
  virtual double evaluate() const override;
  virtual TObject* clone(const char*) const override;

  ClassDef(RooExpectedEvents,1)
};

class RooPdfEvaluator : public RooAbsOptTestStatistic { //nested
  RooAbsPdf* _hesse;
  RooArgSet* nset;
  RooArgList* params;
  TMatrixDSym covMat;
  TMatrixDSym corrMat;
  double threshold;
  double ccheck;
public:
  enum ErrorCalculation {
    NoErrors=0,
    LinearErrors=1,
    SamplingErrors=2
  };

  RooPdfEvaluator(RooAbsReal* func, const RooFitResult* result, double threshold, double ccheck = 0.05);
  RooPdfEvaluator(const RooPdfEvaluator& other);
  virtual ~RooPdfEvaluator();
  virtual TObject* clone(const char*) const override;
  virtual RooAbsTestStatistic* create(const char *name, const char *title, RooAbsReal& pdf, RooAbsData& adata,
				      const RooArgSet& projDeps, const char* rangeName, const char* addCoefRangeName=0,
				      Int_t nCPU=1, RooFit::MPSplit interleave=RooFit::BulkPartition, Bool_t verbose=kTRUE, Bool_t splitRange=kFALSE, Bool_t binnedL=kFALSE) override;
  virtual Double_t evaluatePartition(
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,20,00)
                                     size_t /*firstEvent*/, size_t /*lastEvent*/, size_t /*stepSize*/
#else
                                     Int_t /*firstEvent*/, Int_t /*lastEvent*/, Int_t /*stepSize*/
#endif
                                     ) const ;

  RooAbsReal* getFunc();
  void fillHistogram(TH1 *hist,const RooArgList& plotVars, ErrorCalculation mode = RooPdfEvaluator::LinearErrors);
  double getValue() const;
  double getError(ErrorCalculation mode = RooPdfEvaluator::LinearErrors);
  void setDirty();

  int getParameterIndex(const char* pname) const;
  void setCorrelation(const char* p1, const char* p2, double val);
  double getCorrelation(const char* p1, const char* p2) const;

protected:
  TVectorD getVariations();
  bool allEntriesZero(const TVectorD& v) const;
};

class TSHistogramExporter : public TQNamedTaggable {
  std::list<RooAbsData*> allData;
  TQFolder* styleModel;
  RooFitResult* fitResult;

public:
  TString nfPattern;

protected:
  RooDataSet* getDataRegion(RooDataSet* ds, const TString& catname) const;
public:

  TSHistogramExporter(TQFolder* style = NULL, RooFitResult* fr = NULL);
  virtual ~TSHistogramExporter();

  class Region { //nested
  protected:
    TString name;
    RooAbsPdf* func;
    RooArgSet observables;
    RooSimultaneous* simPdf;
    bool combined = false;
    std::map<TString,RooRealVar*> selectionVars;
    std::map<TString,std::vector<RooAbsReal*> > samples;
    std::map<TString,std::vector<RooDataSet*> >datasets;
  public:
    void collectSelectionVars(const TString& nfPattern);
    void select();
    bool isCombined() const;
    const RooArgSet& getObservables() const;
    const TString& getName() const;
    RooCategory* getChannelCategory();
    Region(const TString& n, RooAbsPdf* p, RooArgSet* obs, bool combined=false);
    Region(const TString& n, RooSimultaneous* sim, RooRealSumPdf* p, RooArgSet* obs,bool combined=false);
    virtual ~Region();
    std::vector<TString> allData() const;
    std::vector<TString> allSamples() const;
    RooDataSet* getData(const TString& name, int index) const ;
    int nData(const TString& name) const;
    void print() const;
    RooAbsPdf* getPdf() const;
    void addData(const TString& name, RooDataSet* ds);
    void addSample(const TString& name, RooAbsReal* obj);
    void addSamples(const Region* other);
    void addSamples(RooRealSumPdf* sumPdf);
    RooAbsReal* createIntegral(const TString& nameAppend, const RooArgSet& normSet, const RooArgSet& projectedVars) const;
    RooAbsReal* createIntegral() const;
    double expectedEvents() const;
    TH1* makeHistogram(const TString& n, RooArgList& x) const;
    void selectComponents  (const std::vector<TString>& keys) const;
    void selectComponentsLegacy  (const std::vector<TString>& keys) const;
    void unselectComponentsLegacy() const;
    void unselectComponents() const;
    TString getComponentNames(const std::vector<TString>& keys) const;
    TString getSelectionVariableNames(const std::vector<TString>& keys) const;
  };

  static RooArgSet* selectBranchNodes(RooAbsReal* func, const TString& compNames);
  static void selectComponents  (RooAbsReal* func, const TString& compNames);
  static void unselectComponents(RooAbsReal* func);

  static std::vector<TSHistogramExporter::Region*> makeRegions(RooSimultaneous* pdf, const RooArgSet* observables, const std::list<RooAbsData*>& datasets);
  static std::map<TString,RooRealSumPdf*> getComponents(RooSimultaneous* pdf);
  static RooCategory* getCategories(RooSimultaneous* simPdf);


  static int addCombinedRegions(TCollection* input, std::vector<TSHistogramExporter::Region*>& regions, const std::vector<TSHistogramExporter::Region*>& origregions);
  static int addCombinedSamples(TCollection* input,std::map< const TString, std::vector<TString> >& samples, const std::vector<TString>& allSamples);

  bool getMC(TDirectory* dir, Region* region, std::map< const TString, std::vector<TString> > samples, int calculateErrors=1, double checkThreshold=0.05);
  int getData(TDirectory* dir, const TString& selector, const TString& histname, const Region* region);

  ClassDef(TSHistogramExporter,0)
};


#endif
