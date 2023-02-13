#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"
#include "RooWorkspace.h"

/*<cafdoc name=ExportWorkspaces>
  ExportWorkspaces
  ===========================
  
  Export a `RooFit` workspace to a file with a given name. This must be a `.root` file.
  
  Usage:
  ---------------
  ```
  +ExportWorkspaces {
    +HWWRun2GGF{
      <outputFile = "./workspaces/run2-ggf-$(fitLabel)/workspace-preFit.root">
    }
  }
  ```

</cafdoc> */


namespace TSBaseActions {

  class ExportWorkspace : public TSStatisticsManager::Action {


    bool execute(TQFolder * config) const override {
      TString filename = config->getTagStringDefault("outputFile");
      
      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("unable to export workspace '%s': no such workspace!",config->GetName()));
        return false;
      }
      if(exportWorkspace(workspace, filename)) return true;
      return false;
    }

    bool exportWorkspace(RooWorkspace * workspace, TString filename) const {

      /* parameter to be used in filename */
      TQTaggable tags;
      tags.setTagString("Name", workspace->GetName());
      filename = tags.replaceInText(filename);
      filename = TQPathManager::getPathManager()->getTargetPath(filename);
      
      manager->info(TString::Format("Exporting workspace '%s'", workspace->GetName()));
      
      /* make sure directory exists */
      if (TQUtils::ensureDirectoryForFile(filename)){
        workspace->writeToFile(TQPathManager::getPathManager()->getTargetPath(filename).c_str(),true);
        manager->info(TString::Format("-> to ROOT file '%s'", filename.Data()));
        return true;
      } else {
        manager->error(TString::Format("Failed to create directory for file '%s'", filename.Data()));
        return false;
      }
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ExportWorkspace(),"ExportWorkspaces");
  }
}
