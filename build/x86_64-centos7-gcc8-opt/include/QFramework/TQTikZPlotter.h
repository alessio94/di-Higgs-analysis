//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQTikZPlotter__
#define __TQTikZPlotter__

#include "QFramework/TQPlotter.h"
#include "QFramework/TQHistogramUtils.h"
#include <ostream>

class TQTikZPlotter : public TQPlotter {
protected:

  std::vector<TH1*> f_stack;        //!
  std::vector<TH1*> f_data;         //!
  std::vector<TH1*> f_signal;       //!
  std::map<TH1*,TString> f_styles;  //!

  void makeStyles(TQTaggable& tags,std::ostream& output);
  using TQPlotter::drawLabels;
  void drawLabels(TQTaggable& tags,std::ostream& output);
  void drawStack  (TQTaggable& tags,std::ostream& output);
  void drawRatio  (TQTaggable& tags,std::ostream& output);
  void drawDminusB(TQTaggable& tags,std::ostream& output);
  void drawOptScan(TQTaggable& tags,std::ostream& output,TQHistogramUtils::FOM FOMmode);
  virtual bool makePlot(std::ostream& output, TQTaggable& inputTags);

  virtual bool plotAndSaveAsInternal(const TString& histogram, const TString& saveAs, TQTaggable& tags) override;

public:
  TQTikZPlotter(TQSampleFolder* sf):TQPlotter(sf) {};
  TQTikZPlotter(TQSampleDataReader* rd):TQPlotter(rd) {};

  void histogramToTikZCoordinatesMC(std::ostream& os, TH1* hist);
  void histogramToTikZCoordinatesData(std::ostream& os, TH1* hist);
  void histogramErrorsToTikZCoordinates(std::ostream& os, TH1* hist, bool up);

  ClassDefOverride(TQTikZPlotter,0); //A default plotter

};

#endif
