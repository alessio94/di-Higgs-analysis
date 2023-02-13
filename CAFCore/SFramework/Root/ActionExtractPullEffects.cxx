#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQTable.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSUtils.h"
#include "SFramework/TSHistogramExporter.h"

#include "RooStats/ModelConfig.h"

#include "RooCategory.h"
#include "RooBinning.h"
#include "RooCatType.h"
#include "RooDataSet.h"
#include "RooRealSumPdf.h"
#include "RooProdPdf.h"
#include "RooArgSet.h"
#include "RooRealIntegral.h"
#include "RooProduct.h"
#include "RooRealVar.h"
#include "RooFitResult.h"
#include "RooAddition.h"
#include "RooSimultaneous.h"

#include "TFile.h"

#include "RooCmdConfig.h"

#include <map>
#include <utility>

/*<cafdoc name=ExtractPullEffects>
  ExtractPullEffects
  ===========================
  
  Extract the effects of pulls on event yields, resolved by parameter,
  sample and region. This action generates a set of tables in an
  output directory.
  
  Usage:
  ---------------
  ```
  +ExtractPullEffects{
    +HWWRun2GGF {
      +postfit {
        <snapshot="SnSh_AllVars_Nominal">
        <outputDirectory="workspaces/run2-ggf-$(fitLabel)/pullEffects">
        <threshold=2>
        <normPattern="ATLAS_HWWlvlvGGF_sampleNorm_$(SAMPLENAME)">
        <fitResult="HWWRun2GGF/Significance/observed/FitResults/observed_muhat">
      }
    }
  }
  ```

  Subfolders can be provided to the action to write more than one set
  of results. Each of these can be configured separately.

  The following configuration options are supported:
    * `snapshot`: Give the name of the snapshot to be used. Uses the name of the subfolder as a default.
    * `outputDirectory`: Output directory to write the tables to.
    * `fitResult`: Define the fit result from which to take the list of parameters.
    * `threshold`: Give a lower threshold on the change in number of
       events -- pulls with effects smaller than this will not be
       included in the tables.

  For further options, especially the usage of the `normPattern` tag, please see `ActionExportHistograms`.

</cafdoc> */



namespace {
  void cleanup(TQTable* table, double threshold){
    for(int i=1;i<table->getNrows(); ++i){
      double maxd = 0;
      for(int j=1;j<table->getNcols(); ++j){
        double val = table->getEntryDouble(i,j);
        maxd = std::max(maxd,fabs(val));
      }
      if(maxd<threshold) table->clearRow(i);
    }
    
    for(int j=1;j<table->getNcols(); ++j){
      double maxd = 0;
      for(int i=1;i<table->getNrows(); ++i){
        double val = table->getEntryDouble(i,j);
        maxd = std::max(maxd,fabs(val));
      }
      if(maxd<threshold) table->clearCol(j);
    }          
    table->cleanup();
    table->shrink();
  }
  bool isIn(const TString& str, std::vector<TString> v){
    for(const auto& s:v){
      if(str == s) return true;
    }
    return false;
  }
}

namespace TSBaseActions {
    
  class ExtractPullEffects : public TSStatisticsManager::Action {
  
    bool execute(TQFolder * config) const override {
      if(!config){
        throw std::runtime_error("received NULL pointer as config object!");
      }
      
      RooWorkspace * ws = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!ws){
        manager->error(TString::Format("unable to load workspace '%s': no such workspace!",config->GetName()));
        return false;
      }

      TString mcname = config->getTagStringDefault("modelConfig","ModelConfig");
      RooStats::ModelConfig* mc = dynamic_cast<RooStats::ModelConfig*>(ws->obj(mcname.Data()));
      if(!mc){
        manager->error(TString::Format("unable to load ModelConfig from workspace '%s'!",config->GetName()));
        return false;
      }
			
      RooSimultaneous* pdf = dynamic_cast<RooSimultaneous*>(mc->GetPdf());
      if(!pdf){
        manager->error("ModelConfig does not have a PDF attached!");
        return false;
      }

      const RooArgSet* nps (mc->GetNuisanceParameters());
      const RooArgSet* pois(mc->GetParametersOfInterest());
      RooArgSet allVars;
      allVars.add(*pois);
      allVars.add(*nps);

      TQFolderIterator itr(config->getListOfFolders("?"));
      int ok = 0;
      while(itr.hasNext()){
        TQFolder* f = itr.readNext();
        if(!f) continue;
        
        TString snapshot = f->getTagStringDefault("snapshot",f->GetName());
        TString pattern;
        
        if(!ws->loadSnapshot(snapshot)){
          manager->error(TString::Format("unable to load snapshot '%s'",snapshot.Data()));
          continue;
        } else {
          manager->info(TString::Format("using snapshot '%s'",snapshot.Data()));
        }

        TSUtils::setParameterValues(&allVars,f,"setPars",true);
        TSUtils::setParametersConstant(&allVars,f->getTagVString("constPars"),true);

        
        TString outdirname;
        if(!f->getTagString("outputDirectory",outdirname)){
          manager->error("no 'outputDirectory' given!");
          continue;
        }
        outdirname = TQPathManager::getPathManager()->getTargetPath(outdirname);

        TQUtils::ensureDirectory(TQFolder::concatPaths(outdirname,"byRegion"));
        TQUtils::ensureDirectory(TQFolder::concatPaths(outdirname,"bySample"));
        TQUtils::ensureDirectory(TQFolder::concatPaths(outdirname,"byNP"));
        
        TString nfPattern;
        if(f->getTagString("normPattern",nfPattern)){
          manager->info(TString::Format("using legacy method with normPattern='%s'",nfPattern.Data()));
        }

        manager->info(TString::Format("extracting pull effects from snapshot '%s'",snapshot.Data()));        


        double threshold = f->getTagDoubleDefault("threshold",-1.);
        
        TString fitResultPath;
        if(!f->getTagString("fitResult",fitResultPath)){
          manager->error("no 'fitResult' given!");
          continue;
        }
        TQFolder* fitResult = results()->getFolder(fitResultPath);
        if(!fitResult){
          manager->error(TString::Format("unable to find fitResult '%s' given!",fitResultPath.Data()));
          continue;
        }

        TQFolder* floatPars = fitResult->getFolder("floatParsFinal");
        
        std::vector<TSHistogramExporter::Region*> regions = TSHistogramExporter::makeRegions(pdf,mc->GetObservables(),ws->allData());

        std::map<TString, std::map<TString, std::map<TString,double> > > numbers;

        std::vector<TString> sampleNames;
        std::vector<TString> regionNames;
        std::vector<TString> parNames;        
        
        for(const auto& region:regions){
          region->select();

          if(nfPattern.IsNull()){
            region->unselectComponents();
          } else {
            region->collectSelectionVars(nfPattern);
            region->unselectComponentsLegacy();
          }
          
          RooAbsReal* integral = region->createIntegral();
          
          if(!isIn(region->getName(),regionNames)) regionNames.push_back(region->getName());          
          std::map<TString, std::map<TString,double> > regionNumbers;
          
          std::vector<TString> allSamples(region->allSamples());
          manager->info(TString::Format("  obtaining pull effects for region %s",region->getName().Data()));
          for(const auto& sample: allSamples){
            if(nfPattern.IsNull())
              region->selectComponents({sample});
            else
              region->selectComponentsLegacy({sample});

            if(!isIn(sample,sampleNames)) sampleNames.push_back(sample);
            
            std::map<TString,double> sampleNumbers;
            
            RooAbsArg* obj;
            RooFIter itr(nps->fwdIterator());
            while((obj = itr.next())){
                                
              RooRealVar* np = dynamic_cast<RooRealVar*>(obj);

              TQFolder* npf = floatPars->getFolder(np->GetName());
              if(!npf) continue;
              
              double val;
              if(!npf->getTagDouble("val",val)) continue;

              if(!isIn(np->GetName(),parNames)) parNames.push_back(np->GetName());
              
              double cval = np->getVal();

              // This is necessary to clear the cache!
              np->setVal(cval+0.5);
              np->setVal(cval);
              double ni = integral->getVal();
              
              np->setVal(val);
              double nf = integral->getVal();
              np->setVal(cval);
              double d = nf-ni;

              sampleNumbers[np->GetName()] = d;
              

            }
            regionNumbers[sample] = sampleNumbers;
          }

          numbers[region->getName()] = regionNumbers;
          
          delete integral;
        }


        // by region
        for(const auto& region:regionNames){
          TQTable* table = new TQTable(region);
          table->setTagBool("standalone",true);
          int iSample = 0;
          for(const auto& sample:sampleNames){
            iSample++;
            table->setEntry(0,iSample,sample);
            int iNP = 0;
            for(const auto& np:parNames){
              iNP++;
              table->setEntry(iNP,0,np);
              table->setEntryValue(iNP,iSample,numbers[region][sample][np]);
            }
          }
          cleanup(table,threshold);

          if(table->getNrows() > 0){
            TString outfile = TQFolder::concatPaths(outdirname,"byRegion",region);
            manager->info("  writing "+outfile);
            table->writeLaTeX(outfile+".tex");
            table->writeHTML(outfile+".html");
          }
          delete table;
        }

        // by region
        for(const auto& sample:sampleNames){
          TQTable* table = new TQTable(sample);
          table->setTagBool("standalone",true);
          int iRegion = 0;
          for(const auto& region:regionNames){
            iRegion++;
            table->setEntry(0,iRegion,region);
            int iNP = 0;
            for(const auto& np:parNames){
              iNP++;
              table->setEntry(iNP,0,np);
              table->setEntryValue(iNP,iRegion,numbers[region][sample][np]);
            }
          }
          cleanup(table,threshold);

          if(table->getNrows() > 0){          
            TString outfile = TQFolder::concatPaths(outdirname,"bySample",sample);
            manager->info("  writing "+outfile);
            table->writeLaTeX(outfile+".tex");
            table->writeHTML(outfile+".html");
          }
          delete table;
        }

        // by region
        for(const auto& np:parNames){
          TQTable* table = new TQTable(np);
          table->setTagBool("standalone",true);
          int iRegion = 0;
          for(const auto& region:regionNames){
            iRegion++;
            table->setEntry(0,iRegion,region);
            int iSample = 0;
            for(const auto& sample:sampleNames){
              iSample++;
              table->setEntry(iSample,0,sample);
              table->setEntryValue(iSample,iRegion,numbers[region][sample][np]);
            }
          }
          cleanup(table,threshold);

          if(table->getNrows() > 0){                    
            TString outfile = TQFolder::concatPaths(outdirname,"byNP",np);
            manager->info("  writing "+outfile);
            table->writeLaTeX(outfile+".tex");
            table->writeHTML(outfile+".html");
          }
          delete table;
        }                
        
        
        ok++;
      }
      return (ok>0);
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ExtractPullEffects(),"ExtractPullEffects");
  }
}
