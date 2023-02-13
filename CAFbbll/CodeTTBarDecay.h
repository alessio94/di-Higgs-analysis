#ifndef __CODETTBARDECAY__
#define __CODETTBARDECAY__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class CodeTTBarDecay
    : public BBLLContainerRetrieval
    , public TQEventObservable {
public:
    CodeTTBarDecay() {};
    CodeTTBarDecay(const TString& name)
        : TQEventObservable(name) { }
    virtual ~CodeTTBarDecay() {};

    virtual double getValue() const override;

    ClassDefOverride(CodeTTBarDecay, 1);
};
#endif // __CODETTBARDECAY__
