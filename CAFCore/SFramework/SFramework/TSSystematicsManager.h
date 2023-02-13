//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef TSSYSTEMATICSMANAGER_H
#define TSSYSTEMATICSMANAGER_H

#include "QFramework/TQSystematicsManager.h"

class TSSystematicsManager : public TQSystematicsManager {
  
 public:
  Bool_t  processAllSystematics(TQFolder * parameters, TList* allSamples, const TString& filter = "*", const std::vector<TString>& sampleBlacklist = {}, const std::vector<TString>& systBlacklist = {});
  
  // including systematics
  Bool_t includeSystematics   (TList* allSamples,      TQFolder* systematic);
  Bool_t includeAllSystematics(TQFolder * systematics, TList* allSamples);  
  
  TSSystematicsManager(TQFolder* h);
  TSSystematicsManager();
};
#endif
