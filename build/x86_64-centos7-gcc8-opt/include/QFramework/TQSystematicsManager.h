//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef TQSYSTEMATICSMANAGER_H
#define TQSYSTEMATICSMANAGER_H

#include "QFramework/TQFolder.h"
#include "TH1.h"
#include "TString.h"

class TQSystematicsManager {

  TQFolder* histos;
  TString sysPrefix = "Systematics";
  TString varPrefix = "Variations";
  
public:
  void setSystematicPrefix(const TString& prefix);
  void setVariationPrefix(const TString& prefix);  
  
  struct HistogramVariationComputer {
    const TH1* fBaseline = NULL;
    TH1* fDelta = NULL; 
    TH1* fRelativeDelta = NULL;
    
    HistogramVariationComputer(const TH1* base);
    ~HistogramVariationComputer() {
      delete fDelta;
      delete fRelativeDelta;
    }

    // compute histogram variation
    bool computeVariation(TObjArray* vars, TString mode);
    bool computeSingle(TObjArray* vars);
    bool computeDifference(TObjArray* vars);
    bool computeMinimum(TObjArray* vars);
    bool computeMaximum(TObjArray* vars);
    bool computeEnvelope(TObjArray* vars);
    bool computeStdDev(TObjArray* vars);
    bool computeHessian(TObjArray* vars);
    // manually set statistical error
    void useStatError(TH1* error, bool relative=true);
    // apply var = nominal +/- delta
    TH1* applyVariation(const TH1* nom, double c=1.0, bool relative=true);
  };

  // store histograms
  // raw variation, e.g. .Histograms/JET_JES_EffectiveNP1__1up
  TString storeVarHisto(TH1 * histo, const TString& nameVariation, const TString& nameChannel);
  TString storeVarHisto(TH1 * histo, const TString& path);  
  // computed systematic variation, e.g. .Histograms/Systematics/ATLAS_JES_EffectiveNP_1/Up
  TString storeSysHisto(TH1 * histo, const TString& nameVariation, const TString& direction, const TString& nameChannel);
  TString storeSysHisto(TH1 * histo, const TString& path);  
  TH1 * getVarHisto(const TString& nameVariation, const TString& nameChannel, const TString& nameSample, bool verbose = true);
  TH1 * getVarHisto(const TString& path, bool verbose = true);  
  TH1 * getSysHisto(const TString& path, bool verbose = true);
  TH1 * getHisto(TString path, bool verbose = true);    

  Bool_t isSystBlacklisted(TQFolder* sys, const std::vector<TString>&  systBlacklist, const std::vector<TString>& types = {"OverallSys","HistoSys"});
  bool processHistoSys_smoothVariation(TQTaggable* parameters, TQFolder* sys, TH1* nom, TH1* up, TH1* down);
  bool processHistoSys_systsToSymmetrizeBins(TQTaggable* parameters, TQFolder *sys, TH1* nom, TH1* up, TH1* down);
  bool processHistoSys_systsToSymmetrizeMaxBins(TQTaggable* parameters, TQFolder *sys, TH1* nom, TH1* up, TH1* down, bool force = false);
  bool processHistoSys_isFlat(TQTaggable* parameters, TQFolder* sys, TH1* ratio);  
  bool processHistoSys_checkCutoff(TQTaggable* parameters, TH1* ratio);
  
protected:
  // histogram variation computation
  TH1* computeHistogramVariation(TQFolder* computeFolder, const TString& nameChannel, const TString& nameSample);
  // bins to smooth histograms manually
  std::vector<int> getSmoothedBinBorders(TQFolder* sysConfig);

  // shape variation normalization/magnification
  TH1* normalizeShapeVariation(const TH1* h_Var, const TH1* h_Nom, const TH1* h_Sym);
  void magnifyShapeVariation(TH1*& shape_Var, const TH1 * h_Nom, Double_t magnification);

  bool smoothHist(const TString& alg, TH1* nom, TH1* var);
  
public:  
  // custom/default mode as appropriate
  bool includeOverallSys(TQFolder* sysFolder, TQFolder* sys, const TString& nameChannel, const TString& nameSample);
  bool includeHistoSys(TQFolder* sysFolder, TQFolder* sys, const TString& nameChannel, const TString& nameSample);

  // systematic processing switches
  bool isBlacklistedSys(TQFolder* sys, const std::vector<TString>&  systBlacklist, const std::vector<TString>& types = {"OverallSys","HistoSys"});
  bool isNegligibleSys(TQFolder* sys, TQTaggable* parameters);
  bool isProtectedSys(TQFolder* sys, TQTaggable* parameters, const TString& prefix) const;

  // post-process included systematics
  bool processOverallSys(TQTaggable* parameters, TQFolder* sys);
  bool processHistoSys  (TQTaggable* parameters, TQFolder* sys);
  
  static void info(TString message);
  static void error(TString message);
  static void warn(TString message);
  
 public:
  TQSystematicsManager(TQFolder* h) : histos(h) {};
  TQSystematicsManager() : TQSystematicsManager(NULL) {};
  void setRepository(TQFolder* histos);
  TQFolder* repository();
  
  virtual ~TQSystematicsManager() = default;
	
  ClassDef(TQSystematicsManager, 0);
  
};

  

#endif
