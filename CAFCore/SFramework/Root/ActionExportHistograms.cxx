#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQStringUtils.h"
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
#include "RooFitResult.h"
#include "RooAddition.h"
#include "RooSimultaneous.h"
#include "RooPoisson.h"

#include "TFile.h"

#include "RooCmdConfig.h"

#include <map>
#include <utility>

/*<cafdoc name=ExportHistograms>
  ExportHistograms
  ===========================
  
  Export histograms from a workspace.
  
  Usage:
  ---------------
  ```
  +ExportHistograms{
    +HWWRun2GGF {
      # export a set of postfit histograms
      +postfit {
        <snapshot="SnSh_NuisPOI_Unconditional_observed_muhat">
        <outputFile="workspaces/run2-ggf-$(fitLabel)/histograms-postFit.root">
        <dataName="obsData">
        <fitResult="HWWRun2GGF/Significance/observed/FitResults/observed_muhat">
      }
      # also export a set of prefit histograms
      +prefit {
        <snapshot="SnSh_AllVars_Nominal">
        <outputFile="workspaces/run2-ggf-$(fitLabel)/histograms-preFit.root">
        <dataName="obsData">
        <constPars={"*_norm_*","muGGF"}>
      }
  
      @?{
        <singleRegions=false>
        +CombinedSamples{
          +top {
            <Components={"ttbar*","wt*"}>
          }
          +higgs {
            <Components={"sig*","otherH*"}>
          }
          +VV {
            <Components={"Vgamma*","*NonWW*"}>
          }
          +WW {
            <Components={"ggWW*","*qqWW*"}>
          }
        }
        +CombinedRegions{
          +SR_0j_DF_all {
            <Components="SR_*0j*">
          }
          +SR_1j_DF_all {
            <Components="SR_*1j*">
          }
          +SR_DF_all {
            <Components="SR_*">
          }
          +all {
            <Components="*">
          }                
        }
      }
  #    <normPattern="ATLAS_HWWlvlvGGF_sampleNorm_$(SAMPLENAME)">
       <cloneStyle="HWWRun2GGF/.Histograms/Nominal"> @?;
  
  #    <export.decorrelate={"theo_*"}> @?;
  #    <export.correlate={"*"}> @?;      
    }
  }
  ```

  Subfolders can be provided to the action to write more than one set
  of histograms. Each of these sets can be configured separately.

  The following configuration options are supported:
    * `snapshot`: Give the name of the snapshot to be used. Uses the name of the subfolder as a default.
    * `outputFile`: Output file to write the histograms to.
    * `fitResult`: Define the fit result from which to take the correlation matrix. If not given, a diagonal matrix will be used.
    * `dataName`: One or more names of datasets to be written to the output file.
    * `constPars`: Set one or more parameters constant to have them removed from the uncertainty bands.
    * `export.decorrelate` list one or more parameters to be decorrelated for the uncertainty band.
    * `export.correlate` list one or more parameters to be correlated (with c=1) for the uncertainty band.
    * `normPattern`: Optionally provide a normalization pattern (see below).
    * `cloneStyle`: Provide a path to set of histograms in a model to clone histogram styles from.

  Furthermore, the sample and channel list can be manually chosen:
    * `singleSamples=false` will disable all pre-defined samples from export.
    * `singleRegions=false` will disable all pre-defined regions from export.
    * Provide an additional subfolder `CombinedSamples` to list one or
      more combinations of samples in the form of a subfolder with a
      tag named `Components`.
    * Provide an additional subfolder `CombinedRegions` to list one or
      more combinations of regions in the form of a subfolder with a
      tag named `Components`.

  Normalization Pattern vs. `selectComponents`
  ---------------

  This action can use one of two strategies to extract histograms from
  a workspace, either of which has its own limitations.

  In order to extract histograms, some components of the pdf need to
  be activated/deactivated selectively, such that the different
  contributions (samples) can be isolated. For this, two mechanisms
  exist.
  * The ´selectComponents` mechanism is the default. It works in all
    cases and makes no specific assumptions about the
    workspace. However, it is not fully functional in `ROOT` versions
    prior to 6.12.
  * The `normPattern` mechanism relies on specific normalization
    factors being present in the workspace, following a certain naming
    scheme. Each sample has to have an individual normalization
    factor. The wildcard `$(SAMPLENAME)` can and should be used as a
    placeholder for the sample name in the expression. For example
    `<normPattern="ATLAS_HWWlvlvGGF_sampleNorm_$(SAMPLENAME)">` can be
    provided as a normalization pattern to identify these
    normalization factors. Prior, they can be added in the
    `CreateModels` step with an addition like this one:
```
  @? {
    # create normalization factors for each sample to allow easier extraction of postfit plots
    # we set these constant, so they make no difference for the fit
    +NormFactor.ATLAS_HWWlvlvGGF_sampleNorm_$(BASEFOLDERNAME) {
      <Val = 1., Low = 0., High = 50., Const = true>
    }
  }
```

</cafdoc> */

  // hack to get access to RooPoisson::_CM
  template <typename RooPoissonTag>
  struct RooPoissonHackResult {
    typedef typename RooPoissonTag::type type;
    static type ptr;
  };
  
  template <typename RooPoissonTag>
  typename RooPoissonHackResult<RooPoissonTag>::type RooPoissonHackResult<RooPoissonTag>::ptr;
  
  template<typename RooPoissonTag, typename RooPoissonTag::type p>
  struct RooPoissonRob : RooPoissonHackResult<RooPoissonTag> {
    struct RooPoissonFiller {
      RooPoissonFiller() {RooPoissonHackResult<RooPoissonTag>::ptr = p;}
    };
    static RooPoissonFiller roopoissonfiller_obj;
  };
  
  template<typename RooPoissonTag, typename RooPoissonTag::type p>
  typename RooPoissonRob<RooPoissonTag, p>::RooPoissonFiller RooPoissonRob<RooPoissonTag, p>::roopoissonfiller_obj;
  
  //now expose some members of RooPoisson that we need to access
  struct RooPoissonMean { typedef RooRealProxy(RooPoisson::*type); };
  template class RooPoissonRob<RooPoissonMean, &RooPoisson::mean>;
  //now expose some members of RooPoisson that we need to access
  struct RooPoissonX { typedef RooRealProxy(RooPoisson::*type); };
  template class RooPoissonRob<RooPoissonX, &RooPoisson::x>;


namespace {
  RooRealVar* getGammaParameter(RooPoisson* pois){
    const RooRealProxy& px = pois->*RooPoissonHackResult<RooPoissonMean>::ptr;
    RooProduct* prod = const_cast<RooProduct*>(dynamic_cast<const RooProduct*>(&(px.arg())));
    RooArgList compList(prod->components());
    RooFIter citr(compList.fwdIterator());
    RooAbsArg* arg;
    while((arg = citr.next())){
      RooRealVar* gamma = dynamic_cast<RooRealVar*>(arg);
      if(gamma) return gamma;
    }
    return NULL;
  }
  double getMean(RooPoisson* pois){
    const RooRealProxy& px = pois->*RooPoissonHackResult<RooPoissonMean>::ptr;
    return px.arg().getVal();
  }
  void setGammaUncertainties(RooWorkspace* ws){
    RooArgSet pdfs (ws->allPdfs());
    RooFIter itr(pdfs.fwdIterator());
    RooAbsArg* obj = NULL;;
    while((obj = itr.next())){
      RooPoisson* pois = dynamic_cast<RooPoisson*>(obj);
      if(!pois) continue;
      if(!TQStringUtils::matches(pois->GetName(),"gamma_stat_*")) continue;
      RooRealVar* gamma = getGammaParameter(pois);
      double val = getMean(pois);
      if(gamma->getError() == 0) {
      gamma->setError(sqrt(val)/val);
      }
      else continue;
    } 
  }
}

namespace TSBaseActions {
    
  class ExportHistograms : public TSStatisticsManager::Action {
  
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

      // const RooArgSet* nps (mc->GetNuisanceParameters()); // BW: hashed (unused variable)
      // const RooArgSet* pois(mc->GetParametersOfInterest()); // BW: hashed (unused variable)
      const RooArgSet* obs(mc->GetObservables()); // BW: hashed (unused variable)
      RooArgSet allVarsOrig(ws->allVars());
      RooArgSet allVars;
      RooFIter paritr(allVarsOrig.fwdIterator());
      RooAbsArg* arg;
      while((arg = paritr.next())){
        if(!obs->find(arg->GetName())){
          allVars.add(*arg);
        }
      }
        
      TQFolderIterator itr(config->getListOfFolders("?"));
      int ok = 0;
      while(itr.hasNext()){
        TQFolder* f = itr.readNext();
        if(!f) continue;

        bool includeMC   = f->getTagBoolDefault("includeMC",true);
        bool includeData = f->getTagBoolDefault("includeData",true);
        int calculateErrors = f->getTagIntegerDefault("calculateErrors",1);
        double checkThreshold = f->getTagDoubleDefault("covarianceCheckThreshold",.05);
        TString snapshot = f->getTagStringDefault("snapshot",f->GetName());
        TString pattern;

        RooFitResult* fitResult = NULL;
        TQFolder* tqFitResult = NULL;
        TString fitResultPath;
        
        if(includeMC){
          if(!ws->loadSnapshot(snapshot)){
            manager->error(TString::Format("unable to load snapshot '%s'",snapshot.Data()));
            continue;
          } else {
            manager->info(TString::Format("using snapshot '%s'",snapshot.Data()));
          }

          // modify the workspace
          TSUtils::setParameterValues(&allVars,f,"setPars",true);
          std::vector<TString> constParList = f->getTagVString("constPars");
          TSUtils::setParametersConstant(&allVars,constParList,true);
          
          setGammaUncertainties(ws); 

          if(f->getTagString("fitResult",fitResultPath)){
            TQFolder* result = results()->getFolder(config->GetName());
            TQFolder* myresult = result ? result->getFolder(fitResultPath) : results()->getFolder(fitResultPath);
            if(myresult){
              manager->info(TString::Format("retrieved fit result from '%s'",fitResultPath.Data()));
              tqFitResult = TSUtils::modifyFitResults(myresult,f);
            } else {
              manager->info(TString::Format("failed to retrieve fit result from '%s'",fitResultPath.Data()));
              continue;
            }
          }
        }

        // obtain the RooFitResult object
        // it is of _VITAL_ importance that this happens _AFTER_ the snapshot loading, otherwise the values will be confused
        if(tqFitResult){
          fitResult = TSUtils::convertFitResults(tqFitResult,&allVars);
          // modify the fit results
          if (fitResult) {
            TSUtils::setParameterValues(&fitResult->floatParsFinal(),f,"setPars",true);
            std::vector<TString> constParList = f->getTagVString("constPars");
            TSUtils::setParametersConstant(&fitResult->floatParsFinal(),constParList,true);
          }
        } else {
          manager->warn("no fit result given: using prefit correlations");
          //setGammaUncertainties(ws);
          fitResult = TSUtils::prefitResults(&allVars);
          fitResult->SetName("prefitResult");
        }
        
        
        TString outfilename = f->getTagStringDefault("outputFile",TString::Format("histograms-%s.root",f->GetName()));
        outfilename = TQPathManager::getPathManager()->getTargetPath(outfilename);
        
        TQUtils::ensureDirectoryForFile(outfilename);
        TFile* outfile = TFile::Open(outfilename,"RECREATE");
        if(!outfile || !outfile->IsOpen()){
          manager->error(TString::Format("unable to open output file '%s'",outfilename.Data()));
          continue;
        }
    
        TString styleString;
        TQFolder* styleModel = NULL;
        if(f->getTagString("cloneStyle",styleString)){
          styleModel = models()->getFolder(styleString);
        }
        
        manager->info(TString::Format("making histograms '%s' from snapshot '%s'",f->GetName(),snapshot.Data()));        

        TSHistogramExporter maker(styleModel,fitResult);
        maker.importTagsWithoutPrefix(f,"export.");
        
        if(f->getTagString("normPattern",maker.nfPattern)){
          manager->info(TString::Format("using legacy method with normPattern='%s'",maker.nfPattern.Data()));
        }

        std::vector<TString> dsnames(f->getTagVString("dataName"));
        std::vector<TSHistogramExporter::Region*> origregions = TSHistogramExporter::makeRegions(pdf,mc->GetObservables(),ws->allData());
        std::vector<TSHistogramExporter::Region*> regions(origregions);
        if(!f->getTagBoolDefault("singleRegions",true)){
          regions.clear();
        }
        TSHistogramExporter::addCombinedRegions(f->getListOfFolders("CombinedRegions/?"),regions,origregions);

        for(const auto& region:regions){
          region->select();
          TDirectory* dir = outfile->mkdir(region->getName());
          std::map< const TString, std::vector<TString> > samples;
          if(f->getTagBoolDefault("singleSamples",true)){
            for(const auto& s:region->allSamples()){
              samples[s]={s};
            }
          }
          std::vector<TString> allSamples(region->allSamples());
          TSHistogramExporter::addCombinedSamples(f->getListOfFolders("CombinedSamples/?"),samples,allSamples);
          samples["total"] = allSamples;

          if(includeMC){
            manager->info(TString::Format("  obtaining MC for region %s",region->getName().Data()));
            maker.getMC(dir,region,samples, calculateErrors, checkThreshold);
          }
          
          if(includeData){
            bool useDataNames = (dsnames.size()>1);
            for(auto dsname:dsnames){
              manager->info(TString::Format("  obtaining Data '%s' for region %s",dsname.Data(),region->getName().Data()));
              try {
                TString name(dsname);
                name.ReplaceAll("*","X");
                if(1>maker.getData(dir,dsname,useDataNames ? name.Data() : "Data",region)){
                  manager->error(TString::Format("no dataset matching '%s'",dsname.Data()));
                }
              } catch(const std::runtime_error& err){
                manager->error(err.what());
              }
            }
          }
        }
        if(fitResult) delete fitResult;
        if(tqFitResult) delete tqFitResult;

        manager->info(TString::Format("writing histograms '%s' from snapshot '%s' to '%s'",f->GetName(),snapshot.Data(),outfilename.Data()));
        
        outfile->Write();
        outfile->Close();

        ok++;
      }
      return (ok>0);
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ExportHistograms(),"ExportHistograms");
  }
}
