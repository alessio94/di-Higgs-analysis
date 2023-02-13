#ifndef __LEPTONFAKETYPE__
#define __LEPTONFAKETYPE__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

#include "xAODBase/IParticleContainer.h"

class LeptonFakeType
    : public BBLLContainerRetrieval
    , public TQEventObservable {
protected:
    std::string m_variation = "";
    size_t m_lepton_index = -1;

public:
    LeptonFakeType();
    LeptonFakeType(const TString& name);
    virtual ~LeptonFakeType();

    virtual bool initializeSelf() override;

    virtual double getValue() const override;

    ClassDefOverride(LeptonFakeType, 1);
};
#endif // __LEPTONFAKETYPE__
