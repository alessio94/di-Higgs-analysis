// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME CxAODToolsDictReflexDict
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
#include "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CxAODTools/CxAODTools/CxAODToolsDict.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *DataVectorlExAODcLcLOverlapRegistergR_Dictionary();
   static void DataVectorlExAODcLcLOverlapRegistergR_TClassManip(TClass*);
   static void *new_DataVectorlExAODcLcLOverlapRegistergR(void *p = 0);
   static void *newArray_DataVectorlExAODcLcLOverlapRegistergR(Long_t size, void *p);
   static void delete_DataVectorlExAODcLcLOverlapRegistergR(void *p);
   static void deleteArray_DataVectorlExAODcLcLOverlapRegistergR(void *p);
   static void destruct_DataVectorlExAODcLcLOverlapRegistergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DataVector<xAOD::OverlapRegister>*)
   {
      ::DataVector<xAOD::OverlapRegister> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::DataVector<xAOD::OverlapRegister>));
      static ::ROOT::TGenericClassInfo 
         instance("DataVector<xAOD::OverlapRegister>", "AthContainers/DataVector.h", 1946,
                  typeid(::DataVector<xAOD::OverlapRegister>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &DataVectorlExAODcLcLOverlapRegistergR_Dictionary, isa_proxy, 4,
                  sizeof(::DataVector<xAOD::OverlapRegister>) );
      instance.SetNew(&new_DataVectorlExAODcLcLOverlapRegistergR);
      instance.SetNewArray(&newArray_DataVectorlExAODcLcLOverlapRegistergR);
      instance.SetDelete(&delete_DataVectorlExAODcLcLOverlapRegistergR);
      instance.SetDeleteArray(&deleteArray_DataVectorlExAODcLcLOverlapRegistergR);
      instance.SetDestructor(&destruct_DataVectorlExAODcLcLOverlapRegistergR);

      ::ROOT::AddClassAlternate("DataVector<xAOD::OverlapRegister>","xAOD::OverlapRegisterContainer");

      ::ROOT::AddClassAlternate("DataVector<xAOD::OverlapRegister>","DataVector<xAOD::OverlapRegister, DataModel_detail::NoBase>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DataVector<xAOD::OverlapRegister>*)
   {
      return GenerateInitInstanceLocal((::DataVector<xAOD::OverlapRegister>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DataVector<xAOD::OverlapRegister>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *DataVectorlExAODcLcLOverlapRegistergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::DataVector<xAOD::OverlapRegister>*)0x0)->GetClass();
      DataVectorlExAODcLcLOverlapRegistergR_TClassManip(theClass);
   return theClass;
   }

   static void DataVectorlExAODcLcLOverlapRegistergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *LumiMetaDataTool_Dictionary();
   static void LumiMetaDataTool_TClassManip(TClass*);
   static void *new_LumiMetaDataTool(void *p = 0);
   static void *newArray_LumiMetaDataTool(Long_t size, void *p);
   static void delete_LumiMetaDataTool(void *p);
   static void deleteArray_LumiMetaDataTool(void *p);
   static void destruct_LumiMetaDataTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LumiMetaDataTool*)
   {
      ::LumiMetaDataTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::LumiMetaDataTool));
      static ::ROOT::TGenericClassInfo 
         instance("LumiMetaDataTool", "LumiMetaDataTool.h", 22,
                  typeid(::LumiMetaDataTool), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &LumiMetaDataTool_Dictionary, isa_proxy, 4,
                  sizeof(::LumiMetaDataTool) );
      instance.SetNew(&new_LumiMetaDataTool);
      instance.SetNewArray(&newArray_LumiMetaDataTool);
      instance.SetDelete(&delete_LumiMetaDataTool);
      instance.SetDeleteArray(&deleteArray_LumiMetaDataTool);
      instance.SetDestructor(&destruct_LumiMetaDataTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LumiMetaDataTool*)
   {
      return GenerateInitInstanceLocal((::LumiMetaDataTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LumiMetaDataTool*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *LumiMetaDataTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::LumiMetaDataTool*)0x0)->GetClass();
      LumiMetaDataTool_TClassManip(theClass);
   return theClass;
   }

   static void LumiMetaDataTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *xAODcLcLOverlapRegister_Dictionary();
   static void xAODcLcLOverlapRegister_TClassManip(TClass*);
   static void *new_xAODcLcLOverlapRegister(void *p = 0);
   static void *newArray_xAODcLcLOverlapRegister(Long_t size, void *p);
   static void delete_xAODcLcLOverlapRegister(void *p);
   static void deleteArray_xAODcLcLOverlapRegister(void *p);
   static void destruct_xAODcLcLOverlapRegister(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::xAOD::OverlapRegister*)
   {
      ::xAOD::OverlapRegister *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::xAOD::OverlapRegister));
      static ::ROOT::TGenericClassInfo 
         instance("xAOD::OverlapRegister", "OverlapRegister.h", 22,
                  typeid(::xAOD::OverlapRegister), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &xAODcLcLOverlapRegister_Dictionary, isa_proxy, 4,
                  sizeof(::xAOD::OverlapRegister) );
      instance.SetNew(&new_xAODcLcLOverlapRegister);
      instance.SetNewArray(&newArray_xAODcLcLOverlapRegister);
      instance.SetDelete(&delete_xAODcLcLOverlapRegister);
      instance.SetDeleteArray(&deleteArray_xAODcLcLOverlapRegister);
      instance.SetDestructor(&destruct_xAODcLcLOverlapRegister);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::xAOD::OverlapRegister*)
   {
      return GenerateInitInstanceLocal((::xAOD::OverlapRegister*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::xAOD::OverlapRegister*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *xAODcLcLOverlapRegister_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::xAOD::OverlapRegister*)0x0)->GetClass();
      xAODcLcLOverlapRegister_TClassManip(theClass);
   return theClass;
   }

   static void xAODcLcLOverlapRegister_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *xAODcLcLOverlapRegisterAuxContainer_Dictionary();
   static void xAODcLcLOverlapRegisterAuxContainer_TClassManip(TClass*);
   static void *new_xAODcLcLOverlapRegisterAuxContainer(void *p = 0);
   static void *newArray_xAODcLcLOverlapRegisterAuxContainer(Long_t size, void *p);
   static void delete_xAODcLcLOverlapRegisterAuxContainer(void *p);
   static void deleteArray_xAODcLcLOverlapRegisterAuxContainer(void *p);
   static void destruct_xAODcLcLOverlapRegisterAuxContainer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::xAOD::OverlapRegisterAuxContainer*)
   {
      ::xAOD::OverlapRegisterAuxContainer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::xAOD::OverlapRegisterAuxContainer));
      static ::ROOT::TGenericClassInfo 
         instance("xAOD::OverlapRegisterAuxContainer", "OverlapRegister.h", 66,
                  typeid(::xAOD::OverlapRegisterAuxContainer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &xAODcLcLOverlapRegisterAuxContainer_Dictionary, isa_proxy, 4,
                  sizeof(::xAOD::OverlapRegisterAuxContainer) );
      instance.SetNew(&new_xAODcLcLOverlapRegisterAuxContainer);
      instance.SetNewArray(&newArray_xAODcLcLOverlapRegisterAuxContainer);
      instance.SetDelete(&delete_xAODcLcLOverlapRegisterAuxContainer);
      instance.SetDeleteArray(&deleteArray_xAODcLcLOverlapRegisterAuxContainer);
      instance.SetDestructor(&destruct_xAODcLcLOverlapRegisterAuxContainer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::xAOD::OverlapRegisterAuxContainer*)
   {
      return GenerateInitInstanceLocal((::xAOD::OverlapRegisterAuxContainer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::xAOD::OverlapRegisterAuxContainer*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *xAODcLcLOverlapRegisterAuxContainer_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::xAOD::OverlapRegisterAuxContainer*)0x0)->GetClass();
      xAODcLcLOverlapRegisterAuxContainer_TClassManip(theClass);
   return theClass;
   }

   static void xAODcLcLOverlapRegisterAuxContainer_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_DataVectorlExAODcLcLOverlapRegistergR(void *p) {
      return  p ? new(p) ::DataVector<xAOD::OverlapRegister> : new ::DataVector<xAOD::OverlapRegister>;
   }
   static void *newArray_DataVectorlExAODcLcLOverlapRegistergR(Long_t nElements, void *p) {
      return p ? new(p) ::DataVector<xAOD::OverlapRegister>[nElements] : new ::DataVector<xAOD::OverlapRegister>[nElements];
   }
   // Wrapper around operator delete
   static void delete_DataVectorlExAODcLcLOverlapRegistergR(void *p) {
      delete ((::DataVector<xAOD::OverlapRegister>*)p);
   }
   static void deleteArray_DataVectorlExAODcLcLOverlapRegistergR(void *p) {
      delete [] ((::DataVector<xAOD::OverlapRegister>*)p);
   }
   static void destruct_DataVectorlExAODcLcLOverlapRegistergR(void *p) {
      typedef ::DataVector<xAOD::OverlapRegister> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DataVector<xAOD::OverlapRegister>

namespace ROOT {
   // Wrappers around operator new
   static void *new_LumiMetaDataTool(void *p) {
      return  p ? new(p) ::LumiMetaDataTool : new ::LumiMetaDataTool;
   }
   static void *newArray_LumiMetaDataTool(Long_t nElements, void *p) {
      return p ? new(p) ::LumiMetaDataTool[nElements] : new ::LumiMetaDataTool[nElements];
   }
   // Wrapper around operator delete
   static void delete_LumiMetaDataTool(void *p) {
      delete ((::LumiMetaDataTool*)p);
   }
   static void deleteArray_LumiMetaDataTool(void *p) {
      delete [] ((::LumiMetaDataTool*)p);
   }
   static void destruct_LumiMetaDataTool(void *p) {
      typedef ::LumiMetaDataTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LumiMetaDataTool

namespace ROOT {
   // Wrappers around operator new
   static void *new_xAODcLcLOverlapRegister(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::xAOD::OverlapRegister : new ::xAOD::OverlapRegister;
   }
   static void *newArray_xAODcLcLOverlapRegister(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::xAOD::OverlapRegister[nElements] : new ::xAOD::OverlapRegister[nElements];
   }
   // Wrapper around operator delete
   static void delete_xAODcLcLOverlapRegister(void *p) {
      delete ((::xAOD::OverlapRegister*)p);
   }
   static void deleteArray_xAODcLcLOverlapRegister(void *p) {
      delete [] ((::xAOD::OverlapRegister*)p);
   }
   static void destruct_xAODcLcLOverlapRegister(void *p) {
      typedef ::xAOD::OverlapRegister current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::xAOD::OverlapRegister

namespace ROOT {
   // Wrappers around operator new
   static void *new_xAODcLcLOverlapRegisterAuxContainer(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::xAOD::OverlapRegisterAuxContainer : new ::xAOD::OverlapRegisterAuxContainer;
   }
   static void *newArray_xAODcLcLOverlapRegisterAuxContainer(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::xAOD::OverlapRegisterAuxContainer[nElements] : new ::xAOD::OverlapRegisterAuxContainer[nElements];
   }
   // Wrapper around operator delete
   static void delete_xAODcLcLOverlapRegisterAuxContainer(void *p) {
      delete ((::xAOD::OverlapRegisterAuxContainer*)p);
   }
   static void deleteArray_xAODcLcLOverlapRegisterAuxContainer(void *p) {
      delete [] ((::xAOD::OverlapRegisterAuxContainer*)p);
   }
   static void destruct_xAODcLcLOverlapRegisterAuxContainer(void *p) {
      typedef ::xAOD::OverlapRegisterAuxContainer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::xAOD::OverlapRegisterAuxContainer

namespace {
  void TriggerDictionaryInitialization_libCxAODToolsDict_Impl() {
    static const char* headers[] = {
"0",
0
    };
    static const char* includePaths[] = {
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCxAODToolsDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace xAOD{class __attribute__((annotate("$clingAutoload$CxAODTools/OverlapRegister.h")))  OverlapRegister;}
class __attribute__((annotate("$clingAutoload$CxAODTools/LumiMetaDataTool.h")))  LumiMetaDataTool;
namespace xAOD{class __attribute__((annotate("$clingAutoload$CxAODTools/OverlapRegister.h")))  OverlapRegisterAuxContainer;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCxAODToolsDict dictionary payload"

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
  #define PACKAGE_VERSION "CxAODTools-00-00-00"
#endif
#ifndef PACKAGE_VERSION_UQ
  #define PACKAGE_VERSION_UQ CxAODTools-00-00-00
#endif
#ifndef EIGEN_DONT_VECTORIZE
  #define EIGEN_DONT_VECTORIZE 1
#endif
#ifndef USE_CMAKE
  #define USE_CMAKE 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#ifndef CXAODTOOLS_CXAODTOOLSDICT
#define CXAODTOOLS_CXAODTOOLSDICT

// Local include(s):
#include "CxAODTools/LumiMetaDataTool.h"
#include "CxAODTools/OverlapRegister.h"
#endif


#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"DataVector<xAOD::OverlapRegister>", payloadCode, "@",
"LumiMetaDataTool", payloadCode, "@",
"xAOD::OverlapRegister", payloadCode, "@",
"xAOD::OverlapRegisterAuxContainer", payloadCode, "@",
"xAOD::OverlapRegisterContainer", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCxAODToolsDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCxAODToolsDict_Impl, {{"namespace DataVector_detail { template <typename B1, typename B2, typename B3> class VirtBases; }", 1},{"template <typename T> class DataVectorBase;", 1},{"template <typename T, typename BASE> class DataVector;", 1}}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCxAODToolsDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCxAODToolsDict() {
  TriggerDictionaryInitialization_libCxAODToolsDict_Impl();
}
