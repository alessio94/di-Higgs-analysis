//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQ_SIGNIFICANCEEVALUTATOR__
#define __TQ_SIGNIFICANCEEVALUTATOR__

#include "TString.h"
#include "TNamed.h"
#include "QFramework/TQTaggable.h"
#include "TH1.h"
#include "THnBase.h"

#include <functional>

class TQSampleDataReader;
class TQGridScanner;
class TQSampleFolder;

class TQSignificanceEvaluator : public TQTaggable, public TNamed {
protected:
  TClass* initialization; //!
  TQSampleDataReader* reader; //!
  TQGridScanner* scanner; //!
  double luminosity;
  double luminosityScale;
  TString fileIdentifier; //!
  bool verbose; //!
  std::vector<TString> m_regions; //!
  std::vector<TString> autoNFs; //!

  std::vector< std::function<double (double, double)> > m_functionsFOM ; //!
  std::vector< std::function<double (double, double, double)> > m_functionsFOMWithBkgUnc ; //!
  std::vector< std::function<double (double, double, double, double)> > m_functionsFOMWithUnc ; //!
  std::vector <TString> m_FOMDefinitions; //!
  std::vector<THnBase*> m_bkgHists; //!
  std::vector<THnBase*> m_signalHists; //!
  
public:
  TString info;
  bool m_multipleFOMsCompatible = false;
  std::vector<TString> FOMDefinitions() {return m_FOMDefinitions;};
  std::vector<TString> regions() {return m_regions;};
  
  virtual double getLuminosity(TString folderName="info",TString tagName="luminosity");
  virtual bool scaleLuminosity(double lumi = 1000);
  virtual void setFileIdentifier(TString s);
  virtual void setVerbose(bool v = true);
  virtual bool setRangeAxis(int axis, double low, double up);
  virtual bool updateHists(std::vector<int> axisToScan, TQGridScanner* scanner, int axisToEvaluate);
  TQSampleDataReader* getReader();

  virtual void bookNF(const TString& path);
  virtual void addRegion(const TString& cutname);

  virtual bool hasNativeRegionSetHandling();
  virtual bool prepareNextRegionSet(const TString& suffix = "");
  virtual bool isPrepared();

  virtual double evaluate() = 0;
  virtual std::vector<double> evaluateMultiple() = 0;
  TQSignificanceEvaluator(const TString& name = "");
  TQSignificanceEvaluator(const TString& name, TQSampleFolder* sf);
  virtual bool initialize(TQGridScanner* scanner) = 0;

  virtual double getSignificance(size_t iregion) = 0;
  virtual double getSignificance2(size_t iregion) = 0;

  virtual bool addFunctionsFOM(std::vector<TString> fomDefinitions, std::vector<double>* parameter = nullptr);
  virtual std::vector<double> getFOMs(size_t iregion);

  double signal(size_t iregion);
  std::pair<double, double> signalAndError(size_t iregion);
  double bkg(size_t iregion);
  std::pair<double, double> bkgAndError(size_t iregion);

  ClassDefOverride(TQSignificanceEvaluator,1) // base class for singificance estimation
};

class TQSignificanceEvaluatorBase : public TQSignificanceEvaluator {
protected:
  TString signalPath; //! 
  TString backgroundPath; //!

  TH1F* bkgproj;
  TH1F* sigproj;
  virtual double getSignificance(size_t iregion) override;
  virtual double getSignificance2(size_t iregion) override;
  virtual double significance(size_t iregion, const std::function<double (double, double)>& signifFunc);

public:
  virtual double evaluate() override;
  virtual std::vector<double> evaluateMultiple() override;
  virtual bool initialize(TQGridScanner* scanner) override;
  virtual void printHistogramAxis();
  virtual bool setRangeAxis(int axis, double low, double up) override;
  virtual bool updateHists(std::vector<int> axisToScan, TQGridScanner* scanner, int axisToEvaluate) override;
  TQSignificanceEvaluatorBase(TQSampleFolder* sf=NULL, TString signal="sig", TString background="bkg", TString name="s/sqrt(b)");

  ClassDefOverride(TQSignificanceEvaluatorBase,1)
};

class TQSimpleSignificanceEvaluator : public TQSignificanceEvaluatorBase {
protected:
  virtual double getSignificance(size_t iregion) override;
public:
  TQSimpleSignificanceEvaluator(TQSampleFolder* sf=NULL, TString signal="sig", TString background="bkg", TString name="s/sqrt(b)");

  ClassDefOverride(TQSimpleSignificanceEvaluator,1) // simple s/sqrt(s+b) significance evaluator
};

class TQSimpleSignificanceEvaluator2 : public TQSignificanceEvaluatorBase {
protected:
  virtual double getSignificance(size_t iregion) override;
public:
  TQSimpleSignificanceEvaluator2(TQSampleFolder* sf=NULL, TString signal="sig", TString background="bkg", TString name="s/sqrt(s+b)");

  ClassDefOverride(TQSimpleSignificanceEvaluator2,1) // simple s/sqrt(s+b) significance evaluator
};

class TQSimpleSignificanceEvaluator3 : public TQSignificanceEvaluatorBase {
protected:
  virtual double getSignificance(size_t iregion) override;
public:
  TQSimpleSignificanceEvaluator3(TQSampleFolder* sf=NULL, TString signal="sig", TString background="bkg", TString name="s/b");

  ClassDefOverride(TQSimpleSignificanceEvaluator3,1) // simple s/b significance evaluator
};

class TQPoissonSignificanceEvaluator : public TQSignificanceEvaluatorBase {
protected:
  virtual double getSignificance(size_t iregion) override;
public:
  TQPoissonSignificanceEvaluator(TQSampleFolder* sf=NULL, TString signal="sig", TString background="bkg", TString name="poisson");

  ClassDefOverride(TQPoissonSignificanceEvaluator,1) // simple poisson significance evaluator
};

#endif
