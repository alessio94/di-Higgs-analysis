#include "CAFbbll/FilteredOutputNode.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include <memory>
#include <stdexcept>

FilteredOutputNode::FilteredOutputNode() { DEBUGclass("Default constructor called"); }

FilteredOutputNode::~FilteredOutputNode() { DEBUGclass("Destructor called"); }

FilteredOutputNode::FilteredOutputNode(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());

    TString arguments = name;

    TString file_name = "";
    TQStringUtils::readUpTo(arguments, file_name, ",");
    TQStringUtils::removeLeading(arguments, ",");
    TQStringUtils::trim(arguments);
    if (arguments.Length() < 1) {
        ERRORclass("Unable to parse expression for FilteredOutputNode Observable: too few arguments (did you forget to "
                   "specify the output nodes to expose?). Arguments were: '%s,%s'",
            file_name.Data(), arguments.Data());
        BREAK("Error in creating FilteredOutputNode observable");
    }
    m_file_name = file_name.Data();
    SetName("FilteredOutputNode(" + file_name + ", " + arguments + ")");
    setExpression(name);

    // output nodes can be encloses by {}
    // this will be converted to () by TQObservable
    TString block = "";
    TQStringUtils::readBlock(arguments, block, "()");
    if (block.Length() < 1) { // block wasn't enclosed
        block = arguments;
    }
    std::vector<TString> asTStrings = TQStringUtils::tokenizeVector(block, ",", true);
    for (const TString& str : asTStrings) {
        m_output_nodes.push_back(str.Data());
    }
}

DEFINE_OBSERVABLE_FACTORY(FilteredOutputNode, TString expression) {
    // a factory for this observable type
    TQStringUtils::trim(expression);
    if (TQStringUtils::removeLeadingText(expression, "FilteredOutputNode")) {
        TQStringUtils::trim(expression);
        TString arguments = "";
        TQStringUtils::readBlock(expression, arguments);
        FilteredOutputNode* obs = new FilteredOutputNode(arguments);
        return obs;
    }
    return nullptr;
}

std::string join(const std::vector<std::string>& v, std::string delimiter) {
    std::string result;
    for (std::vector<std::string>::const_iterator p = v.begin(); p != v.end(); ++p) {
        result += *p;
        if (p != v.end() - 1)
            result += delimiter;
    }
    return result;
}

bool FilteredOutputNode::initializeSelf() {
    DEBUGclass("initializeSelf called");
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    DEBUGclass("m_file_name=%s", m_file_name.c_str());

    std::string all_output_nodes = "";
    size_t i = 0;
    size_t n_output_nodes = m_output_nodes.size();
    for (std::string output_node : m_output_nodes) {
        all_output_nodes += output_node;
        i += 1;
        if (i < n_output_nodes) {
            all_output_nodes += ",";
        }
    }

    for (size_t idx = 0; idx < m_output_nodes.size(); ++idx) {
        TString expression =
            TString::Format("[VecAT(lwtnnSeq(%s,{%s}),%zu)]", m_file_name.c_str(), all_output_nodes.c_str(), idx);
        DEBUGclass("Creating NN observable for expression '%s'", expression.Data());
        TQObservable* obs = TQObservable::getObservable(expression, fSample);
        if (!obs->initialize(fSample)) {
            return false;
        }
        m_observables.push_back(obs);
    }

    return true;
}

bool FilteredOutputNode::finalizeSelf() {
    DEBUGclass("finalizeSelf called");
    for (TQObservable* obs : m_observables) {
        if (obs) {
            obs->finalize();
        }
    }

    m_observables.clear();

    return true;
}

double FilteredOutputNode::getValue() const {
    DEBUGclass("getValue called");

    double max_value = -1;
    int max_index = -1;
    int current_index = 0;
    for (TQObservable* obs : m_observables) {
        double current_value = obs->getValue();
        if (current_value > max_value) {
            max_value = current_value;
            max_index = current_index;
        }
        current_index++;
    }

    return max_index + max_value;
}
