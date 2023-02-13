#include "boost/optional.hpp"
#include "QFramework/TQGridScanPoint.h"
#include "QFramework/TQGridScanObservable.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQLibrary.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQGridScanPoint
//
// a single point in configuration space
// can be manipulated by any TQSignificanceEvaluator 
// to store all necessary information for later investigation
//
////////////////////////////////////////////////////////////////////////////////////////////////

using std::vector;
using std::pair;
using boost::optional;

using NormalBounds = TQGridScanPoint::NormalBounds;
using SplitBounds = TQGridScanPoint::SplitBounds;
using NormalVals = TQGridScanPoint::NormalVals;
using SplitVals = TQGridScanPoint::SplitVals;
//using NormalObsVec = TQGridScanPoint::NormalObsVec;
//using SplitObsVec = TQGridScanPoint::SplitObsVec;

ClassImp(TQGridScanPoint)

//TQGridScanPoint::NormalObsVec* TQGridScanPoint::normalObs = nullptr;
//TQGridScanPoint::SplitObsVec* TQGridScanPoint::splitObs = nullptr;

TQGridScanPoint::TQGridScanPoint(
    NormalBounds normalBounds,
    SplitBounds splitBounds,
    double significance,
    TString info
) :
  m_significance(significance),
  m_info(info),
  m_normalBounds(normalBounds),
  m_splitBounds(splitBounds)
{}

TQGridScanPoint::TQGridScanPoint(
    NormalBounds normalBounds,
    double significance,
    TString info
) :
  m_significance(significance),
  m_info(info),
  m_normalBounds(normalBounds)
{}
TQGridScanPoint::TQGridScanPoint(
    NormalBounds normalBounds,
    SplitBounds splitBounds,
    std::vector<double> foms,
    TString info
) :
  m_significance(foms[0]), // for sorting the first FOM is used!
  m_foms(foms),
  m_info(info),
  m_normalBounds(normalBounds),
  m_splitBounds(splitBounds)
{}
TQGridScanPoint::TQGridScanPoint(
    NormalBounds normalBounds,
    std::vector<double> foms,
    TString info
) :
  m_significance(foms[0]), // for sorting the first FOM is used!
  m_foms(foms),
  m_info(info),
  m_normalBounds(normalBounds)
{}

const NormalVals& TQGridScanPoint::normalVals(const vector<TQGridScanNormalObservable*>& obsVec) {
  if (m_normalVals.empty()) {
    for (size_t i = 0; i < m_normalBounds.size(); ++i) {
      auto lowerBound = m_normalBounds[i].first;
      auto upperBound = m_normalBounds[i].second;
      double lowerVal = kValNotSet;
      double upperVal = kValNotSet;
      auto obs = obsVec[i];
      lowerVal = obs->axis()->GetBinLowEdge(lowerBound);
      upperVal = obs->axis()->GetBinUpEdge(upperBound);
      m_normalVals.push_back(std::make_pair(lowerVal, upperVal));
    }
  }
  return m_normalVals;
}

const SplitVals& TQGridScanPoint::splitVals(const vector<TQGridScanSplitObservable*>& obsVec) {
  if (m_splitVals.empty()) {
    for (size_t i = 0; i < m_splitBounds.size(); ++i) {
      auto bound = m_splitBounds[i];
      auto obs = obsVec[i];
      auto binEdge = obs->axis()->GetBinLowEdge(bound);
      m_splitVals.push_back(binEdge);
    }
  }
  return m_splitVals;
}

#if 0
void TQGridScanPoint::clear() {
  // delete all information on this point
  variables.clear();
  this->significance = 0;
  this->id = 0;
  coordinates.clear();
  switchStatus.clear();
}

TQGridScanPoint::~TQGridScanPoint() {
  // standard destructor
  this->clear();
}
#endif
