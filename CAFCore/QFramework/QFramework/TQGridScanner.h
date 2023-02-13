#ifndef __TQ_GRIDSCANNER__
#define __TQ_GRIDSCANNER__

#include <vector>

#include "TString.h"
#include "TMVA/Timer.h"

#include "QFramework/TQGridScanPoint.h"
#include "QFramework/TQGridScanResults.h"
#include "QFramework/TQFolder.h"
#include "QFramework/TQSignificanceEvaluator.h"
#include "QFramework/TQTaggable.h"

class TQGridScanObservable;

class TQGridScanner : public TQTaggable, public TNamed {
public:
  using ObsVec = std::vector<TQGridScanObservable*>;
  using CutBounds = std::vector<std::pair<int, int>>; // bin numbers
  using SplitCutVals = std::vector<double>;
  using SplitCutBins = std::vector<int>;
  using BoundDirection = TQGridScanBound::Direction;

  // This default constructor is for compatibility with TBufferFile::WriteObject
  TQGridScanner() {}

  TQGridScanner(const TString& name, TQSignificanceEvaluator* evaluator);
  TQGridScanner(const TString& name, TQSignificanceEvaluator* evaluator, TList* obsToScan);
  ~TQGridScanner();

  // Adds a multidimensional histogram to be used as signal(bkg) in significance computation
  // This is done nominally by TQSignificanceEvaluator, for both signal and background
  // Returns false if there exists already a signal(bkg) hist, otherwise true
  bool addSignalHist(THnBase* hist);
  // See addSignalHist
  bool addBkgHist(THnBase* hist);

  // Accesses an observable by name
  TQGridScanObservable* getObs(const TString& obsName);

  // Acess observable names that are scanned
  std::vector<TString>  obsNamesToScan() {return m_obsNamesToScan;}

  // Add names for FOMs that are being evaluated
  void addFOMDefinitions(const std::vector <TString>& definitions);

  // Processes the user-defined bounds and creates some intermediate data structures, then
  // starts the grid scan
  void run();
  TQGridScanResults* results() { return &m_results; }

  void plotAndSaveAllSignificanceProfiles(int topNumber, const TString& options);
  void plotAndSaveSignificanceProfile(BoundDirection direction, int i, int topNumber, const TString& options);
  std::unique_ptr<TH1F> getSignificanceProfile(BoundDirection direction, int i, int topNumber);

  // TODO: implement verbose logging
  void setVerbose(bool v = true) { m_verbose = v; }
  std::vector<TQGridScanPoint>& points() { return m_points; }
  const TString& nDimHistName() const { return m_nDimHistName; }

  void extractInputHistogramProjections();
  void dumpInputHistogramProjections(TQTaggable& tags);
  
  bool hasSplitObs = false;

protected:
  void init();
  // This function skips over points not fulfilling some user-defined criteria when plotting and
  // printing points
  // TODO: implement this
  bool isAcceptedPoint(const TQGridScanPoint& point);

  // Scans over normal bounds (see TQGridScanObservable) recursively, then passes to split bounds
  // if they exist, otherwise creates a scan point (TQGridScanPoint)
  void scan(CutBounds::iterator obsValsIter, ObsVec::iterator obsIter);
  // Calculates the significance for the set of all split observables (scan and fixed) by
  // recursively applying either side of each split cut 
  double splitSignif2(SplitCutBins::iterator obsBinsIter, ObsVec::iterator obsIter, bool isScanSplit);
  // Scans over the scan (as opposed to fixed) split observables, in preparation to call splitSignif2 
  void scanSplit(SplitCutBins::iterator obsBinsIter, ObsVec::iterator obsIter);

  // This is simply a helper function used by splitSignif2
  double splitSignifHandleRecursion(
      bool isFinalLevel,
      bool isScanSplit,
      SplitCutBins::iterator obsBinsIter,
      ObsVec::iterator obsIter
  );

  // Updates the heartbeat
  bool updateHeartbeat();

  // print progress of scan to console
  void printProgress();

  // The map of observables is generated automatically in `init`
  // std::vector< std::map<TString, TQGridScanObservable*> > m_observables_regions;
  std::vector<TString> m_obsNamesToScan;
  std::map<TString, TQGridScanObservable*> m_observables;
  std::vector< THnBase*> m_signalHists;
  std::vector< THnBase*> m_bkgHists;
  TQSignificanceEvaluator* m_evaluator = nullptr;
  std::vector<TQGridScanPoint> m_points;

  // These hold pointers to the observables of normal bounds and split vals, respectively
  ObsVec m_normalObs;
  ObsVec m_splitObs;
  ObsVec m_splitScanObs;

  // Vectors that hold current values for bins/normal bounds/split vals as the scan iterates through them
  // Design chosen for performance
  CutBounds m_normalBounds;
  // These hold the bin numbers and bin edge values for the non-scan split observables
  SplitCutBins m_splitBins;
  // These hold the bin numbers and bin edge values for the scan split observables
  SplitCutBins m_splitScanBins;

  TString m_heartBeatCommand;
  unsigned long m_heartBeatInterval;
  unsigned long m_heartbeat;
  bool m_sorted = false;

  bool m_verbose = false;

  TQGridScanResults m_results;
  TMVA::Timer m_runTimer;
  int m_nPoints = 1;
  int m_nPointsProcessed = 0;
  TString m_nDimHistName;

  ClassDefOverride(TQGridScanner,3) // helper class to facilitate cut optimization scans
};

#endif
