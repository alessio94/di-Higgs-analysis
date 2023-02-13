#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "QFramework/TQLibrary.h"

#include "RooStats/ModelConfig.h"

#include "RooRealVar.h"
#include "RooWorkspace.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSUtils.h"
#include "SFramework/RooFitUtils.h"

#ifdef HAS_RooFitUtils
#pragma message "enabling RooFitUtils::editws"
#include <RooFitUtils/EditWorkspaces.h>
#else
#pragma message "disabling RooFitUtils::editws"
#endif

#include "TFile.h"

/*<cafdoc name=EditWorkspace>
  EditWorkspace
  ===========================

  Edit a workspace using the RooWorkspace.factory function or the `editws`
  functionality of `RooFitUtils` (requires the corresponding library to be
  available).

  Usage:
  ---------------
  ```
  +EditWorkspaces.mergePOIs {
    +HWWRun2GGF {
      # merge some parameters into one
      <merge.muGGF = {"mu_gg2H_*"}>
      <merge.muVBF = {"mu_VBF_*","mu_*_qq2Hqq_*"}>
      <merge.muVH = {"mu_WH_qq2Hlnu_*","mu_ZH_qqHll_*","mu_ZH_ggHll_*"}>
      # add some POIs
      <addPOIs = {"muGGF"}>
      # set some parameters constant
      <constPars = {"muVBF","muVH"}>
    }
  }
  ```

  Supported edits include:
  * merge: merge a set of parameters to one
  * lines: lines of string commands that will be forwarded to the `RooFit::RooWorkspace.factory` function verbatim
  * files: files of string commands that will be forwarded to the `RooFit::RooWorkspace.factory` function verbatim

  The edits will be applied to the workspace, after which a new, clean
  workspace will be created, replacing the old workspace of the same
  name.

  The flag `useRooFitUtils=true` can be used to change the processing
  engine away from the `RooFit::RooWorkspace.factory` to the more
  advanced `editws` engine that is provided within the `RooFitUtils`
  extension package. If that package is not present, this flag will
  trigger a runtime error.

</cafdoc> */


namespace TSBaseActions {

  class EditWorkspace : public TSStatisticsManager::Action {

    RooWorkspace* editWorkspace(TQFolder*config,RooWorkspace* ws) const {
      std::vector<TString> lines = config->getTagVString("lines");
      std::vector<std::string> clines;
      TString mcname = config->getTagStringDefault("ModelConfig","ModelConfig");
      RooStats::ModelConfig* mc = dynamic_cast<RooStats::ModelConfig*>(ws->obj(mcname));
      if(!mc){
        manager->error("unable to get ModelConfig from workspace!");
        return NULL;
      }

      RooAbsPdf* pdf = mc->GetPdf();

      TQIterator itr(config->getListOfKeys("merge.*"),true);
      TString merge = TString::Format("EDIT::%s(%s",pdf->GetName(),pdf->GetName());;
      RooArgSet args(ws->allVars());
      std::vector<TString> allVars;
      TSUtils::getParameterNames(&args,allVars);
      int nmerge = 0;
      while(itr.hasNext()){
        TObject* entry = itr.readNext();
        TString target(entry->GetName());
        TString source;
        config->getTagString(target,source);
        TQStringUtils::removeLeadingText(target,"merge.");
        TString name;
        TQStringUtils::readUpTo(target,name,".");
        RooRealVar* v = NULL;
        for(const auto& p:allVars){
          if(TQStringUtils::matches(p,source)){
            if(!ws->var(name)){
              if(!v){
                v = ws->var(p);
                lines.push_back(TString::Format("%s[%g,%g,%g]",name.Data(),v->getVal(),v->getMin(),v->getMax()));
              }
            }
            nmerge++;
            merge.Append(",");
            merge.Append(p);
            merge.Append("=");
            merge.Append(name);
          }
        }
      }
      merge.Append(")");
      if(nmerge>0) lines.push_back(merge);

      for(const auto& line:lines){
        clines.push_back(line.Data());
      }

      for(auto file:config->getTagVString("files")){
        std::vector<TString>* lines = TQStringUtils::readFileLines(file,1024);
        int i=0;
        for(const auto& line:*lines){
          clines.push_back(line.Data());
          ++i;
        }
        delete lines;
      }

      TString outfile;

#ifdef HAS_RooFitUtils
      bool redirect=config->getTag("logToFile",outfile);
#endif

      RooWorkspace* newws = NULL;

      if(config->getTagBoolDefault("useRooFitUtils",false)){
#ifdef HAS_RooFitUtils
        if(redirect){
          TString fname = config->replaceInText(outfile);
          manager->info(TString::Format("writing edit log to '%s'",fname.Data()));
          TQLibrary::redirect(fname);
        }

        newws = new RooWorkspace(ws->GetName(),ws->GetTitle());
        int nerr = editws(ws,newws,clines,mcname.Data());

        if(redirect){
          TQLibrary::restore();
        }

        if(nerr>0){
          manager->error(TString::Format("grand total of errors encountered: %d",nerr));
        }

#else
        manager->error("unable to edit workspace -- this feature relies on Tim Adye's RooFitUtils package!");
        return NULL;
#endif
      } else {
//        TString editstr = TString::Format("EDIT::%s(%s",pdf->GetName(),pdf->GetName());
//        for(auto s:clines){
//          editstr.Append(",");
//          editstr.Append(s.c_str());
//        }
//        editstr.Append(")");
        manager->info("editing workspace with commands");
        for(auto s:clines){
          manager->info(TString::Format("applying edit: %s",s.c_str()));
          RooAbsArg* arg = ws->factory(s.c_str());
          if(arg){
            arg->Print();
          } else {
            manager->error(TString::Format("unable to commit transaction: %s",s.c_str()));
          }
        }
        manager->info("cleaning up workspace");        
        newws = TSUtils::makeCleanWorkspace(ws,ws->GetName(),mcname,false,false);
      }
      TSUtils::applySettings(config,newws->allVars(),(RooStats::ModelConfig*)(newws->obj(mc->GetName())));

      return newws;
    }

    bool execute(TQFolder * config) const override {
      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("unable to edit workspace '%s': no such workspace!",config->GetName()));
        return false;
      }
      RooWorkspace* ws = editWorkspace(config,workspace);
      if(ws){
        if(ws != workspace){
          workspaces()->Remove(workspace);
          workspaces()->addObject(ws);
          delete workspace;
        }
      } else {
        manager->error(TString::Format("error editing workspace '%s'!",config->GetName()));
        return false;
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new EditWorkspace(),"EditWorkspaces");
  }
}
