#ifndef __TRUTHPT__
#define __TRUTHPT__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class TruthPthh
    : public BBLLContainerRetrieval
    , public TQEventObservable {
public:
    TruthPthh();
    TruthPthh(const TString& name);
    virtual ~TruthPthh();

    virtual double getValue() const override;

    ClassDefOverride(TruthPthh, 1);
};
#endif // __TRUTHPT__
