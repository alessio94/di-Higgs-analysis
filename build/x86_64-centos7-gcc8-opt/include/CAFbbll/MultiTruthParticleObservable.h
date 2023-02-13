#ifndef __MULTITRUTHPARTICLEOBSERVABLE__
#define __MULTITRUTHPARTICLEOBSERVABLE__

#include "CAFxAODUtils/TQEventObservable.h"

#include "xAODTruth/TruthParticleContainer.h"

#include <map>

class MultiTruthParticleObservable : public TQEventObservable {
private:
    static bool compare_pt(const xAOD::TruthParticle* a, const xAOD::TruthParticle* b);
    static std::vector<int> parse_int_list(const std::string& s, char delim);

protected:
    TString m_expression = "";

    std::vector<int> m_pdg_ids;
    std::vector<int> m_indices;

    enum TruthQuantity { PT, ETA, PHI, E, M, DELTAR, DELTAPHI, DELTAETA, DELTAPT, PTSCALAR, UNKNOWN };
    TruthQuantity m_quantity;

    virtual bool initializeSelf() override;
    virtual bool finalizeSelf() override;

public:
    double getValue() const override;

    bool hasExpression() const override { return true; };
    const TString& getExpression() const override { return m_expression; };
    void setExpression(const TString& expr) override { m_expression = expr; };

    MultiTruthParticleObservable();
    MultiTruthParticleObservable(const TString& name);
    virtual ~MultiTruthParticleObservable();

    bool parse_expression(const TString& expr);

    DECLARE_OBSERVABLE_FACTORY(MultiTruthParticleObservable, TString expression)

    ClassDefOverride(MultiTruthParticleObservable, 1);
};

#endif
