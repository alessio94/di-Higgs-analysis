#include "SFramework/TSSystematicsManager.h"

#include "QFramework/TQIterator.h"

// #define _DEBUG_

#include "QFramework/TQLibrary.h"

//__________________________________________________________________________________|___________

TSSystematicsManager::TSSystematicsManager(TQFolder* h) : TQSystematicsManager(h) {
  // legacy compatibility
  this->setVariationPrefix("");
}

//__________________________________________________________________________________|___________

TSSystematicsManager::TSSystematicsManager() : TQSystematicsManager(NULL) {
  // legacy compatibility  
  this->setVariationPrefix("");
}

//__________________________________________________________________________________|___________

bool TSSystematicsManager::processAllSystematics(TQFolder * parameters, TList* allSamples,
                                                   const TString& selectedSystFilter, const std::vector<TString>& sampleBlacklist, const std::vector<TString>&  systBlacklist) {
  // perform processing on all systematics
  info(TString::Format("processAllSystematics(): Start processing systematics matching '%s'",selectedSystFilter.Data()));

  TString overallFilter;
  TString shapeFilter;
  std::vector<TString> systematicsTypes = parameters->getTagVString("select.SystematicsTypes");
  if(systematicsTypes.empty()) {
    systematicsTypes.push_back("HistoSys");
    systematicsTypes.push_back("OverallSys");
  }    
  for (auto t : systematicsTypes) {
    if (t.Contains("OverallSys")) overallFilter = "OverallSys."+selectedSystFilter;
    if (t.Contains("HistoSys")) shapeFilter = "HistoSys."+selectedSystFilter;
  }
  
  TString allFilter = "*"+selectedSystFilter;

  // loop over all regions and samples
  TQFolderIterator sampleitr(allSamples);
  while (sampleitr.hasNext()) {
    TQFolder* sampleDef = sampleitr.readNext();
    if(!sampleDef) continue;
    bool isBlacklisted = false;
    for (auto& s: sampleBlacklist) {
      if(TQStringUtils::matches(sampleDef->getName(),"Sample."+s)) {
        isBlacklisted = true;
        break;
      }
    }
    if(isBlacklisted) continue;
    
    TQFolder* dropped = sampleDef->getFolder(".Dropped+");

    if (parameters->getTagBoolDefault("pruneNegligibles", false)) {
      // loop over all samples to remove the negligible ones
      TQFolderIterator allsysitr(sampleDef->getListOfFolders(allFilter), true);
      while(allsysitr.hasNext()) {
        TQFolder* sys = allsysitr.readNext();
        if (!sys || isBlacklistedSys(sys, systBlacklist)) continue;
        // prune syst based on contribution in region
        if(isNegligibleSys(sys,parameters)) {
          sys->detachFromBase();
          dropped->addObject(sys);
        }
      }
    }

    if (!overallFilter.IsNull()) {
      TQFolderIterator overallsysitr(sampleDef->getListOfFolders(overallFilter), true);
      while(overallsysitr.hasNext()){
        TQFolder* sys = overallsysitr.readNext();
        if(!sys || isBlacklistedSys(sys, systBlacklist)) continue;
        // process overall systematics
        if(!processOverallSys(parameters,sys)){
          sys->detachFromBase();
          dropped->addObject(sys);
        }
      }
    }

    if (!shapeFilter.IsNull()) {
      TQFolderIterator histosysitr(sampleDef->getListOfFolders(shapeFilter), true);
      while(histosysitr.hasNext()){
        TQFolder* sys = histosysitr.readNext();
        if(!sys || isBlacklistedSys(sys, systBlacklist)) continue;
        // process histogram systematics
        if(!processHistoSys(parameters,sys)){
          sys->detachFromBase();
          dropped->addObject(sys);
        }
      }
    }
  }
  return true;
}

//__________________________________________________________________________________|___________

bool TSSystematicsManager::includeSystematics(TList* allSamples, TQFolder* sysFolder) {

  TString sysName(sysFolder->GetName());
  
  //@tag: [IsOverallSys,IsHistoSys] declare as Normalization (OverallSys) and/or shape (HistoSys) systematic
  bool isOverallSys	= sysFolder->getTagBoolDefault("IsOverallSys", true);
  bool isHistoSys	= sysFolder->getTagBoolDefault("IsHistoSys", false);

  //@tag: [Include,Exclude] Inclusion and Exclusion string patterns for systematics  (will be matched to "Channel:Sample")
  TString filterInclude = sysFolder->getTagStringDefault("Include", "*");
  TString filterExclude = sysFolder->getTagStringDefault("Exclude", "");

  // loop over combinations of channels and samples
  int nOverall = 0;
  int nHisto = 0;
  TQFolderIterator itr(allSamples);
  while (itr.hasNext()) {
    TQFolder * sampleDef = itr.readNext();
    TString nameSample;
    TString nameChannel;    
    if(!sampleDef->getTagString("~Sample",nameSample) || !sampleDef->getTagString("~Channel",nameChannel)){
      error("unable to find 'Sample' or 'Channel' information!");
      return false;
    }
    TString typeSample = sampleDef->getTagStringDefault("~Type","B");
    if(typeSample == "D") continue;

    int nbins = sampleDef->getTagIntegerDefault("Bins",0);
    
    /* ===== systematics filter ===== */

    bool passesInclude = TQStringUtils::matchesFilter(nameChannel + ":" + nameSample, filterInclude, ",", true);
    bool passesExclude = TQStringUtils::matchesFilter(nameChannel + ":" + nameSample, filterExclude, ",", true);
    if (!passesInclude || passesExclude) {
      DEBUGclass("skipping '%s:%s' due to filters:\n  Include=%s\n  Exclude=%s",nameChannel.Data(),nameSample.Data(),filterInclude.Data(),filterExclude.Data());
      continue;
    }

    if(isOverallSys){
      TQFolder* sys = sampleDef->getFolder("OverallSys."+sysName+"+");
      sys->setTagString("Systematic",sysName);
      DEBUGclass("including %s as overall systematic for sample %s",sysName.Data(),nameSample.Data());
      if(!includeOverallSys(sysFolder,sys,nameChannel,nameSample)){
        sys->detachFromBase();
        delete sys;
      } else {
        nOverall++;
        sys->importTags(sysFolder->getBase());        
        sys->importTags(sysFolder);
      }
    }
    if(nbins != 1 && isHistoSys){
      TQFolder* sys = sampleDef->getFolder("HistoSys."+sysName+"+");
      sys->setTagString("Systematic",sysName);
      DEBUGclass("including %s as histo systematic for sample %s",sysName.Data(),nameSample.Data());      
      if(!includeHistoSys(sysFolder,sys,nameChannel,nameSample)){
        sys->detachFromBase();
        delete sys;
      } else {
        nHisto++;        
        sys->importTags(sysFolder->getBase());                
        sys->importTags(sysFolder);
      }
    }
  }
  
  info(TString::Format("includeSystematics('%s'): Included %d overall & %d histogram systematics. Included: '%s'. Excluded: '%s'", sysName.Data(), nOverall, nHisto,filterInclude.Data(),filterExclude.Data()));
  
  return true;
}

//__________________________________________________________________________________|___________

bool TSSystematicsManager::includeAllSystematics(TQFolder * systematics, TList* allSamples) {
  // loop over all systematics and include them for all samples
  if(!systematics) return true;

  info("includeAllSystematics(): Start including systematics ...");
  
  TQFolderIterator itr(systematics->getListOfFolders("?"), "!.*", true);
  bool ok = true;
  while (itr.hasNext()) {
    TQFolder* f = itr.readNext();
    DEBUGclass("including %s",f->GetName());
    if(!includeSystematics(allSamples, f)){
      ok = false;
    }
  }
  
  return ok;
}


