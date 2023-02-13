//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQIPARTICLEFOURVECTORVECTOROBSERVABLE__
#define __TQIPARTICLEFOURVECTORVECTOROBSERVABLE__
#include "CAFxAODUtils/TQEventObservable.h"

#ifndef __CINT__
#define XAOD_STANDALONE 1
#include "xAODBase/IParticleContainer.h"
using namespace xAOD;
#else
class IParticleContainer;
class IParticle;
#endif


class TQIParticleFourVectorVectorObservable : public TQEventObservable {
protected:
  TString fExpression = "";
  TString fContainerName = "";
  enum V4Quantity{ETA,PHI,E,M,PT,UNKNOWN};
  V4Quantity fQuantity;
  mutable xAOD::IParticleContainer const * m_container = nullptr;

  virtual bool initializeSelf() override;
  virtual bool finalizeSelf() override;

  mutable std::vector<double> fCache;
  mutable Long64_t fCachedEntry;
  bool makeCache() const;

public:
  virtual double getValue() const override;
  virtual double getValueAt(int index) const override;
  virtual int getNevaluations() const override;
  virtual bool hasExpression() const override;
  virtual const TString& getExpression() const override;
  virtual void setExpression(const TString& expr) override;

  TQIParticleFourVectorVectorObservable();
  TQIParticleFourVectorVectorObservable(const TString& expression);
  virtual ~TQIParticleFourVectorVectorObservable();

  inline virtual TQObservable::ObservableType getObservableType() const override {return TQObservable::ObservableType::vector;}


  static bool parseContainerName(TString& expression, TString& containerName);
  bool parseExpression(const TString& expr);
  void clearParsedExpression();

  virtual TString getActiveExpression() const override;

  DECLARE_OBSERVABLE_FACTORY(TQIParticleFourVectorVectorObservable,TString expression)

  ClassDefOverride(TQIParticleFourVectorVectorObservable, 0);

};
#endif
