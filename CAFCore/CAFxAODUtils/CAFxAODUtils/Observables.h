//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef CAFXAODUTILS_OBSERVABLES
#define CAFXAODUTILS_OBSERVABLES

#include "QFramework/TQObservable.h"

namespace TQObservableFactory {
  bool setupCAFxAODUtils();
}

// provide printint for the types required
DEFINE_TEMPLATE_ARGUMENT( float )
DEFINE_TEMPLATE_ARGUMENT( double )
DEFINE_TEMPLATE_ARGUMENT( char )
DEFINE_TEMPLATE_ARGUMENT( int )
DEFINE_TEMPLATE_ARGUMENT( unsigned int )
DEFINE_TEMPLATE_ARGUMENT( unsigned short )
DEFINE_TEMPLATE_ARGUMENT( unsigned long long )
DEFINE_TEMPLATE_ARGUMENT( bool )
// this preprocessor macro makes typedefs for templated classes
// for type/X=float/F,double/D,char/C,int/I,unsigned int/U,unsigned short/US,unsinged long long/ULL
// we obtain
// typedef FIRST SECOND <type> -> FIRST X SECOND;
#define TYPEDEF_ALL_TEMPLATES(FIRST,SECOND)                    \
  typedef FIRST ## SECOND<float>         FIRST ## F ## SECOND; \
  typedef FIRST ## SECOND<double>        FIRST ## D ## SECOND; \
  typedef FIRST ## SECOND<char>          FIRST ## C ## SECOND; \
  typedef FIRST ## SECOND<int>           FIRST ## I ## SECOND; \
  typedef FIRST ## SECOND<unsigned int>  FIRST ## U ## SECOND; \
  typedef FIRST ## SECOND<unsigned short>  FIRST ## US ## SECOND; \
  typedef FIRST ## SECOND<unsigned long long>  FIRST ## ULL ## SECOND;\
  typedef FIRST ## SECOND<bool>         FIRST ## B ## SECOND;
// this preprocessor macro makes the getExpressionPrefix implementation
// for templated classes. for type/X=float/F,double/D,char/C,int/I,unsigned int/U,unsigned short/US,unsinged long long/ULL
// we obtain
// CLASS<type>::getExpressionPrefix() -> FIRST X SECOND;
#define PREFIX_ALL_TEMPLATES(CLASS,FIRST,SECOND)                        \
  template <> TString CLASS<float>::getExpressionPrefix() { return #FIRST "F" #SECOND; } \
  template <> TString CLASS<double>::getExpressionPrefix() { return #FIRST "D" #SECOND; } \
  template <> TString CLASS<char>::getExpressionPrefix() { return #FIRST "C" #SECOND; } \
  template <> TString CLASS<unsigned int>::getExpressionPrefix() { return #FIRST "U" #SECOND; } \
  template <> TString CLASS<unsigned short>::getExpressionPrefix() { return #FIRST "US" #SECOND; } \
  template <> TString CLASS<unsigned long long>::getExpressionPrefix() { return #FIRST "ULL" #SECOND; } \
  template <> TString CLASS<int>::getExpressionPrefix() { return #FIRST "I" #SECOND; } \
  template <> TString CLASS<bool>::getExpressionPrefix() { return #FIRST "B" #SECOND; }
// this preprocessor macro instantiantes templated classes
// for type=float,double,char,int,unsigned int
#define INSTANTIATE_ALL_TEMPLATES(ARG)                   \
  template class ARG<float>        ;                     \
  template class ARG<double>       ;                     \
  template class ARG<char>         ;                     \
  template class ARG<int>          ;                     \
  template class ARG<unsigned int> ;                     \
  template class ARG<unsigned short> ;                   \
  template class ARG<unsigned long long> ;                   \
//  template class ARG<bool> ;
// this preprocessor macro registers all factories of templated classes
// with the TQObservable manager
#define REGISTER_ALL_TEMPLATES(ARG)                                     \
  TQObservable::manager.registerFactory(ARG<int>::getFactory(),true);   \
  TQObservable::manager.registerFactory(ARG<unsigned int>::getFactory(),true); \
  TQObservable::manager.registerFactory(ARG<unsigned short>::getFactory(),true); \
  TQObservable::manager.registerFactory(ARG<unsigned long long>::getFactory(),true); \
  TQObservable::manager.registerFactory(ARG<char>::getFactory(),true);  \
  TQObservable::manager.registerFactory(ARG<float>::getFactory(),true); \
  TQObservable::manager.registerFactory(ARG<double>::getFactory(),true);\
  //TQObservable::manager.registerFactory(ARG<bool>::getFactory(),true)
#endif
