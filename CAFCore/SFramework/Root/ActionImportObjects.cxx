#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSStatisticsManager.h"

#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"

/*<cafdoc name=ImportObjects>
  ImportObjects
  ===========================
  
  Import an object from one `RooFit` workspace into another.
  
  Usage:
  ---------------
  ```
  +ImportObjects { 
    +CPeven { 
      +asimovData_SM_odd {
        <source="CPodd">
      }
    } 
  }

  ```
  
  The workspace name specified is the name of the target workspace.
  One or more objects can be given as subfolders, the object names are
  defined by the folder names.  For each object, the source workspace
  can be identified by the tag `source`.

  This is most useful to copy entire datasets or pdfs from one
  workspace to the next, for example, asimov datasets generated in a
  different workspace.

</cafdoc> */

namespace TSBaseActions {

  class ImportObjects : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {
			RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("unable to import objects to workspace '%s': no such workspace!",config->GetName()));
        return false;
      }
      
      manager->info(TString::Format("importing objects to workspace '%s'",workspace->GetName()));
      TQFolderIterator importObjects(config->getListOfFolders("?"),true);
      while(importObjects.hasNext()){
          TQFolder* obj = importObjects.readNext();
          TString source;
          if(!obj->getTagString("source",source)){
            manager->error(TString::Format("unable to import object '%s' without source!",obj->GetName()).Data());
            continue;
          }
          RooWorkspace* sourceWS = dynamic_cast<RooWorkspace*>(workspaces()->getObject(source));
          if(!sourceWS){
            manager->error(TString::Format("unable to access source workspace '%s' to import object '%s'",source.Data(),obj->GetName()).Data());
            continue;
          }
          if(sourceWS == workspace){
            manager->error(TString::Format("cannot import object '%s' from workspace '%s' to itself",obj->GetName(),workspace->GetName()).Data());
            continue;
          }
          TObject* tobj = sourceWS->obj(obj->GetName());
          if(!tobj){
            manager->error(TString::Format("unable to obtain object '%s' from source workspace '%s'",obj->GetName(),source.Data()).Data());
            continue;
          }
          RooAbsData* data = dynamic_cast<RooAbsData*>(tobj);
          RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(tobj);
          RooAbsReal* real = dynamic_cast<RooAbsReal*>(tobj);
          if(data){
            if(workspace->import(*data,RooFit::RecycleConflictNodes())){
              manager->error(TString::Format("error importing RooAbsData '%s' from source workspace '%s'",obj->GetName(),source.Data()).Data());
              continue;
            } else {
              manager->info(TString::Format("imported RooAbsData '%s' from source workspace '%s'",obj->GetName(),source.Data()).Data());
            }
          } else if(pdf){
            if(workspace->import(*pdf,RooFit::RecycleConflictNodes())){
              manager->error(TString::Format("error importing RooAbsPdf '%s' from source workspace '%s'",obj->GetName(),source.Data()).Data());
              continue;
            } else {
              manager->info(TString::Format("imported RooAbsPdf '%s' from source workspace '%s'",obj->GetName(),source.Data()).Data());
            }
          } else if(real){
            if(workspace->import(*real,RooFit::RecycleConflictNodes())){
              manager->error(TString::Format("error importing RooAbsPdf '%s' from source workspace '%s'",obj->GetName(),source.Data()).Data());
              continue;
            } else {
              manager->info(TString::Format("imported RooAbsReal '%s' from source workspace '%s'",obj->GetName(),source.Data()).Data());
            }            
          } else {
            manager->error(TString::Format("object '%s' from source workspace '%s' has unknown type '%s'!",obj->GetName(),source.Data(),tobj->ClassName()).Data());
            continue;
          }
      }
      return true;
		}
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ImportObjects(),"ImportObjects");
  }
}
