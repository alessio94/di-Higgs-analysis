//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQROOTPlotter__
#define __TQROOTPlotter__

#include "QFramework/TQPlotter.h"

class TQROOTPlotter : public TQPlotter {

protected:

  TObjArray* pads; //!

  using TQPlotter::addObject;
  virtual void addObject(TLegend* obj, const TString& key = "");

  void drawCutLines1D(TQTaggable& tags);
  int drawAdditionalAxes(TQTaggable& tags);
  int drawHeightLines(TQTaggable& tags);
  bool calculateAxisRanges1D(TQTaggable& tags); 
  bool calculateAxisRangesProfile(TQTaggable& tags);

  virtual bool drawProfiles(TQTaggable& tags);
  virtual void makeLegend(TQTaggable& tags, TObjArray* histos);
  virtual void stackHistograms(TQTaggable& tags, const TString& stackname);
  virtual void drawLegend(TQTaggable& tags);
  template<class T> void drawArrows(TQTaggable &tags,T *ratioGraph, double min, double max);

  virtual bool drawStack(TQTaggable& tags);
  virtual bool drawContours(TQTaggable& tags);
  virtual bool drawHeatmap(TQTaggable& tags);
  virtual bool drawMigration(TQTaggable& tags);
  
  virtual TCanvas* createCanvas(TQTaggable& tags);
  virtual TPad* createPad(TQTaggable& tags, const TString& key);

  virtual void addAllHistogramsToLegend(TQTaggable& tags, TLegend * legend, const TString& processFilter, const TString& options = "", bool reverse=false);
  virtual void addHistogramToLegend(TQTaggable& tags, TLegend * legend, const TString& identifier, TQTaggable& options);
  virtual void addHistogramToLegend(TQTaggable& tags, TLegend * legend, TH1* histo, TQTaggable& options);
  virtual void addHistogramToLegend(TQTaggable& tags, TLegend * legend, TQNamedTaggable* process, const TString& options = "");
  virtual void addHistogramToLegend(TQTaggable& tags, TLegend * legend, TH1* histo, const TString& options = "");

  virtual TCanvas * makePlot(TQTaggable& tags) = 0;

  virtual void clearObjects() override;
  virtual void deleteObjects() override;

  virtual bool plotAndSaveAsInternal(const TString& histogram, const TString& saveAs, TQTaggable& tags) override;  

public:

 
  TPad* getPad(const TString& name);
  
  TQROOTPlotter();
  TQROOTPlotter(TQSampleFolder * baseSampleFolder);
  TQROOTPlotter(TQSampleDataReader * dataSource);

  virtual TCanvas * plot(TString histogram, const TString& inputTags);
  virtual TCanvas * plot(TString histogram, const char* inputTags);
  virtual TCanvas * plot(TString histogram, TQTaggable * inputTags = 0);
  virtual TCanvas * plot(TString histogram, TQTaggable& inputTags);

  static void setStyleAtlas();

  virtual ~TQROOTPlotter(); 

  virtual void reset();

  static bool isDefaultPlotMessage(TString message, const TString& filename);
  
  ClassDefOverride(TQROOTPlotter, 0); //Base class for QFramework plotters using the ROOT graphics engine

};

#endif
