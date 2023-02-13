#ifndef __NWTAUDECAYS__
#define __NWTAUDECAYS__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class NWTauDecays
    : public BBLLContainerRetrieval
    , public TQEventObservable {
public:
    NWTauDecays();
    NWTauDecays(const TString& name);
    virtual ~NWTauDecays();

    virtual double getValue() const override;

    ClassDefOverride(NWTauDecays, 1);
};
#endif // __NWTAUDECAYS__
