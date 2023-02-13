//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQLWTNNOBSERVABLE__
#define __TQLWTNNOBSERVABLE__
#include <vector>
#include <map>
#include <string>

#include "TString.h"
#include "QFramework/TQObservable.h"

//#include "lwtnn/LightweightGraph.hh"
//#include "lwtnn/parse_json.hh"

//forward declarations
#ifndef __CINT__
namespace lwt {
  class LightweightNeuralNetwork;
  class LightweightGraph;
}
#endif
namespace TQObservableFactory {
  bool setupCAFlwtnn();
}

class TQlwtnnObservable : public TQObservable {
protected:
  // put here any data members your class might need
  bool fSequential = false;
  std::map<std::string,TQObservable*> fObservables; //!
  
  std::vector<std::string> fOutputNames;
  std::string fNetworkFileName;
  std::string fInputNodeName = "";//!
  #ifndef __CINT__
  //only one of the two following should ever be actually used at any time.
  lwt::LightweightNeuralNetwork *fNetwork = nullptr; //!
  lwt::LightweightGraph *fGraph = nullptr; //!
  // cache for networks
  static std::map<std::string, lwt::LightweightNeuralNetwork*> fNetworkCache; //!
  static std::map<std::string, lwt::LightweightGraph*> fGraphCache; //!
  static std::map<std::string, std::string> fInputNodeNameCache; //!
  static std::map<std::string, std::map<std::string, TString>> fObservableExpressionCache; //!
  #endif

  mutable Long64_t fCachedEntry; //!
  mutable std::map<std::string, double> fCachedValue; //!
public:
  inline virtual TQObservable::ObservableType getObservableType() const override {return fOutputNames.size()==1? TQObservable::ObservableType::scalar : TQObservable::ObservableType::vector;}
  virtual int getNevaluations() const override;
  virtual double getValue() const override;
  virtual double getValueAt(int i) const override;
  virtual TObjArray* getBranchNames() const override;
  virtual Long64_t getCurrentEntry() const override;
  
protected:
  void evalFunctional(std::map<std::string,double>& output) const;
  void evalSequential(std::map<std::string,double>& output) const;
  bool initializeFunctional(const std::string& fileName);
  bool initializeSequential(const std::string& fileName);
  bool initializeObservables(const std::string& cache_name);
  virtual bool initializeSelf() override;
  virtual bool finalizeSelf() override;
  void parseArguments(TString arguments);

public:
  void setOutputs(const std::string& outputNames);
  void setOutputs(const TString& outputNames);
  void setOutputs(const std::vector<std::string> outputNames);
  void setSequential(bool sequential) {this->fSequential = sequential;};
  TQlwtnnObservable();
  TQlwtnnObservable(const TString& name);
  virtual ~TQlwtnnObservable();
  
  DECLARE_OBSERVABLE_FACTORY(TQlwtnnObservable,TString expression)
  
  ClassDefOverride(TQlwtnnObservable, 0);

};
#endif //> !__TQLWTNNOBSERVABLE__
