// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME CAFxAODUtilsCintDict
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
#include "CAFxAODUtils/Observables.h"
#include "CAFxAODUtils/TQCPConstituentVectorObservable.h"
#include "CAFxAODUtils/TQCPDecorationObservable.h"
#include "CAFxAODUtils/TQCompositorObservable.h"
#include "CAFxAODUtils/TQContainerSizeObservable.h"
#include "CAFxAODUtils/TQEventFlaggingAnalysisJob.h"
#include "CAFxAODUtils/TQEventObservable.h"
#include "CAFxAODUtils/TQIParticleDecorationObservable.h"
#include "CAFxAODUtils/TQIParticleDecorationVectorObservable.h"
#include "CAFxAODUtils/TQIParticleFourVectorObservable.h"
#include "CAFxAODUtils/TQIParticleFourVectorVectorObservable.h"
#include "CAFxAODUtils/TQSGAuxElementDecorationObservable.h"
#include "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h"
#include "CAFxAODUtils/TQToolObservable.h"
#include "CAFxAODUtils/TQTruthParticleInfoObservable.h"
#include "CAFxAODUtils/TQxAODUtils.h"
#include "CAFxAODUtils/TQxAODskimmingAlgorithm.h"
#include "CAFxAODUtils/Observables.h"
#include "CAFxAODUtils/TQCompositorObservable.h"
#include "CAFxAODUtils/TQContainerSizeObservable.h"
#include "CAFxAODUtils/TQCPDecorationObservable.h"
#include "CAFxAODUtils/TQCPConstituentVectorObservable.h"
#include "CAFxAODUtils/TQEventFlaggingAnalysisJob.h"
#include "CAFxAODUtils/TQEventObservable.h"
#include "CAFxAODUtils/TQIParticleDecorationObservable.h"
#include "CAFxAODUtils/TQIParticleDecorationVectorObservable.h"
#include "CAFxAODUtils/TQIParticleFourVectorObservable.h"
#include "CAFxAODUtils/TQIParticleFourVectorVectorObservable.h"
#include "CAFxAODUtils/TQTruthParticleInfoObservable.h"
#include "CAFxAODUtils/TQSGAuxElementDecorationObservable.h"
#include "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h"
#include "CAFxAODUtils/TQToolObservable.h"
#include "CAFxAODUtils/TQxAODskimmingAlgorithm.h"
#include "CAFxAODUtils/TQxAODUtils.h"

// Header files passed via #pragma extra_include

namespace TQObservableFactory {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *TQObservableFactory_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("TQObservableFactory", 0 /*version*/, "QFramework/TQObservable.h", 35,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &TQObservableFactory_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *TQObservableFactory_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace asg {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *asg_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("asg", 0 /*version*/, "CAFxAODUtils/TQEventObservable.h", 15,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &asg_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *asg_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace TQxAODUtils {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *TQxAODUtils_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("TQxAODUtils", 0 /*version*/, "CAFxAODUtils/TQxAODUtils.h", 8,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &TQxAODUtils_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *TQxAODUtils_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void delete_TQEventObservable(void *p);
   static void deleteArray_TQEventObservable(void *p);
   static void destruct_TQEventObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQEventObservable*)
   {
      ::TQEventObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQEventObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQEventObservable", ::TQEventObservable::Class_Version(), "CAFxAODUtils/TQEventObservable.h", 20,
                  typeid(::TQEventObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQEventObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQEventObservable) );
      instance.SetDelete(&delete_TQEventObservable);
      instance.SetDeleteArray(&deleteArray_TQEventObservable);
      instance.SetDestructor(&destruct_TQEventObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQEventObservable*)
   {
      return GenerateInitInstanceLocal((::TQEventObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQEventObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQCompositorObservable(void *p = 0);
   static void *newArray_TQCompositorObservable(Long_t size, void *p);
   static void delete_TQCompositorObservable(void *p);
   static void deleteArray_TQCompositorObservable(void *p);
   static void destruct_TQCompositorObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCompositorObservable*)
   {
      ::TQCompositorObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCompositorObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCompositorObservable", ::TQCompositorObservable::Class_Version(), "CAFxAODUtils/TQCompositorObservable.h", 11,
                  typeid(::TQCompositorObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQCompositorObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQCompositorObservable) );
      instance.SetNew(&new_TQCompositorObservable);
      instance.SetNewArray(&newArray_TQCompositorObservable);
      instance.SetDelete(&delete_TQCompositorObservable);
      instance.SetDeleteArray(&deleteArray_TQCompositorObservable);
      instance.SetDestructor(&destruct_TQCompositorObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCompositorObservable*)
   {
      return GenerateInitInstanceLocal((::TQCompositorObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCompositorObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQContainerSizeObservable(void *p = 0);
   static void *newArray_TQContainerSizeObservable(Long_t size, void *p);
   static void delete_TQContainerSizeObservable(void *p);
   static void deleteArray_TQContainerSizeObservable(void *p);
   static void destruct_TQContainerSizeObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQContainerSizeObservable*)
   {
      ::TQContainerSizeObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQContainerSizeObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQContainerSizeObservable", ::TQContainerSizeObservable::Class_Version(), "CAFxAODUtils/TQContainerSizeObservable.h", 6,
                  typeid(::TQContainerSizeObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQContainerSizeObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQContainerSizeObservable) );
      instance.SetNew(&new_TQContainerSizeObservable);
      instance.SetNewArray(&newArray_TQContainerSizeObservable);
      instance.SetDelete(&delete_TQContainerSizeObservable);
      instance.SetDeleteArray(&deleteArray_TQContainerSizeObservable);
      instance.SetDestructor(&destruct_TQContainerSizeObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQContainerSizeObservable*)
   {
      return GenerateInitInstanceLocal((::TQContainerSizeObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQContainerSizeObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TQCPDecorationObservablelEfloatgR_Dictionary();
   static void TQCPDecorationObservablelEfloatgR_TClassManip(TClass*);
   static void *new_TQCPDecorationObservablelEfloatgR(void *p = 0);
   static void *newArray_TQCPDecorationObservablelEfloatgR(Long_t size, void *p);
   static void delete_TQCPDecorationObservablelEfloatgR(void *p);
   static void deleteArray_TQCPDecorationObservablelEfloatgR(void *p);
   static void destruct_TQCPDecorationObservablelEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCPDecorationObservable<float>*)
   {
      ::TQCPDecorationObservable<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCPDecorationObservable<float> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCPDecorationObservable<float>", ::TQCPDecorationObservable<float>::Class_Version(), "CAFxAODUtils/TQCPDecorationObservable.h", 11,
                  typeid(::TQCPDecorationObservable<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQCPDecorationObservablelEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQCPDecorationObservable<float>) );
      instance.SetNew(&new_TQCPDecorationObservablelEfloatgR);
      instance.SetNewArray(&newArray_TQCPDecorationObservablelEfloatgR);
      instance.SetDelete(&delete_TQCPDecorationObservablelEfloatgR);
      instance.SetDeleteArray(&deleteArray_TQCPDecorationObservablelEfloatgR);
      instance.SetDestructor(&destruct_TQCPDecorationObservablelEfloatgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCPDecorationObservable<float>*)
   {
      return GenerateInitInstanceLocal((::TQCPDecorationObservable<float>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCPDecorationObservable<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQCPDecorationObservablelEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<float>*)0x0)->GetClass();
      TQCPDecorationObservablelEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void TQCPDecorationObservablelEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQCPDecorationObservablelEdoublegR_Dictionary();
   static void TQCPDecorationObservablelEdoublegR_TClassManip(TClass*);
   static void *new_TQCPDecorationObservablelEdoublegR(void *p = 0);
   static void *newArray_TQCPDecorationObservablelEdoublegR(Long_t size, void *p);
   static void delete_TQCPDecorationObservablelEdoublegR(void *p);
   static void deleteArray_TQCPDecorationObservablelEdoublegR(void *p);
   static void destruct_TQCPDecorationObservablelEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCPDecorationObservable<double>*)
   {
      ::TQCPDecorationObservable<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCPDecorationObservable<double> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCPDecorationObservable<double>", ::TQCPDecorationObservable<double>::Class_Version(), "CAFxAODUtils/TQCPDecorationObservable.h", 11,
                  typeid(::TQCPDecorationObservable<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQCPDecorationObservablelEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::TQCPDecorationObservable<double>) );
      instance.SetNew(&new_TQCPDecorationObservablelEdoublegR);
      instance.SetNewArray(&newArray_TQCPDecorationObservablelEdoublegR);
      instance.SetDelete(&delete_TQCPDecorationObservablelEdoublegR);
      instance.SetDeleteArray(&deleteArray_TQCPDecorationObservablelEdoublegR);
      instance.SetDestructor(&destruct_TQCPDecorationObservablelEdoublegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCPDecorationObservable<double>*)
   {
      return GenerateInitInstanceLocal((::TQCPDecorationObservable<double>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCPDecorationObservable<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQCPDecorationObservablelEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<double>*)0x0)->GetClass();
      TQCPDecorationObservablelEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void TQCPDecorationObservablelEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQCPDecorationObservablelEchargR_Dictionary();
   static void TQCPDecorationObservablelEchargR_TClassManip(TClass*);
   static void *new_TQCPDecorationObservablelEchargR(void *p = 0);
   static void *newArray_TQCPDecorationObservablelEchargR(Long_t size, void *p);
   static void delete_TQCPDecorationObservablelEchargR(void *p);
   static void deleteArray_TQCPDecorationObservablelEchargR(void *p);
   static void destruct_TQCPDecorationObservablelEchargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCPDecorationObservable<char>*)
   {
      ::TQCPDecorationObservable<char> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCPDecorationObservable<char> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCPDecorationObservable<char>", ::TQCPDecorationObservable<char>::Class_Version(), "CAFxAODUtils/TQCPDecorationObservable.h", 11,
                  typeid(::TQCPDecorationObservable<char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQCPDecorationObservablelEchargR_Dictionary, isa_proxy, 4,
                  sizeof(::TQCPDecorationObservable<char>) );
      instance.SetNew(&new_TQCPDecorationObservablelEchargR);
      instance.SetNewArray(&newArray_TQCPDecorationObservablelEchargR);
      instance.SetDelete(&delete_TQCPDecorationObservablelEchargR);
      instance.SetDeleteArray(&deleteArray_TQCPDecorationObservablelEchargR);
      instance.SetDestructor(&destruct_TQCPDecorationObservablelEchargR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCPDecorationObservable<char>*)
   {
      return GenerateInitInstanceLocal((::TQCPDecorationObservable<char>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCPDecorationObservable<char>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQCPDecorationObservablelEchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<char>*)0x0)->GetClass();
      TQCPDecorationObservablelEchargR_TClassManip(theClass);
   return theClass;
   }

   static void TQCPDecorationObservablelEchargR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQCPDecorationObservablelEintgR_Dictionary();
   static void TQCPDecorationObservablelEintgR_TClassManip(TClass*);
   static void *new_TQCPDecorationObservablelEintgR(void *p = 0);
   static void *newArray_TQCPDecorationObservablelEintgR(Long_t size, void *p);
   static void delete_TQCPDecorationObservablelEintgR(void *p);
   static void deleteArray_TQCPDecorationObservablelEintgR(void *p);
   static void destruct_TQCPDecorationObservablelEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCPDecorationObservable<int>*)
   {
      ::TQCPDecorationObservable<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCPDecorationObservable<int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCPDecorationObservable<int>", ::TQCPDecorationObservable<int>::Class_Version(), "CAFxAODUtils/TQCPDecorationObservable.h", 11,
                  typeid(::TQCPDecorationObservable<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQCPDecorationObservablelEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQCPDecorationObservable<int>) );
      instance.SetNew(&new_TQCPDecorationObservablelEintgR);
      instance.SetNewArray(&newArray_TQCPDecorationObservablelEintgR);
      instance.SetDelete(&delete_TQCPDecorationObservablelEintgR);
      instance.SetDeleteArray(&deleteArray_TQCPDecorationObservablelEintgR);
      instance.SetDestructor(&destruct_TQCPDecorationObservablelEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCPDecorationObservable<int>*)
   {
      return GenerateInitInstanceLocal((::TQCPDecorationObservable<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCPDecorationObservable<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQCPDecorationObservablelEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<int>*)0x0)->GetClass();
      TQCPDecorationObservablelEintgR_TClassManip(theClass);
   return theClass;
   }

   static void TQCPDecorationObservablelEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQCPDecorationObservablelEunsignedsPintgR_Dictionary();
   static void TQCPDecorationObservablelEunsignedsPintgR_TClassManip(TClass*);
   static void *new_TQCPDecorationObservablelEunsignedsPintgR(void *p = 0);
   static void *newArray_TQCPDecorationObservablelEunsignedsPintgR(Long_t size, void *p);
   static void delete_TQCPDecorationObservablelEunsignedsPintgR(void *p);
   static void deleteArray_TQCPDecorationObservablelEunsignedsPintgR(void *p);
   static void destruct_TQCPDecorationObservablelEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCPDecorationObservable<unsigned int>*)
   {
      ::TQCPDecorationObservable<unsigned int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCPDecorationObservable<unsigned int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCPDecorationObservable<unsigned int>", ::TQCPDecorationObservable<unsigned int>::Class_Version(), "CAFxAODUtils/TQCPDecorationObservable.h", 11,
                  typeid(::TQCPDecorationObservable<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQCPDecorationObservablelEunsignedsPintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQCPDecorationObservable<unsigned int>) );
      instance.SetNew(&new_TQCPDecorationObservablelEunsignedsPintgR);
      instance.SetNewArray(&newArray_TQCPDecorationObservablelEunsignedsPintgR);
      instance.SetDelete(&delete_TQCPDecorationObservablelEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_TQCPDecorationObservablelEunsignedsPintgR);
      instance.SetDestructor(&destruct_TQCPDecorationObservablelEunsignedsPintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCPDecorationObservable<unsigned int>*)
   {
      return GenerateInitInstanceLocal((::TQCPDecorationObservable<unsigned int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCPDecorationObservable<unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQCPDecorationObservablelEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<unsigned int>*)0x0)->GetClass();
      TQCPDecorationObservablelEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void TQCPDecorationObservablelEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQCPDecorationObservablelEunsignedsPshortgR_Dictionary();
   static void TQCPDecorationObservablelEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_TQCPDecorationObservablelEunsignedsPshortgR(void *p = 0);
   static void *newArray_TQCPDecorationObservablelEunsignedsPshortgR(Long_t size, void *p);
   static void delete_TQCPDecorationObservablelEunsignedsPshortgR(void *p);
   static void deleteArray_TQCPDecorationObservablelEunsignedsPshortgR(void *p);
   static void destruct_TQCPDecorationObservablelEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCPDecorationObservable<unsigned short>*)
   {
      ::TQCPDecorationObservable<unsigned short> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCPDecorationObservable<unsigned short> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCPDecorationObservable<unsigned short>", ::TQCPDecorationObservable<unsigned short>::Class_Version(), "CAFxAODUtils/TQCPDecorationObservable.h", 11,
                  typeid(::TQCPDecorationObservable<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQCPDecorationObservablelEunsignedsPshortgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQCPDecorationObservable<unsigned short>) );
      instance.SetNew(&new_TQCPDecorationObservablelEunsignedsPshortgR);
      instance.SetNewArray(&newArray_TQCPDecorationObservablelEunsignedsPshortgR);
      instance.SetDelete(&delete_TQCPDecorationObservablelEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_TQCPDecorationObservablelEunsignedsPshortgR);
      instance.SetDestructor(&destruct_TQCPDecorationObservablelEunsignedsPshortgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCPDecorationObservable<unsigned short>*)
   {
      return GenerateInitInstanceLocal((::TQCPDecorationObservable<unsigned short>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCPDecorationObservable<unsigned short>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQCPDecorationObservablelEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<unsigned short>*)0x0)->GetClass();
      TQCPDecorationObservablelEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void TQCPDecorationObservablelEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQCPDecorationObservablelEULong64_tgR_Dictionary();
   static void TQCPDecorationObservablelEULong64_tgR_TClassManip(TClass*);
   static void *new_TQCPDecorationObservablelEULong64_tgR(void *p = 0);
   static void *newArray_TQCPDecorationObservablelEULong64_tgR(Long_t size, void *p);
   static void delete_TQCPDecorationObservablelEULong64_tgR(void *p);
   static void deleteArray_TQCPDecorationObservablelEULong64_tgR(void *p);
   static void destruct_TQCPDecorationObservablelEULong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCPDecorationObservable<ULong64_t>*)
   {
      ::TQCPDecorationObservable<ULong64_t> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCPDecorationObservable<ULong64_t> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCPDecorationObservable<ULong64_t>", ::TQCPDecorationObservable<ULong64_t>::Class_Version(), "CAFxAODUtils/TQCPDecorationObservable.h", 11,
                  typeid(::TQCPDecorationObservable<ULong64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQCPDecorationObservablelEULong64_tgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQCPDecorationObservable<ULong64_t>) );
      instance.SetNew(&new_TQCPDecorationObservablelEULong64_tgR);
      instance.SetNewArray(&newArray_TQCPDecorationObservablelEULong64_tgR);
      instance.SetDelete(&delete_TQCPDecorationObservablelEULong64_tgR);
      instance.SetDeleteArray(&deleteArray_TQCPDecorationObservablelEULong64_tgR);
      instance.SetDestructor(&destruct_TQCPDecorationObservablelEULong64_tgR);

      ::ROOT::AddClassAlternate("TQCPDecorationObservable<ULong64_t>","TQCPDecorationObservable<unsigned long long>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCPDecorationObservable<ULong64_t>*)
   {
      return GenerateInitInstanceLocal((::TQCPDecorationObservable<ULong64_t>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCPDecorationObservable<ULong64_t>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQCPDecorationObservablelEULong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<ULong64_t>*)0x0)->GetClass();
      TQCPDecorationObservablelEULong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void TQCPDecorationObservablelEULong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_TQCPConstituentVectorObservable(void *p = 0);
   static void *newArray_TQCPConstituentVectorObservable(Long_t size, void *p);
   static void delete_TQCPConstituentVectorObservable(void *p);
   static void deleteArray_TQCPConstituentVectorObservable(void *p);
   static void destruct_TQCPConstituentVectorObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCPConstituentVectorObservable*)
   {
      ::TQCPConstituentVectorObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCPConstituentVectorObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCPConstituentVectorObservable", ::TQCPConstituentVectorObservable::Class_Version(), "CAFxAODUtils/TQCPConstituentVectorObservable.h", 11,
                  typeid(::TQCPConstituentVectorObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQCPConstituentVectorObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQCPConstituentVectorObservable) );
      instance.SetNew(&new_TQCPConstituentVectorObservable);
      instance.SetNewArray(&newArray_TQCPConstituentVectorObservable);
      instance.SetDelete(&delete_TQCPConstituentVectorObservable);
      instance.SetDeleteArray(&deleteArray_TQCPConstituentVectorObservable);
      instance.SetDestructor(&destruct_TQCPConstituentVectorObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCPConstituentVectorObservable*)
   {
      return GenerateInitInstanceLocal((::TQCPConstituentVectorObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCPConstituentVectorObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQEventFlaggingAnalysisJob(void *p = 0);
   static void *newArray_TQEventFlaggingAnalysisJob(Long_t size, void *p);
   static void delete_TQEventFlaggingAnalysisJob(void *p);
   static void deleteArray_TQEventFlaggingAnalysisJob(void *p);
   static void destruct_TQEventFlaggingAnalysisJob(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQEventFlaggingAnalysisJob*)
   {
      ::TQEventFlaggingAnalysisJob *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQEventFlaggingAnalysisJob >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQEventFlaggingAnalysisJob", ::TQEventFlaggingAnalysisJob::Class_Version(), "CAFxAODUtils/TQEventFlaggingAnalysisJob.h", 15,
                  typeid(::TQEventFlaggingAnalysisJob), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQEventFlaggingAnalysisJob::Dictionary, isa_proxy, 4,
                  sizeof(::TQEventFlaggingAnalysisJob) );
      instance.SetNew(&new_TQEventFlaggingAnalysisJob);
      instance.SetNewArray(&newArray_TQEventFlaggingAnalysisJob);
      instance.SetDelete(&delete_TQEventFlaggingAnalysisJob);
      instance.SetDeleteArray(&deleteArray_TQEventFlaggingAnalysisJob);
      instance.SetDestructor(&destruct_TQEventFlaggingAnalysisJob);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQEventFlaggingAnalysisJob*)
   {
      return GenerateInitInstanceLocal((::TQEventFlaggingAnalysisJob*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQEventFlaggingAnalysisJob*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationObservablelEfloatgR_Dictionary();
   static void TQIParticleDecorationObservablelEfloatgR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationObservablelEfloatgR(void *p = 0);
   static void *newArray_TQIParticleDecorationObservablelEfloatgR(Long_t size, void *p);
   static void delete_TQIParticleDecorationObservablelEfloatgR(void *p);
   static void deleteArray_TQIParticleDecorationObservablelEfloatgR(void *p);
   static void destruct_TQIParticleDecorationObservablelEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationObservable<float>*)
   {
      ::TQIParticleDecorationObservable<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationObservable<float> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationObservable<float>", ::TQIParticleDecorationObservable<float>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationObservable.h", 13,
                  typeid(::TQIParticleDecorationObservable<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationObservablelEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationObservable<float>) );
      instance.SetNew(&new_TQIParticleDecorationObservablelEfloatgR);
      instance.SetNewArray(&newArray_TQIParticleDecorationObservablelEfloatgR);
      instance.SetDelete(&delete_TQIParticleDecorationObservablelEfloatgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationObservablelEfloatgR);
      instance.SetDestructor(&destruct_TQIParticleDecorationObservablelEfloatgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationObservable<float>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationObservable<float>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationObservablelEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<float>*)0x0)->GetClass();
      TQIParticleDecorationObservablelEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationObservablelEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationObservablelEdoublegR_Dictionary();
   static void TQIParticleDecorationObservablelEdoublegR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationObservablelEdoublegR(void *p = 0);
   static void *newArray_TQIParticleDecorationObservablelEdoublegR(Long_t size, void *p);
   static void delete_TQIParticleDecorationObservablelEdoublegR(void *p);
   static void deleteArray_TQIParticleDecorationObservablelEdoublegR(void *p);
   static void destruct_TQIParticleDecorationObservablelEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationObservable<double>*)
   {
      ::TQIParticleDecorationObservable<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationObservable<double> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationObservable<double>", ::TQIParticleDecorationObservable<double>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationObservable.h", 13,
                  typeid(::TQIParticleDecorationObservable<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationObservablelEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationObservable<double>) );
      instance.SetNew(&new_TQIParticleDecorationObservablelEdoublegR);
      instance.SetNewArray(&newArray_TQIParticleDecorationObservablelEdoublegR);
      instance.SetDelete(&delete_TQIParticleDecorationObservablelEdoublegR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationObservablelEdoublegR);
      instance.SetDestructor(&destruct_TQIParticleDecorationObservablelEdoublegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationObservable<double>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationObservable<double>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationObservablelEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<double>*)0x0)->GetClass();
      TQIParticleDecorationObservablelEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationObservablelEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationObservablelEchargR_Dictionary();
   static void TQIParticleDecorationObservablelEchargR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationObservablelEchargR(void *p = 0);
   static void *newArray_TQIParticleDecorationObservablelEchargR(Long_t size, void *p);
   static void delete_TQIParticleDecorationObservablelEchargR(void *p);
   static void deleteArray_TQIParticleDecorationObservablelEchargR(void *p);
   static void destruct_TQIParticleDecorationObservablelEchargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationObservable<char>*)
   {
      ::TQIParticleDecorationObservable<char> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationObservable<char> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationObservable<char>", ::TQIParticleDecorationObservable<char>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationObservable.h", 13,
                  typeid(::TQIParticleDecorationObservable<char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationObservablelEchargR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationObservable<char>) );
      instance.SetNew(&new_TQIParticleDecorationObservablelEchargR);
      instance.SetNewArray(&newArray_TQIParticleDecorationObservablelEchargR);
      instance.SetDelete(&delete_TQIParticleDecorationObservablelEchargR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationObservablelEchargR);
      instance.SetDestructor(&destruct_TQIParticleDecorationObservablelEchargR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationObservable<char>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationObservable<char>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<char>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationObservablelEchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<char>*)0x0)->GetClass();
      TQIParticleDecorationObservablelEchargR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationObservablelEchargR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationObservablelEintgR_Dictionary();
   static void TQIParticleDecorationObservablelEintgR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationObservablelEintgR(void *p = 0);
   static void *newArray_TQIParticleDecorationObservablelEintgR(Long_t size, void *p);
   static void delete_TQIParticleDecorationObservablelEintgR(void *p);
   static void deleteArray_TQIParticleDecorationObservablelEintgR(void *p);
   static void destruct_TQIParticleDecorationObservablelEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationObservable<int>*)
   {
      ::TQIParticleDecorationObservable<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationObservable<int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationObservable<int>", ::TQIParticleDecorationObservable<int>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationObservable.h", 13,
                  typeid(::TQIParticleDecorationObservable<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationObservablelEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationObservable<int>) );
      instance.SetNew(&new_TQIParticleDecorationObservablelEintgR);
      instance.SetNewArray(&newArray_TQIParticleDecorationObservablelEintgR);
      instance.SetDelete(&delete_TQIParticleDecorationObservablelEintgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationObservablelEintgR);
      instance.SetDestructor(&destruct_TQIParticleDecorationObservablelEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationObservable<int>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationObservable<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationObservablelEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<int>*)0x0)->GetClass();
      TQIParticleDecorationObservablelEintgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationObservablelEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationObservablelEunsignedsPintgR_Dictionary();
   static void TQIParticleDecorationObservablelEunsignedsPintgR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationObservablelEunsignedsPintgR(void *p = 0);
   static void *newArray_TQIParticleDecorationObservablelEunsignedsPintgR(Long_t size, void *p);
   static void delete_TQIParticleDecorationObservablelEunsignedsPintgR(void *p);
   static void deleteArray_TQIParticleDecorationObservablelEunsignedsPintgR(void *p);
   static void destruct_TQIParticleDecorationObservablelEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationObservable<unsigned int>*)
   {
      ::TQIParticleDecorationObservable<unsigned int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationObservable<unsigned int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationObservable<unsigned int>", ::TQIParticleDecorationObservable<unsigned int>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationObservable.h", 13,
                  typeid(::TQIParticleDecorationObservable<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationObservablelEunsignedsPintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationObservable<unsigned int>) );
      instance.SetNew(&new_TQIParticleDecorationObservablelEunsignedsPintgR);
      instance.SetNewArray(&newArray_TQIParticleDecorationObservablelEunsignedsPintgR);
      instance.SetDelete(&delete_TQIParticleDecorationObservablelEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationObservablelEunsignedsPintgR);
      instance.SetDestructor(&destruct_TQIParticleDecorationObservablelEunsignedsPintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationObservable<unsigned int>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationObservable<unsigned int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationObservablelEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<unsigned int>*)0x0)->GetClass();
      TQIParticleDecorationObservablelEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationObservablelEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationObservablelEunsignedsPshortgR_Dictionary();
   static void TQIParticleDecorationObservablelEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationObservablelEunsignedsPshortgR(void *p = 0);
   static void *newArray_TQIParticleDecorationObservablelEunsignedsPshortgR(Long_t size, void *p);
   static void delete_TQIParticleDecorationObservablelEunsignedsPshortgR(void *p);
   static void deleteArray_TQIParticleDecorationObservablelEunsignedsPshortgR(void *p);
   static void destruct_TQIParticleDecorationObservablelEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationObservable<unsigned short>*)
   {
      ::TQIParticleDecorationObservable<unsigned short> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationObservable<unsigned short> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationObservable<unsigned short>", ::TQIParticleDecorationObservable<unsigned short>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationObservable.h", 13,
                  typeid(::TQIParticleDecorationObservable<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationObservablelEunsignedsPshortgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationObservable<unsigned short>) );
      instance.SetNew(&new_TQIParticleDecorationObservablelEunsignedsPshortgR);
      instance.SetNewArray(&newArray_TQIParticleDecorationObservablelEunsignedsPshortgR);
      instance.SetDelete(&delete_TQIParticleDecorationObservablelEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationObservablelEunsignedsPshortgR);
      instance.SetDestructor(&destruct_TQIParticleDecorationObservablelEunsignedsPshortgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationObservable<unsigned short>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationObservable<unsigned short>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<unsigned short>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationObservablelEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<unsigned short>*)0x0)->GetClass();
      TQIParticleDecorationObservablelEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationObservablelEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationObservablelEULong64_tgR_Dictionary();
   static void TQIParticleDecorationObservablelEULong64_tgR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationObservablelEULong64_tgR(void *p = 0);
   static void *newArray_TQIParticleDecorationObservablelEULong64_tgR(Long_t size, void *p);
   static void delete_TQIParticleDecorationObservablelEULong64_tgR(void *p);
   static void deleteArray_TQIParticleDecorationObservablelEULong64_tgR(void *p);
   static void destruct_TQIParticleDecorationObservablelEULong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationObservable<ULong64_t>*)
   {
      ::TQIParticleDecorationObservable<ULong64_t> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationObservable<ULong64_t> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationObservable<ULong64_t>", ::TQIParticleDecorationObservable<ULong64_t>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationObservable.h", 13,
                  typeid(::TQIParticleDecorationObservable<ULong64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationObservablelEULong64_tgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationObservable<ULong64_t>) );
      instance.SetNew(&new_TQIParticleDecorationObservablelEULong64_tgR);
      instance.SetNewArray(&newArray_TQIParticleDecorationObservablelEULong64_tgR);
      instance.SetDelete(&delete_TQIParticleDecorationObservablelEULong64_tgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationObservablelEULong64_tgR);
      instance.SetDestructor(&destruct_TQIParticleDecorationObservablelEULong64_tgR);

      ::ROOT::AddClassAlternate("TQIParticleDecorationObservable<ULong64_t>","TQIParticleDecorationObservable<unsigned long long>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationObservable<ULong64_t>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationObservable<ULong64_t>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<ULong64_t>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationObservablelEULong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<ULong64_t>*)0x0)->GetClass();
      TQIParticleDecorationObservablelEULong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationObservablelEULong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationVectorObservablelEfloatgR_Dictionary();
   static void TQIParticleDecorationVectorObservablelEfloatgR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationVectorObservablelEfloatgR(void *p = 0);
   static void *newArray_TQIParticleDecorationVectorObservablelEfloatgR(Long_t size, void *p);
   static void delete_TQIParticleDecorationVectorObservablelEfloatgR(void *p);
   static void deleteArray_TQIParticleDecorationVectorObservablelEfloatgR(void *p);
   static void destruct_TQIParticleDecorationVectorObservablelEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationVectorObservable<float>*)
   {
      ::TQIParticleDecorationVectorObservable<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationVectorObservable<float> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationVectorObservable<float>", ::TQIParticleDecorationVectorObservable<float>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationVectorObservable.h", 14,
                  typeid(::TQIParticleDecorationVectorObservable<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationVectorObservablelEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationVectorObservable<float>) );
      instance.SetNew(&new_TQIParticleDecorationVectorObservablelEfloatgR);
      instance.SetNewArray(&newArray_TQIParticleDecorationVectorObservablelEfloatgR);
      instance.SetDelete(&delete_TQIParticleDecorationVectorObservablelEfloatgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationVectorObservablelEfloatgR);
      instance.SetDestructor(&destruct_TQIParticleDecorationVectorObservablelEfloatgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationVectorObservable<float>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationVectorObservable<float>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationVectorObservablelEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<float>*)0x0)->GetClass();
      TQIParticleDecorationVectorObservablelEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationVectorObservablelEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationVectorObservablelEdoublegR_Dictionary();
   static void TQIParticleDecorationVectorObservablelEdoublegR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationVectorObservablelEdoublegR(void *p = 0);
   static void *newArray_TQIParticleDecorationVectorObservablelEdoublegR(Long_t size, void *p);
   static void delete_TQIParticleDecorationVectorObservablelEdoublegR(void *p);
   static void deleteArray_TQIParticleDecorationVectorObservablelEdoublegR(void *p);
   static void destruct_TQIParticleDecorationVectorObservablelEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationVectorObservable<double>*)
   {
      ::TQIParticleDecorationVectorObservable<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationVectorObservable<double> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationVectorObservable<double>", ::TQIParticleDecorationVectorObservable<double>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationVectorObservable.h", 14,
                  typeid(::TQIParticleDecorationVectorObservable<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationVectorObservablelEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationVectorObservable<double>) );
      instance.SetNew(&new_TQIParticleDecorationVectorObservablelEdoublegR);
      instance.SetNewArray(&newArray_TQIParticleDecorationVectorObservablelEdoublegR);
      instance.SetDelete(&delete_TQIParticleDecorationVectorObservablelEdoublegR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationVectorObservablelEdoublegR);
      instance.SetDestructor(&destruct_TQIParticleDecorationVectorObservablelEdoublegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationVectorObservable<double>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationVectorObservable<double>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationVectorObservablelEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<double>*)0x0)->GetClass();
      TQIParticleDecorationVectorObservablelEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationVectorObservablelEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationVectorObservablelEchargR_Dictionary();
   static void TQIParticleDecorationVectorObservablelEchargR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationVectorObservablelEchargR(void *p = 0);
   static void *newArray_TQIParticleDecorationVectorObservablelEchargR(Long_t size, void *p);
   static void delete_TQIParticleDecorationVectorObservablelEchargR(void *p);
   static void deleteArray_TQIParticleDecorationVectorObservablelEchargR(void *p);
   static void destruct_TQIParticleDecorationVectorObservablelEchargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationVectorObservable<char>*)
   {
      ::TQIParticleDecorationVectorObservable<char> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationVectorObservable<char> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationVectorObservable<char>", ::TQIParticleDecorationVectorObservable<char>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationVectorObservable.h", 14,
                  typeid(::TQIParticleDecorationVectorObservable<char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationVectorObservablelEchargR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationVectorObservable<char>) );
      instance.SetNew(&new_TQIParticleDecorationVectorObservablelEchargR);
      instance.SetNewArray(&newArray_TQIParticleDecorationVectorObservablelEchargR);
      instance.SetDelete(&delete_TQIParticleDecorationVectorObservablelEchargR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationVectorObservablelEchargR);
      instance.SetDestructor(&destruct_TQIParticleDecorationVectorObservablelEchargR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationVectorObservable<char>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationVectorObservable<char>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<char>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationVectorObservablelEchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<char>*)0x0)->GetClass();
      TQIParticleDecorationVectorObservablelEchargR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationVectorObservablelEchargR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationVectorObservablelEintgR_Dictionary();
   static void TQIParticleDecorationVectorObservablelEintgR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationVectorObservablelEintgR(void *p = 0);
   static void *newArray_TQIParticleDecorationVectorObservablelEintgR(Long_t size, void *p);
   static void delete_TQIParticleDecorationVectorObservablelEintgR(void *p);
   static void deleteArray_TQIParticleDecorationVectorObservablelEintgR(void *p);
   static void destruct_TQIParticleDecorationVectorObservablelEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationVectorObservable<int>*)
   {
      ::TQIParticleDecorationVectorObservable<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationVectorObservable<int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationVectorObservable<int>", ::TQIParticleDecorationVectorObservable<int>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationVectorObservable.h", 14,
                  typeid(::TQIParticleDecorationVectorObservable<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationVectorObservablelEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationVectorObservable<int>) );
      instance.SetNew(&new_TQIParticleDecorationVectorObservablelEintgR);
      instance.SetNewArray(&newArray_TQIParticleDecorationVectorObservablelEintgR);
      instance.SetDelete(&delete_TQIParticleDecorationVectorObservablelEintgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationVectorObservablelEintgR);
      instance.SetDestructor(&destruct_TQIParticleDecorationVectorObservablelEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationVectorObservable<int>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationVectorObservable<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationVectorObservablelEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<int>*)0x0)->GetClass();
      TQIParticleDecorationVectorObservablelEintgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationVectorObservablelEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationVectorObservablelEunsignedsPintgR_Dictionary();
   static void TQIParticleDecorationVectorObservablelEunsignedsPintgR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationVectorObservablelEunsignedsPintgR(void *p = 0);
   static void *newArray_TQIParticleDecorationVectorObservablelEunsignedsPintgR(Long_t size, void *p);
   static void delete_TQIParticleDecorationVectorObservablelEunsignedsPintgR(void *p);
   static void deleteArray_TQIParticleDecorationVectorObservablelEunsignedsPintgR(void *p);
   static void destruct_TQIParticleDecorationVectorObservablelEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationVectorObservable<unsigned int>*)
   {
      ::TQIParticleDecorationVectorObservable<unsigned int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationVectorObservable<unsigned int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationVectorObservable<unsigned int>", ::TQIParticleDecorationVectorObservable<unsigned int>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationVectorObservable.h", 14,
                  typeid(::TQIParticleDecorationVectorObservable<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationVectorObservablelEunsignedsPintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationVectorObservable<unsigned int>) );
      instance.SetNew(&new_TQIParticleDecorationVectorObservablelEunsignedsPintgR);
      instance.SetNewArray(&newArray_TQIParticleDecorationVectorObservablelEunsignedsPintgR);
      instance.SetDelete(&delete_TQIParticleDecorationVectorObservablelEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationVectorObservablelEunsignedsPintgR);
      instance.SetDestructor(&destruct_TQIParticleDecorationVectorObservablelEunsignedsPintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationVectorObservable<unsigned int>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationVectorObservable<unsigned int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationVectorObservablelEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<unsigned int>*)0x0)->GetClass();
      TQIParticleDecorationVectorObservablelEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationVectorObservablelEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationVectorObservablelEunsignedsPshortgR_Dictionary();
   static void TQIParticleDecorationVectorObservablelEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationVectorObservablelEunsignedsPshortgR(void *p = 0);
   static void *newArray_TQIParticleDecorationVectorObservablelEunsignedsPshortgR(Long_t size, void *p);
   static void delete_TQIParticleDecorationVectorObservablelEunsignedsPshortgR(void *p);
   static void deleteArray_TQIParticleDecorationVectorObservablelEunsignedsPshortgR(void *p);
   static void destruct_TQIParticleDecorationVectorObservablelEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationVectorObservable<unsigned short>*)
   {
      ::TQIParticleDecorationVectorObservable<unsigned short> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationVectorObservable<unsigned short> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationVectorObservable<unsigned short>", ::TQIParticleDecorationVectorObservable<unsigned short>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationVectorObservable.h", 14,
                  typeid(::TQIParticleDecorationVectorObservable<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationVectorObservablelEunsignedsPshortgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationVectorObservable<unsigned short>) );
      instance.SetNew(&new_TQIParticleDecorationVectorObservablelEunsignedsPshortgR);
      instance.SetNewArray(&newArray_TQIParticleDecorationVectorObservablelEunsignedsPshortgR);
      instance.SetDelete(&delete_TQIParticleDecorationVectorObservablelEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationVectorObservablelEunsignedsPshortgR);
      instance.SetDestructor(&destruct_TQIParticleDecorationVectorObservablelEunsignedsPshortgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationVectorObservable<unsigned short>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationVectorObservable<unsigned short>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<unsigned short>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationVectorObservablelEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<unsigned short>*)0x0)->GetClass();
      TQIParticleDecorationVectorObservablelEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationVectorObservablelEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDecorationVectorObservablelEULong64_tgR_Dictionary();
   static void TQIParticleDecorationVectorObservablelEULong64_tgR_TClassManip(TClass*);
   static void *new_TQIParticleDecorationVectorObservablelEULong64_tgR(void *p = 0);
   static void *newArray_TQIParticleDecorationVectorObservablelEULong64_tgR(Long_t size, void *p);
   static void delete_TQIParticleDecorationVectorObservablelEULong64_tgR(void *p);
   static void deleteArray_TQIParticleDecorationVectorObservablelEULong64_tgR(void *p);
   static void destruct_TQIParticleDecorationVectorObservablelEULong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDecorationVectorObservable<ULong64_t>*)
   {
      ::TQIParticleDecorationVectorObservable<ULong64_t> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDecorationVectorObservable<ULong64_t> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDecorationVectorObservable<ULong64_t>", ::TQIParticleDecorationVectorObservable<ULong64_t>::Class_Version(), "CAFxAODUtils/TQIParticleDecorationVectorObservable.h", 14,
                  typeid(::TQIParticleDecorationVectorObservable<ULong64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDecorationVectorObservablelEULong64_tgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDecorationVectorObservable<ULong64_t>) );
      instance.SetNew(&new_TQIParticleDecorationVectorObservablelEULong64_tgR);
      instance.SetNewArray(&newArray_TQIParticleDecorationVectorObservablelEULong64_tgR);
      instance.SetDelete(&delete_TQIParticleDecorationVectorObservablelEULong64_tgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDecorationVectorObservablelEULong64_tgR);
      instance.SetDestructor(&destruct_TQIParticleDecorationVectorObservablelEULong64_tgR);

      ::ROOT::AddClassAlternate("TQIParticleDecorationVectorObservable<ULong64_t>","TQIParticleDecorationVectorObservable<unsigned long long>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDecorationVectorObservable<ULong64_t>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDecorationVectorObservable<ULong64_t>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<ULong64_t>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDecorationVectorObservablelEULong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<ULong64_t>*)0x0)->GetClass();
      TQIParticleDecorationVectorObservablelEULong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDecorationVectorObservablelEULong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_TQIParticleFourVectorObservable(void *p = 0);
   static void *newArray_TQIParticleFourVectorObservable(Long_t size, void *p);
   static void delete_TQIParticleFourVectorObservable(void *p);
   static void deleteArray_TQIParticleFourVectorObservable(void *p);
   static void destruct_TQIParticleFourVectorObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleFourVectorObservable*)
   {
      ::TQIParticleFourVectorObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleFourVectorObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleFourVectorObservable", ::TQIParticleFourVectorObservable::Class_Version(), "CAFxAODUtils/TQIParticleFourVectorObservable.h", 16,
                  typeid(::TQIParticleFourVectorObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQIParticleFourVectorObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleFourVectorObservable) );
      instance.SetNew(&new_TQIParticleFourVectorObservable);
      instance.SetNewArray(&newArray_TQIParticleFourVectorObservable);
      instance.SetDelete(&delete_TQIParticleFourVectorObservable);
      instance.SetDeleteArray(&deleteArray_TQIParticleFourVectorObservable);
      instance.SetDestructor(&destruct_TQIParticleFourVectorObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleFourVectorObservable*)
   {
      return GenerateInitInstanceLocal((::TQIParticleFourVectorObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleFourVectorObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQIParticleFourVectorVectorObservable(void *p = 0);
   static void *newArray_TQIParticleFourVectorVectorObservable(Long_t size, void *p);
   static void delete_TQIParticleFourVectorVectorObservable(void *p);
   static void deleteArray_TQIParticleFourVectorVectorObservable(void *p);
   static void destruct_TQIParticleFourVectorVectorObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleFourVectorVectorObservable*)
   {
      ::TQIParticleFourVectorVectorObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleFourVectorVectorObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleFourVectorVectorObservable", ::TQIParticleFourVectorVectorObservable::Class_Version(), "CAFxAODUtils/TQIParticleFourVectorVectorObservable.h", 16,
                  typeid(::TQIParticleFourVectorVectorObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQIParticleFourVectorVectorObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleFourVectorVectorObservable) );
      instance.SetNew(&new_TQIParticleFourVectorVectorObservable);
      instance.SetNewArray(&newArray_TQIParticleFourVectorVectorObservable);
      instance.SetDelete(&delete_TQIParticleFourVectorVectorObservable);
      instance.SetDeleteArray(&deleteArray_TQIParticleFourVectorVectorObservable);
      instance.SetDestructor(&destruct_TQIParticleFourVectorVectorObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleFourVectorVectorObservable*)
   {
      return GenerateInitInstanceLocal((::TQIParticleFourVectorVectorObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleFourVectorVectorObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQTruthParticleInfoObservable(void *p = 0);
   static void *newArray_TQTruthParticleInfoObservable(Long_t size, void *p);
   static void delete_TQTruthParticleInfoObservable(void *p);
   static void deleteArray_TQTruthParticleInfoObservable(void *p);
   static void destruct_TQTruthParticleInfoObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQTruthParticleInfoObservable*)
   {
      ::TQTruthParticleInfoObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQTruthParticleInfoObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQTruthParticleInfoObservable", ::TQTruthParticleInfoObservable::Class_Version(), "CAFxAODUtils/TQTruthParticleInfoObservable.h", 15,
                  typeid(::TQTruthParticleInfoObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQTruthParticleInfoObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQTruthParticleInfoObservable) );
      instance.SetNew(&new_TQTruthParticleInfoObservable);
      instance.SetNewArray(&newArray_TQTruthParticleInfoObservable);
      instance.SetDelete(&delete_TQTruthParticleInfoObservable);
      instance.SetDeleteArray(&deleteArray_TQTruthParticleInfoObservable);
      instance.SetDestructor(&destruct_TQTruthParticleInfoObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQTruthParticleInfoObservable*)
   {
      return GenerateInitInstanceLocal((::TQTruthParticleInfoObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQTruthParticleInfoObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxElementDecorationObservablelEfloatgR_Dictionary();
   static void TQSGAuxElementDecorationObservablelEfloatgR_TClassManip(TClass*);
   static void *new_TQSGAuxElementDecorationObservablelEfloatgR(void *p = 0);
   static void *newArray_TQSGAuxElementDecorationObservablelEfloatgR(Long_t size, void *p);
   static void delete_TQSGAuxElementDecorationObservablelEfloatgR(void *p);
   static void deleteArray_TQSGAuxElementDecorationObservablelEfloatgR(void *p);
   static void destruct_TQSGAuxElementDecorationObservablelEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxElementDecorationObservable<float>*)
   {
      ::TQSGAuxElementDecorationObservable<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxElementDecorationObservable<float> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxElementDecorationObservable<float>", ::TQSGAuxElementDecorationObservable<float>::Class_Version(), "CAFxAODUtils/TQSGAuxElementDecorationObservable.h", 10,
                  typeid(::TQSGAuxElementDecorationObservable<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxElementDecorationObservablelEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxElementDecorationObservable<float>) );
      instance.SetNew(&new_TQSGAuxElementDecorationObservablelEfloatgR);
      instance.SetNewArray(&newArray_TQSGAuxElementDecorationObservablelEfloatgR);
      instance.SetDelete(&delete_TQSGAuxElementDecorationObservablelEfloatgR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxElementDecorationObservablelEfloatgR);
      instance.SetDestructor(&destruct_TQSGAuxElementDecorationObservablelEfloatgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxElementDecorationObservable<float>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxElementDecorationObservable<float>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxElementDecorationObservablelEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<float>*)0x0)->GetClass();
      TQSGAuxElementDecorationObservablelEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxElementDecorationObservablelEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxElementDecorationObservablelEdoublegR_Dictionary();
   static void TQSGAuxElementDecorationObservablelEdoublegR_TClassManip(TClass*);
   static void *new_TQSGAuxElementDecorationObservablelEdoublegR(void *p = 0);
   static void *newArray_TQSGAuxElementDecorationObservablelEdoublegR(Long_t size, void *p);
   static void delete_TQSGAuxElementDecorationObservablelEdoublegR(void *p);
   static void deleteArray_TQSGAuxElementDecorationObservablelEdoublegR(void *p);
   static void destruct_TQSGAuxElementDecorationObservablelEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxElementDecorationObservable<double>*)
   {
      ::TQSGAuxElementDecorationObservable<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxElementDecorationObservable<double> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxElementDecorationObservable<double>", ::TQSGAuxElementDecorationObservable<double>::Class_Version(), "CAFxAODUtils/TQSGAuxElementDecorationObservable.h", 10,
                  typeid(::TQSGAuxElementDecorationObservable<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxElementDecorationObservablelEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxElementDecorationObservable<double>) );
      instance.SetNew(&new_TQSGAuxElementDecorationObservablelEdoublegR);
      instance.SetNewArray(&newArray_TQSGAuxElementDecorationObservablelEdoublegR);
      instance.SetDelete(&delete_TQSGAuxElementDecorationObservablelEdoublegR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxElementDecorationObservablelEdoublegR);
      instance.SetDestructor(&destruct_TQSGAuxElementDecorationObservablelEdoublegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxElementDecorationObservable<double>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxElementDecorationObservable<double>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxElementDecorationObservablelEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<double>*)0x0)->GetClass();
      TQSGAuxElementDecorationObservablelEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxElementDecorationObservablelEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxElementDecorationObservablelEchargR_Dictionary();
   static void TQSGAuxElementDecorationObservablelEchargR_TClassManip(TClass*);
   static void *new_TQSGAuxElementDecorationObservablelEchargR(void *p = 0);
   static void *newArray_TQSGAuxElementDecorationObservablelEchargR(Long_t size, void *p);
   static void delete_TQSGAuxElementDecorationObservablelEchargR(void *p);
   static void deleteArray_TQSGAuxElementDecorationObservablelEchargR(void *p);
   static void destruct_TQSGAuxElementDecorationObservablelEchargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxElementDecorationObservable<char>*)
   {
      ::TQSGAuxElementDecorationObservable<char> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxElementDecorationObservable<char> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxElementDecorationObservable<char>", ::TQSGAuxElementDecorationObservable<char>::Class_Version(), "CAFxAODUtils/TQSGAuxElementDecorationObservable.h", 10,
                  typeid(::TQSGAuxElementDecorationObservable<char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxElementDecorationObservablelEchargR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxElementDecorationObservable<char>) );
      instance.SetNew(&new_TQSGAuxElementDecorationObservablelEchargR);
      instance.SetNewArray(&newArray_TQSGAuxElementDecorationObservablelEchargR);
      instance.SetDelete(&delete_TQSGAuxElementDecorationObservablelEchargR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxElementDecorationObservablelEchargR);
      instance.SetDestructor(&destruct_TQSGAuxElementDecorationObservablelEchargR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxElementDecorationObservable<char>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxElementDecorationObservable<char>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<char>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxElementDecorationObservablelEchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<char>*)0x0)->GetClass();
      TQSGAuxElementDecorationObservablelEchargR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxElementDecorationObservablelEchargR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxElementDecorationObservablelEintgR_Dictionary();
   static void TQSGAuxElementDecorationObservablelEintgR_TClassManip(TClass*);
   static void *new_TQSGAuxElementDecorationObservablelEintgR(void *p = 0);
   static void *newArray_TQSGAuxElementDecorationObservablelEintgR(Long_t size, void *p);
   static void delete_TQSGAuxElementDecorationObservablelEintgR(void *p);
   static void deleteArray_TQSGAuxElementDecorationObservablelEintgR(void *p);
   static void destruct_TQSGAuxElementDecorationObservablelEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxElementDecorationObservable<int>*)
   {
      ::TQSGAuxElementDecorationObservable<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxElementDecorationObservable<int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxElementDecorationObservable<int>", ::TQSGAuxElementDecorationObservable<int>::Class_Version(), "CAFxAODUtils/TQSGAuxElementDecorationObservable.h", 10,
                  typeid(::TQSGAuxElementDecorationObservable<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxElementDecorationObservablelEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxElementDecorationObservable<int>) );
      instance.SetNew(&new_TQSGAuxElementDecorationObservablelEintgR);
      instance.SetNewArray(&newArray_TQSGAuxElementDecorationObservablelEintgR);
      instance.SetDelete(&delete_TQSGAuxElementDecorationObservablelEintgR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxElementDecorationObservablelEintgR);
      instance.SetDestructor(&destruct_TQSGAuxElementDecorationObservablelEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxElementDecorationObservable<int>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxElementDecorationObservable<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxElementDecorationObservablelEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<int>*)0x0)->GetClass();
      TQSGAuxElementDecorationObservablelEintgR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxElementDecorationObservablelEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxElementDecorationObservablelEunsignedsPintgR_Dictionary();
   static void TQSGAuxElementDecorationObservablelEunsignedsPintgR_TClassManip(TClass*);
   static void *new_TQSGAuxElementDecorationObservablelEunsignedsPintgR(void *p = 0);
   static void *newArray_TQSGAuxElementDecorationObservablelEunsignedsPintgR(Long_t size, void *p);
   static void delete_TQSGAuxElementDecorationObservablelEunsignedsPintgR(void *p);
   static void deleteArray_TQSGAuxElementDecorationObservablelEunsignedsPintgR(void *p);
   static void destruct_TQSGAuxElementDecorationObservablelEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxElementDecorationObservable<unsigned int>*)
   {
      ::TQSGAuxElementDecorationObservable<unsigned int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxElementDecorationObservable<unsigned int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxElementDecorationObservable<unsigned int>", ::TQSGAuxElementDecorationObservable<unsigned int>::Class_Version(), "CAFxAODUtils/TQSGAuxElementDecorationObservable.h", 10,
                  typeid(::TQSGAuxElementDecorationObservable<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxElementDecorationObservablelEunsignedsPintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxElementDecorationObservable<unsigned int>) );
      instance.SetNew(&new_TQSGAuxElementDecorationObservablelEunsignedsPintgR);
      instance.SetNewArray(&newArray_TQSGAuxElementDecorationObservablelEunsignedsPintgR);
      instance.SetDelete(&delete_TQSGAuxElementDecorationObservablelEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxElementDecorationObservablelEunsignedsPintgR);
      instance.SetDestructor(&destruct_TQSGAuxElementDecorationObservablelEunsignedsPintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxElementDecorationObservable<unsigned int>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxElementDecorationObservable<unsigned int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxElementDecorationObservablelEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<unsigned int>*)0x0)->GetClass();
      TQSGAuxElementDecorationObservablelEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxElementDecorationObservablelEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxElementDecorationObservablelEunsignedsPshortgR_Dictionary();
   static void TQSGAuxElementDecorationObservablelEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_TQSGAuxElementDecorationObservablelEunsignedsPshortgR(void *p = 0);
   static void *newArray_TQSGAuxElementDecorationObservablelEunsignedsPshortgR(Long_t size, void *p);
   static void delete_TQSGAuxElementDecorationObservablelEunsignedsPshortgR(void *p);
   static void deleteArray_TQSGAuxElementDecorationObservablelEunsignedsPshortgR(void *p);
   static void destruct_TQSGAuxElementDecorationObservablelEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxElementDecorationObservable<unsigned short>*)
   {
      ::TQSGAuxElementDecorationObservable<unsigned short> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxElementDecorationObservable<unsigned short> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxElementDecorationObservable<unsigned short>", ::TQSGAuxElementDecorationObservable<unsigned short>::Class_Version(), "CAFxAODUtils/TQSGAuxElementDecorationObservable.h", 10,
                  typeid(::TQSGAuxElementDecorationObservable<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxElementDecorationObservablelEunsignedsPshortgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxElementDecorationObservable<unsigned short>) );
      instance.SetNew(&new_TQSGAuxElementDecorationObservablelEunsignedsPshortgR);
      instance.SetNewArray(&newArray_TQSGAuxElementDecorationObservablelEunsignedsPshortgR);
      instance.SetDelete(&delete_TQSGAuxElementDecorationObservablelEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxElementDecorationObservablelEunsignedsPshortgR);
      instance.SetDestructor(&destruct_TQSGAuxElementDecorationObservablelEunsignedsPshortgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxElementDecorationObservable<unsigned short>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxElementDecorationObservable<unsigned short>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<unsigned short>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxElementDecorationObservablelEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<unsigned short>*)0x0)->GetClass();
      TQSGAuxElementDecorationObservablelEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxElementDecorationObservablelEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxElementDecorationObservablelEULong64_tgR_Dictionary();
   static void TQSGAuxElementDecorationObservablelEULong64_tgR_TClassManip(TClass*);
   static void *new_TQSGAuxElementDecorationObservablelEULong64_tgR(void *p = 0);
   static void *newArray_TQSGAuxElementDecorationObservablelEULong64_tgR(Long_t size, void *p);
   static void delete_TQSGAuxElementDecorationObservablelEULong64_tgR(void *p);
   static void deleteArray_TQSGAuxElementDecorationObservablelEULong64_tgR(void *p);
   static void destruct_TQSGAuxElementDecorationObservablelEULong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxElementDecorationObservable<ULong64_t>*)
   {
      ::TQSGAuxElementDecorationObservable<ULong64_t> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxElementDecorationObservable<ULong64_t> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxElementDecorationObservable<ULong64_t>", ::TQSGAuxElementDecorationObservable<ULong64_t>::Class_Version(), "CAFxAODUtils/TQSGAuxElementDecorationObservable.h", 10,
                  typeid(::TQSGAuxElementDecorationObservable<ULong64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxElementDecorationObservablelEULong64_tgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxElementDecorationObservable<ULong64_t>) );
      instance.SetNew(&new_TQSGAuxElementDecorationObservablelEULong64_tgR);
      instance.SetNewArray(&newArray_TQSGAuxElementDecorationObservablelEULong64_tgR);
      instance.SetDelete(&delete_TQSGAuxElementDecorationObservablelEULong64_tgR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxElementDecorationObservablelEULong64_tgR);
      instance.SetDestructor(&destruct_TQSGAuxElementDecorationObservablelEULong64_tgR);

      ::ROOT::AddClassAlternate("TQSGAuxElementDecorationObservable<ULong64_t>","TQSGAuxElementDecorationObservable<unsigned long long>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxElementDecorationObservable<ULong64_t>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxElementDecorationObservable<ULong64_t>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<ULong64_t>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxElementDecorationObservablelEULong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<ULong64_t>*)0x0)->GetClass();
      TQSGAuxElementDecorationObservablelEULong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxElementDecorationObservablelEULong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxFlagCheckingObservablelEfloatgR_Dictionary();
   static void TQSGAuxFlagCheckingObservablelEfloatgR_TClassManip(TClass*);
   static void *new_TQSGAuxFlagCheckingObservablelEfloatgR(void *p = 0);
   static void *newArray_TQSGAuxFlagCheckingObservablelEfloatgR(Long_t size, void *p);
   static void delete_TQSGAuxFlagCheckingObservablelEfloatgR(void *p);
   static void deleteArray_TQSGAuxFlagCheckingObservablelEfloatgR(void *p);
   static void destruct_TQSGAuxFlagCheckingObservablelEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxFlagCheckingObservable<float>*)
   {
      ::TQSGAuxFlagCheckingObservable<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxFlagCheckingObservable<float> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxFlagCheckingObservable<float>", ::TQSGAuxFlagCheckingObservable<float>::Class_Version(), "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h", 10,
                  typeid(::TQSGAuxFlagCheckingObservable<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxFlagCheckingObservablelEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxFlagCheckingObservable<float>) );
      instance.SetNew(&new_TQSGAuxFlagCheckingObservablelEfloatgR);
      instance.SetNewArray(&newArray_TQSGAuxFlagCheckingObservablelEfloatgR);
      instance.SetDelete(&delete_TQSGAuxFlagCheckingObservablelEfloatgR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxFlagCheckingObservablelEfloatgR);
      instance.SetDestructor(&destruct_TQSGAuxFlagCheckingObservablelEfloatgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxFlagCheckingObservable<float>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxFlagCheckingObservable<float>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxFlagCheckingObservablelEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<float>*)0x0)->GetClass();
      TQSGAuxFlagCheckingObservablelEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxFlagCheckingObservablelEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxFlagCheckingObservablelEdoublegR_Dictionary();
   static void TQSGAuxFlagCheckingObservablelEdoublegR_TClassManip(TClass*);
   static void *new_TQSGAuxFlagCheckingObservablelEdoublegR(void *p = 0);
   static void *newArray_TQSGAuxFlagCheckingObservablelEdoublegR(Long_t size, void *p);
   static void delete_TQSGAuxFlagCheckingObservablelEdoublegR(void *p);
   static void deleteArray_TQSGAuxFlagCheckingObservablelEdoublegR(void *p);
   static void destruct_TQSGAuxFlagCheckingObservablelEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxFlagCheckingObservable<double>*)
   {
      ::TQSGAuxFlagCheckingObservable<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxFlagCheckingObservable<double> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxFlagCheckingObservable<double>", ::TQSGAuxFlagCheckingObservable<double>::Class_Version(), "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h", 10,
                  typeid(::TQSGAuxFlagCheckingObservable<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxFlagCheckingObservablelEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxFlagCheckingObservable<double>) );
      instance.SetNew(&new_TQSGAuxFlagCheckingObservablelEdoublegR);
      instance.SetNewArray(&newArray_TQSGAuxFlagCheckingObservablelEdoublegR);
      instance.SetDelete(&delete_TQSGAuxFlagCheckingObservablelEdoublegR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxFlagCheckingObservablelEdoublegR);
      instance.SetDestructor(&destruct_TQSGAuxFlagCheckingObservablelEdoublegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxFlagCheckingObservable<double>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxFlagCheckingObservable<double>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxFlagCheckingObservablelEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<double>*)0x0)->GetClass();
      TQSGAuxFlagCheckingObservablelEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxFlagCheckingObservablelEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxFlagCheckingObservablelEchargR_Dictionary();
   static void TQSGAuxFlagCheckingObservablelEchargR_TClassManip(TClass*);
   static void *new_TQSGAuxFlagCheckingObservablelEchargR(void *p = 0);
   static void *newArray_TQSGAuxFlagCheckingObservablelEchargR(Long_t size, void *p);
   static void delete_TQSGAuxFlagCheckingObservablelEchargR(void *p);
   static void deleteArray_TQSGAuxFlagCheckingObservablelEchargR(void *p);
   static void destruct_TQSGAuxFlagCheckingObservablelEchargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxFlagCheckingObservable<char>*)
   {
      ::TQSGAuxFlagCheckingObservable<char> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxFlagCheckingObservable<char> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxFlagCheckingObservable<char>", ::TQSGAuxFlagCheckingObservable<char>::Class_Version(), "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h", 10,
                  typeid(::TQSGAuxFlagCheckingObservable<char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxFlagCheckingObservablelEchargR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxFlagCheckingObservable<char>) );
      instance.SetNew(&new_TQSGAuxFlagCheckingObservablelEchargR);
      instance.SetNewArray(&newArray_TQSGAuxFlagCheckingObservablelEchargR);
      instance.SetDelete(&delete_TQSGAuxFlagCheckingObservablelEchargR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxFlagCheckingObservablelEchargR);
      instance.SetDestructor(&destruct_TQSGAuxFlagCheckingObservablelEchargR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxFlagCheckingObservable<char>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxFlagCheckingObservable<char>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<char>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxFlagCheckingObservablelEchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<char>*)0x0)->GetClass();
      TQSGAuxFlagCheckingObservablelEchargR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxFlagCheckingObservablelEchargR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxFlagCheckingObservablelEintgR_Dictionary();
   static void TQSGAuxFlagCheckingObservablelEintgR_TClassManip(TClass*);
   static void *new_TQSGAuxFlagCheckingObservablelEintgR(void *p = 0);
   static void *newArray_TQSGAuxFlagCheckingObservablelEintgR(Long_t size, void *p);
   static void delete_TQSGAuxFlagCheckingObservablelEintgR(void *p);
   static void deleteArray_TQSGAuxFlagCheckingObservablelEintgR(void *p);
   static void destruct_TQSGAuxFlagCheckingObservablelEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxFlagCheckingObservable<int>*)
   {
      ::TQSGAuxFlagCheckingObservable<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxFlagCheckingObservable<int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxFlagCheckingObservable<int>", ::TQSGAuxFlagCheckingObservable<int>::Class_Version(), "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h", 10,
                  typeid(::TQSGAuxFlagCheckingObservable<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxFlagCheckingObservablelEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxFlagCheckingObservable<int>) );
      instance.SetNew(&new_TQSGAuxFlagCheckingObservablelEintgR);
      instance.SetNewArray(&newArray_TQSGAuxFlagCheckingObservablelEintgR);
      instance.SetDelete(&delete_TQSGAuxFlagCheckingObservablelEintgR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxFlagCheckingObservablelEintgR);
      instance.SetDestructor(&destruct_TQSGAuxFlagCheckingObservablelEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxFlagCheckingObservable<int>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxFlagCheckingObservable<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxFlagCheckingObservablelEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<int>*)0x0)->GetClass();
      TQSGAuxFlagCheckingObservablelEintgR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxFlagCheckingObservablelEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxFlagCheckingObservablelEunsignedsPintgR_Dictionary();
   static void TQSGAuxFlagCheckingObservablelEunsignedsPintgR_TClassManip(TClass*);
   static void *new_TQSGAuxFlagCheckingObservablelEunsignedsPintgR(void *p = 0);
   static void *newArray_TQSGAuxFlagCheckingObservablelEunsignedsPintgR(Long_t size, void *p);
   static void delete_TQSGAuxFlagCheckingObservablelEunsignedsPintgR(void *p);
   static void deleteArray_TQSGAuxFlagCheckingObservablelEunsignedsPintgR(void *p);
   static void destruct_TQSGAuxFlagCheckingObservablelEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxFlagCheckingObservable<unsigned int>*)
   {
      ::TQSGAuxFlagCheckingObservable<unsigned int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxFlagCheckingObservable<unsigned int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxFlagCheckingObservable<unsigned int>", ::TQSGAuxFlagCheckingObservable<unsigned int>::Class_Version(), "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h", 10,
                  typeid(::TQSGAuxFlagCheckingObservable<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxFlagCheckingObservablelEunsignedsPintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxFlagCheckingObservable<unsigned int>) );
      instance.SetNew(&new_TQSGAuxFlagCheckingObservablelEunsignedsPintgR);
      instance.SetNewArray(&newArray_TQSGAuxFlagCheckingObservablelEunsignedsPintgR);
      instance.SetDelete(&delete_TQSGAuxFlagCheckingObservablelEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxFlagCheckingObservablelEunsignedsPintgR);
      instance.SetDestructor(&destruct_TQSGAuxFlagCheckingObservablelEunsignedsPintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxFlagCheckingObservable<unsigned int>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxFlagCheckingObservable<unsigned int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxFlagCheckingObservablelEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<unsigned int>*)0x0)->GetClass();
      TQSGAuxFlagCheckingObservablelEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxFlagCheckingObservablelEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxFlagCheckingObservablelEunsignedsPshortgR_Dictionary();
   static void TQSGAuxFlagCheckingObservablelEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR(void *p = 0);
   static void *newArray_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR(Long_t size, void *p);
   static void delete_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR(void *p);
   static void deleteArray_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR(void *p);
   static void destruct_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxFlagCheckingObservable<unsigned short>*)
   {
      ::TQSGAuxFlagCheckingObservable<unsigned short> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxFlagCheckingObservable<unsigned short> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxFlagCheckingObservable<unsigned short>", ::TQSGAuxFlagCheckingObservable<unsigned short>::Class_Version(), "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h", 10,
                  typeid(::TQSGAuxFlagCheckingObservable<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxFlagCheckingObservablelEunsignedsPshortgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxFlagCheckingObservable<unsigned short>) );
      instance.SetNew(&new_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR);
      instance.SetNewArray(&newArray_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR);
      instance.SetDelete(&delete_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR);
      instance.SetDestructor(&destruct_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxFlagCheckingObservable<unsigned short>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxFlagCheckingObservable<unsigned short>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<unsigned short>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxFlagCheckingObservablelEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<unsigned short>*)0x0)->GetClass();
      TQSGAuxFlagCheckingObservablelEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxFlagCheckingObservablelEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQSGAuxFlagCheckingObservablelEULong64_tgR_Dictionary();
   static void TQSGAuxFlagCheckingObservablelEULong64_tgR_TClassManip(TClass*);
   static void *new_TQSGAuxFlagCheckingObservablelEULong64_tgR(void *p = 0);
   static void *newArray_TQSGAuxFlagCheckingObservablelEULong64_tgR(Long_t size, void *p);
   static void delete_TQSGAuxFlagCheckingObservablelEULong64_tgR(void *p);
   static void deleteArray_TQSGAuxFlagCheckingObservablelEULong64_tgR(void *p);
   static void destruct_TQSGAuxFlagCheckingObservablelEULong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSGAuxFlagCheckingObservable<ULong64_t>*)
   {
      ::TQSGAuxFlagCheckingObservable<ULong64_t> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSGAuxFlagCheckingObservable<ULong64_t> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSGAuxFlagCheckingObservable<ULong64_t>", ::TQSGAuxFlagCheckingObservable<ULong64_t>::Class_Version(), "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h", 10,
                  typeid(::TQSGAuxFlagCheckingObservable<ULong64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQSGAuxFlagCheckingObservablelEULong64_tgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQSGAuxFlagCheckingObservable<ULong64_t>) );
      instance.SetNew(&new_TQSGAuxFlagCheckingObservablelEULong64_tgR);
      instance.SetNewArray(&newArray_TQSGAuxFlagCheckingObservablelEULong64_tgR);
      instance.SetDelete(&delete_TQSGAuxFlagCheckingObservablelEULong64_tgR);
      instance.SetDeleteArray(&deleteArray_TQSGAuxFlagCheckingObservablelEULong64_tgR);
      instance.SetDestructor(&destruct_TQSGAuxFlagCheckingObservablelEULong64_tgR);

      ::ROOT::AddClassAlternate("TQSGAuxFlagCheckingObservable<ULong64_t>","TQSGAuxFlagCheckingObservable<unsigned long long>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSGAuxFlagCheckingObservable<ULong64_t>*)
   {
      return GenerateInitInstanceLocal((::TQSGAuxFlagCheckingObservable<ULong64_t>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<ULong64_t>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQSGAuxFlagCheckingObservablelEULong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<ULong64_t>*)0x0)->GetClass();
      TQSGAuxFlagCheckingObservablelEULong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void TQSGAuxFlagCheckingObservablelEULong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_TQToolObservable(void *p = 0);
   static void *newArray_TQToolObservable(Long_t size, void *p);
   static void delete_TQToolObservable(void *p);
   static void deleteArray_TQToolObservable(void *p);
   static void destruct_TQToolObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQToolObservable*)
   {
      ::TQToolObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQToolObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQToolObservable", ::TQToolObservable::Class_Version(), "CAFxAODUtils/TQToolObservable.h", 10,
                  typeid(::TQToolObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQToolObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQToolObservable) );
      instance.SetNew(&new_TQToolObservable);
      instance.SetNewArray(&newArray_TQToolObservable);
      instance.SetDelete(&delete_TQToolObservable);
      instance.SetDeleteArray(&deleteArray_TQToolObservable);
      instance.SetDestructor(&destruct_TQToolObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQToolObservable*)
   {
      return GenerateInitInstanceLocal((::TQToolObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQToolObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQxAODskimmingAlgorithm(void *p = 0);
   static void *newArray_TQxAODskimmingAlgorithm(Long_t size, void *p);
   static void delete_TQxAODskimmingAlgorithm(void *p);
   static void deleteArray_TQxAODskimmingAlgorithm(void *p);
   static void destruct_TQxAODskimmingAlgorithm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQxAODskimmingAlgorithm*)
   {
      ::TQxAODskimmingAlgorithm *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQxAODskimmingAlgorithm >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQxAODskimmingAlgorithm", ::TQxAODskimmingAlgorithm::Class_Version(), "CAFxAODUtils/TQxAODskimmingAlgorithm.h", 17,
                  typeid(::TQxAODskimmingAlgorithm), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQxAODskimmingAlgorithm::Dictionary, isa_proxy, 4,
                  sizeof(::TQxAODskimmingAlgorithm) );
      instance.SetNew(&new_TQxAODskimmingAlgorithm);
      instance.SetNewArray(&newArray_TQxAODskimmingAlgorithm);
      instance.SetDelete(&delete_TQxAODskimmingAlgorithm);
      instance.SetDeleteArray(&deleteArray_TQxAODskimmingAlgorithm);
      instance.SetDestructor(&destruct_TQxAODskimmingAlgorithm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQxAODskimmingAlgorithm*)
   {
      return GenerateInitInstanceLocal((::TQxAODskimmingAlgorithm*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQxAODskimmingAlgorithm*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TQEventObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQEventObservable::Class_Name()
{
   return "TQEventObservable";
}

//______________________________________________________________________________
const char *TQEventObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQEventObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQEventObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQEventObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQEventObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQEventObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQEventObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQEventObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQCompositorObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQCompositorObservable::Class_Name()
{
   return "TQCompositorObservable";
}

//______________________________________________________________________________
const char *TQCompositorObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCompositorObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQCompositorObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCompositorObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQCompositorObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCompositorObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQCompositorObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCompositorObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQContainerSizeObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQContainerSizeObservable::Class_Name()
{
   return "TQContainerSizeObservable";
}

//______________________________________________________________________________
const char *TQContainerSizeObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQContainerSizeObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQContainerSizeObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQContainerSizeObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQContainerSizeObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQContainerSizeObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQContainerSizeObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQContainerSizeObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQCPDecorationObservable<float>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<float>::Class_Name()
{
   return "TQCPDecorationObservable<float>";
}

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<float>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQCPDecorationObservable<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<float>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<float>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<float>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQCPDecorationObservable<double>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<double>::Class_Name()
{
   return "TQCPDecorationObservable<double>";
}

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<double>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQCPDecorationObservable<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<double>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<double>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<double>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQCPDecorationObservable<char>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<char>::Class_Name()
{
   return "TQCPDecorationObservable<char>";
}

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<char>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<char>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQCPDecorationObservable<char>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<char>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<char>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<char>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<char>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<char>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQCPDecorationObservable<int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<int>::Class_Name()
{
   return "TQCPDecorationObservable<int>";
}

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQCPDecorationObservable<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQCPDecorationObservable<unsigned int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<unsigned int>::Class_Name()
{
   return "TQCPDecorationObservable<unsigned int>";
}

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<unsigned int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<unsigned int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQCPDecorationObservable<unsigned int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<unsigned int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<unsigned int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<unsigned int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<unsigned int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<unsigned int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQCPDecorationObservable<unsigned short>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<unsigned short>::Class_Name()
{
   return "TQCPDecorationObservable<unsigned short>";
}

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<unsigned short>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<unsigned short>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQCPDecorationObservable<unsigned short>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<unsigned short>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<unsigned short>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<unsigned short>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<unsigned short>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<unsigned short>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQCPDecorationObservable<ULong64_t>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<ULong64_t>::Class_Name()
{
   return "TQCPDecorationObservable<ULong64_t>";
}

//______________________________________________________________________________
template <> const char *TQCPDecorationObservable<ULong64_t>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<ULong64_t>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQCPDecorationObservable<ULong64_t>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<ULong64_t>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<ULong64_t>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<ULong64_t>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQCPDecorationObservable<ULong64_t>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPDecorationObservable<ULong64_t>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQCPConstituentVectorObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQCPConstituentVectorObservable::Class_Name()
{
   return "TQCPConstituentVectorObservable";
}

//______________________________________________________________________________
const char *TQCPConstituentVectorObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPConstituentVectorObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQCPConstituentVectorObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCPConstituentVectorObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQCPConstituentVectorObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPConstituentVectorObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQCPConstituentVectorObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCPConstituentVectorObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQEventFlaggingAnalysisJob::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQEventFlaggingAnalysisJob::Class_Name()
{
   return "TQEventFlaggingAnalysisJob";
}

//______________________________________________________________________________
const char *TQEventFlaggingAnalysisJob::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQEventFlaggingAnalysisJob*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQEventFlaggingAnalysisJob::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQEventFlaggingAnalysisJob*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQEventFlaggingAnalysisJob::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQEventFlaggingAnalysisJob*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQEventFlaggingAnalysisJob::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQEventFlaggingAnalysisJob*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationObservable<float>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<float>::Class_Name()
{
   return "TQIParticleDecorationObservable<float>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<float>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationObservable<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<float>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<float>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<float>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationObservable<double>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<double>::Class_Name()
{
   return "TQIParticleDecorationObservable<double>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<double>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationObservable<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<double>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<double>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<double>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationObservable<char>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<char>::Class_Name()
{
   return "TQIParticleDecorationObservable<char>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<char>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<char>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationObservable<char>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<char>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<char>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<char>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<char>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<char>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationObservable<int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<int>::Class_Name()
{
   return "TQIParticleDecorationObservable<int>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationObservable<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationObservable<unsigned int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<unsigned int>::Class_Name()
{
   return "TQIParticleDecorationObservable<unsigned int>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<unsigned int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<unsigned int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationObservable<unsigned int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<unsigned int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<unsigned int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<unsigned int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<unsigned int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<unsigned int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationObservable<unsigned short>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<unsigned short>::Class_Name()
{
   return "TQIParticleDecorationObservable<unsigned short>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<unsigned short>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<unsigned short>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationObservable<unsigned short>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<unsigned short>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<unsigned short>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<unsigned short>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<unsigned short>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<unsigned short>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationObservable<ULong64_t>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<ULong64_t>::Class_Name()
{
   return "TQIParticleDecorationObservable<ULong64_t>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationObservable<ULong64_t>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<ULong64_t>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationObservable<ULong64_t>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<ULong64_t>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<ULong64_t>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<ULong64_t>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationObservable<ULong64_t>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationObservable<ULong64_t>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationVectorObservable<float>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<float>::Class_Name()
{
   return "TQIParticleDecorationVectorObservable<float>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<float>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationVectorObservable<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<float>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<float>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<float>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationVectorObservable<double>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<double>::Class_Name()
{
   return "TQIParticleDecorationVectorObservable<double>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<double>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationVectorObservable<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<double>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<double>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<double>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationVectorObservable<char>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<char>::Class_Name()
{
   return "TQIParticleDecorationVectorObservable<char>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<char>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<char>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationVectorObservable<char>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<char>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<char>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<char>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<char>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<char>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationVectorObservable<int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<int>::Class_Name()
{
   return "TQIParticleDecorationVectorObservable<int>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationVectorObservable<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationVectorObservable<unsigned int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<unsigned int>::Class_Name()
{
   return "TQIParticleDecorationVectorObservable<unsigned int>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<unsigned int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<unsigned int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationVectorObservable<unsigned int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<unsigned int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<unsigned int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<unsigned int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<unsigned int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<unsigned int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationVectorObservable<unsigned short>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<unsigned short>::Class_Name()
{
   return "TQIParticleDecorationVectorObservable<unsigned short>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<unsigned short>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<unsigned short>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationVectorObservable<unsigned short>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<unsigned short>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<unsigned short>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<unsigned short>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<unsigned short>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<unsigned short>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDecorationVectorObservable<ULong64_t>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<ULong64_t>::Class_Name()
{
   return "TQIParticleDecorationVectorObservable<ULong64_t>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDecorationVectorObservable<ULong64_t>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<ULong64_t>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDecorationVectorObservable<ULong64_t>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<ULong64_t>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<ULong64_t>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<ULong64_t>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDecorationVectorObservable<ULong64_t>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDecorationVectorObservable<ULong64_t>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQIParticleFourVectorObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQIParticleFourVectorObservable::Class_Name()
{
   return "TQIParticleFourVectorObservable";
}

//______________________________________________________________________________
const char *TQIParticleFourVectorObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleFourVectorObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQIParticleFourVectorObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleFourVectorObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQIParticleFourVectorObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleFourVectorObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQIParticleFourVectorObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleFourVectorObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQIParticleFourVectorVectorObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQIParticleFourVectorVectorObservable::Class_Name()
{
   return "TQIParticleFourVectorVectorObservable";
}

//______________________________________________________________________________
const char *TQIParticleFourVectorVectorObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleFourVectorVectorObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQIParticleFourVectorVectorObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleFourVectorVectorObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQIParticleFourVectorVectorObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleFourVectorVectorObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQIParticleFourVectorVectorObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleFourVectorVectorObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQTruthParticleInfoObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQTruthParticleInfoObservable::Class_Name()
{
   return "TQTruthParticleInfoObservable";
}

//______________________________________________________________________________
const char *TQTruthParticleInfoObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTruthParticleInfoObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQTruthParticleInfoObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTruthParticleInfoObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQTruthParticleInfoObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTruthParticleInfoObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQTruthParticleInfoObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTruthParticleInfoObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxElementDecorationObservable<float>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<float>::Class_Name()
{
   return "TQSGAuxElementDecorationObservable<float>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<float>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxElementDecorationObservable<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<float>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<float>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<float>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxElementDecorationObservable<double>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<double>::Class_Name()
{
   return "TQSGAuxElementDecorationObservable<double>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<double>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxElementDecorationObservable<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<double>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<double>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<double>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxElementDecorationObservable<char>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<char>::Class_Name()
{
   return "TQSGAuxElementDecorationObservable<char>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<char>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<char>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxElementDecorationObservable<char>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<char>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<char>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<char>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<char>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<char>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxElementDecorationObservable<int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<int>::Class_Name()
{
   return "TQSGAuxElementDecorationObservable<int>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxElementDecorationObservable<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxElementDecorationObservable<unsigned int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<unsigned int>::Class_Name()
{
   return "TQSGAuxElementDecorationObservable<unsigned int>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<unsigned int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<unsigned int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxElementDecorationObservable<unsigned int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<unsigned int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<unsigned int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<unsigned int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<unsigned int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<unsigned int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxElementDecorationObservable<unsigned short>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<unsigned short>::Class_Name()
{
   return "TQSGAuxElementDecorationObservable<unsigned short>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<unsigned short>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<unsigned short>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxElementDecorationObservable<unsigned short>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<unsigned short>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<unsigned short>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<unsigned short>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<unsigned short>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<unsigned short>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxElementDecorationObservable<ULong64_t>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<ULong64_t>::Class_Name()
{
   return "TQSGAuxElementDecorationObservable<ULong64_t>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxElementDecorationObservable<ULong64_t>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<ULong64_t>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxElementDecorationObservable<ULong64_t>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<ULong64_t>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<ULong64_t>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<ULong64_t>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxElementDecorationObservable<ULong64_t>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxElementDecorationObservable<ULong64_t>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxFlagCheckingObservable<float>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<float>::Class_Name()
{
   return "TQSGAuxFlagCheckingObservable<float>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<float>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxFlagCheckingObservable<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<float>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<float>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<float>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxFlagCheckingObservable<double>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<double>::Class_Name()
{
   return "TQSGAuxFlagCheckingObservable<double>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<double>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxFlagCheckingObservable<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<double>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<double>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<double>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxFlagCheckingObservable<char>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<char>::Class_Name()
{
   return "TQSGAuxFlagCheckingObservable<char>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<char>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<char>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxFlagCheckingObservable<char>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<char>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<char>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<char>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<char>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<char>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxFlagCheckingObservable<int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<int>::Class_Name()
{
   return "TQSGAuxFlagCheckingObservable<int>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxFlagCheckingObservable<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxFlagCheckingObservable<unsigned int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<unsigned int>::Class_Name()
{
   return "TQSGAuxFlagCheckingObservable<unsigned int>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<unsigned int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<unsigned int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxFlagCheckingObservable<unsigned int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<unsigned int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<unsigned int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<unsigned int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<unsigned int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<unsigned int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxFlagCheckingObservable<unsigned short>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<unsigned short>::Class_Name()
{
   return "TQSGAuxFlagCheckingObservable<unsigned short>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<unsigned short>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<unsigned short>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxFlagCheckingObservable<unsigned short>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<unsigned short>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<unsigned short>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<unsigned short>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<unsigned short>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<unsigned short>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQSGAuxFlagCheckingObservable<ULong64_t>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<ULong64_t>::Class_Name()
{
   return "TQSGAuxFlagCheckingObservable<ULong64_t>";
}

//______________________________________________________________________________
template <> const char *TQSGAuxFlagCheckingObservable<ULong64_t>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<ULong64_t>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQSGAuxFlagCheckingObservable<ULong64_t>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<ULong64_t>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<ULong64_t>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<ULong64_t>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQSGAuxFlagCheckingObservable<ULong64_t>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSGAuxFlagCheckingObservable<ULong64_t>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQToolObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQToolObservable::Class_Name()
{
   return "TQToolObservable";
}

//______________________________________________________________________________
const char *TQToolObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQToolObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQToolObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQToolObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQToolObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQToolObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQToolObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQToolObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQxAODskimmingAlgorithm::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQxAODskimmingAlgorithm::Class_Name()
{
   return "TQxAODskimmingAlgorithm";
}

//______________________________________________________________________________
const char *TQxAODskimmingAlgorithm::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQxAODskimmingAlgorithm*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQxAODskimmingAlgorithm::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQxAODskimmingAlgorithm*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQxAODskimmingAlgorithm::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQxAODskimmingAlgorithm*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQxAODskimmingAlgorithm::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQxAODskimmingAlgorithm*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TQEventObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQEventObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQEventObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQEventObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQEventObservable(void *p) {
      delete ((::TQEventObservable*)p);
   }
   static void deleteArray_TQEventObservable(void *p) {
      delete [] ((::TQEventObservable*)p);
   }
   static void destruct_TQEventObservable(void *p) {
      typedef ::TQEventObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQEventObservable

//______________________________________________________________________________
void TQCompositorObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCompositorObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCompositorObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCompositorObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCompositorObservable(void *p) {
      return  p ? new(p) ::TQCompositorObservable : new ::TQCompositorObservable;
   }
   static void *newArray_TQCompositorObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQCompositorObservable[nElements] : new ::TQCompositorObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCompositorObservable(void *p) {
      delete ((::TQCompositorObservable*)p);
   }
   static void deleteArray_TQCompositorObservable(void *p) {
      delete [] ((::TQCompositorObservable*)p);
   }
   static void destruct_TQCompositorObservable(void *p) {
      typedef ::TQCompositorObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCompositorObservable

//______________________________________________________________________________
void TQContainerSizeObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQContainerSizeObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQContainerSizeObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQContainerSizeObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQContainerSizeObservable(void *p) {
      return  p ? new(p) ::TQContainerSizeObservable : new ::TQContainerSizeObservable;
   }
   static void *newArray_TQContainerSizeObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQContainerSizeObservable[nElements] : new ::TQContainerSizeObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQContainerSizeObservable(void *p) {
      delete ((::TQContainerSizeObservable*)p);
   }
   static void deleteArray_TQContainerSizeObservable(void *p) {
      delete [] ((::TQContainerSizeObservable*)p);
   }
   static void destruct_TQContainerSizeObservable(void *p) {
      typedef ::TQContainerSizeObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQContainerSizeObservable

//______________________________________________________________________________
template <> void TQCPDecorationObservable<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCPDecorationObservable<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCPDecorationObservable<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCPDecorationObservable<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCPDecorationObservablelEfloatgR(void *p) {
      return  p ? new(p) ::TQCPDecorationObservable<float> : new ::TQCPDecorationObservable<float>;
   }
   static void *newArray_TQCPDecorationObservablelEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQCPDecorationObservable<float>[nElements] : new ::TQCPDecorationObservable<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCPDecorationObservablelEfloatgR(void *p) {
      delete ((::TQCPDecorationObservable<float>*)p);
   }
   static void deleteArray_TQCPDecorationObservablelEfloatgR(void *p) {
      delete [] ((::TQCPDecorationObservable<float>*)p);
   }
   static void destruct_TQCPDecorationObservablelEfloatgR(void *p) {
      typedef ::TQCPDecorationObservable<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCPDecorationObservable<float>

//______________________________________________________________________________
template <> void TQCPDecorationObservable<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCPDecorationObservable<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCPDecorationObservable<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCPDecorationObservable<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCPDecorationObservablelEdoublegR(void *p) {
      return  p ? new(p) ::TQCPDecorationObservable<double> : new ::TQCPDecorationObservable<double>;
   }
   static void *newArray_TQCPDecorationObservablelEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::TQCPDecorationObservable<double>[nElements] : new ::TQCPDecorationObservable<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCPDecorationObservablelEdoublegR(void *p) {
      delete ((::TQCPDecorationObservable<double>*)p);
   }
   static void deleteArray_TQCPDecorationObservablelEdoublegR(void *p) {
      delete [] ((::TQCPDecorationObservable<double>*)p);
   }
   static void destruct_TQCPDecorationObservablelEdoublegR(void *p) {
      typedef ::TQCPDecorationObservable<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCPDecorationObservable<double>

//______________________________________________________________________________
template <> void TQCPDecorationObservable<char>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCPDecorationObservable<char>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCPDecorationObservable<char>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCPDecorationObservable<char>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCPDecorationObservablelEchargR(void *p) {
      return  p ? new(p) ::TQCPDecorationObservable<char> : new ::TQCPDecorationObservable<char>;
   }
   static void *newArray_TQCPDecorationObservablelEchargR(Long_t nElements, void *p) {
      return p ? new(p) ::TQCPDecorationObservable<char>[nElements] : new ::TQCPDecorationObservable<char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCPDecorationObservablelEchargR(void *p) {
      delete ((::TQCPDecorationObservable<char>*)p);
   }
   static void deleteArray_TQCPDecorationObservablelEchargR(void *p) {
      delete [] ((::TQCPDecorationObservable<char>*)p);
   }
   static void destruct_TQCPDecorationObservablelEchargR(void *p) {
      typedef ::TQCPDecorationObservable<char> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCPDecorationObservable<char>

//______________________________________________________________________________
template <> void TQCPDecorationObservable<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCPDecorationObservable<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCPDecorationObservable<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCPDecorationObservable<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCPDecorationObservablelEintgR(void *p) {
      return  p ? new(p) ::TQCPDecorationObservable<int> : new ::TQCPDecorationObservable<int>;
   }
   static void *newArray_TQCPDecorationObservablelEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQCPDecorationObservable<int>[nElements] : new ::TQCPDecorationObservable<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCPDecorationObservablelEintgR(void *p) {
      delete ((::TQCPDecorationObservable<int>*)p);
   }
   static void deleteArray_TQCPDecorationObservablelEintgR(void *p) {
      delete [] ((::TQCPDecorationObservable<int>*)p);
   }
   static void destruct_TQCPDecorationObservablelEintgR(void *p) {
      typedef ::TQCPDecorationObservable<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCPDecorationObservable<int>

//______________________________________________________________________________
template <> void TQCPDecorationObservable<unsigned int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCPDecorationObservable<unsigned int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCPDecorationObservable<unsigned int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCPDecorationObservable<unsigned int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCPDecorationObservablelEunsignedsPintgR(void *p) {
      return  p ? new(p) ::TQCPDecorationObservable<unsigned int> : new ::TQCPDecorationObservable<unsigned int>;
   }
   static void *newArray_TQCPDecorationObservablelEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQCPDecorationObservable<unsigned int>[nElements] : new ::TQCPDecorationObservable<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCPDecorationObservablelEunsignedsPintgR(void *p) {
      delete ((::TQCPDecorationObservable<unsigned int>*)p);
   }
   static void deleteArray_TQCPDecorationObservablelEunsignedsPintgR(void *p) {
      delete [] ((::TQCPDecorationObservable<unsigned int>*)p);
   }
   static void destruct_TQCPDecorationObservablelEunsignedsPintgR(void *p) {
      typedef ::TQCPDecorationObservable<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCPDecorationObservable<unsigned int>

//______________________________________________________________________________
template <> void TQCPDecorationObservable<unsigned short>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCPDecorationObservable<unsigned short>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCPDecorationObservable<unsigned short>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCPDecorationObservable<unsigned short>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCPDecorationObservablelEunsignedsPshortgR(void *p) {
      return  p ? new(p) ::TQCPDecorationObservable<unsigned short> : new ::TQCPDecorationObservable<unsigned short>;
   }
   static void *newArray_TQCPDecorationObservablelEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQCPDecorationObservable<unsigned short>[nElements] : new ::TQCPDecorationObservable<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCPDecorationObservablelEunsignedsPshortgR(void *p) {
      delete ((::TQCPDecorationObservable<unsigned short>*)p);
   }
   static void deleteArray_TQCPDecorationObservablelEunsignedsPshortgR(void *p) {
      delete [] ((::TQCPDecorationObservable<unsigned short>*)p);
   }
   static void destruct_TQCPDecorationObservablelEunsignedsPshortgR(void *p) {
      typedef ::TQCPDecorationObservable<unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCPDecorationObservable<unsigned short>

//______________________________________________________________________________
template <> void TQCPDecorationObservable<ULong64_t>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCPDecorationObservable<ULong64_t>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCPDecorationObservable<ULong64_t>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCPDecorationObservable<ULong64_t>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCPDecorationObservablelEULong64_tgR(void *p) {
      return  p ? new(p) ::TQCPDecorationObservable<ULong64_t> : new ::TQCPDecorationObservable<ULong64_t>;
   }
   static void *newArray_TQCPDecorationObservablelEULong64_tgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQCPDecorationObservable<ULong64_t>[nElements] : new ::TQCPDecorationObservable<ULong64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCPDecorationObservablelEULong64_tgR(void *p) {
      delete ((::TQCPDecorationObservable<ULong64_t>*)p);
   }
   static void deleteArray_TQCPDecorationObservablelEULong64_tgR(void *p) {
      delete [] ((::TQCPDecorationObservable<ULong64_t>*)p);
   }
   static void destruct_TQCPDecorationObservablelEULong64_tgR(void *p) {
      typedef ::TQCPDecorationObservable<ULong64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCPDecorationObservable<ULong64_t>

//______________________________________________________________________________
void TQCPConstituentVectorObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCPConstituentVectorObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCPConstituentVectorObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCPConstituentVectorObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCPConstituentVectorObservable(void *p) {
      return  p ? new(p) ::TQCPConstituentVectorObservable : new ::TQCPConstituentVectorObservable;
   }
   static void *newArray_TQCPConstituentVectorObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQCPConstituentVectorObservable[nElements] : new ::TQCPConstituentVectorObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCPConstituentVectorObservable(void *p) {
      delete ((::TQCPConstituentVectorObservable*)p);
   }
   static void deleteArray_TQCPConstituentVectorObservable(void *p) {
      delete [] ((::TQCPConstituentVectorObservable*)p);
   }
   static void destruct_TQCPConstituentVectorObservable(void *p) {
      typedef ::TQCPConstituentVectorObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCPConstituentVectorObservable

//______________________________________________________________________________
void TQEventFlaggingAnalysisJob::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQEventFlaggingAnalysisJob.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQEventFlaggingAnalysisJob::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQEventFlaggingAnalysisJob::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQEventFlaggingAnalysisJob(void *p) {
      return  p ? new(p) ::TQEventFlaggingAnalysisJob : new ::TQEventFlaggingAnalysisJob;
   }
   static void *newArray_TQEventFlaggingAnalysisJob(Long_t nElements, void *p) {
      return p ? new(p) ::TQEventFlaggingAnalysisJob[nElements] : new ::TQEventFlaggingAnalysisJob[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQEventFlaggingAnalysisJob(void *p) {
      delete ((::TQEventFlaggingAnalysisJob*)p);
   }
   static void deleteArray_TQEventFlaggingAnalysisJob(void *p) {
      delete [] ((::TQEventFlaggingAnalysisJob*)p);
   }
   static void destruct_TQEventFlaggingAnalysisJob(void *p) {
      typedef ::TQEventFlaggingAnalysisJob current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQEventFlaggingAnalysisJob

//______________________________________________________________________________
template <> void TQIParticleDecorationObservable<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationObservable<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationObservable<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationObservable<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationObservablelEfloatgR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationObservable<float> : new ::TQIParticleDecorationObservable<float>;
   }
   static void *newArray_TQIParticleDecorationObservablelEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationObservable<float>[nElements] : new ::TQIParticleDecorationObservable<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationObservablelEfloatgR(void *p) {
      delete ((::TQIParticleDecorationObservable<float>*)p);
   }
   static void deleteArray_TQIParticleDecorationObservablelEfloatgR(void *p) {
      delete [] ((::TQIParticleDecorationObservable<float>*)p);
   }
   static void destruct_TQIParticleDecorationObservablelEfloatgR(void *p) {
      typedef ::TQIParticleDecorationObservable<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationObservable<float>

//______________________________________________________________________________
template <> void TQIParticleDecorationObservable<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationObservable<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationObservable<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationObservable<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationObservablelEdoublegR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationObservable<double> : new ::TQIParticleDecorationObservable<double>;
   }
   static void *newArray_TQIParticleDecorationObservablelEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationObservable<double>[nElements] : new ::TQIParticleDecorationObservable<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationObservablelEdoublegR(void *p) {
      delete ((::TQIParticleDecorationObservable<double>*)p);
   }
   static void deleteArray_TQIParticleDecorationObservablelEdoublegR(void *p) {
      delete [] ((::TQIParticleDecorationObservable<double>*)p);
   }
   static void destruct_TQIParticleDecorationObservablelEdoublegR(void *p) {
      typedef ::TQIParticleDecorationObservable<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationObservable<double>

//______________________________________________________________________________
template <> void TQIParticleDecorationObservable<char>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationObservable<char>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationObservable<char>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationObservable<char>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationObservablelEchargR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationObservable<char> : new ::TQIParticleDecorationObservable<char>;
   }
   static void *newArray_TQIParticleDecorationObservablelEchargR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationObservable<char>[nElements] : new ::TQIParticleDecorationObservable<char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationObservablelEchargR(void *p) {
      delete ((::TQIParticleDecorationObservable<char>*)p);
   }
   static void deleteArray_TQIParticleDecorationObservablelEchargR(void *p) {
      delete [] ((::TQIParticleDecorationObservable<char>*)p);
   }
   static void destruct_TQIParticleDecorationObservablelEchargR(void *p) {
      typedef ::TQIParticleDecorationObservable<char> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationObservable<char>

//______________________________________________________________________________
template <> void TQIParticleDecorationObservable<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationObservable<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationObservable<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationObservable<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationObservablelEintgR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationObservable<int> : new ::TQIParticleDecorationObservable<int>;
   }
   static void *newArray_TQIParticleDecorationObservablelEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationObservable<int>[nElements] : new ::TQIParticleDecorationObservable<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationObservablelEintgR(void *p) {
      delete ((::TQIParticleDecorationObservable<int>*)p);
   }
   static void deleteArray_TQIParticleDecorationObservablelEintgR(void *p) {
      delete [] ((::TQIParticleDecorationObservable<int>*)p);
   }
   static void destruct_TQIParticleDecorationObservablelEintgR(void *p) {
      typedef ::TQIParticleDecorationObservable<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationObservable<int>

//______________________________________________________________________________
template <> void TQIParticleDecorationObservable<unsigned int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationObservable<unsigned int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationObservable<unsigned int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationObservable<unsigned int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationObservablelEunsignedsPintgR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationObservable<unsigned int> : new ::TQIParticleDecorationObservable<unsigned int>;
   }
   static void *newArray_TQIParticleDecorationObservablelEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationObservable<unsigned int>[nElements] : new ::TQIParticleDecorationObservable<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationObservablelEunsignedsPintgR(void *p) {
      delete ((::TQIParticleDecorationObservable<unsigned int>*)p);
   }
   static void deleteArray_TQIParticleDecorationObservablelEunsignedsPintgR(void *p) {
      delete [] ((::TQIParticleDecorationObservable<unsigned int>*)p);
   }
   static void destruct_TQIParticleDecorationObservablelEunsignedsPintgR(void *p) {
      typedef ::TQIParticleDecorationObservable<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationObservable<unsigned int>

//______________________________________________________________________________
template <> void TQIParticleDecorationObservable<unsigned short>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationObservable<unsigned short>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationObservable<unsigned short>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationObservable<unsigned short>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationObservablelEunsignedsPshortgR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationObservable<unsigned short> : new ::TQIParticleDecorationObservable<unsigned short>;
   }
   static void *newArray_TQIParticleDecorationObservablelEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationObservable<unsigned short>[nElements] : new ::TQIParticleDecorationObservable<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationObservablelEunsignedsPshortgR(void *p) {
      delete ((::TQIParticleDecorationObservable<unsigned short>*)p);
   }
   static void deleteArray_TQIParticleDecorationObservablelEunsignedsPshortgR(void *p) {
      delete [] ((::TQIParticleDecorationObservable<unsigned short>*)p);
   }
   static void destruct_TQIParticleDecorationObservablelEunsignedsPshortgR(void *p) {
      typedef ::TQIParticleDecorationObservable<unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationObservable<unsigned short>

//______________________________________________________________________________
template <> void TQIParticleDecorationObservable<ULong64_t>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationObservable<ULong64_t>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationObservable<ULong64_t>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationObservable<ULong64_t>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationObservablelEULong64_tgR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationObservable<ULong64_t> : new ::TQIParticleDecorationObservable<ULong64_t>;
   }
   static void *newArray_TQIParticleDecorationObservablelEULong64_tgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationObservable<ULong64_t>[nElements] : new ::TQIParticleDecorationObservable<ULong64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationObservablelEULong64_tgR(void *p) {
      delete ((::TQIParticleDecorationObservable<ULong64_t>*)p);
   }
   static void deleteArray_TQIParticleDecorationObservablelEULong64_tgR(void *p) {
      delete [] ((::TQIParticleDecorationObservable<ULong64_t>*)p);
   }
   static void destruct_TQIParticleDecorationObservablelEULong64_tgR(void *p) {
      typedef ::TQIParticleDecorationObservable<ULong64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationObservable<ULong64_t>

//______________________________________________________________________________
template <> void TQIParticleDecorationVectorObservable<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationVectorObservable<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationVectorObservable<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationVectorObservable<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationVectorObservablelEfloatgR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationVectorObservable<float> : new ::TQIParticleDecorationVectorObservable<float>;
   }
   static void *newArray_TQIParticleDecorationVectorObservablelEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationVectorObservable<float>[nElements] : new ::TQIParticleDecorationVectorObservable<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationVectorObservablelEfloatgR(void *p) {
      delete ((::TQIParticleDecorationVectorObservable<float>*)p);
   }
   static void deleteArray_TQIParticleDecorationVectorObservablelEfloatgR(void *p) {
      delete [] ((::TQIParticleDecorationVectorObservable<float>*)p);
   }
   static void destruct_TQIParticleDecorationVectorObservablelEfloatgR(void *p) {
      typedef ::TQIParticleDecorationVectorObservable<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationVectorObservable<float>

//______________________________________________________________________________
template <> void TQIParticleDecorationVectorObservable<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationVectorObservable<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationVectorObservable<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationVectorObservable<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationVectorObservablelEdoublegR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationVectorObservable<double> : new ::TQIParticleDecorationVectorObservable<double>;
   }
   static void *newArray_TQIParticleDecorationVectorObservablelEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationVectorObservable<double>[nElements] : new ::TQIParticleDecorationVectorObservable<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationVectorObservablelEdoublegR(void *p) {
      delete ((::TQIParticleDecorationVectorObservable<double>*)p);
   }
   static void deleteArray_TQIParticleDecorationVectorObservablelEdoublegR(void *p) {
      delete [] ((::TQIParticleDecorationVectorObservable<double>*)p);
   }
   static void destruct_TQIParticleDecorationVectorObservablelEdoublegR(void *p) {
      typedef ::TQIParticleDecorationVectorObservable<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationVectorObservable<double>

//______________________________________________________________________________
template <> void TQIParticleDecorationVectorObservable<char>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationVectorObservable<char>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationVectorObservable<char>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationVectorObservable<char>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationVectorObservablelEchargR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationVectorObservable<char> : new ::TQIParticleDecorationVectorObservable<char>;
   }
   static void *newArray_TQIParticleDecorationVectorObservablelEchargR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationVectorObservable<char>[nElements] : new ::TQIParticleDecorationVectorObservable<char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationVectorObservablelEchargR(void *p) {
      delete ((::TQIParticleDecorationVectorObservable<char>*)p);
   }
   static void deleteArray_TQIParticleDecorationVectorObservablelEchargR(void *p) {
      delete [] ((::TQIParticleDecorationVectorObservable<char>*)p);
   }
   static void destruct_TQIParticleDecorationVectorObservablelEchargR(void *p) {
      typedef ::TQIParticleDecorationVectorObservable<char> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationVectorObservable<char>

//______________________________________________________________________________
template <> void TQIParticleDecorationVectorObservable<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationVectorObservable<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationVectorObservable<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationVectorObservable<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationVectorObservablelEintgR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationVectorObservable<int> : new ::TQIParticleDecorationVectorObservable<int>;
   }
   static void *newArray_TQIParticleDecorationVectorObservablelEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationVectorObservable<int>[nElements] : new ::TQIParticleDecorationVectorObservable<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationVectorObservablelEintgR(void *p) {
      delete ((::TQIParticleDecorationVectorObservable<int>*)p);
   }
   static void deleteArray_TQIParticleDecorationVectorObservablelEintgR(void *p) {
      delete [] ((::TQIParticleDecorationVectorObservable<int>*)p);
   }
   static void destruct_TQIParticleDecorationVectorObservablelEintgR(void *p) {
      typedef ::TQIParticleDecorationVectorObservable<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationVectorObservable<int>

//______________________________________________________________________________
template <> void TQIParticleDecorationVectorObservable<unsigned int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationVectorObservable<unsigned int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationVectorObservable<unsigned int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationVectorObservable<unsigned int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationVectorObservablelEunsignedsPintgR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationVectorObservable<unsigned int> : new ::TQIParticleDecorationVectorObservable<unsigned int>;
   }
   static void *newArray_TQIParticleDecorationVectorObservablelEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationVectorObservable<unsigned int>[nElements] : new ::TQIParticleDecorationVectorObservable<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationVectorObservablelEunsignedsPintgR(void *p) {
      delete ((::TQIParticleDecorationVectorObservable<unsigned int>*)p);
   }
   static void deleteArray_TQIParticleDecorationVectorObservablelEunsignedsPintgR(void *p) {
      delete [] ((::TQIParticleDecorationVectorObservable<unsigned int>*)p);
   }
   static void destruct_TQIParticleDecorationVectorObservablelEunsignedsPintgR(void *p) {
      typedef ::TQIParticleDecorationVectorObservable<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationVectorObservable<unsigned int>

//______________________________________________________________________________
template <> void TQIParticleDecorationVectorObservable<unsigned short>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationVectorObservable<unsigned short>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationVectorObservable<unsigned short>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationVectorObservable<unsigned short>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationVectorObservablelEunsignedsPshortgR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationVectorObservable<unsigned short> : new ::TQIParticleDecorationVectorObservable<unsigned short>;
   }
   static void *newArray_TQIParticleDecorationVectorObservablelEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationVectorObservable<unsigned short>[nElements] : new ::TQIParticleDecorationVectorObservable<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationVectorObservablelEunsignedsPshortgR(void *p) {
      delete ((::TQIParticleDecorationVectorObservable<unsigned short>*)p);
   }
   static void deleteArray_TQIParticleDecorationVectorObservablelEunsignedsPshortgR(void *p) {
      delete [] ((::TQIParticleDecorationVectorObservable<unsigned short>*)p);
   }
   static void destruct_TQIParticleDecorationVectorObservablelEunsignedsPshortgR(void *p) {
      typedef ::TQIParticleDecorationVectorObservable<unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationVectorObservable<unsigned short>

//______________________________________________________________________________
template <> void TQIParticleDecorationVectorObservable<ULong64_t>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDecorationVectorObservable<ULong64_t>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDecorationVectorObservable<ULong64_t>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDecorationVectorObservable<ULong64_t>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDecorationVectorObservablelEULong64_tgR(void *p) {
      return  p ? new(p) ::TQIParticleDecorationVectorObservable<ULong64_t> : new ::TQIParticleDecorationVectorObservable<ULong64_t>;
   }
   static void *newArray_TQIParticleDecorationVectorObservablelEULong64_tgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDecorationVectorObservable<ULong64_t>[nElements] : new ::TQIParticleDecorationVectorObservable<ULong64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDecorationVectorObservablelEULong64_tgR(void *p) {
      delete ((::TQIParticleDecorationVectorObservable<ULong64_t>*)p);
   }
   static void deleteArray_TQIParticleDecorationVectorObservablelEULong64_tgR(void *p) {
      delete [] ((::TQIParticleDecorationVectorObservable<ULong64_t>*)p);
   }
   static void destruct_TQIParticleDecorationVectorObservablelEULong64_tgR(void *p) {
      typedef ::TQIParticleDecorationVectorObservable<ULong64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDecorationVectorObservable<ULong64_t>

//______________________________________________________________________________
void TQIParticleFourVectorObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleFourVectorObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleFourVectorObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleFourVectorObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleFourVectorObservable(void *p) {
      return  p ? new(p) ::TQIParticleFourVectorObservable : new ::TQIParticleFourVectorObservable;
   }
   static void *newArray_TQIParticleFourVectorObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleFourVectorObservable[nElements] : new ::TQIParticleFourVectorObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleFourVectorObservable(void *p) {
      delete ((::TQIParticleFourVectorObservable*)p);
   }
   static void deleteArray_TQIParticleFourVectorObservable(void *p) {
      delete [] ((::TQIParticleFourVectorObservable*)p);
   }
   static void destruct_TQIParticleFourVectorObservable(void *p) {
      typedef ::TQIParticleFourVectorObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleFourVectorObservable

//______________________________________________________________________________
void TQIParticleFourVectorVectorObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleFourVectorVectorObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleFourVectorVectorObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleFourVectorVectorObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleFourVectorVectorObservable(void *p) {
      return  p ? new(p) ::TQIParticleFourVectorVectorObservable : new ::TQIParticleFourVectorVectorObservable;
   }
   static void *newArray_TQIParticleFourVectorVectorObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleFourVectorVectorObservable[nElements] : new ::TQIParticleFourVectorVectorObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleFourVectorVectorObservable(void *p) {
      delete ((::TQIParticleFourVectorVectorObservable*)p);
   }
   static void deleteArray_TQIParticleFourVectorVectorObservable(void *p) {
      delete [] ((::TQIParticleFourVectorVectorObservable*)p);
   }
   static void destruct_TQIParticleFourVectorVectorObservable(void *p) {
      typedef ::TQIParticleFourVectorVectorObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleFourVectorVectorObservable

//______________________________________________________________________________
void TQTruthParticleInfoObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQTruthParticleInfoObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQTruthParticleInfoObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQTruthParticleInfoObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQTruthParticleInfoObservable(void *p) {
      return  p ? new(p) ::TQTruthParticleInfoObservable : new ::TQTruthParticleInfoObservable;
   }
   static void *newArray_TQTruthParticleInfoObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQTruthParticleInfoObservable[nElements] : new ::TQTruthParticleInfoObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQTruthParticleInfoObservable(void *p) {
      delete ((::TQTruthParticleInfoObservable*)p);
   }
   static void deleteArray_TQTruthParticleInfoObservable(void *p) {
      delete [] ((::TQTruthParticleInfoObservable*)p);
   }
   static void destruct_TQTruthParticleInfoObservable(void *p) {
      typedef ::TQTruthParticleInfoObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQTruthParticleInfoObservable

//______________________________________________________________________________
template <> void TQSGAuxElementDecorationObservable<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxElementDecorationObservable<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxElementDecorationObservable<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxElementDecorationObservable<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxElementDecorationObservablelEfloatgR(void *p) {
      return  p ? new(p) ::TQSGAuxElementDecorationObservable<float> : new ::TQSGAuxElementDecorationObservable<float>;
   }
   static void *newArray_TQSGAuxElementDecorationObservablelEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxElementDecorationObservable<float>[nElements] : new ::TQSGAuxElementDecorationObservable<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxElementDecorationObservablelEfloatgR(void *p) {
      delete ((::TQSGAuxElementDecorationObservable<float>*)p);
   }
   static void deleteArray_TQSGAuxElementDecorationObservablelEfloatgR(void *p) {
      delete [] ((::TQSGAuxElementDecorationObservable<float>*)p);
   }
   static void destruct_TQSGAuxElementDecorationObservablelEfloatgR(void *p) {
      typedef ::TQSGAuxElementDecorationObservable<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxElementDecorationObservable<float>

//______________________________________________________________________________
template <> void TQSGAuxElementDecorationObservable<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxElementDecorationObservable<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxElementDecorationObservable<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxElementDecorationObservable<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxElementDecorationObservablelEdoublegR(void *p) {
      return  p ? new(p) ::TQSGAuxElementDecorationObservable<double> : new ::TQSGAuxElementDecorationObservable<double>;
   }
   static void *newArray_TQSGAuxElementDecorationObservablelEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxElementDecorationObservable<double>[nElements] : new ::TQSGAuxElementDecorationObservable<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxElementDecorationObservablelEdoublegR(void *p) {
      delete ((::TQSGAuxElementDecorationObservable<double>*)p);
   }
   static void deleteArray_TQSGAuxElementDecorationObservablelEdoublegR(void *p) {
      delete [] ((::TQSGAuxElementDecorationObservable<double>*)p);
   }
   static void destruct_TQSGAuxElementDecorationObservablelEdoublegR(void *p) {
      typedef ::TQSGAuxElementDecorationObservable<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxElementDecorationObservable<double>

//______________________________________________________________________________
template <> void TQSGAuxElementDecorationObservable<char>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxElementDecorationObservable<char>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxElementDecorationObservable<char>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxElementDecorationObservable<char>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxElementDecorationObservablelEchargR(void *p) {
      return  p ? new(p) ::TQSGAuxElementDecorationObservable<char> : new ::TQSGAuxElementDecorationObservable<char>;
   }
   static void *newArray_TQSGAuxElementDecorationObservablelEchargR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxElementDecorationObservable<char>[nElements] : new ::TQSGAuxElementDecorationObservable<char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxElementDecorationObservablelEchargR(void *p) {
      delete ((::TQSGAuxElementDecorationObservable<char>*)p);
   }
   static void deleteArray_TQSGAuxElementDecorationObservablelEchargR(void *p) {
      delete [] ((::TQSGAuxElementDecorationObservable<char>*)p);
   }
   static void destruct_TQSGAuxElementDecorationObservablelEchargR(void *p) {
      typedef ::TQSGAuxElementDecorationObservable<char> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxElementDecorationObservable<char>

//______________________________________________________________________________
template <> void TQSGAuxElementDecorationObservable<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxElementDecorationObservable<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxElementDecorationObservable<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxElementDecorationObservable<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxElementDecorationObservablelEintgR(void *p) {
      return  p ? new(p) ::TQSGAuxElementDecorationObservable<int> : new ::TQSGAuxElementDecorationObservable<int>;
   }
   static void *newArray_TQSGAuxElementDecorationObservablelEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxElementDecorationObservable<int>[nElements] : new ::TQSGAuxElementDecorationObservable<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxElementDecorationObservablelEintgR(void *p) {
      delete ((::TQSGAuxElementDecorationObservable<int>*)p);
   }
   static void deleteArray_TQSGAuxElementDecorationObservablelEintgR(void *p) {
      delete [] ((::TQSGAuxElementDecorationObservable<int>*)p);
   }
   static void destruct_TQSGAuxElementDecorationObservablelEintgR(void *p) {
      typedef ::TQSGAuxElementDecorationObservable<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxElementDecorationObservable<int>

//______________________________________________________________________________
template <> void TQSGAuxElementDecorationObservable<unsigned int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxElementDecorationObservable<unsigned int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxElementDecorationObservable<unsigned int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxElementDecorationObservable<unsigned int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxElementDecorationObservablelEunsignedsPintgR(void *p) {
      return  p ? new(p) ::TQSGAuxElementDecorationObservable<unsigned int> : new ::TQSGAuxElementDecorationObservable<unsigned int>;
   }
   static void *newArray_TQSGAuxElementDecorationObservablelEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxElementDecorationObservable<unsigned int>[nElements] : new ::TQSGAuxElementDecorationObservable<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxElementDecorationObservablelEunsignedsPintgR(void *p) {
      delete ((::TQSGAuxElementDecorationObservable<unsigned int>*)p);
   }
   static void deleteArray_TQSGAuxElementDecorationObservablelEunsignedsPintgR(void *p) {
      delete [] ((::TQSGAuxElementDecorationObservable<unsigned int>*)p);
   }
   static void destruct_TQSGAuxElementDecorationObservablelEunsignedsPintgR(void *p) {
      typedef ::TQSGAuxElementDecorationObservable<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxElementDecorationObservable<unsigned int>

//______________________________________________________________________________
template <> void TQSGAuxElementDecorationObservable<unsigned short>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxElementDecorationObservable<unsigned short>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxElementDecorationObservable<unsigned short>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxElementDecorationObservable<unsigned short>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxElementDecorationObservablelEunsignedsPshortgR(void *p) {
      return  p ? new(p) ::TQSGAuxElementDecorationObservable<unsigned short> : new ::TQSGAuxElementDecorationObservable<unsigned short>;
   }
   static void *newArray_TQSGAuxElementDecorationObservablelEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxElementDecorationObservable<unsigned short>[nElements] : new ::TQSGAuxElementDecorationObservable<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxElementDecorationObservablelEunsignedsPshortgR(void *p) {
      delete ((::TQSGAuxElementDecorationObservable<unsigned short>*)p);
   }
   static void deleteArray_TQSGAuxElementDecorationObservablelEunsignedsPshortgR(void *p) {
      delete [] ((::TQSGAuxElementDecorationObservable<unsigned short>*)p);
   }
   static void destruct_TQSGAuxElementDecorationObservablelEunsignedsPshortgR(void *p) {
      typedef ::TQSGAuxElementDecorationObservable<unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxElementDecorationObservable<unsigned short>

//______________________________________________________________________________
template <> void TQSGAuxElementDecorationObservable<ULong64_t>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxElementDecorationObservable<ULong64_t>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxElementDecorationObservable<ULong64_t>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxElementDecorationObservable<ULong64_t>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxElementDecorationObservablelEULong64_tgR(void *p) {
      return  p ? new(p) ::TQSGAuxElementDecorationObservable<ULong64_t> : new ::TQSGAuxElementDecorationObservable<ULong64_t>;
   }
   static void *newArray_TQSGAuxElementDecorationObservablelEULong64_tgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxElementDecorationObservable<ULong64_t>[nElements] : new ::TQSGAuxElementDecorationObservable<ULong64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxElementDecorationObservablelEULong64_tgR(void *p) {
      delete ((::TQSGAuxElementDecorationObservable<ULong64_t>*)p);
   }
   static void deleteArray_TQSGAuxElementDecorationObservablelEULong64_tgR(void *p) {
      delete [] ((::TQSGAuxElementDecorationObservable<ULong64_t>*)p);
   }
   static void destruct_TQSGAuxElementDecorationObservablelEULong64_tgR(void *p) {
      typedef ::TQSGAuxElementDecorationObservable<ULong64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxElementDecorationObservable<ULong64_t>

//______________________________________________________________________________
template <> void TQSGAuxFlagCheckingObservable<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxFlagCheckingObservable<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxFlagCheckingObservable<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxFlagCheckingObservable<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxFlagCheckingObservablelEfloatgR(void *p) {
      return  p ? new(p) ::TQSGAuxFlagCheckingObservable<float> : new ::TQSGAuxFlagCheckingObservable<float>;
   }
   static void *newArray_TQSGAuxFlagCheckingObservablelEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxFlagCheckingObservable<float>[nElements] : new ::TQSGAuxFlagCheckingObservable<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxFlagCheckingObservablelEfloatgR(void *p) {
      delete ((::TQSGAuxFlagCheckingObservable<float>*)p);
   }
   static void deleteArray_TQSGAuxFlagCheckingObservablelEfloatgR(void *p) {
      delete [] ((::TQSGAuxFlagCheckingObservable<float>*)p);
   }
   static void destruct_TQSGAuxFlagCheckingObservablelEfloatgR(void *p) {
      typedef ::TQSGAuxFlagCheckingObservable<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxFlagCheckingObservable<float>

//______________________________________________________________________________
template <> void TQSGAuxFlagCheckingObservable<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxFlagCheckingObservable<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxFlagCheckingObservable<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxFlagCheckingObservable<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxFlagCheckingObservablelEdoublegR(void *p) {
      return  p ? new(p) ::TQSGAuxFlagCheckingObservable<double> : new ::TQSGAuxFlagCheckingObservable<double>;
   }
   static void *newArray_TQSGAuxFlagCheckingObservablelEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxFlagCheckingObservable<double>[nElements] : new ::TQSGAuxFlagCheckingObservable<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxFlagCheckingObservablelEdoublegR(void *p) {
      delete ((::TQSGAuxFlagCheckingObservable<double>*)p);
   }
   static void deleteArray_TQSGAuxFlagCheckingObservablelEdoublegR(void *p) {
      delete [] ((::TQSGAuxFlagCheckingObservable<double>*)p);
   }
   static void destruct_TQSGAuxFlagCheckingObservablelEdoublegR(void *p) {
      typedef ::TQSGAuxFlagCheckingObservable<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxFlagCheckingObservable<double>

//______________________________________________________________________________
template <> void TQSGAuxFlagCheckingObservable<char>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxFlagCheckingObservable<char>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxFlagCheckingObservable<char>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxFlagCheckingObservable<char>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxFlagCheckingObservablelEchargR(void *p) {
      return  p ? new(p) ::TQSGAuxFlagCheckingObservable<char> : new ::TQSGAuxFlagCheckingObservable<char>;
   }
   static void *newArray_TQSGAuxFlagCheckingObservablelEchargR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxFlagCheckingObservable<char>[nElements] : new ::TQSGAuxFlagCheckingObservable<char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxFlagCheckingObservablelEchargR(void *p) {
      delete ((::TQSGAuxFlagCheckingObservable<char>*)p);
   }
   static void deleteArray_TQSGAuxFlagCheckingObservablelEchargR(void *p) {
      delete [] ((::TQSGAuxFlagCheckingObservable<char>*)p);
   }
   static void destruct_TQSGAuxFlagCheckingObservablelEchargR(void *p) {
      typedef ::TQSGAuxFlagCheckingObservable<char> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxFlagCheckingObservable<char>

//______________________________________________________________________________
template <> void TQSGAuxFlagCheckingObservable<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxFlagCheckingObservable<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxFlagCheckingObservable<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxFlagCheckingObservable<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxFlagCheckingObservablelEintgR(void *p) {
      return  p ? new(p) ::TQSGAuxFlagCheckingObservable<int> : new ::TQSGAuxFlagCheckingObservable<int>;
   }
   static void *newArray_TQSGAuxFlagCheckingObservablelEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxFlagCheckingObservable<int>[nElements] : new ::TQSGAuxFlagCheckingObservable<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxFlagCheckingObservablelEintgR(void *p) {
      delete ((::TQSGAuxFlagCheckingObservable<int>*)p);
   }
   static void deleteArray_TQSGAuxFlagCheckingObservablelEintgR(void *p) {
      delete [] ((::TQSGAuxFlagCheckingObservable<int>*)p);
   }
   static void destruct_TQSGAuxFlagCheckingObservablelEintgR(void *p) {
      typedef ::TQSGAuxFlagCheckingObservable<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxFlagCheckingObservable<int>

//______________________________________________________________________________
template <> void TQSGAuxFlagCheckingObservable<unsigned int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxFlagCheckingObservable<unsigned int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxFlagCheckingObservable<unsigned int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxFlagCheckingObservable<unsigned int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxFlagCheckingObservablelEunsignedsPintgR(void *p) {
      return  p ? new(p) ::TQSGAuxFlagCheckingObservable<unsigned int> : new ::TQSGAuxFlagCheckingObservable<unsigned int>;
   }
   static void *newArray_TQSGAuxFlagCheckingObservablelEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxFlagCheckingObservable<unsigned int>[nElements] : new ::TQSGAuxFlagCheckingObservable<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxFlagCheckingObservablelEunsignedsPintgR(void *p) {
      delete ((::TQSGAuxFlagCheckingObservable<unsigned int>*)p);
   }
   static void deleteArray_TQSGAuxFlagCheckingObservablelEunsignedsPintgR(void *p) {
      delete [] ((::TQSGAuxFlagCheckingObservable<unsigned int>*)p);
   }
   static void destruct_TQSGAuxFlagCheckingObservablelEunsignedsPintgR(void *p) {
      typedef ::TQSGAuxFlagCheckingObservable<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxFlagCheckingObservable<unsigned int>

//______________________________________________________________________________
template <> void TQSGAuxFlagCheckingObservable<unsigned short>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxFlagCheckingObservable<unsigned short>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxFlagCheckingObservable<unsigned short>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxFlagCheckingObservable<unsigned short>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR(void *p) {
      return  p ? new(p) ::TQSGAuxFlagCheckingObservable<unsigned short> : new ::TQSGAuxFlagCheckingObservable<unsigned short>;
   }
   static void *newArray_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxFlagCheckingObservable<unsigned short>[nElements] : new ::TQSGAuxFlagCheckingObservable<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR(void *p) {
      delete ((::TQSGAuxFlagCheckingObservable<unsigned short>*)p);
   }
   static void deleteArray_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR(void *p) {
      delete [] ((::TQSGAuxFlagCheckingObservable<unsigned short>*)p);
   }
   static void destruct_TQSGAuxFlagCheckingObservablelEunsignedsPshortgR(void *p) {
      typedef ::TQSGAuxFlagCheckingObservable<unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxFlagCheckingObservable<unsigned short>

//______________________________________________________________________________
template <> void TQSGAuxFlagCheckingObservable<ULong64_t>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSGAuxFlagCheckingObservable<ULong64_t>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSGAuxFlagCheckingObservable<ULong64_t>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSGAuxFlagCheckingObservable<ULong64_t>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSGAuxFlagCheckingObservablelEULong64_tgR(void *p) {
      return  p ? new(p) ::TQSGAuxFlagCheckingObservable<ULong64_t> : new ::TQSGAuxFlagCheckingObservable<ULong64_t>;
   }
   static void *newArray_TQSGAuxFlagCheckingObservablelEULong64_tgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQSGAuxFlagCheckingObservable<ULong64_t>[nElements] : new ::TQSGAuxFlagCheckingObservable<ULong64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSGAuxFlagCheckingObservablelEULong64_tgR(void *p) {
      delete ((::TQSGAuxFlagCheckingObservable<ULong64_t>*)p);
   }
   static void deleteArray_TQSGAuxFlagCheckingObservablelEULong64_tgR(void *p) {
      delete [] ((::TQSGAuxFlagCheckingObservable<ULong64_t>*)p);
   }
   static void destruct_TQSGAuxFlagCheckingObservablelEULong64_tgR(void *p) {
      typedef ::TQSGAuxFlagCheckingObservable<ULong64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSGAuxFlagCheckingObservable<ULong64_t>

//______________________________________________________________________________
void TQToolObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQToolObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQToolObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQToolObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQToolObservable(void *p) {
      return  p ? new(p) ::TQToolObservable : new ::TQToolObservable;
   }
   static void *newArray_TQToolObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQToolObservable[nElements] : new ::TQToolObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQToolObservable(void *p) {
      delete ((::TQToolObservable*)p);
   }
   static void deleteArray_TQToolObservable(void *p) {
      delete [] ((::TQToolObservable*)p);
   }
   static void destruct_TQToolObservable(void *p) {
      typedef ::TQToolObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQToolObservable

//______________________________________________________________________________
void TQxAODskimmingAlgorithm::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQxAODskimmingAlgorithm.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQxAODskimmingAlgorithm::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQxAODskimmingAlgorithm::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQxAODskimmingAlgorithm(void *p) {
      return  p ? new(p) ::TQxAODskimmingAlgorithm : new ::TQxAODskimmingAlgorithm;
   }
   static void *newArray_TQxAODskimmingAlgorithm(Long_t nElements, void *p) {
      return p ? new(p) ::TQxAODskimmingAlgorithm[nElements] : new ::TQxAODskimmingAlgorithm[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQxAODskimmingAlgorithm(void *p) {
      delete ((::TQxAODskimmingAlgorithm*)p);
   }
   static void deleteArray_TQxAODskimmingAlgorithm(void *p) {
      delete [] ((::TQxAODskimmingAlgorithm*)p);
   }
   static void destruct_TQxAODskimmingAlgorithm(void *p) {
      typedef ::TQxAODskimmingAlgorithm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQxAODskimmingAlgorithm

namespace {
  void TriggerDictionaryInitialization_libCAFxAODUtils_Impl() {
    static const char* headers[] = {
"CAFxAODUtils/Observables.h",
"CAFxAODUtils/TQCPConstituentVectorObservable.h",
"CAFxAODUtils/TQCPDecorationObservable.h",
"CAFxAODUtils/TQCompositorObservable.h",
"CAFxAODUtils/TQContainerSizeObservable.h",
"CAFxAODUtils/TQEventFlaggingAnalysisJob.h",
"CAFxAODUtils/TQEventObservable.h",
"CAFxAODUtils/TQIParticleDecorationObservable.h",
"CAFxAODUtils/TQIParticleDecorationVectorObservable.h",
"CAFxAODUtils/TQIParticleFourVectorObservable.h",
"CAFxAODUtils/TQIParticleFourVectorVectorObservable.h",
"CAFxAODUtils/TQSGAuxElementDecorationObservable.h",
"CAFxAODUtils/TQSGAuxFlagCheckingObservable.h",
"CAFxAODUtils/TQToolObservable.h",
"CAFxAODUtils/TQTruthParticleInfoObservable.h",
"CAFxAODUtils/TQxAODUtils.h",
"CAFxAODUtils/TQxAODskimmingAlgorithm.h",
0
    };
    static const char* includePaths[] = {
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CAFxAODUtils",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CAFxAODUtils",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
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
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODParticleEvent",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODBase",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEgamma",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/DetectorDescription/GeoPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/EventPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCaloEvent",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Calorimeter/CaloGeoHelpers",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTracking",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTruth",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODJet",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODBTagging",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODMuon",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/MuonSpectrometer/MuonIdHelpers",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTrigger",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODPFlow",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODMissingET",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTau",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEventInfo",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/Interfaces/AsgAnalysisInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/AnalysisCommon/PATInterfaces",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CAFxAODUtils",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CAFxAODUtils",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
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
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODParticleEvent",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODBase",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEgamma",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/DetectorDescription/GeoPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/EventPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCaloEvent",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Calorimeter/CaloGeoHelpers",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTracking",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTruth",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODJet",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODBTagging",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODMuon",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/MuonSpectrometer/MuonIdHelpers",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTrigger",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODPFlow",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODMissingET",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTau",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEventInfo",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/Interfaces/AsgAnalysisInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/AnalysisCommon/PATInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/QFramework",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCutFlow",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/CAFxAODUtils/CMakeFiles/makeCAFxAODUtilsCintDict.cyjiks/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCAFxAODUtils dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQEventObservable.h")))  __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQCompositorObservable.h")))  TQEventObservable;
class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQCompositorObservable.h")))  TQCompositorObservable;
class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQContainerSizeObservable.h")))  TQContainerSizeObservable;
template <class T> class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQCPDecorationObservable.h")))  TQCPDecorationObservable;

class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQCPConstituentVectorObservable.h")))  TQCPConstituentVectorObservable;
class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQEventFlaggingAnalysisJob.h")))  TQEventFlaggingAnalysisJob;
template <class Taux> class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQIParticleDecorationObservable.h")))  TQIParticleDecorationObservable;

template <class Taux> class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQIParticleDecorationVectorObservable.h")))  TQIParticleDecorationVectorObservable;

class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQIParticleFourVectorObservable.h")))  TQIParticleFourVectorObservable;
class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQIParticleFourVectorVectorObservable.h")))  TQIParticleFourVectorVectorObservable;
class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQTruthParticleInfoObservable.h")))  TQTruthParticleInfoObservable;
template <class Taux> class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQSGAuxElementDecorationObservable.h")))  TQSGAuxElementDecorationObservable;

template <class Taux> class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQSGAuxFlagCheckingObservable.h")))  TQSGAuxFlagCheckingObservable;

class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQToolObservable.h")))  TQToolObservable;
class __attribute__((annotate("$clingAutoload$CAFxAODUtils/TQxAODskimmingAlgorithm.h")))  TQxAODskimmingAlgorithm;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCAFxAODUtils dictionary payload"

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
  #define PACKAGE_VERSION "CAFxAODUtils-00-00-00"
#endif
#ifndef PACKAGE_VERSION_UQ
  #define PACKAGE_VERSION_UQ CAFxAODUtils-00-00-00
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CAFxAODUtils/Observables.h"
#include "CAFxAODUtils/TQCPConstituentVectorObservable.h"
#include "CAFxAODUtils/TQCPDecorationObservable.h"
#include "CAFxAODUtils/TQCompositorObservable.h"
#include "CAFxAODUtils/TQContainerSizeObservable.h"
#include "CAFxAODUtils/TQEventFlaggingAnalysisJob.h"
#include "CAFxAODUtils/TQEventObservable.h"
#include "CAFxAODUtils/TQIParticleDecorationObservable.h"
#include "CAFxAODUtils/TQIParticleDecorationVectorObservable.h"
#include "CAFxAODUtils/TQIParticleFourVectorObservable.h"
#include "CAFxAODUtils/TQIParticleFourVectorVectorObservable.h"
#include "CAFxAODUtils/TQSGAuxElementDecorationObservable.h"
#include "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h"
#include "CAFxAODUtils/TQToolObservable.h"
#include "CAFxAODUtils/TQTruthParticleInfoObservable.h"
#include "CAFxAODUtils/TQxAODUtils.h"
#include "CAFxAODUtils/TQxAODskimmingAlgorithm.h"
//this is an automatically generated -*- c++ -*- file - EDITS WILL BE LOST!
#ifndef __CAFxAODUtilsDICT__
#define __CAFxAODUtilsDICT__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winconsistent-missing-override"
#include "CAFxAODUtils/Observables.h"
#include "CAFxAODUtils/TQCompositorObservable.h"
#include "CAFxAODUtils/TQContainerSizeObservable.h"
#include "CAFxAODUtils/TQCPDecorationObservable.h"
#include "CAFxAODUtils/TQCPConstituentVectorObservable.h"
#include "CAFxAODUtils/TQEventFlaggingAnalysisJob.h"
#include "CAFxAODUtils/TQEventObservable.h"
#include "CAFxAODUtils/TQIParticleDecorationObservable.h"
#include "CAFxAODUtils/TQIParticleDecorationVectorObservable.h"
#include "CAFxAODUtils/TQIParticleFourVectorObservable.h"
#include "CAFxAODUtils/TQIParticleFourVectorVectorObservable.h"
#include "CAFxAODUtils/TQTruthParticleInfoObservable.h"
#include "CAFxAODUtils/TQSGAuxElementDecorationObservable.h"
#include "CAFxAODUtils/TQSGAuxFlagCheckingObservable.h"
#include "CAFxAODUtils/TQToolObservable.h"
#include "CAFxAODUtils/TQxAODskimmingAlgorithm.h"
#include "CAFxAODUtils/TQxAODUtils.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;

#pragma link C++ class TQCompositorObservable+;
#pragma link C++ class TQContainerSizeObservable+;
#pragma link C++ class TQEventFlaggingAnalysisJob+;
#pragma link C++ class TQEventObservable+;
#pragma link C++ class TQIParticleFourVectorObservable+;
#pragma link C++ class TQIParticleFourVectorVectorObservable+;
#pragma link C++ class TQTruthParticleInfoObservable+;
#pragma link C++ class TQToolObservable+;
#pragma link C++ class TQxAODskimmingAlgorithm+;
#pragma link C++ class TQCPConstituentVectorObservable+;
#pragma link C++ class TQCPDecorationObservable <float>+;
#pragma link C++ class TQCPDecorationObservable <double>+;
#pragma link C++ class TQCPDecorationObservable <char>+;
#pragma link C++ class TQCPDecorationObservable <unsigned int>+;
#pragma link C++ class TQCPDecorationObservable <unsigned short>+;
#pragma link C++ class TQCPDecorationObservable <unsigned long long>+;
#pragma link C++ class TQCPDecorationObservable <int>+;
#pragma link C++ class TQIParticleDecorationObservable <float>+;
#pragma link C++ class TQIParticleDecorationObservable <double>+;
#pragma link C++ class TQIParticleDecorationObservable <char>+;
#pragma link C++ class TQIParticleDecorationObservable <unsigned int>+;
#pragma link C++ class TQIParticleDecorationObservable <unsigned short>+;
#pragma link C++ class TQIParticleDecorationObservable <unsigned long long>+;
#pragma link C++ class TQIParticleDecorationObservable <int>+;
#pragma link C++ class TQIParticleDecorationVectorObservable <float>+;
#pragma link C++ class TQIParticleDecorationVectorObservable <double>+;
#pragma link C++ class TQIParticleDecorationVectorObservable <char>+;
#pragma link C++ class TQIParticleDecorationVectorObservable <unsigned int>+;
#pragma link C++ class TQIParticleDecorationVectorObservable <unsigned short>+;
#pragma link C++ class TQIParticleDecorationVectorObservable <unsigned long long>+;
#pragma link C++ class TQIParticleDecorationVectorObservable <int>+;
#pragma link C++ class TQSGAuxElementDecorationObservable <float>+;
#pragma link C++ class TQSGAuxElementDecorationObservable <double>+;
#pragma link C++ class TQSGAuxElementDecorationObservable <char>+;
#pragma link C++ class TQSGAuxElementDecorationObservable <unsigned int>+;
#pragma link C++ class TQSGAuxElementDecorationObservable <unsigned short>+;
#pragma link C++ class TQSGAuxElementDecorationObservable <unsigned long long>+;
#pragma link C++ class TQSGAuxElementDecorationObservable <int>+;
#pragma link C++ class TQSGAuxFlagCheckingObservable <float>+;
#pragma link C++ class TQSGAuxFlagCheckingObservable <double>+;
#pragma link C++ class TQSGAuxFlagCheckingObservable <char>+;
#pragma link C++ class TQSGAuxFlagCheckingObservable <unsigned int>+;
#pragma link C++ class TQSGAuxFlagCheckingObservable <unsigned short>+;
#pragma link C++ class TQSGAuxFlagCheckingObservable <unsigned long long>+;
#pragma link C++ class TQSGAuxFlagCheckingObservable <int>+;
#pragma link C++ namespace TQObservableFactory;
#pragma link C++ namespace TQxAODUtils;
#pragma link C++ namespace asg;

#endif
#endif


#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TQCPConstituentVectorObservable", payloadCode, "@",
"TQCPDecorationObservable<ULong64_t>", payloadCode, "@",
"TQCPDecorationObservable<char>", payloadCode, "@",
"TQCPDecorationObservable<double>", payloadCode, "@",
"TQCPDecorationObservable<float>", payloadCode, "@",
"TQCPDecorationObservable<int>", payloadCode, "@",
"TQCPDecorationObservable<unsigned int>", payloadCode, "@",
"TQCPDecorationObservable<unsigned short>", payloadCode, "@",
"TQCompositorObservable", payloadCode, "@",
"TQContainerSizeObservable", payloadCode, "@",
"TQEventFlaggingAnalysisJob", payloadCode, "@",
"TQEventObservable", payloadCode, "@",
"TQIParticleDecorationObservable<ULong64_t>", payloadCode, "@",
"TQIParticleDecorationObservable<char>", payloadCode, "@",
"TQIParticleDecorationObservable<double>", payloadCode, "@",
"TQIParticleDecorationObservable<float>", payloadCode, "@",
"TQIParticleDecorationObservable<int>", payloadCode, "@",
"TQIParticleDecorationObservable<unsigned int>", payloadCode, "@",
"TQIParticleDecorationObservable<unsigned short>", payloadCode, "@",
"TQIParticleDecorationVectorObservable<ULong64_t>", payloadCode, "@",
"TQIParticleDecorationVectorObservable<char>", payloadCode, "@",
"TQIParticleDecorationVectorObservable<double>", payloadCode, "@",
"TQIParticleDecorationVectorObservable<float>", payloadCode, "@",
"TQIParticleDecorationVectorObservable<int>", payloadCode, "@",
"TQIParticleDecorationVectorObservable<unsigned int>", payloadCode, "@",
"TQIParticleDecorationVectorObservable<unsigned short>", payloadCode, "@",
"TQIParticleFourVectorObservable", payloadCode, "@",
"TQIParticleFourVectorVectorObservable", payloadCode, "@",
"TQSGAuxElementDecorationObservable<ULong64_t>", payloadCode, "@",
"TQSGAuxElementDecorationObservable<char>", payloadCode, "@",
"TQSGAuxElementDecorationObservable<double>", payloadCode, "@",
"TQSGAuxElementDecorationObservable<float>", payloadCode, "@",
"TQSGAuxElementDecorationObservable<int>", payloadCode, "@",
"TQSGAuxElementDecorationObservable<unsigned int>", payloadCode, "@",
"TQSGAuxElementDecorationObservable<unsigned short>", payloadCode, "@",
"TQSGAuxFlagCheckingObservable<ULong64_t>", payloadCode, "@",
"TQSGAuxFlagCheckingObservable<char>", payloadCode, "@",
"TQSGAuxFlagCheckingObservable<double>", payloadCode, "@",
"TQSGAuxFlagCheckingObservable<float>", payloadCode, "@",
"TQSGAuxFlagCheckingObservable<int>", payloadCode, "@",
"TQSGAuxFlagCheckingObservable<unsigned int>", payloadCode, "@",
"TQSGAuxFlagCheckingObservable<unsigned short>", payloadCode, "@",
"TQToolObservable", payloadCode, "@",
"TQTruthParticleInfoObservable", payloadCode, "@",
"TQxAODskimmingAlgorithm", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCAFxAODUtils",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCAFxAODUtils_Impl, {{"namespace DataVector_detail { template <typename T> class DVLEltBaseInit; }", 1},{"namespace DataVector_detail { template <typename B1, typename B2, typename B3> class VirtBases; }", 1},{"template <typename T> class DataVectorBase;", 1},{"template <typename T, typename BASE> class DataVector;", 1}}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCAFxAODUtils_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCAFxAODUtils() {
  TriggerDictionaryInitialization_libCAFxAODUtils_Impl();
}
