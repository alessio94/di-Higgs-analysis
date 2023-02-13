#ifndef __METOBSERVABLE__
#define __METOBSERVABLE__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class METObservable
    : public BBLLContainerRetrieval
    , public TQEventObservable {
private:
    enum METMode {
        SUMET,
        MET,
        METX,
        METY,
        METPHI,
        SOFT_SUMET,
        SOFT_MET,
        SOFT_METX,
        SOFT_METY,
        METRHO,
        METSIG,
        METSIG_SOFT,
        METSIG_HARD,
        UNKNOWN
    };
    METMode m_mode = UNKNOWN;

public:
    METObservable();
    METObservable(const TString& name);
    virtual ~METObservable();

    virtual double getValue() const override;

    void set_met_collection(const std::string& jet_collection);

    ClassDefOverride(METObservable, 1);
};
#endif // not __METOBSERVABLE__
