//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQFlagCHECKINGOBSERVABLE__
#define __TQFlagCHECKINGOBSERVABLE__
#include "CAFxAODUtils/TQEventObservable.h"

#ifndef __CINT__
#include "AthContainers/AuxElement.h"
#endif

template <class Taux> class TQSGAuxFlagCheckingObservable : public TQEventObservable { // linkalltemplates
protected:
  TString fExpression = "";
  TString fContainerName = ""; //!
  TString fDecoration = ""; //!
  mutable TString fCachedExpression; //!
  TString fCachedContainerName = ""; //!
#ifndef __CINT__
  mutable SG::AuxElement const * mContainer = 0;
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

  TQSGAuxFlagCheckingObservable();
  TQSGAuxFlagCheckingObservable(const TString& expression);
  virtual ~TQSGAuxFlagCheckingObservable();

  DECLARE_OBSERVABLE_FACTORY(TQSGAuxFlagCheckingObservable,TString expression);
  
  ClassDefT(TQSGAuxFlagCheckingObservable<Taux>, 0)
};

ClassDefT2(TQSGAuxFlagCheckingObservable,Taux)

#include "CAFxAODUtils/Observables.h"

TYPEDEF_ALL_TEMPLATES(TQSGAuxFlag,CheckingObservable)

#endif
