#include "CAFxAODUtils/Observables.h"

#include "CAFxAODUtils/TQIParticleDecorationObservable.h"
#include "CAFxAODUtils/TQIParticleDecorationVectorObservable.h"
#include "CAFxAODUtils/TQCPConstituentVectorObservable.h"
#include "CAFxAODUtils/TQCPDecorationObservable.h"
#include "CAFxAODUtils/TQSGAuxElementDecorationObservable.h"
#include "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h"
#include "CAFxAODUtils/TQIParticleFourVectorObservable.h"
#include "CAFxAODUtils/TQIParticleFourVectorVectorObservable.h"
#include "CAFxAODUtils/TQTruthParticleInfoObservable.h"
#include "CAFxAODUtils/TQContainerSizeObservable.h"
#include "CAFxAODUtils/TQCompositorObservable.h"
#include "CAFxAODUtils/TQToolObservable.h"

bool TQObservableFactory::setupCAFxAODUtils(){
  // clear all factories from the list
  TQObservable::manager.registerFactory(TQContainerSizeObservable::getFactory(),true); 
  TQObservable::manager.registerFactory(TQCompositorObservable::getFactory(),true); 
  TQObservable::manager.registerFactory(TQCPConstituentVectorObservable::getFactory(),true); 
  TQObservable::manager.registerFactory(TQIParticleFourVectorObservable::getFactory(),true); 
  TQObservable::manager.registerFactory(TQIParticleFourVectorVectorObservable::getFactory(),true);
  TQObservable::manager.registerFactory(TQTruthParticleInfoObservable::getFactory(),true);
  TQObservable::manager.registerFactory(TQToolObservable::getFactory(),true); 
  REGISTER_ALL_TEMPLATES(TQIParticleDecorationObservable);
  REGISTER_ALL_TEMPLATES(TQCPDecorationObservable);
  REGISTER_ALL_TEMPLATES(TQSGAuxElementDecorationObservable);
  REGISTER_ALL_TEMPLATES(TQSGAuxFlagCheckingObservable);
  REGISTER_ALL_TEMPLATES(TQIParticleDecorationVectorObservable);
  return true;
}

namespace {
  const bool _TQObservableFactory__setupCAFxAODUtils = TQObservableFactory::setupCAFxAODUtils();
}
