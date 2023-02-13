//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef STATISTICS_MANAGER
#define STATISTICS_MANAGER

#include "TString.h"
#include <map>

class RooWorkspace;

class TQSampleFolder;
class TQFolder;

class TSStatisticsManager {
 public:
  class Action { // nested
  protected:
    virtual bool execute(TQFolder* config) const = 0;
    mutable TSStatisticsManager* manager = NULL;

    TQFolder* models () const;
    TQFolder* workspaces () const;
    TQFolder* results () const;

  public:
    int perform(TQFolder* config, TSStatisticsManager* manager) const;
  };
  
  static bool registerAction(const Action* a, const char* name);
  static void printActions();
  static const std::map<const std::string,const TSStatisticsManager::Action*>& getActions();
  friend Action;

 protected:

		TQFolder * fModels;
		TQFolder * fWorkspaces;
		TQFolder * fResults;
		TQSampleFolder * fDefaultSampleFolder;

		Int_t fFileCompression;

 public:
		void info(TString message);
		void error(TString message);
		void warn(TString message);

		TSStatisticsManager();

    void reset();

    void setDefaultSampleFolder(TQSampleFolder * sampleFolder);
    TQSampleFolder * getDefaultSampleFolder();

		TQFolder * getModels();
		TQFolder * getWorkspaces();
		TQFolder * getResults();

    void  setHistogramsFileCompression(Int_t compression);
    Int_t getHistogramsFileCompression();

    Bool_t run(TQFolder * config);

		virtual ~TSStatisticsManager();
		
		ClassDef(TSStatisticsManager, 0);

};

#endif
