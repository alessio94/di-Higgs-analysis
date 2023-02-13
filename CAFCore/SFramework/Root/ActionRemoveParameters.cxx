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

/*<cafdoc name=RemoveParameters>
  RemoveParameters
  ===========================
  
  This action purges a workspace from all parameters with names matching 
  a given expression.
  
  Note: This has been tested to work by comparing the results of a (ggF HWW)
  workspace build with and without MC stat. uncertainties enabled. When using
  this action one should be very well aware of the workspace's internal
  structure and the potential interplay of different parameters/objects.
  
  This action also supports removing parameters from PiecewiseInterpolations
  and FlexibleInterpvars. When removing parameters from those one currently
  (=FIXME!!) 
  has to make sure to list all related parameter names to obtain a workspace
  in a valid state. Example: theo_QCDScale_ggH*,nom_theo_QCDScale_ggH . 
  If the second one is not listed (or the wildcard at the end of the first 
  entry) some elements are not matched, namely the nominal value of the 
  parameter (which confuses, e.g., Hesse!) or the actual constraint term in 
  the p.d.f.
  
  Usage:
  ---------------
  ```
  +RemoveParameters{
    +NameOfWorkspace {
      #comma separated list of (wildcarded) names of objects to remove
      <name = "*gamma_stat*,mc_stat*"> 
    }
  }
  ```
  To get an impression of the structure of a workspace and the naming of objects 
  it contains consider printing it in a ROOT shell:
  ```
  workspace->Print("t")
  ```

</cafdoc> */




namespace TSBaseActions {
    
  class RemoveParameters : public TSStatisticsManager::Action {
    
    void removeFromArgSet(RooArgSet* argset, const TString& filter) const{
      std::vector<RooAbsArg*> toRemove;
      RooAbsArg* arg = nullptr;
      RooFIter itr(argset->fwdIterator());
      while ((arg=itr.next())) {
        if (!TQStringUtils::matchesFilter(arg->GetName(),filter,",")) continue;
        toRemove.push_back(arg);
      }
      
      for (RooAbsArg* rem : toRemove) {
        argset->RecursiveRemove(rem);
      }
    }
    
    void tryRemoveComponents(RooAbsArg* parent, const TString& filter) const {
      if (!parent) return;
      
      const TClass* parentClass = parent->IsA();
      
      if (parentClass == RooSimultaneous::Class()) {
        //case: it's a RooSimultaneous
        return TSUtils::removeConstituentsFromRooSimultaneous(dynamic_cast<RooSimultaneous*>(parent),filter);
      } else if (parentClass == RooRealSumPdf::Class()) {
        TSUtils::removeConstituentsFromRooRealSumPdf(dynamic_cast<RooRealSumPdf*>(parent),filter);
      } else if (parentClass == RooProdPdf::Class()) {
        TSUtils::removeConstituentsFromRooProdPdf(dynamic_cast<RooProdPdf*>(parent),filter);
      } else if (parentClass == RooProduct::Class()) {
        TSUtils::removeConstituentsFromRooProduct(dynamic_cast<RooProduct*>(parent),filter);
      } else if (parentClass == PiecewiseInterpolation::Class()) {
        TSUtils::removeConstituentsFromPiecewiseInterpolation(dynamic_cast<PiecewiseInterpolation*>(parent),filter);
      } else if (parentClass == RooStats::HistFactory::FlexibleInterpVar::Class()) {
        TSUtils::removeConstituentsFromFlexibleInterpVar(dynamic_cast<RooStats::HistFactory::FlexibleInterpVar*>(parent),filter);
      }
    }

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
      //retrieve pdf to edit
      TString parPattern = config->getTagStringDefault("name","thisParameterShouldNotExist"); //could be "gamma_stat_*" for example
      
      
      RooArgSet functions = ws->allFunctions();
      RooFIter funcItr(functions.fwdIterator());
      RooAbsArg* func = nullptr;
      while((func = funcItr.next())) {
        tryRemoveComponents(func,parPattern);
        if (!TQStringUtils::matchesFilter(func->GetName(),parPattern,",")) continue;
        //std::cout<<"Removing function "<<func->GetName()<<std::endl;
        ws->RecursiveRemove(func);
      }
      RooArgSet pdfs = ws->allPdfs();
      RooFIter pdfItr(pdfs.fwdIterator());
      RooAbsArg* pdf = nullptr;
      while((pdf = pdfItr.next())) {
        tryRemoveComponents(pdf,parPattern);
        if (!TQStringUtils::matchesFilter(pdf->GetName(),parPattern,",")) continue;
        //std::cout<<"Removing pdf "<<pdf->GetName()<<std::endl;
        ws->RecursiveRemove(pdf);
      }
      RooArgSet variables = ws->allVars();
      RooFIter varItr(variables.fwdIterator());
      RooAbsArg* var = nullptr;
      while((var = varItr.next())) {
        tryRemoveComponents(var,parPattern); //checks if var is one of multiple Roo??? classes that have constituent terms (e.g. RooProduct, RooRealSum,...) and detaches consitituents matching our filter
        if (!TQStringUtils::matchesFilter(var->GetName(),parPattern,",")) continue;
        //std::cout<<"Removing variable "<<var->GetName()<<std::endl;
        ws->RecursiveRemove(var);
      }
      
      std::list<TObject*> genObs = ws->allGenericObjects();
      for (TObject*const & gob : genObs) {
        if (TQStringUtils::matchesFilter(gob->GetName(),parPattern,",")) {
          //std::cout<<"Removing generic object "<<gob->GetName()<<std::endl;
          ws->RecursiveRemove(gob);
        } else {
          RooStats::ModelConfig* mcfg = dynamic_cast<RooStats::ModelConfig*>(gob);
          if (mcfg) {
            //we use the ModelConfig to extract some names of sets in the workspace that we need to clean
            std::vector<RooAbsArg*> toRemove;
            const RooArgSet* nuisancePars = mcfg->GetNuisanceParameters();
            RooFIter npItr(nuisancePars->fwdIterator());
            RooAbsArg* arg = nullptr;
            while ((arg=npItr.next())) {
              if (!TQStringUtils::matchesFilter(arg->GetName(),parPattern,",")) continue;
              ws->RecursiveRemove(arg);
            }
            const RooArgSet* globObs = mcfg->GetGlobalObservables();
            RooFIter globsItr(globObs->fwdIterator());
            while((arg=globsItr.next())) {
              if (!TQStringUtils::matchesFilter(arg->GetName(),parPattern,",")) continue;
              ws->RecursiveRemove(arg);
            }
            
            
          } 
        }
      }
      //it seems some items are not removed correctly up to here, so let's hack a bit more...
      std::map<std::string,RooArgSet>* namedSets = TSUtils::getNamedSets(ws);
      for (auto & item : *namedSets) {
        RooFIter itr(item.second.fwdIterator());
        RooAbsArg* entry = nullptr;
        while((entry = itr.next())) {
          if (!TQStringUtils::matchesFilter(entry->GetName(),parPattern,",")) continue;
          item.second.RecursiveRemove(entry);
        }
      }
      
      
      
      RooLinkedList snapshots = TSUtils::getListOfSnapshots(ws);
      RooFIter snapItr(snapshots.fwdIterator());
      RooAbsArg* snap_ = nullptr;
      while((snap_=snapItr.next())) {
        if (!snap_ || (snap_->IsA() != RooArgSet::Class())) continue;
        RooArgSet* snap = (RooArgSet*)snap_; //we can't use a dynamic_cast here for some odd reason... We (sort of) have to rely on RooFit only having RooArgSets as snapshots (possibly this is a reason why a dyncast fails: https://stackoverflow.com/questions/590371/dynamic-cast-fails)
        removeFromArgSet(snap,parPattern);
        
      }
      
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new RemoveParameters(),"RemoveParameters");
  }
}
