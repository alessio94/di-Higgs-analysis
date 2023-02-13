#include "QFramework/TQPathManager.h"

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

#include "QFramework/TQUtils.h"
#include "QFramework/TQStringUtils.h"

// #define _DEBUG_

#include "QFramework/TQLibrary.h"

#include "TString.h"
#include "TSystem.h"

ClassImp(TQPathManager)

//@env: [CAF_USE_TEMP_DIR, CAF_USE_LOCAL_STORAGE] can be used to disable the use of (temporary) 
//@env  node local storage for outputs. If set to "no", "false", or "0"
//@env  outputs will be written directly to their target destination
//@env  (can reduce performance but may be needed on some clusters) 
//@env  CAFUSETEMPDIR takes precedence over CAFUSELOCALSTORAGE in case both are set
const bool TQPathManager::gUseTemporaryStorageDefault = 
  TQUtils::readEnvVarValueBool("CAF_USE_TEMP_DIR", 
    TQUtils::readEnvVarValueBool("CAF_USE_LOCAL_STORAGE", true)
  );

// Index used to uniquely identify all local files
int TQPathManager::filenameIndex = 0;
TQPathManager TQPathManager::gPathManager; //MUST be after initialization of gUseTemporaryStrageDefault!
bool TQPathManager::gPathManagerExists = false;


////////////////////// static methods //////////////////////


std::string TQPathManager::findFileFromEnvVar(const TString& filename, const std::string& envVar, bool printWarnings){
  return findFileFromEnvVar_internal(filename, envVar, printWarnings, true);
}

std::string TQPathManager::findFileFromEnvVarWithoutExecDir(const TString& filename, const std::string& envVar, bool printWarnings){
  return findFileFromEnvVar_internal(filename, envVar, printWarnings, false);
}

std::string TQPathManager::findFileFromEnvVar_internal(const TString& filename, const std::string& envVar, bool printWarnings, bool searchExecutionDir){
  // Take value of envVar and use it to call findFileFromList

  std::string searchPathList = TQUtils::readEnvVarValue(envVar);
  if (searchPathList == ""){
    DEBUGclass("Calling findFileFromList with an empty searchPathList since the environment variable %s was not found.", envVar.c_str());
  }

  std::string filePath = TQPathManager::findFileFromList_internal(filename, searchPathList, printWarnings, searchExecutionDir);
  if ((searchPathList == "") && (filePath == "")){
    TString message = TQStringUtils::format("The environment variable %s is not defined. Is this the reason why the file %s could not be found?", envVar.c_str(), filename.Data());
    if (printWarnings) {WARNclass(message);}
    else {DEBUGclass(message);}
  }

  return filePath;
}

std::string TQPathManager::findFileFromList(TString filename, const std::string& searchPathList, bool printWarnings){
  return findFileFromList_internal(filename, searchPathList, printWarnings, true);
}

std::string TQPathManager::findFileFromListWithoutExecDir(TString filename, const std::string& searchPathList, bool printWarnings){
  return findFileFromList_internal(filename, searchPathList, printWarnings, false);
}

void TQPathManager::setWorkingDirectory(TString dir){
  // changes the working directory to the given path
  // WARNING: this change affects the reading and writing location of files
  //          in many places, some of which the person calling this function
  //          (i.e., you) might not be aware of. Only call this function if
  //          you know *exactly* what you are doing and why!
  DEBUGclass("working directory changed from '%s' to '%s'",this->pwd.Data(),dir.Data());
  this->pwd = dir;
}

std::string TQPathManager::findFileFromList_internal(TString filename, const std::string& searchPathList, bool printWarnings, bool searchExecutionDir){
  // Largely copied from Athena's more sophisticated PathResolver
  // 1. if filename is given as absolute path,
  // 1.1 if it exists, return absolute path
  // 1.2 if it doesn't exist, return ""
  // 2. if filename is local file and searchExecutionDir, return absolute path
  // 3. if filename is in searchPathList, return absolute path
  // 4. otherwise return ""

  // Remove leading and trailing spaces
  filename = TQStringUtils::trim(filename);

  if (searchPathList == "") {DEBUGclass("Trying to find %s locally.", filename.Data());}
  else {DEBUGclass("Trying to find %s in %s.", filename.Data(), searchPathList.c_str());}

  if (filename == ""){
    DEBUGclass("The passed argument filename is empty. Exiting the method.");
    return "";
  }

  TString localDir = gSystem->pwd();
  TString localPath = TQFolder::concatPaths(localDir, filename);

  // Check if file is given as an absolute path and if it is a regular file
  // (as opposed to a directory)
  if (gSystem->IsAbsoluteFileName(filename.Data())){
    if (TQUtils::fileExists(filename)){
      std::string totalPath = filename.Data();
      DEBUGclass("Found absolute path: %s. Not resolving.", filename.Data());
      return totalPath;
    }
    TString message = TQStringUtils::format("It looks like %s is an absolute path, but there is no such file (is it maybe a directory?). Exiting this method.", filename.Data());
    if (printWarnings) {WARNclass(message.Data());}
    else {DEBUGclass(message.Data());}
    return "";
  }
  // Check if filename matches local file in the execution directory
  if ((searchExecutionDir) && (TQUtils::fileExists(localPath))){
    std::string totalPath = localPath.Data();
    DEBUGclass("Found %s here: %s", filename.Data(), totalPath.c_str());
    return totalPath;
  }
  // Loop over all directories in searchPathList and look for filename
  std::vector< TString > searchPathVector;
  searchPathVector = TQStringUtils::split(searchPathList, ":");
  for (auto& searchPath:searchPathVector){
    TString filePath = TQFolder::concatPaths(searchPath, filename);
    if (TQUtils::fileExists(filePath)){
      std::string totalPath = filePath.Data();
      DEBUGclass("Found %s here: %s", filename.Data(), totalPath.c_str());
      return totalPath;
    }
  }
  TString message = TQStringUtils::format("The file %s could not be found.", filename.Data());
  if (printWarnings) {WARNclass(message.Data());}
  else {DEBUGclass(message.Data());}
  return "";
}

TQPathManager* TQPathManager::getPathManager(){
  if (TQPathManager::gPathManagerExists)
    return &gPathManager;
  return nullptr;
}


////////////////////// public methods //////////////////////


bool TQPathManager::getUseTemporaryStorage() const {
  return this->useTemporaryStorage;
}

void TQPathManager::setUseTemporaryStorage(bool useTemporaryStorage){
  if ((this->pathsRequested) and (this->useTemporaryStorage != useTemporaryStorage))
    WARNclass("Changing the variable useTemporaryStorage after path names have been requested might cause problems. Especially the method \"TQPathManager::deleteLocalFile(...)\" will not work reliably. Try to avoid calling this method late during runtime.");
  this->useTemporaryStorage = useTemporaryStorage;
}

TString TQPathManager::getLocalDirectory() const {
  return this->localDirectory;
}

bool TQPathManager::setLocalDirectory(TString localDirectory){
  if (!localDirectory.EndsWith("/"))
    localDirectory.Append("/");

  if ((this->pathsRequested) and (this->localDirectory != localDirectory))
    WARNclass("Changing the variable localDirectory after path names have been requested might cause problems. Especially the method \"TQPathManager::deleteLocalFile(...)\" will not work reliably. Try to avoid calling this method late during runtime.");

  // Create it if it does not exist yet.
  if (!TQUtils::ensureDirectory(localDirectory)){
    WARNclass("Local directory %s could not be created. Do you have the necessary rights? Not setting localDirectory.", localDirectory.Data());
    return false;
  }

  this->localDirectory = localDirectory;
  if (this->useTemporaryStorage){
    DEBUGclass("TQPathManager uses the local directory %s for temporary storage.", localDirectory.Data());
  }
  return true;
}

void TQPathManager::setVerbose(bool verbose){
  this->verbose = verbose;
}

void TQPathManager::registerLocalPath(TString targetFile){
  targetFile = TQStringUtils::trim(targetFile);
  if (!isInLocalPaths_target(targetFile)){
    WARNclass("You request that even getTargetPath(%s) return a local path. Please call this method only after you first retrieve this local path. Otherwise it has no effect.", targetFile.Data());
    return;
  }
  alwaysReturnLocalPath[targetFile] = true;
}

void TQPathManager::unregisterLocalPath(TString targetFile){
  targetFile = TQStringUtils::trim(targetFile);
  if (!isInLocalPaths_target(targetFile)){
    WARNclass("You request that getTargetPath(%s) not return a local path. Since no local path has ever been created, there should be no reason to call this method. Please check your code.", targetFile.Data());
    return;
  }
  if (!alwaysReturnLocalPath[targetFile]){
    DEBUGclass("alwaysReturnLocalPath[%s] is already false", targetFile.Data());
    return;
  }
  alwaysReturnLocalPath[targetFile] = false;
}

std::string TQPathManager::getTargetPath(TString targetFile, bool printWarnings){
  gSystem->ExpandPathName(targetFile);
  targetFile = TQStringUtils::trim(targetFile);

  if (isInLocalPaths_target(targetFile)){
    if (alwaysReturnLocalPath[targetFile]){
      DEBUGclass("Returning local path, because alwaysReturnLocalPath[%s] == true", targetFile.Data());
      return getLocalPath(targetFile);
    }
    else if (printWarnings){
      WARNclass("You are requesting the target path for a file that is already assigned temporary storage. Make sure that you don't copy files over.");
    }
  }
  TString targetPath = getTargetPathName(targetFile);
  DEBUGclass("Target path found: %s.", targetPath.Data());

  ////////////////////////////
  if (targetPath.BeginsWith("//")){
    ERRORclass("Congratulations: you just found a bug! Please post the following lines starting with ERROR on this website: https://gitlab.cern.ch/atlas-caf/CAFCore/issues/54. If possible, please also tell us your current commit of CAFCore (find it out by going into your CAFCore directory and typing \"git rev-parse HEAD\").");
    ERRORclass("$CAFOUTPUTDIR = %s", TQUtils::readEnvVarValue("CAFOUTPUTDIR").c_str());
    ERRORclass("this->pwd = %s", this->pwd.Data());
    ERRORclass("gSystem->pwd() = %s",  gSystem->pwd());
    ERRORclass("$PWD = %s", TQUtils::readEnvVarValue("PWD").c_str());
    ERRORclass("targetPath = %s", targetPath.Data());
    ERRORclass("this->useTemporaryStorage = %s", this->useTemporaryStorage ? "true" : "false");
    ERRORclass("this->filenameIndex = %i", this->filenameIndex);
    ERRORclass("this->pathsRequested = %s", this->pathsRequested ? "true" : "false");
    targetPath = targetPath.Replace(0, 1, "", 0);
    ERRORclass("Trying to copy your output file to %s. If this does not look correct, please log out of the current shell and log back in to temporarily fix this error.", targetPath.Data());
  }
  ////////////////////////////

  if (!isInTargetPaths(targetPath))
    targetPaths.push_back(targetPath);
  this->pathsRequested = true;
  return targetPath.Data();
}


std::string TQPathManager::getLocalPath(TString filename){
  // 1. If temporary storage is disabled, return target path.
  // 2. If target filename is on record, return corresponding local filename.
  // 3. If target filename does not have a local equivalent, create the file
  //    and return the its path.
  //
  // Local files are saved in
  // "/tmp/<username>/<processNumber>_<filenameIndex>_<filename>".

  if (!this->useTemporaryStorage){
    DEBUGclass("You have temporary storage disabled. Return target path instead of local path.");
    return this->getTargetPath(filename);
  }

  // Remove leading and trailing spaces
  filename = TQStringUtils::trim(filename);

  // If file is already on record, return the corresponding local filename.
  if (this->isInLocalPaths_target(filename)){
    DEBUGclass("A local file for %s does already exist at %s.", filename.Data(), localPaths.at(filename).Data());
    return localPaths.at(filename).Data();
  }
  if (this->isInTargetPaths(this->getTargetPathName(filename))){
    WARNclass("You are requesting a local path for a file that a target path has been requested for. This file might copy another one over.");
  }
  if (this->isInLocalPaths_local(TQFolder::concatPaths(this->localDirectory, filename))){
    // This warning does not work if localDirectory is changed during runtime.
    WARNclass("You are trying to get a local file for a local file. This doesn't make much sense. Continuing anyway.");
  }

  // Local file needs to be created. Build the filename.
  int extensionPos = TQStringUtils::rfind(filename,".");
  TString extension = "";
  if (extensionPos != kNPOS) extension = filename(extensionPos,filename.Length()); //if it seems like there is a file name extension, save it and re-apply it after the hashing.
  TString localFilename = std::to_string(::getpid()) + "_" + std::to_string(TQPathManager::filenameIndex++) + "_" + std::to_string(filename.Hash()) + extension;
  TString localPath = TQFolder::concatPaths(this->localDirectory, localFilename);

  if (!TQUtils::ensureDirectoryForFile(localPath)){
    WARNclass("Cannot create directory for local file %s. Use global storage instead and return target path.", localPath.Data());
    return this->getTargetPath(filename);
  }

  localPaths[filename] = localPath;
  alwaysReturnLocalPath[filename] = false;
  DEBUGclass("Assigned local file %s to target file %s.", localPath.Data(), filename.Data());
  this->pathsRequested = true;
  return localPath.Data();
}

std::string TQPathManager::findConfigPath(TString filename, bool errorMessageIfNotFound, bool searchExecutionDir){
  // Filenames with TQFolder syntax e.g. folder.root:objectinside>>::label are supported
  // in these cases, only folder.root will be used for finding the config
  // and the absolute path including :objectinside>>::label will be returned.
  // You can ignore looking for configs in the execution directory with
  // setting ignoreExecutionDirectory to true.

  if (filename == "") {
    WARNclass("Cannot find config from an empty string! Returning empty string.");
    return "";
  }

  std::vector<TString> splittedFileName = TQStringUtils::split(filename, ".root:");
  TString filenameBase;
  TString stringTail = "";
  if (splittedFileName.size() == 2) {
    filenameBase = TQStringUtils::concat(splittedFileName[0],".root");
    stringTail = TQStringUtils::concat(":", splittedFileName[1]);
  } else if (splittedFileName.size() ==1) {
    filenameBase = filename;
  } else {
    ERRORclass("Something odd happened. You are trying to find the path for the config file '%s' and is seems to have two times the identifier '.root:' in it. This is not supported, please check your code!", filename.Data());
  }
  //@env: [CAFANALYSISSHARE] Path to the "share" directory of the analysis package. If paths to config files,... 
  //@env are given as relative paths they will be searched for inside this directory unless a file with the given
  //@env relative paths exists relative to the current working directory.
  TString filePath = TQPathManager::findFileFromEnvVar_internal(filenameBase, "CAFANALYSISSHARE", true, searchExecutionDir);
  if (filePath == "" && errorMessageIfNotFound && TQUtils::readEnvVarValue("CAFANALYSISSHARE") == "") {
    WARNclass("Did you forget to set the environment variable 'CAFANALYSISSHARE' in your analysis package?");
  }

  TString returnFileName = filePath+stringTail;
  return returnFileName.Data();
}

bool TQPathManager::deleteLocalFile(TString filename){
  // 1. If temporary storage is disabled,
  // 1.1 if target path is on record, delete it instead of local path and return true,
  // 1.2 else return false
  // 2. If file is not on record, return false.
  // 3. Otherwise delete the file, erase the entry from localPaths and return true.
  //
  // Returns true if the file is on record (either in local or target location),
  // otherwise returns false. This does not necessarily say that a file has been
  // deleted.
  //
  // This method does not work properly if the variables useTemporaryStorage or
  // localDirectory are changed after paths have been requested.


  // Remove leading and trailing spaces
  filename = TQStringUtils::trim(filename);
  TString pathToDelete = "";

  // Check if temporary storage is enabled
  if (!this->useTemporaryStorage){
    DEBUGclass("You have temporary storage disabled. Deleting target file instead of local file.");
    pathToDelete = this->getTargetPathName(filename);
    if (isInTargetPaths(pathToDelete))
      this->deleteTargetPathEntry(pathToDelete);
  }
  else{
    // If temporary storage is enabled, check if there is a file to delete.
    // If not, return false.
    if (!this->isInLocalPaths_target(filename)){
      DEBUGclass("The file %s does not have a local file. Cannot delete it.", filename.Data());
      return false;
    }
    pathToDelete = localPaths.at(filename);
    // Remove the entry from localPaths.
    localPaths.erase(filename);
    alwaysReturnLocalPath.erase(filename);
    DEBUGclass("Deleted the database entry corresponding to %s.", filename.Data());
  }

  // Delete file if it exists (it might not exist, if user requested a local
  // file, but never wrote anything).
  if (TQUtils::fileExists(pathToDelete)){
    remove(pathToDelete.Data());

    if (!this->useTemporaryStorage)
      DEBUGclass("Deleted the file %s", pathToDelete.Data());
    else{
      DEBUGclass("Deleted the local copy of %s.", filename.Data());
    }
  }
  else{
    DEBUGclass("Nothing to delete. The file %s does not exist.", pathToDelete);
  }

  return true;
}


//////////////// Constructor and Destructor ////////////////


TQPathManager::TQPathManager(bool useTemporaryStorage){
  // Construct local output directory.
  
  //@env:[CAF_TEMP_DIR] Temporary directory CAF outputs are written to during runtime 
  //@env (ideally some fast, node-local scratch space) before copying them to their 
  //@env user-itended location at the end of execution. If not set (or resolving to an
  //@env empty string) '/tmp/<username>' will be used
  TString tempDir = TQUtils::readEnvVarValue("CAF_TEMP_DIR").c_str();
  if (tempDir.Length()==0) { 
    TString username = TQUtils::readEnvVarValue("USER").c_str();
    if (username == ""){
      // if username is not accessible, use "CAF_temporary"
      username = "CAF_temporary";
    }
    tempDir = TQUtils::readEnvVarValue("TMP");
    if (tempDir.Length()==0) {
      tempDir = "/tmp/";
    }
    tempDir = TQFolder::concatPaths(tempDir, username);
  }
  TQStringUtils::ensureTrailingText(tempDir,"/");
  this->initialize(tempDir, useTemporaryStorage);
}

TQPathManager::TQPathManager(TString localDirectory, bool useTemporaryStorage){
  this->initialize(localDirectory, useTemporaryStorage);
}

TQPathManager::~TQPathManager(){
  // 1. Loop over all pairs of local and target files. Copy all local files to
  //    their target location.

  if (localPaths.size() > 0)
    INFOclass("Copying files from local storage to target location: %s.", this->determineTargetDir().Data());

  for (const auto& it:localPaths){
    TString targetPath = getTargetPathName(it.first);
    if (targetPath.EqualTo("")){
      WARNclass("Couldn't create target path. Not copying files.");
      break;
    }
    TString localPath = it.second;
    if (TQUtils::fileExists(localPath)){
      DEBUGclass("Copying %s to %s.", localPath.Data(), targetPath.Data());
      if (verbose) INFO("Writing file to %s.", targetPath.Data());
      if (!TQUtils::ensureDirectoryForFile(targetPath)){
	WARNclass("Cannot create the directory for the file %s. Trying to copy anyway.", targetPath.Data());
      }
      if (TQUtils::fileExists(targetPath)){
	DEBUGclass("There is already a file at %s. Copying it over.", targetPath.Data());
	remove(targetPath.Data());
      }

      // Used
      //   gSystem->Rename(localPath.Data(), targetPath.Data());
      // before for renaming. But I couldn't find the implementation and do not
      // know what it does if file cannot be renamed. Therefore use "rename".

      // Try to just rename the file. If the target path is on another
      // volume, the function returns a non-zero value. In that case, copy the
      // file and delete it afterwards.
      if (rename(localPath, targetPath) != 0){
	DEBUGclass("File could not be renamed. Copying instead.");
	gSystem->CopyFile(localPath.Data(), targetPath.Data(), kTRUE);
	remove(localPath.Data());
      }
    }
    else{
      WARNclass("You used \"TQPathManager::getLocalPath()\" to request a filename, but it seems that you did not create the file. Is this a logical error in your code? The file would have been written to %s.", targetPath.Data());
      DEBUGclass("Cannot copy %s to %s. The file does not seem to exist.", localPath.Data(), targetPath.Data());
    }
  }
  if (this == &TQPathManager::gPathManager){
    DEBUGclass("The static member path manager is being destroyed.");
    TQPathManager::gPathManagerExists = false;
  }
  else{
    DEBUGclass("A path manager that is not the static member is being destroyed.");
  }
}

std::vector<TString> TQPathManager::getAllTargetPaths(){
  std::vector<TString> allTargetPaths;

  allTargetPaths = this->targetPaths;

  for (const auto& it:this->localPaths){
    allTargetPaths.push_back(getTargetPathName(it.first));
  }
  return allTargetPaths;
}


///////////////////// private methods //////////////////////


TString TQPathManager::getTargetPathName(TString targetFile) const {
  if (gSystem->IsAbsoluteFileName(targetFile.Data())){
    return targetFile;
  }

  TString targetDirectory = this->determineTargetDir();
  TString targetPath = TQFolder::concatPaths(targetDirectory, targetFile);
  if (!TQUtils::ensureDirectoryForFile(targetPath)){
    ERRORclass("Cannot create directory for file %s.", targetPath.Data());
  }

  return targetPath;
}

bool TQPathManager::isInLocalPaths_target(TString filename) const {
  // Find out if filename is already in list of target files (i.e. if there is
  // a local file associated with this target file).

  // Remove leading and trailing spaces.
  filename = TQStringUtils::trim(filename);


  // Maybe there is an easier way to check a map for an entry. Anyway, this one
  // works reliably.
  TString localPathname;
  try{
    localPathname = localPaths.at(filename);
  }
  catch(const std::out_of_range& error){
    // If map access throws an error, then the filename is not in the list.
    return false;
  }

  // If the pathname is not empty, return true.
  if (!localPathname.EqualTo(""))
    return true;
  return false;
}

bool TQPathManager::isInLocalPaths_local(const TString& filePath) const {
  // Find out if filePath is already in list of local paths (i.e. if there is
  // a target file associated with this local file).

  for (const auto& it:localPaths){
    if (it.second == filePath)
      return true;
  }
  return false;
}

bool TQPathManager::isInTargetPaths(const TString& filePath) const{
  if (std::find(targetPaths.begin(), targetPaths.end(), filePath) == targetPaths.end())
    return false;
  else
    return true;
}

void TQPathManager::deleteTargetPathEntry(const TString& filePath){
  if (!isInTargetPaths(filePath)){
    DEBUGclass("Cannot delete target path entry %s, because it does not exist.", filePath.Data());
    return;
  }

  std::vector<TString>::iterator it = std::find(targetPaths.begin(), targetPaths.end(), filePath);
  targetPaths.erase(it);
  return;
}

TString TQPathManager::determineTargetDir() const {
  //@env:[CAFOUTPUTDIR] Directory which outputs produced by various CAF classes and tools should end up in
  TString targetDirectory = TQUtils::readEnvVarValue("CAFOUTPUTDIR");
  if (targetDirectory.EqualTo(""))
    return this->pwd;

  if (!gSystem->IsAbsoluteFileName(targetDirectory.Data())){
    WARNclass("The path %s specified in the environment variable \"CAFOUTPUTDIR\" is not an absolute directory. Write output in working directory instead.", targetDirectory.Data());
    return this->pwd;
  }

  if (!TQUtils::ensureDirectory(targetDirectory)){
    WARNclass("The path %s specified in the environment variable \"CAFOUTPUTDIR\" cannot be created. Write output in working directory instead.", targetDirectory.Data());
    return this->pwd;
  }

  return targetDirectory;
}

void TQPathManager::initialize(const TString& localDirectory, bool useTemporaryStorage){
  DEBUGclass("Creating instance of TQPathManager with local directory %s%s using temporary storage.", localDirectory.Data(), useTemporaryStorage ? "" : ", but not");

  this->useTemporaryStorage = useTemporaryStorage;
  this->pathsRequested = false;

  // Set current working directory and try to create output file.
  this->pwd = gSystem->pwd();
  if ((!gSystem->IsAbsoluteFileName(this->pwd.Data())) or (!TQUtils::directoryExists(this->pwd))){
    WARNclass("Current working directory could not be determined. This is not a problem if the environment variable \"CAFOUTPUTDIR\" is set.");
  }
  DEBUGclass("The current working directory is set to %s.", this->pwd.Data());
  if (!setLocalDirectory(localDirectory)){
    this->useTemporaryStorage = false;
  }
  this->verbose = false;

  if (this == &TQPathManager::gPathManager){
    DEBUG("The static member path manager is being created.");
    TQPathManager::gPathManagerExists = true;
  }
  else{
    DEBUG("A path manager that is not the static member is being created.");
  }
}
