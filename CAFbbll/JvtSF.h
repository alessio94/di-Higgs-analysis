#ifndef __JVTSF__
#define __JVTSF__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class JvtSF
    : public BBLLContainerRetrieval
    , public TQEventObservable {
protected:
    std::string m_p4_variation = "";
    std::string m_weight_variation = "";
    std::string m_variation = "";

public:
    JvtSF();
    JvtSF(const TString& name);
    virtual ~JvtSF();

    virtual bool initializeSelf() override;

    virtual double getValue() const override;

    ClassDefOverride(JvtSF, 1);
};
#endif // __JVTSF__
