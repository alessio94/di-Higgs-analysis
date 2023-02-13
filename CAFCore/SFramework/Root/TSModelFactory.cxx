#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <algorithm>

#include "SFramework/TSModelFactory.h"
#include "RooStats/HistFactory/PiecewiseInterpolation.h"
#include "RooStats/HistFactory/FlexibleInterpVar.h"
#include "RooStats/ModelConfig.h"
#include "RooRealVar.h"
#include "RooRealSumPdf.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQTaggable.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQPathManager.h"
#include "QFramework/TQUtils.h"
#include "TSystem.h"
#include "TMath.h"

#ifdef __cpp_generic_lambdas
#if __cpp_generic_lambdas >= 201304
#define HAS_GENERIC_LAMBDAS
#endif
#endif

#include "SFramework/TSIterator.h"

ClassImp(TSModelFactory)

#ifdef HAS_GENERIC_LAMBDAS    
namespace {
  namespace SFINAE {
    // SFINAE test
    // https://stackoverflow.com/questions/257288/is-it-possible-to-write-a-template-to-check-for-a-functions-existence
    
    class true_val    {char dummy[1];};
    class false_val   {char dummy[2];};

    template <typename T>
    class hasStackLabel {
    public:
      template <typename C> static true_val    test(decltype(&C::SetStackLabel));
      template <typename C> static false_val   test(...);    

      enum { value = sizeof(test<T>(0)) == sizeof(true_val) };
    };

    // static_if
    //https://stackoverflow.com/questions/37617677/implementing-a-compile-time-static-if-logic-for-different-string-types-in-a-co
    template <typename T, typename F>          auto static_if(std::true_type, T t, F /*f*/) { return t; }
    template <typename T, typename F>          auto static_if(std::false_type, T /*t*/, F f) { return f; }
    template <bool B, typename T, typename F>  auto static_if(T t, F f) { return static_if(std::integral_constant<bool, B>{}, t, f); }
    template <bool B, typename T>              auto static_if(T t) { return static_if(std::integral_constant<bool, B>{}, t, [](auto&&...){}); }
    
    template <class C> inline void trySetStackLabel(C& staterr, const char* stackLabel){
      static_if<hasStackLabel<C>::value>
        ([&](auto& s) {
           s.SetStackLabel(stackLabel);
         })
        (staterr);
    }
  }
}
#endif

//__________________________________________________________________________________|___________

TSModelFactory::TSModelFactory() {

  fShowInfo  = false;
  fShowError  = true;
  fShowWarn  = false;
}


//__________________________________________________________________________________|___________

TSModelFactory::~TSModelFactory() {
}


//__________________________________________________________________________________|___________

void TSModelFactory::info(TString message) {
  if (fShowInfo) {
    std::cout << "SFramework/TSModelFactory: " << message.Data() << std::endl;
  }
}


//__________________________________________________________________________________|___________

void TSModelFactory::error(TString message) {
  if (fShowError) {
    std::cout << "SFramework/TSModelFactory: " << TQStringUtils::makeBoldRed(TString("ERROR: ") + message) << std::endl;
  }
}


//__________________________________________________________________________________|___________

void TSModelFactory::warn(TString message) {
  if (fShowWarn) {
    std::cout << "SFramework/TSModelFactory: " << TQStringUtils::makeBoldYellow(TString("WARNING: ") + message) << std::endl;
  }
}


//__________________________________________________________________________________|___________

Bool_t TSModelFactory::createXML(TQFolder * model, const TString& xmlDir) {

  /* create model */
  RooStats::HistFactory::Measurement * ms = createMeasurement(model);

  /* write XML */
  if (ms) {
    Bool_t success = createXML(ms, xmlDir, model->getTagStringDefault(".HistogramFile", TQFolder::concatPaths(xmlDir,"histograms.root")));
    delete ms;
    return success;
  } else {
    return false;
  }
}

//__________________________________________________________________________________|___________

Bool_t TSModelFactory::writeHistograms(TQFolder* model, const TString& hfname){
  TQFolder * histos = model->getFolder(".Histograms");
  if (!histos) return false;
  /* replace parameter in filenames */
  TString histosFilename = model->replaceInText(hfname);

  histosFilename = TQPathManager::getPathManager()->getTargetPath(histosFilename);
  /* make sure directory exists */
  if (!TQUtils::ensureDirectoryForFile(histosFilename)) return false;

  TFile * histosFile = TFile::Open(histosFilename, "RECREATE", histosFilename.Data());
  if (!histosFile || !histosFile->IsOpen()) {
    if (histosFile) {
      delete histosFile;
    }
    return false;
  }
    
  TString targetName = TString::Format("%s_histograms", model->GetName());
  
  model->setTagString(".HistogramFile", histosFilename);
  model->setTagString(".HistogramPathPrefix", targetName);
  
  // delete existing object in file with same name
  TDirectory* target = histosFile->mkdir(targetName);
  TQFolder* nominal = histos->getFolder("Nominal");
  if(nominal){
    nominal->writeDirectory(target);
  } else {
    return false;
  }
  TQFolder* shapesysts = histos->getFolder("Systematics");
  if(shapesysts){
    shapesysts->writeDirectory(target);
  }
  histosFile->Write();
  histosFile->Close();
  delete histosFile;
  return true;
}

//__________________________________________________________________________________|___________

Bool_t TSModelFactory::createXML(RooStats::HistFactory::Measurement * measurement, const TString& xmlDir, const TString& histosFile) {

  /* write XML */
  if (measurement) {
    measurement->PrintXML(xmlDir.Data());

    // now, fix some stuff inplace because HistFactory is buggy
    char* rootsys = std::getenv("ROOTSYS");
    TString s = "cp ";
    s.Append(TQFolder::concatPaths(rootsys,"etc","HistFactorySchema.dtd"));
    s.Append(" ");
    s.Append(xmlDir);
    s.Append(+"/");
    info(s);
    TList* l = TQUtils::execute(s);
    delete l;
    l = TQUtils::execute ("sed -i -e 's|<Data[ ]*HistoName=\"\"[ ]*InputFile=\"\"[ ]*HistoPath=\"\"[ ]*/>||' "+xmlDir+"/*.xml");
    delete l;
    l = TQUtils::execute ("sed -i -e 's|InputFile=\"[^\"]*\"|InputFile=\""+histosFile+"\"|' "+xmlDir+"/*.xml");
    delete l;
    TString cwd = gSystem->pwd();
    TQStringUtils::ensureTrailingText(cwd,"/");
    l = TQUtils::execute ("sed -i -e 's|<Input>./"+cwd+"|<Input>|' "+xmlDir+"/*.xml");
    delete l;
    return true;
  } else {
    return false;
  }
}


//__________________________________________________________________________________|___________

RooStats::HistFactory::Measurement * TSModelFactory::createMeasurement(TQFolder * model) {

  /* stop if model definition is missing */
  if (!model) {
    error("createMeasurement(): No model definition provided");
    return 0;
  }

  info("createMeasurement(...): Creating Measurement object...");

  /* create a new Measurement object */
  RooStats::HistFactory::Measurement * measurement = new RooStats::HistFactory::Measurement(model->GetName(), model->getTagStringDefault("Title", model->GetTitle()));
  
  /* set configuration of Measurement object */
  TString outputFilePrefix = model->getTagStringDefault("OutputFilePrefix", model->GetName());
  measurement->SetOutputFilePrefix(outputFilePrefix.Data());
  Bool_t exportOnly = true;
  // @tag:[ExportOnly] If true, no plots or tables are produced but the model is just saved
  if (model->getTagBool("ExportOnly", exportOnly))
    measurement->SetExportOnly(exportOnly);
  auto pois = model->getTagVString("POI");
  //  if(pois.empty()) pois.push_back("mu");
  std::sort(pois.begin(),pois.end());
  for(auto poi:pois){
    measurement->AddPOI(poi.Data());
  }
  Double_t lumi = 1.;
  if (model->getTagDouble("Lumi", lumi))
    measurement->SetLumi(lumi);
  Double_t lumiRelErr = 0.;
  if (model->getTagDouble("LumiRelErr", lumiRelErr))
    measurement->SetLumiRelErr(lumiRelErr);


  /* loop over and add channels */
  TQIterator itrCh(model->getListOfFolders("?"), "!.*", true);
  while (itrCh.hasNext()) {
    addChannel((TQFolder*)itrCh.readNext(), measurement);
  }

  /* set global parameter settings */
  TQIterator itrParam(model->getListOfFolders("ParamSettings/?"), "!.*", true);
  while (itrParam.hasNext()) {
    TQFolder * folder = (TQFolder*)itrParam.readNext();
    TQTaggable tagsParam(folder);
    
    // constant parameter?
    Bool_t isConst = false;
    if (tagsParam.getTagBool("Const", isConst) && isConst) {
      measurement->AddConstantParam(folder->GetName());
    }

    // constraint term?
    TString constraint;
    if (tagsParam.getTagString("ConstraintTerm", constraint)) {
      if (constraint.CompareTo("Uniform", TString::kIgnoreCase) == 0) {
        measurement->AddUniformSyst(folder->GetName());
      } else if (constraint.CompareTo("Gamma", TString::kIgnoreCase) == 0) {
        measurement->AddGammaSyst(folder->GetName(), 1.);
      } else if (constraint.CompareTo("LogNorm", TString::kIgnoreCase) == 0) {
        measurement->AddLogNormSyst(folder->GetName(), 1.);
      } else if (constraint.CompareTo("Gaussian", TString::kIgnoreCase) != 0) {
        warn(TString::Format("createMeasurement(): Unknown constraint term '%s' "
                             "for parameter '%s'", constraint.Data(), folder->GetName()));
      }
    }
  }

  /* return measurement object */
  return measurement;
}


//__________________________________________________________________________________|___________

Bool_t TSModelFactory::reviseWorkspace(RooWorkspace * workspace, TQFolder * model) {

  /* stop if workspace is missing */
  if (!workspace) {
    error("reviseWorkspace(): No valid workspace provided");
    return false;
  }

  /* stop if model definition is missing */
  if (!model) {
    error("reviseWorkspace(): No model definition provided");
    return false;
  }

  // stop if model config is missing
  RooStats::ModelConfig* mc = dynamic_cast<RooStats::ModelConfig*>(workspace->obj("ModelConfig"));
  if (!mc) {
    error("reviseWorkspace(): No ModelConfig available");
    return false;
  }  

  // set title
  workspace->SetTitle(model->getTagStringDefault("Title", model->GetTitle()));
  
  /* check POIs */
  auto pois = model->getTagVString("POI");
  // if(pois.empty()) pois.push_back("mu");
  for(auto poi:pois){
    RooRealVar * var = workspace->var(poi.Data());
    if(!var){
      error(TString::Format("unable to find POI '%s' in workspace, please check for typos!",poi.Data()));
      return false;
    }
    RooRealVar *  poiobj  = dynamic_cast<RooRealVar*>(mc->GetParametersOfInterest()->find(poi.Data()));
    if(!poiobj){
      error(TString::Format("unable to find POI '%s' in ModelConfig, please check for typos!",poi.Data()));
      return false;
    }
  }

  /* check NPs */
  const RooAbsCollection* nps = mc->GetNuisanceParameters();
  if(!nps || nps->getSize() < 1){
    warn("no nuisance parameters set, setting manually");
    RooArgList allVars(workspace->allVars());
    RooRealVarIterator itr(allVars.createIterator());
    std::vector<TString> npnames;
    while (itr.hasNext()) {
      RooRealVar * var = itr.readNext();
      if(!var) continue;
      if(TQStringUtils::matches(var->GetName(),"alpha_*") || TQStringUtils::matches(var->GetName(),"gamma_stat_*")){
        npnames.push_back(var->GetName());
      }
    }
    mc->SetNuisanceParameters(TQStringUtils::concat(npnames,","));
  }
  
  /* set global parameter settings */
  TQIterator itrParam(model->getListOfFolders("ParamSettings/?"), "!.*", true);
  while (itrParam.hasNext()) {
    TQFolder * folder = (TQFolder*)itrParam.readNext();
    TString name = folder->GetName();

    // corresponding variable from workspace
    RooRealVar * var = workspace->var(name.Data());
    if (!var) {
      warn(TString::Format("reviseWorkspace(): Failed to find variable '%s' "
                           "in workspace. Skipping ...", name.Data()));
      continue;
    }

    TQTaggable tagsParam(folder);

    // title of variable
    TString title;
    if (tagsParam.getTagString("Title", title)) {
      var->SetTitle(title.Data());
    }

    // interpolation code?
    Int_t interpCode = 4;
    if (tagsParam.getTagInteger("InterpCode", interpCode)) {
      TQIterator itr(var->clientIterator(), true);
      while (itr.hasNext()) {
        TObject * obj = itr.readNext();
        if (obj->InheritsFrom(FlexibleInterpVar::Class())) {
          ((FlexibleInterpVar*)obj)->setInterpCode(*var, interpCode);
        } else if (obj->InheritsFrom(PiecewiseInterpolation::Class())) {
          ((PiecewiseInterpolation*)obj)->setInterpCode(*var, interpCode);
        } else {
          error(TString::Format("reviseWorkspace(): Failed to set interpolation code "
                                "on variable '%s'. Skipping ...", var->GetName()));
        }
      }
    }

    // description?
    TString desc;
    if(tagsParam.getTagString("Description",desc)){
      var->setAttribute(desc.Data(),true);
    }

    std::vector<TString> attributes = tagsParam.getTagVString("Attributes");
    for(const auto& attr:attributes){
      var->setAttribute(attr.Data());
    }
  }
  
  if(model->getTagBoolDefault("isBinned",true)){
    RooFIter iter = workspace->components().fwdIterator() ;
    RooAbsArg* arg = NULL;
    while((arg = iter.next())) {
      if (arg->IsA() == RooRealSumPdf::Class()) {
        arg->setAttribute("BinnedLikelihood");
      }
    }
  }

  // check for Lumi parameter
  RooRealVar* lumi = workspace->var("Lumi");
  if(lumi){
    lumi->setConstant(true);
  }
  
  // Save model in workspace: export as text listing ...
  TList * lConfig = model->exportToText(false);
  // ... and put it into workspace (keep workspace independent of analysis code library)
  workspace->import(*lConfig, ".Model", true);

  return true;
}


//__________________________________________________________________________________|___________

Bool_t TSModelFactory::addChannel(TQFolder * definition, RooStats::HistFactory::Measurement * measurement) {

  /* extract channel name */
  TString channelName = definition->GetName();
  if (!TQStringUtils::removeLeadingText(channelName, "Channel."))
    return false;
  
  TString histosfile = definition->getTagStringDefault("~.HistogramFile", "");
  TString histospath = TQFolder::concatPaths(definition->getTagStringDefault("~.HistogramPathPrefix", ""),"Nominal",channelName.Data());
  
  /* create Channel object */
  RooStats::HistFactory::Channel * channel = new Channel(channelName.Data(),histospath.Data());

  /* set statistical error configuration */
  channel->SetStatErrorConfig(
                              definition->getTagDoubleDefault("StatRelErrorThreshold", 0.05),
                              definition->getTagStringDefault("StatConstraintType", "Poisson").Data());

  /* loop over subfolder */
  TQIterator itr(definition->getListOfFolders("?"), "!.*", true);
  while (itr.hasNext()) {
    TQFolder * folder = (TQFolder*)itr.readNext();

    if (addSample(folder, channel)) {
      info(TString::Format("Added as Sample '%s'", folder->GetName()));
    }
  }

  /* add Channel object to Measurement object */
  measurement->AddChannel(*channel); //FIXME: this looks like a memory leak. AddChannel does not take a reference but is called by value. However, we never delete our local "channel" object

  return true;
}


//__________________________________________________________________________________|___________

Bool_t TSModelFactory::addSample(TQFolder * definition, RooStats::HistFactory::Channel * channel) {

  // extract sample name by removing the leading
  // "Sample." (stop if this prefix is not present)
  TString sampleName = definition->GetName();
  if (!TQStringUtils::removeLeadingText(sampleName, "Sample.")) {
    // missing prefix "Sample."
    return false;
  }
  TString sampleType = definition->getTagStringDefault("Type","B");

  /* extract histogram location */
  TString histogramLocation;
  if(!definition->getTagString("Histo",histogramLocation)){
    error(TString::Format("unable to find histogram for '%s'",definition->getPath().Data()));
    return false;
  }
  TString histogramFile = definition->getTagStringDefault("~.HistogramFile","histFactor_tmp.root");
  TString histogramPathPrefix = definition->getTagStringDefault("~.HistogramPathPrefix","");
  
  /* split histogram location */
  TString histoPath = TQFolder::concatPaths(histogramPathPrefix, histogramLocation);
  TString histoName = TQFolder::getPathTail(histoPath);    
  TQStringUtils::ensureTrailingText(histoPath,"/");

  TString histoFilePath = TQPathManager::getPathManager()->getTargetPath(histogramFile).c_str();
  
  if (sampleType != "D"){
    
    /* create Sample object */
    Sample * sample = new Sample(sampleName.Data(),histoName.Data(), histoFilePath.Data(), histoPath.Data());
  
    /* NormalizeByTheory */
    sample->SetNormalizeByTheory(definition->getTagBoolDefault("NormalizeByTheory", true));

    /* StatError ? */
    if (definition->getTagBoolDefault("ActivateStatError", false)) {
      sample->GetStatError().Activate(true);

      #ifdef HAS_GENERIC_LAMBDAS    
      TString stackLabel;
      if(definition->getTagString("StatErrorStackLabel",stackLabel)){
        ::SFINAE::trySetStackLabel(sample->GetStatError(),stackLabel.Data());
      }
      #endif
      
      if (definition->hasTag("StatErrorHisto")) {
        TString statErrorHistoFile;
        TString statErrorHistoName;
        TString statErrorHistoPath;
        splitHistoLocation(definition->getTagStringDefault("StatErrorHisto"),statErrorHistoFile, statErrorHistoName, statErrorHistoPath);
        TString statErrorHistoFilePath = TQPathManager::getPathManager()->getTargetPath(statErrorHistoFile).c_str();
        sample->GetStatError().SetUseHisto(true);
        sample->GetStatError().SetHistoName(statErrorHistoName.Data());
        sample->GetStatError().SetHistoName(statErrorHistoFilePath.Data());
        sample->GetStatError().SetHistoName(statErrorHistoPath.Data());
      } 
    } else {
      sample->GetStatError().Activate(false);
    }
    
    /* loop over subfolders */
    TQIterator itr(definition->getListOfFolders("?"), "!.*", true);
    while (itr.hasNext()) {
      TQFolder * folder = (TQFolder*)itr.readNext();
      TString name = folder->GetName();

      if (addNormFactor(folder, sample)) {
        info(TString::Format("Added as NormFactor '%s'", folder->GetName()));
      } else if (addOverallSys(folder, sample)) {
        info(TString::Format("Added as OverallSys '%s'", folder->GetName()));
      } else if (addHistoSys(folder, sample)) {
        info(TString::Format("Added as HistoSys '%s'", folder->GetName()));
      } else if (addShapeSys(folder, sample)) {
        info(TString::Format("Added as ShapeSys '%s'", folder->GetName()));
      } else if (addShapeFactor(folder, sample)) {
        info(TString::Format("Added as ShapeFactor '%s'", folder->GetName()));
      } else {
        error(TString::Format("Unrecognized element '%s' "
                              "in sample '%s'", name.Data(), sampleName.Data()));
      }
    }

    /* add the Sample object to the Channel object */
    channel->AddSample(*sample);
  } else {
    /* add data */
    channel->SetData(histoName.Data(), histoFilePath.Data(), histoPath.Data());
  }

  return true;
}


//__________________________________________________________________________________|___________

Bool_t TSModelFactory::addNormFactor(TQFolder * definition, Sample * sample) {

  /* extract norm factor name */
  TString normFactorName = definition->GetName();
  if (!TQStringUtils::removeLeadingText(normFactorName, "NormFactor.")) {
    // missing prefix "NormFactor."
    return false;
  }

  /* add norm factor */
  sample->AddNormFactor(normFactorName.Data(),
                        definition->getTagDoubleDefault("Val", 1.),
                        definition->getTagDoubleDefault("Low", 1.),
                        definition->getTagDoubleDefault("High", 1.),
                        definition->getTagBoolDefault("Const",false));

  /* propagate constant attribute of parameter */
  Bool_t isConst = false;
  if (definition->getTagBool("Const", isConst)) {
    TQFolder * paramSettings = definition->getFolder(
                                                     TQFolder::concatPaths("../../../ParamSettings", normFactorName) + "+");

    TQTaggable tagsParam(paramSettings);
    if (tagsParam.getTagBoolDefault("Const", isConst) == isConst) {
      paramSettings->setTagBool("Const", isConst);
    } else {
      paramSettings->setTagBool(".Const.Recessive", isConst);
    }
  }

  return true;
}


//__________________________________________________________________________________|___________

Bool_t TSModelFactory::addShapeFactor(TQFolder * definition, Sample * sample) {

  /* extract shape factor name */
  TString shapeFactorName = definition->GetName();
  if (!TQStringUtils::removeLeadingText(shapeFactorName, "ShapeFactor.")) {
    // missing prefix "ShapeFactor."
    return false;
  }

  /* add shape factor */
  sample->AddShapeFactor(shapeFactorName.Data());

  return true;
}


//__________________________________________________________________________________|___________

Bool_t TSModelFactory::addOverallSys(TQFolder * definition, Sample * sample) {

  /* extract systematic name */
  TString overallSysName = definition->GetName();
  if (!TQStringUtils::removeLeadingText(overallSysName, "OverallSys.")) {
    // missing prefix "OverallSys."
    return false;
  }

  /* add norm factor */
  sample->AddOverallSys(overallSysName.Data(),
                        definition->getTagDoubleDefault("Low", 1.),
                        definition->getTagDoubleDefault("High", 1.));

  return true;
}


//__________________________________________________________________________________|___________

Bool_t TSModelFactory::addHistoSys(TQFolder * definition, Sample * sample) {

  /* extract systematic name */
  TString histoSysName = definition->GetName();
  if (!TQStringUtils::removeLeadingText(histoSysName, "HistoSys.")) {
    // missing prefix "HistoSys."
    return false;
  }
  
  /* extract histogram locations */
  TString histogramLocationHigh,histogramLocationLow;
  if(!definition->getTagString("HistoHigh",histogramLocationHigh) || !definition->getTagString("HistoLow",histogramLocationLow)){
    error(TString::Format("no histogram assigned to '%s'",definition->getPath().Data()));
    return false;
  }
  
  /* split histogram locations */
  /* extract histogram location */
  
  TString histogramFile = definition->getTagStringDefault("~.HistogramFile","histFactor_tmp.root");
  TString histogramPathPrefix = definition->getTagStringDefault("~.HistogramPathPrefix","");
  
  /* split histogram location */
  TString histoPathLow = TQFolder::concatPaths(histogramPathPrefix, histogramLocationLow);
  TString histoPathHigh = TQFolder::concatPaths(histogramPathPrefix, histogramLocationHigh);
  TString histoNameLow = TQFolder::getPathTail(histoPathLow);    
  TString histoNameHigh = TQFolder::getPathTail(histoPathHigh);    
  TQStringUtils::ensureTrailingText(histoPathLow,"/");
  TQStringUtils::ensureTrailingText(histoPathHigh,"/");
  
  TString histoFilePath = TQPathManager::getPathManager()->getTargetPath(histogramFile).c_str();
  
  /* add norm factor */
  sample->AddHistoSys(histoSysName.Data(), 
                      histoNameLow.Data(),  histoFilePath.Data(), histoPathLow.Data(),
                      histoNameHigh.Data(), histoFilePath.Data(), histoPathHigh.Data());
  
  return true;
}

//__________________________________________________________________________________|___________

Bool_t TSModelFactory::addShapeSys(TQFolder * definition, Sample * sample) {

  /* extract systematic name */
  TString shapeSysName = definition->GetName();
  if (!TQStringUtils::removeLeadingText(shapeSysName, "ShapeSys.")) {
    // missing prefix "ShapeSys."
    return false;
  }

  /* extract histogram locations */
  TString histogramLocation;
  if(!definition->getTagString("Histo",histogramLocation)){
    error(TString::Format("unable to find histogram for '%s'",definition->getPath().Data()));
    return false;
  }  
  TString histogramFile;
  TString histogramPathPrefix;
  definition->getTagString("~.HistogramFile", histogramFile, true);
  definition->getTagString("~.HistogramPathPrefix", histogramPathPrefix, true);
  
  bool usePoisson = definition->getTagBoolDefault("usePoisson",false);
  
  histogramLocation = histogramFile + ":"
    + TQFolder::concatPaths(histogramPathPrefix, histogramLocation);

  /* split histogram locations */
  TString histoFile;
  TString histoName;
  TString histoPath;
  splitHistoLocation(histogramLocation, histoFile, histoName, histoPath);
  
  /* add norm factor */
  sample->AddShapeSys(shapeSysName.Data(), ( usePoisson ? RooStats::HistFactory::Constraint::Type::Poisson : RooStats::HistFactory::Constraint::Type::Gaussian ) , histoName.Data(), TQPathManager::getPathManager()->getTargetPath(histoFile).c_str(), histoPath.Data());

  return true;
}


//__________________________________________________________________________________|___________

void TSModelFactory::splitHistoLocation(TString input,
                                        TString &histoFile, TString &histoName, TString &histoPath) {
  /* extract histogram location */
  histoPath = input;
  histoFile = TQStringUtils::readPrefix(histoPath, ":");
  histoName = TQFolder::getPathTail(histoPath);

  if (!histoPath.IsNull())
    histoPath.Append("/");
}
