// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME CxAODToolsCintDict
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
#include "CxAODTools/ConfigFieldBase.h"
#include "CxAODTools/ConfigField.h"
#include "CxAODTools/ConfigStore.h"
#include "CxAODTools/OverlapRegister.h"
#include "CxAODTools/ConfigFieldBase.h"
#include "CxAODTools/ConfigField.h"
#include "CxAODTools/ConfigStore.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_ConfigFieldBase(void *p);
   static void deleteArray_ConfigFieldBase(void *p);
   static void destruct_ConfigFieldBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigFieldBase*)
   {
      ::ConfigFieldBase *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigFieldBase >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigFieldBase", ::ConfigFieldBase::Class_Version(), "CxAODTools/ConfigFieldBase.h", 8,
                  typeid(::ConfigFieldBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ConfigFieldBase::Dictionary, isa_proxy, 4,
                  sizeof(::ConfigFieldBase) );
      instance.SetDelete(&delete_ConfigFieldBase);
      instance.SetDeleteArray(&deleteArray_ConfigFieldBase);
      instance.SetDestructor(&destruct_ConfigFieldBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigFieldBase*)
   {
      return GenerateInitInstanceLocal((::ConfigFieldBase*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConfigFieldBase*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *ConfigFieldlEvectorlEstringgRsPgR_Dictionary();
   static void ConfigFieldlEvectorlEstringgRsPgR_TClassManip(TClass*);
   static void *new_ConfigFieldlEvectorlEstringgRsPgR(void *p = 0);
   static void *newArray_ConfigFieldlEvectorlEstringgRsPgR(Long_t size, void *p);
   static void delete_ConfigFieldlEvectorlEstringgRsPgR(void *p);
   static void deleteArray_ConfigFieldlEvectorlEstringgRsPgR(void *p);
   static void destruct_ConfigFieldlEvectorlEstringgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigField<vector<string> >*)
   {
      ::ConfigField<vector<string> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigField<vector<string> > >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigField<vector<string> >", ::ConfigField<vector<string> >::Class_Version(), "CxAODTools/ConfigField.h", 12,
                  typeid(::ConfigField<vector<string> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ConfigFieldlEvectorlEstringgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(::ConfigField<vector<string> >) );
      instance.SetNew(&new_ConfigFieldlEvectorlEstringgRsPgR);
      instance.SetNewArray(&newArray_ConfigFieldlEvectorlEstringgRsPgR);
      instance.SetDelete(&delete_ConfigFieldlEvectorlEstringgRsPgR);
      instance.SetDeleteArray(&deleteArray_ConfigFieldlEvectorlEstringgRsPgR);
      instance.SetDestructor(&destruct_ConfigFieldlEvectorlEstringgRsPgR);

      ::ROOT::AddClassAlternate("ConfigField<vector<string> >","ConfigField<std::vector<std::string> >");

      ::ROOT::AddClassAlternate("ConfigField<vector<string> >","ConfigField<std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigField<vector<string> >*)
   {
      return GenerateInitInstanceLocal((::ConfigField<vector<string> >*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConfigField<vector<string> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ConfigFieldlEvectorlEstringgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<string> >*)0x0)->GetClass();
      ConfigFieldlEvectorlEstringgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void ConfigFieldlEvectorlEstringgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ConfigFieldlEvectorlEdoublegRsPgR_Dictionary();
   static void ConfigFieldlEvectorlEdoublegRsPgR_TClassManip(TClass*);
   static void *new_ConfigFieldlEvectorlEdoublegRsPgR(void *p = 0);
   static void *newArray_ConfigFieldlEvectorlEdoublegRsPgR(Long_t size, void *p);
   static void delete_ConfigFieldlEvectorlEdoublegRsPgR(void *p);
   static void deleteArray_ConfigFieldlEvectorlEdoublegRsPgR(void *p);
   static void destruct_ConfigFieldlEvectorlEdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigField<vector<double> >*)
   {
      ::ConfigField<vector<double> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigField<vector<double> > >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigField<vector<double> >", ::ConfigField<vector<double> >::Class_Version(), "CxAODTools/ConfigField.h", 12,
                  typeid(::ConfigField<vector<double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ConfigFieldlEvectorlEdoublegRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(::ConfigField<vector<double> >) );
      instance.SetNew(&new_ConfigFieldlEvectorlEdoublegRsPgR);
      instance.SetNewArray(&newArray_ConfigFieldlEvectorlEdoublegRsPgR);
      instance.SetDelete(&delete_ConfigFieldlEvectorlEdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_ConfigFieldlEvectorlEdoublegRsPgR);
      instance.SetDestructor(&destruct_ConfigFieldlEvectorlEdoublegRsPgR);

      ::ROOT::AddClassAlternate("ConfigField<vector<double> >","ConfigField<std::vector<double> >");

      ::ROOT::AddClassAlternate("ConfigField<vector<double> >","ConfigField<std::vector<double, std::allocator<double> > >");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigField<vector<double> >*)
   {
      return GenerateInitInstanceLocal((::ConfigField<vector<double> >*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConfigField<vector<double> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ConfigFieldlEvectorlEdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<double> >*)0x0)->GetClass();
      ConfigFieldlEvectorlEdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void ConfigFieldlEvectorlEdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ConfigFieldlEvectorlEfloatgRsPgR_Dictionary();
   static void ConfigFieldlEvectorlEfloatgRsPgR_TClassManip(TClass*);
   static void *new_ConfigFieldlEvectorlEfloatgRsPgR(void *p = 0);
   static void *newArray_ConfigFieldlEvectorlEfloatgRsPgR(Long_t size, void *p);
   static void delete_ConfigFieldlEvectorlEfloatgRsPgR(void *p);
   static void deleteArray_ConfigFieldlEvectorlEfloatgRsPgR(void *p);
   static void destruct_ConfigFieldlEvectorlEfloatgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigField<vector<float> >*)
   {
      ::ConfigField<vector<float> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigField<vector<float> > >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigField<vector<float> >", ::ConfigField<vector<float> >::Class_Version(), "CxAODTools/ConfigField.h", 12,
                  typeid(::ConfigField<vector<float> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ConfigFieldlEvectorlEfloatgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(::ConfigField<vector<float> >) );
      instance.SetNew(&new_ConfigFieldlEvectorlEfloatgRsPgR);
      instance.SetNewArray(&newArray_ConfigFieldlEvectorlEfloatgRsPgR);
      instance.SetDelete(&delete_ConfigFieldlEvectorlEfloatgRsPgR);
      instance.SetDeleteArray(&deleteArray_ConfigFieldlEvectorlEfloatgRsPgR);
      instance.SetDestructor(&destruct_ConfigFieldlEvectorlEfloatgRsPgR);

      ::ROOT::AddClassAlternate("ConfigField<vector<float> >","ConfigField<std::vector<float> >");

      ::ROOT::AddClassAlternate("ConfigField<vector<float> >","ConfigField<std::vector<float, std::allocator<float> > >");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigField<vector<float> >*)
   {
      return GenerateInitInstanceLocal((::ConfigField<vector<float> >*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConfigField<vector<float> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ConfigFieldlEvectorlEfloatgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<float> >*)0x0)->GetClass();
      ConfigFieldlEvectorlEfloatgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void ConfigFieldlEvectorlEfloatgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ConfigFieldlEvectorlEintgRsPgR_Dictionary();
   static void ConfigFieldlEvectorlEintgRsPgR_TClassManip(TClass*);
   static void *new_ConfigFieldlEvectorlEintgRsPgR(void *p = 0);
   static void *newArray_ConfigFieldlEvectorlEintgRsPgR(Long_t size, void *p);
   static void delete_ConfigFieldlEvectorlEintgRsPgR(void *p);
   static void deleteArray_ConfigFieldlEvectorlEintgRsPgR(void *p);
   static void destruct_ConfigFieldlEvectorlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigField<vector<int> >*)
   {
      ::ConfigField<vector<int> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigField<vector<int> > >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigField<vector<int> >", ::ConfigField<vector<int> >::Class_Version(), "CxAODTools/ConfigField.h", 12,
                  typeid(::ConfigField<vector<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ConfigFieldlEvectorlEintgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(::ConfigField<vector<int> >) );
      instance.SetNew(&new_ConfigFieldlEvectorlEintgRsPgR);
      instance.SetNewArray(&newArray_ConfigFieldlEvectorlEintgRsPgR);
      instance.SetDelete(&delete_ConfigFieldlEvectorlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_ConfigFieldlEvectorlEintgRsPgR);
      instance.SetDestructor(&destruct_ConfigFieldlEvectorlEintgRsPgR);

      ::ROOT::AddClassAlternate("ConfigField<vector<int> >","ConfigField<std::vector<int> >");

      ::ROOT::AddClassAlternate("ConfigField<vector<int> >","ConfigField<std::vector<int, std::allocator<int> > >");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigField<vector<int> >*)
   {
      return GenerateInitInstanceLocal((::ConfigField<vector<int> >*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConfigField<vector<int> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ConfigFieldlEvectorlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<int> >*)0x0)->GetClass();
      ConfigFieldlEvectorlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void ConfigFieldlEvectorlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ConfigFieldlEvectorlEboolgRsPgR_Dictionary();
   static void ConfigFieldlEvectorlEboolgRsPgR_TClassManip(TClass*);
   static void *new_ConfigFieldlEvectorlEboolgRsPgR(void *p = 0);
   static void *newArray_ConfigFieldlEvectorlEboolgRsPgR(Long_t size, void *p);
   static void delete_ConfigFieldlEvectorlEboolgRsPgR(void *p);
   static void deleteArray_ConfigFieldlEvectorlEboolgRsPgR(void *p);
   static void destruct_ConfigFieldlEvectorlEboolgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigField<vector<bool> >*)
   {
      ::ConfigField<vector<bool> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigField<vector<bool> > >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigField<vector<bool> >", ::ConfigField<vector<bool> >::Class_Version(), "CxAODTools/ConfigField.h", 12,
                  typeid(::ConfigField<vector<bool> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ConfigFieldlEvectorlEboolgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(::ConfigField<vector<bool> >) );
      instance.SetNew(&new_ConfigFieldlEvectorlEboolgRsPgR);
      instance.SetNewArray(&newArray_ConfigFieldlEvectorlEboolgRsPgR);
      instance.SetDelete(&delete_ConfigFieldlEvectorlEboolgRsPgR);
      instance.SetDeleteArray(&deleteArray_ConfigFieldlEvectorlEboolgRsPgR);
      instance.SetDestructor(&destruct_ConfigFieldlEvectorlEboolgRsPgR);

      ::ROOT::AddClassAlternate("ConfigField<vector<bool> >","ConfigField<std::vector<bool> >");

      ::ROOT::AddClassAlternate("ConfigField<vector<bool> >","ConfigField<std::vector<bool, std::allocator<bool> > >");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigField<vector<bool> >*)
   {
      return GenerateInitInstanceLocal((::ConfigField<vector<bool> >*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConfigField<vector<bool> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ConfigFieldlEvectorlEboolgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<bool> >*)0x0)->GetClass();
      ConfigFieldlEvectorlEboolgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void ConfigFieldlEvectorlEboolgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ConfigFieldlEstringgR_Dictionary();
   static void ConfigFieldlEstringgR_TClassManip(TClass*);
   static void *new_ConfigFieldlEstringgR(void *p = 0);
   static void *newArray_ConfigFieldlEstringgR(Long_t size, void *p);
   static void delete_ConfigFieldlEstringgR(void *p);
   static void deleteArray_ConfigFieldlEstringgR(void *p);
   static void destruct_ConfigFieldlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigField<string>*)
   {
      ::ConfigField<string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigField<string> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigField<string>", ::ConfigField<string>::Class_Version(), "CxAODTools/ConfigField.h", 12,
                  typeid(::ConfigField<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ConfigFieldlEstringgR_Dictionary, isa_proxy, 4,
                  sizeof(::ConfigField<string>) );
      instance.SetNew(&new_ConfigFieldlEstringgR);
      instance.SetNewArray(&newArray_ConfigFieldlEstringgR);
      instance.SetDelete(&delete_ConfigFieldlEstringgR);
      instance.SetDeleteArray(&deleteArray_ConfigFieldlEstringgR);
      instance.SetDestructor(&destruct_ConfigFieldlEstringgR);

      ::ROOT::AddClassAlternate("ConfigField<string>","ConfigField<std::string>");

      ::ROOT::AddClassAlternate("ConfigField<string>","ConfigField<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigField<string>*)
   {
      return GenerateInitInstanceLocal((::ConfigField<string>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConfigField<string>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ConfigFieldlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ConfigField<string>*)0x0)->GetClass();
      ConfigFieldlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void ConfigFieldlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ConfigFieldlEdoublegR_Dictionary();
   static void ConfigFieldlEdoublegR_TClassManip(TClass*);
   static void *new_ConfigFieldlEdoublegR(void *p = 0);
   static void *newArray_ConfigFieldlEdoublegR(Long_t size, void *p);
   static void delete_ConfigFieldlEdoublegR(void *p);
   static void deleteArray_ConfigFieldlEdoublegR(void *p);
   static void destruct_ConfigFieldlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigField<double>*)
   {
      ::ConfigField<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigField<double> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigField<double>", ::ConfigField<double>::Class_Version(), "CxAODTools/ConfigField.h", 12,
                  typeid(::ConfigField<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ConfigFieldlEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::ConfigField<double>) );
      instance.SetNew(&new_ConfigFieldlEdoublegR);
      instance.SetNewArray(&newArray_ConfigFieldlEdoublegR);
      instance.SetDelete(&delete_ConfigFieldlEdoublegR);
      instance.SetDeleteArray(&deleteArray_ConfigFieldlEdoublegR);
      instance.SetDestructor(&destruct_ConfigFieldlEdoublegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigField<double>*)
   {
      return GenerateInitInstanceLocal((::ConfigField<double>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConfigField<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ConfigFieldlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ConfigField<double>*)0x0)->GetClass();
      ConfigFieldlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void ConfigFieldlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ConfigFieldlEfloatgR_Dictionary();
   static void ConfigFieldlEfloatgR_TClassManip(TClass*);
   static void *new_ConfigFieldlEfloatgR(void *p = 0);
   static void *newArray_ConfigFieldlEfloatgR(Long_t size, void *p);
   static void delete_ConfigFieldlEfloatgR(void *p);
   static void deleteArray_ConfigFieldlEfloatgR(void *p);
   static void destruct_ConfigFieldlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigField<float>*)
   {
      ::ConfigField<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigField<float> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigField<float>", ::ConfigField<float>::Class_Version(), "CxAODTools/ConfigField.h", 12,
                  typeid(::ConfigField<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ConfigFieldlEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::ConfigField<float>) );
      instance.SetNew(&new_ConfigFieldlEfloatgR);
      instance.SetNewArray(&newArray_ConfigFieldlEfloatgR);
      instance.SetDelete(&delete_ConfigFieldlEfloatgR);
      instance.SetDeleteArray(&deleteArray_ConfigFieldlEfloatgR);
      instance.SetDestructor(&destruct_ConfigFieldlEfloatgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigField<float>*)
   {
      return GenerateInitInstanceLocal((::ConfigField<float>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConfigField<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ConfigFieldlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ConfigField<float>*)0x0)->GetClass();
      ConfigFieldlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void ConfigFieldlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ConfigFieldlEintgR_Dictionary();
   static void ConfigFieldlEintgR_TClassManip(TClass*);
   static void *new_ConfigFieldlEintgR(void *p = 0);
   static void *newArray_ConfigFieldlEintgR(Long_t size, void *p);
   static void delete_ConfigFieldlEintgR(void *p);
   static void deleteArray_ConfigFieldlEintgR(void *p);
   static void destruct_ConfigFieldlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigField<int>*)
   {
      ::ConfigField<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigField<int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigField<int>", ::ConfigField<int>::Class_Version(), "CxAODTools/ConfigField.h", 12,
                  typeid(::ConfigField<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ConfigFieldlEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::ConfigField<int>) );
      instance.SetNew(&new_ConfigFieldlEintgR);
      instance.SetNewArray(&newArray_ConfigFieldlEintgR);
      instance.SetDelete(&delete_ConfigFieldlEintgR);
      instance.SetDeleteArray(&deleteArray_ConfigFieldlEintgR);
      instance.SetDestructor(&destruct_ConfigFieldlEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigField<int>*)
   {
      return GenerateInitInstanceLocal((::ConfigField<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConfigField<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ConfigFieldlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ConfigField<int>*)0x0)->GetClass();
      ConfigFieldlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void ConfigFieldlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ConfigFieldlEboolgR_Dictionary();
   static void ConfigFieldlEboolgR_TClassManip(TClass*);
   static void *new_ConfigFieldlEboolgR(void *p = 0);
   static void *newArray_ConfigFieldlEboolgR(Long_t size, void *p);
   static void delete_ConfigFieldlEboolgR(void *p);
   static void deleteArray_ConfigFieldlEboolgR(void *p);
   static void destruct_ConfigFieldlEboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigField<bool>*)
   {
      ::ConfigField<bool> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigField<bool> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigField<bool>", ::ConfigField<bool>::Class_Version(), "CxAODTools/ConfigField.h", 12,
                  typeid(::ConfigField<bool>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ConfigFieldlEboolgR_Dictionary, isa_proxy, 4,
                  sizeof(::ConfigField<bool>) );
      instance.SetNew(&new_ConfigFieldlEboolgR);
      instance.SetNewArray(&newArray_ConfigFieldlEboolgR);
      instance.SetDelete(&delete_ConfigFieldlEboolgR);
      instance.SetDeleteArray(&deleteArray_ConfigFieldlEboolgR);
      instance.SetDestructor(&destruct_ConfigFieldlEboolgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigField<bool>*)
   {
      return GenerateInitInstanceLocal((::ConfigField<bool>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConfigField<bool>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ConfigFieldlEboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ConfigField<bool>*)0x0)->GetClass();
      ConfigFieldlEboolgR_TClassManip(theClass);
   return theClass;
   }

   static void ConfigFieldlEboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_ConfigStore(void *p = 0);
   static void *newArray_ConfigStore(Long_t size, void *p);
   static void delete_ConfigStore(void *p);
   static void deleteArray_ConfigStore(void *p);
   static void destruct_ConfigStore(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigStore*)
   {
      ::ConfigStore *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigStore >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigStore", ::ConfigStore::Class_Version(), "CxAODTools/ConfigStore.h", 16,
                  typeid(::ConfigStore), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ConfigStore::Dictionary, isa_proxy, 4,
                  sizeof(::ConfigStore) );
      instance.SetNew(&new_ConfigStore);
      instance.SetNewArray(&newArray_ConfigStore);
      instance.SetDelete(&delete_ConfigStore);
      instance.SetDeleteArray(&deleteArray_ConfigStore);
      instance.SetDestructor(&destruct_ConfigStore);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigStore*)
   {
      return GenerateInitInstanceLocal((::ConfigStore*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConfigStore*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ConfigFieldBase::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ConfigFieldBase::Class_Name()
{
   return "ConfigFieldBase";
}

//______________________________________________________________________________
const char *ConfigFieldBase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigFieldBase*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ConfigFieldBase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigFieldBase*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ConfigFieldBase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigFieldBase*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ConfigFieldBase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigFieldBase*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr ConfigField<vector<string> >::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *ConfigField<vector<string> >::Class_Name()
{
   return "ConfigField<vector<string> >";
}

//______________________________________________________________________________
template <> const char *ConfigField<vector<string> >::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<string> >*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int ConfigField<vector<string> >::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<string> >*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *ConfigField<vector<string> >::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<string> >*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *ConfigField<vector<string> >::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<string> >*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr ConfigField<vector<double> >::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *ConfigField<vector<double> >::Class_Name()
{
   return "ConfigField<vector<double> >";
}

//______________________________________________________________________________
template <> const char *ConfigField<vector<double> >::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<double> >*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int ConfigField<vector<double> >::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<double> >*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *ConfigField<vector<double> >::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<double> >*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *ConfigField<vector<double> >::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<double> >*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr ConfigField<vector<float> >::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *ConfigField<vector<float> >::Class_Name()
{
   return "ConfigField<vector<float> >";
}

//______________________________________________________________________________
template <> const char *ConfigField<vector<float> >::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<float> >*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int ConfigField<vector<float> >::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<float> >*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *ConfigField<vector<float> >::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<float> >*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *ConfigField<vector<float> >::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<float> >*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr ConfigField<vector<int> >::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *ConfigField<vector<int> >::Class_Name()
{
   return "ConfigField<vector<int> >";
}

//______________________________________________________________________________
template <> const char *ConfigField<vector<int> >::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<int> >*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int ConfigField<vector<int> >::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<int> >*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *ConfigField<vector<int> >::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<int> >*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *ConfigField<vector<int> >::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<int> >*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr ConfigField<vector<bool> >::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *ConfigField<vector<bool> >::Class_Name()
{
   return "ConfigField<vector<bool> >";
}

//______________________________________________________________________________
template <> const char *ConfigField<vector<bool> >::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<bool> >*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int ConfigField<vector<bool> >::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<bool> >*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *ConfigField<vector<bool> >::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<bool> >*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *ConfigField<vector<bool> >::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<vector<bool> >*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr ConfigField<string>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *ConfigField<string>::Class_Name()
{
   return "ConfigField<string>";
}

//______________________________________________________________________________
template <> const char *ConfigField<string>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<string>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int ConfigField<string>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<string>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *ConfigField<string>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<string>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *ConfigField<string>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<string>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr ConfigField<double>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *ConfigField<double>::Class_Name()
{
   return "ConfigField<double>";
}

//______________________________________________________________________________
template <> const char *ConfigField<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<double>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int ConfigField<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<double>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *ConfigField<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<double>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *ConfigField<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<double>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr ConfigField<float>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *ConfigField<float>::Class_Name()
{
   return "ConfigField<float>";
}

//______________________________________________________________________________
template <> const char *ConfigField<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<float>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int ConfigField<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<float>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *ConfigField<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<float>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *ConfigField<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<float>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr ConfigField<int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *ConfigField<int>::Class_Name()
{
   return "ConfigField<int>";
}

//______________________________________________________________________________
template <> const char *ConfigField<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int ConfigField<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *ConfigField<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *ConfigField<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr ConfigField<bool>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *ConfigField<bool>::Class_Name()
{
   return "ConfigField<bool>";
}

//______________________________________________________________________________
template <> const char *ConfigField<bool>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<bool>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int ConfigField<bool>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<bool>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *ConfigField<bool>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<bool>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *ConfigField<bool>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigField<bool>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ConfigStore::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ConfigStore::Class_Name()
{
   return "ConfigStore";
}

//______________________________________________________________________________
const char *ConfigStore::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigStore*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ConfigStore::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigStore*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ConfigStore::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigStore*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ConfigStore::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigStore*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ConfigFieldBase::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigFieldBase.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ConfigFieldBase::Class(),this);
   } else {
      R__b.WriteClassBuffer(ConfigFieldBase::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ConfigFieldBase(void *p) {
      delete ((::ConfigFieldBase*)p);
   }
   static void deleteArray_ConfigFieldBase(void *p) {
      delete [] ((::ConfigFieldBase*)p);
   }
   static void destruct_ConfigFieldBase(void *p) {
      typedef ::ConfigFieldBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ConfigFieldBase

//______________________________________________________________________________
template <> void ConfigField<vector<string> >::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigField<vector<string> >.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ConfigField<vector<string> >::Class(),this);
   } else {
      R__b.WriteClassBuffer(ConfigField<vector<string> >::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigFieldlEvectorlEstringgRsPgR(void *p) {
      return  p ? new(p) ::ConfigField<vector<string> > : new ::ConfigField<vector<string> >;
   }
   static void *newArray_ConfigFieldlEvectorlEstringgRsPgR(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigField<vector<string> >[nElements] : new ::ConfigField<vector<string> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigFieldlEvectorlEstringgRsPgR(void *p) {
      delete ((::ConfigField<vector<string> >*)p);
   }
   static void deleteArray_ConfigFieldlEvectorlEstringgRsPgR(void *p) {
      delete [] ((::ConfigField<vector<string> >*)p);
   }
   static void destruct_ConfigFieldlEvectorlEstringgRsPgR(void *p) {
      typedef ::ConfigField<vector<string> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ConfigField<vector<string> >

//______________________________________________________________________________
template <> void ConfigField<vector<double> >::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigField<vector<double> >.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ConfigField<vector<double> >::Class(),this);
   } else {
      R__b.WriteClassBuffer(ConfigField<vector<double> >::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigFieldlEvectorlEdoublegRsPgR(void *p) {
      return  p ? new(p) ::ConfigField<vector<double> > : new ::ConfigField<vector<double> >;
   }
   static void *newArray_ConfigFieldlEvectorlEdoublegRsPgR(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigField<vector<double> >[nElements] : new ::ConfigField<vector<double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigFieldlEvectorlEdoublegRsPgR(void *p) {
      delete ((::ConfigField<vector<double> >*)p);
   }
   static void deleteArray_ConfigFieldlEvectorlEdoublegRsPgR(void *p) {
      delete [] ((::ConfigField<vector<double> >*)p);
   }
   static void destruct_ConfigFieldlEvectorlEdoublegRsPgR(void *p) {
      typedef ::ConfigField<vector<double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ConfigField<vector<double> >

//______________________________________________________________________________
template <> void ConfigField<vector<float> >::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigField<vector<float> >.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ConfigField<vector<float> >::Class(),this);
   } else {
      R__b.WriteClassBuffer(ConfigField<vector<float> >::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigFieldlEvectorlEfloatgRsPgR(void *p) {
      return  p ? new(p) ::ConfigField<vector<float> > : new ::ConfigField<vector<float> >;
   }
   static void *newArray_ConfigFieldlEvectorlEfloatgRsPgR(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigField<vector<float> >[nElements] : new ::ConfigField<vector<float> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigFieldlEvectorlEfloatgRsPgR(void *p) {
      delete ((::ConfigField<vector<float> >*)p);
   }
   static void deleteArray_ConfigFieldlEvectorlEfloatgRsPgR(void *p) {
      delete [] ((::ConfigField<vector<float> >*)p);
   }
   static void destruct_ConfigFieldlEvectorlEfloatgRsPgR(void *p) {
      typedef ::ConfigField<vector<float> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ConfigField<vector<float> >

//______________________________________________________________________________
template <> void ConfigField<vector<int> >::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigField<vector<int> >.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ConfigField<vector<int> >::Class(),this);
   } else {
      R__b.WriteClassBuffer(ConfigField<vector<int> >::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigFieldlEvectorlEintgRsPgR(void *p) {
      return  p ? new(p) ::ConfigField<vector<int> > : new ::ConfigField<vector<int> >;
   }
   static void *newArray_ConfigFieldlEvectorlEintgRsPgR(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigField<vector<int> >[nElements] : new ::ConfigField<vector<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigFieldlEvectorlEintgRsPgR(void *p) {
      delete ((::ConfigField<vector<int> >*)p);
   }
   static void deleteArray_ConfigFieldlEvectorlEintgRsPgR(void *p) {
      delete [] ((::ConfigField<vector<int> >*)p);
   }
   static void destruct_ConfigFieldlEvectorlEintgRsPgR(void *p) {
      typedef ::ConfigField<vector<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ConfigField<vector<int> >

//______________________________________________________________________________
template <> void ConfigField<vector<bool> >::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigField<vector<bool> >.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ConfigField<vector<bool> >::Class(),this);
   } else {
      R__b.WriteClassBuffer(ConfigField<vector<bool> >::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigFieldlEvectorlEboolgRsPgR(void *p) {
      return  p ? new(p) ::ConfigField<vector<bool> > : new ::ConfigField<vector<bool> >;
   }
   static void *newArray_ConfigFieldlEvectorlEboolgRsPgR(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigField<vector<bool> >[nElements] : new ::ConfigField<vector<bool> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigFieldlEvectorlEboolgRsPgR(void *p) {
      delete ((::ConfigField<vector<bool> >*)p);
   }
   static void deleteArray_ConfigFieldlEvectorlEboolgRsPgR(void *p) {
      delete [] ((::ConfigField<vector<bool> >*)p);
   }
   static void destruct_ConfigFieldlEvectorlEboolgRsPgR(void *p) {
      typedef ::ConfigField<vector<bool> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ConfigField<vector<bool> >

//______________________________________________________________________________
template <> void ConfigField<string>::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigField<string>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ConfigField<string>::Class(),this);
   } else {
      R__b.WriteClassBuffer(ConfigField<string>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigFieldlEstringgR(void *p) {
      return  p ? new(p) ::ConfigField<string> : new ::ConfigField<string>;
   }
   static void *newArray_ConfigFieldlEstringgR(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigField<string>[nElements] : new ::ConfigField<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigFieldlEstringgR(void *p) {
      delete ((::ConfigField<string>*)p);
   }
   static void deleteArray_ConfigFieldlEstringgR(void *p) {
      delete [] ((::ConfigField<string>*)p);
   }
   static void destruct_ConfigFieldlEstringgR(void *p) {
      typedef ::ConfigField<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ConfigField<string>

//______________________________________________________________________________
template <> void ConfigField<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigField<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ConfigField<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(ConfigField<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigFieldlEdoublegR(void *p) {
      return  p ? new(p) ::ConfigField<double> : new ::ConfigField<double>;
   }
   static void *newArray_ConfigFieldlEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigField<double>[nElements] : new ::ConfigField<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigFieldlEdoublegR(void *p) {
      delete ((::ConfigField<double>*)p);
   }
   static void deleteArray_ConfigFieldlEdoublegR(void *p) {
      delete [] ((::ConfigField<double>*)p);
   }
   static void destruct_ConfigFieldlEdoublegR(void *p) {
      typedef ::ConfigField<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ConfigField<double>

//______________________________________________________________________________
template <> void ConfigField<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigField<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ConfigField<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(ConfigField<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigFieldlEfloatgR(void *p) {
      return  p ? new(p) ::ConfigField<float> : new ::ConfigField<float>;
   }
   static void *newArray_ConfigFieldlEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigField<float>[nElements] : new ::ConfigField<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigFieldlEfloatgR(void *p) {
      delete ((::ConfigField<float>*)p);
   }
   static void deleteArray_ConfigFieldlEfloatgR(void *p) {
      delete [] ((::ConfigField<float>*)p);
   }
   static void destruct_ConfigFieldlEfloatgR(void *p) {
      typedef ::ConfigField<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ConfigField<float>

//______________________________________________________________________________
template <> void ConfigField<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigField<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ConfigField<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(ConfigField<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigFieldlEintgR(void *p) {
      return  p ? new(p) ::ConfigField<int> : new ::ConfigField<int>;
   }
   static void *newArray_ConfigFieldlEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigField<int>[nElements] : new ::ConfigField<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigFieldlEintgR(void *p) {
      delete ((::ConfigField<int>*)p);
   }
   static void deleteArray_ConfigFieldlEintgR(void *p) {
      delete [] ((::ConfigField<int>*)p);
   }
   static void destruct_ConfigFieldlEintgR(void *p) {
      typedef ::ConfigField<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ConfigField<int>

//______________________________________________________________________________
template <> void ConfigField<bool>::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigField<bool>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ConfigField<bool>::Class(),this);
   } else {
      R__b.WriteClassBuffer(ConfigField<bool>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigFieldlEboolgR(void *p) {
      return  p ? new(p) ::ConfigField<bool> : new ::ConfigField<bool>;
   }
   static void *newArray_ConfigFieldlEboolgR(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigField<bool>[nElements] : new ::ConfigField<bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigFieldlEboolgR(void *p) {
      delete ((::ConfigField<bool>*)p);
   }
   static void deleteArray_ConfigFieldlEboolgR(void *p) {
      delete [] ((::ConfigField<bool>*)p);
   }
   static void destruct_ConfigFieldlEboolgR(void *p) {
      typedef ::ConfigField<bool> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ConfigField<bool>

//______________________________________________________________________________
void ConfigStore::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigStore.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ConfigStore::Class(),this);
   } else {
      R__b.WriteClassBuffer(ConfigStore::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigStore(void *p) {
      return  p ? new(p) ::ConfigStore : new ::ConfigStore;
   }
   static void *newArray_ConfigStore(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigStore[nElements] : new ::ConfigStore[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigStore(void *p) {
      delete ((::ConfigStore*)p);
   }
   static void deleteArray_ConfigStore(void *p) {
      delete [] ((::ConfigStore*)p);
   }
   static void destruct_ConfigStore(void *p) {
      typedef ::ConfigStore current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ConfigStore

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = 0);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 339,
                  typeid(vector<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));

      ::ROOT::AddClassAlternate("vector<string>","std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<string>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<string>*)0x0)->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete ((vector<string>*)p);
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] ((vector<string>*)p);
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 339,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));

      ::ROOT::AddClassAlternate("vector<int>","std::vector<int, std::allocator<int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = 0);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 339,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));

      ::ROOT::AddClassAlternate("vector<float>","std::vector<float, std::allocator<float> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)0x0)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 339,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));

      ::ROOT::AddClassAlternate("vector<double>","std::vector<double, std::allocator<double> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlEboolgR_Dictionary();
   static void vectorlEboolgR_TClassManip(TClass*);
   static void *new_vectorlEboolgR(void *p = 0);
   static void *newArray_vectorlEboolgR(Long_t size, void *p);
   static void delete_vectorlEboolgR(void *p);
   static void deleteArray_vectorlEboolgR(void *p);
   static void destruct_vectorlEboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<bool>*)
   {
      vector<bool> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<bool>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<bool>", -2, "vector", 588,
                  typeid(vector<bool>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEboolgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<bool>) );
      instance.SetNew(&new_vectorlEboolgR);
      instance.SetNewArray(&newArray_vectorlEboolgR);
      instance.SetDelete(&delete_vectorlEboolgR);
      instance.SetDeleteArray(&deleteArray_vectorlEboolgR);
      instance.SetDestructor(&destruct_vectorlEboolgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<bool> >()));

      ::ROOT::AddClassAlternate("vector<bool>","std::vector<bool, std::allocator<bool> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<bool>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<bool>*)0x0)->GetClass();
      vectorlEboolgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEboolgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bool> : new vector<bool>;
   }
   static void *newArray_vectorlEboolgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bool>[nElements] : new vector<bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEboolgR(void *p) {
      delete ((vector<bool>*)p);
   }
   static void deleteArray_vectorlEboolgR(void *p) {
      delete [] ((vector<bool>*)p);
   }
   static void destruct_vectorlEboolgR(void *p) {
      typedef vector<bool> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<bool>

namespace ROOT {
   static TClass *maplEstringcOConfigFieldBasemUgR_Dictionary();
   static void maplEstringcOConfigFieldBasemUgR_TClassManip(TClass*);
   static void *new_maplEstringcOConfigFieldBasemUgR(void *p = 0);
   static void *newArray_maplEstringcOConfigFieldBasemUgR(Long_t size, void *p);
   static void delete_maplEstringcOConfigFieldBasemUgR(void *p);
   static void deleteArray_maplEstringcOConfigFieldBasemUgR(void *p);
   static void destruct_maplEstringcOConfigFieldBasemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,ConfigFieldBase*>*)
   {
      map<string,ConfigFieldBase*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,ConfigFieldBase*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,ConfigFieldBase*>", -2, "map", 100,
                  typeid(map<string,ConfigFieldBase*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOConfigFieldBasemUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,ConfigFieldBase*>) );
      instance.SetNew(&new_maplEstringcOConfigFieldBasemUgR);
      instance.SetNewArray(&newArray_maplEstringcOConfigFieldBasemUgR);
      instance.SetDelete(&delete_maplEstringcOConfigFieldBasemUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOConfigFieldBasemUgR);
      instance.SetDestructor(&destruct_maplEstringcOConfigFieldBasemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,ConfigFieldBase*> >()));

      ::ROOT::AddClassAlternate("map<string,ConfigFieldBase*>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, ConfigFieldBase*, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, ConfigFieldBase*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,ConfigFieldBase*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOConfigFieldBasemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,ConfigFieldBase*>*)0x0)->GetClass();
      maplEstringcOConfigFieldBasemUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOConfigFieldBasemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOConfigFieldBasemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,ConfigFieldBase*> : new map<string,ConfigFieldBase*>;
   }
   static void *newArray_maplEstringcOConfigFieldBasemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,ConfigFieldBase*>[nElements] : new map<string,ConfigFieldBase*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOConfigFieldBasemUgR(void *p) {
      delete ((map<string,ConfigFieldBase*>*)p);
   }
   static void deleteArray_maplEstringcOConfigFieldBasemUgR(void *p) {
      delete [] ((map<string,ConfigFieldBase*>*)p);
   }
   static void destruct_maplEstringcOConfigFieldBasemUgR(void *p) {
      typedef map<string,ConfigFieldBase*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,ConfigFieldBase*>

namespace {
  void TriggerDictionaryInitialization_libCxAODTools_Impl() {
    static const char* headers[] = {
"CxAODTools/ConfigFieldBase.h",
"CxAODTools/ConfigField.h",
"CxAODTools/ConfigStore.h",
"CxAODTools/OverlapRegister.h",
0
    };
    static const char* includePaths[] = {
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CxAODTools",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CxAODTools",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CxAODTools",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CxAODTools",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/D3PDTools/EventLoop",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/D3PDTools/RootCoreUtils",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/D3PDTools/SampleHandler",
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
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/D3PDTools/AnaAlgorithm",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthToolSupport/AsgServices",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Tools/PathResolver",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODBase",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEventInfo",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/AnalysisCommon/PATInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODJet",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODBTagging",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTracking",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/DetectorDescription/GeoPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/EventPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODMuon",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCaloEvent",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Calorimeter/CaloGeoHelpers",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/MuonSpectrometer/MuonIdHelpers",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTrigger",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODPFlow",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEgamma",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTruth",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/xAODBTaggingEfficiency",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/AnalysisCommon/PATCore",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/Interfaces/FTagAnalysisInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/CalibrationDataInterface",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/onnxruntime",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/AnalysisCommon/AssociationUtils",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTau",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODMissingET",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODLuminosity",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonEfficiencyCorrections",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/Interfaces/AsgAnalysisInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/Interfaces/MuonAnalysisInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/AnalysisCommon/PileupReweighting",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Trigger/TrigEvent/TrigDecisionInterface",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Reconstruction/Jet/JetCalibTools",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEventShape",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Reconstruction/Jet/JetInterface",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CxAODTools/CMakeFiles/makeCxAODToolsCintDict.az8RRF/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCxAODTools dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$CxAODTools/ConfigFieldBase.h")))  ConfigFieldBase;
namespace std{template <class _CharT> struct __attribute__((annotate("$clingAutoload$bits/char_traits.h")))  __attribute__((annotate("$clingAutoload$string")))  char_traits;
}
namespace std{template <typename > class __attribute__((annotate("$clingAutoload$bits/memoryfwd.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$CxAODTools/ConfigStore.h")))  ConfigStore;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCxAODTools dictionary payload"

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
#ifndef USE_CMAKE
  #define USE_CMAKE 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CxAODTools/ConfigFieldBase.h"
#include "CxAODTools/ConfigField.h"
#include "CxAODTools/ConfigStore.h"
#include "CxAODTools/OverlapRegister.h"
#include <CxAODTools/OverlapRegister.h>
#include "CxAODTools/ConfigFieldBase.h"
#include "CxAODTools/ConfigField.h"
#include "CxAODTools/ConfigStore.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#endif

#ifdef __CINT__

#pragma link C++ class ConfigFieldBase+;
#pragma link C++ class ConfigField<bool>+;
#pragma link C++ class ConfigField<int>+;
#pragma link C++ class ConfigField<float>+;
#pragma link C++ class ConfigField<double>+;
#pragma link C++ class ConfigField<std::string>+;
#pragma link C++ class ConfigField<std::vector<bool> >+;
#pragma link C++ class ConfigField<std::vector<int> >+;
#pragma link C++ class ConfigField<std::vector<float> >+;
#pragma link C++ class ConfigField<std::vector<double> >+;
#pragma link C++ class ConfigField<std::vector<std::string> >+;
#pragma link C++ class ConfigStore+;

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ConfigField<bool>", payloadCode, "@",
"ConfigField<double>", payloadCode, "@",
"ConfigField<float>", payloadCode, "@",
"ConfigField<int>", payloadCode, "@",
"ConfigField<std::string>", payloadCode, "@",
"ConfigField<std::vector<bool> >", payloadCode, "@",
"ConfigField<std::vector<double> >", payloadCode, "@",
"ConfigField<std::vector<float> >", payloadCode, "@",
"ConfigField<std::vector<int> >", payloadCode, "@",
"ConfigField<std::vector<std::string> >", payloadCode, "@",
"ConfigField<string>", payloadCode, "@",
"ConfigField<vector<bool> >", payloadCode, "@",
"ConfigField<vector<double> >", payloadCode, "@",
"ConfigField<vector<float> >", payloadCode, "@",
"ConfigField<vector<int> >", payloadCode, "@",
"ConfigField<vector<string> >", payloadCode, "@",
"ConfigFieldBase", payloadCode, "@",
"ConfigStore", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCxAODTools",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCxAODTools_Impl, {{"namespace DataVector_detail { template <typename B1, typename B2, typename B3> class VirtBases; }", 1},{"template <typename T> class DataVectorBase;", 1},{"template <typename T, typename BASE> class DataVector;", 1}}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCxAODTools_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCxAODTools() {
  TriggerDictionaryInitialization_libCxAODTools_Impl();
}
