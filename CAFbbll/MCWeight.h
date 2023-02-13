#ifndef __MCWEIGHT__
#define __MCWEIGHT__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class MCWeight
    : public BBLLContainerRetrieval
    , public TQEventObservable {
private:
    std::string m_mc_name = "";
    int m_weight_index = 0;

public:
    MCWeight();
    MCWeight(const TString& name);
    virtual ~MCWeight();

    virtual bool initializeSelf() override;
    virtual double getValue() const override;

    ClassDefOverride(MCWeight, 1);
};
#endif // __MCWEIGHT__
