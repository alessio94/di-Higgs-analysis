#ifndef __PUWEIGHT__
#define __PUWEIGHT__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class PUWeight
    : public BBLLContainerRetrieval
    , public TQEventObservable {
protected:
    std::string m_p4_variation = "";
    std::string m_weight_variation = "";
    std::string m_variation = "";

public:
    PUWeight();
    PUWeight(const TString& name);
    virtual ~PUWeight();

    virtual bool initializeSelf() override;

    virtual double getValue() const override;

    ClassDefOverride(PUWeight, 1);
};
#endif // __PUWEIGHT__
