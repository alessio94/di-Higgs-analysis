#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"
#include "RooWorkspace.h"

/*<cafdoc name=ExtractModels>
  ExtractModels
  ===========================
  
  Extract a `TQFolder`-model from a workspace.

  Workspaces built with `SFramework` have the original model added to
  the workspace as a simple `TList` of `TObjString` for bookkeeping
  purposes. If this object is still present (and was not stripped from
  the workspace for whatever reason), it can be extracted and put
  together to be a model using this action.

  Usage:
  ---------------
  ```
  +ExtractModels {
    +HWWRun2GGF {
      # name of the input workspace
      <workspace = "Run2GGF">
      # name of the model-object inside the workspace
      <object="Model"> 
    }
  }
  ```
</cafdoc> */

namespace TSBaseActions {

  class ExtractModel : public TSStatisticsManager::Action {
    bool execute(TQFolder * config) const override {
      TString wsname = config->getTagStringDefault("workspace",config->GetName());

      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(wsname));
      if(!workspace){
        manager->error(TString::Format("unable to find workspace '%s': no such workspace!",wsname.Data()));
        return false;
      }
      
      TString objname = config->getTagStringDefault("object",".Model");
      TObject* obj = workspace->obj(objname);

      if(!obj){
        manager->error(TString::Format("unable to find object '%s': in workspace!",workspace->GetName()));
        return false;
      }

      TCollection* c = dynamic_cast<TCollection*>(obj);
      TObjString* s = dynamic_cast<TObjString*>(obj);

      TString text;
      if(c){
        text = TQStringUtils::concat(c,"\n","");
      } else if(s){
        text = s->GetName();
      } else {
        manager->error(TString::Format("unable to extract model from object of type '%s'!",obj->Class()->GetName()));
        return false;
      }
      
      TQFolder* model = TQFolder::newFolder(config->GetName());

      TString msg;
      if(!model->importFromText(text,msg)){
        manager->error(TString::Format("Failed to extract model: %s",msg.Data()));
        return false;
      }
      
      models()->addFolder(model);
      return true;
    }
	};
  namespace {
    bool available = TSStatisticsManager::registerAction(new ExtractModel(),"ExtractModels");
  }
}
