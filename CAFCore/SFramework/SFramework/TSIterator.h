//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TSIterator__
#define __TSIterator__

#include "QFramework/TQIterator.h"

//////////////////////////////////////////////////////////////////////
//
// IMPORTANT NOTICE for adding custom class iterators:
//
// If you want to add a typedef and/or be able to access a TQ...Iterator
// fromt the ROOT shell, you need to undertake the following steps:
// - add a line 
// typedef TQIteratorT<MyClass> TQMyClassIterator; 
// to TQIterator.h
// - add an include statement
// #include <MyClass.h> 
// in the 'include'-section of TQIteratorT.h
// - add the line 
// #pragma link C++ class TQIteratorT<TQValue>+;
// to TQIteratorTLinkDef.h.add
// 
//////////////////////////////////////////////////////////////////////

typedef TQIteratorT<RooWorkspace> RooWorkspaceIterator;
typedef TQIteratorT<RooRealVar> RooRealVarIterator;

#endif
