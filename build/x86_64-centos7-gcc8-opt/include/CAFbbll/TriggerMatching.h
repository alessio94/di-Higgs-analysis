#ifndef __TRIGGERMATCHING__
#define __TRIGGERMATCHING__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class TriggerMatching
    : public BBLLContainerRetrieval
    , public TQEventObservable {
protected:
    static int year_to_trigger_id(bool is2015, bool is2016, bool is2017, bool is2018);

public:
    TriggerMatching();
    TriggerMatching(const TString& name);
    virtual ~TriggerMatching();

    virtual double getValue() const override;

    ClassDefOverride(TriggerMatching, 1);
};
#endif // __TRIGGERMATCHING__
