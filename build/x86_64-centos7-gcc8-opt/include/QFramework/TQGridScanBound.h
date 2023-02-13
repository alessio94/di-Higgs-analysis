#ifndef __TQ_GRIDSCANBOUND__
#define __TQ_GRIDSCANBOUND__

// TODO: remove this once full C++11 is used
#include <climits>
#include <iostream>
#include <vector>

#include <TObject.h>

// This class holds a single bound range, which means the range of values scanned over as an upper
// or lower or split cut. It can be a fixed value, a switch value, or a range with arbitrary step
// size.
class TQGridScanBound : public TObject {
public:
  // lower bin, upper bin, step size
  //using Range = std::array<int, 3>;
  // std::array doesn't have a streamer definition, apparently...
  using Range = std::vector<int>;
  enum class Direction { lower, upper, split };
  enum Type { normal = 0, split = 1 };

  // TODO: replace with numeric_limits once full C++11 is used
  //enum BinExtrema { min = 0, max = std::numeric_limits<int>::max };
  // Note: 0th bin includes underflow!
  enum BinExtrema { min = 0, max = INT_MAX/2 };

  TQGridScanBound() = default;
  TQGridScanBound(int fixedBin) :
  m_bins({fixedBin, fixedBin, 1})
  {}
  TQGridScanBound(int switchCutBin, int extremumBin) {
    range(switchCutBin, extremumBin);
  }
  TQGridScanBound(Range boundRange) :
    m_bins(boundRange)
  {}
  TQGridScanBound(int lowerBin, int upperBin, int stepSize) :
    m_bins(Range{lowerBin, upperBin, stepSize})
  {}

  void range(int fixedBin) {
    m_bins = (Range{fixedBin, fixedBin, 1});
  }
  int nPoints() const {
    return 1 + (m_bins[1] - m_bins[0]) / m_bins[2];
  }
  // Switch cut works by setting two scan points: one at the switch cut
  // val and the other at (including) the over- or underflow bin.
  void range(int switchCutBin, int extremumBin) {
    if (extremumBin > switchCutBin) {
      m_bins = (Range{switchCutBin, extremumBin, extremumBin - switchCutBin});
    } else {
      m_bins = (Range{0, switchCutBin, switchCutBin});
    }
  }
  void range(Range boundRange) {
    m_bins = boundRange;
  }

  bool isFixed() const {
    return m_bins[0] == m_bins[1];
  }

  Range* range() { return &m_bins; }
  Range range() const { return m_bins; }

  ClassDefOverride(TQGridScanBound, 1)
private:
  Range m_bins;
};

class TQGridScanBounds : public TObject {
public:
  using BinExtrema = TQGridScanBound::BinExtrema;
  TQGridScanBounds() :
      TQGridScanBounds(
        TQGridScanBound{BinExtrema::min, BinExtrema::min, 1},
        TQGridScanBound{BinExtrema::max, BinExtrema::max, 1}
      )
  {}
  TQGridScanBounds(TQGridScanBound lower, TQGridScanBound upper) : m_lower(lower), m_upper(upper) {}

  TQGridScanBound* lower() { return &m_lower; }
  TQGridScanBound lower() const { return m_lower; }
  TQGridScanBound* upper() { return &m_upper; }
  TQGridScanBound upper() const { return m_upper; }

private:
  TQGridScanBound m_lower;
  TQGridScanBound m_upper;

  ClassDefOverride(TQGridScanBounds, 1)
};

#endif
