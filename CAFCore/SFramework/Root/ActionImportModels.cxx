#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"

/*<cafdoc name=ImportModels>
  ImportModels
  ===========================
  
  Import a `TQFolder`-model from a file with a given name. This can either be a `.root` or a `.txt` file.

  Usage:
  ---------------
  ```
  +ImportModels {
    +HWWRun2GGF {
      <inputFile = "./workspaces/run2-ggf-$(fitLabel)/model.root">
  
      # in the case of a .root file, it is possible to specify the name of the input object with the tag objectName
      # if no object-name is given, the object name is assumed to be identical to the expected name of the model
      <objectName="HWWRun2GGF"> 
    }
  }
  ```
</cafdoc> */

namespace TSBaseActions {

  class ImportModel : public TSStatisticsManager::Action {
    bool execute(TQFolder * config) const override {
      if(models()->getFolder(config->GetName())){
        manager->error(TString::Format("unable to import model with name '%s' - a model with that name already exists!",config->GetName()));
        return false;
      }
      
      TString filename;
      if(!config->getTagString("inputFile",filename)){
        manager->error("no input file given, please use tag 'inputFile'!");
        return false;
      }
      
      manager->info(TString::Format("Importing models from file '%s'", filename.Data()));
      
      TQFolder* model = NULL;
      if(filename.Contains(".root:")){
        model = TQFolder::loadFolder(TQPathManager::getPathManager()->getTargetPath(filename).c_str());
      } else if(filename.EndsWith(".root")){
        TString objectName = config->getTagStringDefault("objectName",config->GetName());
        model = TQFolder::loadFolder(TQPathManager::getPathManager()->getTargetPath(filename + ":" + objectName).c_str());
      } else {
        model = TQFolder::loadFromTextFile(TQPathManager::getPathManager()->getTargetPath(filename).c_str());
      }
      
      if (!model) {
        manager->error(TString::Format("Failed to import from file '%s'", filename.Data()));
        return false;
      }
      
      // extract name from name tag
      model->setTagString(".OriginalName", model->GetName());
      model->SetName(config->GetName());
      model->setTagString("Name", model->GetName());
      
      models()->addFolder(model);
      return true;
    }
	};
  namespace {
    bool available = TSStatisticsManager::registerAction(new ImportModel(),"ImportModels");
  }
}
