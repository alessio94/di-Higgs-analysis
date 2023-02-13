#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"

#include <vector>
#include <algorithm>

// #define _DEBUG_

#include "QFramework/TQLibrary.h"

namespace{
  template<class X,class Y> bool in(const std::map<X,Y>& m, const X& e){
    return m.find(e) != m.end();
  }
}

namespace TSBaseActions {
  
  class RemoveNormalizationUncertainty : public TSStatisticsManager::Action {
    

    bool execute(TQFolder * config) const override {
      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        models()->print();
        return false;
      }


      TQFolderIterator itr(config->getListOfFolders());
      while (itr.hasNext()) {
        TQFolder* subconfig = itr.readNext();
        manager->info(TString::Format("RemoveNormalizationUnceratinty(...): removing systematics: %s",subconfig->GetName()));

        bool verbose = subconfig->getTagBoolDefault("~verbose",false);
        
        double integral_nom = 0;
        std::map<TString,double> integral_up;
        std::map<TString,double> integral_down;        

        TString chanName;
        if (!subconfig->getTagString("Channel",chanName)) {
          manager->error("Missing config option: 'Channel'");
          return false;
        }
        TQStringUtils::removeLeadingText(chanName,"Channel.");

        TString samName;
        if (!subconfig->getTagString("Sample",samName)) {
          manager->error("Missing config option: 'Sample'");
          return false;
        }
        TQStringUtils::removeLeadingText(samName,"Sample.");                
        
        std::vector<TString> systematicNames = subconfig->getTagVString("systematic");
        if (systematicNames.size() < 1) {
          systematicNames = subconfig->getTagVString("systematics");
        }
        if (systematicNames.size() < 1) {
          manager->error("Missing config option: 'systematic'/'systematics'");
          return false;
        }

        std::vector<TQFolder*> samples;
        // first pass: collect information
        TQFolderIterator channelItr(model->getListOfFolders("Channel.*"),true);
        while(channelItr.hasNext()) {
          TQFolder* channel = channelItr.readNext();
          TString channelName = channel->GetName();
          TQStringUtils::removeLeadingText(channelName,"Channel.");
          channel->getTagString("~Channel",channelName);

          DEBUG("Matching Channel '%s' to '%s'",channelName.Data(),chanName.Data());
          if(!TQStringUtils::matches(channelName,chanName)) continue;

          TQFolderIterator sampleItr(channel->getListOfFolders("Sample.*"),true);
          while(sampleItr.hasNext()) {
            TQFolder* sample = sampleItr.readNext();
            TString sampleName = sample->GetName();
            TQStringUtils::removeLeadingText(sampleName,"Sample.");
            sample->getTagString("~Sample",sampleName);

            DEBUG("Matching Sample '%s' to '%s'",sampleName.Data(),samName.Data());
            if(!TQStringUtils::matches(sampleName,samName)) continue;

            double nom = sample->getTagDoubleDefault(".integral.nominal",0.);
            integral_nom += nom;
            samples.push_back(sample);

            TQFolderIterator systItr(sample->getListOfFolders("OverallSys.*"),true);
            while(systItr.hasNext()) {
              TQFolder* systematic = systItr.readNext();
              TString systName = systematic->GetName();
              TQStringUtils::removeLeadingText(systName,"OverallSys.");              
              systematic->getTagString("~Systematic",systName);
              
              bool process = false;
              for(const auto& syst:systematicNames){
                DEBUG("Matching Systematic '%s' to '%s'",systName.Data(),syst.Data());                
                if(TQStringUtils::matches(systName,syst)) process=true;
              }
              if(!process) continue;

              double up,down;
              if (!systematic->getTagDouble("High",up) || !systematic->getTagDouble("Low",down)) {
                manager->error(TString::Format("Failed to retrieve source variations for '%s'!",systematic->GetName()));
                continue;
              }

              if(!in(integral_up,systName)){
                integral_up[systName] = up*nom;
              } else {
                integral_up[systName] += up*nom;
              }
              
              if(!in(integral_down,systName)){
                integral_down[systName] = down*nom;
              } else {
                integral_down[systName] += down*nom;
              }              
            }
          }
        }


        // second pass: apply changes
        for(auto* sample:samples){
          TString sampleName = sample->GetName();
          TQStringUtils::removeLeadingText(sampleName,"Sample.");
          DEBUG("processing sample '%s'",sample->getPath().Data());
          int applied = 0;
          for(const auto& syst:integral_up){
            TString systName(syst.first);
            TQFolder* systematic = sample->getFolder("OverallSys."+systName);

            if(!systematic){
              DEBUG("Failed to retrieve overall systematic for '%s' from sample '%s'!",systName.Data(),sample->getPath().Data());
              continue;              
            }
            
            double up,down;
            if (!systematic->getTagDouble("High",up) || !systematic->getTagDouble("Low",down)) {
              manager->error(TString::Format("Failed to retrieve source variations for '%s'!",systName.Data()));
              continue;
            }
            
            systematic->setTagDouble(TString::Format("High.before_%s",subconfig->GetName()),up);
            systematic->setTagDouble(TString::Format("Low.before_%s",subconfig->GetName()),down);
            
            double total_up   = integral_up  [systName] / integral_nom;
            double total_down = integral_down[systName] / integral_nom;
            
            double new_up   = up/total_up;
            double new_down = down/total_down;

            if(verbose) manager->info(TString::Format("scaling '%s' from %g/%g by %g/%g to %g/%g",systematic->getPath().Data(),up,down,total_up,total_down,new_up,new_down));
            if(fabs(new_up-1)>fabs(up-1) || fabs(new_down-1)>fabs(down-1)){
              manager->warn(TString::Format("uncertainty '%s' increased by removing total normalization, please double-check!",systName.Data()));
            }
            applied++;
            systematic->setTagDouble("High",up/total_up);
            systematic->setTagDouble("Low", down/total_down);
          }
          if(applied == 0){
            manager->error(TString::Format("failed to find any of the matched systematics in sample '%s'",sample->getPath().Data()));
          }
        }
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new RemoveNormalizationUncertainty(),"RemoveNormalizationUncertainty");
  }
}
    
