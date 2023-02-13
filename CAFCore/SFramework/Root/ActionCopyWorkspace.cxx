#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSUtils.h"
#include "SFramework/TSStatisticsManager.h"

#include "RooRealVar.h"

#include "RooStats/ModelConfig.h"

#include "TFile.h"

#include <algorithm>

/*<cafdoc name=CopyWorkspaces>
  CopyWorkspaces
  ===========================
  
  Copy a `RooFit` workspace to a new name.
  
  Usage:
  ---------------
  ```
  +CopyWorkspaces {
    +NewWS{
      <original="OldWS">
    }
  }
  ```

  This action supports some options to modify the workspace while copying. 

  * With `saveSnasphot="somename"`, the initial configuration of the
    workspace can be saved as a snapshot.
  * The following options can be used to edit the lists of nuisance
    parameters and parameters of interest:
    * `removeNPs={"a","b",...}`, `addNPs={"a","b",...}`
    * `removePOIs={"a","b",...}`, `addPOIs={"a","b",...}`
  * The following options can be used to edit `const`/`float`
    properties of parameters:
    * `constPars={"a","b",...}`
    * `floatPars={"a","b",...}`
  * The following options can be used to edit the values and errors of parameters
    *  `setPars.a=1.,setPars.b=2.`
    *  `setParErrors.a=1.,setParErrors.b=1.`
  * By default, the workspace is re-generated to obtain a clean,
    variant. Alternatively, the code can be instructed to
    instead of building a clean copy of the workspace just use the
    ROOT Streamer to clone the workspace including all its
    disconnected elements using the `clone=true` flag.
  * The `modelConfig` option can be used to provide the name of the
    `RooStats::ModelConfig` object. This is required when the
    `RooStats::ModelConfig` in the workspace is not called
    "ModelConfig". For workspaces without a ModelConfig object, this
    field can be used to identify the top-level pdf.
  * The option `clearSnapshots=true` will not copy any snapshots into
    the in-memory representation into the workspace. Has no effect
    when `infile=true` is used.

</cafdoc> */

namespace TSBaseActions {

  class CopyWorkspace : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {
      TString original = config->getTagStringDefault("original");

      TString mcname = config->getTagStringDefault("modelConfig","ModelConfig");
      bool clearSnapshots = config->getTagBoolDefault("clearSnapshots",false);
      
      RooWorkspace * origWS = dynamic_cast<RooWorkspace*>(workspaces()->getObject(original));
      if(!origWS){
        manager->error(TString::Format("unable to copy workspace '%s': no such workspace!",original.Data()));
        return false;
      }
      
      RooWorkspace* workspace = NULL;
      if(config->getTagBoolDefault("clone",false)){
        manager->info(TString::Format("making clone '%s' of workspace '%s'",config->GetName(),original.Data()));
        workspace = (RooWorkspace*)(origWS->Clone());
        workspace->SetName(config->GetName());
      } else {
        manager->info(TString::Format("making clean copy '%s' of workspace '%s'",config->GetName(),original.Data()));
        workspace = TSUtils::makeCleanWorkspace(origWS,config->GetName(),mcname.Data(),!clearSnapshots);
      }
      
      if(workspace){
        RooStats::ModelConfig* mc = dynamic_cast<RooStats::ModelConfig*>(workspace->obj("ModelConfig"));
        TSUtils::applySettings(config,workspace->allVars(),mc);
        TString snapshot;
        if(config->getTagString("saveSnapshot",snapshot)){
          std::vector<TString> snapshots = TSUtils::getListOfSnapshotNames(workspace);
          bool save = true;
          for(const auto& snsh:snapshots) if(snsh == snapshot) save=false;
          if(save){
            manager->info(TString::Format("saving configuration as snapshot '%s'",snapshot.Data()));
            workspace->saveSnapshot(snapshot,workspace->allVars());
          } else {
            manager->warn(TString::Format("not saving snapshot '%s' - a snapshot of this name is already present in the workspace",snapshot.Data()));
          }
        }
        
        workspaces()->addObject(workspace);
      } else {
        manager->error(TString::Format("unable to copy workspace '%s' from '%s'",config->GetName(),original.Data()));
        return false;
      }
      
      return true;
    }
    
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new CopyWorkspace(),"CopyWorkspaces");
  }
}
