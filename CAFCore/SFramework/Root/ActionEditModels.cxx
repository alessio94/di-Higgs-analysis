#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSModelBuilder.h"

/*<cafdoc name=EditModels>
  EditModels
  ===========================
  
  Edit a model using some `TQFolder` edit commands. These can either be
  supplied in-place using the tag `commands`, or from some internal
  file using the tag `file`.
  
  Usage:
  ---------------
  ```
  +EditModels.systematics {
    +HWWRun2GGF {
      #remove shape component for processes which have low stats/relevant artifacts
      +ImportEdits.removeExpShape {
        <file = "./config/statistics/ggf/snippets/edit.removeHistoSys.txt">
      }
      # remove systematics on the VH samples
      +ImportEdits.removeVHSystematics {
        <commands = {"@Channel.* { @Sample.sigVH*jet* { $delete(OverallSys.*!); $delete(HistoSys.*!); } }"}>
      }
    }
  }
  ```

  The edits will be made in-place on the model.

</cafdoc> */

namespace TSBaseActions {

  class EditModel : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        return false;
      }

      auto edits = config->getTagVString("applyEdits");
      for(auto edit:edits){
        manager->info(TString::Format("applying edit '%s'",edit.Data()));
        model->importFromText(edit);
      }

      TCollection * c = config->getListOfFolders("ImportEdits*");
      TQFolderIterator itr(c,true);
      while(itr.hasNext()){
        TQFolder* edit = itr.readNext();
        if(!edit) continue;
        TSModelBuilder::applyEdits(edit,model);
      }

      model->sortByNameRecursive();      
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new EditModel(),"EditModels");
  }
}
