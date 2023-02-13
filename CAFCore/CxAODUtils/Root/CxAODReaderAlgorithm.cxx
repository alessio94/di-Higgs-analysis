#include "CxAODUtils/CxAODReaderAlgorithm.h"

#include <string>

#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "xAODRootAccess/TActiveStore.h"

#include "xAODBase/IParticle.h"

#include "AssociationUtils/OverlapRemovalInit.h"
#include "AssociationUtils/OverlapRemovalTool.h"
#include "AssociationUtils/ToolBox.h"

#include "QFramework/TQSample.h"
#include "QFramework/TQToken.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

ClassImp(CxAODReaderAlgorithm)

//__________________________________________________________________________________|___________

CxAODReaderAlgorithm::CxAODReaderAlgorithm(bool doTaus, bool doPhotons) :
  fDoTaus(doTaus),
  fDoPhotons(doPhotons),
  fORInputDecorator(fInputLabel),
  fOROutputAccessor(fOutputLabel)
{
  
  // Initialize the harmonization recommendation tools (deprecated from AnalysisBase release 21.2.5 on -> use ORFlags and recommendedTools(...) function)
  // if(!ORUtils::harmonizedTools(this->fORToolBox, "OverlapRemovalTool",this->fInputLabel, this->fOutputLabel,outputPassValue, this->fDoTaus, this->fDoPhotons).isSuccess())
  //   throw std::runtime_error("unable to call ORUtils::harmonizedTools!");
  ORUtils::ORFlags ORFlags("OverlapRemovalTool",this->fInputLabel, this->fOutputLabel);
  ORFlags.outputPassValue = false;
  ORFlags.doTaus = fDoTaus;
  ORFlags.doPhotons = fDoPhotons;
  // Initialize all tools
  if(!ORUtils::recommendedTools(ORFlags, fToolBox).isSuccess()) {
    throw std::runtime_error("unable to call ORUtils::recommendedTools!");
  }
  if (!fToolBox.initialize().isSuccess()) {
    throw std::runtime_error("unable to initialize ORUtils::Toolbox!");
  }

  // Functions setMsgLevel not available in new (~rel 21.2.5 on) Toolbox class.
  // Set message level for all tools
  // #ifdef _DEBUG_
  // this->fToolBox.setMsgLevel(MSG::DEBUG);
  // #else
  // this->fToolBox.setMsgLevel(MSG::ERROR);
  // #endif

}

//__________________________________________________________________________________|___________

void CxAODReaderAlgorithm::select(CxAODSelectors::ObjectSelector* electrons,
                                  CxAODSelectors::ObjectSelector* muons,
                                  CxAODSelectors::ObjectSelector* jets,
                                  CxAODSelectors::ObjectSelector* taus,
                                  CxAODSelectors::ObjectSelector* photons){
  this->fElectronAcc = electrons;
  this->fMuonAcc = muons;
  this->fJetAcc = jets;
  this->fTauAcc = taus;
  this->fPhotonAcc = photons;
}

//__________________________________________________________________________________|___________

bool CxAODReaderAlgorithm::initialize(TQSample* s){
  // initialize the algorithm
  if(!s) throw(std::runtime_error("received NULL sample"));
  this->fSample = s;
  this->fEventToken = s->getEventToken();
  if(!this->fEventToken) return false;
  this->fEvent = (xAOD::TEvent*)(this->fEventToken->getContent());
  if(!this->fEvent) return false;

  this->fStore = xAOD::TActiveStore::store();
  if(!this->fStore){
    throw std::runtime_error("unable to access active store, please create a TStore object and set it active using 'TStore::setActive()'!");
  }
  
  return true;
}

//__________________________________________________________________________________|___________

bool CxAODReaderAlgorithm::finalize(){
  // finalize the algorithm
  this->fSample->returnToken(this->fEventToken);
  this->fEventToken = NULL;
  this->fEvent = NULL;
  this->fSample = NULL;
  this->fStore = NULL;
  return true;
}

//__________________________________________________________________________________|___________

bool CxAODReaderAlgorithm::cleanup(){
  // cleanup the cache of the algorithm after an event
  this->fStore->clear();
  return true;
}

//__________________________________________________________________________________|___________

bool CxAODReaderAlgorithm::execute(){
  // execute this algorithm on an event
  const xAOD::ElectronContainer* electrons = 0;
  const xAOD::MuonContainer* muons = 0;
  const xAOD::JetContainer* jets = 0;
  const xAOD::TauJetContainer* taus = 0;
  const xAOD::PhotonContainer* photons = 0;
    
  if(!fEvent->retrieve(electrons, this->fElectronContainerName).isSuccess())
    throw (std::runtime_error("unable to retrieve electron collection '"+this->fElectronContainerName+"'"));
  if(!fEvent->retrieve(muons, this->fMuonContainerName).isSuccess())
    throw (std::runtime_error("unable to retrieve muon collection '"+this->fMuonContainerName+"'"));
  if(!fEvent->retrieve(jets, this->fJetContainerName).isSuccess())
    throw (std::runtime_error("unable to retrieve jet collection '"+this->fJetContainerName+"'"));
  if(this->fDoTaus){
    if(!fEvent->retrieve(taus, this->fTauContainerName).isSuccess())
      throw (std::runtime_error("unable to retrieve tau collection '"+this->fTauContainerName+"'"));
  }
  if(this->fDoPhotons){
    if(!fEvent->retrieve(photons, this->fPhotonContainerName).isSuccess())
      throw (std::runtime_error("unable to retrieve photon collection '"+this->fPhotonContainerName+"'"));
  }
    
  // Set input decorations
  selectObjects(electrons);
  selectObjects(muons);
  selectObjects(jets);
  if(taus)    selectObjects(taus);
  if(photons) selectObjects(photons);    

  // Apply the overlap removal to all objects (dumb example)
  if(!this->fToolBox.masterTool->removeOverlaps(electrons, muons, jets, taus, photons))
    throw std::runtime_error("error while performing overlap removal!");

  recordContainer(this->fElectronKey, electrons);
  recordContainer(this->fMuonKey,    muons);
  recordContainer(this->fJetKey,     jets);
  if(this->fDoTaus){
    recordContainer(this->fTauKey,   taus);
  }
  if(this->fDoPhotons){
    recordContainer(this->fPhotonKey,photons);
  }
  return true;
}

//__________________________________________________________________________________|___________

template<class ContainerType> void  CxAODReaderAlgorithm::selectObjects(const ContainerType* container){
  for(auto obj : *container){
    this->fORInputDecorator(*obj) = checkObject(obj);
  }
}

//__________________________________________________________________________________|___________

bool CxAODReaderAlgorithm::comparePt( const xAOD::IParticle* partA,
                                      const xAOD::IParticle* partB ) const {
  return (partA->pt() > partB->pt());
}

//__________________________________________________________________________________|___________

template<class Container> inline void  CxAODReaderAlgorithm::recordContainer(const std::string& name, const Container* cont){
  ConstDataVector<Container>* newCont = new ConstDataVector<Container>(SG::VIEW_ELEMENTS);
  newCont->reserve(cont->size());
  for(const typename Container::base_value_type* elem : *cont){
    if(static_cast<bool>(fOROutputAccessor(*elem))){
      newCont->push_back( elem );
    }
  }
  newCont->sort([this](const xAOD::IParticle* a, const xAOD::IParticle* b) {
      return this->comparePt(a,b);
    });
  if(!this->fStore->record(newCont,name).isSuccess()) throw std::runtime_error(std::string("error recording ")+name);
}

//__________________________________________________________________________________|___________

template<> bool CxAODReaderAlgorithm::checkObject(const xAOD::Muon* obj){
  return (this->fMuonAcc && this->fMuonAcc->get(obj));
}

//__________________________________________________________________________________|___________

template<> bool CxAODReaderAlgorithm::checkObject(const xAOD::TauJet* obj){
  return (this->fTauAcc && this->fTauAcc->get(obj));
}

//__________________________________________________________________________________|___________

template<> bool CxAODReaderAlgorithm::checkObject(const xAOD::Electron* obj){
  return (this->fElectronAcc && this->fElectronAcc->get(obj));
}

//__________________________________________________________________________________|___________

template<> bool CxAODReaderAlgorithm::checkObject(const xAOD::Jet* obj){
  return (this->fJetAcc && this->fJetAcc->get(obj));
}

//__________________________________________________________________________________|___________

template<> bool CxAODReaderAlgorithm::checkObject(const xAOD::Photon* obj){
  return (this->fPhotonAcc && this->fPhotonAcc->get(obj));
}


