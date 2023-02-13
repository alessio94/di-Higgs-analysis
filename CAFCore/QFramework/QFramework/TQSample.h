//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQSample__
#define __TQSample__

#include "TTree.h"

class TQSampleVisitor;
class TQCounter;
class TQToken;

#include "QFramework/TQSampleFolder.h"

#include "QFramework/ASG.h"
#ifdef HAS_XAOD
namespace xAOD { //EXCLUDE
  class TEvent;
}
#endif

class TQSample : public TQSampleFolder { 
  protected:
  TString fTreeLocation = "";
  double fNormalisation = 1.;

  
  TTree * fTree = 0; //!
#ifdef HAS_XAOD
  xAOD::TEvent* fEvent = 0; //!
#endif
  bool fTreeIsTransient = false; //!
  bool fTokensAreShared = false; //!
  
  TFile * fFile = 0; //!
  TList * fTokens = 0; //!

  void setTree(TFile* file, const TString& treename);
  bool getTree();
  void promoteTreeToFriends();
  void retractTreeFromFriends();

  void clearTree();

  friend TQSampleFolder;

  virtual void findFriendsInternal(TQSampleFolder* otherSF, bool forceUpdateSubsamples = false, std::shared_ptr<TList> otherSFContents = nullptr) override;
  bool createFriendLinksForSamplesWithTreeLocation(TQSampleFolder* otherSF, std::shared_ptr<TList> otherSFContents = nullptr);

public:

  static bool gUseTransientTree;
  static bool gUseAthenaAccessMode; //use kAthenaAccess mode for xAODs (default is to use kClassAccess mode which is faster but does not work in all cases)

  static TList * splitTreeLocations(TString treeLocations);
  static TString extractFilename(const TString& treeLocation);
  static TString extractTreename(const TString& treeLocation);
  
  TQSample();
  TQSample(const TString& name);

  virtual TQFolder * newInstance(const TString& name) override;
 
  void setNormalisation(double normalisation_);
  double getNormalisation();

  bool setTreeLocation(TString treeLocation_);
  TString getTreeLocation();
  const TString& getTreeLocationRef();
  
  bool updateTreeLocation();

  TString getFilename();
  TString getTreename();
  TFile* getFile();

  TQSample * getBaseSample();

  bool addSubSample(TQSample * subSample);
  TQSample * addSelfAsSubSample(const TString& name);
  TQSample * getSubSample(const TString& path);
  bool isSubSample();
  bool hasSubSamples();

  TQToken * getTreeToken();
  TQToken * getFileToken();
  TQToken * getEventToken();
  bool returnTreeToken(TQToken * &token_);
  bool returnToken(TQToken * &token_);
  int getNTreeTokens();
  void printTreeTokens();

  bool checkTreeAccessibility();
  virtual int getOwnSize() const override;
  
  virtual ~TQSample();
 
  ClassDefOverride(TQSample, 3); // container class to hold a specific data or monte carlo sample

};

#endif
