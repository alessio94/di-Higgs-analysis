#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQTable.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TString.h"


#include <vector>
#include <algorithm>

#define nan std::numeric_limits<double>::quiet_NaN()

namespace{
  template<class X,class Y> bool in(const std::vector<X>& v, const Y& e){
    return std::find(v.begin(), v.end(), e) != v.end();
  }

  bool matchAny(const TString& s,const std::vector<TString>& filters){
    for(auto filter:filters){
      if(TQStringUtils::matches(s,filter)){
        return true;
      }
    }
    return false;
  }
}

namespace TSBaseActions {
  
  class PrintSystematicsImpact : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {
      if (!config) return false;
      TString outputPath = config->replaceInText(config->getTagStringDefault("outputPath"));
      outputPath = TQPathManager::getPathManager()->getTargetPath(outputPath);
      bool verbose = config->getTagBoolDefault("verbose",false);
      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        return false;
      }

      manager->info(TString::Format("printing for model '%s'",model->GetName()));
      
      TQFolder textReplacements("replacements");
      for(const auto& s:config->getTagVString("textReplacements")){
        textReplacements.importFromTextFile(TQPathManager::getPathManager()->findConfigPath(s).c_str());
      }
      bool showYields = config->getTagBoolDefault("showYields",false);
      bool showDiff = config->getTagBoolDefault("showDifference",true);
      bool useTitles = config->getTagBoolDefault("useTitles",false);
			
      std::map< std::pair< const std::string, const std::string>, std::map<const std::string, TQFolder* > > systematics;
      std::map< const std::string, std::string > sampleTitles;
      std::vector<std::string> channelList;
      std::vector<std::string> sampleList;
      std::vector<std::string> systematicList;
      std::vector<double> sampleNorm;

      std::vector<TString> systFilter = config->getTagVString("systematicsFilter");
      if(systFilter.empty()) systFilter.push_back("*");
      std::vector<TString> channelFilter = config->getTagVString("channelFilter");
      if(channelFilter.empty()) channelFilter.push_back("*");
      std::vector<TString> sampleFilter = config->getTagVString("sampleFilter");
      if(sampleFilter.empty()) sampleFilter.push_back("*");
      std::vector<TString> systBlacklist = config->getTagVString("systematicsBlacklist");
      std::vector<TString> channelBlacklist = config->getTagVString("channelBlacklist");
      std::vector<TString> sampleBlacklist = config->getTagVString("sampleBlacklist");
      
      int nLines = 0;
      // collect all the folders
      TQFolderIterator channels(model->getListOfFolders("Channel.*"),true);
      while(channels.hasNext()){
        TQFolder* channel = channels.readNext();
        if(!channel) continue;
        TString channelname(channel->GetName());
        if(!TQStringUtils::removeLeadingText(channelname,"Channel.")) continue;
				if(matchAny(channelname,channelBlacklist) || !matchAny(channelname,channelFilter)) continue;
        if(!in(channelList,channelname.Data())) channelList.push_back(channelname.Data());
        TQFolderIterator samples(channel->getListOfFolders("Sample.*"),true);
        while(samples.hasNext()){
          TQFolder* sample = samples.readNext();
          if(!sample) continue;
          TString samplename(sample->GetName());
          if(!TQStringUtils::removeLeadingText(samplename,"Sample.")) continue;
          if(matchAny(samplename,sampleBlacklist) || !matchAny(samplename,sampleFilter)) continue;
          std::string samplenamestr(samplename.Data());
          std::string sampletitle = sample->getTagStandardStringDefault("style.title","");
          sampleTitles[samplenamestr] = sampletitle;
          if(!in(sampleList,samplename.Data())) {
            sampleList.push_back(samplename.Data());
            sampleNorm.push_back(sample->getTagDoubleDefault(".integral.nominal",0.));
          }
          //check if there will be any entries:
          
          TList* goodSamples = config->getTagBoolDefault("showGoodSystematics",true) ? sample->getListOfFolders("OverallSys.*") : NULL;
          TList* badSamples = config->getTagBoolDefault("showBadSystematics",false) ? sample->getListOfFolders(".Dropped/OverallSys.*") : NULL;
          if (0 == (goodSamples ? goodSamples->GetSize() : 0) + (badSamples ? badSamples->GetSize() : 0) ) continue; //don't create an entry if there would be just an empty line 
          nLines++; //if there should be an entry in this line, we increment this counter
          auto coordinate = std::make_pair(channelname.Data(),samplename.Data());
          systematics[coordinate] = std::map<const std::string, TQFolder*>();
          //if (config->getTagBoolDefault("showGoodSystematics",true)) {
            TQFolderIterator goodSystematics(goodSamples,true);
            while(goodSystematics.hasNext()){
              TQFolder* syst = goodSystematics.readNext();
              if(!syst) continue;
              TString systname(syst->GetName());
              if(!TQStringUtils::removeLeadingText(systname,"OverallSys.")) continue;
              systematics[coordinate][systname.Data()] = syst;
              if(!in(systematicList,systname.Data()) && (!matchAny(systname,systBlacklist)) && matchAny(systname,systFilter)) systematicList.push_back(systname.Data());
            }
          //}
          //if (config->getTagBoolDefault("showBadSystematics",false)) {
            TQFolderIterator badSystematics(badSamples,true);
            while(badSystematics.hasNext()){
              TQFolder* syst = badSystematics.readNext();
              if(!syst) continue;
              TString systname(syst->GetName());
              if(!TQStringUtils::removeLeadingText(systname,"OverallSys.")) continue;
              systematics[coordinate][systname.Data()] = syst;
              if(!in(systematicList,systname.Data()) && (!matchAny(systname,systBlacklist)) && matchAny(systname,systFilter)) systematicList.push_back(systname.Data());
            }
          //}
        }
      }

      std::sort(systematicList.begin(),systematicList.end());

      
      const size_t nSysts = systematicList.size();
      const size_t nRegions = channelList.size();
      const size_t nSamples = sampleList.size();

      std::stringstream ss;
      for(size_t i=0; i<nSysts; ++i){
        ss << systematicList[i] << ",";
      }
			if(verbose) manager->info("systematics: "+ss.str());
      ss.str("");
      for(size_t i=0; i<nRegions; ++i){
        ss << channelList[i] << ",";
      }
      if(verbose) manager->info("regions: "+ss.str());
      ss.str("");
      for(size_t i=0; i<nSamples; ++i){
        ss << sampleList[i] << ",";
      }
      if(verbose) manager->info("channels: "+ss.str());
      ss.str("");
      
      TQFolder outFolder("tables");
      TQFolder * regionTablesFolder = outFolder.getFolder("byRegion+");
      for(size_t i=0; i<nRegions; ++i){
        TQTable* table = new TQTable(channelList[i]);
        table->setEntry(0,0,channelList[i]);
        size_t jOffset = 0; //needed to avoid empty columns when skipping one column, since it's not present in this table
        for(size_t j=0; j<nSamples; ++j){
          auto coordinate = std::make_pair(channelList[i],sampleList[j]);
          double yieldNominal = sampleNorm[j];
          if (systematics.count(coordinate) == 0) {
            //table->setEntry(k+1,j+1,"---");
            jOffset++;
            continue; //this entry doesn't exist (we skipped it before)
          }
          TString collabel = useTitles ? sampleTitles[sampleList[j]] : textReplace(TString(sampleList[j].data()),textReplacements);
          table->setEntry(0, j+1-jOffset, collabel);
          for(size_t k=0; k<nSysts; ++k){
            TString rowlabel = textReplace(TString(systematicList[k].data()),textReplacements);
            table->setEntry(k+1,0,rowlabel);
            TQFolder* syst = systematics[coordinate][systematicList[k]];
            if(!syst){
              if(verbose)
                manager->warn(TString::Format("in Action 'PrintSystematicsImpact': unable to access systematic '%s' for sample '%s' in channel '%s'",systematicList[k].c_str(),sampleList[j].c_str(),channelList[i].c_str()));
              continue;
            }
            double up = (syst->getTagDoubleDefault("High",showDiff)- (showDiff ? 1. : 0.) ) * (showYields ? yieldNominal : 100.); //up variation in percent or absolute
            double down = (syst->getTagDoubleDefault("Low",showDiff)- (showDiff ? 1. : 0.) ) * (showYields ? yieldNominal : 100.); //down variation in percent
            int count   = (syst->getTagIntegerDefault("Entries",0));
            table->setEntry(k+1,j+1-jOffset,TString::Format("%s%.1f / %s%.1f", (up>0&&showDiff)?"+":"", up , (down>0&&showDiff)?"+":"", down ));
            if(count > 0){
              table->setProperty(k+1,j+1-jOffset,"tooltip",TString::Format("nRAW >= %d",count));
            } else {
              table->setProperty(k+1,j+1-jOffset,"tooltip","<no information available>");
            }
                     
          }
        }
        table->setHline(1);
        table->setVline(1);
        
        TString path = TQFolder::concatPaths(outputPath,"byRegion",channelList[i]);
        TQUtils::ensureDirectoryForFile(path);
        table->writeCSV(path+".csv");
        table->writeHTML(path+".html");
        table->writeLaTeX(path+".tex");
        table->writePlain(path+".txt");
        regionTablesFolder->addObject(table);
      }
      
      
      TQFolder* sampleTablesFolder = outFolder.getFolder("bySample+");
      for(size_t j=0; j<nSamples; ++j){
        TQTable* table = new TQTable(sampleList[j]);
        table->setEntry(0,0,sampleList[j]);
        double yieldNominal = sampleNorm[j];
        size_t iOffset = 0; //needed to avoid empty columns when skipping one column, since it's not present in this table
        for(size_t i=0; i<nRegions; ++i){
          auto coordinate = std::make_pair(channelList[i],sampleList[j]);
          if (systematics.count(coordinate) == 0) {
            //table->setEntry(k+1,i+1,"---");
            iOffset++;
            continue; //this entry doesn't exist (we skipped it before)
          }
          table->setEntry(0, i+1-iOffset, textReplace(TString(channelList[i].data()),textReplacements));
          for(size_t k=0; k<nSysts; ++k){
            //if (i==0) { //on first iteration over all syst, create row label
              table->setEntry(k+1,0,textReplace(TString(systematicList[k].data()),textReplacements));
            //}
            TQFolder* syst = systematics[coordinate][systematicList[k]];
            if(!syst){
              if(verbose)
                manager->warn(TString::Format("in Action 'PrintSystematicsImpact': unable to access systematic '%s' for sample '%s' in channel '%s'",systematicList[k].c_str(),sampleList[j].c_str(),channelList[i].c_str()));
              continue;
            }
            double up   = (syst->getTagDoubleDefault("High",showDiff)- (showDiff ? 1. : 0.) ) * (showYields ? yieldNominal : 100.); //up variation in percent or absolute
            double down = (syst->getTagDoubleDefault("Low" ,showDiff)- (showDiff ? 1. : 0.) ) * (showYields ? yieldNominal : 100.); //down variation in percent
            int count   = (syst->getTagIntegerDefault("Entries",0));
            table->setEntry(k+1,i+1-iOffset,TString::Format("%s%.1f / %s%.1f", (up>0&&showDiff)?"+":"", up , (down>0&&showDiff)?"+":"", down ));
            if(count > 0){
              table->setProperty(k+1,i+1-iOffset,"tooltip",TString::Format("nRAW >= %d",count));
            } else {
              table->setProperty(k+1,i+1-iOffset,"tooltip","<no information available>");
            }
          }
        }
        table->setHline(1);
        table->setVline(1);
        
        TString path = TQFolder::concatPaths(outputPath,"bySample",sampleList[j]);
        TQUtils::ensureDirectoryForFile(path);
        table->writeCSV(path+".csv");
        table->writeHTML(path+".html");
        table->writeLaTeX(path+".tex");
        table->writePlain(path+".txt");
        sampleTablesFolder->addObject(table);
      }
      
      
      
      TQFolder* systematicTablesFolder = outFolder.getFolder("bySystematic+");
      for(size_t k=0; k<nSysts; ++k){
        TQTable* table = new TQTable(systematicList[k]);
        table->setEntry(0,0,systematicList[k]);
        
        for(size_t j=0; j<nSamples; ++j){
          double yieldNominal = sampleNorm[j];
          size_t iOffset = 0; //needed to avoid empty columns when skipping one column, since it's not present in this table
          table->setEntry(0,j+1,textReplace(TString(sampleList[j].data()),textReplacements));
          for(size_t i=0; i<nRegions; ++i){
            auto coordinate = std::make_pair(channelList[i],sampleList[j]);
            if (systematics.count(coordinate) == 0) {
            //  iOffset++;
              continue; //this entry doesn't exist (we skipped it before)
            }
            table->setEntry(i+1-iOffset, 0, textReplace(TString(channelList[i].data()),textReplacements));
            TQFolder* syst = systematics[coordinate][systematicList[k]];
            if(!syst){
              if(verbose)
                manager->warn(TString::Format("in Action 'PrintSystematicsImpact': unable to access systematic '%s' for sample '%s' in channel '%s'",systematicList[k].c_str(),sampleList[j].c_str(),channelList[i].c_str()));
              continue;
            }
            double up   = (syst->getTagDoubleDefault("High",showDiff)- (showDiff ? 1. : 0.) ) * (showYields ? yieldNominal : 100.); //up variation in percent or absolute
            double down = (syst->getTagDoubleDefault("Low" ,showDiff)- (showDiff ? 1. : 0.) ) * (showYields ? yieldNominal : 100.); //down variation in percent
            int count   = (syst->getTagIntegerDefault("Entries",0));
            table->setEntry(i+1-iOffset, j+1,TString::Format("%s%.1f / %s%.1f", (up>0&&showDiff)?"+":"", up , (down>0&&showDiff)?"+":"", down ));
            if(count > 0){
              table->setProperty(i+1-iOffset, j+1,"tooltip",TString::Format("nRAW >= %d",count));
            } else {
              table->setProperty(i+1-iOffset, j+1,"tooltip","<no information available>");
            }
          }
        }
        table->setHline(1);
        table->setVline(1);
        
        TString path = TQFolder::concatPaths(outputPath,"bySystematic",systematicList[k]);
        TQUtils::ensureDirectoryForFile(path);
        table->writeCSV(path+".csv");
        table->writeHTML(path+".html");
        table->writeLaTeX(path+".tex");
        table->writePlain(path+".txt");
        systematicTablesFolder->addObject(table);
      }
      
      
      
      outFolder.writeToFile(TQPathManager::getPathManager()->getTargetPath(TQFolder::concatPaths(outputPath,"systematicsImpact.root")).c_str(),true);
      //cleanup
      outFolder.deleteAll();
      textReplacements.deleteAll();
      return true;
    }
    
    TString textReplace(TString text, TQFolder& replacements) const {
      TQFolder* f = replacements.getFolder(text);
      if (f) return f->getTagStringDefault("title",text); //return original text if title tag is not present
      return text;
    }
    
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new PrintSystematicsImpact(),"PrintSystematicsImpact");
  }
}
    
