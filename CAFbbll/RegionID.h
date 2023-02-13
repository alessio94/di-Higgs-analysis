#ifndef __REGIONID__
#define __REGIONID__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class RegionID
    : public TQEventObservable
    , public BBLLContainerRetrieval {
private:
    TQObservable* m_mll_obs = nullptr;
    TQObservable* m_mbb_obs = nullptr;
    TQObservable* m_nbjets_obs = nullptr;

    const int SR1_2TAG = 1901;
    const int SR2_2TAG = 1902;
    const int TOPCR_2TAG = 1903;
    const int ZLLCR_2TAG = 1904;
    const int SR1_1TAG = 1905;
    const int SR2_1TAG = 1906;
    const int TOPCR_1TAG = 1907;

protected:
    mutable Long64_t m_cached_entry;          //!
    mutable std::vector<int> m_cached_values; //!

    bool virtual initializeSelf() override;
    bool virtual finalizeSelf() override;

public:
    RegionID();
    RegionID(const TString& name);
    virtual ~RegionID();

    inline virtual TQObservable::ObservableType getObservableType() const override {
        return TQObservable::ObservableType::vector;
    }
    virtual int getNevaluations() const override;
    virtual double getValue() const override;
    virtual double getValueAt(int index) const override;

    void fillOutputVector() const;

    ClassDefOverride(RegionID, 1);
};
#endif // not __REGIONID__
