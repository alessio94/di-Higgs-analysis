//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQCounter__
#define __TQCounter__

#include "TNamed.h"

class TQCounter : public TNamed {

protected:
 
  double fCounter; // counter
  double fErrorSquared; // error
  int fRawCounter; // raw counter

  // TODO: remove
  bool fWarning;


public:

  static TString getComparison(TQCounter * cnt1, TQCounter * cnt2, bool colored = false, double order = 1E-7);

  TQCounter();
  TQCounter(const char * name);
  TQCounter(const char * name, const char* title);
  TQCounter(const TString& name, double cnt, double err = 0.);
  TQCounter(const TString& name, double cnt, double err, int raw);
  TQCounter(const TQCounter * counter);

  double getCounter() const ;
  void setCounter(double counter);
  int getRawCounter() const;
  void setRawCounter(int raw);

  double getError() const;
  double getErrorSquared() const;
  void setErrorSquared(double errorSquared);
  void setError(double error);

  double getStatError() const;
  double getStatErrorSquared() const;

  double getSysError() const;
  double getSysErrorSquared() const;

 
  void reset();

  TString getAsString(const TString& options = "") const;
  void print(const TString& options = "") const;
  void printCounter(const TString& options = "") const;

  void setWarning(bool warning = true);
  bool getWarning() const ;
 
  void add(double weight);
  void add(const TQCounter * counter, double scale = 1., double scaleUncertainty = 0., double correlation = 0., bool includeScaleUncertainty = true);
  void add(const TQCounter * counter, TQCounter * scale, double correlation = 0., bool includeScaleUncertainty = false);
  void subtract(const TQCounter * counter, double scale = 1., double scaleUncertainty = 0., double correlation = 0., bool includeScaleUncertainty = true);
  void subtract(const TQCounter * counter, TQCounter* scale, double correlation = 0., bool includeScaleUncertainty = false);
  void multiply(double factor, double uncertainty = 0., double correlation = 0., bool includeScaleUncertainty = true);
  void multiply(const TQCounter * counter, double correlation = 0., bool includeScaleUncertainty = false);
  void divide(double denominator, double uncertainty = 0., double correlation = 0., bool includeScaleUncertainty = true);
  void divide(const TQCounter * counter, double correlation = 0, bool includeScaleUncertainty = false);
 
  void scale(double factor, double scaleUncertainty = 0., bool includeScaleUncertainty = true);
  void scale(const TQCounter * scale, bool includeScaleUncertainty = true);

  bool isEqualTo(TQCounter * counter, double order = 1E-7);
  bool isRawEqualTo(TQCounter * counter);

  virtual ~TQCounter();

  ClassDefOverride(TQCounter, 1); // event counter
};

#endif

