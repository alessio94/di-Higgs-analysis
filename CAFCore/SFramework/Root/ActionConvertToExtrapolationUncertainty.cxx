#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"

#include <vector>
#include <algorithm>

/*<cafdoc name=ConvertToExtrapolationUncertainties>
  ConvertToExtrapolationUncertainties
  ===========================

  Usually, workspaces built with SFramework rely on the
  Normalization-Factor method of incorporating systematic
  uncertainties with normalization effects. This means that
  normalization effects of the systematic uncertainty are determined
  in both the signal and the control region, and it is left for the
  fit itself to resolve any cancellation between these from the
  observed correlations. However, under some circumstances, it can be
  desireable to explicitly convert normalization uncertainties to
  extrapolation factors by performing the cancellation "by hand" and
  removing the uncertainty in the control region. This is what this
  action is for.

  Usage:
  ---------------
  ```
  +ConvertToExtrapolationUncertainty.top {
    +HWWRun2GGF2J {
      <source="Channel.ShapeTopCR_bTag">
      <sample="Sample.top">
    }
  }
  ```


</cafdoc>*/

namespace{
  template<class X,class Y> bool in(const std::vector<X>& v, const Y& e){
    return std::find(v.begin(), v.end(), e) != v.end();
  }
}

namespace TSBaseActions {
  
  class ConvertToExtrapolationUncertainty : public TSStatisticsManager::Action {
    

    bool execute(TQFolder * config) const override {
      manager->info("ConvertToExtrapolationUncertainty(...): converting systematics");
      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        models()->print();
        return false;
      }
      
      std::vector<TString> processedSystematics;
      
      TQFolderIterator itr(config->getListOfFolders());
      while (itr.hasNext()) {
        TQFolder* subconfig = itr.readNext();
        TString sourceName;
        
        if (!subconfig->getTagString("source",sourceName)) {
          manager->error("Missing config option: 'source'");
          return false;
        }
        TQStringUtils::ensureLeadingText(sourceName,"Channel.");
        
        std::vector<TString> systematicNames = subconfig->getTagVString("systematic");
        if (systematicNames.size() < 1) {
          systematicNames = subconfig->getTagVString("systematics");
        }
        if (systematicNames.size() < 1 && (subconfig->hasTagString("systematic") || subconfig->hasTagString("systematics") ) ) {
          systematicNames.push_back(subconfig->getTagStringDefault("systematic",subconfig->getTagStringDefault("systematics","")));
        }
        if (systematicNames.size() < 1) {
          manager->error("Missing config option: 'systematic'/'systematics'");
          return false;
        }
        //safety check if systematic was already processed, e.g., for a different region (this would lead to unpredictable behavior!)
        for (size_t i=0; i<systematicNames.size(); i++) {
          manager->info(systematicNames[i]);
          TQStringUtils::ensureLeadingText(systematicNames[i],"OverallSys.");
          manager->info(systematicNames[i]);
          if (in(processedSystematics,systematicNames[i])) {
            manager->error(TString::Format("Caught attempt to modify systematic '%s' more than once. This could lead to unpredictable results!",systematicNames[i].Data()));
            return false;
          } else {
            processedSystematics.push_back(systematicNames[i]);
          }
        }
        
        
        std::vector<TString> sampleNames = subconfig->getTagVString("sample");
        if (sampleNames.size() < 1) {
          sampleNames = subconfig->getTagVString("samples");
        }
        if (sampleNames.size() < 1 && (subconfig->hasTagString("sample") || subconfig->hasTagString("samples") ) ) {
          sampleNames.push_back(subconfig->getTagStringDefault("sample",subconfig->getTagStringDefault("samples","")));
        }
        if (sampleNames.size() < 1) sampleNames.push_back("*");
        
        for (size_t i=0; i<sampleNames.size(); i++) {
          TQStringUtils::ensureLeadingText(sampleNames[i],"Sample.");
        }
        
        TQFolder* sourceChannel = model->getFolder(sourceName);
        if (!sourceChannel) {
          manager->error(TString::Format("Failed to retrieve source channel (region) '%s'!",sourceName.Data()));
          return false;
        }
        
        TQFolderIterator channelItr(model->getListOfFolders("Channel.*"));
        while(channelItr.hasNext()) {
          TQFolder* channel = channelItr.readNext();
          if (channel==sourceChannel || !channel) continue; //nothing to do YET on the source (CR) channel (we will delete the systematics there later on)
          for (size_t sampleCounter=0; sampleCounter<sampleNames.size(); sampleCounter++) {
            TQFolderIterator sampleItr(channel->getListOfFolders(sampleNames[sampleCounter]));
            
            while(sampleItr.hasNext()) {
              TQFolder* sample = sampleItr.readNext();
              TQFolder* sourceSample = sourceChannel->getFolder(sample->GetName());
              if (!sourceSample || !sample) continue;
              
              for (size_t systCounter=0; systCounter<systematicNames.size(); systCounter++) {
                TQFolderIterator systItr(sample->getListOfFolders(systematicNames[systCounter]));
                while(systItr.hasNext()) {
                  TQFolder* systematic = systItr.readNext();
                  TQFolder* sourceSystematic = sourceSample->getFolder(systematic->GetName());
                  if (!systematic || !sourceSystematic) continue;
                  double sUp,sDown,tUp,tDown;
                  if (!sourceSystematic->getTagDouble("High",sUp) || !sourceSystematic->getTagDouble("Low",sDown)) {
                    manager->error(TString::Format("Failed to retrieve source variations for '%s'!",sourceSystematic->GetName()));
                    continue;
                  }
                  if (sUp == 0. || sDown == 0.) {
                    manager->error(TString::Format("A source variation for '%s' is zero, skipping this variation.",sourceSystematic->GetName()));
                    continue;
                  }
                  if (systematic->getTagDouble("High",tUp)) {
                    systematic->setTagDouble("High",tUp/sUp);
                  }
                  if (systematic->getTagDouble("Low",tDown)) {
                    systematic->setTagDouble("Low",tDown/sDown);
                  }
                  manager->info(TString::Format("Converted systematic '%s' in region '%s' for sample '%s'",systematic->GetName(),channel->GetName(),sample->GetName()));
                  sourceSystematic->setTagBool(".converted.extrapolation",true);
                  systematic->setTagBool(".converted.extrapolation",true);
                  
                }
              }
            }
          }
        }
        manager->info("ConvertToExtrapolationUncertainty(...): cleaning up source systematics...");
        //cleanup of source systematics:        
        for (size_t sampleCounter=0; sampleCounter<sampleNames.size(); sampleCounter++) {
          TQFolderIterator sampleItr(sourceChannel->getListOfFolders(sampleNames[sampleCounter]));
          while(sampleItr.hasNext()) {
            TQFolder* sample = sampleItr.readNext();
            if (!sample) continue;
            for (size_t systCounter=0; systCounter<systematicNames.size(); systCounter++) {
              TQFolderIterator systItr(sample->getListOfFolders(systematicNames[systCounter]));
              while(systItr.hasNext()) {
                TQFolder* systematic = systItr.readNext();
                if (!systematic) continue;
                if (systematic->getTagBoolDefault(".converted.extrapolation",false)) {
                  systematic->getFolder("../.converted+"); //ensure a ".converted" folder exists
                  systematic->moveTo("../.converted"); //move systematic
                }
              }
            }
          }
        }
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ConvertToExtrapolationUncertainty(),"ConvertToExtrapolationUncertainty");
  }
}
    
