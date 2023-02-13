#ifndef __TRUTHMHH__
#define __TRUTHMHH__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class TruthMhh
    : public BBLLContainerRetrieval
    , public TQEventObservable {
public:
    TruthMhh();
    TruthMhh(const TString& name);
    virtual ~TruthMhh();

    virtual double getValue() const override;

    ClassDefOverride(TruthMhh, 1);
};
#endif // __TRUTHMHH__
