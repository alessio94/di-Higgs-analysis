#include <map>
#include <set>

#include "TString.h"

#include "QFramework/TQTable.h"
#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQStringUtils.h"
#include "TString.h"
#include "QFramework/TQHistogramUtils.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSSystematicsManager.h"

/*<cafdoc name=TransferSystematics>
  TransferSystematics
  ===========================
  
  Transfer systematics from one sample to another, or from one channel/region to another.
  
  This is useful in cases where individual samples or regions have low
  MC statistics and a larger region with similar kinematics is
  avialable to estimate the systematics from.  What is transferred is
  the relative normalization and/or shape.
  
  
  Usage:
  ---------------
  ```
  +TransferSystematics {
    +HWWRun2GGF {
      +ZtautauMerging {
        <mode = "channel">
        # wildcarding is allowed everywhere
        # you can also mix values and lists of values
        <source = "SR_*Mll1_PtSubLeadincl*_x">
        <select.Channels={"SR_*Mll1_PtSubLead2*_e","SR_*Mll1_PtSubLead2*_m","SR_*Mll1_PtSubLead2*_x"}>
        <select.Samples = {"Zjets*"}>
        <select.Systematics = "theo_ztautau_*">
        }
      +ww_0j_merging_all {
        <mode = "channel">
        <select.Samples = {"qqWW0jet"}>
        # it is typically useful to define merged regions to evaluate the systematics on
        # these merged regions can be present when the model is built, and can be removed from the model before building the workspace using EditModel
        <source = "SR_All">
        <select.Channels={"SR_bin*"}>
        <select.Systematics ={"theo_ww_CKKW","theo_ww_QSF" }>
      }
      +zttandvgamma_0j_merging_from_all {
        <mode = "channel">
        <source = "SR_All">
        <select.Samples = {"Zjets0jet","Vgamma0jet"}>
        <select.Channels={"SR_bin*"}>
        <select.Systematics ={"ATLAS_JER*","ATLAS_JES*","ATLAS_MET*","MUONS_SAGITTA*", "MUONS_SCALE", "MUONS_MS", "MUONS_ID", "ATLAS_EG_SCALE_AF2","ATLAS_EG_SCALE_ALL","EG_RESOLUTION_ALL","PRW_DATASF"}>
      }
      # you can instruct the action to create a table of all the transfers actually made in the end like this
      <writeTable="transfer.tex">
    }
  }
  ```

  The edits will be made in-place on the model.

</cafdoc> */

namespace TSBaseActions {

  class TransferSystematics : public TSStatisticsManager::Action {
    
    typedef std::map<TString,std::set<TString> > LogElement;    
    typedef std::map<TString,LogElement> Log;
    void addToLog(Log& log, const TString& sysname, const TString& originname, const TString& targetname) const{
      if(log.find(sysname) == log.end()) log[sysname] = LogElement();
      if(log[sysname].find(originname) == log[sysname].end()) log[sysname][originname] = LogElement::mapped_type();
      log[sysname][originname].insert(targetname);
    }

    TString cleanup(TString input) const {
      TQStringUtils::removeLeadingText(input,"Sample.");
      TQStringUtils::removeLeadingText(input,"Channel.");
//      TQStringUtils::removeLeadingText(input,"OverallSys.");
//      TQStringUtils::removeLeadingText(input,"HistoSys.");
      return input;
    }

    void writeLog(const Log& log,TQTaggable* config) const {
      TString tableFile;
      bool writeTable = config->getTagString("writeTable",tableFile);
      if(!writeTable) return;
      TQTable table;
      table.setEntry(0,0,"Systematic");      
      table.setEntry(0,2,"Source");      
      table.setEntry(0,3,"Target");      
      int line = 1;
      for(const auto& sys:log){
        table.setEntry(line,0,sys.first);
        for(const auto& source:sys.second){
          table.setEntry(line,1,source.first);
          for(const auto& target:source.second){
            table.setEntry(line,2,"$\\rightarrow$");            
            table.setEntry(line,3,target);
            line++;
          }
        }
      }
      if(tableFile.EndsWith(".tex")){
        table.writeLaTeX(tableFile);
      } else if(tableFile.EndsWith(".html")){
        table.writeHTML(tableFile);
      } else {
        table.write(tableFile);
      }
    }
    
    bool matchesAny(TQFolder* f,const std::vector<TString>& filters, const TString&prefix = "") const {
      bool matched = false;
      for(const auto& x:filters){
        if(TQStringUtils::matches(f->GetName(),prefix+"."+x)){
          matched = true;
          break;
        }
      }
      return matched;
    }

    bool execute(TQFolder * config) const override {

      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        return false;
      }

      TQFolderIterator transfers = config->getListOfFolders("?");
      int nOK = 0;
      Log log;
      while(transfers.hasNext()){
        TQFolder* transfer = transfers.readNext();
        if(!transfer) continue;
        nOK += performTransfer(transfer,model,log);
      }
      if(nOK>0){
        writeLog(log,config);        
      }        
      
      return nOK;
    }

    
    bool performTransfer(TQFolder * config,TQFolder* model,Log& log) const {
      TString mode = config->getTagStringDefault("mode","sample");
      mode.ToLower();
      TQFolder* histograms = model->getFolder(".Histograms");

      TSSystematicsManager man;
      man.setRepository(histograms);
      
      bool verbose = config->getTagBoolDefault("~verbose",false);
      
      TString tagCheckFilter = config->getTagStringDefault("compareTags","rebin*,include*flow,remap");
      
      std::vector<TString> exceptRegions = config->getTagVString("except.Channels");
      std::vector<TString> exceptSamples = config->getTagVString("except.Samples");
      std::vector<TString> exceptSystematics = config->getTagVString("except");

      manager->info(TString::Format("performing transfer '%s' in mode '%s'",config->GetName(),mode.Data()));
      
      std::vector<TString> targetSamples = config->getTagVString("select.Samples");
      if(targetSamples.empty()){
        targetSamples.push_back("*");
      }
      std::vector<TString> targetChannels = config->getTagVString("select.Channels");
      if(targetChannels.empty()){
        targetChannels.push_back("*");
      }
      std::vector<TString> targetSystematics = config->getTagVString("select.Systematics");
      if(targetSystematics.empty()){
        targetSystematics.push_back("*");
      }

      TString mark;
      bool marked = config->getTagString("selectTag",mark);

      std::vector<TString> sysTypes = config->getTagVString("types");
      if(sysTypes.size() == 0){
        sysTypes.push_back("OverallSys");
        if(mode.Contains("sample")){
          sysTypes.push_back("HistoSys");
        }
      }
      
      if (mode.Contains("sample",TString::kIgnoreCase)) {
        TString sourceSample;
        if(!config->getTagString("source",sourceSample)){
          manager->error(TString::Format("missing required 'source' information on transfer '%s'",config->GetName()));
          return false;
        }
  
        int i=0;
  
        for(const auto& channelExp:targetChannels){
          TQFolderIterator regions(model->getListOfFolders("Channel."+channelExp),true);
          while(regions.hasNext()){
            TQFolder* region = regions.readNext();
            if(!region) continue;
            if(matchesAny(region,exceptRegions,"Channel")) continue;
            if(verbose) manager->info(TString::Format("transferring in region '%s'",region->GetName()));
            TQFolder* source = region->getFolder("Sample."+sourceSample);
            if(!source){
              manager->warn(TString::Format("unable to find source '%s' in channel '%s', skipping!",sourceSample.Data(),region->GetName()));
              continue;
            }
            for(const auto& sampleExp:targetSamples){
              TQFolderIterator samples(region->getListOfFolders("Sample."+sampleExp),true);
              while(samples.hasNext()){
                TQFolder* sample = samples.readNext();
                if(!sample) continue;
                if(matchesAny(sample,exceptSamples,"Sample")) continue;
                if(TQStringUtils::matches(sample->GetName(),"Sample."+sourceSample)) continue;
                if(verbose) manager->info(TString::Format("\tfor sample '%s'",sample->GetName()));
                sample->deleteObject(".Dropped!");
                for (const auto& systType : sysTypes){
                  std::vector<TQFolder*> remove;
                  {
                    TQFolderIterator oldsystematics(sample->getListOfFolders(systType+".*"),true);
                    while(oldsystematics.hasNext()){
                      TQFolder* systematic = oldsystematics.readNext();
                      if(!systematic) continue;
                      if(matchesAny(systematic,exceptSystematics,systType) || !matchesAny(systematic,targetSystematics,systType)) continue;
                      if(marked && !systematic->getTagBoolDefault(mark,false)) continue;
                      if(verbose) manager->info(TString::Format("\t\tremoving '%s'",systematic->GetName()));
                      remove.push_back(systematic);
                    }
                  }
                  for(auto systematic:remove){
                    systematic->detachFromBase();
                    delete systematic;
                  }
                  {
                    TQFolderIterator newsystematics(source->getListOfFolders(systType+".*"),true);
                    int j = 0;
                    while(newsystematics.hasNext()){
                      TQFolder* systematic = newsystematics.readNext();
                      if(!systematic) continue;
                      if(matchesAny(systematic,exceptSystematics,systType) || !matchesAny(systematic,targetSystematics,systType)) continue;
                      if(marked && !systematic->getTagBoolDefault(mark,false)) continue;
                      if(verbose) manager->info(TString::Format("\t\tcopying '%s'",systematic->GetName()));
                      TQFolder* copy = systematic->copy();
                      copy->setTagString("transferred",source->GetName());
                      if(systematic->hasTagString("HistoLow") && systematic->hasTagString("HistoHigh")){
                        TH1* nomHist  = man.getHisto(sample->getTagStringDefault("Histo",""));
                        TH1* lowHist  = man.getHisto(systematic->getTagStringDefault("HistoLow",""));
                        TH1* highHist = man.getHisto(systematic->getTagStringDefault("HistoHigh",""));
                        if(!TQHistogramUtils::checkConsistency(nomHist,lowHist) || !TQHistogramUtils::checkConsistency(nomHist,highHist)){
                          manager->error(TString::Format("transferred histograms for systematic '%s' in sample '%s' in region '%s' are inconsistent, skipping!\n\tnom\t%s\n\tlow\t%s\n\thigh\t%s",systematic->GetName(),sample->GetName(),region->GetName(),TQStringUtils::getDetails(nomHist).Data(),TQStringUtils::getDetails(lowHist).Data(),TQStringUtils::getDetails(highHist).Data()));
                          delete copy;
                          continue;
                        }
                      }                      
                      if(copy){
                        ++j;
                        addToLog(log,cleanup(systematic->GetName()),sourceSample,cleanup(sample->GetName()));
                        sample->addFolder(copy);
                      }
                    }
                    if(verbose && j==0){
                      manager->warn(TString::Format("\tno systematics of type %s found in '%s' matching '%s' except '%s'!",systType.Data(),source->GetName(),TQStringUtils::concat(targetSystematics,",").Data(),TQStringUtils::concat(exceptSystematics,",").Data()));                      
                    }
                    i += j;                    
                  }
                }
              }
            }
          }
        }

        if(i>0) return true;
        manager->error("no targets found for sample transfer!");
        return false;
        
      } else if (mode.Contains("channel",TString::kIgnoreCase)) {
        
        TString sourceChannel;
        if(!config->getTagString("source",sourceChannel)){
          manager->error(TString::Format("missing required 'source' information on transfer '%s'",config->GetName()));
          return false;
        }
           
        int i=0;
        for(const auto& channelExp:targetChannels){
          TQFolderIterator regions(model->getListOfFolders("Channel."+channelExp),true);
          while(regions.hasNext()){
            TQFolder* region = regions.readNext();
            if(!region) continue;
            if(region->GetName() == sourceChannel) continue; //there is no point in transfering from A to A (it would even delete some stuff!)
            if(matchesAny(region,exceptRegions,"Channel")) continue;
            if(verbose) manager->info(TString::Format("transferring in region '%s'",region->GetName()));
            for(const auto& sampleExp:targetSamples){
              TQFolderIterator samples(region->getListOfFolders("Sample."+sampleExp),true);
              while(samples.hasNext()){
                TQFolder* sample = samples.readNext();
                if(!sample) continue;               
                if(matchesAny(sample,exceptSamples,"Sample")) continue;
                TQFolder* sourceRegion = model->getFolder("Channel."+sourceChannel);
                TQFolder* source = sourceRegion ? sourceRegion->getFolder(sample->GetName()) : NULL;
                if(!source){
                  manager->warn(TString::Format("unable to find source '%s'/'%s' in model '%s', skipping!",sourceChannel.Data(),sample->GetName(),model->GetName()));
                  continue;
                }
                if(verbose) manager->info(TString::Format("\tfor sample '%s'",sample->GetName()));
                sample->deleteObject(".Dropped!");
                for (const auto& systType : sysTypes){                
                  std::vector<TQFolder*> remove;
                  {
                    TQFolderIterator oldsystematics(sample->getListOfFolders(systType+".*"),true);
                    while(oldsystematics.hasNext()){
                      TQFolder* systematic = oldsystematics.readNext();
                      if(!systematic) continue;
                      if(matchesAny(region,exceptSystematics,systType)) continue;
                      if(marked && !systematic->getTagBoolDefault(mark,false)) continue;
                      if(!matchesAny(systematic,targetSystematics,systType)) continue;
                      if(verbose) manager->info(TString::Format("\t\tremoving '%s'",systematic->GetName()));
                      remove.push_back(systematic);
                    }
                  }
                  for(auto systematic:remove){
                    systematic->detachFromBase();
                    delete systematic;
                  }
                  {
                    int j=0;                    
                    TQFolderIterator newsystematics(source->getListOfFolders(systType+".*"),true);
                    while(newsystematics.hasNext()){
                      TQFolder* systematic = newsystematics.readNext();
                      if(!systematic) continue;
                      if(matchesAny(region,exceptSystematics,systType)) continue;
                      if(marked && !systematic->getTagBoolDefault(mark,false)) continue;
                      if(!matchesAny(systematic,targetSystematics,systType)) continue;
                      if(verbose) manager->info(TString::Format("\t\tcopying '%s'",systematic->GetName()));
                      TQFolder* copy = systematic->copy();
                      copy->setTagString("transferred",sourceRegion->GetName());
                      if(systematic->hasTagString("HistoLow") && systematic->hasTagString("HistoHigh")){
                        TString histoPathLow=systematic->getTagStringDefault("HistoLow","");
                        TString histoPathHigh=systematic->getTagStringDefault("HistoHigh","");
                        TString nameLow = TString::Format("%s_%s_HistoLow",region->GetName(),sample->GetName());
                        TString nameHigh = TString::Format("%s_%s_HistoHigh",region->GetName(),sample->GetName());
                        TH1* TargetnomHist      = man.getHisto(sample->getTagStringDefault("Histo",""));
                        TH1* nomHist            = man.getHisto(source->getTagStringDefault("Histo",""));
                        const TH1* lowHistOrig  = man.getHisto(histoPathLow);
                        const TH1* highHistOrig = man.getHisto(histoPathHigh);
                        if(!TQHistogramUtils::checkConsistency(nomHist,lowHistOrig) || !TQHistogramUtils::checkConsistency(nomHist,highHistOrig)){
                          manager->error(TString::Format("transferred histograms for systematic '%s' in sample '%s' in region '%s' are inconsistent, skipping!\n\tnom\t%s\n\tlow\t%s\n\thigh\t%s",systematic->GetName(),sample->GetName(),region->GetName(),TQStringUtils::getDetails(nomHist).Data(),TQStringUtils::getDetails(lowHistOrig).Data(),TQStringUtils::getDetails(highHistOrig).Data()));                          
                          delete copy;
                          continue;
                        }
                        if(!systematic->getBase()->tagsAreEquivalentTo(sample,tagCheckFilter)){
                          manager->error(TString::Format("cowardly refusing to transfer histogram with inconsistent production tags!"));
                          continue;
                        }
                        if(lowHistOrig){
                          TH1* lowHist = TQHistogramUtils::copyHistogram(lowHistOrig,nameLow);
                          TQHistogramUtils::normalize(lowHist,nomHist);
                          lowHist->Divide(nomHist);
                          lowHist->Multiply(TargetnomHist);
                          TQHistogramUtils::normalize(lowHist,TargetnomHist);
                          TQFolder::getPathTail(histoPathLow);
                          histograms->addObject(lowHist,histoPathLow+"+");
                          copy->setTagString("HistoLow",TQFolder::concatPaths(histoPathLow,lowHist->GetName()));
                        }
                        if(highHistOrig){
                          TH1* highHist = TQHistogramUtils::copyHistogram(highHistOrig,nameHigh);
                          TQHistogramUtils::normalize(highHist,nomHist);
                          highHist->Divide(nomHist);
                          highHist->Multiply(TargetnomHist);
                          TQHistogramUtils::normalize(highHist,TargetnomHist);
                          TQFolder::getPathTail(histoPathHigh);
                          histograms->addObject(highHist,histoPathHigh+"+");
                          copy->setTagString("HistoHigh",TQFolder::concatPaths(histoPathHigh,highHist->GetName()));
                        }
                      }
                      if(copy){
                        ++j;
                        sample->addFolder(copy);
                        addToLog(log,cleanup(systematic->GetName()),sourceChannel,cleanup(region->GetName()));                    
                      }
                    }
                    if(verbose && j==0){
                      manager->warn(TString::Format("\tno systematics of type %s found in '%s/%s' matching '%s' except '%s'!",systType.Data(),sourceRegion->GetName(),source->GetName(),TQStringUtils::concat(targetSystematics,",").Data(),TQStringUtils::concat(exceptSystematics,",").Data()));
                    }
                    i += j;
                  }
                }
              }
            }
          }
        }

        if(i>0) return true;
        manager->error("no targets found for channel transfer!");
        return false;        
      }      
      //no valid mode was selected
      manager->error("Failed to perform, no valid mode was selected!");
      return false;
      
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new TransferSystematics(),"TransferSystematics");
  }
}
