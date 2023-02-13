#include "CAFxAODUtils/TQxAODskimmingAlgorithm.h"

#include <string>
#include <typeinfo>
#include <typeindex>
#include <stdio.h>

#include "TFile.h"
#include "TBranch.h"

#include "xAODRootAccess/TEvent.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODCutFlow/CutBookkeeperContainer.h"
#include "xAODCutFlow/CutBookkeeperAuxContainer.h"

#include "QFramework/TQIterator.h"
#include "QFramework/TQSampleFolder.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQToken.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQSampleVisitor.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQPathManager.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

ClassImp(TQxAODskimmingAlgorithm)

//__________________________________________________________________________________|___________
#define MAXFILENAMELENGTH 240 //just shy of 256

std::vector<std::string> TQxAODskimmingAlgorithm::getListOfCutBookkeeperNames() const {
  //hint: only call this function when you already have an event/tree/file token from fSample to prevent unnessecary file opening/closing and transientTree creations!
  std::vector<std::string> retval;
  if (!fSample) return retval;
  TQToken* tok = fSample->getFileToken();
  if (!tok) {
    ERRORfunc("Failed to obtain file token from sample '%s'",fSample->getPath().Data());
    return retval;
  }
  TFile* inFile = (TFile*)tok->getContent();
  if (!inFile) {
    ERRORfunc("Failed to obtain TFile from file token from sample '%s'", fSample->getPath().Data());
    fSample->returnToken(tok);
    return retval;
  }
  TTree* metaTree = (TTree*)inFile->Get("MetaData");
  if (!metaTree) {
    ERRORfunc("Failed to obtain MetaData tree from input file for sample '%s'", fSample->getPath().Data());
    fSample->returnToken(tok);
    return retval;
  }
  
  TQBranchIterator bitr(metaTree->GetListOfBranches(),false);
  while( bitr.hasNext() ) {
    TBranch* br = bitr.readNext();
    if (!br) continue;
    TClass* cl = 0; EDataType dt = kNumDataTypes;
    if (br->GetExpectedType(cl,dt)) {
      ERROR("Failed to get expected type for branch '%s'",br->GetName());
      continue;
    }
    if (!cl) continue;
    DEBUG("Found branch called '%s' with expected class type '%s",br->GetName(),cl?cl->GetName():"<null>");
    if (cl->GetTypeInfo() && (std::type_index(*cl->GetTypeInfo()) == std::type_index(typeid(xAOD::CutBookkeeperContainer)))) {
      DEBUG("Branch identifier as containing CutBookkeeperContainers");
      retval.push_back(br->GetName());
    }
  }
  //cleanup:
  fSample->returnToken(tok);
  return retval;
}


std::vector<TString> TQxAODskimmingAlgorithm::getCommonAmiTags(const std::vector<TString>& inputFileNames) const {
  std::vector<TString> firstTags; //tags from the first file, to be reduced later based on tags present in all files. We use a vector here to ensure the order stays the same
  std::set<TString> commonTags; //tags present in all files, will be reduces as input file names are parsed
  bool first=true;
  for (const TString& input : inputFileNames) {
    std::vector<TString> tags = TQStringUtils::extractMatches(input, fAmiTagRegexp);
    if (first) {
      firstTags = tags;
      for (const TString& t : firstTags) {
        commonTags.insert(t);
      }
      first=false;
    } else {
      bool found=true;
      for (const TString& c: commonTags) {
        found=false;
        for (const TString& t : tags) {
          if (c==t) {
            found=true;
            break;
          }
        }
        if (!found) {
          commonTags.erase(c);
        }
      }
    }
  }
  std::vector<TString> retVal;
  for (const TString& t : firstTags) {
    if (commonTags.count(t)>0) retVal.push_back(t);
  }
  return retVal;
}

TString TQxAODskimmingAlgorithm::getDSID(TQSampleFolder* sf) const {
  return sf->tagIsOfTypeString(".xAODskimming.treatAsSample")? sf->getTagStringDefault(".xAODskimming.treatAsSample","") : sf->getTagStringDefault("dsid", sf->getTagStringDefault("name" , sf->GetName() ) );
}

bool TQxAODskimmingAlgorithm::initializeSingleton(const std::vector<TQSample*>& samples){
  // initialize the algorithm
  if (samples.size()<1) {
    ERRORclass("Cannot initialize xAODslimmingAlgorithm with zero samples");
    return false;
  }
  TQSample* s = samples[0];  //TODO: implement additional checks if all samples seem consistent. For now we rely on the visitor to correctly select friend samples.
  
  if(!s) throw(std::runtime_error("received NULL sample"));
  if (s->hasSubSamples()) ERROR("Has the internal logic of the AnalysisVisitor changed? Initializing this algorithm on a sample with subsamples is not expected! Please inform qframework-users@cern.ch !");
  
  if (this->fSample == s) return true; //already initialized for this sample
  this->fSample = s;
  this->fEventToken = s->getEventToken();
  if(!this->fEventToken) {
    ERRORclass("Failed to obtain TEvent token!");
    return false;
  }
  this->fEvent = (xAOD::TEvent*)(this->fEventToken->getContent());
  if(!this->fEvent) {
    s->returnToken(fEventToken);
    return false;
  }
  
  TQSampleFolder* newSuperSample = nullptr;
  TQFolder* base = fSample->getBase();
  //@tag:[.xAODskimming.treatAsSample] Sample folders on which this tag is set are treated as if they were an instance of TQSample w.r.t. checking if this instance represents a "super sample" (i.e. a TQSample containing other TQSamples where only the contained instances of TQSample reffer to an actual input data source). This allows, for example, for merging data samples for which the TQSample instances corresponding to the input files are typically stored directly in a TQSampleFolder instead of within a super-sample like MC samples with multiple input files for a single DSID). The (string) value of this tag is used in place of the DSID (normally taken from the 'dsid' tag) as a seed for finding a common substring of the individual input files/samples name.
  if (base && ( base->InheritsFrom(TQSample::Class()) || ( base->InheritsFrom(TQSampleFolder::Class()) && base->hasTag(".xAODskimming.treatAsSample") ) ) ) {
    newSuperSample = static_cast<TQSampleFolder*>( base );
  }
  TString outFileName = "";
  if (! newSuperSample) { //there is no super sample, we got the simple case
    DEBUG("no super-sample found");
    fSuperSample = fSample; //our sample is the master sample itself 
    fNsubSamplesToProcess = 1;
    fNsubSamplesProcessed = 0;
    TString tmp = fSample->getFilename();
    tmp = TQFolder::getPathTail( tmp );
    // OLD naming scheme
    //outFileName = TQStringUtils::trim(TQFolder::getPathTail( tmp ));
    // NEW naming scheme
    std::vector<TString> amiTags = TQStringUtils::extractMatches(tmp, fAmiTagRegexp);
    
    outFileName = this->getDSID(fSample) + "_";
    outFileName += TQStringUtils::concatenateVector(amiTags,"_");
  } else if ( !fSuperSample || !fSuperSample->isFriend(newSuperSample) ) { //we don't have a super sample set yet. We need to figure out how many sub samples will be visited
    DEBUGclass("Found new super-sample, reason: %s",!fSuperSample?"no super-sample existing":"new super-sample is not a friend of the existing one!");
    fSuperSample = newSuperSample;
    
    // the user might have restricted the analysis run to a subset of the full sample. In order to know after how many sub-samples we need to close the output file, we need to keep track of how many we have already processed/will process
    //get number of sub samples that will be visited in this run
    bool requireSelectionTag = TQSampleVisitor::checkRestrictionTag(fSample); //are we running a full analysis in one go or are we restricted to a subset of 
    TQSampleIterator itr(fSuperSample->getListOfSamples("?"),true); //<- is this correct? does this indicate ownership of the list or also its contents???
    fNsubSamplesToProcess = 0;
    fNsubSamplesProcessed = 0;
    std::vector<TString> inFileNames;
    while(itr.hasNext()) { 
      TQSample* sub = itr.readNext();
      if (!requireSelectionTag || TQSampleVisitor::checkRestrictionTag(sub)) {
        ++fNsubSamplesToProcess;
        TString tmp = sub->getFilename();
        if (tmp.Length()<1) continue;
        inFileNames.push_back( TQFolder::getPathTail( tmp ) );
      }
    }
    
    // seed priority: dsid (from tag) > name (from tag) > name (GetName())    
    // NEW: constructed file name
    std::vector<TString> amiTags = this->getCommonAmiTags(inFileNames);
    outFileName = this->getDSID(fSuperSample) +"_";
    outFileName += TQStringUtils::concatenateVector(amiTags,"_");
    // OLD: based on common substring
    //outFileName = TQStringUtils::getLongestCommonSubstring( inFileNames , fSuperSample->tagIsOfTypeString(".xAODskimming.treatAsSample")? fSuperSample->getTagStringDefault(".xAODskimming.treatAsSample","") : fSuperSample->getTagStringDefault("dsid", fSuperSample->getTagStringDefault("name" , fSuperSample->GetName() ) ) );
    
  } else if (newSuperSample == fSuperSample) {
    DEBUGclass("re-using existing super-sample");
    // we're still in the same sample, there shouldn't be much to do
    //return true;
  }
  if (outFileName.Length()>0) {
    //if we don't have an open output file yet, we construct the full path and try to open it
    TString filePrefix = TString("CAFxAOD_") + fFilePrefix;
    
    //if we exceed a certain threshold we need to attempt to shorten the file name
    if (outFileName.Length() > MAXFILENAMELENGTH - 5/*= length of ".root"*/ - filePrefix.Length()) { //only consider the auto-created part of the file name, the user supplied parts and the "CAFxAOD" prefix should stay untouched
      TString buffer;      
      outFileName = TQStringUtils::removeDuplicateSubStrings(outFileName,buffer,16);
      WARNclass("The contructed output file name was too long. All but the first occurance of the substring '%s' have been removed in an attempt to shorten it!",buffer.Data());
    }
    outFileName = filePrefix + outFileName;
    outFileName = TQFolder::concatPaths(fBasePath, outFileName);
    //@env: [CAF_SKIM_OUTPUT_FILE] if set to a non-empty string the specified file name
    //@env  is used for the output file of the xAOD skimming algorithm. This also implies 
    //@env  that the skimming will switch to a single-DSID mode and will throw an error
    //@env  if a second DSID is encountered in the job.
    std::string forcedFileName = TQUtils::readEnvVarValue("CAF_SKIM_OUTPUT_FILE");
    if (forcedFileName.size()>0) {
      if (this->fFinishedFirstDSID) { //we already finished one DSID/output file. Complain loudly and abort!
        throw std::runtime_error("TQxAODskimmingAlgorithm is running in single DSID mode but it seems that more than one DSID and/or output file is attempted to be processed. Aborting! (if single DSID mode is not desired, unset the CAF_SKIM_OUTPUT_FILE environment variable)");
      }
      outFileName = forcedFileName.c_str();
      this->fSingleDSIDmode = true;
    }
    
    TQStringUtils::ensureTrailingText(outFileName,".root");
    outFileName =  TQPathManager::getPathManager()->getLocalPath(outFileName);
    TQUtils::ensureDirectoryForFile(outFileName);
    
    if (fOutfile) {
      //there is still an output file.. this shouldn't happen!
      throw std::runtime_error("Trying to create a new output file while still having an old one open!");
      return false;
    }
    //TODO: can we improve this (or the file name) a bit, such that we can have multiple output files (as in produced by multiple jobs) for one DSID? -> cleanest solution is management by user, e.g., passing individual prefix (fragment) during submission.
    fOutfile = TFile::Open(outFileName,"RECREATE", "", 105); //105 is the compression setting. Default somehow seems to be 101 but it creates terrible performance (should be less compressed = better CPU wise performance but somehow shows the opposite in file size and CPU usage comapred to the expectation...)
    if (!fOutfile || fOutfile->IsZombie()) {
      throw std::runtime_error(TString::Format("[TQxAODskimmingAlgorithm] Failed to open/create output file with name '%s'",outFileName.Data()).Data());
    }
    if (!fEvent->writeTo(fOutfile).isSuccess()) {
      throw std::runtime_error(TString::Format("Failed to write to output file '%s'",outFileName.Data()).Data());
      return false;
    }
    
    fEvent->setAuxItemList("EventInfoAux.",std::string("-")+this->fFlagName.Data());
  }
  
  //Take care of CutBookkeepers: 
  std::vector<std::string> cbkNames = getListOfCutBookkeeperNames();
  for (const std::string& name : cbkNames) {
    xAOD::CutBookkeeperContainer* outCbk = nullptr;
    const xAOD::CutBookkeeperContainer* inCbk = nullptr;
    
    if ( ! fEvent->retrieveMetaInput(inCbk,name).isSuccess() ) {
      WARNclass("Failed to obtain CutBookkeeperContainer named '%s' from input file",name.c_str());
      continue; //if we didn't get a Cbk we can neither add it to the output nor merge it with an already existing one in the output
    }

    int oldErrorIgnoreLevel = gErrorIgnoreLevel;
    gErrorIgnoreLevel = 2000; //silence errors (we sort of expect them)
    if ( fEvent->retrieveMetaOutput<xAOD::CutBookkeeperContainer>(outCbk,name).isSuccess() ) { //we got a matching Cbk in the output file, let's merge with the new one.
      outCbk->merge(inCbk);
    } else {
      xAOD::CutBookkeeperContainer* inCopy = new xAOD::CutBookkeeperContainer();
      xAOD::CutBookkeeperAuxContainer* inCopyAux = new xAOD::CutBookkeeperAuxContainer();
      inCopy->setStore(inCopyAux);
      if (!fEvent->recordMeta<xAOD::CutBookkeeperContainer>(inCopy,name).isSuccess()) {
        throw std::runtime_error(TString::Format("Failed to record CutBookkeeperContainer '%s'",name.c_str()).Data());
        return false;
      }
      if (!fEvent->recordMeta<xAOD::CutBookkeeperAuxContainer>(inCopyAux,name+"Aux.").isSuccess()) {
        throw std::runtime_error(TString::Format("Failed to record CutBookkeeperAuxContainer '%sAux.'",name.c_str()).Data());
        return false;
      }
      inCopy->merge(inCbk);
    }
    gErrorIgnoreLevel = oldErrorIgnoreLevel;
  }
  
  return true;
}

//__________________________________________________________________________________|___________

bool TQxAODskimmingAlgorithm::finalize(){
  // finalize the algorithm
  if (!this->fSample) return true; //we don't have a sample (anymore), just smile and wave
  if (!fOutfile) { throw std::runtime_error("Failed to finalize TQxAODskimmingAlgorithm: not output file present"); return false; }
  
  ++fNsubSamplesProcessed; //we're done with another sample
  if ( fNsubSamplesProcessed >= fNsubSamplesToProcess ) { //we reached the end of the last input file
    DEBUGclass("Closing output file '%s'",fOutfile->GetName() );
    TString outfileName = fOutfile->GetName();
    if (!fEvent->finishWritingTo(fOutfile).isSuccess()) {
      throw std::runtime_error(TString::Format("Failed to finalize file '%s'!",outfileName.Data()).Data());
      return false;
    }
    fOutfile->Close();
    if (this->fSingleDSIDmode) {
      this->fFinishedFirstDSID = true; //remember that we have finished a file. In single DSID mode this triggers an error if another one is attempted to be opened
    }
    delete fOutfile;
    fOutfile = nullptr;
    //check if the file is readable
    TFile* checkFile = TFile::Open(outfileName);
    if (!checkFile || !checkFile->IsOpen() || checkFile->IsZombie()) {
      ERRORclass("Failed to validate output file '%s', either it could not be read back or it is corrupted. Will attempt to rename the file by prepending 'broken_' indicating its broken state but still allow for investigation.");
      TString outfileName_ = outfileName; //make a copy
      TString tail = TQFolder::getPathTail(outfileName);
      tail.Prepend("broken_");
      if ( rename(outfileName.Data(), TQFolder::concatPaths(outfileName_,tail).Data()) != 0 ) { //rename(old,new) returns 0 on success
        ERRORclass("Failed to rename file, will now attempt deleteing it!");
        if ( remove(outfileName.Data()) != 0 ) {
          ERRORclass("Removal failed as well, please investigate manually!");
        }
      }
      throw std::runtime_error("Validity check for output xAOD file has failed!");
      return false;
    } else {
      checkFile->Close();
      delete checkFile;
    }
    fSuperSample = nullptr;
    fNsubSamplesProcessed = 0;
    fNsubSamplesToProcess = 0;
  }
  
  this->fSample->returnToken(this->fEventToken);
  this->fEventToken = NULL;
  this->fEvent = NULL;
  this->fSample = NULL;
  return true;
}

//__________________________________________________________________________________|___________

bool TQxAODskimmingAlgorithm::cleanup(){
  // cleanup the cache of the algorithm after an event (after all cuts/analysisJobs/... are executed)
  const xAOD::EventInfo* eventInfo = NULL;
  if(this->fEvent->retrieve(eventInfo, "EventInfo").isFailure()){
    throw std::runtime_error("unable to retrieve event info from TEvent");
    return false;
  }
  if (eventInfo->isAvailable<char>(this->fFlagName.Data()) && eventInfo->auxdecor<char>(this->fFlagName.Data()) ) {
    
    this->fEvent->copy().ignore();
    this->fEvent->fill();
  }
  

  return true;
}

//__________________________________________________________________________________|___________

bool TQxAODskimmingAlgorithm::execute(){
  // execute this algorithm on an event (called before any cut)
  return true;
}

//__________________________________________________________________________________|___________


//__________________________________________________________________________________|___________



