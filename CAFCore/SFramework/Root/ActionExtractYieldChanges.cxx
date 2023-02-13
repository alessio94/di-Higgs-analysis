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
#include "RooProduct.h"
#include "RooRealVar.h"
#include "RooRealIntegral.h"
#include "RooFitResult.h"
#include "RooAddition.h"
#include "RooSimultaneous.h"

#include "TFile.h"

#include "RooCmdConfig.h"

#include <map>
#include <utility>

/*<cafdoc name=ExtractYieldChanges>
  ExtractYieldChanges
  ===========================
  
  Extract the effects on the yields that individual parameters have on
  the workspace, resolved by parameter, sample and region. This action
  generates a set of tables in an output directory. The tables will
  contain the change in event yield for a +/- 1sigma variation of each
  parameter.
  
  Usage:
  ---------------
  ```
  +ExtractYieldChanges{
    +HWWRun2GGF {
      +prefit {
        <outputDirectory="workspaces/run2-ggf-$(fitLabel)/yieldChanges">
      }
    }
    @ ?/prefit {
      <snapshot="SnSh_AllVars_Nominal">
      <normPattern="ATLAS_HWWlvlvGGF_sampleNorm_$(SAMPLENAME)">
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
       events -- parameters with effects smaller than this will not be
       included in the tables.

  For further options, especially the usage of the `normPattern` tag, please see `ActionExportHistograms`.

</cafdoc> */

namespace TSBaseActions {
    
  class ExtractYieldChanges : public TSStatisticsManager::Action {
  
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
      if(pois)
        allVars.add(*pois);
      if(nps)
        allVars.add(*nps);

      TQFolder* output = new TQFolder("tables");

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
        TSUtils::setParametersConstant(&allVars,f->getTagVString("floatPars"),false);
        
        TString outdirname;
        if(!f->getTagString("outputDirectory",outdirname)){
          manager->error("no 'outputDirectory' given!");
          continue;
        }
        
        outdirname = TQPathManager::getPathManager()->getTargetPath(outdirname);        
        TQUtils::ensureDirectory(outdirname);
    
        TString nfPattern;
        if(f->getTagString("normPattern",nfPattern)){
          manager->info(TString::Format("using legacy method with normPattern='%s'",nfPattern.Data()));
        }

        manager->info(TString::Format("extracting yield changes from snapshot '%s'",snapshot.Data()));        

        bool showRelative = f->getTagBoolDefault("relative",false);
        bool verbose = f->getTagBoolDefault("verbose",false);

        
        std::vector<TSHistogramExporter::Region*> origregions = TSHistogramExporter::makeRegions(pdf,mc->GetObservables(),ws->allData());
        std::vector<TSHistogramExporter::Region*> regions(origregions);
        if(!f->getTagBoolDefault("singleRegions",true)){
          regions.clear();
        }
        TSHistogramExporter::addCombinedRegions(f->getListOfFolders("CombinedRegions/?"),regions,origregions);

        for(const auto& region:regions){
          region->select();

          if(nfPattern.IsNull()){
            region->unselectComponents();
          } else {
            region->collectSelectionVars(nfPattern);
            region->unselectComponentsLegacy();
          }
          
          RooAbsReal* integral = region->createIntegral();
          
          TQTable* table = new TQTable(region->getName());
          table->setTagBool("standalone",true);

          int iSample = 0;
          
          std::map< const TString, std::vector<TString> > samples;
          if(f->getTagBoolDefault("singleSamples",true)){
            for(const auto& s:region->allSamples()){
              samples[s]={s};
            }
          }
          std::vector<TString> allSamples(region->allSamples());
          TSHistogramExporter::addCombinedSamples(f->getListOfFolders("CombinedSamples/?"),samples,allSamples);
          samples["total"] = allSamples;

          manager->info(TString::Format("  obtaining yields for region %s",region->getName().Data()));
          for(const auto& sample: samples){
            if(nfPattern.IsNull())
              region->selectComponents(sample.second);
            else
              region->selectComponentsLegacy(sample.second);
            iSample++;

            if(verbose){
              std::cout << "    " << sample.first << std::endl;
            }

            RooAbsArg* obj;
            int iNP = 0;
            RooFIter itr(allVars.fwdIterator());
            while((obj = itr.next())){
              
              RooRealVar* np = dynamic_cast<RooRealVar*>(obj);

              double cval = np->getVal();
              double eup = np->getErrorHi();
              double edn = np->getErrorLo();

              if(eup == 0 && edn == 0){
                eup = cval;
                edn = cval;
              }

              // this is just to clear the cache
              np->setVal(0.999*cval);
              np->setVal(cval);

              double nc = integral->getVal();
              np->setVal(cval+fabs(eup));
              double nu = integral->getVal();
              np->setVal(cval-fabs(edn));
              double nd = integral->getVal();
              np->setVal(cval);

              double up = nu-nc;
              double down = nd-nc;


              if(verbose){
                std::cout << "      " << np->GetName() << "=" << cval << "+" << fabs(eup) << "/-" << fabs(edn) << ": " << nc << " " << nu << "~" << up << " " << nd << "~" << down <<  std::endl;
              }
              if (showRelative) {
                if (nc != 0) {
                  up = up/nc;
                  down = down/nc;
                }
              }
              iNP++;
              
              if(up>0 || down>0){
                table->setEntry(0,iSample,sample.first);
                table->setEntry(iNP,0,np->GetName());
                table->setEntry(iNP,iSample, showRelative? TString::Format("%.1f / %.1f %%",up*100,down*100) :  TString::Format("%g / %g",up,down));
                table->setProperty(iNP,iSample,"up",up);
                table->setProperty(iNP,iSample,"down",down);
              }
            }
          }
          delete integral;
          TString outfile = TQFolder::concatPaths(outdirname,region->getName());
          manager->info("  writing "+outfile);
          table->shrink();
          table->writeLaTeX(outfile+".tex");
          table->writeHTML(outfile+".html");
          output->addObject(table);
        }
        
        TString outrootfile(TQFolder::concatPaths(outdirname,"tables.root"));
        output->writeToFile(TQPathManager::getPathManager()->getTargetPath(outrootfile).c_str());
        
        ok++;
      }
      return (ok>0);
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ExtractYieldChanges(),"ExtractYieldChanges");
  }
}
