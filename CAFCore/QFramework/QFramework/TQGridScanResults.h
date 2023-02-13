#ifndef __TQ_GRIDSCANRESULTS__
#define __TQ_GRIDSCANRESULTS__

#include <iostream>
#include <vector>
#include <utility>

#include "TLine.h"
#include "TString.h"
#include "TNamed.h"
#include "TGraphAsymmErrors.h"

#include "QFramework/TQGridScanPoint.h"
#include "QFramework/TQGridScanBound.h"
#include "QFramework/TQTaggable.h"
#include "QFramework/TQGridScanObservable.h"

class TQGridScanResults : public TQTaggable, public TNamed {
public:
  using NormalBins = std::vector<std::pair<int, int>>;
  using SplitBins = std::vector<int>;
  using ViewRanges = NormalBins;
  using BoundDirection = TQGridScanBound::Direction;
  struct InputHists{TString regionName; std::vector<TH1F> hists;};
  
  TQGridScanResults() :
    TNamed("TQGridScanResults", "TQGridScanResults")
  {}
  // TNamed doesn't have a constructor for a moved TString
  TQGridScanResults(const TString& name) :
    TNamed(name.Data(), "TQGridScanResults")
  {}
  ~TQGridScanResults () {
    for (auto obs : m_normalObs) {
      delete obs;
      obs = NULL;
    }
    for (auto obs : m_splitObs) {
      delete obs;
      obs = NULL;
    }
    m_normalObs.clear();
    m_splitObs.clear();
  }

  std::vector<TString> FOMDefinitions;
  
  // Sorts the points by descending significance
  void sortPoints();
  void sortPoints(unsigned int fom_index_for_sorting);

  //void printPoint(size_t n = 0);
  void printPoint(TQGridScanPoint* point);
  void printPoints(size_t first, size_t last);
  void printPoints(size_t last = -1);
  std::unique_ptr<TLine> drawCutLine(TH1* hist);
  
  void plotAndSaveAllSignificanceProfiles(double topFraction, const TString& options, TString outPath = "");
  void plotAndSaveAllSignificanceProfiles(int topNumber, const TString& options, TString outPath = "");
  void plotAndSaveSignificanceProfile(BoundDirection direction, int i, int topNumber, const TString& options, TString outPath = "");
  std::unique_ptr<TH1F> getSignificanceProfile(BoundDirection direction, int i, int topNumber);
  std::unique_ptr<TGraphAsymmErrors> makeGraphFromLowBinEdges(TH1* hist, BoundDirection direction);
  void setFOMIndexForPlotting(int fom_index_for_plotting);

  void plotInputDistributions(TQTaggable& tags, const TString& channel = "");
  
  std::vector<TQGridScanPoint>* points() { return &m_points; }
  const std::vector<TQGridScanPoint>& points() const { return m_points; }
  std::vector<TQGridScanNormalObservable*>* normalObs() { return &m_normalObs; }
  std::vector<TQGridScanSplitObservable*>* splitObs() { return &m_splitObs; }

  std::vector<InputHists>* inputHistsSig() { return &m_inputHistsSig; }
  std::vector<InputHists>* inputHistsBkg() { return &m_inputHistsBkg; }
  
protected:
  void init();
  bool isAcceptedPoint(const TQGridScanPoint& point);

  std::vector<TQGridScanPoint> m_points;

  // These store the arguments to make histograms for the respective quantities stored in gridscan points
  std::vector<TQGridScanNormalObservable*> m_normalObs;
  std::vector<TQGridScanSplitObservable*> m_splitObs;

  std::vector<InputHists> m_inputHistsSig; 
  std::vector<InputHists> m_inputHistsBkg; 
  
  bool m_sorted = false;
  
  int m_fom_index_for_plotting = 0;

  // No idea what this does
  ClassDefOverride(TQGridScanResults, 3)
};

#endif

