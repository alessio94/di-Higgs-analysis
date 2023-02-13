#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSUtils.h"
#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSStatisticsCalculator.h"

/*<cafdoc name=MultiWorkspaceLikelihoodScan>
  MultiWorkspaceLikelihoodScan
  ===========================
  
  Many analyses do not have a continuous model of the likelihood as a
  function of their parameter of interest. Often times, high mass
  searches and similar analyses who do not have such a model need to
  resort to a simple solution: They create one workspace for every
  (Higgs mass) scenario. In that series of workspaces, they then fit
  the likelihood tot he observed data and ultimately retrieve a single
  number from each of these fits -- the likelihood value. Given the
  POI (higgs mass) values associated to these input workspaces, those
  values can then be combined into something resembling a proper
  likelihood scan.
  
  Usage:
  ---------------
  ```
  +MultiWorkspaceLikelihoodScan {
    +Run1 {
      +cosa {
        +ggHcosa000 { <cosa=0.> }
        +ggHcosa005 { <cosa=.05> }
        +ggHcosa006 { <cosa=.06> }
        +ggHcosa008 { <cosa=.08> }
        +ggHcosa010 { <cosa=.10> }
        +ggHcosa012 { <cosa=.12> }
        +ggHcosa015 { <cosa=.15> }
        +ggHcosa020 { <cosa=.20> }
        +ggHcosa025 { <cosa=.25> }
        +ggHcosa030 { <cosa=.30> }
        +ggHcosa035 { <cosa=.35> }
        +ggHcosa040 { <cosa=.40> }
        +ggHcosa045 { <cosa=.45> }
        +ggHcosa050 { <cosa=.50> }
        +ggHcosa055 { <cosa=.55> }
        +ggHcosa060 { <cosa=.60> }
        +ggHcosa065 { <cosa=.65> }
        +ggHcosa070 { <cosa=.70> }
        +ggHcosa075 { <cosa=.75> }
        +ggHcosa080 { <cosa=.80> }
        +ggHcosa085 { <cosa=.85> }
        +ggHcosa090 { <cosa=.90> }
        +ggHcosa095 { <cosa=.95> }
        +ggHcosa100 { <cosa=1.0> }
        <snapshot="default",data="obsData"> @ ?;
        <fit.initParam.mu=1.1> @ ?;
      }
    }
  }

  ```
</cafdoc> */

namespace TSBaseActions {

  class ActionMultiWorkspaceLikelihoodScan : public TSStatisticsManager::Action {

    class DummyCalculator : public TSStatisticsCalculator {
    public:
      DummyCalculator(const TString& name, RooWorkspace* ws) : TSStatisticsCalculator(name,ws) {};
      virtual TQFolder* runCalculation(TQFolder*) override {
        return NULL;
      }
    };

    
    bool execute(TQFolder * config) const override {
      
      TQFolder* result = results()->getFolder(TQFolder::concatPaths(config->GetName(),"LikelihoodScans")+"+");

      TQFolderIterator itr(config->getListOfFolders("?"),true);
      while(itr.hasNext()){
        TQFolder* scan = itr.readNext();
      
        TQFolder* thisresult = result->getFolder(scan->getName()+"+");
        thisresult->setTagInteger("nDim",1);
        TString x = scan->getTagStringDefault("x",scan->GetName());
        
        TQFolderIterator points(scan->getListOfFolders("?"),true);
        double minval = std::numeric_limits<double>::infinity();

        std::map<double,TQFolder*> fitresults;
        while(points.hasNext()){
          TQFolder* point = points.readNext();
          TQTaggable options;
          options.importTagsWithoutPrefix(point,"fit.");
          RooWorkspace* ws = dynamic_cast<RooWorkspace*>(workspaces()->getObject(point->getName()));
          if(!ws){
            throw std::runtime_error(TString::Format("unable to obtain workspace '%s'",point->GetName()).Data());
          }
          TString snapshot = point->getTagStringDefault("snapshot","SnSh_AllVars_Nominal");
          if(!ws->loadSnapshot(snapshot)){
            manager->warn(TString::Format("unable to load snapshot '%s' in workspace '%s'",snapshot.Data(),ws->GetName()).Data());
          }
          double xval = 0.;
          if(!point->getTagDouble(x,xval)){
            throw std::runtime_error(TString::Format("unable to obtain x-value '%s' from point '%s'",x.Data(),point->GetName()).Data());
          }
          TString dataname = point->getTagStringDefault("data","obsData");

          DummyCalculator calc(point->getName(),ws);
          manager->info(TString::Format("fitting '%s' to '%s'",point->GetName(),dataname.Data()).Data());
          
          TQFolder* fitresult = calc.fitPdfToData(dataname,&options);
          if(!fitresult) continue;
          
          double yval = fitresult->getTagDoubleDefault("minNll",0.);
          int status = fitresult->getTagIntegerDefault("status",0.);

          if(status == 0 || status == 1){
            minval = std::min(yval,minval);
            fitresults[xval] = fitresult;
            manager->info(TString::Format("converged with status=%d, minNll=%g",status,yval).Data());
          } else {
            manager->info(TString::Format("did not converge, status=%d'",status).Data());
            delete fitresult;
          }
        }
          
        thisresult->setTagDouble("minNll",minval);
        
        int i=0;
        for(const auto& r:fitresults){
          double x = r.first;
          TQFolder* fitresult = r.second;
          double val = fitresult->getTagDoubleDefault("minNll",0.);
          TString pname = TString::Format("p.%d+",i);
          TQFolder* p = thisresult->getFolder(pname);
          fitresult->SetName(".fit");
          p->addFolder(fitresult);
          double y = 2 * (val - minval);
          p->setTagDouble("y",y);
          p->setTagDouble("x0",x);
          ++i;
        }
      }
      
      return true;
    }
    
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ActionMultiWorkspaceLikelihoodScan(),"MultiWorkspaceLikelihoodScan");
  }
    
}
