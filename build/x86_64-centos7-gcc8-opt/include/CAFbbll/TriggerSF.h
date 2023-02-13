#ifndef __TRIGGERSF__
#define __TRIGGERSF__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class TriggerSF
    : public BBLLContainerRetrieval
    , public TQEventObservable {
private:
    std::string m_p4_variation = "";
    std::string m_weight_variation = "";
    std::string m_variation = "";

public:
    TriggerSF();
    TriggerSF(const TString& name);
    virtual ~TriggerSF();

    virtual bool initializeSelf() override;

    virtual double getValue() const override;

    ClassDefOverride(TriggerSF, 1);
};
#endif // __TRIGGERSF__
