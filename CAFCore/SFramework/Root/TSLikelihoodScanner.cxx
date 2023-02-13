#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>

#include "SFramework/TSLikelihoodScanner.h"

#include "TKey.h"
#include "RooNLLVar.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooRealVar.h"
#include "Math/ProbFuncMathCore.h"
#include "Math/MinimizerOptions.h"
#include "Math/QuantFuncMathCore.h"
#include "TStopwatch.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TCanvas.h"

#include "QFramework/TQIterator.h"
#include "QFramework/TQNamedTaggable.h"

#include "QFramework/TQLibrary.h"

#include "SFramework/TSUtils.h"

ClassImp(TSLikelihoodScanner)

namespace {
  inline bool areEqualAbs(Double_t af, Double_t bf, Double_t epsilon) {
    //return kTRUE if absolute difference between af and bf is less than epsilon
    return TMath::Abs(af-bf) < epsilon;
  }
}

//__________________________________________________________________________________|___________

TSLikelihoodScanner::TSLikelihoodScanner(RooWorkspace * ws, TQFolder* snapshots) : TSStatisticsCalculator("TSLikelihoodScanner",ws,snapshots) {
  // default constructor
}


//__________________________________________________________________________________|___________

TSLikelihoodScanner::~TSLikelihoodScanner() {
  // default destructor
}


//__________________________________________________________________________________|___________

void TSLikelihoodScanner::info(TString message) {
  // print an info message
  std::cout << "SFramework/TSLikelihoodScanner: " << message.Data() << std::endl;
}

//__________________________________________________________________________________|___________

TString TSLikelihoodScanner::stringify(const Point& p){
  // get a string-representation of a point in likelihood space
  std::stringstream ss;
  bool first = true;
  for(const auto& par:p){
    if(!first) ss << ", ";
    first=false;
    ss << par.first;
    ss << "=";
    ss << par.second;
  }
  return TString(ss.str().c_str());
}

//__________________________________________________________________________________|___________

std::map<TString,TQFolder*> TSLikelihoodScanner::collectParameters(TQFolder* options){
  // collect all the parameters to be scanned
  std::map<TString,TQFolder*> params;
  TQFolderIterator itr(options->getListOfFolders("?"));
  while(itr.hasNext()){
    TQFolder* par = itr.readNext();
    if(!par) continue;
    TString parname= par->getTagStringDefault("name",par->GetName());
    params[parname] = par;
    RooRealVar* v = fWorkspace->var(parname);
    if(!v){
      error(TString::Format("unable to retrieve parameter '%s' from workspace!",parname.Data()));
    }
  }
  return params;
}

//__________________________________________________________________________________|___________

TQFolder* TSLikelihoodScanner::runPreFit(TQFolder * options){
  // run a pre-fit, refitting the unconditional minimum
  info("running pre-fit");

  const TString datasetName = options->getTagStringDefault("datasetName", "obsData");
  TString snapshotInitial = options->getTagStringDefault("snapshot.initial",options->getTagStringDefault("snapshot",TString::Format("SnSh_AllVars_Unconditional_%s", datasetName.Data())));
  TString snapshotUnconditional = options->getTagStringDefault("snapshot.unconditional",TString::Format("SnSh_AllVars_Unconditional_%s", datasetName.Data()));

  // setup the fit options

  TQTaggable fitOptions;
  fitOptions.importTagsWithoutPrefix(*options,"fit.");
  fitOptions.setTagBool("reuseMinimizer",false); // this is changed by the "recycle" option
  fitOptions.setTagBool("reuseNll",false); // this we always set to true for consistency
  fitOptions.setTagBool("adjustPOI",false);
  fitOptions.setTagString("datasetName",datasetName);

  // iterate of parameters to scan to collect their names
  std::map<TString,TQFolder*> params = collectParameters(options);

  // load nominal snapshot to start from
  fitOptions.setTagString("snapshot",snapshotInitial);
  if(!this->loadSnapshot(&fitOptions)){
    throw std::runtime_error(TString::Format("unable to load snapshot '%s'",snapshotInitial.Data()).Data());
  } else {
    info(TString::Format("using snapshot '%s'",snapshotInitial.Data()));
  }

  TQFolder* uncondResult = NULL;
  this->setParametersConstFloat(fWorkspace->allVars(),&fitOptions);
  if(options->getTagBoolDefault("refit",true)){
    // refit the unconditional minimum to be extra-safe
    info("refitting unconditional minimum");
    for(auto p:params){
      RooRealVar* v = fWorkspace->var(p.first);
      if(!v) throw std::runtime_error(TString::Format("unable to find parameter '%s'",p.first.Data()).Data());
      v->setConstant(false);
    }
    this->setup(datasetName,&fitOptions);
    uncondResult = this->minimizeNll(&fitOptions);
    int strategy = -1;
    int status = -1;
    if(uncondResult){
      uncondResult->getTagInteger("status", status);
      uncondResult->getTagInteger("strategy", strategy);
    }
    if (!uncondResult || (status != 0 && status != 1)) {
      throw std::runtime_error(TString::Format("runCalculation(...): Unconditional fit failed with status=%d (strategy %d). Stopping!",status,strategy).Data());
    }

  } else {

    // acquire the unconditional point and fit result from the workspace snapshot loaded previously
    uncondResult = new TQFolder("unconditional");
    RooArgSet floatVars(fWorkspace->allVars());
    TSUtils::removeConstantParameters(floatVars);
    TSUtils::convertParameterList(&floatVars,uncondResult->getFolder("floatParsFinal+"));
    RooArgSet constVars(fWorkspace->allVars());
    TSUtils::removeFloatingParameters(floatVars);
    TSUtils::convertParameterList(&constVars,uncondResult->getFolder("constPars+"));

    for(auto par:params){
      TString name(par.first);
      TQFolder * f = uncondResult->getFolder("floatParsFinal+")->getFolder(name+"+");
      double val = fWorkspace->var(name)->getVal();
      f->setTagDouble("val",val);
    }
    double nllval = fNll->getVal();

    uncondResult->setTagBool("dummy",true);
    uncondResult->setTagDouble("minNll",nllval);
    info(TString::Format("re-using unconditional minimum from snapshot '%s' (Nll=%.5f)",snapshotUnconditional.Data(),nllval));
  }
  // unconditional minimal NLL
  Double_t uncondMinNll;
  if(!uncondResult->getTagDouble("minNll",uncondMinNll)){
    throw std::runtime_error("unable to dermine Nll minimum!");
  }

  // if the initial snapshot is not supposed to be the unconditional one, then create it
  if(snapshotInitial != snapshotUnconditional){
    info(TString::Format("runCalculation(...): saving unnconditional snapshot as '%s'!",snapshotUnconditional.Data()));
    fWorkspace->saveSnapshot(snapshotUnconditional.Data(),fWorkspace->allVars());
  }

  this->resetOffset();
  if(uncondMinNll != fNll->getVal()){
    warn(TString::Format("inconsistency detected (%.5f vs %.5f), Nll value of unconditional fit not preserved accurately through offsetting!",uncondMinNll,fNll->getVal()).Data());
    uncondMinNll = fNll->getVal();
    uncondResult->setTagDouble("minNll",uncondMinNll);
  }
  uncondResult->setTagBool("unconditional",true);

  Point uncond;
  for(auto p:params){
    TQFolder* floatParsFinal = uncondResult->getFolder("floatParsFinal+");
    TQFolder* par = floatParsFinal->getFolder(p.first);
    if(par){
      uncond[p.first] = par->getTagDoubleDefault("val",0.);
    }
  }
  info(TString::Format("found unconditional minimum (point 0) at %s with %.5f",stringify(uncond).Data(),uncondMinNll));

  return uncondResult;
}

//__________________________________________________________________________________|___________

TQFolder * TSLikelihoodScanner::runCalculation(TQFolder * options){
  // perform the entire calculation chain, including the pre-fit as well as the actual scan

  if (!fWorkspace || !fModelConfig || !options){
    // expect valid inputs
    return NULL;
  }

  TQFolder* uncondResult = NULL;
  const TString datasetName = options->getTagStringDefault("datasetName", "obsData");
  bool refitUnconditional = options->getTagBoolDefault("refitUnconditional", true);
  TString snapshotUnconditional = options->getTagStringDefault("snapshot.unconditional",options->getTagStringDefault("snapshot",TString::Format("SnSh_AllVars_Unconditional_%s", datasetName.Data())));
  if(!fWorkspace->loadSnapshot(snapshotUnconditional) || refitUnconditional){
    uncondResult = runPreFit(options);
  }

  std::map<Point,TQFolder*> result = runScan(options,uncondResult);

  TQFolder* scan = finalizeScan(options,result);
  scan->setTagString("content","points");

  const TString snapshotNominal = options->getTagStringDefault("snapshot.nominal","SnSh_AllVars_Nominal");
  fWorkspace->loadSnapshot(snapshotNominal);

  return scan;
}

//__________________________________________________________________________________|___________

std::map<TSLikelihoodScanner::Point,TQFolder*> TSLikelihoodScanner::runScan(TQFolder * options, TQFolder* uncondResult){
  // run scan of the conditional points
  TString logfile;
  bool redirect = options->getTagString("fit.logToFile",logfile);
  // read the options
  bool recycle = options->getTagBoolDefault("recycle",false);
  bool shufflePoints = options->getTagBoolDefault("shufflePoints",false);
  bool startFromLastPoint = options->getTagBoolDefault("startFromLastPoint",false);
  const TString datasetName = options->getTagStringDefault("datasetName", "obsData");
  const TString snapshotUnconditional = options->getTagStringDefault("snapshot.unconditional",TString::Format("SnSh_AllVars_Unconditional_%s", datasetName.Data()));
  const TString snapshotInitial =  options->getTagStringDefault("snapshot.initial",snapshotUnconditional);

  // setup the fit options
  TQTaggable fitOptions;
  fitOptions.importTagsWithoutPrefix(*options,"fit.");
  // bool hesse = fitOptions.getTagBoolDefault("runHesse",false); // BW: hashed (unused variable)
  fitOptions.setTagBool("reuseNll",true); // this we always set to true for consistency
  fitOptions.setTagBool("adjustPOI",false);
  fitOptions.setTagString("datasetName",datasetName);

  int constOpt = fitOptions.getTagIntegerDefault("constOptimize",1);

  // iterate of parameters to scan to collect their names
  std::map<TString,TQFolder*> params = collectParameters(options);
  fitOptions.setTagBool("runMinos",false);
  fitOptions.setTagBool("runHesse",false);
  fitOptions.setTagBool("loadSnapshot",false);

  for(const auto& par:params){
    fitOptions.removeTag(TString("initParam.")+par.first);
    fitOptions.removeTag(TString("presetParam.")+par.first);
  }

  // create the list of scheduled points
  std::vector<Point> points;
  Point emptypoint;
  std::vector<TQFolder*> folders;
  for(auto p:params){
    folders.push_back(p.second);
  }
  generatePoints(folders,points,emptypoint,0);

  // iterate over all scheduled points
  const size_t npoints = points.size();
  info(TString::Format("running scan over %d points",(int)npoints));

  size_t lastPoint  = std::min(options->getTagIntegerDefault("onlyPoints.last",npoints-1),(int)(npoints-1));
  size_t firstPoint = options->getTagIntegerDefault("onlyPoints.first",0);
  if(shufflePoints){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle ( points.begin()+firstPoint, points.begin()+lastPoint, std::default_random_engine(seed));
  }

  Point uncond;
  std::map<TSLikelihoodScanner::Point,TQFolder*> result;
  bool isFirstPoint = true;
  RooArgSet* allVars = NULL;
  for(size_t ipoint = firstPoint; ipoint<=lastPoint; ++ipoint){
    const Point& point(points[ipoint]);

    // if this is the first point, take care that we don't need to recreate the minimizer everytime
    if(isFirstPoint){

      // create the nll
      fWorkspace->loadSnapshot(snapshotUnconditional);
      this->setParametersConstFloat(fWorkspace->allVars(),&fitOptions);
      for(const auto& par:point){
        RooRealVar* var = fWorkspace->var(par.first);
        var->setConstant(true);
      }
      fitOptions.setTagBool("reuseMinimizer",true);
      if(redirect) TQLibrary::redirect(logfile,true);
      this->setup(datasetName,&fitOptions);
      if(redirect) TQLibrary::restore();
      allVars = fNll->getVariables();

      // check for the unconditional result
      if(uncondResult){

        if(!uncondResult->getTagBoolDefault("unconditional",false)) throw std::runtime_error("internal error: unconditional result is missing the 'unconditional' tag!");

        // prepare the uncondtional point from the fit result
        for(auto par:params){
          TString name(par.first);
          TQFolder * f = uncondResult->getFolder("floatParsFinal")->getFolder(name);
          if(!f){
            throw std::runtime_error(TString::Format("unable to find parameter '%s' in fit result!",name.Data()).Data());
          }
          double val = f->getTagDoubleDefault("val");
          uncond[name] = val;
        }
      } else {
        // acquire the unconditional point and fit result from the workspace snapshot loaded previously

        uncondResult = new TQFolder("unconditionalDummy");
        RooArgSet floatVars(fWorkspace->allVars());
        floatVars.remove(*(fModelConfig->GetObservables()));
        floatVars.remove(*(fModelConfig->GetGlobalObservables()));
        TSUtils::removeConstantParameters(floatVars);
        TSUtils::convertParameterList(&floatVars,uncondResult->getFolder("floatParsFinal+"));
        RooArgSet constVars(fWorkspace->allVars());
        constVars.remove(*(fModelConfig->GetObservables()));
        constVars.remove(*(fModelConfig->GetGlobalObservables()));
        TSUtils::removeFloatingParameters(floatVars);
        TSUtils::convertParameterList(&constVars,uncondResult->getFolder("constPars+"));

        for(auto par:params){
          TString name(par.first);
          TQFolder * f = uncondResult->getFolder("floatParsFinal+")->getFolder(name+"+");
          double val = fWorkspace->var(name)->getVal();
          f->setTagDouble("val",val);
          uncond[name] = val;
        }
        TSUtils::setParameterValues(allVars,fWorkspace->allVars());
        double nllval = fNll->getVal();
        this->resetOffset();
        if(nllval != fNll->getVal()){
          throw std::runtime_error("Nll offset broken!");
        }
        uncondResult->setTagBool("unconditional",true);
        uncondResult->setTagBool("dummy",true);
        uncondResult->setTagDouble("minNll",nllval);
        info(TString::Format("re-using unconditional minimum (point 0) from snapshot '%s' at %s (Nll=%.5f)",snapshotUnconditional.Data(),stringify(uncond).Data(),nllval));
      }
      result[uncond] = uncondResult;
    }

    // acquire the starting values
    if(isFirstPoint || !startFromLastPoint){
      fWorkspace->loadSnapshot(snapshotInitial);
      // set the the const/float attributes so the parameter counting doesn't get confused later
      this->setParametersConstFloat(fWorkspace->allVars(),&fitOptions);
      for(const auto& par:point){
        RooRealVar* var = fWorkspace->var(par.first);
        var->setConstant(true);
      }
    }

    // set the POI values to the point
    bool isuncond = true;
    for(const auto& par:point){
      RooRealVar* var = fWorkspace->var(par.first);
      const double val(par.second);
      var->setVal(val);
      var->setConstant(true);
      if(var->getVal() != par.second){
        throw std::runtime_error(TString::Format("inconsistency detected, scanned parameter '%s' [%g,%g] from workspace has value %g after setting it to %g!",var->GetName(),var->getMin(),var->getMax(),var->getVal(),val).Data());
      }
      if(uncond[par.first] != par.second){
        isuncond=false;
      }
    }

    isFirstPoint = false;

    // print an info message
    // check if the point is identical to the unconditional minimum
    if(isuncond){
      info(TString::Format(" point %s is identical to unconditional minimum",stringify(point).Data()));
      continue;
    } else {
      info(TString::Format(" fitting point %d/%d at %s",int(ipoint+1),int(npoints),stringify(point).Data()));
    }

    if(!recycle){
      // create the Nll
      if(redirect){
        info(TString::Format(" writing log of Nll creation to '%s'",logfile.Data()));
        TQLibrary::redirect(logfile,true);
      }
      this->setup(datasetName,&fitOptions);
      allVars = fNll->getVariables();
      if(redirect) TQLibrary::restore();
    }

    // set the POI values to the point
    TSUtils::setParameterValues(allVars,fWorkspace->allVars());
    for(const auto& par:point){
      RooRealVar* nllvar = (RooRealVar*)(allVars->find(par.first));
      const double val(par.second);
      nllvar->setVal(val);
      nllvar->setConstant(true);
      if(nllvar->getVal() != par.second){
        throw std::runtime_error(TString::Format("inconsistency detected, scanned parameter '%s' [%g,%g] from Nll has value %g after setting it to %g!",nllvar->GetName(),nllvar->getMin(),nllvar->getMax(),nllvar->getVal(),val).Data());
      }
    }

    // perform a consistency check for const'ness
    for(const auto& par:point){
      RooRealVar* var = dynamic_cast<RooRealVar*>(allVars->find(par.first));
      if(!var->isConstant()){
        this->error(TString::Format("inconsistency detected, scanned parameter '%s' is floating before the fit (should be constant)!",var->GetName()).Data());
      }
      if(var->getVal() != par.second){
        this->error(TString::Format("inconsistency detected, scanned parameter '%s' has value %g before the fit (should be %g)!",var->GetName(),var->getVal(),par.second).Data());
      }
    }

    // run the minimizer
    fNll->constOptimizeTestStatistic(RooAbsArg::ValueChange,constOpt>1);
    if(redirect) TQLibrary::redirect(logfile,true);
    TQFolder* fitResult = this->minimizeNll(&fitOptions);
    if(redirect) TQLibrary::restore();

    // perform a consistency check for const'ness
    for(const auto& par:point){
      RooRealVar* var = dynamic_cast<RooRealVar*>(allVars->find(par.first));
      if(!var){
        this->error(TString::Format("unable to find parameter '%s'!",par.first.Data()).Data());
        continue;
      }

      if(!var->isConstant()){
        this->error(TString::Format("inconsistency detected, scanned parameter '%s' is floating after the fit (should be constant)!",var->GetName()).Data());
      }

      if(var->getVal() != par.second){
        this->error(TString::Format("inconsistency detected, scanned parameter '%s' has value %g after the fit (should be %g)!",var->GetName(),var->getVal(),par.second).Data());
      }
    }

    // extract and store the results
    double minnll = fitResult->getTagDoubleDefault("minNll",std::numeric_limits<double>::infinity());
    fNll->constOptimizeTestStatistic(RooAbsArg::ValueChange,constOpt>1);
    if(minnll != fNll->getVal()){
      this->error(TString::Format("inconsistency detected (%.5f vs %.5f), Nll value not preserved accurately!",minnll, fNll->getVal()).Data());
      continue;
    }

    result[point]=fitResult;
    if(!recycle){
      delete allVars;
      allVars = NULL;
    }
  }


  // check back the unconditional point
  double uncondMinNll;
  if(!uncondResult->getTagDouble("minNll",uncondMinNll)){
    throw std::runtime_error("unable to determine Nll minimum!");
  }

  // acquire and propagate the values properly
  if(!allVars) allVars = fNll->getVariables();
  fWorkspace->loadSnapshot(snapshotUnconditional);
  TSUtils::setParameterValues(allVars,fWorkspace->allVars());
  this->setParametersConstFloat(allVars,&fitOptions);

  // perform the check
  double minNll = fNll->getVal();
  if(!::areEqualAbs(uncondMinNll,minNll,1e-5)){
    warn(TString::Format("inconsistency detected (%.5f vs %.5f), Nll value of unconditional fit (%s) not preserved accurately - will be refitted later!",uncondMinNll,minNll,stringify(uncond).Data()).Data());
  } else {
    info(TString::Format("absolute minimum of Nll is at %.5f",uncondMinNll).Data());
  }
  delete allVars;

  return result;
}

//__________________________________________________________________________________|___________

TQFolder* TSLikelihoodScanner::finalizeScan(TQFolder* options, std::map<Point,TQFolder*>& result){
  // finalize the scan, cross-checking the minima if requested
  TString logfile;
  bool redirect = options->getTagString("fit.logToFile",logfile);
  // read the options
  bool storeFullResults = options->getTagBoolDefault("storeFullResults",false);
  bool saveFullSnapshots = options->getTagBoolDefault("saveFullSnapshots",false);
  const TString datasetName = options->getTagStringDefault("datasetName", "obsData");
  TString snapshotInitial = options->getTagStringDefault("snapshot.initial",options->getTagStringDefault("snapshot",TString::Format("SnSh_AllVars_Unconditional_%s", datasetName.Data())));
  TString snapshotUnconditional = options->getTagStringDefault("snapshot.unconditional",options->getTagStringDefault("snapshot",TString::Format("SnSh_AllVars_Unconditional_%s", datasetName.Data())));

  // setup the fit options
  TQTaggable fitOptions;
  fitOptions.importTagsWithoutPrefix(*options,"fit.");
  fitOptions.setTagBool("reuseMinimizer",false); // this is changed by the "recycle" option
  fitOptions.setTagBool("reuseNll",true); // this we always set to true for consistency
  fitOptions.setTagBool("adjustPOI",false);
  fitOptions.setTagString("datasetName",datasetName);

  bool crossCheckMinima = options->getTagBoolDefault("crossCheckMinima",false);
  RooArgSet* allVars = fNll->getVariables();

  std::map<Point,TQFolder*> final;
  for(const auto& r:result){
    bool consistent = true;
    bool correct = true;
    bool isuncond = r.second->getTagBoolDefault("unconditional",false);
    double minnll = r.second->getTagDoubleDefault("minNll",std::numeric_limits<double>::infinity());
    if(crossCheckMinima){
      TSUtils::setParameterValues(*allVars,r.second->getFolder("floatParsFinal"));
      TSUtils::setParameterValues(*allVars,r.second->getFolder("constPars"));
      fNll->constOptimizeTestStatistic(RooAbsArg::ValueChange,false);
      double nllval = fNll->getVal();
      correct = ::areEqualAbs(nllval,minnll,1e-5);
      if(!correct){
        if(isuncond) warn(TString::Format("inconsistency detected: unconditional minimum at %s changes Nll value from %.5f to %.5f when loading snapshot, refitting!",stringify(r.first).Data(),minnll,nllval).Data());
        else warn(TString::Format("inconsistency detected: minimum from %s changes Nll value from %.5f to %.5f when loading snapshot, refitting!",stringify(r.first).Data(),nllval,minnll).Data());
      }
    }
    Point startpoint=r.first;
    if(correct && crossCheckMinima){
      double minval = minnll;
      for(const auto& r2:result){
        TSUtils::setParameterValues(*allVars,r2.second->getFolder("floatParsFinal"));
        TSUtils::setParameterValues(*allVars,r.second->getFolder("constPars"));
        for(const auto& par:r.first){
          RooRealVar* var = (RooRealVar*)(allVars->find(par.first));
          var->setVal(par.second);
        }
        fNll->constOptimizeTestStatistic(RooAbsArg::ValueChange,false);
        double othermin = fNll->getVal();
        if(othermin < minval){
          startpoint = r2.first;
          minval = othermin;
        }
      }
      consistent = !(minnll < minval);
      if(!consistent) warn(TString::Format("inconsistency detected: minimum from %s provides improvement for %s, refitting!",stringify(startpoint).Data(),stringify(r.first).Data()));
    }
    if(!consistent || !correct){
      TSUtils::setParameterValues(*allVars,result[startpoint]->getFolder("floatParsFinal"));
      for(const auto& par:r.first){
        RooRealVar* var = (RooRealVar*)(fWorkspace->var(par.first));
        var->setVal(par.second);
        if(!isuncond){
          var->setConstant(true);
        } else{
          var->setConstant(false);
        }
      }

      // run the minimizer
      if(redirect) TQLibrary::redirect(logfile,true);
      fNll->constOptimizeTestStatistic(RooAbsArg::ValueChange,false);
      TQFolder* fitResult = this->minimizeNll(&fitOptions);
      if(redirect) TQLibrary::restore();

      // extract and store the results
      double minnll = fitResult->getTagDoubleDefault("minNll",std::numeric_limits<double>::infinity());
      if(minnll != fNll->getVal()){
        throw std::runtime_error("inconsistency detected, Nll value not preserved accurately!");
      }
      final[r.first]=fitResult;
    } else {
      //      info(TString::Format("minimum from %s is consistent, no improvement found!",stringify(r.first).Data()));
      final[r.first]=r.second;
    }

    if(saveFullSnapshots){
      TString k(stringify(r.first));
      k.ReplaceAll("=","_");
      k.ReplaceAll(",","_");
      k.ReplaceAll(" ","");
      TSUtils::setParameterValues(fWorkspace->allVars(),allVars);
      TSUtils::setParametersConstFloat(fWorkspace->allVars(),allVars);
      this->fWorkspace->saveSnapshot(TString::Format("Scan_%s_%s",options->GetName(),k.Data()).Data(),*allVars);
    }

  }
  double ymin = std::numeric_limits<double>::infinity();
  for(const auto& r:final){
    double minnll = r.second->getTagDoubleDefault("minNll",std::numeric_limits<double>::infinity());
    ymin = std::min(ymin,minnll);
  }

  delete allVars;

  // create the result folder
  std::map<TString,TQFolder*> params = collectParameters(options);
  const size_t npar(params.size());
  TQFolder * resultfolder = TQFolder::newFolder("Scan");
  TQFolder* graph = resultfolder->getFolder("Scan+");
  graph->setTagInteger("nDim",npar);
  int ipar=0;
  for(const auto& par:params){
    TString lbl(TString::Format("x%d",ipar));
    graph->setTagString(lbl,par.first);
    ++ipar;
  }

  size_t ipoint = options->getTagIntegerDefault("onlyPoints.first",0);;
  for(const auto& r:final){
    double minnll = r.second->getTagDoubleDefault("minNll",std::numeric_limits<double>::infinity());
    double y = 2 * ( minnll - ymin );
    // put them together nicely
    bool isuncond = r.second->getTagBoolDefault("unconditional",false);
    TQFolder* p = graph->getFolder(TString::Format("p.%d+",isuncond ? 0 : int(ipoint+1)));
    if(isuncond || storeFullResults){
      r.second->setName(".fit");
      p->addFolder(r.second);
    } else {
      delete r.second;
    }
    size_t ipar=0;
    for(auto par:params){
      const double val = r.first.at(par.first);
      p->setTagDouble(TString::Format("x%d",(int)(ipar)),val);
      ++ipar;
    }
    p->setTagDouble("y",y);
    p->setTagDouble("minNll",minnll);
    info(TString::Format("result at %s is %f (%.15g)",stringify(r.first).Data(),y,minnll));
    ipoint++;
  }
  fWorkspace->loadSnapshot(snapshotUnconditional);

  return resultfolder;
}

int TSLikelihoodScanner::nPoints(TQFolder* config){
  TQFolderIterator itr(config->getListOfFolders("?"),true);
  int n = 1;
  while(itr.hasNext()){
    TQFolder* par = itr.readNext();
    int nbins;
    if(!par->getTagInteger("nbins",nbins)){
      throw std::runtime_error(TString::Format("unable to estimate number of points without <nbins> tag in %s/%s",config->getRoot()->GetName(),config->getPath().Data()).Data());
    }
    n *= (nbins+1);
  }
  return n;
}

void TSLikelihoodScanner::generatePoints(const std::vector<TQFolder*>& params, std::vector<Point>& points, const Point& thispoint, size_t idx){
  TQFolder* par = params[idx];
  TString parname= par->getTagStringDefault("name",par->GetName());

  int nbins;
  RooRealVar* var = fWorkspace->var(parname);
  double low,high;

  double lo,hi;
  bool rel = par->getTagBoolDefault("rel",false);
  TSUtils::getPlotRange(var,lo,hi,nbins);
  par->getTagDouble("min",lo);
  par->getTagDouble("max",hi);
  par->getTagInteger("nbins",nbins);
  if(rel){
    double absmin = par->getTagDoubleDefault("absmin",var->getMin());
    double absmax = par->getTagDoubleDefault("absmax",var->getMax());
    double val = var->getVal();
    low  = std::max(absmin,val - fabs(lo * var->getErrorLo()));
    high = std::min(absmax,val + fabs(hi * var->getErrorHi()));
  } else {
    low = lo;
    high = hi;
  }

  // Perform the scan
  double delta_x = (high - low) / nbins;

  for (int i = 0; i <= nbins; ++i) {
    double val = low + i * delta_x;
    Point point(thispoint);
    point[parname] = val;
    if(idx+1 < params.size()){
      generatePoints(params,points,point,idx+1);
    } else {
      points.push_back(point);
    }
  }
}
