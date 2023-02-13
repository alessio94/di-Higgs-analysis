// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TTbarNNLOReweighterCintDict
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
#include "TTbarNNLOReweighter/TTbarNNLOReweighter.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *TTbarNNLOReweighter_Dictionary();
   static void TTbarNNLOReweighter_TClassManip(TClass*);
   static void *new_TTbarNNLOReweighter(void *p = 0);
   static void *newArray_TTbarNNLOReweighter(Long_t size, void *p);
   static void delete_TTbarNNLOReweighter(void *p);
   static void deleteArray_TTbarNNLOReweighter(void *p);
   static void destruct_TTbarNNLOReweighter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TTbarNNLOReweighter*)
   {
      ::TTbarNNLOReweighter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TTbarNNLOReweighter));
      static ::ROOT::TGenericClassInfo 
         instance("TTbarNNLOReweighter", "TTbarNNLOReweighter/TTbarNNLOReweighter.h", 13,
                  typeid(::TTbarNNLOReweighter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TTbarNNLOReweighter_Dictionary, isa_proxy, 4,
                  sizeof(::TTbarNNLOReweighter) );
      instance.SetNew(&new_TTbarNNLOReweighter);
      instance.SetNewArray(&newArray_TTbarNNLOReweighter);
      instance.SetDelete(&delete_TTbarNNLOReweighter);
      instance.SetDeleteArray(&deleteArray_TTbarNNLOReweighter);
      instance.SetDestructor(&destruct_TTbarNNLOReweighter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TTbarNNLOReweighter*)
   {
      return GenerateInitInstanceLocal((::TTbarNNLOReweighter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TTbarNNLOReweighter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TTbarNNLOReweighter_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TTbarNNLOReweighter*)0x0)->GetClass();
      TTbarNNLOReweighter_TClassManip(theClass);
   return theClass;
   }

   static void TTbarNNLOReweighter_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_TTbarNNLOReweighter(void *p) {
      return  p ? new(p) ::TTbarNNLOReweighter : new ::TTbarNNLOReweighter;
   }
   static void *newArray_TTbarNNLOReweighter(Long_t nElements, void *p) {
      return p ? new(p) ::TTbarNNLOReweighter[nElements] : new ::TTbarNNLOReweighter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TTbarNNLOReweighter(void *p) {
      delete ((::TTbarNNLOReweighter*)p);
   }
   static void deleteArray_TTbarNNLOReweighter(void *p) {
      delete [] ((::TTbarNNLOReweighter*)p);
   }
   static void destruct_TTbarNNLOReweighter(void *p) {
      typedef ::TTbarNNLOReweighter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TTbarNNLOReweighter

namespace {
  void TriggerDictionaryInitialization_libTTbarNNLOReweighter_Impl() {
    static const char* headers[] = {
"TTbarNNLOReweighter/TTbarNNLOReweighter.h",
0
    };
    static const char* includePaths[] = {
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/TTbarNNLOReweighter",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/TTbarNNLOReweighter",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Tools/PathResolver",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthToolSupport/AsgTools",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthToolSupport/AsgMessaging",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/xAODRootAccessInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/xAODRootAccess",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/CxxUtils",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthContainersInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthContainers",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthLinksSA",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCore",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEventFormat",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/TTbarNNLOReweighter",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/TTbarNNLOReweighter",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Tools/PathResolver",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthToolSupport/AsgTools",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthToolSupport/AsgMessaging",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/xAODRootAccessInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/xAODRootAccess",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/CxxUtils",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthContainersInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthContainers",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthLinksSA",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCore",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEventFormat",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/TTbarNNLOReweighter/CMakeFiles/makeTTbarNNLOReweighterCintDict.lPJ26w/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libTTbarNNLOReweighter dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$TTbarNNLOReweighter/TTbarNNLOReweighter.h")))  TTbarNNLOReweighter;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libTTbarNNLOReweighter dictionary payload"

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
  #define PACKAGE_VERSION "TTbarNNLOReweighter-00-00-00"
#endif
#ifndef PACKAGE_VERSION_UQ
  #define PACKAGE_VERSION_UQ TTbarNNLOReweighter-00-00-00
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "TTbarNNLOReweighter/TTbarNNLOReweighter.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TTbarNNLOReweighter", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libTTbarNNLOReweighter",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libTTbarNNLOReweighter_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libTTbarNNLOReweighter_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libTTbarNNLOReweighter() {
  TriggerDictionaryInitialization_libTTbarNNLOReweighter_Impl();
}
