#ifndef __TQPathManager__
#define __TQPathManager__

#include <iostream>
#include <map>
#include <string>
#include "TString.h"

class TQPathManager{
public:

  ///////////////////// static methods /////////////////////


 
  // Like findFileFromList(...). Instead of a string of paths, it takes an
  // environment variable as input and searches the paths therein.
  static std::string findFileFromEnvVar(const TString& filename, const std::string& envVar, bool printWarnings = true);

  // This method does not return results relative to the execution directory.
  static std::string findFileFromEnvVarWithoutExecDir(const TString& filename, const std::string& envVar, bool printWarnings = true);

  // Searches a list of paths (variable searchPath, different paths separated
  // by ":") for filename. When the first file has been found, stop and return
  // its absolute path.
  static std::string findFileFromList(TString filename, const std::string& searchPathList, bool printWarnings = true);

  // This method does not return results relative to the execution directory.
  static std::string findFileFromListWithoutExecDir(TString filename, const std::string& searchPathList, bool printWarnings = true);

  // Returns gPathManager. This instance is automatically created and supposed
  // to be used to handle paths. Don't create your own instance.
  static TQPathManager* getPathManager();

private:
  
  // internal implementation of findFileFromEnvVar
  static std::string findFileFromEnvVar_internal(const TString& filename, const std::string& envVar, bool printWarnings, bool searchExecutionDir);

  // internal implementation of findFileFromList
  static std::string findFileFromList_internal(TString filename, const std::string& searchPathList, bool printWarnings, bool searchExecutionDir);
  
  // auxilliary variable to store result of reading the CAFUSELOCALSTORAGE 
  // env var while loading the library
  static const bool gUseTemporaryStorageDefault;
  
  //////////////////////////////////////////////////////////

public:
  bool getUseTemporaryStorage() const;
  void setUseTemporaryStorage(bool useTemporaryStorage);
  TString getLocalDirectory() const;
  bool setLocalDirectory(TString localDirectory);
  void setVerbose(bool verbose);
  void setWorkingDirectory(TString dir);

  // If you have retrieved a local path before, this method will tell the path
  // manager that it exists. As long as the file is registered, the path manager
  // will always return the local path and never the target path.
  // Note that the argument is not a local path, but the targetFile (the same
  // argument that you used to call getLocalPath(...) in the first place).
  void registerLocalPath(TString targetFile);
  void unregisterLocalPath(TString targetFile);

  // Returns the full path of the target filename (i.e. path where output will
  // be saved for user).
  std::string getTargetPath(TString filename, bool printWarnings=true);

  // Returns the full path of a local file. If something is written to this
  // local file, copy it to the target location in the destructor.
  std::string getLocalPath(TString filename);

  // Find the config file "filename"
  std::string findConfigPath(TString filename, bool errorMessageIfNotFound = true, bool searchExecutionDir = true);

  // Deletes the local file that corresponds to the target file given as
  // argument and returns true. Returns false if the target file has never been
  // used to create a local file.
  bool deleteLocalFile(TString filename);

  // Returns vector of all target paths of files (local and target) that have
  // been requested and not deleted. This might contain requested files that
  // were never written by the user.
  std::vector<TString> getAllTargetPaths();

  // Constructor: Show a warning if environment variable CAFOUTPUTDIR cannot
  // be read.
  // gUseTemporaryStrageDefault is read from env vars "CAFUSETEMPDIR" or 
  // (at lower priority) "CAFUSELOCALSTORAGE" upon loading the library
  TQPathManager(bool useTemporaryStorage = gUseTemporaryStorageDefault);
  TQPathManager(TString localDirectory, bool useTemporaryStorage = gUseTemporaryStorageDefault);

  // Destructor: Copy all local files to target location.
  virtual ~TQPathManager();

  ClassDef(TQPathManager, 0);

private:
  
  
  // Counter to give local files a unique name.
  static int filenameIndex;

  // Pointer to instance of TQPathManager that is used throughout the entire
  // analysis. Don't create another instance.
  static TQPathManager gPathManager;
  static bool gPathManagerExists;

  // This vector has the names of all target paths that have been returned by
  // getTargetPath.
  std::vector<TString> targetPaths;

  // Holds name of working directory. Is only set once at the beginning. Used
  // as default for output if $CAFOUTPUTDIR is not specified.
  TString pwd;

  // This map holds the link between target files (relative to environment
  // variable CAFOUTPUTDIR) and local paths (absolute path).
  // < targetFile, localPath >
  std::map<TString, TString> localPaths;

  // This map works in parallel to localPaths. If getTargetPath() is called,
  // targetFile is in the map and the bool is true, then the local path is
  // returned. This behavior can be requested with alwaysReturnLocalPathFor().
  std::map<TString, bool> alwaysReturnLocalPath;

  // Local files are written in this directory.
  TString localDirectory;

  // difference to getTargetPath(...): does not store file path in targetPaths
  TString getTargetPathName(TString targetFile) const;

  // Reads $CAFOUTPUTDIR and decides if it's good to use. If not, it returns
  // $PWD.
  TString determineTargetDir() const;

  // Returns true if the filename is in localPaths among target files
  bool isInLocalPaths_target(TString filename) const;

  // Returns true if the filePath is in localPaths among local paths.
  bool isInLocalPaths_local(const TString& filePath) const;

  // Returns true if the filePath is in targetPaths.
  bool isInTargetPaths(const TString& filePath) const;

  // Erases this entry from targetPaths.
  void deleteTargetPathEntry(const TString& filePath);

  // Called by constructors.
  void initialize(const TString& localDirectory, bool useTemporaryStorage);

  // Use the feature of temporary storage. If false, getLocalPath(...) has the
  // same functionality as getTargetPath(...).
  bool useTemporaryStorage;

  // Indicates if paths have been requested (false in constructor, true after
  // after getLocalPath or getTargetPath are called).
  bool pathsRequested;

  // Write verbose output
  bool verbose;
};

#endif
