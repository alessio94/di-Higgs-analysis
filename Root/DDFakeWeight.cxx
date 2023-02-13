#include "CAFbbll/DDFakeWeight.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include <algorithm>
#include <string>

DDFakeWeight::DDFakeWeight() { DEBUGclass("Default constructor called"); }

DDFakeWeight::~DDFakeWeight() { DEBUGclass("Destructor called"); }

DDFakeWeight::DDFakeWeight(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());

    TString input = name;

    TString obs_name;
    if (!TQStringUtils::readToken(input, obs_name, TQStringUtils::alphanumvarext)) {
        ERRORclass("The name of the DDFakeWeight has to have the following scheme: 'Name<:Variation>'");
        BREAK("Error in creating DDFakeWeight observable");
    }
    DEBUGclass("Observable name is %s", obs_name.Data());

    if (!input.IsNull()) { // there is a ":variation" after the observable name
        if (TQStringUtils::removeLeading(input, ":") != 1) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating DDFakeWeight observable");
        }

        if (input.IsNull()) {
            ERRORclass("Missing weight variation declaration in expression '%s'", name.Data());
            BREAK("Error in creating DDFakeWeight observable");
        }
        m_weight_variation = input.Data();
        DEBUGclass("Using weight variation %s", m_weight_variation.c_str());
    } else {
        DEBUGclass("No systematic weight variation specified.");
    }
}

bool DDFakeWeight::initializeSelf() {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    if (m_weight_variation.empty()) {
        m_weight_variation = "Nominal";
        m_variation_type = DDFakeWeightVariationType::Nominal;
    } else {
        if (m_weight_variation.rfind("FAKE_STAT_UP", 0) == 0) {
            m_variation_type = DDFakeWeightVariationType::Stat_up;
        } else if (m_weight_variation.rfind("FAKE_STAT_DOWN", 0) == 0) {
            m_variation_type = DDFakeWeightVariationType::Stat_down;
        } else if (m_weight_variation.rfind("FAKE_GEN_UP", 0) == 0) {
            m_variation_type = DDFakeWeightVariationType::Gen_up;
        } else if (m_weight_variation.rfind("FAKE_GEN_DOWN", 0) == 0) {
            m_variation_type = DDFakeWeightVariationType::Gen_down;
        } else if (m_weight_variation == "Nominal") {
            m_variation_type = DDFakeWeightVariationType::Nominal;
        } else {
            ERRORclass("Did not recognize weight variation '%s'", m_weight_variation.c_str());
            return false;
        }
    }

    // initialize observables which are used for the selection and binning of the transfer factors
    std::shared_ptr<TQTaggable> aliases = TQTaggable::getGlobalTaggable("aliases");
    for (const std::string& observable_string : m_observables_to_initialize) {
        DEBUGclass("Initializing observable '%s'", observable_string.c_str());
        TString observable_expression(observable_string);
        observable_expression = aliases->replaceInTextRecursive(observable_expression);
        TQObservable* observable = TQObservable::getObservable(observable_expression, fSample);
        if (!observable->initialize(fSample)) {
            return false;
        }
        m_observables[observable_string] = observable;
    }

    return true;
}

bool DDFakeWeight::finalizeSelf() {
    DEBUGclass("finalizing");

    for (auto const& observable : m_observables) {
        DEBUGclass("finalizing observable '%s'", observable.first.c_str());
        if (!observable.second->finalize()) {
            return false;
        }
    }
    m_observables.clear();

    return true;
}

void TransferFactorConfig::add_weight_bin(const float bin_edge_low, const float bin_edge_high, const float value,
    const float stat_error, const float sys_error) {
    weight_bins.emplace_back(bin_edge_low, bin_edge_high, value, stat_error, sys_error);
}

TransferFactorConfig& DDFakeWeight::add_config(
    const std::string& id, const std::string& variable, const std::string& selection) {
    TransferFactorConfig transfer_factor_config;
    DEBUGclass(
        "Adding config with id='%s', variable='%s', selection='%s'", id.c_str(), variable.c_str(), selection.c_str());
    transfer_factor_config.id = id;
    transfer_factor_config.variable = variable;
    transfer_factor_config.selection = selection;

    m_observables_to_initialize.insert(variable);
    if (!selection.empty()) {
        m_observables_to_initialize.insert(selection);
    }

    m_transfer_factor_configs.emplace_back(transfer_factor_config);
    return m_transfer_factor_configs.back();
}

void DDFakeWeight::set_variation_bin(const int variation_bin) { m_variation_bin = variation_bin; }

// helper function to get calculate weight based on variation
float get_fake_weight_variation(float weight, float stat_error, float sys_error, DDFakeWeightVariationType variation) {
    switch (variation) {
    case DDFakeWeightVariationType::Stat_up:
        return weight + stat_error;
    case DDFakeWeightVariationType::Stat_down:
        return weight - stat_error;
    case DDFakeWeightVariationType::Gen_up:
        return weight + sys_error;
    case DDFakeWeightVariationType::Gen_down:
        return weight - sys_error;
    default:
        return weight;
    }
}

float TransferFactorConfig::get_fake_weight(
    float variable_value, DDFakeWeightVariationType variation_type, int variation_bin) const {
    bool is_stat_variation =
        variation_type == DDFakeWeightVariationType::Stat_up || variation_type == DDFakeWeightVariationType::Stat_down;
    bool is_gen_variation =
        variation_type == DDFakeWeightVariationType::Gen_up || variation_type == DDFakeWeightVariationType::Gen_down;
    int weight_bin_index = 0;
    for (const WeightBin& weight_bin : weight_bins) {
        if (weight_bin.bin_edge_low <= variable_value && variable_value < weight_bin.bin_edge_high) {
            // find the weight bin which matches the value of the variable used for selecting the weight bin
            if (is_stat_variation && weight_bin_index == variation_bin) {
                // for the stat variations only apply variation if weight bin matches the variation bin
                return get_fake_weight_variation(
                    weight_bin.value, weight_bin.stat_error, weight_bin.sys_error, variation_type);
            } else if (is_gen_variation) {
                // gen variations are treated correlated between weight bins -> always apply variation
                return get_fake_weight_variation(
                    weight_bin.value, weight_bin.stat_error, weight_bin.sys_error, variation_type);
            } else {
                // nominal variation, just return value
                return weight_bin.value;
            }
        }
        ++weight_bin_index;
    }

    // if no bin met the requirements, use the last bin and do the same procedure as above
    const WeightBin& weight_bin = weight_bins.back();
    weight_bin_index = weight_bins.size() - 1;
    if (is_stat_variation && weight_bin_index == variation_bin) {
        return get_fake_weight_variation(weight_bin.value, weight_bin.stat_error, weight_bin.sys_error, variation_type);
    } else if (is_gen_variation) {
        return get_fake_weight_variation(weight_bin.value, weight_bin.stat_error, weight_bin.sys_error, variation_type);
    }
    return weight_bin.value;
}

double DDFakeWeight::getValue() const {
    bool applyDDFakeWeight = false;
    fSample->getTag("~applyDDFakeWeight", applyDDFakeWeight);
    if (!applyDDFakeWeight) {
        return 1;
    }

    float weight = 1;
    for (const TransferFactorConfig& transfer_factor_config : m_transfer_factor_configs) {
        bool use_this_config = false;
        if (transfer_factor_config.has_selection()) {
            if (m_observables.at(transfer_factor_config.selection)->getValue()) {
                use_this_config = true;
            }
        } else {
            use_this_config = true;
        }
        if (use_this_config) {
            DEBUGclass("Using transfer factor config '%s'", transfer_factor_config.id.c_str());
            float variable_value = m_observables.at(transfer_factor_config.variable)->getValue();
            weight = transfer_factor_config.get_fake_weight(variable_value, m_variation_type, m_variation_bin);
            break;
        }
    }

    // we need to subtract the prompt MC background from the SS region
    bool isMC = false;
    fSample->getTag("~isMC", isMC);
    if (isMC) {
        weight = weight * -1;
    }

    DEBUGclass("[%s] weight=%f", m_weight_variation.c_str(), weight);
    return weight;
}
