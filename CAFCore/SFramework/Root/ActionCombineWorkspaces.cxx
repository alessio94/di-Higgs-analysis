#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSUtils.h"

#include <stdexcept>

#ifdef ROOTCORE
#include <RootCore/Packages.h>
#endif
#ifdef HAS_RooFitUtils
#include "RooFitUtils/AbsMeasurement.h"
#include "RooFitUtils/CorrelationScheme.h"
#include "RooFitUtils/CombinedMeasurement.h"
#include "RooFitUtils/Measurement.h"
#endif

/*<cafdoc name=CombineWorkspaces>
  CombineWorkspaces
  ===========================
  
  Combine several workspaces to a combined measurement.
  
  Usage:
  ---------------
  ```
  +CombineWorkspaces {
    +HWWRun2 {
      <verbose=true,asimov=false>
      <correlationSchemes = {
        "config/statistics/comb/snippets/correlations.ggF.txt",
        "config/statistics/comb/snippets/correlations.vbf.txt"
      }>
      +HWWRun2GGF {
        <SnapshotName="SnSh_AllVars_Nominal",DataName="obsData">
      }
      +HWWRun2VBF {
        <SnapshotName="NominalParamValues",DataName="obsData">
      }
      <BinnedLikelihood=true> @ ?;
    }
  }
  ```
  
  This Action internally uses the `RooFitUtils` extension package and
  will fail with a runtime error if that is not present.

  Options include:

    * `correlationSchemes`: List of correlation schemes. These are
       text files with lines in the format
       ```
       WorkspaceName::OldParameterName>>NewParameterName
       ```
       These define which parameters from which workspaces should be
       correlated with each other by renaming them.
    * `autocorrelation`: Set to true to auto-correlate all parameters
       with the same name from different input workspaces.
    * `verbose`: Set to true to get verbose output.
    * `asimov`: Set to true to generate an asimov dataset on the output workspace.
    * `writeTable`: Provide a file name to write a table of which
      parameters were correlated in the end.

  Each workspace to be combined should be provided as one
  subfolder. Options on the input workspaces include
    * `SnapshotName`: Set the Snapshot name to be loaded before combining.
    * `DataName`: Set the name of the dataset the be used for the combination.
    * `BinnedLikelihood`: Activate BinnedLikelihood for the input measurement.


</cafdoc> */

namespace TSBaseActions {

  class CombineWorkspaces : public TSStatisticsManager::Action {
#ifdef HAS_RooFitUtils
    RooFitUtils::CombinedMeasurement::SnapshotName getSnapshot(TString name) const {
      name.ToLower();
      if(name == "nominal"){
        return RooFitUtils::CombinedMeasurement::nominal;
      }
      if(name == "umcles"){
        return RooFitUtils::CombinedMeasurement::ucmles;
      }
      if(name == "background"){
        return RooFitUtils::CombinedMeasurement::background;
      }
      throw std::runtime_error(TString::Format("unknown snapshot name: %s",name.Data()).Data());
    }

    bool execute(TQFolder * config) const override {
      // Create a new combined measurement
      TString combwsname(config->GetName());
      TString combdataname(config->getTagStringDefault("dataName","obsData"));
      TString combmcname("ModelConfig");
      TString combcatname(config->getTagStringDefault("categoryName","channelCat"));

      bool verbose = config->getTagBoolDefault("verbose",false);
      
      RooFitUtils::CombinedMeasurement combined(combwsname.Data(),combwsname.Data(),combmcname.Data(),combdataname.Data());
      combined.SetNameCategory(combcatname.Data());

      // Define a correlation scheme that should be used when combining the specified
      // measurements. Parameters in the scheme that are not present in the
      // measurements will be ignored.
      RooFitUtils::CorrelationScheme correlation("CorrelationScheme");
      correlation.SetAutoCorrelation(config->getTagBoolDefault("autocorrelation",false));
      
      TQFolderIterator wsconfigs(config->getListOfFolders("?"));
      while(wsconfigs.hasNext()){
        TQFolder* wsconfig = wsconfigs.readNext();
        if(!wsconfig) continue;

        TString wsname = wsconfig->getTagStringDefault("WorkspaceName",wsconfig->GetName());

        bool binned;
        if(!wsconfig->getTagBool("BinnedLikelihood",binned)){
          manager->error(TString::Format("for measurement '%s', please specify 'BinnedLikelihood' (bool)",wsconfig->GetName()));
          continue;
        }
        TString snapshotName;
        if(!wsconfig->getTagString("SnapshotName",snapshotName)){
          manager->error(TString::Format("for measurement '%s', please specify 'SnapshotName' (string)",wsconfig->GetName()));
          continue;
        }
        TString modelconfig = wsconfig->getTagStringDefault("ModelConfigName","ModelConfig");
        TString dataname;
        if(!wsconfig->getTagString("DataName",dataname)){
          manager->error(TString::Format("for measurement '%s', please specify 'DataName' (string)",wsconfig->GetName()));
          continue;
        }

        RooWorkspace* ws = dynamic_cast<RooWorkspace*>(workspaces()->getObject(wsname));
        if(!ws){
          manager->error(TString::Format("unable to obtain workspace '%s'!",wsname.Data()));
          continue;
        }
        RooStats::ModelConfig* mc = dynamic_cast<RooStats::ModelConfig*>(ws->obj(modelconfig));
        if(!mc){
          manager->error(TString::Format("workspace '%s' has not ModelConfig!",wsname.Data()));
          continue;
        }
        if(!mc->GetPdf()){
          manager->error(TString::Format("ModelConfig in workspace '%s' has no Pdf assigned!",wsname.Data()));
          continue;
        }

        manager->info(TString::Format("creating measurement for '%s'",wsconfig->GetName()));        
        RooFitUtils::Measurement* measurement = new RooFitUtils::Measurement (wsconfig->GetName(),ws,modelconfig.Data(),dataname.Data(),snapshotName.Data(),binned);
        if(!measurement){
          manager->error(TString::Format("unable to create measurement from workspace config '%s'!",wsconfig->GetName()));
          continue;
        }
        combined.AddMeasurement(measurement);

        TQIterator corrtags(wsconfig->getListOfKeys("merge.*.0"));
        while(corrtags.hasNext()){
          TString key(corrtags.readNext()->GetName());
          TQStringUtils::removeLeadingText(key,"merge.");
          TString pname;
          TQStringUtils::readUpTo(key,pname,".");
          std::vector<TString> params = wsconfig->getTagVString("merge."+pname);
          manager->info(TString::Format("obtaining merged parameter '%s' from %s",pname.Data(),TQStringUtils::concat(params,",").Data()));
          for(auto& p:params){
            p.Prepend("::");
            p.Prepend(wsconfig->GetName());            
          }
          TString paramlist(TQStringUtils::concat(params,","));
          correlation.CorrelateParameter(paramlist,pname);
        }

        
      }

      manager->info("applying correlation scheme");

      // Define parameters of interest for the combined measurement. Only parameters
      // present in the final workspace will be considered.
      std::vector<TString> pois = config->getTagVString("POI");
      if(pois.empty()) pois.push_back("mu");
      TString poilist = TQStringUtils::concat(pois,",");
      correlation.SetParametersOfInterest(poilist.Data());

      std::vector<TString> correlate = config->getTagVString("correlate");

      for(auto infile:config->getTagVString("correlationSchemes")){
        manager->info("importing correlations from "+infile);
        TQStringUtils::readFileLines(&correlate,infile,2048,true);
      }

      for(auto it:correlate){
        TString orig;
        TQStringUtils::readUpTo(it,orig,">>");
        TQStringUtils::removeLeading(it,">");
        correlation.CorrelateParameter(orig,it);
      }

      if(verbose){
        correlation.Print();
      }
      

      // Use the correlation scheme for the combined measurement.
      combined.SetCorrelationScheme(&correlation);

      manager->info("combining measurements");

      try {
        combined.CollectMeasurements();
        combined.CombineMeasurements();
      } catch(std::exception& e){
        manager->error(e.what());
        return false;
      }

      if(verbose){
        combined.Print();
      }
      
      if(config->getTagBoolDefault("asimov",false)){
        manager->info("creating asimov data");        
        combined.MakeAsimovData(config->getTagBoolDefault("asimov.conditional",false),
                                 getSnapshot(config->getTagStringDefault("asimov.profile","nominal")),
                                 getSnapshot(config->getTagStringDefault("asimov.generate","nominal")));
      }

      manager->info("obtaining combined workspace");              
      RooWorkspace* ws = combined.GetWorkspace();
      if(!ws){
        manager->error("unable to combine workspaces!");
        return false;
      }

      RooStats::ModelConfig* newMC = (RooStats::ModelConfig*)(ws->obj("ModelConfig"));

      TSUtils::applySettings(config,ws->allVars(),newMC);

      workspaces()->addObject(ws);

      TString outfile;
      if(config->getTagString("writeTable",outfile)){
        manager->info(TString::Format("writing correlation table to %s",outfile.Data()));
        correlation.printToFile(outfile,true);
      }

      return true;
    }
#else
    bool execute(TQFolder * /*config*/) const override {
      manager->error("unable to combine workspaces -- this feature relies on Stefan Gadatsch's RooFitUtils package!");
      return false;
    }
#endif
	};
  namespace {
    bool available = TSStatisticsManager::registerAction(new CombineWorkspaces(),"CombineWorkspaces");
  }
}
