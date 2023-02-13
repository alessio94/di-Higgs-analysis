#ifndef __TRUTHHIGGSDECAYINFO__
#define __TRUTHHIGGSDECAYINFO__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

#ifndef __CINT__
#include "AthContainers/AuxElement.h"
#endif

class TruthHiggsDecayInfo
    : public BBLLContainerRetrieval
    , public TQEventObservable {
private:
    int m_higgs_index = -1;
    int m_child_index = -1;
    SG::AuxElement::Accessor<int>* m_decorator = nullptr; //!

public:
    TruthHiggsDecayInfo();
    TruthHiggsDecayInfo(const TString& name);
    virtual ~TruthHiggsDecayInfo();

    virtual bool initializeSelf() override;
    virtual bool finalizeSelf() override;
    virtual double getValue() const override;

    ClassDefOverride(TruthHiggsDecayInfo, 1);
};
#endif // __TRUTHHIGGSDECAYINFO__
