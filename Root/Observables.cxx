#include "CAFbbll/Observables.h"
#include "QFramework/ASG.h"

#ifdef HAS_XAOD
#include "CAFbbll/FilteredOutputNode.h"
#include "CAFbbll/MultiParticleObservable.h"
#include "CAFbbll/MultiTruthParticleObservable.h"
#include "CAFbbll/PNN_KL.h"
#include "CAFbbll/TQIParticleDefaultDecorationObservable.h"

#endif

bool TQObservableFactory::setupCAFbbll() {
#ifdef HAS_XAOD
    TQObservable::manager.registerFactory(MultiParticleObservable::getFactory(), true);
    TQObservable::manager.registerFactory(MultiTruthParticleObservable::getFactory(), true);
    TQObservable::manager.registerFactory(FilteredOutputNode::getFactory(), true);
    TQObservable::manager.registerFactory(PNN_KL::getFactory(), true);
    REGISTER_ALL_TEMPLATES(TQIParticleDefaultDecorationObservable);
#endif

    return true;
}

namespace {
    const bool _TQObservableFactory__setupCAFbbll = TQObservableFactory::setupCAFbbll();
}
