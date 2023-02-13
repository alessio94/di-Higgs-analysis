#include "QFramework/TQROOTPlotter.h"

#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLatex.h"
#include "THStack.h"
#include "TMath.h"
#include "TArrow.h"
#include "TLine.h"
#include "TGaxis.h"

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

#include "QFramework/TQIterator.h"
#include "QFramework/TQLibrary.h"
#include "QFramework/TQNamedTaggable.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQUtils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQROOTPlotter:
//
// The abstract TQROOTPlotter class provides a base class for custom,
// analysis-specific plotters like the TQHWWPlotter.
// By inheriting from the TQROOTPlotter, a base plotting interface is provided
// for the daughter classes. The only purely virtual function
// that needs to be implemented by the user is the
// TCanvas* TQPlotter::makePlot
// Other functionality like data management is provided by the TQPlotter class.
//
////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(TQROOTPlotter)

//__________________________________________________________________________________|___________

TQROOTPlotter::TQROOTPlotter() :
  TQPlotter(),
  pads(new TObjArray())
{
  // Constructor of TQPlotter class
}

//__________________________________________________________________________________|___________

TQROOTPlotter::TQROOTPlotter(TQSampleFolder * baseSampleFolder) :
  TQPlotter(baseSampleFolder),
  pads(new TObjArray())
{
  // Constructor of TQPlotter class
}

//__________________________________________________________________________________|___________

TQROOTPlotter::TQROOTPlotter(TQSampleDataReader * dataSource) :
  TQPlotter(dataSource),
  pads(new TObjArray())
{
  // Constructor of TQPlotter class
}


//__________________________________________________________________________________|___________

void TQROOTPlotter::reset() {
  TQPlotter::reset();

  // set the official ATLAS style
  this->setStyleAtlas();
}

//__________________________________________________________________________________|___________

void TQROOTPlotter::addHistogramToLegend(TQTaggable& tags, TLegend * legend, TQNamedTaggable* process, const TString& options){
  // add a single histogram to the legend
  if(!process) return;
  // transfer process tags to the options container locally to avoid having to pass around spurious data
  TQTaggable opts(options);
  opts.importTags(process);
  this->addHistogramToLegend(tags,legend,this->makeHistogramIdentifier(process),opts);
}

//__________________________________________________________________________________|___________

void TQROOTPlotter::addHistogramToLegend(TQTaggable& tags, TLegend * legend, const TString& identifier, TQTaggable& options){
  // add a single histogram to the legend
  this->addHistogramToLegend(tags,legend,this->getObject<TH1>(identifier),options);
}


//__________________________________________________________________________________|___________

void TQROOTPlotter::addHistogramToLegend(TQTaggable& tags, TLegend * legend, TH1* histo, const TString& options){
  // add a single histogram to the legend
  TQTaggable opts(options);
  this->addHistogramToLegend(tags,legend,histo,opts);
}

//__________________________________________________________________________________|___________

void TQROOTPlotter::addHistogramToLegend(TQTaggable& tags, TLegend * legend, TH1* histo, TQTaggable& options){
  // add a single histogram to the legend
  bool showMissing = tags.getTagBoolDefault("style.showMissing",true);
  bool showEventYields = tags.getTagBoolDefault("style.showEventYields",false);
  bool showMean = tags.getTagBoolDefault("style.showMean");
  bool showRMS = tags.getTagBoolDefault("style.showRMS");
  bool showUnderOverflow = tags.getTagBoolDefault("style.showEventYields.useUnderOverflow",true);
  bool showEventYieldErrors = tags.getTagBoolDefault("style.showEventYields.showErrors",false);
  bool verbose = tags.getTagBoolDefault("verbose",false);

  TString title = options.getTagStringDefault("defaultTitle", "");
  if (histo) title = histo->GetTitle();
  options.getTagString("title", title);
  title = TQStringUtils::convertLaTeX2ROOTTeX(title);

  /* add an entry to the legend */
  if (options.getTagBoolDefault("showInLegend", true)) {
    if (histo) {
      if (showEventYields){
        double err;
        double val = TQHistogramUtils::getIntegralAndError(histo,err,showUnderOverflow);
        if(showEventYieldErrors){
          title.Append(TString::Format(" {%.3g #pm %.3g}", val, err));
        } else {
          title.Append(TString::Format(" {%.3g}", val ));
        }
      }
      if (showMean)
        title.Append(TString::Format("#mu=%.3g", histo->GetMean()));
      if (showRMS)
        title.Append(TString::Format("(%.3g)", histo->GetRMS()));
      if(verbose) VERBOSEclass("adding legend entry '%s', attributed to histogram '%s'",title.Data(),histo->GetName());
      title.Prepend(" ");
      legend->AddEntry(histo, title, options.getTagStringDefault(".legendOptions", "f"));
    } else {
      if (showMissing){
        if(verbose) VERBOSEclass("adding empty legend entry for missing histogram (showMissing=true)");
        title.Prepend(" ");
        legend->AddEntry(new TObject(), title, "");
      }
    }
  } else {
    DEBUGclass("process '%s' is not added to legend (showInLegend=false)",title.Data());
  }
}

//__________________________________________________________________________________|___________

void TQROOTPlotter::addAllHistogramsToLegend(TQTaggable& tags, TLegend * legend, const TString& processFilter,
                                         const TString& options, bool reverse){

  // add all histograms matching the process filter to the legend
  bool verbose = tags.getTagBoolDefault("verbose",false);
  if(verbose) VERBOSEclass("entering function, processFilter='%s'",processFilter.Data());

  TQTaggableIterator itr(this->fProcesses->MakeIterator(reverse ? kIterBackward : kIterForward),true);
  while(itr.hasNext()){
    TQNamedTaggable * process = itr.readNext();
    if(!process){
      if(verbose) VERBOSEclass("skipping NULL entry");
    } else {
      if(!processFilter.IsNull() && !process->getTagBoolDefault(processFilter,false)){
        if(verbose) VERBOSEclass("skipping empty legend entry for '%s' - does not match filter '%s'",process->getTagStringDefault(".path",process->GetName()).Data(),processFilter.Data());
      } else {
        this->addHistogramToLegend(tags,legend,process,options);
      }
    }
  }
}


//__________________________________________________________________________________|___________

TCanvas * TQROOTPlotter::plot(TString histogram, const TString& inputTags) {
  // plot the given histogram using the given tags
  // the tags are forwarded to and interpreted by the makeplot function
  TQTaggable taggable(inputTags);
  return plot(histogram, taggable);
}

//__________________________________________________________________________________|___________

TCanvas * TQROOTPlotter::plot(TString histogram, TQTaggable* inputTags) {
  // plot the given histogram using the given tags
  // the tags are forwarded to and interpreted by the makeplot function
  TQTaggable tags;
  tags.importTags(inputTags);
  TCanvas* c = this->plot(histogram, tags);
  return c;
}

//__________________________________________________________________________________|___________

TCanvas * TQROOTPlotter::plot(TString histogram, TQTaggable& tags) {
  // plot the given histogram using the given tags
  // the tags are forwarded to and interpreted by the makeplot function
  bool verbose = tags.getTagBoolDefault("verbose",false);
  if(verbose) VERBOSEclass("TQROOTPlotter::plot");
  this->deleteObjects();
  this->clearObjects();
  //@tags: printProcesses: print processes to the console before plotting
  if(tags.getTagBoolDefault("printProcesses",false)){
    this->printProcesses();
  }
  //@tags: useNamePrefix: prefix histogram names with the variable names. will not affect the look of the plot, but possibly required for elaborate plots to be saved in .C format
  if(tags.getTagBoolDefault("useNamePrefix",true)){
    TString tmp(histogram);
    TString prefix = TQFolder::getPathTail(tmp);
    TQStringUtils::ensureTrailingText(prefix,".");
    tags.importTagsWithoutPrefix(tags,prefix);
  }
  tags.importTags(this);
  if(histogram.Contains("=")){
    tags.importTagsWithPrefix(histogram,"input");
  } else {
    tags.setTagString("input.histogram",histogram);
  }
  TQTaggable copyOfTags(tags);
  TCanvas* c = this->makePlot(copyOfTags);
  //@tags: printObjects: print the objects created for plotting
  if(tags.getTagBoolDefault("printObjects",false)){
    std::cout << TQStringUtils::makeBoldBlue(this->Class()->GetName()) << TQStringUtils::makeBoldWhite(" - objects:") << std::endl;
    this->printObjects();
  }
  //@tags: printLegend: print the legend entries
  if(tags.getTagBoolDefault("printLegend",false)){
    std::cout << TQStringUtils::makeBoldBlue("TQPlotter") << TQStringUtils::makeBoldWhite(" - legend entries:") << std::endl;
    TLegend* leg = this->getObject<TLegend>("legend");
    if(!leg){
      ERRORclass("no legend found!");
    } else {
      TQLegendEntryIterator itr(leg->GetListOfPrimitives());
      while(itr.hasNext()){
        TLegendEntry* entry = itr.readNext();
        if(!entry) continue;
        TObject* obj = entry->GetObject();
        if(obj){
          std::cout << TQStringUtils::makeBoldBlue(TQStringUtils::fixedWidth(obj->Class()->GetName(),20)) << TQStringUtils::makeBoldWhite(TQStringUtils::fixedWidth(obj->GetName(),20))<< '"' << TQStringUtils::makeBoldWhite(entry->GetLabel()) << '"' << std::endl;
        } else {
          std::cout << TQStringUtils::makeBoldRed(TQStringUtils::fixedWidth("NULL",40)) << '"' << TQStringUtils::makeBoldWhite(entry->GetLabel()) << '"' << std::endl;
        }
      }
    }
  }
  //@tags: printStyle: print the style tags active after plotting (includes temporary tags set by the plotter itself)
  if(tags.getTagBoolDefault("printStyle",false))
    tags.printTags();

  return c;
}

//__________________________________________________________________________________|___________

TPad * TQROOTPlotter::getPad(const TString& name){
  // retrieve a pad by name
  if(!this->pads) return NULL;
  if(name.IsNull()) return NULL;
  TQIterator itr(this->pads);
  while(itr.hasNext()){
    TObject* obj = itr.readNext();
    if(!obj) continue;
    if(!TQStringUtils::matches(obj->GetName(),name)) continue;
    TPad* p = dynamic_cast<TPad*>(obj);
    if(!p) return NULL;
    p->cd();
    return p;
  }
  return NULL;
}

//__________________________________________________________________________________|___________

TCanvas * TQROOTPlotter::plot(TString histogram, const char* inputTags) {
  // plot the given histogram using the given tags
  // the tags are forwarded to and interpreted by the makeplot function
  TQTaggable taggable(inputTags);
  TCanvas * c = plot(histogram, taggable);
  return c;
}

//__________________________________________________________________________________|___________

bool TQROOTPlotter::plotAndSaveAsInternal(const TString& histogram, const TString& saveAs, TQTaggable& tags) {
  // plot and save the given histogram using the given tags
  // the tags are forwarded to and interpreted by the makeplot function
  bool verbose = tags.getTagBoolDefault("verbose",false);
  if(verbose) VERBOSEclass("TQROOTPlotter::plotAndSaveAs");

  TDirectory* tmpObjects = NULL;
  TDirectory* oldDir = gDirectory;
  if(saveAs.EndsWith(".root")){
    tmpObjects = this->objects;
    if(verbose) VERBOSEclass("opening .root output file");
    this->objects = TFile::Open(saveAs,"RECREATE");
  }
  gDirectory = oldDir;

  TCanvas * canvas = plot(histogram, tags);
  bool success = canvas;

  //@tags: sortObjects: Sort plotter objects before printing .C files. This produces consistent files that can be compared to previous commits. However, the pdfs can look a little sketchy. Don't use this tag if you want to get nice results. Default: false.
  if(success && tags.getTagBoolDefault("sortObjects",false)){
    // sort everything to ensure a consistent layout of .C files
    if(verbose) VERBOSEclass("sorting objects");
    TQListUtils::sortByName(const_cast<TList*>(canvas->GetListOfPrimitives()));
    TQIterator paditr(this->pads);
    while(paditr.hasNext()){
      TPad* pad = dynamic_cast<TPad*>(paditr.readNext());
      if(!pad) continue;
      TList* primitives = const_cast<TList*>(pad->GetListOfPrimitives());
      if(!primitives) continue;
      TQListUtils::sortByName(primitives);
    }
  }

  if (success && !tmpObjects) {
    this->pads->SetOwner(true);

    // Save the canvas and capture the default root message
    int capturingSuccessful = TQLibrary::captureStdout();
    canvas->SaveAs(saveAs.Data());
    if (capturingSuccessful == 0){
      // If capturing works, suppress the standard message.
      TString output = TQLibrary::readCapturedStdout();
      TQLibrary::restore_stdout();
      if (!TQROOTPlotter::isDefaultPlotMessage(output, saveAs)){
        WARNclass("Unexpected output from ROOT:");
        std::cout << output.Data();
        std::cout.flush();
      }
    }

    //@tags: embedfonts: run external font embedding command on created pdf plots
    if(saveAs.EndsWith(".pdf") && tags.getTagBoolDefault("embedfonts",false)){
      TQLibrary::embedFonts(saveAs);
    }
    if(saveAs.EndsWith(".pdf") || saveAs.EndsWith(".jpg") || saveAs.EndsWith(".png")){
      TString exifinfostring = histogram;
      //@tags: exiftitle: set meta-information as exif string on pdf,jpg and png files
      this->getTagString("exiftitle",exifinfostring);
      tags.getTagString("exiftitle",exifinfostring);
      if(TQLibrary::hasEXIFsupport() && !TQLibrary::setEXIF(saveAs,exifinfostring)){
        ERRORclass("setting EXIF meta-information on %s failed!",saveAs.Data());
      }
    }
  }
  if(success && tmpObjects){
    this->objects->Add(canvas);
    this->objects->Add(this->pads);
    this->objects->Write();
    this->objects->Close();
    this->objects = tmpObjects;
    this->pads = new TObjArray();
    std::cout << "Info in " << this->IsA()->GetName() << ": created file " << saveAs << std::endl;
    this->clearObjects();
  } else if(tags.getTagBoolDefault("deleteObjects",true)){
    //@tags: deleteObjects: control whether plotting objects will be kept in memory after plotting (default: false for plotAndSaveAs, true for plot)
    this->deleteObjects();
    delete canvas;
  }
  return success;
}


//______________________________________________________________________________________________

TQROOTPlotter::~TQROOTPlotter() {
  // Destructor of TQPlotter class:
  // if(this->pads) delete this->pads;
}

//__________________________________________________________________________________|___________

void TQROOTPlotter::setStyleAtlas() {
  // apply atlas style options to gStyle

  int icol = 0;
  gStyle->SetFrameBorderMode(icol);
  gStyle->SetFrameFillColor(icol);
  gStyle->SetCanvasBorderMode(icol);
  gStyle->SetCanvasColor(icol);
  gStyle->SetPadBorderMode(icol);
  gStyle->SetPadColor(icol);
  gStyle->SetStatColor(icol);

  // set the paper & margin sizes
  gStyle->SetPaperSize(20,26);

  // set margin sizes
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  gStyle->SetTitleXOffset(1.4);
  gStyle->SetTitleYOffset(1.4);

  // use large fonts
  //int font=72; // Helvetica italics
  int font=42; // Helvetica
  double tsize=0.05;
  gStyle->SetTextFont(font);

  gStyle->SetTextSize(tsize);
  gStyle->SetLabelFont(font,"x");
  gStyle->SetTitleFont(font,"x");
  gStyle->SetLabelFont(font,"y");
  gStyle->SetTitleFont(font,"y");
  gStyle->SetLabelFont(font,"z");
  gStyle->SetTitleFont(font,"z");

  gStyle->SetLabelSize(tsize,"x");
  gStyle->SetTitleSize(tsize,"x");
  gStyle->SetLabelSize(tsize,"y");
  gStyle->SetTitleSize(tsize,"y");
  gStyle->SetLabelSize(tsize,"z");
  gStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);
  gStyle->SetHistLineWidth((Width_t)2.);
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars
  //gStyle->SetErrorX(0.001);
  // get rid of error bar caps
  //gStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  gStyle->SetOptTitle(0);
  //gStyle->SetOptStat(1111);
  gStyle->SetOptStat(0);
  //gStyle->SetOptFit(1111);
  gStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

}


//__________________________________________________________________________________|___________

void TQROOTPlotter::clearObjects(){
  // clear all objects maintained by the plotter
  this->pads->SetOwner(false);
  this->pads->Clear();
  TQPlotter::clearObjects();
}

//__________________________________________________________________________________|___________

void TQROOTPlotter::deleteObjects(){
  // clear all objects maintained by the plotter
  this->pads->SetOwner(true);
  this->pads->Clear();
  TQPlotter::deleteObjects();
}

//__________________________________________________________________________________|___________

TPad* TQROOTPlotter::createPad(TQTaggable& tags, const TString& key){
  // create a pad and add it to the list
  // geometry and styling parameters will be read from the given tag set
  // the tags according to the given key will be read
  //@tags: geometry.*.scaling: scaling of a pad
  //@tags: [geometry.*.xMin,geometry.*.xMax,geometry.*.yMin,geometry.*.yMax]: set the geometry parameters of a pad
  //@tags: [style.*.fillStyle, style.*.fillColor]: control fill style and color of a pad
  //@tags: [geometry.*.margins.left,geometry.*.margins.right,geometry.*.margins.top,geometry.*.margins.bottom]: control margins of a pad
  //@tags: [style.*.tickx,style.*.ticky]: control whether ticks are shown on x and y axes of this pad
  //@tags: [style.tickx,style.ticky]: control whether ticks are shown on x and y axes of all pads
  //@tags: [style.*.borderSize,style.*.borderMode] control appearance of the borders of this pad
  //@tags: [style.*.logScaleY] control log scale in y
  double padscaling = tags.getTagDoubleDefault("geometry."+key+".scaling",1.);
  TPad * pad = new TPad(key,key,
                        tags.getTagDoubleDefault("geometry."+key+".xMin",0.),
                        tags.getTagDoubleDefault("geometry."+key+".yMin",0.),
                        tags.getTagDoubleDefault("geometry."+key+".xMax",1.),
                        tags.getTagDoubleDefault("geometry."+key+".yMax",1.));
  pad->SetFillStyle(tags.getTagIntegerDefault("style."+key+".fillStyle",0));
  pad->SetFillColor(tags.getTagIntegerDefault("style."+key+".fillColor",0));
  pad->SetMargin(tags.getTagDoubleDefault("geometry."+key+".margins.left" ,0.16),
                 tags.getTagDoubleDefault("geometry."+key+".margins.right" ,0.05),
                 padscaling*tags.getTagDoubleDefault("geometry."+key+".margins.bottom",0.16),
                 padscaling*tags.getTagDoubleDefault("geometry."+key+".margins.top" ,0.05));
  pad->SetTickx(tags.getTagIntegerDefault("style."+key+".tickx",tags.getTagIntegerDefault("style.tickx",1)));
  pad->SetTicky(tags.getTagIntegerDefault("style."+key+".ticky",tags.getTagIntegerDefault("style.ticky",1)));
  pad->SetBorderSize(tags.getTagIntegerDefault("style."+key+".borderSize",0));
  pad->SetBorderMode(tags.getTagIntegerDefault("style."+key+".borderMode",0));
  pad->SetLogy(tags.getTagBoolDefault ("style."+key+".logScaleY", false));
  this->pads->Add(pad);
  return pad;
}

//__________________________________________________________________________________|___________

TCanvas* TQROOTPlotter::createCanvas(TQTaggable& tags){
  // create a new canvas
  bool verbose = tags.getTagBoolDefault("verbose",false);
  TString canvasName = tags.getTagStringDefault("input.name","histogram");
  canvasName.ReplaceAll("/", "_");
  canvasName=TQStringUtils::makeValidIdentifier(canvasName,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890._","");

  // don't make the new canvas kill another with the same name: append an increasing number until it's unique
  int iCanvas = 1;
  TCollection * listOfCanvases = gROOT->GetListOfCanvases();
  const TString origCanvasName(canvasName);
  while (listOfCanvases && listOfCanvases->FindObject(canvasName.Data()))
    canvasName = TString::Format("%s_n%d", origCanvasName.Data(), iCanvas++);

  // @tags:geometry.canvas.width: set the width of the canvas
  // @tags:geometry.canvas.height: set the height of the canvas
  if(verbose) VERBOSEclass("creating canvas with name '%s'",canvasName.Data());
  int width = tags.getTagIntegerDefault("geometry.canvas.width",800);
  int height = tags.getTagIntegerDefault("geometry.canvas.height",600);
  TCanvas* canvas = new TCanvas(TQFolder::makeValidIdentifier(canvasName),canvasName,0,0,width,height);
  canvas->SetWindowSize(width + (width - canvas->GetWw()), height + (height - canvas->GetWh()));
  canvas->SetMargin(0.,0.,0.,0);
  canvas->cd();

  TPad* pad = this->createPad(tags,"main");
  // @tags:style.logScale: control whether the main plot will be shown in log scale (default:false)
  if (tags.getTagBoolDefault ("style.logScale",false ) && (
    (tags.getTagIntegerDefault("style.nDim",1) == 1) || (tags.getTagIntegerDefault("style.nDim",1) == -1) // -1 for TProfile
  ) ){
    pad->SetLogy();
  }
  // @tags:style.logScaleX: control whether the main plot will be shown in logX scale (default:false)
  if (tags.getTagBoolDefault ("style.logScaleX",false )){
    pad->SetLogx();
  }
  pad->Draw();
  // @tags:style.showSub: control whether any subplot will be shown. overwrites subplots defined elsewhere.
  if (tags.getTagBoolDefault ("style.showSub",false)){
    canvas->cd();
    TPad * ratioPad = this->createPad(tags,"sub");
    ratioPad->SetGridy(true);
    // @tags:style.logScaleX: control whether the subplot will be shown in logX scale (default:false)
    if (tags.getTagBoolDefault ("style.logScaleX",false )){
      ratioPad->SetLogx();
    }
    ratioPad->Draw();
  }

  canvas->cd();
  return canvas;
}

//__________________________________________________________________________________|___________

void TQROOTPlotter::drawCutLines1D(TQTaggable& tags){
  // draw vertical lines as requested by the user
  TH1* hMaster = this->getObject<TH1>("Graph_master");
  double upper = hMaster->GetMaximum();
  double lower = hMaster->GetMinimum();
  bool logScale = tags.getTagBoolDefault ("style.logScale",false );

  // read the list of cut thresholds to display
  int iCut = 0;
  double threshold = 0.;
  //@tags: cuts.*: a list of (vertical) cut lines to be drawn. value will be x-value of the vertical line
  while (tags.getTagDouble(TString::Format("cuts.%d", iCut++), threshold)) {
    int iBlock = 0;
    double block_x = 0;
    double block_x_old = 0;
    double block_y = 1;
    while(tags.getTag(TString::Format("blocks.x.%d",iBlock),block_x) && tags.getTag(TString::Format("blocks.y.%d",iBlock),block_y)){
      if(threshold > block_x_old && threshold < block_x){
        break;
      }
      block_x_old = block_x;
      iBlock++;
    }
    //@tags: style.cutLineHeightScaleFactor: Cut line height is nominally the same as the histogram maximum; use this tag to scale it by some factor.
    auto cutLineHeightScaleFactor = tags.getTagDoubleDefault("style.cutLineHeightScaleFactor",1.0);
    block_y *= cutLineHeightScaleFactor;
    double max = logScale ? TMath::Exp((TMath::Log(upper) - TMath::Log(lower)) * block_y + TMath::Log(lower)) : (upper - lower) * block_y + lower;

    TLine * line = new TLine(threshold, lower, threshold, max);
    //@tags:[style.cutLineStyle,style.cutLineWidth,style.cutLineColor]: control appearance of cutlines (TLine::SetLineStyle,TLine::SetLineWidth,TLine::SetLineColor)
    line->SetLineStyle(tags.getTagIntegerDefault("style.cutLineStyle",7));
    line->SetLineWidth(tags.getTagIntegerDefault("style.cutLineWidth",2));
    line->SetLineColor(tags.getTagIntegerDefault("style.cutLineColor",kRed));
    line->Draw();

    //@tags:[style.cutArrows]: draw an arrow in the direction of the cut at the top of the cut line
    bool arrowsToDraw = tags.getTagBoolDefault("style.cutArrows",false);
    if (arrowsToDraw){
      //@tags:[style.cutLineArrowDirection]: specify which direction the cut is in so the arrow is drawn correctly
      auto arrowDirection = tags.getTagStringDefault("style.cutLineArrowDirection", "left");

      double arrow_lo = 0.;
      double arrow_hi = 0.;
      TString arrowDir;

      if (arrowDirection == "left") {
	arrow_lo = threshold*0.8;
	arrow_hi = threshold;
	arrowDir = "<";
      }
      else if (arrowDirection == "right") {
	arrow_lo = threshold;
	arrow_hi = threshold*1.2;
	arrowDir = ">";
      }      

      TArrow * arrow = new TArrow(arrow_lo, max, arrow_hi, max, 0.015, arrowDir);
      arrow->SetLineWidth(tags.getTagIntegerDefault("style.cutLineWidth",2));
      //Always draw arrow as solid line
      arrow->SetLineStyle(1);
      arrow->SetLineColor(tags.getTagIntegerDefault("style.cutLineColor",kBlack));
      arrow->Draw();

    }
  }
}

//__________________________________________________________________________________|___________

int TQROOTPlotter::drawHeightLines(TQTaggable& tags){
  // draw height lines onto the canvas
  if(!tags.getTagBoolDefault("heightlines.show",false))
    return 0;

  bool verbose = tags.getTagBoolDefault("verbose",false);
  if(verbose) VERBOSEclass("attempting to draw height lines");

  TH1* hMaster = this->getObject<TH1>("Graph_master");

  //@tags:[heightlines.show] draw additional diagonal height lines in 2D plots
  //@tags:[heightlines.xCoeff,heightlines.yCoeff,heightlines.constCoeff]  control the slope of diagonal height lines in 2D plots
  double xCoeff = tags.getTagDoubleDefault("heightlines.xCoeff",0.);
  double yCoeff = tags.getTagDoubleDefault("heightlines.yCoeff",0.);
  double constCoeff = tags.getTagDoubleDefault("heightlines.constCoeff",0.);

  bool rotate = tags.getTagBoolDefault("heightlines.rotateLabels",true);
  //@tags:[heightlines.rotateLabels] rotate labels according to the inclination angle of height lines in 2D plots

  double labelSize = tags.getTagDoubleDefault("style.axes.labelSize",0.03);
  double labelOffset = tags.getTagDoubleDefault("style.axes.labelOffset",0.005);

  //@tags:[heightlines.color,heightlines.style]  control the visuals of diagonal height lines in 2D plots
  //@tags:[heightlines.values] list of values along the x axis at which height lines should appear
  int color = tags.getTagIntegerDefault("heightlines.color",kBlack);
  int linestyle = tags.getTagIntegerDefault("heightlines.style",1.);

  std::vector<double> vals = tags.getTagVDouble("heightlines.values");
  double xmin = TQHistogramUtils::getAxisXmin(hMaster);
  double xmax = TQHistogramUtils::getAxisXmax(hMaster);

  int n = 0;

  double slope = - xCoeff/yCoeff;


  TLatex latex;
  latex.SetTextColor(color);
  latex.SetTextSize(labelSize);
  double latexXOffset, latexYOffset;

  if(rotate){
    double visualSlope = - TQUtils::convertdYtoPixels(xCoeff)/TQUtils::convertdXtoPixels(yCoeff);
    double visualAngle = atan(visualSlope) * 180./TMath::Pi();
    latex.SetTextAngle(visualAngle);
    latexXOffset = TQUtils::convertdXfromNDC(labelOffset * sin(-visualSlope));
    latexYOffset = TQUtils::convertdYfromNDC(labelOffset * cos( visualSlope));
  } else {
    latexXOffset = 0.;
    latexYOffset = TQUtils::convertdYfromNDC(labelOffset);
  }

  for(size_t i=0; i<vals.size(); i++){
    if(verbose) VERBOSEclass("drawing height line for z = %g",vals[i]);

    double offset = (vals[i] - constCoeff) / yCoeff;

    double y0 = offset + slope * xmin;
    double y1 = offset + slope * xmax;
    double x0 = xmin;
    double x1 = xmax;

    if(verbose) VERBOSEclass("pre-crop coordinates are x0=%g, x1=%g, y0=%g, y1=%g",x0,x1,y0,y1);

    TLine* l = new TLine(x0,y0,x1,y1);
    if(TQHistogramUtils::cropLine(hMaster,l)){
      if(verbose) VERBOSEclass("post-crop coordinates are x0=%g, x1=%g, y0=%g, y1=%g",l->GetX1(),l->GetX2(),l->GetY1(),l->GetY2());
      l->SetLineColor(color);
      l->SetLineStyle(linestyle);
      l->Draw();
      latex.DrawLatex(latexXOffset + 0.5*(l->GetX2()+l->GetX1()),latexYOffset + 0.5*(l->GetY2()+l->GetY1()),TString::Format("%g",vals[i]));
      n++;
    } else {
      if(verbose) VERBOSEclass("line-crop failed - no line plotted");
      delete l;
    }
  }

  return n;
}

//__________________________________________________________________________________|___________


int TQROOTPlotter::drawAdditionalAxes(TQTaggable& tags){
  // draw an additional axis onto the canvas
  double defaultLabelSize = tags.getTagDoubleDefault("style.axes.labelSize",0.03);
  double defaultTitleSize = tags.getTagDoubleDefault("style.axes.titleSize",0.03);
  double defaultTitleOffset = tags.getTagDoubleDefault("style.axes.titleOffset",1.0);

  /* read the list of cut thresholds to display */
  int iAxis = -1;
  bool show = false;

  //@tags:axis.IDX.show: draw an additional axis with index IDX
  //@tags:[axis.IDX.xMin,axis.IDX.xMax,axis.IDX.yMin,axis.IDX.yMax]: control geometry of additional axis IDX to be drawn
  //@tags:[axis.IDX.wMin,axis.IDX.wMax,axis.IDX.nDiv,axis.IDX.title]: control labeling of additional axis IDX to be drawn
  //@tags:[axis.*.labelSize,axis.IDX.titleSize,axis.IDX.titleOffset]: control style of additional axis IDX to be drawn

  while (tags.getTagBool(TString::Format("axis.%d.show", ++iAxis), show)) {
    if(!show) continue;

    double xmin, xmax, ymin, ymax;
    if(!tags.getTagDouble(TString::Format("axis.%d.xMin", iAxis), xmin)) continue;
    if(!tags.getTagDouble(TString::Format("axis.%d.xMax", iAxis), xmax)) continue;
    if(!tags.getTagDouble(TString::Format("axis.%d.yMin", iAxis), ymin)) continue;
    if(!tags.getTagDouble(TString::Format("axis.%d.yMax", iAxis), ymax)) continue;

    double wmin = tags.getTagDoubleDefault (TString::Format("axis.%d.wMin" , iAxis), 0);
    double wmax = tags.getTagDoubleDefault (TString::Format("axis.%d.wMax" , iAxis), 1);
    int nDiv = tags.getTagIntegerDefault (TString::Format("axis.%d.nDiv" , iAxis), 110);
    TString title = tags.getTagStringDefault(TString::Format("axis.%d.title", iAxis), "");


    double labelSize = tags.getTagDoubleDefault (TString::Format("axis.%d.labelSize", iAxis), defaultLabelSize);
    double titleSize = tags.getTagDoubleDefault (TString::Format("axis.%d.titleSize", iAxis), defaultTitleSize);
    double titleOffset = tags.getTagDoubleDefault (TString::Format("axis.%d.titleOffset", iAxis), defaultTitleOffset);

    TGaxis *addAxis = new TGaxis(xmin, ymin, xmax, ymax,wmin,wmax,nDiv);
    if(!title.IsNull()) addAxis->SetTitle(title);
    addAxis->SetLabelSize(labelSize);
    addAxis->SetTitleSize(titleSize);
    addAxis->SetTitleOffset(titleOffset);
    addAxis->Draw();
    show = false;
  }
  return iAxis;
}


//__________________________________________________________________________________|___________

bool TQROOTPlotter::isDefaultPlotMessage(TString message, const TString& filename){
  // check if message only contains one newline

  // The message should not usually be empty. But it happens when
  // running in standalone mode. The root message can not be captured
  // for some reason.
  if (message.EqualTo(""))
    return true;

  if (!(message.CountChar(10) == 1))
    return false;

  if ((filename.EndsWith(".C")) || (filename.EndsWith(".cxx")) || (filename.EndsWith(".json"))){
    if ((TQStringUtils::removeLeadingText(message, "TCanvas::SaveSource") == 1)
	&& (TQStringUtils::removeLeadingBlanks(message) > 0)
	&& (TQStringUtils::removeLeadingText(message, "INFO") == 1)
	&& (message.EndsWith("has been generated\n"))){
      return true;
    }
  }
  else if (filename.EndsWith(".root") || filename.EndsWith(".xml")){
    if ((TQStringUtils::removeLeadingText(message, "TCanvas::SaveAs") == 1)
	&& (TQStringUtils::removeLeadingBlanks(message) > 0)
	&& (TQStringUtils::removeLeadingText(message, "INFO") == 1)
	&& (message.EndsWith("has been created\n"))){
      return true;
    }
  }
  else{
    if ((TQStringUtils::removeLeadingText(message, "TCanvas::Print") == 1)
	&& (TQStringUtils::removeLeadingBlanks(message) > 0)
	&& (TQStringUtils::removeLeadingText(message, "INFO") == 1)
	&& (message.EndsWith("has been created\n"))){
      return true;
    }
  }
  return false;
}

//__________________________________________________________________________________|___________

void TQROOTPlotter::stackHistograms(TQTaggable& tags, const TString& stackname){
  // create the histogram stack
  bool verbose = tags.getTagBoolDefault("verbose",false );
  if(verbose) VERBOSEclass("stacking histograms");

  //@tag:normalize: normalize all histograms to 1
  bool normalize = tags.getTagBoolDefault("normalize",false ) || tags.getTagBoolDefault("normalizeWithoutOverUnderflow",false );
  //@tag:normalizeWithoutOverUnderflow: disable using underflow and overflow for normalization purposes. This tag automatically activates the "normalize" tag.
  bool normalizeWithoutOverUnderflow = !tags.getTagBoolDefault("normalizeWithoutOverUnderflow",false );
  TH1* hMaster = this->getObject<TH1>("Graph_master");
  TH1* hTotalStack = TQHistogramUtils::copyHistogram(hMaster,"totalStack");
  // these are some default values
  hTotalStack->SetTitle("SM");
  hTotalStack->SetLineColor(kBlue);
  hTotalStack->SetFillColor(14);
  hTotalStack->SetFillStyle(3245);
  hTotalStack->SetMarkerStyle(0);
  TH1* hTotalBkg = TQHistogramUtils::copyHistogram(hTotalStack,"totalBkg");

  // the list of histograms to be stacked
  std::vector<TH1*> otherHistList;;
  std::vector<TH1*> histStackList;
  std::vector<TQNamedTaggable*> processStackList;
  TQTaggableIterator itr(this->fProcesses);
  while(itr.hasNext()){
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    if(!h) continue;

    bool bkg = process->getTagBoolDefault(".isBackground",false);
    bool sig = process->getTagBoolDefault(".isSignal",false);
    bool bkgStack = bkg && process->getTagBoolDefault("stack", true);
    bool sigStack = sig && tags.getTagBoolDefault ("style.autoStackSignal",false);

    if(bkg) hTotalBkg->Add(h);
    if(bkgStack || sigStack){
      histStackList.push_back(h);
      processStackList.push_back(process);
      hTotalStack->Add(h);
    } else {
      otherHistList.push_back(h);
    }
  }

  // scale to normalize total background
  double totalBkgScale = 0;
  if (hTotalBkg){
    totalBkgScale = TQHistogramUtils::getIntegral(hTotalBkg, normalizeWithoutOverUnderflow);
  }
  if (normalize){
    if (hTotalBkg && totalBkgScale > 0.){
      hTotalBkg->Scale(1. / totalBkgScale);
      for(auto h:histStackList){
        h->Scale(1. / totalBkgScale);
      }
    }
    for(auto h:otherHistList){
      //also normalize non-stacked histograms such that their integral matches the normalized stack ("totalBackground")
      if (TQHistogramUtils::getIntegral(h, normalizeWithoutOverUnderflow) > 0) h->Scale(1./TQHistogramUtils::getIntegral(h, normalizeWithoutOverUnderflow));
    }
  }

  // sort the histograms to be stacked by ascending integral (sum of weights)
  if (!tags.getTagBoolDefault("style.manualStacking",false) && tags.getTagBoolDefault("style.autoStack",tags.getTagBoolDefault("style.logScale",false))){
    for (unsigned iHist = 0; iHist < histStackList.size(); iHist++) {
      unsigned iHistMin = iHist;
      double intMin = ((TH1*)histStackList.at(iHistMin))->GetSumOfWeights();
      for (unsigned iHist2 = iHist + 1; iHist2 < histStackList.size(); iHist2++) {
        double intMin2 = ((TH1*)histStackList.at(iHist2))->GetSumOfWeights();
        if (intMin2 < intMin) {
          iHistMin = iHist2;
          intMin = intMin2;
        }
      }
      if (iHistMin != iHist) {
        TH1 * temp = (TH1*)(histStackList)[iHist];
        (histStackList)[iHist] = (histStackList)[iHistMin];
        (histStackList)[iHistMin] = temp;
        TQNamedTaggable * temptag = (processStackList)[iHist];
        (processStackList)[iHist] = (processStackList)[iHistMin];
        (processStackList)[iHistMin] = temptag;
      }
    }
  }

  // create the stack
  THStack * stack = new THStack(stackname, tags.getTagBoolDefault("style.stackSignal",false) ? "Signal+Background Stack" : "Background Stack");

  // add the histograms to the stack (following the order in the histStackList)
  if (tags.getTagBoolDefault ("style.reverseStacking",false )) {
    for (int iHist = histStackList.size(); iHist > 0 ; iHist--){
      TH1* h = (histStackList.at(iHist-1));
      stack->Add(h);
    }
  } else {
    for (unsigned iHist = 0; iHist < histStackList.size(); iHist++){
      TH1* h = dynamic_cast<TH1*>(histStackList.at(iHist));
      stack->Add(h);
    }
  }

  TQTaggableIterator sitr(this->fProcesses);
  bool stackSignal = tags.getTagBoolDefault ("style.stackSignal",false);
  if (tags.getTagBoolDefault ("style.autoStackSignal",false)) stackSignal = false;
  while(sitr.hasNext()){
    TQNamedTaggable* process = sitr.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(".isSignal",false)) continue;
    TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    if(!h) continue;
    if(totalBkgScale > 0 && process->getTagBoolDefault("normalizeToTotalBkg",false) && !normalize){ //don't scale again if everything was already normalized to unity (see above)
      //@tag:normalizeToTotalBkg: process tag to normalize individual signal contributions to the total background. Apply this tag on the individual process. This tag will be ignored in combination with other normalization tags.
      h->Scale(totalBkgScale / TQHistogramUtils::getIntegral(h));
    }
    if (process->getTagBoolDefault("stack", stackSignal)){
      stack->Add(h);
      hTotalStack->Add(h);      
    }
  }

  if(stack->GetNhists() > 0){
    if(verbose) VERBOSEclass("successfully stacked %d histograms",stack->GetNhists());
    this->addObject(stack,stackname);
  } else {
    DEBUGclass("stack is empty!");
    delete stack;
  }
}

//__________________________________________________________________________________|___________

void TQROOTPlotter::addObject(TLegend* obj, const TString& key){
  // add a legend to the list of graphics objects maintained by the plotter
  if(!obj) return;
  if(!key.IsNull()) obj->SetName(key);
  this->objects->Add(obj);
}

//__________________________________________________________________________________|___________

void TQROOTPlotter::makeLegend(TQTaggable& tags, TObjArray* histos){
  // create a legend including the given list of histograms
  // @tags:style.showEventYields: show event yields (integral counts) in the legend
  // @tags:style.showEventYields.useUnderOverflow: include underflow and overflow in the event yields displayed in the legend (default:true)
  // @tags:style.nLegendCols: number of columns to be shown in legend
  // @tags:geometry.legend.height: scaling factor for height of the legend
  bool showEventYields = tags.getTagBoolDefault ("style.showEventYields",false);
  int nLegendCols = tags.getTagIntegerDefault ("style.nLegendCols",showEventYields ? 1 : 2);
  bool showTotalBkg = tags.getTagBoolDefault ("style.showTotalBkg",true);
  double legendHeight = tags.getTagDoubleDefault ("geometry.legend.height",1. );
  double scaling = tags.getTagDoubleDefault("geometry.main.scaling",1.);

  bool drawData = tags.getTagBoolDefault ("style.drawData",true);
  bool verbose = tags.getTagBoolDefault("verbose",false);

  // the nominal coordinates of the legend

  // calculate the number of entries
  int nEntries = 0;
  //@tag: style.showMissing: show empty legend entries where histogram is empty or could not be retrieved (default:true)
  bool showMissing = tags.getTagBoolDefault ("style.showMissing",true );

  nEntries += (showMissing ? histos->GetEntriesFast() : histos->GetEntries());
  TH1* hTotalStack = this->getObject<TH1>("totalStack");
  if (showTotalBkg && (hTotalStack || (showMissing && this->getNProcesses(".isBackground") > 0))) nEntries++;

  // calculate the height of the legend
  int nLegendRows = (int)nEntries / nLegendCols + ((nEntries % nLegendCols) > 0 ? 1 : 0);

  TLegend* legend = NULL;
  // @tags:[geometry.legend.xMin,geometry.legend.xMax,geometry.legend.yMin,geometry.legend.yMax]: control the geometry of the legend in relative coordinates
  bool legpad = tags.getTagBoolDefault("style.useLegendPad",false);
  //@tags:style.useLegendPad: put the legend on a separate pad on the side of the plot
  if(legpad){
    if(verbose) VERBOSEclass("creating legend with unity coordinates");
    legend = new TLegend(tags.getTagDoubleDefault("geometry.legend.margins.left",0),
                         tags.getTagDoubleDefault("geometry.legend.margins.bottom",0),
                         1.-tags.getTagDoubleDefault("geometry.legend.margins.right",0),
                         1.-tags.getTagDoubleDefault("geometry.legend.margins.top",0));
    legend->SetFillColor(0);
    legend->SetFillStyle(0);
  } else {
    // if we plot the ratio, the canvas has to be divided which results in a
    // scaling of the legend. To avoid this, we have to rescale the legend's
    // position
    double x1 = tags.getTagDoubleDefault("geometry.legend.xMin",0.59);
    double y1 = tags.getTagDoubleDefault("geometry.legend.yMin",0.75);
    double x2 = tags.getTagDoubleDefault("geometry.legend.xMax",0.90);
    double y2 = tags.getTagDoubleDefault("geometry.legend.yMax",0.92);

    y1 = y2 - (y2 - y1) * scaling;
    legendHeight *= (y2 - y1) * (double)nLegendRows / tags.getTagDoubleDefault("geometry.legend.nRows",5.);

    // set the height of the legend
    y1 = y2 - legendHeight;
    // create the legend and set some attributes
    double tmpx1 = x1;
    double tmpx2 = x2;
    double tmpy1 = y1;
    double tmpy2 = y2;
    if(verbose) VERBOSEclass("creating legend with coordinates %g/%g - %g/%g",tmpx1,tmpy1,tmpx2,tmpy2);
    legend = new TLegend(tmpx1, tmpy1, tmpx2, tmpy2);
    //@tags:[style.legend.fillColor,style.legend.fillStyle]: control color and style of the legend with TLegend::SetFillColor and TLegend::SetFillStyle. defaults are 0.
    legend->SetFillColor(tags.getTagIntegerDefault("style.legend.fillColor",0));
    legend->SetFillStyle(tags.getTagIntegerDefault("style.legend.fillStyle",0));
  }
  legend->SetBorderSize(0);
  legend->SetNColumns(nLegendCols);

  //@tags: style.legend.textSize: control the font size (floating point number, default is 0.0375)
  //@tags: style.legend.textSizeFixed: boolean to control whether the text size will be interpreted relative to canvas size (default) or absolute
  double textsize = tags.getTagDoubleDefault("style.legend.textSize",0.45*tags.getTagDoubleDefault("style.textSize",0.0375));
  if (tags.getTagBoolDefault ("style.legend.textSizeFixed", false))
    legend->SetTextSize(textsize);
  else
    legend->SetTextSize(textsize * scaling);

  // show the error band on SM MC backgrounds in the legend. We have to use a
  // dummy histogram for the legend to get the correct appearance

  bool statMcErrors = tags.getTagBoolDefault("errors.drawStatMC",true );
  bool sysMcErrors = tags.getTagBoolDefault("errors.drawSysMC",false ) || tags.hasTag("errors.showSys");
  TH1* hTotalStackError = NULL;
  if (statMcErrors || sysMcErrors) {
    hTotalStackError = TQHistogramUtils::copyHistogram(hTotalStack,"totalStackError");
    if(hTotalStackError){
      hTotalStackError->Reset();
      hTotalStackError->SetTitle("total background error (legend dummy)");
      this->applyStyle (tags,hTotalStackError,"main.totalStackError");
      if(verbose){ DEBUGclass("totalStackError style: %s",TQHistogramUtils::getDetailsAsString(hTotalStackError,5).Data()); }
    }
  }

  // create the SM legend entry label depending on which error is shown as a
  // band around it
  TString totalStackLabel = tags.getTagStringDefault ("labels.totalStack", "SM");
  TString legendTotalBkgLabel = tags.getTagStringDefault ("labels.totalStack", "SM");
  legendTotalBkgLabel = " " + tags.getTagStringDefault ("labels.legendTotalBkg", legendTotalBkgLabel); // overwrite if you explicitly want it different in legend
  if(tags.getTagBoolDefault("legend.showTotalBkgErrorType",true)){
    if (statMcErrors && sysMcErrors)
      legendTotalBkgLabel.Append(" (sys #oplus stat)");
    else if (sysMcErrors)
      legendTotalBkgLabel.Append(" (sys)");
    else if (statMcErrors)
      legendTotalBkgLabel.Append(" (stat)");
  }
  if (tags.getTagBoolDefault("isCompPlot",false))
    addAllHistogramsToLegend(tags,legend, ".isBackground", tags.getTagStringDefault("legend.dataDisplayType",".legendOptions='lep'"));

  if(!tags.getTagBoolDefault("style.unsorted",false) || tags.getTagBoolDefault("style.listDataFirst", false)){
    // add the data processes
    if (drawData) {
      addAllHistogramsToLegend(tags,legend, ".isData", tags.getTagStringDefault("legend.dataDisplayType",".legendOptions='lep'"));
    }
  }

  // add the total background histogram to the legend
  if (showTotalBkg) {
    if (hTotalStackError){
      if(verbose) VERBOSEclass("adding total stack error to legend");
      TString opt = tags.getTagStringDefault("legend.errorDisplayType","lf");
      if(!opt.Contains("l")){
        hTotalStackError->SetLineColor(0);
        hTotalStackError->SetLineStyle(0);
        hTotalStackError->SetLineWidth(0);
      }
      DEBUGclass(TQHistogramUtils::getDetailsAsString(hTotalStackError,5));
      legend->AddEntry(hTotalStackError, legendTotalBkgLabel,opt);
    } else {
      if(verbose) VERBOSEclass("no total stack error found");
      if (showMissing && this->getNProcesses(".isBackground") > 0){
        legend->AddEntry((TObject*)NULL,"","");
      }
    }
  }

  bool stackSignal = tags.getTagBoolDefault ("style.stackSignal",false);
  bool autoStackSignal = tags.getTagBoolDefault ("style.autoStackSignal",false);
  bool listSignalFirst = tags.getTagBoolDefault ("style.listSignalFirst",false);
  bool showSignal = tags.getTagBoolDefault ("style.showSignalInLegend",true);
  if(tags.getTagBoolDefault("style.unsorted",false)){
    //@tag:style.unsorted: do not apply any sorting whatsoever, list all processes in the order in which they were added
    if (tags.getTagBoolDefault("style.listDataFirst", false)){
    //@tag:style.listDataFirst: show data before anything else on the legend
      addAllHistogramsToLegend(tags,legend, ".isBackground");  
      addAllHistogramsToLegend(tags,legend, ".isSignal");    
    }
    else{
      addAllHistogramsToLegend(tags,legend, ""); 
    }   
  } else {
    if (!tags.getTagBoolDefault ("style.manualStacking", false)) {
      if(!tags.getTagBoolDefault("style.autoStackLegend",false)){
	// add the background and signal processes
	if(verbose) VERBOSEclass("generating legend in default mode");
	if(listSignalFirst){
	  if (showSignal)
	    addAllHistogramsToLegend(tags,legend, ".isSignal");
	  if (!tags.getTagBoolDefault("isCompPlot",false))
	    addAllHistogramsToLegend(tags,legend, ".isBackground");
	} else {
	  if (!tags.getTagBoolDefault("isCompPlot",false))
	    addAllHistogramsToLegend(tags,legend, ".isBackground");
	  if (showSignal)
	    addAllHistogramsToLegend(tags,legend, ".isSignal");
	}
      } else {
	THStack* stack = this->getObject<THStack>("stack");
	if(!stack){
	  if(verbose){
	    VERBOSEclass("cannot generate legend in auto-stack mode - no stack!");
	  }
	  return;
	} else {
	  if(verbose) VERBOSEclass("generating legend in auto-stack mode");
	  if (!stackSignal && listSignalFirst && showSignal && !autoStackSignal) {
	    addAllHistogramsToLegend(tags,legend, ".isSignal");
	  }
	  TQTH1Iterator itr(stack->GetHists()->MakeIterator(kIterBackward),true);
	  while(itr.hasNext()){
	    TH1* hist = itr.readNext();
	    addHistogramToLegend(tags,legend,hist);
	  }
	  if (!stackSignal && !listSignalFirst && showSignal  && !autoStackSignal) {
	    addAllHistogramsToLegend(tags,legend, ".isSignal");
	  }
	}
      }
    } else {
      if (stackSignal) {
	if(verbose) VERBOSEclass("generating legend in manual stacking mode - stackSignal=true");
	if (listSignalFirst) {
	  if (showSignal)
	    addAllHistogramsToLegend(tags,legend, ".isSignal","",true);
	  addAllHistogramsToLegend(tags,legend, ".isBackground","",true);
	} else {
	  addAllHistogramsToLegend(tags,legend, ".isBackground","",true);
	  addAllHistogramsToLegend(tags,legend, ".isSignal","",true);
	}
      } else {
	if(verbose) VERBOSEclass("generating legend in manual stacking mode - stackSignal=false");
	if (listSignalFirst) {
        if (showSignal)
          addAllHistogramsToLegend(tags,legend, ".isSignal");
        addAllHistogramsToLegend(tags,legend, ".isBackground","",true);
	} else {
	  addAllHistogramsToLegend(tags,legend, ".isBackground","",true);
	  if (showSignal)
	    addAllHistogramsToLegend(tags,legend, ".isSignal");
	}
      }
    }
  }
  this->addObject(legend,"legend");
}

//__________________________________________________________________________________|___________

bool TQROOTPlotter::drawHeatmap(TQTaggable& tags){
  // draw a heatmap
  // this is a TH2 plotting mode
  TString heatmap = "totalStack";
  TString overlay = "totalSig";
  //@tag:[style.heatmap] If this tag is set a heatmap ('colz') style 2D histogram is drawn for the histogram (read: process, e.g., "hist_Zjets") instead of contour ones
  if(!tags.getTagString("style.heatmap",heatmap)) return false;
  TPad* pad = this->getPad("main");
  pad->cd();
  //@tag:[style.drawGrid] Draws a grid on heatmap plots if set to true. Default: false.
  if (tags.getTagBoolDefault("style.drawGrid",false)) pad->SetGrid(1,1);
  TH2* hMaster = this->getObject<TH2>("Graph_master");
  TH2* hHeatmap = this->getObject<TH2>(heatmap);
  TH2* hOverlay = NULL;
  bool verbose = tags.getTagBoolDefault("verbose",false);
  bool doOverlay = tags.getTagString("style.heatmapoverlay",overlay);

  bool logScale = tags.getTagBoolDefault("style.logScale",false);
  if (logScale) pad->SetLogz();

  if(doOverlay){
    if(verbose) VERBOSEclass("retrieving overlay histogram by name '%s'",overlay.Data());
    hOverlay = this->getObject<TH2>(overlay);
  }

  if(hHeatmap){
    hMaster->SetMaximum(hHeatmap->GetMaximum());
    hMaster->SetMinimum(hHeatmap->GetMinimum());
  }
  /*
  // should this be used at all?
  if(hOverlay){
    hMaster->SetMaximum(hOverlay->GetMaximum());
    hMaster->SetMinimum(hOverlay->GetMinimum());
  }
  */

  if(verbose) VERBOSEclass("drawing master histogram");
  hMaster->Draw("HIST");

  if(hHeatmap){
    if(verbose) VERBOSEclass("drawing histogram '%s' as heatmap",heatmap.Data());
    hHeatmap->Draw("COLZSAME");
  } else {
    if(verbose){
      VERBOSEclass("cannot draw '%s' as heatmap - object not found",heatmap.Data());
      this->printObjects();
    }
    return false;
  }

  if(hOverlay){
    if(verbose) VERBOSEclass("drawing histogram '%s' as heatmap overlay",overlay.Data());
    hOverlay->Draw("BOXSAME");
  } else if(doOverlay){
    if(verbose){
      VERBOSEclass("cannot draw '%s' as heatmap overlay - object not found",overlay.Data());
      this->printObjects();
    }
  }

  return true;
}

//__________________________________________________________________________________|___________

bool TQROOTPlotter::drawContours(TQTaggable& tags){
  // draw a contour plot
  // this is a TH2 plotting mode
  TPad* pad = this->getPad("main");
  TH2* hMaster = this->getObject<TH2>("Graph_master");
  TObjArray* histos = this->getObject<TObjArray>("histos");
  bool verbose = tags.getTagBoolDefault("verbose",false);

  // select the z-values of the contours to be drawn
  //@tag:[style.logMin,style.logMinRel] These argument tags determine the minimum value contour plots in logarithmic scale. Default of "logMin" is 1. If the latter one is set, the minimum of the scale is taken as the product of the "logMinRel" tag's value and the highest bin content of the underlying histogram.
  double logMin = tags.getTagDoubleDefault("style.logMin",1.);
  bool logScale = tags.getTagIntegerDefault("style.logScale",false);
  double max = TQHistogramUtils::getMax(histos,false);
  //if the user has set a relative (to the maximum value) lower boundary for log scaled histograms use that one
  if (logScale && tags.hasTagDouble("style.logMinRel") ) logMin = max*tags.getTagDoubleDefault("style.logMinRel",42.); //the default value should never do anything, and if it does we want it to be obvious that something is wrong
  double min = logScale ? std::max(logMin,TQHistogramUtils::getMin(histos,false)) : TQHistogramUtils::getMin(histos,false);

  size_t nContours = tags.getTagIntegerDefault("style.nContours",6);
  double step = logScale ? (log(max) - log(min))/(nContours+1) : (max-min)/(nContours+1);
  std::vector<double> contours;
  for(size_t i=0; i<nContours; i++){
    double z_orig = logScale ? min*exp((i+1)*step) : min+(i+1)*step;
    double z = TQUtils::roundAuto(z_orig,1);
    contours.push_back(z);
  }

  // create the contour graphs
  TObjArray* contourGraphs = new TObjArray();
  std::vector<double> contourVals;
  //@tag:[style.minContourArea] This argument tag determines up to what size contours are omitted. Default is three time the minimum bin area of the master histogram. Removing this limit would create PDFs easily reaching ~100MByte!
  double minContourArea = tags.getTagDoubleDefault("style.minContourArea",3*TQHistogramUtils::getMinBinArea(hMaster));
  bool batch = gROOT->IsBatch();
  gROOT->SetBatch(true);
  //@tag:[style.doContourLevelsPerHistogram] If this argument tag is set to true countour levels (in contour plots) are determined for each process separately. Default: false.
  bool contourLevelsPerHistogram = tags.getTagBoolDefault("style.doContourLevelsPerHistogram",false);
  TQIterator histItr(histos);
  while(histItr.hasNext()){
    // create a temporary canvas and draw the histogram to create the contours
    TCanvas* tmp = new TCanvas("tmp","tmp");
    TH2* hist = dynamic_cast<TH2*>(histItr.readNext());
    if(tags.getTagBoolDefault("style.smooth",false)){
      hist->Smooth(1,"k5a");
    }
    if(!hist) continue;
    if (contourLevelsPerHistogram) {
      contours.clear();
      max = TQHistogramUtils::getMax(hist,false,false);
      //if the user has set a relative (to the maximum value) lower boundary for log scaled histograms use that one
      if (logScale && tags.hasTagDouble("style.logMinRel") ) logMin = max*tags.getTagDoubleDefault("style.logMinRel",42.); //the default value should never do anything, and if we want it to be obvious that something is wrong
      min = logScale ? std::max(logMin,TQHistogramUtils::getMin(hist,false,false)) : TQHistogramUtils::getMin(hist,false,false);
      //@tag:[style.nContours] This argument tag sets the number of contour levels shown in contour plots. Default is 6 unless "style.doContourLevelsPerHistogram" is set to true, in which case default is 2.
      size_t nContours = tags.getTagIntegerDefault("style.nContours",2);
      double step = logScale ? (log(max) - log(min))/(nContours+1) : (max-min)/(nContours+1);
      for(size_t i=0; i<nContours; i++){
        double z_orig = logScale ? min*exp((i+1)*step) : min+(i+1)*step;
        double z = TQUtils::roundAuto(z_orig,1);
        contours.push_back(z);
      }
    }
    hist->SetContour(contours.size(), &contours[0]);
    if(verbose) VERBOSEclass("drawing contours for %s",hist->GetName());
    hist->Draw("CONT Z LIST");
    // Needed to force the plotting and retrieve the contours in TGraphs
    tmp->Update();
    // retrieve the contours
    TQIterator contItr2(dynamic_cast<TObjArray*>(gROOT->GetListOfSpecials()->FindObject("contours")));
    while(contItr2.hasNext()){
      // contours by level
      TList* contLevel = dynamic_cast<TList*>(contItr2.readNext());
      int idx = contItr2.getLastIndex();
      double z0 = contours[idx];
      if(verbose) VERBOSEclass("\tretrieving %d contours for level %f",contLevel->GetEntries(),z0);
      int nGraphs = 0;
      std::vector<double> contourAreas;
      std::vector<double> contourIndices;
      TQIterator contItr3(contLevel);
      while(contItr3.hasNext()){
        // individual graphs per contour level
        TGraph* curv = dynamic_cast<TGraph*>(contItr3.readNext());
        // veto against non-existant contours
        if(!curv) continue;
        double area = fabs(TQHistogramUtils::getContourArea(curv));
        double triangle = 0.5*pow(TQHistogramUtils::getContourJump(curv),2);
        double val = std::max(area,triangle);
        if(verbose) VERBOSEclass("\t\tcontour %d has area=%f and triangle jump=%f -- contour value is %f",nGraphs,area,triangle,val);
        contourAreas.push_back(val);
        contourIndices.push_back(contItr3.getLastIndex());
        nGraphs++;
      }
      if(verbose) VERBOSEclass("identified %i non-vanishing contours",contourAreas.size());
      nGraphs = 0;
      int nContoursMax = tags.getTagIntegerDefault("style.contourLimit",7);
      while(nGraphs < nContoursMax){
        size_t index = 0;
        double max = 0;
        for(size_t i=0; i<contourAreas.size(); i++){
          if(contourAreas[i] > max){
            max = contourAreas[i];
            index = contourIndices[i];
            contourAreas[i] = 0;
          }
        }
        // veto against micro-blob contours
        TGraph* curv = dynamic_cast<TGraph*>(contLevel->At(index));
        if(max < minContourArea && nGraphs > 0) {
          DEBUGclass("removing micro-blob");
          break;
        }

        if(max <= 0) break;
        // individual graphs per contour level
        // veto against non-existant contours
        if(!curv) continue;
        // create clones of the graphs to avoid deletions
        TGraph* gc = dynamic_cast<TGraph*>(curv->Clone());
        if(!gc) continue;
        // apply the styles to the graphs
        int color = hist->GetFillColor();
        int style = 1;
        if((color == kWhite) || (color == 0)){
          color = hist->GetLineColor();
          style = TQStringUtils::equal("hist_data",hist->GetName()) ? 3 : 7;
        }
        gc->SetLineColor(color);
        gc->SetLineStyle(style);
        if(tags.getTagBoolDefault("style.contourLines.shade",false)){
          gc->SetFillStyle(3004+ (histItr.getLastIndex() % 4));
          gc->SetLineWidth(-100
                           *TQUtils::sgn(tags.getTagIntegerDefault("style.contourLines.shadeDirection",1))
                           *tags.getTagIntegerDefault("style.contourLines.shadeWidth",3)
                           + tags.getTagIntegerDefault("style.contourLines.width",1));
        } else {
          gc->SetLineWidth(tags.getTagIntegerDefault("style.contourLines.width",1));
        }
        gc->SetTitle(TString::Format("%s: contour #%d to level %g",hist->GetTitle(),(int)nGraphs,z0));
        this->addObject(gc,TString::Format("contour_%d_%s_%g_%d",contourGraphs->GetEntries(),hist->GetName(),z0,(int)nGraphs));
        contourGraphs->Add(gc);
        contourVals.push_back(z0);
        nGraphs++;
      }
      if(verbose) VERBOSEclass("\tretrieved %d (dismissed all others)",nGraphs);
    }
    // delete the temporary canvas
    delete tmp;
  }
  this->addObject(contourGraphs,"contours");
  if(!batch) gROOT->SetBatch(false);

  // switch to the correct pad
  pad->cd();
  hMaster->Draw("hist");

  // prepare the TLatex object for drawing the labels
  TLatex l;
  bool autoColor = tags.getTagBoolDefault("style.contourLabels.autocolor",false);
  int color = tags.getTagIntegerDefault("style.contourLabels.color",kBlack);
  double size = tags.getTagDoubleDefault("style.contourLabels.size",0.03);
  l.SetTextSize(size);
  l.SetNDC(true);
  if(!autoColor) l.SetTextColor(color);

  std::vector<double> labelSpotsX;
  std::vector<double> labelSpotsY;
  TQIterator itrGraphs(contourGraphs);
  while(itrGraphs.hasNext()){
    // iterate over the contour graphs
    TGraph* gc = dynamic_cast<TGraph*>(itrGraphs.readNext());
    if(!gc) continue;
    // actually draw the contour graph
    gc->Draw("C");
    // choose a "central" point of the graph
    // to retrieve coordinates for the label
    int index = 0.5*gc->GetN();
    int indexStep = 1;
    double x0, y0, z0;
    double xNDC, yNDC;
    // create the label text
    z0 = contourVals[itrGraphs.getLastIndex()];
    TString val = TString::Format("%g",z0);
    double minDistX = 0.5*size*TQStringUtils::getWidth(val);
    double minDistY = size;
    // find a location to draw the label
    bool acceptPosition = false;
    if(tags.getTagBoolDefault("style.contourLabels.avoidCollisions",true)){
      while(index > 0 && index < gc->GetN()){
        acceptPosition = true;
        gc->GetPoint(index, x0, y0);
        xNDC = TQUtils::convertXtoNDC(x0);
        yNDC = TQUtils::convertYtoNDC(y0);
        for(size_t i=0; i<labelSpotsX.size(); i++){
          double dx = fabs(xNDC - labelSpotsX[i]);
          double dy = fabs(yNDC - labelSpotsY[i]);
          if((dx < minDistX) && (dy < minDistY)) acceptPosition = false;
        }
        if(acceptPosition) break;
        index += indexStep;
        indexStep = -TQUtils::sgn(indexStep)*(abs(indexStep)+1);
      }
    }
    if(!acceptPosition){
      if(verbose) VERBOSEclass("did not find any suitable label position, using default");
      gc->GetPoint((int)(0.5*gc->GetN()), x0, y0);
      xNDC = TQUtils::convertXtoNDC(x0);
      yNDC = TQUtils::convertYtoNDC(y0);
    }
    labelSpotsX.push_back(xNDC);
    labelSpotsY.push_back(yNDC);
    // choose a color
    if(autoColor) l.SetTextColor(gc->GetLineColor());
    // draw the label
    l.DrawLatex(xNDC,yNDC,val);
    if(verbose) VERBOSEclass("drawing label '%s' at (%f,%f) == (%f,%f) with minDist=(%f,%f)",val.Data(),x0,y0,xNDC,yNDC,minDistX,minDistY);
  }
  return true;
}

//__________________________________________________________________________________|___________

bool TQROOTPlotter::drawMigration(TQTaggable& tags){
  // draw a migration plot
  // this is a TH2 plotting mode
  TString migration = "ggf";
  //@tag:[style.migration] If this tag is set a migration ('col4z') style 2D histogram is drawn for the histogram (read: process, e.g., "hist_Zjets") instead of contour or heatmap ones
  if(!tags.getTagString("style.migration",migration)) return false;
  TPad* pad = this->getPad("main");
  TH2* hMaster = this->getObject<TH2>("Graph_master");
  TQTaggableIterator itr_sig(fProcesses);
  TQNamedTaggable* process_mig = NULL;
  while(itr_sig.hasNext()){
    TQNamedTaggable* process = itr_sig.readNext();
    if(!process) continue;
    if(process->getTagBoolDefault(".isData",false)) continue;
    if(process->getName() == migration)
      process_mig = process;
  }
  TH2* hMigration = this->getObject<TH2>(this->makeHistogramIdentifier(process_mig));
  bool verbose = tags.getTagBoolDefault("verbose",false);
  bool logScale = tags.getTagBoolDefault("style.logScale",false);
  if (logScale) pad->SetLogz();

  hMaster->Draw("HIST");

  if(hMigration){
    if(verbose) VERBOSEclass("drawing histogram '%s' as migration",migration.Data());
    for(int i=0; i <= hMigration->GetNbinsY(); i++) {
      double integral = hMigration->Integral(0,hMigration->GetNbinsY()+1,i,i);
      if(integral > 0){
        for(int j=0; j <= hMigration->GetNbinsX(); j++) {
          double bincontent = hMigration->GetBinContent(j,i)/integral*100;
          hMigration->SetBinContent(j,i,bincontent);
        }
      }
    }
    gStyle->SetPaintTextFormat("4.1f");
    hMigration->SetContour(99);
    hMigration->Draw("col4zsame");
    hMigration->SetMarkerColor(kBlack);
    hMigration->SetMarkerSize(1.4);
    hMigration->Draw("textsame");
  } else {
    if(verbose){
      VERBOSEclass("cannot draw '%s' as migration - object not found",migration.Data());
      this->printObjects();
    }
    return false;
  }

  return true;

}

//__________________________________________________________________________________|___________

bool TQROOTPlotter::drawStack(TQTaggable& tags){
  // draw the stack produced by stackHistograms
  // @tag:errors.drawStatMC: control drawing of statistical MC errors (default:true)
  // @tag:errors.drawSysMC: control drawing of systematic MC errors (default:false; deprecated, use errors.totalBkgSys)
  // @tag:errors.showSys: alias for errors.totalBkgSys
  // @tag:errors.totalBkgSys: include in the total background error the specified systematics tag
  // @tag:style.showTotalBkg: control display of additional line (and legend entry) for total background (default:true)
  // @tag:style.drawData: control whether data points will be shown (default:true)
  // @tag:style.data.asymErrors: show asymmetric errors (poisson errors) for data points

  this->getPad("main");
  bool statMcErrors = tags.getTagBoolDefault("errors.drawStatMC",true );
  bool sysMcErrors = tags.getTagBoolDefault("errors.drawSysMC",false );
  bool showTotalBkg = tags.getTagBoolDefault ("style.showTotalBkg",true);
  bool drawData = tags.getTagBoolDefault ("style.drawData",true);
  bool asymErrorsData = tags.getTagBoolDefault("style.data.asymErrors",false);
  bool verbose = tags.getTagBoolDefault("verbose",false);
  double scaling = tags.getTagDoubleDefault("geometry.main.scaling",1.);

  TH1* hMaster = this->getObject<TH1>("Graph_master");
  TH1* hTotalStack = this->getObject<TH1>(tags.getTagStringDefault("errors.shiftTo","totalStack"));
  if(!hMaster) return false;

  // the first histogram to draw is the SM histogram.
  hMaster->Draw("hist");

  //////////////////////////////////////////////////////
  // error band on background MC
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("creating MC error band");
  // all this only makes sense if we have a histogram representing the "total background"
  TGraphAsymmErrors * errorGraph = 0;
  if (hTotalStack && (statMcErrors || sysMcErrors)){
    TObjArray* histosAsymmSys = this->getObject<TObjArray>("asymmSys");
    if(histosAsymmSys) {
      errorGraph = TQHistogramUtils::getGraph(hTotalStack, histosAsymmSys);
    } else {
      errorGraph = TQHistogramUtils::getGraph(hTotalStack);
    }
    this->applyStyle(tags,errorGraph,"main.totalStackError",1.,scaling);
    this->addObject(errorGraph,"totalStackErr");
  }

  if(tags.getTagBoolDefault("errors.showX",true)){
    double errWidthX = 0.5;
    if(tags.getTagDouble("errors.widthX", errWidthX))
      gStyle->SetErrorX(errWidthX);
  } else {
    gStyle->SetErrorX(0.);
  }

  //////////////////////////////////////////////////////
  // calculate axis ranges
  // rescale to avoid graphical collisions
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("calculating axis ranges & rescaling histograms");
  bool axisOK = this->calculateAxisRanges1D(tags);
  if(!axisOK){
    if(verbose) VERBOSEclass("encountered invalid axis ranges, using defaults");
  }

  //////////////////////////////////////////////////////
  // draw everything
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("drawing backgrounds");

  THStack* stack = this->getObject<THStack>("stack");
  TString stackDrawOptions = tags.getTagStringDefault ("style.stackDrawOptions","hist");

  // draw the backgrounds
  if (hTotalStack && stack) {
    stack->Draw(stackDrawOptions + " same");
    if (showTotalBkg && errorGraph) errorGraph->Draw("2");
  } else {
    if (verbose) VERBOSEclass("failed to obtain stack");
  }

  //@tag:stack: process tag to steer whether this process will be added to the stack (default: true for bkg, else false)
  //@tag:drawOptions: control the draw options of this process (default: 'hist' for MC, 'ep' for data)

  if(verbose) VERBOSEclass("drawing signal (and other non-stacked histograms)");
  // draw signal
  bool stackSignal = tags.getTagBoolDefault ("style.stackSignal",false);
  TQTaggableIterator itr_sig(fProcesses);
  while(itr_sig.hasNext()){
    TQNamedTaggable* process = itr_sig.readNext();
    if(!process) continue;
    if(process->getTagBoolDefault(".isData",false)) continue;
    if(!(process->getTagBoolDefault("stack",process->getTagBoolDefault(".isBackground") || stackSignal))){
      TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
      if(!h) continue;
      TString drawOpt = process->getTagStringDefault("drawOptions", "hist") + " same";
      if(process->getTagBoolDefault("stackShift",false)){
        TH1* hcopy = TQHistogramUtils::copyHistogram(h,TString::Format("%s_shifted",h->GetName()));
        hcopy->Add(hTotalStack);
        hcopy->Draw(drawOpt);
      } else {
        h->Draw(drawOpt);
      }
    }
  }

  // draw data
  if (drawData) {
    if(verbose) VERBOSEclass("drawing data");
    TQTaggableIterator itr_data(fProcesses);
    while(itr_data.hasNext()){
      TQNamedTaggable* process = itr_data.readNext();
      if(!process) continue;
      if(!process->getTagBoolDefault(".isData",false)) continue;
      TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
      if(!h) continue;
      this->applyStyle(tags,h,"main.data");
      if (asymErrorsData) {
        h->Sumw2(false); //only do this for data, since uncertainty is sqrt(n). However, it is needed for kPoisson to take effect
        h->SetBinErrorOption(TH1::kPoisson);
      }
      TString drawOpt = process->getTagStringDefault("drawOptions","ep") + " same";
      if(verbose)
        VERBOSEclass("drawing data histogram '%s' with option '%s'", h->GetName(),drawOpt.Data());
      h->Draw(drawOpt);
    }
  }

  if (tags.getTagBoolDefault("isCompPlot",false))
    stack->Draw(stackDrawOptions + " same");
  return true;
}

//__________________________________________________________________________________|___________

bool TQROOTPlotter::drawProfiles(TQTaggable& tags){
  // draw background TProfiles
  this->getPad("main");
  bool verbose = tags.getTagBoolDefault("verbose",false);

  TProfile* hMaster = this->getObject<TProfile>("Graph_master");
  TProfile* hTotalStack = this->getObject<TProfile>("totalStack");
  if(!hMaster || !hTotalStack) return false;
  //hOverlay = this->getObject<TProfile>(overlay);
  hMaster->SetMaximum(hMaster->GetYmax());
  hMaster->SetMinimum(hMaster->GetYmin());

  //////////////////////////////////////////////////////
  // calculate axis ranges
  // rescale to avoid graphical collisions
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("calculating axis ranges & rescaling histograms");
  bool axisOK = this->calculateAxisRangesProfile(tags);
  if(!axisOK){
    if(verbose) VERBOSEclass("encountered invalid axis ranges, using defaults");
  }

  // the first histogram to draw is the SM histogram.
  hMaster->Draw("hist");

  //////////////////////////////////////////////////////
  // draw everything
  //////////////////////////////////////////////////////

  if(verbose) VERBOSEclass("drawing profiles");

  // draw the backgrounds
  TQTaggableIterator itr_bkg(fProcesses);
  while(itr_bkg.hasNext()){
    TQNamedTaggable* process = itr_bkg.readNext();
    TString tmp1 = process->getName();
    if(verbose){
      INFOclass("drawing %s",tmp1.Data());
    }
    if(!process) continue;
    TProfile * h = this->getObject<TProfile>(this->makeHistogramIdentifier(process));
    if(!h) continue;
    this->applyStyle(tags,h,"main.bkg");
    //@tag:drawOptions: control the draw options of this process (default: 'hist' for MC, 'ep' for data)
    h->Draw(process->getTagStringDefault("drawOptions", "ep") + " same");
  }

  //@tag:stack: process tag to steer whether this process will be added to the stack (default: true for bkg, else false)
  return true;
}

//__________________________________________________________________________________|___________

void TQROOTPlotter::drawLegend(TQTaggable& tags){
  // draw the legend produced by TQROOTPlotter::makeLegend
  bool verbose = tags.getTagBoolDefault("verbose",false);
  bool legpad = tags.getTagBoolDefault("style.useLegendPad",false);
  //@tag:[style.showLegend] Controls if legend is shown. Default: true
  if (!tags.getTagBoolDefault("style.showLegend",true)) return;
  TLegend* legend = this->getObject<TLegend>("legend");
  if(legpad){
    if(verbose) VERBOSEclass("drawing legend pad");
    if(!this->getPad("legend")){
      ERRORclass("error retrievling legend pad!");
    }
    legend->Draw();
  } else {
    if(verbose) VERBOSEclass("drawing legend on-pad");
    this->getPad("main");
    legend->Draw("same");
  }
}

//__________________________________________________________________________________|___________

bool TQROOTPlotter::calculateAxisRanges1D(TQTaggable& tags){
  // calculate the axis ranges, taking into account the given block tags
  bool logScale = tags.getTagBoolDefault ("style.logScale",false );
  bool drawData = tags.getTagBoolDefault ("style.drawData",true);
  bool verbose = tags.getTagBoolDefault("verbose",false);

  TH1* hTotalStack = this->getObject<TH1>("totalStack");
  TList* histograms = new TList();
  if (hTotalStack) histograms->Add(hTotalStack);


  double min = std::numeric_limits<double>::infinity();
  TQTaggableIterator itr(fProcesses);
  while(itr.hasNext()){
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(process->getTagBoolDefault(".isData",false) && !drawData) continue;
    TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
    if(!h) continue;
    histograms->Add(h);
    //ignore empty/very small bins for log scale plots when trying to automatically determine axis range
    min = std::min(min,TQHistogramUtils::getMin(h, true, true, logScale ? tags.getTagDoubleDefault("style.logMinMin",1e-9) : -std::numeric_limits<double>::infinity() ));
  }
  //@tag:[style.min,style.logMin,style.linMin,(style.logMinMin)] These argument tags determine the lower bound of the y axis in 1D plots. "style.min" is used unless the specific tag for the plot type (lin/log) is set. Additionally, "style.logMinMin" defaults to 1e-9 and acts as an additional lower bound; use with great care!
  tags.getTagDouble("style.min", min);
  if(logScale){
    tags.getTagDouble("style.logMin",min);
  } else {
    tags.getTagDouble("style.linMin",min);
  }

  if(logScale && min < tags.getTagDoubleDefault("style.logMinMin",1e-9) ) min = tags.getTagDoubleDefault("style.logMinMin",1e-9);

  double max_precise = this->getHistogramUpperLimit(tags, histograms,min,true);
  delete histograms;

  double max;
  if(max_precise <= 0 || !TQUtils::isNum(max_precise) || max_precise < min){
    max = std::max(2*min,10.);
    if(verbose) VERBOSEclass("using default range");
  } else {
    if(verbose) VERBOSEclass("using rounded range");
    max = TQUtils::roundAutoUp(max_precise);
  }

  if(verbose) VERBOSEclass("calculated y-axis range is %g < y < %g (%g)",min,max,max_precise);

  // the user might want to overwrite the automatic upper limit on the y axis
  tags.getTagDouble("style.max", max);
  double maxscale = 1.0;
  tags.getTagDouble("style.max.scale", maxscale);

  TH1* hMaster = this->getObject<TH1>("Graph_master");
  hMaster->SetMinimum(min);
  hMaster->SetMaximum(max * maxscale);

  //@tag:[style.xmin,style.xmax] Set custom x-axis range
  double xmin;
  double xmax;
  if (tags.getTagDouble("style.xmin", xmin)) {
    hMaster->GetXaxis()->SetRangeUser(xmin, TQHistogramUtils::getAxisXmax(hMaster));
    hTotalStack->GetXaxis()->SetRangeUser(xmin, TQHistogramUtils::getAxisXmax(hTotalStack));
  }
  if (tags.getTagDouble("style.xmax", xmax)) {
    hMaster->GetXaxis()->SetRangeUser(TQHistogramUtils::getAxisXmin(hMaster), xmax);
    hTotalStack->GetXaxis()->SetRangeUser(TQHistogramUtils::getAxisXmin(hTotalStack), xmax);
  }

  return !(max == 0);
}

//__________________________________________________________________________________|___________

bool TQROOTPlotter::calculateAxisRangesProfile(TQTaggable& tags){
  // calculate the axis ranges, taking into account the given block tags
  bool logScale = tags.getTagBoolDefault ("style.logScale",false );
  double logMin = tags.getTagDoubleDefault("style.logMin", -1.);
  bool drawData = tags.getTagBoolDefault ("style.drawData",true);
  bool verbose = tags.getTagBoolDefault("verbose",false);
  TString profileRange = tags.getTagStringDefault("style.profileRange","filtered"); //"full", "predefined", or "filtered"
  
  TList* histograms = new TList();
  
  double ymin = std::numeric_limits<double>::infinity();
  double xmin = std::numeric_limits<double>::infinity();
  double ymax = -std::numeric_limits<double>::infinity();
  double xmax = -std::numeric_limits<double>::infinity();
  double min,max;
  TQTaggableIterator itr(fProcesses);
  while(itr.hasNext()){
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(process->getTagBoolDefault(".isData",false) && !drawData) continue;
    TProfile * h = this->getObject<TProfile>(this->makeHistogramIdentifier(process));
    if(!h) continue;
    histograms->Add(h);
    ymin=std::min(ymin,h->GetYmin() );
    ymax=std::max(ymax,h->GetYmax() );
    xmin=std::min(xmin,h->GetXaxis()->GetXmin());
    xmax=std::max(xmax,h->GetXaxis()->GetXmax());
  }
  if (profileRange.EqualTo("filtered")) {
    TQHistogramUtils::getFilteredRange(histograms, xmin, xmax, ymin, ymax, min, max, logMin);
    min=std::max(min, ymin);
    max=std::min(max, ymax);
    if (min==ymax) min=ymin;
    if (max==ymin) max=ymax;
  } else if (profileRange.EqualTo("predefined")) {
    min=ymin;
    max=ymax;
  } else if (profileRange.EqualTo("full")){
    TQHistogramUtils::getUnFilteredRange(histograms, xmin, xmax, ymin, ymax, min, max);
  }
  
  if(logScale) min = std::max(min,tags.getTagDoubleDefault("style.logMinMin",1e-9));
  
  //rescale to avoid colliding with legend and labels
  int iBlock = 0;
  double block_y;
  double vetoFrac = 1;
  while(tags.getTagDouble(TString::Format("blocks.y.%d",iBlock),block_y)){
    //block_y is a (lowest) y1 coordinate of a block with legend/label
    //avoid the most intruding of all blocks
    vetoFrac = std::min(vetoFrac, block_y);
    iBlock++;
  }
  if (logScale) max = std::max(max, exp(log(max/min) / vetoFrac ) * min);
  else          max = std::max(max, (max - min) / vetoFrac + min);
  
  delete histograms;
  
  if(verbose) VERBOSEclass("calculated y-axis range is %g < y < %g",min,max);
  
  // the user might want to overwrite the automatic upper limit on the y axis
  tags.getTagDouble("style.max", max);
  double maxscale = 1.0;
  tags.getTagDouble("style.max.scale", maxscale);
  
  TProfile* hMaster = this->getObject<TProfile>("Graph_master");
  hMaster->SetMinimum(min);
  hMaster->SetMaximum(max * maxscale);
  
  return !(max == 0);
}

//__________________________________________________________________________________|___________

namespace {
  template<class T> size_t getN(T* obj);
  template<class T> bool getXY(T*obj,size_t i,double& x,double& y);

  template<> size_t getN<TGraphErrors>(TGraphErrors* obj){ return obj->GetN(); }
  template<> size_t getN<TGraph>(TGraph* obj){ return obj->GetN(); }
  template<> size_t getN<TH1>   (TH1* obj){ return obj->GetNbinsX(); }
  template<> bool getXY<TGraph> (TGraph* obj,size_t i,double& x,double& y){ return (i==(size_t)(obj->GetPoint(i,x,y))); }
  template<> bool getXY<TGraphErrors> (TGraphErrors* obj,size_t i,double& x,double& y){ return (i==(size_t)(obj->GetPoint(i,x,y))); }
  template<> bool getXY<TH1>    (TH1* obj,size_t i,double& x,double& y){ x=obj->GetBinCenter(i+1); y=obj->GetBinContent(i+1); return true; }
}

template<class T>
void TQROOTPlotter::drawArrows(TQTaggable &tags,T *obj, double yMin, double yMax){
  // Check if the red arrows should be drawn
  if(!tags.getTagBoolDefault("style.showArrows",true)) return;
  bool verbose = tags.getTagBoolDefault("verbose",false);

  size_t nBins = ::getN<T>(obj);

  double arrowLength = tags.getTagDoubleDefault ("style.arrowLength",0.12 ); // fraction of the y-range
  double arrowOffset = tags.getTagDoubleDefault ("style.arrowOffset",0.08 ); // fraction of the y-range
  int arrowLineWidth = tags.getTagIntegerDefault ("style.arrowLineWidth",2 );
  double arrowHeadSize = tags.getTagDoubleDefault ("style.arrowHeadSize",0.03 );
  double padRatio = tags.getTagDoubleDefault("geometry.sub.height",0.35);

  double canvasHeight = tags.getTagDoubleDefault("geometry.canvas.height",600.);
  double canvasWidth = tags.getTagDoubleDefault("geometry.canvas.width",600.);
  double frameWidthFrac = 1. - tags.getTagDoubleDefault("geometry.sub.margins.right",0.1) - tags.getTagDoubleDefault("geometry.sub.margins.left",0.1);
  double frameWidth = frameWidthFrac * canvasWidth;
  double arrowHeight = arrowHeadSize * canvasHeight;
  double binWidth = frameWidth / nBins;
  double alpha = 2*std::atan(binWidth/(2*arrowHeight)) * 180 / 3.1415926;

  double arrowHeadAngle = tags.getTagDoubleDefault ("style.arrowHeadAngle",std::min(60.,alpha));
  TString arrowType = tags.getTagStringDefault ("style.arrowType", "|>" ); // refer to TArrow
  int arrowColor = tags.getTagIntegerDefault ("style.arrowColor",kRed);

  double plrange = yMax - yMin;

  TArrow marker;
  marker.SetLineWidth(arrowLineWidth);
  marker.SetLineColor(arrowColor);
  marker.SetFillColor(arrowColor);
  marker.SetAngle(arrowHeadAngle);
  for(size_t i=0; i < nBins; ++i){
    double x; double y;
    if(!::getXY<T>(obj,i,x,y)) continue;
    if(y > yMax){
      marker.DrawArrow(x,
                       yMax - (arrowOffset+arrowLength)*plrange,
                       x,
                       yMax - (arrowOffset)*plrange,
                       arrowHeadSize*padRatio,
                       arrowType);
      if(verbose) VERBOSEclass("drawing marker for point %i, y > %f",i,yMax);
    }
    if(y < yMin && y != 0){
      marker.DrawArrow(x,
                       yMin + (arrowOffset+arrowLength)*plrange,
                       x,
                       yMin + arrowOffset*plrange,
                       arrowHeadSize*padRatio,
                       arrowType);
      if(verbose) VERBOSEclass("drawing marker for point %i, y < %f",i,yMin);
    }
  }
}

template void TQROOTPlotter::drawArrows<TGraph>(TQTaggable &tags,TGraph *obj, double yMin, double yMax);
template void TQROOTPlotter::drawArrows<TGraphErrors>(TQTaggable &tags,TGraphErrors *obj, double yMin, double yMax);
template void TQROOTPlotter::drawArrows<TH1>(TQTaggable &tags,TH1 *obj, double yMin, double yMax);
