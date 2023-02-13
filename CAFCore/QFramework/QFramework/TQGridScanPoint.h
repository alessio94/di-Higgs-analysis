//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQ_GRIDSCANPOINT__
#define __TQ_GRIDSCANPOINT__

#include <iostream>
#include "TH1.h"
#include "TString.h"

class TQGridScanNormalObservable;
class TQGridScanSplitObservable;


// This class holds significance data for a given set of cuts
// It holds the bin numbers corresponding to these cut values
// When the cut values themselves are requested, they are lazily evaluated from the 
// corresponding hist of the grid scan results and stored in memory. The results can then be
// re-written to disck to store the cut values permanently.
class TQGridScanPoint : public TObject {
public:
  using NormalBounds = std::vector<std::pair<int, int>>; // bin numbers
  using SplitBounds = std::vector<int>;
  using NormalVals = std::vector<std::pair<double, double>>; // values of bin edges
  using SplitVals = std::vector<double>;
  constexpr static double kValNotSet = std::numeric_limits<double>::quiet_NaN();
  //using NormalObsVec = std::vector<TQGridScanNormalObservable*>;
  //using SplitObsVec = std::vector<TQGridScanSplitObservable*>;

  TQGridScanPoint() = default;

  TQGridScanPoint(
      NormalBounds normalBounds,
      SplitBounds splitBounds,
      double significance,
      TString info
  );
  TQGridScanPoint(
      NormalBounds normalBounds,
      double significance,
      TString info
  );
  TQGridScanPoint(
      NormalBounds normalBounds,
      SplitBounds splitBounds,
      std::vector<double> foms, // figure of merits
      TString info
  );
TQGridScanPoint(
    NormalBounds normalBounds,
    std::vector<double> foms,
    TString info
);

  double significance() const { return m_significance; }
  std::vector<double> foms() const { return m_foms; }

  const NormalBounds& normalBounds() const { return m_normalBounds; }
  const SplitBounds& splitBounds() const { return m_splitBounds; }
  const NormalVals& normalVals(const std::vector<TQGridScanNormalObservable*>& obsVec);
  const SplitVals& splitVals(const std::vector<TQGridScanSplitObservable*>& obsVec);
  
  void print();

  const TString& info() const { return m_info; }

  // Pointers to the normal and split scan observables are stored to access bin edge values
  //static void setNormalObs(NormalObsVec* obsVec) { normalObs = obsVec; }
  //static void setSplitObs(SplitObsVec* obsVec) { splitObs = obsVec; }
  
  //static NormalObsVec* normalObs;
  //static SplitObsVec* splitObs;

  friend bool operator < (const TQGridScanPoint& lhs, const TQGridScanPoint& rhs) { return lhs.significance() < rhs.significance(); }
  friend bool operator > (const TQGridScanPoint& lhs, const TQGridScanPoint& rhs) { return lhs.significance() > rhs.significance(); }

  ClassDefOverride(TQGridScanPoint,2)  // auxiliary class for the TQGridScanner

protected:
  double m_significance = 0.0;
  std::vector<double> m_foms;
  TString m_info;
  
  NormalBounds m_normalBounds;
  SplitBounds m_splitBounds;
  NormalVals m_normalVals;
  SplitVals m_splitVals;
};

//bool operator < (const TQGridScanPoint& first, const TQGridScanPoint& second) {
//  return (first.significance() < second.significance());
//}
//bool operator > (const TQGridScanPoint& first, const TQGridScanPoint& second) {
//  return (first.significance() > second.significance());
//}

#endif

