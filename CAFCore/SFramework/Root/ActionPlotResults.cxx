#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQTable.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSStatisticsPlotter.h"

#include "TSystem.h"
#include "TROOT.h"

#include <fstream>
#include <algorithm>
#include <vector>
#include <list>

#include "QFramework/TQLibrary.h"

/*<cafdoc name=PlotResults>
  ActionPlotResults/ActionPlotResultsTikZ
  ===========================
  
  Plot the results of a fit, parameter by parameter.
  Several types of data sources are supported.
  * Pulls and constraints
  * Breakdowns
  * Impacts

  The `PlotResults` action uses the `ROOT` plotter as a graphics
  backend, whereas the `PlotResultsTikZ` variant produces a `tex` file
  with TikZ code for the graphic.
  
  Usage:
  ---------------
  ```
  +PlotResultsTikZ.asimov.full {
    +Morphing{
      +PlotFitParameter {
        <outputFile="workspaces/$(fitLabel)/pulls-full_AL.tex">
        +PULLS.asimov {
          <parameterSet = "LikelihoodScans/ScanLikelihood.2D/Scan/p.0/.fit/floatParsFinal">  
          <style.title = "$a_{L} = \hat{a_{L}}$ (Asimov)", style.color = kBlue>
        }
        +BREAKDOWN.asimov {
          <parameterSet = "Breakdowns/asimov/Breakdown/AL">
          <style.title = "Asimov breakdown", style.fillColor = kMagenta-7>
          <style.drawOption = 'E2'>
          <master=true>
        }
        +IMPACTS.asimov {
          <parameterSet = "Impacts/asimov/Impacts/AL">
          <style.title = "Asimov impacts", style.fillColor = kBlack,  style.lineColor = kBlack>
          <style.fillStyle=0>
          <style.drawOption = 'E5'>
        }
      }
    }
  }

  ```

  Datasets can be added using the `PULLS`, `BREAKDOWN` and `IMPACTS`
  subfolders. Several datasets of the same type can be added to one
  plot by suffixing them with identifiers like `.asimov` or
  `.observed`.

  Global options include
  * `writeTable` in addition to the plot, write a table to the given filename.
  * `writeJSON` in addition to the plot, write the data in JSON format to the given filename.


  For each folder, the source of the parameter sets needs to be
  identified using the `parameterSet` tag. Other than that, the
  following style options are supported:
  * `style.title` Change the title of the dataset.
  * `style.color` Change the line and fill color of the dataset.
  * `style.fillColor` Change the fill color of the dataset.
  * `style.fillStyle` Change the fill style of the dataset.
  * `style.lineColor` Change the line color of the dataset.
  * `style.lineStyle` Change the line style of the dataset.
  * `style.drawOption` Change the draw option of the dataset.

  Specific tags for the ranking (BREAKDOWN/IMPACT) displays include
  * `ranking.relativeToValue` normalize everything to the postfit POI value.
  * `ranking.relativeToUncertainty` normalize everything to the postfit POI uncertainty.

</cafdoc> */


namespace {
  int extractColorNumberFromTag(TQTaggable* config, const TString& tagName, int fallback/*TColor number*/) {
    if (!config) return fallback;
    int color = 0;
    TString hexColor = TQStringUtils::trim(config->getTagStringDefault(tagName+".rgb",""));
    if (hexColor.Length()>0) {
      hexColor = TQStringUtils::replace(hexColor,"0x","#"); //if someone uses the nicer notation for hex numbers...
      TQStringUtils::ensureLeadingText(hexColor,"#"); //for the lazy ones (not specifying any indication of hex numbers)
      color = TColor::GetColor(hexColor);
    } else { //no rgb color specified, so fall back to a regular TColor number
      color = config->getTagIntegerDefault(tagName,fallback);
      if (!gROOT->GetColor(color)) { //color is not known
        color = fallback;
      }
    }
    return color;
  }
  
  //just a lengthy definition...
  TString customTikzHatchPattern = "\n\
  % defining the new dimensions and parameters\n\
\\newlength{\\hatchspread}\n\
\\newlength{\\hatchthickness}\n\
\\newlength{\\hatchshift}\n\
\\newcommand{\\hatchcolor}{}\n\
% declaring the keys in tikz\n\
\\tikzset{hatchspread/.code={\\setlength{\\hatchspread}{#1}},\n\
         hatchthickness/.code={\\setlength{\\hatchthickness}{#1}},\n\
         hatchshift/.code={\\setlength{\\hatchshift}{#1}},% must be >= 0\n\
         hatchcolor/.code={\\renewcommand{\\hatchcolor}{#1}}}\n\
% setting the default values\n\
\\tikzset{hatchspread=3pt,\n\
         hatchthickness=0.4pt,\n\
         hatchshift=0pt,% must be >= 0\n\
         hatchcolor=black}\n\
% declaring the pattern\n\
\\pgfdeclarepatternformonly[\\hatchspread,\\hatchthickness,\\hatchshift,\\hatchcolor]% variables\n\
   {custom north west lines}% name\n\
   {\\pgfqpoint{\\dimexpr-2\\hatchthickness}{\\dimexpr-2\\hatchthickness}}% lower left corner\n\
   {\\pgfqpoint{\\dimexpr\\hatchspread+2\\hatchthickness}{\\dimexpr\\hatchspread+2\\hatchthickness}}% upper right corner\n\
   {\\pgfqpoint{\\dimexpr\\hatchspread}{\\dimexpr\\hatchspread}}% tile size\n\
   {% shape description\n\
    \\pgfsetlinewidth{\\hatchthickness}\n\
    \\pgfpathmoveto{\\pgfqpoint{0pt}{\\dimexpr\\hatchspread+\\hatchshift}}\n\
    \\pgfpathlineto{\\pgfqpoint{\\dimexpr\\hatchspread+0.15pt+\\hatchshift}{-0.15pt}}\n\
    \\ifdim \\hatchshift > 0pt\n\
      \\pgfpathmoveto{\\pgfqpoint{0pt}{\\hatchshift}}\n\
      \\pgfpathlineto{\\pgfqpoint{\\dimexpr0.15pt+\\hatchshift}{-0.15pt}}\n\
    \\fi\n\
    \\pgfsetstrokecolor{\\hatchcolor}\n\
    \\pgfusepath{stroke}\n\
   }\n\
  ";
  /*
  TString defineTikzHatchPattern(double distance, const TString& name_, TString& fullPatternName) {
    TString name = TQStringUtils::makeValidIdentifier(name_,TQStringUtils::getLowerLetters());
    fullPatternName = name+" north west lines";
    TString def =           
"\\pgfdeclarepatternformonly[\\LineSpace"+name+"]{"+name+" north west lines}{\\pgfqpoint{-1pt}{-1pt}}{\\pgfqpoint{\\LineSpace"+name+"}{\\LineSpace"+name+"}}{\\pgfqpoint{\\LineSpace"+name+"}{\\LineSpace"+name+"}}%\n" +
"{\n\n\
    \\pgfsetcolor{\\tikz@pattern@color} \n\n\
    \\pgfsetlinewidth{0.4pt}\n\n\
    \\pgfpathmoveto{\\pgfqpoint{0pt}{0pt}}\n\n\
    \\pgfpathlineto{\\pgfqpoint{\\LineSpace"+name+" + 0.1pt}{\\LineSpace"+name+" + 0.1pt}}\n\n\
    \\pgfusepath{stroke}\n\n\
}\n\n\
\\makeatother\n\n\
\\newdimen\\LineSpace"+name+"\n\n\
\\tikzset{\n\n\
    line space/.code={\\LineSpace"+name+"=#1},\n\n\
    line space="+std::to_string(distance)+"pt\n\n\
}";
  return def;
  }
  */
  
  
  
}

namespace TSBaseActions {
  
  class PlotResults : public TSStatisticsManager::Action {

    virtual TQFolder* setup(TQFolder* plotconfig,TQFolder* results) const {
      TQFolder* parSets = collectParameterSets(plotconfig,results);
			     
      std::vector<TString> parameter = makeParameterList(parSets,plotconfig);
      if(parameter.size() == 0) return NULL;

      TQFolder * output = TQFolder::newFolder("output");
     
      int overlayIndex = 0;
      TCollection* breakdowns = parSets->getListOfFolders("BREAKDOWN*,IMPACTS*,RANKING*");
      int nbreakdowns = 0;
      bool hasMaster = false;
      if(breakdowns){
        breakdowns->SetOwner(false);
        TQFolderIterator breakdownSets(breakdowns, true);
        double max = -1;
        std::vector<TQFolder*> subs;
        while (breakdownSets.hasNext()) {         
          TQFolder* set = breakdownSets.readNext();
          if(!set) continue;
          bool master = set->getTagBoolDefault("master",false);
          if(master && !hasMaster){
            makeParameterBreakdown(plotconfig,set,output,parameter,true,max);
            hasMaster = true;
            nbreakdowns = 1;
          } else {
            subs.push_back(set);
          }
        }
        for(auto set:subs){
          makeParameterBreakdown(plotconfig,set,output,parameter,!hasMaster,max);
          nbreakdowns ++;
          hasMaster = true;
        }
      }
      
      TCollection* pulls = parSets->getListOfFolders("PULLS*");

      if(!pulls && nbreakdowns==0){
        manager->error("unable to obtain PULLS or BREAKDOWN configuration!");
        return NULL;
      }
      if(pulls){
        pulls->SetOwner(false);
        int npulls = pulls->GetEntries();
        TQFolderIterator pullSets(pulls, true);
        double step = plotconfig->getTagDoubleDefault("style.stepWidth",0.2);
        while (pullSets.hasNext()) {
          TQFolder* set = pullSets.readNext();
          bool master = set->getTagBoolDefault("master",false);
          if(!set) continue;
          double pos = step*overlayIndex;
          pos -= 0.5*step*(npulls-1);
          makeParameterPulls(plotconfig,set,output,parameter,master,pos);
          overlayIndex++;
        }
      }
      
      makeLabels(plotconfig,output,parameter);

      //      delete parSets;
      return output;
    }
    
    bool execute(TQFolder * config) const override {
      TQFolder * result = results()->getFolder(config->GetName());
      if (!config) {
        return 0;
      }
      Int_t nResults = 0;
      if (result) {
        TQFolder* plotconfig = config->getFolder("PlotFitParameter");
        if(plotconfig){
          TQFolder* output = setup(plotconfig,result);
          if(output){ nResults++;
            bool ok = makePlot(plotconfig,output);
            nResults += ok;
            TString jsonoutname;
            if(plotconfig->getTagString("writeJSON",jsonoutname)){
              TQUtils::ensureDirectoryForFile(TQPathManager::getPathManager()->getTargetPath(jsonoutname).c_str());
              writeJSON(output,TQPathManager::getPathManager()->getTargetPath(jsonoutname).c_str());
            }
            TString tableoutname;
            if(plotconfig->getTagString("writeTable",tableoutname)){
              TQTable* tab = makeTable(config,output);
              if(tab){
		for (int i = 0; i < tab->getNcols(); i++) { tab->setColAlign(i, plotconfig->getTagStringDefault("colAlign", "l")); }
                //@tag[writeTableFormats] list of formats in which the table should be written. For possible options see TQTable documentation
                std::vector<TString> formats = plotconfig->getTagVString("writeTableFormats");
                if(formats.size()==0) formats.push_back("tex");
                TQTaggable tags("standalone=true");
                for(auto fmt:formats){
                  tab->write(TQPathManager::getPathManager()->getTargetPath(tableoutname)+"."+fmt,fmt,tags);
                }
                delete tab;
              } else {
                manager->warn("failed to create table!");
              }
            }
          } else {
            manager->error(TString::Format("unable to setup  plot %s, skipping",plotconfig->GetName()));;
          }
          delete output;
        }
      }
      if (nResults == 0) {
        manager->warn(TString::Format("%s: No results have been plotted", config->GetName()));
        return false;
      }
      return true;
    }
    
    virtual TQFolder* collectParameterSets(TQFolder* config, TQFolder* results, const TString& expression = "?") const{
      // the folder containing all parameter sets to be plotted onto one plot
      TQFolder * parSets = TQFolder::newFolder("parSets");
      
      TQIterator itr(config->getListOfFolders(expression), true);
      while (itr.hasNext()) {
        TQFolder * group = (TQFolder*)itr.readNext();
        TString name = group->GetName();
    
        TQTaggable tags(group);
        TString parSetName;
        if (!tags.getTagString("parameterSet", parSetName)) {
          manager->warn(TString::Format("Missing parameter set for plotting of group '%s'. Skipping ...", name.Data()));
          continue;
        }

        TString uncondPath;
        TQFolder* uncond = NULL;
        if(tags.getTagString("unconditional",uncondPath)){
          uncond = results->getFolder(uncondPath);
        }

        TQFolder * parSet = results->getFolder(parSetName);
        if (!parSet) {
          manager->warn(TString::Format(
                                        "Failed to find parameter set '%s' for plotting of group '%s' in result '%s'. Skipping ...",
                                        parSetName.Data(), name.Data(), results->getPath().Data()));
          continue;
        }

        if(parSet->isEmpty()){
          manager->error("found empty parameter set!");
        } else {
          TQFolder * copyOfParSet = parSet->copy(name);
          copyOfParSet->importTags(tags);
          parSets->addObject(copyOfParSet);
          if(uncond) copyOfParSet->importTags(uncond);
        }
      }

      return parSets;
    };

    virtual void makeLabels(TQFolder* config, TQFolder* output, const std::vector<TString>&  parameter) const {
      // make the labels

      std::vector<TString> stripPrefixes = config->getTagVString("stripPrefixes");
      std::vector<TString> stripSuffixes = config->getTagVString("stripSuffixes");

      TQFolder* textReplacements = TQFolder::newFolder("textReplacements");
      if (config->hasTag("textReplacements")) textReplacements->importFromTextFile(TQPathManager::getPathManager()->findConfigPath(config->getTagStringDefault("textReplacements","")).c_str());
      
      bool vertical = config->getTagBoolDefault("style.vertical",false);
      double textSize = config->getTagDoubleDefault("style.textSize",0.025);
      double shift = config->getTagDoubleDefault("style.yshift",0.4);

      int boxColor;
      bool showBoxes = config->getTagInteger("style.highlight",boxColor);
      double labelOffset = config->getTagDoubleDefault("style.labelOffset",1.5);
      Int_t i = -1;
      for(auto name:parameter){
        TQFolder* f = textReplacements->getFolder(name);
        if (f && f->getTagBoolDefault("skipParameter",false)) continue;
        i++;
        TQFolder* text = output->getFolder(TString::Format("Text.p.i%d+", i));
        if(vertical){
          text->setTagDouble("y", -shift+i);
          text->setTagDouble("x", labelOffset);
        } else {
          text->setTagDouble("x", i);
          text->setTagDouble("y", labelOffset);
        }
        if(showBoxes && i%2){
          TQFolder* box = output->getFolder(TString::Format("Box.p.i%d+", i));
          if(vertical){
            box->setTagDouble("y1", i-0.5);
            box->setTagDouble("y2", i+0.5);
          } else {
            box->setTagDouble("x1", i-0.5);
            box->setTagDouble("x2", i+0.5);
          }
          box->setTagInteger("style.color", config->getTagIntegerDefault("style.highlight", 16));
          box->setTagString("level","back");
        }
    	
        TString t(f ? f->getTagStringDefault("title",name) : name);
        for(auto s:stripPrefixes){
          TQStringUtils::removeLeadingText(t,s);
        }
        for(auto s:stripSuffixes){
          TQStringUtils::removeTrailingText(t,s);
        }
        text->setTagString("text", t);
        text->setTagDouble("style.textSize", textSize);
        
        text->setTagInteger("style.textAngle", vertical ? 0 : 60);
      }
      delete textReplacements;
    }

    std::vector<TString> makeParameterList(TQFolder* input, TQFolder* config) const {
      // collect list of parameters present in at least one set
      TQTaggable par;
      TQFolderIterator itrPar(input->getListOfFolders("?/?"), true);
      while (itrPar.hasNext()) {
        par.setTagBool(itrPar.readNext()->GetName(), true);
      }
      
      // organize the list of parameters by adding those from the list created
      // in the previous step that pass the parameter filter <parFilter>
      std::vector<TString> parFilter = config->getTagVString("parameterFilter");
      if(parFilter.size() == 0){
        parFilter.push_back("*");
      }
      std::vector<TString> blacklist = config->getTagVString("blacklist");
      if ( !config->getTagBoolDefault("showIndividualStatUncertainties",false) ) {
        blacklist.push_back("*gamma_stat*");
      }
      blacklist.push_back(".cond.*");
      blacklist.push_back(".uncond");
      std::vector<TString> whitelist = config->getTagVString("whitelist");

      std::vector<TString> result;
      for(auto item:parFilter){
        TCollection* c = par.getListOfKeys(item);
        if(!c) continue;
        TQIterator itr(c,true);
        c->SetOwner(true);
        while(itr.hasNext()){
          TObject* p = itr.readNext();
          bool remove = false;
          for(auto item:blacklist){
            if(TQStringUtils::matches(p->GetName(),item)){
              remove = true;
            }
          }
          for(auto item:whitelist){
            if(TQStringUtils::matches(p->GetName(),item)){
              remove = false;
            }
          }
          if(!remove){
            result.push_back(p->GetName());
          }
        }
      }

      if(result.size() == 0){
        manager->error(TString::Format("obtained empty parameter list from '%s'\n  whitelist=%s\n  blacklist=%s\n  filter=%s",
                                       input->getPath().Data(),
                                       TQStringUtils::concat(whitelist).Data(),
                                       TQStringUtils::concat(blacklist).Data(),
                                       TQStringUtils::concat(parFilter).Data()));
      }
			
      return result;
    }
    

    virtual bool makeParameterBreakdown(TQFolder * config, TQFolder * set, TQFolder* output, std::vector<TString>& parameter, bool master, double& max) const{
      if(parameter.size()==0){
        manager->error("no parameter set given!");
        return false;
      }
      TQFolder* overlay = output->getFolder(TString::Format("Overlay.Ranking.%s+", set->GetName()));
      TQFolder* axis = NULL;
      if(master) axis = output->getFolder(TString::Format("Axis.%s+", set->GetName()));

      bool vertical = config->getTagBoolDefault("style.vertical",false);
      TString setName = set->GetName();

      overlay->importTags(set);

      /* loop over parameter */
      Int_t i = -1;
      bool relativeToValue = config->getTagBoolDefault("ranking.relativeToValue",false);
      bool relativeToUncertainty = config->getTagBoolDefault("ranking.relativeToUncertainty",false);      
      if(relativeToValue && relativeToUncertainty){
        manager->error("the options ranking.relativeToValue and ranking.relativeToUncertainty are mutually exclusive!");
        return false;
      }
      
      std::vector<TQFolder*> folders;
      bool findMax = (max<0);
      for(auto name:parameter){
        TQFolder * var = set->getFolder(name);
        if (!var) {
          manager->warn(TString::Format("Parameter '%s' does not exist in set '%s'", name.Data(), setName.Data()));
          folders.push_back(NULL);
        } else if(!var->getTagBoolDefault("IsOk",true)){
          manager->warn(TString::Format("Parameter '%s' from set '%s' is not OK, skipping", name.Data(), setName.Data()));          
          folders.push_back(NULL);
        } else {
          folders.push_back(var);
          if(findMax){
            Double_t high,low;
            var->getTagDouble("High", high);
            var->getTagDouble("Low", low);
            max = std::max(max,std::max(fabs(high),fabs(low)));
          }
        }
      }
      if(relativeToValue) max /= fabs(set->getTagDoubleDefault("val",0.));
      if(relativeToUncertainty) max /= std::max(fabs(set->getTagDoubleDefault("errHigh",0.)),fabs(set->getTagDoubleDefault("errLow",0.)));
      
      if(master){
        if(config->getTagBoolDefault("sort",master)){
          if(relativeToValue){
            std::sort(folders.begin(), folders.end(), 
                      [](TQFolder* a,TQFolder* b) -> bool
                      { 
                        return (a && b && (fabs(a->getTagDoubleDefault( "Avg_Rel" ,0.)) > fabs(b->getTagDoubleDefault("Avg_Rel",0.)))) || (a && !b); 
                      }
                      );
          } else {
            std::sort(folders.begin(), folders.end(),             
                      [](TQFolder* a,TQFolder* b) -> bool
                      { 
                        return (a && b && (fabs(a->getTagDoubleDefault( "Avg" ,0.)) > fabs(b->getTagDoubleDefault("Avg",0.)))) || (a && !b); 
                      });
          }
        }
        parameter.clear();
        std::vector<TQFolder*> tmp;
        int nMax = std::min((int)(folders.size()),config->getTagIntegerDefault("nMax",folders.size()));
        for(size_t i = nMax; i>0; --i){
          TQFolder* f = folders[i-1];
          if(!f){
            // if some parameter is missing from the master list, we just skip it - it's not included in the plot then
            continue;
          } else {
            parameter.push_back(f->GetName());
            tmp.push_back(f);
          }            
        }
        folders = tmp;
      }

      manager->info(TString::Format("plotting parameter breakdown '%s' as %s",set->GetName(),master ? "master" : "slave"));

      for(auto var:folders){
        i++;
        if (!var) {
          continue;
        }        
        TQFolder * par = overlay->getFolder(TString::Format("p.i%d+", i));
        double high = 0;
        double low = 0;
        if(relativeToValue){
          overlay->setTagBool(".relativeToValue", true);
          if(!var->getTagDouble("High_Rel",high)){ var->getTagDouble("High",high); high /= fabs(set->getTagDoubleDefault("val",0.)); par->setTagBool(".fallback", true); };
          if(!var->getTagDouble("Low_Rel",low)){ var->getTagDouble("Low",low); low /= fabs(set->getTagDoubleDefault("val",0.)); par->setTagBool(".fallback", true);};
        } else if(relativeToUncertainty){
          overlay->setTagBool(".relativeToUncertainty", true);
          if(!var->getTagDouble("High_RelUnc",high)){ var->getTagDouble("High",high); high /= fabs(set->getTagDoubleDefault("errHigh",0.)); par->setTagBool(".fallback", true); };
          if(!var->getTagDouble("Low_RelUnc",low)){ var->getTagDouble("Low",low); low /= fabs(set->getTagDoubleDefault("errLow",0.)); par->setTagBool(".fallback", true);};            
        } else {
          var->getTagDouble("High",high);
          var->getTagDouble("Low",low);
        }
        
        par->setTagDouble(".index", i);
        par->setTagDouble(".low", low);
        par->setTagDouble(".high", high);
        if(vertical){
          par->setTagDouble("y", i);
          par->setTagDouble("x", 0);
          par->setTagDouble("yp", 0.5+i);
          par->setTagDouble("yn", -0.5+i);
          par->setTagDouble("xn", low /max);
          par->setTagDouble("xp", high/max);
        } else {
          par->setTagDouble("x", i);
          par->setTagDouble("xp", 0.5+i);
          par->setTagDouble("xn", -0.5+i);
          par->setTagDouble("y", 0);
          par->setTagDouble("yn", low /max);
          par->setTagDouble("yp", high/max);
        }
      }
      overlay->setTagString("template", "ranking");

      if(axis){
        if(vertical){
          output->setTagDouble("style.yMaxMin",(0.015*i)+i+2);
          axis->setTagDouble("x1",-1.1);
          axis->setTagDouble("x2", 1.1);
          axis->setTagDouble("y",(0.015*i)+i+1);
          axis->setTagString("style.drawOption","<->");
          axis->setTagDouble("style.labelSize", set->getTagDoubleDefault("style.labelSize", 0.025));
          axis->setTagDouble("style.textSize", set->getTagDoubleDefault("style.textSize", 0.025));
          axis->setTagDouble("style.titleOffset", set->getTagDoubleDefault("style.titleOffsetSize", 1));
          axis->setTagInteger("ndiv", set->getTagIntegerDefault("ndiv", 510));
          axis->setTagInteger("style.lineColor", set->getTagIntegerDefault("style.lineColor", 1));
          axis->setTagDouble("style.lineWidth", set->getTagDoubleDefault("style.lineWidth", 2.));
        }
        axis->setTagString("style.title",config->getTagStringDefault("ranking.axis",relativeToValue ? "#Delta#mu/#mu" : "#Delta#mu"));
        axis->setTagDouble("wmin",-max);
        axis->setTagDouble("wmax",+max);
      }
      return i;
    }

    bool makeParameterPulls(TQFolder * config, TQFolder * set, TQFolder* output, std::vector<TString>& parameter, bool master, double shift) const{
      TQFolder* overlay = output->getFolder(TString::Format("Overlay.Pulls.%s+", set->GetName()));
      bool vertical = config->getTagBoolDefault("style.vertical",false);
      TString setName = set->GetName();
      overlay->importTags(set);

      if(master){
        std::sort(parameter.begin(), parameter.end(), 
                  [set](const TString& aname, const TString& bname) -> bool
                  {
                    TQFolder* a = set->getFolder(aname);
                    TQFolder* b = set->getFolder(bname);
                    if(!a || !b) return false;
                    double aerrsym = a->getTagDoubleDefault( "err" ,0.);
                    double berrsym = b->getTagDoubleDefault( "err" ,0.);
                    double aerr = 0.5*(fabs(a->getTagDoubleDefault( "errLow" ,aerrsym))+fabs(a->getTagDoubleDefault( "errHigh" ,aerrsym)));
                    double berr = 0.5*(fabs(b->getTagDoubleDefault( "errLow" ,berrsym))+fabs(b->getTagDoubleDefault( "errHigh" ,berrsym)));                    
                    return aerr > berr;
                  }
                  );
      }
        

      double constraintThreshold = config->getTagDoubleDefault("constraintThreshold",-500);
        
      /* loop over parameter */
      Int_t i = -1;
      for(auto name:parameter){
        i++;
        /* load parameter */
        TQFolder * var = set->getFolder(name);
        if (!var) {
          manager->warn(TString::Format("Parameter '%s' does not exist in set '%s'", name.Data(), setName.Data()));
          continue;
        }

        Double_t val;
        Double_t err;
        // parameter central value
        var->getTagDouble("val", val);
        // parameter symmetric uncertainty
        var->getTagDouble("err", err);
    			
        // parameter asymmetric uncertainties [please note: uncertainties will
        // be either symmetric (one value) or asymmetric (two values). Here, the
        // symmetric value is used as default for the asymmetric values]
        Double_t errHigh	= err;
        Double_t errLow		= -err;
        var->getTagDouble("errHigh", errHigh);
        var->getTagDouble("errLow", errLow);

        double constrain = 1.-(fabs(errHigh-errLow)/2);
        if(100*constrain < constraintThreshold) continue;
        
        TQFolder * par = overlay->getFolder(TString::Format("p.i%d+", i));
        par->setTagDouble(".index", i);
        par->setTagDouble(".val", val);
        par->setTagDouble(".errLow", errLow);
        par->setTagDouble(".errHigh", errHigh);
        if(vertical){
          par->setTagDouble("y", shift + i);
          par->setTagDouble("x", val);
          par->setTagDouble("xn", val - fabs(errLow));
          par->setTagDouble("xp", val + fabs(errHigh));
        } else {
          par->setTagDouble("x", shift + i);
          par->setTagDouble("y", val);
          par->setTagDouble("yn", val - fabs(errLow));
          par->setTagDouble("yp", val + fabs(errHigh));
        }
      }
      overlay->setTagString("template", "nuis");

      return i;
    }


    virtual void writeJSON(TQFolder* output, const TString& ofname)const{
      if(!output){
        throw std::runtime_error("no data given!");
      }
      
      TCollection* pars = output->getListOfFolders("Text.p.*");
      if(!pars || pars->GetEntries()<1){
        manager->error("parameter list is empty!");
      }
      //const int npar = pars->GetEntries();
      
      std::ofstream out(TQPathManager::getPathManager()->getTargetPath(ofname).c_str());
      
      TQFolderIterator pnames(output->getListOfFolders("Text.p.*"),true);
      std::vector<TString> pnamelist;
      while(pnames.hasNext()){
        TQFolder* line = pnames.readNext();
        TString text  = line->getTagStringDefault("text");
        pnamelist.push_back(text);
      }
      
      out << "ranking_plots = {\n";
      TQFolderIterator rankings(output->getListOfFolders("Overlay.Ranking.*"),true);
      //int nrankings = 0;
      // rankings
      while(rankings.hasNext()){
        TQFolder* ranking = rankings.readNext();
        TString name(ranking->GetName());
        TQStringUtils::removeLeading(name,"Overlay.Ranking.");
        out << "  " << TQStringUtils::quote(name) << " : {\n";
        TString title;
        out << "    ";
        if(ranking->getTagString("style.title",title)){
          out << TQStringUtils::quote("title") << " : " << TQStringUtils::quote(title) << ", ";
        }
        out << TQStringUtils::quote("data") << " : {\n";
        TQFolderIterator lines(ranking->getListOfFolders("p.*"),true);
        while(lines.hasNext()){
          TQFolder* line = lines.readNext();
          int index = line->getTagIntegerDefault(".index",-1);
          double xn = line->getTagDoubleDefault(".low");
          double xp = line->getTagDoubleDefault(".high");
          out << "      " << TQStringUtils::quote(pnamelist[index]) << " : { "
              << TQStringUtils::quote("low") << " : " << xn << ", "
              << TQStringUtils::quote("high") << " : " << xp << " },\n";
        }
        out << "    }\n  }\n";
      }
      out << "}\n";
      // pulls
      out << "pull_plots = {\n";
      TQFolderIterator pulls(output->getListOfFolders("Overlay.Pulls.*"),true);
      while(pulls.hasNext()){
        TQFolder* pull = pulls.readNext();
        TString name(pull->GetName());
        TQStringUtils::removeLeading(name,"Overlay.Pulls.");
        out << "  " << TQStringUtils::quote(name) << " : {\n";
        // points and lines
        TString title;
        out << "    ";
        if(pull->getTagString("style.title",title)){
          out << TQStringUtils::quote("title") << " : " << TQStringUtils::quote(title) << ", ";
        }
        out << TQStringUtils::quote("data") << " : {\n";
        TQFolderIterator lines(pull->getListOfFolders("p.*"),true);
        while(lines.hasNext()){
          TQFolder* line = lines.readNext();
          double xn = line->getTagDoubleDefault(".errLow");
          double xp = line->getTagDoubleDefault(".errHigh");
          double x = line->getTagDoubleDefault(".val");
          //double constrain = fabs(xn-xp)/2;
          int index = line->getTagIntegerDefault(".index",-1);
          out << "      " << TQStringUtils::quote(pnamelist[index]) << " : { "
              << TQStringUtils::quote("central") << " : " << x << ", "
              << TQStringUtils::quote("low") << " : " << xn << ", "
              << TQStringUtils::quote("high") << " : " << xp << " },\n";
        }
        out << "    }\n  },\n";
      }
      out << "}\n";
      out.close();
    }
    
    virtual TQTable* makeTable(TQFolder*/*config*/, TQFolder* output)const{
      if(!output){
        throw std::runtime_error("no data given!");
      }
      
      TCollection* pars = output->getListOfFolders("Text.p.*");
      if(!pars || pars->GetEntries()<1){
        manager->error("parameter list is empty!");
        return NULL;
      }
      const int npar = pars->GetEntries();
			
      TQTable* tab = new TQTable("summary");
      tab->setEntry(0,0,"Contribution");
      tab->expand(npar+2,5);
      
      TQFolderIterator pnames(output->getListOfFolders("Text.p.*"),true);
      std::list<TQFolder*> pnamelist;
      while(pnames.hasNext()){
        TQFolder* line = pnames.readNext();
        pnamelist.push_front(line);
      }
			
      size_t ipar=2;
      for(auto line:pnamelist){
        TString text  = line->getTagStringDefault("text");
        tab->setEntry(ipar,0,text);
        ipar++;
      }

      TQFolderIterator breakdowns(output->getListOfFolders("Overlay.Ranking.*"),true);
      int nbreakdowns = 0;
      // breakdowns
      while(breakdowns.hasNext()){
        TQFolder* breakdown = breakdowns.readNext();
        nbreakdowns++;
        TString title;
        TString POIname = breakdown->getTagStringDefault("style.POI", "POI");
        if (breakdown->getTagBoolDefault(".relativeToValue", false)){
          tab->setEntry(0,nbreakdowns,TString::Format("Avg. contribution (%% of %s)", POIname.Data()));
        }
        else if (breakdown->getTagBoolDefault(".relativeToUncertainty", false)){
          tab->setEntry(0,nbreakdowns,TString::Format("Avg. contribution (%% of %s unc.)", POIname.Data()));
        }
        else{
          tab->setEntry(0,nbreakdowns,TString::Format("Avg. contribution (abs. to %s)", POIname.Data()));
        }
        if(breakdown->getTagString("style.title",title)){
          tab->setEntry(1,nbreakdowns,title);
        }
        TQFolderIterator lines(breakdown->getListOfFolders("p.*"),true);
        std::list<TQFolder*> linelist;
        while(lines.hasNext()){
          TQFolder* line = lines.readNext();
          linelist.push_front(line);
        }
        ipar=2;
        for(auto line:linelist){
          double xn = line->getTagDoubleDefault(".low");
          double xp = line->getTagDoubleDefault(".high");
          tab->setEntryValue(ipar,nbreakdowns,100.*0.5*(fabs(xn)+fabs(xp)));
          if (line->getTagBoolDefault(".fallback", false)){
            tab->setProperty(ipar,nbreakdowns,"italic", true);
          }
          ipar++;
        }
      }
      // pulls
      int nPulls = 0;
      TQFolderIterator pulls(output->getListOfFolders("Overlay.Pulls.*"),true);
      while(pulls.hasNext()){
        TQFolder* pull = pulls.readNext();
        nPulls++;
        // points and lines
        TString title;
        tab->setEntry(0,nPulls+nbreakdowns,"Constraint in \\%");
        if(pull->getTagString("style.title",title)){
          tab->setEntry(1,nPulls+nbreakdowns,title);
        }
        TQFolderIterator lines(pull->getListOfFolders("p.*"),true);
        std::list<TQFolder*> linelist;
        while(lines.hasNext()){
          TQFolder* line = lines.readNext();
          linelist.push_front(line);
        }
        for(auto line:linelist){
          TString name(line->GetName());
          TQStringUtils::removeLeadingText(name,"p.i");
          int idx = atof(name.Data());
          double xn = line->getTagDoubleDefault(".errLow");
          double xp = line->getTagDoubleDefault(".errHigh");
          //double x = line->getTagDoubleDefault(".val");
          double constrain = 1.-(fabs(xn-xp)/2);
          tab->setEntryValue(1+npar-idx,nPulls+nbreakdowns,100*constrain);
        }
        nPulls++;
      }

      // Consistency of pull with constraint
      int nPullSigs = 0;
      TQFolderIterator pullSigs(output->getListOfFolders("Overlay.Pulls.*"),true);
      while(pullSigs.hasNext()){
        TQFolder* pull = pullSigs.readNext();
        nPullSigs++;
        // points and lines
        TString title;
        tab->setEntry(0,nPulls+nbreakdowns+nPullSigs,"Pull significance");
        if(pull->getTagString("style.title",title)){
          tab->setEntry(1,nPulls+nbreakdowns+nPullSigs,title);
        }
        TQFolderIterator lines(pull->getListOfFolders("p.*"),true);
        std::list<TQFolder*> linelist;
        while(lines.hasNext()){
          TQFolder* line = lines.readNext();
          linelist.push_front(line);
        }
        for(auto line:linelist){
          TString name(line->GetName());
          TQStringUtils::removeLeadingText(name,"p.i");
          int idx = atof(name.Data());
          double xn = line->getTagDoubleDefault(".errLow");
          double xp = line->getTagDoubleDefault(".errHigh");
          double x = line->getTagDoubleDefault(".val");
          double pullSideErr=xp;
          if(x<0) pullSideErr=xn;
          // The pull significance is x/(sqrt(1-pullSideErr^2)), where pullSideErr is the high/low err if x is pos/neg.
          // If the pull side error is close to 1 the method is not accurate, and eventually the sqrt cannot be evaluated
          // Assume a post-fit error of 0.999, for which the pullSig is 1 if the pull is 0.014 (very small)
          if (abs(pullSideErr)>0.999) pullSideErr=0.999;
          double pullSig = x/(sqrt(1-pullSideErr*pullSideErr));
          tab->setEntryValue(1+npar-idx,nPulls+nbreakdowns+nPullSigs,pullSig);
        }
        nPullSigs++;
      }

      for(int i=0; i<tab->getNrows(); ++i){
        bool keep = false;
        for(int j=1; j<tab->getNcols(); ++j){
          if(tab->hasEntry(i,j)) keep=true;
        }
        if(!keep) tab->removeEntry(i,0);
      }
      tab->shrink();
      return tab;
    }
    
    virtual bool makePlot(TQFolder* config, TQFolder* output) const {
      TSStatisticsPlotter pl;
      TString tqpath = TQLibrary::getTQPATH();
      // set plot style templates
      TQFolder * templates = TQFolder::loadFromTextFile(tqpath+"/../SFramework/share/templates/OverlayTemplates.txt");
      pl.setTemplates(templates);
      bool vertical = config->getTagBoolDefault("style.vertical",false);
      
      // load some basic plot elements (e.g. lines representing 0, and +/- 1)
      TString path = config->getTagStringDefault("arrangement",
                                                 tqpath+"/../SFramework/share/templates/PlotNuisPull_"+(vertical ? "vertical" : "horizontal")+".txt");
      config->exportTags(output,"","style.*");
      config->exportTags(output,"","labels.*");
      gSystem->ExpandPathName(path);
      output->setGlobalOverwrite(false);
      output->importFromTextFile(TQPathManager::getPathManager()->getTargetPath(path).c_str());
      
      if(config->getTagBoolDefault("ATLAS",false)){
        output->setTagBool("labels.drawATLAS",true);
        TString label;
        if(config->getTagString("ATLASlabel",label)){
          output->setTagString("labels.atlas",label);
        }
      }
      
      TCanvas * c = pl.plot(output);

      delete templates;
      if (c) {
        TString outputfile = config->getTagStringDefault("outputFile", "plot.pdf");
        TQUtils::ensureDirectoryForFile(TQPathManager::getPathManager()->getTargetPath(outputfile).c_str());
        c->SaveAs(TQPathManager::getPathManager()->getTargetPath(outputfile).c_str());
        delete c;
        return true;
      }

      return false;
    }
  };  
    
    
  namespace {
    bool available = TSStatisticsManager::registerAction(new PlotResults(),"PlotResults");
  }

  class PlotResultsTikZ : public TSBaseActions::PlotResults {
    virtual bool makePlot(TQFolder* config, TQFolder* output)const{
      if(!config){
        throw std::runtime_error("no config given!");
      }
      if(!output){
        throw std::runtime_error("no output given!");
      }
      TString outputfile;
      if(!config->getTagString("outputFile",outputfile)){
        manager->error("no output filename set!");
        return false;
      }
      if(TQStringUtils::removeTrailingText(outputfile,".pdf") > 0){
        manager->warn("renaming output file from '.pdf' to '.tex'");
        outputfile.Append(".tex");
      }
      TQUtils::ensureDirectoryForFile(TQPathManager::getPathManager()->getTargetPath(outputfile).c_str());
      double textscale = config->getTagDoubleDefault("tikz.textScale",1.0);
      bool vertical = config->getTagBoolDefault("style.vertical",false);      
      std::stringstream tex;
      std::stringstream defs;
      std::stringstream legend;
      TCollection* pars = output->getListOfFolders("Text.p.*");
      if(!pars || pars->GetEntries()<1){
        manager->error("parameter list is empty!");
        return false;
      }
      const int npar = pars->GetEntries();
      delete pars;
      tex << "\\pgfdeclarelayer{background}\\pgfsetlayers{background,main}\n";
      TQFolderIterator pnames(output->getListOfFolders("Text.p.*"),true);
      std::list<TQFolder*> pnamelist;
      while(pnames.hasNext()){
        TQFolder* line = pnames.readNext();
        pnamelist.push_front(line);
      }
      for(auto line:pnamelist){
        double x = line->getTagDoubleDefault("x");
        double y = line->getTagDoubleDefault("y");
        TString text  = line->getTagStringDefault("text");
        tex << "\\node[scale="<<textscale;
        if(vertical) tex << ",anchor=west";
        else         tex << ",anchor=east";
        if(line->hasTagDouble("style.textAngle")) tex << ",rotate=" << line->getTagDoubleDefault("style.textAngle",0);
        tex << "] at (";
        if(vertical) tex << x << "," << y-npar;
        else         tex << x+1 << "," << -2;
        tex << ") {";
        if(text.Contains("{") && !text.Contains("$")){
          tex << "\\ensuremath{" << text << "}";
        } else {
          text.ReplaceAll("_","\\_");
          tex << text;
        }
        tex << "};\n";
      }
      TQFolderIterator rankings(output->getListOfFolders("Overlay.Ranking.*"),true);
      int ilegend = 0;
      int nrankings = 0;
      //add definition for custom hatching patterns (e.g. with support for custom densities)
      defs << customTikzHatchPattern <<"\n";
      const TString hatchPatternName = "custom north west lines";
      // rankings
      std::stringstream breakdowntext;
      while(rankings.hasNext()){
        TQFolder* ranking = rankings.readNext();              
        int fillstyle = ranking->getTagIntegerDefault("style.fillStyle",1);
        double fillOpacity = ranking->getTagDoubleDefault("style.fillOpacity",1.0);
        TString styleName = TQFolder::makeValidIdentifier(ranking->GetName());
        TString colorNamePos = "col"+styleName+"Pos";
        TString colorNameNeg = "col"+styleName+"Neg";     
        TString colorNameHatchPos = "col"+styleName+"HatchPos";
        TString colorNameHatchNeg = "col"+styleName+"HatchNeg";
        
        //extractColorNumberFromTag(TQTaggable* config, const TString& tagName, int fallback/*TColor number*/)
        int color = extractColorNumberFromTag(ranking, "style.fillColor", kOrange-2);
        //ranking->getTagIntegerDefault("style.fillColor",kOrange-2);
        int poscolor = extractColorNumberFromTag(ranking, "style.fillColor.pos", color);
        //ranking->getTagIntegerDefault("style.fillColor.pos",color);
        int negcolor = extractColorNumberFromTag(ranking, "style.fillColor.neg", color);
        //ranking->getTagIntegerDefault("style.fillColor.neg",color);     
        defs << TQStringUtils::getColorDefStringLaTeX(colorNamePos,poscolor)<<"\n";
        defs << TQStringUtils::getColorDefStringLaTeX(colorNameNeg,negcolor)<<"\n"; 
        
        bool hatchDefault = ranking->getTagBoolDefault("style.hatch",false);
        int hatchSpread = ranking->getTagIntegerDefault("style.hatchSpread",-1);
        //hatching/pattern color
        int posHatchColor = extractColorNumberFromTag(ranking, "style.hatchColor.pos", poscolor);
        int negHatchColor = extractColorNumberFromTag(ranking, "style.hatchColor.neg", negcolor);
        int posHatchSpread = ranking->getTagIntegerDefault("style.hatchSpread.pos",hatchSpread);
        int negHatchSpread = ranking->getTagIntegerDefault("style.hatchSpread.neg",hatchSpread);
        defs << TQStringUtils::getColorDefStringLaTeX(colorNameHatchPos,posHatchColor)<<"\n";
        defs << TQStringUtils::getColorDefStringLaTeX(colorNameHatchNeg,negHatchColor)<<"\n";
        
        /*
        TString hatchPatternName = "";
        if (ranking->getTagBoolDefault("style.hatch.pos",hatchDefault) || ranking->getTagBoolDefault("style.hatch.neg",hatchDefault)) {
          double hatchDistance = ranking->getTagDoubleDefault("style.hatchDistance",10.); //distance between lines in pt
          defs << defineTikzHatchPattern(hatchDistance, styleName, hatchPatternName) << "\n";
        }
        */
        
        defs << "\\tikzset{"<<styleName<<"Pos" << "/.style={";
        if(fillstyle==1){
          defs << "draw=none,fill=" << colorNamePos << ",text opacity = 1.0, fill opacity=" << fillOpacity << ",";
        } else if (fillstyle == 0){
          defs << "draw=" << colorNamePos << ",";
        }
        if (ranking->getTagBoolDefault("style.hatch.pos",hatchDefault)) {
          //defs << "pattern=" << hatchPatternName << ", pattern color=" << colorNameHatchPos << ",";
          defs << "pattern=" << hatchPatternName << ", hatchcolor=" << colorNameHatchPos << ",";
          if (posHatchSpread>0) defs <<"hatchspread=" << posHatchSpread << "pt,";
        }
        defs << "}}\n";
        
        defs << "\\tikzset{"<<styleName<<"Neg" << "/.style={";
        if(fillstyle==1){
          defs << "draw=none,fill=" << colorNameNeg << ",text opacity = 1.0, fill opacity=" << fillOpacity << ",";
        } else if (fillstyle == 0){
          defs << "draw=" << colorNameNeg << ",";
        }
        if (ranking->getTagBoolDefault("style.hatch.neg",hatchDefault)) {
          //defs << "pattern=" << hatchPatternName << ", pattern color=" << colorNameHatchNeg << ",";
          defs << "pattern=" << hatchPatternName << ", hatchcolor=" << colorNameHatchNeg << ",";
          if (negHatchSpread>0) defs <<"hatchspread=" << negHatchSpread << "pt,";
        }
        defs << "}}\n";
        // boxes
        TQFolderIterator lines(ranking->getListOfFolders("p.*"),true);
        std::list<TQFolder*> linelist;
        while(lines.hasNext()){
          TQFolder* line = lines.readNext();
          linelist.push_front(line);
        }
        for(auto line:linelist){
          double yn = line->getTagDoubleDefault("yn");
          double yp = line->getTagDoubleDefault("yp");
          double xn = line->getTagDoubleDefault(".low");
          double xp = line->getTagDoubleDefault(".high");
          breakdowntext << "  \\draw[" << styleName << "Neg](" << xn << "," << yn-npar << ") rectangle (" << 0 << "," << yp-npar << ");\n";
          breakdowntext << "  \\draw[" << styleName << "Pos](" << 0 << "," << yn-npar << ") rectangle (" << xp << "," << yp-npar << ");\n";          
        }
        // legend
        TString title; 
        if(ranking->hasTagString("style.title.pos") || ranking->hasTagString("style.title.neg")) {
          //positive part
          legend << "\\draw[" << styleName << "Pos](" << -1 << "," << -0.3+ilegend << ") rectangle (" << 1 << "," << 0.3+ilegend << ");\n";          
          legend << "\\node[anchor=west] at(1.1,"<<ilegend<<"){" << TQStringUtils::convertROOTTeX2LaTeX(ranking->getTagStringDefault("style.title.pos",ranking->getTagStringDefault("style.title","missing title, use style.title.pos to specify") ) ) << "};\n";
          ilegend++;
          //negative part
          legend << "\\draw[" << styleName << "Neg](" << -1 << "," << -0.3+ilegend << ") rectangle (" << 1 << "," << 0.3+ilegend << ");\n";          
          legend << "\\node[anchor=west] at(1.1,"<<ilegend<<"){" << TQStringUtils::convertROOTTeX2LaTeX(ranking->getTagStringDefault("style.title.neg",ranking->getTagStringDefault("style.title","missing title, use style.title.neg to specify") ) ) << "};\n";
          ilegend++;
        } else if(ranking->getTagString("style.title",title)) {
          legend << "\\draw[" << styleName << "Neg](" << -1 << "," << -0.3+ilegend << ") rectangle (" << 0 << "," << 0.3+ilegend << ");\n";
          legend << "\\draw[" << styleName << "Pos](" << 0 << "," << -0.3+ilegend << ") rectangle (" << 1 << "," << 0.3+ilegend << ");\n";          
          legend << "\\node[anchor=west] at(1.1,"<<ilegend<<"){" << TQStringUtils::convertROOTTeX2LaTeX(title) << "};\n";
          ilegend++;
        }
        nrankings++;
      }
      // ranking axis
      if(nrankings>0){
        int effColorInt = extractColorNumberFromTag(config, "style.effectAxisColor", kRed);
        defs << TQStringUtils::getColorDefStringLaTeX("colEffectAxes",effColorInt)<<"\n";
        tex << "\\begin{scope}[name=ranking,colEffectAxes,";
        TQFolder* rankingaxis = output->getFolder("Axis.*");
        double wmin = rankingaxis->getTagDoubleDefault("wmin");
        double wmax = rankingaxis->getTagDoubleDefault("wmax");
        double abswmax = std::max(fabs(wmin),fabs(wmax));
        
        double abswmax_round = TQUtils::roundAutoUp(2*abswmax,1)/2.;
        double xscale = 1./abswmax_round;
        tex << "xscale=" << xscale;
        tex << "]\n";
        tex << breakdowntext.str();
        int nTicks = config->getTagIntegerDefault("nRankingTicksPerSide",2);
        double step = abswmax_round/nTicks; // with a total of 2*n+1 ticks (including the one at 0), we have 2*n intervalls. Hence, the interval width (=step) is given as calculated here. abswmax_round is already reasonably rounded, so no need for fancy rounding magic here anymore (we get at most one additional digit which is always a 5 (or 0).
        //std::cout<<"abswmax = "<<abswmax<<", abswmax_round = "<<abswmax_round<<", step = "<<step<<std::endl;
        for(double x=0; x<=abswmax_round; x+=step){
          tex << "  \\draw (" << x << "," << -0.2 << ") -- (" << x << "," << 0 << ") node [axlbl,above=3pt]{";
          tex << x;
          tex << "};\n";
        }
        for(double x=-step; x>=-abswmax_round; x-=step){
          tex << "  \\draw (" << x << "," << -0.2 << ") -- (" << x << "," << 0 << ") node [axlbl,above=3pt]{";
          tex << x;
          tex << "};\n";
        }        
        tex << "  \\draw (" << -abswmax_round << "," << 0 << ") -- (" << abswmax_round << "," << 0 << ");\n";
        tex << "  \\node[axlbl,above left=5pt] at (" << -1.1*abswmax_round << "," << 0 << ") {";
        if (config->hasTagString("style.effectAxisLabel")) {
          tex << config->getTagStringDefault("style.effectAxisLabel","");
        } else {
          if(config->getTagBoolDefault("ranking.relativeToValue",false)){
            tex << "$\\frac{\\Delta\\mu}{\\mu}$";
          } else if(config->getTagBoolDefault("ranking.relativeToUncertainty",false)){
            tex << "$\\frac{\\Delta\\mu}{\\sigma}$";
          } else {
            tex << "$\\Delta\\mu$";
          }
        }
        tex << "};\n";
        tex << "\\end{scope}\n";
      }
      tex << "\\begin{scope}[name=pulls]\n";
      // pulls
      int nPulls = 0;
      TQFolderIterator pulls(output->getListOfFolders("Overlay.Pulls.*"),true);
      while(pulls.hasNext()){
        TQFolder* pull = pulls.readNext();
        int color = pull->getTagIntegerDefault("style.color",kBlack);
        TString styleName = TQFolder::makeValidIdentifier(pull->GetName());
        TString colorName = styleName;
        colorName.Prepend("col");
        defs << TQStringUtils::getColorDefStringLaTeX(colorName,color)<<"\n";
        defs << "\\tikzset{"<<styleName << "/.style={pull,color="<<colorName<<"}}\n";
        // points and lines
        TQFolderIterator lines(pull->getListOfFolders("p.*"),true);
        std::list<TQFolder*> linelist;
        while(lines.hasNext()){
          TQFolder* line = lines.readNext();
          linelist.push_front(line);
        }
        for(auto line:linelist){
          double x = line->getTagDoubleDefault("x");
          double y = line->getTagDoubleDefault("y");
          if(vertical){
            double xn = line->getTagDoubleDefault("xn");
            double xp = line->getTagDoubleDefault("xp");
            tex << "  \\draw["<<styleName<<"] (" << xn << "," << y-npar << ") -- (" << xp << "," << y-npar << ");";
            tex << "  \\node[dot,"<<colorName<<"] at (" << x << "," << y-npar << ") {};\n";
          } else {
            double yn = line->getTagDoubleDefault("yn");
            double yp = line->getTagDoubleDefault("yp");            
            tex << "  \\draw["<<styleName<<"] (" << x+1 << "," << yn << ") -- (" << x+1 << "," << yp << ");";
            tex << "  \\node[dot,"<<colorName<<"] at (" << x+1 << "," << y << ") {};\n";
          }
        }
        nPulls++;
        // legend
        TString title; if(pull->getTagString("style.title",title)){
          legend << "\\draw["<<styleName<<"](" << -1 << "," << ilegend << ") -- (" << 1 << "," << ilegend << ");";
          legend << "\\node[dot,"<<colorName<<"] at (" << 0 << "," << ilegend << ") {};\n";
          legend << "\\node[anchor=west] at(1.1,"<<ilegend<<"){" << TQStringUtils::convertROOTTeX2LaTeX(title) << "};\n";
          ilegend++;
        }
      }
      // pull axis
      
      int pullColorInt = extractColorNumberFromTag(config, "style.pullAxisColor", kRed);
      defs << TQStringUtils::getColorDefStringLaTeX("colPullAxes",pullColorInt)<<"\n";
      if(nPulls>0){
        double pullAxMin = config->getTagDoubleDefault("style.pullAxisMin",-1);
        double pullAxDiv = config->getTagDoubleDefault("style.pullAxisDiv",.25);
        double pullAxMid = config->getTagDoubleDefault("style.pullAxisMid",0);                
        double pullAxMax = config->getTagDoubleDefault("style.pullAxisMax",+1);
        TString pullAxLabel = config->getTagStringDefault("style.pullAxisTitle","$\\frac{\\theta-\\theta_0}{\\Delta\\theta_0}$");
        
        for(double x=pullAxMin; x<=pullAxMax; x+=pullAxDiv){
          tex << "\\draw[colPullAxes] (";
          if(vertical) tex << x << "," << -npar-0.5+0.2;
          else tex << -0.2 << "," << x;
          tex << ") -- (";
          if(vertical) tex << x << "," << -npar-0.5;
          else tex << 0 << "," << x;
          tex << ") node [axlbl,";
          if(vertical) tex << "below=3pt";
          else tex << "left=3pt";
          tex << "]{" << x << "};\n";
          
        }
        
        tex << "\\node[axlbl,colPullAxes,anchor=north east,";
        tex << "xshift=-1em,yshift=-1em";
        tex << "] at (";
        if(vertical) tex << pullAxMin << "," << -npar-0.5;
        else         tex << 0 << "," << pullAxMin;
        tex << ") {" << pullAxLabel << "};\n";
        tex << "\\draw[colPullAxes] (";
        if(vertical) tex << pullAxMin << "," << -npar-0.5;
        else         tex << 0 << "," << pullAxMin;
        tex << ") -- (";
        if(vertical) tex << pullAxMax << "," << -npar-0.5;
        else         tex << 0 << "," << pullAxMax;
        tex << ");\n";
        tex << "\\draw[dashed,black] (";
        if(vertical) tex << pullAxMid << "," << -npar-0.5;
        else         tex << 0 << "," << pullAxMid;        
        tex << ") -- (";
        if(vertical) tex <<  pullAxMid << "," << 0;
        else         tex << npar << "," << pullAxMid;        
        tex << ");\n";
        tex << "\\draw[dashed,colPullAxes]   (";
        if(vertical) tex << pullAxMin << "," << -npar-0.5;
        else         tex << 0 << "," << pullAxMin;
        tex << ") -- (";
        if(vertical) tex << pullAxMin << "," << 0;
        else         tex << npar << "," << pullAxMin;        
        tex << ");\n";
        tex << "\\draw[dashed,colPullAxes]   (";
        if(vertical) tex << pullAxMax << "," << -npar-0.5;
        else         tex << 0 << "," << pullAxMax;        
        tex << ") -- (";
        if(vertical) tex << pullAxMax << "," << 0;
        else         tex << npar << "," << pullAxMax;                
        tex << ");\n";
        }
      tex << "\\end{scope}\n";
      // background highlighting
      int highlightColor;
      if(config->getTagInteger("style.highlight",highlightColor)){
        defs << TQStringUtils::getColorDefStringLaTeX("colHighlighting",highlightColor)<<"\n";
        tex << "\\begin{pgfonlayer}{background}\n";
        tex << "  \\foreach \\i in ";
        if(npar>3) tex << "{-1,-3,...,"<<-2*((npar+1)/2)<<"}";
        else tex << "{1}";
        tex << "{\\fill[fill=colHighlighting] let \\p1 = (current bounding box.east), \\p2 = (current bounding box.west) in (\\x1,\\i-0.5) rectangle (\\x2,\\i+0.5); }\n";
        tex << "\\end{pgfonlayer}\n";
      }
      // put everything together
      std::ofstream out(TQPathManager::getPathManager()->getTargetPath(outputfile).c_str());
      bool standalone = config->getTagBoolDefault("standalone",false);
      if(standalone){
        out << "\\documentclass{standalone}\n";
        out << "\\usepackage{tikz}\n";
        out << "\\usetikzlibrary{calc,arrows.meta,decorations.markings,math,patterns}\n";
        out << "\\ifpdftex\\usepackage[scaled=1]{helvet}\\fi\n";
        out << "\\ifxetex\\usepackage{fontspec}\\setsansfont{TeX Gyre Heros}\\fi\n";
        out << "\\begin{document}\n";
      }
      bool atlas = config->getTagBoolDefault("ATLAS",false);
      out << "\\begingroup\n";
      if(atlas){
        out << "\\ifpdftex\n";
        out << "\\renewcommand\\sfdefault{phv}\n";
        out << "\\renewcommand\\rmdefault{phv}\n";
        out << "\\renewcommand\\ttdefault{pcr}\n";
        out << "\\fi\n";        
      }
      TString xunit = config->getTagStringDefault("tikz.xUnit","3cm");
      TString yunit = config->getTagStringDefault("tikz.yUnit",".5cm");
      out << "\\begin{tikzpicture}[x="<<xunit<<",y="<<yunit<<",%\n";
      out << "  axlbl/.style={scale=" << textscale << ",anchor=center},%\n";
      out << "  pull/.style={{|[scale=" << textscale << "]}-{|[scale=" << textscale << "]}},%\n";
      out << "  dot/.style={circle,fill,inner sep=1pt,scale="<<textscale<<"},\n";
      out << "  every node/.append style={font=\\sffamily}\n";
      out << "]\n";
      out << defs.str();
      TString description = "";
      bool hasDesc = config->getTagString("description",description);
      if(atlas || hasDesc){
        out << "\\node[scale=1.,align=left] at (-0.1,2) {";
        if (atlas) {
          out << "\\huge\\textbf{ATLAS}";
          TString label = config->getTagStringDefault("ATLASlabel","");
          if(!label.IsNull()){
            out << " " << label;
          }
          if (hasDesc) out<<"\\\\"; //append newline in case we also have a description
        }
        if (hasDesc) {
          out<<description;
        }
        out << "};\n";
      }
      out << "\\begin{scope}[name=legend,shift={(" << 1.5 << ","<<1<<")},x=0.3cm]";
      out << legend.str();
      out << "\\end{scope}";
      out << tex.str();
      out << "\\end{tikzpicture}\n";
      out << "\\endgroup\n";
      if(standalone){
        out << "\\end{document}\n";
      }
      manager->info(TString::Format("wrote file '%s'",outputfile.Data()));
      return true;
    }
  };
    
  namespace {
    bool available_tikz = TSStatisticsManager::registerAction(new PlotResultsTikZ(),"PlotResultsTikZ");
  }
  
}
