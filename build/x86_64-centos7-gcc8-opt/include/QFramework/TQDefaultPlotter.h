//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQDefaultPlotter__
#define __TQDefaultPlotter__

#include "QFramework/TQROOTPlotter.h"

class TQDefaultPlotter : public TQROOTPlotter {
public:
  TQDefaultPlotter(TQSampleFolder* sf):TQROOTPlotter(sf) {};
  TQDefaultPlotter(TQSampleDataReader* rd):TQROOTPlotter(rd) {};

protected:
  virtual TCanvas * makePlot(TQTaggable& inputTags) override;

  virtual void drawLabels(TQTaggable& tags) override;
  
  void drawSub_Ratio(TQTaggable& tags);
  void drawSub_CutOptimization(TQTaggable& tags);
  void drawSub_DataMinusBackground(TQTaggable& tags);

  ClassDefOverride(TQDefaultPlotter,0); //A default plotter
  
};

#endif
