//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef TS_UTILS
#define TS_UTILS

class RooAbsReal;
class TQFolder;
class TGraph;
class RooAbsCollection;
class RooWorkspace;
class RooFitResult;
class RooDataSet;
class RooAbsPdf;
class RooArgSet;
class RooArgList;
class RooAbsArg;
class RooRealVar;
class RooLinkedList;
class PiecewiseInterpolation;
class RooHistFunc;
class RooDataHist;
class RooSimultaneous;
class RooProdPdf;
class RooProduct;
class RooRealSumPdf;
class TH1;
class TH2;
class RooListProxy;
namespace ROOT { //EXCLUDE
  namespace Fit { //EXCLUDE
    class FitConfig;
  }
}


#include <RooAbsCollection.h>
#if ROOT_VERSION_CODE < ROOT_VERSION(6,18,0)
struct RooAbsCollection_IteratorHelper { //EXCLUDE
  RooFIter itr;
  RooAbsArg* nextItem;
  RooAbsCollection_IteratorHelper(const RooAbsCollection& c, bool end);
  RooAbsArg* operator++();
  bool operator!=(const RooAbsCollection_IteratorHelper& other);
  bool operator!=(const RooAbsArg* other);
  RooAbsArg* operator*();
};
RooAbsCollection_IteratorHelper begin(const RooAbsCollection& c);
RooAbsCollection_IteratorHelper end(const RooAbsCollection& c);
#endif
struct RooLinkedList_IteratorHelper { //EXCLUDE
  RooFIter itr;
  RooAbsArg* nextItem;
  RooLinkedList_IteratorHelper(const RooLinkedList& c, bool end);
  RooAbsArg* operator++();
  bool operator!=(const RooLinkedList_IteratorHelper& other);
  bool operator!=(const RooAbsArg* other);
  RooAbsArg* operator*();
};
RooLinkedList_IteratorHelper begin(const RooLinkedList& c);
RooLinkedList_IteratorHelper end(const RooLinkedList& c);
struct RooFIter_IteratorHelper { //EXCLUDE
  RooFIter* itr;
  RooAbsArg* nextItem;
  RooFIter_IteratorHelper(RooFIter& it, bool end);
  RooAbsArg* operator++();
  bool operator!=(const RooFIter_IteratorHelper& other);
  bool operator!=(const RooAbsArg* other);
  RooAbsArg* operator*();
};
RooFIter_IteratorHelper begin(RooFIter& it);
RooFIter_IteratorHelper end(RooFIter& it);
struct TIterator_IteratorHelper { //EXCLUDE
  TIterator* itr;
  TObject* nextItem;
  TIterator_IteratorHelper(TIterator* it, bool end);
  TObject* operator++();
  bool operator!=(const TIterator_IteratorHelper& other);
  bool operator!=(const TObject* other);
  TObject* operator*();
};
TIterator_IteratorHelper begin(TObject* it);
TIterator_IteratorHelper end(TObject* it);



#include <RooMinimizer.h>

class TQTaggable;
class TSStatisticsManager;

#include "TMatrixDSym.h"
namespace RooStats { //EXCLUDE
  class ModelConfig;
}
#include "RooStats/HistFactory/FlexibleInterpVar.h" //forward declaration of the class doesN't quite work here as Cling / Root's dict generation doesn't cope well with nested namespaces...

namespace TSUtils {
  void getPlotRange(RooAbsReal* rv, double& min, double& max, int& nbins);
  const RooArgSet& getVariables(const RooAbsData* _ds);
  const TMatrixDSym* getCovarianceMatrix(const RooFitResult* fitResult);
  TVectorD getErrors(const TMatrixD& covariance);
  TVectorD getErrors(const RooAbsCollection* parameters);  
  TMatrixDSym makeCovarianceMatrix(const TMatrixD& correlation, const TVectorD& errors);
  TMatrixDSym makeCovarianceMatrix(const TMatrixD& correlation, const std::vector<double>& errors);  
  TMatrixDSym makeCorrelationMatrix(const TMatrixD& covariance);  
  const TMatrixDSym* getCorrelationMatrix(const RooFitResult* fitResult);
  TQFolder* convertGraph(TGraph* g);
  TQFolder* makeGraphFolder(const char* name,const std::map<double,double>& values);

  void applySettings(TQTaggable* config, const RooArgSet& allVars, RooStats::ModelConfig* mc);
  TQFolder* applyFitResult(RooAbsCollection* vars, TQFolder* fitResult);

  
  double diffParameterSets(TQFolder* p1, TQFolder* p2, bool verbose);

  RooLinkedList& getListOfSnapshots(RooWorkspace* ws);
  std::vector<TString> getListOfSnapshotNames(RooWorkspace* ws);
  
  bool takeSnapshot(RooWorkspace* workspace, const TString& name, TQTaggable* config = nullptr, TSStatisticsManager* manager = nullptr);
  
  void expandKeys(const RooAbsCollection* nuis, TQFolder * options);
  std::vector<TString> expandNames(const RooAbsCollection* nuis, const TString& filter, const TString& exceptions);
  
  int countConstParameters(const RooAbsCollection* params);
  int countFloatParameters(const RooAbsCollection* params);
  int countConstParameters(const RooAbsCollection& params);
  int countFloatParameters(const RooAbsCollection& params);

  RooAbsPdf* stripConstraints(RooProdPdf* product, const RooArgSet& observables, RooArgSet& constraints);  

  TH2* makeValidMatrix(TH2* orig, bool up, bool isCovariance, bool makeCorrelation, bool verbose);
  
  std::map<TString,double> getParameterValues(const RooAbsCollection& parameters);
  std::map<TString,double> getParameterValues(const RooAbsCollection* parameters);

  void setParametersConstant(const RooAbsCollection* params, bool constVal = true);
  void setParameterValues(const RooAbsCollection* parameters, double val);
  void setParameterValues(const RooAbsCollection* parameters, const RooAbsCollection* values);
  void setParameterValues(const RooAbsCollection& parameters, const RooAbsCollection* values);
  void setParameterValues(const RooAbsCollection* parameters, const RooAbsCollection& values);
  void setParameterValues(const RooAbsCollection& parameters, const RooAbsCollection& values);
  void setParametersConstFloat(const RooAbsCollection* parameters, const RooAbsCollection* values);
  void setParametersConstFloat(const RooAbsCollection& parameters, const RooAbsCollection* values);
  void setParametersConstFloat(const RooAbsCollection* parameters, const RooAbsCollection& values);
  void setParametersConstFloat(const RooAbsCollection& parameters, const RooAbsCollection& values);
  void setParameterValues(const RooAbsCollection* parameters, TQTaggable* options, const TString& prefix, bool forceSetConst = false);
  void setParameterValues(const RooAbsCollection& parameters, TQFolder* fitResult);
  void setParametersConstant(const RooAbsCollection* parameters, const std::vector<TString>& parnames, bool constval);

  void setParameterErrors(const RooAbsCollection* parameters, TQTaggable* options, const TString& prefix, bool forceSetConst = false);
  void addParameters(const RooAbsCollection* parameters, const std::vector<TString>& parnames, RooAbsCollection& pars);
  std::vector<TString> getParameterKeys(TQTaggable* tags, const TString& prefix);  
  
  template<class listT, class stringT> void getParameterNames(const listT* l,std::vector<stringT>& names);
  RooWorkspace* makeCleanWorkspace(RooWorkspace* oldWS, const char* newName, const char* mcname = "ModelConfig", bool copySnapshots=true, bool catchUnaccountedParameters=true);

  TQFolder * convertFitResults(RooFitResult * fitResult, TQTaggable* options = NULL, ROOT::Fit::FitConfig* fitConfig=NULL);
  TQFolder * convertParameterList(const RooAbsCollection * list, const TString& filter = "*");
  int convertParameterList(const RooAbsCollection * list, TQFolder* result_list, const TString& filter = "*");

  RooFitResult * prefitResults(RooAbsCollection* params);
  RooFitResult * convertFitResults(TQFolder* fitResult, RooAbsCollection * params = NULL);
  template<class T> RooFitResult * convertFitResults(TQFolder* fitResult, const std::vector<T>& params);  
  RooArgList* convertParameterList(TQFolder* folder, RooAbsCollection * params);

  void removeFloatingParameters(RooAbsCollection& set);
  void removeConstantParameters(RooAbsCollection& set);  
  
  TQFolder* modifyFitResults(TQFolder* fitResults, TQFolder* changes);
  RooAbsPdf * createHessePdf(const RooFitResult* fr, const RooAbsCollection* params);

  RooDataSet* getDataHistogram(RooAbsPdf* pdf, RooArgSet* observables, RooRealVar* weightVar = NULL, RooAbsArg* moreArgs = NULL);
  TH1* getHistogram(RooAbsPdf* pdf, RooArgSet* observables);

  std::vector<double> getBinning(RooAbsPdf* pdf, RooRealVar* obs);

  int nParameters(RooAbsReal* func);
  int nParameters(const RooMinimizer& minimizer);
  RooMinimizerFcn* getFunction(RooMinimizer& minimizer);

  
  double  getPropagatedError(RooAbsReal* var, const RooFitResult* fr, const double threshold = 0);

  TMatrixDSym getCorrelationMatrix(const TMatrixDSym& V);
  
  //moderatly hacky helpers to modify workspaces inplace
  std::vector<const RooAbsArg*>* getConstituents(const RooAbsArg* parent);
  std::set<const RooAbsArg*>* getContributionsRecursive(const RooAbsArg* rootNode, TClass* cl = nullptr, bool recurseMatching=false, std::set<const RooAbsArg*>* elements = nullptr);
  
  //helper classes to access some protected members of certain RooFit classes
 
  void printInfo(const PiecewiseInterpolation* _interp);
  RooAbsArg* getNominalMember(const PiecewiseInterpolation* _interp);
  RooListProxy* getParamSetMember(const PiecewiseInterpolation* _interp);
  RooListProxy* getHighSetMember(const PiecewiseInterpolation* _interp);
  RooListProxy* getLowSetMember(const PiecewiseInterpolation* _interp);

  //RooStats::HistFactory::FlexibleInterpVar
  Double_t* getNominalMemberPtr(const RooStats::HistFactory::FlexibleInterpVar* interp);
  Double_t getNominalMember(const RooStats::HistFactory::FlexibleInterpVar* interp);
  std::vector<double>* getHighMember(const RooStats::HistFactory::FlexibleInterpVar* interp);
  std::vector<double>* getLowMember(const RooStats::HistFactory::FlexibleInterpVar* interp);
  RooListProxy* getParamListMember(const RooStats::HistFactory::FlexibleInterpVar* interp);

  void printInfo(RooDataSet* _ds);

  void printBins(RooDataHist* _dh);
  void scaleHist(RooDataHist* _dh, double scale);
  void scaleDifferenceToNominal(RooDataHist* _var, const RooDataHist* _nom, double scale);

  //std::vector<const RooAbsArg*>* getConstituents(const RooSimultaneous* sim);
  void removeConstituentsFromRooSimultaneous(RooSimultaneous* sim, const TString& filter);
  void removeConstituentsFromRooRealSumPdf(RooRealSumPdf* pdf, const TString& filter);
  void removeConstituentsFromRooProdPdf(RooProdPdf* pdf, const TString& filter);
  void removeConstituentsFromRooProduct(RooProduct* prod, const TString& filter);
  void removeConstituentsFromPiecewiseInterpolation(PiecewiseInterpolation* interp, const TString& filter);
  void removeConstituentsFromFlexibleInterpVar(RooStats::HistFactory::FlexibleInterpVar* interp, const TString& filter);
  
  RooDataHist* getDataHist(const RooHistFunc* h);
  
  std::map<std::string,RooArgSet>* getNamedSets(RooWorkspace* ws);

  bool importObjectToWorkspace(RooWorkspace* ws, RooAbsArg* obj);
  bool importObjectToWorkspace(RooWorkspace* ws, RooAbsData* obj);

  std::map<std::vector<double> ,RooAbsReal*> slice(RooAbsReal* func, const std::vector<RooRealVar*>& observables, const RooArgList& nps, double relThreshold = 0);
}

#endif
