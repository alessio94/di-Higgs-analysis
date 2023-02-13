#ifndef __SUMOFWEIGHT__
#define __SUMOFWEIGHT__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class SumOfWeight
    : public BBLLContainerRetrieval
    , public TQEventObservable {
private:
    std::string m_mc_name = "Default";
    std::string m_weight_variation = "Nominal";
    float m_sum_of_weight = 0;

    static std::map<std::string, std::set<std::string>> s_weight_variation_initialization_list;
    static std::map<std::string, bool> s_initialized;
    static std::map<std::string, std::map<std::string, double>> s_sum_of_weights;
    static bool initializeSumOfWeights(
        const std::string& mc_name, const std::string& default_sumw_hist, const TString& filename);

public:
    SumOfWeight();
    SumOfWeight(const TString& name);
    virtual ~SumOfWeight();

    virtual bool initializeSelf() override;
    virtual bool finalizeSelf() override;
    virtual double getValue() const override;

    static void addWeightToInitializationList(const std::string& sample, const std::string& variation);

    ClassDefOverride(SumOfWeight, 1);
};
#endif // __SUMOFWEIGHT__
