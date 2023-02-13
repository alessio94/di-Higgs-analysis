//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __CxAODReaderAlgorithm__
#define __CxAODReaderAlgorithm__

#include "QFramework/TQAlgorithm.h"
#include "QFramework/TQToken.h"

#include "AsgTools/IAsgTool.h"
#include "AsgTools/ToolHandle.h"

#include <string>

#include "AssociationUtils/OverlapRemovalInit.h"
#include "AssociationUtils/OverlapRemovalTool.h"
#include "AssociationUtils/ToolBox.h"

namespace xAOD {
  class TEvent;
  class TStore;
}

namespace CxAODSelectors {
  class ObjectSelector {
  public:
    virtual bool get(const xAOD::IParticle* elem) = 0;
    virtual ~ObjectSelector(){};
  };
  
  class PassThrough : public ObjectSelector{
    virtual bool get(const xAOD::IParticle* /*elem*/) override{
      return true;
    }
    virtual ~PassThrough(){};
  };
  
  class AcceptanceSelector : public ObjectSelector{
    double fPtMin;
    double fAbsEtaMax;
  public:
    AcceptanceSelector(double ptmin, double etamax) :
      fPtMin(ptmin),
      fAbsEtaMax(etamax)
    {}
    virtual bool get(const xAOD::IParticle* elem) override {
      return (elem->pt() > this->fPtMin) && (fabs(elem->eta()) < this->fAbsEtaMax);
    }
    virtual ~AcceptanceSelector(){};
  };
  
  template<class T> class InputAccessor : public ObjectSelector {
    SG::AuxElement::Accessor<T> fAccessor;
  public:
    InputAccessor(const std::string& s) :
      fAccessor(s)
    {};
    virtual bool get(const xAOD::IParticle* elem) override{
      return static_cast<bool>(this->fAccessor(*elem));
    }
    virtual ~InputAccessor(){};
  };
  
  typedef InputAccessor<char> CharAccessor;
  typedef InputAccessor<int> IntAccessor;
}

class CxAODReaderAlgorithm : public TQAlgorithm{
protected:
  bool fDoTaus;
  bool fDoPhotons;
  
  const std::string fInputLabel  = "ORInputLabel";
  const std::string fOutputLabel = "OROutputLabel";
  ort::inputDecorator_t fORInputDecorator;
  ort::outputAccessor_t fOROutputAccessor;

  TQSample* fSample = NULL;
  TQToken* fEventToken = NULL;
  xAOD::TEvent* fEvent = NULL;
  xAOD::TStore* fStore = NULL;
   
  std::string fElectronContainerName = "Electrons___Nominal";
  std::string fMuonContainerName = "Muons___Nominal";
  std::string fJetContainerName = "AntiKt4EMTopoJets___Nominal";
  std::string fTauContainerName = "TauJets___Nominal";
  std::string fPhotonContainerName = "Photons___Nominal";

  std::string fElectronKey = "Electrons";
  std::string fMuonKey = "Muons";
  std::string fJetKey = "Jets";
  std::string fTauKey = "Taus";
  std::string fPhotonKey = "Photons";
  
  CxAODSelectors::ObjectSelector* fMuonAcc = NULL;
  CxAODSelectors::ObjectSelector* fElectronAcc = NULL;
  CxAODSelectors::ObjectSelector* fJetAcc = NULL;
  CxAODSelectors::ObjectSelector* fTauAcc = NULL;
  CxAODSelectors::ObjectSelector* fPhotonAcc = NULL;
 
  ORUtils::ToolBox fToolBox;

  template<class ObjType> bool checkObject(const ObjType* obj);
  template<class ContainerType> void selectObjects(const ContainerType* container);
  template<class Container> inline void recordContainer(const std::string& name, const Container* cont);
 
public:

  CxAODReaderAlgorithm(bool doTaus = false, bool doPhotons=false);

  bool comparePt(const xAOD::IParticle* a,const xAOD::IParticle* b) const;
  
  void select(CxAODSelectors::ObjectSelector* electrons,
              CxAODSelectors::ObjectSelector* muons, 
              CxAODSelectors::ObjectSelector* jets,
              CxAODSelectors::ObjectSelector* taus = NULL,
              CxAODSelectors::ObjectSelector* photons = NULL);
   
  virtual bool cleanup() override;
  virtual bool initialize(TQSample* s) override;
  virtual bool finalize() override;
  virtual bool execute() override;  

  ClassDefOverride(CxAODReaderAlgorithm,0)
};

#endif
