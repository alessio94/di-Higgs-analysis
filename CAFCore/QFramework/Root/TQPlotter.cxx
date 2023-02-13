#include "QFramework/TQPlotter.h"

#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TObjArray.h"

// #define _DEBUG_

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

#define DEFAULTTEXTSIZE 0.04
////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQPlotter:
//
// The abstract TQPlotter class provides a base class for custom plotters.
// By inheriting from the TQPlotter, a base data management interface is provided.
// For plotting with the ROOT graphics interface, refer to TQROOTPlotter.
//
////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(TQPlotter)

//__________________________________________________________________________________|___________

TQPlotter::TQPlotter() :
TQPresenter(),
  objects(new TDirectory("plotter_tmp",""))
{
  // Constructor of TQPlotter class
}

//__________________________________________________________________________________|___________

TQPlotter::TQPlotter(TQSampleFolder * baseSampleFolder) :
  TQPresenter(baseSampleFolder),
  objects(new TDirectory("plotter_tmp",""))
{
  // Constructor of TQPlotter class
}

//__________________________________________________________________________________|___________

TQPlotter::TQPlotter(TQSampleDataReader * dataSource) :
  TQPresenter(dataSource),
  objects(new TDirectory("plotter_tmp",""))
{
  // Constructor of TQPlotter class
}


//__________________________________________________________________________________|___________

void TQPlotter::reset() {
  TQPresenter::reset();

  // Reset the plotter
  this->clearObjects();
}

//__________________________________________________________________________________|___________

TString TQPlotter::makeHistogramIdentifier(TQNamedTaggable* process){
  TString name = process->getTagStringDefault(".name",process->getTagStringDefault("name",process->GetName()));
  if(TQStringUtils::isEmpty(name) || process->getTagBoolDefault(".ignoreProcessName",false)){
    return "hist_"+TQStringUtils::makeValidIdentifier(process->exportTagsAsString(),
                                                      TQStringUtils::letters+TQStringUtils::numerals+"_","_");
  } else {
    return "hist_"+TQStringUtils::makeValidIdentifier(name,
                                                      TQStringUtils::letters+TQStringUtils::numerals+"_","_");
  }
}

//__________________________________________________________________________________|___________

bool TQPlotter::addData(TString path, TString options) {
  // add a new data process to the plotter
  TQNamedTaggable* data = new TQNamedTaggable(path);
  data->setTagBool(".isData",true);
  data->setTagString(".legendOptions","lep");
  data->setTagString(".path",path);
  data->importTags(options,true);
  this->fProcesses->Add(data);
  return true;
}


//__________________________________________________________________________________|___________

bool TQPlotter::addBackground(TString path, TString options) {
  // add a new background process to the plotter
  TQNamedTaggable* bkg = new TQNamedTaggable(path);
  bkg->setTagBool(".isBackground",true);
  bkg->setTagString(".path",path);
  bkg->importTags(options,true);
  this->fProcesses->Add(bkg);
  return true;
}


//__________________________________________________________________________________|___________

bool TQPlotter::addSignal(TString path, TString options) {
  // add a new signal process to the plotter
  TQNamedTaggable* sig = new TQNamedTaggable(path);
  sig->setTagBool(".isSignal",true);
  sig->setTagString(".path",path);
  sig->importTags(options,true);
  this->fProcesses->Add(sig);
  return true;
}

//__________________________________________________________________________________|___________

TString TQPlotter::getScaleFactorList(TString histname){
  // retrieve a comma-separated list of the scaled contributions (titles only)
  TString cutname;
  if(!TQStringUtils::readUpTo(histname,cutname,"/")) return "";
  if(!this->getNormalizationInfo()) return "";
  TQFolder* f = this->getNormalizationInfo()->getFolder(TString::Format(".cut.%s",cutname.Data()));
  if(!f) return "";
  TString retval = "";
  TQIterator itr(f->getListOfKeys(),true);
  while(itr.hasNext()){
    TObject* obj = itr.readNext();
    if(!obj) continue;
    if(!retval.IsNull()) retval.Append(",");
    retval.Append(f->getTagStringDefault(obj->GetName(),obj->GetName()));
  }
  return retval;
}

//__________________________________________________________________________________|___________

bool TQPlotter::includeSystematics(TQTaggable& tags){
  // include the systematics entry from the systematics folder
  TString sysID = "";
  bool verbose = tags.getTagBoolDefault("verbose",false);
  bool showSys = tags.getTagString("errors.showSys",sysID);
  if(!showSys){
    if(verbose) VERBOSEclass("no showSys tag set, quitting");
    return false;
  }
  TString histName = tags.getTagStringDefault("input.sys",tags.getTagStringDefault("input.histogram"));
  TQFolder* sysFolder = this->fSystematics->getFolder(sysID);
  if(!sysFolder){
    if(verbose){
      VERBOSEclass("unable to retrieve systematics folder '%s'",sysID.Data());
      this->fSystematics->print();
    }
    return false;
  } else {
    if(verbose) VERBOSEclass("successfully retrieved systematics folder '%s'",sysID.Data());
  }
  TH1* hTotalBkg = this->getObject<TH1>("totalStack");
  if(!hTotalBkg){
    if(verbose) VERBOSEclass("unable to retrieve totalStack histogram!");
    return false;
  }
  TString sourcename = tags.getTagStringDefault("errors.source","totalStack");
  TString targetname = tags.getTagStringDefault("errors.shiftTo",sourcename);  
  bool normSys = tags.getTagBoolDefault("errors.normSys",showSys);
  bool shapeSys = tags.getTagBoolDefault("errors.shapeSys",showSys);
  TString cutname = TQFolder::getPathHead(histName);
  TQFolder* cutfolder= sysFolder->getFolder(cutname);
  if(!cutfolder) cutfolder = sysFolder;
  if (!cutfolder->hasTag("~yield")) {
    ERRORclass("unable to retrieve normalization systematic for '%s/%s'.", cutname.Data(), histName.Data());
    return false;
  }
  double sys = cutfolder->getTagDoubleDefault("~yield",0);
  TH1* hSys = TQHistogramUtils::copyHistogram(hTotalBkg,targetname+"Sys");
  bool done = false;
  if (shapeSys){
    TH1* hSysRel = dynamic_cast<TH1*>(cutfolder->getObject(histName));
    bool histosConsistent = TQHistogramUtils::checkConsistency(hTotalBkg,hSysRel,verbose);
    if(hSysRel && histosConsistent){
      hSys->Multiply(hSysRel);
      bool shapeIncludesNorm = cutfolder->getTagBoolDefault("~shapeIncludesNormalization",true);
      if(shapeIncludesNorm != normSys){
        double factor = normSys ? 1 : -1;
        TQHistogramUtils::addHistogramInQuadrature(hSys,hTotalBkg,factor*sys*sys);
      }
      done = true;
    } else if(hSysRel){
      WARNclass("nominal and systematics histograms are inconsistent, disabling shape systematic!");
    } else {
      WARNclass("systematics histograms not found, disabling shape systematic!");
    }
  }
  if(!done && normSys){
    hSys->Scale(sys);
  }
  if(verbose) VERBOSEclass("successfully created total background systematics histogram '%s' with integral '%f'",hSys->GetName(),TQHistogramUtils::getIntegral(hSys));
  return true;
}

//__________________________________________________________________________________|___________

TObjArray * TQPlotter::getHistograms(TObjArray* processes, const TString& tagFilter, const TString& histName, const TString& namePrefix, TQTaggable& aliases,TQTaggable& options){
  // retrieve histograms using the internal reader
  // histogram options can be controlled by tags

  // stop if the reader is invalid
  if (!fReader) return 0;
  if(processes->GetEntries() == 0){
    ERRORclass("no processes given!");
    return 0;
  }

  bool verbose = options.getTagBoolDefault("verbose",false);

  // create the list of histograms
  TObjArray * histograms = new TObjArray();


  // loop over the list of processes and get histograms
  int i = 0;
  TQTaggableIterator itr(processes);
  while(itr.hasNext()){
    /* get the process properties */
    TQNamedTaggable * process = itr.readNext();
    if(!process) continue;
    if(!process->getTagBoolDefault(tagFilter,false)){
      if(verbose) VERBOSEclass(TString::Format("process '%s' not selected!",process->GetName()));
      continue;
    }
    i++;

    // get the path of the histogram
    TString path = process->getTagStringDefault(".path", "");
    path = aliases.replaceInText(path);

    // check for a predefined histogram name in the process info
    TString histogramName = process->replaceInText(process->getTagStringDefault("input",histName));
    histogramName = aliases.replaceInText(process->getTagStringDefault("input",histogramName));

    if (path.IsNull() || histogramName.IsNull()){
      if(verbose) VERBOSEclass("skipping histogram '%s' from '%s'",histogramName.Data(),path.Data());
      continue;
    }

    TQTaggable histoOptions;
    if (!namePrefix.IsNull()) {
      histoOptions.setTagString("prefix.name", namePrefix);
    }
    //the order of the following two lines is important, since TQTaggable::exportTags does not overwrite existing tags. However it is desirable to prioritize process specific tags over global ones.
    // import process specific options
    process->exportTags(&histoOptions);
    // import global options
    options.exportTags(&histoOptions);


    // get the histogram
    if(verbose) VERBOSEclass("retrieving histogram '%s' from '%s' with options '%s''",histogramName.Data(),path.Data(),histoOptions.exportTagsAsString().Data());
    TH1 * histo = fReader->getHistogram(path, histogramName, &histoOptions);
    if(histo){
      this->addObject(histo,this->makeHistogramIdentifier(process));
      TString histTitle = "";
      if(process->getTagString(".title",histTitle) || process->getTagString("title",histTitle)) histo->SetTitle(histTitle);
      TQHistogramUtils::applyStyle(histo,process);
      
      if (process->getTagBoolDefault("resetSumw2",false)) {
        histo->Sumw2(false);
      }
      
      if(TQUtils::isNum(TQHistogramUtils::getIntegral(histo))){
        histograms->Add(histo);
      } else {
        if(verbose) VERBOSEclass("histogram '%s' from '%s' is empty!'",histogramName.Data(),path.Data());
        delete histo;
      }
    } else if(verbose){
      VERBOSEclass("failed to retrieve histogram, skipping");
    }
  }

  /* return the list of histograms */
  return histograms;
}


//__________________________________________________________________________________|___________

bool TQPlotter::collectOptScanSimplifiedSystHistograms(std::vector<TH1*>& histos, TQTaggable& tags) {
  // collect one or multiple histograms based on a simplified systematics estimation
  // for use in optimizations (intended for optScan subplots). 
  // Note: caller has to take ownership of histograms being placed in the 
  // referenced vector<TH1*> !
  
  std::shared_ptr<TQTaggable> masterCfg = TQTaggable::getGlobalTaggable("master");
  //@tag: [significance.bkgErrorFromPath] This global("master") tag allows to specify a list of paths (including $(channel) and $(campaign) placeholders) to be used to determine a (systematic) uncertainty of the background estimate to be used with optScan subplots for FOMs which account for background uncertainties (for Poisson significance with background systematics see https://cds.cern.ch/record/2643488/files/ATL-COM-GEN-2018-026.pdf). The usual path arithmetics can be used to, e.g., define the error to be a fraction of some process' yield: "0.3*bkg/$(channel)/$(campaign)/fakes" . If multiple, comma seperated paths are given they are summed in quadrature to determine the full systematic uncertainty.
  std::vector<TString> bkgSystSources = masterCfg->getTagVString("significance.bkgErrorFromPath");
  bool allOK = true;
  if (bkgSystSources.size()>0) {
    TQTaggable inputTags;
    inputTags.importTagsWithoutPrefix(tags,"input.");
    TString histoName; 
    if (!inputTags.getTagString("name",histoName)) {
      ERRORclass("Failed to recover cut/histogram name, cannot determine systematic uncertainty for optScan!");
      //clean vector so caller can simply move on/return with error when something goes wrong
      for (TH1* h : histos) {
        delete h;
      }
      histos.clear();
      return false;
    }
    for (TString& systSource: bkgSystSources) { //on purpuse *not* const -> replace tags in place (channel, campaigns,...)
      systSource = inputTags.replaceInText(systSource);
      //we don't care about additional info here, so be a little faster and use getCounter directly...
      TH1* hist = fReader->getHistogram(systSource, histoName, &tags);
      if (!hist) {
        WARNclass( "Failed to obtain systematic uncertainty source histogram with path specification '%s' for distribution '%s'. Skipping this contribution!", systSource.Data(), histoName.Data() );
        allOK=false;
        continue;
      }
      histos.push_back(hist);
    }
  } 
  return allOK; 
}

//__________________________________________________________________________________|___________

bool TQPlotter::checkConsistency(TH1 * &hMaster, TObjArray * histograms) {
  // check the consistency of an array of histograms with the master histogram
  // will create the master histogram if not present

  TQIterator itr(histograms);
  while(itr.hasNext()){
    // iterate over list and check consistency of histograms
    TH1 * h = dynamic_cast<TH1*>(itr.readNext());
    if (!h) continue;
    if (hMaster) {
      if (!TQHistogramUtils::checkConsistency(hMaster, h) || TMath::IsNaN(TQHistogramUtils::getIntegral(h)))
        return false;
    } else {
      hMaster = TQHistogramUtils::copyHistogram(h,"Graph_master");
      hMaster->SetTitle("Main Coordinate System");
    }
  }

  /* return if histograms are consistent */
  return true;
}

//__________________________________________________________________________________|___________

double TQPlotter::getHistogramUpperLimit(TQTaggable& tags, TList * histograms, double lower, bool includeErrors){
  // calculate the "blocks" (x-axis ranges and corresponding y-values)
  // these are employed to avoid collisions of the bins with labels and other graphic elements (i.e. the legend)

  if (!histograms)
    return 0;

  bool logScale = tags.getTagBoolDefault ("style.logScale",false );

  double left = 0;
  double maxUpperLimit = 0.;

  int iBlock = 0;
  double block_x = 0;
  double block_y = 100;

  TH1* exampleHist = dynamic_cast<TH1*>(histograms->At(0));
  double xmin = TQHistogramUtils::getAxisXmin(exampleHist);
  double xmax = TQHistogramUtils::getAxisXmax(exampleHist);

  if(!(TQUtils::isNum(xmin) && TQUtils::isNum(xmax))) return std::numeric_limits<double>::quiet_NaN();
#ifdef _DEBUG_
  histograms->Print();
#endif

  while(tags.getTagDouble(TString::Format("blocks.x.%d",iBlock),block_x) && tags.getTagDouble(TString::Format("blocks.y.%d",iBlock),block_y)){
    double right = block_x;
    double vetoFrac = block_y;

    double block_min = xmin+left*(xmax-xmin);
    double block_max = xmin+right*(xmax-xmin);

    double max = TQHistogramUtils::getMaximumBinValue(histograms, block_min, block_max, includeErrors);

    double upperLimit = 0.;
    // std::cout << max << ", " << lower << ", " << vetoFrac << std::endl;
    if (logScale) upperLimit =  exp(log(max/lower) / vetoFrac ) * lower;
    else          upperLimit = (max - lower) / vetoFrac + lower;


    if (upperLimit > maxUpperLimit) maxUpperLimit = upperLimit;
    left = right;
    iBlock++;
  }
  if(iBlock == 0){
    maxUpperLimit = TQHistogramUtils::getMaximumBinValue(histograms, xmin, xmax, includeErrors);
  }

  return maxUpperLimit;
}

//______________________________________________________________________________________________

bool TQPlotter::plotAndSaveAs(const TString& histogram, const TString& saveAs, const TString& inputTags) {
  // plot and save the given histogram using the given tags
  // the tags are forwarded to and interpreted by the makeplot function

  TQTaggable tags(inputTags);
  return this->plotAndSaveAs(histogram,saveAs,tags);
}

//______________________________________________________________________________________________

bool TQPlotter::plotAndSaveAs(const TString& histogram, const TString& saveAs, const char* inputTags) {
  // plot and save the given histogram using the given tags
  // the tags are forwarded to and interpreted by the makeplot function

  TQTaggable tags((const TString)(inputTags));
  return this->plotAndSaveAs(histogram,saveAs,tags);
}

//______________________________________________________________________________________________

bool TQPlotter::plotAndSaveAs(const TString& histogram, const TString& saveAs, TQTaggable& inputTags) {
  // plot and save the given histogram using the given tags
  // the tags are forwarded to and interpreted by the makeplot function
  return this->plotAndSaveAs(histogram,saveAs,&inputTags);
}


//______________________________________________________________________________________________

bool TQPlotter::plotAndSaveAs(const TString& histogram, const TString& saveAs, TQTaggable * inputTags) {
  // plot and save the given histogram using the given tags
  // the tags are forwarded to and interpreted by the makeplot function

  this->deleteObjects();
  this->clearObjects();

  TQTaggable tags(inputTags);
  tags.importTags(this);
  tags.setGlobalOverwrite(false);

  bool verbose = tags.getTagBoolDefault("verbose",false);

  //@tags: ensureDirectory: create directories to ensure target path exists
  if(tags.getTagBoolDefault("ensureDirectory",false)){
    if(verbose) VERBOSEclass("ensuring directory");
    TQUtils::ensureDirectoryForFile(saveAs);
  }

  return plotAndSaveAsInternal(histogram,saveAs,tags);
}

//______________________________________________________________________________________________

TQPlotter::~TQPlotter() {
  // Destructor of TQPlotter class:
  // this->clearObjects();
  // if(this->objects) delete this->objects;
}

//__________________________________________________________________________________|___________

void TQPlotter::estimateRangeY(TH1* h, double& min, double &max, double tolerance){
  TGraphErrors * g = new TGraphErrors(h);
  estimateRangeY(g,min,max,tolerance);
  delete g;
}

//__________________________________________________________________________________|___________

void TQPlotter::estimateRangeY(TGraphErrors* g, double& min, double &max, double tolerance){
  // estimate the y-range of a TGraphErrors
  if(tolerance < 0) tolerance = std::numeric_limits<double>::infinity();
  if(g->GetN() < 1){
    // we can't estimate the range of an empty graph
    return;
  }
  if(g->GetN() < 2){
    // if there's only one point, that is the range;
    double x,y;
    g->GetPoint(0,x,y);
    min = y - g->GetErrorY(0);
    max = y + g->GetErrorY(0);
    return;
  }
  double sumwy = 0;
  double sumw = 0;
  for(size_t i=0; i < (size_t)(g->GetN()); i++){
    double x, y;
    if( i != (size_t)(g->GetPoint((int)i, x, y))) continue;
    DEBUGclass("looking at point %d: x=%f, y=%f",(int)i,x,y);
    if(y < min) continue;
    if(y > max) continue;
    double err = g->GetErrorY(i);
    if(TQUtils::isNum(err) && err > 0){
      double w = pow(err,-2);
      sumw += w;
      sumwy += w*y;
    }
  }
  double ym = sumwy/sumw;
  DEBUGclass("found ym=%f (sumwy=%f, sumw=%f)", ym, sumwy, sumw);
  double sumsigma = 0;
  double sumw2 = 0;
  for(size_t i=0; i < (size_t)(g->GetN()); i++){
    double x, y;
    if( i != (size_t)(g->GetPoint((int)i, x, y))) continue;
    if(y < min) continue;
    if(y > max) continue;
    double err = g->GetErrorY(i);
    if(TQUtils::isNum(err) && err > 0){
      double w = pow(err,-2);
      sumsigma += w * pow(y - ym,2);
      sumw2 += w*w;
    }
  }
  double sy2 = sumw / (sumw * sumw - sumw2) * sumsigma;
  double sy = sqrt(sy2);
  DEBUGclass("found sy2=%f, sy=%f",sy2,sy);


  double tmpmin = ym;
  double tmpmax = ym;
  for(size_t i=0; i < (size_t)(g->GetN()); i++){
    double x, y;
    if( i != (size_t)(g->GetPoint((int)i, x, y))) continue;
    if(y > max) continue;
    if(y < min) continue;
    if(y > ym + tolerance * sy) continue;
    if(y < ym - tolerance * sy) continue;
    if(y > tmpmax) tmpmax = y+g->GetErrorY(i);
    if(y < tmpmin) tmpmin = y-g->GetErrorY(i);
  }
  min = tmpmin;
  max = tmpmax;
}


//__________________________________________________________________________________|___________


void TQPlotter::estimateRangeY(TGraphAsymmErrors* g, double& min, double &max, double tolerance){
  // estimate the y-range of a TGraphAsymmErrors
  if(tolerance < 0) tolerance = std::numeric_limits<double>::infinity();
  if(g->GetN() < 1){
    // we can't estimate the range of an empty graph
    return;
  }
  if(g->GetN() < 2){
    // if there's only one point, that is the range;
    double x,y;
    g->GetPoint(0,x,y);
    min = y - g->GetErrorYlow(0);
    max = y + g->GetErrorYhigh(0);
    return;
  }
  double sumwy = 0;
  double sumw = 0;
  for(size_t i=0; i < (size_t)(g->GetN()); i++){
    double x, y;
    if( i != (size_t)(g->GetPoint((int)i, x, y))) continue;
    DEBUGclass("looking at point %d: x=%f, y=%f",(int)i,x,y);
    if(y < min) continue;
    if(y > max) continue;
    double err = sqrt(pow(g->GetErrorYlow(i),2)+pow(g->GetErrorYhigh(i),2));
    if(TQUtils::isNum(err) && err > 0){
      double w = pow(err,-2);
      sumw += w;
      sumwy += w*y;
    }
  }
  double ym = sumwy/sumw;
  DEBUGclass("found ym=%f (sumwy=%f, sumw=%f)", ym, sumwy, sumw);
  double sumsigma = 0;
  double sumw2 = 0;
  for(size_t i=0; i < (size_t)(g->GetN()); i++){
    double x, y;
    if( i != (size_t)(g->GetPoint((int)i, x, y))) continue;
    if(y < min) continue;
    if(y > max) continue;
    double err = sqrt(pow(g->GetErrorYlow(i),2)+pow(g->GetErrorYhigh(i),2));
    if(TQUtils::isNum(err) && err > 0){
      double w = pow(err,-2);
      sumsigma += w * pow(y - ym,2);
      sumw2 += w*w;
    }
  }
  double sy2 = sumw / (sumw * sumw - sumw2) * sumsigma;
  double sy = sqrt(sy2);
  DEBUGclass("found sy2=%f, sy=%f",sy2,sy);


  double tmpmin = ym;
  double tmpmax = ym;
  for(size_t i=0; i < (size_t)(g->GetN()); i++){
    double x, y;
    if( i != (size_t)(g->GetPoint((int)i, x, y))) continue;
    if(y > max) continue;
    if(y < min) continue;
    if(y > ym + tolerance * sy) continue;
    if(y < ym - tolerance * sy) continue;
    if(y > tmpmax) tmpmax = y+g->GetErrorY(i);
    if(y < tmpmin) tmpmin = y-g->GetErrorY(i);
  }
  min = tmpmin;
  max = tmpmax;
}


//__________________________________________________________________________________|___________

void TQPlotter::getRange(TGraphErrors* g, double &xlow, double &xhigh, double &ylow, double &yhigh, bool get_xrange, bool get_yrange, double maxQerr){
  // extract the range from a TGraphErrors
  if(maxQerr < 0) maxQerr = std::numeric_limits<double>::infinity();
  int nx = 0;
  int ny = 0;
  double x;
  double y;
  double sumx = 0;
  double sumx2 = 0;
  double sumy = 0;
  double sumy2 = 0;
  for(size_t i=0; i < (size_t)(g->GetN()); i++){
    if( i == (size_t)(g->GetPoint((int)i, x, y))){
      if((get_xrange || TQUtils::inRange(x, xlow , xhigh)) && (get_yrange || TQUtils::inRange(y, ylow , yhigh))){
        if(get_xrange){
          nx++;
          sumx += x;
          sumx2 += x*x;
        }
        if(get_yrange){
          ny++;
          sumy += y;
          sumy2 += y*y;
        }
      }
    }
  }
  double xmean = sumx/nx;
  double ymean = sumy/ny;
  double xvar = sumx2/nx - pow(sumx/nx,2);
  double yvar = sumy2/ny - pow(sumy/ny,2);
  for(size_t i=0; i < (size_t)(g->GetN()); i++){
    if( i == (size_t)(g->GetPoint((int)i, x, y))){
      if((get_xrange || TQUtils::inRange(x, xlow , xhigh)) && (get_yrange || TQUtils::inRange(y, ylow , yhigh))){
        if(get_xrange){
          if(!TQUtils::isNum(xlow)) xlow = xmean-sqrt(xvar);
          if(!TQUtils::isNum(xhigh)) xhigh = xmean+sqrt(xvar);
          double xm = 0.5*(xhigh + xlow);
          double xd = (xhigh-xlow);
          if(xd < 2*std::numeric_limits<double>::epsilon()) xd = std::numeric_limits<double>::infinity();
          if(TQUtils::inRange(x-g->GetErrorX(i),xm-(xd*maxQerr),xlow)) { xlow = x-g->GetErrorX(i); }
          if(TQUtils::inRange(x+g->GetErrorX(i),yhigh,xm+(xd*maxQerr))){ xhigh = x+g->GetErrorX(i); }
        }
        if(get_yrange){
          if(!TQUtils::isNum(ylow)) ylow = ymean-sqrt(yvar);
          if(!TQUtils::isNum(yhigh)) yhigh = ymean+sqrt(yvar);
          double ym = 0.5*(yhigh + ylow);
          double yd = (yhigh-ylow);
          if(yd < 2*std::numeric_limits<double>::epsilon()) yd = std::numeric_limits<double>::infinity();
          if(TQUtils::inRange(y-g->GetErrorY(i),ym-(yd*maxQerr),ylow)) { ylow = y-g->GetErrorY(i); }
          if(TQUtils::inRange(y+g->GetErrorY(i),yhigh,ym+(yd*maxQerr))){ yhigh = y+g->GetErrorY(i); }
        }
      }
    }
  }
}

//__________________________________________________________________________________|___________

void TQPlotter::setErrors(TQTaggable& tags, const TString& sourcename){
  // the histograms retrieved using the TQSampleDataReader class
  // have the statistical uncertainty filled into bin errors by default
  // this function either sets them to zero (for statMCerrors=false)
  // or adds the systematic uncertainties in quadrature (for sysMCerrors=true)
  bool verbose = tags.getTagBoolDefault("verbose",false);
  TString targetname = tags.getTagStringDefault("errors.shiftTo",sourcename);
  TH1* hSource = this->getObject<TH1>(sourcename);  
  TH1* hTarget = this->getObject<TH1>(targetname);
  bool includeSys = tags.getTagBoolDefault("errors.drawSysMC", tags.hasTag("errors.showSys") || tags.hasTag("errors.totalBkgSys") );
  bool includeStat = tags.getTagBoolDefault("errors.drawStatMC", tags.getTagBoolDefault("errors.showStat", true ) );
  if(!hSource){
    if(verbose) VERBOSEclass("no total histogram named '%s' found",sourcename.Data());
    return;
  }
  if (!includeStat) {
    /* set bin errors to zero */
    if(verbose) VERBOSEclass("removing statistical errors from total background histogram");
    TQHistogramUtils::resetBinErrors(hSource);
  }
  
  if (includeSys){
    TH1* hSys = this->getObject<TH1>(targetname+"Sys");
    if (includeSys && !hSys && verbose) {
      VERBOSEclass("no total background systematics found");
    }
    double sysLin = 0.0;
    double sysSq = 0.0;
    /* include systematics */
    if(verbose) VERBOSEclass("adding systematic errors to total background histogram");
    for (int iBin = 1; iBin <= hSource->GetNbinsX(); iBin++) {
      double stat = hSource->GetBinError(iBin);
      if(hSys) {
        sysLin = hSource->GetBinContent(iBin);
        sysSq = hSource->GetBinError(iBin);
      }
      double total = TMath::Sqrt(stat*stat + sysLin*sysLin + sysSq*sysSq);
      hTarget->SetBinError(iBin, total);
    }
  }
}

//__________________________________________________________________________________|___________

void TQPlotter::addObject(TNamed* obj, const TString& key){
  // add an object to the list of graphics objects maintained by the plotter
  if(!obj) return;
  if(!key.IsNull()) obj->SetName(key);
  if(this->objects->FindObject(obj->GetName())){
    ERRORclass("cannot add object '%s' - an object of this name already exists!",obj->GetName());
  }
  this->objects->Add(obj);
}

//__________________________________________________________________________________|___________

void TQPlotter::addObject(TGraph* obj, TString key){
  // add an object to the list of graphics objects maintained by the plotter
  if(!obj) return;
  if(key.IsNull()) key = obj->GetName();
  obj->SetName("tmpgraph");
  obj->GetHistogram()->SetName(TString::Format("h_%s",key.Data()));
  obj->SetName(key.Data());
  if(this->objects->FindObject(obj->GetName())){
    ERRORclass("cannot add object '%s' - an object of this name already exists!",obj->GetName());
  }
  this->objects->Add(obj);
  obj->GetHistogram()->SetDirectory(NULL);
  DEBUGclass("%s@%#x <=> %s@%#x",obj->GetName(),this->objects,obj->GetHistogram()->GetName(),obj->GetHistogram()->GetDirectory());
}

//__________________________________________________________________________________|___________

void TQPlotter::addObject(TCollection* obj, const TString& key){
  // add a collection to the list of graphics objects maintained by the plotter
  if(!obj) return;
  if(!key.IsNull()) obj->SetName(key);
  this->objects->Add(obj);
}

//__________________________________________________________________________________|___________

void TQPlotter::addObject(TH1* obj, const TString& key){
  // add a histogram to the list of graphics objects maintained by the plotter
  if(!obj) return;
  if(!key.IsNull()) obj->SetName(key);
  if(this->objects->FindObject(obj->GetName())){
    ERRORclass("cannot add histogram '%s' - an object of this name already exists!",obj->GetName());
  }
  obj->SetDirectory(this->objects);
}

//__________________________________________________________________________________|___________

void TQPlotter::removeObject(const TString& key, bool deleteObject){
  // remove an object from the list of graphics object maintained by the plotter
  TObject* obj = this->objects->FindObject(key);
  if(!obj) return;
  this->objects->Remove(obj);
  if(deleteObject) delete obj;
}

//__________________________________________________________________________________|___________

void TQPlotter::clearObjects(){
  // clear all objects maintained by the plotter
  this->objects->Clear();
}

//__________________________________________________________________________________|___________

void TQPlotter::deleteObjects(){
  // clear all objects maintained by the plotter
  this->objects->DeleteAll();
  this->objects->Clear();
}

//__________________________________________________________________________________|___________

void TQPlotter::printObjects(){
  // print all objects maintained by the plotter
  TQIterator itr(this->objects->GetList());
  while(itr.hasNext()){
    TObject* obj = itr.readNext();
    if(!obj) continue;
    std::cout << TQStringUtils::makeBoldBlue(TQStringUtils::fixedWidth(obj->ClassName(),15));
    std::cout << " ";
    std::cout << TQStringUtils::makeBoldWhite(TQStringUtils::fixedWidth(obj->GetName(),50,"l"));
    std::cout << " ";
    std::cout << TQStringUtils::makeBoldWhite(TQStringUtils::fixedWidth(obj->GetTitle(),50,"l"));
    std::cout << " ";
    TGraph* g = dynamic_cast<TGraph*>(obj);
    if(g){
      std::cout << TQHistogramUtils::getDetailsAsString(g);
      if(TQStringUtils::matches(g->GetName(),"contour_*")) std::cout << ", contour area=" << fabs(TQHistogramUtils::getContourArea(g));
    } else if (obj->InheritsFrom(TH1::Class())) {
      std::cout << TQHistogramUtils::getDetailsAsString((TH1*)obj,4);
    } else {
      std::cout << TQStringUtils::getDetails(obj);
    }
    std::cout << std::endl;
  }
}

//__________________________________________________________________________________|___________

TObject* TQPlotter::getTObject(const TString& key){
  // retrieve a graphics object by name
  TQIterator itr(this->objects->GetList());
  while(itr.hasNext()){
    TObject* obj = itr.readNext();
    if(!obj) continue;
    if(TQStringUtils::matches(obj->GetName(),key)){
      return obj;
    }
  }
  return NULL;
}


//__________________________________________________________________________________|___________

void TQPlotter::applyStyle(TQTaggable& tags, TAxis* a, const TString& key, double/*distscaling*/, double/*sizescaling*/){
  // apply a tag-defined style to an axis
  // tags are read by the given key
  // sizes and distances are scaled according to the given parameters
  //@tags: style.*.showTitle: control whether axis title will be shown
  //@tags: style.*.showLabels: control whether axis labels will be shown
  //@tags: style.*.showTicks: control whether axis ticks
  //@tags: style.*.nDiv: number of divisions (encoded in 5 digit number, default 00510) for this axis
  if(!a) return;
  if(!tags.getTagBoolDefault("style."+key+".showTitle",true)){
    a->SetTitleOffset(0.);
    a->SetTitleSize (0.);
  }
  if(!tags.getTagBoolDefault("style."+key+".showLabels",true)){
    a->SetLabelOffset(0.);
    a->SetLabelSize (0.);
  }
  if(!tags.getTagBoolDefault("style."+key+".showTicks",true)){
    a->SetTickLength (0.);
  }
  if(!tags.getTagBoolDefault("style."+key+".allowExponent",true)){
    a->SetNoExponent(true);
  }
  int ndiv = 510;;
  if(tags.getTagInteger("style."+key+".nDiv",ndiv)){
    a->SetNdivisions(ndiv);
  }
}

//__________________________________________________________________________________|___________

void TQPlotter::applyGeometry(TQTaggable& tags, TAxis* a, const TString& key, double distscaling, double sizescaling){
  // apply a tag-defined geometry to an axis
  // tags are read by the given key
  // sizes and distances are scaled according to the given parameters
  //@tags: style.textSize: text size (default 0.05)
  //@tags: geometry.*.titleSize: title size (default:textSize)
  //@tags: geometry.*.labelSize: label size (default:textSize)
  //@tags: geometry.*.titleOffset: title offset
  //@tags: geometry.*.labelOffset: label offset
  //@tags: geometry.*.tickLength: label offset
  if(!a) return;
  double textSize = tags.getTagDoubleDefault("style.textSize",DEFAULTTEXTSIZE);
  double titleSize = textSize;
  if(TMath::AreEqualRel(a->GetTitleSize(),0.035,0.01) || tags.getTagDouble ("geometry."+key+".titleSize",textSize)){
    a->SetTitleSize (sizescaling*titleSize);
  }
  double titleoffset=1;
  if(TMath::AreEqualAbs(a->GetTitleOffset(),1.,0.01) || TMath::AreEqualAbs(a->GetTitleOffset(),0,0.01) || tags.getTagDouble ("geometry."+key+".titleOffset", titleoffset)){
    a->SetTitleOffset(distscaling*tags.getTagDoubleDefault ("geometry."+key+".titleOffset", 1.) / titleSize*0.07);
  }
  double labelSize = textSize;
  if(TMath::AreEqualRel(a->GetLabelSize(),0.035,0.01) || tags.getTagDouble ("geometry."+key+".labelSize",labelSize)){
    a->SetLabelSize (sizescaling*labelSize);
  }
  double labelOffset = 0.005;
  if(TMath::AreEqualRel(a->GetLabelOffset(),labelOffset,0.01) || tags.getTagDoubleDefault ("geometry."+key+".labelOffset",labelOffset)){
    a->SetLabelOffset(distscaling*labelOffset/labelSize*0.05);
  }
  double ticklength = tags.getTagDoubleDefault("style.tickLength",0.03);
  if(TMath::AreEqualRel(a->GetTickLength(),0.03,0.01) || tags.getTagDouble ("geometry."+key+".tickLength",ticklength)){
    a->SetTickLength (distscaling*sizescaling*ticklength);
  }
}

//__________________________________________________________________________________|___________

void TQPlotter::applyGeometry(TQTaggable& tags, TH1* hist, const TString& key, double xscaling, double yscaling){
  // apply a tag-defined geometry to a histogram
  // tags are read by the given key
  // sizes and distances are scaled according to the given parameters
  if(!hist) return;
  TAxis* xAxis = hist->GetXaxis();
  // *jedi move* you do not want to know of the handling of xscaling vs. yscaling
  applyGeometry(tags,xAxis,key+".xAxis",1.,std::max(yscaling/xscaling,xscaling/yscaling));
  TAxis* yAxis = hist->GetYaxis();
  // *jedi move* you do not want to know of the handling of xscaling vs. yscaling
  applyGeometry(tags,yAxis,key+".yAxis",xscaling/yscaling,std::max(yscaling/xscaling,xscaling/yscaling));
}

//__________________________________________________________________________________|___________

void TQPlotter::applyGeometry(TQTaggable& tags, TGraph* g, const TString& key, double xscaling, double yscaling){
  // apply a tag-defined geometry to a graph
  // tags are read by the given key
  // sizes and distances are scaled according to the given parameters
  if(!g) return;
  TAxis* xAxis = g->GetXaxis();
  // *jedi move* you do not want to know of the handling of xscaling vs. yscaling
  applyGeometry(tags,xAxis,key+".xAxis",1.,std::max(yscaling/xscaling,xscaling/yscaling));
  TAxis* yAxis = g->GetYaxis();
  // *jedi move* you do not want to know of the handling of xscaling vs. yscaling
  applyGeometry(tags,yAxis,key+".yAxis",xscaling/yscaling,std::max(yscaling/xscaling,xscaling/yscaling));
}

//__________________________________________________________________________________|___________

void TQPlotter::applyStyle(TQTaggable& tags, TH1* hist, const TString& key, double xscaling, double yscaling){
  // apply a tag-defined style to a histogram
  // tags are read by the given key
  // sizes and distances are scaled according to the given parameters
  //@tags: style.*.fillColor: set fill color using TH1::SetFillColor
  //@tags: style.*.fillStyle: set fill style using TH1::SetFillStyle
  //@tags: style.*.lineColor: set line color using TH1::SetLineColor
  //@tags: style.*.lineStyle: set line style using TH1::SetLineStyyle
  //@tags: style.*.markerColor: set marker color using TH1::SetMarkerColor
  //@tags: style.*.markerSize: set marker size using TH1::SetMarkerSize
  //@tags: style.*.markerStyle: set marker size using TH1::SetMarkerStyle
  if(!hist) return;
  TAxis* xAxis = hist->GetXaxis();
  // *jedi move* you do not want to know of the handling of xscaling vs. yscaling
  applyStyle(tags,xAxis,key+".xAxis",yscaling/xscaling,xscaling/yscaling);
  TAxis* yAxis = hist->GetYaxis();
  // *jedi move* you do not want to know of the handling of xscaling vs. yscaling
  applyStyle(tags,yAxis,key+".yAxis",xscaling/yscaling,yscaling/xscaling);
  int fillColor = hist->GetFillColor (); tags.getTagInteger("style."+key+".fillColor", fillColor); hist->SetFillColor (fillColor);
  int fillStyle = hist->GetFillStyle (); tags.getTagInteger("style."+key+".fillStyle", fillStyle); hist->SetFillStyle (fillStyle);
  int lineColor = hist->GetLineColor (); tags.getTagInteger("style."+key+".lineColor", lineColor); hist->SetLineColor (lineColor);
  double lineWidth = hist->GetLineWidth (); tags.getTagDouble ("style."+key+".lineWidth", lineWidth); hist->SetLineWidth (lineWidth);
  int lineStyle = hist->GetLineStyle (); tags.getTagInteger("style."+key+".lineStyle", lineStyle); hist->SetLineStyle (lineStyle);
  int markerColor = hist->GetMarkerColor(); tags.getTagInteger("style.markerColor",markerColor); tags.getTagInteger("style."+key+".markerColor", markerColor); hist->SetMarkerColor(markerColor);
  double markerSize = hist->GetMarkerSize (); tags.getTagDouble ("style.markerSize", markerSize ); tags.getTagDouble ("style."+key+".markerSize" , markerSize ); hist->SetMarkerSize (markerSize );
  int markerStyle = hist->GetMarkerStyle(); tags.getTagInteger("style.markerStyle",markerStyle); tags.getTagInteger("style."+key+".markerStyle", markerStyle); hist->SetMarkerStyle(markerStyle);
  if(tags.getTagBoolDefault ("style.binticks",false )){
    hist->GetXaxis()->SetNdivisions(hist->GetNbinsX(),0,0);
  }
}

//__________________________________________________________________________________|___________

void TQPlotter::applyStyle(TQTaggable& tags, TGraph* g, const TString& key, double xscaling, double yscaling){
  // apply a tag-defined style to a graph
  // tags are read by the given key
  // sizes and distances are scaled according to the given parameters
  if(!g) return;
  TAxis* xAxis = g->GetXaxis();
  applyStyle(tags,xAxis,key+".xAxis",yscaling/xscaling,xscaling/yscaling);
  TAxis* yAxis = g->GetYaxis();
  applyStyle(tags,yAxis,key+".yAxis",xscaling/yscaling,yscaling/xscaling);
  int fillColor = 0; if(tags.getTagInteger("style."+key+".fillColor", fillColor) || tags.getTagInteger("style.fillColor", fillColor)) g->SetFillColor(fillColor);
  int fillStyle = 0; if(tags.getTagInteger("style."+key+".fillStyle", fillStyle)  || tags.getTagInteger("style.fillStyle", fillStyle)) g->SetFillStyle(fillStyle);
  int lineColor = 0; if(tags.getTagInteger("style."+key+".lineColor", lineColor) || tags.getTagInteger("style.lineColor", lineColor)) g->SetLineColor(lineColor);
  int lineStyle = 0; if(tags.getTagInteger("style."+key+".lineStyle", lineStyle) || tags.getTagInteger("style.lineStyle", lineStyle)) g->SetLineStyle(lineStyle);
  double lineWidth = 0; if(tags.getTagDouble("style."+key+".lineWidth", lineWidth) || tags.getTagDouble("style.lineWidth", lineWidth)) g->SetLineWidth(lineWidth);
  int markerColor = 0; if(tags.getTagInteger("style."+key+".markerColor", markerColor) || tags.getTagInteger("style.markerColor", markerColor)) g->SetMarkerColor(markerColor);
  double markerSize = 0; if(tags.getTagDouble("style."+key+".markerSize", markerSize) || tags.getTagDouble("style.markerSize", markerSize)) g->SetMarkerSize(markerSize);
  int markerStyle = 0; if(tags.getTagInteger("style."+key+".markerStyle", markerStyle) || tags.getTagInteger("style.markerStyle", markerStyle)) g->SetMarkerStyle(markerStyle);
  if(tags.getTagBoolDefault ("style.binticks",false )){
    g->GetXaxis()->SetNdivisions(g->GetHistogram()->GetNbinsX(),0,0);
  }
}

//__________________________________________________________________________________|___________

void TQPlotter::setAxisLabels(TQTaggable& tags){
  // set the labels of the master histogram according to the avialable tags

  TH1* hMaster = this->getObject<TH1>("Graph_master");
  // @tags: labels.axes.mainX: control the x axis labels of the main frame
  TString xLabel = tags.getTagStringDefault("labels.axes.mainX", hMaster->GetXaxis()->GetTitle());
  hMaster->GetXaxis()->SetTitle(xLabel);

  if(TQHistogramUtils::getDimension(hMaster) == 1){
    TString xUnit = TQStringUtils::getUnit(TString(hMaster->GetXaxis()->GetTitle()));
    double binWidth = (hMaster->GetBinLowEdge(hMaster->GetNbinsX() + 1) -
                       hMaster->GetBinLowEdge(1)) / hMaster->GetNbinsX();

    // We're using variable width binning (should still work with fixed width)
    int densityBin = 0;
    if (tags.getTagInteger("scaleDensityToBin",densityBin)){
      binWidth = hMaster->GetXaxis()->GetBinWidth(densityBin);
    }

    bool isInteger = ((binWidth - (int)binWidth) < 0.0001);


    bool normalize = tags.getTagBoolDefault("normalize",false );

    TString yLabel = "Events";

    if(normalize){
      // "Events" or "arbitrary units"?
      yLabel = "arbitrary units";
    } else if (TQHistogramUtils::hasUniformBinning(hMaster) && !(xUnit.IsNull() && TMath::AreEqualRel(binWidth, 1., 1E-6))) {
      // there is only one case in which we don't have to add anything to the "Events" label: no unit on the x axis and a bin width of exactly 1
      // also, if the binning is irregular, we skip the bin width addition

      if (isInteger)
        yLabel.Append(TString::Format(" / %.0f", binWidth));
      else
        yLabel.Append(TString::Format(" / %.2g", binWidth));
      // append the unit of the x axis
      if (xUnit.Length() > 0)
        yLabel.Append(TString::Format(" %s", xUnit.Data()));
    }
    // @tags: labels.axes.mainY: control the y axis labels of the main frame (usually 'Events' or 'arbitrary units')
    tags.getTagString("labels.axes.mainY", yLabel);

    hMaster->GetYaxis()->SetTitle(yLabel.Data());
  } else {
    // for a >1D case, we just set whatever the user requested
    TString yLabel;
    if(tags.getTagString("labels.axes.mainY",yLabel)) hMaster->GetYaxis()->SetTitle(yLabel);
  }
}

//__________________________________________________________________________________|___________

TString TQPlotter::createAxisTagsAsString(const TString& prefix, const TString& title, double xCoeff, double yCoeff, double constCoeff, double wMin, double wMax, double xCoord, double yCoord, int /*nDiv*/){
  // convert a variable definition into tags for an additional axis
  TQTaggable tags;
  if(TQPlotter::createAxisTags(tags,prefix,title,xCoeff, yCoeff, constCoeff, wMin, wMax, xCoord, yCoord)){
    return tags.exportTagsAsString();
  }
  return "";
}

//__________________________________________________________________________________|___________

TString TQPlotter::createAxisTagsAsConfigString(const TString& prefix, const TString& title, double xCoeff, double yCoeff, double constCoeff, double wMin, double wMax, double xCoord, double yCoord, int /*nDiv*/){
  // convert a variable definition into tags for an additional axis
  TQTaggable tags;
  if(TQPlotter::createAxisTags(tags,prefix,title,xCoeff, yCoeff, constCoeff, wMin, wMax, xCoord, yCoord)){
    return tags.exportTagsAsConfigString("");
  }
  return "";
}

//__________________________________________________________________________________|___________

TQTaggable* TQPlotter::createAxisTags(const TString& prefix, const TString& title, double xCoeff, double yCoeff, double constCoeff, double wMin, double wMax, double xCoord, double yCoord, int /*nDiv*/){
  // convert a variable definition into tags for an additional axis
  TQTaggable* tags = new TQTaggable();;
  TQPlotter::createAxisTags(*tags,prefix,title,xCoeff, yCoeff, constCoeff, wMin, wMax, xCoord, yCoord);
  return tags;
}

//__________________________________________________________________________________|___________

bool TQPlotter::createAxisTags(TQTaggable& tags, const TString& prefix, const TString& title, double xCoeff, double yCoeff, double constCoeff, double wMin, double wMax, double xCoord, double yCoord, int nDiv){
  // convert a variable definition into tags for an additional axis
  double wCoord = xCoeff*xCoord + yCoeff*yCoord + constCoeff;
  double coeff2 = xCoeff* xCoeff + yCoeff * yCoeff;

  double tmin = (wMin - wCoord)/coeff2;
  double tmax = (wMax - wCoord)/coeff2;

  double xmin = xCoord + xCoeff * tmin;
  double xmax = xCoord + xCoeff * tmax;
  double ymin = yCoord + yCoeff * tmin;
  double ymax = yCoord + yCoeff * tmax;

  tags.setTagBool(prefix+"show",true);
  tags.setTagDouble(prefix+"xMin",xmin);
  tags.setTagDouble(prefix+"xMax",xmax);
  tags.setTagDouble(prefix+"yMin",ymin);
  tags.setTagDouble(prefix+"yMax",ymax);
  tags.setTagDouble(prefix+"wMin",wMin);
  tags.setTagDouble(prefix+"wMax",wMax);
  tags.setTagInteger(prefix+"nDiv",nDiv);
  tags.setTagString(prefix+"title",title);

  return true;
}

//__________________________________________________________________________________|___________

int TQPlotter::getNProcesses(const TString& tagFilter){
  // return the number of added processes matching the tag filter
  TQTaggableIterator itr(this->fProcesses);
  int retval = 0;
  while(itr.hasNext()){
    TQNamedTaggable* tags = itr.readNext();
    if(!tags) continue;
    if(tags->getTagBoolDefault(tagFilter,false)) retval++;
  }
  return retval;
}

//__________________________________________________________________________________|___________

int TQPlotter::sanitizeProcesses() {
  // sanitize all processes
  TQTaggableIterator itr(fProcesses);
  std::vector<TQNamedTaggable*> removals;
  int retval = 0;
  while(itr.hasNext()){
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(process->getTagStringDefault(".path","").Contains("|")){
      removals.push_back(process);
    }
  }
  for(size_t i=0; i<removals.size(); i++){
    fProcesses->Remove(removals[i]);
    delete removals[i];
    retval++;
  }
  return retval;
}

//__________________________________________________________________________________|___________

void TQPlotter::applyBlinding(TQTaggable& tags, TCollection* histosSig, TCollection* histosBkg, TCollection* histosData) {
  //determine total background and total signal to apply dynamic blinding criteria
  // note that this function modifies the data histograms in-place, i.e., you must pass in a TCollection of the data histograms to be (potentially) blinded. No internal lists of histos are updated/changed in this function!
  if (tags.hasMatchingTag("blind*")) {
    TH1 *totalBkg = TQHistogramUtils::sumHistograms(histosBkg);
    TH1 *totalSig = TQHistogramUtils::sumHistograms(histosSig);
    bool fullBlind = false;
    if (!totalBkg || !totalSig) {
      WARNclass("Could not determine total signal and/or total background. Will fully blind data in this plot to be safe!");
      fullBlind = true;
    }
    if (!TQHistogramUtils::checkConsistency(totalBkg, totalSig)) {
      WARNclass("Total signal and total background histograms are inconsistent. Will fully blind data in this plot to be safe! (likely the plot will fail completely anyways)");
      fullBlind = true;
    }
    if (histosData) { //if there are not data histos just skip this part...
      double sOverBthreshold = 0.05;
      double sOverSqrtSpBthreshold = 0.125; //some more or less sensible default values
      //@tag: [blinding.SoverB, blinding.SoverSqrtSpB] if at least one of these tags is set data histograms are blinded in bins where the expected signal to background ratio or significance (s/sqrt(s+b)) exceed the configured values. For safety both criteria are applied with thresholds of 0.1 and 0.5 respectively if an option matching "blind*" is found (to avoid accidential unblinding by mistyping). If a bin is blinded the bin contents and error are set to zero. 
      bool useSoB = tags.getTagDouble("blinding.SoverB", sOverBthreshold);
      bool useSoSqrtSpB = tags.getTagDouble("blinding.SoverSqrtSpB", sOverSqrtSpBthreshold);
      if (! (useSoB || useSoSqrtSpB) ) { //if none of the criteria is selected there may be a user error (e.g. mis-spelled tag) so enable both with the defaults specified above
        WARNclass("It seems you requested data to be dynamically blinded but no thresholds for any supported criterion were found. Will use defaults for s/b (0.1) and s/sqrt(s+b) (0.5). The supported tag names are \"(plotter.)blinding.SoverB\" and \"(plotter.)blinding.SoverSqrtSpB\".");
        useSoB = true;
        useSoSqrtSpB = true;
      }
      TQTH1Iterator dataItr(histosData);
      while (dataItr.hasNext()) {
        std::cout<<"Iterating over data histograms"<<std::endl;
        TH1* dataHist = dataItr.readNext();
        if (!dataHist) continue;
        if (fullBlind || !TQHistogramUtils::checkConsistency(totalBkg,dataHist)) { //if something is odd, blind the entire data histogram
          for (int bin=0; bin<TQHistogramUtils::getNBins(dataHist); ++bin) {
            dataHist->SetBinContent(bin, 0.); dataHist->SetBinError(bin, 0.);
          }
        }
        double s=0,b=0;
        for (int bin=0; bin<TQHistogramUtils::getNBins(dataHist); ++bin) { //we can iterate over any histogram's nBins since they are verified to be consistent!
          s = totalSig->GetBinContent(bin);
          b = totalBkg->GetBinContent(bin);
          if (b==0 ||
              ( useSoB && s/b>sOverBthreshold ) ||
              ( useSoSqrtSpB && s*s/(s+b) > sOverSqrtSpBthreshold*sOverSqrtSpBthreshold )) {
            dataHist->SetBinContent(bin, 0.); dataHist->SetBinError(bin, 0.);
          }
        }
      }
    }
  }
  
}

//__________________________________________________________________________________|___________

TObjArray* TQPlotter::collectHistograms(TQTaggable& tags){
  // use the TQSampleDataReader to retrieve all histograms from the sample folder
  bool verbose = tags.getTagBoolDefault("verbose",false );

  //@tag: [style.showUnderflow,style.showOverflow] This argument tag controls if under/overflow bins are shown in the histogram. Default: false.
  bool showUnderflow = tags.getTagBoolDefault ("style.showUnderflow",false);
  bool showOverflow = tags.getTagBoolDefault ("style.showOverflow",false );
  tags.setTagBool("includeOverflow",showOverflow);
  tags.setTagBool("includeUnderflow",showUnderflow);

  //import alias and input tags
  TQTaggable aliases;
  aliases.importTagsWithoutPrefix(tags,"alias.");
  aliases.importTagsWithoutPrefix(tags,"input.");

  if(verbose) VERBOSEclass("getting data histograms");
  TObjArray* histosData = getHistograms(this->fProcesses,".isData", tags.getTagStringDefault("input.data", tags.getTagStringDefault("input.histogram","")), "", aliases, tags);
  if(verbose) VERBOSEclass("getting background histograms");
  TObjArray* histosBkg = getHistograms(this->fProcesses,".isBackground", tags.getTagStringDefault("input.bkg", tags.getTagStringDefault("input.histogram","")), "", aliases, tags);
  if(verbose) VERBOSEclass("getting signal histograms");
  TObjArray* histosSig = getHistograms(this->fProcesses,".isSignal", tags.getTagStringDefault("input.sig", tags.getTagStringDefault("input.histogram","")), "", aliases, tags);

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
  }
  if(!histosBkg || histosBkg->GetEntries() < 1){
    if(verbose) VERBOSEclass("no background histograms found, disabling background");
    tags.setTagBool("style.drawBkg",false);
  }
  if(!histosSig || histosSig->GetEntries() < 1){
    if(verbose) VERBOSEclass("no signal histograms found, disabling signal");
    tags.setTagBool("style.drawSig",false);
  }
  
  this->applyBlinding(tags,histosSig,histosBkg,histosData);
  
  // the histograms in these TObjArrays were already collected in "histos". 
  // The deletion of the individual lists is postponed up to this point to allow
  // for the checks and blinding logic in between.
  if(histosData) delete histosData;
  if(histosBkg) delete histosBkg;
  if(histosSig) delete histosSig;
  
  
  TQTH1Iterator itr(histos);
  //@tag: [requireMinimumContent] Skip histograms with integral below given value. This is set to 1e-5 by default!
  double minContent = tags.getTagDoubleDefault("requireMinimumContent",1e-5);
  double maxint = 0;
  while(itr.hasNext()){
    TH1* hist = itr.readNext();
    double integral = hist->Integral();
    maxint = std::max(integral,maxint);
  }
  if(verbose) VERBOSEclass("highest integral of histogram set is %g",maxint);
  if( maxint < minContent){
    WARNclass("skipping plot, histograms do not exceed minimum integral requirement (requireMinimumBinContent=%g)",minContent);
    delete histos;
    return NULL;
  }

  // check the consistency and create master histogram
  if(verbose) VERBOSEclass("checking histogram consistency");
  // expect at least one histogram to be available
  bool consistent = (histos->GetEntries() > 0);
  TH1* hMaster = NULL;
  consistent = checkConsistency(hMaster, histos) && consistent;
  if(!hMaster){
    if(verbose) VERBOSEclass("unable to obtain master histogram from consistency check!");
  }

  // stop if there is no valid histogram or histograms are invalid
  if (!consistent){
    if(verbose) VERBOSEclass("consistency check failed");
    delete histos;
    return NULL;
  }
  hMaster->Reset();
  this->addObject(histos,"histos");

  return histos;
}

//__________________________________________________________________________________|___________

void TQPlotter::drawLabels(TQTaggable& /*tags*/){
  // draw the labels given by the tags
  // does nothing by default, can be overridden by derived classes
}

//__________________________________________________________________________________|___________

TGraphAsymmErrors* TQPlotter::getRatioErrorGraph(TH1* hTotalStack){
  // get a TGraphAsymmErrors with the relative errors of a histogram
  int nBins = hTotalStack->GetNbinsX();

  int nPoints = 0;
  for (int i = 1; i <= nBins; i++) {
    if (hTotalStack->GetBinContent(i) != 0.) {
      nPoints++;
    }
  }

  TGraphAsymmErrors * ratioErrorGraph = new TGraphAsymmErrors(nPoints);
  ratioErrorGraph->SetName(TString::Format("ratioError%s",hTotalStack->GetName()));
  ratioErrorGraph->SetTitle("Monte Carlo ratio error band");

  int iPoint = 0;
  for (int iBin = 1; iBin <= nBins; iBin++) {
    double MC = hTotalStack->GetBinContent(iBin);
    double MCErr = hTotalStack->GetBinError(iBin);
    double MCErrUpper = MCErr;
    double MCErrLower = MCErr;
    if(MCErrUpper == 0 || MCErrLower == 0 || MC == 0) continue;
    double ratioBandErrorUpper =  MCErrUpper / MC;
    double ratioBandErrorLower =  MCErrLower / MC;
    // set the position and the width of the ratio error band
    ratioErrorGraph->SetPoint(iPoint, hTotalStack->GetBinCenter(iBin), 1.);
    ratioErrorGraph->SetPointError(iPoint, hTotalStack->GetBinWidth(iBin) / 2.,
                                   hTotalStack->GetBinWidth(iBin) / 2.,
                                   ratioBandErrorLower, ratioBandErrorUpper);
    iPoint++;
  }
  return ratioErrorGraph;
}

//__________________________________________________________________________________|___________

TGraphAsymmErrors* TQPlotter::getRatioGraph(TH1* h_data, TH1* hTotalBkg, bool invert, double ratioContentThreshold, bool verbose){
  // produce a TGraphAsymmErrors as the ratio between a data and an MC histogram

  int nBins = hTotalBkg->GetNbinsX();

  int nRatioPoints = 0;
  for (int i = 1; i <= nBins; i++) {
    double mcVal = hTotalBkg->GetBinContent(i);
    double dataVal = h_data->GetBinContent(i);
    if (mcVal < ratioContentThreshold || dataVal < ratioContentThreshold) continue;
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
    return NULL;
  }

  // the graph used to draw the ratio points
  TGraphAsymmErrors * ratioGraph = new TGraphAsymmErrors(nRatioPoints);
  ratioGraph->SetName(TString::Format("ratio_%s_%s",h_data->GetName(),hTotalBkg->GetName()));
  ratioGraph->SetTitle(TString::Format("%s (ratio)",h_data->GetTitle()));
  ratioGraph->SetLineColor(h_data->GetLineColor());
  ratioGraph->SetMarkerSize(h_data->GetMarkerSize());
  ratioGraph->SetMarkerStyle(h_data->GetMarkerStyle());
  ratioGraph->SetMarkerColor(h_data->GetMarkerColor());

  int iRatioPoint = 0;

  // loop over all bins of the histogram
  for (int iBin = 1; iBin <= nBins; iBin++) {
    double x = hTotalBkg->GetBinCenter(iBin);
    // get the values and errors of data and MC for this bin
    double data    = h_data ->GetBinContent(iBin);
    double dataErrUp = h_data->GetBinErrorUp(iBin);
    double dataErrDown = h_data->GetBinErrorLow(iBin);
    double MC      = hTotalBkg->GetBinContent(iBin);
    // cannot do anything if MC expectation is zero
    if (MC < ratioContentThreshold || data < ratioContentThreshold) continue;

    double ratio = invert ? MC / data : data / MC;
    double ratioErrorUp = dataErrUp / MC;
    double ratioErrorDown = dataErrDown / MC;
    if(verbose) VERBOSEclass("adding ratio point with x=%f, y=%f (data=%f, MC=%f)",x,ratio,data,MC);
    ratioGraph->SetPoint(iRatioPoint, x, ratio);
    ratioGraph->SetPointError(iRatioPoint,
                              0,0,
                              ratioErrorDown,ratioErrorUp
                              );
    iRatioPoint++;
  }
  if(verbose) VERBOSEclass("completed ratio graph with %d (%d) points",iRatioPoint,ratioGraph->GetN());
  return ratioGraph;
}
