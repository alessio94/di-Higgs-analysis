//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQPlotter__
#define __TQPlotter__

#include "QFramework/TQPresenter.h"
#include "QFramework/TQStringUtils.h"

class TPad;
class TCanvas;
class TLegend;
class TH1;
class TGraphErrors;
class TGraphAsymmErrors;


class TQSampleDataReader;

class TQPlotter : public TQPresenter {

protected:

  TDirectory* objects; //!
 
  virtual void drawLabels(TQTaggable& tags);
   
  void setAxisLabels(TQTaggable& tags);
  TString makeHistogramIdentifier(TQNamedTaggable* process);

  virtual double getHistogramUpperLimit(TQTaggable& tags, TList * histograms, double lower, bool includeErrors = false);
  virtual bool checkConsistency(TH1 * &hMaster, TObjArray * histograms);

  bool includeSystematics(TQTaggable& tags);
  void setErrors(TQTaggable& tags, const TString& hname);

  void applyBlinding(TQTaggable& tags, TCollection* sigHistos, TCollection* backgroundHistos, TCollection* dataHistos);

  virtual TObjArray* collectHistograms(TQTaggable& tags);
  virtual TObjArray * getHistograms(TObjArray* processes, const TString& tagFilter, const TString& histogramName, const TString& namePrefix, TQTaggable& aliases, TQTaggable& options);
 
  virtual bool collectOptScanSimplifiedSystHistograms(std::vector<TH1*>& histos, TQTaggable& tags);
  
  virtual void addObject(TNamed* obj, const TString& key = "");
  virtual void addObject(TGraph* obj, TString key = "");
  virtual void addObject(TH1* obj, const TString& key = "");
  virtual void addObject(TCollection* obj, const TString& key = "");
  virtual void removeObject(const TString& key, bool deleteObject = false);

  virtual void clearObjects();
  virtual void deleteObjects();
 
  virtual void applyStyle(TQTaggable& tags, TAxis* a, const TString& key, double distscaling = 1., double sizescaling = 1.);
  virtual void applyStyle(TQTaggable& tags, TH1* hist, const TString& key, double xscaling = 1., double yscaling = 1.);
  virtual void applyGeometry(TQTaggable& tags, TAxis* a, const TString& key, double distscaling = 1., double sizescaling = 1.);
  virtual void applyGeometry(TQTaggable& tags, TH1* hist, const TString& key, double xscaling = 1., double yscaling = 1.);
  virtual void applyStyle(TQTaggable& tags, TGraph* g, const TString& key, double xscaling = 1., double yscaling = 1.);
  virtual void applyGeometry(TQTaggable& tags, TGraph* g, const TString& key, double xscaling = 1., double yscaling = 1.);

  virtual bool plotAndSaveAsInternal(const TString& histogram, const TString& saveAs, TQTaggable& tags) = 0;
  
public:

  virtual int getNProcesses(const TString& tagFilter);
 
  virtual void printObjects();
  template <class Type>
  Type* getObject(const TString& key){
    // retrieve a graphics object by name and cast to the given class
    return dynamic_cast<Type*>(this->getTObject(key));
  }
  TObject* getTObject(const TString& key);
  
  TQPlotter();
  TQPlotter(TQSampleFolder * baseSampleFolder);
  TQPlotter(TQSampleDataReader * dataSource);

  virtual bool addData (TString path, TString options = "");
  virtual bool addBackground(TString path, TString options = "");
  virtual bool addSignal (TString path, TString options = "");

  virtual bool plotAndSaveAs(const TString& histogram, const TString& saveAs, const char* inputTags);
  virtual bool plotAndSaveAs(const TString& histogram, const TString& saveAs, const TString& inputTags);
  virtual bool plotAndSaveAs(const TString& histogram, const TString& saveAs, TQTaggable& inputTags);
  virtual bool plotAndSaveAs(const TString& histogram, const TString& saveAs, TQTaggable * inputTags = 0);

  virtual ~TQPlotter(); 

  virtual void reset();

  int sanitizeProcesses();

  static TString createAxisTagsAsString(const TString& prefix, const TString& title, double xCoeff, double yCoeff, double constCoeff, double wMin, double wMax, double xCoord, double yCoord, int nDiv = 101);
  static bool createAxisTags(TQTaggable& tags, const TString& prefix, const TString& title, double xCoeff, double yCoeff, double constCoeff, double wMin, double wMax, double xCoord, double yCoord, int nDiv = 101);
  static TString createAxisTagsAsConfigString(const TString& prefix, const TString& title, double xCoeff, double yCoeff, double constCoeff, double wMin, double wMax, double xCoord, double yCoord, int nDiv = 101);
  static TQTaggable* createAxisTags(const TString& prefix, const TString& title, double xCoeff, double yCoeff, double constCoeff, double wMin, double wMax, double xCoord, double yCoord, int nDiv = 101);

  static void estimateRangeY(TH1* h, double& min, double &max, double tolerance = std::numeric_limits<double>::infinity());
  static void estimateRangeY(TGraphErrors* g, double& min, double &max, double tolerance = std::numeric_limits<double>::infinity());
  static void estimateRangeY(TGraphAsymmErrors* g, double& min, double &max, double tolerance = std::numeric_limits<double>::infinity());
  static void getRange(TGraphErrors* g, double &xlow, double &xhigh, double &ylow, double &yhigh, bool get_xrange=true, bool get_yrange=true, double maxQerr = std::numeric_limits<double>::infinity());
 
  TGraphAsymmErrors* getRatioErrorGraph(TH1* hTotakBkg);
  TGraphAsymmErrors* getRatioGraph(TH1* h_data, TH1* hTotakBkg, bool invert, double ratioContentThreshold, bool verbose);

  TString getScaleFactorList(TString histname);

  ClassDefOverride(TQPlotter, 0); //Base class for QFramework plotters

};

#endif
