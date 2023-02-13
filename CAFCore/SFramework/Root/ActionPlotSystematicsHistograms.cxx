#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQDefaultPlotter.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQPathManager.h"
#include "QFramework/TQSystematics.h"

#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"

#include <vector>
#include <set>

namespace{
  template<class X,class Y> bool in(const std::vector<X>& v, const Y& e){
    return std::find(v.begin(), v.end(), e) != v.end();
  }
}

/*<cafdoc name=PlotSystematicsHistograms>
  PlotSystematicsHistograms
  ===========================
  
  Plot the systematic variation histograms from a model. 

  Usage:
  ---------------
  ```
    +PlotSystematicsHistograms.inputs {
      +HWWRun2GGF{
    	<outputDir = "./workspaces/run2-ggf-$(fitLabel)/systematicsInputs">
        <showInputs=true, warnMissing=false, printSummary=true, exportSummary="summary.txt">
        <variationFilter="*">

        # (optional)
        +CompareToModels {
           +HWWVBFSimple {
            +Plotter{
              +Up {
                <color=80, title="Raw Up", sub.drawOption="hist", .isData=true, histFillColor=0, histMarkerStyle=20>
              }
              +Down {
                <color=30, title="Raw Down", sub.drawOption="hist", .isData=true, histFillColor=0, histMarkerStyle=20>
              }
            }
           }
         }

    	  +Plotter {
          <style.subPlot="ratio", geometry.sub.height=0.55, style.sub.min=0.75, style.sub.max=1.25>
          <ensureDirectory=true, style.showTotalBkg=false,  style.nLegendCols=1, style.ratio.title = "Variation / Nominal">
          +Up {
            <color=kRed, title="Up", .isData=true, histMarkerStyle=20>
          }
          +Down {
            <color=kBlue, title="Down", .isData=true, histMarkerStyle=20>
          }
          +Nominal {
            <color=kBlack, title="Nominal", .isBackground=true, histFillStyle=0>
          }
        }
      }
    }

    +PlotSystematicsHistograms.outputs {
      +HWWRun2GGF{
        <outputDir = "./workspaces/run2-ggf-$(fitLabel)/systematicsOutputs">
        <showInputs=false, warnMissing=false>
        <variationFilter="*WW*,*ttbar*,*ggf*,*Wt*">
        <variationFilter="theo_*">
        <sampleFilter="qqWW*jet,ttbar*jet,singletop*jet,sigGGF*jet,sigVBF*jet">
        <channelFilter="*SR*">
        +Plotter {
          <style.subPlot="ratio", geometry.sub.height=0.55>
          <ensureDirectory=true, style.showTotalBkg=false, style.nLegendCols=1, style.ratio.title = "Variation / Nominal">
          +Up {
            <color=kRed, title="Up", .isData=true>
          }
          +Down {
            <color=kBlue, title="Down", .isData=true>
          }
        }
      }
    }
  ```

  This action supports a wealth of options:
  * `outputDir="somedir"` defines the directory to which the plots will be written
  * `showInputs=true/false` define whether the systemtics inputs (before pruning/sanitization) or outputs (after pruning/sanitization) should be shown
  * `varationFilter`, `sampleFilter`, `channelFilter`: select which histograms to show in terms of variations, samples and channels
  * `warnMissing=true/false` select whether to emit a warning about missing systematic histograms
  * `+CompareToModels` can be used to configure other models to compare to. The name of the folder inside corresponds to the model name.

  The contents of the plots can be controlled with a subfolder
  `Plotter`. Subfolders to this define the contents of the plots in
  terms of processes, where the histograms `Up`, `Down` and `Nominal`
  are available.  The `Plotter` folder accepts any and all plotting
  style options.

</cafdoc> */

namespace TSBaseActions {
  
  class PlotSystematicsHistograms : public TSStatisticsManager::Action {

    // ---------------------------------------------------------------
    // helper functions to manage the samplefolder that is used as input to TQDefaultPlotter
    // always two variants one for the nominal model and one for potential alternative models
    
    void addHistFolderAndProcess(TQFolder* config, const TString& varName, TQSampleFolder* base, TQDefaultPlotter* pl)  const {
      TQSampleFolder* sf = base->getSampleFolder(varName+"+");
      sf->getFolder(".histograms+");
      pl->addProcess(varName, config->getFolder("Plotter/"+varName));
    }
    void addHistFolderAndProcess(TQFolder* config, const TString& otherModelName, const TString&varName, TQSampleFolder* base, TQDefaultPlotter* pl)  const {
      TQSampleFolder* sf = base->getSampleFolder(otherModelName+varName+"+");
      sf->getFolder(".histograms+");
      pl->addProcess(otherModelName+varName, config->getFolder("Plotter/"+varName));
    }

    void addHist(TH1* h, const TString& varName, TQSampleFolder* base)  const {
      if (h) {
        base->addObject(h, TQFolder::concatPaths(varName, ".histograms+"));
      }
    }
    void addHist(TH1* h, const TString& otherModelName, const TString& varName, TQSampleFolder* base)  const {
      if (h) {
        base->addObject(h, TQFolder::concatPaths(otherModelName+varName, ".histograms+"));
      }
    }

    void removeHist(TH1* h, const TString& varName, TQSampleFolder* base)  const {
      if (h) {
        base->getFolder(TQFolder::concatPaths(varName, ".histograms"))->Remove(h);
      }
    }
    void removeHist(TH1* h, const TString& otherModelName, const TString& varName, TQSampleFolder* base)  const {
      if (h) {
        base->getFolder(TQFolder::concatPaths(otherModelName+varName, ".histograms"))->Remove(h);
      }
    }

    // -------------------------------------------------------------------
    // helper i/o functions to retrieve histograms
    TH1* getHistFromBaseContainer(TQFolder *container, const TString& path, const TString& name) const {
      TH1* newHist = TQHistogramUtils::copyHistogram(dynamic_cast<TH1*>(container->getObject(path)),name);
      return newHist;
    }

    // --------------------------------------------------------------------------
    // Main execute function
    bool execute(TQFolder * config) const override {
      if(!config) return false;

      TString outputdir = config->replaceInText(config->getTagStringDefault("outputDir"));
      outputdir = TQPathManager::getPathManager()->getTargetPath(outputdir).c_str();

      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        return false;
      }
      
      bool showInputs = config->getTagBoolDefault("showInputs",true);

      TString format = config->getTagStringDefault("format","pdf");

      TString variationFilter = config->getTagStringDefault("variationFilter","*");
      TString sampleFilter    = config->getTagStringDefault("sampleFilter",   "*");
      TString channelFilter   = config->getTagStringDefault("channelFilter",  "*");

      bool warnMissing = config->getTagBoolDefault("warnMissing",true);

      double warnChi2Level = config->getTagBoolDefault("runFits.warnChi2Level",4);
      double warnSlopeSignificanceLevel = config->getTagBoolDefault("runFits.warnSlopeSignificanceLevel",2);

      if(!model->getFolder(".Configuration/Systematics")){
				manager->error("no systematics scheduled!");
				return false;
      }
      if(!model->getFolder(".Configuration/Samples")){
				manager->error("no samples scheduled!");
				return false;
      }
      if(!model->getFolder(".Configuration/Channels")){
				manager->error("no channels scheduled!");
				return false;
      }

      // --------------------------------------------------------------------
      // Construct dummy sample folder to which histograms
      // will be added and to be used with the TQDefaultPlotter
      
      TQSampleFolder* base = new TQSampleFolder("variation");
      TQDefaultPlotter pl(base);

      addHistFolderAndProcess(config, "Nominal", base, &pl);
      addHistFolderAndProcess(config, "Up", base, &pl);
      addHistFolderAndProcess(config, "Down", base, &pl);

      // From alternative models, add folders to dummy sample folder and processes to plotter
      std::vector<TQFolder*> otherModels = {};
      std::vector<TString> otherModelNames = {};
      TQIterator itrOtherModels(config->getListOfFolders("CompareToModels/?"), true);
      while (itrOtherModels.hasNext()) {
        TQFolder* f = dynamic_cast<TQFolder*>(itrOtherModels.readNext());
        if (!f) continue;
        TString otherModelName = f->getName();
        otherModelNames.push_back(otherModelName);
        TQFolder* otherModel = models()->getFolder(otherModelName);
        if (!otherModel) {
          manager->warn("You specified another model to compare plots to " \
                        "but the model could not be found. Please check your configuration.");
          continue;
        }
        if (otherModel) {
          otherModels.push_back(otherModel);
          if (f->getFolder("Plotter/Nominal")) {
            addHistFolderAndProcess(f, otherModelName, "Nominal", base, &pl);
          }
          if (f->getFolder("Plotter/Up")) {
            addHistFolderAndProcess(f, otherModelName, "Up", base, &pl);
          }
          if (f->getFolder("Plotter/Down")) {
            addHistFolderAndProcess(f, otherModelName, "Down", base, &pl);
          }
          
        }
      }

      pl.importTags(config->getFolder("Plotter"));
      std::vector<TString> warnings;
      std::set<TString> warnfiles;

      TString exportTagKey = config->getTagStringDefault("exportTagKey",showInputs  ? 
                                                         "export.fitSlope.ratioGraph_hist_" : 
                                                         "export.fitSlope.hist_");
      TQFolder* container = model->getFolder(".Histograms");
      std::vector<TQFolder*> otherContainers = {};
      for (auto m : otherModels) {
        otherContainers.push_back(m->getFolder(".Histograms"));
      }
      TQFolder* nominal = container->getFolder("Nominal");
      TQFolder* systematics = container->getFolder("Systematics");
      if(!showInputs && !systematics){
        manager->error("cannot plot output systematics without any shape systematics enabled!");
        return false;
      }

      TQFolderIterator variations(model->getFolder(".Configuration/Systematics")->getListOfFolders("?"));
      TQFolderIterator samples   (model->getFolder(".Configuration/Samples"    )->getListOfFolders("?"));
      TQFolderIterator regions   (model->getFolder(".Configuration/Channels"   )->getListOfFolders("?"));
      while(variations.hasNext()){
        TQFolder* variation = variations.readNext();
        if(!variation) continue;
        if(!TQStringUtils::matchesFilter(variation->GetName(),variationFilter,",")) continue;
        manager->info(variation->GetName());
        if(showInputs && variation->getTagBoolDefault("imported",false)) continue;
        //        manager->info("plotting variation "+variation->getName());
        while(regions.hasNext()){
          TQFolder* region = regions.readNext();
          if(!region) continue;
          if(!TQStringUtils::matchesFilter(region->GetName(),channelFilter,",")) continue;
          TH1* hupTotal = NULL;
          TH1* hdownTotal = NULL;
          TH1* hnomTotal = NULL;
          while(samples.hasNext()){
            pl.removeTags("labels.*");
            std::vector<TString> labels;
            TQFolder* sample = samples.readNext();
            if(!sample) continue;
            if(!TQStringUtils::matchesFilter(sample->GetName(),sampleFilter,",")) continue;
            TH1* hup=NULL;
            std::vector<TH1*> otherHups;
            TH1* hdown=NULL;
            std::vector<TH1*> otherHdowns;
            TH1* hnom=NULL;
            std::vector<TH1*> otherHnoms;
            for (size_t i=0; i < otherModels.size(); i++) {
              otherHups.push_back(NULL);
              otherHdowns.push_back(NULL);
              otherHnoms.push_back(NULL);
            }
            TString nompath = TQFolder::concatPaths(region->GetName(),sample->GetName());
            hnom = dynamic_cast<TH1*>(nominal->getObject(nompath));
            if(TQHistogramUtils::getIntegral(hnom) < 1e-6) continue;
            TString sysfolderpath = TQFolder::concatPaths(TQStringUtils::concat("Channel.",region->GetName()),TQStringUtils::concat("Sample.",sample->GetName()));
            TQFolder* sysFolder = model->getFolder(sysfolderpath);
            TQFolder* shapeSystematic = NULL;
            TQFolder* overallSystematic = NULL;
            if(sysFolder){
              shapeSystematic   = sysFolder->getFolder(TQFolder::concatPaths("*",TQStringUtils::concat("HistoSys.",variation->GetName())));
              overallSystematic = sysFolder->getFolder(TQFolder::concatPaths("*",TQStringUtils::concat("OverallSys.",variation->GetName())));
            }
            if(overallSystematic){
              double percent;
              if(overallSystematic->getTagDouble("Percent",percent)){
                labels.push_back(TString::Format("Normalization effect: %.2f%%",percent));
              }
            }            
            if(showInputs){
              TString name;
              // get raw variations
              if(variation->getTagString("Up",name)){
                TString histPath = TQFolder::concatPaths(name,region->GetName(),sample->GetName());
                hup = getHistFromBaseContainer(container, histPath, sample->GetName());
                for (size_t i=0; i < otherModels.size(); i++) {
                  otherHups[i] = getHistFromBaseContainer(otherContainers[i], histPath, sample->GetName());
                }
              }
              if(variation->getTagString("Down",name)){
                TString histPath = TQFolder::concatPaths(name,region->GetName(),sample->GetName());
                hdown = getHistFromBaseContainer(container, histPath, sample->GetName());
                for (size_t i=0; i < otherModels.size(); i++) {
                  otherHdowns[i] = getHistFromBaseContainer(otherContainers[i], histPath, sample->GetName());
                }
              }
            } else {              
              TString upPath = TQFolder::concatPaths("Systematics",variation->getName(),"Up",region->GetName(),sample->GetName());
              TString dnPath = TQFolder::concatPaths("Systematics",variation->getName(),"Down",region->GetName(),sample->GetName());
              if(shapeSystematic){
                shapeSystematic->getTagString("HistoHigh",upPath);
                shapeSystematic->getTagString("HistoLow",dnPath);
              }
              // get massaged histograms, i.e. systematics
              hup   = getHistFromBaseContainer(container, upPath, sample->GetName());
              hdown = getHistFromBaseContainer(container, dnPath, sample->GetName());
              for (size_t i=0; i < otherModels.size(); i++) {
                otherHups[i] = getHistFromBaseContainer(otherContainers[i], upPath, sample->GetName());
                otherHdowns[i] = getHistFromBaseContainer(otherContainers[i], dnPath, sample->GetName());
              }
              
              if(overallSystematic) labels.push_back(TString::Format("Normalization Status: %s",TQSystematics::name(overallSystematic->getTagIntegerDefault("Status",TQSystematics::UNKNOWN))));
              if(shapeSystematic)  labels.push_back(TString::Format("Shape Status: %s",TQSystematics::name(shapeSystematic->getTagIntegerDefault("Status",TQSystematics::UNKNOWN))));
            }
            if(!hnom || !(hup || hdown) || hnom->GetEntries()==0 || (hup && hup->GetEntries()==0) || (hdown && hdown->GetEntries()==0)){
              if(warnMissing) manager->warn("unable to obtain histogram for "+TQFolder::concatPaths(variation->getName(),region->GetName(),sample->GetName()));
              continue;
            }
            for (size_t i=0; i < otherModels.size(); i++) {
              if (!otherHups[i] || otherHdowns[i]) {
                if(warnMissing) manager->warn("unable to obtain histogram of model to compare to for " + TQFolder::concatPaths(variation->getName(),region->GetName(),sample->GetName()));
              }
            }
            if (showInputs) {
              if(hup){
                if (!hupTotal)   {hupTotal   = TQHistogramUtils::copyHistogram(hup);   hupTotal->SetTitle("total");hupTotal->SetName("total");}
                else hupTotal->Add(hup);
              }
              if(hdown){
                if (!hdownTotal) {hdownTotal = TQHistogramUtils::copyHistogram(hdown); hdownTotal->SetTitle("total");hdownTotal->SetName("total");}
                else hdownTotal->Add(hdown);
              }
              if (!hnomTotal)    {hnomTotal  = TQHistogramUtils::copyHistogram(hnom);  hnomTotal->SetTitle("total");hnomTotal->SetName("total");}
              else hnomTotal->Add(hnom);
            }

            addHist(hup, "Up", base);
            addHist(hdown, "Down", base);
            addHist(hnom, "Nominal", base);
            for (size_t i=0; i < otherModels.size(); i++) {
              addHist(otherHups[i], otherModelNames[i], "Up", base);
              addHist(otherHdowns[i], otherModelNames[i], "Down", base);
              addHist(otherHnoms[i], otherModelNames[i], "Nominal", base);
            }

            TString filename = TQFolder::concatPaths(outputdir,variation->getName(),region->getName(),sample->GetName())+"."+format;
            pl.setTagString("labels.info",TQFolder::concatPaths(variation->getName(),region->getName(),sample->GetName()));
            pl.setTagList("labels",labels);
            if(!pl.plotAndSaveAs(sample->GetName(),filename)){
              manager->error("cannot plot "+TQFolder::concatPaths(variation->getName(),region->GetName(),sample->GetName()));
              base->print("rdt");
            }
            base->writeToFile("test.root");
            for(auto var:{"Up","Down"}){
              TString baseName = exportTagKey + (TString)(var);
              double slope,slopeErr,chi2;
              if(pl.getTagDouble(baseName+".slope",slope) && pl.getTagDouble(baseName+".slopeError",slopeErr) && pl.getTagDouble(baseName+".chi2",chi2)){
                double sig = fabs(slope / slopeErr);
                if(sig > warnSlopeSignificanceLevel){
                  warnfiles.insert(filename);
                  TString warning = TString::Format("in plot '%s': slope exceeds level: %f > %f",TQFolder::concatPaths(variation->getName(),region->GetName(),sample->GetName()).Data(),sig,warnSlopeSignificanceLevel);
                  manager->warn(warning);
                  warnings.push_back(warning);
                }
                if(chi2 > warnChi2Level){
                  TString warning = TString::Format("in plot '%s': chi2 exceeds level: %f > %f",TQFolder::concatPaths(variation->getName(),region->GetName(),sample->GetName()).Data(),chi2,warnChi2Level);
                  warnfiles.insert(filename);
                  manager->warn(warning);
                  warnings.push_back(warning);
                }
              }
              pl.removeTag(baseName+".slope");
              pl.removeTag(baseName+".slopeError");
              pl.removeTag(baseName+".chi2");
            }
            removeHist(hup, "Up", base);
            removeHist(hdown, "Down", base);              
            removeHist(hnom, "Nominal", base);
            for (size_t i=0; i < otherModels.size(); i++) {
              removeHist(otherHups[i], otherModelNames[i], "Up", base);
              removeHist(otherHdowns[i], otherModelNames[i], "Down", base);
              removeHist(otherHnoms[i], otherModelNames[i], "Nominal", base);
            }
          }
          
          if (hupTotal && hdownTotal && hnomTotal) {
            //plot the sum of all processes (only for input histograms)
            addHist(hupTotal, "Up", base);
            addHist(hdownTotal, "Down", base);
            addHist(hnomTotal, "Nominal", base);

            TString filename = TQFolder::concatPaths(outputdir,variation->getName(),region->getName(),"total")+"."+format;
            pl.setTagString("labels.info",TQFolder::concatPaths(variation->getName(),region->getName(),"total"));
            if(!pl.plotAndSaveAs("total",filename)){
              manager->error("cannot plot "+TQFolder::concatPaths(variation->getName(),region->GetName(),"total"));
              base->print("rdt");
            }
            for(auto var:{"Up","Down"}){
              TString baseName = exportTagKey + (TString)(var);
              double slope,slopeErr,chi2;
              if(pl.getTagDouble(baseName+".slope",slope) && pl.getTagDouble(baseName+".slopeError",slopeErr) && pl.getTagDouble(baseName+".chi2",chi2)){
                double sig = fabs(slope / slopeErr);
                if(sig > warnSlopeSignificanceLevel){
                  warnfiles.insert(filename);
                  TString warning = TString::Format("in plot '%s': slope exceeds level: %f > %f",TQFolder::concatPaths(variation->getName(),region->GetName(),"total").Data(),sig,warnSlopeSignificanceLevel);
                  manager->warn(warning);
                  warnings.push_back(warning);
                }
                if(chi2 > warnChi2Level){
                  TString warning = TString::Format("in plot '%s': chi2 exceeds level: %f > %f",TQFolder::concatPaths(variation->getName(),region->GetName(),"total").Data(),chi2,warnChi2Level);
                  warnfiles.insert(filename);
                  manager->warn(warning);
                  warnings.push_back(warning);
                }
              }
              pl.removeTag(baseName+".slope");
              pl.removeTag(baseName+".slopeError");
              pl.removeTag(baseName+".chi2");
            }
            removeHist(hupTotal, "Up", base);
            removeHist(hdownTotal, "Down", base);
            removeHist(hnomTotal, "Nominal", base);
          }
            
          samples.reset();
        }
        regions.reset();
      }

      if(config->getTagBoolDefault("printSummary")){
        if(warnings.size() == 0){
          manager->info("no significant shapes were encountered!");
        } else {
          manager->info("the following warnings were encountered:");
          for(const auto& warning:warnings){
            manager->info(warning);
          }
        }
      }
      TString exportSummary;
      if(config->getTagString("exportSummary",exportSummary)){
        TString fname = TQFolder::concatPaths(outputdir,exportSummary);
        TQUtils::ensureDirectoryForFile(TQPathManager::getPathManager()->getTargetPath(fname));
        std::ofstream summary(TQPathManager::getPathManager()->getTargetPath(fname).c_str());
        for(auto plot:warnfiles){
          summary << plot << "\n";
        }
        summary.close();
      }
      return true;
    }

    
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new PlotSystematicsHistograms(),"PlotSystematicsHistograms");
  }
}
