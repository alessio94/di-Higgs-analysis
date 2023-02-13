#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"

#include "QFramework/TQIterator.h"
#include "QFramework/TQDefaultPlotter.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

#define DEFAULTTEXTSIZE 0.04
////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQDefaultPlotter:
//
// The TQDefaultPlotter is an example implementation, inheriting basic
// plotting functionality from the abstract TQPlotter base class.
//
////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(TQDefaultPlotter)

namespace {
  enum PlotType {
    Stack,Profile,Contours,Heatmap,Migration
  };

  enum SubPlotType {
    None,Ratio,DataMinusBackground,CutOptimization
  };
}

TCanvas * TQDefaultPlotter::makePlot(TQTaggable& tags){
  // master-function controlling the plotting
  bool verbose = tags.getTagBoolDefault("verbose",false);
  
  // set some basic options
  gStyle->SetOptStat(false);
  gStyle->SetOptFit(false);
  gStyle->SetOptTitle(false);

  if (!tags.hasTag("style.textSize"))
    tags.setTagDouble("style.textSize", DEFAULTTEXTSIZE);
  if (!tags.hasTag("style.legend.textSize"))
    tags.setTagDouble("style.legend.textSize", 0.85*tags.getTagDoubleDefault("style.textSize",DEFAULTTEXTSIZE));
  if (!tags.hasTag("geometry.legend.nRows"))
    tags.setTagDouble("geometry.legend.nRows",4.);
  if(verbose) VERBOSEclass("collecting histograms");
  TObjArray* histos = this->collectHistograms(tags);
  if(!histos) return NULL;

  // determine the plot type
  TQTH1Iterator histitr(histos);
  int nEntries = 0;
  PlotType plottype = Stack;
  while(histitr.hasNext()){
    TH1* hist = histitr.readNext();
    nEntries += hist->GetEntries();
    if(dynamic_cast<TH2*>(hist)){
      if(tags.hasTagString("style.heatmap")){
        plottype=Heatmap;
      } else if(tags.hasTagString("style.migration")){
        plottype=Migration;
      } else {
        plottype=Contours;
      }
    }
    if(dynamic_cast<TProfile*>(hist)) plottype=Profile;
  }
  if(nEntries < 1){
    WARNclass("refusing to plot histogram - no entries!");
    return NULL;
  }

  // determine the subplot type
  TString subplotTypeString;
  SubPlotType subplot = None;
  //@tags:[style.subPlot] Activate a sub plot. Available options are "ratio", "cutopt", "dmb" (data minus background), none (equivalent to not setting the tag). If the main histogram is 2D, subplots will be deactivated.
  if(tags.getTagString("style.subPlot",subplotTypeString)){
    subplotTypeString.ToLower();
    if(subplotTypeString == "ratio") subplot = Ratio;
    else if(subplotTypeString == "cutopt") subplot = CutOptimization;
    else if(subplotTypeString == "dmb") subplot = DataMinusBackground;
    else if(subplotTypeString == "none") subplot = None;
    else {
      WARNclass("unknown subplot type '%s'",subplotTypeString.Data());
    }
    if (plottype != Stack)
      subplot = None;
  }
  if(subplot != None){
    // configure the subplot tags
    tags.setTagBool("style.showSub",true);
    double subpadSize = .35;
    //@tags:[geometry.sub.height] Set the height of the sub pad as a fraction of the full height (default 0.35)
    if(!tags.getTagDouble("geometry.sub.height",subpadSize)){
      tags.setTagDouble("geometry.sub.height",subpadSize);
    }
    double mainPadScaling = 1.;
    double subPadScaling = mainPadScaling / subpadSize;
    tags.setTagDouble("geometry.sub.yMax",subpadSize);
    tags.setTagDouble("geometry.sub.scaling",subPadScaling);
    tags.setTagDouble("geometry.sub.margins.top", tags.getTagDoubleDefault("geometry.sub.margins.top", 0.0));
    tags.setTagDouble("geometry.main.scaling",mainPadScaling);
    tags.setTagDouble("geometry.main.margins.bottom", subpadSize/mainPadScaling);
    tags.setTagBool("style.main.xAxis.showLabels",false);
    tags.setTagBool("style.main.xAxis.showTitle",false);
  }


  TH1* hMaster = this->getObject<TH1>("Graph_master");
  if (!hMaster){
    if(verbose) VERBOSEclass("no master histogram found, quitting");
    return NULL;
  }
  this->setAxisLabels(tags);

  //////////////////////////////////////////////////////
  // apply the style
  //////////////////////////////////////////////////////

  double xscaling = tags.getTagDoubleDefault("geometry.xscaling",1.);
  //@tags:[geometry.main.*] control geometry parameters of the main pad
  this->applyGeometry(tags,hMaster, "main", xscaling,tags.getTagDoubleDefault("geometry.main.scaling",1.));
  this->applyStyle (tags,hMaster, "main", xscaling,tags.getTagDoubleDefault("geometry.main.scaling",1.));

  TH1* hTotalBkg = this->getObject<TH1>("totalBkg");
  if(hTotalBkg){
    this->applyStyle (tags,hTotalBkg,"main.totalBkg",xscaling,tags.getTagDoubleDefault("geometry.main.scaling",1.));
  }
  TH1* hTotalSig = this->getObject<TH1>("totalSig");
  if(hTotalSig){
    this->applyStyle (tags,hTotalSig,"main.totalSig",xscaling,tags.getTagDoubleDefault("geometry.main.scaling",1.));
  }

  //////////////////////////////////////////////////////
  // canvas and pads
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("creating canvas");
  TCanvas * canvas = this->createCanvas(tags);
  if(!canvas) return NULL;
  canvas->Draw();
  TPad* pad = this->getPad("main");
  if(!pad){
    delete canvas;
    return NULL;
  }
  canvas->cd();

  //////////////////////////////////////////////////////
  // create the stack
  //////////////////////////////////////////////////////

  if(plottype == Stack){
    this->stackHistograms(tags,"stack");
  }

  //////////////////////////////////////////////////////
  // legend
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("creating legend");
  if(tags.getTagBoolDefault("style.useLegendPad",false)){
    canvas->cd();
    TPad * legendPad = this->createPad(tags,"legend");
    legendPad->Draw();
    this->getPad("legend");
    if(!legendPad) return NULL;
    this->makeLegend(tags,histos);
  } else {
    this->makeLegend(tags,histos);
  }
  //create the legend before the stack! (stack creation also involves normalization, i.e., after the stack is created we don't have the original event yields at hand anymore!)

  //////////////////////////////////////////////////////
  // basic label setup
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("setting labels");
  TString label;
  tags.setGlobalOverwrite(true);

  int labelidx = 0;
  if (tags.getTagString("labels.lumi", label)){
    labelidx++;
    tags.setTagString(TString::Format("labels.%d",labelidx), label);
  }
  if (tags.getTagString("labels.process", label)){
    labelidx++;
    tags.setTagString(TString::Format("labels.%d",labelidx), label);
  }

  //////////////////////////////////////////////////////
  // manage the total background error bars
  //////////////////////////////////////////////////////

  // set the errors
  if(tags.hasTag("errors.showSys")){
    if(verbose) VERBOSEclass("including systematics");
    this->includeSystematics(tags);
  }
  this->setErrors(tags,tags.getTagStringDefault("errors.source","totalBkg"));

  tags.setGlobalOverwrite(false);

  //////////////////////////////////////////////////////
  // draw main pad
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("drawing main pad");
  pad->cd();
  bool ok = false;
  switch(plottype){
  case Profile:
    ok = this->drawProfiles(tags);
    this->drawLegend(tags);
    break;
  case Stack:
    ok = this->drawStack(tags);
    this->drawLegend(tags);
    if(verbose) VERBOSEclass("drawing cut lines");
    this->drawCutLines1D(tags);
    break;
  case Heatmap:
    ok = this->drawHeatmap(tags);
    if(tags.getTagBoolDefault("style.useLegendPad",false)){
      TPad * legendPad = this->getPad("legend");
      legendPad->cd();
    }
    this->drawLegend(tags);
    pad->cd();
    this->drawAdditionalAxes(tags);
    break;
  case Migration:
    ok = this->drawMigration(tags);
    if(tags.getTagBoolDefault("style.useLegendPad",false)){
      TPad * legendPad = this->getPad("legend");
      legendPad->cd();
    }
    this->drawLegend(tags);
    pad->cd();
    break;
  case Contours:
    ok = this->drawContours(tags);
    if(tags.getTagBoolDefault("style.useLegendPad",false)){
      TPad * legendPad = this->getPad("legend");
      legendPad->cd();
    }
    this->drawLegend(tags);
    pad->cd();
    this->drawHeightLines(tags);
    this->drawAdditionalAxes(tags);
  }
  if (not ok) VERBOSEclass("Did not attempt to draw main pad or drawing was unsuccessful");

  //////////////////////////////////////////////////////
  // draw the labels
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("drawing labels");
  this->drawLabels(tags);

  //////////////////////////////////////////////////////
  // redraw main pad
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("refreshing drawing area");
  pad->RedrawAxis();
  pad->Update();
  pad->cd();

  //////////////////////////////////////////////////////
  // draw sub pad
  //////////////////////////////////////////////////////

  if(subplot != None){
    TPad* subPad = this->getPad("sub");
    if(!subPad) throw std::runtime_error("unable to obtain sub pad!");
    subPad->cd();
  }
  switch(subplot){
  case Ratio:
    if(verbose) VERBOSEclass("drawing ratio");
    this->drawSub_Ratio(tags);
    break;
  case CutOptimization:
    if(verbose) VERBOSEclass("drawing cut optimization scan");
    this->drawSub_CutOptimization(tags);
    break;
  case DataMinusBackground:
    if(verbose) VERBOSEclass("drawing data-minus-background");
    this->drawSub_DataMinusBackground(tags);
    break;
  default:
    if(verbose) VERBOSEclass("Do not draw subplot");
  }

  if(verbose) VERBOSEclass("all done!");
  // return the canvas
  return canvas;
}


//__________________________________________________________________________________|___________

void TQDefaultPlotter::drawLabels(TQTaggable& tags){
  bool verbose = tags.getTagBoolDefault("verbose",false);
  // draw the labels given by the tags
  this->getPad("main");

  double scaling = tags.getTagDoubleDefault("geometry.main.scaling",1.);
  double textsize = tags.getTagDoubleDefault("style.textSize",DEFAULTTEXTSIZE);
  int font = tags.getTagDoubleDefault("style.text.font",42);
  int color = tags.getTagDoubleDefault("style.text.color",1);
  double xscaling = tags.getTagDoubleDefault("geometry.xscaling",1.);
  //@tag:geometry.labels.xPos: Set the position of the default labels (ATLAS, xsec, lumi, ...). The value specifies the left edge. (Default: 0.2)
  //@tag:geometry.labels.yPos: Set the position of the default labels (ATLAS, xsec, lumi, ...). The value specifies the top edge. (Default: 0.86)
  double x = tags.getTagDoubleDefault("geometry.labels.xPos",0.2)*xscaling;
  double y = tags.getTagDoubleDefault("geometry.labels.yPos",0.86);

  //@tag:labels.atlas: draw ATLAS label. If "true" or "false", label is drawn/not drawn. If set to any other string, this string will be displayed after the ATLAS label (eg. "Internal")
  TString atlasLabel;
  if(tags.getTagString("labels.atlas",atlasLabel) && atlasLabel != "false"){
    //@tag:labels.atlas.scale: scale of the addition to the ATLAS label (Internal, Private,...). Defaults to 1.25 if neither of the two tags are present
    double atlasScale = tags.getTagDoubleDefault("labels.atlas.scale",1.25) * scaling;
    // draw the ATLAS label
    TLatex atlas;
    atlas.SetNDC();
    atlas.SetTextFont(72);
    //@tag:scale of the ATLAS label wrt other text size (1.25 by default)
    atlas.SetTextSize(textsize * atlasScale);
    atlas.SetTextColor(1);
    //@tag:labels.atlas.yPos : Set a different y position for the ATLAS label than for the other labels
    double atlas_y = tags.getTagDoubleDefault("labels.atlas.yPos", y);
    atlas.DrawLatex(x, atlas_y, "ATLAS");
    if(!atlasLabel.IsNull() && atlasLabel != "true"){
      TLatex label;
      label.SetNDC();
      label.SetTextFont(font);
      label.SetTextColor(color);
      label.SetTextSize(textsize * atlasScale);
      //@tag:labels.atlas.xPos : horizontal offset between ATLAS label and its addition. (default: 0.16)
      label.DrawLatex(x + tags.getTagDoubleDefault("labels.atlas.xPos",0.16)*xscaling, atlas_y, atlasLabel.Data());
    }
  }

  TString nfLabel;
  //@tag:labels.info: decide whether to draw the technical info tag on the top right of the plot
  //@tag:labels.drawNFInfo: decide whether to draw information on which NFs were applied. This is only shown if labels.info is enabled.
  if(tags.getTagBoolDefault ("labels.drawNFInfo",false)){
    TString tmpLabel = tags.getTagStringDefault("labels.nfInfo","#color[2]{(NF applied for %s)}");
    if(TQStringUtils::countText(tmpLabel,"%s") == 1){
      TString nflist = this->getScaleFactorList(tags.getTagStringDefault("input.histogram",""));
      if(!nflist.IsNull()){
        nfLabel = TString::Format(tmpLabel.Data(),TQStringUtils::convertLaTeX2ROOTTeX(nflist).Data());
      }
    }
  }
  TString infoLabel;
  bool drawInfo = tags.getTagString ("labels.info",infoLabel);
  if (verbose and not drawInfo) VERBOSEclass("Did not get infoLabel tag");
  if(infoLabel.IsNull() || infoLabel == "true"){
    infoLabel = TString::Format("Plot: \"%s\"", tags.getTagStringDefault("input.histogram","histogram").Data() );
    if(!nfLabel.IsNull()){
      infoLabel.Prepend(" ");
      infoLabel.Prepend(nfLabel);
    }
  }
  if (!infoLabel.IsNull() && infoLabel != "false") {
    // draw the info label
    TLatex l0;
    l0.SetNDC();
    l0.SetTextFont(font);
    l0.SetTextSize(textsize * tags.getTagDoubleDefault("labels.info.size",0.6) * scaling);
    l0.SetTextColor(color);
    double xpos = tags.getTagDoubleDefault("geometry.main.margins.left",0.16) + tags.getTagDoubleDefault("labels.info.xPos",1.)*(1. - tags.getTagDoubleDefault("geometry.main.margins.right",0.05) - tags.getTagDoubleDefault("geometry.main.margins.left",0.16));
    double ypos = 1. - scaling*(1.-tags.getTagDoubleDefault("labels.info.yPos",0.2))*tags.getTagDoubleDefault("geometry.main.margins.top",0.05);
    l0.SetTextAlign(tags.getTagIntegerDefault("labels.info.align",31));
    l0.DrawLatex(xpos, ypos, infoLabel.Data());
  }

  // draw  labels
  double marginStep = tags.getTagDoubleDefault("style.labels.marginStep",0.045);
  double labelTextScale = tags.getTagDoubleDefault("style.labels.scale",0.85);
  TQIterator itr(tags.getTagList("labels"),true);
  size_t index = 1;
  while(itr.hasNext()){
    TObject* obj = itr.readNext();
    if(!obj) break;
    TLatex latex;
    latex.SetNDC();
    latex.SetTextFont(font);
    latex.SetTextSize(textsize * labelTextScale * scaling);
    latex.SetTextColor(color);
    latex.DrawLatex(x, y - marginStep * index * scaling,obj->GetName());
    index++;
  }
}



void TQDefaultPlotter::drawSub_Ratio(TQTaggable& tags){
  // draw a ratio-plot in the sub-pad
  // Line colors of ratio histograms are taken from fill colors of the numerator of the corresponding main histogram
  bool verbose = tags.getTagBoolDefault("verbose",false);
  if (verbose) VERBOSEclass("'verbose' tag enabled");

  // @tags: [style.ratio.numeratorFilter]: Tag on processes that indicates that a process is considered as numerator. Default: ".isData".
  // @tags: [style.ratio.denominator]: Name of the histogram that is used for the denominator. Histogram names are formatted like "hist_ttbar" with process name "ttbar". Default: "totalStack".
  TString numeratorFilter = tags.getTagStringDefault("style.ratio.numeratorFilter",".isData");
  TString denominatorName = tags.getTagStringDefault("style.ratio.denominator","totalStack");

  // get the denominator and make the denominator error band
  TH1* denominatorOrig = this->getObject<TH1>(denominatorName);
  if(!denominatorOrig){
    ERRORclass("unable to find denominator '%s'",denominatorName.Data());
    return;
  }
  double xmin = denominatorOrig->GetXaxis()->GetXmin();
  double xmax = denominatorOrig->GetXaxis()->GetXmax();
  TH1* errors = TQHistogramUtils::copyHistogram(denominatorOrig,TString::Format("ratioDenominatorErrors_%s",denominatorOrig->GetName()));
  TQHistogramUtils::divideHistogramWithoutError(errors,denominatorOrig);

  // loop over all the numerators to prepare the ratio
  TString dataLabel("");
  std::vector<TH1*> ratios;
  std::vector<TString> drawOptions;
  TQTaggableIterator numerator_processes(fProcesses);
  bool copied_xaxis_style = false;
  double yLowerLimit    = tags.getTagDoubleDefault("style.sub.linMin",-std::numeric_limits<double>::infinity());
  double yLowerLimitLog = std::max(tags.getTagDoubleDefault("style.sub.logMin"), tags.getTagDoubleDefault("style.sub.logMinMin", 1e-9));

  double ymax = TQHistogramUtils::getMaximumBinValue(errors,xmin,xmax,true);
  double ymin = TQHistogramUtils::getMinimumBinValue(errors,xmin,xmax,true,yLowerLimit);
  double yminLog = TQHistogramUtils::getMinimumBinValue(errors,xmin,xmax,true,yLowerLimitLog);

  // @tag: [style.sub.showUnity]: The automatic y range of the ratio plot includes 1. Default: true.
  // @tag: [style.sub.logMin,style.sub.linMin,(style.sub.logMinMin)]: Restricts the automatic y-axis ranges of the sub plot for linear and log plots. They are not set by default. "style.logMinMin" defaults to 1e-9 and acts as an additional lower bound for log plots; use with great care!

  bool showUnity = tags.getTagBoolDefault("style.ratio.showUnity", true);
  if (!showUnity){
    ymax = -std::numeric_limits<double>::infinity();
    ymin    = std::numeric_limits<double>::infinity();
    yminLog = std::numeric_limits<double>::infinity();
  }

  while(numerator_processes.hasNext()){
    TQNamedTaggable* numeratorProcess = numerator_processes.readNext();
    if(!numeratorProcess || !numeratorProcess->getTagBoolDefault(numeratorFilter,false)) continue;
    TH1 * numeratorOrig = this->getObject<TH1>(this->makeHistogramIdentifier(numeratorProcess));
    if(!numeratorOrig) continue;
    if (!copied_xaxis_style) {
        TQHistogramUtils::copyAxisStyle(numeratorOrig, errors);
        copied_xaxis_style = true;
    }
    TH1* ratio = TQHistogramUtils::copyHistogram(numeratorOrig,TString::Format("ratio_%s_%s",numeratorOrig->GetName(),denominatorOrig->GetName()));

    // Color the ratio line with the fill color of the main histogram.
    // Don't do it if the fill color is white. Then it's probably data
    // and you don't want white error bars.
    if (ratio->GetFillColor() != kWhite){
      ratio->SetLineColor(ratio->GetFillColor());
    }
    ratio->SetLineWidth(2);

    TQHistogramUtils::divideHistogramWithoutError(ratio,denominatorOrig);
    ymax = std::max(ymax,TQHistogramUtils::getMaximumBinValue(ratio,xmin,xmax,true));
    ymin = std::min(ymin,TQHistogramUtils::getMinimumBinValue(ratio,xmin,xmax,true,yLowerLimit));
    yminLog = std::min(yminLog,TQHistogramUtils::getMinimumBinValue(ratio,xmin,xmax,true,yLowerLimitLog));
    ratios.push_back(ratio);
    drawOptions.push_back(numeratorProcess->getTagStringDefault("sub.drawOption", "P E"));
  }
  if (tags.getTagBoolDefault("style.sub.logScaleY", false)){
    ymin = yminLog;
  }

  double dy = ymax - ymin;
  ymin -= 0.1*dy;
  ymax += 0.1*dy;

  // do some final corrections

  // @tags: [style.ratio.numerator.title, style.ratio.denominator.title]: Name of numerator and denominator. If both are set, the y-axis caption of the ratio plot is "num / den". If the denominator string is empty, only the numerator is printed. Default: "Data" and the title of the denominator histogram.
  TString denominatorTitle = tags.getTagStringDefault("style.ratio.denominator.title",denominatorOrig->GetTitle());
  TString numeratorTitle =  tags.getTagStringDefault("style.ratio.numerator.title","Data");
  TString title = "";
  if (!TQStringUtils::isEmpty(denominatorTitle, true))
    title = tags.getTagStringDefault("style.ratio.title",TString::Format("%s / %s",numeratorTitle.Data(),denominatorTitle.Data()));
  else
    title = tags.getTagStringDefault("style.ratio.title",TString::Format("%s",numeratorTitle.Data()));
  errors->GetYaxis()->SetTitle(title);

  // @tags: [style.ratio.firstLabel, style.ratio.lastLabel]: show first/last ratio axis labels.
  // delete last label so it doesn't overlap with main axis "0"
  bool lastLabel = tags.getTagBoolDefault("style.sub.lastLabel",false);
  if (!lastLabel) errors->GetYaxis()->ChangeLabel(-1,-1,0.0);
  // delete first label to make things symmetric
  bool firstLabel = tags.getTagBoolDefault("style.sub.firstLabel",false);
  if (!firstLabel) errors->GetYaxis()->ChangeLabel(1,-1,0.0);

  // @tags:style.ratio.ndivisions: Control the number of divisions for the y-axis in the ratio plot. See TAttAxis::SetNdivisions().
  errors->GetYaxis()->SetNdivisions(tags.getTagIntegerDefault("style.sub.ndivisions", errors->GetYaxis()->GetNdivisions()));
  tags.getTagDouble("style.sub.min",ymin);
  tags.getTagDouble("style.sub.max",ymax);
  errors->SetMaximum(ymax);
  errors->SetMinimum(ymin);
  double scaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);
  this->applyGeometry(tags,errors, "main", 1.,scaling);
  this->applyStyle (tags,errors, "sub", 1.,scaling);
  this->applyStyle (tags,errors, "ratio.errors", 1.,scaling);  

  // draw everything
  // unity line
  if (showUnity){
    errors->Draw("E2");
    TLine line;
    line.SetLineStyle(errors->GetLineStyle());
    line.SetLineWidth(errors->GetLineWidth());
    line.DrawLine(errors->GetXaxis()->GetXmin(),1,errors->GetXaxis()->GetXmax(),1);
  } else{
    errors->SetLineWidth(0);
    errors->SetMarkerSize(0);
    errors->Draw();
  }
  // ratios
  for(size_t i = 0; i < ratios.size(); i++) {

    ratios[i]->Draw(drawOptions[i] + "SAME");

    // Plotting with the E0 option draws errorbars also if the datapoint
    // is outside of the pad (that's what we want), but it draws datapoints
    // when they are at 0 (we don't want that).
    // Workaround: set the marker size to 0 and draw with E0 option. For
    // some reason, plotting the original histogram messes things up. So
    // use a clone instead.
    // The clone is not deleted (memory leak!), because otherwise the errorbars
    // disappear again on the plot.
    if (drawOptions[i].Contains("E")) {
      TH1* ratioClone = (TH1*) ratios[i]->Clone();
      ratioClone->SetMarkerSize(0);
      ratioClone->Draw("SAME " + drawOptions[i] + "0");
    }

    this->drawArrows(tags,ratios[i],ymin,ymax);
  }
}

void TQDefaultPlotter::drawSub_DataMinusBackground(TQTaggable& tags){
  // draw a data-minus-background plot in the sub-pad

  TString subtractionName = tags.getTagStringDefault("style.dmb.subtraction","totalBkg");
  TH1* subtraction = this->getObject<TH1>(subtractionName);
  if(!subtraction) return;

  TString errorSourceName = tags.getTagStringDefault("style.dmb.errors","totalBkg");
  TH1* errorSource = this->getObject<TH1>(errorSourceName);
  if(!errorSource) return;

  // set the x range of the dmb graph to match the one of the main histogram
  double subPadScaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);

  TString dataLabel("");

  double max = 0;
  double min = 0;

  std::vector<TH1*> v_signal;
  std::vector<TH1*> v_data;

  std::vector<TString> drawOptions;

  TQTaggableIterator itrSig(fProcesses);
  while(itrSig.hasNext()){
    TQNamedTaggable* process = itrSig.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(".isSignal",false)) continue;
    TH1* h_sig = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    h_sig = TQHistogramUtils::copyHistogram(h_sig,TString::Format("%s_dminusb",h_sig->GetName()));
    this->applyStyle(tags,h_sig,"sub.sig",1.,subPadScaling);
    max = std::max(max,TQHistogramUtils::getMax(h_sig,false));
    min = std::min(min,TQHistogramUtils::getMin(h_sig,false));
    if(!h_sig) continue;
    v_signal.push_back(h_sig);
  }

  // loop over data histograms
  TQTaggableIterator itr(fProcesses);
  while(itr.hasNext()){
    // get the data histogram
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(".isData",false)) continue;
    TH1 * h_data = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    h_data = TQHistogramUtils::copyHistogram(h_data,TString::Format("%s_minus_bkg",h_data->GetName()));
    if(!h_data) continue;
    this->applyStyle(tags,h_data,"sub.data",1.,subPadScaling);
    TQHistogramUtils::addHistogramWithoutError(h_data,subtraction,-1.);
    max = std::max(max,TQHistogramUtils::getMax(h_data,true));
    min = std::min(min,TQHistogramUtils::getMin(h_data,true));
    v_data.push_back(h_data);
    drawOptions.push_back(process->getTagStringDefault("sub.drawOption", "P E"));    
  }

  gStyle->SetEndErrorSize(0);

  TGraphErrors* errorGraph = new TGraphErrors(errorSource->GetNbinsX());
  TQHistogramUtils::copyStyle(errorGraph,errorSource);
  for(int i=1; i<errorSource->GetNbinsX(); ++i){
    double val = errorSource->GetBinError(i);
    max = std::max(max,1.1*val);
    min = std::min(min,-1.1*val);
    errorGraph->SetPoint(i,errorSource->GetBinCenter(i),errorSource->GetBinContent(i)-subtraction->GetBinContent(i));
    errorGraph->SetPointError(i,errorSource->GetBinCenter(i)-errorSource->GetBinLowEdge(i),val);
  }

  double margin = tags.getTagDoubleDefault("style.sub.margin",1.1);

  TH1* frame = TQHistogramUtils::copyHistogram(subtraction,"Graph_subFrame");
  frame->Reset();
  frame->SetMaximum(margin*max);
  frame->SetMinimum(margin*min);
  this->applyStyle (tags,frame,"sub");
  this->applyGeometry(tags,frame,"main",1.,subPadScaling);
  this->applyGeometry(tags,frame,"sub",1.,subPadScaling);

  frame->GetYaxis()->SetTitle("Data-Bkg.");
  frame->Draw("HIST");

  // sort by integral to have smallest contribution in front
  std::sort(v_signal.begin(),v_signal.end(),[&](TH1* a, TH1* b){ return a->GetSumOfWeights() >= b->GetSumOfWeights();});

  for(auto h:v_signal){
    h->Draw("HISTSAME");
  }

  errorGraph->SetTitle("mc error band");
  this->applyStyle(tags,errorGraph,"main.totalStackError",1.,subPadScaling);
  errorGraph->Draw("E2SAME");

  for(size_t i = 0; v_data.size(); i++) {
    v_data[i]->Draw(drawOptions[i] + "SAME");

    // See explanation in drawSub_Ratio above.
    TH1* hClone = (TH1*) v_data[i]->Clone();
    hClone->SetMarkerSize(0);
    hClone->Draw("SAME " + drawOptions[i] + "0");
  }
}

void TQDefaultPlotter::drawSub_CutOptimization(TQTaggable& tags){
  // draw a cut optimization scan in the sub-pad
  double xscaling = tags.getTagDoubleDefault("geometry.xscaling",1.);
  double subPadScaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);
  bool verbose = tags.getTagBoolDefault("verbose",false);

  TH1* hTotalStack = this->getObject<TH1>("totalStack");
  if(!hTotalStack) return;

  TQTaggableIterator itr(this->fProcesses);
  TH1* hSig = NULL;
  while(itr.hasNext() && !hSig){
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(process->getTagBoolDefault(".isSignal",false)){
      hSig = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    }
  }
  if(!hSig) return;

  //@tag:optScan.FOMmode: figure of merit to be used. currently available are: s/sqrt(s+b),s/b,poisson,poissonwerr,s/sqrt(b),s/sqrt(b+db2)
  //@tag:optScan.FOMbbb: evaluate the figure-of-merit bin-by-bin instead of integrated left and right (default:false)
  //@tag:style.FOMmode: deprecated, use optScan.FOMmode
  //@tag:style.FOMbbb: deprecated, use optScan.FOMbbb
  TString fommodestr = tags.getTagStringDefault("optScan.FOMmode",tags.getTagStringDefault ("style.FOMmode","s/sqrt(b)"));
  TQHistogramUtils::FOM FOMmode = TQHistogramUtils::readFOM(fommodestr);
  if(FOMmode == TQHistogramUtils::kUndefined){
    WARNclass("unknown figure of merit '%s'!",fommodestr.Data());
    return;
  }
  bool binByBin = tags.getTagBoolDefault("optScan.FOMbbb",tags.getTagBoolDefault ("style.FOMbbb",false));
  bool drawLegend = !binByBin;

  std::vector<TH1*> bkgSystHistos;
  collectOptScanSimplifiedSystHistograms(bkgSystHistos, tags); //TODO? if this returns false something was wrong and no syst histograms are provided -> should abort?

  std::map<std::string,TH1*> hists;
  double actualmax = 0;
  if(binByBin){
    if(verbose){
      VERBOSEclass("drawing bin-by-bin significances with FOM=%s for S=%s and B=%s",TQHistogramUtils::getFOMTitleROOT(FOMmode).Data(),hSig->GetTitle(),hTotalStack->GetTitle());
    }
    TH1* hFOM = TQHistogramUtils::getFOMHistogram(FOMmode,hSig, hTotalStack, 0, bkgSystHistos);
    actualmax = hFOM->GetMaximum() * tags.getTagDoubleDefault("optScan.enlargeY",1.3);
    this->addObject(hFOM,"hist_FOM_bbb");
    hists["default"] = hFOM;
  } else {
    if(verbose){
      VERBOSEclass("drawing optimization scan with FOM=%s for S=%s and B=%s",TQHistogramUtils::getFOMTitleROOT(FOMmode).Data(),hSig->GetTitle(),hTotalStack->GetTitle());
    }
    TH1* hFOMl = TQHistogramUtils::getFOMScan(FOMmode,hSig, hTotalStack, true, 0.05,verbose, bkgSystHistos);
    TH1* hFOMr = TQHistogramUtils::getFOMScan(FOMmode,hSig, hTotalStack, false,0.05,verbose, bkgSystHistos);
    if (FOMmode == TQHistogramUtils::kSoSqB){
      double rmax = hFOMr->GetBinContent(hFOMr->GetMaximumBin());
      double rmaxxval = hFOMr->GetBinLowEdge(hFOMr->GetMaximumBin()) + hFOMr->GetBinWidth(hFOMr->GetMaximumBin());
      double lmax = hFOMl->GetBinContent(hFOMl->GetMaximumBin());
      double lmaxxval = hFOMl->GetBinLowEdge(hFOMl->GetMaximumBin());
      hFOMl->SetTitle(TString::Format("#rightarrow cut Max=%.2f (%.2f)",lmax,lmaxxval));
      hFOMr->SetTitle(TString::Format("#leftarrow cut Max=%.2f (%.2f)",rmax,rmaxxval));
    } else {
      hFOMl->SetTitle("#rightarrow cut");
      hFOMr->SetTitle("#leftarrow cut" );
    }
    //@tag:optScan.autoselect: select the optimization scan (left,right) that is better suited for every histogram and only show that one (default:false)
    if(tags.getTagBoolDefault("optScan.autoselect",false)){
      if(verbose) VERBOSEclass("autoselecting opt scan");
      if(TQHistogramUtils::hasGreaterMaximumThan(hFOMr,hFOMl)){
        if(verbose) VERBOSEclass("removing left-hand FOM histogram");
        this->addObject(hFOMr,"hist_FOM");
        hists["right"] = hFOMr;
        delete hFOMl;
        actualmax = hFOMr->GetMaximum() * tags.getTagDoubleDefault("optScan.enlargeY",1.3);
      } else {
        if(verbose) VERBOSEclass("removing right-hand FOM histogram");
        this->addObject(hFOMl,"hist_FOM");
        hists["left"] = hFOMl;
        delete hFOMr;
        actualmax = hFOMl->GetMaximum() * tags.getTagDoubleDefault("optScan.enlargeY",1.3);
      }
    } else {
      if(verbose) VERBOSEclass("using all opt scans");
      hists["right"] = hFOMr;
      hists["left"] = hFOMl;
      this->addObject(hFOMl,"hist_FOM_left");
      this->addObject(hFOMr,"hist_FOM_right");
      actualmax = std::max(hFOMl->GetMaximum(),hFOMr->GetMaximum()) * tags.getTagDoubleDefault("optScan.enlargeY",1.3);
    }
  }

  //cleanup
  for (TH1* toDel : bkgSystHistos) {
    delete toDel;
  }
  bkgSystHistos.clear(); //just to make sure the otherwise dangling pointers can not accessed anymore afterwards...



  TLegend* leg = NULL;
  if(drawLegend){
    leg = new TLegend(0.21,0.85,0.93,0.95);
    leg->SetNColumns(2);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetLineColor(0);
    leg->SetLineWidth(0);
  }

  bool first = true;
  for(auto h:hists){
    //@tag:style.optScan.default.*: control styling of the auto-selected FOM graph
    if(verbose) VERBOSEclass("drawing FOM histogram");
    this->applyStyle (tags,h.second,"optScan."+h.first,xscaling,subPadScaling);
    this->applyGeometry(tags,h.second,"main" ,xscaling,subPadScaling);
    this->applyGeometry(tags,h.second,"sub" ,xscaling,subPadScaling);
    h.second->SetMaximum(actualmax);
    h.second->SetFillStyle(0);
    h.second->SetMinimum(0);
    h.second->SetNdivisions(50008);
    h.second->GetYaxis()->SetNdivisions(50004);
    h.second->Draw(first ? "HIST" : "HIST SAME");
    if(drawLegend){
      leg->AddEntry(h.second,h.second->GetTitle(),"l");
    }
    first = false;
  }

  if(drawLegend){
    leg->Draw("SAME");
  }
}
