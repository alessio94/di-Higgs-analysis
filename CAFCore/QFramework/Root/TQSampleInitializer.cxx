#include "QFramework/TQSampleInitializer.h"
#include "QFramework/TQSample.h"
#include "TFile.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQIterator.h"

#include "definitions.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQSampleInitializer:
//
// A simple sample initializer, capable of traversing a given sample folder structure,
// locating the nTuples in the form of ROOT files an TTrees contained therein.
// 
// This simple initializer variant has two means of determining the correct file path:
// - the ".xsp.filepath" tag which might be set on the individual sample
// - the file system directory announced to the initializer via readDirectory
// this variant will use the sample name in conjuction with the
// "filenamePrefix" and "filenameSuffix" tags to find the sample root file
//
// Additionally, the tree name is automatically determined with the following method
// - the file is opened and any objects inheriting from TTree are investigated
// - their names are compared to the tag "treeName" (default: "*" unless specified differently
//     by the tag with key "treeName" set on the initializer)
// - if there is exactly one match, the initializer proceeds.
// - in the case of zero or multiple matches, it will terminate with an error.
//
// After the tree name has been determined, the number of events is retrieved from
// the tree. In conjunction with the luminosity (can be set as "luminosity" tag on any
// parent folder of the sample or the sample itself, as well as on the initializer)
// and the tags "kfactor" and "filtereff" on the sample, it is used to set the sample
// normalization.
// 
////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(TQSampleInitializer)

TQSampleInitializer::TQSampleInitializer() :
TQSampleVisitor("init")
{
  // default constructor
}

TQSampleInitializer::TQSampleInitializer(const TString& dirname, int depth) :
TQSampleVisitor("init")
{
  this->readDirectory(dirname,depth);
}

TQSampleInitializer::~TQSampleInitializer(){
  // default destructor
}

namespace {
  inline TString makeFullPath(const TString& prefix, const TString& path){
    TString tmppath = TQStringUtils::makeASCII(path);
    int hashpos = tmppath.First("#");
    if(hashpos >= 0 && hashpos < tmppath.Length()) tmppath.Remove(hashpos);
    TString basepath(TQStringUtils::makeASCII(prefix));
    TQStringUtils::ensureLeadingText(tmppath,basepath);
    return tmppath;
  }
}

int TQSampleInitializer::visitSample(TQSample * sample, TString& message) {
  int retCode = this->visitSampleInternal(sample, message);
  if (retCode == visitOK) {
    //apply timestamp of initialization and traceID to this sample (important for merging!)
    this->stamp(sample);
  }
  return retCode;
}

int TQSampleInitializer::visitSampleInternal(TQSample * sample, TString& message){
  // visit and initialize the given sample
  if(sample->isSubSample()) return visitOK;
  if(sample->getTagBoolDefault("~isInitialized",false)) return visitSKIPPEDOK;
  
  TString sampleName = sample->GetName();
  sample->getTagString("name",sampleName);
  sample->getTagString(".xsp.sampleName",sampleName);
  TString samplePath = sample->getPath();
  
  /* get the best filepath */
  TString filename = this->getTagStringDefault("filenamePrefix","") + sampleName + this->getTagStringDefault("filenameSuffix",".root");
  TString subdir = this->getTagStringDefault("subdirectory","./");
  
  TString path;
  TString fullpath;
  TString fpattern;
  if(sample->getTagString(".xsp.filepath",path)){
    // the path is already given by the XSectionParser
    // there is not much to do, just visit the sample to make sure that it's readable and contains the right tree
    fullpath = sample->replaceInText(path);
    fullpath = TQStringUtils::makeASCII(fullpath);
    if(!this->initializeSample(sample,fullpath,message)) {
      if (getExitOnFail()) exit(66); 
      return visitFAILED;
    }
    this->setSampleNormalization(sample);
    return visitOK;
  } else if(this->fPaths){
    // no given absolute path, but a set of paths on local file system or EOS
    // this is the default case
    DEBUGclass("trying to retrieve object paths for filename '%s' from subdir '%s'",filename.Data(),subdir.Data());
    //this->fPaths->print("rdt");
    //the first lever of folders represents different storage systems (for hybrid reading, e.g., partially from a remote storage/server and some mounted offline storage)
    TList* paths = new TList();
    paths->SetOwner(true);
    TQFolderIterator storageSystems(this->fPaths->getListOfFolders("?"),true); //first level of subfolders represents different file systems/sources (to not mix up local and remote file access)
    while (storageSystems.hasNext()) {
      TQFolder* storageSys = storageSystems.readNext();
      storageSys->detachFromBase(); //temporarily detach it from the base folder to prevent the name of this folder to appear in the paths obtained below
      DEBUG("scanning storage system '%s'",storageSys->GetName());
      TList* thesePaths = storageSys->getObjectPaths(filename,subdir,TObjString::Class());
      DEBUG("found %d paths matching '%s' in '%s'",thesePaths->GetEntries(),filename.Data(),subdir.Data());
      this->fPaths->addFolder(storageSys);
      TQStringIterator pathItr(thesePaths,false); //don't delete the list after iteration
      while (pathItr.hasNext()) {
        TObjString* p = pathItr.readNext();
        if (!p) continue;
        //modify the name into a full name making use of the basepath set for this particular storageSys
        TString s = makeFullPath(storageSys->getTagStringDefault("basepath",""),p->GetName());
        DEBUGclass("found path %s",s.Data());
        p->SetString( s );
      }
      //thesePaths now contains the modified paths (after makeFullPath)
      thesePaths->SetOwner(false); //we'll add these objects to another list which should take over ownership
      DEBUG("adding %d paths",thesePaths->GetEntries());
      paths->AddAll(thesePaths); //add all the (full) paths to the main list
      delete thesePaths; //delete the (local) helper list
    }
    DEBUGclass("trying to initialize sample '%s' from list of length %d",sample->getPath().Data(),paths->GetEntries());
    paths->SetOwner(true);
    if(!paths || (paths->GetEntries() == 0)){
      // there is no path at all
      // emit an error message
      message = "no valid file path given for '"+filename+"'";
      #ifdef _DEBUG_
      this->fPaths->print("rdt");
      #endif
      if(paths) delete paths;
      paths = nullptr;
      return visitWARN;
    } 
    if(paths->GetEntries() == 1){
      DEBUGclass("found exactly one entry for file '%s' in '%s'",filename.Data(),subdir.Data());
      // there is exactly one path
      // do the usual thing
      //fullpath = makeFullPath(this->fPaths->getTagStringDefault("basepath","."),paths->First()->GetName());
      fullpath = paths->First()->GetName();
      delete paths;
      paths = nullptr;
      if(!this->initializeSample(sample,fullpath,message)) { 
        if (getExitOnFail()) exit(66); 
        return visitFAILED;
      }
      this->setSampleNormalization(sample,1.);
      return visitOK;
    }
    if(paths->GetEntries() > 0){
      // we have more than one matching path
      // this is experimental sample-splitting 
      // use with caution and beware of errors
      DEBUGclass("found %d entries for file '%s' in '%s'",paths->GetEntries(),filename.Data(),subdir.Data());
      TQIterator itr(paths,true); //iterator takes ownership of 'paths' list here
      int nOK = 0;
      double neventstotal = 0;
      while(itr.hasNext()){
	//fullpath = makeFullPath(this->fPaths->getTagStringDefault("basepath","."),itr.readNext()->GetName());
	      fullpath = itr.readNext()->GetName();
        TString tmppath(fullpath);
        DEBUGclass("adding %s",fullpath.Data());
        TQSample* subSample = new TQSample(TQFolder::makeValidIdentifier(TQFolder::getPathTail(tmppath)));
        if(!sample->addSubSample(subSample)){
          if(sample->getObject(subSample->GetName())){
            message = TString::Format("unable to add subsample '%s', an object of this name exists already", subSample->GetName());
          } else {
            message = TString::Format("unable to add subsample '%s'", subSample->GetName());
          }
          DEBUGclass("error: %s",message.Data());
          delete subSample;
          return visitFAILED;
        } 
        TString subMessage;
        if(this->initializeSample(subSample,fullpath,subMessage)){
          DEBUGclass("initialized %s: %s",fullpath.Data(),subMessage.Data());
          nOK++;
          neventstotal += subSample->getTagDoubleDefault(".init.sumOfWeights",0);
          subSample->setTagString(TString::Format(".%s.message",this->GetName()), TQStringUtils::compactify(subMessage));
        } 
      }
      TQSampleIterator sitr(sample->getListOfFolders("?",TQSample::Class()),true);
      while(sitr.hasNext()){
        TQSample* s = sitr.readNext();
        this->setSampleNormalization(s,s->getTagDoubleDefault(".init.sumOfWeights",0)/neventstotal);
      }
      sample->setTagDouble(".init.sumOfWeightsTotal",neventstotal);
      if(nOK == paths->GetEntries()){
        message = TString::Format("multisample n=%d: all OK",nOK);
        return visitOK;
      } else if(nOK == 0){
        message = TString::Format("multisample n=%d: all FAILED",paths->GetEntries());
        if (getExitOnFail()) exit(66);
        return visitFAILED;
      } else {
        message = TString::Format("multisample n=%d: %d succeeded, %d failed",paths->GetEntries(),nOK,(paths->GetEntries()-nOK));
        if (getExitOnFail()) exit(66); 
        return visitFAILED;
      }
    }
  } else if(this->getTagString("dCacheFilePattern",fpattern)){
    // path is on dCache
    // highly experimental
    TList* l = TQUtils::lsdCache(sample->replaceInText(fpattern),TQLibrary::getLocalGroupDisk(), TQLibrary::getDQ2PathHead(), TQLibrary::getdCachePathHead(), TQLibrary::getDQ2cmd());
    if(l->GetEntries() == 0){
      message = "no such dataset";
      if (getExitOnFail()) exit(66); 
      return visitFAILED;
    } else if(l->GetEntries() == 1){
      TObjString* s = dynamic_cast<TObjString*>(l->First());
      fullpath = TQStringUtils::makeASCII(s->GetName());
      if(!this->initializeSample(sample,fullpath,message)) {
	if (getExitOnFail()) exit(66); 
	return visitFAILED;
      }
      return visitOK;
    } else {
      TQStringIterator itr(l,true);
      while(itr.hasNext()){
        TObjString* s = itr.readNext();
        fullpath = TQStringUtils::makeASCII(s->GetName());
        TQSample* subSample = sample->addSelfAsSubSample(fullpath);
        if(!this->initializeSample(subSample,fullpath,message)) {
	  if (getExitOnFail()) exit(66); 
	  return visitFAILED;
	}
      }
      return visitOK;
    }
  } else {
    message = "no known paths";
    if (getExitOnFail()) exit(66); 
    return visitFAILED;
  }
  
  return visitWARN;
}


bool TQSampleInitializer::getExitOnFail() {
  return this->getTagBoolDefault("exitOnFail",false);
}

void TQSampleInitializer::setExitOnFail(bool ex) {
  this->setTagBool("exitOnFail",ex);
}