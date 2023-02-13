//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQNamedTaggable__
#define __TQNamedTaggable__

#include "TNamed.h"
#include "QFramework/TQTaggable.h"

class TQNamedTaggable : public TNamed, public TQTaggable {

public:

  TQNamedTaggable();
  TQNamedTaggable(const TString& name);
  TQNamedTaggable(const TString& name, const TString& tags);
  TQNamedTaggable(const TString& name, TQTaggable * tags);
  TQNamedTaggable(TQNamedTaggable * tags);

  virtual ~TQNamedTaggable();

  virtual TString getName() const;
  virtual void setName(const TString& newName);
  virtual const TString& getNameConst() const;

  virtual TString getTitle();
  virtual void setTitle(const TString& newTitle);
  virtual const TString& getTitleConst() const;

  ClassDefOverride(TQNamedTaggable, 1); // base class for named taggable objects

};

#endif
