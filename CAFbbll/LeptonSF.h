#ifndef __LEPTONSF__
#define __LEPTONSF__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

#include "xAODBase/IParticleContainer.h"

class LeptonSF
    : public BBLLContainerRetrieval
    , public TQEventObservable {
protected:
    std::string m_electron_id_sf = "";
    std::string m_electron_iso_sf = "";
    std::string m_muon_iso_sf = "";

    std::string m_p4_variation = "";
    std::string m_weight_variation = "";
    std::string m_variation = "";

public:
    LeptonSF();
    LeptonSF(const TString& name);
    virtual ~LeptonSF();

    virtual bool initializeSelf() override;

    virtual double getValue() const override;

    void set_electron_id(const std::string& id);
    void set_electron_iso(const std::string& iso, const std::string& id);
    void set_muon_iso(const std::string& iso);

    ClassDefOverride(LeptonSF, 1);
};
#endif // __LEPTONSF__
