#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSUtils.h"

#include "RooRealVar.h"

#include "RooStats/ModelConfig.h"

#include "TFile.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

#include <algorithm>

/*<cafdoc name=ImportWorkspaces>
  ImportWorkspaces
  ===========================
  
  Import a `RooFit` workspace from a file with a given name. This must be a `.root` file.
  
  Usage:
  ---------------
  ```
  +ImportWorkspaces {
    +HWWRun2GGF{
      <inputFile = "./workspaces/run2-ggf-$(fitLabel)/workspace-preFit.root:HWWRun2GGF">
      <saveSnapshot="SnSh_AllVars_Nominal">
    }
  }
  ```

  This action supports a wealth of options to modify the workspace while loading. 

  * With `saveSnasphot="somename"`, the initial configuration of the
    workspace can be saved as a snapshot.
  * The following options can be used to edit the lists of nuisance
    parameters and parameters of interest:
    * `removeNPs={"a","b",...}`, `addNPs={"a","b",...}`
    * `removePOIs={"a","b",...}`, `addPOIs={"a","b",...}`
  * The following options can be used to edit `const`/`float`
    properties of parameters:
    * `constPars={"a","b",...}`
    * `floatPars={"a","b",...}`
  * The following options can be used to edit the values and errors of parameters
    *  `setPars.a=1.,setPars.b=2.`
    *  `setParErrors.a=1.,setParErrors.b=1.`
  * By default, the workspace is re-generated to obtain a clean,
    in-memory variant, after which the input file is closed. This can
    be avoided and the in-file representation can be used by using the
    `infile=true` flag. Alternatively, the code can be instructed to
    instead of building a clean copy of the workspace just use the
    ROOT Streamer to clone the workspace including all its
    disconnected elements using the `rebuild=false` flag.
  * The `modelConfig` option can be used to provide the name of the
    `RooStats::ModelConfig` object. This is required when the
    `RooStats::ModelConfig` in the workspace is not called
    "ModelConfig".
  * The option `clearSnapshots=true` will not copy any snapshots into
    the in-memory representation into the workspace. Has no effect
    when `infile=true` is used.

</cafdoc> */

namespace TSBaseActions {

  class ImportWorkspace : public TSStatisticsManager::Action {
    mutable std::vector<TFile*> mFiles;

    bool execute(TQFolder * config) const override {

      if(workspaces()->getObject(config->GetName())){
        manager->error(TString::Format("unable to import workspace with name '%s' - a workspace with that name already exists!",config->GetName()));
        return false;
      }
      
      TString inputname = config->getTagStringDefault("inputFile");
      TString filename,objname;
      TQFolder::parseLocation(inputname,filename,objname);
      TString wsname(config->GetName());
      if(objname.IsNull()) objname = wsname;
      if(filename.IsNull()){
        manager->error(TString::Format("you need to provide the path to the input workspace as  '<inputFile=\"file:workspace\">' in your configuration!'"));
        return false;
      }

      if(workspaces()->getObject(wsname)){
        manager->error(TString::Format("unable to import workspace with object name '%s' from file '%s' - a workspace with name '%s' already exists!",objname.Data(),filename.Data(),wsname.Data()));
        return false;
      }

      DEBUG("opening file '%s'",filename.Data());
      
      TFile * file = TFile::Open(TQPathManager::getPathManager()->getTargetPath(filename).c_str(), "READ");
      if (!file || !file->IsOpen()) {
        manager->error(TString::Format("Failed to open file '%s'", filename.Data()));
        if (file) {
          delete file;
        }
        return false;
      }

      DEBUG("opened file '%s'",filename.Data());
      
      TString mcname = config->getTagStringDefault("modelConfig","ModelConfig");
      bool clearSnapshots = config->getTagBoolDefault("clearSnapshots",false);
      bool checkParameters = config->getTagBoolDefault("checkParameters",true);      
      
      TQKeyIterator itr(file->GetListOfKeys());
      RooWorkspace * workspace = NULL;
      bool keepfile = config->getTagBoolDefault("infile",false);
      while (itr.hasNext() && !workspace) {
        TKey * key = itr.readNext();
        TString keyName = key->GetName();
        DEBUG("looking at object '%s'",keyName.Data());
        if (!TQStringUtils::matchesFilter(keyName, objname)) {
          continue;
        }
        TObject * obj = key->ReadObj();
        DEBUG("loaded object '%s'",keyName.Data());        
        if (!obj->InheritsFrom(RooWorkspace::Class())) {
          delete obj;
          continue;
        }
        /* make sure we don't run into trouble when closing the file */
        RooWorkspace* infile = dynamic_cast<RooWorkspace*>(obj);
        if(keepfile){
          if(TQStringUtils::equal(wsname,infile->GetName())){
            workspace = infile;
          } else {
            manager->error(TString::Format("with infile=true, the workspace name cannot be changed. please change your config workspace name to '%s' or allow for workspace rebuilding!",infile->GetName()));
          }
        } else if(config->getTagBoolDefault("rebuild",true)){
          RooMsgService::instance().saveState();
          RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);

          workspace = TSUtils::makeCleanWorkspace(infile,wsname.Data(),mcname.Data(),!clearSnapshots,checkParameters);

          RooMsgService::instance().restoreState();
        } else {
          if(TQStringUtils::equal(wsname,infile->GetName())){
            workspace = (RooWorkspace*)infile->Clone();
          } else {
            manager->error(TString::Format("with rebuild=false, the workspace name cannot be changed. please change your config workspace name to '%s' or allow for workspace rebuilding!",infile->GetName()));
          }
        }    
      }
      if(workspace){
        manager->info(TString::Format("successfully imported workspace '%s' from '%s:%s'",workspace->GetName(),filename.Data(),objname.Data()));
        
        RooStats::ModelConfig* mc = dynamic_cast<RooStats::ModelConfig*>(workspace->obj(mcname));

        TSUtils::applySettings(config,workspace->allVars(),mc);
        
        TString snapshot;
        if(config->getTagString("saveSnapshot",snapshot)){
          std::vector<TString> snapshots = TSUtils::getListOfSnapshotNames(workspace);
          bool save = true;
          for(const auto& snsh:snapshots) if(snsh == snapshot) save=false;
          if(save){
            manager->info(TString::Format("saving configuration as snapshot '%s'",snapshot.Data()));
            workspace->saveSnapshot(snapshot,workspace->allVars());
          } else {
            manager->warn(TString::Format("not saving snapshot '%s' - a snapshot of this name is already present in the workspace",snapshot.Data()));
          }
        }
        workspaces()->addObject(workspace);
      } else {
        manager->error(TString::Format("unable to import workspace '%s' from '%s:%s'",config->GetName(),filename.Data(),objname.Data()));
      }
      
      if(keepfile){
        this->mFiles.push_back(file);
      } else {
        file->Close();
        delete file;
      }
      
      return workspace;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ImportWorkspace(),"ImportWorkspaces");
  }
}
