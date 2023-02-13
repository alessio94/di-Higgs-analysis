#ifndef __DDFAKEWEIGHT__
#define __DDFAKEWEIGHT__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

#include "xAODBase/IParticleContainer.h"

#include <set>

enum class DDFakeWeightVariationType {
    Nominal,
    Stat_up,
    Stat_down,
    Gen_up,
    Gen_down,
};

struct WeightBin {
    float bin_edge_low;
    float bin_edge_high;
    float value;
    float stat_error;
    float sys_error;

    WeightBin(float bin_edge_low, float bin_edge_high, float value, float stat_error, float sys_error)
        : bin_edge_low(bin_edge_low)
        , bin_edge_high(bin_edge_high)
        , value(value)
        , stat_error(stat_error)
        , sys_error(sys_error) { }

    WeightBin() { }
};

struct TransferFactorConfig {
    std::string id = "";
    std::string variable = "";
    std::string selection = "";
    std::vector<WeightBin> weight_bins;

    TransferFactorConfig() { }

    bool has_selection() const { return !selection.empty(); }
    void add_weight_bin(const float bin_edge_low, const float bin_edge_high, const float value, const float stat_error,
        const float sys_error);
    float get_fake_weight(float variable_value, DDFakeWeightVariationType variation_type, int variation_bin) const;
};

class DDFakeWeight
    : public BBLLContainerRetrieval
    , public TQEventObservable {
protected:
    std::set<std::string> m_observables_to_initialize;
    std::map<std::string, TQObservable*> m_observables;
    std::vector<TransferFactorConfig> m_transfer_factor_configs;

    std::string m_weight_variation = "";
    int m_variation_bin = -1;
    DDFakeWeightVariationType m_variation_type = DDFakeWeightVariationType::Nominal;

public:
    DDFakeWeight();
    DDFakeWeight(const TString& name);
    virtual ~DDFakeWeight();

    virtual bool initializeSelf() override;
    virtual bool finalizeSelf() override;

    virtual double getValue() const override;

    TransferFactorConfig& add_config(
        const std::string& id, const std::string& variable, const std::string& selection = "");
    void set_variation_bin(const int);

    ClassDefOverride(DDFakeWeight, 1);
};
#endif // __DDFAKEWEIGHT__
