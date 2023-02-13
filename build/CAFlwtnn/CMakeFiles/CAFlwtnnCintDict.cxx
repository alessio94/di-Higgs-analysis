// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME CAFlwtnnCintDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "CAFlwtnn/TQlwtnnObservable.h"
#include "CAFlwtnn/TQlwtnnObservable.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TQlwtnnObservable(void *p = 0);
   static void *newArray_TQlwtnnObservable(Long_t size, void *p);
   static void delete_TQlwtnnObservable(void *p);
   static void deleteArray_TQlwtnnObservable(void *p);
   static void destruct_TQlwtnnObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQlwtnnObservable*)
   {
      ::TQlwtnnObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQlwtnnObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQlwtnnObservable", ::TQlwtnnObservable::Class_Version(), "CAFlwtnn/TQlwtnnObservable.h", 25,
                  typeid(::TQlwtnnObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQlwtnnObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQlwtnnObservable) );
      instance.SetNew(&new_TQlwtnnObservable);
      instance.SetNewArray(&newArray_TQlwtnnObservable);
      instance.SetDelete(&delete_TQlwtnnObservable);
      instance.SetDeleteArray(&deleteArray_TQlwtnnObservable);
      instance.SetDestructor(&destruct_TQlwtnnObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQlwtnnObservable*)
   {
      return GenerateInitInstanceLocal((::TQlwtnnObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQlwtnnObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TQlwtnnObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQlwtnnObservable::Class_Name()
{
   return "TQlwtnnObservable";
}

//______________________________________________________________________________
const char *TQlwtnnObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQlwtnnObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQlwtnnObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQlwtnnObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQlwtnnObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQlwtnnObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQlwtnnObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQlwtnnObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TQlwtnnObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQlwtnnObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQlwtnnObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQlwtnnObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQlwtnnObservable(void *p) {
      return  p ? new(p) ::TQlwtnnObservable : new ::TQlwtnnObservable;
   }
   static void *newArray_TQlwtnnObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQlwtnnObservable[nElements] : new ::TQlwtnnObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQlwtnnObservable(void *p) {
      delete ((::TQlwtnnObservable*)p);
   }
   static void deleteArray_TQlwtnnObservable(void *p) {
      delete [] ((::TQlwtnnObservable*)p);
   }
   static void destruct_TQlwtnnObservable(void *p) {
      typedef ::TQlwtnnObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQlwtnnObservable

namespace {
  void TriggerDictionaryInitialization_libCAFlwtnn_Impl() {
    static const char* headers[] = {
"CAFlwtnn/TQlwtnnObservable.h",
0
    };
    static const char* includePaths[] = {
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFlwtnn",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFlwtnn",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFlwtnn",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFlwtnn",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFlwtnn",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/QFramework",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCore",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthContainers",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthContainersInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/CxxUtils",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthLinksSA",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/xAODRootAccessInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/xAODRootAccess",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEventFormat",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTruth",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODBase",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCutFlow",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFlwtnn",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFlwtnn/CMakeFiles/makeCAFlwtnnCintDict.gq5i8I/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCAFlwtnn dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$CAFlwtnn/TQlwtnnObservable.h")))  TQlwtnnObservable;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCAFlwtnn dictionary payload"

#ifndef HAVE_PRETTY_FUNCTION
  #define HAVE_PRETTY_FUNCTION 1
#endif
#ifndef HAVE_64_BITS
  #define HAVE_64_BITS 1
#endif
#ifndef __IDENTIFIER_64BIT__
  #define __IDENTIFIER_64BIT__ 1
#endif
#ifndef ATLAS
  #define ATLAS 1
#endif
#ifndef ROOTCORE
  #define ROOTCORE 1
#endif
#ifndef XAOD_STANDALONE
  #define XAOD_STANDALONE 1
#endif
#ifndef XAOD_ANALYSIS
  #define XAOD_ANALYSIS 1
#endif
#ifndef ROOTCORE_RELEASE_SERIES
  #define ROOTCORE_RELEASE_SERIES 25
#endif
#ifndef PACKAGE_VERSION
  #define PACKAGE_VERSION "CAFlwtnn-00-00-00"
#endif
#ifndef PACKAGE_VERSION_UQ
  #define PACKAGE_VERSION_UQ CAFlwtnn-00-00-00
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CAFlwtnn/TQlwtnnObservable.h"

#ifndef __CAFlwtnnDICT__
#define __CAFlwtnnDICT__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winconsistent-missing-override"

#include "CAFlwtnn/TQlwtnnObservable.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
// >>> begin TQlwtnnObservable.h
#pragma link C++ class TQlwtnnObservable+;
// <<< end TQlwtnnObservable.h


#endif //__CINT__
#endif //__CAFlwtnnDICT__

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TQlwtnnObservable", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCAFlwtnn",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCAFlwtnn_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCAFlwtnn_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCAFlwtnn() {
  TriggerDictionaryInitialization_libCAFlwtnn_Impl();
}
