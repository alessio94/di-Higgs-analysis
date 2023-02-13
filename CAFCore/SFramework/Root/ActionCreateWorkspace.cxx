#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSModelFactory.h"
#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSUtils.h"

#include "RooWorkspace.h"
#include "RooStats/HistFactory/Measurement.h"
#include "RooStats/HistFactory/HistoToWorkspaceFactoryFast.h"
#include "RooStats/ModelConfig.h"

#include "QFramework/TQLibrary.h"

/*<cafdoc name=CreateWorkspaces>
  CreateWorkspaces
  ===========================
  
  Create a `RooFit` workspace from a model of the same name. As all
  the information is already contained in the model, the amount of
  configuration required in minimal.

  Usage:
  ---------------
  ```
  +CreateWorkspaces {
    +Run2GGF2j {
      <logTofile= "./workspaces/myAnalysis/createWS.log">
      <histogramsFile = "./workspaces/myAnalysis/histograms-preFit.root">
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

  class CreateWorkspace : public TSStatisticsManager::Action {

    RooWorkspace * createWorkspace(TQFolder* config) const {
      
      TString modelname = config->getTagStringDefault("model",config->GetName());
      TQFolder * model = models()->getFolder(modelname);

      if(!model){
        manager->error(TString::Format("unable to find model '%s'",modelname.Data()));
        return NULL;
      }

      TSModelFactory mf;
      
      /* write histograms to file */
      Bool_t histoFileWritten= mf.writeHistograms(model,config->getTagStringDefault("histogramsFile","histFactory_tmp.root"));
      if(!histoFileWritten){
        manager->error(TString::Format("Failed to write histograms for model '%s'", model->GetName()));
      }

      // let the TSModelFactory class produce a Measurement object from the model
      // (this is using HistFactory's C++ bindings replacing XML files)
      Measurement * ms = mf.createMeasurement(model);
      if (!ms) {
        manager->error("Failed to create Measurement object");
        return NULL;
      }
      
      // create the RooWorkspace object: this is where the main HistFactory enters the game
      RooWorkspace * workspace = NULL;

      TString outfile;
      bool redirect=config->getTagString("logToFile",outfile);
      if (histoFileWritten) {
        try {
          if(redirect){
            TString fname = config->replaceInText(outfile);
            manager->info(TString::Format("writing build log to '%s'",fname.Data()));
            if(TQLibrary::redirect(TQPathManager::getPathManager()->getTargetPath(fname).c_str())<0){
              manager->error("failed to establish redirection");
            }
          }
          // collect histograms from the ROOT file that has been written only for this purpose
          ms->CollectHistograms();
          // invoke HistFactory to build workspace
          workspace = HistoToWorkspaceFactoryFast::MakeCombinedModel(*ms);
          if(redirect){
            TQLibrary::restore();
          }
        } catch(const std::exception& error){
          if(redirect){
            TQLibrary::restore();
          }
          manager->error(error.what());
          delete ms;
          return NULL;
        }
      }
      
      if (!workspace) {
        manager->error("Failed to create workspace");
        delete ms;
        return NULL;
      } else {
        // apply additional settings on the workspace
        manager->info("revising workspace");
        if(!mf.reviseWorkspace(workspace, model)){
          manager->error("failed to revise workspace!");
        }
        TSUtils::applySettings(config,workspace->allVars(),(RooStats::ModelConfig*)(workspace->obj("ModelConfig")));
        // the workspace gets the same name as the corresponding model
        workspace->SetName(config->GetName());
        // save nominal snapshot
        workspace->saveSnapshot("SnSh_AllVars_Nominal", workspace->allVars());
      }
      delete ms;
      return workspace;
    }
    
    bool execute(TQFolder * config) const override {
           
      RooWorkspace * workspace = createWorkspace(config);
      if (workspace) {
        workspaces()->addObject(workspace);
        return true;
      }     
      return false;
    }
  };

  namespace {
    bool available = TSStatisticsManager::registerAction(new CreateWorkspace(),"CreateWorkspaces");
  }
}
