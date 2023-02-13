//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQCPDECORATIONOBSERVABLE__
#define __TQCPDECORATIONOBSERVABLE__
#include "CAFxAODUtils/TQEventObservable.h"

#ifndef __CINT__
#define XAOD_STANDALONE 1
#include "xAODParticleEvent/CompositeParticleContainer.h"
#endif

template <class T> class TQCPDecorationObservable : public TQEventObservable { // linkalltemplates
protected:
  TString fExpression = "";
  TString fContainerName = ""; //!
  TString fDecoration = ""; //!
  unsigned int fContainerIndex = 0; //!
  std::vector<unsigned int> fParticleIndices; //!
  mutable TString fCachedExpression; //!
  TString fCachedContainerName = ""; //!
  std::vector<bool> fUseOtherParts; //!
#ifndef __CINT__
  mutable xAOD::CompositeParticleContainer const * mContainer = 0;
  SG::AuxElement::Accessor<T>* fDecoAccess = 0; //!
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

  TQCPDecorationObservable();
  TQCPDecorationObservable(const TString& expression);
  virtual ~TQCPDecorationObservable();

  DECLARE_OBSERVABLE_FACTORY(TQCPDecorationObservable,TString expression);
  
  ClassDefT(TQCPDecorationObservable<T>,0)
};

ClassDefT2(TQCPDecorationObservable,T)

#include "CAFxAODUtils/Observables.h"

TYPEDEF_ALL_TEMPLATES(TQCP,DecorationObservable)

#endif
