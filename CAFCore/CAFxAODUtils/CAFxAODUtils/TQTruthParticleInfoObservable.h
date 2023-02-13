//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQTRUTHPARTICLEINFOOBSERVABLE__
#define __TQTRUTHPARTICLEINFOOBSERVABLE__
#include "CAFxAODUtils/TQEventObservable.h"

#ifndef __CINT__
#define XAOD_STANDALONE 1
#include "xAODTruth/TruthParticleContainer.h"
using namespace xAOD;
#else
class TruthParticleContainer;
class TruthParticle;
#endif

class TQTruthParticleInfoObservable : public TQEventObservable {
protected:
  TString fExpression = "";
  TString fContainerName = "";
  unsigned int fIndex = 0;
  enum TruthQuantity{HASPRODVTX,HASDECAYVTX,NPARENTS,NCHILDREN,PDGID,ABSPDGID,STATUS,CHARGE,UNKNOWN};
  TruthQuantity fQuantity;
  mutable xAOD::TruthParticleContainer const * mContainer = 0;

  virtual bool initializeSelf() override;
  virtual bool finalizeSelf() override;

public:
  virtual double getValue() const override;
  virtual bool hasExpression() const override;
  virtual const TString& getExpression() const override;
  virtual void setExpression(const TString& expr) override;

  TQTruthParticleInfoObservable();
  TQTruthParticleInfoObservable(const TString& expression);
  virtual ~TQTruthParticleInfoObservable();

  bool parseExpression(const TString& expr);
  void clearParsedExpression();

  virtual TString getActiveExpression() const override;

  DECLARE_OBSERVABLE_FACTORY(TQTruthParticleInfoObservable,TString expression)

  ClassDefOverride(TQTruthParticleInfoObservable, 0);

};
#endif
