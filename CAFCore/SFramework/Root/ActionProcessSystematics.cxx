#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "SFramework/TSSystematicsManager.h"

#include "SFramework/TSStatisticsManager.h"

namespace TSBaseActions {

  class ProcessSystematics : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        return false;
      }

      // get configured systematics
      std::vector<TString> selectedSystematics = config->getTagVString("select.Systematics");
      if(selectedSystematics.empty()) selectedSystematics.push_back("*");
      std::vector<TString> blacklistedSystematics = config->getTagVString("except.Systematics");
      std::vector<TString> selectedSamples = config->getTagVString("select.Samples");
      if(selectedSamples.empty()) selectedSamples.push_back("*");
      std::vector<TString> blacklistedSamples = config->getTagVString("except.Samples");

      TList* allSamples = model->getListOfFolders("?/?");
      if(!allSamples){
        manager->error(TString::Format("no samples in model '%s'",model->GetName()));
        return false;
      }
      
      TSSystematicsManager man;
      TQFolder * histos = model->getFolder(".Histograms+");
      man.setRepository(histos);
      if(config->getTagBoolDefault("includeSystematics",false)){
        man.includeAllSystematics(config,allSamples);
      } 
      if(config->getTagBoolDefault("processSystematics",true)){
        for(auto f : selectedSystematics){
          man.processAllSystematics(config,allSamples,f,blacklistedSamples, blacklistedSystematics);
        }
      }
      
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ProcessSystematics(),"ProcessSystematics");
  }
}
