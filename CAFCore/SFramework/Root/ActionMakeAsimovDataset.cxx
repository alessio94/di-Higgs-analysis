#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQHistogramUtils.h"

#include "SFramework/TSUtils.h"
#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSStatisticsCalculator.h"
#include "SFramework/RooFitUtils.h"

#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooMinimizer.h"
#include "TMath.h"
#include "Math/MinimizerOptions.h"
#include "RooStats/AsymptoticCalculator.h"

//#ifdef ASG_BUILD
//#include <RootCore/Packages.h>
//#ifdef HAS_RooFitUtils
//#include <RooFitUtils/EditWorkspaces.h>
//#endif
//#endif

#ifdef HAS_RooFitUtils
#include <RooFitUtils/EditWorkspaces.h>
#endif

#include "TFile.h"

#include <iomanip>

/*<cafdoc name=MakeAsimovDataset>
  MakeAsimovDataset
  ===========================
  
  Generate an Asimov dataset based on some snapshot.

  Usage:
  ---------------
  ```
  +MakeAsimovDataset {
    +HWWRun2GGF {
      <name="asimovData">
      <snapshot="SnSh_AllVars_Nominal>
      <mode="minimal">
    }
  }
  ```

  The default name for asimov datasets is "asimovData_1" and can be
  changed with the `name` tag.

  The default snapshot to be used is "SnSh_AllVars_Nominal" and can be
  changed with the `snapshot` tag. Additional changes to the
  parameters can be made with tags of the type `setPars.a=x`.

  This action supports several "modes" of Asimov dataset generation,
  that correspond implementations adopted from several other
  statistics frameworks.
  * "minimal": a minimal custom implementation (recommended)
  * "RooStats": the RooStats::AsymptoticsCalculator implementation
  * "legacy": the default, legacy method adopted from Aarons statistics framework
  * "valerio": a method adopted from Valerio Daos statistics framework

</cafdoc> */


namespace AsimovGenerator {

  void statinfo(TString message) {
    
    std::cout << "SFramework/AsimovGenerator: " << message.Data() << std::endl;
  }
  
  
  //__________________________________________________________________________________|___________
  
  void staterror(TString message) {
    
    statinfo(TQStringUtils::makeBoldRed(TString("ERROR: ") + message));
  }
  
  
  //__________________________________________________________________________________|___________
  
  void statwarn(TString message) {
    
    statinfo(TQStringUtils::makeBoldYellow(TString("WARNING: ") + message));
  }

  //__________________________________________________________________________________|___________


  std::map<RooRealVar*,RooRealVar*> getNuisAndGlobs(RooStats::ModelConfig* fModelConfig, RooAbsPdf* combPdf){
    
    std::map<RooRealVar*,RooRealVar*> retval;
    RooArgSet mc_obs(*fModelConfig->GetObservables());
    RooArgSet mc_globs(*fModelConfig->GetGlobalObservables());
    RooArgSet mc_nuis(*fModelConfig->GetNuisanceParameters());

    //pair the nuisance parameter to the global observable
    RooArgSet mc_nuis_tmp = mc_nuis;
    
    RooArgSet constraint_set_tmp(*combPdf->getAllConstraints(mc_obs, mc_nuis_tmp, false));
    RooArgSet constraint_set;
    Int_t counter_tmp = 0;
    TSStatisticsCalculator::unfoldConstraints(constraint_set_tmp, constraint_set, mc_obs, mc_nuis_tmp, counter_tmp);

    TIterator * cIter = constraint_set.createIterator();
    RooAbsArg * arg;
    while ((arg = (RooAbsArg*)cIter->Next())) {
      RooAbsPdf * pdf = (RooAbsPdf*)arg;

      TIterator * nIter = mc_nuis.createIterator();
      RooRealVar * thisNui = NULL;
      RooAbsArg * nui_arg;
      while ((nui_arg = (RooAbsArg*)nIter->Next()) && !thisNui) {
        if (pdf->dependsOn(*nui_arg)) {
          thisNui = (RooRealVar*)nui_arg;
        }
      }
      delete nIter;

      // need this incase the observable isn't fundamental.
      // in this case, see which variable is dependent on the nuisance parameter and use that.
      RooArgSet * components = pdf->getComponents();
      components->remove(*pdf);
      if (components->getSize()) {
        TIterator* itr1 = components->createIterator();
        RooAbsArg* arg1;
        while ((arg1 = (RooAbsArg*)itr1->Next())) {
          TIterator* itr2 = components->createIterator();
          RooAbsArg* arg2;
          while ((arg2 = (RooAbsArg*)itr2->Next())) {
            if (arg1 == arg2) {
              continue;
            }
            if (arg2->dependsOn(*arg1)) {
              components->remove(*arg1);
            }
          }
          delete itr2;
        }
        delete itr1;
      }

      if (components->getSize() > 1) {
        throw std::runtime_error("makeAsimovData(...): Couldn't isolate proper nuisance parameter");
      } else if (components->getSize() == 1) {
        thisNui = (RooRealVar*)components->first();
      }

    
      TIterator* gIter = mc_globs.createIterator();
      RooRealVar* thisGlob = NULL;
      RooAbsArg* glob_arg;
      while ((glob_arg = (RooAbsArg*)gIter->Next()) && !thisGlob) {
        if (pdf->dependsOn(*glob_arg)) {
          thisGlob = (RooRealVar*)glob_arg;
        }
      }
      delete gIter;
		
      if (!thisNui || !thisGlob) {
        AsimovGenerator::statwarn(TString::Format("makeAsimovData(...): Couldn't find nui or glob for constraint '%s'", pdf->GetName()));
        AsimovGenerator::statwarn(TString::Format("makeAsimovData(...): nui: '%d', glob: '%d'", thisNui?1:0, thisGlob?1:0));
        continue;
      }
		
      //statinfo(TString::Format("makeAsimovData(...): Pairing nui '%s' with glob '%s' from constraint '%s'",
      //	thisNui->GetName(), thisGlob->GetName(), pdf->GetName()));

      retval[thisNui] = thisGlob;
    }
    delete cIter;
    return retval;
  }
  
  RooAbsData* makeAsimovDataMinimal(RooWorkspace* fWorkspace, RooStats::ModelConfig* fModelConfig) {
    
    //const double inf = pow(10., 18);
    
    //    std::map<RooRealVar*,RooRealVar*> nuisAndGlobs = getNuisAndGlobs(fModelConfig,combPdf);
    //
    //		// save the snapshots of nominal parameters
    //		fWorkspace->saveSnapshot("nominalGlobs", *fModelConfig->GetGlobalObservables());
    //		fWorkspace->saveSnapshot("nominalNuis", *fModelConfig->GetNuisanceParameters());
    //	
    //		TSUtils::setParametersConstant(fModelConfig->GetParametersOfInterest(),false);
    //        
    //		for (auto it:nuisAndGlobs){
    //			RooRealVar* nui = it.first;
    //			RooRealVar* glob = it.second;
    //			glob->setVal(nui->getVal());
    //		}
    //	
    //		// save the snapshots of conditional parameters
    //		fWorkspace->saveSnapshot("conditionalGlobs", *fModelConfig->GetGlobalObservables());
    //		fWorkspace->saveSnapshot("conditionalNuis" , *fModelConfig->GetNuisanceParameters());

    RooArgSet observables(*fModelConfig->GetObservables());
    
    RooArgSet allVars;
    allVars.add(observables);
    RooSimultaneous* simPdf = dynamic_cast<RooSimultaneous*>(fModelConfig->GetPdf());
    RooCategory * channelCat = (RooCategory*)&simPdf->indexCat();
    allVars.add(*channelCat);
    const char * weightName = "weightVar";
    RooRealVar * weightVar = fWorkspace->var(weightName);
    if (!weightVar) {
      RooRealVar * weightVar_tmp = new RooRealVar(weightName, weightName, 1, 0, 10000000);
      fWorkspace->import(*weightVar_tmp);
      weightVar = fWorkspace->var(weightName);
    }
    allVars.add(*weightVar);

    // try fix for sim pdf
    TIterator * iter = channelCat->typeIterator();
    RooCatType * tt = NULL;
    std::map<std::string, RooDataSet*> asimovDataMap;
    while ((tt = (RooCatType*)iter->Next())) {
      std::string label(tt->GetName());
      channelCat->setLabel(label.c_str());
      
      // Get pdf associated with state from simpdf
      RooAbsPdf* pdf = simPdf->getPdf(label.c_str());
      
      RooDataSet* data = TSUtils::getDataHistogram(pdf,&observables,weightVar,channelCat);
      
      if (TMath::IsNaN(data->sumEntries())) {
        throw std::runtime_error("makeAsimovData(...): Sum entries is NaN");
      }
      
      asimovDataMap[label] = data;
    }
    
    RooDataSet* asimovData = new RooDataSet("asimovData","AsimovData",allVars,RooFit::Index(*channelCat),RooFit::Import(asimovDataMap),RooFit::WeightVar(*weightVar));
    
    return asimovData;
  }
  
  
  RooAbsData* makeAsimovDataDefault(RooWorkspace* fWorkspace, RooStats::ModelConfig* fModelConfig, bool doConditional, RooAbsPdf * combPdf,
                                    RooDataSet * combData, Int_t profileMode) {
    Double_t muVal;
    if (profileMode == 0) {
      muVal = 0;
    } else if (profileMode == 1) {
      muVal = 1;
    } else {
      muVal = 1;
    }

    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
    ROOT::Math::MinimizerOptions::SetDefaultStrategy(1);
    ROOT::Math::MinimizerOptions::SetDefaultPrintLevel(1);

    std::stringstream muStr;
    if (profileMode == 0 || profileMode == 1) {
      muStr << "_" << muVal;
    } else {
      muStr << "_muhat";
    }

    TSUtils::setParameterValues(fModelConfig->GetParametersOfInterest(),muVal);

    RooArgSet mc_obs	= *fModelConfig->GetObservables();
    RooArgSet mc_globs	= *fModelConfig->GetGlobalObservables();
    RooArgSet mc_nuis	= *fModelConfig->GetNuisanceParameters();

    //pair the nuisance parameter to the global observable
    RooArgSet mc_nuis_tmp = mc_nuis;
    RooArgList nui_list("ordered_nuis");
    RooArgList glob_list("ordered_globs");
    RooArgSet constraint_set_tmp(*combPdf->getAllConstraints(mc_obs, mc_nuis_tmp, false));
    RooArgSet constraint_set;
    Int_t counter_tmp = 0;
    TSStatisticsCalculator::unfoldConstraints(constraint_set_tmp, constraint_set, mc_obs, mc_nuis_tmp, counter_tmp);

    TIterator * cIter = constraint_set.createIterator();
    RooAbsArg * arg;
    while ((arg = (RooAbsArg*)cIter->Next())) {
      RooAbsPdf * pdf = (RooAbsPdf*)arg;

      TIterator * nIter = mc_nuis.createIterator();
      RooRealVar * thisNui = NULL;
      RooAbsArg * nui_arg;
      while ((nui_arg = (RooAbsArg*)nIter->Next()) && !thisNui) {
        if (pdf->dependsOn(*nui_arg)) {
          thisNui = (RooRealVar*)nui_arg;
        }
      }
      delete nIter;

      // need this incase the observable isn't fundamental.
      // in this case, see which variable is dependent on the nuisance parameter and use that.
      RooArgSet * components = pdf->getComponents();
      components->remove(*pdf);
      if (components->getSize()) {
        TIterator* itr1 = components->createIterator();
        RooAbsArg* arg1;
        while ((arg1 = (RooAbsArg*)itr1->Next())) {
          TIterator* itr2 = components->createIterator();
          RooAbsArg* arg2;
          while ((arg2 = (RooAbsArg*)itr2->Next())) {
            if (arg1 == arg2) {
              continue;
            }
            if (arg2->dependsOn(*arg1)) {
              components->remove(*arg1);
            }
          }
          delete itr2;
        }
        delete itr1;
      }

      if (components->getSize() > 1) {
        AsimovGenerator::staterror("makeAsimovData(...): Couldn't isolate proper nuisance parameter");
        return NULL;
      } else if (components->getSize() == 1) {
        thisNui = (RooRealVar*)components->first();
      }

    
      TIterator* gIter = mc_globs.createIterator();
      RooRealVar* thisGlob = NULL;
      RooAbsArg* glob_arg;
      while ((glob_arg = (RooAbsArg*)gIter->Next()) && !thisGlob) {
        if (pdf->dependsOn(*glob_arg)) {
          thisGlob = (RooRealVar*)glob_arg;
        }
      }
      delete gIter;
		
      if (!thisNui || !thisGlob) {
        AsimovGenerator::statwarn(TString::Format("makeAsimovData(...): Couldn't find nui or glob for constraint '%s'", pdf->GetName()));
        AsimovGenerator::statwarn(TString::Format("makeAsimovData(...): nui: '%s', glob: '%s'", thisNui?thisNui->GetName():"-", thisGlob?thisGlob->GetName():"-"));
        continue;
      }
		
      //statinfo(TString::Format("makeAsimovData(...): Pairing nui '%s' with glob '%s' from constraint '%s'",
      //	thisNui->GetName(), thisGlob->GetName(), pdf->GetName()));
		
      nui_list.add(*thisNui);
      glob_list.add(*thisGlob);
		
    }
    delete cIter;


    // save the snapshots of nominal parameters
    fWorkspace->saveSnapshot("nominalGlobs", *fModelConfig->GetGlobalObservables());
    fWorkspace->saveSnapshot("nominalNuis", *fModelConfig->GetNuisanceParameters());
	
    RooArgSet nuiSet_tmp(nui_list);

    TSUtils::setParameterValues(fModelConfig->GetParametersOfInterest(),muVal);
    TSUtils::setParametersConstant(fModelConfig->GetParametersOfInterest(),profileMode == 0 || profileMode == 1);
	
    Int_t status = 0;
    if (doConditional) {
      RooAbsReal* nll = combPdf->createNLL(*combData, RooFit::Constrain(nuiSet_tmp));
      RooMinimizer minim(*nll);
      minim.setStrategy(2);
      minim.setPrintLevel(1);
      status = minim.minimize(ROOT::Math::MinimizerOptions::DefaultMinimizerType().c_str(), ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo().c_str());
      if (status != 0) {
        std::cout << "Fit failed with status " << status << std::endl;
      }
      //combPdf->fitTo(*combData,Hesse(false),Hesse(false),PrintLevel(0),Extended(), Constrain(nuiSet_tmp));
    }
    TSUtils::setParametersConstant(fModelConfig->GetParametersOfInterest(),false);
        
    // loop over the nui/glob list, grab the corresponding variable from the tmp ws, and set the glob to the value of the nui
    Int_t nrNuis = nui_list.getSize();
    if (nrNuis != glob_list.getSize()) {
      AsimovGenerator::staterror("nui_list.getSize() != glob_list.getSize()!");
      return NULL;
    }
	
    for (Int_t i = 0; i < nrNuis; i++) {
      RooRealVar* nui = (RooRealVar*)nui_list.at(i);
      RooRealVar* glob = (RooRealVar*)glob_list.at(i);
      glob->setVal(nui->getVal());
    }
	
    // save the snapshots of conditional parameters
    fWorkspace->saveSnapshot(("conditionalGlobs" + muStr.str()).c_str(), *fModelConfig->GetGlobalObservables());
    fWorkspace->saveSnapshot(("conditionalNuis"  + muStr.str()).c_str(), *fModelConfig->GetNuisanceParameters());
	
    if (profileMode == 2) {
      RooArgSet nuisAndPOI(*fModelConfig->GetNuisanceParameters(),*fModelConfig->GetParametersOfInterest());
      fWorkspace->saveSnapshot("ucmles", nuisAndPOI);
    }
	
    if (!doConditional) {
      fWorkspace->loadSnapshot("nominalGlobs");
      fWorkspace->loadSnapshot("nominalNuis");
    }


    //fWorkspace->var("alpha_ATLAS_EL_EFF")->setVal(1.);
	
    // make the asimov data (snipped from Kyle)
    if (profileMode != 2) {
      TSUtils::setParameterValues(fModelConfig->GetParametersOfInterest(),muVal);
    }

    Int_t iFrame = 0;
	
    const char * weightName = "weightVar";
    RooArgSet obsAndWeight;
    obsAndWeight.add(*fModelConfig->GetObservables());

    RooRealVar * weightVar = fWorkspace->var(weightName);
    if (!weightVar) {
      fWorkspace->import(*(new RooRealVar(weightName, weightName, 1, 0, 10000000)));
      weightVar = fWorkspace->var(weightName);
    }
    obsAndWeight.add(*fWorkspace->var(weightName));
	
    fWorkspace->defineSet("obsAndWeight",obsAndWeight);


    // dummy var can just have one bin since it's a dummy
    if (fWorkspace->var("ATLAS_dummyX")) {
      fWorkspace->var("ATLAS_dummyX")->setBins(1);
    }
	
    // RooDataSet* simData=NULL;
    RooSimultaneous* simPdf = dynamic_cast<RooSimultaneous*>(fModelConfig->GetPdf());
	
    std::map<std::string, RooDataSet*> asimovDataMap;
	
    // try fix for sim pdf
    RooCategory * channelCat = (RooCategory*)&simPdf->indexCat();
    TIterator * iter = channelCat->typeIterator();
    RooCatType * tt = NULL;
    Int_t nrIndices = 0;
    while ((tt = (RooCatType*)iter->Next())) {
      nrIndices++;
    }

    for (Int_t i = 0; i < nrIndices; i++) {
      channelCat->setIndex(i);
      iFrame++;

      // Get pdf associated with state from simpdf
      RooAbsPdf* pdftmp = simPdf->getPdf(channelCat->getLabel());

      // Generate observables defined by the pdf associated with this state
      RooArgSet * obstmp = pdftmp->getObservables(*fModelConfig->GetObservables());

      RooDataSet * obsDataUnbinned = new RooDataSet(Form("combAsimovData%d", iFrame),
                                                    Form("combAsimovData%d", iFrame), RooArgSet(obsAndWeight, *channelCat), RooFit::WeightVar(*weightVar));

      RooRealVar* thisObs = ((RooRealVar*)obstmp->first());
      double expectedEvents = pdftmp->expectedEvents(*obstmp);
      double thisNorm = 0;
      for (Int_t jj = 0; jj < thisObs->numBins(); ++jj) {
        thisObs->setBin(jj);
        thisNorm = pdftmp->getVal(obstmp) * thisObs->getBinWidth(jj);
        if (thisNorm * expectedEvents > 0. && thisNorm * expectedEvents < pow(10., 18)) {
          obsDataUnbinned->add(*fModelConfig->GetObservables(), thisNorm * expectedEvents);
        }
      }
		
      if (TMath::IsNaN(obsDataUnbinned->sumEntries())) {
        AsimovGenerator::staterror("makeAsimovData(...): Sum entries is NaN");
        exit(1);
      }

      asimovDataMap[std::string(channelCat->getLabel())] = obsDataUnbinned;
    }

    RooDataSet* asimovData = new RooDataSet("asimovData","AsimovData",
                                            RooArgSet(obsAndWeight,*channelCat), RooFit::Index(*channelCat),RooFit::Import(asimovDataMap),RooFit::WeightVar(*weightVar));
	
    //bring us back to nominal for exporting
    fWorkspace->loadSnapshot("nominalNuis");
    fWorkspace->loadSnapshot("nominalGlobs");

    return asimovData;
  }

  using namespace std;
  using namespace RooFit;
  RooDataSet* makeAsimovDataValerio(RooWorkspace* w, RooStats::ModelConfig* mc, RooAbsData* data, double mu_val, bool fluctuateData){
    
    ////////////////////
    //make asimov data//
    ////////////////////
    RooAbsPdf* combPdf = mc->GetPdf();
    
    int _printLevel = 0;

    std::stringstream muStr;
    muStr << std::setprecision(5);
    muStr << "_" << mu_val;
    
    std::stringstream muStrProf;
    
    RooRealVar* mu = (RooRealVar*)mc->GetParametersOfInterest()->first();
    mu->setVal(mu_val);
    
    RooArgSet mc_obs = *mc->GetObservables();
    RooArgSet mc_globs = *mc->GetGlobalObservables();
    RooArgSet mc_nuis = *mc->GetNuisanceParameters();
    
    //pair the nuisance parameter to the global observable
    RooArgSet mc_nuis_tmp = mc_nuis;
    RooArgList nui_list("ordered_nuis");
    RooArgList glob_list("ordered_globs");
    RooArgSet constraint_set_tmp(*combPdf->getAllConstraints(mc_obs, mc_nuis_tmp, false));
    RooArgSet constraint_set;
    int counter_tmp = 0;
    TSStatisticsCalculator::unfoldConstraints(constraint_set_tmp, constraint_set, mc_obs, mc_nuis_tmp, counter_tmp);
    
    TIterator* cIter = constraint_set.createIterator();
    RooAbsArg* arg;
    while ((arg = (RooAbsArg*)cIter->Next())){
      RooAbsPdf* pdf = (RooAbsPdf*)arg;
      if (!pdf) continue;
      TIterator* nIter = mc_nuis.createIterator();
      RooRealVar* thisNui = NULL;
      RooAbsArg* nui_arg;
      while ((nui_arg = (RooAbsArg*)nIter->Next())){
        if (pdf->dependsOn(*nui_arg)){
          thisNui = (RooRealVar*)nui_arg;
          break;
        }
      }
      delete nIter;
      
      //need this in case the observable isn't fundamental. 
      //in this case, see which variable is dependent on the nuisance parameter and use that.
      RooArgSet* components = pdf->getComponents();
      components->remove(*pdf);
      if (components->getSize()){
        TIterator* itr1 = components->createIterator();
        RooAbsArg* arg1;
        while ((arg1 = (RooAbsArg*)itr1->Next())){
          TIterator* itr2 = components->createIterator();
          RooAbsArg* arg2;
          while ((arg2 = (RooAbsArg*)itr2->Next())){
            if (arg1 == arg2) continue;
            if (arg2->dependsOn(*arg1)){
              components->remove(*arg1);
            }
          }
          delete itr2;
        }
        delete itr1;
      }
      if (components->getSize() > 1){
        cout << "ERROR::Couldn't isolate proper nuisance parameter" << endl;
        return NULL;
      }
      else if (components->getSize() == 1){
        thisNui = (RooRealVar*)components->first();
      }
      
      TIterator* gIter = mc_globs.createIterator();
      RooRealVar* thisGlob = NULL;
      RooAbsArg* glob_arg;
      while ((glob_arg = (RooAbsArg*)gIter->Next())){
        if (pdf->dependsOn(*glob_arg)){
          thisGlob = (RooRealVar*)glob_arg;
          break;
        }
      }
      delete gIter;
      
      if (!thisNui || !thisGlob){
        cout << "WARNING::Couldn't find nui or glob for constraint: " << pdf->GetName() << endl;
        continue;
      }
      
      if (_printLevel >= 1) cout << "Pairing nui: " << thisNui->GetName() << ", with glob: " << thisGlob->GetName() << ", from constraint: " << pdf->GetName() << endl;
      
      nui_list.add(*thisNui);
      glob_list.add(*thisGlob);
      
    }
    delete cIter;
    
    //save the snapshots of nominal parameters, but only if they're not already saved
    w->saveSnapshot("tmpGlobs",*mc->GetGlobalObservables());
    w->saveSnapshot("tmpNuis",*mc->GetNuisanceParameters());
    if (!w->loadSnapshot("nominalGlobs")){
      cout << "nominalGlobs doesn't exist. Saving snapshot." << endl;
      w->saveSnapshot("nominalGlobs",*mc->GetGlobalObservables());
    }
    else w->loadSnapshot("tmpGlobs");
    if (!w->loadSnapshot("nominalNuis")){
      cout << "nominalNuis doesn't exist. Saving snapshot." << endl;
      w->saveSnapshot("nominalNuis",*mc->GetNuisanceParameters());
    }
    else w->loadSnapshot("tmpNuis");
    
    mu->setConstant(1);
    mu->setVal(mu_val);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// LAST THING FROM VALERIO
    RooSimultaneous *fitPdf = (RooSimultaneous*)(mc->GetPdf());
    RooArgSet* constrainedParams = fitPdf->getParameters(*data);
    Constrain(*constrainedParams);
    const RooArgSet* glbObs = mc->GetGlobalObservables();
    RooAbsReal * nll = fitPdf->createNLL(*data, 
                                         Constrain(*constrainedParams), 
                                         GlobalObservables(*glbObs),
                                         Offset(1), NumCPU(3, RooFit::Hybrid)
                                         );
    nll->getVal();
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
    int strat = ROOT::Math::MinimizerOptions::DefaultStrategy();
    strat=1;
    RooMinimizer minim(*nll);
    minim.setStrategy(strat);
    minim.setPrintLevel(1);
    minim.setEps(1);
    minim.minimize(ROOT::Math::MinimizerOptions::DefaultMinimizerType().c_str(),
                   ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo().c_str());
    /////// end of thing from valerio
    
    mu->setConstant(0);
    mu->setVal(mu_val);

    //loop over the nui/glob list, grab the corresponding variable from the tmp ws, and set the glob to the value of the nui
    int nrNuis = nui_list.getSize();
    if (nrNuis != glob_list.getSize()){
      cout << "ERROR::nui_list.getSize() != glob_list.getSize()!" << endl;
      return NULL;
    }
    
    for (int i=0;i<nrNuis;i++){
      RooRealVar* nui = (RooRealVar*)nui_list.at(i);
      RooRealVar* glob = (RooRealVar*)glob_list.at(i);
      
      //cout << "nui: " << nui << ", glob: " << glob << endl;
      //cout << "Setting glob: " << glob->GetName() << ", which had previous val: " << glob->getVal() << ", to conditional val: " << nui->getVal() << endl;
      
      glob->setVal(nui->getVal());
    }
    
    w->loadSnapshot("nominalGlobs");
    w->loadSnapshot("nominalNuis");
    
    if (_printLevel >= 1) cout << "Making asimov mu=" <<mu_val<< endl;
    
    //make the asimov data (snipped from Kyle)
    mu->setVal(mu_val);
    
    int iFrame=0;
    
    const char* weightName="weightVar";
    RooArgSet obsAndWeight;
    obsAndWeight.add(*mc->GetObservables());
    
    RooRealVar* weightVar = NULL;
    if (!(weightVar = w->var(weightName))){
      w->import(*(new RooRealVar(weightName, weightName, 1,0,10000000)));
      weightVar = w->var(weightName);
    }
    obsAndWeight.add(*w->var(weightName));
    w->defineSet("obsAndWeight",obsAndWeight);

    
    //////////////////////////////////////////////////////
    // MAKE ASIMOV DATA FOR OBSERVABLES
    //////////////////////////////////////////////////////
    
    RooSimultaneous* simPdf = dynamic_cast<RooSimultaneous*>(mc->GetPdf());
    
    RooDataSet* asimovData;
    if (!simPdf)
      {
        // Get pdf associated with state from simpdf
        RooAbsPdf* pdftmp = mc->GetPdf();//simPdf->getPdf(channelCat->getLabel()) ;
	
        // Generate observables defined by the pdf associated with this state
        RooArgSet* obstmp = pdftmp->getObservables(*mc->GetObservables()) ;
	
        if (_printLevel >= 1) obstmp->Print();
	
        if( fluctuateData ){
          cout<<"Creating extended datasample"<<endl;
          asimovData = pdftmp->generate(RooArgSet(obsAndWeight),Extended(kTRUE));
        }else{
          asimovData = new RooDataSet(("asimovData"+muStr.str()).c_str(),("asimovData"+muStr.str()).c_str(),RooArgSet(obsAndWeight),WeightVar(*weightVar));
	  
          RooRealVar* thisObs = ((RooRealVar*)obstmp->first());
          double expectedEvents = pdftmp->expectedEvents(*obstmp);
          double thisNorm = 0;
          for(int jj=0; jj<thisObs->numBins(); ++jj){
            thisObs->setBin(jj);
	    
            thisNorm=pdftmp->getVal(obstmp)*thisObs->getBinWidth(jj);
            if (thisNorm*expectedEvents <= 0)
              {
                cout << "WARNING::Detected bin with zero expected events (" << thisNorm*expectedEvents << ") ! Please check your inputs. Obs = " << thisObs->GetName() << ", bin = " << jj << endl;
              }
            if (thisNorm*expectedEvents > 0 && thisNorm*expectedEvents < pow(10.0, 18)) {
              asimovData->add(*mc->GetObservables(), thisNorm*expectedEvents); 
            }
          }
        }
        if (_printLevel >= 1){
          asimovData->Print();
          cout <<"sum entries "<<asimovData->sumEntries()<<endl;
        }
        if(asimovData->sumEntries()!=asimovData->sumEntries()){
          cout << "sum entries is nan"<<endl;
          exit(1);
        }
	
        if (_printLevel >= 1){
          asimovData->Print();
          cout << endl;
        }
      }
    
    else{
      map<string, RooDataSet*> asimovDataMap;
      
      //try fix for sim pdf
      RooCategory* channelCat = (RooCategory*)&simPdf->indexCat();
      TIterator* iter = channelCat->typeIterator() ;
      RooCatType* tt = NULL;
      int nrIndices = 0;
      while((tt=(RooCatType*) iter->Next())) {
        nrIndices++;
      }
      
      for (int i=0;i<nrIndices;i++){
        channelCat->setIndex(i);
        iFrame++;
        // Get pdf associated with state from simpdf
        RooAbsPdf* pdftmp = simPdf->getPdf(channelCat->getLabel()) ;
	
        // Generate observables defined by the pdf associated with this state
        RooArgSet* obstmp = pdftmp->getObservables(*mc->GetObservables()) ;
	
        if (_printLevel >= 1){
          obstmp->Print();
          cout << "on type " << channelCat->getLabel() << " " << iFrame << endl;
        }
	
        RooDataSet* obsDataUnbinned = new RooDataSet(Form("combAsimovData%d",iFrame),Form("combAsimovData%d",iFrame),RooArgSet(obsAndWeight,*channelCat),WeightVar(*weightVar));
        RooRealVar* thisObs = ((RooRealVar*)obstmp->first());
        double expectedEvents = pdftmp->expectedEvents(*obstmp);
        double thisNorm = 0;

        if( fluctuateData ){
          cout<<"Creating extended datasample"<<endl;
          obsDataUnbinned = pdftmp->generate(RooArgSet(obsAndWeight),Extended(kTRUE));
          obsDataUnbinned = pdftmp->generate(RooArgSet(obsAndWeight),Extended(kTRUE));
        }
        else{
          for(int jj=0; jj<thisObs->numBins(); ++jj){
            thisObs->setBin(jj);
	    
            thisNorm=pdftmp->getVal(obstmp)*thisObs->getBinWidth(jj);
            if (thisNorm*expectedEvents > 0 && thisNorm*expectedEvents < pow(10.0, 18)) obsDataUnbinned->add(*mc->GetObservables(), thisNorm*expectedEvents);
          }
        }
        if (_printLevel >= 1){
          obsDataUnbinned->Print();
          cout <<"sum entries "<<obsDataUnbinned->sumEntries()<<endl;
        }
        if(obsDataUnbinned->sumEntries()!=obsDataUnbinned->sumEntries()){
          cout << "sum entries is nan"<<endl;
          exit(1);
        }
	
        asimovDataMap[string(channelCat->getLabel())] = obsDataUnbinned;
	
        if (_printLevel >= 1){
          cout << "channel: " << channelCat->getLabel() << ", data: ";
          obsDataUnbinned->Print();
          cout << endl;
        }
      }
      
      asimovData = new RooDataSet(("asimovData"+muStr.str()).c_str(),
                                  ("asimovData"+muStr.str()).c_str(),
                                  RooArgSet(obsAndWeight,*channelCat),
                                  Index(*channelCat),
                                  Import(asimovDataMap),
                                  WeightVar(*weightVar));
    }
    
    //bring us back to nominal for exporting
    w->loadSnapshot("nominalGlobs");
    
    return asimovData;
  }  
}

namespace TSBaseActions {

  class MakeAsimovDataset : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {
      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("unable to make asimov dataset for '%s': no such workspace!",config->GetName()));
        return false;
      }
      TString snapshot = config->getTagStringDefault("snapshot","SnSh_AllVars_Nominal");
      if(!workspace->loadSnapshot(snapshot)){
        manager->error(TString::Format("no snapshot named '%s': available in workspace '%s'!",snapshot.Data(),workspace->GetName()));
        return false;        
      }
      
      TString name = config->getTagStringDefault("name","asimovData_1");
      if(workspace->data(name)){
        manager->error(TString::Format("workspace '%s' already includes dataset named '%s'",workspace->GetName(),name.Data()).Data());
        return false;
      }
      
      RooStats::ModelConfig* mc = (RooStats::ModelConfig*)(workspace->obj("ModelConfig"));

      TString dataname;
      RooAbsData* data = NULL;
      if(config->getTagString("data",dataname)){
        data = workspace->data(dataname);
        if(!data){
          manager->error(TString::Format("unable to load data '%s'!",dataname.Data()));
        }
      }

      RooArgSet allVars(workspace->allVars());
      TSUtils::setParameterValues(&allVars,config,"setPars",true);
			
      TString mode = config->getTagStringDefault("mode","minimal");
      RooAbsData* asimovData = NULL;
      if(mode == "default"){
        asimovData = AsimovGenerator::makeAsimovDataDefault(workspace,mc,false,mc->GetPdf(),NULL,1);
      } else if(mode == "valerio"){
        if(!data){
          manager->error(TString::Format("cannot use mode '%s' without initial data!",mode.Data()));
        } else {
          asimovData = AsimovGenerator::makeAsimovDataValerio(workspace,mc,data,1,false);
        }
      } else if(mode == "RooStats"){
        RooArgSet globs(*mc->GetGlobalObservables());
        RooArgSet allVars(workspace->allVars());
        asimovData = RooStats::AsymptoticCalculator::MakeAsimovData(*mc,allVars,globs);
      } else if(mode == "minimal"){
        asimovData = AsimovGenerator::makeAsimovDataMinimal(workspace,mc);
      }			
      if(asimovData){
        asimovData->SetName(name);
        workspace->import(*asimovData);
        manager->info(TString::Format("successfully generated asimov dataset '%s' in workspace '%s' with %g events with method '%s'!",name.Data(),workspace->GetName(),asimovData->sumEntries(),mode.Data()));
      } else {
        manager->error(TString::Format("failed to generate asimov dataset with method '%s'!",mode.Data()));
      }
      return asimovData;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new MakeAsimovDataset(),"MakeAsimovDataset");
  }
}
