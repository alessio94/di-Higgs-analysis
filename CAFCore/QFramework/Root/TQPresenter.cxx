#include "QFramework/TQPresenter.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQPathManager.h"
#include <fstream>

#define _DEBUG_

#include "QFramework/TQLibrary.h"


////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQPresenter:
//
// The abstract TQPresenter class provides a base class for all types
// of data visualization appliances by providing extensive handling of
// configurations for cuts and processes in a physics analysis. The
// TQPlotter, TQCutflowPrinter and TQCutflowPlotter class inherit from
// TQPresenter.
//
////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(TQPresenter)

TQPresenter::TQPresenter() : 
fReader(NULL),
  fOwnerOfReader(false),
  fNormalizationInfo(NULL),
  fCuts(new TObjArray()),
  fProcesses(new TObjArray()),
  fSystematics(new TQFolder("systematics"))
{
  // default constructor
  this->fCuts->SetOwner(true);
  this->fProcesses->SetOwner(true);
}

TQPresenter::TQPresenter(TQSampleDataReader* reader) : 
  fReader(reader),
  fOwnerOfReader(false),
  fNormalizationInfo(NULL),
  fCuts(new TObjArray()),
  fProcesses(new TObjArray()),
  fSystematics(new TQFolder("systematics"))
{
  // default constructor
  this->fCuts->SetOwner(true);
  this->fProcesses->SetOwner(true);
}

TQPresenter::TQPresenter(TQSampleFolder* samples) : 
  fReader(samples ? new TQSampleDataReader(samples) : NULL),
  fOwnerOfReader(samples),
  fNormalizationInfo(NULL),
  fCuts(new TObjArray()),
  fProcesses(new TObjArray()),
  fSystematics(new TQFolder("systematics"))
{
  // default constructor
  this->fCuts->SetOwner(true);
  this->fProcesses->SetOwner(true);
}

int TQPresenter::nProcesses() const{
  // return the number of currently listed processes
  return this->fProcesses->GetEntries();
}
  
int TQPresenter::nCuts() const{
  // return the number of currently listed cuts
  return this->fCuts->GetEntries();
}

std::vector<TString> TQPresenter::getProcessPaths(const TString& processFilter) const{
  // get a list of all individual paths accessed by this presenter
  std::vector<TString> paths;
  this->appendProcessPaths(processFilter,paths);
  return paths;
}

int TQPresenter::appendProcessPaths(const TString& processFilter, std::vector<TString>* paths) const{
  // append to a list all individual paths accessed by this presenter
  if(!paths) throw std::runtime_error("unable to append to NULL pointer!");
  return this->appendProcessPaths(processFilter,*paths);
}

int TQPresenter::appendProcessPaths(const TString& processFilter, std::vector<TString>& paths) const{
  // append to a list all individual paths accessed by this presenter
  TQTaggableIterator pitr(this->fProcesses);
  int i=0;
  while(pitr.hasNext()){
    TQNamedTaggable* process = pitr.readNext();
    if(!process) continue;
    if(TQStringUtils::matches(process->GetName(),processFilter)){
      paths.push_back(process->getTagStringDefault(".path",""));
      ++i;
    }
  }
  return i;
}

void TQPresenter::addCut (const TString& cutName, const TString& cutTitle, const TString& tags){
  // add a cut with the given name, title, and tags
  TQNamedTaggable* cut = new TQNamedTaggable(cutName);
  cut->SetTitle(cutTitle);
  cut->importTags(tags);
  cut->setTagString(".name",cutName);
  cut->setTagString(".title",cutTitle);
  this->fCuts->Add(cut);
}

void TQPresenter::addProcess(const TString& processName, const TString& processTitle, const TString& tags){
  // add a process with the given name, title, and tags
  TQNamedTaggable* process = new TQNamedTaggable(processName);
  process->SetTitle(processTitle);
  process->importTags(tags);
  process->setTagString(".path",processName);
  process->setTagString(".name",TQFolder::makeValidIdentifier(processName));
  process->setTagString(".title",processTitle);
  this->fProcesses->Add(process);
}

void TQPresenter::addCut (const TString& cutName, const TString& cutTitle, TQTaggable& tags){
  // add a cut with the given name, title, and tags
  TQNamedTaggable* cut = new TQNamedTaggable(cutName);
  cut->SetTitle(cutTitle);
  cut->importTags(tags);
  cut->setTagString(".name",cutName);
  cut->setTagString(".title",cutTitle);
  this->fCuts->Add(cut);
}

void TQPresenter::addProcess(const TString& processName, const TString& processTitle, TQTaggable& tags){
  // add a process with the given name, title, and tags
  TQNamedTaggable* process = new TQNamedTaggable(processName);
  process->SetTitle(processTitle);
  process->importTags(tags);
  process->setTagString(".path",processName);
  process->setTagString(".name",TQFolder::makeValidIdentifier(processName));
  process->setTagString(".title",processTitle);
  this->fProcesses->Add(process);
}

void TQPresenter::addCut (const TString& cutName, const TString& tags){
  // add a cut with the given name and tags
  TQNamedTaggable* cut = new TQNamedTaggable(cutName);
  cut->importTags(tags);
  if(cut->getNTags() < 1){
    cut->setTagString(".title",tags);
    cut->setName(tags);
  }
  cut->setTagString(".name",cutName);
  this->fCuts->Add(cut);
}

void TQPresenter::addProcess(const TString& processName, const TString& tags){
  // add a process with the given name and tags
  TQNamedTaggable* process = new TQNamedTaggable(processName);
  process->importTags(tags);
  process->setTagString(".name",TQFolder::makeValidIdentifier(processName));
  process->setTagString(".path",processName);
  this->fProcesses->Add(process);
}

void TQPresenter::addCut (const TString& cutName, TQTaggable& tags){
  // add a cut with the given name and tags
  TQNamedTaggable* cut = new TQNamedTaggable(cutName);
  cut->importTags(tags);
  cut->setTagString(".name",cutName);
  this->fCuts->Add(cut);
}

void TQPresenter::addProcess(const TString& processName, TQTaggable* tags){
  // add a process with the given name and tags
  TQNamedTaggable* process = new TQNamedTaggable(processName);
  process->importTags(tags);
  process->setTagString(".name",TQFolder::makeValidIdentifier(processName));
  process->setTagString(".path",processName);
  this->fProcesses->Add(process);
}

void TQPresenter::addProcess(const TString& processName, TQTaggable& tags){
  // add a process with the given name and tags
  TQNamedTaggable* process = new TQNamedTaggable(processName);
  process->importTags(tags);
  process->setTagString(".name",TQFolder::makeValidIdentifier(processName));
  process->setTagString(".path",processName);
  this->fProcesses->Add(process);
}

void TQPresenter::addCut (const TString& tags){
  // add a cut with the given tags
  TQNamedTaggable* cut = new TQNamedTaggable("");
  cut->importTags(tags);
  if(cut->getNTags() < 1){
    cut->setTagString(".name",tags);
    cut->setName(tags);
  }
  this->fCuts->Add(cut);
}

void TQPresenter::addProcess(const TString& tags){
  // add a process with the given tags
  TQNamedTaggable* process = new TQNamedTaggable("");
  process->importTags(tags);
  TString name;
  process->getTagString(".name",name);
  if(name.IsNull()){
    name=tags;
  }
  process->setName(name);
  this->fProcesses->Add(process);
}

void TQPresenter::addCut (TQTaggable& tags){
  // add a cut with the given tags
  TQNamedTaggable* cut = new TQNamedTaggable("");
  cut->importTags(tags);
  TString name;
  cut->getTagStringDefault(".name",name);
  if(name.IsNull()){
    name=tags.exportTagsAsString();
  }
  cut->setName(name);
  this->fCuts->Add(cut);
}

void TQPresenter::addProcess(TQTaggable& tags){
  // add a process with the given tags
  TString name = tags.getTagStringDefault(".name","");
  TQNamedTaggable* process = new TQNamedTaggable(name);
  process->importTags(tags);
  this->fProcesses->Add(process);
}

int TQPresenter::sanitizeProcesses() {
  // sanitize all processes 
  // this is a no-op function to be overwritten by derived classes
  // it should return the number of removed items
  return 0;
}

int TQPresenter::sanitizeCuts() {
  // sanitize all cuts 
  // this is a no-op function to be overwritten by derived classes
  // it should return the number of removed items
  return 0;
}

void TQPresenter::reset() {
  // reset everything
  this->resetProcesses();
  this->resetCuts();
  this->fSystematics->deleteAll();
}

void TQPresenter::resetCuts() {
  // reset all cuts
  this->fCuts->Clear();
}

void TQPresenter::resetProcesses() {
  // reset all processes
  this->fProcesses->Clear();
}

void TQPresenter::setReader(TQSampleDataReader * reader) {
  // set the sample data reader to be used to obtain data
  if (fReader && fOwnerOfReader) delete fReader;
  fReader = reader;
  fOwnerOfReader = false;
}

TQSampleDataReader * TQPresenter::getReader() {
  // get the data reader to be used to obtain data
  return fReader;
}

void TQPresenter::setSampleFolder(TQSampleFolder* sf){
  // set/change the base sample folder
  if(this->fReader && this->fOwnerOfReader){
    delete this->fReader;
  }
  if(sf){
    this->fReader = new TQSampleDataReader(sf);
    this->fOwnerOfReader = true;
  } else {
    this->fReader = NULL;
  } 
}

TQSampleFolder* TQPresenter::getSampleFolder(){
  // retrieve the base sample folder
  if(!this->fReader) return NULL;
  return this->fReader->getSampleFolder();
}

void TQPresenter::exportScheme(TQTaggable* tags){
  // exports the current scheme to some taggable object
  if(!tags) return;
  tags->importTags(this);
  TQTaggableIterator pitr(this->fProcesses);
  while(pitr.hasNext()){
    TQNamedTaggable* entry = pitr.readNext();
    if(!entry) continue;
    tags->importTagsWithPrefix(entry,TString::Format(".processes.%d.",pitr.getLastIndex()));
  }
  TQTaggableIterator citr(this->fCuts);
  while(citr.hasNext()){
    TQNamedTaggable* entry = citr.readNext();
    if(!entry) continue;
    tags->importTagsWithPrefix(entry,TString::Format(".cuts.%d.",citr.getLastIndex()));
  }
}

void TQPresenter::importScheme(TQTaggable* tags){
  // imports the scheme from the given taggable object
  if(!tags) return;
  this->reset();
  this->importTags(tags);
  this->removeTags(".processes.*");
  this->removeTags(".cuts.*");
  int idx = 0;
  TString name;
  while(tags->getTagString(TString::Format(".processes.%d..name",idx),name)){
		if(tags->getTagString(TString::Format(".processes.%d..name",idx),name)){
			TQNamedTaggable* process = new TQNamedTaggable(name);
			process->importTagsWithoutPrefix(tags,TString::Format(".processes.%d.",idx));
			this->fProcesses->Add(process);
			idx++;
		}
	}
	idx = 0;
  while(tags->getTagString(TString::Format(".cuts.%d..name",idx),name)){
    TQNamedTaggable* cut = new TQNamedTaggable(name);
    cut->importTagsWithoutPrefix(tags,TString::Format(".cuts.%d.",idx));
    this->fCuts->Add(cut);
    idx++;
  }
}

void TQPresenter::clearProcesses(){
  // remove all scheduled processes
  this->fProcesses->Clear();
}

void TQPresenter::clearCuts(){
  // remove all scheduled processes
  this->fCuts->Clear();
}

void TQPresenter::importSchemeFromPath(const TString& path){
  // imports the scheme from a folder at the given path
  if(!this->fReader || !this->fReader->getSampleFolder()) return;
  this->importScheme(this->fReader->getSampleFolder()->getFolder(path));
}

void TQPresenter::printProcesses(){
  // print all currently defined processes
  TQTaggableIterator itr(fProcesses);
  while(itr.hasNext()){
    TQNamedTaggable* tags = itr.readNext();
    if(!tags){
      std::cout << TQStringUtils::makeBoldYellow("<invalid NULL entry>") << std::endl;
      continue;
    } 
    std::cout << TQStringUtils::makeBoldWhite(TQStringUtils::fixedWidth(tags->GetName(),30));
    std::cout << " ";
    std::cout << tags->exportTagsAsString();
    std::cout << std::endl;
  }
}


void TQPresenter::printCuts(){
  // print all currently defined processes
  TQTaggableIterator itr(fCuts);
  while(itr.hasNext()){
    TQNamedTaggable* tags = itr.readNext();
    if(!tags){
      std::cout << TQStringUtils::makeBoldYellow("<invalid NULL entry>") << std::endl;
      continue;
    } 
    std::cout << TQStringUtils::makeBoldWhite(TQStringUtils::fixedWidth(tags->GetName(),30));
    std::cout << " ";
    std::cout << tags->exportTagsAsString();
    std::cout << std::endl;
  }
}

void TQPresenter::removeProcessesByName(const TString& nameFilter){
  // remove all processes matching the given name
  TQTaggableIterator itr(fProcesses);
  std::vector<TQNamedTaggable*> removals;
  while(itr.hasNext()){
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(TQStringUtils::matches(process->getTagStringDefault(".name",process->GetName()),nameFilter)){
      removals.push_back(process);
    }
  }
  for(size_t i=0; i<removals.size(); i++){
    fProcesses->Remove(removals[i]);
    delete removals[i];
  }
}

void TQPresenter::removeCutsByName(const TString& nameFilter){
  // remove all cuts matching the given name
  TQTaggableIterator itr(fCuts);
  std::vector<TQNamedTaggable*> removals;
  while(itr.hasNext()){
    TQNamedTaggable* cut = itr.readNext();
    if(!cut) continue;
    if(TQStringUtils::matches(cut->getTagStringDefault(".name",cut->GetName()),nameFilter)){
      removals.push_back(cut);
    }
  }
  for(size_t i=0; i<removals.size(); i++){
    fCuts->Remove(removals[i]);
    delete removals[i];
  }
}


int TQPresenter::importProcessesFromFile(const TString& fileName,const TString& tags){
  // read processes from a file - each line is interpreted as a TQTaggable string
  // text replacements are made from the given tags
  TQTaggable tmp(tags);
  return this->importProcessesFromFile(fileName,tmp);
}

int TQPresenter::importCutsFromFile(const TString& fileName, const TString& tags){
  // read cuts from a file - each line is interpreted as a TQTaggable string
  // text replacements are made from the given tags
  TQTaggable tmp(tags);
  return this->importCutsFromFile(fileName,tmp);
}

int TQPresenter::importProcessesFromFile(const TString& fileName,TQTaggable& tags){
  // read processes from a file - each line is interpreted as a TQTaggable string
  // text replacements are made from the given tags
  int retval = 0;
  std::vector<TString>* lines = TQStringUtils::readFileLines(TQPathManager::getPathManager()->findConfigPath(fileName).c_str());
  if(!lines){
    ERRORclass("unable to read file '%s'",fileName.Data());
    return 0;
  }
  // get list of tag lines
  for(size_t i=0; i<lines->size(); i++){
    TQNamedTaggable* thisTags = new TQNamedTaggable("");
    thisTags->importTags(tags.replaceInText(lines->at(i)));
    // TQSampleFolder being analyzed should have an identifier name, set by ".name" or ".path"
    // if neither are found (e.g. special), the line is processed standalone
    TString thisName = thisTags->getTagStringDefault(".name",thisTags->getTagStringDefault(".path",thisTags->GetName()));
    thisTags->SetName(thisName);
    // check if vline
    bool vline = thisName.EqualTo("|") || thisName.EqualTo("||");
    // check if a previous line specifying the process already exists
    TQNamedTaggable* processTags = !thisName.IsNull() ? this->getProcess(thisName) : nullptr;
    if (vline) {
      // vertical lines or special processes are always standalone
      this->fProcesses->Add(thisTags);
    } else if (processTags) {
      // tags specifying the same process are merged into one
      processTags->importTags(thisTags);
      delete thisTags;
    } else {
      // line is added standalone if process is new or not valid.
      this->fProcesses->Add(thisTags);
    }
    // count imported line
    retval++;
  }
  retval -= this->sanitizeProcesses();
  delete lines;
  return retval;
}

int TQPresenter::importCutsFromFile(const TString& fileName, TQTaggable& tags){
  // read cuts from a file - each line is interpreted as a TQTaggable string
  // text replacements are made from the given tags
  int retval = 0;
  std::vector<TString>* lines = TQStringUtils::readFileLines(fileName);
  if(!lines){
    ERRORclass("unable to read file '%s'",fileName.Data());
    return 0;
  }
  // get list of tag lines
  for(size_t i=0; i<lines->size(); i++){
    TQNamedTaggable* thisTags = new TQNamedTaggable("");
    thisTags->importTags(tags.replaceInText(lines->at(i)));
    // TQCut being processed an identifier name, set by ".name" or ".path"
    // if neither are found, the line is forcibly added standalone
    TString thisName = thisTags->getTagStringDefault(".name");
    thisTags->SetName(thisName);
    // check if hline
    bool hline = thisName.EqualTo("|") || thisName.EqualTo("||");
    // check if a previous line specifying the cut already exists
    TQNamedTaggable* cutTags = !thisName.IsNull() ? this->getCut(thisName) : nullptr;
    if (hline) {
      // hlines are always added standalone
      this->fCuts->Add(thisTags);
    } else if (cutTags) {
      // check if tags specifying cut already exists, merge into one.
      cutTags->importTags(thisTags);
      delete thisTags;
    } else {
      // line is added standalone
      this->fCuts->Add(thisTags);
    }
    // count imported line
    retval++;
  }
  retval -= this->sanitizeCuts();
  delete lines;
  return retval;
}

int TQPresenter::exportProcessesToFile(const TString& fileName,const TString& tags){
  // write processes to a file - each line is formatted as a TQTaggable string
  TQTaggable tmp(tags);
  return this->exportProcessesToFile(fileName,tmp);
}

int TQPresenter::exportCutsToFile(const TString& fileName, const TString& tags){
  // write cuts to a file - each line is formatted as a TQTaggable string
  TQTaggable tmp(tags);
  return this->exportCutsToFile(fileName,tmp);
}

int TQPresenter::exportProcessesToFile(const TString& fileName,TQTaggable&/*tags*/){
  // write processes to a file - each line is formatted as a TQTaggable string
  TQTaggableIterator itr(this->fProcesses);
  std::ofstream ofile(fileName);
  if(!ofile.is_open()) return -1;
  int retval = 0;
  while(itr.hasNext()){
    TQTaggable* process = itr.readNext();
    if(!process) continue;
    ofile << process->exportTagsAsString() << std::endl;
    retval++;
  }
  ofile.close();
  return retval;
}

int TQPresenter::exportCutsToFile(const TString& fileName, TQTaggable&/*tags*/){
  // write cuts to a file - each line is formatted as a TQTaggable string
  TQTaggableIterator itr(this->fCuts);
  std::ofstream ofile(fileName);
  if(!ofile.is_open()) return -1;
  int retval = 0;
  while(itr.hasNext()){
    TQTaggable* cut = itr.readNext();
    if(!cut) continue;
    ofile << cut->exportTagsAsString() << std::endl;
    retval++;
  }
  ofile.close();
  return retval;
}

TQPresenter::~TQPresenter(){
  // default destructor
  delete this->fProcesses;
  delete this->fCuts;
  if(this->fOwnerOfReader) delete this->fReader;
  delete this->fSystematics;
}


TQFolder* TQPresenter::setNormalizationInfo(const TString& path){
  // set the normalization info folder
  if(this->fReader && this->fReader->getSampleFolder()){
    this->fNormalizationInfo = this->fReader->getSampleFolder()->getFolder(path);
  }
  return this->fNormalizationInfo;
}

void TQPresenter::setNormalizationInfo(TQFolder* f){
  // set the normalization info folder
  this->fNormalizationInfo = f;
}

TQFolder* TQPresenter::getNormalizationInfo(){
  // retrieve the normalization info folder
  return this->fNormalizationInfo;
}

const TQNamedTaggable* TQPresenter::getCutConst(const TString& cutName) const{
  // get the title of a cut
  TQTaggableIterator itr(this->fCuts);
  while(itr.hasNext()){
    TQNamedTaggable* cut = itr.readNext();
    if(!cut) continue;
    if(!TQStringUtils::matches(cut->GetName(),cutName)) continue;
    return cut;
  }
  return NULL;
}

TQNamedTaggable* TQPresenter::getCut(const TString& cutName){
  // get the title of a cut
  TQTaggableIterator itr(this->fCuts);
  while(itr.hasNext()){
    TQNamedTaggable* cut = itr.readNext();
    if(!cut) continue;
    if(!TQStringUtils::matches(cut->GetName(),cutName)) continue;
    return cut;
  }
  return NULL;
}

TString TQPresenter::getCutTitle(const TString& cutName) const{
  // get the title of a cut
  TQNamedTaggable* cut = const_cast<TQNamedTaggable*>(this->getCutConst(cutName));
  if(cut) return cut->getTagStringDefault(".title",cut->GetTitle());
  return "";
}

bool TQPresenter::hasProcess(const TString& processName) const{
  // return true if a process exists
  return (bool)(this->getProcessConst(processName));
}

bool TQPresenter::hasCut(const TString& cutName) const{
  // return true if a process exists
  return (bool)(this->getCutConst(cutName));
}


const TQNamedTaggable* TQPresenter::getProcessConst(const TString& processName) const{
  // get a process
  TQTaggableIterator itr(this->fProcesses);
  while(itr.hasNext()){
    const TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(!TQStringUtils::matches(process->GetName(),processName)) continue;
    return process;
  }
  return NULL;
}

TQNamedTaggable* TQPresenter::getProcess(const TString& processName){
  // get a process
  TQTaggableIterator itr(this->fProcesses);
  while(itr.hasNext()){
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(!TQStringUtils::matches(process->GetName(),processName)) continue;
    return process;
  }
  return NULL;
}
 
TString TQPresenter::getProcessTitle(const TString& processName) const{
  // get the title of a process
  TQNamedTaggable* process = const_cast<TQNamedTaggable*>(this->getProcessConst(processName));
  if(process) return process->getTagStringDefault(".title",process->GetTitle());
  return "";
}

TString TQPresenter::getCutTags(const TString& cutName) const{
  // get the tags of a cut
  TQNamedTaggable* cut = const_cast<TQNamedTaggable*>(this->getCutConst(cutName));
  if(cut) return cut->exportTagsAsString();
  return "";
}

TString TQPresenter::getProcessTags(const TString& processName) const{
  // get the tags of a process
  TQNamedTaggable* process = const_cast<TQNamedTaggable*>(this->getProcessConst(processName));
  if(process) return process->exportTagsAsString();
  return "";
}

TCollection* TQPresenter::getListOfProcessNames() const{
  // get the list of process names
  TList * retval = new TList();
  TQTaggableIterator itr(this->fProcesses);
  while(itr.hasNext()){
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    retval->Add(new TObjString(process->getTagStringDefault(".name",process->GetName())));
  }
  return retval;
}

TCollection* TQPresenter::getListOfCutNames() const{
  // get the list of process names
  TList * retval = new TList();
  TQTaggableIterator itr(this->fCuts);
  while(itr.hasNext()){
    TQNamedTaggable* cut = itr.readNext();
    if(!cut) continue;
    retval->Add(new TObjString(cut->getTagStringDefault(".name",cut->GetName())));
  }
  return retval;
}

TString TQPresenter::getProcessPath(const TString& processName) const{
  // get the tags of a process
  TQTaggableIterator itr(this->fProcesses);
  while(itr.hasNext()){
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    if(TQStringUtils::matches(process->GetName(),processName))
      return process->getTagStringDefault(".path","");
  }
  return "";
}

bool TQPresenter::loadSystematics(const TString& path, const TString& id){
  // load a systematics file and save the contents under the given ID
  TString importPath(path+">>::"+id);
  return this->loadSystematics(importPath);
}

bool TQPresenter::loadSystematics(TQFolder* f){
  // add a systematics folder under the given ID
  this->fSystematics->addObject(f->copy());
  return true;
}

bool TQPresenter::loadSystematics(const TString& importPath){
  // load a systematics file and save the contents under the given ID
  bool retval = this->fSystematics->importObject(importPath);
  if(!retval) WARNclass("unable to load systematics folder from '%s'",importPath.Data());
  return retval;
}

TQFolder* TQPresenter::getSystematics(const TString& id){
	// obtain a systematics TQFolder by its ID string
  return this->fSystematics->getFolder(id);
}

int TQPresenter::importProcesses(const TQPresenter* other){
  // import processes from another instance
  int i=0;
  TQTaggableIterator itr(other->fProcesses);
  while(itr.hasNext()){
    TQNamedTaggable* tags = itr.readNext();
    this->fProcesses->Add(new TQNamedTaggable(tags));
    i++;
  }
  return i;
}


int TQPresenter::importCuts(const TQPresenter* other){
  // import processes from another instance
  int i=0;
  TQTaggableIterator itr(other->fCuts);
  while(itr.hasNext()){
    TQNamedTaggable* tags = itr.readNext();
    this->fCuts->Add(new TQNamedTaggable(tags));
    i++;
  }
  return i;
}

