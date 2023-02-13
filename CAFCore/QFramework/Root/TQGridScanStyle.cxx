#include "TCanvas.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
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

using std::unique_ptr;

using BoundDirection = TQGridScanBound::Direction;

// Rebins a histogram based on the desired range (which includes step size)
// Histograms with different binning (i.e. stepSize > 1) than the input ndim hist should be
// rebinned; otherwise, nothing happens
unique_ptr<TH1F> TQGridScanStyle::rebinHist(unique_ptr<TH1F> hist, TQGridScanBound::Range range, BoundDirection direction) {
  int stepSize = range[2];
  if (stepSize > 1) {
    int lowerBin = range[0];
    int upperBin = range[1];
    int nBins = (upperBin - lowerBin) / stepSize + 1;

    // This is necessary because user-space upper bound is exclusive, but bin upper bound is
    // defined to be inclusive.
    if (direction == BoundDirection::upper) ++lowerBin;
    if (lowerBin == 0) lowerBin = 1;
    // The upper edge of the highest bin must also be stored
    double* newBins = new double[nBins + 1];
    double newMin = hist->GetXaxis()->GetBinLowEdge(lowerBin);
    double newBinSize = hist->GetXaxis()->GetBinLowEdge(lowerBin + stepSize) - newMin;
    for (auto i = 0; i < nBins + 1; ++i) {
      newBins[i] = newMin + i * newBinSize;
    }
    auto result = unique_ptr<TH1F>(dynamic_cast<TH1F*>(hist->Rebin(nBins, "", newBins)));
    delete newBins;
    return result;
  }
  return hist;
}

unique_ptr<TCanvas> TQGridScanStyle::defaultCanvas() {
  return unique_ptr<TCanvas>(new TCanvas("c", "c", 600, 600));
}

void TQGridScanStyle::drawLabels(TQTaggable& tags) {
  // draw some of the official atlas labels
  double textsize = tags.getTagDoubleDefault("style.textSize",0.05);
  int font = tags.getTagDoubleDefault("style.text.font",42);
  int color = tags.getTagDoubleDefault("style.text.color",1);
  double x = tags.getTagDoubleDefault("style.labels.xOffset",0.2);
  double y = tags.getTagDoubleDefault("style.labels.yPos",0.92);
  if (tags.getTagBoolDefault("style.drawATLAS",false)) {
    // draw the ATLAS label
    TLatex l;
    l.SetNDC();
    l.SetTextFont(72);
    l.SetTextSize(textsize * tags.getTagDoubleDefault("labels.drawATLAS.scale",1.25));
    l.SetTextColor(1);
    l.DrawLatex(x, y, tags.getTagStringDefault("labels.drawATLAS.text","ATLAS"));
    TString atlasLabel;
    if(tags.getTagString("labels.atlas.text",atlasLabel)) {
      // draw the ATLAS label addition
      TLatex p;
      p.SetNDC();
      p.SetTextFont(font);
      p.SetTextColor(color);
      p.SetTextSize(textsize * tags.getTagDoubleDefault("labels.atlas.scale",1.25));
      p.DrawLatex(x + tags.getTagDoubleDefault("labels.atlas.xOffset",0.16), y, atlasLabel.Data());
    }
  }
}

void TQGridScanStyle::setStyleAndRangeSignifProfile(TH1F* hist, TH1F* histmax, TQTaggable& tags) {
  hist->SetMarkerStyle(20);
  hist->SetMarkerColor(kBlack);
  hist->SetLineColor(kBlack);
  if (histmax) {
    histmax->SetMarkerStyle(21);
    histmax->SetMarkerColor(kBlue);
    histmax->SetLineColor(tags.getTagIntegerDefault("style.optimum.color",kRed));
    hist->SetMaximum(0.05*(histmax->GetMaximum() - hist->GetMinimum())+histmax->GetMaximum());
    histmax->SetMaximum(hist->GetMaximum());
    TQHistogramUtils::edge(histmax, tags.getTagDoubleDefault("profile.plotMinSignificance",0.));
  }
}

void TQGridScanStyle::setStyleInputHists(TH1F* hist_sig, TH1F* hist_bkg, TQTaggable& /*tags*/) {
  hist_sig->SetLineColor(kRed);
  hist_sig->SetLineWidth(2);
  hist_bkg->SetLineColor(kBlack);
  hist_bkg->SetLineWidth(2);
  double ymax = TQHistogramUtils::getHistogramMaximum(2, hist_sig, hist_bkg);
  hist_sig->GetYaxis()->SetRangeUser(0, ymax*3/2.);
  hist_bkg->GetYaxis()->SetRangeUser(0, ymax*3/2.);
}

void TQGridScanStyle::setStyleSignifProfileGraphs(TGraph* graph, TGraph* graphmax, TQTaggable& tags) {
  graphmax->SetLineWidth(tags.getTagDoubleDefault("profile.arrow.width", 2));
  graphmax->SetMarkerSize(tags.getTagDoubleDefault("profile.marker.size", 2));
  graphmax->SetMarkerStyle(tags.getTagDoubleDefault("profile.marker.style", 33));

  graph->SetLineWidth(tags.getTagDoubleDefault("profile.arrow.width", 2));
  graph->SetMarkerSize(tags.getTagDoubleDefault("profile.marker.size", 2));
  graph->SetMarkerStyle(27);
      
  graphmax->SetMarkerColor(tags.getTagDoubleDefault("profile.marker.color", 2));
  graphmax->SetFillColor(tags.getTagDoubleDefault("profile.marker.color", 2));
  graphmax->SetLineColor(tags.getTagDoubleDefault("profile.marker.color", 2));
      
  graph->SetMarkerColorAlpha(13, 1); // make gray
  graph->SetFillColorAlpha(13, 1);
  graph->SetLineColorAlpha(13, 1);
}

void TQGridScanStyle::setStyle(TPad* pad, TH1F* hist, TQTaggable& tags) {
  // set the style for 1D plotting
  TQROOTPlotter::setStyleAtlas();
  double xaxisLabelSize = tags.getTagDoubleDefault("style.XaxisLabelSize",0.04);
  double yaxisLabelSize = tags.getTagDoubleDefault("style.YaxisLabelSize",0.04);
  double xaxisTitleSize = tags.getTagDoubleDefault("style.XaxisTitleSize",0.04);
  double yaxisTitleSize = tags.getTagDoubleDefault("style.YaxisTitleSize",0.04);
  gStyle->SetLabelSize(xaxisLabelSize,"X");
  gStyle->SetLabelSize(yaxisLabelSize,"Y");
  gStyle->SetTitleSize(xaxisTitleSize,"X");
  gStyle->SetTitleSize(yaxisTitleSize,"Y");
  bool showTitle = tags.getTagBoolDefault("style.showTitle",true);
  if(!showTitle) {
    gStyle->SetOptTitle(false);
    // pad->SetTopMargin(0.01);
  }
  hist->UseCurrentStyle();
  pad->UseCurrentStyle();
  // histogram style
  gPad->Modified();
  gPad->Update();
  drawLabels(tags);
}

void TQGridScanStyle::drawLegend(TH1F* hist, TH1F* histmax, TQTaggable& tags, const TString& histlabel, TLine* cutline, TGraph* graph, TGraph* graphmax) {
  // draw the legend for this plot
  if(!hist && !histmax) return;
  gPad->Modified();
  gPad->Update();
  double ndc_hmargin = tags.getTagDoubleDefault("profile.legend.hmargin",0.2);
  double ndc_width = tags.getTagDoubleDefault("profile.legend.width",0.3);
  double ndc_height = tags.getTagDoubleDefault("profile.legend.height",0.3);
  double ndc_vmargin = tags.getTagDoubleDefault("profile.legend.vmargin",0.02);
  double xmax = TQHistogramUtils::getAxisXmax(hist);
  double xmin = TQHistogramUtils::getAxisXmin(hist);
  double ymax = TQHistogramUtils::getHistogramMaximum(2,hist,histmax);
  double ymin = TQHistogramUtils::getHistogramMinimum(2,hist,histmax);
  double width = ndc_width * (xmax - xmin);
  double height = ndc_height * (ymax - ymin);
  double hmargin = ndc_hmargin * (xmax - xmin);
  double hmargin_left = (ndc_hmargin-0.105) * (xmax - xmin);
  double vmargin = ndc_vmargin * (ymax - ymin);
  double left = xmin + hmargin;
  double right = xmax - hmargin;
  double bottom = ymin + vmargin;
  double top = ymax - vmargin;
  // first try: lets fit the legend on the right side
  double min,max;
  bool drawn = false;
  min = std::min(TQHistogramUtils::getMinimumBinValue(hist,xmax-width-hmargin,xmax),TQHistogramUtils::getMinimumBinValue(histmax,xmax-width-hmargin,xmax));
  max = std::max(TQHistogramUtils::getMaximumBinValue(hist,xmax-width-hmargin,xmax),TQHistogramUtils::getMaximumBinValue(histmax,xmax-width-hmargin,xmax));
  if(max < top-height-vmargin) {
    // does it fit at the top?
    bottom = top-height;
    drawn = true;
  } else if(min > bottom+height+vmargin) {
    // does it fit at the bottom?
    top = bottom+height;
    drawn = true;
  }
  if(drawn) {
    left = right-width;
  } else {
    // next try: lets fit the legend on the left side
    min = std::min(TQHistogramUtils::getMinimumBinValue(hist,xmin,xmin+width+hmargin_left),TQHistogramUtils::getMinimumBinValue(histmax,xmin,xmin+width+hmargin_left));
    max = std::max(TQHistogramUtils::getMaximumBinValue(hist,xmin,xmin+width+hmargin_left),TQHistogramUtils::getMaximumBinValue(histmax,xmin,xmin+width+hmargin_left));
    if(max < top-height-vmargin) {
      // does it fit at the top?
      bottom = top-height;
      drawn = true;
    } else if(min > bottom+height+vmargin) {
      // does it fit at the bottom?
      top = bottom+height;
      drawn = true;
    }
    if(drawn) right = left+width;
  }
  // Can't make this a unique_ptr because of weird ROOT ownership reasons or something
  // I hope it gets deleted!
  TLegend* legend;
  if(drawn) {
    legend = new TLegend(TQUtils::convertXtoNDC(left),TQUtils::convertYtoNDC(bottom),TQUtils::convertXtoNDC(right),TQUtils::convertYtoNDC(top));
  } else {
    legend = new TLegend(TQUtils::convertXtoNDC(right-width),TQUtils::convertYtoNDC(top-height),TQUtils::convertXtoNDC(right),TQUtils::convertYtoNDC(top));
    // we're unable to fit the legend, we need to rescale 
    if(histmax) histmax->SetMaximum(1.3*histmax->GetMaximum());
    if(hist) hist->SetMaximum(1.3*hist->GetMaximum());
  }
  legend->SetTextSize(tags.getTagDoubleDefault("profile.legend.fontSize",0.04));
  legend->SetFillStyle(3001);
  legend->SetFillColor(kWhite);
  legend->SetBorderSize(0);
  if (graph) { // Add entry with style from graphs
    legend->AddEntry(graph, histlabel, "P");
    if(graphmax) legend->AddEntry(graphmax,tags.getTagStringDefault("style.optimum.label","profiled optimum"), "P");
  } else {
    if(histmax) legend->AddEntry(histmax,tags.getTagStringDefault("style.optimum.label","profiled optimum"), "L");
    if(hist) legend->AddEntry(hist, histlabel, "LE");
  }
  if(cutline) legend->AddEntry(cutline,tags.getTagStringDefault("style.cutline.label","current cut value"),"L");
  if(legend) {
    legend->Draw();
  }
}

TString TQGridScanStyle::getObsTitleSuffix(TString /*obsName*/, BoundDirection direction) {
  // set the variable title of the given variable
  TString suffix;
  switch (direction) {
    case BoundDirection::lower:
      suffix = "min";
      break;
    case BoundDirection::upper:
      suffix = "max";
      break;
    case BoundDirection::split:
      suffix = "split";
      break;
  }
  return suffix;
}

void TQGridScanStyle::fillHists(TH1F* hist, TH1F* histSquares, TH1F* histFills, int nBin, double significance, int topNumber, bool showUnderflow, bool showOverflow) {
  auto nBins = hist->GetNbinsX();
  if(nBin < 1) {
    nBin = showUnderflow ? 1 : 0;
  } else if(nBin > nBins) {
    nBin = showOverflow ? nBins : nBins + 1;
  }
  if(histFills->GetBinContent(nBin) < topNumber) {
    hist->AddBinContent(nBin, significance);
    histSquares->AddBinContent(nBin, significance*significance);
    histFills->AddBinContent(nBin, 1);
  }
}

void TQGridScanStyle::fillErrors(TH1F* hist, TH1F* histSquares, TH1F* histFills) {
  for(int iBin=0; iBin<hist->GetNbinsX()+2; iBin++) {
    if(histFills->GetBinContent(iBin) > 1) {
      double err = sqrt(histSquares->GetBinContent(iBin) - pow(hist->GetBinContent(iBin),2));
      // check for nan in the case where the sum of squares and the square sum
      // are identical enough so that err will be the sqrt of a negative (yet small) number
      if(err != err) hist->SetBinError(iBin,0);
      // cut off exceedingly large error bars
      // relative errors on Z_exp larger than 100% are not sensible anymore
      else if(err > hist->GetBinContent(iBin)) hist->SetBinError(iBin, hist->GetBinContent(iBin));
      // set the error
      else hist->SetBinError(iBin,err);
    } else {
      hist->SetBinError(iBin,0);
    }
  }
}

void TQGridScanStyle::excludeOverflow(TAxis* axis) {
  auto first = axis->GetFirst();
  auto last = axis->GetLast();
  auto nBins = axis->GetNbins();
  auto showsUnderflow = first == 0;
  auto showsOverflow = last == nBins + 1;
  if (showsUnderflow and showsOverflow) {
    axis->SetRange(1, nBins);
  } else if (showsUnderflow) {
    axis->SetRange(1, last);
  } else if (showsOverflow) {
    axis->SetRange(first, nBins);
  }
}

void TQGridScanStyle::printDelimiter(int i, int n) {
  if(i == n) {
    std::cout << "; ";
  } else {
    std::cout << ", ";
  }
}

#if 0
Bound* TQGridScanStyle::getBound(TQGridScanObservable* obs, BoundDirection direction) {
  auto& bounds = obs->normalBounds();
  return (direction == BoundDirection::lower) ? &bounds.lower : &bounds.upper;
}

// Split bound
Bound* TQGridScanStyle::getSplitBound(TQGridScanObservable* obs) {
  return &obs->splitBounds();
}
#endif


