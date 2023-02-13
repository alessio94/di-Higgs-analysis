#include "QFramework/TQLibrary.h"
#include "TTree.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQFolder.h"
#include "definitions.h"
#include "locals.h"
#include <iostream>
#include <stdlib.h>
#include "stdio.h"
#include "unistd.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sys/ioctl.h>
#include <algorithm>
#include "TROOT.h"
#include "TInterpreter.h"

#include "QFramework/TQUtils.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQPathManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQLibrary
//
// The TQLibrary is a global/static class of which there should only be one instance at a time.
// It is not intended to be instantiated by the user -- instead, an instance of it is created
// automatically at runtime as a global static member variable TQLibrary::gQFramework.
//
// It facilitates a set of OS and/or root-version specific 'hacks' to help the user
// in keeping his or her own code clean of such workarounds.
//
// It also provides functions to access version numbers of the compiler and root itself.
//
////////////////////////////////////////////////////////////////////////////////////////////////

int TQLibrary::stdoutfd(dup(fileno(stdout)));
int TQLibrary::stderrfd(dup(fileno(stderr)));
bool TQLibrary::stdoutfd_isRedirected(false);
bool TQLibrary::stderrfd_isRedirected(false);
bool TQLibrary::stdoutfd_allowRedirection(true);
bool TQLibrary::stderrfd_allowRedirection(true);
bool TQLibrary::stdoutstr_isCaptured(false);
bool TQLibrary::stderrstr_isCaptured(false);
bool TQLibrary::stdouterrstr_areCaptured(false);
bool TQLibrary::stdoutstr_allowCapturing(true);
bool TQLibrary::stderrstr_allowCapturing(true);
std::streambuf* TQLibrary::defaultStdoutStreamBuf(NULL);
std::streambuf* TQLibrary::defaultStderrStreamBuf(NULL);
std::stringstream* TQLibrary::capturingStdoutStream(NULL);
std::stringstream* TQLibrary::capturingStderrStream(NULL);
std::stringstream* TQLibrary::capturingStdoutStderrStream(NULL);

TQMessageStream TQLibrary::msgStream(std::cout);


TQLibrary* TQLibrary::getQLibrary(){
  // return a pointer to gQFramework
  if(TQLibrary::isInitialized)
    return &TQLibrary::gQFramework;
  return NULL;
}

TQLibrary TQLibrary::gQFramework;
bool TQLibrary::isInitialized(false);

TQLibrary::TQLibrary() :
#ifdef SVNVERSION
  libsvnversion(SVNVERSION),
#else
  libsvnversion("UNKNOWN"),
#endif
#ifdef ROOTVERSION
  rootversion(ROOTVERSION),
#else
  rootversion("UNKNOWN"),
#endif
#ifdef GCCVERSION
  gccversion(GCCVERSION),
#else
  gccversion("UNKNOWN"),
#endif
#ifdef TQPATH
  tqpath(TQPATH),
#else
  tqpath(""),
#endif
  eosmgmurl("root://eosuser.cern.ch"),
  eoscmd("/afs/cern.ch/project/eos/installation/0.3.84-aquamarine.user/bin/eos"),
#ifdef LOCALGROUPDISK
  localGroupDiskIdentifier(LOCALGROUPDISK),
#else
  localGroupDiskIdentifier(""),
#endif
#ifdef DQ2PATHHEAD
  dq2PathHead(DQ2PATHHEAD),
#else
  dq2PathHead(""),
#endif
#ifdef DCACHEPATHHEAD
  dCachePathHead(DCACHEPATHHEAD),
#else
  dCachePathHead(""),
#endif
  dq2cmd("dq2"),
  website("http://atlas-caf.web.cern.ch/"),
  appName("My CAFCore App"),
  procStatPath("/proc/self/stat"),
#ifdef PDFFONTEMBEDCMD
  pdfFontEmbedCommand(PDFFONTEMBEDCMD),
#else
  pdfFontEmbedCommand("mv $(filename) $(filename).bak && ( gs -q -dNOPAUSE -dBATCH -dPDFSETTINGS=/prepress -sDEVICE=pdfwrite -sOutputFile=$(filename) $(filename).bak && rm $(filename).bak ) || mv $(filename).bak $(filename)"),
#endif
  pdfFontEmbedEnabled(false),
#ifdef EXIFTOOLPATH
  exiftoolPath(EXIFTOOLPATH),
#else
  exiftoolPath(""),
#endif
  exiftoolEnabled(false),
#ifdef LIBXMLPATH
  libXMLpath(LIBXMLPATH)
#else
  libXMLpath(TQLibrary::findLibrary("libxml2*.so*"))
#endif
{
  this->timeOfConstruction = TQUtils::getCurrentTime();

  TTree::SetMaxTreeSize(1e15);

  char cCurrentPath[FILENAME_MAX];

  if (getcwd(cCurrentPath, sizeof(cCurrentPath)))
    this->workingDirectory = TString(cCurrentPath);

  if (this == &TQLibrary::gQFramework){
    DEBUG("The static member gQFramework is being created.");
    TQLibrary::isInitialized = true;
  }
  else{
    DEBUG("An instance of TQLibrary that is not the static member gQFramework is being created.");
  }

#ifdef PACKAGES
  for(auto pkg:PACKAGES){
		packages.push_back(pkg);
  }
#endif

#ifdef CONSOLEWIDTH
  this->setConsoleWidth(CONSOLEWIDTH);
#else
  this->findConsoleWidth();
#endif
}

TQLibrary::~TQLibrary(){
  // If snowflakeMode, check the number of printed messages. If they exceed
  // a threshold, break. A threshold can be set for warnings. The threshold
  // value for everything worse than that is 0.
  if (this->snowflakeMode){
    for (auto type = TQMessageStream::messageTypesAlert.begin(); type != TQMessageStream::messageTypesAlert.end(); type++){
      unsigned int messageCount = TQMessageStream::getGlobMessageCount(*type);
      if ((*type == TQMessageStream::WARNING) && (messageCount <= this->maxWarningsAllowed))
        continue;
      if (messageCount > 0){
        // Make sure that files are written before crashing
        if (TQPathManager::getPathManager())
          TQPathManager::getPathManager()->~TQPathManager();
        printGlobAlertMessageCount();
        BREAK("You are running in snowflake mode and the allowed number of error messages was exceeded (allowed are %d warning messages and 0 other alert messages).", this->maxWarningsAllowed);
      }
    }
  }
  if (this == &TQLibrary::gQFramework){
    DEBUGclass("The static member gQFramework is being destroyed.");
    TQLibrary::isInitialized = false;
  }
  else{
    DEBUGclass("An instance of TQLibrary that is not the static member gQFramework is being destroyed.");
  }

}


const TString& TQLibrary::getLocalGroupDisk(){
  // retrieve the string identifying the local group disk
  // this is the local group disk identifier, i.e. something like
  // UNI-FREIBURG_LOCALGROUPDISK
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.localGroupDiskIdentifier;
}
void TQLibrary::setLocalGroupDisk(const TString& lcgid){
  // set the string identifying the local group disk
  // this is the local group disk identifier, i.e. something like
  // UNI-FREIBURG_LOCALGROUPDISK
  this->localGroupDiskIdentifier = lcgid;
}

const TString& TQLibrary::getDQ2PathHead(){
  // retrieve the DQ2 path head for the local storage
  // this is something like
  // srm://se.bfg.uni-freiburg.de
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.dq2PathHead;
}
void TQLibrary::setDQ2PathHead(const TString& dq2ph){
  // set the DQ2 path head for the local storage
  // this is something like
  // srm://se.bfg.uni-freiburg.de
  this->dq2PathHead = dq2ph;
}

const TString& TQLibrary::getdCachePathHead(){
  // retrieve the dCache path head for the local storage
  // this is something like
  // dcap://se.bfg.uni-freiburg.de:22125
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.dCachePathHead;
}
void TQLibrary::setdCachePathHead(const TString& dCache2ph){
  // set the dCache path head for the local storage
  // this is something like
  // dcap://se.bfg.uni-freiburg.de:22125
  this->dCachePathHead = dCache2ph;
}

const TString& TQLibrary::getDQ2cmd(){
  // retrieve the local DQ2 command
  // this usually either "dq2" or "rucio"
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.dq2cmd;
}
void TQLibrary::setDQ2cmd(const TString& dq2command){
  // set the local DQ2 command
  // this usually either "dq2" or "rucio"
  this->dq2cmd = dq2command;
}

TString TQLibrary::getEOScmd(){
  // retrieve the eos activation command
  // on CERN AFS and for the ATLAS EOS, this is
  // export EOS_MGM_URL=root://eosatlas.cern.ch; /afs/cern.ch/project/eos/installation/atlas/bin/eos
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return "export EOS_MGM_URL="+TQLibrary::gQFramework.eosmgmurl + "; " + TQLibrary::gQFramework.eoscmd;
}

void TQLibrary::setEOScmd(TString neweoscmd){
  // set the path to the eos binary
  // on CERN AFS, this is
  // /afs/cern.ch/project/eos/installation/atlas/bin/eos
  this->eoscmd = neweoscmd;
}

void TQLibrary::setEOSurl(TString neweosurl){
  // set the eos url, for example
  // root://eosuser.cern.ch for USER EOS
  // root://eosatlas.cern.ch for ATLAS EOS
  this->eosmgmurl = neweosurl;
}

void TQLibrary::printMessage(){
  // print an informational message
  // containing all important version numbers
  std::cout << "This is libQFramework rev. " << TQLibrary::getVersion() << ", compiled with root " << TQLibrary::getROOTVersion() << " and g++ " << TQLibrary::getGCCVersion() << std::endl;
  std::cout << "for bug reports, feature requests and general questions, please contact cburgard@cern.ch" << std::endl;
}

const TString& TQLibrary::getVersion(){
  // return the svn revision of the libQFramework
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.libsvnversion;
}

const TString& TQLibrary::getROOTVersion(){
  // return the root version
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.rootversion;
}

const TString& TQLibrary::getGCCVersion(){
  // return the compiler version
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.gccversion;
}

int TQLibrary::getVersionNumber(){
  // return the svn revision of the libQFramework (numeric value);
  if (!TQLibrary::isInitialized) return 0;
  return atoi(TQLibrary::gQFramework.libsvnversion);
}

float TQLibrary::getROOTVersionNumber(){
  // return the root version (numeric value)
  if (!TQLibrary::isInitialized) return 0;
  return atoi(TQLibrary::gQFramework.rootversion);
}

float TQLibrary::getGCCVersionNumber(){
  // return the compiler version (numeric value)
  if (!TQLibrary::isInitialized) return 0;
  return atoi(TQLibrary::gQFramework.gccversion);
}


int TQLibrary::redirect_stdout(const TString& fname, bool append){
  // redirect stdout to the file of the given name
  // if the file exists, it will be overwritten
  // if the file does not exist, it will be created
  if(!TQLibrary::stdoutfd_allowRedirection) return -1;
  if(TQLibrary::stdoutfd_isRedirected) return -2;
  if(TQLibrary::stdoutstr_isCaptured) return -3;
  fflush(stdout);
  int newstdout = append ? open(fname.Data(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) : open(fname.Data(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  dup2(newstdout, fileno(stdout));
  close(newstdout);
  TQLibrary::stdoutfd_isRedirected = true;
  return fileno(stdout);
}

int TQLibrary::restore_stdout(){
  // if TQLibrary::redirect_stdout or TQLibrary::capture_stdout
  // was called previously this function will restore the
  // original output stream
  if (TQLibrary::stdoutfd_isRedirected){
    fflush(stdout);
    dup2(TQLibrary::stdoutfd, fileno(stdout));
    TQLibrary::stdoutfd_isRedirected = false;
    return TQLibrary::stdoutfd;
  }
  else if (TQLibrary::stdoutstr_isCaptured){
    if (TQLibrary::stdouterrstr_areCaptured){
      VERBOSE("Call TQLibrary::restore() to end capturing of stdout and stderr. Not restoring stdout.");
      return -2;
    }
    std::cout.rdbuf(TQLibrary::defaultStdoutStreamBuf);
    delete TQLibrary::capturingStdoutStream;
    TQLibrary::capturingStdoutStream = NULL;
    TQLibrary::stdoutstr_isCaptured = false;
    return 0;
  }
  return -1;
}

TString TQLibrary::readCapturedStdout(){
  if (!TQLibrary::stdoutstr_isCaptured) return "";
  return TQLibrary::capturingStdoutStream->str();
}

int TQLibrary::resetCapturedStdout(){
  if (!TQLibrary::stdoutstr_isCaptured) return -3;
  TQLibrary::capturingStdoutStream->str("");
  TQLibrary::capturingStdoutStream->clear();
  return 0;
}

TString TQLibrary::readAndResetCapturedStdout(){
  TString str = TQLibrary::readCapturedStdout();
  TQLibrary::resetCapturedStdout();
  return str;
}

TString TQLibrary::readCapturedStderr(){
  if (!TQLibrary::stderrstr_isCaptured) return "";
  return TQLibrary::capturingStderrStream->str();
}

int TQLibrary::resetCapturedStderr(){
  if (!TQLibrary::stderrstr_isCaptured) return -3;
  TQLibrary::capturingStderrStream->str("");
  TQLibrary::capturingStderrStream->clear();
  return 0;
}

TString TQLibrary::readAndResetCapturedStderr(){
  TString str = TQLibrary::readCapturedStderr();
  TQLibrary::resetCapturedStderr();
  return str;
}

TString TQLibrary::readCapturedStdoutStderr(){
  if (!TQLibrary::stdouterrstr_areCaptured) return "";
  return TQLibrary::capturingStdoutStderrStream->str();
}

int TQLibrary::resetCapturedStdoutStderr(){
  if (!TQLibrary::stdouterrstr_areCaptured) return -3;
  TQLibrary::capturingStdoutStderrStream->str("");
  TQLibrary::capturingStdoutStderrStream->clear();
  return 0;
}

TString TQLibrary::readAndResetCapturedStdoutStderr(){
  TString str = TQLibrary::readCapturedStdoutStderr();
  TQLibrary::resetCapturedStdoutStderr();
  return str;
}


int TQLibrary::redirect_stderr(const TString& fname, bool append){
  // redirect stderr to the file of the given name
  // if the file exists, it will be overwritten
  // if the file does not exist, it will be created
  if(!TQLibrary::stderrfd_allowRedirection) return -1;
  if(TQLibrary::stderrfd_isRedirected) return -2;
  if(TQLibrary::stderrstr_isCaptured) return -3;
  fflush(stderr);
  int newstderr = append ? open(fname.Data(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) : open(fname.Data(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  dup2(newstderr, fileno(stderr));
  close(newstderr);
  TQLibrary::stderrfd_isRedirected = true;
  return newstderr;
}

int TQLibrary::restore_stderr(){
  // if TQLibrary::redirect_stderr or TQLibrary::capture_stderr
  // was called previously this function will restore the
  // original output stream
  if(TQLibrary::stderrfd_isRedirected){
    fflush(stderr);
    dup2(TQLibrary::stderrfd, fileno(stderr));
    TQLibrary::stderrfd_isRedirected = false;
    return TQLibrary::stderrfd;
  }
  else if (TQLibrary::stderrstr_isCaptured){
    if (TQLibrary::stdouterrstr_areCaptured){
      VERBOSE("Call TQLibrary::restore() to end capturing of stdout and stderr. Not restoring stderr.");
      return -2;
    }
    std::cerr.rdbuf(TQLibrary::defaultStderrStreamBuf);
    delete TQLibrary::capturingStderrStream;
    TQLibrary::capturingStderrStream = NULL;
    TQLibrary::stderrstr_isCaptured = false;
    return 0;
  }
  return -1;
}

int TQLibrary::captureStdout(){
  // Don't print stdout, but instead store it in
  // TQLibrary::capturingStdoutStream. The stored string
  // can be read using TQLibrary::readCapturedStdout().
  // Use TQLibrary::restore_stdout() to stop capturing.
  if(!TQLibrary::stdoutstr_allowCapturing) return -1;
  if(TQLibrary::stdoutfd_isRedirected) return -2;
  if(TQLibrary::stdoutstr_isCaptured) return -3;
  TQLibrary::capturingStdoutStream = new std::stringstream();
  std::cout.flush();
  TQLibrary::defaultStdoutStreamBuf = std::cout.rdbuf();
  std::cout.rdbuf(TQLibrary::capturingStdoutStream->rdbuf());
  TQLibrary::stdoutstr_isCaptured = true;
  return 0;
}

int TQLibrary::captureStderr(){
  // Don't print stderr, but instead store it in
  // TQLibrary::capturingStderrStream. The stored string
  // can be read using TQLibrary::readCapturedStderr().
  // Use TQLibrary::restore_stderr() to stop capturing.
  if(!TQLibrary::stderrstr_allowCapturing) return -1;
  if(TQLibrary::stderrfd_isRedirected) return -2;
  if(TQLibrary::stderrstr_isCaptured) return -3;
  TQLibrary::capturingStderrStream = new std::stringstream();
  std::cerr.flush();
  TQLibrary::defaultStderrStreamBuf = std::cerr.rdbuf();
  std::cerr.rdbuf(TQLibrary::capturingStderrStream->rdbuf());
  TQLibrary::stderrstr_isCaptured = true;
  return 0;
}

int TQLibrary::captureStdoutStderr(){
  // Captures both stdout and stderr and writes them to the same
  // stream.

  if ((!TQLibrary::stdoutstr_allowCapturing) or
      (!TQLibrary::stderrstr_allowCapturing))
    return -1;
  if ((TQLibrary::stdoutfd_isRedirected) or
      (TQLibrary::stderrfd_isRedirected))
    return -2;
  if ((TQLibrary::stdoutstr_isCaptured) or
      (TQLibrary::stderrstr_isCaptured))
    return -3;
  TQLibrary::capturingStdoutStderrStream = new std::stringstream();
  std::cout.flush();
  std::cerr.flush();
  TQLibrary::defaultStdoutStreamBuf = std::cout.rdbuf();
  TQLibrary::defaultStderrStreamBuf = std::cerr.rdbuf();
  std::cout.rdbuf(TQLibrary::capturingStdoutStderrStream->rdbuf());
  std::cerr.rdbuf(TQLibrary::capturingStdoutStderrStream->rdbuf());
  TQLibrary::stdoutstr_isCaptured = true;
  TQLibrary::stderrstr_isCaptured = true;
  TQLibrary::stdouterrstr_areCaptured = true;
  return 0;
}

void TQLibrary::allowRedirection_stdout(bool allow){
  if (TQLibrary::stdouterrstr_areCaptured)
    TQLibrary::restore();
  else
    TQLibrary::restore_stdout();
  TQLibrary::stdoutfd_allowRedirection = allow;
}
void TQLibrary::allowRedirection_stderr(bool allow){
  if (TQLibrary::stdouterrstr_areCaptured)
    TQLibrary::restore();
  else
    TQLibrary::restore_stderr();
  TQLibrary::stderrfd_allowRedirection = allow;
}
void TQLibrary::allowRedirection(bool allow){
  TQLibrary::restore();
  TQLibrary::stdoutfd_allowRedirection = allow;
  TQLibrary::stderrfd_allowRedirection = allow;
}

void TQLibrary::allowCapturing_stdout(bool allow){
  if (TQLibrary::stdouterrstr_areCaptured)
    TQLibrary::restore();
  else
    TQLibrary::restore_stdout();
  TQLibrary::stdoutstr_allowCapturing = allow;
}
void TQLibrary::allowCapturing_stderr(bool allow){
  if (TQLibrary::stdouterrstr_areCaptured)
    TQLibrary::restore();
  else
    TQLibrary::restore_stderr();
  TQLibrary::stderrstr_allowCapturing = allow;
}
void TQLibrary::allowCapturing(bool allow){
  TQLibrary::restore();
  TQLibrary::stdoutstr_allowCapturing = allow;
  TQLibrary::stderrstr_allowCapturing = allow;
}

int TQLibrary::redirect(const TString& fname, bool append){
  // redirect stdout and stderr to the file of the given name
  // if the file exists, it will be overwritten
  // if the file does not exist, it will be created
  if(!TQLibrary::stdoutfd_allowRedirection || !TQLibrary::stderrfd_allowRedirection) return -1;
  if(TQLibrary::stdoutfd_isRedirected || TQLibrary::stderrfd_isRedirected) return -2;
  fflush(stdout);
  fflush(stderr);
  int newstdeo = append ? open(fname.Data(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) : open(fname.Data(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  dup2(newstdeo, fileno(stdout));
  dup2(newstdeo, fileno(stderr));
  close(newstdeo);
  TQLibrary::stdoutfd_isRedirected = true;
  TQLibrary::stderrfd_isRedirected = true;
  return newstdeo;
}

bool TQLibrary::restore(){
  // if TQLibrary::redirect_stdout was called previously
  // this function will restore the original output stream
  if (!TQLibrary::stdouterrstr_areCaptured){
    TQLibrary::restore_stderr();
    TQLibrary::restore_stdout();
  }
  else{
    std::cout.rdbuf(TQLibrary::defaultStdoutStreamBuf);
    std::cerr.rdbuf(TQLibrary::defaultStderrStreamBuf);
    delete TQLibrary::capturingStdoutStderrStream;
    TQLibrary::capturingStdoutStderrStream = NULL;
    TQLibrary::stdoutstr_isCaptured = false;
    TQLibrary::stderrstr_isCaptured = false;
    TQLibrary::stdouterrstr_areCaptured = false;
  }
  return true;
}

Long64_t TQLibrary::getVirtualMemory(){
	try {
		std::string line;
		std::ifstream myfile (TQLibrary::getProcStatPath().Data());
		if (myfile.is_open()){
			std::string pid, comm, state, ppid, pgrp, session, tty_nr;
			std::string tpgid, flags, minflt, cminflt, majflt, cmajflt;
			std::string utime, stime, cutime, cstime, priority, nice;
			std::string O, itrealvalue, starttime;
			Long64_t vsize;
			Long64_t rss;
			if( myfile.good() ){
				myfile >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
							 >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
							 >> utime >> stime >> cutime >> cstime >> priority >> nice
							 >> O >> itrealvalue >> starttime >> vsize >> rss;
			}
			myfile.close();
			return vsize;
		}
	} catch (const std::bad_alloc& oom){
		// do nothing
	}
	return -1;
}

void TQLibrary::printMemory(){
  // print the current memory usage of the code
	Long64_t vsize = getVirtualMemory();
	if(vsize>0){
		std::cout << TQLibrary::getApplicationName() <<": virtual memory used = " << TQStringUtils::fixedWidth(TQStringUtils::getThousandsSeparators(vsize,"'"),20) << std::endl;
	} else {
		std::cout << "Unable to open file" << std::endl;
	}
}

void TQLibrary::recordMemory(short color) {
  if (!TQLibrary::isInitialized) return;
  try {
    TQLibrary::gQFramework.rssUsageTimestamps.push_back(TQUtils::getCurrentTime());
    TQLibrary::gQFramework.rssUsageMemory.push_back(TQUtils::getCurrentRSS());
    TQLibrary::gQFramework.rssUsageColors.push_back(color);
  } catch (const std::bad_alloc& oom){
    // do nothing
  }
  return;
}

TMultiGraph* TQLibrary::getMemoryGraph(bool differential) {
  // returns a TGraph recording the memory usage over time
  if (!TQLibrary::isInitialized) return NULL;

  std::vector<double>timestamps(TQLibrary::gQFramework.rssUsageTimestamps.begin(),TQLibrary::gQFramework.rssUsageTimestamps.end());
  std::vector<double>memory(TQLibrary::gQFramework.rssUsageMemory.begin(),TQLibrary::gQFramework.rssUsageMemory.end());


  //if (dTimestamps.size() != dMemory.size() || dMemory.size()==0) return nullptr;
  if (differential) {
    std::vector<double> deltaMemory;
    deltaMemory.reserve(memory.size());
    for (size_t i=0; i<memory.size(); ++i) {
      if (i==0) {
        deltaMemory.push_back(0.);
      } else {
        deltaMemory.push_back(memory[i]-memory[i-1]);
      }
    }
    return TQHistogramUtils::makeMultiColorGraph(timestamps, deltaMemory, TQLibrary::gQFramework.rssUsageColors);
  }

  return TQHistogramUtils::makeMultiColorGraph(timestamps, memory, TQLibrary::gQFramework.rssUsageColors);

  //new TGraph(dTimestamps.size(),&dTimestamps[0],&dMemory[0]);
}

const TString& TQLibrary::getProcStatPath(){
  // retrieve the path to the file/device providing process information
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.procStatPath;
}

void TQLibrary::setProcStatPath(TString newpath){
  // set the path to the file/device providing process information
  this->procStatPath = newpath;
}

const TString& TQLibrary::getEXIFtoolPath(){
  // retrieve the path of the EXIFtool binary
  // which allows modifying PDF meta-information
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.exiftoolPath;
}

void TQLibrary::setEXIFtoolPath(TString newpath){
  // set the path of the EXIFtool binary
  // which allows modifying PDF meta-information
  this->exiftoolPath = newpath;
}

bool TQLibrary::hasEXIFsupport(){
  // return true if TQLibrary is configured for EXIF support
  // return false otherwise
  return TQLibrary::isInitialized &&gQFramework.exiftoolEnabled && !(gQFramework.exiftoolPath.IsNull());
}

bool TQLibrary::enableEXIFsupport(bool val){
  // enable support of EXIFtool
  // this requires a valid EXIFtool path to be set
  if (!TQLibrary::isInitialized) return false;
  if(TQLibrary::gQFramework.exiftoolPath.IsNull()) return false;
  TQLibrary::gQFramework.exiftoolEnabled = val;
  return true;
}

bool TQLibrary::setEXIF(const TString& fname, const TString& title, const TString& keywords){
  // use the EXIFtool to set the title and keywords metainformation on some PDF
  if(title.IsNull() || keywords.IsNull()) return false;
  if(!TQLibrary::hasEXIFsupport()) return false;
  system(TQLibrary::getEXIFtoolPath() + " -Author=\"$(whoami)\" -Title=\""+title+"\" -Keywords=\""+keywords+"\" -Creator\"="+TQLibrary::getApplicationName()+" with libQFramework rev. "+TQLibrary::getVersion()+" (ROOT "+TQLibrary::getROOTVersion() + ", g++ " + TQLibrary::getGCCVersion() + ")\" " + fname);
  return true;
}


const TString& TQLibrary::getPDFfontEmbedCommand(){
  // retrieve the command used to embed fonts in PDF files
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.pdfFontEmbedCommand;
}

void TQLibrary::setPDFfontEmbedCommand(TString cmd){
  // set the command used to embed fonts in PDF files
  this->pdfFontEmbedCommand = cmd;
}

bool TQLibrary::hasPDFfontEmbedding(){
  // return true if TQLibrary is configured to perfrom font embedding in PDFs
  // return false otherwise
  return TQLibrary::isInitialized && gQFramework.pdfFontEmbedEnabled && !(gQFramework.pdfFontEmbedCommand.IsNull());
}

bool TQLibrary::enablePDFfontEmbedding(bool val){
  // enable font embedding in PDFs
  // this requires pdfFonntEmbedCommand to be set
  if (!TQLibrary::isInitialized) return false;
  if(TQLibrary::gQFramework.pdfFontEmbedCommand.IsNull()) return false;
  TQLibrary::gQFramework.pdfFontEmbedEnabled = val;
  return true;
}

bool TQLibrary::embedFonts(const TString& filename, bool verbose){
  // embed fonts in some PDF file
  // if verbose, print the command output
  if(!TQLibrary::hasPDFfontEmbedding()) return false;
  TString cmd(TQLibrary::getPDFfontEmbedCommand());
  cmd.ReplaceAll("$(filename)",filename);
  TList* result = TQUtils::execute(cmd);
  if(!result) return false;
  if(verbose && (result->GetEntries() > 0)){
    msgStream.sendFunctionMessage(TQMessageStream::VERBOSE,__FUNCTION__,"embedding fonts for file %s",filename.Data());
    TQIterator itr(result);
    while(itr.hasNext()){
      TObject* obj = itr.readNext();
      msgStream.sendFunctionMessage(TQMessageStream::VERBOSE,__FUNCTION__,obj->GetName());
    }
  }
  delete result;
  return true;
}

const TString& TQLibrary::getApplicationName(){
  // get the name of this application
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.appName;
}

void TQLibrary::setApplicationName(TString newname){
  // set the name of this application
  this->appName = newname;
}

TString TQLibrary::getAbsolutePath(const TString& path){
  // retrieve the absolute path to some local path inside the working directory
  if(path.IsNull()) return TQLibrary::getWorkingDirectory();
  if(path[0]=='/') return path;
  return TQFolder::concatPaths(TQLibrary::getWorkingDirectory(),path);
}
const TString& TQLibrary::getWorkingDirectory(){
  // retrieve the current working directory
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.workingDirectory;
}
void TQLibrary::setWorkingDirectory(TString newpath){
  // set the current working directory
  this->workingDirectory = newpath;
}
const TString& TQLibrary::pwd(){
  // retrieve the current working directory
  // same as getWorkingDirectory
  return TQLibrary::getWorkingDirectory();
}
void TQLibrary::cd(TString newpath){
  // change the current working directory
  // same as setWorkingDirectory
  this->workingDirectory = newpath;
}

const TString& TQLibrary::getTQPATH(){
  // get the TQPATH
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.tqpath;
}
void TQLibrary::setTQPATH(TString newpath){
  // set the TQPATH
  this->tqpath = newpath;
}

const TString& TQLibrary::getlibXMLpath(){
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.libXMLpath;
}

const TString& TQLibrary::getWebsite(){
  // retrieve the URL of the documentation website
  if (!TQLibrary::isInitialized) return TQStringUtils::emptyString;
  return TQLibrary::gQFramework.website;
}


void TQLibrary::setMessageStream(std::ostream& os, bool allowColors){
  // set the message output stream
  TQLibrary::msgStream.absorb(TQMessageStream(os,false,allowColors));
}

void TQLibrary::setMessageStream(std::ostream* os, bool allowColors){
  // set the message output stream
  TQLibrary::msgStream.absorb(TQMessageStream(os,false,allowColors));
}

bool TQLibrary::openLogFile(const TString& fname, bool allowColors){
  // open a log file at the given location
  INFOfunc(TQUtils::getTimeStamp()+ " - attempting to use log file '%s'",fname.Data());
  TQLibrary::msgStream.absorb(TQMessageStream(fname,allowColors));
  if(msgStream.isGood()){
    INFOfunc(TQUtils::getTimeStamp()+ " - successfully opened log file for writing");
  } else {
    ERRORfunc(TQUtils::getTimeStamp()+ " - failed to open log file for writing");
  }
  return true;
}

bool TQLibrary::closeLogFile(){
  // close the currently opened log file and redirect logging to std::cout
  // the same effect can be accomplished by calling TQLibrary::setMessageStream(std::cout)
  INFOfunc(TQUtils::getTimeStamp()+ " - closing log");
  TQLibrary::setMessageStream(std::cout,true);
  INFOfunc(TQUtils::getTimeStamp()+ " - closed log file");
  return true;
}

int TQLibrary::getConsoleWidth(){
  // get the currently set console width
  return TQLibrary::gQFramework.consoleWidth;
}

void TQLibrary::setConsoleWidth(int width){
  // set the global console width
  TQLibrary::gQFramework.consoleWidth = width;
}

void TQLibrary::findConsoleWidth(){
  // recalculate the console width from the kernel
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  this->consoleWidth = std::max(std::min((int)(w.ws_col),(int)300),40);
}


TString TQLibrary::findLibrary(const TString& filename){
  // find the path of some library
  TString retval = "";
  TList* paths = TQUtils::execute("find $(g++ --print-search-dirs | grep libraries | sed -e 's/^[ ]*libraries:[ ]*=//' -e 's/:/ /g') -name '"+filename+".*so' 2>/dev/null -print -quit",255);
  paths->SetOwner(true);
  if(!paths) return retval;
  if(paths->GetEntries() > 0){
    retval = paths->First()->GetName();
  }
  delete paths;
  return retval;
}

bool TQLibrary::hasPackage(const char* pkgname){
	// check if a given package was known at build time
	for(const auto& pkg:TQLibrary::getQLibrary()->packages){
		if(TQStringUtils::equal(pkgname,pkg)) return true;
	}
	return false;
}

std::vector<std::string> TQLibrary::getListOfPackages() {
  std::vector<std::string> pks;
  for (std::string p : TQLibrary::getQLibrary()->packages) {
    pks.push_back(p);
  }
  return pks;
}

void TQLibrary::printGlobMessageCount(){
  TQMessageStream::printGlobMessageCount();
}

void TQLibrary::printGlobAlertMessageCount(){
  TQMessageStream::printGlobAlertMessageCount();
}

void TQLibrary::setSnowflakeMode(bool snowflakeMode, unsigned int maxWarningsAllowed){
  this->snowflakeMode = snowflakeMode;
  this->maxWarningsAllowed = maxWarningsAllowed;
}

bool TQLibrary::getSnowflakeMode(){
  return this->snowflakeMode;
}

unsigned long TQLibrary::getTimeOfConstruction(){
  return this->timeOfConstruction;
}

unsigned long TQLibrary::getTimeSinceConstruction(){
  return TQUtils::getCurrentTime() - this->timeOfConstruction;
}
