#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <iomanip>


#include "RooFitResult.h"
#include "RooMinimizer.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooAbsArg.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooProfileLL.h"
#include "RooPlot.h"
#include "TFile.h"
#include "TMath.h"
#include "Math/MinimizerOptions.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

#include "QFramework/TQStringUtils.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsCalculator.h"
#include "SFramework/TSUtils.h"


#include "SFramework/RooFitUtils.h"
#ifdef HAS_RooFitUtils
#pragma message "using RooFitUtils ExtendedMinimizer"
#include <RooFitUtils/ExtendedMinimizer.h>
#define ROOFITUTILS true
#else
#pragma message "using standard RooMinimizer"
#define ROOFITUTILS false
#endif

ClassImp(TSStatisticsCalculator)


namespace {
  void statinfo(TString message) {
    std::cout << "SFramework/TSStatisticsCalculator: " << message.Data() << std::endl;
  }


  //__________________________________________________________________________________|___________

  void staterror(TString message) {
    statinfo(TQStringUtils::makeBoldRed(TString("ERROR: ") + message));
  }


  //__________________________________________________________________________________|___________

  void statwarn(TString message) {
    statinfo(TQStringUtils::makeBoldYellow(TString("WARNING: ") + message));
  }

}

//__________________________________________________________________________________|___________

TSStatisticsCalculator::TSStatisticsCalculator(const char* className) :
#ifdef HAS_RooFitUtils
  RooFitUtils::ExtendedMinimizer(className,NULL,NULL),
#endif
  fModelConfig(NULL)
{
	// default constructor
	(void)className; //silence compiler warning (unused parameter)
}


//__________________________________________________________________________________|___________

TSStatisticsCalculator::TSStatisticsCalculator(const char* className, RooWorkspace * ws) :
  TSStatisticsCalculator(className)
{
	// constructor taking workspace as an argument
  setWorkspace(ws);
}

//__________________________________________________________________________________|___________

TSStatisticsCalculator::TSStatisticsCalculator(const char* className, RooWorkspace * ws, TQFolder* snapshots) :
  TSStatisticsCalculator(className)
{
	// constructor taking workspace as an argument
	this->fSnapshots = snapshots;
  setWorkspace(ws);
}


//__________________________________________________________________________________|___________

TSStatisticsCalculator::~TSStatisticsCalculator() {
	// default destructor
}


//__________________________________________________________________________________|___________

void TSStatisticsCalculator::info(TString message) {
	// print an info message
	statinfo(message);
}


//__________________________________________________________________________________|___________

void TSStatisticsCalculator::error(TString message) {
	// print an error message
  info(TQStringUtils::makeBoldRed(TString("ERROR: ") + message));
}


//__________________________________________________________________________________|___________

void TSStatisticsCalculator::warn(TString message) {
	// print a warning message
  info(TQStringUtils::makeBoldYellow(TString("WARNING: ") + message));
}


//__________________________________________________________________________________|___________

Bool_t TSStatisticsCalculator::setWorkspace(RooWorkspace * ws) {
	// set the workspace
	if (!ws) {
		return false;
	}

	TObject * obj = ws->obj("ModelConfig");
	if (!obj) {
		return false;
	}

	fModelConfig = (RooStats::ModelConfig*)obj;
	fWorkspace = ws;
	return true;
}


//__________________________________________________________________________________|___________

RooWorkspace * TSStatisticsCalculator::getWorkspace() {
	// obtain the workspace
	return fWorkspace;
}

//__________________________________________________________________________________|___________

TQFolder * TSStatisticsCalculator::fitPdfToData(const TString& datasetName, TQTaggable * options) {
	// run a fit to the dataset with the given name using the given options
	DEBUGclass("entering function with name,options signature");
	// check for valid workspace and model config
	if (!fWorkspace){
		error("no workspace!");
		return NULL;
	}
	if (!fModelConfig){
		error("no model config!");
		return NULL;
	}

	// get data set to construct NLL from
	RooDataSet * data = (RooDataSet*)fWorkspace->data(datasetName.Data());
	if (!data) {
		error(TString::Format("no dataset '%s' available in workspace '%s'!",datasetName.Data(),fWorkspace->GetName()));
		return NULL;
	}

	// use standards for PDF and nuisance parameters
	RooAbsPdf * pdf		= fModelConfig->GetPdf();
	if (!pdf) {
		error("no Pdf associated to ModelConfig!");
		return NULL;
	}

	RooArgSet nuisAndPOIs(*(fModelConfig->GetNuisanceParameters()));
	nuisAndPOIs.add(*(fModelConfig->GetParametersOfInterest()));

	// finally create NLL
	return fitPdfToData(pdf, data, &nuisAndPOIs, options);
}

//__________________________________________________________________________________|___________

void TSStatisticsCalculator::setup(const TString& datasetName, TQTaggable * options) {
  // setup a fit to the dataset with the given name using the given options
  DEBUGclass("entering function with name,options signature");
  // check for valid workspace and model config
  if (!fWorkspace){
    throw std::runtime_error("no workspace!");
  }
  if (!fModelConfig){
    throw std::runtime_error("no model config!");
  }

  // get data set to construct NLL from
  RooDataSet * data = (RooDataSet*)fWorkspace->data(datasetName.Data());
  if (!data) {
    throw std::runtime_error(TString::Format("no dataset '%s' available in workspace '%s'!",datasetName.Data(),fWorkspace->GetName()).Data());
  }

  // use standards for PDF and nuisance parameters
  RooAbsPdf * pdf		= fModelConfig->GetPdf();
  if (!pdf) {
    throw std::runtime_error("no Pdf associated to ModelConfig!");
  }

  // finally create NLL
  setup(pdf, data, fModelConfig->GetNuisanceParameters(), options);
}

//__________________________________________________________________________________|___________

namespace {
  RooNLLVar * createNll(RooWorkspace* fWorkspace, RooAbsPdf * pdf, RooDataSet * data, const RooArgSet * constrain, TQTaggable * options) {
		// helper function to create the NLL

    // check for valid inputs
    if (!pdf || !data) {
      return NULL;
    }

    // extended mode?
    Int_t extMode = pdf->extendMode();
    Bool_t extended = (extMode == RooAbsPdf::CanBeExtended
                       || extMode == RooAbsPdf::MustBeExtended);

    // parameters with constraints
    RooArgSet constrainedPars(*constrain);

    // fix/release parameters, set parameter values
    if (options) {
      TQIterator itr(options->getListOfKeys("par.const.*, par.val.*"), true);
      while (itr.hasNext()) {
        TString key = itr.readNext()->GetName();
        TString parName = key;

        Bool_t setConst = false;
        Bool_t setVal = false;
        if (TQStringUtils::removeLeadingText(parName, "par.const.")) {
          setConst = true;
        } else if (TQStringUtils::removeLeadingText(parName, "par.val.")) {
          setVal = true;
        } else {
          // should not happen
          continue;
        }

        RooRealVar * var = fWorkspace->var(parName);
        if (var) {
          if (setConst) {
            Bool_t isConst = false;
            if (options->getTagBool(key, isConst)) {
              if (isConst) {
                statinfo(TString::Format("createNll(): Fixing parameter '%s'", parName.Data()));
              } else {
                statinfo(TString::Format("createNll(): Releasing parameter '%s'", parName.Data()));
              }
              var->setConstant(isConst);
              // if parameter is set constant => remove from list of nuisance parameter
              if (isConst) {
                RooAbsArg * arg = constrainedPars.find(key.Data());
                if (arg) {
                  constrainedPars.remove(*arg);
                  statwarn(TString::Format("createNll(): Removing constraint term "
                                       "for constant variable '%s'", parName.Data()));
                }
              }
            } else {
              statwarn(TString::Format("createNll(): Invalid type of option '%s'. Ignoring ...", key.Data()));
            }

          } else if (setVal) {
            Double_t val = 0.;
            if (options->getTagDouble(key, val)) {
              statinfo(TString::Format("createNll(): Setting value of parameter '%s' to %g", parName.Data(), val));
              var->setVal(val);
            } else {
              statwarn(TString::Format("createNll(): Invalid type of option '%s'. Ignoring ...", key.Data()));
            }
          }

        } else {
          statwarn(TString::Format("createNll(): Unknown variable '%s'", parName.Data()));
        }
      }
    }

    // create NLL
    RooNLLVar * nll;
    if (constrainedPars.getSize() > 0) {
      // use constraint terms for nuisance parameters
      nll = (RooNLLVar*)pdf->createNLL(*data, RooFit::Constrain(constrainedPars), RooFit::Extended(extended));
    } else {
      nll = (RooNLLVar*)pdf->createNLL(*data, RooFit::Extended(extended));
    }

    return nll;
  }
}
#ifdef HAS_RooFitUtils      
namespace {
  void buildArgs(TQTaggable* options, std::vector<RooCmdArg>& args, RooLinkedList& l, RooStats::ModelConfig* mc){
    TString	minType		= ROOT::Math::MinimizerOptions::DefaultMinimizerType();
    TString	minAlgo		= ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo();
    if (options) {
      options->getTagString("minimizerType", minType);
      options->getTagString("minimizerAlgo", minAlgo);
    }
    args.push_back(RooFit::Minimizer(minType.Data(), minAlgo.Data()));
    args.push_back(RooFit::Save(true));

    RooArgSet * runMinosArgs = NULL;
    if (options) {
      // set the starting strategy to 1 regardless
      Int_t defaultStrategy = 1;
      options->getTagInteger("startingStrategy", defaultStrategy);
      args.push_back(RooFit::Strategy(defaultStrategy));
      Int_t numCPU;
      if(options->getTagInteger("numCPU", numCPU)){
        args.push_back(RooFit::NumCPU(numCPU, 1));
      }
      Double_t precision;
      if(options->getTagDouble("precision", precision)){
        args.push_back(RooFit::Precision(precision));
      }
#ifdef HAS_RooFitUtils      
      double eps;
      if(options->getTagDouble("eps", eps) || options->getTagDouble("tolerance", eps)){
        args.push_back(RooFitUtils::ExtendedMinimizer::Eps(eps));
      }
      if(options->getTagBoolDefault("reuseNll", false)){
        args.push_back(RooFitUtils::ExtendedMinimizer::ReuseNLL(1));
      }
      bool reuse = false;
      if(options->getTagBool("reuseMinimizer", reuse)){
        args.push_back(RooFitUtils::ExtendedMinimizer::ReuseMinimizer(reuse));
      }
      int maxcalls = 10000;
      if(options->getTagInteger("maxCalls", maxcalls)){
        args.push_back(RooFitUtils::ExtendedMinimizer::MaxCalls(maxcalls));
      }
      int maxiterations = 10000;
      if(options->getTagInteger("maxIterations", maxiterations)){
        args.push_back(RooFitUtils::ExtendedMinimizer::MaxIterations(maxiterations));
      }      
      Int_t numRetry = options->getTagIntegerDefault("numRetry", 1);
      if(numRetry > 0){
        args.push_back(RooFitUtils::ExtendedMinimizer::NumRetryFit(numRetry));
      }
      bool minimize = true;
      if(options->getTagBool("minimize", minimize)){
        args.push_back(RooFitUtils::ExtendedMinimizer::Minimize(minimize));
      }
#endif      
      bool runMinos = options->getTagBoolDefault("runMinos",options->hasTag("runMinosVars.0"));
      bool useFindSigma = options->getTagBoolDefault("useFindSigma",false);
      if(runMinos){
        std::vector<TString> runMinosVars = options->getTagVString("runMinosVars");
        if(runMinosVars.size() > 0){
          runMinosArgs = TSStatisticsCalculator::makeMinosArgSet(mc->GetWS(),runMinosVars);
        } else {
          runMinosArgs = const_cast<RooArgSet*>(mc->GetParametersOfInterest());
        }
#ifdef HAS_RooFitUtils
        if(useFindSigma){
          args.push_back(RooFitUtils::ExtendedMinimizer::FindSigma(*runMinosArgs));
        } else
#endif
          {
            args.push_back(RooFit::Minos(*runMinosArgs));
          }
      } else {
        args.push_back(RooFit::Minos(false));
      }

      Bool_t	runHesse;
      if(options->getTagBool("runHesse", runHesse)){
        if(runHesse){
          args.push_back(RooFit::Hesse(runHesse));
        }
      }
      Int_t  constOpt;
      if(options->getTagInteger("constOptimize", constOpt)){
        args.push_back(RooFit::Optimize(constOpt));
      }
      int  printLevel;
      if(options->getTagInteger("printLevel", printLevel)){
        args.push_back(RooFit::PrintLevel(printLevel));
      }
      bool offset;
      if(options->getTagBool("offset", offset)){
        args.push_back(RooFit::Offset(offset));
      }
    }
    const RooArgSet* nuis = mc->GetNuisanceParameters();
    args.push_back(RooFit::Constrain(*nuis));
    const RooArgSet* globs = mc->GetGlobalObservables();
    args.push_back(RooFit::GlobalObservables(*globs));
    //    std::cout << "#NPs: " << nuis->getSize() << std::endl;
    //    std::cout << "#Globs: " << globs->getSize() << std::endl;
    //    std::cout << "#Obs: " << mc->GetObservables()->getSize() << std::endl;
    //    std::cout << "#POIs: " << mc->GetParametersOfInterest()->getSize() << std::endl;

    for(auto& arg:args){
      //      INFO(TString::Format("%s %s: %d %d %f %f %s %s",arg.GetName(),arg.opcode(),arg.getInt(0),arg.getInt(1),arg.getDouble(0),arg.getDouble(1),arg.getString(0),arg.getString(1)));
      l.Add((TObject*)(&arg));
    }

  }
}
#endif

RooArgSet* TSStatisticsCalculator::getPOIs(TQTaggable* options){
	// get the parameters of interest in a RooArgSet
  const RooArgSet* poisPtr = fModelConfig->GetParametersOfInterest();
  if(!poisPtr){
    staterror("unable to retrieve list of POIs from ModelConfig!");
    return NULL;
  }

  Bool_t doRightSided			= false;
  options->getTagBool("~doRightSided", doRightSided);


  bool adjustPOI = options->getTagBoolDefault("fit.adjustPOI",false);
  std::vector<TString> poiNames = options->getTagVString("POI");
  std::vector<TString> constNames = options->getTagVString("const");
  if(poiNames.empty()) poiNames.push_back("*");

  std::vector<RooRealVar*> params;

  // collect things from the list of POIs
  RooAbsArg* obj;
  RooFIter itr(poisPtr->fwdIterator());
  while((obj = itr.next())){
    RooRealVar* poi = dynamic_cast<RooRealVar*>(obj);
    bool use = false;
    for(auto poiname:poiNames){
      if(TQStringUtils::matches(poi->GetName(),poiname)){
        use = true;
      }
    }
    for(auto poiname:constNames){
      if(TQStringUtils::matches(poi->GetName(),poiname)){
        use = false;
      }
    }

    if(use){
      params.push_back(poi);
    } else {
      poi->setConstant(true);
    }
  }

  // for explicitly listed parameters, also collect from the workspace
  for(const auto& pname:poiNames){
    if(pname.Contains("*")) continue;
    RooRealVar* v = fWorkspace->var(pname);
    if(v) params.push_back(v);
  }

  RooArgSet* pois = new RooArgSet();;
  for(auto poi:params){
    pois->add(*poi);
    if(adjustPOI){
      if (!doRightSided) {
        poi->setRange(0, 40);
      } else {
        poi->setRange(-40, 40);
      }
    }
  }

  return pois;
}

//__________________________________________________________________________________|___________

TQFolder * TSStatisticsCalculator::minimizeNll(const TString& options) {
  // run the minimizer
  TQTaggable tags(options);
  return minimizeNll(&tags);
}

#ifdef HAS_RooFitUtils
namespace {
  template<class T> void setLabels(TH2* hist, const T& names){
    if(hist->GetXaxis()->GetNbins() != names.size()) throw std::runtime_error("binning mismatch in X");
    if(hist->GetYaxis()->GetNbins() != names.size()) throw std::runtime_error("binning mismatch in Y");
    int i = 1;
    for(const auto& name:names){
      hist          ->GetXaxis()->SetBinLabel(i,name);
      hist          ->GetYaxis()->SetBinLabel(i,name);
      i++;
    }
  }
  
  TQFolder * convertFitResults(const RooFitUtils::ExtendedMinimizer::Result* extresult, TQTaggable* options){
    // convert a RooFitResult into a TQFolder
    TQFolder * result = TQFolder::newFolder("fitResult");

    // minimized -log(L) value
    result->setTagDouble("minNll", extresult->min.nll);
    // status
    result->setTagInteger("status", extresult->min.status);

    std::vector<TString> paramNames;
    for(auto p:extresult->min.config.ParamsSettings()){
      paramNames.push_back(p.Name());
    }
    
    if(extresult->min.ok()){
      RooFitResult* fitResult = extresult->min.fit;
      if(fitResult){
        // estimated distance to minimum
        result->setTagDouble("edm", fitResult->edm());
        for (UInt_t i = 0; i < fitResult->numStatusHistory(); i++) {
          result->setTagInteger(TString::Format("status.%s", fitResult->statusLabelHistory(i)),
                                fitResult->statusCodeHistory(i));
        }
        // number of NLL evaluations with problems
        result->setTagInteger("numInvalidNLL", fitResult->numInvalidNLL());
        
        // parameters
        TQFolder* constPars      = TSUtils::convertParameterList(&fitResult->constPars());
        TQFolder* floatParsInit  = TSUtils::convertParameterList(&fitResult->floatParsInit());
        TQFolder* floatParsFinal = TSUtils::convertParameterList(&fitResult->floatParsFinal());
        int floatPars = floatParsFinal->getNElements(false,TQFolder::Class());
        
        result->addObject(constPars,"::constPars");
        result->addObject(floatParsInit,"::floatParsInit");
        result->addObject(floatParsFinal,"::floatParsFinal");
      } else {
        TQFolder* floatParsInit = result->getFolder("floatParsInit+");
        TQFolder* floatParsFinal = result->getFolder("floatParsFinal+");        
        for(const auto& par:extresult->min.parameters){
          TQFolder* p = floatParsInit->getFolder(par.name+"+");
          p->setTagDouble("val",par.value);
          p->setTagDouble("errHi",par.errHi);
          p->setTagDouble("errLo",par.errLo);
          floatParsFinal->getFolder(par.name+"+")->importTags(p);
        }
      }
        
      // MINUIT quality code of covariance matrix
      result->setTagInteger("covQual", extresult->min.covqual);
      if(extresult->min.hesse){
        TH2* hHesse = TQHistogramUtils::convertMatrixToHistogram(extresult->min.hesse,"hesse");
        setLabels(hHesse,paramNames);
        result->addObject(hHesse);            
        Double_t determ = 0;
        result->setTagDouble("hesse.determinant",determ);
        TMatrixDSym cov = extresult->min.hesse->Invert(&determ);
        if(determ != 0){
          TMatrixDSym corr = TSUtils::getCorrelationMatrix(cov);
          TH2* hCov   = TQHistogramUtils::convertMatrixToHistogram(&cov, "covariance");
          TH2* hCorr  = TQHistogramUtils::convertMatrixToHistogram(&corr,"correlation");      
          setLabels(hCov,paramNames);
          setLabels(hCorr,paramNames);
          result->addObject(hCov);      
          result->addObject(hCorr);
        }
      }
    }
    return result;
  }
}
#endif


//__________________________________________________________________________________|___________

TQFolder * TSStatisticsCalculator::minimizeNll(TQTaggable* options) {
  // run the minimizer
  if(!options){
    this->error("invalid options passed to minimization function!");
    return NULL;
  }
  bool useRooFitUtils = options->getTagBoolDefault("useRooFitUtils",ROOFITUTILS);  

  RooFitResult * fitResult = NULL;
  int status = -1;
  int ndim = 0;

  this->setParameterValues(options,"initParam");
  this->setParameterErrors(options,"initParamError");

  // make sure that the parameters are the same as in the workspace
  if(fNll){
    RooArgSet* args = fNll->getVariables();
    TSUtils::setParameterValues(args,fWorkspace->allVars());
    TSUtils::setParametersConstFloat(args,fWorkspace->allVars());
    delete args;
  }

  TString outfile;
  bool redirect=options->getTag("logToFile",outfile);
  if(redirect){
    TString fname = options->replaceInText(outfile);
    fname = TQPathManager::getPathManager()->getTargetPath(fname);
    TQUtils::ensureDirectoryForFile(fname);
    statinfo(TString::Format("writing log of minimization to '%s'",fname.Data()));
    TQLibrary::redirect(fname,true);
  }

  // run the minimizer

#ifdef HAS_RooFitUtils
  RooFitUtils::ExtendedMinimizer::Result* extResult = NULL;
#endif
  Double_t tMini	= 0.;
  Double_t tHesse	= 0.;
  Double_t tMinos	= 0.;
  int strat = options->getTagIntegerDefault("startingStrategy", 1);
  if(useRooFitUtils){
#ifdef HAS_RooFitUtils
    DEBUG("starting extended minimizer");
    extResult = RooFitUtils::ExtendedMinimizer::run();
    DEBUG("fit completed");
    fitResult = extResult->min.fit;
    status = extResult->min.status;
    ndim = extResult->min.ndim;
    strat = extResult->min.strategy;
#else
    throw std::runtime_error("RooFitUtils not available!");
#endif
  } else {
    // default settings used if not overwritten by options
    //	Bool_t	saveNLL		= false;
    Bool_t	verbose		= false;
    Bool_t	runMinos	= false;
    Bool_t	runHesse	= false;
    Int_t	printLevel	= -1;
    TString	minType		= ROOT::Math::MinimizerOptions::DefaultMinimizerType();
    TString	minAlgo		= ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo();
    Double_t tolerance = 1.;
  
    if (options) {
      //		options->getTagBool("saveNLL", saveNLL);
      options->getTagBool("verbose", verbose);
      options->getTagBool("runMinos", runMinos);
      options->getTagBool("runHesse", runHesse);
      options->getTagInteger("printLevel", printLevel);
      options->getTagString("minimizerType", minType);
      options->getTagString("minimizerAlgo", minAlgo);
      options->getTagDouble("tolerance", tolerance);
      options->getTagDouble("eps", tolerance);
    }
  
    // check for valid input
    if (!this->fNll) {
      return NULL;
    }
  
    bool minimize = options->getTagBoolDefault("minimize",true);
    try {
      RooMinimizer minimizer(*(this->fNll));
      minimizer.setPrintLevel(printLevel);
      minimizer.setEps(tolerance);
  
      // measure time needed to run minimizer, HESSE, and MINOS
      TStopwatch sw;
  
      Bool_t done = false;
      Int_t maxStrat = 2;
      for (; strat <= maxStrat && !done; ++strat) {
  
        // set strategy for minimization
        minimizer.setStrategy(strat);
  
        // perform the fit
        sw.Start(true);
        DEBUG("starting native minimizer");
        ndim = TSUtils::nParameters(minimizer);
        if(minimize && ndim>0){
          status = minimizer.minimize(minType.Data(), minAlgo.Data());
        } else {
          status = 0;
        }
  
        sw.Stop();
        tMini += sw.RealTime();
  
        // check status of fit an repeat trying different strategy if it failed
        if (status < 0) {
          TString msg = TString::Format("minimizeNll(...): "
                                        "Fit using strategy %d failed with status %d", strat, status);
          if (strat < maxStrat) {
            msg.Append(TString::Format(". Will retry with strategy %d ...", strat + 1));
            statwarn(msg);
          } else {
            msg.Append(". Stopping!");
            staterror(msg);
          }
          continue;
        } else {
          statinfo(TString::Format("minimizeNll(...): Fit using strategy %d converged with status %d", strat, status));
        }
        // run HESSE if requested
        if (runHesse) {
          sw.Start(true);
          statinfo("minimizeNll(...): running hesse after minimization (this might take a while) ...");
          int hesseStatus = minimizer.hesse();
          statinfo(TString::Format("minimizeNll(...): hesse finished with status %d",hesseStatus));          
          sw.Stop();
          tHesse = sw.RealTime();
        }
        
        // run MINOS if requested
        if (runMinos) {
          statinfo("minimizeNll(...): running minos after minimization (this might take a while) ...");  
          TString runMinosVars;
          RooArgSet * runMinosArgset = NULL;
  
          std::vector<TString> runMinos;
          if(options) runMinos = options->getTagVString("runMinosVars");
          runMinosArgset = this->makeMinosArgSet(this->fWorkspace,runMinos);
  
          if (runMinosArgset) {
            sw.Start(true);
            minimizer.minos(*runMinosArgset);
            sw.Stop();
            tMinos = sw.RealTime();
            delete runMinosArgset;
          } else {
            statwarn("minimizeNll(...): MINOS variables not "
                     "found or set constant. Skipping MINOS ...");
          }
        }
  
        done = true;
      }
  
      // fit results
      fitResult = minimizer.save();
    } catch (std::string& s){
      throw std::runtime_error(s);
    }
  }

  TQLibrary::restore();

  // read the fit result

  TQFolder* result = NULL;

  TString saveSnapshot;
  if (options && options->getTagString("saveSnapshot", saveSnapshot)) {
    info(TString::Format("saving fit snapshot as '%s'",saveSnapshot.Data()));
    fWorkspace->saveSnapshot(saveSnapshot.Data(), fWorkspace->allVars());
  }

  if(useRooFitUtils){
#ifdef HAS_RooFitUtils
    result = ::convertFitResults(extResult,options);
#else
    throw std::runtime_error("RooFitUtils not available!");
#endif
  } else {
    if(fitResult) result = TSUtils::convertFitResults(fitResult,options,NULL);
  }
  
  TString resultname;
  if(options->getTagString("resultName",resultname)){
    result->SetName(resultname);
  }
  result->setTagInteger("status",status);

  
  // finalize the result
  if(result){
    result->setTagDouble("strategy",strat);
    result->setTagInteger("nDim", ndim);
    if(!useRooFitUtils){
      // time needed to perform the fit
      Double_t tTotal = tMini + tHesse + tMinos;
      if (tTotal > 0.) {
        result->setTagDouble("time.total", tTotal);
      }
      if (tMini > 0.) {
        result->setTagDouble("time.MINIMIZE", tMini);
      }
      if (tHesse > 0.) {
        result->setTagDouble("time.HESSE", tHesse);
      }
      if (tMinos > 0.) {
        result->setTagDouble("time.MINOS", tMinos);
      }
    } 
  } else {
    result = new TQFolder("error");
    result->setTagInteger("status",status);
    result->setTagString("error","no fit result!");
  }

#ifdef HAS_RooFitUtils
  if(extResult) delete extResult;
#endif
  
  if(fitResult) delete fitResult;
  return result;
}

//__________________________________________________________________________________|___________

RooArgSet* TSStatisticsCalculator::makeMinosArgSet(RooWorkspace* ws, const std::vector<TString>& filters){
  // collect all variables from a workspace and make them into a RooArgSet suitable to pass to Minos according to the filters given
  RooArgSet allVars(ws->allVars());
  TQIterator itr(allVars.createIterator());
  RooArgSet* runMinosArgSet = NULL;
  while (itr.hasNext()) {
    TObject * obj = itr.readNext();
    if (!obj->InheritsFrom(RooAbsReal::Class())) {
      continue;
    }
    RooAbsReal * var = (RooAbsReal*)obj;

    bool ok = (filters.size()==0);
    for(auto filter:filters){
      if (TQStringUtils::matches(var->GetName(), filter)){
        ok=true;
      }
    }
    if(!ok) continue;
    if (var->isConstant()) {
      // statwarn because constant variable passed explicit filter
      //      statwarn(TString::Format("minimizeNll(...): Variable '%s' set constant. "
      //                           "Not considering for MINOS ...", var->GetName()));
      //
      continue;
    }

    // add variable to list of MINOS vars
    if (!runMinosArgSet) {
      runMinosArgSet = new RooArgSet();
    }
    runMinosArgSet->add(*var);
  }
  return runMinosArgSet;
}

//__________________________________________________________________________________|___________

void TSStatisticsCalculator::setParameterValues(TQTaggable* options, const TString& prefix){
  // the parameters in the workspace to the values given by the tags
  RooArgList allVars(fWorkspace->allVars());
  TSUtils::setParameterValues(&allVars,options,prefix,false);
}


void TSStatisticsCalculator::setParameterErrors(TQTaggable* options, const TString& prefix){
  // the parameters in the workspace to the values given by the tags
  RooArgList allVars(fWorkspace->allVars());
  TSUtils::setParameterErrors(&allVars,options,prefix,false);
}


//__________________________________________________________________________________|___________

TQFolder * TSStatisticsCalculator::fitPdfToData(RooAbsPdf * pdf, RooDataSet * data, const RooArgSet * constrain, const TString& options) {
  // fit a PDF to a dataset, given constraints and options
  DEBUGclass("entering function with pdf,data,constrain,options signature");

  TQTaggable tags(options);
  return fitPdfToData(pdf, data, constrain, &tags);
}

//__________________________________________________________________________________|___________

void TSStatisticsCalculator::setParametersConstFloat(const RooArgSet& params, TQTaggable * options) {
  // set parameters to constant or floating depending on the options given
  setParametersConstFloat(&params,options);
}

//__________________________________________________________________________________|___________

void TSStatisticsCalculator::setParametersConstFloat(const RooArgSet * params, TQTaggable * options) {
  // set parameters to constant or floating depending on the options given
  if(!params || params->getSize() < 1){
    error("empty parameter list given!");
    return;
  }
  if(!options){
    error("empty options given!");
    return;
  }
  int verbose = options->getTagIntegerDefault("verbose",0);
  bool setmode = options->getTagIntegerDefault("setMode",false);
  std::vector<TString> constPars   = options->getTagVString("constPars");
  std::vector<TString> constExcept = options->getTagVString("constPars.except");
  std::vector<TString> floatPars   = options->getTagVString("floatPars");
  std::vector<TString> floatExcept = options->getTagVString("floatPars.except");
  TQIterator itr(params->createIterator(), true);
  while (itr.hasNext()) {
    RooRealVar * var = dynamic_cast<RooRealVar*>(itr.readNext());
    if(!var) continue;
    bool setConst = false;
    for(const auto& p:constPars){
      if(TQStringUtils::matches(var->GetName(), p)){
        setConst = true;
        for(const auto& p2:constExcept){
          if(TQStringUtils::matches(var->GetName(), p2)) setConst = false;
        }
      }
    }
    bool setFloat = false;
    for(const auto& p:floatPars){
      if(TQStringUtils::matches(var->GetName(), p)){
        setFloat = true;
          for(const auto& p2:floatExcept){
            if(TQStringUtils::matches(var->GetName(), p2)) setFloat = false;
          }
      }
    }
    if(setConst && !setFloat){
      if (verbose>1) info(TString::Format("setting parameter '%s' to constant",var->GetName()));
      var->setConstant(true);
    }
    if(setFloat && !setConst){
      if (verbose>1) info(TString::Format("setting parameter '%s' to floating",var->GetName()));
      var->setConstant(false);
      if(setmode) var->setOperMode(RooAbsArg::ADirty,true);
    }
    if(setFloat && setConst){
      warn(TString::Format("refusing to change const property of parameter '%s' matching both constPars='%s' and floatPars='%s'",var->GetName(),TQStringUtils::concat(constPars,",").Data(),TQStringUtils::concat(floatPars,",").Data()));
    }
  }
}

//__________________________________________________________________________________|___________

TQFolder * TSStatisticsCalculator::fitPdfToData(RooAbsPdf * pdf, RooDataSet * data, const RooArgSet * constrain, TQTaggable * options) {
  // fit a PDF to a dataset, given constraints and options
  DEBUGclass("entering function with pdf,data,constrain,options signature");
  if(!options){
    staterror("invalid options passed to fitting function!");
    return NULL;
  }

  bool useFitTo = options->getTagBoolDefault("useFitTo",false);

  TQFolder* result = NULL;
  
  if(useFitTo){
    RooFitResult* rfr = pdf->fitTo(*data,RooFit::Save(true));
    result = TSUtils::convertFitResults(rfr);
    delete rfr;
  } else {
    DEBUGclass("performing setup");
    this->setup(pdf,data,constrain,options);
    DEBUGclass("starting minimize");
    result = this->minimizeNll(options);
  }
  
  // return fit results
  return result;
}

//__________________________________________________________________________________|___________

void TSStatisticsCalculator::clear(){
  // delete all cached and temporary objects
  if(this->fNll){
    delete this->fNll;
    this->fNll = NULL;
  }
}

//__________________________________________________________________________________|___________

void TSStatisticsCalculator::setup(RooAbsPdf * pdf, RooDataSet * data, const RooArgSet * constrain, TQTaggable * options) {
  // setup function to initialize the minimizer
  DEBUGclass("entering function");

  // load nominal snapshot
  bool loadsnapshot = options->getTagBoolDefault("loadSnapshot",true);
  TString snapshot = options->getTagStringDefault("snapshot","SnSh_AllVars_Nominal");
  if(loadsnapshot){
    if(!fWorkspace->loadSnapshot(snapshot)){
      throw std::runtime_error(TString::Format("unable to load snapshot '%s'",snapshot.Data()).Data());
    }
  }

  bool useRooFitUtils = options->getTagBoolDefault("useRooFitUtils",ROOFITUTILS);
  
  DEBUGclass("preparing for nll creation");
  this->setParameterValues(options,"presetParam");
  this->setParameterErrors(options,"presetParamError");
  this->setParametersConstFloat(fWorkspace->allVars(),options);

  TString outfile;
  bool redirect=options->getTag("logToFile",outfile);
  if(redirect){
    TString fname = options->replaceInText(outfile);
    fname = TQPathManager::getPathManager()->getTargetPath(fname);
    TList* l = TQUtils::execute("rm -f "+outfile);
    delete l;
    statinfo(TString::Format("writing log of Nll creation to '%s'",fname.Data()));
    TQUtils::ensureDirectoryForFile(fname);
    TQLibrary::redirect(fname,false);
  }

  if(loadsnapshot){
    info(TString::Format("loaded snapshot '%s' for Nll construction, %d parameters floating",snapshot.Data(),TSUtils::countFloatParameters(fWorkspace->allVars())));
  } else {
    info(TString::Format("using current workspace configruation for Nll construction, %d parameters floating",TSUtils::countFloatParameters(fWorkspace->allVars())));
  }

  bool reuse = options->getTagBoolDefault("reuseNll",false);
  if(this->fNll && !reuse){
    this->clear();
  }

  TString writePreWS;
  if(options->getTagString("writeWorkspacePreNll",writePreWS)){
    this->fWorkspace->writeToFile(TQPathManager::getPathManager()->getTargetPath(writePreWS).c_str(),true);
  }


  if(!useRooFitUtils){  
    // this part uses the local implementation of the minimization
    this->fNll = createNll(this->fWorkspace,pdf, data, constrain, options);
  } else {
#ifndef HAS_RooFitUtils
    throw std::runtime_error("RooFitUtils not available!");
#else
    // this part uses the ExtendedMinimizer from RooFitUtils
    this->SetPdf(pdf);
    this->SetData(data);
    RooLinkedList l;
    std::vector<RooCmdArg> owner;
    buildArgs(options,owner,l,fModelConfig);
    //    for(auto it:owner){
    //      std::cout << it.GetName() << " " << (it.getString(0)?it.getString(0):"") << " " << it.getInt(0) << std::endl;
    //    }
    
    if(!reuse) RooFitUtils::ExtendedMinimizer::parseNllConfig(l);
    RooFitUtils::ExtendedMinimizer::parseFitConfig(l);
    RooFitUtils::ExtendedMinimizer::setup();
    RooFitUtils::ExtendedMinimizer::initialize();
    
#endif
  }

  if(!fNll){
    if(redirect) TQLibrary::restore();
    throw std::runtime_error("construction of Nll failed!");
  }

  if(loadsnapshot){
    fWorkspace->loadSnapshot(snapshot);
  }
  DEBUGclass("preparing nll");
  this->setParameterValues(options,"presetParam");
  this->setParameterErrors(options,"presetParamError");
  this->setParametersConstFloat(fWorkspace->allVars(),options);

  RooArgSet* allVars = fNll->getVariables();

  TSUtils::setParameterValues(allVars,fWorkspace->allVars());
  TSUtils::setParametersConstFloat(allVars,fWorkspace->allVars());
  int npars = TSUtils::countFloatParameters(allVars);

  delete allVars;

//  RooMsgService::instance().saveState();
//  RooMsgService::instance().setGlobalKillBelow(RooFit::DEBUG);
//  RooMsgService::instance().addStream(RooFit::DEBUG,RooFit::Topic(RooFit::Eval));
  double val = fNll->getVal();
//  RooMsgService::instance().restoreState();
  this->resetOffset();

  if(!TQUtils::isNum(val)){
    if(redirect) TQLibrary::restore();
    throw std::runtime_error(TString::Format("construction of Nll failed, initial value is %g",val).Data());
  }

  info(TString::Format("constructed Nll with %d floating parameters and starting value %g",npars,val));

  if(redirect) TQLibrary::restore();

  TString writePostWS;
  if(options->getTagString("writeWorkspacePostNll",writePostWS)){
    this->fWorkspace->writeToFile(TQPathManager::getPathManager()->getTargetPath(writePostWS).c_str(),true);
  }

}

//__________________________________________________________________________________|___________

bool TSStatisticsCalculator::loadSnapshot(TQTaggable * options) {
  // load a snapshot according to the options - if it doesn't exist, create it
  TString datasetName = options->getTagStringDefault("datasetName", "obsData");
  TString snapshot = options->getTagStringDefault("snapshot",TString::Format("SnSh_AllVars_Unconditional_%s", datasetName.Data()));
  TQFolder* retval = NULL;
  if(fSnapshots){
    retval = fSnapshots->getFolder(snapshot);
  }
  bool ok = fWorkspace->loadSnapshot(snapshot.Data());
  if(!ok){
    TString snapshotName = options->getTagStringDefault("snapshot.nominal","SnSh_AllVars_Nominal");
    info(TString::Format("no snapshot named '%s' found, generating based on '%s' ...",snapshot.Data(),snapshotName.Data()));

//    std::map<RooRealVar*, bool> poisconst;
//    RooArgSet *	listOfPOIs	= this->getPOIs(options);
//    TQIterator itr(listOfPOIs->createIterator(),true);
//    while (itr.hasNext()) {
//      RooRealVar * poi = dynamic_cast<RooRealVar*>(itr.readNext());
//      if (poi) {
//        poisconst[poi]=poi->isConstant();
//      }
//    }
//    delete listOfPOIs;

    TQTaggable fitOptions(options);
    fitOptions.setTagString("snapshot",snapshotName);
    retval = fitPdfToData(datasetName, &fitOptions);

    if(retval){
      int status = retval->getTagIntegerDefault("status",-1);
      if(status == 0 || status == 1){
//        for(auto it:poisconst){
//          it.first->setConstant(it.second);
//        }
        fWorkspace->saveSnapshot(snapshot.Data(), fWorkspace->allVars());
        info(TString::Format("saved snapshot '%s', %d parameters floating",snapshot.Data(),TSUtils::countFloatParameters(fWorkspace->allVars())));
        ok = true;
        retval->SetName(snapshot.Data());
        if(fSnapshots){
          fSnapshots->addFolder(retval);
        }
        return true;
      } else {
        error(TString::Format("fit for creation of snapshot '%s' failed with status %d",snapshot.Data(),status));
        return false;
      }
    } else {
      error(TString::Format("fit for creation of snapshot '%s' failed",snapshot.Data()));
      return false;
    }
  }

  return ok;
}

//__________________________________________________________________________________|___________

void TSStatisticsCalculator::unfoldConstraints(RooArgSet &initial, RooArgSet &final,
                                               RooArgSet &obs, RooArgSet &nuis, Int_t &counter) {

  if (counter > 100) {
    throw std::runtime_error("unfoldConstraints(...): Failed to unfold constraints");
  }

  TIterator* itr = initial.createIterator();
  RooAbsPdf * pdf;
  while ((pdf = (RooAbsPdf*)itr->Next())) {
    RooArgSet nuis_tmp = nuis;
    RooArgSet constraint_set(*pdf->getAllConstraints(obs, nuis_tmp, false));
    std::string className(pdf->ClassName());
    if (className != "RooGaussian" && className != "RooLognormal" && className != "RooGamma" && className != "RooPoisson" && className != "RooBifurGauss") {
      counter++;
      unfoldConstraints(constraint_set, final, obs, nuis, counter);
    } else {
      final.add(*pdf);
    }
  }
  delete itr;
}

//__________________________________________________________________________________|___________

RooStats::ModelConfig* TSStatisticsCalculator::getModelConfig(){
  // obtain the modelconfig
  return this->fModelConfig;
}

//__________________________________________________________________________________|___________

const RooArgSet* TSStatisticsCalculator::getNuisanceParameters(){
  // obtain the list of nuisance parameters
  return this->fModelConfig ? this->fModelConfig->GetNuisanceParameters() : NULL;
}

//__________________________________________________________________________________|___________

const RooArgSet* TSStatisticsCalculator::getPOIs(){
	// obtain the list of parameters of interest
	return this->fModelConfig ? this->fModelConfig->GetParametersOfInterest() : NULL;
}

//__________________________________________________________________________________|___________

void TSStatisticsCalculator::resetOffset(){
  // reset the likelihood offset to the current coordinates
  if(!this->fNll){
    throw std::runtime_error("unable to reset offset without Nll!");
  }
  if(this->fNll->isOffsetting()){
    this->fNll->enableOffsetting(false);
    this->fNll->enableOffsetting(true);
    this->fNll->getVal();
  }
}

//__________________________________________________________________________________|___________

RooAbsPdf* TSStatisticsCalculator::getPdf(){
  // return the internal Pdf
  if(!this->fModelConfig) return NULL;
  return this->fModelConfig->GetPdf();
}

//__________________________________________________________________________________|___________

RooSimultaneous* TSStatisticsCalculator::getSimPdf(){
  // retrieve the internal Pdf as a RooSimultaneous, returns NULL for any other Pdf type
  return dynamic_cast<RooSimultaneous*>(this->getPdf());
}

//__________________________________________________________________________________|___________

RooCategory* TSStatisticsCalculator::getIndexCategory(){
  // retrieve the index category of the simPdf, returns NULL for any other Pdf type
  RooSimultaneous* simPdf = this->getSimPdf();
  if(!simPdf) return NULL;
  return const_cast<RooCategory*>(static_cast<const RooCategory*>(&(simPdf->indexCat())));
}
