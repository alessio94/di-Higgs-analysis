#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"

/*<cafdoc name=ExportResults>
  ExportResults
  ===========================
  
  Export a `TQFolder`-result to a file with a given name. This can either be a `.root` or a `.txt` file.
  
  Usage:
  ---------------
  ```
  +ExportResults {
    +HWWRun2GGF{
      <outputFile = "./workspaces/run2-ggf-$(fitLabel)/result-preliminary.root">
    }
  }
  ```

</cafdoc> */

namespace TSBaseActions {

  class ExportResult : public TSStatisticsManager::Action {


    bool execute(TQFolder * config) const override {
      
      TString filename = config->replaceInText(config->getTagStringDefault("outputFile"));
      filename = TQPathManager::getPathManager()->getTargetPath(filename);

      TQFolder* result = results()->getFolder(config->GetName());
      if(!result){
        manager->error(TString::Format("no such result available: '%s'",config->GetName()));
        return false;
      }
      
      manager->info(TString::Format("Exporting result '%s'", result->GetName()));
      
      /* make sure directory exists */
      if (!TQUtils::ensureDirectoryForFile(filename)) {
        manager->error(TString::Format("Failed to create directory for file '%s'", filename.Data()));
        return false;
      }
      
      /* write file */
      if (filename.EndsWith(".root")) {
        if(result->writeToFile(TQPathManager::getPathManager()->getTargetPath(filename).c_str(),true)){
          manager->info(TString::Format("-> to ROOT file '%s'", filename.Data()));
        } else {
          manager->error(TString::Format("-> Failed to write ROOT file '%s'", filename.Data()));
        }
      } else {
        // store name as tag
        result->setTagString("Name", result->GetName());
        
        if (result->exportToTextFile(TQPathManager::getPathManager()->getTargetPath(filename).c_str(),true)) {
          manager->info(TString::Format("-> to text file '%s'", filename.Data()));
        } else {
          manager->error(TString::Format("-> Failed to write text file '%s'", filename.Data()));
        }
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ExportResult(),"ExportResults");
  }
}
    
