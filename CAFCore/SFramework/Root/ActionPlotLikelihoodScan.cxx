#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQLibrary.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSStatisticsPlotter.h"
#include "SFramework/TSUtils.h"

#include "TGraph2D.h"
#include "THistPainter.h"

#include "TSystem.h"
#include <limits>

#define inf std::numeric_limits<double>::infinity()

/*<cafdoc name=PlotLikelihoodScan>
  PlotLikelihoodScan
  ===========================
  
  Plot a likelihood scan. This action supports plotting 
    * several 1D scan curves on the same canvas
    * several 2D scan contours on the same canvas
  
  Usage:
  ---------------

  The following is an example for a single 1D curve.
  ```
  +PlotLikelihoodScan {
    +HWWRun2GGF {
      <arrangement="$TQPATH/../SFramework/share/templates/PlotLikelihoodScanSignificance.txt">
      <outputFile="workspaces/run2-ggf-$(fitLabel)/likelihood.pdf">
      +asmGGF {
        <source = "LikelihoodScans/ScanLikelihood.asmGGF/Scan">
        <style.lineColor=kRed, style.title="#mu_{GGF}">
      }
      <style.title.xAxis = "#mu_{ggF} (profiled)">
      <style.showLegend = true>
    }
  }
  ```

  The following is an example for two 2D contours (asimov and observed) plotted together.
  ```
  +PlotLikelihoodScan {
    +Run2GGF2j {
      <outputFile="./workspaces/run2-comb-$(fitLabel)/likelihood.pdf">
      +2Dasm {
        <source = "/HWWRun2/LikelihoodScans/ScanLikelihood.2Dasm/Scan">
        <style.lineColor=kBlue>
        <style.markerColor=kBlue>
        <style.markerStyle=34>
        <style.showPoints = true>
      }
      +2Dobs {
        <source = "/HWWRun2/LikelihoodScans/ScanLikelihood.2Dobs/Scan">
        <style.lineColor=kRed>
        <style.markerColor=kRed>
        <style.markerStyle=47>
        <style.showPoints = true>
      }
      <style.title.xAxis = "#mu_{ggH}">
      <style.title.yAxis = "#mu_{VBF}">
      <style.showLegend = false>
    }
  }

  ```

  The name of the output file can be determined with the `outputFile` tag globally.

  The curves are added as subfolders to the action.  The source of
  each scan is identified with the `source` tag for each curve
  separately. The source can either point to a result of a
  ScanLikelihood action, or to the result of a ScanContour action.

  Some global options exist to control the styling:
    * `style.title.xAxis`, `style.title.yAxis`: choose titles for the axes.
    * `style.showLegend`, enable/disable showing the legend.

  Additionally, style options can be set for each curve separately:
    * `style.lineColor`, `style.lineStyle` Control the color and style of the lines.
    * `style.showPoints` Control whether points will be drawn to indicate the anchors used for interpolating the smooth curve.
    * `style.markerColor`, `style.markerStyle` Control the color and style of the markers drawn with `style.showPoints=true`.
    * `style.title` Set the title of the curve to be displayed in the legend (if drawn).
    * `showMinimum` Draw a marker to indicate the minimum.
    * `lineOption` Choose the draw option used for the line ("C" or "L")
    * `yTag` instead of using the defalt `y` tag of the scan result,
      use some other tag to extract the result. The result can be
      scaled using the `yScale` option. One typical use case is to
      plot the non-sanitized, not minimum-subtracted ("raw") curve with the
      combination `yTag="minNll", yScale=.5`.

  Some specific options exist for 1D plots:
    * `outlierThreshold` control at which threshold (above the
      minimum) outliers should be removed from the plot. Can be
      helpful to get rid of points from failed fits.
    * `plotWalk=x` If given, in addition to the likelihood curve, an
      additional curve will be shown, indicating how far the
      individual best-fit-values of the points in the likelihood curve
      are away from each other -- showing the `walking distance` from
      one point to the next. As the total value of this is
      meaningless, the curve can be scaled freely, for which the value
      of this tag can be used.

  Some specific options exist for 2D contour plots:

    * `style.shading` select whether contours should be progressively
       faded out (such that, e.g., the 2-sigma contour has a less
       strong color than the 1-sigma contour).
    * `contours.x=y` Define a contour `x` at value `y`. If not given,
      the default values for the 1,2,3,4,5 sigma contours in 2D will
      be used.
    * `colormap` Choose a color in which to overlay the area of the curve
    * `interpolate.x,interpolate.y` By default, the graph triangulation will be used,
      which can sometimes lead to sub-optimal plotting results (wiggly
      lines). With this option, the scan will first be converted into
      a 2D histogram with a defined, regular grid before the contours
      are computed. This tag can be used to decide the number of bins 
      in x and y direction to choose.
    * `binPrecision` By default, the graph triangulation will be used,
      which can sometimes lead to sub-optimal plotting results (wiggly
      lines). With this option, the scan will first be converted into
      a 2D histogram with a defined, regular grid before the contours
      are computed. This tag is an integer defining the number of
      significant digits used to identify the grid lines (a useful
      choice for scans with less than 100 points is 2)
    * When using the `binPrecision` tag, the resulting histogram can
      be auto-sanitized by filling empty bins (i.e., failed fits) from
      their neighbors. The tag `binSmoothing` defines the radius of
      the smoothing to be applied in number of bins (i.e., how large
      the neighborhood is from which values can be
      taken). Additionally, `binSmoothingSources` can be used to
      identify how many bins in a neighborhood need to be filled (at
      least) in order to proceed with the smoothing in that
      neighborhood.

</cafdoc> */

namespace TSBaseActions {
  
  class PlotLikelihoodScan : public TSStatisticsManager::Action {
    
    int makePlot1D(TQFolder* plot, TQFolder* config, TQFolder* source)const{
      TQFolder* graph = new TQFolder(TString::Format("Overlay.%s.Points",config->GetName()));
      TQFolderIterator points(source->getListOfFolders("?"),true);
      TQFolder* walk = NULL;
      double scaleWalk;
      if(config->getTagDouble("plotWalk",scaleWalk)){
        walk = new TQFolder(TString::Format("Overlay.%s.HelperLine",config->GetName()));
      }
      TString ytag = config->getTagStringDefault("yTag","y");
      double yScale = config->getTagDoubleDefault("yScale",1.);
      double outlierThreshold = config->getTagDoubleDefault("outlierThreshold",inf);
      int npoints = 0;
      TQFolder* lastpoint = NULL;
      double ymin=0.;
      while(points.hasNext()){
        TQFolder* point = points.readNext();
        if(!point) continue;
        double y;
        if(point->getTagDouble(ytag,y) && y==y){
          ymin = std::min(y,ymin);
        }
      }
      points.reset();
      while(points.hasNext()){
        TQFolder* point = points.readNext();
        if(!point) continue;
        double x,y;
        if(point->getTagDouble("x0",x) && point->getTagDouble(ytag,y) && x==x && y==y && fabs(x)<inf && fabs(y - ymin)<outlierThreshold){
          TQFolder* newpoint = graph->getFolder(point->getName()+"+");
          newpoint->setTagDouble("y",yScale*(y-ymin));
          newpoint->setTagDouble("x",x);
          npoints++;
        }
        if(walk){
          if(lastpoint){
            TQFolder* thisfit = point->getFolder(".fit/floatParsFinal");
            TQFolder* lastfit = lastpoint->getFolder(".fit/floatParsFinal");
            if(!thisfit || !lastfit){
              manager->error(TString::Format("cannot plot walk without .fit information on point %s!",point->GetName()));
              delete walk;
              walk=NULL;
            } else {
              double diff = TSUtils::diffParameterSets(thisfit,lastfit,false);
              TQFolder* newpoint = walk->getFolder(point->getName()+"+");
              double x1,x2;
              if(point->getTagDouble("x0",x1) && lastpoint->getTagDouble("x0",x2) ){
                newpoint->setTagDouble("x",0.5*(x1+x2));
                newpoint->setTagDouble("y",diff*scaleWalk);
              }
            }
          }
        }
        lastpoint = point;
      }
      TQFolder* graphLine = graph->copy(TString::Format("Overlay.%s.Line",config->GetName()));
      if(!plot->addFolder(graphLine) || !plot->addFolder(graph)){
        manager->error(TString::Format("unable to manipulate folder '%s'",source->getPath().Data()));
      }
      graph->importTags(config);
      graphLine->importTags(config);
      if(walk){
        plot->addFolder(walk);
        walk->importTags(config);
        walk->removeTag("style.title");
      }
      if(config->getTagBoolDefault("showMinimum",false)){
        TQFolder* line = plot->getFolder(TString::Format("Line.%s.Minimum+",config->GetName()));
        TQFolderIterator itr(graph->getListOfFolders("?"));
        double xMin=0;
        double yMin=inf;
        while(itr.hasNext()){
          TQFolder* p = itr.readNext();
          double y = p->getTagDoubleDefault("y",inf);
          if(y<yMin){
            yMin=y;
            xMin=p->getTagDoubleDefault("x0",0);
          }
        }
        line->setTagDouble("x",xMin);
        line->importTags(graphLine);
      }
      return npoints;
    }

    void makePlot2D(TQFolder* plot, TQFolder* config, TQFolder* source)const{
      bool shading = config->getTagBoolDefault("style.shading",true);

      std::vector<std::string> names;
      std::vector<double> thresholds;
      TQIterator contourItr(config->getListOfKeys("contours.*"),true);
      while(contourItr.hasNext()){
        TString name = contourItr.readNext()->GetName();
        double val = 0;
        config->getTagDouble(name,val);
        TQStringUtils::removeLeadingText(name,"contour.");
        names.push_back(name.Data()); thresholds.push_back(val);
      }
      if(names.size() == 0){
        // source of these numbers:
        // https://gitlab.cern.ch/cburgard/RooFitUtils/-/blob/master/scripts/printthresholds.py
        names.push_back("68pcent"  ); thresholds.push_back( 2.279);
        names.push_back("95pcent"  ); thresholds.push_back( 5.991);
        names.push_back("99pcent"  ); thresholds.push_back( 9.210);
        names.push_back("99.9pcent"); thresholds.push_back( 13.816);
      }

      if(!config->hasTagString("xTag")) config->setTagString("xTag","x0");
      if(!config->hasTagString("yTag")) config->setTagString("yTag","x1");
      if(!config->hasTagString("zTag")) config->setTagString("zTag","y");
      
      TQFolder* point = TSStatisticsPlotter::findGraphMinimum(source);
      double xmin,ymin;
      point->getTagDouble("x0",xmin);
      point->getTagDouble("x1",ymin);
      
      int precision = -1;
      bool makeAccurateHisto = config->getTagInteger("binPrecision",precision);
      int colormap = config->getTagIntegerDefault("colormap",-1);
      int interpx,interpy;
      bool makeInterpolatedHisto = config->getTagInteger("interpolate.x",interpx) && config->getTagInteger("interpolate.y",interpy);
      TGraph2D* helperGraph = TSStatisticsPlotter::createGraph2D(source,config);
      helperGraph->SetName(config->GetName());
      std::vector<TList*> contours;
      if(makeAccurateHisto || makeInterpolatedHisto){
        TH2* helperHisto = NULL;
        if(makeAccurateHisto){
          helperHisto = TQHistogramUtils::createHistogram(helperGraph,precision);
        } else {
          helperHisto = TQHistogramUtils::createHistogram(helperGraph,interpx,interpy);
        }
        // hack to avoid filling in the minimum as a "hole"
        int minbin = helperHisto->FindBin(xmin,ymin);
        helperHisto->SetBinContent(minbin,1e-9);
        int delta = config->getTagIntegerDefault("binSmoothing",1);
        int minSources = config->getTagIntegerDefault("binSmoothingSources",delta);
        while(TQHistogramUtils::countHoles(helperHisto) > 0){
          int nfixed = TQHistogramUtils::fillHoles2DMax(helperHisto,0,false,delta,delta,minSources);
          if(nfixed == 0) break;
        }
        contours = TQHistogramUtils::getContourList(helperHisto,thresholds);
        if(colormap>=0){
          helperHisto->SetFillColor(colormap);
          helperHisto->SetMinimum(0);          
          helperHisto->SetMaximum(thresholds[thresholds.size()-1]);
          plot->getFolder("Colormaps+")->addObject(helperHisto);          
        } else {
          delete helperHisto;
        }
      } else {
        for(size_t contour = 0; contour < thresholds.size(); ++contour){
          TList* l = helperGraph->GetContourList(thresholds[contour]);
          contours.push_back(l);
        }
      }

      TString lineOption = config->getTagStringDefault("lineOption","L");
      for(size_t contour = 0; contour < thresholds.size(); ++contour){
        TList* l = contours[contour];
        if(!l){
          manager->error(TString::Format("unable to retrieve contour list for contour %d!",(int)(contour)));
          continue;
        }
        l->SetOwner(true);
        TQIteratorT<TGraph> itr(l,true);
        int i = 0;
        while(itr.hasNext()){
          TGraph* g = itr.readNext();
          TQFolder* graph = TSStatisticsPlotter::convertGraph(g);
          graph->SetName(TString::Format("Overlay.Contour.%s.%s.%d",config->GetName(),names[contour].c_str(),i));
          ++i;
          graph->importTags(config);
          if(shading) graph->setTagDouble("style.lineAlpha",1.-(1.0*contour)/names.size());
          if(config->getTagBoolDefault("style.showPoints",false)){
            graph->setTagString("style.drawOption",lineOption+"P");
            graph->setTagInteger("style.markerColor",kBlack);
            graph->setTagInteger("style.markerStyle",1);
          } else {
            graph->setTagString("style.drawOption",lineOption);
          }
          if(!plot->addFolder(graph)){
            manager->error(TString::Format("unable to manipulate folder '%s' trying to add graph '%s'",plot->getPath().Data(),graph->GetName()));
          }
        }
      }
      delete helperGraph;

      if(config->getTagBoolDefault("showMinimum",true)){
        TQFolder* minPoint = new TQFolder(TString::Format("Overlay.Point.%s.min",config->GetName()));
        minPoint->setTagString("style.drawOption","P");
        minPoint->importTags(config);
        TQFolder* pmin = minPoint->getFolder("p.0+");
        //double zmin = pow(10.,18); // BW: hashed (unused variable)
        
        // TQFolder* point = TSStatisticsPlotter::findGraphMinimum(source); // BW: hashed (unused variable)
        pmin->setTagDouble("x",xmin * config->getTagDoubleDefault("xScale",1.));
        pmin->setTagDouble("y",ymin * config->getTagDoubleDefault("yScale",1.));
        
        if(!plot->addFolder(minPoint)){
          manager->error(TString::Format("unable to manipulate folder '%s' trying to add minimum point '%s'",plot->getPath().Data(),minPoint->GetName()));
        }
      }
    }


    void makeContour(TQFolder* plot, TQFolder* config, TQFolder* source)const{
      bool shading = config->getTagBoolDefault("style.shading",true);
      TString lineOption = config->getTagStringDefault("lineOption","L");
      TQFolderIterator contours(source->getListOfFolders("?"),true);
      int icont = 0;
      int nconts = contours.getCollection()->GetSize();
      while(contours.hasNext()){
        TQFolder* contour = contours.readNext();
        int i=0;
        TString name = contour->getTagStringDefault("label",contour->GetName());
        TQFolder* graph = contour->copy(TString::Format("Overlay.Contour.%s.%s.%d",config->GetName(),name.Data(),i));
        int npoints = contour->getTagIntegerDefault("n",-1);
        if(npoints > 0){
          TQFolder* p0 = graph->getFolder("p.0");
          if(p0){
            TQFolder* plast = p0->copy(TString::Format("p.%d",npoints+1));
            graph->addFolder(plast);
          }
        }
        graph->importTags(config);        
        if(config->getTagBoolDefault("style.showPoints",false)){
          graph->setTagString("style.drawOption",lineOption+"P");
          graph->setTagInteger("style.markerColor",kBlack);
          graph->setTagInteger("style.markerStyle",1);
        } else {
          graph->setTagString("style.drawOption",lineOption);
        }
        if(shading) graph->setTagDouble("style.lineAlpha",1.-(1.0*icont)/nconts);
        if(!plot->addFolder(graph)){
          manager->error(TString::Format("unable to manipulate folder '%s' trying to add graph '%s'",plot->getPath().Data(),graph->GetName()));
        }
        icont++;
      }
      if(config->getTagBoolDefault("showMinimum",true)){
        TQFolder* minPoint = new TQFolder(TString::Format("Overlay.Point.%s.min",config->GetName()));
        minPoint->setTagString("style.drawOption","P");
        minPoint->importTags(config);
        TQFolder* pmin = minPoint->getFolder("p.0+");
        pmin->setTagDouble("x",source->getTagDoubleDefault("xmin",0));
        pmin->setTagDouble("y",source->getTagDoubleDefault("ymin",0));        
        if(!plot->addFolder(minPoint)){
          manager->error(TString::Format("unable to manipulate folder '%s' trying to add minimum point '%s'",plot->getPath().Data(),minPoint->GetName()));
        }
      }
    }
    
    
    bool execute(TQFolder * config) const override {
      
    	TQFolder * results = manager->getResults();
    	if (!config) {
    		return 0;
    	}
      
      TString outputFile = config->getTagStringDefault("outputFile", "plot.pdf");
    	TSStatisticsPlotter pl;
      TString tqpath = TQLibrary::getTQPATH();
      
      TString path = config->getTagStringDefault("arrangement",tqpath+"/../SFramework/share/templates/PlotLikelihoodScan.txt");
      gSystem->ExpandPathName(path);
      
      TQFolder* plot = TQFolder::newFolder("plot");

      int nOK = 0;
      TQFolderIterator itr(config->getListOfFolders("?"));
      while(itr.hasNext()){
        TQFolder* plotconfig = itr.readNext();
        if(!plotconfig) continue;
        
        TString sourcePath = plotconfig->getTagStringDefault("source","LikelihoodScan/POI");
        TQFolder* source = results->getFolder(config->getName()+"+")->getFolder(sourcePath);
        if(source){
          source->sortByName();
          TString type = source->getTagStringDefault("content","points");
          int ndim = source->getTagIntegerDefault("nDim",1);
          if(type == "points"){
            if(ndim==1){
              manager->info(TString::Format("plotting %s",sourcePath.Data()));
              if(makePlot1D(plot, plotconfig,source) == 0){
                manager->error(TString::Format("unable to extract any points from path '%s'!",sourcePath.Data()));
                continue;
              }
            } else if(ndim==2){
              manager->info(TString::Format("plotting contours for %s",sourcePath.Data()));
              makePlot2D(plot, plotconfig,source);
            } else {
              manager->error(TString::Format("plotting of %d-dimensional liklihood scan '%s' not supported yet!",ndim,sourcePath.Data()));
              continue;
            }
          } else if(type == "contour"){
            if(ndim != 2){
              manager->error(TString::Format("cannot plot likelihood contour with dimensionality %d",ndim));
            } else {
              makeContour(plot,plotconfig,source);
            }
          } else {
            manager->error(TString::Format("cannot plot likelihood with unknown content type '%s'",type.Data()));
          }
        } else {
          manager->error(TString::Format("unable to access likelihood scan from path '%s'",sourcePath.Data()));
          continue;
        }
        nOK++;
      }

      if(nOK>0){
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
      }
      return false;
    };
        
  };  
    
    
  namespace {
    bool available = TSStatisticsManager::registerAction(new PlotLikelihoodScan(),"PlotLikelihoodScan");
  }
}
