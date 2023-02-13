//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQEventInfoDECORATIONOBSERVABLE__
#define __TQEventInfoDECORATIONOBSERVABLE__
#include "CAFxAODUtils/TQEventObservable.h"

#ifndef __CINT__
#include "AthContainers/AuxElement.h"
#endif

template <class Taux> class TQSGAuxElementDecorationObservable : public TQEventObservable { // linkalltemplates
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

  TQSGAuxElementDecorationObservable();
  TQSGAuxElementDecorationObservable(const TString& expression);
  virtual ~TQSGAuxElementDecorationObservable();

  DECLARE_OBSERVABLE_FACTORY(TQSGAuxElementDecorationObservable,TString expression);
  
  ClassDefT(TQSGAuxElementDecorationObservable<Taux>, 0)
};

ClassDefT2(TQSGAuxElementDecorationObservable,Taux)

#include "CAFxAODUtils/Observables.h"

TYPEDEF_ALL_TEMPLATES(TQSGAuxElement,DecorationObservable)

#endif
