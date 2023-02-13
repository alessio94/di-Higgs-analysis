#ifndef __FILTEREDOUTPUTNODE__
#define __FILTEREDOUTPUTNODE__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

#include <string>
#include <vector>

class FilteredOutputNode
    : public TQEventObservable
    , public BBLLContainerRetrieval {
private:
    TString m_expression = "";
    std::string m_file_name = "";
    std::vector<std::string> m_output_nodes;
    std::vector<TQObservable*> m_observables;

protected:
    bool virtual initializeSelf() override;
    bool virtual finalizeSelf() override;

public:
    FilteredOutputNode();
    FilteredOutputNode(const TString& name);
    virtual ~FilteredOutputNode();

    virtual bool hasExpression() const override { return true; }
    virtual const TString& getExpression() const override { return m_expression; }
    virtual void setExpression(const TString& expr) override { m_expression = expr; }

    virtual double getValue() const override;

    DECLARE_OBSERVABLE_FACTORY(FilteredOutputNode, TString expression)

    ClassDefOverride(FilteredOutputNode, 1);
};
#endif // not __FILTEREDOUTPUTNODE__
