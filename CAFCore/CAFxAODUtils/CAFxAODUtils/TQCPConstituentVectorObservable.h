//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQCPCONSTITUENTVECTOROBSERVABLE__
#define __TQCPCONSTITUENTVECTOROBSERVABLE__
#include "CAFxAODUtils/TQEventObservable.h"

#ifndef __CINT__
#define XAOD_STANDALONE 1
#include "xAODParticleEvent/CompositeParticleContainer.h"
#endif

class TQCPConstituentVectorObservable : public TQEventObservable { 
protected:
  enum class IterateVariant{PART,OTHERPART,ANYPART,UNKNOWN};
  IterateVariant fIterateVariant = IterateVariant::UNKNOWN; //!
  enum class Method{ETA,PHI,E,M,PT,RAPIDITY,TYPE,UNKNOWN}; /*PX,PY,PZ,ET,CHARGE,PDGID,*/
  Method fMethod = Method::UNKNOWN; //!

  TString fExpression = "";
  TString fContainerName = ""; //!
  unsigned int fContainerIndex = 0; //!
  std::vector<unsigned int> fParticleIndices; //!
  mutable TString fCachedExpression; //!
  std::vector<bool> fUseOtherParts; //!
  
  bool parseObjectDef(const TString& objDef);
  bool setMethod(TString& method);
  bool setIterMode(TString& iter);
  
  static TString getMethodString(Method method);
  static TString getIterateVariantString(IterateVariant iterVar);
  
#ifndef __CINT__
  const xAOD::CompositeParticle* getLastCompositeParticle() const;
  double getSingleValue(const xAOD::IParticle* part) const;
#endif

  void clearParsedExpression();
  //bool parseExpression(const TString& expr);

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

  TQCPConstituentVectorObservable();
  static TQCPConstituentVectorObservable* createInstance(const TString& expression);
  virtual ~TQCPConstituentVectorObservable();

  DECLARE_OBSERVABLE_FACTORY(TQCPConstituentVectorObservable,TString expression);
  
  ClassDefOverride(TQCPConstituentVectorObservable,1);
};


//#include "CAFxAODUtils/Observables.h"

#endif
