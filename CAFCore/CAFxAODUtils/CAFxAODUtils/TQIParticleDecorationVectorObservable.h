//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQIPARTICLEDECORATIONVECTOROBSERVABLE__
#define __TQIPARTICLEDECORATIONVECTOROBSERVABLE__
#include "CAFxAODUtils/TQEventObservable.h"
#include <iostream>

#ifndef __CINT__
#include "AthContainers/AuxElement.h"
#include "xAODBase/IParticle.h"
#include "xAODBase/IParticleContainer.h"
#endif


template <class Taux> class TQIParticleDecorationVectorObservable : public TQEventObservable { // linkalltemplates
protected:
  TString fExpression = "";
  TString fContainerName = ""; //!
  unsigned int fContainerMaxIndex = 0; //!
  TString fDecoration = ""; //!
  mutable TString fCachedExpression; //!
  TString fCachedContainerName = ""; //!
  
  mutable Long64_t fCachedEntry; //!
#ifndef __CINT__
  mutable xAOD::IParticleContainer const * mContainer = 0;
  SG::AuxElement::Accessor<Taux>* fDecoAccess = 0; //!
#endif

  void clearParsedExpression();
  bool parseExpression(const TString& expr);

public:
  static TString getExpressionPrefix();
  virtual double getValue() const override;
  virtual double getValueAt(int index) const override;
  virtual int getNevaluations() const override;
  
  inline virtual TQObservable::ObservableType getObservableType() const override {return TQObservable::ObservableType::vector;}

  virtual bool hasExpression() const override;
  virtual const TString& getExpression() const override;
  virtual TString getActiveExpression() const override;
  virtual void setExpression(const TString& expr) override;

  virtual bool initializeSelf() override;
  virtual bool finalizeSelf() override;

  TQIParticleDecorationVectorObservable();
  TQIParticleDecorationVectorObservable(const TString& expression);
  virtual ~TQIParticleDecorationVectorObservable();

  DECLARE_OBSERVABLE_FACTORY(TQIParticleDecorationVectorObservable,TString expression);
  
  ClassDefT(TQIParticleDecorationVectorObservable<Taux>, 0)
};

ClassDefT2(TQIParticleDecorationVectorObservable,Taux)

#include "CAFxAODUtils/Observables.h"

TYPEDEF_ALL_TEMPLATES(TQIParticle,DecorationVectorObservable)

#endif
