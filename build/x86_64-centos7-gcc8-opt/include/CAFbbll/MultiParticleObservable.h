#ifndef __MULTIPARTICLEOBSERVABLE__
#define __MULTIPARTICLEOBSERVABLE__

#include "CAFxAODUtils/TQEventObservable.h"

#include <map>

#include "xAODBase/IParticleContainer.h"

struct MultiParticleConstituent {
    TString container_name;
    int index;
};

class MultiParticleObservable : public TQEventObservable {
private:
    TString m_expression = "";
    std::vector<MultiParticleConstituent> m_constituents;
    mutable std::map<TString, const xAOD::IParticleContainer*> m_containers;

    enum MPQuantity { PT, ETA, PHI, E, M, DELTAR, DELTAPHI, DELTAETA, DELTAPT, PTSCALAR, UNKNOWN };
    MPQuantity m_quantity;

    const double DEFAULT_VALUE = -1e10;

protected:
    virtual bool initializeSelf() override;
    virtual bool finalizeSelf() override;

public:
    MultiParticleObservable();
    MultiParticleObservable(const TString& name);
    virtual ~MultiParticleObservable();

    virtual bool hasExpression() const override { return true; }
    virtual const TString& getExpression() const override { return m_expression; }
    virtual void setExpression(const TString& expr) override { m_expression = expr; }
    bool parse_expression(const TString& expr);

    bool retrieve_containers() const;
    const xAOD::IParticle* getParticle(MultiParticleConstituent constituent) const;
    virtual double getValue() const override;

    DECLARE_OBSERVABLE_FACTORY(MultiParticleObservable, TString expression)

    ClassDefOverride(MultiParticleObservable, 1);
};
#endif
