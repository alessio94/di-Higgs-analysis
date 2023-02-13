//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQIPARTICLEFOURVECTOROBSERVABLE__
#define __TQIPARTICLEFOURVECTOROBSERVABLE__
#include "CAFxAODUtils/TQEventObservable.h"

#ifndef __CINT__
#define XAOD_STANDALONE 1
#include "xAODBase/IParticleContainer.h"
using namespace xAOD;
#else
class IParticleContainer;
class IParticle;
#endif


class TQIParticleFourVectorObservable : public TQEventObservable {
protected:
  TString fExpression = "";
  TString fContainerName = "";
  unsigned int fIndex = 0;
  enum V4Quantity{ETA,PHI,E,M,PT,UNKNOWN};
  V4Quantity fQuantity;
  mutable xAOD::IParticleContainer const * mContainer = 0;
  
  virtual bool initializeSelf() override;
  virtual bool finalizeSelf() override;

public:
  virtual double getValue() const override;
  virtual bool hasExpression() const override;
  virtual const TString& getExpression() const override;
  virtual void setExpression(const TString& expr) override;

  TQIParticleFourVectorObservable();
  TQIParticleFourVectorObservable(const TString& expression);
  virtual ~TQIParticleFourVectorObservable();

  bool parseExpression(const TString& expr);
  void clearParsedExpression();

  virtual TString getActiveExpression() const override;

  DECLARE_OBSERVABLE_FACTORY(TQIParticleFourVectorObservable,TString expression)
  
  ClassDefOverride(TQIParticleFourVectorObservable, 0);

};
#endif
