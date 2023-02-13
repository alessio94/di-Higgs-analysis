#include "TMVA/Timer.h"
#include "QFramework/TQGridScanner.h"
#include "QFramework/TQGridScanPoint.h"
#include "QFramework/TQGridScanObservable.h"
#include "QFramework/TQGridScanStyle.h"
#include "TLine.h"
#include <algorithm>
#include <iostream>
#include "TCanvas.h"
#include "QFramework/TQSignificanceEvaluator.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQTHnBaseUtils.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQFolder.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQPlotter.h"
#include "QFramework/TQPathManager.h"
#include "TLatex.h"
#include "TList.h"
#include "TPaletteAxis.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TROOT.h"
#include "THnBase.h"
#include "QFramework/TQLibrary.h"
#include <math.h>
#include <limits>

// #define _DEBUG_

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQGridScanner
//
// The TQGridScanner is the last element in the chain of classes
// that can be used for a full scan cut optimization.
//
// the workflow here could be as follows
// - create a multidimensional histogram during the analyze with the desired variables
//   for optimization
// - run the runGridScanner.py script for creating a TQGridScanner
//   and supplying it with a TQSignificanceEvaluator
//   that fits your needs regarding precision/runtime requirements
// - sort the results using the TQGridScanner::sortPoints method
// - retrieve the first (best) entry with TQGridScanner::printPoint(0)
//
// Plotting options:
// - ext=pdf,ps,...
// file extension. Works for all "plotAndSave..."-functions
// accepts all extension that are accepted by TCanvas->SaveAs(...)
// - showmax=true,false
// show the maximum. works for plotAndSave[All]SignficanceProfile[s]
// will additionally show the absolute maximum for each bin
// this option is ignored if topNumber is 1
// - cut.$VARNAME=X. Works if set on the GridScanner itself.
// Will draw a vertical line at X. Can be used to mark the baseline cut value.
//
////////////////////////////////////////////////////////////////////////////////////////////////

#define PRECISION 0.000000001 // for double/float comparison

using Bound = TQGridScanBound;
using Bounds = TQGridScanBounds;
//using FixedBoundType = TQGridScanObservable::FixedBoundType;
using BoundsType = TQGridScanBound::Type;
using BoundRange = TQGridScanBound::Range;
//using RangeBoundBin = TQGridScanObservable::RangeBound;
using BinExtrema = TQGridScanBound::BinExtrema;
using ObsVec = TQGridScanner::ObsVec;
using CutBounds = TQGridScanner::CutBounds;
using SplitCutVals = TQGridScanner::SplitCutVals;
using SplitCutBins = TQGridScanner::SplitCutBins;
using BoundDirection = TQGridScanner::BoundDirection;

using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::tie;
using std::unique_ptr;
//using FixedBoundUser = TQGridScanner::FixedBoundUser;
//using RangeBoundUser = TQGridScanner::RangeBoundUser;

ClassImp(TQGridScanner)

TQGridScanner::TQGridScanner(const TString& resultsName, TQSignificanceEvaluator* evaluator):
  TNamed("TQGridScanner", "TQGridScanner"),
  m_evaluator(evaluator),
  m_results(resultsName),
  m_runTimer("Scanning observables"),
  m_nDimHistName(resultsName)
{
  if (not evaluator) throw std::invalid_argument("Missing significance evaluator in TQGridScanner constructor");
  init();
}

TQGridScanner::TQGridScanner(const TString& resultsName, TQSignificanceEvaluator* evaluator, TList* obsToScan):
  TNamed("TQGridScanner", "TQGridScanner"),
  m_evaluator(evaluator),
  m_results(resultsName),
  m_runTimer("Scanning observables"),
  m_nDimHistName(resultsName)
{
  if (not evaluator) throw std::invalid_argument("Missing significance evaluator in TQGridScanner constructor");
  for(int i=0; i<=obsToScan->LastIndex(); i++) {
    m_obsNamesToScan.push_back(obsToScan->At(i)->GetName());
  }
  init();
}

TQGridScanner::~TQGridScanner() {
  for (auto obs : m_observables) {
    delete obs.second;
    obs.second = NULL;
  }
}

void TQGridScanner::init() {
  // TQGridScanner has a convoluted relationship with TQSignificanceEvaluator
  if (not m_evaluator->initialize(this)) {
    throw std::runtime_error("Significance evaluator failed to initialize in TQGridScanner constructor");
  }

  // Initialize the observables
  std::vector<TAxis*> signalAxes;
  std::vector<TAxis*> bkgAxes;
  std::vector<TString> obs_names;
  for (int i=0; i<m_signalHists[0]->GetNdimensions(); ++i) {
    signalAxes.clear();
    bkgAxes.clear();
    obs_names.clear();
    for(size_t ireg=0; ireg<m_evaluator->regions().size(); ireg++) {
      signalAxes.push_back(m_signalHists[ireg]->GetAxis(i));
      bkgAxes.push_back(m_bkgHists[ireg]->GetAxis(i));
      obs_names.push_back(signalAxes.back()->GetName());
    }
    m_observables.emplace(obs_names.back(), new TQGridScanObservable(obs_names, signalAxes, bkgAxes));
  }
  INFOclass("Now initializing observables");
}

TQGridScanObservable* TQGridScanner::getObs(const TString& obsName) {
  if ( m_observables.find(obsName) == m_observables.end() ) {
    BREAKclass("Observable with name %s not found in input histogram! Please check your input configuration and try again.", obsName.Data());
    return nullptr;
  } else {
    return m_observables.at(obsName);
  }
}

bool TQGridScanner::addSignalHist(THnBase* hist) {
  m_signalHists.push_back(hist);
  if (m_signalHists.size() > 1) {
    return TQTHnBaseUtils::checkConsistency(hist, m_signalHists[0], true);
  }
  return true;
}

bool TQGridScanner::addBkgHist(THnBase* hist) {
  m_bkgHists.push_back(hist);
  if (m_bkgHists.size() > 1) {
    return TQTHnBaseUtils::checkConsistency(hist, m_bkgHists[0], true);
  }
  return true;
}

void TQGridScanner::addFOMDefinitions(const std::vector <TString>& definitions) {
  // Add names for FOMs that are being evaluated
  this->results()->FOMDefinitions = definitions;
}

#if 0
void TQGridScanner::addSplitBounds(TString obsName, int fixedVal) {
  auto obs = getObs(obsName);
  obs.splitBounds.range(fixedVal);
}

void TQGridScanner::addSplitBounds(TString obsName, int lower, int upper) {
  auto obs = getObs(obsName);
  obs.splitBounds.range(lower, upper, 1);
}

void TQGridScanner::addSplitBounds(TString obsName, int lower, int upper) {
  auto obs = getObs(obsName);
  obs.splitBounds.range(lower, upper, 1);
}

void TQGridScanner::addBound(const TString& name, BoundDirection direction, FixedBoundUser fixedBoundUser) {
  auto obs = m_observables.at(name);
  auto axis = obs->axis();
  FixedBoundType boundType;
  double coord;
  tie(boundType, coord) = fixedBoundUser;
  int bin = coordinateToBin(coord);
  auto bound = getBound(obs);
  bound->fixed(std::make_pair(boundType, bin));
}

void TQGridScanner::addSplitBound(const TString& name, FixedBoundUser fixedBoundUser) {
  auto obs = m_observables.at(name);
  auto axis = obs->axis();
  FixedBoundType boundType;
  double coord;
  tie(boundType, coord) = fixedBoundUser;
  int bin = coordinateToBin(coord);
  auto bound = getSplitBound(obs);
  bound->fixed(std::make_pair(boundType, bin));
}

void TQGridScanner::addBound(const TString& name, BoundDirection direction, RangeBoundUser coords) {
  auto obs = m_observables.at(name);
  auto axis = obs->axis();
  auto bins = coordinatesToBins(axis, coords);
  auto bound = getBound(obs);
  bound->range(bins);
}

void TQGridScanner::addSplitBound(const TString& name, RangeBoundUser coords) {
  auto obs = m_observables.at(name);
  auto axis = obs->axis();
  auto bins = coordinatesToBins(axis, coords);
  auto bound = getSplitBound(obs);
  bound->range(bins);
}

void TQGridScanner::addVariableLowerFixed(const TString& varname, double value) {
  addBound(varname, BoundDirection::lower, value);
}

void TQGridScanner::addVariableUpperFixed(const TString& varname, double value) {
  addBound(varname, BoundDirection::upper, value);
}

void TQGridScanner::addVariableUpper(const TString& varname) {
  return;
}
void TQGridScanner::addVariableLower(const TString& varname) {
  return;
}
void TQGridScanner::addVariableSplit(const TString& varname) {
  return;
}
void TQGridScanner::addVariableUpperSwitch(const TString& varname) {
  return;
}
void TQGridScanner::addVariableLowerSwitch(const TString& varname) {
  return;
}

void TQGridScanner::addVariableWindowFixed(const TString& varname, double low, double up) {
  addBound(varname, BoundDirection::lower, low);
  addBound(varname, BoundDirection::upper, up);
}
#endif

void TQGridScanner::extractInputHistogramProjections() {
  DEBUGclass("This function will save the input distributions in the TQGridScanResults for the first defined region only!");
  // TODO: Add support for adding input hists for the multiple regions!

  gROOT->SetBatch(true);
  int ireg = 0;
  for (auto reg : m_evaluator->regions()) {
    TQGridScanResults::InputHists newInputHists;
    newInputHists.regionName = reg;
    m_results.inputHistsSig()->push_back(newInputHists);
    m_results.inputHistsBkg()->push_back(newInputHists);
    for (int i=0; i<m_signalHists[0]->GetNdimensions(); i++) {
      auto hist_sig = unique_ptr<TH1F>();
      auto hist_bkg = unique_ptr<TH1F>();
      hist_sig = unique_ptr<TH1F>(reinterpret_cast<TH1F*>(m_signalHists[ireg]->Projection(i)));
      hist_sig->SetName(m_signalHists[ireg]->GetAxis(i)->GetName());
      m_results.inputHistsSig()->back().hists.push_back(*hist_sig.get());
      hist_sig->SetName(TString::Format("%s_%d", hist_sig->GetName(), i)); // to get rid of memory leak warnings
      hist_bkg = unique_ptr<TH1F>(reinterpret_cast<TH1F*>(m_bkgHists[ireg]->Projection(i)));
      hist_bkg->SetName(m_bkgHists[ireg]->GetAxis(i)->GetName());
      m_results.inputHistsBkg()->back().hists.push_back(*hist_bkg.get());
      hist_bkg->SetName(TString::Format("%s_%d", hist_bkg->GetName(), i)); // to get rid of memory leak warnings
    }
    ireg++;
  }
  gROOT->SetBatch(false);

}

void TQGridScanner::dumpInputHistogramProjections(TQTaggable& tags) {
  // This function dumps the input distributions of the first defined region
  // This function is only useful for debugging when the scan is not working
  // For proper plotting of the input distributions TQGridScanResults::plotInputDistributions is used!

  DEBUGclass("This function will plot the input distributions for the first defined region only!");
  gROOT->SetBatch(true);
  TStyle *style = TQHistogramUtils::ATLASstyle();
  style->SetName("ATLAS");
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();

  for (int i=0; i<m_signalHists[0]->GetNdimensions(); i++) {
    auto hist_sig = unique_ptr<TH1F>();
    auto hist_bkg = unique_ptr<TH1F>();
    auto c = TQGridScanStyle::defaultCanvas();
    c->cd();

    hist_sig = unique_ptr<TH1F>(reinterpret_cast<TH1F*>(m_signalHists[0]->Projection(i)));
    hist_bkg = unique_ptr<TH1F>(reinterpret_cast<TH1F*>(m_bkgHists[0]->Projection(i)));

    TQGridScanStyle::setStyle(c.get(), hist_sig.get(), tags);
    TQGridScanStyle::setStyleInputHists(hist_sig.get(), hist_bkg.get(), tags);

    // Add generic legend
    unique_ptr<TLegend> leg (new TLegend(0.65, 0.75, 0.9, 0.9));
    leg->AddEntry(hist_sig.get(), "Signal", "F");
    leg->AddEntry(hist_bkg.get(), "Total Bkg", "F");

    // Draw not normalized histogram
    hist_bkg->GetYaxis()->SetTitle("Events");
    hist_bkg->Draw("hist");
    hist_sig->Draw("histsame");
    leg->Draw("same");

    // Save
    TString baseFilePath = tags.getTagStringDefault("plotDirectory", "plots/");
    baseFilePath = TQFolder::concatPaths(baseFilePath, "input-observables");
    TString obsName = TQFolder::makeValidIdentifier(m_signalHists[0]->GetAxis(i)->GetName());
    TString extension = tags.getTagStringDefault("plotFormat","pdf");
    TQUtils::ensureDirectory(TQPathManager::getPathManager()->getTargetPath(baseFilePath));
    TString outputname =  TQFolder::concatPaths(baseFilePath, "inputDistribution_"+obsName);
    c->SaveAs( TQPathManager::getPathManager()->getTargetPath(outputname+"."+extension).c_str() );

    // Save normalized histogram
    c->Clear();
    hist_sig->Scale(1./hist_sig->GetSumOfWeights());
    hist_bkg->Scale(1./hist_bkg->GetSumOfWeights());
    double ymax1 = TQHistogramUtils::getHistogramMaximum(2, hist_sig.get(), hist_bkg.get());
    hist_sig->GetYaxis()->SetRangeUser(0, ymax1*3/2.);
    hist_sig->GetYaxis()->SetTitle("Normalized Events");
    hist_sig->Draw("hist");
    hist_bkg->Draw("histsame");
    leg->Draw("same");
    c->SaveAs( TQPathManager::getPathManager()->getTargetPath(outputname+"_norm."+extension).c_str() );

  }
  gROOT->SetBatch(false);
}

void TQGridScanner::run() {
  for (auto& obsPair: m_observables) {
    auto& obs = obsPair.second;
    const auto& bounds = *obs->bounds();
    // If no bounds have been set, skip this obs
    if (bounds) {
      const auto boundsType = bounds->which();
      if (boundsType == BoundsType::normal) {
        const auto& normalBounds = *obs->normalBounds();
        const auto& lowerBounds = normalBounds.lower();
        const auto& upperBounds = normalBounds.upper();
        // If fixed cuts are applied on both sides, there's nothing to scan
        if (lowerBounds.isFixed() and upperBounds.isFixed()) {
          obs->setAxesRange(lowerBounds.range()[0], upperBounds.range()[0]);
        } else {
          m_nPoints *= lowerBounds.nPoints();
          m_nPoints *= upperBounds.nPoints();
          m_normalObs.push_back(obs);
          // this is just a dummy value
          m_normalBounds.push_back({0, 0});
          m_results.normalObs()->push_back(new TQGridScanNormalObservable(obs->signalAxes(),normalBounds));
        }
      } else {
        const auto splitBounds = *obs->splitBounds();
        if (splitBounds.isFixed()) {
          m_splitObs.push_back(obs);
          m_splitBins.push_back(-1);
        } else {
          m_nPoints *= splitBounds.nPoints();
          m_splitScanObs.push_back(obs);
          m_splitScanBins.push_back(-1);
          m_results.splitObs()->push_back(new TQGridScanSplitObservable(obs->signalAxes(),splitBounds));
        }
      }
    }
  }
  m_runTimer.Init(m_nPoints);
  m_heartbeat = TQUtils::getCurrentTime();
  INFOclass("Scanning %d points", m_nPoints);
  m_nPointsProcessed = 0;
  scan(m_normalBounds.begin(), m_normalObs.begin());
}

void TQGridScanner::scan(CutBounds::iterator obsValsIter, ObsVec::iterator obsIter) {
  auto obs = *obsIter;
  const auto& bounds = *obs->normalBounds();
  const auto lowerBounds = bounds.lower();
  const auto lowerBoundsRange = lowerBounds.range();
  const auto upperBounds = bounds.upper();
  const auto upperBoundsRange = upperBounds.range();
  for (auto lowerBin = lowerBoundsRange[0]; lowerBin <= lowerBoundsRange[1]; lowerBin += lowerBoundsRange[2]) {
    for (auto upperBin = upperBoundsRange[0]; upperBin <= upperBoundsRange[1]; upperBin += upperBoundsRange[2]) {
      // Only bins from the set ranges are kept when the ndim hist is projected onto any number of
      //   axes. There's an option to ignore the set ranges, but we don't want that.
      // See SetRange documentation for why we can't pass it (0,0)
      // TODO: in setAxesRange function we should not only set all axes range for the different regions but
      // also the ones for the different systematic variations, if available!
      if (upperBin == 0) {
        obs->setAxesRange(-1, 0);
      } else {
        obs->setAxesRange(lowerBin, upperBin);
      }

      obsValsIter->first = lowerBin;
      obsValsIter->second = upperBin;

      updateHeartbeat();

      auto isFinalLevel = (obsIter + 1 == m_normalObs.end());
      if (isFinalLevel) {
        if (m_splitScanObs.empty()) {printProgress();}
        if (not m_splitScanObs.empty()) { // split scan observable used
          scanSplit(m_splitScanBins.begin(), m_splitScanObs.begin());
        } else if (not m_splitObs.empty()) { // fixed split observable used
          auto signif2 = splitSignif2(m_splitBins.begin(), m_splitObs.begin(), false);
          auto signif = sqrt(signif2);
          m_evaluator->info += TString::Format("FOM=%f", signif);
          m_results.points()->emplace_back(m_normalBounds, signif, m_evaluator->info);
        } else { // only upper and lower observable scans used
          if (m_evaluator->m_multipleFOMsCompatible) {
            auto foms = m_evaluator->evaluateMultiple();
            m_results.points()->emplace_back(m_normalBounds, foms, m_evaluator->info);
          } else { // old evaluation functions, in principal not needed anymore
            auto signif = m_evaluator->evaluate();
            m_results.points()->emplace_back(m_normalBounds, signif, m_evaluator->info);
          }
        }
      } else {
        scan(obsValsIter + 1, obsIter + 1);
      }
    }
  }
}

double TQGridScanner::splitSignif2(SplitCutBins::iterator obsBinsIter, ObsVec::iterator obsIter, bool isScanSplit) {
  double signifQuadrature = 0;
  auto obs = *obsIter;
  //auto axis = obs->axis();
  const auto bin = *obsBinsIter;
  auto isFinalLevel = (obsIter + 1 == (isScanSplit ? m_splitScanObs.end() : m_splitObs.end()));

  // sums in quadrature the significance both below and above the split point
  // below
  obs->setAxesRange(BinExtrema::min, bin - 1);
  signifQuadrature += splitSignifHandleRecursion(isFinalLevel, isScanSplit, obsBinsIter, obsIter);

  // above
  obs->setAxesRange(bin, BinExtrema::max);
  signifQuadrature += splitSignifHandleRecursion(isFinalLevel, isScanSplit, obsBinsIter, obsIter);

  return signifQuadrature;
}

double TQGridScanner::splitSignifHandleRecursion(
    bool isFinalLevel,
    bool isScanSplit,
    SplitCutBins::iterator obsBinsIter,
    ObsVec::iterator obsIter
) {
  double signifQuadrature = 0;
  if (isFinalLevel) {
    // if we are currently iterating over scan split observables, iterate over non-scan split observables
    // if there are any
    if (isScanSplit and not m_splitObs.empty()) {
      auto signif2 = splitSignif2(m_splitBins.begin(), m_splitObs.begin(), false);
      signifQuadrature += signif2;
    } else {
      double signif = 0;
      // not actually compatible for multiple FOMs with a split cut! Only first specified FOM is used
      if (m_evaluator->m_multipleFOMsCompatible) {
        auto foms = m_evaluator->evaluateMultiple();
        signif = foms[0];
      } else {
        signif = m_evaluator->evaluate();
      }
      signifQuadrature += signif*signif;
    }
  } else {
    auto signif2 = splitSignif2(obsBinsIter + 1, obsIter + 1, isScanSplit);
    signifQuadrature += signif2;
  }
  return signifQuadrature;
}

// Iterate over split points for scan split observables. For each set of points, evaluate the significance
void TQGridScanner::scanSplit(SplitCutBins::iterator obsBinsIter, ObsVec::iterator obsIter) {
  auto obs = *obsIter;
  const auto bound = *(obs->splitBounds());
  const auto boundRange = bound.range();
  for (auto bin = boundRange[0]; bin <= boundRange[1]; bin += boundRange[2]) {
    *obsBinsIter = bin;
    updateHeartbeat();
    //m_runTimer.DrawProgressBar(++m_nPointsProcessed);
    auto isFinalLevel = ((obsIter + 1) == m_splitScanObs.end());
    if (isFinalLevel) {
      printProgress();
      auto signif2 = splitSignif2(m_splitScanBins.begin(), m_splitScanObs.begin(), true);
      auto signif = sqrt(signif2);
      m_evaluator->info += TString::Format("FOM=%f", signif);
      std::vector<double> foms = {signif}; // fill in vector here to have simpler code later on
      m_results.points()->emplace_back(m_normalBounds, m_splitScanBins, foms, m_evaluator->info);
    } else {
      scanSplit(obsBinsIter + 1, obsIter + 1);
    }
  }
}

bool TQGridScanner::updateHeartbeat() {
  // this routine will submit the heartbeat if neccessary
  if(m_heartBeatInterval == 0)
    return false;
  if(m_heartBeatCommand.IsNull())
    return false;
  unsigned long t = TQUtils::getCurrentTime();
  if(m_heartbeat + m_heartBeatInterval > t)
    return false;
  m_heartbeat = t;
  gSystem->Exec(m_heartBeatCommand.Data());
  return true;
}

void TQGridScanner::printProgress() {
  ++m_nPointsProcessed;
  m_runTimer.DrawProgressBar(m_nPointsProcessed);
  if ((m_nPoints < 10) || (m_nPointsProcessed % (m_nPoints / 10) == 0)) {
    INFOclass("%d percent points processed!", (int)((float)(m_nPointsProcessed)/m_nPoints*100+1));
  }
}
