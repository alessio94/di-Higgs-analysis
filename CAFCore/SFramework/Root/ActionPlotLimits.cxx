#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSStatisticsPlotter.h"
#include "SFramework/TSUtils.h"

#include "TGraph2D.h"
#include "THistPainter.h"

#include "TSystem.h"
#include <limits>

#include "QFramework/TQLibrary.h"

#define inf std::numeric_limits<double>::infinity()

/*<cafdoc name=PlotLimits>
  PlotLimits
  ===========================
  
  Plot a limit scan.
  
  Usage:
  ---------------

  The following is an example for a single expected limit.
  ```
  +PlotLimits {
    +allMasses {
      <outputFile="plot.pdf">
      +expected{
        +mh600 {
          # identify the masses and nominal cross sections of each mass point
          <x=600>
          <yScale=1.3207e-02>
          <source="/mh600/Limits/asimov">
        }
        +mh800 {
          # identify the masses and nominal cross sections of each mass point
          <x=800>
          <yScale=9.1080e-03>
          <source="/mh800/Limits/asimov">
        }
        +mh1000 {
          # identify the masses and nominal cross sections of each mass point
          <x=1000>
          <yScale=1.5939e-03>
          <source="/mh1000/Limits/asimov">
        }
        # identify the tags to be used from each data source
        <y="exp_upper_med", yUp.1="exp_upper_p1s", yDn.1="exp_upper_m1s", yUp.2="exp_upper_p2s", yDn.2="exp_upper_m2s"> @ ?;
        # plot cosmetics
        <style.title="expected">
        <style.lineStyle=2>
        <xLabel="m_{H}">
        <yLabel="#sigma_{H}">
      }
    }
  }
  ```
</cafdoc> */

namespace TSBaseActions {
  
  class PlotLimits : public TSStatisticsManager::Action {
    
    bool execute(TQFolder * config) const override {
      
    	TQFolder * results = manager->getResults();
    	if (!config) {
    		return 0;
    	}
      
      TString outputFile = config->getTagStringDefault("outputFile", "plot.pdf");
    	TSStatisticsPlotter pl;
      TString tqpath = TQLibrary::getTQPATH();
      
      TString path = config->getTagStringDefault("arrangement",tqpath+"/../SFramework/share/templates/PlotLimits.txt");
      gSystem->ExpandPathName(path);
      
      TQFolder* plot = TQFolder::newFolder("plot");

      int nOK = 0;
      TQFolderIterator itr(config->getListOfFolders("?"));
      while(itr.hasNext()){
        TQFolder* plotconfig = itr.readNext();
        if(!plotconfig) continue;
        
        int nPoints=0;
        bool showBands = plotconfig->getTagBoolDefault("showBands",false);
        bool isReference = plotconfig->getTagBoolDefault("isReference",false);
        bool isObserved = plotconfig->getTagBoolDefault("isObserved",!showBands);

        TQFolderIterator points(plotconfig->getListOfFolders("?"));
        while(points.hasNext()){
          TQFolder* point = points.readNext();
          if(!point) continue;
          TString sourcePath = point->getTagStringDefault("~source","Limits/asimov");
          TQFolder* result = results->getFolder(config->getName()+"+");
          if(!result){
            manager->error(TString::Format("unable to access result '%s'",config->GetName()));
            continue;
          }
          TQFolder* source = result->getFolder(sourcePath);
          if(!source){
            manager->error(TString::Format("unable to access limit point '%s' from path '%s' from result '%s'",point->getPath().Data(),sourcePath.Data(),config->GetName()));
            continue;
          }
          double x = point->getTagDoubleDefault("x",1.);
          TString yname = "";
          if(isObserved) {
            yname = point->getTagStringDefault("~y","upper");
          } else {
            yname = point->getTagStringDefault("~y","exp_upper_med");
          }
          double yScale = point->getTagDoubleDefault("~yScale",1.);
          double yval = 0.;
          if(isReference) { 
            yval = 1.0;
          } 
          else {
            yval = source->getTagDoubleDefault(yname,0);
          }

          if(showBands){
            int i=1;
            TString ynameUp, ynameDn;
            std::vector<TString> ynamesUp, ynamesDn;            
            while(point->getTagString(TString::Format("~yUp.%d",i),ynameUp) && point->getTagString(TString::Format("~yDn.%d",i),ynameDn)){
              ++i;
              ynamesUp.push_back(ynameUp);
              ynamesDn.push_back(ynameDn);
            }
            if(ynamesUp.size() == 0){
              for(size_t i=0; i<2; ++i){
                ynamesUp.push_back(TString::Format("exp_upper_p%ds",(int)i+1));
                ynamesDn.push_back(TString::Format("exp_upper_m%ds",(int)i+1));
              }
            }
            for(size_t i=ynamesUp.size(); i>0; --i){
              double yUp = source->getTagDoubleDefault(ynamesUp[i-1],yval);
              double yDn = source->getTagDoubleDefault(ynamesDn[i-1],yval);
              TQFolder* pt = plot->getFolder(TString::Format("Overlay.%s.band.%ds/p.%d+",plotconfig->GetName(),(int)i,nPoints));
              pt->setTagDouble("x",x);
              pt->setTagDouble("y",yval*yScale);
              pt->setTagDouble("yp",(yval+fabs(yUp-yval))*yScale);
              pt->setTagDouble("yn",(yval-fabs(yDn-yval))*yScale);
            }
          }
          TQFolder* pt = plot->getFolder(TString::Format("Overlay.%s.med/p.%d+",plotconfig->GetName(),nPoints));
          pt->setTagDouble("x",x);
          pt->setTagDouble("y",yval*yScale);
          
          plot->getFolder(TString::Format("Overlay.%s.med",plotconfig->GetName()))->importTags(plotconfig);
          
          nPoints++;
        }
        if(nPoints>0) nOK++;
      }


      plot->setGlobalOverwrite(false);
      plot->importFromTextFile(TQPathManager::getPathManager()->getTargetPath(path).c_str());
      TString addElements;
      if(config->getTagString("addElements",addElements)){
        TString errMsg;
        if(!plot->importFromText(addElements,errMsg)){
          manager->error(errMsg);
        }
      }
      plot->setGlobalOverwrite(true);
      plot->importTags(config);
      TCanvas * c = pl.plot(plot);
      if (c) {
        c->SaveAs(TQPathManager::getPathManager()->getTargetPath(outputFile).c_str());
        delete c;
      } 
      delete plot;
      return c;
    };
        
  };  
    
    
  namespace {
    bool available = TSStatisticsManager::registerAction(new PlotLimits(),"PlotLimits");
  }
}
