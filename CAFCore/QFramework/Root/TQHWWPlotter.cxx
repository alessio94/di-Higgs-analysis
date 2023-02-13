#include "TCanvas.h"
#include "TH1.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TLegend.h"
#include "TLatex.h"
#include "THStack.h"
#include "TParameter.h"
#include "TMap.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TArrow.h"
#include "TLine.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TGaxis.h"
#include "TFormula.h"
#include "TF1.h"
#include "TF2.h"
#include "TFitResult.h"
#include "TRandom3.h"
#include "TObjArray.h"

#include "QFramework/TQNamedTaggable.h"
#include "QFramework/TQHWWPlotter.h"
#include "QFramework/TQSampleDataReader.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQIterator.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <cmath>

#define DEFAULTTEXTSIZE 0.0375
////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQHWWPlotter:
//
// The TQHWWPlotter provides advanced plotting features for the H->WW->lvlv analysis.
// It inherits basic plotting functionality from the abstract TQPlotter base class.
//
////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(TQHWWPlotter)

//__________________________________________________________________________________|___________

TQHWWPlotter::TQHWWPlotter() : TQROOTPlotter() {
  // Default constructor of TQHWWPlotter class
}

//__________________________________________________________________________________|___________

TQHWWPlotter::TQHWWPlotter(TQSampleFolder * baseSampleFolder) : TQROOTPlotter(baseSampleFolder) {
  // Constructor of TQHWWPlotter class
}

//__________________________________________________________________________________|___________

TQHWWPlotter::TQHWWPlotter(TQSampleDataReader * dataSource) : TQROOTPlotter(dataSource) {
  // Constructor of TQHWWPlotter clas
}

//______________________________________________________________________________________________

TQHWWPlotter::~TQHWWPlotter() {
  // Destructor of TQHWWPlotter class:
}

//__________________________________________________________________________________|___________

bool TQHWWPlotter::setScheme(TString scheme) {
  // set the plotting scheme
  // this is legacy functionality, please use explicit calls to addProcess instead
  //@tag:[energy,luminosity] (legacy!) This argument tag sets the center-of-mass energy and integrated luminosity denoted on the plots. Default: "(?)".
  TString energy = this->getTagStringDefault("energy","(?)");
  TString lumi = this->getTagStringDefault("luminosity","(?)");

  if(this->fReader && this->fReader->getSampleFolder()){
    this->fReader->getSampleFolder()->setTagBool("wildcarded?",true,"sig/?");
    this->fReader->getSampleFolder()->setTagBool("wildcarded?",true,"bkg/?");
    this->fReader->getSampleFolder()->setTagBool("wildcarded?",true,"data/?");
  }

  if (scheme.CompareTo("hww.minimal.uniflavour") == 0){
    this->resetProcesses();
    this->addBackground("bkg");
    this->addData("data");
    this->addSignal("sig/mh$(mh)");
    return true;
  }
  if (scheme.CompareTo("hww.vbf") == 0){
    resetProcesses();

    addData("data/$(lepch)");
    addBackground("bkg/$(lepch)/diboson/WW");
    addBackground("bkg/$(lepch)/diboson/NonWW");
    addBackground("bkg/$(lepch)/top/ttbar");
    addBackground("bkg/$(lepch)/top/singletop");
    addBackground("bkg/$(lepch)/Zjets");
    addBackground("bkg/$(lepch)/Wjets");
    addBackground("sig/$(lepch)/mh$(mh)/ggf");
    addSignal("sig/$(lepch)/mh$(mh)/vbf");
    setTotalBkgSystematics("bkg/$(lepch)");

    clear();
    setTagBool("style.autoStack", true);
    setTagString("labels.0", "#sqrt{s} = "+energy+" TeV, #lower[-0.2]{#scale[0.6]{#int}} Ldt = "+lumi+" fb^{-1}");
    setTagString("labels.1", "H#rightarrowWW^{*}#rightarrowl#nul#nu");
    setTagBool("style.stackSignal", (scheme.CompareTo("hww.default.sigStacked") == 0));
    return true;
  }
  if (scheme.CompareTo("hww.default") == 0 ||
      scheme.CompareTo("hww.default.sigStacked") == 0) {

    resetProcesses();
    addData("data/$(lepch)");
    addBackground("bkg/$(lepch)/diboson/WW");
    addBackground("bkg/$(lepch)/diboson/NonWW");
    addBackground("bkg/$(lepch)/top/ttbar");
    addBackground("bkg/$(lepch)/top/singletop");
    addBackground("bkg/$(lepch)/Zjets");
    addBackground("bkg/$(lepch)/Wjets");
    addBackground("");
    addSignal("sig/$(lepch)/mh$(mh)");
    setTotalBkgSystematics("bkg/$(lepch)");

    clear();
    setTagBool("style.autoStack", true);
    setTagString("labels.0", "#sqrt{s} = "+energy+" TeV, #lower[-0.2]{#scale[0.6]{#int}} Ldt = "+lumi+" fb^{-1}");
    setTagString("labels.1", "H#rightarrowWW^{*}#rightarrowl#nul#nu");
    setTagBool("style.stackSignal", (scheme.CompareTo("hww.default.sigStacked") == 0));

    return true;
  } else if (scheme.CompareTo("hww.bkgsig") == 0 ||
             scheme.CompareTo("hww.bkgsig.normalize") == 0 ||
             scheme.CompareTo("hww.bkgggfvbf") == 0 ||
             scheme.CompareTo("hww.bkgggfvbf.normalize") == 0) {

    resetProcesses();
    addBackground("bkg/$(lepch)");
    if (scheme.Contains(".bkgsig")) {
      addSignal("sig/$(lepch)/mh$(mh)");
    } else if (scheme.Contains(".bkgggfvbf")) {
      addSignal("sig/$(lepch)/mh$(mh)/ggf");
      addSignal("sig/$(lepch)/mh$(mh)/vbf");
    }
    setTotalBkgSystematics("bkg/$(lepch)");

    clear();
    setTagBool("style.autoStack", true);
    setTagBool("style.showTotalBkg", false);
    setTagBool("normalize", scheme.EndsWith(".normalize"));
    setTagInteger("style.nLegendCols", 1);
    setTagDouble("style.legendHeight", 1.75);
    setTagString("labels.0", "#sqrt{s} = "+energy+" TeV, #lower[-0.2]{#scale[0.6]{#int}} Ldt = "+lumi+" fb^{-1}");
    setTagString("labels.1", "H#rightarrowWW^{*}#rightarrowl#nul#nu");

    return true;
  } else if (scheme.CompareTo("hww.databkgsig") == 0) {

    resetProcesses();
    addData("data/$(lepch)");
    addBackground("bkg/$(lepch)");
    addSignal("sig/$(lepch)/mh$(mh)");
    setTotalBkgSystematics("bkg/$(lepch)");

    clear();
    setTagBool("style.autoStack", true);
    setTagBool("style.showTotalBkg", false);
    setTagBool("style.stackSignal", true);
    setTagInteger("style.nLegendCols", 1);
    setTagDouble("style.legendHeight", 1.75);
    setTagString("labels.0", "#sqrt{s} = "+energy+" TeV, #lower[-0.2]{#scale[0.6]{#int}} Ldt = "+lumi+" fb^{-1}");
    setTagString("labels.1", "H#rightarrowWW^{*}#rightarrowl#nul#nu");

    return true;
  } else if (scheme.CompareTo("comparePlots") == 0) {
    setTagBool("normalize",true);
    setTagBool("style.showRatio",true);
    setTagBool("style.showSub",true);
    setTagString("style.stackDrawOptions", "ep");
    setTagBool("style.showTotalBkg", false);
    setTagBool("errors.showX", false);
    TQTaggableIterator itr(this->fProcesses);
    while(itr.hasNext()){
      TQNamedTaggable * process = itr.readNext();
      if(!process) continue;
      process->setTagBool(".ignoreProcessName",true);
      process->setTagInteger("histLineWidth", 2);
      process->setTagInteger("histMarkerStyle", 20);

      if(getNProcesses(".isData") < 2)
        process->setTagDouble( "histMarkerSize", 0.9);
      else if(getNProcesses(".isData") < 4)
        process->setTagDouble( "histMarkerSize", 0.8);
      else
        process->setTagDouble( "histMarkerSize", 0.7);
      //@tag:[color] (legacy!) The value of this process tag is copied to the process tags "histMarkerColor" and "histLineColor". Defaults are 0 and 1, respectively.
      process->setTagInteger("histMarkerColor", process->getTagIntegerDefault("color",0) );
      process->setTagInteger("histLineColor", process->getTagIntegerDefault("color",1) );
      if(process->getTagBoolDefault(".isBackground",false))
        {
          if(getNProcesses(".isBackground") == 1)
            //@tag:[title] (legacy!) This process tag is copied to the process tag "labels.totalStack" ("labels.data") if the process is the only background (data) process. Default: "Y" ("X")
            this->setTagString("labels.totalStack",process->getTagStringDefault("title","Y"));
          else
            setTagString("labels.totalStack","sum");
        }
      else if(process->getTagBoolDefault(".isData",false))
        {
          if(getNProcesses("isData") == 1)
            setTagString("labels.data",process->getTagStringDefault("title","X"));
          else
            setTagString("labels.data","X");
        }
    }
    return true;
  } else {
    return false;
  }
}


//__________________________________________________________________________________|___________

TCanvas * TQHWWPlotter::plotHistogram(TH1 * histo, const TString& options) {
  // plot a single histogram with the given options
  TQSampleFolder * sfTmp = TQSampleFolder::newSampleFolder("sfTmp");
  sfTmp->addObject(TQHistogramUtils::copyHistogram(histo), ".histograms+::histo");

  // parse options
  TQTaggable tags(options);

  // initialize instance of plotter
  TQHWWPlotter pl(sfTmp);
  pl.resetProcesses();
  pl.addData(".", TString::Format("drawOptions = '%s'",
                                  //@tag:[drawOptions] This argument tag is forwarded by TQHWWPlotter::plotHistogram to TQPlotter::addData. Default: "hist".
                                  tags.getTagStringDefault("drawOptions", "hist").Data()));
  tags.removeTag("drawOptions");
  pl.importTags(&tags);

  // plot
  TCanvas * cv = pl.plot("histo");

  delete sfTmp;
  return cv;
}


//__________________________________________________________________________________|___________

TCanvas * TQHWWPlotter::plotHistograms(TH1 * data, TH1* mc, const TString& options) {
  // plot a pair of histograms histograms (data and MC) with the given options
  // set individual styling options with "histstyles.data.XXX" and "histstyles.mc.XXX"

  TQTaggable tags(options);
  TQSampleFolder * sfTmp = TQSampleFolder::newSampleFolder("sfTmp");

  TQSampleFolder* dataSF = sfTmp->getSampleFolder("data+");
  TQTaggable dataTags;
  //@tag: [histstyles.data.,histstyles.mc.; style.default.] The first two argument tag prefixes are used to apply tags either to plotting of data or plotting of MC in TQHWWPlotter::plotHistograms. Only tags continuing with "style.default." are then actually used.
  dataTags.importTagsWithoutPrefix(tags,"histstyles.data.");
  dataSF->importTagsWithPrefix(dataTags,"style.default.");
  dataSF->addObject(TQHistogramUtils::copyHistogram(data), ".histograms+::histogram");

  TQSampleFolder* mcSF = sfTmp->getSampleFolder("mc+");
  TQTaggable mcTags;
  mcTags.importTagsWithoutPrefix(tags,"histstyles.mc.");
  mcSF->importTagsWithPrefix(mcTags,"style.default.");
  mcSF->addObject(TQHistogramUtils::copyHistogram(mc), ".histograms+::histogram");

  TQHWWPlotter pl(sfTmp);
  pl.resetProcesses();
  pl.addData ("data", TString::Format("title='%s'",data->GetTitle()));
  pl.addBackground("mc", TString::Format("title='%s'",mc->GetTitle()));

  TCanvas * cv = pl.plot("histogram", options);

  delete sfTmp;
  return cv;
}



//__________________________________________________________________________________|___________

bool TQHWWPlotter::plotAndSaveHistogram(TH1 * histo, const TString& saveAs, const TString& options) {
  // quick-access-function to plot one histogram and save it
  TCanvas * cv = TQHWWPlotter::plotHistogram(histo, options);
  if (cv) {
    cv->SaveAs(saveAs.Data());
    delete cv;
    return true;
  } else {
    return false;
  }
  delete cv;
}

//__________________________________________________________________________________|___________

bool TQHWWPlotter::plotAndSaveHistograms(TH1 * data, TH1* mc, const TString& saveAs, const TString& options) {
  // quick-access-function to plot two histograms and save them
  TCanvas * cv = TQHWWPlotter::plotHistograms(data,mc, options);
  if (cv) {
    cv->SaveAs(saveAs.Data());
    delete cv;
    return true;
  } else {
    return false;
  }
  delete cv;
}

//__________________________________________________________________________________|___________


void TQHWWPlotter::setStyleIllinois(TQTaggable& tags){
  // setup the "new" (Illinois) style tags
  double padScaling = 1.;
  double ratioPadScaling = 1.;
  double additionalTopMargin = 0.;
  //@tag: [geometry.sub.height] This argument tag (TQHWWPlotter::setStyleIllinois) controls the height of the sub plot (e.g. ratio plot)
  double ratioPadRatio = tags.getTagDoubleDefault("geometry.sub.height",0.35);
  //@tag:[style.titleOffset] This argument tag (TQHWWPlotter::setStyleIllinois) controls the offset of axis lables. Default: 1.
  double titleOffset = tags.getTagDoubleDefault("style.titleOffset",1);
  //@tag:[style.textSize]
  double textsize = tags.getTagDoubleDefault("style.textSize",DEFAULTTEXTSIZE);
  //@tag:[style.showSub] This argument tag controls if a sub plot (e.g. ratio plot) is shown or not. Default: false.
  bool showSub = tags.getTagBoolDefault ("style.showSub",false);
  //@tag:[geometry.legendPadRatio] This argument tag (TQHWWPlotter::setStyleIllinois) controls the width of the legend on plots. Default: 0.25
  double legend_divider_x = 1. - tags.getTagDoubleDefault("geoemetry.legendPadRatio",0.25);

  double geometry_left_margin = 0.20;

  tags.setTagDouble("geometry.main.margins.right", 0.05);
  tags.setTagDouble("geometry.main.margins.left", geometry_left_margin);
  tags.setTagDouble("geometry.main.margins.top", textsize * 1.6 * padScaling);

  tags.setTagDouble("geometry.main.xMin",0.);
  tags.setTagDouble("geometry.main.xMax",legend_divider_x);
  tags.setTagDouble("geometry.main.yMax",1.);

  tags.setTagInteger("style.tickx",0);
  tags.setTagInteger("style.ticky",0);

  //tags.setTagDouble("style.legendHeight",0.35 * (showSub ? 1.0 : 1.1));
  tags.setTagDouble("style.legendHeight",0.33 * (showSub ? 1.0 : 1.1));
  tags.setTagDouble("geometry.legend.margins.right", 0.05);
  tags.setTagDouble("geometry.legend.margins.top", textsize * 1.6 * padScaling);
  tags.setTagDouble("geometry.legend.margins.left",0.16);
  tags.setTagDouble("geometry.legend.margins.bottom", 0.);

  tags.setTagInteger("geometry.canvas.height",600);
  if (showSub){
    tags.setTagInteger("geometry.canvas.width",700);
    padScaling = 1. / (1. - ratioPadRatio) / 8. * 6.;
    ratioPadScaling = (1. / ratioPadRatio) / 8. * 6.;
    double legendPadScaling = 1.;//1/legend_divider_x;
    additionalTopMargin = 0.1 * (padScaling - 1);
    tags.setTagDouble("geometry.sub.margins.top", 0.);
    tags.setTagDouble("geometry.sub.margins.bottom",0.16);
    tags.setTagDouble("geometry.sub.margins.left", geometry_left_margin);
    tags.setTagDouble("geometry.sub.margins.right",0.05);

    tags.setTagDouble("geometry.main.additionalTopMargin",additionalTopMargin);
    tags.setTagDouble("geometry.main.scaling",padScaling);
    tags.setTagDouble("geometry.sub.scaling",ratioPadScaling);
    tags.setTagDouble("geometry.xscaling",legendPadScaling);

    tags.setTagDouble("geometry.sub.xMin",0.);
    tags.setTagDouble("geometry.sub.yMin",0.);
    tags.setTagDouble("geometry.sub.xMax",legend_divider_x);
    tags.setTagDouble("geometry.sub.yMax",ratioPadRatio);

    tags.setTagDouble("geometry.main.yMin",ratioPadRatio);
    tags.setTagDouble("geometry.main.margins.bottom", 0.);

    tags.setTagBool("style.main.xAxis.showLabels",false);
    tags.setTagBool("style.main.xAxis.showTitle",false);
  } else {
    tags.setTagInteger("geometry.canvas.width",800);
    tags.setTagDouble("geometry.main.margins.bottom", 0.18);
    tags.setTagDouble("geometry.main.yMin",0.);
  }

  tags.setTagDouble("geometry.legend.xMin",legend_divider_x);
  tags.setTagDouble("geometry.legend.yMin", 0.);
  tags.setTagDouble("geometry.legend.xMax",1.);
  tags.setTagDouble("geometry.legend.yMax",1. - (1. - (showSub?ratioPadRatio:0)) *tags.getTagDoubleDefault("geometry.main.margins.top",0.3));

  tags.setTagInteger("style.text.font",42);
  tags.setTagDouble("style.textSize",DEFAULTTEXTSIZE);
	// tags.setTagDouble("style.markerSize",1.2);
  tags.setTagInteger("style.markerType",20);
  tags.setTagBool("errors.showX", false);
  //@tag: [style.showEventYields] If this argument tag is set to true, integrated event yields for each process are shown in the legend.
  if (tags.getTagBoolDefault ("style.showEventYields",false))
    tags.setTagDouble("legend.textSize",0.080);

  tags.setTagBool("style.useLegendPad",true);
  tags.setTagDouble("style.logMin",0.011);
  tags.setTagDouble("legend.margin.right",-0.2);

  tags.setTagDouble("geometry.main.xAxis.titleOffset",1.2*titleOffset);
  tags.setTagDouble("geometry.main.xAxis.labelOffset",0.03);
  tags.setTagDouble("geometry.main.xAxis.titleSize",showSub ? 0. : textsize*1.2);
  tags.setTagDouble("geometry.main.xAxis.labelSize",showSub ? 0. : textsize);
  tags.setTagDouble("geometry.main.yAxis.titleOffset",(showSub? 1.5:1.7)*titleOffset);
  tags.setTagDouble("geometry.main.yAxis.labelOffset",0.03);
  tags.setTagDouble("geometry.main.yAxis.titleSize",textsize*1.2);
  tags.setTagDouble("geometry.main.yAxis.labelSize",textsize);

  if(showSub){
    tags.setTagDouble("geometry.sub.xAxis.titleOffset",1.1*titleOffset);
    tags.setTagDouble("geometry.sub.xAxis.labelOffset",0.04);
    tags.setTagDouble("geometry.sub.xAxis.titleSize",textsize*1.2);
    tags.setTagDouble("geometry.sub.xAxis.labelSize",textsize);
    tags.setTagDouble("geometry.sub.yAxis.titleOffset",1.5*titleOffset);
    tags.setTagDouble("geometry.sub.yAxis.labelOffset",0.03);
    tags.setTagDouble("geometry.sub.yAxis.titleSize",textsize*1.2);
    tags.setTagDouble("geometry.sub.yAxis.labelSize",textsize);
  }

  double tickLength = tags.getTagDoubleDefault("style.tickLength",0.02);
  tags.setTagDouble("geometry.main.yAxis.tickLength",-tickLength);
  tags.setTagDouble("geometry.main.xAxis.tickLength",-tickLength);
  tags.setTagDouble("geometry.sub.yAxis.tickLength", -tickLength);
  tags.setTagDouble("geometry.sub.xAxis.tickLength",-tickLength);

  tags.setTagInteger("style.main.xAxis.nDiv",50008);
  tags.setTagInteger("style.main.yAxis.nDiv",50004);

  tags.setTagInteger("style.main.lineColor",0);
  tags.setTagInteger("style.main.markerColor",0);

  tags.setTagInteger("style.ratio.mcErrorBand.fillColor",14);
  tags.setTagInteger("style.ratio.mcErrorBand.fillStyle",3254);

  tags.setTagInteger("style.significance.fillColor",kRed);
  tags.setTagInteger("style.significance.fillStyle",3254);
  tags.setTagInteger("style.significance.lineColor",0);
  tags.setTagInteger("style.significance.lineStyle",0);

  tags.setTagInteger("style.main.data.lineWidth",2);
  tags.setTagInteger("style.main.data.lineColor",kBlack);
  // tags.setTagDouble ("style.main.data.markerSize",1.0);

  tags.setTagInteger("style.main.totalStack.fillColor",0);
  tags.setTagInteger("style.main.totalStack.fillStyle",0);
  tags.setTagInteger("style.main.totalStackError.fillColor",14);
  tags.setTagInteger("style.main.totalStackError.fillStyle",3254);

  tags.setTagInteger("style.sub.xAxis.nDiv",50008);

  tags.setTagInteger("style.nLegendCols",1);

  tags.setTagDouble("legend.xMin",0.0);
  tags.setTagDouble("legend.yMin",0.0);
  tags.setTagDouble("legend.xMax",1.0);
  tags.setTagDouble("legend.yMax",1.0);

  tags.setTagInteger("labels.info.align",11);
  tags.setTagDouble("labels.info.size",0.8);
  tags.setTagDouble("labels.info.xPos",0.);
  tags.setTagDouble("labels.atlas.scale",1.);
  if (showSub) // the following controls the xoffset of "internal" in ATLAS "internal".
    tags.setTagDouble("labels.atlas.xOffset",0.16);
  else
    tags.setTagDouble("labels.atlas.xOffset",0.20);
  tags.setTagDouble("labels.drawATLAS.scale",1.21);
  tags.setTagDouble("style.labels.scale",1.1);
  tags.setTagDouble("style.labels.xOffset",geometry_left_margin+0.015);
  tags.setTagBool("style.legend.textSizeFixed",true);
  tags.setTagDouble("legend.textSize",0.132);

  tags.setTagBool("legend.showTotalBkgErrorType",false);
  tags.setTagString("legend.dataDisplayType","ex1y2p");
  tags.setTagBool("style.manualStacking",false);
  tags.setTagBool("style.autoStackLegend",true);

  double yVetoLeft = 0.70;
  double yVetoRight = 0.70;
  //@tag: [style.logScale] If this argument tag is set to true, the y axis is shown in log scale. Default: false.
  if (tags.getTagBoolDefault ("style.logScale",false )) {
    yVetoLeft -= 0.15;
    yVetoRight -= 0.15;
  }
  tags.setTagDouble("blocks.x.0",0.5); tags.setTagDouble("blocks.y.0",yVetoLeft);
  tags.setTagDouble("blocks.x.1",1.0); tags.setTagDouble("blocks.y.1",yVetoRight);
}

//__________________________________________________________________________________|___________

void TQHWWPlotter::setStyle(TQTaggable& tags){
  //this method is still in use
  // setup the default style tags
  double padScaling = 1.;
  double ratioPadScaling = 1.;
  double additionalTopMargin = 0.;
  //tag doc see setStyleIllinois
  bool showSub = tags.getTagBoolDefault("style.showSub",false);
  double ratioPadRatio = tags.getTagDoubleDefault("geometry.sub.height",0.35);
  //@tag: [style.topLegend] If this argument tag is set to true, the legend is shown at the top of the plot. Default: false.
  if(tags.getTagBoolDefault("style.topLegend",false)){
    tags.setTagDouble("geometry.main.margins.top",0.25);
    tags.setTagInteger("style.legend.fillColor",kWhite);
    tags.setTagInteger("style.legend.fillStyle",1001);
    tags.setTagDouble("labels.info.yPos",0.9);
    tags.setTagDouble("style.labels.yPos",0.9);
    tags.setTagBool("style.showMissing",false);
    tags.setTagDouble("legend.yMin",0.75);
    tags.setTagDouble("legend.yMax",0.95);
  } else {
    tags.setTagDouble("geometry.main.margins.top", 0.05);
    tags.setTagDouble("geometry.legend.margins.top", 0.05);
  }
  if((tags.hasTag("style.heatmap") || tags.hasTag("style.migration"))&& tags.getTagIntegerDefault("style.nDim",1) == 2){ //only change the margin if it's an actual 2D plot.
    tags.setTagDouble("geometry.main.margins.right", 0.12);
  } else {
    tags.setTagDouble("geometry.main.margins.right", 0.05);
  }
  tags.setTagDouble("geometry.main.margins.left", 0.16);

  tags.setTagDouble("geometry.main.xMin",0.);
  tags.setTagDouble("geometry.main.xMax",1.);
  tags.setTagDouble("geometry.main.yMax",1.);

  tags.setTagInteger("geometry.canvas.height",600);
  if (showSub){
    //@tag: [style.ratioTopMargin] This argument tag sets the top margin for ratio plots. Default: 0.015
    tags.setTagDouble("geometry.sub.margins.top", tags.getTagDoubleDefault("style.ratioTopMargin",0.015));
    tags.setTagDouble("geometry.sub.margins.bottom",0.16);
    tags.setTagDouble("geometry.sub.margins.left", 0.16);
    tags.setTagDouble("geometry.sub.margins.right", 0.05);
    tags.setTagInteger("geometry.canvas.width",600);
    double height = tags.getTagDoubleDefault("geometry.canvas.height",0);
    double width = tags.getTagDoubleDefault("geometry.canvas.width",0);
    padScaling = width / height;
    ratioPadScaling =  (height > width ? padScaling : 1.) / ratioPadRatio;
    additionalTopMargin = 0.1 * (padScaling - 1);
    tags.setTagDouble("geometry.main.additionalTopMargin",additionalTopMargin);
    tags.setTagDouble("geometry.main.scaling",padScaling);
    tags.setTagDouble("geometry.sub.scaling",ratioPadScaling);

    tags.setTagDouble("geometry.sub.xMin",0.);
    tags.setTagDouble("geometry.sub.yMin",0.);
    tags.setTagDouble("geometry.sub.xMax",1.);
    tags.setTagDouble("geometry.sub.yMax",ratioPadRatio);
    tags.setTagDouble("geometry.main.yMin",0);
    tags.setTagDouble("geometry.main.margins.bottom",ratioPadRatio);
    tags.setTagDouble("geometry.legend.margins.bottom",ratioPadRatio);

    tags.setTagBool("style.main.xAxis.showLabels",false);
    tags.setTagBool("style.main.xAxis.showTitle",false);
  } else {
    tags.setTagDouble("geometry.main.margins.bottom",0.16);
    tags.setTagDouble("geometry.legend.margins.bottom",0.16);
    tags.setTagDouble("geometry.main.yMin",0.);
    tags.setTagInteger("geometry.canvas.width",800);
  }

  tags.setTagInteger("style.text.font",42);
  tags.setTagDouble("style.textSize",DEFAULTTEXTSIZE);
  // tags.setTagDouble("style.markerSize",1.2);
  tags.setTagInteger("style.markerType",20);

  tags.setTagInteger("style.main.lineColor",0);
  tags.setTagInteger("style.main.markerColor",0);
  tags.setTagInteger("style.main.fillColor",0);
  tags.setTagInteger("style.main.fillStyle",0);

  tags.setTagInteger("style.main.totalStack.lineColor",kBlue);
  tags.setTagInteger("style.main.totalStack.lineWidth",1);
  tags.setTagInteger("style.main.totalStack.fillColor",0);
  tags.setTagInteger("style.main.totalStack.fillStyle",0);
  tags.setTagInteger("style.main.totalStackError.fillColor",14);
  tags.setTagInteger("style.main.totalStackError.fillStyle",3254);
  tags.setTagInteger("style.main.totalStackError.lineStyle",0);

  tags.setTagInteger("style.ratio.mcErrorBand.fillColor",kOrange -2);
  tags.setTagInteger("style.ratio.mcErrorBand.fillStyle",1001);
  tags.setTagInteger("style.optScan.default.fillColor",kOrange -2);
  tags.setTagInteger("style.optScan.default.fillStyle",1001);
  tags.setTagInteger("style.optScan.left.fillColor",kRed);
  tags.setTagInteger("style.optScan.left.fillStyle",1001);
  tags.setTagInteger("style.optScan.right.fillColor",kBlue);
  tags.setTagInteger("style.optScan.right.fillStyle",1001);

  tags.setTagInteger("style.main.data.lineWidth",2);
	tags.setTagDouble ("style.main.data.markerStyle",20);
	if (tags.hasTag("style.markerSize") && !tags.hasTag("style.main.data.markerSize")) {
		tags.setTagDouble("style.main.data.markerSize",tags.getTagDoubleDefault("style.markerSize", 1.));
	}

  // tags.setTagInteger("style.main.data.lineColor",kBlack);

  tags.setTagInteger("style.significance.fillColor",kRed);
  tags.setTagInteger("style.significance.fillStyle",1001);
  tags.setTagInteger("style.significance.lineColor",0);
  tags.setTagInteger("style.significance.lineStyle",0);

  //@tag:[style.tickLength] This argument tag controls the length of the x- and y-axis ticks. Default: 0.03
  double tickLength = tags.getTagDoubleDefault("style.tickLength",0.03);
  tags.setTagDouble("geometry.main.yAxis.tickLength",tickLength);
  tags.setTagDouble("geometry.main.xAxis.tickLength",tickLength);
  tags.setTagDouble("geometry.sub.yAxis.tickLength", tickLength);
  tags.setTagDouble("geometry.sub.xAxis.tickLength",tickLength);

  //@tag:[style.sub.yAxis.nDiv, style.ratio.nYdiv] This tag controls the number of divisions/ticks of the sub plot. The number of top level ticks is given by the two least significant digits (in decimal notation). The second two least significant digits determine the number of sub divisions (smaller ticks), the thrid least significant set of two digits controls the sub-sub-devisions. When "setStyle" is called, the first tag (...yAxis.nDiv) defaults to the value determined from latter tag (...nYdiv) or 510 (10 top level divisions, 5 sub divisions) if the latter one is not set.
  tags.setTagInteger("style.sub.yAxis.nDiv",tags.getTagIntegerDefault("style.ratio.nYdiv",510));
  tags.setTagInteger("style.sub.xAxis.nDiv", tags.getTagIntegerDefault("style.ratio.nXdiv", 510));

  tags.setTagDouble("legend.xMin",0.59);
  tags.setTagDouble("legend.xMax",0.90);
  tags.setTagDouble("legend.yMin",0.70);
  tags.setTagDouble("legend.yMax",0.92);

  tags.setTagBool("errors.showX",true);
  tags.setTagDouble("erros.widthX",0.5);

  tags.setTagDouble("blocks.x.0",0.5);
  tags.setTagDouble("blocks.x.1",1.0);
  if(tags.getTagBoolDefault("style.showLabels",true)){
    double yVetoLeft = 0.84;
    TList* labels = tags.getTagList("labels");
    if (labels) yVetoLeft -= (showSub ? 0.08 : 0.09) * (double)labels->GetEntries() * tags.getTagDoubleDefault("geometry.main.scaling",1.);
    delete labels;
    tags.setTagDouble("blocks.y.0",yVetoLeft);
  } else {
    tags.setTagDouble("blocks.y.0",1);
  }
  if(!tags.getTagBoolDefault("style.useLegendPad",false)){
    double yVetoRight = tags.getTagDoubleDefault("legend.yMin",0.5) - tags.getTagDoubleDefault("legend.margin.right",0.05);
    tags.setTagDouble("blocks.y.1",yVetoRight);
  } else {
    tags.setTagDouble("blocks.y.1",1);
  }
}

//__________________________________________________________________________________|___________

bool TQHWWPlotter::includeSystematicsLegacy(TQTaggable& tags,TQTaggable& aliases){
  // legacy function for systematics handling
  // the new version is to be found in the TQPlotter base class

  //@tag:[verbose] This argument tag enables verbosity. Default: false.
  bool verbose = tags.getTagBoolDefault("verbose",false);
  if(verbose) VERBOSEclass("collecting uncertainty band data");

  // get the systematic uncertainty histogram
  TH1 * hTotalStackSys = 0;
  //@tag: [errors.drawSysMC,errors.drawAsymmSysMC] These argument tags enable drawing of (asymmetric) systematic uncertainties for MC. Default: false
  bool sysMcErrors = tags.getTagBoolDefault("errors.drawSysMC",false );
  bool asymmSysMcErrors = tags.getTagBoolDefault("errors.drawAsymmSysMC", false);

  if (sysMcErrors && !asymmSysMcErrors) {
    //@tag: [input.bkg] This argument tag specifies the path of the total background. Default: "bkg"
    TObjArray * histoSystematics = getHistograms(this->fProcesses,"isTotalBkgSys", tags.getTagStringDefault("input.bkg", "bkg"),".systematics/", aliases, tags);
    if (histoSystematics && histoSystematics->GetEntries() > 0){
      hTotalStackSys = (TH1*)histoSystematics->First();
      this->addObject(hTotalStackSys,"totalStackSys");
    }
  } else {
    TObjArray * histoSystematicsAsymm = 0;
    if (asymmSysMcErrors) {
      TObjArray * procSystematicsAsymm = new TObjArray();
      procSystematicsAsymm->SetOwner(true);
      TQTaggableIterator sysItr(this->fProcesses);
      TObjArray* processes = NULL;
      //@tag: [errors.drawAsymmSysList] This argument tag contains a comma seperated list of systematic uncertainties.
      TObjArray* systList = (tags.getTagStringDefault("errors.drawAsymmSysList", "")).Tokenize(",");
      while(!processes && sysItr.hasNext()){
        TQTaggable* process = sysItr.readNext();
        if(!process) continue;
        //@tag: [isTotalBkgSys] This process tag labels the process as the "process" representing the total systematic uncertainties.
        if(!process->getTagBoolDefault("isTotalBkgSys")) continue;
        //@tag: [.path] This process tag contains the path(s) (possibly including path arithmetics) of the corresponding sample folder(s).
        processes = process->getTagStringDefault(".path").Tokenize("+");
      }
      for (int iSys = 0; iSys < systList->GetEntries(); iSys++) {
        TString entry = "";
        for (int iProc = 0; iProc < processes->GetEntries(); iProc++) {
          TString process = TQStringUtils::trim(processes->At(iProc)->GetName());
          TString syst = TQStringUtils::trim(systList->At(iSys)->GetName());
          if (iProc)
            entry += "+" + process + "/" + syst;
          else
            entry = process + "/" + syst;
        }
        TQNamedTaggable* tmp = new TQNamedTaggable();
        tmp->setTagString(".path",entry);
        tmp->setTagBool("isBkgSys",true);
        procSystematicsAsymm->Add(tmp);

      }
      histoSystematicsAsymm = getHistograms(processes, "isBkgSys", tags.getTagStringDefault("input.bkg", "bkg"), ".asymmsystematics/", aliases, tags);
      delete procSystematicsAsymm;
      delete processes;
      delete systList;
    }
    this->addObject(histoSystematicsAsymm,"asymmSys");
  }

  return true;
}

//__________________________________________________________________________________|___________

bool TQHWWPlotter::setTotalBkgSystematics(const TString& path) {
  // set the total background systematics to be retrieved from the given path
  TQNamedTaggable* totalStackSys = new TQNamedTaggable("totalStackSys");
  totalStackSys->setTagBool("isTotalBkgSys",true);
  totalStackSys->setTagString(".path",path);
  this->fProcesses->Add(totalStackSys);
  return true;
}

//__________________________________________________________________________________|___________

TObjArray* TQHWWPlotter::collectHistograms(TQTaggable& tags){
  // use the TQSampleDataReader to retrieve all histograms from the sample folder

  //@tag: [style.showUnderflow,style.showOverflow] This argument tag controls if under/overflow bins are shown in the histogram. Default: false.
  bool showUnderflow = tags.getTagBoolDefault ("style.showUnderflow",false);
  bool showOverflow = tags.getTagBoolDefault ("style.showOverflow",false );
  tags.setTagBool("includeOverflow",showOverflow);
  tags.setTagBool("includeUnderflow",showUnderflow);
  //@tag: [input.mh] (HWW legacy) This argument tag defines the higgs-boson mass. Default: 125
  int mh = tags.getTagIntegerDefault ("input.mh", 125);
  bool verbose = tags.getTagBoolDefault("verbose",false );
  //tags.setTagBool("norm",normalize); //this is problematic! this causes every histogram individually to be normalized in the SampleDataReader (which changes the shape of the histogram stack and is very misleading) TODO: remove this line after some checks/time

  // get the histograms
  TQTaggable aliases;
  //@tag: [input.lepch,input.channel] These argument tags do something (TODO)
  aliases.setTagString("lepch",tags.getTagStringDefault("input.lepch","?")); //in case it's not specified, we set a default in this way
  aliases.setTagString("channel",tags.getTagStringDefault("input.channel","?"));
  aliases.setTagString("datachannel",tags.getTagStringDefault("input.datachannel","?"));
  aliases.setTagInteger("mh",mh);
  aliases.importTagsWithoutPrefix(tags,"alias.");//import alias and input tags
  aliases.importTagsWithoutPrefix(tags,"input.");

  if(verbose) VERBOSEclass("getting data histograms");
  TObjArray* histosData = getHistograms(this->fProcesses,".isData", tags.getTagStringDefault("input.data", "histogram"), "", aliases, tags);
  if(verbose) VERBOSEclass("getting background histograms");
  TObjArray* histosBkg = getHistograms(this->fProcesses,".isBackground", tags.getTagStringDefault("input.bkg", "histogram"), "", aliases, tags);
  if(verbose) VERBOSEclass("getting signal histograms");
  TObjArray* histosSig = getHistograms(this->fProcesses,".isSignal", tags.getTagStringDefault("input.sig", "histogram"), "", aliases, tags);

  TObjArray* histos = new TObjArray();

  histos->AddAll(histosData);
  histos->AddAll(histosBkg);
  histos->AddAll(histosSig);
  if(histos->GetEntries() < 1){
    delete histos;
    ERRORclass("no histograms found: "+tags.exportTagsAsString("input.*"));
    return NULL;
  }
  if(!histosData || histosData->GetEntries() < 1){
    if(verbose) VERBOSEclass("no data histograms found, disabling data");
    tags.setTagBool("style.drawData",false);
  } else {
    if(verbose) VERBOSEclass("found %d data histograms",histosData->GetEntries());
  }
  
  this->applyBlinding(tags,histosSig, histosBkg, histosData); 
  
  if(histosData) delete histosData;
  if(histosBkg) delete histosBkg;
  if(histosSig) delete histosSig;

  TQTH1Iterator itr(histos);
  double maxint = 0;
  while(itr.hasNext()){
    TH1* hist = itr.readNext();
    double integral = hist->Integral();
    maxint = std::max(integral,maxint);
  }
  if(verbose) VERBOSEclass("highest integral of histogram set is %g",maxint);
  if( maxint < tags.getTagDoubleDefault("skipEmptyHistograms",1e-5) && !tags.getTagDoubleDefault("skipEmptyHistograms",1e-5) == 0 ){  //@tag: [skipEmptyHistograms] Skip histograms with integral below given value. This is set to 1e-5 by default!
    delete histos;
    return NULL;
  }

  //////////////////////////////////////////////////////
  // check the consistency
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("checking histogram consistency");
  // expect at least one histogram to be available
  bool consistent = (histos->GetEntries() > 0);
  // check consistency and create master histogram
  TH1* hMaster = NULL;
  consistent = checkConsistency(hMaster, histos) && consistent;

  // stop if there is no valid histogram or histograms are invalid
  if (!consistent){
    if(verbose) VERBOSEclass("consistency check failed");
    delete histos;
    return NULL;
  }
  hMaster->Reset();
  this->addObject(histos,"histos");

  tags.setTagInteger("style.nDim",TQHistogramUtils::getDimension(hMaster));

  //////////////////////////////////////////////////////
  // initialize background histogram
  //////////////////////////////////////////////////////

  // prepare the total background histogram
  if(verbose) VERBOSEclass("preparing total background histogram");
  TH1* hTotalStack = TQHistogramUtils::copyHistogram(hMaster,"totalStack");
  hTotalStack->SetTitle(tags.getTagStringDefault ("labels.totalStack", "SM"));
  TH1* hTotalSig = TQHistogramUtils::copyHistogram(hMaster,"totalSig");
  hTotalSig->SetTitle(tags.getTagStringDefault ("labels.totalSig", "H#rightarrowWW#rightarrowl#nul#nu"));

  // reset the histogram, because it is a clone of any of the subprocess
  // histograms and we are only interested in the the binning. Reset() resets
  // bin content and errors.
  hTotalStack->Reset();
  hTotalSig->Reset();

  // sum all background contributions by hand and (in the same step) check
  // whether there is any background process to be plotted. If so, the SM
  // background histogram will be shown (including the entry in the legend).
  bool hasTotalBkg = false;
  TString stackFilter = tags.getTagStringDefault ("style.stackFilter","" );
  TQTaggableIterator itr_bkg(this->fProcesses);
  while(itr_bkg.hasNext()){
    TQNamedTaggable* process = itr_bkg.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(".isBackground",false)) continue;
    TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    if(!h) continue;
    if (!stackFilter.IsNull()) {
      TString title = h->GetTitle();
      if (!stackFilter.Contains(title))
        continue;
    }
    hTotalStack->Add(h);
    hasTotalBkg = true;
  }

  TH1* hTotalBkg = NULL;
  if(hTotalStack){
    hTotalBkg = TQHistogramUtils::copyHistogram(hTotalStack,"totalBkg");
    hTotalBkg->SetDirectory(NULL);
    this->addObject(hTotalBkg);
  }

  // If stacking signal, add it to total background so that it shows up as a
  // part of the ratio
  bool hasTotalSig = false;
  bool stackSignal = tags.getTagBoolDefault ("style.stackSignal",false);
  if (hTotalStack) {
    TQTaggableIterator itr_sig(this->fProcesses);
    while(itr_sig.hasNext()){
      TQNamedTaggable* process = itr_sig.readNext();
      if(!process) continue;
      if(!process->getTagBoolDefault(".isSignal",false)) continue;
      TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
      if(!h) continue;
      if(process->getTagBoolDefault("stack",stackSignal)){
        hTotalStack->Add(h);
        hasTotalSig = true;
      } else {
        hTotalSig->Add(h);
        hasTotalSig = true;
      }
    }
  }

  if(hTotalStack && !hasTotalBkg){
    this->removeObject("totalStack",true);
  }

  if(hTotalSig && !hasTotalSig){
    this->removeObject("totalSig",true);
  }

  //////////////////////////////////////////////////////
  // systematics handling
  //////////////////////////////////////////////////////

  if(tags.hasTag("errors.totalBkgSys")){
    tags.setTagString("errors.showSys",tags.getTagStringDefault("errors.totalBkgSys",""));
  }

  if(tags.getTagBoolDefault("errors.drawSysMC",false ) || tags.getTagBoolDefault("errors.drawAsymmSysMC", false)){
    // the "old" way
    this->includeSystematicsLegacy(tags,aliases);
  }
  if(tags.hasTag("errors.showSys")){
    // the "new" way
    this->sysOk = this->includeSystematics(tags);
  }

  //////////////////////////////////////////////////////
  // rebinning options
  //////////////////////////////////////////////////////

  if(this->getNProcesses(".isData") == 0 || !hasTotalBkg) tags.setTagBool("style.showSub",false);

  return histos;
}

//__________________________________________________________________________________|___________

void TQHWWPlotter::stackHistograms(TQTaggable& tags, const TString& stackname){
  // create the histogram stack
  //@tag: normalize: TODO what exactly does this tag do? Default: false
  bool normalize = tags.getTagBoolDefault("normalize",false );
  //@tag:normalizeWithoutOverUnderflow: disable using underflow and overflow for normalization purposes
  bool normalizeWithoutOverUnderflow = !tags.getTagBoolDefault("normalizeWithoutOverUnderflow",false );
  TString stackFilter = tags.getTagStringDefault ("style.stackFilter","");
  TH1* hTotalStack = this->getObject<TH1>("totalStack");

  // scale to normalize total background
  double totalStackScale = TQHistogramUtils::getIntegral(hTotalStack, normalizeWithoutOverUnderflow);
  if (hTotalStack && normalize) {
    if (totalStackScale != 0.)
      hTotalStack->Scale(1. / totalStackScale);
  }

  // the list of histograms to be stacked
  TObjArray * histStackList = new TObjArray();
  TObjArray * processStackList = new TObjArray();
  TQTaggableIterator itr(this->fProcesses);
  while(itr.hasNext()){
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(    (process->getTagBoolDefault(".isBackground",false) && process->getTagBoolDefault("stack", true))   // background
        || (process->getTagBoolDefault(".isSignal",false) &&  tags.getTagBoolDefault ("style.autoStackSignal",false)) // signal
           ){
      TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
      if (!h) continue;
      if (!stackFilter.IsNull()) {
        TString title = h->GetTitle();
        if (!stackFilter.Contains(title))
          continue;
      }
      if(totalStackScale > 0 && normalize){
        h->Scale(1. / totalStackScale);
      }
      histStackList->Add(h);
      processStackList->Add(process);
    } else if (totalStackScale > 0 && normalize) {
      //also normalize non-stacked histograms such that their integral matches the normalized stack ("totalBackground")
      TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
      if (TQHistogramUtils::getIntegral(h, normalizeWithoutOverUnderflow) > 0) h->Scale(1/TQHistogramUtils::getIntegral(h, normalizeWithoutOverUnderflow));
    }
  }

  // sort the histograms to be stacked by ascending integral (sum of weights)
  if (!tags.getTagBoolDefault("style.manualStacking",false) && tags.getTagBoolDefault("style.autoStack",tags.getTagBoolDefault("style.logScale",false))){
    for (int iHist = 0; iHist < histStackList->GetEntries(); iHist++) {
      int iHistMin = iHist;
      double intMin = ((TH1*)histStackList->At(iHistMin))->GetSumOfWeights();
      for (int iHist2 = iHist + 1; iHist2 < histStackList->GetEntries(); iHist2++) {
        double intMin2 = ((TH1*)histStackList->At(iHist2))->GetSumOfWeights();
        if (intMin2 < intMin) {
          iHistMin = iHist2;
          intMin = intMin2;
        }
      }
      if (iHistMin != iHist) {
        TH1 * temp = (TH1*)(*histStackList)[iHist];
        (*histStackList)[iHist] = (*histStackList)[iHistMin];
        (*histStackList)[iHistMin] = temp;
        TQNamedTaggable * temptag = (TQNamedTaggable*)(*processStackList)[iHist];
        (*processStackList)[iHist] = (*processStackList)[iHistMin];
        (*processStackList)[iHistMin] = temptag;
      }
    }
  }

  // create the stack
  THStack * stack = new THStack(stackname, tags.getTagBoolDefault("style.stackSignal",false) ? "Signal+Background Stack" : "Background Stack");

  // add the histograms to the stack (following the order in the histStackList)
  if (tags.getTagBoolDefault ("style.reverseStacking",false )) {
    for (int iHist = histStackList->GetEntries(); iHist >= 0 ; iHist--){
      TH1* h = dynamic_cast<TH1*>(histStackList->At(iHist));
      stack->Add(h);
    }
  } else {
    for (int iHist = 0; iHist < histStackList->GetEntries(); iHist++){
      TH1* h = dynamic_cast<TH1*>(histStackList->At(iHist));
      stack->Add(h);
    }
  }

  // the histStackList was only a vehicle for an easy implementation of the loop
  delete histStackList;
  delete processStackList;

  TQTaggableIterator sitr(this->fProcesses);
  bool stackSignal = tags.getTagBoolDefault ("style.stackSignal",false);
  if (tags.getTagBoolDefault ("style.autoStackSignal",false)) stackSignal = false;
  if(hTotalStack){
    while(sitr.hasNext()){
      TQNamedTaggable* process = sitr.readNext();
      if(!process) continue;
      if(!process->getTagBoolDefault(".isSignal",false)) continue;
      TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
      if(!h) continue;
      if(totalStackScale > 0 && process->getTagBoolDefault("normalizeToTotalBkg",false) && !normalize){ //don't scale again if everything was already normalized to unity (see above)
        //@tag:normalizeToTotalBkg: process tag to normalize individual signal contributions to the total background
        h->Scale(totalStackScale / TQHistogramUtils::getIntegral(h));
      }
      if (process->getTagBoolDefault("stack", stackSignal)){
        stack->Add(h);
      }
    }
  }

  this->addObject(stack,stackname);
}

//__________________________________________________________________________________|___________

TCanvas * TQHWWPlotter::makePlot(TQTaggable& tags) {
  // master-function controlling the plotting
  bool verbose = tags.getTagBoolDefault("verbose",false);

  if(verbose) VERBOSEclass("entering function");
  TString lepch;

  TString histogram = tags.getTagStringDefault("input.histogram");

  TString histname_data = "";
  TString histname_bkg = "";
  TString histname_sig = "";
  if(histogram.First('=') != kNPOS){
    TQTaggable names(histogram);
    names.getTagString("bkg",histname_bkg);
    names.getTagString("sig",histname_sig);
    names.getTagString("data",histname_data);
    if(histname_bkg.IsNull()) getTagString("sig" ,histname_bkg);
    if(histname_bkg.IsNull()) getTagString("data",histname_bkg);
    if(histname_bkg.IsNull()) return NULL;
    if(histname_data.IsNull()) histname_data = histname_bkg;
    if(histname_sig.IsNull()) histname_sig = histname_bkg;
    names.getTagString("lepch",lepch);
    names.getTagString("channel",lepch);
    histogram = "";
    //@tag:.isSignal: process tag to identify signal
    if(this->getNProcesses(".isSignal") > 0) histogram+="sig:"+histname_sig+",";
    //@tag:.isBackground: process tag to identify background
    if(this->getNProcesses(".isBackground") > 0) histogram+="bkg:"+histname_bkg+",";
    //@tag:.isData: process tag to identify data
    if(this->getNProcesses(".isData") > 0) histogram+="data:"+histname_data+",";
    TQStringUtils::removeTrailing(histogram,",");
  } else {
    lepch = TQStringUtils::readPrefix(histogram, ":", "?");
    histname_bkg = histogram;
    histname_sig = histogram;
    histname_data = histogram;
  }

  gStyle->SetOptStat(false);
  gStyle->SetOptFit(false);
  gStyle->SetOptTitle(false);

  //@tag:[input.bkg]: overwrite the sample folder path used for background
  //@tag:[input.sig]: overwrite the sample folder path used for signal
  //@tag:[input.data]: overwrite the sample folder path used for data
  //@tag:[input.lepch,input.chname]: overwrite the lepton channel used (deprecated, use 'input.channel' instead)
  //@tag:[input.channel]: overwrite the channel
  //@tag:[input.sys]: choose the path from which to read the systematics

  tags.getTagString("input.lepch", lepch);
  tags.getTagString("input.channel", lepch); //this takes precedence over the deprecated 'lepch'
  tags.setTagString("input.channel", lepch);
  tags.setTagString("input.chname",lepch == "?" ? "all" : lepch);
  tags.setTagString("input.name",histogram);
  tags.setTagString("input.bkg", histname_bkg);
  tags.setTagString("input.data",histname_data);
  tags.setTagString("input.sig", histname_sig);

  tags.setTagString("input.sys", histname_bkg);

  if(verbose) VERBOSEclass("bkg: %s",histname_bkg.Data());
  if(verbose) VERBOSEclass("sig: %s",histname_sig.Data());
  if(verbose) VERBOSEclass("data: %s",histname_data.Data());
  if(verbose) VERBOSEclass("sys: %s",histname_data.Data());

  //////////////////////////////////////////////////////
  // obtain the histograms
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("collecting histograms");
  TObjArray* histos = this->collectHistograms(tags);
  if(!histos) return NULL;

  // @tag:style.drawData: select whether to draw data points (default:true)
  bool drawData = tags.getTagBoolDefault ("style.drawData",true);
  if (!drawData) {
    tags.setGlobalOverwrite(true);
    tags.setTagBool("style.showKS",false);
    tags.setTagBool("style.showRatio",false);
    tags.setTagBool("style.showDoverB",false);
    tags.setTagBool("style.showDminusB",false);
    tags.setTagBool("style.showSignificance",false);
    tags.setGlobalOverwrite(false);
  }
  
  TQTH1Iterator histitr(histos);
  int nEntries = 0;
  bool is2D = false;
  bool isTProfile = false;
  while(histitr.hasNext()){
    TH1* hist = histitr.readNext();
    nEntries += hist->GetEntries();
    if(dynamic_cast<TH2*>(hist)) is2D=true;
    if(dynamic_cast<TProfile*>(hist)) isTProfile=true;
  }
  if(nEntries < 1){
    WARNclass("refusing to plot histogram '%s' - no entries!",histogram.Data());
    return NULL;
  }

  //////////////////////////////////////////////////////
  // the ratio plot can only be shown if there is a valid
  // data histogram and at least one MC background histogram
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("sanitizing tags");

  // @tag:style.overrideTotalBkgRequirement: usually, 1D plots without background are skipped. force plotting data/signal only with this option.
  bool overrideTotalBkgRequirement = tags.getTagBoolDefault("style.overrideTotalBkgRequirement", is2D);

  TH1* hTotalStack = this->getObject<TH1>("totalStack");
  if ((!hTotalStack || hTotalStack->GetEntries() <= 0) && !overrideTotalBkgRequirement){
    if(verbose) VERBOSEclass("no total background histogram found, quitting");
    return NULL;
  }
  TH1* hMaster = this->getObject<TH1>("Graph_master");
  if (!hMaster){
    if(verbose) VERBOSEclass("no master histogram found, quitting");
    return NULL;
  }

  /*@tag:[style.showRatio,style.showSignificance,style.showDoverB,style.showDminusB,style.showDminusBoverD,style.showOptScan,style.showCustom]

    control what is shown in the sub-plot. all of these default to 'false', only showing the main plot.
    if any of these are set to true, the corresponding sub plot is shown. only one sub plot can be shown at a time.

    style.showRatio: show the ratio between data and signal+bkg
    style.showMultiRatio: show the ratio between pairs of data and background processes in order of adding
    style.showSignificance: show the poisson significance of the data over the background in each  bin
    style.showDoverB: show the ratio between data and background
    style.showDminusB: show the difference between data and background
    style.showDminusBoverD: show the the ratio between the data-bkg (signal estimate) and the data
    style.showOptScan: show an optimization scan. several figure-of-merit (FOM) options are supported via optScan.FOMmode and optScan.FOMbbb
    style.showCustom, style.ratioFormula: customized subplot. can be controlled with style.ratioFormula
   */
  bool showPull = tags.getTagBoolDefault ("style.showPull",false);
  bool showMultiRatio = tags.getTagBoolDefault ("style.showMultiRatio",false);
  bool showSignificance = tags.getTagBoolDefault ("style.showSignificance",false);
  bool showDoverB = tags.getTagBoolDefault ("style.showDoverB",false) ||  tags.getTagBoolDefault ("style.showRatio",false);
  bool showDminusB = tags.getTagBoolDefault ("style.showDminusB",false);
  bool showDminusBoverD = tags.getTagBoolDefault ("style.showDminusBoverD",false);
  bool showOptScan = tags.getTagBoolDefault ("style.showOptScan",false);
  bool showCustom = tags.getTagBoolDefault ("style.showCustom",false) && tags.hasTagString("style.ratioFormula") ;
  bool showUnitNorm = tags.getTagBoolDefault ("style.showUnitNorm", false);
  int nDim = tags.getTagIntegerDefault("style.nDim",1);
  if(nDim != 1){
    showPull = false;
    showMultiRatio = false;
    showDoverB = false;
    showSignificance = false;
    showDminusB = false;
    showDminusBoverD = false;
    showOptScan = false;
    showCustom = false;
    tags.setTagBool("style.showTotalBkg",false);
  }

  if(showSignificance || showDoverB || showOptScan || showDminusB || showDminusBoverD || showCustom || showMultiRatio || showPull || showUnitNorm){
    tags.setGlobalOverwrite(true);
    tags.setTagBool("style.showSub",true);
    tags.setGlobalOverwrite(false);
  } else {
    tags.setTagBool("style.showSub",false);
  }

  //////////////////////////////////////////////////////
  // set and apply the style
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("applying style");
  bool newPlotStyle = tags.getTagBoolDefault ("style.newPlotStyle", false);
  tags.setGlobalOverwrite(false);
  if (newPlotStyle){
    this->setStyleIllinois(tags);
  } else {
    this->setStyle(tags);
  }

  double xscaling = tags.getTagDoubleDefault("geometry.xscaling",1.);
  //@tags:[geometry.main.*] control geometry parameters of the main pad
  this->applyGeometry(tags,hMaster, "main", xscaling,tags.getTagDoubleDefault("geometry.main.scaling",1.));
  this->applyStyle (tags,hMaster, "main", xscaling,tags.getTagDoubleDefault("geometry.main.scaling",1.));
  this->applyStyle (tags,hTotalStack,"main.totalStack",xscaling,tags.getTagDoubleDefault("geometry.main.scaling",1.));

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
  // create the stack
  //////////////////////////////////////////////////////

  if(hTotalStack && nDim == 1){
    if(verbose) VERBOSEclass("stacking histograms");
    this->stackHistograms(tags,"stack");
  }

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
  // calculate advanced labels
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("calculating tests and other labels");

  if(hTotalStack){
    if(tags.getTagBoolDefault("style.showBkgRMS",false)){
      if(verbose) VERBOSEclass("calculating total background RMS");
      double RMS = hTotalStack->GetRMS();
      tags.setTagString(TString::Format("labels.%d",labelidx),TString::Format("#font[72]{RMS(%s) = %g}", hTotalStack->GetTitle(), RMS));
      labelidx++;
    }
    TQTaggableIterator itr(fProcesses);
    while(itr.hasNext()){
      // get the data histogram
      TQNamedTaggable* process = itr.readNext();
      if(!process) continue;
      if(!process->getTagBoolDefault(".isData",false)) continue;
      TH1 * h_data = this->getObject<TH1>(this->makeHistogramIdentifier(process));
      if(!h_data || h_data->GetEntries() == 0) continue;
      if(tags.getTagBoolDefault ("style.showKS",false)){
        if(verbose) VERBOSEclass("calculating KS test for '%s'",h_data->GetTitle());
        float ks = std::numeric_limits<double>::quiet_NaN();
        if(h_data->GetEntries() > 0 && hTotalStack->GetEntries() > 0 && h_data->Integral() > 0 && hTotalStack->Integral() > 0){
          ks = hTotalStack->KolmogorovTest(h_data);
        }
        TString id = TString::Format("labels.%d",labelidx);
        if(TQUtils::isNum(ks)){
          float roundedKS = (float) int(ks*10000.+0.5)/10000.;
          tags.setTagString(id,TString::Format("#color[2]{#font[72]{KS Prob = %2.1f%%}}", roundedKS*100.));
        } else {
          tags.setTagString(id,"#color[2]{#font[72]{KS Prob = N.A.}}");
        }
        labelidx++;
      }
      if(tags.getTagBoolDefault ("style.showChi2",false)){
        if(verbose) VERBOSEclass("calculating Chi2 test for '$s'",h_data->GetTitle());
        Double_t chi2 = 0;
        Int_t ndf = 0, igood = 0;
        TH1F* h_data_tmp = (TH1F*) h_data->Clone("h_data_tmp");
        TH1F* hTotalStack_tmp = (TH1F*) hTotalStack->Clone("hTotalStack_tmp");
        double maxSanitization = 0.;
        if (tags.getTagDouble("style.chi2.maxSanitization",maxSanitization)) {
          for(int i_tmp=1;i_tmp<=h_data_tmp->GetNbinsX()+1;i_tmp++){
            if( h_data_tmp->GetBinContent(i_tmp)==0 && hTotalStack_tmp->GetBinContent(i_tmp)<maxSanitization ){
              hTotalStack_tmp->SetBinContent(i_tmp, 0);
              hTotalStack_tmp->SetBinError(i_tmp, 0);
            }
            if( hTotalStack_tmp->GetBinContent(i_tmp)==0 && h_data_tmp->GetBinContent(i_tmp)<maxSanitization ){
              h_data_tmp->SetBinContent(i_tmp, 0);
              h_data_tmp->SetBinError(i_tmp, 0);
            }
          }
        }
        double prob = h_data_tmp->Chi2TestX(hTotalStack_tmp,chi2,ndf,igood,"UW UF OF");
        float roundedChi2Prob = (float) int(prob*10000.+0.5)/10000.;
        tags.setTagString(TString::Format("labels.%d",labelidx),TString::Format("#color[2]{#font[72]{Chi2 Prob = %2.1f%%}}", roundedChi2Prob*100.));
        labelidx++;
        delete h_data_tmp;
        delete hTotalStack_tmp;
      }
      if(tags.getTagBoolDefault ("style.showChi2P",false)){
        TH1F* h_data_tmp = (TH1F*) h_data->Clone("h_data_tmp");
        TH1F* hTotalStack_tmp = (TH1F*) hTotalStack->Clone("hTotalStack_tmp");
        double maxSanitization = 0.;
        if (tags.getTagDouble("style.chi2.maxSanitization",maxSanitization)) {
          for(int i_tmp=1;i_tmp<=h_data_tmp->GetNbinsX()+1;i_tmp++){
            if( h_data_tmp->GetBinContent(i_tmp)==0 && hTotalStack_tmp->GetBinContent(i_tmp)<maxSanitization ){
              hTotalStack_tmp->SetBinContent(i_tmp, 0);
              hTotalStack_tmp->SetBinError(i_tmp, 0);
            }
            if( hTotalStack_tmp->GetBinContent(i_tmp)==0 && h_data_tmp->GetBinContent(i_tmp)<maxSanitization ){
              h_data_tmp->SetBinContent(i_tmp, 0);
              h_data_tmp->SetBinError(i_tmp, 0);
            }
          }
        }
        double p_value = h_data_tmp->Chi2Test(hTotalStack_tmp,"UW UF OF");
        tags.setTagString(TString::Format("labels.%d",labelidx),TString::Format("#color[2]{#font[72]{#chi^{2} p-value = %f}}", p_value));
        labelidx++;
        delete h_data_tmp;
        delete hTotalStack_tmp;

      }
      if(tags.getTagBoolDefault ("style.showDataRMS",false)){
        if(verbose) VERBOSEclass("calculating RMS for '$s'",h_data->GetTitle());
        double RMS = h_data->GetRMS();
        tags.setTagString(TString::Format("labels.%d",labelidx),TString::Format("#color[2]{#font[72]{RMS(%s) = %2.1f%%}}", h_data->GetTitle(),RMS));
        labelidx++;
      }
    }
  } else {
    if(verbose) VERBOSEclass("no total background histogram found!");
  }


  //////////////////////////////////////////////////////
  // manage the total background error bars
  //////////////////////////////////////////////////////

  this->setErrors(tags,"totalBkg");

  tags.setGlobalOverwrite(false);

  //////////////////////////////////////////////////////
  // draw main pad
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("drawing main pad");
  if(isTProfile){
    this->drawProfiles(tags);
    this->drawLegend(tags);
  } else if(nDim == 1){
    if(!tags.getTagBoolDefault("allow1D",true))	return NULL;
    bool ok = this->drawStack(tags);
    if(!ok){
      return NULL;
    }
    this->drawLegend(tags);
    if(verbose) VERBOSEclass("drawing cut lines");
    this->drawCutLines1D(tags);
  } else if(nDim == 2){
    if(!tags.getTagBoolDefault("allow2D",true)) return NULL;
    if(tags.hasTagString("style.heatmap")){
      this->drawHeatmap(tags);
    } else if (tags.hasTagString("style.migration")){
      this->drawMigration(tags);
    } else {
      this->drawContours(tags);
      this->drawLegend(tags);
    }
    if(verbose) VERBOSEclass("drawing decorations");
    this->drawAdditionalAxes(tags);
    this->drawHeightLines(tags);
  } else {
    ERRORclass("unsupported dimensionality (nDim=%d)!",nDim);
    return NULL;
  }

  //////////////////////////////////////////////////////
  // do some advanced magic
  //////////////////////////////////////////////////////

  TString fit;
  if(hTotalStack && nDim == 1 && tags.getTagString("totalStackFit.function",fit)){
    TFitResultPtr p = hTotalStack->Fit(fit,"Q");
    TF1* func = dynamic_cast<TF1*>(hTotalStack->GetListOfFunctions()->Last());
    if(func){
      func->SetLineColor(tags.getTagIntegerDefault("totalStackFit.color",kRed));
      func->SetLineStyle(tags.getTagIntegerDefault("totalStackFit.style",1));
      func->Draw("SAME");
      if(tags.getTagBoolDefault("totalStackFit.showResults",true)){
        TString info = "";
        for(Int_t i=0; i<func->GetNpar(); ++i){
          TString name = func->GetParName(i);
          tags.getTagString(TString::Format("totalStackFit.parameter.%d.name",i),name);
          double val = func->GetParameter(i);
          double uncertainty = func->GetParError(i);
          tags.setTagString(TString::Format("labels.%d",labelidx),TString::Format("%s = %g #pm %g",name.Data(),val,uncertainty));
          labelidx++;
        }
      }
    }
  }

  if (drawData) {
    if(verbose) VERBOSEclass("fitting data slopes");
    TQTaggableIterator itr_data(fProcesses);
    while(itr_data.hasNext()){
      TQNamedTaggable* process = itr_data.readNext();
      if(!process) continue;
      if(!process->getTagBoolDefault(".isData",false)) continue;
      TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
      if(!h) continue;
      if(tags.getTagBoolDefault("style.data.fitSlope",false)){
        double fitSlopeXmin = tags.getTagDoubleDefault("style.data.fitSlope.xmin",hMaster->GetXaxis()->GetXmin());
        double fitSlopeXmax = tags.getTagDoubleDefault("style.data.fitSlope.xmax",hMaster->GetXaxis()->GetXmax());
        h->Fit("pol1","QG","", fitSlopeXmin, fitSlopeXmax);
        TF1* f = h->GetFunction("pol1");
        f->SetName(TString::Format("%s_fit",h->GetName()));
        //      this->addObject(f);
        f->SetLineColor(h->GetMarkerColor());
        f->SetLineWidth(tags.getTagIntegerDefault("style.data.fitSlope.lineWidth",1));
        f->SetLineStyle(tags.getTagIntegerDefault("style.data.fitSlope.lineStyle",2));
        //@tag:style.data.fitSlope.exportResults: export the fit results as tags on the plotter
        if (tags.getTagBoolDefault("style.data.fitSlope.exportResults",false)) {
          this->setTagDouble(TString::Format("export.fitSlope.%s.slope",h->GetName()),f->GetParameter(1));
          this->setTagDouble(TString::Format("export.fitSlope.%s.slopeError",h->GetName()),f->GetParError(1));
          this->setTagDouble(TString::Format("export.fitSlope.%s.chi2",h->GetName()),f->GetChisquare());
        }
        if (tags.getTagBoolDefault("style.data.fitSlope.printResults",true)) {
          double slope = TQUtils::roundAuto(f->GetParameter(1));
          double slopeErr = TQUtils::roundAuto(f->GetParError(1));
          double chi2 = TQUtils::roundAuto(f->GetChisquare());
          h->SetTitle(TString::Format("%s (slope #approx %g #pm %g with #chi^{2}/NDF#approx%g)",h->GetTitle(),slope,slopeErr,chi2/f->GetNDF()));
        }
      }
    }
  }


  //////////////////////////////////////////////////////
  // draw the labels
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("drawing labels");
  this->setAxisLabels(tags);
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

  if(tags.getTagBoolDefault("style.showSub",false)){
    if(verbose) VERBOSEclass("drawing subplot");
    canvas->cd();

    TPad* subPad = this->getPad("sub");
    if(!subPad){
      throw std::runtime_error("unable to obtain sub pad!");
    }
    subPad->cd();

    if (showDoverB){
      if(verbose) VERBOSEclass("drawing ratio");
      this->drawRatio(tags);
    } else if(showPull){
      if(verbose) VERBOSEclass("drawing pull");
      this->drawPull(tags);
    } else if(showMultiRatio){
      if(verbose) VERBOSEclass("drawing multi ratio");
      this->drawMultiRatio(tags);
    } else if(showSignificance){
      if(verbose) VERBOSEclass("drawing significance");
      this->drawSignificance(tags);
    } else if(showOptScan){
      if(verbose) VERBOSEclass("drawing optimization scan");
      this->drawOptScan(tags);
    } else if(showDminusB){
      if(verbose) VERBOSEclass("drawing data minus background");
      this->drawDataMinusBackground(tags);
    } else if(showDminusBoverD){
      if(verbose) VERBOSEclass("drawing data minus background over data");
      this->drawDataMinusBackgroundOverData(tags);
    } else if(showCustom) {
      if(verbose) VERBOSEclass("drawing custom subplot");
      this->drawCustomSubPlot(tags);
    } else if(showUnitNorm) {
      if(verbose) VERBOSEclass("drawing unit norm subplot");
      this->drawUnitNormSubPlot(tags);
    }
    subPad->RedrawAxis();
    subPad->Update();
    subPad->cd();
  }

  if(verbose) VERBOSEclass("all done!");
  // return the canvas
  return canvas;
}

//__________________________________________________________________________________|___________

void TQHWWPlotter::drawLabels(TQTaggable& tags){
  // draw the labels given by the tags
  bool drawlabels = tags.getTagBoolDefault("style.showLabels",true);
  double scaling = tags.getTagDoubleDefault("geometry.main.scaling",1.);
  double textsize = tags.getTagDoubleDefault("style.textSize",DEFAULTTEXTSIZE);
  int font = tags.getTagDoubleDefault("style.text.font",42);
  int color = tags.getTagDoubleDefault("style.text.color",1);
  double xscaling = tags.getTagDoubleDefault("geometry.xscaling",1.);

  double x = tags.getTagDoubleDefault("style.labels.xOffset",0.2)*xscaling;

  double y = tags.getTagDoubleDefault("style.labels.yPos",0.86 - tags.getTagDoubleDefault("geometry.main.additionalTopMargin",0.));

  //@tag:labels.drawATLAS: decide whether to draw the 'ATLAS' label
  bool drawATLAS = tags.getTagBoolDefault ("labels.drawATLAS",drawlabels);

  TString nfLabel = "";
  //@tag:labels.drawNFInfo: decide whether to draw information on which NFs were applied
  if(tags.getTagBoolDefault ("labels.drawNFInfo",false)){
    TString tmpLabel = tags.getTagStringDefault("labels.nfInfo","#color[2]{(NF applied for %s)}");
    if(TQStringUtils::countText(tmpLabel,"%s") == 1){
      TString nflist = this->getScaleFactorList(tags.getTagStringDefault("input.bkg",""));
      if(!nflist.IsNull()){
        nfLabel = TString::Format(tmpLabel.Data(),TQStringUtils::convertLaTeX2ROOTTeX(nflist).Data());
      }
    }
  }

  //@tag:labels.drawInfo: decide whether to draw the technical info tag on the top right of the plot
  TString infoLabel = tags.getTagBoolDefault ("labels.drawInfo",drawlabels) ? tags.getTagStringDefault ("labels.info",TString::Format("Plot: \"%s\"", tags.getTagStringDefault("input.name","histogram").Data())) : "";
  //@tag:labels.atlas: which ATLAS label to use (Private, work in progress, Internal, Preliminary, ... - default:'Private')
  TString atlasLabel = tags.getTagStringDefault ("labels.atlas","Private");
  TString stickerLabel = tags.getTagStringDefault ("labels.sticker","");

  if (drawATLAS) {
    // draw the ATLAS label
    TLatex l;
    l.SetNDC();
    l.SetTextFont(72);
    //@tag:labels.drawATLAS.scale: scale of the ATLAS label. Defaults to the scale set by 'labels.atlas.scale' or 1.25 if neither of the two tags are present
    l.SetTextSize(textsize * tags.getTagDoubleDefault("labels.drawATLAS.scale",tags.getTagDoubleDefault("labels.atlas.scale",1.25)) * scaling);
    l.SetTextColor(1);
    //@tag:labels.drawATLAS.text: text of the ATLAS tag (default: ATLAS)
    l.DrawLatex(x, y, tags.getTagStringDefault("labels.drawATLAS.text","ATLAS"));
  }

  if (drawATLAS && !atlasLabel.IsNull()){
    // draw the ATLAS label addition
    TLatex p;
    p.SetNDC();
    p.SetTextFont(font);
    p.SetTextColor(color);
    //@tag:labels.atlas.scale: scale of the addition to the ATLAS label (Internal, Private,...). Defaults to the scale set by 'labels.drawATLAS.scale' or 1.25 if neither of the two tags are present
    p.SetTextSize(textsize * tags.getTagDoubleDefault("labels.atlas.scale",tags.getTagDoubleDefault("labels.drawATLAS.scale",1.25)) * scaling);
    //@tag:labels.atlas.xOffset : horizontal offset between ATLAS label and its addition. (default: 0.16)
    p.DrawLatex(x + tags.getTagDoubleDefault("labels.atlas.xOffset",0.16)*xscaling, y, atlasLabel.Data());
  }

  if (!infoLabel.IsNull()){
    // draw the info label
    if(!nfLabel.IsNull()){
      infoLabel.Prepend(" ");
      infoLabel.Prepend(nfLabel);
    }
    TLatex l0;
    l0.SetNDC();
    l0.SetTextFont(font);
    bool newPlotStyle = tags.getTagBoolDefault ("style.newPlotStyle", false);
    if (newPlotStyle)
      l0.SetTextSize(textsize * tags.getTagDoubleDefault("labels.info.size",0.6) * scaling * 0.7);
    else
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
  if(drawlabels){
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

  // draw extra labels
  if(true){
    int index = 0;
    while(true){
      TString key = TString::Format("extralabels.%d",index);
      TString labeltext;
      if(!tags.getTagString(key,labeltext)){
        break;
      }
      TLatex latex;
      latex.SetNDC();
      latex.SetTextFont(font);
      latex.SetTextSize(textsize * labelTextScale * scaling);
      latex.SetTextColor(color);
      latex.DrawLatex(tags.getTagDoubleDefault(key+".x",x),
                      tags.getTagDoubleDefault(key+".y", y - marginStep * index * scaling),
                      labeltext);
      index++;
    }
  }
}

//__________________________________________________________________________________|___________

void TQHWWPlotter::drawSignificance(TQTaggable& tags){
  // draw a significance plot in the sub-pad
  TString totalStackLabel = tags.getTagStringDefault ("labels.totalStack", "SM");

  TH1* hMaster = this->getObject<TH1>("Graph_master");
  TH1* hTotalStack = this->getObject<TH1>("totalStack");
  if(!hTotalStack) return;

  int nBins = hMaster->GetNbinsX();

  // loop over all histograms
  TQTaggableIterator itr(fProcesses);
  while(itr.hasNext()){
    // get the data histograms only
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(".isData",false)) continue;
    TH1 * h_data = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    if (!h_data) continue;

    // calculate the number of valid ratio points: ratio points are considered
    // valid if they have a finite value (MC prediction != 0) (--> nPoints) and
    // the observed data is greater than zero (--> nRatioPoints)
    int nPoints = 0;
    int nRatioPoints = 0;
    for (int i = 1; i <= nBins; i++) {
      if (hTotalStack->GetBinContent(i) != 0.) {
        nPoints++;
        if (h_data->GetBinContent(i) > 0)
          nRatioPoints++;
      }
    }

    if(nRatioPoints < 1){
      // there is nothing to draw -- well, let's do nothing, then
      continue;
    }

    // the graph used to draw the error band on the ratio
    TGraphAsymmErrors * significanceGraph = new TGraphAsymmErrors(nPoints);
    this->addObject(significanceGraph,TString::Format("significanceGraph_%s",h_data->GetName()));

    int iPoint = 0;

    // actual minimum and maximum ratio
    double actualSigMin = 0.;
    double actualSigMax = 0.;

    bool first = true;
    // loop over all bins of the histogram
    for (int iBin = 1; iBin <= nBins; iBin++) {

      // get the values and errors of data and MC for this bin
      double data = h_data ->GetBinContent(iBin);
      double MC = hTotalStack->GetBinContent(iBin);
      // cannot do anything if MC expectation is zero
      if (MC == 0.)
        continue;

      double sig = 0.;
      double pValue = 0.;

      // set the position and the width of the significance band
      significanceGraph->SetPoint(iPoint, hTotalStack->GetBinCenter(iBin), 0.);

      pValue = TQHistogramUtils::pValuePoisson((unsigned)data, MC);
      if (pValue < 0.5)
        sig = TQHistogramUtils::pValueToSignificance(pValue, (data > MC));

      if (sig < 0.) {
        significanceGraph->SetPointError(
                                         iPoint, hTotalStack->GetBinWidth(iBin) / 2.,
                                         hTotalStack->GetBinWidth(iBin) / 2., -sig, 0.);
      } else {
        significanceGraph->SetPointError(
                                         iPoint, hTotalStack->GetBinWidth(iBin) / 2.,
                                         hTotalStack->GetBinWidth(iBin) / 2., 0., sig);
      }

      if (sig < actualSigMin){
        actualSigMin = sig;
      }
      if (sig > actualSigMax){
        actualSigMax = sig;
      }

      // set the position and the width of the significance band
      significanceGraph->SetPoint(iPoint, hTotalStack->GetBinCenter(iBin), 0.);

      pValue = TQHistogramUtils::pValuePoisson((unsigned)data, MC);
      if (pValue < 0.5)
        sig = TQHistogramUtils::pValueToSignificance(pValue, (data > MC));

      if (sig < 0.) {
        significanceGraph->SetPointError(
                                         iPoint, hTotalStack->GetBinWidth(iBin) / 2.,
                                         hTotalStack->GetBinWidth(iBin) / 2., -sig, 0.);
      } else {
        significanceGraph->SetPointError(
                                         iPoint, hTotalStack->GetBinWidth(iBin) / 2.,
                                         hTotalStack->GetBinWidth(iBin) / 2., 0., sig);
      }

      if (sig < actualSigMin){
        actualSigMin = sig;
      }
      if (sig > actualSigMax){
        actualSigMax = sig;
      }

      iPoint++;

    }

    double xscaling = tags.getTagDoubleDefault("geometry.xscaling",1.);
    double sigPadScaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);
    this->applyStyle(tags,significanceGraph,"significance",xscaling,sigPadScaling);
    this->applyGeometry(tags,significanceGraph,"main" ,xscaling,sigPadScaling);
    this->applyGeometry(tags,significanceGraph,"sub" ,xscaling,sigPadScaling);

    // set the x range of the ratio graph to match the one of the main histogram
    double xLowerLimit = hTotalStack->GetBinLowEdge(1);
    double xUpperLimit = hTotalStack->GetBinLowEdge(nBins) + hTotalStack->GetBinWidth(nBins);
    significanceGraph->GetXaxis()->SetLimits(xLowerLimit, xUpperLimit);

    // set the titles of the axis of the ratio graph
    significanceGraph->GetXaxis()->SetTitle(hTotalStack->GetXaxis()->GetTitle());

    // confine the y axis of the ratio plot
    significanceGraph->GetYaxis()->SetTitle("Significance");

    actualSigMin = TMath::Abs(actualSigMin);
    int y1 = TMath::Nint(actualSigMin);
    if (y1 < actualSigMin)
      actualSigMin = y1 + 0.5;
    else
      actualSigMin = y1;

    if (fmod(actualSigMin, 1) == 0)
      actualSigMin += 0.5;
    int y2 = TMath::Nint(actualSigMax);
    if (y2 < actualSigMax)
      actualSigMax = y2 + 0.5;
    else
      actualSigMax = y2;
    if (fmod(actualSigMax, 1) == 0)
      actualSigMax += 0.5;

    significanceGraph->GetHistogram()->SetMinimum(-actualSigMin);
    significanceGraph->GetHistogram()->SetMaximum(actualSigMax);

    // draw the ratio/significance graph
    if (first)
      significanceGraph->Draw("A2");
    else
      significanceGraph->Draw("2");

    // if 1. is included in the range of the y axis of the ratio plot...
    if ((significanceGraph->GetHistogram()->GetMinimum() <= 0.) && (significanceGraph->GetHistogram()->GetMaximum() >= 0.)) {
      // draw the red line indicating 1 in the ratio plot and around 0 in case
      // of significance
      TLine * line = new TLine(xLowerLimit, 0, xUpperLimit, 0);
      line->SetLineColor(kRed);
      line->Draw();
    }

    first = false;
  }
}

//__________________________________________________________________________________|___________

void TQHWWPlotter::drawRatio(TQTaggable& tags){
  // draw a ratio-plot in the sub-pad
  double ratioMax = tags.getTagDoubleDefault ("style.ratioMax",1000.);
  double ratioMin = tags.getTagDoubleDefault ("style.ratioMin",0.);
  double ratioMaxQerr = tags.getTagDoubleDefault ("style.ratioMaxQerr",std::numeric_limits<double>::infinity());
  bool forceRatioLimits = tags.getTagBoolDefault ("style.forceRatioLimits",false );
  bool asymmStatErrorData = tags.getTagBoolDefault("style.data.asymErrors", false);
  bool verbose = tags.getTagBoolDefault("verbose",false);
  /// BW: Hashed the following two lines (unused variables)
  //bool showXErrors = tags.getTagBoolDefault("style.ratio.showXErrors", false);
  //bool showYErrors = tags.getTagBoolDefault("style.ratio.showYErrors", true);

  TString ratioDenominator = tags.getTagStringDefault("style.ratio.denominator","totalStack");
  TH1* denominator = this->getObject<TH1>(ratioDenominator);
  if(!denominator) return;

  int nBins = denominator->GetNbinsX();

  // the graph used to draw the error band on the ratio
  if(verbose) VERBOSEclass("generating ratio error graphs");

  // todo: tag documentation
  bool invertRatio = tags.getTagBoolDefault("style.invertRatio",false);

  // todo: make this a histogram, not graph
  TGraphAsymmErrors * ratioErrorGraph = getRatioErrorGraph(denominator);
  this->addObject(ratioErrorGraph);

  if(verbose) VERBOSEclass("calculating geometry and axis ranges");
  // set the x range of the ratio graph to match the one of the main histogram
  //double xLowerLimit = denominator->GetBinLowEdge(1);
  //double xUpperLimit = denominator->GetBinLowEdge(nBins) + denominator->GetBinWidth(nBins);
  double xLowerLimit;
  double xUpperLimit;
  //@tag:[style.xmin,style.xmax] Set custom x-axis range
  if (tags.getTagDouble("style.xmin", xLowerLimit) && tags.getTagDouble("style.xmax", xUpperLimit)) {
    ratioErrorGraph->GetXaxis()->SetLimits(xLowerLimit, xUpperLimit);
  }
  else {
    xLowerLimit = denominator->GetBinLowEdge(1);
    xUpperLimit = denominator->GetBinLowEdge(nBins) + denominator->GetBinWidth(nBins);
    ratioErrorGraph->GetXaxis()->SetLimits(xLowerLimit, xUpperLimit);
  }
  //ratioErrorGraph->GetXaxis()->SetLimits(xLowerLimit, xUpperLimit);
  double ratioPadScaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);
  double xscaling = tags.getTagDoubleDefault("geometry.xscaling",1);
  this->applyStyle (tags,ratioErrorGraph,"ratio.mcErrorBand",xscaling,ratioPadScaling);
  this->applyGeometry(tags,ratioErrorGraph,"main" ,xscaling,ratioPadScaling);
  this->applyGeometry(tags,ratioErrorGraph,"sub" ,xscaling,ratioPadScaling);

  if(verbose) VERBOSEclass("drawing ratio error graph");
  ratioErrorGraph->Draw("A2");
  TH1* hMaster = this->getObject<TH1>("Graph_master");
  ratioErrorGraph->GetHistogram()->GetXaxis()->SetTitle(hMaster->GetXaxis()->GetTitle());

  TString dataLabel("");
  tags.getTagString("labels.numerator",dataLabel);
  TList* graphs = new TList();

  // actual minimum and maximum ratio
  double actualRatioMin = 1.;
  double actualRatioMax = 1.;

  if(verbose) VERBOSEclass("generating ratio graphs");
  // loop over data histograms
  TQTaggableIterator itr(fProcesses);

  //@tag:style.ratio.dropYieldsBelow: suppress points in the ratio graph where either MC or data yield are below this number
  double ratioContentThreshold = tags.getTagDoubleDefault("style.ratio.dropYieldsBelow",0.0001);

  double ratioMinAllowed = tags.getTagDoubleDefault ("style.ratioMinAllowed",ratioMin);
  double ratioMaxAllowed = tags.getTagDoubleDefault ("style.ratioMaxAllowed",ratioMax);
  actualRatioMin=ratioMinAllowed;
  actualRatioMax=ratioMaxAllowed;
  if(verbose) VERBOSEclass("drawRatio: allowed range of ratio graph: %f -- %f",actualRatioMin,actualRatioMax);

  while(itr.hasNext()){
    // get the data histogram
    DEBUGclass("next process...");
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(".isData",false)) continue;
    TH1 * h_data = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    if(!h_data) continue;
    if(dataLabel.IsNull()) dataLabel = h_data->GetTitle();

    if (asymmStatErrorData) {
      h_data->Sumw2(false); //only do this on data, all sum-of-squared-weights information is deleted (sqrt(n) will be used instead)
      h_data->SetBinErrorOption(TH1::kPoisson);
    }

    TGraphAsymmErrors* ratioGraph = getRatioGraph(h_data,denominator,invertRatio,ratioContentThreshold,verbose);
    if(!ratioGraph) continue;

    this->addObject(ratioGraph,TString::Format("ratioGraph_%s",h_data->GetName()));

    this->applyStyle(tags   ,ratioGraph,"sub.data",1.,ratioPadScaling);

    this->estimateRangeY(ratioGraph,actualRatioMin,actualRatioMax,ratioMaxQerr);

    if(verbose) VERBOSEclass("drawRatio: estimated range of ratio graph: %f -- %f (ratioMaxQerr=%f)",actualRatioMin,actualRatioMax,ratioMaxQerr);

    graphs->Add(ratioGraph);
  }


  if(actualRatioMin == actualRatioMax){
    if(verbose) VERBOSEclass("expanding ratio to not be empty");
    actualRatioMin *= 0.9;
    actualRatioMax *= 1.1;
  }
  if (forceRatioLimits)  actualRatioMin = ratioMin;
  else                   actualRatioMin = actualRatioMin-0.1*(actualRatioMax-actualRatioMin);

  if (forceRatioLimits)  actualRatioMax = ratioMax;
  else                   actualRatioMax = actualRatioMax+0.1*(actualRatioMax-actualRatioMin);
  if(verbose) VERBOSEclass("drawRatio: final of ratio graph: %f -- %f",actualRatioMin,actualRatioMax);
  ratioErrorGraph->GetHistogram()->SetMaximum(actualRatioMax);
  ratioErrorGraph->GetHistogram()->SetMinimum(actualRatioMin);

  TString totalStackLabel = tags.getTagStringDefault ("labels.totalStack", "SM");

  tags.getTagString("labels.data",dataLabel);
  ratioErrorGraph->GetHistogram()->GetXaxis()->SetTitle(hMaster->GetXaxis()->GetTitle());
  TString subXName = tags.getTagStringDefault("labels.axes.subX", "");
  if (subXName.Length()>0) {
    ratioErrorGraph->GetXaxis()->SetTitle(subXName);
  }
  ratioErrorGraph->GetYaxis()->SetTitle(tags.getTagStringDefault("labels.ratio",dataLabel + " / "+ tags.getTagStringDefault ("labels.totalStack", "SM") +" "));
  gStyle->SetEndErrorSize(0);

  if(verbose) VERBOSEclass("drawing lines");
  // if 1. is included in the range of the y axis of the ratio plot...
  this->applyStyle(tags,ratioErrorGraph->GetHistogram()->GetXaxis(),"sub.xAxis");
  this->applyStyle(tags,ratioErrorGraph->GetHistogram()->GetYaxis(),"sub.yAxis");
  this->applyStyle(tags,ratioErrorGraph->GetHistogram()->GetXaxis(),"sub.xAxis");
  
  if ((ratioErrorGraph->GetHistogram()->GetMinimum() <= 1) && (ratioErrorGraph->GetHistogram()->GetMaximum() >= 1.)) {
    // draw the red line indicating 1 in the ratio plot and around 0 in case of
    // significance
    TLine * line = new TLine(xLowerLimit, 1., xUpperLimit, 1.);
    line->SetLineColor(kRed);
    line->Draw();
  }


  // slope fitting for ratio plots
  double textsize = tags.getTagDoubleDefault("style.textSize",DEFAULTTEXTSIZE)* ratioPadScaling * tags.getTagDoubleDefault("style.ratio.fitSlope.printResults.textSize",0.5);
  TLatex l;
  l.SetNDC();
  l.SetTextSize(textsize);
  double fitResultPrintPosX = tags.getTagDoubleDefault("style.ratio.fitSlope.printResults.posX",0.2);
  double fitResultPrintPosY = tags.getTagDoubleDefault("style.ratio.fitSlope.printResults.posY",0.85);
  double fitResultPrintStepY = tags.getTagDoubleDefault("style.ratio.fitSlope.printResults.stepY",1.);

  TString ratioDrawStyle("SAME ");
  ratioDrawStyle.Append(tags.getTagStringDefault("style.ratio.drawOption","P e0"));

  TQGraphIterator itr2(graphs);
  while(itr2.hasNext()){
    TGraph* ratioGraph = itr2.readNext();
    if(!ratioGraph) continue;

    if(tags.getTagBoolDefault("style.ratio.fitSlope",false) && ratioGraph->GetN() > 1){
      if(verbose) VERBOSEclass("running fit on %s",ratioGraph->GetName());
      double fitSlopeXmin = tags.getTagDoubleDefault("style.ratio.fitSlope.xmin",xLowerLimit);
      double fitSlopeXmax = tags.getTagDoubleDefault("style.ratio.fitSlope.xmax",xUpperLimit);
      ratioGraph->Fit("pol1","EQF","", fitSlopeXmin, fitSlopeXmax);
      TF1* f = ratioGraph->GetFunction("pol1");
      f->SetName(TString::Format("%s_fit",ratioGraph->GetName()));
      //      this->addObject(f);
      f->SetLineColor(ratioGraph->GetLineColor());
      f->SetLineWidth(tags.getTagIntegerDefault("style.ratio.fitSlope.lineWidth",1));
      f->SetLineStyle(tags.getTagIntegerDefault("style.ratio.fitSlope.lineStyle",2));
      //@tag:style.ratio.fitSlope.printResults: print the fit results on the ratio canvas
      if (tags.getTagBoolDefault("style.ratio.fitSlope.printResults",false)) {
        l.SetTextColor(ratioGraph->GetLineColor());
        double slope = TQUtils::roundAuto(f->GetParameter(1));
        double slopeErr = TQUtils::roundAuto(f->GetParError(1));
        double chi2 = TQUtils::roundAuto(f->GetChisquare());
        TString s = TString::Format("slope #approx %g #pm %g (#chi^{2}#approx%g)",slope,slopeErr,chi2);
        l.DrawLatex(fitResultPrintPosX,fitResultPrintPosY,s);
        fitResultPrintPosY -= fitResultPrintStepY * textsize;
      }
      //@tag:style.ratio.fitSlope.exportResults: export the fit results as tags on the plotter
      if (tags.getTagBoolDefault("style.ratio.fitSlope.exportResults",false)) {
        this->setTagDouble(TString::Format("export.fitSlope.%s.slope",ratioGraph->GetName()),f->GetParameter(1));
        this->setTagDouble(TString::Format("export.fitSlope.%s.slopeError",ratioGraph->GetName()),f->GetParError(1));
        this->setTagDouble(TString::Format("export.fitSlope.%s.chi2",ratioGraph->GetName()),f->GetChisquare());
      }
    }

    ratioGraph->Draw(ratioDrawStyle);

    if(verbose) VERBOSEclass("drawing additional markers");

    this->drawArrows(tags,ratioGraph, actualRatioMin,actualRatioMax);

  }
}


//__________________________________________________________________________________|___________


void TQHWWPlotter::drawMultiRatio(TQTaggable& tags){
  // draw a ratio-plot in the sub-pad
  double ratioMax = tags.getTagDoubleDefault ("style.ratioMax",1000.);
  double ratioMin = tags.getTagDoubleDefault ("style.ratioMin",0.);
  double ratioMaxQerr = tags.getTagDoubleDefault ("style.ratioMaxQerr",std::numeric_limits<double>::infinity());
  bool forceRatioLimits = tags.getTagBoolDefault ("style.forceRatioLimits",false );
  bool verbose = tags.getTagBoolDefault("verbose",false);

  double ratioPadScaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);
  double xscaling = tags.getTagDoubleDefault("geometry.xscaling",1.);

  if(verbose) VERBOSEclass("calculating geometry and axis ranges");
  TString dataLabel("");
  TList* graphs = new TList();
  // graphs->SetOwner(true);

  // actual minimum and maximum ratio
  double actualRatioMin = 1.;
  double actualRatioMax = 1.;

  if(verbose) VERBOSEclass("generating ratio graphs");
  // loop over data histograms
  TQTaggableIterator numerator_processes(fProcesses);
  TQTaggableIterator denominator_processes(fProcesses);
  TString axtitle;
  double xmin = 0;
  double xmax = 0;
  int nbins = 0;
  while(numerator_processes.hasNext() && denominator_processes.hasNext()){
    DEBUGclass(" in the loop");
    // get the data histogram
    TQNamedTaggable* denominator = NULL;
    TQNamedTaggable* numerator = NULL;
    while(!numerator &&  numerator_processes.hasNext()){
      DEBUGclass("Numinator");
      TQNamedTaggable* next = numerator_processes.readNext();
      //if(next->getTagBoolDefault(".isData",false)) data = next;
      if(next->getTagBoolDefault(".isNumerator",false)) numerator = next;
    }

    while(!denominator && denominator_processes.hasNext()){
    //while(!prediction){
      DEBUGclass(" Denominator ");
      TQNamedTaggable* next = denominator_processes.readNext();
      if(next->getTagBoolDefault(".isDenominator",false)) denominator = next;
      // if(next->getTagBoolDefault(".isSignal",false)) prediction = next;
      //if(next->getTagBoolDefault(".isBackground",false)) prediction = next;
      //prediction_processes.reset();
    }

    // TQNamedTaggable* denominator = next_1;
    if(!numerator || !denominator) continue;

    if(verbose) VERBOSEclass("drawing comparsion between %s and %s",numerator->GetName(), denominator->GetName());

    TH1 * h_numerator = this->getObject<TH1>(this->makeHistogramIdentifier(numerator));
    TH1 * h_denominator = this->getObject<TH1>(this->makeHistogramIdentifier(denominator));
    if(!h_numerator) continue;
    if(!h_denominator) continue;
    if(axtitle.IsNull()){
      axtitle = h_numerator->GetXaxis()->GetTitle();
      xmin = h_numerator->GetXaxis()->GetXmin();
      xmax = h_numerator->GetXaxis()->GetXmax();
      nbins = h_numerator->GetNbinsX();
    }

    // calculate the number of valid ratio points: ratio points are considered
    // valid if they have a finite value (MC prediction != 0) (--> nPoints) and
    // the observed data is greater than zero (--> nRatioPoints)
    int nRatioPoints = 0;
    for (int i = 1; i <= h_numerator->GetNbinsX(); i++) {
      double denVal = h_denominator->GetBinContent(i);
      double numVal = h_numerator->GetBinContent(i);
      if(denVal == 0) continue;
      if(numVal == 0) continue;
      if(!TQUtils::isNum(denVal)){
        WARNclass("encountered non-numeric denominator value: %f",denVal);
        continue;
      }
      if(!TQUtils::isNum(numVal)){
        WARNclass("encountered non-numeric numerator value: %f",numVal);
        continue;
      }
      nRatioPoints++;
    }

    if(nRatioPoints < 1){
      // there is nothing to draw -- well, let's do nothing, then
      continue;
    }

    // the graph used to draw the ratio points
    TGraphErrors * ratioGraph = new TGraphErrors(nRatioPoints);
    this->addObject(ratioGraph,TString::Format("ratioGraph_%s",h_numerator->GetName()));
    ratioGraph->SetTitle(TString::Format("%s (ratio)",h_numerator->GetTitle()));
    ratioGraph->SetLineColor(h_numerator->GetLineColor());
    ratioGraph->SetMarkerSize(h_numerator->GetMarkerSize());
    ratioGraph->SetMarkerStyle(h_numerator->GetMarkerStyle());
    ratioGraph->SetMarkerColor(h_numerator->GetMarkerColor());

    int iRatioPoint = 0;

    // loop over all bins of the histogram
    for (int iBin = 1; iBin <= h_numerator->GetNbinsX(); iBin++) {
      double x = h_denominator->GetBinCenter(iBin);
      // get the values and errors of data and MC for this bin
      double num = h_numerator ->GetBinContent(iBin);
      double numErr = h_numerator ->GetBinError (iBin);
      double den = h_denominator->GetBinContent(iBin);
      // cannot do anything if MC expectation is zero
      if (den == 0. || num <= 0.) continue;

      double ratio = num / den;
      double ratioError = ratio * numErr / num;
      if(verbose) VERBOSEclass("adding ratio point with x=%f, y=%f (numerator=%f, denominator=%f)",x,ratio,num,den);
      ratioGraph->SetPoint(iRatioPoint, x, ratio);
      ratioGraph->SetPointError(iRatioPoint, 0., ratioError);
      iRatioPoint++;
    }

    this->applyStyle(tags   ,ratioGraph,"sub.data",1.,ratioPadScaling);

    double ratioMinAllowed = tags.getTagDoubleDefault ("style.ratioMinAllowed",ratioMin);
    double ratioMaxAllowed = tags.getTagDoubleDefault ("style.ratioMaxAllowed",ratioMax);
    actualRatioMin=ratioMinAllowed;
    actualRatioMax=ratioMaxAllowed;
    if(verbose) VERBOSEclass("drawMultiRatio: allowed range of ratio graph: %f -- %f",actualRatioMin,actualRatioMax);

    this->estimateRangeY(ratioGraph,actualRatioMin,actualRatioMax,ratioMaxQerr);

    if(verbose) VERBOSEclass("drawMultiRatio: estimated range of ratio graph: %f -- %f (ratioMaxQerr=%f)",actualRatioMin,actualRatioMax,ratioMaxQerr);

    if(actualRatioMin == actualRatioMax){
      if(verbose) VERBOSEclass("expanding multi ratio to not be empty");
      actualRatioMin *= 0.9;
      actualRatioMax *= 1.1;
    }

    if (forceRatioLimits)
      actualRatioMin = ratioMin;
    else
      actualRatioMin = actualRatioMin-0.1*(actualRatioMax-actualRatioMin);

    if (forceRatioLimits)
      actualRatioMax = ratioMax;
    else
      actualRatioMax = actualRatioMax+0.1*(actualRatioMax-actualRatioMin);

    if(verbose) VERBOSEclass("drawMultiRatio: final of ratio graph: %f -- %f",actualRatioMin,actualRatioMax);

    DEBUGclass(" ratio ");
    graphs->Add(ratioGraph);
  }

  if(verbose) VERBOSEclass("built %d ratio graphs",graphs->GetEntries());

  TString label = tags.getTagStringDefault("labels.ratio","ratio");

  gStyle->SetEndErrorSize(0);

  if(verbose) VERBOSEclass("drawing graphs, range is %g < x %g",xmin,xmax);

  TQGraphErrorsIterator itr2(graphs);
  bool first = true;
  while(itr2.hasNext()){
    TGraphErrors* ratioGraph = itr2.readNext();
    if(!ratioGraph) continue;
    ratioGraph->SetMinimum(actualRatioMin);
    ratioGraph->SetMaximum(actualRatioMax);
    DEBUGclass(" in the loop iiter next");
    ratioGraph->Draw(first ? "AP" : "P SAME");
    if(first){
      ratioGraph->GetYaxis()->SetTitle(label);
      ratioGraph->GetXaxis()->SetTitle(axtitle);
      ratioGraph->GetXaxis()->Set(nbins,xmin,xmax);
      this->applyGeometry(tags,ratioGraph,"main" ,xscaling,ratioPadScaling);
      this->applyGeometry(tags,ratioGraph,"sub" ,xscaling,ratioPadScaling);
    }
    first = false;
    this->drawArrows(tags,ratioGraph, actualRatioMin,actualRatioMax);
  }
}

/*
void TQHWWPlotter::drawMultiRatio(TQTaggable& tags){
  // draw a ratio-plot in the sub-pad
  double ratioMax = tags.getTagDoubleDefault ("style.ratioMax",1000.);
  double ratioMin = tags.getTagDoubleDefault ("style.ratioMin",0.);
  double ratioMaxQerr = tags.getTagDoubleDefault ("style.ratioMaxQerr",std::numeric_limits<double>::infinity());
  bool forceRatioLimits = tags.getTagBoolDefault ("style.forceRatioLimits",false );
  bool verbose = tags.getTagBoolDefault("verbose",false);

  double ratioPadScaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);
  double xscaling = tags.getTagDoubleDefault("geometry.xscaling",1.);

  if(verbose) VERBOSEclass("calculating geometry and axis ranges");

  TString dataLabel("");
  TList* graphs = new TList();
  // graphs->SetOwner(true);

  // actual minimum and maximum ratio
  double actualRatioMin = 1.;
  double actualRatioMax = 1.;

  if(verbose) VERBOSEclass("generating ratio graphs");
  // loop over data histograms
  TQTaggableIterator data_processes(fProcesses);
  TQTaggableIterator prediction_processes(fProcesses);
  TString axtitle;
  double xmin = 0;
  double xmax = 0;

  while(data_processes.hasNext() && prediction_processes.hasNext()){
    // get the data histogram
    TQNamedTaggable* data = NULL;
    while(!data &&  data_processes.hasNext()){
      TQNamedTaggable* next = data_processes.readNext();
      if(next->getTagBoolDefault(".isData",false)) data = next;
    }
    TQNamedTaggable* prediction = NULL;
    while(!prediction && prediction_processes.hasNext()){
      TQNamedTaggable* next = prediction_processes.readNext();
      if(next->getTagBoolDefault(".isSignal",false)) prediction = next;
    }
    if(!data || !prediction) continue;

    if(verbose) VERBOSEclass("drawing comparsion between %s and %s",data->GetName(), prediction->GetName());

    TH1 * h_data = this->getObject<TH1>(this->makeHistogramIdentifier(data));
    TH1 * h_prediction = this->getObject<TH1>(this->makeHistogramIdentifier(prediction));
    if(!h_data) continue;
    if(!h_prediction) continue;

    if(axtitle.IsNull()){
      axtitle = h_data->GetXaxis()->GetTitle();
      xmin = h_data->GetXaxis()->GetXmin();
      xmax = h_data->GetXaxis()->GetXmax();
    }

    // calculate the number of valid ratio points: ratio points are considered
    // valid if they have a finite value (MC prediction != 0) (--> nPoints) and
    // the observed data is greater than zero (--> nRatioPoints)
    int nRatioPoints = 0;
    for (int i = 1; i <= h_data->GetNbinsX(); i++) {
      double mcVal = h_prediction->GetBinContent(i);
      double dataVal = h_data->GetBinContent(i);
      if(mcVal == 0) continue;
      if(dataVal == 0) continue;
      if(!TQUtils::isNum(mcVal)){
        WARNclass("encountered non-numeric MC value: %f",mcVal);
        continue;
      }
      if(!TQUtils::isNum(dataVal)){
        WARNclass("encountered non-numeric data value: %f",dataVal);
        continue;
      }
      nRatioPoints++;
    }

    if(nRatioPoints < 1){
      // there is nothing to draw -- well, let's do nothing, then
      continue;
    }

    // the graph used to draw the ratio points
    TGraphErrors * ratioGraph = new TGraphErrors(nRatioPoints);
    this->addObject(ratioGraph,TString::Format("ratioGraph_%s",h_data->GetName()));
    ratioGraph->SetTitle(TString::Format("%s (ratio)",h_data->GetTitle()));
    ratioGraph->SetLineColor(h_data->GetLineColor());
    ratioGraph->SetMarkerSize(h_data->GetMarkerSize());
    ratioGraph->SetMarkerStyle(h_data->GetMarkerStyle());
    ratioGraph->SetMarkerColor(h_data->GetMarkerColor());

    int iRatioPoint = 0;

    // loop over all bins of the histogram
    for (int iBin = 1; iBin <= h_data->GetNbinsX(); iBin++) {
      double x = h_prediction->GetBinCenter(iBin);
      // get the values and errors of data and MC for this bin
      double data = h_data ->GetBinContent(iBin);
      double dataErr = h_data ->GetBinError (iBin);
      double MC = h_prediction->GetBinContent(iBin);
      // cannot do anything if MC expectation is zero
      if (MC == 0. || data <= 0.) continue;

      double ratio = data / MC;
      double ratioError = ratio * dataErr / data;
      if(verbose) VERBOSEclass("adding ratio point with x=%f, y=%f (data=%f, MC=%f)",x,ratio,data,MC);
      ratioGraph->SetPoint(iRatioPoint, x, ratio);
      ratioGraph->SetPointError(iRatioPoint, 0., ratioError);
      iRatioPoint++;
    }

    this->applyStyle(tags   ,ratioGraph,"sub.data",1.,ratioPadScaling);

    double ratioMinAllowed = tags.getTagDoubleDefault ("style.ratioMinAllowed",ratioMin);
    double ratioMaxAllowed = tags.getTagDoubleDefault ("style.ratioMaxAllowed",ratioMax);
    actualRatioMin=ratioMinAllowed;
    actualRatioMax=ratioMaxAllowed;
    if(verbose) VERBOSEclass("drawMultiRatio: allowed range of ratio graph: %f -- %f",actualRatioMin,actualRatioMax);

    this->estimateRangeY(ratioGraph,actualRatioMin,actualRatioMax,ratioMaxQerr);

    if(verbose) VERBOSEclass("drawMultiRatio: estimated range of ratio graph: %f -- %f (ratioMaxQerr=%f)",actualRatioMin,actualRatioMax,ratioMaxQerr);

    if(actualRatioMin == actualRatioMax){
      if(verbose) VERBOSEclass("expanding multi ratio to not be empty");
      actualRatioMin *= 0.9;
      actualRatioMax *= 1.1;
    }

    if (forceRatioLimits)
      actualRatioMin = ratioMin;
    else
      actualRatioMin = actualRatioMin-0.1*(actualRatioMax-actualRatioMin);

    if (forceRatioLimits)
      actualRatioMax = ratioMax;
    else
      actualRatioMax = actualRatioMax+0.1*(actualRatioMax-actualRatioMin);

    if(verbose) VERBOSEclass("drawMulti Ratio: final of ratio graph: %f -- %f",actualRatioMin,actualRatioMax);

    graphs->Add(ratioGraph);
  }

  if(verbose) VERBOSEclass("built %d ratio graphs",graphs->GetEntries());

  TString label = tags.getTagStringDefault("labels.ratio","ratio");

  gStyle->SetEndErrorSize(0);

  if(verbose) VERBOSEclass("drawing line");

  TQGraphErrorsIterator itr2(graphs);
  bool first = true;
  while(itr2.hasNext()){
    TGraphErrors* ratioGraph = itr2.readNext();
    if(!ratioGraph) continue;
    ratioGraph->SetMinimum(actualRatioMin);
    ratioGraph->SetMaximum(actualRatioMax);
    ratioGraph->Draw(first ? "AL" : "L SAME");
    if(first){
      ratioGraph->GetYaxis()->SetTitle(label);
      ratioGraph->GetXaxis()->SetTitle(axtitle);
      ratioGraph->GetXaxis()->SetRangeUser(xmin,xmax);
      this->applyGeometry(tags,ratioGraph,"sub" ,xscaling,ratioPadScaling);
    }
    first = false;
    this->drawArrows(tags,ratioGraph, actualRatioMin,actualRatioMax);
  }
}

*/

//__________________________________________________________________________________|___________

void TQHWWPlotter::drawOptScan(TQTaggable& tags){
  // draw a cut optimization scan in the sub-pad
  double xscaling = tags.getTagDoubleDefault("geometry.xscaling",1.);
  double subPadScaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);
  bool verbose = tags.getTagBoolDefault("verbose",false);

  TH1* hTotalStack = this->getObject<TH1>("totalStack");
  TQTaggableIterator itr(this->fProcesses);
  TH1* hSig = NULL;
  while(itr.hasNext() && !hSig){
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(process->getTagBoolDefault(".isSignal",false)){
      hSig = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    }
  }


  if(!hTotalStack){
    return;
  }
  if(!hSig){
    return;
  }

  TPad* optscanPad = this->getPad("sub");
  optscanPad->SetGridy(0);

  // figure of merit (FOM) plot. e.g. S/sqrt(B) and etc. one can implement more
  TH1* hFOMl = 0;
  TH1* hFOMr = 0;
  TH1* hFOM = 0;

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
  
  double actualmax = 0;
  if(binByBin){
    if(verbose){
      VERBOSEclass("drawing bin-by-bin significances with FOM=%s for S=%s and B=%s",TQHistogramUtils::getFOMTitleROOT(FOMmode).Data(),hSig->GetTitle(),hTotalStack->GetTitle());
    }
    hFOM = TQHistogramUtils::getFOMHistogram(FOMmode,hSig, hTotalStack, 0, bkgSystHistos);
    actualmax = hFOM->GetMaximum() * tags.getTagDoubleDefault("optScan.enlargeY",1.3);
    this->addObject(hFOM,"hist_FOM_bbb");
  } else {
    if(verbose){
      VERBOSEclass("drawing optimization scan with FOM=%s for S=%s and B=%s",TQHistogramUtils::getFOMTitleROOT(FOMmode).Data(),hSig->GetTitle(),hTotalStack->GetTitle());
    }
    hFOMl = TQHistogramUtils::getFOMScan(FOMmode,hSig, hTotalStack, true,0.05,verbose, bkgSystHistos);
    hFOMr = TQHistogramUtils::getFOMScan(FOMmode,hSig, hTotalStack, false,0.05,verbose, bkgSystHistos);
    //@tag:optScan.autoselect: select the optimization scan (left,right) that is better suited for every histogram and only show that one (default:false)
    if(tags.getTagBoolDefault("optScan.autoselect",false)){
      if(verbose) VERBOSEclass("autoselecting opt scan");
      if(TQHistogramUtils::hasGreaterMaximumThan(hFOMr,hFOMl)){
        if(verbose) VERBOSEclass("removing left-hand FOM histogram");
        this->addObject(hFOMr,"hist_FOM");
        delete hFOMl;
        hFOMl = NULL;
        actualmax = hFOMr->GetMaximum() * tags.getTagDoubleDefault("optScan.enlargeY",1.3);
      } else {
        if(verbose) VERBOSEclass("removing right-hand FOM histogram");
        this->addObject(hFOMl,"hist_FOM");
        delete hFOMr;
        hFOMr = NULL;
        actualmax = hFOMl->GetMaximum() * tags.getTagDoubleDefault("optScan.enlargeY",1.3);
      }
    } else {
      if(verbose) VERBOSEclass("using all opt scans");
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

  bool first = true;
  // set style
  if (hFOM) {
    //@tag:style.optScan.default.*: control styling of the auto-selected FOM graph
    if(verbose) VERBOSEclass("drawing FOM histogram");
    this->applyStyle (tags,hFOM,"optScan.default",xscaling,subPadScaling);
    this->applyGeometry(tags,hFOM,"main" ,xscaling,subPadScaling);
    this->applyGeometry(tags,hFOM,"sub" ,xscaling,subPadScaling);
    hFOM->SetMaximum(actualmax);
    hFOM->SetMinimum(0);
    hFOM->SetNdivisions(50008);
    hFOM->GetYaxis()->SetNdivisions(50004);
    hFOM->Draw(first ? "HIST" : "HIST SAME");
    first = false;
  }
  if (hFOMl) {
    //@tag:style.optScan.left.*: control styling of the left-hand-side (lower) FOM graph
    if(verbose) VERBOSEclass("drawing FOM histogram (lhs)");
    this->applyStyle (tags,hFOMl,"optScan.left",xscaling,subPadScaling);
    this->applyGeometry(tags,hFOMl,"main" ,xscaling,subPadScaling);
    this->applyGeometry(tags,hFOMl,"sub" ,xscaling,subPadScaling);
    hFOMl->SetFillStyle(0);
    hFOMl->SetMaximum(actualmax);
    hFOMl->SetNdivisions(50008);
    hFOMl->GetYaxis()->SetNdivisions(50004);
    hFOMl->SetMinimum(0);
    hFOMl->Draw(first ? "HIST" : "HIST SAME");
    first = false;
  }
  if (hFOMr) {
    //@tag:style.optScan.right.*: control styling of the right-hand-side (upper) FOM graph
    if(verbose) VERBOSEclass("drawing FOM histogram (rhs)");
    this->applyStyle (tags,hFOMr,"optScan.right",xscaling,subPadScaling);
    this->applyGeometry(tags,hFOMr,"main" ,xscaling,subPadScaling);
    this->applyGeometry(tags,hFOMr,"sub" ,xscaling,subPadScaling);
    hFOMr->SetFillStyle(0);
    hFOMr->SetMaximum(actualmax);
    hFOMr->SetNdivisions(50008);
    hFOMr->GetYaxis()->SetNdivisions(50004);
    hFOMr->SetMinimum(0);
    hFOMr->Draw(first ? "HIST" : "HIST SAME");
    first = false;
  }

  // TLegend
  if(drawLegend){
    TLegend * leg = new TLegend(0.21,0.85,0.93,0.95);
    leg->SetNColumns(2);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetLineColor(0);
    leg->SetLineWidth(0);

    if (FOMmode == TQHistogramUtils::kSoSqB){
      if(hFOMr){
        double rmax = hFOMr->GetBinContent(hFOMr->GetMaximumBin());
        double rmaxxval = hFOMr->GetBinLowEdge(hFOMr->GetMaximumBin()) + hFOMr->GetBinWidth(hFOMr->GetMaximumBin());
        leg->AddEntry(hFOMr,TString::Format("#leftarrow cut Max=%.2f (%.2f)",rmax,rmaxxval), "l");
      }
      if(hFOMl){
        double lmax = hFOMl->GetBinContent(hFOMl->GetMaximumBin());
        double lmaxxval = hFOMl->GetBinLowEdge(hFOMl->GetMaximumBin());
        leg->AddEntry(hFOMl,TString::Format("#rightarrow cut Max=%.2f (%.2f)",lmax,lmaxxval), "l");
      }
    } else {
      if(hFOMl) leg->AddEntry(hFOMl,"#rightarrow cut", "l");
      if(hFOMr) leg->AddEntry(hFOMr,"#leftarrow cut" , "l");
    }
    leg->Draw("SAME");
  }

}

//__________________________________________________________________________________|___________

void TQHWWPlotter::drawDataMinusBackground(TQTaggable& tags){
  // draw a data-minus-background plot in the sub-pad

  TString subtractionName = tags.getTagStringDefault("style.DminusB.subtraction","totalBkg");
  TH1* subtraction = this->getObject<TH1>(subtractionName);
  if(!subtraction) return;

  TString errorSourceName = tags.getTagStringDefault("style.DminusB.errors","totalBkg");
  TH1* errorSource = this->getObject<TH1>(errorSourceName);
  if(!errorSource) return;

  // set the x range of the dmb graph to match the one of the main histogram
  double dmbPadScaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);

  TString dataLabel("");

  double max = 0;
  double min = 0;

  std::vector<TH1*> v_signal;
  std::vector<TH1*> v_data;

  TQTaggableIterator itrSig(fProcesses);
  while(itrSig.hasNext()){
    TQNamedTaggable* process = itrSig.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(".isSignal",false)) continue;
    TH1* h_sig = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    h_sig = TQHistogramUtils::copyHistogram(h_sig,TString::Format("%s_dminusb",h_sig->GetName()));
    this->applyStyle(tags,h_sig,"sub.sig",1.,dmbPadScaling);
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
    this->applyStyle(tags,h_data,"sub.data",1.,dmbPadScaling);
    TQHistogramUtils::addHistogramWithoutError(h_data,subtraction,-1.);
    max = std::max(max,TQHistogramUtils::getMax(h_data,true));
    min = std::min(min,TQHistogramUtils::getMin(h_data,true));
    v_data.push_back(h_data);
  }

  gStyle->SetEndErrorSize(0);

  TGraphErrors* errorGraph = new TGraphErrors(errorSource->GetNbinsX());
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
  this->applyGeometry(tags,frame,"main",1.,dmbPadScaling);
  this->applyGeometry(tags,frame,"sub",1.,dmbPadScaling);

  frame->GetYaxis()->SetTitle(tags.getTagStringDefault("labels.axes.subY", "Data-Bkg."));
  //@tag:labels.axes.subY: y-axis subplot label

  frame->Draw("HIST");

  // sort by integral to have smallest contribution in front
  std::sort(v_signal.begin(),v_signal.end(),[&](TH1* a, TH1* b){ return a->GetSumOfWeights() >= b->GetSumOfWeights();});

  for(auto h:v_signal){
    h->Draw("HISTSAME");
  }

  errorGraph->SetTitle("mc error band");
  this->applyStyle(tags,errorGraph,"main.totalStackError",1.,dmbPadScaling);
  errorGraph->Draw("E2SAME");

  for(auto h:v_data){
    h->Draw("EPSAME");
  }

  // if 0. is included in the range of the y axis of the dmb plot...
  if (min <= 0. && max >= 0.){
    // draw the black line indicating 0 in the dmb
    TLine* line = new TLine();
    line->SetLineColor(kBlack);
    line->DrawLine(TQHistogramUtils::getAxisXmin(subtraction), 0., TQHistogramUtils::getAxisXmax(subtraction), 0.);
    delete line;
  }
}

//__________________________________________________________________________________|___________

void TQHWWPlotter::drawDataMinusBackgroundOverData(TQTaggable& tags){
  // draw a data-minus-background plot in the sub-pad

  TString subtractionName = tags.getTagStringDefault("style.DminusB.subtraction","totalBkg");
  TH1* subtraction = this->getObject<TH1>(subtractionName);
  if(!subtraction) return;

  TString errorSourceName = tags.getTagStringDefault("style.DminusB.errors","totalBkg");
  TH1* errorSource = this->getObject<TH1>(errorSourceName);
  if(!errorSource) return;

  // set the x range of the dmb graph to match the one of the main histogram
  double dmbPadScaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);

  TString dataLabel("");

  double max = 0;
  double min = 0;

  std::vector<TH1*> v_signal;
  std::vector<TH1*> v_data;

  // loop over data histograms
  TQTaggableIterator itr(fProcesses);
  while(itr.hasNext()){
    // get the data histogram
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(".isData",false)) continue;
    TH1 * h_data = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    h_data = TQHistogramUtils::copyHistogram(h_data,TString::Format("%s_minus_bkg_over_d",h_data->GetName()));
    if(!h_data) continue;
    this->applyStyle(tags,h_data,"sub.data",1.,dmbPadScaling);
    v_data.push_back(h_data);
  }

  if(v_data.size() != 1){
    ERRORclass("cannot plot data minus background over data when the number of datasets is != 1");
  }
  TH1* data = v_data[0];

  TQTaggableIterator itrSig(fProcesses);
  while(itrSig.hasNext()){
    TQNamedTaggable* process = itrSig.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(".isSignal",false)) continue;
    TH1* h_sig = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    h_sig = TQHistogramUtils::copyHistogram(h_sig,TString::Format("%s_dminusboverd",h_sig->GetName()));
    TQHistogramUtils::divideHistogramWithoutError(h_sig,data);
    this->applyStyle(tags,h_sig,"sub.sig",1.,dmbPadScaling);
    max = std::max(max,TQHistogramUtils::getMax(h_sig,false));
    min = std::min(min,TQHistogramUtils::getMin(h_sig,false));
    if(!h_sig) continue;
    v_signal.push_back(h_sig);
  }

  gStyle->SetEndErrorSize(0);

  TGraphErrors* errorGraph = new TGraphErrors(errorSource->GetNbinsX());
  for(int i=1; i<=errorSource->GetNbinsX(); ++i){
    double val = 1000;
    if (data->GetBinContent(i) != 0){
      val = errorSource->GetBinError(i)/data->GetBinContent(i);
    }
    double point = (data->GetBinContent(i)-subtraction->GetBinContent(i))/data->GetBinContent(i);
    if (val < 1000){
      max = std::max(max, 1.1*(abs(point)+val));
      min = std::min(min, -1.1*(abs(point)+val));
    }
    else {
      max = std::max(max, 1.1*(abs(point)));
      min = std::min(min, -1.1*(abs(point)));
    }

    errorGraph->SetPoint(i,errorSource->GetBinCenter(i),(errorSource->GetBinContent(i)-subtraction->GetBinContent(i))/data->GetBinContent(i));
    errorGraph->SetPointError(i,errorSource->GetBinCenter(i)-errorSource->GetBinLowEdge(i),val);
  }

  //@tag:[sub.yMin, sub.yMax] Overwrites automatic axis range of sub plot when drawing drawDataMinusBackgroundOverData
  tags.getTagDouble("sub.yMin", min);
  tags.getTagDouble("sub.yMax", max);

  double margin = tags.getTagDoubleDefault("style.sub.margin",1.1);

  TH1* frame = TQHistogramUtils::copyHistogram(subtraction,"Graph_subFrame");
  frame->Reset();
  frame->SetMaximum(margin*max);
  frame->SetMinimum(margin*min);
  this->applyStyle (tags,frame,"sub");
  this->applyGeometry(tags,frame,"main",1.,dmbPadScaling);
  this->applyGeometry(tags,frame,"sub",1.,dmbPadScaling);

  TString subXName = tags.getTagStringDefault("labels.axes.subX", "");
  //@tag:labels.axes.subX: x-axis subplot label
  
  frame->GetYaxis()->SetTitle(tags.getTagStringDefault("labels.axes.subY", "(Data-Bkg.)/Data"));
  if (subXName.Length()>0) {
    frame->GetXaxis()->SetTitle(subXName);
  }
  frame->Draw("HIST");

  // sort by integral to have smallest contribution in front
  std::sort(v_signal.begin(),v_signal.end(),[&](TH1* a, TH1* b){ return a->GetSumOfWeights() >= b->GetSumOfWeights();});
  for(auto h:v_signal){
    h->Draw("HISTSAME");
  }
  errorGraph->SetTitle("mc error band");
  this->applyStyle(tags,errorGraph,"main.totalStackError",1.,dmbPadScaling);
  errorGraph->Draw("E2SAME");

  TH1* copydata = TQHistogramUtils::copyHistogram(data);
  TQHistogramUtils::addHistogramWithoutError(data,subtraction,-1.);
  TQHistogramUtils::divideHistogramWithoutError(data,copydata);
  delete copydata;

  // X0 don't draw horizontal error bar
  data->Draw("E P SAME X0");

  // Plotting with the e0 option draws errorbars also if the datapoint
  // is outside of the pad (that's what we want), but it draws datapoints
  // when they are at 0 (we don't want that).
  // Workaround: set the marker size to 0 and draw with e0 option. For
  // some reason, plotting the original histogram messes things up. So
  // use a clone instead.
  TH1* dataClone = (TH1*) data->Clone();
  dataClone->SetMarkerSize(0);
  dataClone->Draw("SAME P e0 X0");

  // if 1. is included in the range of the y axis of the dmb plot...
  if (min <= 1. && max >= 1.){
    // draw the black line indicating 0 in the dmb
    TLine* line = new TLine();
    line->SetLineColor(kBlack);
    line->DrawLine(TQHistogramUtils::getAxisXmin(subtraction), 0., TQHistogramUtils::getAxisXmax(subtraction), 0.);
    delete line;
  }

  this->drawArrows(tags, data, frame->GetMinimum(), frame->GetMaximum());
}
 
void TQHWWPlotter::drawUnitNormSubPlot(TQTaggable& tags){
   
  gPad->SetGridy(0);

  double unitNormPadScaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);
  double margin = tags.getTagDoubleDefault("style.sub.margin",1.1);
  double max = 0;
  double min = 0;

  std::vector<TH1*> v_Sig;
  std::vector<TH1*> v_Bkg;
  std::vector<TH1*> v_SigBkg;
  std::vector<TH1*> v_SigBkg_bkwds;

  // Loop over signal histograms and scale them to unit norm
  TQTaggableIterator itrSig(fProcesses);
  while(itrSig.hasNext()){
    TQNamedTaggable* process = itrSig.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(".isSignal",false)) continue;
    TH1 * histo = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    histo = TQHistogramUtils::copyHistogram(histo,TString::Format("%s_unitNorm",histo->GetName()));
    histo->Scale(1./histo->Integral());
    this->applyStyle(tags,histo,"sub",1.,unitNormPadScaling);
    this->applyGeometry(tags,histo,"sub",1.,unitNormPadScaling);
    v_Sig.push_back(histo);
    if (!histo) continue;
  }

  // Loop over background histograms and scale them to unit norm
  TQTaggableIterator itrBkg(fProcesses);
  while(itrBkg.hasNext()){
    TQNamedTaggable* process = itrBkg.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(".isBackground",false)) continue;
    TH1 * histo = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    histo = TQHistogramUtils::copyHistogram(histo,TString::Format("%s_unitNorm",histo->GetName()));
    histo->Scale(1./histo->Integral());
    this->applyStyle(tags,histo,"sub",1.,unitNormPadScaling);
    this->applyGeometry(tags,histo,"sub",1.,unitNormPadScaling);
    v_Bkg.push_back(histo);
    if (!histo) continue;
  }

  // HS: Not sure how to fully deal with the plotting order to ensure the legends
  // in the main pad and sub pad are identical.
  // For now, I've put the signal histograms in the vector of histograms to
  // plot first, and then the background ones (but swapping the order)
  for(auto h:v_Sig){ 
    v_SigBkg.push_back(h);
  }
  for (Int_t i = v_Bkg.size() - 1; i >= 0; --i ){
    v_SigBkg.push_back(v_Bkg[i]);
    v_SigBkg_bkwds.push_back(v_Bkg[i]);
  } 
  for (auto h:v_Sig){
    v_SigBkg_bkwds.push_back(h);
  }
  // v_SigBkg has signal first, followed by background histograms
  // v_SigBkg_bkwds has background histograms first, and then signal last. This works for getting 
  // the drawing order right for the histograms, and for filling the legend such that it is the 
  // same as the legend in the main pad

  TH1* hMain = TQHistogramUtils::copyHistogram(v_SigBkg[0],"Graph_subFrame");
  hMain->Reset();
  hMain->SetMaximum(margin*max);
  hMain->SetMinimum(margin*min);
  this->applyStyle(tags,hMain,"sub");
  this->applyGeometry(tags,hMain,"main",1.,unitNormPadScaling);
  this->applyGeometry(tags,hMain,"sub",1.,unitNormPadScaling);

  hMain->GetYaxis()->SetTitle(tags.getTagStringDefault("labels.axes.subY", "Unit norm."));
  hMain->GetXaxis()->SetTitle(tags.getTagStringDefault("labels.axes.subX", ""));
  hMain->Draw("HIST");

  // Add a legend to the subplot
  double leg_xmin = tags.getTagDoubleDefault("subLegend.xMin",0.65);
  double leg_xmax = tags.getTagDoubleDefault("subLegend.xMax",0.95);
  double leg_ymin = tags.getTagDoubleDefault("subLegend.yMin",0.58);
  double leg_ymax = tags.getTagDoubleDefault("subLegend.yMax",0.90);
  double leg_textSize = tags.getTagDoubleDefault("subLegend.textSize", 0.065); 

  TLegend* fLegend_sub = new TLegend(leg_xmin, leg_ymin, leg_xmax, leg_ymax);
  int nColumnsSubLegend=tags.getTagIntegerDefault("subLegend.nColumns",2);
  fLegend_sub->SetNColumns(nColumnsSubLegend);
  fLegend_sub->SetFillColor(0);
  fLegend_sub->SetFillStyle(0);
  fLegend_sub->SetBorderSize(0);
  fLegend_sub->SetTextFont(42);
  fLegend_sub->SetTextSize(leg_textSize);
  fLegend_sub->SetShadowColor(kWhite);

  double maxVal = 0.;
  double max_t = 0.;

  //@tags: [style.fillSignal] Tag to choose whether the signal is drawn with a fill colour or not 
  //@tags: [style.fillSignalName] The name of the signal histogram to draw with a fill colour, default sigGGF
  bool fillSignal = tags.getTagBoolDefault("style.fillSignal", false);
  TString fillSignalName = tags.getTagStringDefault("style.fillSignalName", "sigGGF");

  // If fillSignal is true, draw signal first so other lines are drawn over it, so use v_SigBkg
  if (fillSignal) {
    for(auto h:v_SigBkg){
      max_t = h->GetBinContent(h->GetMaximumBin());
      if (max_t > maxVal) {
	maxVal = max_t;
      }
      
      TString title = h->GetTitle();
      title = TQStringUtils::convertLaTeX2ROOTTeX(title);
      TString histoName = h->GetName();
      
      //The line colour of the unit norm histos should be the same as the fill color for the histos in the main pad
      h->SetLineColor(h->GetFillColor());
      
      if (fillSignal && histoName.Contains(fillSignalName)) {
	h->SetFillColor(h->GetFillColor());
	fLegend_sub->AddEntry(h, title, "FL");
      }    
      else{
	h->SetFillColor(0);
	fLegend_sub->AddEntry(h, title, "L");
      }
      h->SetLineWidth(3);
      h->Draw("HISTSAME");
      
    }
  }
  // If not fillSignal, draw signal last so the line is clearly seen, so use v_SigBkg_bkwds. But, to keep the legend order the same as the main pad, use v_SigBkg for legend
  else {
    for(auto h:v_SigBkg_bkwds) {
      max_t = h->GetBinContent(h->GetMaximumBin());
      if (max_t > maxVal) {
        maxVal = max_t;
      }
      
      //The line colour of the unit norm histos should be the same as the fill color for the histos in the main pad                                               
      h->SetLineColor(h->GetFillColor());
      h->SetFillColor(0);
      h->SetLineWidth(3);
      h->Draw("HISTSAME");
    }
    for(auto h:v_SigBkg) {
      TString title = h->GetTitle();
      title = TQStringUtils::convertLaTeX2ROOTTeX(title);
      TString histoName = h->GetName();
      fLegend_sub->AddEntry(h, title, "L");
    }
  }
  
  fLegend_sub->Draw();
  
  double yMaxSubPlot = tags.getTagDoubleDefault("sub.yMax",maxVal*1.25);
  double yMinSubPlot = tags.getTagDoubleDefault("sub.yMin",0.0);
  hMain->GetYaxis()->SetRangeUser(yMinSubPlot, yMaxSubPlot);

  //@tags: style.cutLineSubPlot] If including a cut line, also include it in the subpad
  bool cutLineSubPlot = tags.getTagBoolDefault("style.cutLineSubPlot", false);
  if (cutLineSubPlot) {
    double cutVal = 0.;
    int it= 0.;
    //@tags: cuts.*: a list of (vertical) cut lines to be drawn. value will be x-value of the vertical line
    while (tags.getTagDouble(TString::Format("cuts.%d", it++), cutVal)) { 
      TLine * line_sub = new TLine(cutVal, yMinSubPlot, cutVal, yMaxSubPlot);
      line_sub->SetLineStyle(tags.getTagIntegerDefault("style.cutLineStyle",7));
      line_sub->SetLineWidth(tags.getTagIntegerDefault("style.cutLineWidth",2));
      line_sub->SetLineColor(tags.getTagIntegerDefault("style.cutLineColor",kRed));
      line_sub->Draw();
    }
  }
  
}

void TQHWWPlotter::drawCustomSubPlot(TQTaggable& tags){
  // draw a significance plot in the sub-pad
  TString totalStackLabesl = tags.getTagStringDefault ("labels.totalStack", "SM");
  TString formulaString = tags.getTagStringDefault ("style.ratioFormula","d/b");
  TString formulaName = tags.getTagStringDefault ("style.ratioName",formulaString);
  double min = tags.getTagDoubleDefault("style.customSubMin",0.);
  double max = tags.getTagDoubleDefault("style.customSubMax",0.);

  TH1* hMaster = this->getObject<TH1>("Graph_master");
  TH1* hTotalStack = this->getObject<TH1>("totalStack");
  TH1* hTotalSig = this->getObject<TH1>("totalSig");
  if(!hTotalStack) return;

  int nBins = hMaster->GetNbinsX();

  // loop over all histograms
  TQTaggableIterator itr(fProcesses);
  while(itr.hasNext()){
    // get the data histograms only
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(".isData",false)) continue;
    TH1 * hdata = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    if (!hdata) continue;



    // the graph used to draw the values (errors may be suported at a later time)
    //TGraphAsymmErrors * customGraph = new TGraphAsymmErrors(nBins);
    TGraph * customGraph = new TGraph(nBins);
    customGraph->SetLineColor(hdata->GetLineColor());
    customGraph->SetMarkerSize(hdata->GetMarkerSize());
    customGraph->SetMarkerStyle(hdata->GetMarkerStyle());
    customGraph->SetMarkerColor(hdata->GetMarkerColor());

    this->addObject(customGraph,TString::Format("custom_%s",hdata->GetName()));

    int iPoint = 0;

    // actual minimum and maximum value
    double actualSigMin = 0.;
    double actualSigMax = 0.;

    bool first = true;
    // loop over all bins of the histogram
    for (int iBin = 1; iBin <= nBins; iBin++) {
      TString localFormula = formulaString;

      // retrieve required values and fill them into the formula provided

      //watch the order in which expressions are replaced!!
      //@tags:style.ratioFormula: arbitrary formula that can be defined using the following placeholders: sHighXbinWidth,sLowXbinWidth,sHighXbinWidth,sLow,sHigh,sig,bHighXbinWidth,bLowXbinWidth,bLow,bHigh,bkg,dHighXbinWidth,dLowXbinWidthdLow,dHigh,data,binN,binX,binWidth
      //@tags:style.ratioName: name of the custom subplot to be shown
      localFormula = TQStringUtils::replace(localFormula,"sLowXbinWidth",std::to_string(hTotalSig->Integral(0,iBin,"width")));
      localFormula = TQStringUtils::replace(localFormula,"sHighXbinWidth",std::to_string(hTotalSig->Integral(iBin,nBins+1,"width")));
      localFormula = TQStringUtils::replace(localFormula,"sLow",std::to_string(hTotalSig->Integral(0,iBin)));
      localFormula = TQStringUtils::replace(localFormula,"sHigh",std::to_string(hTotalSig->Integral(iBin,nBins+1)));
      localFormula = TQStringUtils::replace(localFormula,"sig",std::to_string(hTotalSig->GetBinContent(iBin)));

      localFormula = TQStringUtils::replace(localFormula,"bLowXbinWidth",std::to_string(hTotalStack->Integral(0,iBin,"width")));
      localFormula = TQStringUtils::replace(localFormula,"bHighXbinWidth",std::to_string(hTotalStack->Integral(iBin,nBins+1,"width")));
      localFormula = TQStringUtils::replace(localFormula,"bLow",std::to_string(hTotalStack->Integral(0,iBin)));
      localFormula = TQStringUtils::replace(localFormula,"bHigh",std::to_string(hTotalStack->Integral(iBin,nBins+1)));
      localFormula = TQStringUtils::replace(localFormula,"bkg",std::to_string(hTotalStack->GetBinContent(iBin)));

      localFormula = TQStringUtils::replace(localFormula,"dLowXbinWidth",std::to_string(hdata->Integral(0,iBin,"width")));
      localFormula = TQStringUtils::replace(localFormula,"dHighXbinWidth",std::to_string(hdata->Integral(iBin,nBins+1,"width")));
      localFormula = TQStringUtils::replace(localFormula,"dLow",std::to_string(hdata->Integral(0,iBin)));
      localFormula = TQStringUtils::replace(localFormula,"dHigh",std::to_string(hdata->Integral(iBin,nBins+1)));
      localFormula = TQStringUtils::replace(localFormula,"data",std::to_string(hdata->GetBinContent(iBin)));

      localFormula = TQStringUtils::replace(localFormula,"binN",std::to_string(iBin));
      localFormula = TQStringUtils::replace(localFormula,"binX",std::to_string(hTotalStack->GetBinCenter(iBin)));
      localFormula = TQStringUtils::replace(localFormula,"binWidth",std::to_string(hTotalStack->GetBinWidth(iBin)));

      TFormula frml(formulaName,localFormula);
      if (0 != frml.Compile()) {
        WARNclass("failed to compile formula %s (raw: %s)",localFormula.Data(),formulaString.Data());
        continue;
      }
      double value = frml.Eval(0.);
      // set the position and the width of the significance band
      customGraph->SetPoint(iPoint, hTotalStack->GetBinCenter(iBin), value);

      //this part might be properly implemented at some time, for now it's irrelevant
      /*
      if (value < 0.) {
        customGraph->SetPointError(
                                         iPoint, hTotalStack->GetBinWidth(iBin) / 2.,
                                         hTotalStack->GetBinWidth(iBin) / 2., -value, 0.);
      } else {
        customGraph->SetPointError(
                                         iPoint, hTotalStack->GetBinWidth(iBin) / 2.,
                                         hTotalStack->GetBinWidth(iBin) / 2., 0., value);
      }
      */
      if (value < actualSigMin){
        actualSigMin = value;
      }
      if (value > actualSigMax){
        actualSigMax = value;
      }

      iPoint++;

    }
    if (min < max) {
      actualSigMin = std::max(actualSigMin,min);
      actualSigMax = std::min(actualSigMax,max);
    }
    //@tag: [geometry.xscaling,geomatry.sub.scaling] These argument tags influence the appearance of the subplot. The precise effect is considered 'black magic'.
    double xscaling = tags.getTagDoubleDefault("geometry.xscaling",1.);
    double sigPadScaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);
    this->applyStyle(tags,customGraph,formulaName,xscaling,sigPadScaling);
    this->applyGeometry(tags,customGraph,"main" ,xscaling,sigPadScaling);
    this->applyGeometry(tags,customGraph,"sub" ,xscaling,sigPadScaling);

    // set the x range of the ratio graph to match the one of the main histogram
    double xLowerLimit = hTotalStack->GetBinLowEdge(1);
    double xUpperLimit = hTotalStack->GetBinLowEdge(nBins) + hTotalStack->GetBinWidth(nBins);
    customGraph->GetXaxis()->SetLimits(xLowerLimit, xUpperLimit);

    // set the titles of the axis of the ratio graph
    customGraph->GetXaxis()->SetTitle(hTotalStack->GetXaxis()->GetTitle());

    // confine the y axis of the ratio plot
    customGraph->GetYaxis()->SetTitle(formulaName);

    actualSigMin = TMath::Abs(actualSigMin);
    int y1 = TMath::Nint(actualSigMin);
    if (y1 < actualSigMin)
      actualSigMin = y1 + 0.5;
    else
      actualSigMin = y1;

    if (fmod(actualSigMin, 1) == 0)
      actualSigMin += 0.5;
    int y2 = TMath::Nint(actualSigMax);
    if (y2 < actualSigMax)
      actualSigMax = y2 + 0.5;
    else
      actualSigMax = y2;
    if (fmod(actualSigMax, 1) == 0)
      actualSigMax += 0.5;

    customGraph->GetHistogram()->SetMinimum(actualSigMin);
    customGraph->GetHistogram()->SetMaximum(actualSigMax);

    // draw the graph
    if (first)
      customGraph->Draw("AP2");
    else
      customGraph->Draw("P2");

    first = false;
  }
}

//__________________________________________________________________________________|___________

void TQHWWPlotter::drawPull(TQTaggable& tags){
  // draw a pull-plot in the sub-pad

  //@tag:[style.pullMax,style.pullMin,style.pullMinAllowed,style.pullMaxAllowed] These argument tags give a suggested range of the y-axis for pull sub-plots. pullMin/MaxAllowed override the other two, unless 'style.forcePullLimits' is set to true, in which case pullMin/Max are hard limits for the subplot's y-axis.
  double pullMax = tags.getTagDoubleDefault ("style.pullMax",1000.);
  double pullMin = tags.getTagDoubleDefault ("style.pullMin",0.);
  //@tag:[style.pullMaxQerr] This argument tag specifies a tolerance to include outlying points when estimating the y-axis range (passed as 'tolerance' argument to TQPlotter::estimateRangeY). Default: std::numeric_limits<double>::infinity.
  double pullMaxQerr = tags.getTagDoubleDefault ("style.pullMaxQerr",std::numeric_limits<double>::infinity());
  //@tag:[style.forcePullLimits] If this argument tag is set to true the y-axis range of the pull sub-plot is enforced to the values given by the tags 'style.pullMin' and 'style.pullMax'. Default: false
  bool forcePullLimits = tags.getTagBoolDefault ("style.forcePullLimits",false );
  bool verbose = tags.getTagBoolDefault("verbose",false);

  TH1* hTotalStack = this->getObject<TH1>("totalStack");
  if(!hTotalStack) return;

  int nBins = hTotalStack->GetNbinsX();

  int nPoints = 0;
  for (int i = 1; i <= nBins; i++) {
    if (hTotalStack->GetBinContent(i) != 0.) {
      nPoints++;
    }
  }

  if(verbose) VERBOSEclass("calculating geometry and axis ranges");
  // set the x range of the pull graph to match the one of the main histogram
  double xLowerLimit = hTotalStack->GetBinLowEdge(1);
  double xUpperLimit = hTotalStack->GetBinLowEdge(nBins) + hTotalStack->GetBinWidth(nBins);
  //@tag:[geometry.sub.scaling] This argument tag sets a scaling factor for the pull sub-plot. Default: 1.
  double pullPadScaling = tags.getTagDoubleDefault("geometry.sub.scaling",1.);

  TString dataLabel("");
  TList* graphs = new TList();
  // graphs->SetOwner(true);

  // actual minimum and maximum pull
  double actualPullMin = 1.;
  double actualPullMax = 1.;

  if(verbose) VERBOSEclass("generating pull graphs");
  // loop over data histograms
  TQTaggableIterator itr(fProcesses);

  while(itr.hasNext()){
    // get the data histogram
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    //@tag:[.isData] This process tag identifies the corresponding histograms as data histograms. Default: false.
    if(!process->getTagBoolDefault(".isData",false)) continue;
    TH1 * h_data = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    if(!h_data) continue;
    if(dataLabel.IsNull()) dataLabel = h_data->GetTitle();

    // calculate the number of valid pull points: pull points are considered
    // valid if they have a finite value (MC prediction != 0) (--> nPoints) and
    // the observed data is greater than zero (--> nPullPoints)
    int nPullPoints = 0;
    for (int i = 1; i <= nBins; i++) {
      double mcErr = hTotalStack->GetBinError(i);
      double dataErr = h_data->GetBinError(i);
      if(mcErr == 0) continue;
      if(dataErr == 0) continue;
      if(!TQUtils::isNum(hTotalStack->GetBinContent(i))){
        WARNclass("encountered non-numeric MC value: %f",mcErr);
        continue;
      }
      if(!TQUtils::isNum(h_data->GetBinContent(i))){
        WARNclass("encountered non-numeric data value: %f",dataErr);
        continue;
      }
      nPullPoints++;
    }

    if(nPullPoints < 1){
      // there is nothing to draw -- well, let's do nothing, then
      continue;
    }

    // the graph used to draw the pull points
    TGraphErrors * pullGraph = new TGraphErrors(nPullPoints);
    this->addObject(pullGraph,TString::Format("pullGraph_%s",h_data->GetName()));
    pullGraph->SetTitle(TString::Format("%s (pull)",h_data->GetTitle()));
    pullGraph->SetLineColor(h_data->GetLineColor());
    pullGraph->SetMarkerSize(h_data->GetMarkerSize());
    pullGraph->SetMarkerStyle(h_data->GetMarkerStyle());
    pullGraph->SetMarkerColor(h_data->GetMarkerColor());

    int iPullPoint = 0;

    // loop over all bins of the histogram
    for (int iBin = 1; iBin <= nBins; iBin++) {
      double x = hTotalStack->GetBinCenter(iBin);
      // get the values and errors of data and MC for this bin
      double data = h_data ->GetBinContent(iBin);
      double mc = hTotalStack->GetBinContent(iBin);
      double value = data - mc;
      double error2 = pow(h_data->GetBinContent(iBin),2) + pow(hTotalStack->GetBinContent(iBin),2);
      if(verbose) VERBOSEclass("adding pull point with x=%f, v=%f, e=%f (data=%f, MC=%f)",x,value,sqrt(error2),data,mc);
      pullGraph->SetPoint(iPullPoint, x, value/sqrt(error2));
      iPullPoint++;
    }

    this->applyStyle(tags   ,pullGraph,"sub.data",1.,pullPadScaling);
    //tag documentation see above
    double pullMinAllowed = tags.getTagDoubleDefault ("style.pullMinAllowed",pullMin);
    double pullMaxAllowed = tags.getTagDoubleDefault ("style.pullMaxAllowed",pullMax);
    actualPullMin=pullMinAllowed;
    actualPullMax=pullMaxAllowed;
    if(verbose) VERBOSEclass("drawPull: allowed range of pull graph: %f -- %f",actualPullMin,actualPullMax);

    this->estimateRangeY(pullGraph,actualPullMin,actualPullMax,pullMaxQerr);

    if(verbose) VERBOSEclass("drawPull: estimated range of pull graph: %f -- %f (pullMaxQerr=%f)",actualPullMin,actualPullMax,pullMaxQerr);

    if(actualPullMin == actualPullMax){
      if(verbose) VERBOSEclass("expanding pull to not be empty");
      //TODO: this is not how this works. this is not how any of this works...
      actualPullMin *= 1.1;
      actualPullMax *= 1.1;
    }

    if (forcePullLimits)
      actualPullMin = pullMin;
    else
      actualPullMin = actualPullMin-0.1*(actualPullMax-actualPullMin);

    if (forcePullLimits)
      actualPullMax = pullMax;
    else
      actualPullMax = actualPullMax+0.1*(actualPullMax-actualPullMin);

    if(verbose) VERBOSEclass("drawPull: final of pull graph: %f -- %f",actualPullMin,actualPullMax);

    graphs->Add(pullGraph);
  }
  //@tag:[labels.totalStack] This argument tag determines the label used for the total (MC) background. Default: "SM".
  TString totalStackLabel = tags.getTagStringDefault ("labels.totalStack", "SM");
  //@tag:[labels.data] This argument tag determines the label for data. Default is the title of the data histogram.
  tags.getTagString("labels.data",dataLabel);

  gStyle->SetEndErrorSize(0);


  if(verbose) VERBOSEclass("drawing line");
  // if 1. is included in the range of the y axis of the pull plot...
  TLine * line = new TLine(xLowerLimit, 0., xUpperLimit, 0.);
  line->SetLineColor(kRed);
  line->Draw();

  if(verbose) VERBOSEclass("drawing additional markers");
  TQGraphErrorsIterator itr2(graphs);
  bool first = true;
  while(itr2.hasNext()){
    TGraphErrors* pullGraph = itr2.readNext();
    if(!pullGraph) continue;
    if(first){
      pullGraph->Draw("AP");
      pullGraph->GetXaxis()->SetRangeUser(hTotalStack->GetXaxis()->GetXmin(),hTotalStack->GetXaxis()->GetXmax());
    } else {
      pullGraph->Draw("P SAME");
    }

    this->drawArrows(tags,pullGraph, actualPullMin,actualPullMax);
  }
}
