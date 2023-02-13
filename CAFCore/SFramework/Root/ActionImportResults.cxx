#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"

/*<cafdoc name=ImportResults>
  ImportResults
  ===========================
  
  Import a `TQFolder`-result from a file with a given name. This can either be a `.root` or a `.txt` file.

  Usage:
  ---------------
  ```
  +ImportResults {
    +HWWRun2GGF {
      <inputFile = "./workspaces/run2-ggf-$(fitLabel)/result.root">
  
      # in the case of a .root file, it is possible to specify the name of the input object with the tag objectName
      # if no object-name is given, the object name is assumed to be identical to the expected name of the result
      <objectName="HWWRun2GGF"> 
    }
  }
  ```
  To import multiple (partial) results the ```inputFile``` tag can be a comma separated string specifying the location of more than one result file.
</cafdoc> */

namespace TSBaseActions {

  class ImportResult : public TSStatisticsManager::Action {
    bool execute(TQFolder * config) const override {
      /*
      if(results()->getFolder(config->GetName())){
        manager->error(TString::Format("unable to import result with name '%s' - a result with that name already exists!",config->GetName()));
        return false;
      }
      */
      
      TString filenamesString;
      if(!config->getTagString("inputFile",filenamesString)){
        manager->error("no input file given, please use tag 'inputFile'!");
        return false;
      }
      std::vector<TString> filenames = TQStringUtils::split(filenamesString,",");
      for (const TString& fname : filenames) {
        TString filename = TQStringUtils::trim(fname);
        manager->info(TString::Format("Importing results from file '%s'", filename.Data()));

        TQFolder* result = NULL;
        if(filename.Contains(".root:")){
          result = TQFolder::loadFolder(TQPathManager::getPathManager()->getTargetPath(filename).c_str());
        } else if(filename.EndsWith(".root")){
          TString objectName = config->getTagStringDefault("objectName",config->GetName());
          result = TQFolder::loadFolder(TQPathManager::getPathManager()->getTargetPath(filename + ":" + objectName).c_str());
        } else {
          result = TQFolder::loadFromTextFile(TQPathManager::getPathManager()->getTargetPath(filename).c_str());
        }

        if (!result) {
          manager->error(TString::Format("Failed to import from file '%s'", filename.Data()));
          return false;
        }
        
        // extract name from name tag
        result->setTagString(".OriginalName", result->GetName());
        result->SetName(config->GetName());
        result->setTagString("Name", result->GetName());
        TQFolder* existingResults = results()->getFolder(result->GetName());
        if (existingResults) {
          existingResults->merge(result);
          delete result;
        } else {
          results()->addFolder(result);
        }
      }
      return true;
    }
	};
  namespace {
    bool available = TSStatisticsManager::registerAction(new ImportResult(),"ImportResults");
  }
}
