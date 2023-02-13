#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQHistogramUtils.h"

#include "SFramework/TSStatisticsManager.h"

namespace TSBaseActions {
  
  class SmoothHistograms : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      TString outputdir = config->replaceInText(config->getTagStringDefault("outputDir"));

      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        models()->print();
        return false;
      }

      int times = config->getTagIntegerDefault("times",1);
      TString option = config->getTagStringDefault("option","");
      bool verbose = config->getTagBoolDefault("verbose",false);
      
      std::vector<TString> select = config->getTagVString("select");
      if(select.empty()) select.push_back("Nominal/*");

      TQFolder* histograms = model->getFolder(".Histograms");
      
      for(const auto& s:select){
        manager->info(TString::Format("smoothing '%s' %d times with option '%s'",s.Data(),times,option.Data()));
        TQIterator itr(histograms->getListOfObjectPaths(s),true);
        while(itr.hasNext()){
          TObject * obj = itr.readNext();
          TString path(obj->GetName());
          TString objname = TQFolder::getPathTail(path);
          TQFolder* container = histograms->getFolder(path);
          TQFolder* beforeContainer = container->getFolder(".BeforeSmoothing+");
          TH1* hist = dynamic_cast<TH1*>(container->getObject(objname));
          if(!hist) continue;
          if(verbose) manager->info(TString::Format("smoothing '%s' at '%s'",hist->GetName(),container->getPath().Data()));
          TH1* copy = TQHistogramUtils::copyHistogram(hist);
          beforeContainer->addObject(copy);
          hist->Smooth(times,option);
        }
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new SmoothHistograms(),"SmoothHistograms");
  }
}
