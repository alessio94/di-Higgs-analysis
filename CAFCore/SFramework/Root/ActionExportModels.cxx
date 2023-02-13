#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSModelFactory.h"

#include "TFile.h"

/*<cafdoc name=ExportModels>
  ExportModels
  ===========================
  
  Export a `TQFolder`-model to a file with a given name. This can either be a `.root` or a `.txt` file.
  
  Usage:
  ---------------
  ```
  +ExportModels {
    +HWWRun2GGF{
      <outputFile = "./workspaces/run2-ggf-$(fitLabel)/model-simple.root">
    }
  }
  ```

</cafdoc> */

namespace TSBaseActions {

  class ExportModel : public TSStatisticsManager::Action {


    bool execute(TQFolder * config) const override {
      
      TString filename = config->replaceInText(config->getTagStringDefault("outputFile"));
      filename = TQPathManager::getPathManager()->getTargetPath(filename);
                                               
      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        return false;
      }
      
      manager->info(TString::Format("Exporting model '%s'", model->GetName()));
      
      /* make sure directory exists */
      if (!TQUtils::ensureDirectoryForFile(filename)) {
        manager->error(TString::Format("Failed to create directory for file '%s'", filename.Data()));
        return false;
      }
      
      /* write file */
      if (filename.EndsWith(".root")) {
        if(model->writeToFile(filename,true,1,true)){
          manager->info(TString::Format("-> to ROOT file '%s'", filename.Data()));
        } else {
          manager->error(TString::Format("-> Failed to write ROOT file '%s'", filename.Data()));
        }
      } else {
        // store name as tag
        model->setTagString("Name", model->GetName());
        
        if (model->exportToTextFile(filename)) {
          manager->info(TString::Format("-> to text file '%s'", filename.Data()));
        } else {
          manager->error(TString::Format("-> Failed to write text file '%s'", filename.Data()));
        }
      }

      TString xmlDir;
      if(config->getTagString("writeXML",xmlDir)){
        // make sure directory exists 
        xmlDir = TQPathManager::getPathManager()->getTargetPath(xmlDir).c_str();
        if (TQUtils::ensureDirectory(xmlDir)) {
          TString histosFilename = model->replaceInText(config->getTagStringDefault("histogramsFile","histFactory_tmp.root"));

          // create Measurement object 
          TSModelFactory mf;
          manager->info(TString::Format("writing histograms to '%s'",histosFilename.Data()));
          mf.writeHistograms(model,histosFilename);
          Measurement * ms = mf.createMeasurement(model);
          if (!ms) {
            manager->error("Failed to create Measurement object");
            return false;
          }
          // create XML 
          TString xmldirtmp(xmlDir);
          TQStringUtils::removeLeadingText(xmldirtmp,"./");
          mf.createXML(ms,xmldirtmp,histosFilename);
          delete ms;
          manager->info(TString::Format("-> to XML in directory '%s'", xmlDir.Data()));
        } else {
          manager->error(TString::Format("Failed to create/find directory '%s'", xmlDir.Data()));
        }
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ExportModel(),"ExportModels");
  }
}
    
