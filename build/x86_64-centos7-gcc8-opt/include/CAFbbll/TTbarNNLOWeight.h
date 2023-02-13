#ifndef __TTBARNNLOWEIGHT__
#define __TTBARNNLOWEIGHT__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

#include "TTbarNNLOReweighter/TTbarNNLOReweighter.h"

enum class TTbarSampleType {
    None,
    Powheg_Pythia8_Nominal,
    Sherpa,
    Powheg_Herwig7,
    aMCNLO_Pythia8,
    Powheg_Pythia8_Hdamp
};

class TTbarNNLOWeight
    : public BBLLContainerRetrieval
    , public TQEventObservable {
private:
    TTbarNNLOReweighter* m_ttbar_nnlo_reweighter = nullptr;
    TTbarSampleType m_ttbar_sample_type { TTbarSampleType::None };

    std::string m_data_path = "";

protected:
    bool initializeSelf() override;
    bool finalizeSelf() override;

public:
    TTbarNNLOWeight();
    TTbarNNLOWeight(const TString& name);

    virtual ~TTbarNNLOWeight();

    virtual double getValue() const override;
    void set_data_path(const std::string& data_path);

    ClassDefOverride(TTbarNNLOWeight, 1);
};
#endif // __TTBARNNLOWEIGHT__
