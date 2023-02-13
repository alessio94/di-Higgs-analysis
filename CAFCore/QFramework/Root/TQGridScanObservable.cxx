#include "QFramework/TQGridScanObservable.h"

using Bound = TQGridScanBound;
using Bounds = TQGridScanBounds;
using BoundsType = TQGridScanBound::Type;
using BoundRange = TQGridScanBound::Range;
using BoundRangeUser = TQGridScanObservable::RangeUser;
using BinExtrema = TQGridScanBound::BinExtrema;

//using std::cout;
//using std::endl;
using std::vector;
using std::pair;
using std::tie;
using std::move;

ClassImp(TQGridScanBound)
ClassImp(TQGridScanBounds)
ClassImp(TQGridScanObservable)

namespace {

// These functions transform user-space coords to bin numbers for some histgram axis
// Bound range in user coords is _exclusive_ on the upper end, so subtract 1 from the bin
// for upper bounds
int coordToBin(TAxis* axis, double coord) {
  return axis->FindFixBin(coord);
}

int coordToBinLower(TAxis* axis, double coord) {
  return coordToBin(axis, coord);
}

int coordToBinUpper(TAxis* axis, double coord) {
  return coordToBin(axis, coord) - 1;
}

int coordToBinSplit(TAxis* axis, double coord) {
  return coordToBin(axis, coord);
}

BoundRange coordsToBins(
    TAxis* axis,
    BoundRangeUser boundRangeUser,
    const std::function<int (TAxis*, double)>& coordToBinFunc
) {
  double lowerCoord, upperCoord;
  int nSteps;
  // TODO C++17: destructuring
  tie(lowerCoord, upperCoord, nSteps) = boundRangeUser;
  auto lowerBin = coordToBinFunc(axis, lowerCoord);
  auto upperBin = coordToBinFunc(axis, upperCoord);
  int stepSize = 1; // in bins
  if (nSteps > 0) {
    stepSize = (upperBin - lowerBin) / nSteps;
    if (stepSize < 1) stepSize = 1;
  }
  return {lowerBin, upperBin, stepSize};
}

BoundRange coordsToBinsLower(TAxis* axis, BoundRangeUser boundRangeUser) {
  return coordsToBins(axis, boundRangeUser, coordToBinLower);
}

BoundRange coordsToBinsUpper(TAxis* axis, BoundRangeUser boundRangeUser) {
  return coordsToBins(axis, boundRangeUser, coordToBinUpper);
}

BoundRange coordsToBinsSplit(TAxis* axis, BoundRangeUser boundRangeUser) {
  return coordsToBins(axis, boundRangeUser, coordToBinSplit);
}

}

Bounds* TQGridScanObservable::normalBounds() {
  if (not m_bounds or m_bounds->which() == BoundsType::split) {
    // If any the lower or upper  bounds are not set, they will just be single cuts below or above
    // the under- or overflow bins, respectively
    m_bounds = Bounds{};
  }
  return &boost::get<Bounds>(*m_bounds);
}

Bound* TQGridScanObservable::splitBounds() {
  if (not m_bounds or m_bounds->which() == BoundsType::normal) {
    // This is just a dummy split point
    m_bounds = Bound{1, 1, 1};
  }
  return &boost::get<Bound>(*m_bounds);
}

void TQGridScanObservable::setAxesRange(int first, int last) {
  // Set axes ranges for all regions!
  for (unsigned int i=0; i<m_signalAxes.size(); i++) {
    m_signalAxes[i]->SetRange(first, last);
    m_bkgAxes[i]->SetRange(first, last);
  }
}

void TQGridScanObservable::setRangeCutLower(BoundRangeUser boundRangeUser) {
  auto binRange = coordsToBinsLower(signalAxis(), boundRangeUser);
  normalBounds()->lower()->range(move(binRange));
}
void TQGridScanObservable::setRangeCutUpper(BoundRangeUser boundRangeUser) {
  auto binRange = coordsToBinsUpper(signalAxis(), boundRangeUser);
  normalBounds()->upper()->range(move(binRange));
}
void TQGridScanObservable::setFixedCutLower(double cutVal) {
  int lowerBin = coordToBinLower(signalAxis(), cutVal);
  normalBounds()->lower()->range(lowerBin);
}
void TQGridScanObservable::setFixedCutUpper(double cutVal) {
  int upperBin = coordToBinUpper(signalAxis(), cutVal);
  normalBounds()->upper()->range(upperBin);
}
void TQGridScanObservable::setSwitchCutLower(double cutVal) {
  int lowerBin = coordToBinLower(signalAxis(), cutVal);
  normalBounds()->lower()->range(0, lowerBin);
}
void TQGridScanObservable::setSwitchCutUpper(double cutVal) {
  int upperBin = coordToBinUpper(signalAxis(), cutVal);
  normalBounds()->upper()->range(upperBin, signalAxis()->GetNbins()+1);
}
void TQGridScanObservable::setRangeCutSplit(BoundRangeUser boundRangeUser) {
  auto binRange = coordsToBinsSplit(signalAxis(), boundRangeUser);
  splitBounds()->range(move(binRange));
}
void TQGridScanObservable::setFixedCutSplit(double cutVal) {
  int bin = coordToBinSplit(signalAxis(), cutVal);
  splitBounds()->range(bin);
}
void TQGridScanObservable::setRangeCutLower(double lower, double upper, int nSteps) {
  setRangeCutLower(BoundRangeUser{lower, upper, nSteps});
}
void TQGridScanObservable::setRangeCutUpper(double lower, double upper, int nSteps) {
  setRangeCutUpper(BoundRangeUser{lower, upper, nSteps});
}
void TQGridScanObservable::setRangeCutSplit(double lower, double upper, int nSteps) {
  setRangeCutSplit(BoundRangeUser{lower, upper, nSteps});
}
