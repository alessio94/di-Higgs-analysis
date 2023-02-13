#ifndef __TQ_GRIDSCANOBSERVABLE__
#define __TQ_GRIDSCANOBSERVABLE__

// TODO: remove this once full C++11 is used
#include <climits>
#include <iostream>

#include "boost/optional.hpp"
#include "boost/variant.hpp"
#include "boost/variant/get.hpp"

#include "QFramework/TQGridScanBound.h"

#include "TAxis.h"
#include "TH1.h"

// Warning: namespaces don't play well with ROOT; use ugly verbose names instead

// These hold the relevant information about observables when initializing the grid scan
// Not meant to be serialised
class TQGridScanObservable : public TObject {
public:
  // These can be used to ergonomically query variant types
  // "split" is a special type of bound where the cut is applied in both directions,
  //   then the significances in both regions are added in quadrature
  // "normal" is every other type of bound
  using RangeUser = std::tuple<double, double, int>;
  using Range = TQGridScanBound::Range;

  TQGridScanObservable() = default;
  // Observables hold vector of axes for cases where more than one region specified to optimize!
  // This is needed to execute TQGridScanObservable::setAxesRange in all regions!
  // Since the feature space must be consistent within the regions only the first entry
  // for the axes is regarded for most of the range and bound settings
  TQGridScanObservable(std::vector<TString> names, std::vector<TAxis*> signalAxes, std::vector<TAxis*> bkgAxes) :
    m_names(names),
    m_signalAxes(signalAxes),
    m_bkgAxes(bkgAxes)
  {}

  const TString& name() const { return m_names[0]; }
  TAxis* signalAxis() { return m_signalAxes[0]; }
  TAxis* bkgAxis() { return m_bkgAxes[0]; }
  std::vector<TString> names() const { return m_names; }
  std::vector<TAxis*> signalAxes() { return m_signalAxes; }
  std::vector<TAxis*> bkgAxes() { return m_bkgAxes; }

  // Set the ranges for both signal and bkg hist
  void setAxesRange(int first, int last);

  boost::optional<boost::variant<TQGridScanBounds, TQGridScanBound>>* bounds() {
    return &m_bounds;
  }
  const boost::optional<boost::variant<TQGridScanBounds, TQGridScanBound>>& bounds() const {
    return m_bounds;
  }

  TQGridScanBounds* normalBounds();
  TQGridScanBound* splitBounds();

  void setRangeCutLower(RangeUser boundRangeUser);
  void setRangeCutUpper(RangeUser boundRangeUser);
  void setFixedCutLower(double cutVal);
  void setFixedCutUpper(double cutVal);
  void setSwitchCutLower(double cutVal);
  void setSwitchCutUpper(double cutVal);
  void setRangeCutSplit(RangeUser boundRangeUser);
  void setFixedCutSplit(double cutVal);

  void setRangeCutLower(double lower, double upper, int nSteps);
  void setRangeCutUpper(double lower, double upper, int nSteps);
  void setRangeCutSplit(double lower, double upper, int nSteps);

private:
  std::vector<TString> m_names;
  std::vector<TAxis*> m_signalAxes;
  std::vector<TAxis*> m_bkgAxes;
  boost::optional<boost::variant<TQGridScanBounds, TQGridScanBound>> m_bounds;

public:
  ClassDefOverride(TQGridScanObservable, 1)
};

class TQHistParams {
public:
  TQHistParams() = default;
  TQHistParams(std::vector<TAxis*> axes) 
  {
    for (unsigned int i=0; i < axes.size(); i++) {
      if (i==0) {
        m_hists.push_back( TH1F(
                                axes[i]->GetName(),
                                axes[i]->GetTitle(),
                                axes[i]->GetNbins(),
                                axes[i]->GetXmin(),
                                axes[i]->GetXmax()
                                ) );
      } else { // to avoid memory leak (separated from above to keep GetName() in first axes entry)
        m_hists.push_back( TH1F(
                                axes[i]->GetName()+TString::Format("%d", i),
                                axes[i]->GetTitle(),
                                axes[i]->GetNbins(),
                                axes[i]->GetXmin(),
                                axes[i]->GetXmax()
                                ) );
      }
    }
  }
  TString name() const { return m_hists[0].GetName(); }
  TString title() const { return m_hists[0].GetTitle(); }
  int nBins() const { return m_hists[0].GetXaxis()->GetNbins(); }
  double min() const { return m_hists[0].GetXaxis()->GetXmin(); }
  double max() const { return m_hists[0].GetXaxis()->GetXmax(); }

  const TH1F& hist() const { return m_hists[0]; }
  std::vector<TH1F> hists() const { return m_hists; }
  const TAxis* axis() const { return m_hists[0].GetXaxis(); }

protected:
  std::vector<TH1F> m_hists;
};

// These hold the relevant information to plot grid scan results, used after reading grid scan results
// They can be serialised with ROOT
// It stores separate properties for the view range vs. the actual binning; as far as I know there's no
// ergonomic way to apply cuts to the ndim histograms at the outset, so we never bother to remove
// bins that will never be used.
class TQGridScanNormalObservable : public TObject, public TQHistParams {
public:

  TQGridScanNormalObservable(
      std::vector<TAxis*>  axes,
      TQGridScanBounds bounds
  ) :
    TQHistParams(axes),
    m_bounds(bounds)
  {}

  TQGridScanNormalObservable() = default;

  TQGridScanBounds* bounds() {
    return &m_bounds;
  }

  const TQGridScanBounds& bounds() const {
    return m_bounds;
  }

private:
  TQGridScanBounds m_bounds;

public:
  ClassDefOverride(TQGridScanNormalObservable, 1)
};

class TQGridScanSplitObservable : public TObject, public TQHistParams {
public:
  TQGridScanSplitObservable(
      std::vector<TAxis*>  axes,
      TQGridScanBound bound
  ) :
    TQHistParams(axes),
    m_bound(bound)
  {}

  TQGridScanSplitObservable() = default;

  TQGridScanBound* bound() {
    return &m_bound;
  }

  const TQGridScanBound& bound() const {
    return m_bound;
  }

private:
  TQGridScanBound m_bound;

public:
  ClassDefOverride(TQGridScanSplitObservable, 1)
};
#endif
