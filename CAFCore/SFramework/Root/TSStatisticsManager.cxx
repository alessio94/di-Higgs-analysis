#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iterator>

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSStatisticsPlotter.h"

#include "TSystem.h"
#include "TParameter.h"
#include "Math/ProbFuncMathCore.h"
#include "Math/QuantFuncMathCore.h"

#include "QFramework/TQSampleFolder.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQSampleDataReader.h"
#include "QFramework/TQTaggable.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQIteratorT.h"
#include "QFramework/TQFolder.h"
#include "QFramework/TQPathManager.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

ClassImp(TSStatisticsManager)

typedef std::map<const std::string,const TSStatisticsManager::Action*> ActionList;

namespace {
  inline ActionList& Actions(){
    static ActionList sActions;
    return sActions;
  }
}

//__________________________________________________________________________________|___________

int TSStatisticsManager::Action::perform(TQFolder* config, TSStatisticsManager* m) const {
  // apply this action
  if(!config){
    throw std::runtime_error("invalid config passed to statistics manager!");
  }
  TQFolderIterator itr(config->getListOfFolders("?"));
  this->manager = m;
  bool content = false;
  size_t nerr = 0;
  while(itr.hasNext()){
    TQFolder* c = itr.readNext();
    if(!c){
      throw std::runtime_error("invalid NULL configuration!");
    }
    content = true;
    if(!this->execute(c)){
      ERRORclass("unable to perform action '%s'",config->GetName());
      ++nerr;
    }
  }
  this->manager = NULL;
  if(!content){
    ERRORclass("action '%s' has no content!",config->GetName());
  }
  return nerr;
}

//__________________________________________________________________________________|___________

TQFolder* TSStatisticsManager::Action::models () const{
  return manager->getModels();
}

//__________________________________________________________________________________|___________

TQFolder* TSStatisticsManager::Action::workspaces () const{
  return manager->getWorkspaces();
}

//__________________________________________________________________________________|___________

TQFolder* TSStatisticsManager::Action::results () const{
  return manager->getResults();
}

//__________________________________________________________________________________|___________

bool TSStatisticsManager::registerAction(const Action* a, const char* name){
  Actions()[name] = a;
  return true;
}

//__________________________________________________________________________________|___________

const std::map<const std::string,const TSStatisticsManager::Action*>& TSStatisticsManager::getActions() {
  return Actions();
}

//__________________________________________________________________________________|___________

void TSStatisticsManager::printActions(){
  for(auto a:Actions()){
    std::cout << a.first << std::endl;
  }
}

//__________________________________________________________________________________|___________

TSStatisticsManager::TSStatisticsManager(): 
  fModels		(NULL),
  fWorkspaces	(NULL),
  fResults	(NULL),
  fDefaultSampleFolder(NULL),
  fFileCompression(1)
{
  this->reset();
}


//__________________________________________________________________________________|___________

void TSStatisticsManager::info(TString message) {
  cout << "SFramework/TSStatisticsManager: " << message.Data() << endl;
}


//__________________________________________________________________________________|___________

void TSStatisticsManager::error(TString message) {
  info(TQStringUtils::makeBoldRed(TString("ERROR: ") + message));
}


//__________________________________________________________________________________|___________

void TSStatisticsManager::warn(TString message) {
  info(TQStringUtils::makeBoldYellow(TString("WARNING: ") + message));
}


//__________________________________________________________________________________|___________

void TSStatisticsManager::reset() {

  if (fModels) {
    delete fModels;
  }
  fModels = new TQFolder("models");
  
  if (fWorkspaces) {
    delete fWorkspaces;
  }
  fWorkspaces = new TQFolder("workspaces");
  
  
  if (fResults) {
    delete fResults;
  }
  fResults = new TQFolder("results");
}


//__________________________________________________________________________________|___________

void TSStatisticsManager::setDefaultSampleFolder(TQSampleFolder * sampleFolder) {

  fDefaultSampleFolder = sampleFolder;
}


//__________________________________________________________________________________|___________

TQSampleFolder * TSStatisticsManager::getDefaultSampleFolder() {

  return fDefaultSampleFolder;
}


//__________________________________________________________________________________|___________

TQFolder * TSStatisticsManager::getModels() {
  // Returns the list of models
  return fModels;
}


//__________________________________________________________________________________|___________

TQFolder * TSStatisticsManager::getWorkspaces() {
  // Returns the list of workspaces
  return fWorkspaces;
}


//__________________________________________________________________________________|___________

TQFolder * TSStatisticsManager::getResults() {
  // Returns the list of results
  return fResults;
}

//__________________________________________________________________________________|___________

Bool_t TSStatisticsManager::run(TQFolder * config) {
  bool dummy = config->getTagBoolDefault("dummy",false);
  std::vector<TString> libraries = config->getTagVString("extraLibs");
  for(size_t i=0; i<libraries.size(); ++i){
    TString lib(libraries[i]);
    TQStringUtils::ensureLeadingText(lib,"lib");
    if(!gSystem->Load(lib)){
      this->info(TString::Format("loaded external library '%s'",lib.Data()));
    } else {
      this->error(TString::Format("error loading external library '%s'",lib.Data()));
    }
  }
  TString outfile;
  if(config->getTagString("saveConfig",outfile)){
    if(outfile.Contains("$")){
      this->error("invalid output file given for saveConfig option: "+outfile);
      return false;
    }
    // TString outPath = TQPathManager::getPathManager()->getLocalPath(outfile);
    TString outPath = TQPathManager::getPathManager()->getTargetPath(outfile);
    TQUtils::ensureDirectoryForFile(outPath);
    TString resolved = TQPathManager::getPathManager()->getTargetPath(outPath);
    INFOclass("writing expanded config to '%s'",resolved.Data());
    if(resolved.EndsWith(".root")){
      config->writeToFile(resolved);
    } else {
      config->exportToTextFile(resolved);
    }
  }
  bool ok = true;
  TQFolderIterator itr(config->getListOfFolders("?"));
  while(itr.hasNext()){
    TQFolder* f = itr.readNext();
    f->detachFromBase(); //safety measure to prevent actions from changing configs for other actions
    if(!f) continue;
    TString name(f->GetName());
    TString action;
    TQStringUtils::readUpTo(name,action,".");
    TQStringUtils::removeLeading(name,".");
    f->setTagString(".label",name);
    auto it = Actions().find(action.Data());
    if(it == Actions().end()){
      ERRORclass("unknown action '%s'",f->GetName());
    } else {
      INFOclass(" === %s: %s === ",it->first.c_str(),f->GetName());
      //      this->fModels->print();
      //      this->fWorkspaces->print();
      //      this->fResults->print();
      if (!dummy){
        if(it->second->perform(f,this) > 0){
          ok = false;
        }
      }
    }
  }
  
  return ok;
}

//__________________________________________________________________________________|___________

void  TSStatisticsManager::setHistogramsFileCompression(Int_t compression){
  this->fFileCompression = compression;
}

//__________________________________________________________________________________|___________

Int_t TSStatisticsManager::getHistogramsFileCompression(){
  return this->fFileCompression;
}

//__________________________________________________________________________________|___________

TSStatisticsManager::~TSStatisticsManager() {
}
