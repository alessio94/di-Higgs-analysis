#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSStatisticsCalculator.h"
#include "SFramework/TSBreakdownCalculator.h"
#include "SFramework/TSImpactCalculator.h"
#include "SFramework/TSUtils.h"
#include "TFile.h"

/*<cafdoc name=WriteCalculateBreakdown>
  WriteCalculateBreakdown
  ===========================
  
  This is a batch-parallelized version of the `CalculateBreakdown` action.
  Instead of performing all the fits locally, it will write out a
  directory with a copy of the workspaces as well as configs to submit
  jobs fitting the individual points to a batch system.
  
  The unconditional fit will be run first locally, only after that the
  config files will be written to disk - so unless an unconditional
  snapshot exists already, also this version can take some time to
  perform.
  
  Usage:
  ---------------
  ```
  +WriteCalculateBreakdown {
    +Morphing {
      # compute breakdown for the fit to asimov
      +asimov { 
        <outputPath = "./workspaces/run2-comb-$(fitLabel)/Batch/Breakdowns/asm"> # output path for all the configs
        <datasetName="asimovData_SM">
        <invert=false>
      }   
      +asimovinvert {
        <outputPath = "./workspaces/run2-comb-$(fitLabel)/Batch/Breakdowns/asminv"> # output path for all the configs
        <datasetName="asimovData_SM">
        <invert=true>
      } 

      # in case this action has been executed on this sets of inputs
      #  previously, this action will instruct the framework to re-use
      #  the workspace file instead of recomputing the unconditional
      #  fit.
      <allowReuse=true> 

      @ ? {
        # all the options supported for the usual CalculateBreakdown action
      }
    }
  }
  ```

  In order to collect the results, use the `CollectBreakdown` action.

</cafdoc> */

/*<cafdoc name=CollectBreakdown>
  CollectBreakdown
  ===========================
  
  This is the result collection for the batch-parallelized version of the `CalculateBreakdown` action.
  
  Usage:
  ---------------
  ```
  +CollectBreakdown.asimov {
    +Morphing {
        +asimovinvert {
            <inputPath = "./workspaces/run2-comb-$(fitLabel)/Batch/Breakdowns/asimov"> # input path for all the results
        }
    }
  }
  ```

  The results are produced with the `WriteCalculateBreakdown` action.

</cafdoc> */

/*<cafdoc name=WriteCalculateImpacts>
  WriteCalculateImpacts
  ===========================
  
  This is a batch-parallelized version of the `CalculateImpacts` action.
  Instead of performing all the fits locally, it will write out a
  directory with a copy of the workspaces as well as configs to submit
  jobs fitting the individual points to a batch system.
  
  The unconditional fit will be run first locally, only after that the
  config files will be written to disk - so unless an unconditional
  snapshot exists already, also this version can take some time to
  perform.
  
  Usage:
  ---------------
  ```
  +WriteCalculateImpacts {
    +Morphing {
      # compute breakdown for the fit to asimov
      +asimov { 
        <outputPath = "./workspaces/run2-comb-$(fitLabel)/Batch/Impacts/asm"> # output path for all the configs
        <datasetName="asimovData_SM">
        <workspaceFileName="workspace.root"> #optional, defaults to workspace.root
        <invert=false>
      }   
      +asimovinvert {
        <outputPath = "./workspaces/run2-comb-$(fitLabel)/Batch/Impacts/asminv"> # output path for all the configs
        <datasetName="asimovData_SM">
        <invert=true>
      } 

      # in case this action has been executed on this sets of inputs
      #  previously, this action will instruct the framework to re-use
      #  the workspace file instead of recomputing the unconditional
      #  fit.
      <allowReuse=true> 

      @ ? {
        # all the options supported for the usual CalculateImpacts action
      }
    }
  }
  ```

  In order to collect the results, use the `CollectImpacts` action.

</cafdoc> */

/*<cafdoc name=CollectImpacts>
  CollectImpacts
  ===========================
  
  This is the result collection for the batch-parallelized version of the `CalculateImpacts` action.
  
  Usage:
  ---------------
  ```
  +CollectImpacts.asimov {
    +Morphing {
        +asimovinvert {
            <inputPath = "./workspaces/run2-comb-$(fitLabel)/Batch/Breakdowns/asimov"> # input path for all the results
        }
    }
  }
  ```

  The results are produced with the `WriteCalculateImpacts` action.

</cafdoc> */

namespace TSBaseActions {

  template<class BaseCalculator> class WriteImpactBreakdownConfig : public TSStatisticsManager::Action {
    const TString actionname;
    const TString component;
    const TString subcomponent;

  public:
    WriteImpactBreakdownConfig(const TString& aname, const TString& comp, const TString& subcomp) :
      actionname(aname),
      component(comp),
      subcomponent(subcomp)
    {
      // do nothing
    }
		
    bool execute(TQFolder * config) const override {
      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));

      if(!workspace){
        manager->error(TString::Format("no such workspace available: '%s'",config->GetName()));
        return false;
      }
      
      TQFolder * result = results()->getFolder(TString::Format("%s+",config->GetName()));

      int nOK = 0;
      TQFolderIterator itr(config->getListOfFolders("?"));
      while(itr.hasNext()){
        TQFolder* cfg = itr.readNext();
        manager->info(TString::Format("Calculating breakdown '%s' on workspace '%s'",cfg->GetName(),config->GetName()));
        nOK += writeConfigs(workspace,cfg,result);
      }
      return nOK>0;
    }

			
    bool writeConfigs(RooWorkspace* ws, TQFolder* config, TQFolder* result) const {
      BaseCalculator calc(ws,result->getFolder("Snapshots+"));
      const RooArgSet * nuis	= calc.getNuisanceParameters();

      RooArgSet *	listOfPOIs	= calc.getPOIs(config);
      if (!listOfPOIs) {
        manager->error("no POIs found!");
        return false;
      }

      TString output = TQFolder::concatPaths("batch",actionname);
      config->getTagString("~outputPath",output);
      output = TQPathManager::getPathManager()->getTargetPath(output);
      TQUtils::ensureDirectory(TQFolder::concatPaths(output,config->GetName(),"fitlogs"));
      TQUtils::ensureDirectory(TQFolder::concatPaths(output,config->GetName(),"configs"));
      TQUtils::ensureDirectory(TQFolder::concatPaths(output,config->GetName(),"results"));
      manager->info(TString::Format("creating configuration for '%s' in '%s'",config->GetName(),output.Data()));
      
      TString workspaceFileName = "workspace.root";
      config->getTagString("~workspaceFileName",workspaceFileName);
      TString snapshotfile = TQFolder::concatPaths(output,config->GetName(),"snapshots.txt");			
      TString filename = TQFolder::concatPaths(output,config->GetName(),workspaceFileName);

      TQFolder* resulttarget = result->getFolder(TQFolder::concatPaths(component,config->GetName(),calc.fFolderName)+"+");
      TQFolder* snapshots = result->getFolder("Snapshots+");

      if(!config->getTagBoolDefault("~allowReuse",false) || !TQUtils::fileExists(snapshotfile) || !TQUtils::fileExists(snapshotfile)){
        calc.runPreFit(config,listOfPOIs,resulttarget);
        
        snapshots->detachFromBase();
        TQFolder* tmp = new TQFolder("uncondresult");
        tmp->addFolder(snapshots);
        tmp->exportToTextFile(TQPathManager::getPathManager()->getTargetPath(snapshotfile).c_str());
        snapshots->detachFromBase();
        result->addFolder(snapshots);
        delete tmp;

        ws->writeToFile(TQPathManager::getPathManager()->getTargetPath(filename).c_str());
      } else {
        manager->warn(TString::Format("reusing files '%s' and '%s' from previous run!",filename.Data(),snapshotfile.Data()));
      }  

      TQFolder* outconfig = new TQFolder("config");
      outconfig->setTagString("expectedInput",filename);      
      TQFolder* import = outconfig->getFolder(TQFolder::concatPaths("ImportWorkspaces",ws->GetName())+"+");
      import->setTagString("inputFile",filename+":"+ws->GetName());
      import->setTagBool("infile",true);
				
      TQFolder* action = outconfig->getFolder(TQFolder::concatPaths(this->actionname,ws->GetName(),config->GetName())+"+");

      TQFolder* exprt = outconfig->getFolder(TQFolder::concatPaths("ExportResults",ws->GetName())+"+");
			
      TQTaggable options(config);
      TSUtils::expandKeys(nuis,config);
			
      TQIterator itrGroups(config->getListOfKeys("group.*"), true);
      while (itrGroups.hasNext()) {
        action->clearTags();
        exprt->clearTags();
        TString name = itrGroups.readNext()->GetName();
        action->importTags(options);
        //cleanup of tags
        action->removeTags("config.group.*"); //remove all group/singles specific options
        action->removeTags("config.singles.*");
        options.exportTags(action,"","config."+name+".*");//re-add only the applicable ones
        //action->importTagsWithPrefix(options,"config."+name); //re-import only the applicable ones //note: this method doesn't work as one might expect... it imports everything but prepends the prefix to all keys...
        //remove all group and singles definitions, will re-add only the currently treated one afterwards.
        action->removeTags("group.*");
        action->removeTags("singles.*");
        //"move" the config.* ones to their proper namings (the filtering for suitable ones was already done before when exporting the tags from 'options'
        //action->importTagsWithoutPrefix(action,"config.");
        
        action->setTagString("fit.logToFile",TQFolder::concatPaths(output,config->GetName(),"fitlogs",name)+".log");
        
        TString filter = config->getTagStringDefault(name, "!*");
        action->setTagString(name,filter);//re-add the group/single that we're interested in
        
        TString resultfile = TQFolder::concatPaths(output,config->GetName(),"results",name)+".txt";
        exprt->setTagString("outputFile",resultfile);
        outconfig->setTagString("expectedOutput",resultfile);
        TString outconfigfile = TQFolder::concatPaths(output,config->GetName(),"configs",name)+".txt";
        outconfig->setTagString("identifier",TString::Format("%s_%s_%s",actionname.Data(),config->GetName(),name.Data()));        
        if(outconfig->exportToTextFile(TQPathManager::getPathManager()->getTargetPath(outconfigfile).c_str())){
          manager->info(TString::Format("wrote %s",outconfigfile.Data()));
        } else {
          manager->warn(TString::Format("error writing %s",outconfigfile.Data()));
        }
      }
      delete outconfig;
      delete listOfPOIs;
      return true;
    }
  };

  class CollectImpactBreakdownResults : public TSStatisticsManager::Action {

    const TString component;
    const TString subcomponent;

  public:
    CollectImpactBreakdownResults(const TString& comp, const TString& subcomp) :
      component(comp),
      subcomponent(subcomp)
    {
      // do nothing
    };
		
    bool execute(TQFolder * config) const override {
			
      TString configname(config->GetName());
      TQFolder* resultset = results()->getFolder(configname+"+");
      if(!resultset){
        manager->error(TString::Format("no such result available: '%s'",config->GetName()));
        return false;
      }
      resultset->detachFromBase();
			
      TQFolderIterator versions(config->getListOfFolders("?"),true);
      while(versions.hasNext()){
        TQFolder* version = versions.readNext();
        if(!version) continue;
				
        TString path;
        if(!version->getTagString("inputPath",path)){
          manager->error(TString::Format("need to provide input path for '%s', skipping!",version->GetName()));
          continue;
        }
        
        manager->info(TString::Format("reading input from '%s'",path.Data()));
      
        TString resultfiles = TQFolder::concatPaths(TQPathManager::getPathManager()->getTargetPath(path),"results");
        TQStringUtils::ensureTrailingText(resultfiles,"/*.txt");
        TCollection* list = TQUtils::ls(resultfiles);
        if(!list || list->GetEntries() == 0) {
          manager->error(TString::Format("no files found under '%s'!",resultfiles.Data()));
          if(list) delete list;
          continue;
        }
        TQIterator itr(list,true);
				
				
        TQFolder* snapshots = resultset->getFolder(TQFolder::concatPaths(this->component,version->GetName(),"FitResults")+"+");
        TString err;
        if(!snapshots->importFromTextFile(TQFolder::concatPaths(TQPathManager::getPathManager()->getTargetPath(path),"snapshots.txt",err))){
          manager->warn("unable to read snapshot file!");
        }
	
        while(itr.hasNext()){
          err.Clear();
          TString fname(itr.readNext()->GetName());
          TQFolder* component = TQFolder::loadFromTextFile(fname,err);
          if(!component){
            manager->error(TString::Format("unable to load file '%s': %s",fname.Data(),err.Data()));
            continue;
          } else {
            manager->info(TString::Format("loaded file '%s'",fname.Data()));
          }
          TString path(TQFolder::concatPaths(this->component,version->GetName(),this->subcomponent,"?/?"));
          TQFolderIterator folders(component->getListOfFolders(path),true);
          while(folders.hasNext()){
            TQFolder* f = folders.readNext();
            if(!f) continue;
            TString fpath(f->getPath());
            TQFolder::getPathTail(fpath);
            TQFolder* target = resultset->getFolder(fpath+"+");
            if(!target->getFolder(f->GetName())){
              f->detachFromBase();
              target->addObject(f);
            } else {
              manager->warn(TString::Format("skip collecting '%s', already present in '%s'",f->GetName(),target->getPath().Data()));
            }
          }
        }
      }
      results()->addObject(resultset);
      return true;
    }
  };
  namespace {
    bool writeBreakdown   = TSStatisticsManager::registerAction(new WriteImpactBreakdownConfig<TSBreakdownCalculator>   ("CalculateBreakdown","Breakdowns","Breakdown"),"WriteCalculateBreakdown");
    bool writeImpacts     = TSStatisticsManager::registerAction(new WriteImpactBreakdownConfig<TSImpactCalculator>      ("CalculateImpacts",  "Impacts",   "Impacts"  ),"WriteCalculateImpacts");
    bool collectBreakdown = TSStatisticsManager::registerAction(new CollectImpactBreakdownResults("Breakdowns",        "Breakdown"),"CollectBreakdown");
    bool collectImpacts   = TSStatisticsManager::registerAction(new CollectImpactBreakdownResults("Impacts",           "Impacts"  ),"CollectImpacts");
  }
}


    
