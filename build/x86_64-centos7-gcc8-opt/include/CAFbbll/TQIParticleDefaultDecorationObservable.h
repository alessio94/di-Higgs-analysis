// this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQIPARTICLEDEFAULTDECORATIONOBSERVABLE__
#define __TQIPARTICLEDEFAULTDECORATIONOBSERVABLE__

#include "CAFxAODUtils/Observables.h"
#include "CAFxAODUtils/TQEventObservable.h"

#ifndef __CINT__
#include "AthContainers/AuxElement.h"
#include "xAODBase/IParticle.h"
#include "xAODBase/IParticleContainer.h"
#endif

template <class Taux> class TQIParticleDefaultDecorationObservable : public TQEventObservable { // linkalltemplates
protected:
    TString m_expression = "";
    TString m_container_name = "";       //!
    unsigned int m_container_index = 0;  //!
    TString m_decoration = "";           //!
    mutable TString m_cached_expression; //!
    double m_default_value = -1;         // returned when specified value is not available
#ifndef __CINT__
    mutable xAOD::IParticleContainer const* m_container = 0;
    SG::AuxElement::Accessor<Taux>* m_deco_access = 0; //!
#endif

    void clearParsedExpression();
    bool parseExpression(const TString& expr);

public:
    TQIParticleDefaultDecorationObservable();
    TQIParticleDefaultDecorationObservable(const TString& expression);
    virtual ~TQIParticleDefaultDecorationObservable();

    virtual bool initializeSelf() override;
    virtual bool finalizeSelf() override;

    static TString getExpressionPrefix();
    virtual double getValue() const override;
    virtual bool hasExpression() const override { return true; };
    virtual const TString& getExpression() const override { return m_expression; };
    virtual TString getActiveExpression() const override;
    virtual void setExpression(const TString& expr) override { m_expression = expr; };

    DECLARE_OBSERVABLE_FACTORY(TQIParticleDefaultDecorationObservable, TString expression);

    ClassDefT(TQIParticleDefaultDecorationObservable<Taux>, 1)
};

ClassDefT2(TQIParticleDefaultDecorationObservable, Taux)

    TYPEDEF_ALL_TEMPLATES(TQIParticleDefault, DecorationObservable)

#endif
