//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQCONTAINERSIZEOBSERVABLE__
#define __TQCONTAINERSIZEOBSERVABLE__
#include "CAFxAODUtils/TQEventObservable.h"

class TQContainerSizeObservable : public TQEventObservable {
public:
  virtual double getValue() const override;
protected:
  TString fExpression = "";

public:
  virtual bool hasExpression() const override;
  virtual const TString& getExpression() const override;
  virtual void setExpression(const TString& expr) override;

  TQContainerSizeObservable();
  TQContainerSizeObservable(const TString& expression);
  virtual ~TQContainerSizeObservable();
public:
  DECLARE_OBSERVABLE_FACTORY(TQContainerSizeObservable,TString expr)

  ClassDefOverride(TQContainerSizeObservable, 0);

};
#endif
