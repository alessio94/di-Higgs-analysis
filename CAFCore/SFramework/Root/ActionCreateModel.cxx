#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQPathManager.h"
#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSModelBuilder.h"

/*<cafdoc name=CreateModels>
  CreateModels
  ===========================
  
  Create a `TQFolder` model. Since the model contains the lion share
  of all the configuration information for the fit, the configuration
  for this step is typically quite extensive. Generally, the following
  things need to be defined:
  * Samples: All the samples entering the fit need to be defined,
    announcing the following information:
    * Which name they should have 
    * Which path in the sample folder they should incorporate
    * Whether they are Data, Background or Signal
  * Channels: All the channels (or regions) entering the fit need to
    be defined, announcing the following information:
    * Which name they should have 
    * Which histogram or counter name in the sample folder they should incorporate
  * Systematics and Variations: 
    * Variations are variants of the sample folder that correspond to
      a systematic change, for example, a different generator or a
      changed event weight definition.
    * Systematics are parameters corresponding to systematic
      uncertainties, incorporating one or more of the variations given
      above, for example the "up" and "down" variations of the same
      systematic uncertainty.


  For the example here, it is assumed that the sample folder looks like this:
  ```
  samples
  |\data
  | \allChannels
  |  \.histograms
  |  |\CutSR
  |  | \myVar  # histogram
  |   \CutSR
  |    \myVar  # histogram
  |\bkg
  ||\allChannels
  || \myBkg
  ||  \.histograms
  ||  |\CutSR
  ||  | \myVar  # histogram
  ||   \CutSR
  ||    \myVar  # histogram
  ||\allChannels_someSyst_Up
  || \myBkg
  ||  \.histograms
  ||  |\CutSR
  ||  | \myVar  # histogram
  ||   \CutSR
  ||    \myVar  # histogram
  | \allChannels_someSyst_Dn
  |  \myBkg
  |   \.histograms
  |   |\CutSR
  |   | \myVar  # histogram
  |    \CutSR
  |     \myVar  # histogram
   \sig
    ... (as for bkg) ...
  ```
  

  Usage:
  ---------------
  ```
  +CreateModels {
    # The name of the model
    +Test{
      # The Parameters block is optional, but very useful
      +Parameters {
        <Title = "Test Model">
        <POI = "mu">
        # The luminosity scale factor (here, it is one because input
        # histograms already have been scaled to correct luminosity)
        <Lumi = 1.0>
        # The relative uncertainty on the luminosity scale factor. We
        # model the Lumi with an explicit systematic uncertainty
        # later, so we set it to a small, non-zero number to supprress
        # it.
        <LumiRelErr = 0.0001>
        # The minimum bin content for each histogram bin
        # This is important since zero or negative values do not go
        # well together with probability density functions
        <MinBinContent = 1E-12>
      }
      # In the "Samples" block all samples (individual signal and background contributions as well as
      # data) have to be listed.
      +Samples {
        # This is the observed data. Please note that unlike all other
        # samples the name of the data sample has to be exactly "Data"
        +Data {
        	<Type = D, Path = "data/?">
        }
        # This is the signal contribution (here, individual signal
        # contributions are summed to get the total signal contribution)
        +sig {
    	    <Type = S, Path = "sig/$(variation)">
    	    +NormFactor.mu {
    	      <Val = 1., Low = -50, High = 50., Const = false>
    	    }
        }
        # This is the background contribution (here, individual signal
        # contributions are summed to get the total signal contribution)
        +bkg {
    	    <Type = B, Path = "bkg/$(variation)">
    	    +NormFactor.norm_bkg {
    	      <Val = 1., Low = -50, High = 50., Const = false>
    	    }
        }
        # The following lines activates the MC statistical uncertainty only for background.
        <ActivateStatError = true> @ bkg;
        # We add a global normalization factor to all samples to model the Lumi uncertainty
        @ ? {
          +OverallSys.ATLAS_LUMI {
            <Val = 1., Low = 0.98, High = 1.02> 
          }
        }
      }
      # In the "Channels" block all channels (regions, e.g. signal and control regions) have to be listed.
      +Channels {
        +SR {
    	    <Histogram = "CutSR/MyVar">
        }
        +CR {
    	    <Counter = "CutCR">
        }
      }
      # In the "Variations" block, all the variations can be listed
      # Each variation contains a "variation" tag that will replace
      # the "$(variation)" placeholder in the sample paths above.
      +Variations {
        +Nominal {
          <variation="">
        }
        +SomeSystematicUp {
          <variation="someSyst_Up">
        }
        +SomeSystematicDown {
          <variation="someSyst_Dn">
        }
      }
      +Systematics {
        +SomeSyst {
          # Put the two variations of SomeSystematic together to make up one nuisance parameter
          # Set this to be a normalization (overall) systeamtic without a shape (HistoSys) component
          <Up = "SomeSystematicUp", Down="SomeSystematicDown", IsOverallSys=true, IsHistoSys=false>
        }
      }
    }
  }
  ```

  Internally, this action uses the `HistFactory::Measurement` class to
  generate the workspace, which requires the histograms in an external
  file. Hence, the `histogramsFile` tag can be used to define the name
  of the temporary file to be written. This file is an *output* of
  this step, and can be used for third-party processing.

</cafdoc> */

namespace TSBaseActions {
  class CreateModel : public TSStatisticsManager::Action {

    virtual bool execute(TQFolder* modelConfig) const override {
      manager->info(TString::Format("Creating model '%s' ",modelConfig->GetName()));
      
      /* get TSModelBuilder configuration */
      if (modelConfig) {
        /* copy workspace configuration */
        modelConfig = modelConfig->copy();
        /* copy input file tag */
        TString inputFile;
        if (modelConfig->getTagString("inputFile", inputFile)) {
          modelConfig->setTagString(".InputFile", inputFile, "Parameters");
        }
        /* copy name tag */
        modelConfig->setTagString("Name",modelConfig->getTagStringDefault("name", modelConfig->GetName()), "Parameters");
        modelConfig->exportTags(modelConfig, "Parameters", ".mass");
      } else {
        manager->error("createModel(...): Missing folder 'ModelConfiguration'");
        return NULL;
      }
      
      /* create model builder */
      TSModelBuilder modelBuilder;
      
      // set its default sample folder
      modelBuilder.setDefaultSampleFolder(manager->getDefaultSampleFolder());
      
      /* make a copy of the model configuration before model building */
      TQFolder * modelConfigPreBuild = modelConfig->copy();
      
      /* prepare model */
      TQFolder * model = modelBuilder.createDefinition(modelConfig);
      
      if (!model) {
        manager->error("Failed to prepare model");
        delete modelConfig;
        delete modelConfigPreBuild;
        return NULL;
      }

      /* include model configuration in model */
      model->addObject(modelConfig, "::.Configuration");
      model->addObject(modelConfigPreBuild, "::.Configuration.PreBuild");
      
      /* finalize model */
      bool ok = true;
      if (!modelBuilder.finalizeModel(model, modelConfig)) {
        manager->error("Failed to finalize model");
        ok = false;
      }
      
      models()->addFolder(model);

      return ok;
    }
  };

  namespace {
    bool available = TSStatisticsManager::registerAction(new CreateModel(),"CreateModels");
  }

  class ReCreateModel : public TSStatisticsManager::Action {

    virtual bool execute(TQFolder* config) const override {
      manager->info(TString::Format("Recreating model '%s' ",config->GetName()));

      TString source;
      if(!config->getTagString("source",source)){
        manager->error("recreateModel(...): Missing tag 'source'");
        return NULL;
      }

      
      TQFolder* origModel = models()->getFolder(source);
      if(!origModel){
        manager->error("createModel(...): Unable to retrieve configuration from original model!");
        return NULL;
      }
      TQFolder* origConfig = origModel->getFolder(".Configuration.PreBuild");
      if(!origConfig){
        manager->error("createModel(...): Unable to retrieve configuration from original model!");
        return NULL;
      }
      
      /* create model builder */
      TSModelBuilder modelBuilder;
      
      /* include model configuration in model */
      TQFolder* newConfig = origConfig->copy();

      TString patch;
      if(config->getTagString("patch",patch)){
        TString msg;
        manager->info(TString::Format("applying edits from file '%s'",patch.Data()));
        if(!config->importFromTextFile(TQPathManager::getPathManager()->findConfigPath(patch).c_str(),msg)){
          manager->error(msg);
        }
      }
      
      newConfig->setTagBool("collectHistograms",false);
      TQFolder* newOrigConfig =newConfig->copy();

      /* prepare model */
      TQFolder * model = modelBuilder.createDefinition(newConfig);
      if (!model) {
        manager->error("Failed to prepare model");
        return NULL;
      }
      
      model->addObject(newConfig, "::.Configuration");
      model->addObject(newOrigConfig->copy(), "::.Configuration.PreBuild");

      manager->info(TString::Format("Copying histograms for model '%s' ",config->GetName()));      
      if(model->getFolder(".Histograms")){
        TQFolder* f = model->getFolder(".Histograms");
        f->detachFromBase();
        delete f;
      }
      TQFolder* orighistos = origModel->getFolder(".Histograms");
      TQFolder* sys = orighistos->getFolder("Systematics");
      if(sys) sys->detachFromBase();
      TQFolder* histos = orighistos->copy();
      orighistos->addFolder(sys);
      model->addObject(histos);

      /* finalize model */
      bool ok = true;
      if (!modelBuilder.finalizeModel(model, newConfig)) {
        manager->error("Failed to finalize model");
        ok = false;
      }
      
      models()->addFolder(model);

      return ok;
    }
  };

  namespace {
    bool available_2 = TSStatisticsManager::registerAction(new ReCreateModel(),"ReCreateModels");
  }  
}

