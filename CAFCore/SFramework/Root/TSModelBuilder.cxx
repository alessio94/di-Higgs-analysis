#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <limits>
#include <iterator>

#include "SFramework/TSModelBuilder.h"
#include "RooStats/HistFactory/HistoToWorkspaceFactoryFast.h"

#include "QFramework/TQSampleFolder.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQSampleDataReader.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQTaggable.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

// #define _DEBUG_

#include "QFramework/TQLibrary.h"

#include "TSystem.h"
#include "TMath.h"
#include "RooWorkspace.h"
#include "RooMsgService.h"

#include "TStopwatch.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
//	TSModelBuilder
//
//	The TSModelBuilder class allows to create a complete definition of a statistical model to be
//	passed to the TSModelFactory class to run HistFactory. The specifics of the model to be
//	created have to be provided as configuration data by means of a TQFolder structure.
//
//	A model is represented by the root instance of a TQFolder structure holding detailed
//	definitions of channels, samples, systematic uncertainties as well as the corresponding
//	histograms.
//
//	Creation of a model by the TSModelBuilder class is split into two different steps: the
//	preparation step and the finalization step. The finalization step involves retrieving
//	histograms and counter representing signal and background expectations as well as data
//	observations from a sample folders (instances of TQSampleFolder) either stored in external
//	files or made available as memory resident object. Furthermore, it involves the analysis of
//	variations corresponding to systematic uncertainties and their proper treatment.
//
//
//
//	The configuration has to be provided as the following structure:
//
//	Samples/                           A folder listing all samples to be used in the model
//	                                   (entries starting with a dot "." are ignored)
//	  {sample/}                        [one folder for each sample]
//	    <Path>                         Default path to the corresponding sample (with respect to the
//	                                   base sample folder), e.g. "sig/em/mh125/ggf". Placeholders
//	                                   (e.g. "$(mass)") can be used and will be replaced using tags
//	                                   from configuration of corresponding channel and from Parameter
//	                                   block.
//	    <forChannel.<channel>.Path>    Overwriting default path (see <Path>) for channel <channel>
//	    <Type>                         [optional] The type of the corresponding sample,
//	                                   e.g. "S" for signal, "B" for background, and "D" for data
//	    <ActivateStatError>            [optional, default as in TSModelFactory (false)]
//	    <StatErrorHisto>               [optional]
//	    <NormalizeByTheory>            [optional, default as in TSModelFactory (true)]
//	    <forChannel.<channel>.XXX>
//	    {additional-items/}            [one folder for each additional items]
//	      <.AppliesToChannels>         String filter to be passed by name of channel in order
//	                                   to copy corresponding item to final instance of sample
//
//	Channels/                          A folder listing all channels (regions) to be used in
//	                                   the model (entries starting with a dot "." are ignored)
//	  {channel/}                       [one folder for each channel/region]
//	    <Histogram>                    Name of histogram to be used for corresponding channel,
//	                                   e.g. "CutTopoDPhill_0jet/MT".
//	    <Counter>                      Name of counter to be used for corresponding channel,
//	                                   e.g. "CutTopoDPhill_0jet" (counter will be converted to
//	                                   1-bin histogram)
//	    <StatRelErrorThreshold>
//	    <StatConstraintType>
//
//	    <RemapXTo>                     Number of bins to remap X axis of histogram to (histogram is
//	                                   remapped such that histogram obtained from <PathRemapFlat> is
//	                                   approximately flat).
//	    <RemapYTo>                     Number of bins to remap Y axis of histogram to (histogram is
//	                                   remapped such that histogram obtained from <PathRemapFlat> is
//	                                   approximately flat). Please note: remapping both X and Y axes
//	                                   simultaneously is not supported.
//	    <PathRemapFlat>                Path to histogram to become approximately flat by remapping
//	    <RemapSlices>                  Controls whether when remapping a 2D histogram each bin of the
//	                                   dimension NOT subject to the remapping is remapped individually
//	                                   (<RemapSlices> == true) or only the full projection
//	                                   (<RemapSlices> == false). This option is true by default
//	    <convertTo1D>                  Definition of conversion ("unfolding", "unrolling") of a 2D
//	                                   histogram to a 1D histogram.
//	    <HistogramOptions>
//
//
//	                                   ***** For documentation of the following options please refer to
//	                                   the documentation of TQSampleDataReader::getHistogram(...) *****
//
//	    <cutBinLowX>                   Removing bins from histograms
//	    <cutBinHighX>                  /.../
//	    <cutBinLowY>                   /.../
//	    <cutBinHighY>                  /.../
//	    <cutBinLow>                    /.../
//	    <cutBinHigh>                   /.../
//	    <zoomBinLowX>                  Zooming into histograms
//	    <zoomBinHighX>                 /.../
//	    <zoomBinLowY>                  /.../
//	    <zoomBinHighY>                 /.../
//	    <zoomBinLow>                   /.../
//	    <zoomBinHigh>                  /.../
//	    <projXBinLowY>                 Projecting 2D histograms onto one if its axes
//	    <projXBinHighY>                /.../
//	    <projXBinY>                    /.../
//	    <projX>                        /.../
//	    <projYBinLowX>                 /.../
//	    <projYBinHighX>                /.../
//	    <projYBinX>                    /.../
//	    <projY>                        /.../
//	    <rebinX>                       Rebinning histograms
//	    <rebin>                        /.../
//	    <rebinY>                       /.../
//	    <rebinZ>                       /.../
//	    <scale>                        Scaling the histogram
//	    <verbosity>                    Temporarily setting the verbosity mode of the
//	                                   TQSampleDataReader class
//	    <applyPoissonErrors>           Set bin errors of histogram to sqrt(bin contents)
//
//	    <forSample.<sample>.XXX>
//
//	                                   *****************************************************************
//
//	Variations/                        A folder listing all (systematic) variations to be used
//	                                   in the model (entries starting with a dot "." are ignored)
//	  {variation/}                     [one folder for each (systematic) variation]
//	    <SampleFolder>                 Location of the sample folder of the corresponding
//	                                   variation, e.g. "samples.root:samples_Nominal". Please
//	                                   note: there at least needs to be a "Nominal" folder
//
//	Systematics/                       A folder listing all sources of systematic uncertainties
//	                                   to be used in the model (entries starting with a dot "."
//	                                   are ignored)
//	  {systematic/}                    [one folder for each source of systematic uncertainty]
//	    <Up>                           Name of UP variation corresponding to systematic
//	                                   uncertainty (must be listed in Variations/ folder)
//	    <Down>                         Name of DOWN variation corresponding to systematic
//	                                   uncertainty (must be listed in Variations/ folder)
//	    <IsOverallSys>                 Selects if systematic uncertainty is treated as a
//	                                   normalization systematic ('OverallSys')
//	    <IsHistoSys>                   Selects if systematic uncertainty is treated as a shape
//	                                   systematic ('HistoSys'). Please note: the normalization
//	                                   component is factorized and removed for 'HistoSys'
//	    <.Include>                     String filter tested for each combination of channel
//	                                   (region) and sample (in the form "<channel>:<sample>")
//	                                   which has to be pass in order that the corresponding
//	                                   systematic uncertainty is considered for this
//	                                   combination of channel/sample
//	    <.Exclude>                     String filter tested for each combination of channel
//	                                   (region) and sample (in the form "<channel>:<sample>")
//	                                   which must NOT be pass in order that the corresponding
//	                                   systematic uncertainty is considered for this
//	                                   combination of channel/sample (if a combination passes
//	                                   both the inclusion and the exclusion filter it is
//	                                   excluded)
//	    <InterpCode>                   Interpolation code to be used by HistFactory
//	    <OverallSysMagnification>      Factor to artificially magnify the normalization
//	                                   response of the systematic uncertainty
//	    <HistoSysMagnification>        Factor to artificially magnify the shape response of
//	                                   the systematic uncertainty
//	    <Magnification>                Factor passed as default to <OverallSysMagnification>
//	                                   and <HistoSysMagnification>
//
//	Parameters/
//	  <Name>
//	  <Title>
//	  <OutputFilePrefix>
//	  <ExportOnly>
//	  <POI>                            Name of the Parameter Of Interest (POI)
//	  <Lumi>                           Luminosity scale factor
//	  <LumiRelErr>                     Relative luminosity uncertainty
//	  <SysRelThreshold>                Minimum relative variation to be exceeded by a systematic
//	                                   variation in order to be considered
//	  <SysRelMin>                      Minimum relative variation to be exceeded by a systematic
//	                                   variation in order to not be ignored
//	  <SysRelCutoff>                   Maximum relative variation a systematic variation must
//	                                   not exceed in order to be considered
//	  <SysRelCap>                      The maximum relative variation a systematic variation. If
//                                     the maximum is exceeded, the relative variation is set to this maximum
//	  <MinBinContent>                  The minimum bin content of histograms of each process
//	                                   (except Data histograms). If the bin content is lower
//	                                   than this minimum it is set to the minimum
//	  <UncertaintyFiles>
//	  NormFiles/
//	    {<sample>}
//
//	Maps/
//	  Samples/
//	    {<sample_in_ext_file>}
//	  Channels/
//	    {<channel_in_ext_file>}
//	  Regions/
//	    {<region_in_ext_file>}
//	  Jets/
//	    {<jetbin_in_ext_file>}
//
//
//	The model built by the TSModelBuilder class uses the convention that each element available
//	in HistFactory (e.g. a "Sample", a "Channel", a "NormFactor) is represented by a
//	corresponding instance of TQFolder whose name prefix indicates the type of element followed
//	by the name of the element (e.g. "Sample.ttbar"). Properties of an element are stored as
//	tags on the corresponding folders and sub-elements are represented by sub folders. Available
//	elements and their properties are:
//
//	- Channel ("Channel.name/");
//	  - <StatRelErrorThreshold>
//	  - <StatConstraintType>
//
//	- Sample ("Sample.name/");
//	  - <Histo>
//	  - <.HistogramFile>
//	  - <.HistogramPathPrefix>
//	  - <NormalizeByTheory>
//	  - <ActivateStatError>
//	  - <StatErrorHisto>
//
//	- NormFactor ("NormFactor.name/");
//	  - <Val>
//	  - <Low>
//	  - <High>
//	  - <Const>
//
//	- OverallSys ("OverallSys.name/");
//	  - <High>
//	  - <Low>
//
//	- HistoSys ("HistoSys.name/");
//	  - <HistoHigh>
//	  - <HistoLow>
//	  - <.HistogramFile>
//	  - <.HistogramPathPrefix>
//
//
//	It will have to following structure:
//
//	{Channel.*/}                       [one folder for each channel]
//	  {Sample.*/}                      [one folder for each sample in channel]
//	.Configuration/                    The model configuration folder after the model building
//	                                   process which includes additional information (e.g. bin
//	                                   boundaries from remapping)
//	.Configuration.PreBuild/           A copy of the model configuration folder BEFORE the model
//	                                   building process
//	.Histograms/                       All histograms used in the model building process
//	                                   (including those created from counter on-the-fly) are
//	                                   stored here (this is also the folder written to a ROOT
//	                                   file to let HistFactory grab the histograms from).
//
//	A model created by the TSModelBuilder class is interfaced by the TSModelFactory class to
//	HistFactory.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(TSModelBuilder)


//__________________________________________________________________________________|___________

TSModelBuilder::TSModelBuilder() {
  // Default constructor for TSModelBuilder class.

  // there is no default sample folder
  fDefaultSampleFolder = NULL;
}


//__________________________________________________________________________________|___________

TSModelBuilder::~TSModelBuilder() {
}


//__________________________________________________________________________________|___________

void TSModelBuilder::info(TString message) {

  // print an info message
  std::cout << "SFramework/TSModelBuilder: " << message.Data() << std::endl;
}


//__________________________________________________________________________________|___________

void TSModelBuilder::error(TString message) {
  errorCount++;

  // print a red error message
  if(errorCount < maxErrorCount){
    info(TQStringUtils::makeBoldRed(TString("ERROR: ") + message));
  } else if(errorCount == maxErrorCount){
    info(TQStringUtils::makeBoldRed(TString("ERROR: ") + message));
    info(TQStringUtils::makeBoldRed("ERROR: too many error messages, suppressing further output"));
  }
}
  

//__________________________________________________________________________________|___________

void TSModelBuilder::warn(TString message) {
  warnCount++;
  
  // print a yellow warning message
  if(warnCount < maxErrorCount){
    info(TQStringUtils::makeBoldYellow(TString("WARNING: ") + message));
  } else if(warnCount == maxErrorCount){
    info(TQStringUtils::makeBoldYellow(TString("WARNING: ") + message));    
    info(TQStringUtils::makeBoldRed("WARNING: too many warning messages, suppressing further output"));
  }
}


//__________________________________________________________________________________|___________

void TSModelBuilder::setDefaultSampleFolder(TQSampleFolder * sampleFolder) {
  // Sets the default sample folder to <sampleFolder>. The default sample folder is
  // that one which is used to read histograms and counter from if no external file
  // name is provided.

  // set the default sample folder
  fDefaultSampleFolder = sampleFolder;
}


//__________________________________________________________________________________|___________

TQSampleFolder * TSModelBuilder::getDefaultSampleFolder() {
  // Returns a pointer to the default sample folder.

  // return the default sample folder
  return fDefaultSampleFolder;
}


//__________________________________________________________________________________|___________

Bool_t TSModelBuilder::createChannelDefinitions(TQFolder * config, TQFolder * model) {
  // Creates the basic channel definitions of a model from configuration <config>
  // and returns true in case of success or false in case of failure.

  Bool_t success = true;

  // iterate over list of channels defined in configuration
  // (ignoring entries starting with a dot ".")
  TQIterator itr(config->getListOfFolders("Channels/?"), "!.*", true);
  while (itr.hasNext()) {
    // name of the channel
    TString name = itr.readNext()->GetName();
    // create definition of channel <name>
    if (!createChannelDefinition(config, model, name)) {
      // in case of failure ...
      success = false;
      error(TString::Format("createChannelDefinitions(...): "
                            "Failed to create definition for channel '%s'", name.Data()));
    }
  }

  // return true in case of success or false in case of failure
  return success;
}


//__________________________________________________________________________________|___________

Bool_t TSModelBuilder::createChannelDefinition(TQFolder * config, TQFolder * model, TString name) {
  // Creates the basic channel definitions of a model from configuration <config>
  // and returns true in case of success or false in case of failure.

  /* load channel configuration */
  TQFolder * channelConfig = config->getFolder(TQFolder::concatPaths("Channels", name));
  if (!channelConfig) {
    error(TString::Format("createChannelDefinition(...): "
                          "Configuration for channel '%s' missing", name.Data()));
    return false;
  }

  /* create new channel in model definition */
  TQFolder * channelDef = model->getFolder(TString::Format("Channel.%s+", name.Data()));
  channelDef->setTagString("Channel",name);

  /* copy tags from Channel configuration to Channel definition */
  channelDef->importTags(channelConfig);

  /* loop over samples in template list */
  TQIterator itrSamples(config->getListOfFolders("Samples/?"), "!.*", true);
  while (itrSamples.hasNext()) {
    TQFolder * sampleConfig = (TQFolder*)itrSamples.readNext();
    TString nameSample = sampleConfig->GetName();
    TString sampleType = sampleConfig->getTagStringDefault("Type","B");    

    // create sample definition folder in model
    TQFolder * sampleDef = channelDef->getFolder(TString::Format("Sample.%s+", nameSample.Data()));
    sampleDef->setTagString("Sample",nameSample);
    sampleDef->setTagString("Type", sampleType);

    // nothing more to do in case of Data
    //if (sampleType == "D"){
    //  continue;
    //}

    // copy some tags from sample configuration to sample definition in model
    sampleDef->importTags(sampleConfig);

    // loop over sub elements of sample and add them to the sample definition
    TQIterator itrElements(sampleConfig->getListOfFolders("?"), "!.*", true);
    while (itrElements.hasNext()) {
      TQFolder * element = (TQFolder*)itrElements.readNext();
      TString nameElement = element->GetName();

      // the string filter to be passed by name of channel in order to
      // consider an additional sample item, e.g. a NormFactor. Default is "*"
      TString channelFilter = "*";
      element->getTagString(".AppliesToChannels", channelFilter);

      // if string filter is passed ...
      if (TQStringUtils::matchesFilter(name, channelFilter, ",", true)) {
        // ... add a copy of additional sample item
        sampleDef->addObject(element->copy());
      }
    }
  }

  // everything seems to have worked out properly
  return true;
}


//__________________________________________________________________________________|___________

TQFolder * TSModelBuilder::createDefinition(TQFolder * config) {

  /* basic configuration of workspace/model */
  TString modelName = config->GetName();

  /* create new definition */
  TQFolder * model = TQFolder::newFolder("Model");

  // the folder the histograms are put in
  TQFolder * histos = model->getFolder(".Histograms+");
  this->setRepository(histos);
  
  TQFolder * parConfig = config->getFolder("Parameters");
  if (parConfig) {
    // get the name of the model
    parConfig->getTagString("Name", modelName);
    config->getTagString("Name", modelName);
    parConfig->setTagString("Name", modelName);
    parConfig->exportTags(model);
    modelName = parConfig->replaceInText(modelName);
  }

  modelName = TQFolder::makeValidIdentifier(modelName);
  model->SetName(modelName);

  info(TString::Format("createDefinition(): Creating definition for model '%s' ...", modelName.Data()));

  if (!createChannelDefinitions(config, model)) {
    error("createDefinition(): Failed to create model definition");
    return NULL;
  }

  /* copy ParamSettings */
  TQFolder * paramSettings = config->getFolder("ParamSettings");
  if (paramSettings) {
    model->addObject(paramSettings->copy());
  }


  TQFolderIterator edits(config->getListOfFolders("Edits/?"),true);
  while(edits.hasNext()){
    TQFolder* edit = edits.readNext();
    if(!edit) continue;
    this->applyEdits(edit,model);
  }

  return model;
}


//__________________________________________________________________________________|___________

void TSModelBuilder::applyEdits(TQFolder* edit, TQFolder* model){
  TString file;
  if(edit->getTagString("file",file)){
    TString msg;
    INFOfunc(TString::Format("applying edits from file '%s'",file.Data()));
    if(!model->importFromTextFile(TQPathManager::getPathManager()->findConfigPath(file).c_str(),msg)){
      ERRORfunc(msg);
    }

  }
  std::vector<TString> commands = edit->getTagVString("commands");
  if (commands.empty() && edit->hasTagString("command")) commands.push_back(edit->getTagStringDefault("command",""));
  for (auto com:commands) {
    TString msg;
    com.ReplaceAll("\n","");
    com.ReplaceAll(" ","");
    INFOfunc(TString::Format("applying edit command '%s'",com.Data()));
    if(!model->importFromText(com,msg)){
      ERRORfunc(msg);
    }
  }
}

//__________________________________________________________________________________|___________

Bool_t TSModelBuilder::finalizeModel(TQFolder * model, TQFolder * config) {
  // Finalizes the model <model> based on the configuration <config> by collecting
  // all histograms, including systematic uncertainties inferred from variations of
  // input histograms, and returns true in case of success or false otherwise.

  if (!model || !config) {
    error("finalizeModel(...): Model and/or configuration missing. Stopping ...");
    return false;
  }

  // collect histograms (for all variations including nominal)
  if(config->getTagBoolDefault("collectHistograms",true)){
    if (!collectAllHistograms(config, model)) {
      error("finalizeModel(...): Failed to collect histograms. Stopping ...");
      return false;
    }
  } else {
    info("finalizeModel(...): Skipping histogram collection");
  }
    

  if(config->getTagBoolDefault("includeSystematics",true)){
    TQFolder* parameters = config->getFolder("Parameters");
    TQFolder* systematics = config->getFolder("Systematics");      
    TList* allSamples = model->getListOfFolders("Channel.*/Sample.*");

    // include systematic uncertainties (evaluate variations for systematics
    // from input histograms and create corresponding entries in the model)
    bool ok = true;
    if (ok && !includeAllSystematics(systematics, allSamples)) {
      error("finalizeModel(...): Failed to include systematics. Stopping ...");
      ok = false;
    }

    if(config->getTagBoolDefault("processSystematics",true)){    
      if (ok && !processAllSystematics(parameters, allSamples)) {
        error("finalizeModel(...): Failed to process systematics. Stopping ...");
        ok = false;
      }
    }
    
    delete allSamples;
    if(!ok) return false;
  }

  if(this->fDefaultSampleFolder){
    for(auto s:config->getFolder("Variations")->getTagVString("importTags")){
      TString sourcePath = TQStringUtils::readPrefix(s,":","./");
      
      TString sourceName;
      TQStringUtils::readUpTo(s,sourceName,">>");
      TString targetName;
      if(TQStringUtils::removeLeading(s,">")>0){
        targetName=s;
      } else {
        targetName=sourceName;
      }
      TQFolder* source = this->fDefaultSampleFolder->getFolder(sourcePath);
      if(!source){
        error(TString::Format("cannot import tag from '%s': no such folder in input!",sourcePath.Data()));
      } else {
        TString value;
        if(!source->getTagString(sourceName,value)){
          error(TString::Format("cannot import tag '%s' from '%s': no such tag!",sourceName.Data(),sourcePath.Data()));
        } else {
          model->setTagAuto(targetName,value);
        }
      }
    }
  }

  model->sortByNameRecursive();

  return true;
}


//__________________________________________________________________________________|___________

TQFolder * TSModelBuilder::buildModel(TQFolder * config) {
  // Builds a model from configuration <config> and returns a pointer to the
  // corresponding instance of TQFolder representing the model or a NULL pointer in
  // case of failure

  TQFolder * model = createDefinition(config);
  // prepare model
  if (!model) {
    error("buildModel(...): Failed to prepare model. Stopping ...");
    return NULL;
  }

  // finalize model
  if (!finalizeModel(model, config)) {
    error("buildModel(...): Failed to finalize model. Stopping ...");
  }

  // return the model
  return model;
}

//__________________________________________________________________________________|___________

void TSModelBuilder::purgeVariation(TQFolder* model, const TString& name, Bool_t /*notify*/){
  // remove a variation from a model
  warn(TString::Format("purgeVariation('%s') : removing variation... ",name.Data()));
  model->removeObject(TQFolder::concatPaths("Variations",name));
  TQIterator itr(model->getListOfFolders("Systematics/?"),true);
  while(itr.hasNext()){
    TQFolder* f = dynamic_cast<TQFolder*>(itr.readNext());
    if(!f) continue;
    if((f->getTagStringDefault("Up","") == name) || (f->getTagStringDefault("Down","") == name)){
      warn(TString::Format("purgeVariation('%s') : removing systematic '%s'",name.Data(),f->GetName()));
      model->deleteObject(TQFolder::concatPaths("Systematics",f->getName()));
    }
  }
}


//__________________________________________________________________________________|___________

Bool_t TSModelBuilder::collectAllHistograms(TQFolder * config, TQFolder * model) {
  // collect all histograms from the sample folder and store them in the model
  info("collectAllHistograms(): Start collecting histograms...");


  TQFolder* variations = config->getFolder("Variations+");
  TString defaultSF;
  bool didLoadSF = false;
  if(variations->getTagString("SampleFolder",defaultSF)){
    TQSampleFolder* sf = this->getSampleFolder(variations);
    // replace with the following line for faster dummy-debugging
    // TQSampleFolder* sf = new TQSampleFolder("samples");
    
    if(!sf){
      error(TString::Format("collectAllHistograms: unable to load default sample folder '%s'",defaultSF.Data()));
      return false;
    }
    didLoadSF=true;
    this->setDefaultSampleFolder(sf);
  }
  
  /* "Nominal" has to be first because it fixes the remapping information */
  collectHistograms(config, model, variations->getFolder("Nominal+"));

  /* ===== loop over variations ===== */
  TQFolderIterator itr(variations->getListOfFolders("?"), "!.*", true);
  while (itr.hasNext()) {
    TQFolder* variation = itr.readNext();
    TString name = variation->GetName();
    /* now run all but "Nominal" */
    if (name.CompareTo("Nominal") != 0) {
      bool success = collectHistograms(config, model, variation);
      if(!success && config->getTagBoolDefault("skipMissingVariations",false)){
        this->purgeVariation(config,name,true);
      }
    }
  }
  if (didLoadSF && this->fDefaultSampleFolder) {
    //this->fDefaultSampleFolder->collapse(); //collapsing the sf can cause loss of information (i.e. modifications to parts that are beyond TQImportLinks after collapsing). Hence, only do this if we just opened the sample folder
    delete this->fDefaultSampleFolder;
    this->fDefaultSampleFolder = nullptr;
  }
  return true;
}

//__________________________________________________________________________________|___________

TQSampleFolder * TSModelBuilder::getSampleFolder(TQFolder * varConfig) {
  // get the sample folder belonging to the given variation
  
  /* get sample folder name (location) */
  TString sampleFolderName;
  if (!varConfig->getTagString("SampleFolder", sampleFolderName)){
    if(this->fDefaultSampleFolder){
      return this->fDefaultSampleFolder;
    } else {
      error(TString::Format(
                            "getSampleFolder(...): Couldn't determine sample folder location for variation '%s'",
                            varConfig->GetName()));
      return NULL;
    }
  }
  
  // temporarily store sample folder name because it might
  // be changed by 'TQStringUtils::removeLeading(...)' and
  // we still need the original string later
  TString tmpSampleFolderName = sampleFolderName;
  
  // load sample folder ...
  TQSampleFolder * samples = NULL;
  Int_t colonpos = sampleFolderName.First(':');
  if (colonpos > 1 && colonpos != kNPOS) {
    // ... either from the default sample folder provided via 'setDefaultSampleFolder(...)' ...
    bool lazy = varConfig->getTagDefault("~lazy",false);    
    info(TString::Format("loadSampleFolder('%s',%d) - opening...",sampleFolderName.Data(),(int)lazy));
    samples = TQSampleFolder::loadSampleFolder(sampleFolderName, lazy);

    if(samples){
      for(auto patch:varConfig->getTagVString("~patches")){
	TString msg;
	if(!samples->importFromTextFile(TQPathManager::getPathManager()->findConfigPath(patch).c_str(),msg)){
	  error(msg);
	}
      }
      
      // replace with the following line for faster dummy-debugging    
      // samples = new TQSampleFolder("samples");
    }
  }
  if(!samples){
    error(TString::Format(
                          "getSampleFolder(...): Unable to open sample folder '%s'",
                          tmpSampleFolderName.Data()));
    return NULL;
  }
  
  return samples;
}


//__________________________________________________________________________________|___________

  Bool_t TSModelBuilder::collectHistograms(
                                         TQFolder * config, TQFolder * model, TQFolder* variation) {

  /* load sample folder */
  TQSampleFolder * samples = getSampleFolder(variation);
  if (!samples) {
    error(TString::Format(
                          "collectHistograms('%s'): Failed to get sample folder",
                          variation->GetName()));
    return false;
  }

  // now collect histograms
  Bool_t success = collectHistograms(config, model, samples, variation);

  // finalize use of sample folder
  if (samples != this->getDefaultSampleFolder()) {
    // delete sample folder since it was loaded
    // from an external file
    delete samples;
  } else {
    // remove temporary tags since the sample folder
    // was taken from the default sample folder
    samples->removeTag(".Variation");
    samples->removeTag(".HistoFromSampleFolder");
  }

  // return true or false depending on whether 'collectHistograms(...)'
  // was successful or not (indicated by its return value)
  return success;
}


//__________________________________________________________________________________|___________

Bool_t TSModelBuilder::parseConversion(TString def, Bool_t &alongX,
                                       Bool_t &includeUnderflowX, Bool_t &includeOverflowX,
                                       Bool_t &includeUnderflowY, Bool_t &includeOverflowY) {

  Bool_t uv1 = TQStringUtils::removeLeading(def, "^", 1);
  TString axis1;
  TQStringUtils::readToken(def, axis1, "XYxy");
  Bool_t ov1 = TQStringUtils::removeLeading(def, "^", 1);

  if (!TQStringUtils::removeLeading(def, ":", 1)) {
    return false;
  }

  Bool_t uv2 = TQStringUtils::removeLeading(def, "^", 1);
  TString axis2;
  TQStringUtils::readToken(def, axis2, "XYxy");
  Bool_t ov2 = TQStringUtils::removeLeading(def, "^", 1);

  if (!def.IsNull()) {
    return false;
  }

  if (axis1.CompareTo("x", TString::kIgnoreCase) == 0
      && axis2.CompareTo("y", TString::kIgnoreCase) == 0) {
    alongX = true;
    includeUnderflowX = uv1;
    includeOverflowX = ov1;
    includeUnderflowY = uv2;
    includeOverflowY = ov2;
    return true;
  } else if (axis1.CompareTo("y", TString::kIgnoreCase) == 0
             && axis2.CompareTo("x", TString::kIgnoreCase) == 0) {
    alongX = false;
    includeUnderflowX = uv2;
    includeOverflowX = ov2;
    includeUnderflowY = uv1;
    includeOverflowY = ov1;
    return true;
  } else {
    return false;
  }
}


//__________________________________________________________________________________|___________

std::vector<int> TSModelBuilder::getRemapping(TQFolder * channelConfig, TQSampleFolder * refSamples,
                                              TString refPath, TString refHistogram, TQTaggable * refHistogramOptions,
                                              Int_t remapXTo, Int_t remapYTo, Int_t &dim, Bool_t remapSlices) {


  /* name of channel */
  TString nameChannel = channelConfig->GetName();

  /* remapping bin border information to return */
  std::vector<int> borders;

  /* ===== try to load/create the remapping information ===== */
  TQFolder * remappingConfig = channelConfig->getFolder("Remapping");
  if (remappingConfig) {
    borders = remappingConfig->getTagVInteger("Borders");

    remappingConfig->getTagInteger("Dim", dim);
    remappingConfig->getTagInteger("RemapXTo", remapXTo);
    if (dim > 1) {
      remappingConfig->getTagInteger("RemapYTo", remapYTo);
    }

  } else {
    /* load remapping reference histogram */
    TQSampleDataReader rd(refSamples);
    rd.setApplyStyles(false);
    //    rd.setVerbose(1);
    TH1 * h_ref = rd.getHistogram(refPath, refHistogram, refHistogramOptions);

    if (h_ref) {

      /* ===== create remapping information ===== */

      dim = TQHistogramUtils::getDimension(h_ref);

      /* check remapping definition */
      if (remapYTo > 0 && dim < 2) {
        error(TString::Format(
                              "getRemapping(...): Cannot remap Y axis of one dimensional histogram in channel '%s'",
                              nameChannel.Data()));
        delete h_ref;
        return borders;
      }
      if (remapXTo > 0 && remapYTo > 0) {
        error(TString::Format(
                              "getRemapping(...): Cannot remap more than one dimension in channel '%s'",
                              nameChannel.Data()));
        delete h_ref;
        return borders;
      }

      /* determine remapping */
      if (dim == 1) {
        borders = TQHistogramUtils::getBinBordersFlat(h_ref, remapXTo, true);
      } else if (dim == 2) {
        if (remapYTo > 0) {
          borders = TQHistogramUtils::getBinBordersFlat2D((TH2*)h_ref, remapYTo, false, true, remapSlices);
        } else if (remapXTo > 0) {
          borders = TQHistogramUtils::getBinBordersFlat2D((TH2*)h_ref, remapXTo, true, true, remapSlices);
        }
      } else {
        error("getRemapping(...): Remapping only supported for one- or two-dimensional histograms");
        delete h_ref;
        return borders;
      }
      delete h_ref;

      if (borders.size()==0) {
        error("getRemapping(...): Somehow failed to determine histogram remapping");
        return borders;
      }

      /* write remapping information to configuration */
      remappingConfig = channelConfig->getFolder("Remapping+");
      for (UInt_t i = 0; i < borders.size(); i++) {
        remappingConfig->setTagInteger(TString::Format("i%d", i), borders.at(i));
      }
      remappingConfig->setTagBool("RemapSlices", remapSlices);
      remappingConfig->setTagInteger("Dim", dim);
      remappingConfig->setTagInteger("RemapXTo", remapXTo);
      if (dim > 1) {
        remappingConfig->setTagInteger("RemapYTo", remapYTo);
      }

    } else {
      error(TString::Format("getRemapping(...): Failed to load remapping reference histogram '%s' for channel '%s' from path '%s' with options '%s'",
                            refHistogram.Data(), nameChannel.Data(), refPath.Data(), refHistogramOptions->exportTagsAsString().Data()));
    }
  }

  /* return remapping bin borders */
  return borders;
}

std::vector<int> TSModelBuilder::getRemappingOptimizedSgnf(TQFolder * channelConfig, TQSampleFolder * refSamples,
                                                           TString sigPath, TString bkgPath, TString histname, TQTaggable * histogramOptions,
                                                           Int_t &dim) {

  /* name of channel */
  TString nameChannel = channelConfig->GetName();

  /* remapping bin border information to return */
  std::vector<int> borders;

  /* parameters for binning optimiziation */
  double minSignal;
  double maxSignal;
  double minBkg;
  double maxBkgUnc;
  bool significanceAgnostic;
  double estimatedMaxSignInBin;
  dim = 1;

  /* ===== try to load/create the remapping information ===== */
  TQFolder * remappingConfig = channelConfig->getFolder("Remapping");
  if (remappingConfig) {

    Int_t id = 0;
    Int_t value = 0;
    while (remappingConfig->getTagInteger(TString::Format("i%d", id++), value)) {
      borders.push_back(value);
    }
    remappingConfig->getTagInteger("Dim", dim);
    remappingConfig->getTagDouble("minSignal", minSignal);
    remappingConfig->getTagDouble("maxSignal", maxSignal);
    remappingConfig->getTagDouble("minBkg", minBkg);
    remappingConfig->getTagDouble("maxBkgUnc", maxBkgUnc);
    remappingConfig->getTagBool("significanceAgnostic", significanceAgnostic);
    remappingConfig->getTagDouble("estimatedMaxSignInBin", estimatedMaxSignInBin);

  } else {
    /* load remapping reference histogram */
    TQSampleDataReader rd(refSamples);
    rd.setApplyStyles(false);
    // rd.setVerbose(1);
    TH1 * hsig = rd.getHistogram(sigPath, histname, histogramOptions);
    TH1 * hbkg = rd.getHistogram(bkgPath, histname, histogramOptions);

    if (hsig || hbkg) {

      /* ===== create remapping information ===== */

      dim = TQHistogramUtils::getDimension(hsig);

      /* check remapping definition */
      if (dim > 1) {
        error(TString::Format(
                              "getRemappingOptimizedSgnf(...): Remapping with significance optimization algorithm is not support "
                              "for >1D histograms!"));
        delete hsig;
        delete hbkg;
        return borders;
      }
      /* determine remapping */

      //@tag:[minSignal, maxSignal, minBkg, maxBkgUnc, significanceAgnostic, estimatedMaxSignInBin] Tags to steer binning optimization algorithm. For more information see the function in TQHistogramUtils.
      dim = channelConfig->getTagIntegerDefault("Dim", 1);
      minSignal = channelConfig->getTagDoubleDefault("rebinMinSignal", 10);
      maxSignal = channelConfig->getTagDoubleDefault("rebinMaxSignal", 30);
      minBkg = channelConfig->getTagDoubleDefault("rebinMinBkg", 10);
      maxBkgUnc = channelConfig->getTagDoubleDefault("rebinMaxBkgUnc", 0.2);
      significanceAgnostic = channelConfig->getTagBoolDefault("significanceAgnostic", false);
      estimatedMaxSignInBin = channelConfig->getTagDoubleDefault("rebinEstimatedMaxSignInBin", -1);

      // @tag:[verboseRebinningAlgorithm] If true, lots of outputs from the binning optimization algorithm will be printed
      borders = TQHistogramUtils::getOptimizedBinBorders(hsig, hbkg, minSignal, minBkg, maxBkgUnc,
                                                            significanceAgnostic, maxSignal, estimatedMaxSignInBin,
                                                            channelConfig->getTagBoolDefault("verboseRebinningAlgorithm", false));
      
      delete hsig;
      delete hbkg;

      if (borders.size()==0) {
        error("getRemappingOptimizedSgnf(...): Somehow failed to determine histogram remapping");
        return borders;
      }

      /* write remapping information to configuration */
      remappingConfig = channelConfig->getFolder("Remapping+");
      for (UInt_t i = 0; i < borders.size(); i++) {
        remappingConfig->setTagInteger(TString::Format("i%d", i), borders.at(i));
      }
      remappingConfig->setTagInteger("Dim", 1);
      remappingConfig->setTagDouble("minSignal", minSignal);
      remappingConfig->setTagDouble("maxSignal", maxSignal);
      remappingConfig->setTagDouble("minBkg", minBkg);
      remappingConfig->setTagDouble("maxBkgUnc", maxBkgUnc);
      remappingConfig->setTagBool("significanceAgnostic", significanceAgnostic);
      remappingConfig->setTagDouble("estimatedMaxSignInBin", estimatedMaxSignInBin);
    } else {
      error(TString::Format("getRemappingOptimizedSgnf(...): Failed to load remapping "
                            "histograms for channel '%s'", nameChannel.Data()));
    }
  }

  /* return remapping bin borders */
  return borders;
}

std::vector<int> TSModelBuilder::getMergeBins(TQFolder * config, TQFolder * channelConfig, std::map<TQFolder*,TH1*> histograms, TString varname, Bool_t isNominal, Int_t &dim){
  // merge bins of a histogram such that no histogram has contributions below mergeBins.minimalBinContent (default: 0), and the stack has no bin below mergeBins.minimalSummedBinContent (default: 1)

  // merged bin border information to return
  std::vector<int> borders;

  /* first try to load an already existing configuration */
  TQFolder * mergeBinsConfig = channelConfig->getFolder("MergeBins");

  /* if exists, load it */
  if (mergeBinsConfig) {
    Int_t id = 0;
    Int_t value = 0;
    while (mergeBinsConfig->getTagInteger(TString::Format("i%d", id++), value)) {
      borders.push_back(value);
    }
    mergeBinsConfig->getTagInteger("Dim", dim);
  } else { // if not, it must be calculated

    /* minimal bin content in induvidual bin after merging */
    Double_t minimalBinContent;

    /* minimal bin content in summed (over samples) bin after merging */
    Double_t minimalSummedBinContent;

    /* bin border that may not be merged */
    std::vector<int> hardBorders;

    /* vector filled with all content from all bins of all induvidual samples */
    std::map< const TString,  std::vector<Double_t> > totalBinContent;

    /* vector filled with all content from all bins of all summed samples */
    std::vector<Double_t> sumBinContent;

    /* read in values from the file */
    minimalBinContent = channelConfig->getTagDoubleDefault("mergeBins.minimalBinContent",0.);
    minimalSummedBinContent = channelConfig->getTagDoubleDefault("mergeBins.minimalSummedBinContent",1.);
    hardBorders = channelConfig->getTagVInt("mergeBins.hardBorders");

    /* loop over all histograms */
    for(auto it:histograms){
      /* vector filled with all content from all bins of a sample */
      std::vector<Double_t> histogramBinContent;

      TQFolder* sampleDef = it.first;
      TString nameSample = sampleDef->GetName();
      TString sampleType = sampleDef->getTagStringDefault("Type","B");          
      // only consider objects starting with "Sample."
      if (!TQStringUtils::removeLeadingText(nameSample, "Sample.")) {
        continue;
      }
      // skip Data if not in Nominal
      if (!isNominal && sampleType=="D"){
        continue;
      }
      // get configuration of sample
      TQFolder * sampleConfig = config->getFolder(TQFolder::concatPaths("Samples", nameSample));
      if (!sampleConfig) {
        error(TString::Format(
                              "collectHistograms('%s'): Failed to load configuration for sample '%s'. "
                              "Skipping this channel ...", varname.Data(), nameSample.Data()));
        continue;
      }

      TH1* histo = it.second;
      dim = TQHistogramUtils::getDimension(histo);
      if(dim != 1){
        throw std::runtime_error("dynamic bin merging currently only supported for one-dimensional histograms!");
      }

      // only merge histograms with more than one bin
      if (histo->GetNbinsX() <= 1) {
        continue;
      }

      // start filling vectors with needed info
      if (sumBinContent.empty()) {
        for (UInt_t i = 0; i < (unsigned) histo->GetNbinsX()+1; i++) { // exclude under and overflow
          histogramBinContent.push_back(histo->GetBinContent(i));
          sumBinContent.push_back(histo->GetBinContent(i));
          if (i != (unsigned) histo->GetNbinsX()){
            borders.push_back(i); // remove borders afterwards when two bins need to get merged
          }
        }
        totalBinContent[nameSample] = histogramBinContent;
      } else {
        for (UInt_t i = 0; i < (unsigned) histo->GetNbinsX()+1; i++) { // exclude under and overflow
          histogramBinContent.push_back(histo->GetBinContent(i));
          sumBinContent[i]+=histo->GetBinContent(i);
        }
        totalBinContent[nameSample] = histogramBinContent;
      }
    }

    // merge to left
    if (!totalBinContent.empty()){
      for (UInt_t iBin = totalBinContent.begin()->second.size()-1; iBin > 0; iBin--) { //run over all bins, ignore most left bin (iBin = 0)
        bool merge = false; // bool that says if one should merge
        for (auto& sample:totalBinContent){ // run over all samples
          if (sumBinContent[iBin] < minimalSummedBinContent || sample.second[iBin] < minimalBinContent) { // if true...
            merge = true; // ..do merge
            for (UInt_t iHardBorder = 0; iHardBorder < hardBorders.size(); iHardBorder++) { // check if by merging one to the left we encounter a hard border
              if (borders[iBin-1] == hardBorders[iHardBorder]){ // if in hardBorders...
                merge = false; // ...do not merge
                break; // quit run over samples if we already know that we cannot merge
              }
            }
            break; // quit run over samples if we already know that we have to merge
          }
        }
        if (merge == true) { // merge only if needed
          borders[iBin-1]=-1; // set border between bin iBin-1 and iBin to -1
          sumBinContent[iBin-1]+=sumBinContent[iBin]; // move content bin to another bin
          sumBinContent[iBin] = 0; // empty old bin
          for (auto& sample:totalBinContent){ // run over all samples
            sample.second[iBin-1]+=sample.second[iBin]; // move content bin to another bin
            sample.second[iBin]=0; // empty old bin
          }
        }
      }
    }

    // merge to right
    if (!totalBinContent.empty()){
      for (UInt_t iBin = 0; iBin < totalBinContent.begin()->second.size()-1; iBin++) { //run over all bins, ignore most right bin (iBin totalBinContent[0].size()-1)
        bool merge = false; // bool that says if one should merge
        for (auto& sample:totalBinContent){ // run over all samples
          if ((sumBinContent[iBin] < minimalSummedBinContent || sample.second[iBin] < minimalBinContent) && sumBinContent[iBin] != 0.) { // if true...
            merge = true; // ..do merge
            for (UInt_t iHardBorder = 0; iHardBorder < hardBorders.size(); iHardBorder++) { // check if by merging one to the left we encounter a hard border
              if (borders[iBin] == hardBorders[iHardBorder]){ // if in hardBorders...
                merge = false; // ...do not merge
                warn(TString::Format("negative bins still present in sample '%s' in bin '%d', cannot merge!",sample.first.Data(),iBin).Data());
                break; // quit run over samples if we already know that we cannot merge
              }
            }
            break; // quit run over samples if we already know that we have to merge
          }
        }
        if (merge == true) { // merge only if needed
          borders[iBin]=-1; // set border between bin iBin and iBin+1 to -1
          sumBinContent[iBin+1]+=sumBinContent[iBin]; // move content bin to another bin
          sumBinContent[iBin] = 0; // empty old bin
          for (auto& sample:totalBinContent){ // run over all samples
            sample.second[iBin+1]+=sample.second[iBin]; // move content bin to another bin
            sample.second[iBin]=0; // empty old bin
          }
        }
      }
    }

    // start erasing borders
    int erasedBins = 0;
    unsigned bordersSize = borders.size();
    for (UInt_t i = 0; i < bordersSize; i++){
      if (borders[i-erasedBins] == -1){
        borders.erase(borders.begin()+i-erasedBins);
        erasedBins++;
      }
    }

    /* write bin merging information to configuration */
    mergeBinsConfig = channelConfig->getFolder("MergeBins+");
    for (UInt_t i = 0; i < borders.size(); i++) {
      mergeBinsConfig->setTagInteger(TString::Format("i%d", i), borders.at(i));
    }
    mergeBinsConfig->setTagInteger("Dim",dim);
  }

  /* return merged bin borders */
  return borders;
}

//__________________________________________________________________________________|___________

Bool_t TSModelBuilder::collectHistograms(TQFolder * config, TQFolder * model, TQSampleFolder * samples, TQFolder* variation) {
  // collect all histograms from the sample folder

  // @tag:[applyStyles] determines if styles should be applied to retrieved histograms. Setting this to false can save a significant amount of time but might make the styling of post-fit plots require more effort. Defaults to true.
  //@tag:[transferRelativeErrorsFromChannel, channelFolderLayer] the errors of the retrieved histogram can be set to the values found in an analog histogram at the channel given by 'transferRelativeErrorsFromChannel'. The 'channelFolderLayer' is a helper tag to specify the layer inside the path at which the channels are specified. This was introdcued for JER varied histograms, which are obtained by adding/subtracting 4 highly correlated histograms. In this case the errors are taken on from the nominal histogram.

  
  bool allowHistogramProcessing = config->getTagBoolDefault("processHistograms",true);
  bool useHistoTags = config->getTagBoolDefault("useHistogramTags",true);
  
  TQFolder* fallbackVariation = NULL;
  TString fallbackVariationName;
  if(variation->getTagString("Fallback",fallbackVariationName)){
    fallbackVariation = variation->getBase()->getFolder(fallbackVariationName);
  }
  // name of variation is given by name of sample folder
  TString varname = variation->GetName();
  Bool_t isNominal = TQStringUtils::equal("Nominal",variation->GetName());
  bool applyStyles = config->getTagBoolDefault("applyStyles", isNominal);

  // the file the sample folder is taken from
  TString sampleFolderName;

  // parameter folder
  TQFolder* parameters = config->getFolder("Parameters");

  // the histogram sample data reader
  TQSampleDataReader rd(samples);
  rd.setApplyStyles(applyStyles);
  // rd.setVerbose(1);

  info(TString::Format("collectHistograms('%s'): Collecting histograms...", varname.Data()));

  //@tag: [Include,Exclude] Inclusion and Exclusion string patterns for systematics  (will be matched to "Channel:Sample")  
  TString filterInclude = variation->getTagStringDefault("Include", "*");
  TString filterExclude = variation->getTagStringDefault("Exclude", "");

  // ===== loop over channels =====
  TQFolderIterator itrChannels(model->getListOfFolders("?"), true);
  while (itrChannels.hasNext()) {
    TQFolder * channelDef = itrChannels.readNext();
    TString nameChannel = channelDef->GetName();
    // skip elements with names not starting with "Channel."
    if (!TQStringUtils::removeLeadingText(nameChannel, "Channel.")) {
      continue;
    }
    // load the configuration of the current channel
    TQFolder * channelConfig = config->getFolder(TQFolder::concatPaths("Channels", nameChannel));
    if (!channelConfig) {
      error(TString::Format("collectHistograms('%s'): Failed to load configuration for "
                            "channel '%s'. Skipping this channel ...", varname.Data(), nameChannel.Data()));
      continue;
    }

    // the name of the input histogram or counter
    TString name;
    bool useCounter = channelConfig->getTagString("Counter",name);
    bool useHistogram = channelConfig->getTagString("Histogram",name);
    if (useHistogram && useCounter){
      // requesting both histogram and counter ==> ERROR
      error(TString::Format(
                            "collectHistograms('%s'): Cannot use both histogram and counter for channel '%s'. "
                            "Skipping this channel ...", varname.Data(), nameChannel.Data()));
      continue;
    }

    channelConfig->getTagBool("useCounter",useCounter);
    useHistogram = useHistogram && !useCounter;

    if (!useHistogram && !useCounter){
      /* neither histogram nor counter provided ==> ERROR */
      error(TString::Format(
                            "collectHistograms('%s'): Neither histogram nor counter provided for channel '%s'. "
                            "Skipping this channel ...", varname.Data(), nameChannel.Data()));
      continue;
    }

    // Remapping of input distribution
    // ===============================

    // the bin borders for histogram remapping
    Int_t dim = 0;
    Int_t remapXTo = channelConfig->getTagIntegerDefault("RemapXTo", 0);
    Int_t remapYTo = channelConfig->getTagIntegerDefault("RemapYTo", 0);
    Bool_t remapSlices = channelConfig->getTagBoolDefault("RemapSlices", true);

    Bool_t rebinToOptimalSgnf = channelConfig->getTagIntegerDefault("rebinToOptimalSgnf", false);

    // the number of bins to remap the histograms to
    if (!useCounter && (remapXTo > 0 || remapYTo > 0 || rebinToOptimalSgnf) ) {
      std::vector<int> borders;

      if ( (remapXTo > 0 || remapYTo > 0) && rebinToOptimalSgnf) {
        warn(TString::Format(
                             "collectHistograms('%s'): configured two types of rebinning mechanisms, "
                             "flat signal rebinning gets preference!", varname.Data()));
        rebinToOptimalSgnf = false;
      }

      // reference path for histogram remapping
      TString refPath;
      if (channelConfig->getTagString("PathRemapFlat", refPath)) {
        refPath = channelConfig->replaceInText(refPath);
        refPath = parameters->replaceInText(refPath);

        // get remapping information
        borders = getRemapping(channelConfig, samples, refPath, name,
                               channelConfig, remapXTo, remapYTo, dim, remapSlices);
      } else {
        if (!rebinToOptimalSgnf) {
          error(TString::Format(
                                "collectHistograms('%s'): No reference path provided for remapping in channel "
                                "'%s'. Skipping remapping ...", varname.Data(), nameChannel.Data()));
        }
      }
      if (rebinToOptimalSgnf) {
        TString sigPath;
        TString bkgPath;
        if (!channelConfig->getTagString("rebinSigPath", sigPath) || !channelConfig->getTagString("rebinBkgPath", bkgPath)) {
          error(TString::Format(
                                "collectHistograms('%s'): No valid reference paths for signal and background provided for significance "
                                "optimized rebinning in channel '%s'. Skipping remapping ...", varname.Data(), nameChannel.Data()));
        }
        sigPath = channelConfig->replaceInText(sigPath);
        sigPath = parameters->replaceInText(sigPath);
        bkgPath = channelConfig->replaceInText(bkgPath);
        bkgPath = parameters->replaceInText(bkgPath);
        borders = getRemappingOptimizedSgnf(channelConfig, samples, sigPath, bkgPath, name,channelConfig, dim);
      }

      if (borders.size()==0) {
        error(TString::Format(
                              "collectHistograms('%s'): Failed to obtain remapping information for channel "
                              "'%s'. Skipping remapping ...", varname.Data(), nameChannel.Data()));
      } else {
        channelDef->setTagList(".Remapping.Bins",borders);
        channelDef->setTagBool(".Remapped", true);
      }
    }

    // loop over samples in current channel
    std::map<TQFolder*,TH1*> histograms;
    std::map<TQFolder*,TString> finalHistoPaths;
    TQFolderIterator itrSamples(channelDef->getListOfFolders("?"), true);
    while (itrSamples.hasNext()) {
      TQFolder * sampleDef = itrSamples.readNext();
      TString nameSample = sampleDef->GetName();
      TString sampleType = sampleDef->getTagStringDefault("Type","B");      
      // only consider objects starting with "Sample."
      if (!TQStringUtils::removeLeadingText(nameSample, "Sample.")) {
        continue;
      }
      // skip Data if not in Nominal
      if (!isNominal && sampleType == "D"){
        continue;
      }

      Bool_t passesInclude = TQStringUtils::matchesFilter(nameChannel + ":" + nameSample, filterInclude, ",", true);
      Bool_t passesExclude = TQStringUtils::matchesFilter(nameChannel + ":" + nameSample, filterExclude, ",", true);
      if (!passesInclude || passesExclude) {
        continue;
      }

      // load the path to take the histogram or counter from and replace
      // potential placeholders using tags from Channel and Parameter configuration
      std::map<TString,TString> paths;
      TQTaggable tags;
      tags.importTags(sampleDef,true,true);
      tags.importTags(parameters,true,true);
      TString defaultPath = sampleDef->getTagStringDefault("Path");
      tags.importTags(variation,true,true);
      paths["path"] = tags.replaceInTextRecursive(defaultPath);
      if(fallbackVariation){
        tags.importTags(fallbackVariation,true,true);        
        paths["fallbackPath"] = tags.replaceInTextRecursive(defaultPath);
      }
      bool hasAlternate = false;     
      for(auto expr:sampleDef->getTagVString("AlternatePath.selectChannels")){
        if(TQStringUtils::matches(nameChannel,expr)){
          hasAlternate=true;
        }
        if(!hasAlternate) break;
        TString alternatePath;
        hasAlternate = sampleDef->getTagString("AlternatePath",alternatePath);
        tags.importTags(variation,true,true);        
        if(hasAlternate){
          alternatePath = tags.replaceInTextRecursive(alternatePath);
          paths["alternatePath"] = alternatePath;
          if(fallbackVariation){
            tags.importTags(fallbackVariation,true,true);                    
            paths["fallbackAlternatePath"] = tags.replaceInTextRecursive(alternatePath);
          }          
        }
      }
      
      TQTaggable finalHistoTags(channelConfig);
      if(useHistoTags){
        sampleDef->exportTags(&finalHistoTags);
        TString prefixChannel = TString::Format("forChannel.%s.", nameChannel.Data());
        sampleDef->exportTags(&finalHistoTags, "", prefixChannel + "*");
        finalHistoTags.renameTags(prefixChannel, "");
        
        TString prefixSample = TString::Format("forSample.%s.", nameSample.Data());
        channelConfig->exportTags(&finalHistoTags, "", prefixSample + "*");
        finalHistoTags.renameTags(prefixSample, "");
      }
      
      
      std::map<TString,TH1*> histos;
      for(auto it:paths){
        TString key(it.first);
        TString path(it.second);
        
        //@tag:[transferRelativeErrorsFromChannel]
        TString pathToTransferFrom;
        TString channelToTransferFrom;
        if(variation->getTagString("transferRelativeErrorsFromChannel", channelToTransferFrom)){
          TQStringUtils::removeLeading(path, "/");
          std::vector<TString> layers = TQStringUtils::tokenizeVector(path, "/");
          if ((int)layers.size() > variation->getTagIntegerDefault("channelFolderLayer", 1)) {
            layers[variation->getTagIntegerDefault("channelFolderLayer", 1)] = channelToTransferFrom;
          } else {
            error(TString::Format("Cannot construct path to transfer from for histogram with name '%s' and path '%s'", name.Data(), path.Data()));
          }
          pathToTransferFrom = TQStringUtils::concatenateVector(layers, "/");
        }

        if (!channelToTransferFrom.IsNull()) finalHistoTags.setTagString("transferRelativeErrorsFromPath", pathToTransferFrom);
       
        TH1 * histo = NULL;
        TList* sfList = new TList();
        
        if (useHistogram) {
          // load the histogram from sample folder structure
          TString finalHistoName(name);
          //finalHistoTags.getTagString("Histogram",finalHistoName);
          finalHistoTags.getTagString(TString::Format("forChannel.%s.Histogram",nameChannel.Data()),finalHistoName);
          histo = rd.getHistogram(path, finalHistoName, &finalHistoTags,sfList);
        } else {
          // load the counter from sample folder structure and convert to a 1-bin histogram
          TString finalHistoName(name);
          finalHistoTags.getTagString("Counter",finalHistoName);
          TQCounter * cnt = rd.getCounter(path, finalHistoName, &finalHistoTags,sfList);
          if (cnt) {
            histo = TQHistogramUtils::counterToHistogram(cnt);
            if (applyStyles) rd.applyStyleToElement(histo,sfList,&finalHistoTags);
            delete cnt;
          }
        }
        delete sfList;

        histos[key] = histo;
      }
      
      int verbosity = parameters->getTagIntegerDefault("verbosity",0);
        
      TH1* histo = histos["path"];
      TH1* althisto = NULL;
      if(hasAlternate) althisto = histos["alternatePath"];
      
      TString origpath = paths["path"];
      TString path = origpath;
      if(!histo && fallbackVariation){
        histo = histos["fallbackPath"];
        if(hasAlternate) althisto = histos["fallbackAlternatePath"];        
        TString fallbackPath = paths["fallbackPath"];        
        if (histo && verbosity>4) warn(TString::Format("collectHistograms('%s'): Failed to load counter '%s' for sample '%s' in channel '%s' from path '%s', using fallback path '%s'", varname.Data(), histo->GetName(),  nameSample.Data(), nameChannel.Data(), path.Data(),fallbackPath.Data()));
        path = fallbackPath;
      }
      
      if(hasAlternate){
        std::vector<int> altBins = sampleDef->getTagVInteger("AlternatePath.bins");
        if(altBins.size() == 0){
          warn(TString::Format("collectHistograms('%s'): Failed to merge alternate histograms for sample '%s' in channel '%s', no bins given!", varname.Data(), nameSample.Data(), nameChannel.Data()));
        } else {
          if(!TQHistogramUtils::replaceBins(histo,althisto,altBins)){
            warn(TString::Format("collectHistograms('%s'): Failed to merge alternate histograms for sample '%s' in channel '%s', histograms incompatible!", varname.Data(), nameSample.Data(), nameChannel.Data()));
          } else {
            info(TString::Format("collectHistograms('%s'): Merged alternate histogram in bins %s for sample '%s' in channel '%s'!", varname.Data(), TQStringUtils::concat(altBins,",").Data(), nameSample.Data(), nameChannel.Data()));
          }
        }
        delete althisto;
      }
      
      if (!histo) {
        error(TString::Format("collectHistograms('%s'): Failed to load histogram '%s' for sample '%s' in channel '%s'. Path given was %s", varname.Data(), name.Data(),  nameSample.Data(), nameChannel.Data(), (fallbackVariation ? origpath + " or " + path : origpath).Data()));
        continue;
      }
      
      // keep track of the histogram's details and where it was taken from
      if (isNominal) {
        sampleDef->setTagString(".HistoDetails.Original",
                                TQHistogramUtils::getDetailsAsString(histo));
        sampleDef->setTagString(".HistoFromSampleFolder", sampleFolderName);
        sampleDef->setTagString(".HistoFromPath", path);
        sampleDef->setTagDouble(".integral.nominal",TQHistogramUtils::getIntegral(histo,true) );
        sampleDef->setTagDouble("Bins",histo->GetNbinsX()*histo->GetNbinsY()*histo->GetNbinsZ());
        sampleDef->importTags(finalHistoTags);
      }
      
      /* the path to this sample's histogram folder */
      TString histoSamplePath = TQFolder::concatPaths(varname, nameChannel);
      histo->SetName(nameSample);
      
      histograms[sampleDef]=histo;
      finalHistoPaths[sampleDef]=path;
    }
    
    // the number of bins to remap the histograms to
    if (!useCounter) {
      Bool_t mergeBinsManual = channelConfig->getTagBoolDefault("mergeBinsManual", false);
      Bool_t mergeBins = channelConfig->getTagBoolDefault("mergeBins", false);
      std::vector<int> borders;
      if(mergeBinsManual && mergeBins){
        error(TString::Format("Cannot both merge bins manually and automatically, refusing to do either!"));
      } else if(mergeBins) {
        if (remapXTo > 0 || remapYTo > 0){
          error(TString::Format("Cannot both remap and merge bins! Remap will be skipped!"));
        }
        // get bin merging information
        borders = getMergeBins(config, channelConfig, histograms, varname, isNominal, dim);
        // save uncertainty under name .Remapping so also ActionImportSystematics can handle it
        channelDef->setTagList(".Remapping.Bins",borders);
        channelDef->setTagBool(".Remapped", true);
      } else if(mergeBinsManual){
        dim = 1;
        borders = channelConfig->getTagVInt("mergeBinsManual.borders");
        channelDef->setTagList(".Remapping.Bins",borders);
        channelDef->setTagBool(".Remapped", true);
      }
    }

    channelDef->setTagInteger("nDim",dim);
    
    /* the path to this sample's histogram folder */
    TString histoSamplePath = TQFolder::concatPaths(varname, nameChannel);

    for(auto it:histograms){
      TQFolder* sampleDef = it.first;
      TString nameSample = sampleDef->GetName();
      TString sampleType = sampleDef->getTagStringDefault("Type","B");
      // only consider objects starting with "Sample."
      if (!TQStringUtils::removeLeadingText(nameSample, "Sample.")) {
        continue;
      }
      // skip Data if not in Nominal
      if (!isNominal && sampleType == "D"){
        continue;
      }

      TH1* histo = it.second;
      if(allowHistogramProcessing){
        TH1* newhisto = processHistogram(config,sampleDef,histo);
        if(newhisto){
          TQFolder* target = this->repository()->getFolder(histoSamplePath+"+");
          TQFolder* before = target->getFolder(".BeforeProcessing+");
          sampleDef->setTagString(".Histogram.BeforeProcessing",TQFolder::concatPaths(before->getPath(),histo->GetName()));
          target->Remove(histo);     
          before->addObject(histo);
          histo = newhisto;
        }
      }
      
      /* store histogram */
      TQFolder* histoFolder = this->repository()->getFolder(histoSamplePath+"+");
      histoFolder->setTagString(TString::Format("Path.%s",histo->GetName()),finalHistoPaths[sampleDef].Data());
      if (!histoFolder->addObject(histo)){
        error(TString::Format("collectHistograms('%s'): Failed to store histogram", varname.Data()));
      }
      
      if (isNominal) {
        // set histogram path in definition in case of variation "Nominal"
        it.first->setTagString("Histo", TQFolder::concatPaths(histoSamplePath, nameSample));
      }
      
      // extract style tags from histogram and write to sample config
      if (applyStyles && !sampleDef->hasMatchingTag("style.*")) {
        TQHistogramUtils::extractStyle(histo, sampleDef, "/");
      }
    }
  }
  return true;
}

TH1* TSModelBuilder::processHistogram(TQFolder* config, TQFolder* sampleDef, const TH1* orighisto){
  // parameter folder
  TQFolder* parameters = config->getFolder("Parameters");

  TH1* histo = NULL;
  
  // @tag:[MinBinContent] If a positive number, ensures a minimum bin content for each histogram bin (default: -1). Tag is read from folder "Parameters".
  // @tag:[ignoreNegative] Can be used with MinBinContent. See definition in TQHistogramUtils::ensureMinimumContent(...) (default: false)
  // @tag:[flipToPositive] Can be used with MinBinContent. See definition in TQHistogramUtils::ensureMinimumContent(...) (default: false)
  // @tag:[assignUncertainty] Can be used with MinBinContent. See definition in TQHistogramUtils::ensureMinimumContent(...) (default: true)
  Double_t minBinContent = parameters->getTagDoubleDefault("MinBinContent", -1.);
  bool ignoreNegative = parameters->getTagBoolDefault("ignoreNegative", false);
  bool flipToPositive = parameters->getTagBoolDefault("flipToPositive", false);
  bool assignUncertainty = parameters->getTagBoolDefault("assignUncertainty", true);
  
  std::vector<int> borders = sampleDef->getTagVInteger("~.Remapping.Bins");                                                        
  int dim = sampleDef->getTagIntegerDefault("~dim",1);
  Int_t remapXTo = sampleDef->getTagIntegerDefault("~RemapXTo", 0);
  Int_t remapYTo = sampleDef->getTagIntegerDefault("~RemapYTo", 0);  
  
  // remap histogram
  if (borders.size()!=0) {
    TH1 * h_remapped = NULL;
    if (dim == 1) {
      h_remapped = TQHistogramUtils::getRebinned(orighisto, borders, true);
    } else if (dim == 2) {
      if (remapYTo > 0) {
        // remapping Y axis of 2D histogram
        h_remapped = TQHistogramUtils::getRemapped2D((TH2*)orighisto, borders, false);
      } else if (remapXTo > 0) {
        // remapping X axis of 2D histogram
        h_remapped = TQHistogramUtils::getRemapped2D((TH2*)orighisto, borders, true);
      }
    }
    if (h_remapped) {
      if(histo) delete histo;
      histo = h_remapped;
    } else {
      warn(TString::Format("processHistograms(): Somehow failed to remap %d-dim histogram '%s' into %d bins ",dim,histo->GetName(),(int)(borders.size()-1)));
    }
  }

  /* convert histogram from 2D to 1D? */
  Int_t dimHisto = TQHistogramUtils::getDimension(histo);
  TString convertTo1DDef = sampleDef->getTagStringDefault("~convertTo1D");
  if (dimHisto == 2 && !convertTo1DDef.IsNull()) {
    Bool_t alongX = false;
    Bool_t inclUVX = false;
    Bool_t inclOVX = false;
    Bool_t inclUVY = false;
    Bool_t inclOVY = false;
    if (!parseConversion(convertTo1DDef, alongX, inclUVX, inclOVX, inclUVY, inclOVY)) {
      warn(TString::Format(
                           "collectHistograms(): Invalid histogram conversion to 1D '%s'",
                           convertTo1DDef.Data()));
    } else {
      TH1 * h_converted = TQHistogramUtils::convertTo1D(
                                                        (TH2*)histo, alongX, inclUVX, inclOVX, inclUVY, inclOVY);
      if (h_converted) {
        sampleDef->setTagBool(".ConvertedTo1D", true);
        if(histo) delete histo;        
        histo = h_converted;
      } else {
        warn("collectHistograms(): Somehow failed to convert histogram to 1D");
      }
    }
  }
  
  // @tag:[warnings.dips] checks for dips in distributions with TQHistogramUtils::getNDips() and throws warning if dips are found.
  if(config->getTagBoolDefault("warnings.dips",true) && TQHistogramUtils::getNDips(histo) > 0) {
    warn(TString::Format("processHistogram(): Histogram for sample '%s' in channel"
                         " '%s' has dips. Please consider using less bins.", sampleDef->getTagStringDefault("~Sample","<unknown>").Data(),sampleDef->getTagStringDefault("~Channel","<unknown>").Data()));
  }
  
  // ensure minimum bin content
  if (minBinContent >= 0.){
    if(!histo) histo = TQHistogramUtils::copyHistogram(orighisto);
    TQHistogramUtils::ensureMinimumBinContent(histo, minBinContent, ignoreNegative, flipToPositive, assignUncertainty);
  }
  return histo;
}

void TSModelBuilder::applyStyle(TQFolder* model, const TString& samplename, TH1* hist){
  TQFolderIterator itr(model->getListOfFolders(TQFolder::concatPaths("?","Sample."+samplename)),true);
  while(itr.hasNext()){
    TQFolder* sample = itr.readNext();
    if(sample->hasMatchingTag("style.*")){
      TQHistogramUtils::applyStyle(hist,sample,"/");
      return;
    }
  }
}
