#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQHistogramUtils.h"

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
#include "RooAbsReal.h"
#include "RooFitResult.h"
#include "RooAddition.h"
#include "RooSimultaneous.h"
#include "RooPoisson.h"
#include "RooConstVar.h"
#include "RooHistFunc.h"
#include "RooDataHist.h"
#include "RooStats/HistFactory/PiecewiseInterpolation.h"
#include "RooStats/HistFactory/FlexibleInterpVar.h"
#include "RooStats/HistFactory/ParamHistFunc.h"
#include "RooAbsDataStore.h"

#include "TFile.h"

#include "RooCmdConfig.h"

#include <map>
#include <utility>

/*<cafdoc name=ScaleProcesses>
  ScaleProcesses
  ===========================
  
  This action scales the yields of certain processes in a workspace by an
  arbitrary factor.
  
  The Action scales all (matching) RooDataHist instances in a workspace.
  Note: This has been tested to work by scaling the input histograms
  when building the workspace and "unscaling" them on a workspace level 
  using this implementation. Behavior on (partially) unbinned fits has not
  been tested (yet).
  
  
  Usage:
  ---------------
  ```
  +ScaleProcesses{
    +NameOfWorkspace {
      <pdf = "nameOfPdfToEdit"> #defaults to "simPdf"
      +someProcess{
        <name = "expression*ToMatch", scale = 2.0, invert=false> 
        #scale all processes with names matching the expression given as 
        #'name' by the given scaling factor. If 'invert' is true, yields
        #are scaled by 1/'scale' instead (default is false).
      }
      +otherProcess {
        <scale = 1.5> 
        #if 'name' is not specified, the name of the folder is used (here:
        # "otherProcess")
      }
    }
  }
  ```
  The process name is matched against the name of the relevant RooDataHist
  instances. If their names are not known, try printing a workspace in
  cling (root shell): ws->Print("t")

</cafdoc>*/


 



namespace TSBaseActions {
    
  class ScaleProcesses : public TSStatisticsManager::Action {
    
    bool execute(TQFolder * config) const override {
      if(!config){
        throw std::runtime_error("received NULL pointer as config object!");
      }
      //retrieve workspace
      RooWorkspace * ws = dynamic_cast<RooWorkspace*>(workspaces()->getObject(config->GetName()));
      if(!ws){
        manager->error(TString::Format("unable to load workspace '%s': no such workspace!",config->GetName()));
        return false;
      }
      //ws->obj("ModelConfig")->Print("t");
      //retrieve pdf to edit
      TString pdfName = config->getTagStringDefault("pdfName","simPdf");
      RooAbsPdf* pdf = ws->pdf(pdfName);
      if (!pdf){
        manager->error(TString::Format("unable to retrieve pdf '%s' from workspace '%s': no such pdf!",pdfName.Data(),config->GetName()));
        return false;
      }
      
      /*
      RooSimultaneous* simpdf = dynamic_cast<RooSimultaneous*>(pdf);
      if (!simpdf) {
        manager->error(TString::Format("unable to cast pdf '%s' to RooSumultaneous: your workspace '%s' seems to have a structure that is not (yet) supported!",pdfName.Data(),config->GetName()));
        return false;
      }
      std::vector<RooHistFunc*>* vhf = RooSimultaneousHelper::getAllDataHists(simpdf);
      */
      
      //this is a "manual" hack to get all nominal RooHistFuncs
      //std::set<const RooAbsArg*>* allHistFuncs = TSUtils::getContributionsRecursive(simpdf, RooHistFunc::Class(), false);
      //more complete (and direct) way:
      std::list<RooAbsData*> allData = ws->allData();
      std::list<RooAbsData*> embeddedData = ws->allEmbeddedData();
      
      
      TQFolderIterator operationsItr(config->getListOfFolders("?"),true); //retrieve the individual scaling operations we should perform
      while (operationsItr.hasNext()) {
        TQFolder* op = operationsItr.readNext();
        //read some basic options for this part of the payload
        TString pattern = op->getTagStringDefault("name",op->GetName());
        double scale = op->getTagDoubleDefault("scale",1.);
        if (scale>0 && op->getTagBoolDefault("invert",false)) {
          scale = 1./scale;
          std::cout<<"inverting scale factor to "<<scale<<std::endl;
        }
        
        for (RooAbsData* data : allData) {
          if (!data) continue;
          RooDataHist* dh = dynamic_cast<RooDataHist*>(data);
          if (!dh || !TQStringUtils::matchesFilter(dh->GetName(), pattern, ",")) {
            //std::cout<<"Not a DH: "<<data->GetName()<<" (type: "<< data->IsA()->GetName()<<")" <<std::endl;
            //data->Print();
            //RooDataSet* ds = dynamic_cast<RooDataSet*>(data);
            //if (ds) RooDataSetHelper::printInfo(ds);
            continue;
          }
          std::cout<<"Scaling DataHist "<<dh->GetName()<<std::endl;
          TSUtils::scaleHist(dh,scale);
        }
        for (RooAbsData* data : embeddedData) {
          if (!data) continue;
          RooDataHist* dh = dynamic_cast<RooDataHist*>(data);
          if (!dh || !TQStringUtils::matchesFilter(dh->GetName(), pattern, ",")) {
            //std::cout<<"Not a DH: "<<data->GetName()<<" (type: "<< data->IsA()->GetName()<<")" <<std::endl;
            continue;
          }
          std::cout<<"Scaling DataHist "<<dh->GetName()<<std::endl;
          TSUtils::scaleHist(dh,scale);
        }
        
        /*
        for (const RooAbsArg*const& raa : *allHistFuncs) {
          const RooHistFunc* hf = dynamic_cast<const RooHistFunc*>(raa);
          if (!hf) continue;
          //check if this RooHistFunc matches our selection criteria and skip if not
          if (!TQStringUtils::matchesFilter(hf->GetName(), pattern, ",")) {
            std::cout<<"Skipping HistFunc "<<hf->GetName()<<std::endl;
            continue;
          }
          std::cout<<"Scaling HistFunc "<<hf->GetName()<<std::endl;
          RooDataHist* dh = RooHistFuncHelper::getDataHist(hf);
          TSUtils::RooDataHistHelper::scaleHist(dh,scale);          
        }
        */
        
      }
      //delete allHistFuncs;
      //ws->loadSnapshot("SnSh_AllVars_Nominal");
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ScaleProcesses(),"ScaleProcesses");
  }
}
