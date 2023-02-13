#include "QFramework/TQSampleVisitor.h"
#include "QFramework/TQSampleFolder.h"
#include "QFramework/TQFolder.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQToken.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQStringUtils.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQSampleVisitor:
//
// Base class for other classes that visit samples or sample folders, most notably
//   * TQSampleInitializer
//   * TQAnalysisSampleVisitor
//   * TQMultiChannelAnalysisSampleVisitor
//
////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(TQSampleVisitor)

TString TQSampleVisitor::statusSKIPPED(bool pretty){ if(pretty) return  TQStringUtils::makeBoldWhite("[ ")+TQStringUtils::makeBoldPink  ("SKIP")+TQStringUtils::makeBoldWhite(" ]"); else return "[ SKIP ]";}
TString TQSampleVisitor::statusOK(bool pretty){ if(pretty) return       TQStringUtils::makeBoldWhite("[ ")+TQStringUtils::makeBoldGreen (" OK ")+TQStringUtils::makeBoldWhite(" ]"); else return "[  OK  ]";}
TString TQSampleVisitor::statusFAILED(bool pretty){ if(pretty) return   TQStringUtils::makeBoldWhite("[ ")+TQStringUtils::makeBoldRed   ("FAIL")+TQStringUtils::makeBoldWhite(" ]"); else return "[ FAIL ]";}
TString TQSampleVisitor::statusWARN(bool pretty){ if(pretty) return     TQStringUtils::makeBoldWhite("[ ")+TQStringUtils::makeBoldYellow("WARN")+TQStringUtils::makeBoldWhite(" ]"); else return "[ WARN ]";}
TString TQSampleVisitor::statusSKIPPEDOK(bool pretty){ if(pretty) return  TQStringUtils::makeBoldWhite("[ ")+TQStringUtils::makeBoldGreen  ("SKIP")+TQStringUtils::makeBoldWhite(" ]"); else return "[ SKIP ]";}
TString TQSampleVisitor::statusRUNNING(bool pretty){ if(pretty) return  TQStringUtils::makeBoldWhite("[ ")+TQStringUtils::makeBoldWhite ("....")+TQStringUtils::makeBoldWhite(" ]"); else return "[ .... ]";}
TString TQSampleVisitor::statusPROGRESS(bool pretty, double fraction) { if(pretty) return TQStringUtils::makeBoldWhite("[ ")+TQStringUtils::makeBoldWhite (TQStringUtils::fixedWidth(TString::Format("%.0f%%",fraction*100.),4,'c') )+TQStringUtils::makeBoldWhite(" ]"); else return TString("[ ")+ (TQStringUtils::fixedWidth(TString::Format("%.0f%%",fraction*100.),4,'c') )+ (" ]");} // %% is escaped '%' character

//__________________________________________________________________________________|___________

TQSampleVisitor::TQSampleVisitor(const TString& name) :
  TNamed(name,name),
  fSampleColWidth(0.45*TQLibrary::getConsoleWidth()),
  fVerbose(false),
  fPrettyPrint(true),
  fUpdateLines(true)
{
  fVisitTraceID.Clear();
}


//__________________________________________________________________________________|___________

void TQSampleVisitor::setVerbose(bool verbose) {
  fVerbose = verbose;
}

//__________________________________________________________________________________|___________

void TQSampleVisitor::setPrettyPrint(bool pretty) {
  fPrettyPrint = pretty;
}

//__________________________________________________________________________________|___________

void TQSampleVisitor::setLineUpdates(bool allow) {
  fUpdateLines = allow;
}

//__________________________________________________________________________________|___________

bool TQSampleVisitor::setVisitTraceID(TString id) {

  if (id.IsNull()) {
    fVisitTraceID.Clear();
    return true;
  } else if (TQSampleFolder::isValidName(id)) {
    fVisitTraceID = id;
    return true;
  } else {
    return false;
  }
}


//__________________________________________________________________________________|___________

TString TQSampleVisitor::getVisitTraceID() const {
  return fVisitTraceID;
}

//__________________________________________________________________________________|___________

const char* TQSampleVisitor::getVisitTraceIDConst() const {
  return fVisitTraceID.Data();
}

//__________________________________________________________________________________|___________

TString TQSampleVisitor::getStatusString(int status, bool pretty, double progress) {
  if (status == visitSKIPPED)   return TQSampleVisitor::statusSKIPPED(pretty);
  if (status == visitOK)        return TQSampleVisitor::statusOK(pretty);
  if (status == visitFAILED)    return TQSampleVisitor::statusFAILED(pretty);
  if (status == visitWARN)      return TQSampleVisitor::statusWARN(pretty);
  if (status == visitSKIPPEDOK) return TQSampleVisitor::statusSKIPPEDOK(pretty);
  if (status == visitPROGRESS)  return TQSampleVisitor::statusPROGRESS(pretty,progress);
  return "";
}



//__________________________________________________________________________________|___________

bool TQSampleVisitor::callInitialize(TQSampleFolder * sampleFolder) {
  // call initialization of the sample visitor
  if (fVerbose) {
    int width = 50;

    std::cout << std::endl;
    std::cout << TQStringUtils::repeat('-', width) << std::endl;
    if(fPrettyPrint) std::cout << TQStringUtils::makeBoldWhite("Initializing...") << std::endl;
    else std::cout << "Initializing..." << std::endl;
    std::cout << TString::Format("%-20s: %s", "Visitor", ClassName()) << std::endl;
    std::cout << TString::Format("%-20s: %s", "Sample Folder",
                            sampleFolder->getPath().Data()) << std::endl;
    std::cout << TQStringUtils::repeat('-', width) << std::endl;

    std::cout << std::endl << std::endl;
  }

  TString message;
  int result = initialize(sampleFolder, message);

  if (fVerbose && fPrettyPrint) {
    std::cout << TQStringUtils::makeBoldWhite(TQStringUtils::fixedWidth("Sample",fSampleColWidth));
    std::cout << TQStringUtils::makeBoldWhite(TQStringUtils::fixedWidth("Status",8,"c"));
    std::cout << " ";
    std::cout << TQStringUtils::makeBoldWhite(message);
    std::cout << std::endl;
    std::cout << TQStringUtils::makeBoldWhite(TQStringUtils::repeat("=", TQLibrary::getConsoleWidth())) << std::endl;
  } else if(fVerbose){
    std::cout << (TQStringUtils::fixedWidth("Sample",fSampleColWidth));
    std::cout << (TQStringUtils::fixedWidth("Status",8,"c"));
    std::cout << " ";
    std::cout << (message);
    std::cout << std::endl;
    std::cout << (TQStringUtils::repeat("=", TQLibrary::getConsoleWidth())) << std::endl;
  }

  /* consider initialisation succeful if result was OK or WARN */
  return (result != visitFAILED);

}


//__________________________________________________________________________________|___________

int TQSampleVisitor::initialize(TQSampleFolder * /*sampleFolder*/, TString& message) {
  message.Append(TQStringUtils::fixedWidth("Info", 30));
  return visitOK;

}

//__________________________________________________________________________________|___________

TString TQSampleVisitor::printLine(TQSampleFolder* f, int level, bool isSample, const TString& bullet){
  // set the listing bullet ("+" for first visit, "-" for revisit)
  TString line = TString::Format("%*s%s ", level*2, "", bullet.Data());

  if (isSample) {
    line += f->getNameConst();
  } else {
    if(fPrettyPrint){
      line += TQStringUtils::makeBoldBlue(f->getNameConst());
    } else {
      line += (f->getNameConst());
    }
  }

  if(fUpdateLines || !isSample){
    std::cout << TQStringUtils::fixedWidth(line, fSampleColWidth, "l");

    if(isSample){
      std::cout << TQSampleVisitor::statusRUNNING(fPrettyPrint);
      std::cout << "\r";
      std::cout.flush();
    } else {
      std::cout << std::endl;
    }
  }
  return line;
}

//__________________________________________________________________________________|___________

void TQSampleVisitor::updateLine(const TString& line, const TString& message, int result, bool ignore, double progress){
  /* update status button */
  if(ignore){
    if (result != visitPROGRESS) std::cout << std::endl; //don't end this line for ignored progress indications
    return;
  } else {
    if (result == visitSKIPPED || result == visitOK || result == visitWARN || result == visitFAILED || result == visitSKIPPEDOK) {
      std::cout << TQStringUtils::fixedWidth(line, fSampleColWidth, "l") << this->getStatusString(result,fPrettyPrint) << " " << TQStringUtils::rtrim(message) << std::endl;
    } else if (result == visitLISTONLY) {
      std::cout << TQStringUtils::fixedWidth(line, fSampleColWidth, "l") << std::endl;
    } else if (result == visitIGNORE) {
      std::cout << TQStringUtils::fixedWidth(line, fSampleColWidth, "l") << std::endl;
    } else if (fUpdateLines && result == visitPROGRESS) {
      std::cout << TQStringUtils::fixedWidth(line, fSampleColWidth, "l") << this->getStatusString(result,fPrettyPrint,progress) << " " << message << "\r";
      std::cout.flush();
    }
  }
}



//__________________________________________________________________________________|___________

int TQSampleVisitor::visit(TQSampleFolder * sampleFolder, bool requireSelectionTag) {
  // visit the sample folder pointed to
	int nVisits = 0;
	try {
		this->callInitialize(sampleFolder);
		nVisits = this->callVisit(sampleFolder,0,requireSelectionTag);
		this->callFinalize();
	} catch (const std::bad_alloc& oom){
		Long64_t vsize = TQLibrary::getVirtualMemory();
		TQLibrary::recordMemory();
		if(vsize > 0){
		  //		  double mem = (double)vsize / 1024./1024.;
		  ERRORclass("machine ran out of memory while visiting sample folder '%s', current memory usage is %.3f MB",sampleFolder->getPath().Data(),vsize);
		} else {
		  ERRORclass("machine ran out of memory while visiting sample folder '%s' and unable to retrieve current memory usage (sorry!)",sampleFolder->getPath().Data());
		}
		nVisits=-1;
	}
  return nVisits;
}

//__________________________________________________________________________________|___________

void TQSampleVisitor::leaveTrace(TQSampleFolder* sampleFolder, TString prefix, int result, const TString& message){
  // leave a trace on the given sample folder
  TString resultStr = this->getStatusString(result,false);
  prefix.Append(this->getVisitTraceIDConst());
  if(!message.IsNull()){
    DEBUGclass("writing message");
    sampleFolder->setTagString(prefix+".message", TQStringUtils::compactify(message));
  }
  sampleFolder->setTagInteger(prefix + ".statusID", result);
  if(!resultStr.IsNull())
    sampleFolder->setTagString(prefix + ".status", resultStr);
}

//__________________________________________________________________________________|___________

int TQSampleVisitor::getStatusID(TQSampleFolder* sampleFolder, TString prefix){
  // read statusID of the given sample folder
  prefix.Append(this->getVisitTraceIDConst());
  return sampleFolder->getTagIntegerDefault(prefix + ".statusID", -1);
}

//__________________________________________________________________________________|___________

bool TQSampleVisitor::checkRestrictionTag(TQSampleFolder* sf) {
  // test if upwards or downwards of the sample folder a restriction tag has been
  // set to select this path to be processed in the current run.
  if (!sf) return false;
  //search up and downwards for a selection tag
  return (sf->getTagBoolDefault(TString("~")+TQSampleFolder::restrictSelectionTagName, false) ||
           sf->getTagBoolDefault(TQSampleFolder::restrictSelectionTagName+TString("~"), false));
}

int TQSampleVisitor::callVisit(TQSampleFolder * sampleFolder, int level, bool requireSelectionTag) {
  // Returns the number of sample folders visited.

  /* skip this visit of sample folder is invalid */
  if (!sampleFolder) return 0;

  if (requireSelectionTag) { //if we should only visit explicitly marked TQSampleFolders we need to check for the presence of the corresponding tag somewhere up/down the folder hierarchy
    DEBUGclass("Checking selection tag requirement for TQSampleFolder '%s'",sampleFolder->GetName());
    if (! TQSampleVisitor::checkRestrictionTag(sampleFolder) ) {
      DEBUGclass("Skipping sample folder '%s' as it doesn't seem to be selected",sampleFolder->GetName());
      return 0; //we ignore this folder as it is not selected
    }
  }

  /* check if we visit a sample or a sample folder */
  TQSample* sample = dynamic_cast<TQSample*>(sampleFolder);
  bool isSample = (bool)(sample) && (!sample->hasSubSamples());

  //Debugging option
  if (sampleFolder->hasTagString("asv.initialize.dumpTopTo")) {
    TQUtils::dumpTop(sampleFolder->getTagStringDefault("asv.initialize.dumpTopTo",".") , TString::Format("%ld_initialize_%s",TQUtils::getCurrentTime(),sampleFolder->getName().Data()) , TString::Format("%s\n-----------------------------\n",sampleFolder->getPath().Data()));
  }
  TQLibrary::recordMemory();

  /* print details */
  //TString line;
  int result;
  int nVisits = 0;
  TString message;
  /* decide whether to visit as sample or as folder */
  if (isSample) {
     /* visit as sample */
      DEBUGclass("visiting as sample");
      if (fVerbose) fStatusLine = this->printLine(sampleFolder,level,true,"+");
      result = visitSample(sample, message);
      nVisits += (result == visitOK);
      this->leaveTrace(sampleFolder,".sv.visit.",result,message);
      if (fVerbose) this->updateLine(fStatusLine,message,result);
      DEBUGclass("revisiting as sample");
      result = revisitSample(sample, message);
      this->leaveTrace(sampleFolder,".sv.revisit.",result,message);
  } else {
    /* visit as folder */
    DEBUGclass("visiting as folder");
    if (fVerbose) fStatusLine = this->printLine(sampleFolder,level,false,"+");
    result = visitFolder(sampleFolder, message);
    this->leaveTrace(sampleFolder,".sv.visit.",result,message);
    /* next: visit all sub sample folders (if not disabled) */
    if(result != visitFAILED && result != visitSKIPPED && result != visitSKIPPEDOK){
      TQSampleFolderIterator itr(sampleFolder->getListOfSampleFolders("?"),true);
      while (itr.hasNext()) {
        // the next element (might be a sample folder)
        TQSampleFolder * element = itr.readNext();
        // consider only TQSampleFolders
        if (element){
          // visit the TQSampleFolder recursively
          nVisits += this->callVisit(element, level + 1, requireSelectionTag);
        }
      }
    }
    DEBUGclass("revisiting as folder");
    if (fVerbose) fStatusLine = this->printLine(sampleFolder,level,false,"-");
    result = revisitFolder(sampleFolder, message);
    this->leaveTrace(sampleFolder,".sv.revisit.",result,message);
  }
  if (sampleFolder->hasTagString("sv.finalize.dumpTopTo")) {
    TQUtils::dumpTop(sampleFolder->getTagStringDefault("sv.finalize.dumpTopTo",".") , TString::Format("%ld_finalize_%s",TQUtils::getCurrentTime(),sampleFolder->getName().Data()) , TString::Format("%s\n-----------------------------\n",sampleFolder->getPath().Data()));
  }
  TQLibrary::recordMemory();

  return (result == visitOK || result == visitWARN || result == visitIGNORE || result == visitLISTONLY) ? nVisits : 0;
}


//__________________________________________________________________________________|___________

int TQSampleVisitor::visitFolder(TQSampleFolder * sampleFolder, TString& /*message*/) {
  (void)sampleFolder; //silence compiler warning (unused parameter)
  DEBUGclass("Visiting folder '%s'", sampleFolder? sampleFolder->getPath().Data() : "");
  return visitLISTONLY;
}


//__________________________________________________________________________________|___________


int TQSampleVisitor::visitSample(TQSample * sample, TString& message) {
  DEBUGclass("Entering function");
  /* stop if the sample given is invalid */
  if (!sample) return visitFAILED;

  int result = visitFAILED;
  int nEntries = -1;
  TString msg = "";
   /* try to open tree */
  TQToken * treeToken = sample->getTreeToken();

  if (treeToken) {
    result = visitOK;
    nEntries = ((TTree*)treeToken->getContent())->GetEntries();
    sample->returnTreeToken(treeToken);
  } else {
    result = visitFAILED;
    msg = "failed to load tree";
  }

  message.Append(" ");

  /* print tree location */
  TString treeLocation = sample->getTreeLocation();
  if (treeLocation.Length() > 40) {
    if (treeLocation.Length() > 43) {
      treeLocation.Remove(0, treeLocation.Length() - 37);
      treeLocation.Prepend("...");
    } else {
      treeLocation.Remove(0, treeLocation.Length() - 40);
    }
  }

  message.Append(TQStringUtils::fixedWidth(treeLocation, 40, true));

  /* print the n events bin */
  int nEventsBin;
  if (sample->getTagInteger(".init.neventsbin", nEventsBin)) {
    message.Append(TQStringUtils::fixedWidth(TString::Format("%d", nEventsBin), 12)); }
  else {
    message.Append(TQStringUtils::fixedWidth("--", 12)); }

  /* print number of entries in the tree */
  if (nEntries >= 0) {
    message.Append(TQStringUtils::fixedWidth(TString::Format("%d", nEntries), 12)); }
  else {
    message.Append(TQStringUtils::fixedWidth("--", 12)); }

  /* print normalisation factor */
  message.Append(TQStringUtils::fixedWidth(TString::Format("%.3f", sample->getNormalisation()), 12));

  /* print cross section */
  double xSec;
  if (sample->getTagDouble("xsection", xSec)) {
    message.Append(TQStringUtils::fixedWidth(TString::Format("%.3f", xSec), 12)); }
  else {
    message.Append(TQStringUtils::fixedWidth("--", 12)); }

  /* print k factor */
  double kFactor;
  if (sample->getTagDouble("kfactor", kFactor)) {
    message.Append(TQStringUtils::fixedWidth(TString::Format("%.3f", kFactor), 12)); }
  else {
    message.Append(TQStringUtils::fixedWidth("--", 12)); }

  /* print (error) message */
  message.Append(TQStringUtils::fixedWidth(msg, 30));

  return result;

}


//__________________________________________________________________________________|___________


int TQSampleVisitor::revisitSample(TQSample * /*sample*/, TString& /*message*/) {
  return visitIGNORE;
}


//__________________________________________________________________________________|___________


int TQSampleVisitor::revisitFolder(TQSampleFolder * /*sampleFolder*/, TString& /*message*/) {
  return visitIGNORE;
}


//__________________________________________________________________________________|___________

bool TQSampleVisitor::callFinalize() {
  // call finalization of the sample visitor
  int result = finalize();
  return (result != visitFAILED);
}

//__________________________________________________________________________________|___________

int TQSampleVisitor::finalize() {
  // finalize method (virtual)
  return visitLISTONLY;
}

//__________________________________________________________________________________|___________

TQSampleVisitor::~TQSampleVisitor() {
  // destructor
}

//__________________________________________________________________________________|___________

void TQSampleVisitor::stamp(TQTaggable* obj) const {
  // stamp an object as visited
  obj->setTagInteger(TString::Format(".%s.timestamp.machine",this->getVisitTraceIDConst()),TQUtils::getCurrentTime());
  obj->setTagString(TString::Format(".%s.timestamp.human",this->getVisitTraceIDConst()),TQUtils::getTimeStamp());
  obj->setTagBool(TString::Format(".%s.visited",this->getVisitTraceIDConst()),true);
}

//__________________________________________________________________________________|___________

bool TQSampleVisitor::checkVisit(TQTaggable* obj) const {
  // check if an object has been visited (i.e. stamped) by this visitor
  return obj->getTagBoolDefault(TString::Format(".%s.visited",this->getVisitTraceIDConst()),false);
}

//__________________________________________________________________________________|___________

bool TQSampleVisitor::checkVisitFriends(TQSampleFolder* obj) const {
  // check if all friends of a SampleFolder (including itself) have been visited
  // (i.e. stamped) by this visitor. Returns true if all have been visited and
  // false if any one or more have not been visited yet.
  if (!obj) {
    ERRORfunc("SampleFolder provided is a Null pointer!");
    return false;
  }
  updateFriends(obj);
  std::shared_ptr<std::set<TQSampleFolder*>> friends = obj->getFriends();
  //while(itr.hasNext()) {
  if (!friends) {
    throw std::runtime_error( TString::Format("Could not obtain set of Friend SampleFolders (got Null pointer) for SF at path '%s'",obj->getPath().Data() ).Data() );
  }
  for (auto sf : (*friends)) {
    if (!this->checkVisit(sf)) return false;
  }
  return true;
}


void TQSampleVisitor::updateFriends(TQSampleFolder* sf) {
  if (!sf) return;
  if (sf->countFriends() > 0) {DEBUGclass("Already has friends..."); return;}
  sf->findFriends();
  return;
}
