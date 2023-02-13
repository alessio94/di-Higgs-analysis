#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"

/*<cafdoc name=MergeResults>
  MergeResults
  ===========================
  
  Merge several results into one.

  Usage:
  ---------------
  ```
  +MergeResults {
    +MergedResult {
      <inputs = {"Result1","Result2",...}>
    }
  }
  ```
</cafdoc> */

namespace TSBaseActions {

  class MergeResults : public TSStatisticsManager::Action {
    bool execute(TQFolder * config) const override {
      TQFolder * result = results()->getFolder(TString::Format("%s+",config->GetName()));
      std::vector<TString> inputs = config->getTagVString("inputs");
      TString errMsg;
      TQFolderIterator itr(results()->getListOfFolders());
      while(itr.hasNext()){
        TQFolder* component = itr.readNext();
        bool match = false;
        for(const auto& s:inputs){
          if(TQStringUtils::matches(component->GetName(),s)) match = true;
        }
        if(!match) continue;
        manager->info(TString::Format("merging '%s' into '%s'",component->GetName(),result->GetName()));
        if(!result->merge(component,false)){
          manager->error(TString::Format("unable to merge '%s' into '%s'",component->GetName(),result->GetName()));
        }
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new MergeResults(),"MergeResults");
  }
}
