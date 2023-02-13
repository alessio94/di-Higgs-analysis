//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQEventObservable__
#define __TQEventObservable__

#include "TString.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQObservable.h"

class TQToken;

#ifndef __CINT__
namespace xAOD { // EXCLUDE this from LinkDef.h (please do not delete/modify this magical comment)
  class TEvent;
}
namespace asg {
  class SgTEvent;
}
#endif

class TQEventObservable : public TQObservable {
protected:
  TQToken* fEventToken = NULL;
  asg::SgTEvent* fEvent = NULL;

  virtual bool initializeSelf() override;
  virtual bool finalizeSelf() override;

  virtual TObjArray* getBranchNames() const override;

  template< typename T > inline void retrieve( T*& pobj, const char* name );
  template< typename T > inline void retrieve( const T*& pobj, const char* name ) const;
  
public:

  TQEventObservable();
  TQEventObservable(const TString& expression);
  virtual ~TQEventObservable();

  static bool parseParticleReference(TString& expression, TString& containerName, unsigned int& containerIndex);
  static bool parseParticleReference(TString& expression, TString& containerName, unsigned int& containerIndex, std::vector<unsigned int>& particleIndices, const TString& stopatchars = "");
  static bool parseParticleReference(TString& expression, TString& containerName, unsigned int& containerIndex, std::vector<unsigned int>& particleIndices, std::vector<bool>& useOtherParts, const TString& stopatchars = "");

  virtual bool initialize(TQSample * sample) override;
  virtual bool finalize() override;

  virtual Long64_t getCurrentEntry() const override;
  virtual void print() const override;

  ClassDefOverride(TQEventObservable, 0);
};

#ifndef __CINT__
#include "CAFxAODUtils/TQEventObservable.icc"
#endif

#endif
