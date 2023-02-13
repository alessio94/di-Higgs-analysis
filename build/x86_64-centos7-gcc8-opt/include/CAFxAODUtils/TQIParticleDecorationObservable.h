//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQIPARTICLEDECORATIONOBSERVABLE__
#define __TQIPARTICLEDECORATIONOBSERVABLE__
#include "CAFxAODUtils/TQEventObservable.h"

#ifndef __CINT__
#include "AthContainers/AuxElement.h"
#include "xAODBase/IParticle.h"
#include "xAODBase/IParticleContainer.h"
#endif


template <class Taux> class TQIParticleDecorationObservable : public TQEventObservable { // linkalltemplates
protected:
  TString fExpression = "";
  TString fContainerName = ""; //!
  unsigned int fContainerIndex = 0; //!
  TString fDecoration = ""; //!
  mutable TString fCachedExpression; //!
  TString fCachedContainerName = ""; //!
#ifndef __CINT__
  mutable xAOD::IParticleContainer const * mContainer = 0;
  SG::AuxElement::Accessor<Taux>* fDecoAccess = 0; //!
#endif

  void clearParsedExpression();
  bool parseExpression(const TString& expr);

public:
  static TString getExpressionPrefix();
  virtual double getValue() const override;
  virtual bool hasExpression() const override;
  virtual const TString& getExpression() const override;
  virtual TString getActiveExpression() const override;
  virtual void setExpression(const TString& expr) override;

  virtual bool initializeSelf() override;
  virtual bool finalizeSelf() override;

  TQIParticleDecorationObservable();
  TQIParticleDecorationObservable(const TString& expression);
  virtual ~TQIParticleDecorationObservable();

  DECLARE_OBSERVABLE_FACTORY(TQIParticleDecorationObservable,TString expression);
  
  ClassDefT(TQIParticleDecorationObservable<Taux>, 0)
};

ClassDefT2(TQIParticleDecorationObservable,Taux)

#include "CAFxAODUtils/Observables.h"

TYPEDEF_ALL_TEMPLATES(TQIParticle,DecorationObservable)

#endif
