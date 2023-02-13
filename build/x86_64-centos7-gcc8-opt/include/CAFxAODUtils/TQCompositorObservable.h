//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQCOMPOSITOROBSERVABLE__
#define __TQCOMPOSITOROBSERVABLE__
#include "CAFxAODUtils/TQEventObservable.h"

#include "TString.h"
#include "TLorentzVector.h"

#include <vector>

class TQCompositorObservable : public TQEventObservable {
protected:
  enum V4Quantity {
    MASS,
    PT,
    ETA,
    PHI,
  };

  std::vector<TString> fContainerNames;
  std::vector<unsigned int> fContainerIndices;
  std::vector<std::vector<unsigned int> > fParticleIndices;
  V4Quantity f4Quantity;

  mutable TLorentzVector fCompositeParticle;

public:
  virtual double getValue() const override;
  bool computeValue() const;
protected:
  TString fExpression = "";

public:
  virtual bool hasExpression() const override;
  virtual const TString& getExpression() const override;
  virtual void setExpression(const TString& expr) override;

  TQCompositorObservable();
  TQCompositorObservable(const TString& expression);
  virtual ~TQCompositorObservable();
public:
  bool parseExpression(const TString& expr);
  void clearParsedExpression();

  virtual TString getActiveExpression() const override;
  virtual bool initializeSelf() override;
  virtual bool finalizeSelf() override;
public:

  DECLARE_OBSERVABLE_FACTORY(TQCompositorObservable,TString expression)
  
  ClassDefOverride(TQCompositorObservable, 0);

};
#endif
