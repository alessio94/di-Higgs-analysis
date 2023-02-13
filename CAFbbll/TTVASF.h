#ifndef __TTVASF__
#define __TTVASF__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class TTVASF
    : public BBLLContainerRetrieval
    , public TQEventObservable {
public:
    TTVASF();
    TTVASF(const TString& name);
    virtual ~TTVASF();

    virtual double getValue() const override;

    ClassDefOverride(TTVASF, 1);
};
#endif // __TTVASF__
