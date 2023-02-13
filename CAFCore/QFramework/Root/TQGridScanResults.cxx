#include <memory>

#include "TCanvas.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TROOT.h"
#include "TString.h"
#include "TGraphErrors.h"
#include "TStyle.h"
//#include "TSystem.h"

#include "QFramework/TQGridScanner.h"
#include "QFramework/TQGridScanResults.h"
#include "QFramework/TQGridScanObservable.h"
#include "QFramework/TQGridScanStyle.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQROOTPlotter.h"
#include "QFramework/TQTaggable.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQLibrary.h"
#include "QFramework/TQPathManager.h"

using std::unique_ptr;

using BoundDirection = TQGridScanBound::Direction;

double kValNotSet = TQGridScanPoint::kValNotSet;

ClassImp(TQGridScanResults)

void TQGridScanResults::sortPoints() {
  if(not m_sorted) {
    std::function<double (const TQGridScanPoint&, const TQGridScanPoint&)> sortFunction;
    if (points()->at(0).foms().empty()) {
      sortFunction = [](const TQGridScanPoint& a, const TQGridScanPoint& b) {
                       return a.significance() > b.significance();
                     };
    }
    else {
      sortFunction = [](const TQGridScanPoint& a, const TQGridScanPoint& b) {
                       return a.foms()[0] > b.foms()[0];
                     };
    }
    sort(
         m_points.begin(),
         m_points.end(),
         sortFunction
         );
  }
  m_sorted = true;
}

void TQGridScanResults::sortPoints(unsigned int fom_index_for_sorting) {
  // Sort points w.r.t. specific FOM (specified by index in fom vector)
  if (points()->at(0).foms().empty()) {
    DEBUGclass("There are not multiple figure of merits defined! Trying to sort w.r.t. the specified evaluator mode!");
    sortPoints();
    return;
  }
  if (fom_index_for_sorting >= points()->at(0).foms().size()) {
    WARNclass("Specified index '%i' is out of range in vector of figure of merits! Cannot sort points w.r.t. this, falling back to first specified figure of merit (with index 0)", fom_index_for_sorting);
    fom_index_for_sorting = 0;
  }
  auto sortFunction = [fom_index_for_sorting](const TQGridScanPoint& a, const TQGridScanPoint& b) {
                        return a.foms().at(fom_index_for_sorting) > b.foms().at(fom_index_for_sorting);
                      };
  sort(
       m_points.begin(),
       m_points.end(),
       sortFunction
       );
}

void TQGridScanResults::printPoints(size_t first, size_t last) {
  // print all points with indices ranging between first and last
  // will only print points that meet the requirements
  if(last >= m_points.size()) last = m_points.size() - 1;
  for(size_t i = first; i<last; i++) {
    auto& point = m_points[i];
    if(isAcceptedPoint(point)) printPoint(&point);
  }
}

void TQGridScanResults::printPoints(size_t last) {
  // print all points up to given index
  // will only print points that meet the requirements
  printPoints(0, last);
}

unique_ptr<TLine> TQGridScanResults::drawCutLine(TH1* hist) {
  // draw the vertical line denoting the current cut value
  TString vname = TString::Format("cut.%s",hist->GetName());
  if(!hasTag(vname)) vname.ReplaceAll(":","");
  // What's the point of this?
  //if(!hasTag(vname)) {
  //  size_t idx = findVariable(hist->GetName());
  //  if(idx > variables.size()) return nullptr;
  //  vname = TString::Format("cut.%s",variables[idx].Data());
  //}
  double cut = getTagDoubleDefault(vname,std::numeric_limits<double>::quiet_NaN());
  if(!TQUtils::isNum(cut))
    return nullptr;
  double xmax = TQHistogramUtils::getAxisXmax(hist);
  double xmin = TQHistogramUtils::getAxisXmin(hist);
  if(cut > xmax || cut < xmin) return nullptr;
  gPad->Modified();
  gPad->Update();
  double ymin = gPad->GetUymin();
  double ymax = gPad->GetUymax();
  auto cutline = unique_ptr<TLine>(new TLine(cut,ymin,cut,ymax));
  int cutlinecolor = getTagIntegerDefault("cutLine.color",kRed);
  int cutlinestyle = getTagIntegerDefault("cutLine.style",7);
  int cutlinewidth = getTagIntegerDefault("cutLine.width",2);
  cutline->SetLineStyle(cutlinestyle);
  cutline->SetLineWidth(cutlinewidth);
  cutline->SetLineColor(cutlinecolor);
  cutline->Draw();
  return cutline;
}

bool TQGridScanResults::isAcceptedPoint(const TQGridScanPoint& /*point*/) {
  // TODO: implement
  // VERBOSEclass("Accept the following grid scan point");
  // VERBOSEclass(point.info().Data());
  return true;
}

void TQGridScanResults::plotAndSaveAllSignificanceProfiles(double topFraction, const TString& options, TString outPath) {
  auto topNumber = static_cast<int>(ceil(topFraction * m_points.size()));
  plotAndSaveAllSignificanceProfiles(topNumber, options, outPath);
}

void TQGridScanResults::plotAndSaveAllSignificanceProfiles(int topNumber, const TString& options, TString outPath) {
  // obtain histogram-like plots
  // showing the significance as a function of each varname
  // optimized over all invisible dimesions to the average
  // of the best topNumber points
  // plot and save them under the given path with the given options

  gROOT->SetBatch(true);
  auto i = 0;
  for (const auto& obs: m_normalObs) {
    if (not obs->bounds()->lower()->isFixed()) {
      plotAndSaveSignificanceProfile(BoundDirection::lower, i, topNumber, options, outPath);
    }
    if (not obs->bounds()->upper()->isFixed()) {
      plotAndSaveSignificanceProfile(BoundDirection::upper, i, topNumber, options, outPath);
    }
    ++i;
  }
  for (unsigned int i=0; i < m_splitObs.size(); i++) {
    plotAndSaveSignificanceProfile(BoundDirection::split, i, topNumber, options, outPath);
  }
  gROOT->SetBatch(false);
}

void TQGridScanResults::plotAndSaveSignificanceProfile(BoundDirection direction, int i, int topNumber, const TString& options, TString outPath) {
  // obtain a histogram-like plot
  // showing the significance as a function of varname
  // optimized over all invisible dimesions to the average
  // of the best topNumber points
  // plot and save it under the given path with the given options
  TQTaggable tags(options);
  tags.importTags(this);
  bool showmax = tags.getTagBoolDefault("showmax",getTagBoolDefault("profile.showmax",true));
  TString extension = tags.getTagStringDefault("ext","pdf");
  auto hist = getSignificanceProfile(direction, i, topNumber);
  // This gets invalidated if hist gets reassigned, just FYI
  auto histptr = hist.get();
  if(!hist) return;
  auto c = TQGridScanStyle::defaultCanvas();
  c->cd();
  unique_ptr<TLine> cutline = nullptr;
  unique_ptr<TH1F> histmax = nullptr;
  unique_ptr<TGraphAsymmErrors> ga = nullptr;
  unique_ptr<TGraphAsymmErrors> ga_max = nullptr;
  hist->SetLineColor(tags.getTagIntegerDefault("style.profile.color",getTagIntegerDefault("color",kBlue+4)));
  TQHistogramUtils::edge(histptr, getTagDoubleDefault("profile.plotMinSignificance",0.));
  if(showmax && topNumber > 1) {
    histmax = getSignificanceProfile(direction, i, 1);
    TQGridScanStyle::setStyle(c.get(), histptr, tags);
    TQGridScanStyle::setStyleAndRangeSignifProfile(histptr, histmax.get(), tags);
    if (tags.getTagBoolDefault("profile.newArrowStyle", true)) {
      ga = makeGraphFromLowBinEdges(histptr, direction);
      ga_max = makeGraphFromLowBinEdges(histmax.get(), direction);
      TQGridScanStyle::setStyleSignifProfileGraphs(ga.get(), ga_max.get(), tags);
      ga_max->Draw("AP|>");
      ga->Draw("sameP|>");
      if (tags.getTagBoolDefault("profile.drawHistogram", false)) {
        histmax->Draw("histsame");
        ga_max->Draw("sameP|>");
        ga->Draw("sameP|>");
      }
    } else {
      hist->Draw("same");
      histmax->Draw("histsame");
    }
    cutline = drawCutLine(histmax.get());
  } else {
    TQGridScanStyle::setStyle(c.get(), histptr, tags);
    TQGridScanStyle::setStyleAndRangeSignifProfile(histptr, nullptr, tags);
    hist->Draw("");
    cutline = drawCutLine(histptr);
  }
  auto topFraction = static_cast<double>(topNumber) / m_points.size();
  if(topNumber > 1) {
    // Will anyone want to use top number rather than fraction?
    //drawLegend(histptr, histmax.get(), tags, TString::Format("top %d average",topNumber), cutline.get());
    TQGridScanStyle::drawLegend(histptr, histmax.get(), tags, TString::Format("top %0.1g%% average", topFraction*100), cutline.get(), ga.get(), ga_max.get());
  } else {
    // TODO: verify this gives correct behavior
    TQGridScanStyle::drawLegend(nullptr, histptr, tags, "", cutline.get(), ga.get(), ga_max.get());
  }
  gPad->Modified();
  gPad->Update();

  auto obsName = (direction == BoundDirection::split) ? m_splitObs[i]->name() : m_normalObs[i]->name();
  if (outPath.IsNull()) outPath = getTagStringDefault("plotDirectory", "plots/");
  TQUtils::ensureDirectoryForFile(TQPathManager::getPathManager()->getTargetPath(outPath));
  auto outFilename = TQFolder::concatPaths(outPath, "signifProfile_" + TQFolder::makeValidIdentifier(obsName));
  //auto suffix = TQFolder::makeValidIdentifier(TString::Format("_topn_%d",topNumber));
  auto suffix = TQFolder::makeValidIdentifier(TString::Format("_topf_%0.1g",topFraction));
  outFilename = outFilename + suffix + "." + extension;
  c->SaveAs(
            TQPathManager::getPathManager()->getTargetPath(outFilename).c_str(),
      extension
  );
}

unique_ptr<TH1F> TQGridScanResults::getSignificanceProfile(BoundDirection direction, int i, int topNumber) {
  // obtain a histogram-like plot
  // showing the significance as a function of varname
  // optimized over all invisible dimesions to the average
  // of the best topNumber points
  sortPoints();

  auto hist = unique_ptr<TH1F>();
  TString obsName;
  TQGridScanBound::Range range;

  if (direction == BoundDirection::lower) {
    const auto obs = m_normalObs[i];
    hist = unique_ptr<TH1F>(dynamic_cast<TH1F*>(obs->hist().Clone()));
    obsName = obs->name();
    const TQGridScanBound* bound = obs->bounds()->lower();
    range = bound->range();
  } else if (direction == BoundDirection::upper) {
    const auto obs = m_normalObs[i];
    obsName = obs->name();
    hist = unique_ptr<TH1F>(dynamic_cast<TH1F*>(obs->hist().Clone()));
    const TQGridScanBound* bound = obs->bounds()->upper();
    range = bound->range();
  } else {
    const auto obs = m_splitObs[i];
    obsName = obs->name();
    hist = unique_ptr<TH1F>(dynamic_cast<TH1F*>(obs->hist().Clone()));
    const TQGridScanBound* bound = obs->bound();
    range = bound->range();
  }
  int lowerBin = range[0];
  int upperBin = range[1];

  hist->SetStats(kFALSE);
  hist->SetDirectory(nullptr);

  auto histFills = unique_ptr<TH1F>(dynamic_cast<TH1F*>(hist->Clone()));
  auto histSquares = unique_ptr<TH1F>(dynamic_cast<TH1F*>(hist->Clone()));
  auto histPtr = hist.get();
  auto histSquaresPtr = histSquares.get();
  auto histFillsPtr = histFills.get();

  bool showExtraBins = getTagBoolDefault("profile.showExtraBins",false);
  bool showUnderflow = showExtraBins && direction == BoundDirection::lower;
  bool showOverflow = showExtraBins && direction == BoundDirection::upper;

  auto axis = hist->GetXaxis();

  switch (direction) {
    case BoundDirection::lower:
      for(const auto& point: m_points) {
        if(!isAcceptedPoint(point)) continue;
        auto nBin = point.normalBounds()[i].first;
        auto fom = point.foms().at(m_fom_index_for_plotting);
        //if (not nBin) nBin = BinExtrema::min;
        TQGridScanStyle::fillHists(histPtr, histSquaresPtr, histFillsPtr, nBin, fom, topNumber, showUnderflow, showOverflow);
      }
      axis->SetRange(lowerBin, upperBin);
      break;
    case BoundDirection::upper:
      for(const auto& point: m_points) {
        if(!isAcceptedPoint(point)) continue;
        // Add 1 to account for the inclusive->exclusive upper bound conversion
        auto nBin = point.normalBounds()[i].second + 1;
        auto fom = point.foms().at(m_fom_index_for_plotting);
        //if (not nBin) nBin = BinExtrema::max;
        TQGridScanStyle::fillHists(histPtr, histSquaresPtr, histFillsPtr, nBin, fom, topNumber, showUnderflow, showOverflow);
      }
      hist->GetXaxis()->SetRange(lowerBin + 1, upperBin + 1);
      break;
    case BoundDirection::split:
      for(const auto& point: m_points) {
        if(!isAcceptedPoint(point)) continue;
        auto nBin = point.splitBounds()[i];
        auto fom = point.foms().at(m_fom_index_for_plotting);
        TQGridScanStyle::fillHists(histPtr, histSquaresPtr, histFillsPtr, nBin, fom, topNumber, showUnderflow, showOverflow);
      }
      hist->GetXaxis()->SetRange(lowerBin, upperBin);
      break;
  }
  TQGridScanStyle::excludeOverflow(axis);
  hist->Divide(histFillsPtr);
  histSquares->Divide(histFillsPtr);
  TQGridScanStyle::fillErrors(histPtr, histSquaresPtr, histFillsPtr);
  hist->SetEntries(topNumber);
  hist->SetMinimum(getTagDoubleDefault("profile.plotMinSignificance",4.));

  // Build and set title
  TString titleFigureOfMerit = getTagStringDefault("profile.titleFigureOfMerit", "Z_{exp}");
  TString plotTitle = getTagStringDefault("profile.title", hist->GetTitle());
  TString titleSuffix = TQGridScanStyle::getObsTitleSuffix(obsName, direction);
  TString fullTitle = ";";
  if(!plotTitle.IsNull()){
    plotTitle.ReplaceAll("$(VAR)", hist->GetTitle());
    if (plotTitle.First("[") > 0 || plotTitle.First("(") > 0) { // if unit is specified
      fullTitle += plotTitle.Insert(plotTitle.First("["), titleSuffix+" ")+";";
    } else {fullTitle += plotTitle+" "+titleSuffix+";";}
  } else {
    fullTitle += ";";
  }
  fullTitle += titleFigureOfMerit;
  hist->SetTitle(fullTitle);

  // Now that the hist is filled, we can rebin it; rebinning only happens if necessary
  // (i.e. stepSize > 1)
  hist = TQGridScanStyle::rebinHist(std::move(hist), range, direction);

  return hist;
}

std::unique_ptr<TGraphAsymmErrors> TQGridScanResults::makeGraphFromLowBinEdges(TH1* hist, BoundDirection direction) {
  // Convert histogram to TGraphAsymmErrors with low bin edges of histogram as points
  // and errors only in one direction, depending on the BoundDirection
  // Those errors are in the end displayed as arrows in the 'direction the cut is applied'
  double binwidth = hist->GetBinWidth(1);
  double lowlimit = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
  double uplimit = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
  auto gr = unique_ptr<TGraphErrors>(new TGraphErrors(hist));
  std::vector<double> x, y, exlow, eylow, exhigh, eyhigh;
  for (int i=0; i <  gr->GetN(); i++) {
    double tmp_x, tmp_y;
    gr->GetPoint(i, tmp_x, tmp_y);
    // only regard points in range
    if ((tmp_x < lowlimit) || (tmp_x > uplimit)) continue;
    double tmp_ex = gr->GetErrorX(i);
    // double tmp_ey = gr->GetErrorY(i); // XXX BW: hashed to fix compiler warning (unused variable)
    // shift x by half binwidth
    tmp_x = tmp_x - binwidth/2.;
    x.push_back(tmp_x);
    y.push_back(tmp_y);
    if (direction == BoundDirection::lower) {
      exlow.push_back(0);
      exhigh.push_back(tmp_ex);
    } else if (direction == BoundDirection::upper) {
      exlow.push_back(tmp_ex);
      exhigh.push_back(0);
    } else { // split cut, no arrow -> no error
      exlow.push_back(0);
      exhigh.push_back(0);
    }
    eylow.push_back(0);
    eyhigh.push_back(0);
  }

  auto ga = unique_ptr<TGraphAsymmErrors>(new TGraphAsymmErrors(x.size(), x.data(), y.data(), exlow.data(), exhigh.data(), eylow.data(), eyhigh.data()));
  ga->GetXaxis()->SetRangeUser(lowlimit, uplimit);
  if (direction == BoundDirection::upper) {
    ga->GetXaxis()->SetRangeUser(lowlimit-binwidth, uplimit);
  }
  ga->SetMinimum(hist->GetMinimum());
  ga->SetMaximum(hist->GetMaximum());
  ga->GetXaxis()->SetTitle(hist->GetXaxis()->GetTitle());
  ga->GetYaxis()->SetTitle(hist->GetYaxis()->GetTitle());
  return ga;
}

void TQGridScanResults::setFOMIndexForPlotting(int fom_index_for_plotting) {
  if (unsigned(fom_index_for_plotting) >= FOMDefinitions.size()) {
    WARNclass("Index for figure of merit to plot is specified to %d, which is out of range! Falling back to using the first specified figure of merit (index 0)", fom_index_for_plotting);
    m_fom_index_for_plotting = 0;
    return;
  }
  m_fom_index_for_plotting = fom_index_for_plotting;
}

void TQGridScanResults::plotInputDistributions(TQTaggable& tags, const TString& channelName) {
  DEBUGclass("This function will plot the input distributions for the first defined region only!");
  gROOT->SetBatch(true);
  TStyle *style = TQHistogramUtils::ATLASstyle();
  style->SetName("ATLAS");
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();

  for (unsigned ireg = 0; ireg < m_inputHistsSig.size(); ireg++) {
    TString regionName = m_inputHistsSig[ireg].regionName;

    for (unsigned ihist = 0; ihist < m_inputHistsSig[ireg].hists.size(); ihist++) {

      TH1F* hist_sig = &m_inputHistsSig[ireg].hists[ihist];
      TH1F* hist_bkg = &m_inputHistsBkg[ireg].hists[ihist];
      TString obsName = TQFolder::makeValidIdentifier(hist_sig->GetName());

      // Create canvas and set some style options
      auto c = TQGridScanStyle::defaultCanvas();
      c->cd();
      TQGridScanStyle::setStyle(c.get(), hist_sig, tags);
      TQGridScanStyle::setStyleInputHists(hist_sig, hist_bkg, tags);

      // Add generic legend
      unique_ptr<TLegend> leg (new TLegend(0.65, 0.75, 0.9, 0.9));
      leg->AddEntry(hist_sig, "Signal", "F");
      leg->AddEntry(hist_bkg, "Total Bkg", "F");

      // Draw not normalized histogram
      hist_bkg->GetYaxis()->SetTitle("Events");
      hist_bkg->Draw("hist");
      hist_sig->Draw("histsame");
      leg->Draw("same");

      // Save
      TString baseFilePath = tags.getTagStringDefault("plotDirectory", "plots/");
      baseFilePath = TQFolder::concatPaths(baseFilePath, "input-observables");
      TString extension = tags.getTagStringDefault("plotFormat","pdf");
      TQUtils::ensureDirectory(TQPathManager::getPathManager()->getTargetPath(baseFilePath));
      TString plotname;
      if (!channelName.IsNull()) {
          plotname = "inputDistribution-"+channelName+"-"+regionName+"-"+obsName;
        } else {
          plotname = "inputDistribution-"+regionName+"-"+obsName;
        }
      TString outputname =  TQFolder::concatPaths(baseFilePath, plotname);
      c->SaveAs( TQPathManager::getPathManager()->getTargetPath(outputname+"."+extension).c_str() );

      // Save normalized histogram
      c->Clear();
      hist_sig->Scale(1./hist_sig->GetSumOfWeights());
      hist_bkg->Scale(1./hist_bkg->GetSumOfWeights());
      double ymax1 = TQHistogramUtils::getHistogramMaximum(2, hist_sig, hist_bkg);
      hist_sig->GetYaxis()->SetRangeUser(0, ymax1*3/2.);
      hist_sig->GetYaxis()->SetTitle("Normalized Events");
      hist_sig->Draw("hist");
      hist_bkg->Draw("histsame");
      leg->Draw("same");
      c->SaveAs( TQPathManager::getPathManager()->getTargetPath(outputname+"_norm."+extension).c_str() );
    }
  }
  gROOT->SetBatch(false);
  gROOT->SetStyle("Default");
  gROOT->ForceStyle();
}

void TQGridScanResults::printPoint(TQGridScanPoint* point) {

  auto normalBinEdges = point->normalVals(m_normalObs);
  auto normalBinBounds = point->normalBounds();
  auto splitBinEdges = point->splitVals(m_splitObs);

  auto nNormalObs = normalBinEdges.size();
  for(size_t i = 0; i < nNormalObs; ++i) {
    auto vals = normalBinEdges[i];
    auto lowerVal = vals.first;
    auto upperVal = vals.second;
    auto binbounds = normalBinBounds[i];
    auto lowerBin = binbounds.first;
    auto upperBin = binbounds.second;
    auto obs = m_normalObs[i];
    auto obsName = obs->name();
    if (lowerVal == kValNotSet and upperVal == kValNotSet) {
      std::cout << obsName << " (no cut)";
    }
    else {
      if (lowerVal != kValNotSet) std::cout << lowerVal;
      if (lowerBin == TQGridScanBound::BinExtrema::min) std::cout << " (underflow bin)";
      std::cout << " < ";
      std::cout << obsName;
      if (upperVal != kValNotSet) std::cout << " < " << upperVal;
      if (upperBin == TQGridScanBound::BinExtrema::max) std::cout << " (overflow bin)";
    }
    TQGridScanStyle::printDelimiter(i, nNormalObs);
  }
  auto nSplitObs = splitBinEdges.size();
  for(size_t i = 0; i < nSplitObs; ++i) {
    auto val = splitBinEdges[i];
    auto obsName = m_splitObs[i]->name();
    std::cout << obsName << " | " << val;
    TQGridScanStyle::printDelimiter(i, nSplitObs);
  }
  std::cout << point->info() << std::endl;
}
