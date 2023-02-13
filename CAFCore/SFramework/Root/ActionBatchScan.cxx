#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSStatisticsCalculator.h"
#include "SFramework/TSLikelihoodScanner.h"
#include "SFramework/TSUtils.h"
#include "TFile.h"


namespace {
  TString getName(TQFolder* config){
      TString name(config->getBase()->GetName());
      TString buffer;
      if(name.Contains(".")){
        TQStringUtils::readUpTo(name,buffer,".");
        TQStringUtils::removeLeading(name,".");
      } else {
        name = "POI";
      }
      config->getTagString("name",name);
      return name;
  }
}

/*<cafdoc name=WriteScanLikelihood>
  WriteScanLikelihood
  ===========================
  
  This is a batch-parallelized version of the `ScanLikelihood` action.
  Instead of performing all the fits locally, it will write out a
  directory with a copy of the workspaces as well as configs to submit
  jobs fitting the individual points to a batch system.
  
  
  Usage:
  ---------------
  ```
  +WriteScanLikelihood.2Dobs{
    +HWWRun2{
      <outputPath = "./workspaces/run2-comb-$(fitLabel)/Batch/Scans/2Dobs"> # output path for all the configs
      <datasetName="obsData">
      +muGGF{    
        <min=0,max=2., nbins=20>
      }
      +muVBF{    
        <min=0.,max=3., nbins=20>
      }
    }
  }
  ```

  In order to collect the results, use the `CollectScanResults` action.

</cafdoc> */

/*<cafdoc name=CollectScanResults>
  CollectScanResults
  ===========================
  
  This is the result collection for the batch-parallelized version of the `ScanLikelihood` action.
  
  Usage:
  ---------------
  ```
  +CollectScanResults.2Dobs {
    +Morphing {
      <inputPath = "./workspaces/run2-comb-$(fitLabel)/Batch/Scans/2Dobs"> # input path for all the results
    }
  }
  ```

  The results are produced with the `WriteScanLikelihood` action.

</cafdoc> */


namespace TSBaseActions {

  class WriteScanLikelihood : public TSStatisticsManager::Action {
    
  public:
    WriteScanLikelihood(){
      // do nothing
    }
		
    bool execute(TQFolder * config) const override {
      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!workspace){
        manager->error(TString::Format("no such workspace available: '%s'",config->GetName()));
        return false;
      }
      
      TQFolder * result = results()->getFolder(TString::Format("%s+",config->GetName()));
      
      bool ok = writeConfigs(workspace,::getName(config),config,result);

      return ok;
    }

			
    bool writeConfigs(RooWorkspace* ws, const TString& name, TQFolder* config, TQFolder* result) const {
      TSLikelihoodScanner scan(ws,result->getFolder("Snapshots+"));

      TString output(TQFolder::concatPaths("batch","ScanLikelihood",name));
      config->getTagString("outputPath",output);
      output = TQPathManager::getPathManager()->getTargetPath(output);
      TQUtils::ensureDirectory(TQFolder::concatPaths(output,"configs"));
      TQUtils::ensureDirectory(TQFolder::concatPaths(output,"results"));
      TQUtils::ensureDirectory(TQFolder::concatPaths(output,"logs"));            
      manager->info(TString::Format("Writing configs for ScanLikelihood.%s to '%s'",name.Data(),output.Data()));

      TQFolder base("base");
      TString wsfile(TQFolder::concatPaths(output,"workspace.root"));
      base.setTagString("expectedInput",wsfile);      
      TQFolder* scanconfig = config->copy();
      TQFolder* importconfig = base.getFolder(TQFolder::concatPaths("ImportWorkspaces",config->GetName())+"+");
      importconfig->setTagString("inputFile",TString::Format("%s:%s",wsfile.Data(),config->GetName()));
      base.getFolder(TString::Format("ScanLikelihood.%s+",name.Data()))->addFolder(scanconfig);
      TQFolder* exportconfig = base.getFolder(TQFolder::concatPaths("ExportResults",config->GetName())+"+");      

      if(config->getTagBoolDefault("prefit",true)){
        TQFolder* uncond = scan.runPreFit(config);
        delete uncond;
        scanconfig->setTagBool("refitUnconditional",config->getTagBoolDefault("refitUnconditional",false));
      }
      
      const TString snapshotNominal = config->getTagStringDefault("snapshot.nominal","SnSh_AllVars_Nominal");      
      ws->loadSnapshot(snapshotNominal);
      
      ws->writeToFile(TQPathManager::getPathManager()->getTargetPath(wsfile).c_str());
      
      int n = TSLikelihoodScanner::nPoints(config);
      int nPointsPerJob = config->getTagIntegerDefault("nPointsPerJob",5);

      for(int i=0; i<n; i+=nPointsPerJob){
        int first = i;
        int last = std::min(i+nPointsPerJob-1,n);
        TString id(TString::Format("p%d_p%d",first,last));
        scanconfig->setTagInteger("onlyPoints.first",first);
        scanconfig->setTagInteger("onlyPoints.last",last);
        scanconfig->setTagString("fit.logToFile",TQFolder::concatPaths(output,"logs","scan."+id+".log"));
        scanconfig->removeTag("fit.writeWorkspacePreNll");
        scanconfig->removeTag("fit.writeWorkspacePostNll");    
        
        TString result = TQFolder::concatPaths(output,"results","result."+id+".txt");
        exportconfig->setTagString("outputFile",result);
        TString outfile(TQFolder::concatPaths(output,"configs","scan."+id+".txt"));
        manager->info("writing config "+outfile);
        base.setTagString("identifier","scan_"+name+"_"+id);        
        base.setTagString("expectedOutput",result);
        base.exportToTextFile(TQPathManager::getPathManager()->getTargetPath(outfile).c_str());
      }
      return true;
    }
  };

  class CollectScanResults : public TSStatisticsManager::Action {
  public:
    CollectScanResults(){
      // do nothing
    };
		
    bool execute(TQFolder * config) const override {
			
      TString resultname(config->GetName());
      TQFolder* resultset = results()->getFolder(resultname+"+");
      if(!resultset){
        manager->error(TString::Format("no such result available: '%s'",config->GetName()));
        return false;
      }

      TString configname = ::getName(config);
      TString path = config->getTagStringDefault("inputPath","batch/ScanLikelihood/"+configname);
      path = TQPathManager::getPathManager()->getTargetPath(path);      
      
      manager->info(TString::Format("reading input from '%s'",path.Data()));
      
      TString resultfiles = TQFolder::concatPaths(path,"results");
      TQStringUtils::ensureTrailingText(resultfiles,"/*.txt");
      TCollection* list = TQUtils::ls(resultfiles);
      if(!list || list->GetEntries() == 0) {
        manager->error(TString::Format("no files found under '%s'!",resultfiles.Data()));
        if(list) delete list;
        return false;
      }
      resultset->detachFromBase();
      TQIterator itr(list,true);
      while(itr.hasNext()){
        TString fname(itr.readNext()->GetName());
        TString err;
        TQFolder* component = TQFolder::loadFromTextFile(fname,err);
        if(!component){
          manager->error(TString::Format("unable to load file '%s': %s",fname.Data(),err.Data()));
          continue;
        }				

        TString path(TQFolder::concatPaths("LikelihoodScans",TString::Format("ScanLikelihood.%s",configname.Data()),"Scan"));
        
        TQFolder* scans = component->getFolder(path);
        if(!scans) {
          manager->error(TString::Format("unable to obtain scan '%s' from file '%s'",path.Data(),fname.Data()));
          continue;
        }
        TQFolderIterator folders(scans->getListOfFolders("?"),true);
        while(folders.hasNext()){
          TQFolder* f = folders.readNext();
          if(!f) continue;
          TString fpath(f->getPath());
          TQFolder::getPathTail(fpath);
          TQFolder* target = resultset->getFolder(fpath+"+");
          if(!target->getFolder(f->GetName())){
            target->importTags(f->getBase());
            f->detachFromBase();
            target->addObject(f);
          }
        }
      }
      results()->addObject(resultset);
      return true;
    }
  };
  namespace {
    bool writeConfigs     = TSStatisticsManager::registerAction(new WriteScanLikelihood(),"WriteScanLikelihood");
    bool collectResults   = TSStatisticsManager::registerAction(new CollectScanResults(),"CollectScanResults");
  }
}


    
