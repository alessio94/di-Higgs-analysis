#ifndef __NTRUTHLEPTONS__
#define __NTRUTHLEPTONS__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

class NTruthLeptons
    : public BBLLContainerRetrieval
    , public TQEventObservable {
public:
    NTruthLeptons();
    NTruthLeptons(const TString& name);
    virtual ~NTruthLeptons();

    virtual double getValue() const override;

    ClassDefOverride(NTruthLeptons, 1);
};
#endif // __NTRUTHLEPTONS__
