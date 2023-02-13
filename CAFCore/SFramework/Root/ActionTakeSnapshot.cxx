#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSUtils.h"
#include "SFramework/TSStatisticsManager.h"

#include "RooStats/ModelConfig.h"


/*<cafdoc name=TakeSnapshot>
  TakeSnapshot
  ===========================
  
  Take a snapshot of the current status of a `RooFit` workspace.
  
  Usage:
  ---------------
  ```
  +TakeSnapshot {
    +Morphing {
      +SnSh_SM {
        <base="SnSh_AllVars_Nominal">
        <setPars.AL=1.0>
        <setPars.AT=1.0>
      }
    }
  }
  ```
  
  One or more snapshots can be given as subfolders. Optionally, a
  `base` snapshot can be given. If this is omitted, the current status
  is used (and a warning is emitted).
  
  The tags `setPars`, `constPars` and `floatPars` can be used to edit
  parameters before taking the snapshot.


</cafdoc> */

class ActionTakeSnapshot : public TSStatisticsManager::Action {
  
  bool execute(TQFolder * config) const override {
    RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
    if(!workspace){
      manager->error(TString::Format("unable to take snapshot for '%s': no such workspace!",config->GetName()));
      return false;
    }
    //RooStats::ModelConfig* mc = dynamic_cast<RooStats::ModelConfig*>(workspace->obj(config->getTagStringDefault("modelConfig","ModelConfig")));
    RooArgSet allVars(workspace->allVars());
    TQFolderIterator itr(config->getListOfFolders("?"));
    int count = 0;
    while(itr.hasNext()){
      TQFolder* snsh = itr.readNext();
      if(!snsh) continue;
      TString name = snsh->getTagStringDefault("name",snsh->GetName());
      TString base;
      if(!snsh->getTagString("base",base)){
        manager->warn(TString::Format("no 'base' given for snapshot '%s', using current workspace status instead",name.Data()));
      } else {
        if(!workspace->loadSnapshot(base)){
          manager->error(TString::Format("cannot load base '%s' for snapshot '%s'",base.Data(),name.Data()));
          continue;
        }
      }
      
      TSUtils::setParametersConstant(&allVars,snsh->getTagVString("constPars"),true);
      TSUtils::setParametersConstant(&allVars,snsh->getTagVString("floatPars"),true);
      TSUtils::setParameterValues(&allVars,snsh,"setPars",true);
      TString fitresult;
      if(snsh->getTagString("fitResult",fitresult)){
        TQFolder* result = results()->getFolder(config->GetName());
        TQFolder* myresult = result ? result->getFolder(fitresult) : results()->getFolder(fitresult);
        if(myresult){
          TQFolder* newresult = TSUtils::applyFitResult(&allVars,myresult);
          TString writeFitResult;
          if(snsh->getTagString("writeFitResult",writeFitResult)){
            TString name = TQFolder::getPathTail(writeFitResult);
            newresult->SetName(name);
            TQFolder* target = results()->getFolder(writeFitResult+"+");
            target->addFolder(newresult);
          } else {
            delete newresult;
          }
        } else {
          manager->error(TString::Format("unable to find result '%s'",fitresult.Data()));
        }
      } 
      if ( TSUtils::takeSnapshot(workspace, name, snsh, manager) ) ++count;
    }
    if(count == 0){
      manager->error("no valid configurations found!");
      return false;
    }
    return true;
  }
};
namespace {
  bool available = TSStatisticsManager::registerAction(new ActionTakeSnapshot(),"TakeSnapshot");
}

  
