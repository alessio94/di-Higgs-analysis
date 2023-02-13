//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQTOOLOBSERVABLE__
#define __TQTOOLOBSERVABLE__
#include "QFramework/TQTaggable.h"
#include "CAFxAODUtils/TQEventObservable.h"

#include "AsgTools/ToolHandle.h"
#include "AsgAnalysisInterfaces/IObservableTool.h"

class TQToolObservable : public TQEventObservable, public TQTaggable {
protected:
  TString fExpression = "";
  TString fToolName = "";
  TString fContainerName = "";
  unsigned int fContainerIndex = 0;
  std::vector<unsigned int> fParticleIndices;

  IObservableTool* fTool = NULL;


  virtual bool initializeSelf() override;
  virtual bool finalizeSelf() override;
  
public:
  TQToolObservable();
  TQToolObservable(const TString& expression);
  virtual ~TQToolObservable();

  static bool registerTool(IObservableTool* tool);
  static IObservableTool* getTool(const char* name);
  
  virtual double getValue() const override;
  virtual bool hasExpression() const override;
  virtual const TString& getExpression() const override;
  virtual void setExpression(const TString& expr) override;

  bool parseExpression(const TString& expr);
  void clearParsedExpression();

  virtual TString getActiveExpression() const override;

  DECLARE_OBSERVABLE_FACTORY(TQToolObservable,TString expression);

  ClassDefOverride(TQToolObservable, 0);

};
#endif
