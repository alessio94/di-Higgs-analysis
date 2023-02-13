//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQxAODskimmingAlgorithm__
#define __TQxAODskimmingAlgorithm__

#include "TString.h"
#include "QFramework/TQAlgorithm.h"

namespace xAOD {
  class TEvent;
  class TStore;
}
class TFile;
class TQToken;
class TQSampleFolder;
class TQSample;

class TQxAODskimmingAlgorithm : public TQAlgorithm{
protected:
  TFile* fOutfile = NULL;
  bool fSingleDSIDmode = false;
  bool fFinishedFirstDSID = false;
  TString fBasePath;
  TString fFilePrefix;
  
  TString fFlagName = "passedSelection";
  TString fAmiTagRegexp = "[esarp][0-9][0-9][0-9][0-9]{0,3}"; //default: one valid tag type character followed by at least 4 digits
  
  TQSampleFolder* fSuperSample = NULL; //in case of samples with subSamples this should point to the common base/super sample. This is a TQSampleFolder* in order to allow for setting a special tag which causes a sample folder with a bunch of TQSamples inside to be considered like a super-sample. Usecase: e.g. for data samples 
  TQSample* fSample = NULL; //currently used sample (i.e. sample with refference to an input file)
  TQToken* fEventToken = NULL;
  xAOD::TEvent* fEvent = NULL;
  int fNsubSamplesToProcess = 0;
  int fNsubSamplesProcessed = 0;
  
  std::vector<std::string> getListOfCutBookkeeperNames() const;
  
  TString getDSID(TQSampleFolder* sf) const;
  std::vector<TString> getCommonAmiTags(const std::vector<TString>& inputFileNames) const;
  
public:
  
  inline virtual bool isSingleton() const override {return true;} 
  
  inline void setOutputDir(const TString& path) {fBasePath=path;}
  inline const TString& getOutputDir() const {return fBasePath;}
  
  inline void setFilePrefix(const TString& prefix) {fFilePrefix=prefix;}
  inline const TString& getFilePrefix() const {return fFilePrefix;}
  
  inline void setAmiTagRegexp(const TString& regExp) {fAmiTagRegexp=regExp;}
  inline const TString& getAmiTagRegexp() const {return fAmiTagRegexp;}
  
  inline void setFlagName(const TString& flag) {fFlagName = flag;}
  inline const TString& getFlagName() const {return fFlagName;}
  
  virtual bool cleanup() override;
  virtual bool initializeSingleton(const std::vector<TQSample*>& samples) override;
  virtual bool finalize() override;
  virtual bool execute() override;  

  ClassDefOverride(TQxAODskimmingAlgorithm,0)
};

#endif
