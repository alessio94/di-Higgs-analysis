// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME CAFbbllCintDict
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
#include "CAFbbll/BBLLBJetCalibStudyAlgorithm.h"
#include "CAFbbll/BBLLBTaggingStudyAlgorithm.h"
#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFbbll/BBLLObjectSelectionAlgorithm.h"
#include "CAFbbll/BTagSF.h"
#include "CAFbbll/CodeTTBarDecay.h"
#include "CAFbbll/CombinedQuantities.h"
#include "CAFbbll/DDFakeWeight.h"
#include "CAFbbll/FilteredOutputNode.h"
#include "CAFbbll/JvtSF.h"
#include "CAFbbll/LeptonFakeType.h"
#include "CAFbbll/LeptonSF.h"
#include "CAFbbll/MCWeight.h"
#include "CAFbbll/METObservable.h"
#include "CAFbbll/MultiParticleObservable.h"
#include "CAFbbll/MultiTruthParticleObservable.h"
#include "CAFbbll/NTruthLeptons.h"
#include "CAFbbll/NWTauDecays.h"
#include "CAFbbll/Observables.h"
#include "CAFbbll/PNN_KL.h"
#include "CAFbbll/PUWeight.h"
#include "CAFbbll/RegionID.h"
#include "CAFbbll/SumOfWeight.h"
#include "CAFbbll/TQIParticleDefaultDecorationObservable.h"
#include "CAFbbll/TTVASF.h"
#include "CAFbbll/TTbarNNLOWeight.h"
#include "CAFbbll/TriggerMatching.h"
#include "CAFbbll/TriggerSF.h"
#include "CAFbbll/TruthHiggsDecayInfo.h"
#include "CAFbbll/TruthMhh.h"
#include "CAFbbll/TruthPthh.h"
#include "CAFbbll/TwoNeutrinoScan.h"
#include "CAFbbll/BBLLBJetCalibStudyAlgorithm.h"
#include "CAFbbll/BBLLBTaggingStudyAlgorithm.h"
#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFbbll/BBLLObjectSelectionAlgorithm.h"
#include "CAFbbll/BTagSF.h"
#include "CAFbbll/CodeTTBarDecay.h"
#include "CAFbbll/CombinedQuantities.h"
#include "CAFbbll/DDFakeWeight.h"
#include "CAFbbll/FilteredOutputNode.h"
#include "CAFbbll/JvtSF.h"
#include "CAFbbll/LeptonFakeType.h"
#include "CAFbbll/LeptonSF.h"
#include "CAFbbll/MCWeight.h"
#include "CAFbbll/METObservable.h"
#include "CAFbbll/MultiParticleObservable.h"
#include "CAFbbll/MultiTruthParticleObservable.h"
#include "CAFbbll/NTruthLeptons.h"
#include "CAFbbll/NWTauDecays.h"
#include "CAFbbll/Observables.h"
#include "CAFbbll/PNN_KL.h"
#include "CAFbbll/PUWeight.h"
#include "CAFbbll/RegionID.h"
#include "CAFbbll/SumOfWeight.h"
#include "CAFbbll/TQIParticleDefaultDecorationObservable.h"
#include "CAFbbll/TTVASF.h"
#include "CAFbbll/TTbarNNLOWeight.h"
#include "CAFbbll/TriggerMatching.h"
#include "CAFbbll/TriggerSF.h"
#include "CAFbbll/TruthHiggsDecayInfo.h"
#include "CAFbbll/TruthMhh.h"
#include "CAFbbll/TruthPthh.h"
#include "CAFbbll/TwoNeutrinoScan.h"

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

namespace lwt {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *lwt_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("lwt", 0 /*version*/, "CAFbbll/PNN_KL.h", 16,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &lwt_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *lwt_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static TClass *BBLLContainerRetrieval_Dictionary();
   static void BBLLContainerRetrieval_TClassManip(TClass*);
   static void *new_BBLLContainerRetrieval(void *p = 0);
   static void *newArray_BBLLContainerRetrieval(Long_t size, void *p);
   static void delete_BBLLContainerRetrieval(void *p);
   static void deleteArray_BBLLContainerRetrieval(void *p);
   static void destruct_BBLLContainerRetrieval(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BBLLContainerRetrieval*)
   {
      ::BBLLContainerRetrieval *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::BBLLContainerRetrieval));
      static ::ROOT::TGenericClassInfo 
         instance("BBLLContainerRetrieval", "CAFbbll/BBLLContainerRetrieval.h", 16,
                  typeid(::BBLLContainerRetrieval), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &BBLLContainerRetrieval_Dictionary, isa_proxy, 4,
                  sizeof(::BBLLContainerRetrieval) );
      instance.SetNew(&new_BBLLContainerRetrieval);
      instance.SetNewArray(&newArray_BBLLContainerRetrieval);
      instance.SetDelete(&delete_BBLLContainerRetrieval);
      instance.SetDeleteArray(&deleteArray_BBLLContainerRetrieval);
      instance.SetDestructor(&destruct_BBLLContainerRetrieval);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BBLLContainerRetrieval*)
   {
      return GenerateInitInstanceLocal((::BBLLContainerRetrieval*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::BBLLContainerRetrieval*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *BBLLContainerRetrieval_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::BBLLContainerRetrieval*)0x0)->GetClass();
      BBLLContainerRetrieval_TClassManip(theClass);
   return theClass;
   }

   static void BBLLContainerRetrieval_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_BBLLObjectSelectionAlgorithm(void *p = 0);
   static void *newArray_BBLLObjectSelectionAlgorithm(Long_t size, void *p);
   static void delete_BBLLObjectSelectionAlgorithm(void *p);
   static void deleteArray_BBLLObjectSelectionAlgorithm(void *p);
   static void destruct_BBLLObjectSelectionAlgorithm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BBLLObjectSelectionAlgorithm*)
   {
      ::BBLLObjectSelectionAlgorithm *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::BBLLObjectSelectionAlgorithm >(0);
      static ::ROOT::TGenericClassInfo 
         instance("BBLLObjectSelectionAlgorithm", ::BBLLObjectSelectionAlgorithm::Class_Version(), "CAFbbll/BBLLObjectSelectionAlgorithm.h", 28,
                  typeid(::BBLLObjectSelectionAlgorithm), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::BBLLObjectSelectionAlgorithm::Dictionary, isa_proxy, 4,
                  sizeof(::BBLLObjectSelectionAlgorithm) );
      instance.SetNew(&new_BBLLObjectSelectionAlgorithm);
      instance.SetNewArray(&newArray_BBLLObjectSelectionAlgorithm);
      instance.SetDelete(&delete_BBLLObjectSelectionAlgorithm);
      instance.SetDeleteArray(&deleteArray_BBLLObjectSelectionAlgorithm);
      instance.SetDestructor(&destruct_BBLLObjectSelectionAlgorithm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BBLLObjectSelectionAlgorithm*)
   {
      return GenerateInitInstanceLocal((::BBLLObjectSelectionAlgorithm*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::BBLLObjectSelectionAlgorithm*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_BBLLBJetCalibStudyAlgorithm(void *p = 0);
   static void *newArray_BBLLBJetCalibStudyAlgorithm(Long_t size, void *p);
   static void delete_BBLLBJetCalibStudyAlgorithm(void *p);
   static void deleteArray_BBLLBJetCalibStudyAlgorithm(void *p);
   static void destruct_BBLLBJetCalibStudyAlgorithm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BBLLBJetCalibStudyAlgorithm*)
   {
      ::BBLLBJetCalibStudyAlgorithm *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::BBLLBJetCalibStudyAlgorithm >(0);
      static ::ROOT::TGenericClassInfo 
         instance("BBLLBJetCalibStudyAlgorithm", ::BBLLBJetCalibStudyAlgorithm::Class_Version(), "CAFbbll/BBLLBJetCalibStudyAlgorithm.h", 6,
                  typeid(::BBLLBJetCalibStudyAlgorithm), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::BBLLBJetCalibStudyAlgorithm::Dictionary, isa_proxy, 4,
                  sizeof(::BBLLBJetCalibStudyAlgorithm) );
      instance.SetNew(&new_BBLLBJetCalibStudyAlgorithm);
      instance.SetNewArray(&newArray_BBLLBJetCalibStudyAlgorithm);
      instance.SetDelete(&delete_BBLLBJetCalibStudyAlgorithm);
      instance.SetDeleteArray(&deleteArray_BBLLBJetCalibStudyAlgorithm);
      instance.SetDestructor(&destruct_BBLLBJetCalibStudyAlgorithm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BBLLBJetCalibStudyAlgorithm*)
   {
      return GenerateInitInstanceLocal((::BBLLBJetCalibStudyAlgorithm*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::BBLLBJetCalibStudyAlgorithm*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_BBLLBTaggingStudyAlgorithm(void *p = 0);
   static void *newArray_BBLLBTaggingStudyAlgorithm(Long_t size, void *p);
   static void delete_BBLLBTaggingStudyAlgorithm(void *p);
   static void deleteArray_BBLLBTaggingStudyAlgorithm(void *p);
   static void destruct_BBLLBTaggingStudyAlgorithm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BBLLBTaggingStudyAlgorithm*)
   {
      ::BBLLBTaggingStudyAlgorithm *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::BBLLBTaggingStudyAlgorithm >(0);
      static ::ROOT::TGenericClassInfo 
         instance("BBLLBTaggingStudyAlgorithm", ::BBLLBTaggingStudyAlgorithm::Class_Version(), "CAFbbll/BBLLBTaggingStudyAlgorithm.h", 6,
                  typeid(::BBLLBTaggingStudyAlgorithm), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::BBLLBTaggingStudyAlgorithm::Dictionary, isa_proxy, 4,
                  sizeof(::BBLLBTaggingStudyAlgorithm) );
      instance.SetNew(&new_BBLLBTaggingStudyAlgorithm);
      instance.SetNewArray(&newArray_BBLLBTaggingStudyAlgorithm);
      instance.SetDelete(&delete_BBLLBTaggingStudyAlgorithm);
      instance.SetDeleteArray(&deleteArray_BBLLBTaggingStudyAlgorithm);
      instance.SetDestructor(&destruct_BBLLBTaggingStudyAlgorithm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BBLLBTaggingStudyAlgorithm*)
   {
      return GenerateInitInstanceLocal((::BBLLBTaggingStudyAlgorithm*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::BBLLBTaggingStudyAlgorithm*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_BTagSF(void *p = 0);
   static void *newArray_BTagSF(Long_t size, void *p);
   static void delete_BTagSF(void *p);
   static void deleteArray_BTagSF(void *p);
   static void destruct_BTagSF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BTagSF*)
   {
      ::BTagSF *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::BTagSF >(0);
      static ::ROOT::TGenericClassInfo 
         instance("BTagSF", ::BTagSF::Class_Version(), "CAFbbll/BTagSF.h", 10,
                  typeid(::BTagSF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::BTagSF::Dictionary, isa_proxy, 4,
                  sizeof(::BTagSF) );
      instance.SetNew(&new_BTagSF);
      instance.SetNewArray(&newArray_BTagSF);
      instance.SetDelete(&delete_BTagSF);
      instance.SetDeleteArray(&deleteArray_BTagSF);
      instance.SetDestructor(&destruct_BTagSF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BTagSF*)
   {
      return GenerateInitInstanceLocal((::BTagSF*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::BTagSF*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CodeTTBarDecay(void *p = 0);
   static void *newArray_CodeTTBarDecay(Long_t size, void *p);
   static void delete_CodeTTBarDecay(void *p);
   static void deleteArray_CodeTTBarDecay(void *p);
   static void destruct_CodeTTBarDecay(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CodeTTBarDecay*)
   {
      ::CodeTTBarDecay *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CodeTTBarDecay >(0);
      static ::ROOT::TGenericClassInfo 
         instance("CodeTTBarDecay", ::CodeTTBarDecay::Class_Version(), "CAFbbll/CodeTTBarDecay.h", 7,
                  typeid(::CodeTTBarDecay), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CodeTTBarDecay::Dictionary, isa_proxy, 4,
                  sizeof(::CodeTTBarDecay) );
      instance.SetNew(&new_CodeTTBarDecay);
      instance.SetNewArray(&newArray_CodeTTBarDecay);
      instance.SetDelete(&delete_CodeTTBarDecay);
      instance.SetDeleteArray(&deleteArray_CodeTTBarDecay);
      instance.SetDestructor(&destruct_CodeTTBarDecay);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CodeTTBarDecay*)
   {
      return GenerateInitInstanceLocal((::CodeTTBarDecay*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CodeTTBarDecay*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CombinedQuantities(void *p = 0);
   static void *newArray_CombinedQuantities(Long_t size, void *p);
   static void delete_CombinedQuantities(void *p);
   static void deleteArray_CombinedQuantities(void *p);
   static void destruct_CombinedQuantities(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CombinedQuantities*)
   {
      ::CombinedQuantities *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CombinedQuantities >(0);
      static ::ROOT::TGenericClassInfo 
         instance("CombinedQuantities", ::CombinedQuantities::Class_Version(), "CAFbbll/CombinedQuantities.h", 7,
                  typeid(::CombinedQuantities), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CombinedQuantities::Dictionary, isa_proxy, 4,
                  sizeof(::CombinedQuantities) );
      instance.SetNew(&new_CombinedQuantities);
      instance.SetNewArray(&newArray_CombinedQuantities);
      instance.SetDelete(&delete_CombinedQuantities);
      instance.SetDeleteArray(&deleteArray_CombinedQuantities);
      instance.SetDestructor(&destruct_CombinedQuantities);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CombinedQuantities*)
   {
      return GenerateInitInstanceLocal((::CombinedQuantities*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CombinedQuantities*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DDFakeWeight(void *p = 0);
   static void *newArray_DDFakeWeight(Long_t size, void *p);
   static void delete_DDFakeWeight(void *p);
   static void deleteArray_DDFakeWeight(void *p);
   static void destruct_DDFakeWeight(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DDFakeWeight*)
   {
      ::DDFakeWeight *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DDFakeWeight >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DDFakeWeight", ::DDFakeWeight::Class_Version(), "CAFbbll/DDFakeWeight.h", 50,
                  typeid(::DDFakeWeight), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DDFakeWeight::Dictionary, isa_proxy, 4,
                  sizeof(::DDFakeWeight) );
      instance.SetNew(&new_DDFakeWeight);
      instance.SetNewArray(&newArray_DDFakeWeight);
      instance.SetDelete(&delete_DDFakeWeight);
      instance.SetDeleteArray(&deleteArray_DDFakeWeight);
      instance.SetDestructor(&destruct_DDFakeWeight);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DDFakeWeight*)
   {
      return GenerateInitInstanceLocal((::DDFakeWeight*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DDFakeWeight*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_FilteredOutputNode(void *p = 0);
   static void *newArray_FilteredOutputNode(Long_t size, void *p);
   static void delete_FilteredOutputNode(void *p);
   static void deleteArray_FilteredOutputNode(void *p);
   static void destruct_FilteredOutputNode(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::FilteredOutputNode*)
   {
      ::FilteredOutputNode *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::FilteredOutputNode >(0);
      static ::ROOT::TGenericClassInfo 
         instance("FilteredOutputNode", ::FilteredOutputNode::Class_Version(), "CAFbbll/FilteredOutputNode.h", 10,
                  typeid(::FilteredOutputNode), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::FilteredOutputNode::Dictionary, isa_proxy, 4,
                  sizeof(::FilteredOutputNode) );
      instance.SetNew(&new_FilteredOutputNode);
      instance.SetNewArray(&newArray_FilteredOutputNode);
      instance.SetDelete(&delete_FilteredOutputNode);
      instance.SetDeleteArray(&deleteArray_FilteredOutputNode);
      instance.SetDestructor(&destruct_FilteredOutputNode);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::FilteredOutputNode*)
   {
      return GenerateInitInstanceLocal((::FilteredOutputNode*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::FilteredOutputNode*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_JvtSF(void *p = 0);
   static void *newArray_JvtSF(Long_t size, void *p);
   static void delete_JvtSF(void *p);
   static void deleteArray_JvtSF(void *p);
   static void destruct_JvtSF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JvtSF*)
   {
      ::JvtSF *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JvtSF >(0);
      static ::ROOT::TGenericClassInfo 
         instance("JvtSF", ::JvtSF::Class_Version(), "CAFbbll/JvtSF.h", 7,
                  typeid(::JvtSF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JvtSF::Dictionary, isa_proxy, 4,
                  sizeof(::JvtSF) );
      instance.SetNew(&new_JvtSF);
      instance.SetNewArray(&newArray_JvtSF);
      instance.SetDelete(&delete_JvtSF);
      instance.SetDeleteArray(&deleteArray_JvtSF);
      instance.SetDestructor(&destruct_JvtSF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JvtSF*)
   {
      return GenerateInitInstanceLocal((::JvtSF*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JvtSF*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_LeptonFakeType(void *p = 0);
   static void *newArray_LeptonFakeType(Long_t size, void *p);
   static void delete_LeptonFakeType(void *p);
   static void deleteArray_LeptonFakeType(void *p);
   static void destruct_LeptonFakeType(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LeptonFakeType*)
   {
      ::LeptonFakeType *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LeptonFakeType >(0);
      static ::ROOT::TGenericClassInfo 
         instance("LeptonFakeType", ::LeptonFakeType::Class_Version(), "CAFbbll/LeptonFakeType.h", 9,
                  typeid(::LeptonFakeType), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LeptonFakeType::Dictionary, isa_proxy, 4,
                  sizeof(::LeptonFakeType) );
      instance.SetNew(&new_LeptonFakeType);
      instance.SetNewArray(&newArray_LeptonFakeType);
      instance.SetDelete(&delete_LeptonFakeType);
      instance.SetDeleteArray(&deleteArray_LeptonFakeType);
      instance.SetDestructor(&destruct_LeptonFakeType);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LeptonFakeType*)
   {
      return GenerateInitInstanceLocal((::LeptonFakeType*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LeptonFakeType*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_LeptonSF(void *p = 0);
   static void *newArray_LeptonSF(Long_t size, void *p);
   static void delete_LeptonSF(void *p);
   static void deleteArray_LeptonSF(void *p);
   static void destruct_LeptonSF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LeptonSF*)
   {
      ::LeptonSF *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LeptonSF >(0);
      static ::ROOT::TGenericClassInfo 
         instance("LeptonSF", ::LeptonSF::Class_Version(), "CAFbbll/LeptonSF.h", 9,
                  typeid(::LeptonSF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LeptonSF::Dictionary, isa_proxy, 4,
                  sizeof(::LeptonSF) );
      instance.SetNew(&new_LeptonSF);
      instance.SetNewArray(&newArray_LeptonSF);
      instance.SetDelete(&delete_LeptonSF);
      instance.SetDeleteArray(&deleteArray_LeptonSF);
      instance.SetDestructor(&destruct_LeptonSF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LeptonSF*)
   {
      return GenerateInitInstanceLocal((::LeptonSF*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LeptonSF*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MCWeight(void *p = 0);
   static void *newArray_MCWeight(Long_t size, void *p);
   static void delete_MCWeight(void *p);
   static void deleteArray_MCWeight(void *p);
   static void destruct_MCWeight(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MCWeight*)
   {
      ::MCWeight *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MCWeight >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MCWeight", ::MCWeight::Class_Version(), "CAFbbll/MCWeight.h", 7,
                  typeid(::MCWeight), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MCWeight::Dictionary, isa_proxy, 4,
                  sizeof(::MCWeight) );
      instance.SetNew(&new_MCWeight);
      instance.SetNewArray(&newArray_MCWeight);
      instance.SetDelete(&delete_MCWeight);
      instance.SetDeleteArray(&deleteArray_MCWeight);
      instance.SetDestructor(&destruct_MCWeight);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MCWeight*)
   {
      return GenerateInitInstanceLocal((::MCWeight*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::MCWeight*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_METObservable(void *p = 0);
   static void *newArray_METObservable(Long_t size, void *p);
   static void delete_METObservable(void *p);
   static void deleteArray_METObservable(void *p);
   static void destruct_METObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::METObservable*)
   {
      ::METObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::METObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("METObservable", ::METObservable::Class_Version(), "CAFbbll/METObservable.h", 7,
                  typeid(::METObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::METObservable::Dictionary, isa_proxy, 4,
                  sizeof(::METObservable) );
      instance.SetNew(&new_METObservable);
      instance.SetNewArray(&newArray_METObservable);
      instance.SetDelete(&delete_METObservable);
      instance.SetDeleteArray(&deleteArray_METObservable);
      instance.SetDestructor(&destruct_METObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::METObservable*)
   {
      return GenerateInitInstanceLocal((::METObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::METObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MultiParticleObservable(void *p = 0);
   static void *newArray_MultiParticleObservable(Long_t size, void *p);
   static void delete_MultiParticleObservable(void *p);
   static void deleteArray_MultiParticleObservable(void *p);
   static void destruct_MultiParticleObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MultiParticleObservable*)
   {
      ::MultiParticleObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MultiParticleObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MultiParticleObservable", ::MultiParticleObservable::Class_Version(), "CAFbbll/MultiParticleObservable.h", 15,
                  typeid(::MultiParticleObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MultiParticleObservable::Dictionary, isa_proxy, 4,
                  sizeof(::MultiParticleObservable) );
      instance.SetNew(&new_MultiParticleObservable);
      instance.SetNewArray(&newArray_MultiParticleObservable);
      instance.SetDelete(&delete_MultiParticleObservable);
      instance.SetDeleteArray(&deleteArray_MultiParticleObservable);
      instance.SetDestructor(&destruct_MultiParticleObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MultiParticleObservable*)
   {
      return GenerateInitInstanceLocal((::MultiParticleObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::MultiParticleObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MultiTruthParticleObservable(void *p = 0);
   static void *newArray_MultiTruthParticleObservable(Long_t size, void *p);
   static void delete_MultiTruthParticleObservable(void *p);
   static void deleteArray_MultiTruthParticleObservable(void *p);
   static void destruct_MultiTruthParticleObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MultiTruthParticleObservable*)
   {
      ::MultiTruthParticleObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MultiTruthParticleObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MultiTruthParticleObservable", ::MultiTruthParticleObservable::Class_Version(), "CAFbbll/MultiTruthParticleObservable.h", 10,
                  typeid(::MultiTruthParticleObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MultiTruthParticleObservable::Dictionary, isa_proxy, 4,
                  sizeof(::MultiTruthParticleObservable) );
      instance.SetNew(&new_MultiTruthParticleObservable);
      instance.SetNewArray(&newArray_MultiTruthParticleObservable);
      instance.SetDelete(&delete_MultiTruthParticleObservable);
      instance.SetDeleteArray(&deleteArray_MultiTruthParticleObservable);
      instance.SetDestructor(&destruct_MultiTruthParticleObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MultiTruthParticleObservable*)
   {
      return GenerateInitInstanceLocal((::MultiTruthParticleObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::MultiTruthParticleObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NTruthLeptons(void *p = 0);
   static void *newArray_NTruthLeptons(Long_t size, void *p);
   static void delete_NTruthLeptons(void *p);
   static void deleteArray_NTruthLeptons(void *p);
   static void destruct_NTruthLeptons(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NTruthLeptons*)
   {
      ::NTruthLeptons *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NTruthLeptons >(0);
      static ::ROOT::TGenericClassInfo 
         instance("NTruthLeptons", ::NTruthLeptons::Class_Version(), "CAFbbll/NTruthLeptons.h", 7,
                  typeid(::NTruthLeptons), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NTruthLeptons::Dictionary, isa_proxy, 4,
                  sizeof(::NTruthLeptons) );
      instance.SetNew(&new_NTruthLeptons);
      instance.SetNewArray(&newArray_NTruthLeptons);
      instance.SetDelete(&delete_NTruthLeptons);
      instance.SetDeleteArray(&deleteArray_NTruthLeptons);
      instance.SetDestructor(&destruct_NTruthLeptons);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NTruthLeptons*)
   {
      return GenerateInitInstanceLocal((::NTruthLeptons*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NTruthLeptons*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NWTauDecays(void *p = 0);
   static void *newArray_NWTauDecays(Long_t size, void *p);
   static void delete_NWTauDecays(void *p);
   static void deleteArray_NWTauDecays(void *p);
   static void destruct_NWTauDecays(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NWTauDecays*)
   {
      ::NWTauDecays *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NWTauDecays >(0);
      static ::ROOT::TGenericClassInfo 
         instance("NWTauDecays", ::NWTauDecays::Class_Version(), "CAFbbll/NWTauDecays.h", 7,
                  typeid(::NWTauDecays), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NWTauDecays::Dictionary, isa_proxy, 4,
                  sizeof(::NWTauDecays) );
      instance.SetNew(&new_NWTauDecays);
      instance.SetNewArray(&newArray_NWTauDecays);
      instance.SetDelete(&delete_NWTauDecays);
      instance.SetDeleteArray(&deleteArray_NWTauDecays);
      instance.SetDestructor(&destruct_NWTauDecays);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NWTauDecays*)
   {
      return GenerateInitInstanceLocal((::NWTauDecays*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NWTauDecays*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PNN_KL(void *p = 0);
   static void *newArray_PNN_KL(Long_t size, void *p);
   static void delete_PNN_KL(void *p);
   static void deleteArray_PNN_KL(void *p);
   static void destruct_PNN_KL(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PNN_KL*)
   {
      ::PNN_KL *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PNN_KL >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PNN_KL", ::PNN_KL::Class_Version(), "CAFbbll/PNN_KL.h", 25,
                  typeid(::PNN_KL), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PNN_KL::Dictionary, isa_proxy, 4,
                  sizeof(::PNN_KL) );
      instance.SetNew(&new_PNN_KL);
      instance.SetNewArray(&newArray_PNN_KL);
      instance.SetDelete(&delete_PNN_KL);
      instance.SetDeleteArray(&deleteArray_PNN_KL);
      instance.SetDestructor(&destruct_PNN_KL);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PNN_KL*)
   {
      return GenerateInitInstanceLocal((::PNN_KL*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PNN_KL*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PUWeight(void *p = 0);
   static void *newArray_PUWeight(Long_t size, void *p);
   static void delete_PUWeight(void *p);
   static void deleteArray_PUWeight(void *p);
   static void destruct_PUWeight(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PUWeight*)
   {
      ::PUWeight *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PUWeight >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PUWeight", ::PUWeight::Class_Version(), "CAFbbll/PUWeight.h", 7,
                  typeid(::PUWeight), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PUWeight::Dictionary, isa_proxy, 4,
                  sizeof(::PUWeight) );
      instance.SetNew(&new_PUWeight);
      instance.SetNewArray(&newArray_PUWeight);
      instance.SetDelete(&delete_PUWeight);
      instance.SetDeleteArray(&deleteArray_PUWeight);
      instance.SetDestructor(&destruct_PUWeight);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PUWeight*)
   {
      return GenerateInitInstanceLocal((::PUWeight*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PUWeight*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_RegionID(void *p = 0);
   static void *newArray_RegionID(Long_t size, void *p);
   static void delete_RegionID(void *p);
   static void deleteArray_RegionID(void *p);
   static void destruct_RegionID(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RegionID*)
   {
      ::RegionID *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RegionID >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RegionID", ::RegionID::Class_Version(), "CAFbbll/RegionID.h", 7,
                  typeid(::RegionID), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RegionID::Dictionary, isa_proxy, 4,
                  sizeof(::RegionID) );
      instance.SetNew(&new_RegionID);
      instance.SetNewArray(&newArray_RegionID);
      instance.SetDelete(&delete_RegionID);
      instance.SetDeleteArray(&deleteArray_RegionID);
      instance.SetDestructor(&destruct_RegionID);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RegionID*)
   {
      return GenerateInitInstanceLocal((::RegionID*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RegionID*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_SumOfWeight(void *p = 0);
   static void *newArray_SumOfWeight(Long_t size, void *p);
   static void delete_SumOfWeight(void *p);
   static void deleteArray_SumOfWeight(void *p);
   static void destruct_SumOfWeight(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SumOfWeight*)
   {
      ::SumOfWeight *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SumOfWeight >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SumOfWeight", ::SumOfWeight::Class_Version(), "CAFbbll/SumOfWeight.h", 7,
                  typeid(::SumOfWeight), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::SumOfWeight::Dictionary, isa_proxy, 4,
                  sizeof(::SumOfWeight) );
      instance.SetNew(&new_SumOfWeight);
      instance.SetNewArray(&newArray_SumOfWeight);
      instance.SetDelete(&delete_SumOfWeight);
      instance.SetDeleteArray(&deleteArray_SumOfWeight);
      instance.SetDestructor(&destruct_SumOfWeight);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SumOfWeight*)
   {
      return GenerateInitInstanceLocal((::SumOfWeight*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::SumOfWeight*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDefaultDecorationObservablelEfloatgR_Dictionary();
   static void TQIParticleDefaultDecorationObservablelEfloatgR_TClassManip(TClass*);
   static void *new_TQIParticleDefaultDecorationObservablelEfloatgR(void *p = 0);
   static void *newArray_TQIParticleDefaultDecorationObservablelEfloatgR(Long_t size, void *p);
   static void delete_TQIParticleDefaultDecorationObservablelEfloatgR(void *p);
   static void deleteArray_TQIParticleDefaultDecorationObservablelEfloatgR(void *p);
   static void destruct_TQIParticleDefaultDecorationObservablelEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDefaultDecorationObservable<float>*)
   {
      ::TQIParticleDefaultDecorationObservable<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDefaultDecorationObservable<float> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDefaultDecorationObservable<float>", ::TQIParticleDefaultDecorationObservable<float>::Class_Version(), "CAFbbll/TQIParticleDefaultDecorationObservable.h", 14,
                  typeid(::TQIParticleDefaultDecorationObservable<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDefaultDecorationObservablelEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDefaultDecorationObservable<float>) );
      instance.SetNew(&new_TQIParticleDefaultDecorationObservablelEfloatgR);
      instance.SetNewArray(&newArray_TQIParticleDefaultDecorationObservablelEfloatgR);
      instance.SetDelete(&delete_TQIParticleDefaultDecorationObservablelEfloatgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDefaultDecorationObservablelEfloatgR);
      instance.SetDestructor(&destruct_TQIParticleDefaultDecorationObservablelEfloatgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDefaultDecorationObservable<float>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDefaultDecorationObservable<float>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDefaultDecorationObservablelEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<float>*)0x0)->GetClass();
      TQIParticleDefaultDecorationObservablelEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDefaultDecorationObservablelEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDefaultDecorationObservablelEdoublegR_Dictionary();
   static void TQIParticleDefaultDecorationObservablelEdoublegR_TClassManip(TClass*);
   static void *new_TQIParticleDefaultDecorationObservablelEdoublegR(void *p = 0);
   static void *newArray_TQIParticleDefaultDecorationObservablelEdoublegR(Long_t size, void *p);
   static void delete_TQIParticleDefaultDecorationObservablelEdoublegR(void *p);
   static void deleteArray_TQIParticleDefaultDecorationObservablelEdoublegR(void *p);
   static void destruct_TQIParticleDefaultDecorationObservablelEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDefaultDecorationObservable<double>*)
   {
      ::TQIParticleDefaultDecorationObservable<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDefaultDecorationObservable<double> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDefaultDecorationObservable<double>", ::TQIParticleDefaultDecorationObservable<double>::Class_Version(), "CAFbbll/TQIParticleDefaultDecorationObservable.h", 14,
                  typeid(::TQIParticleDefaultDecorationObservable<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDefaultDecorationObservablelEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDefaultDecorationObservable<double>) );
      instance.SetNew(&new_TQIParticleDefaultDecorationObservablelEdoublegR);
      instance.SetNewArray(&newArray_TQIParticleDefaultDecorationObservablelEdoublegR);
      instance.SetDelete(&delete_TQIParticleDefaultDecorationObservablelEdoublegR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDefaultDecorationObservablelEdoublegR);
      instance.SetDestructor(&destruct_TQIParticleDefaultDecorationObservablelEdoublegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDefaultDecorationObservable<double>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDefaultDecorationObservable<double>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDefaultDecorationObservablelEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<double>*)0x0)->GetClass();
      TQIParticleDefaultDecorationObservablelEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDefaultDecorationObservablelEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDefaultDecorationObservablelEchargR_Dictionary();
   static void TQIParticleDefaultDecorationObservablelEchargR_TClassManip(TClass*);
   static void *new_TQIParticleDefaultDecorationObservablelEchargR(void *p = 0);
   static void *newArray_TQIParticleDefaultDecorationObservablelEchargR(Long_t size, void *p);
   static void delete_TQIParticleDefaultDecorationObservablelEchargR(void *p);
   static void deleteArray_TQIParticleDefaultDecorationObservablelEchargR(void *p);
   static void destruct_TQIParticleDefaultDecorationObservablelEchargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDefaultDecorationObservable<char>*)
   {
      ::TQIParticleDefaultDecorationObservable<char> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDefaultDecorationObservable<char> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDefaultDecorationObservable<char>", ::TQIParticleDefaultDecorationObservable<char>::Class_Version(), "CAFbbll/TQIParticleDefaultDecorationObservable.h", 14,
                  typeid(::TQIParticleDefaultDecorationObservable<char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDefaultDecorationObservablelEchargR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDefaultDecorationObservable<char>) );
      instance.SetNew(&new_TQIParticleDefaultDecorationObservablelEchargR);
      instance.SetNewArray(&newArray_TQIParticleDefaultDecorationObservablelEchargR);
      instance.SetDelete(&delete_TQIParticleDefaultDecorationObservablelEchargR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDefaultDecorationObservablelEchargR);
      instance.SetDestructor(&destruct_TQIParticleDefaultDecorationObservablelEchargR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDefaultDecorationObservable<char>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDefaultDecorationObservable<char>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<char>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDefaultDecorationObservablelEchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<char>*)0x0)->GetClass();
      TQIParticleDefaultDecorationObservablelEchargR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDefaultDecorationObservablelEchargR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDefaultDecorationObservablelEintgR_Dictionary();
   static void TQIParticleDefaultDecorationObservablelEintgR_TClassManip(TClass*);
   static void *new_TQIParticleDefaultDecorationObservablelEintgR(void *p = 0);
   static void *newArray_TQIParticleDefaultDecorationObservablelEintgR(Long_t size, void *p);
   static void delete_TQIParticleDefaultDecorationObservablelEintgR(void *p);
   static void deleteArray_TQIParticleDefaultDecorationObservablelEintgR(void *p);
   static void destruct_TQIParticleDefaultDecorationObservablelEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDefaultDecorationObservable<int>*)
   {
      ::TQIParticleDefaultDecorationObservable<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDefaultDecorationObservable<int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDefaultDecorationObservable<int>", ::TQIParticleDefaultDecorationObservable<int>::Class_Version(), "CAFbbll/TQIParticleDefaultDecorationObservable.h", 14,
                  typeid(::TQIParticleDefaultDecorationObservable<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDefaultDecorationObservablelEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDefaultDecorationObservable<int>) );
      instance.SetNew(&new_TQIParticleDefaultDecorationObservablelEintgR);
      instance.SetNewArray(&newArray_TQIParticleDefaultDecorationObservablelEintgR);
      instance.SetDelete(&delete_TQIParticleDefaultDecorationObservablelEintgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDefaultDecorationObservablelEintgR);
      instance.SetDestructor(&destruct_TQIParticleDefaultDecorationObservablelEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDefaultDecorationObservable<int>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDefaultDecorationObservable<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDefaultDecorationObservablelEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<int>*)0x0)->GetClass();
      TQIParticleDefaultDecorationObservablelEintgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDefaultDecorationObservablelEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDefaultDecorationObservablelEunsignedsPintgR_Dictionary();
   static void TQIParticleDefaultDecorationObservablelEunsignedsPintgR_TClassManip(TClass*);
   static void *new_TQIParticleDefaultDecorationObservablelEunsignedsPintgR(void *p = 0);
   static void *newArray_TQIParticleDefaultDecorationObservablelEunsignedsPintgR(Long_t size, void *p);
   static void delete_TQIParticleDefaultDecorationObservablelEunsignedsPintgR(void *p);
   static void deleteArray_TQIParticleDefaultDecorationObservablelEunsignedsPintgR(void *p);
   static void destruct_TQIParticleDefaultDecorationObservablelEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDefaultDecorationObservable<unsigned int>*)
   {
      ::TQIParticleDefaultDecorationObservable<unsigned int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDefaultDecorationObservable<unsigned int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDefaultDecorationObservable<unsigned int>", ::TQIParticleDefaultDecorationObservable<unsigned int>::Class_Version(), "CAFbbll/TQIParticleDefaultDecorationObservable.h", 14,
                  typeid(::TQIParticleDefaultDecorationObservable<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDefaultDecorationObservablelEunsignedsPintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDefaultDecorationObservable<unsigned int>) );
      instance.SetNew(&new_TQIParticleDefaultDecorationObservablelEunsignedsPintgR);
      instance.SetNewArray(&newArray_TQIParticleDefaultDecorationObservablelEunsignedsPintgR);
      instance.SetDelete(&delete_TQIParticleDefaultDecorationObservablelEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDefaultDecorationObservablelEunsignedsPintgR);
      instance.SetDestructor(&destruct_TQIParticleDefaultDecorationObservablelEunsignedsPintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDefaultDecorationObservable<unsigned int>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDefaultDecorationObservable<unsigned int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDefaultDecorationObservablelEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<unsigned int>*)0x0)->GetClass();
      TQIParticleDefaultDecorationObservablelEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDefaultDecorationObservablelEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDefaultDecorationObservablelEunsignedsPshortgR_Dictionary();
   static void TQIParticleDefaultDecorationObservablelEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR(void *p = 0);
   static void *newArray_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR(Long_t size, void *p);
   static void delete_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR(void *p);
   static void deleteArray_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR(void *p);
   static void destruct_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDefaultDecorationObservable<unsigned short>*)
   {
      ::TQIParticleDefaultDecorationObservable<unsigned short> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDefaultDecorationObservable<unsigned short> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDefaultDecorationObservable<unsigned short>", ::TQIParticleDefaultDecorationObservable<unsigned short>::Class_Version(), "CAFbbll/TQIParticleDefaultDecorationObservable.h", 14,
                  typeid(::TQIParticleDefaultDecorationObservable<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDefaultDecorationObservablelEunsignedsPshortgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDefaultDecorationObservable<unsigned short>) );
      instance.SetNew(&new_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR);
      instance.SetNewArray(&newArray_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR);
      instance.SetDelete(&delete_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR);
      instance.SetDestructor(&destruct_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDefaultDecorationObservable<unsigned short>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDefaultDecorationObservable<unsigned short>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<unsigned short>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDefaultDecorationObservablelEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<unsigned short>*)0x0)->GetClass();
      TQIParticleDefaultDecorationObservablelEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDefaultDecorationObservablelEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIParticleDefaultDecorationObservablelEULong64_tgR_Dictionary();
   static void TQIParticleDefaultDecorationObservablelEULong64_tgR_TClassManip(TClass*);
   static void *new_TQIParticleDefaultDecorationObservablelEULong64_tgR(void *p = 0);
   static void *newArray_TQIParticleDefaultDecorationObservablelEULong64_tgR(Long_t size, void *p);
   static void delete_TQIParticleDefaultDecorationObservablelEULong64_tgR(void *p);
   static void deleteArray_TQIParticleDefaultDecorationObservablelEULong64_tgR(void *p);
   static void destruct_TQIParticleDefaultDecorationObservablelEULong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIParticleDefaultDecorationObservable<ULong64_t>*)
   {
      ::TQIParticleDefaultDecorationObservable<ULong64_t> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIParticleDefaultDecorationObservable<ULong64_t> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIParticleDefaultDecorationObservable<ULong64_t>", ::TQIParticleDefaultDecorationObservable<ULong64_t>::Class_Version(), "CAFbbll/TQIParticleDefaultDecorationObservable.h", 14,
                  typeid(::TQIParticleDefaultDecorationObservable<ULong64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIParticleDefaultDecorationObservablelEULong64_tgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIParticleDefaultDecorationObservable<ULong64_t>) );
      instance.SetNew(&new_TQIParticleDefaultDecorationObservablelEULong64_tgR);
      instance.SetNewArray(&newArray_TQIParticleDefaultDecorationObservablelEULong64_tgR);
      instance.SetDelete(&delete_TQIParticleDefaultDecorationObservablelEULong64_tgR);
      instance.SetDeleteArray(&deleteArray_TQIParticleDefaultDecorationObservablelEULong64_tgR);
      instance.SetDestructor(&destruct_TQIParticleDefaultDecorationObservablelEULong64_tgR);

      ::ROOT::AddClassAlternate("TQIParticleDefaultDecorationObservable<ULong64_t>","TQIParticleDefaultDecorationObservable<unsigned long long>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIParticleDefaultDecorationObservable<ULong64_t>*)
   {
      return GenerateInitInstanceLocal((::TQIParticleDefaultDecorationObservable<ULong64_t>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<ULong64_t>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIParticleDefaultDecorationObservablelEULong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<ULong64_t>*)0x0)->GetClass();
      TQIParticleDefaultDecorationObservablelEULong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIParticleDefaultDecorationObservablelEULong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_TTVASF(void *p = 0);
   static void *newArray_TTVASF(Long_t size, void *p);
   static void delete_TTVASF(void *p);
   static void deleteArray_TTVASF(void *p);
   static void destruct_TTVASF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TTVASF*)
   {
      ::TTVASF *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TTVASF >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TTVASF", ::TTVASF::Class_Version(), "CAFbbll/TTVASF.h", 7,
                  typeid(::TTVASF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TTVASF::Dictionary, isa_proxy, 4,
                  sizeof(::TTVASF) );
      instance.SetNew(&new_TTVASF);
      instance.SetNewArray(&newArray_TTVASF);
      instance.SetDelete(&delete_TTVASF);
      instance.SetDeleteArray(&deleteArray_TTVASF);
      instance.SetDestructor(&destruct_TTVASF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TTVASF*)
   {
      return GenerateInitInstanceLocal((::TTVASF*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TTVASF*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TTbarNNLOWeight(void *p = 0);
   static void *newArray_TTbarNNLOWeight(Long_t size, void *p);
   static void delete_TTbarNNLOWeight(void *p);
   static void deleteArray_TTbarNNLOWeight(void *p);
   static void destruct_TTbarNNLOWeight(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TTbarNNLOWeight*)
   {
      ::TTbarNNLOWeight *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TTbarNNLOWeight >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TTbarNNLOWeight", ::TTbarNNLOWeight::Class_Version(), "CAFbbll/TTbarNNLOWeight.h", 18,
                  typeid(::TTbarNNLOWeight), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TTbarNNLOWeight::Dictionary, isa_proxy, 4,
                  sizeof(::TTbarNNLOWeight) );
      instance.SetNew(&new_TTbarNNLOWeight);
      instance.SetNewArray(&newArray_TTbarNNLOWeight);
      instance.SetDelete(&delete_TTbarNNLOWeight);
      instance.SetDeleteArray(&deleteArray_TTbarNNLOWeight);
      instance.SetDestructor(&destruct_TTbarNNLOWeight);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TTbarNNLOWeight*)
   {
      return GenerateInitInstanceLocal((::TTbarNNLOWeight*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TTbarNNLOWeight*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TriggerMatching(void *p = 0);
   static void *newArray_TriggerMatching(Long_t size, void *p);
   static void delete_TriggerMatching(void *p);
   static void deleteArray_TriggerMatching(void *p);
   static void destruct_TriggerMatching(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TriggerMatching*)
   {
      ::TriggerMatching *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TriggerMatching >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TriggerMatching", ::TriggerMatching::Class_Version(), "CAFbbll/TriggerMatching.h", 7,
                  typeid(::TriggerMatching), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TriggerMatching::Dictionary, isa_proxy, 4,
                  sizeof(::TriggerMatching) );
      instance.SetNew(&new_TriggerMatching);
      instance.SetNewArray(&newArray_TriggerMatching);
      instance.SetDelete(&delete_TriggerMatching);
      instance.SetDeleteArray(&deleteArray_TriggerMatching);
      instance.SetDestructor(&destruct_TriggerMatching);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TriggerMatching*)
   {
      return GenerateInitInstanceLocal((::TriggerMatching*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TriggerMatching*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TriggerSF(void *p = 0);
   static void *newArray_TriggerSF(Long_t size, void *p);
   static void delete_TriggerSF(void *p);
   static void deleteArray_TriggerSF(void *p);
   static void destruct_TriggerSF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TriggerSF*)
   {
      ::TriggerSF *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TriggerSF >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TriggerSF", ::TriggerSF::Class_Version(), "CAFbbll/TriggerSF.h", 7,
                  typeid(::TriggerSF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TriggerSF::Dictionary, isa_proxy, 4,
                  sizeof(::TriggerSF) );
      instance.SetNew(&new_TriggerSF);
      instance.SetNewArray(&newArray_TriggerSF);
      instance.SetDelete(&delete_TriggerSF);
      instance.SetDeleteArray(&deleteArray_TriggerSF);
      instance.SetDestructor(&destruct_TriggerSF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TriggerSF*)
   {
      return GenerateInitInstanceLocal((::TriggerSF*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TriggerSF*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TruthHiggsDecayInfo(void *p = 0);
   static void *newArray_TruthHiggsDecayInfo(Long_t size, void *p);
   static void delete_TruthHiggsDecayInfo(void *p);
   static void deleteArray_TruthHiggsDecayInfo(void *p);
   static void destruct_TruthHiggsDecayInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TruthHiggsDecayInfo*)
   {
      ::TruthHiggsDecayInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TruthHiggsDecayInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TruthHiggsDecayInfo", ::TruthHiggsDecayInfo::Class_Version(), "CAFbbll/TruthHiggsDecayInfo.h", 11,
                  typeid(::TruthHiggsDecayInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TruthHiggsDecayInfo::Dictionary, isa_proxy, 4,
                  sizeof(::TruthHiggsDecayInfo) );
      instance.SetNew(&new_TruthHiggsDecayInfo);
      instance.SetNewArray(&newArray_TruthHiggsDecayInfo);
      instance.SetDelete(&delete_TruthHiggsDecayInfo);
      instance.SetDeleteArray(&deleteArray_TruthHiggsDecayInfo);
      instance.SetDestructor(&destruct_TruthHiggsDecayInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TruthHiggsDecayInfo*)
   {
      return GenerateInitInstanceLocal((::TruthHiggsDecayInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TruthHiggsDecayInfo*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TruthMhh(void *p = 0);
   static void *newArray_TruthMhh(Long_t size, void *p);
   static void delete_TruthMhh(void *p);
   static void deleteArray_TruthMhh(void *p);
   static void destruct_TruthMhh(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TruthMhh*)
   {
      ::TruthMhh *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TruthMhh >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TruthMhh", ::TruthMhh::Class_Version(), "CAFbbll/TruthMhh.h", 7,
                  typeid(::TruthMhh), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TruthMhh::Dictionary, isa_proxy, 4,
                  sizeof(::TruthMhh) );
      instance.SetNew(&new_TruthMhh);
      instance.SetNewArray(&newArray_TruthMhh);
      instance.SetDelete(&delete_TruthMhh);
      instance.SetDeleteArray(&deleteArray_TruthMhh);
      instance.SetDestructor(&destruct_TruthMhh);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TruthMhh*)
   {
      return GenerateInitInstanceLocal((::TruthMhh*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TruthMhh*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TruthPthh(void *p = 0);
   static void *newArray_TruthPthh(Long_t size, void *p);
   static void delete_TruthPthh(void *p);
   static void deleteArray_TruthPthh(void *p);
   static void destruct_TruthPthh(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TruthPthh*)
   {
      ::TruthPthh *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TruthPthh >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TruthPthh", ::TruthPthh::Class_Version(), "CAFbbll/TruthPthh.h", 7,
                  typeid(::TruthPthh), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TruthPthh::Dictionary, isa_proxy, 4,
                  sizeof(::TruthPthh) );
      instance.SetNew(&new_TruthPthh);
      instance.SetNewArray(&newArray_TruthPthh);
      instance.SetDelete(&delete_TruthPthh);
      instance.SetDeleteArray(&deleteArray_TruthPthh);
      instance.SetDestructor(&destruct_TruthPthh);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TruthPthh*)
   {
      return GenerateInitInstanceLocal((::TruthPthh*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TruthPthh*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TwoNeutrinoScan(void *p = 0);
   static void *newArray_TwoNeutrinoScan(Long_t size, void *p);
   static void delete_TwoNeutrinoScan(void *p);
   static void deleteArray_TwoNeutrinoScan(void *p);
   static void destruct_TwoNeutrinoScan(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TwoNeutrinoScan*)
   {
      ::TwoNeutrinoScan *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TwoNeutrinoScan >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TwoNeutrinoScan", ::TwoNeutrinoScan::Class_Version(), "CAFbbll/TwoNeutrinoScan.h", 10,
                  typeid(::TwoNeutrinoScan), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TwoNeutrinoScan::Dictionary, isa_proxy, 4,
                  sizeof(::TwoNeutrinoScan) );
      instance.SetNew(&new_TwoNeutrinoScan);
      instance.SetNewArray(&newArray_TwoNeutrinoScan);
      instance.SetDelete(&delete_TwoNeutrinoScan);
      instance.SetDeleteArray(&deleteArray_TwoNeutrinoScan);
      instance.SetDestructor(&destruct_TwoNeutrinoScan);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TwoNeutrinoScan*)
   {
      return GenerateInitInstanceLocal((::TwoNeutrinoScan*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TwoNeutrinoScan*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr BBLLObjectSelectionAlgorithm::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *BBLLObjectSelectionAlgorithm::Class_Name()
{
   return "BBLLObjectSelectionAlgorithm";
}

//______________________________________________________________________________
const char *BBLLObjectSelectionAlgorithm::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BBLLObjectSelectionAlgorithm*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int BBLLObjectSelectionAlgorithm::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BBLLObjectSelectionAlgorithm*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BBLLObjectSelectionAlgorithm::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BBLLObjectSelectionAlgorithm*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BBLLObjectSelectionAlgorithm::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BBLLObjectSelectionAlgorithm*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr BBLLBJetCalibStudyAlgorithm::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *BBLLBJetCalibStudyAlgorithm::Class_Name()
{
   return "BBLLBJetCalibStudyAlgorithm";
}

//______________________________________________________________________________
const char *BBLLBJetCalibStudyAlgorithm::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BBLLBJetCalibStudyAlgorithm*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int BBLLBJetCalibStudyAlgorithm::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BBLLBJetCalibStudyAlgorithm*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BBLLBJetCalibStudyAlgorithm::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BBLLBJetCalibStudyAlgorithm*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BBLLBJetCalibStudyAlgorithm::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BBLLBJetCalibStudyAlgorithm*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr BBLLBTaggingStudyAlgorithm::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *BBLLBTaggingStudyAlgorithm::Class_Name()
{
   return "BBLLBTaggingStudyAlgorithm";
}

//______________________________________________________________________________
const char *BBLLBTaggingStudyAlgorithm::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BBLLBTaggingStudyAlgorithm*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int BBLLBTaggingStudyAlgorithm::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BBLLBTaggingStudyAlgorithm*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BBLLBTaggingStudyAlgorithm::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BBLLBTaggingStudyAlgorithm*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BBLLBTaggingStudyAlgorithm::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BBLLBTaggingStudyAlgorithm*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr BTagSF::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *BTagSF::Class_Name()
{
   return "BTagSF";
}

//______________________________________________________________________________
const char *BTagSF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BTagSF*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int BTagSF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BTagSF*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BTagSF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BTagSF*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BTagSF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BTagSF*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CodeTTBarDecay::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CodeTTBarDecay::Class_Name()
{
   return "CodeTTBarDecay";
}

//______________________________________________________________________________
const char *CodeTTBarDecay::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CodeTTBarDecay*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CodeTTBarDecay::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CodeTTBarDecay*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CodeTTBarDecay::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CodeTTBarDecay*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CodeTTBarDecay::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CodeTTBarDecay*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CombinedQuantities::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CombinedQuantities::Class_Name()
{
   return "CombinedQuantities";
}

//______________________________________________________________________________
const char *CombinedQuantities::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CombinedQuantities*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CombinedQuantities::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CombinedQuantities*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CombinedQuantities::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CombinedQuantities*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CombinedQuantities::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CombinedQuantities*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DDFakeWeight::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DDFakeWeight::Class_Name()
{
   return "DDFakeWeight";
}

//______________________________________________________________________________
const char *DDFakeWeight::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DDFakeWeight*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DDFakeWeight::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DDFakeWeight*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DDFakeWeight::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DDFakeWeight*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DDFakeWeight::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DDFakeWeight*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr FilteredOutputNode::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *FilteredOutputNode::Class_Name()
{
   return "FilteredOutputNode";
}

//______________________________________________________________________________
const char *FilteredOutputNode::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::FilteredOutputNode*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int FilteredOutputNode::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::FilteredOutputNode*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *FilteredOutputNode::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::FilteredOutputNode*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *FilteredOutputNode::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::FilteredOutputNode*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr JvtSF::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *JvtSF::Class_Name()
{
   return "JvtSF";
}

//______________________________________________________________________________
const char *JvtSF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JvtSF*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int JvtSF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JvtSF*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *JvtSF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JvtSF*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *JvtSF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JvtSF*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr LeptonFakeType::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *LeptonFakeType::Class_Name()
{
   return "LeptonFakeType";
}

//______________________________________________________________________________
const char *LeptonFakeType::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LeptonFakeType*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int LeptonFakeType::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LeptonFakeType*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LeptonFakeType::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LeptonFakeType*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LeptonFakeType::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LeptonFakeType*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr LeptonSF::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *LeptonSF::Class_Name()
{
   return "LeptonSF";
}

//______________________________________________________________________________
const char *LeptonSF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LeptonSF*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int LeptonSF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LeptonSF*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LeptonSF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LeptonSF*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LeptonSF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LeptonSF*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MCWeight::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MCWeight::Class_Name()
{
   return "MCWeight";
}

//______________________________________________________________________________
const char *MCWeight::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MCWeight*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MCWeight::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MCWeight*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MCWeight::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MCWeight*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MCWeight::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MCWeight*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr METObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *METObservable::Class_Name()
{
   return "METObservable";
}

//______________________________________________________________________________
const char *METObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::METObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int METObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::METObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *METObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::METObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *METObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::METObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MultiParticleObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MultiParticleObservable::Class_Name()
{
   return "MultiParticleObservable";
}

//______________________________________________________________________________
const char *MultiParticleObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MultiParticleObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MultiParticleObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MultiParticleObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MultiParticleObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MultiParticleObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MultiParticleObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MultiParticleObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MultiTruthParticleObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MultiTruthParticleObservable::Class_Name()
{
   return "MultiTruthParticleObservable";
}

//______________________________________________________________________________
const char *MultiTruthParticleObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MultiTruthParticleObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MultiTruthParticleObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MultiTruthParticleObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MultiTruthParticleObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MultiTruthParticleObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MultiTruthParticleObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MultiTruthParticleObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NTruthLeptons::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *NTruthLeptons::Class_Name()
{
   return "NTruthLeptons";
}

//______________________________________________________________________________
const char *NTruthLeptons::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NTruthLeptons*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int NTruthLeptons::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NTruthLeptons*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NTruthLeptons::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NTruthLeptons*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NTruthLeptons::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NTruthLeptons*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NWTauDecays::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *NWTauDecays::Class_Name()
{
   return "NWTauDecays";
}

//______________________________________________________________________________
const char *NWTauDecays::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NWTauDecays*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int NWTauDecays::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NWTauDecays*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NWTauDecays::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NWTauDecays*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NWTauDecays::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NWTauDecays*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PNN_KL::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PNN_KL::Class_Name()
{
   return "PNN_KL";
}

//______________________________________________________________________________
const char *PNN_KL::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PNN_KL*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PNN_KL::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PNN_KL*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PNN_KL::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PNN_KL*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PNN_KL::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PNN_KL*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PUWeight::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PUWeight::Class_Name()
{
   return "PUWeight";
}

//______________________________________________________________________________
const char *PUWeight::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PUWeight*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PUWeight::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PUWeight*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PUWeight::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PUWeight*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PUWeight::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PUWeight*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RegionID::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RegionID::Class_Name()
{
   return "RegionID";
}

//______________________________________________________________________________
const char *RegionID::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RegionID*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RegionID::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RegionID*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RegionID::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RegionID*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RegionID::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RegionID*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SumOfWeight::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SumOfWeight::Class_Name()
{
   return "SumOfWeight";
}

//______________________________________________________________________________
const char *SumOfWeight::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SumOfWeight*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SumOfWeight::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SumOfWeight*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SumOfWeight::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SumOfWeight*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SumOfWeight::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SumOfWeight*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDefaultDecorationObservable<float>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<float>::Class_Name()
{
   return "TQIParticleDefaultDecorationObservable<float>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<float>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDefaultDecorationObservable<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<float>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<float>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<float>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDefaultDecorationObservable<double>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<double>::Class_Name()
{
   return "TQIParticleDefaultDecorationObservable<double>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<double>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDefaultDecorationObservable<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<double>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<double>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<double>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDefaultDecorationObservable<char>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<char>::Class_Name()
{
   return "TQIParticleDefaultDecorationObservable<char>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<char>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<char>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDefaultDecorationObservable<char>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<char>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<char>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<char>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<char>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<char>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDefaultDecorationObservable<int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<int>::Class_Name()
{
   return "TQIParticleDefaultDecorationObservable<int>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDefaultDecorationObservable<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDefaultDecorationObservable<unsigned int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<unsigned int>::Class_Name()
{
   return "TQIParticleDefaultDecorationObservable<unsigned int>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<unsigned int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<unsigned int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDefaultDecorationObservable<unsigned int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<unsigned int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<unsigned int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<unsigned int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<unsigned int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<unsigned int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDefaultDecorationObservable<unsigned short>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<unsigned short>::Class_Name()
{
   return "TQIParticleDefaultDecorationObservable<unsigned short>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<unsigned short>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<unsigned short>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDefaultDecorationObservable<unsigned short>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<unsigned short>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<unsigned short>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<unsigned short>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<unsigned short>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<unsigned short>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIParticleDefaultDecorationObservable<ULong64_t>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<ULong64_t>::Class_Name()
{
   return "TQIParticleDefaultDecorationObservable<ULong64_t>";
}

//______________________________________________________________________________
template <> const char *TQIParticleDefaultDecorationObservable<ULong64_t>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<ULong64_t>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIParticleDefaultDecorationObservable<ULong64_t>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<ULong64_t>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<ULong64_t>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<ULong64_t>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIParticleDefaultDecorationObservable<ULong64_t>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIParticleDefaultDecorationObservable<ULong64_t>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TTVASF::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TTVASF::Class_Name()
{
   return "TTVASF";
}

//______________________________________________________________________________
const char *TTVASF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TTVASF*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TTVASF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TTVASF*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TTVASF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TTVASF*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TTVASF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TTVASF*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TTbarNNLOWeight::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TTbarNNLOWeight::Class_Name()
{
   return "TTbarNNLOWeight";
}

//______________________________________________________________________________
const char *TTbarNNLOWeight::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TTbarNNLOWeight*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TTbarNNLOWeight::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TTbarNNLOWeight*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TTbarNNLOWeight::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TTbarNNLOWeight*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TTbarNNLOWeight::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TTbarNNLOWeight*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TriggerMatching::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TriggerMatching::Class_Name()
{
   return "TriggerMatching";
}

//______________________________________________________________________________
const char *TriggerMatching::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriggerMatching*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TriggerMatching::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriggerMatching*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TriggerMatching::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriggerMatching*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TriggerMatching::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriggerMatching*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TriggerSF::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TriggerSF::Class_Name()
{
   return "TriggerSF";
}

//______________________________________________________________________________
const char *TriggerSF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriggerSF*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TriggerSF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriggerSF*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TriggerSF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriggerSF*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TriggerSF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriggerSF*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TruthHiggsDecayInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TruthHiggsDecayInfo::Class_Name()
{
   return "TruthHiggsDecayInfo";
}

//______________________________________________________________________________
const char *TruthHiggsDecayInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TruthHiggsDecayInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TruthHiggsDecayInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TruthHiggsDecayInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TruthHiggsDecayInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TruthHiggsDecayInfo*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TruthHiggsDecayInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TruthHiggsDecayInfo*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TruthMhh::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TruthMhh::Class_Name()
{
   return "TruthMhh";
}

//______________________________________________________________________________
const char *TruthMhh::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TruthMhh*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TruthMhh::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TruthMhh*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TruthMhh::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TruthMhh*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TruthMhh::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TruthMhh*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TruthPthh::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TruthPthh::Class_Name()
{
   return "TruthPthh";
}

//______________________________________________________________________________
const char *TruthPthh::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TruthPthh*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TruthPthh::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TruthPthh*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TruthPthh::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TruthPthh*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TruthPthh::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TruthPthh*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TwoNeutrinoScan::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TwoNeutrinoScan::Class_Name()
{
   return "TwoNeutrinoScan";
}

//______________________________________________________________________________
const char *TwoNeutrinoScan::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TwoNeutrinoScan*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TwoNeutrinoScan::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TwoNeutrinoScan*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TwoNeutrinoScan::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TwoNeutrinoScan*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TwoNeutrinoScan::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TwoNeutrinoScan*)0x0)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_BBLLContainerRetrieval(void *p) {
      return  p ? new(p) ::BBLLContainerRetrieval : new ::BBLLContainerRetrieval;
   }
   static void *newArray_BBLLContainerRetrieval(Long_t nElements, void *p) {
      return p ? new(p) ::BBLLContainerRetrieval[nElements] : new ::BBLLContainerRetrieval[nElements];
   }
   // Wrapper around operator delete
   static void delete_BBLLContainerRetrieval(void *p) {
      delete ((::BBLLContainerRetrieval*)p);
   }
   static void deleteArray_BBLLContainerRetrieval(void *p) {
      delete [] ((::BBLLContainerRetrieval*)p);
   }
   static void destruct_BBLLContainerRetrieval(void *p) {
      typedef ::BBLLContainerRetrieval current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::BBLLContainerRetrieval

//______________________________________________________________________________
void BBLLObjectSelectionAlgorithm::Streamer(TBuffer &R__b)
{
   // Stream an object of class BBLLObjectSelectionAlgorithm.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(BBLLObjectSelectionAlgorithm::Class(),this);
   } else {
      R__b.WriteClassBuffer(BBLLObjectSelectionAlgorithm::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_BBLLObjectSelectionAlgorithm(void *p) {
      return  p ? new(p) ::BBLLObjectSelectionAlgorithm : new ::BBLLObjectSelectionAlgorithm;
   }
   static void *newArray_BBLLObjectSelectionAlgorithm(Long_t nElements, void *p) {
      return p ? new(p) ::BBLLObjectSelectionAlgorithm[nElements] : new ::BBLLObjectSelectionAlgorithm[nElements];
   }
   // Wrapper around operator delete
   static void delete_BBLLObjectSelectionAlgorithm(void *p) {
      delete ((::BBLLObjectSelectionAlgorithm*)p);
   }
   static void deleteArray_BBLLObjectSelectionAlgorithm(void *p) {
      delete [] ((::BBLLObjectSelectionAlgorithm*)p);
   }
   static void destruct_BBLLObjectSelectionAlgorithm(void *p) {
      typedef ::BBLLObjectSelectionAlgorithm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::BBLLObjectSelectionAlgorithm

//______________________________________________________________________________
void BBLLBJetCalibStudyAlgorithm::Streamer(TBuffer &R__b)
{
   // Stream an object of class BBLLBJetCalibStudyAlgorithm.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(BBLLBJetCalibStudyAlgorithm::Class(),this);
   } else {
      R__b.WriteClassBuffer(BBLLBJetCalibStudyAlgorithm::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_BBLLBJetCalibStudyAlgorithm(void *p) {
      return  p ? new(p) ::BBLLBJetCalibStudyAlgorithm : new ::BBLLBJetCalibStudyAlgorithm;
   }
   static void *newArray_BBLLBJetCalibStudyAlgorithm(Long_t nElements, void *p) {
      return p ? new(p) ::BBLLBJetCalibStudyAlgorithm[nElements] : new ::BBLLBJetCalibStudyAlgorithm[nElements];
   }
   // Wrapper around operator delete
   static void delete_BBLLBJetCalibStudyAlgorithm(void *p) {
      delete ((::BBLLBJetCalibStudyAlgorithm*)p);
   }
   static void deleteArray_BBLLBJetCalibStudyAlgorithm(void *p) {
      delete [] ((::BBLLBJetCalibStudyAlgorithm*)p);
   }
   static void destruct_BBLLBJetCalibStudyAlgorithm(void *p) {
      typedef ::BBLLBJetCalibStudyAlgorithm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::BBLLBJetCalibStudyAlgorithm

//______________________________________________________________________________
void BBLLBTaggingStudyAlgorithm::Streamer(TBuffer &R__b)
{
   // Stream an object of class BBLLBTaggingStudyAlgorithm.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(BBLLBTaggingStudyAlgorithm::Class(),this);
   } else {
      R__b.WriteClassBuffer(BBLLBTaggingStudyAlgorithm::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_BBLLBTaggingStudyAlgorithm(void *p) {
      return  p ? new(p) ::BBLLBTaggingStudyAlgorithm : new ::BBLLBTaggingStudyAlgorithm;
   }
   static void *newArray_BBLLBTaggingStudyAlgorithm(Long_t nElements, void *p) {
      return p ? new(p) ::BBLLBTaggingStudyAlgorithm[nElements] : new ::BBLLBTaggingStudyAlgorithm[nElements];
   }
   // Wrapper around operator delete
   static void delete_BBLLBTaggingStudyAlgorithm(void *p) {
      delete ((::BBLLBTaggingStudyAlgorithm*)p);
   }
   static void deleteArray_BBLLBTaggingStudyAlgorithm(void *p) {
      delete [] ((::BBLLBTaggingStudyAlgorithm*)p);
   }
   static void destruct_BBLLBTaggingStudyAlgorithm(void *p) {
      typedef ::BBLLBTaggingStudyAlgorithm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::BBLLBTaggingStudyAlgorithm

//______________________________________________________________________________
void BTagSF::Streamer(TBuffer &R__b)
{
   // Stream an object of class BTagSF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(BTagSF::Class(),this);
   } else {
      R__b.WriteClassBuffer(BTagSF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_BTagSF(void *p) {
      return  p ? new(p) ::BTagSF : new ::BTagSF;
   }
   static void *newArray_BTagSF(Long_t nElements, void *p) {
      return p ? new(p) ::BTagSF[nElements] : new ::BTagSF[nElements];
   }
   // Wrapper around operator delete
   static void delete_BTagSF(void *p) {
      delete ((::BTagSF*)p);
   }
   static void deleteArray_BTagSF(void *p) {
      delete [] ((::BTagSF*)p);
   }
   static void destruct_BTagSF(void *p) {
      typedef ::BTagSF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::BTagSF

//______________________________________________________________________________
void CodeTTBarDecay::Streamer(TBuffer &R__b)
{
   // Stream an object of class CodeTTBarDecay.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CodeTTBarDecay::Class(),this);
   } else {
      R__b.WriteClassBuffer(CodeTTBarDecay::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CodeTTBarDecay(void *p) {
      return  p ? new(p) ::CodeTTBarDecay : new ::CodeTTBarDecay;
   }
   static void *newArray_CodeTTBarDecay(Long_t nElements, void *p) {
      return p ? new(p) ::CodeTTBarDecay[nElements] : new ::CodeTTBarDecay[nElements];
   }
   // Wrapper around operator delete
   static void delete_CodeTTBarDecay(void *p) {
      delete ((::CodeTTBarDecay*)p);
   }
   static void deleteArray_CodeTTBarDecay(void *p) {
      delete [] ((::CodeTTBarDecay*)p);
   }
   static void destruct_CodeTTBarDecay(void *p) {
      typedef ::CodeTTBarDecay current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CodeTTBarDecay

//______________________________________________________________________________
void CombinedQuantities::Streamer(TBuffer &R__b)
{
   // Stream an object of class CombinedQuantities.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CombinedQuantities::Class(),this);
   } else {
      R__b.WriteClassBuffer(CombinedQuantities::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CombinedQuantities(void *p) {
      return  p ? new(p) ::CombinedQuantities : new ::CombinedQuantities;
   }
   static void *newArray_CombinedQuantities(Long_t nElements, void *p) {
      return p ? new(p) ::CombinedQuantities[nElements] : new ::CombinedQuantities[nElements];
   }
   // Wrapper around operator delete
   static void delete_CombinedQuantities(void *p) {
      delete ((::CombinedQuantities*)p);
   }
   static void deleteArray_CombinedQuantities(void *p) {
      delete [] ((::CombinedQuantities*)p);
   }
   static void destruct_CombinedQuantities(void *p) {
      typedef ::CombinedQuantities current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CombinedQuantities

//______________________________________________________________________________
void DDFakeWeight::Streamer(TBuffer &R__b)
{
   // Stream an object of class DDFakeWeight.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DDFakeWeight::Class(),this);
   } else {
      R__b.WriteClassBuffer(DDFakeWeight::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DDFakeWeight(void *p) {
      return  p ? new(p) ::DDFakeWeight : new ::DDFakeWeight;
   }
   static void *newArray_DDFakeWeight(Long_t nElements, void *p) {
      return p ? new(p) ::DDFakeWeight[nElements] : new ::DDFakeWeight[nElements];
   }
   // Wrapper around operator delete
   static void delete_DDFakeWeight(void *p) {
      delete ((::DDFakeWeight*)p);
   }
   static void deleteArray_DDFakeWeight(void *p) {
      delete [] ((::DDFakeWeight*)p);
   }
   static void destruct_DDFakeWeight(void *p) {
      typedef ::DDFakeWeight current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DDFakeWeight

//______________________________________________________________________________
void FilteredOutputNode::Streamer(TBuffer &R__b)
{
   // Stream an object of class FilteredOutputNode.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(FilteredOutputNode::Class(),this);
   } else {
      R__b.WriteClassBuffer(FilteredOutputNode::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_FilteredOutputNode(void *p) {
      return  p ? new(p) ::FilteredOutputNode : new ::FilteredOutputNode;
   }
   static void *newArray_FilteredOutputNode(Long_t nElements, void *p) {
      return p ? new(p) ::FilteredOutputNode[nElements] : new ::FilteredOutputNode[nElements];
   }
   // Wrapper around operator delete
   static void delete_FilteredOutputNode(void *p) {
      delete ((::FilteredOutputNode*)p);
   }
   static void deleteArray_FilteredOutputNode(void *p) {
      delete [] ((::FilteredOutputNode*)p);
   }
   static void destruct_FilteredOutputNode(void *p) {
      typedef ::FilteredOutputNode current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::FilteredOutputNode

//______________________________________________________________________________
void JvtSF::Streamer(TBuffer &R__b)
{
   // Stream an object of class JvtSF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JvtSF::Class(),this);
   } else {
      R__b.WriteClassBuffer(JvtSF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_JvtSF(void *p) {
      return  p ? new(p) ::JvtSF : new ::JvtSF;
   }
   static void *newArray_JvtSF(Long_t nElements, void *p) {
      return p ? new(p) ::JvtSF[nElements] : new ::JvtSF[nElements];
   }
   // Wrapper around operator delete
   static void delete_JvtSF(void *p) {
      delete ((::JvtSF*)p);
   }
   static void deleteArray_JvtSF(void *p) {
      delete [] ((::JvtSF*)p);
   }
   static void destruct_JvtSF(void *p) {
      typedef ::JvtSF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JvtSF

//______________________________________________________________________________
void LeptonFakeType::Streamer(TBuffer &R__b)
{
   // Stream an object of class LeptonFakeType.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(LeptonFakeType::Class(),this);
   } else {
      R__b.WriteClassBuffer(LeptonFakeType::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LeptonFakeType(void *p) {
      return  p ? new(p) ::LeptonFakeType : new ::LeptonFakeType;
   }
   static void *newArray_LeptonFakeType(Long_t nElements, void *p) {
      return p ? new(p) ::LeptonFakeType[nElements] : new ::LeptonFakeType[nElements];
   }
   // Wrapper around operator delete
   static void delete_LeptonFakeType(void *p) {
      delete ((::LeptonFakeType*)p);
   }
   static void deleteArray_LeptonFakeType(void *p) {
      delete [] ((::LeptonFakeType*)p);
   }
   static void destruct_LeptonFakeType(void *p) {
      typedef ::LeptonFakeType current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LeptonFakeType

//______________________________________________________________________________
void LeptonSF::Streamer(TBuffer &R__b)
{
   // Stream an object of class LeptonSF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(LeptonSF::Class(),this);
   } else {
      R__b.WriteClassBuffer(LeptonSF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LeptonSF(void *p) {
      return  p ? new(p) ::LeptonSF : new ::LeptonSF;
   }
   static void *newArray_LeptonSF(Long_t nElements, void *p) {
      return p ? new(p) ::LeptonSF[nElements] : new ::LeptonSF[nElements];
   }
   // Wrapper around operator delete
   static void delete_LeptonSF(void *p) {
      delete ((::LeptonSF*)p);
   }
   static void deleteArray_LeptonSF(void *p) {
      delete [] ((::LeptonSF*)p);
   }
   static void destruct_LeptonSF(void *p) {
      typedef ::LeptonSF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LeptonSF

//______________________________________________________________________________
void MCWeight::Streamer(TBuffer &R__b)
{
   // Stream an object of class MCWeight.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MCWeight::Class(),this);
   } else {
      R__b.WriteClassBuffer(MCWeight::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MCWeight(void *p) {
      return  p ? new(p) ::MCWeight : new ::MCWeight;
   }
   static void *newArray_MCWeight(Long_t nElements, void *p) {
      return p ? new(p) ::MCWeight[nElements] : new ::MCWeight[nElements];
   }
   // Wrapper around operator delete
   static void delete_MCWeight(void *p) {
      delete ((::MCWeight*)p);
   }
   static void deleteArray_MCWeight(void *p) {
      delete [] ((::MCWeight*)p);
   }
   static void destruct_MCWeight(void *p) {
      typedef ::MCWeight current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MCWeight

//______________________________________________________________________________
void METObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class METObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(METObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(METObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_METObservable(void *p) {
      return  p ? new(p) ::METObservable : new ::METObservable;
   }
   static void *newArray_METObservable(Long_t nElements, void *p) {
      return p ? new(p) ::METObservable[nElements] : new ::METObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_METObservable(void *p) {
      delete ((::METObservable*)p);
   }
   static void deleteArray_METObservable(void *p) {
      delete [] ((::METObservable*)p);
   }
   static void destruct_METObservable(void *p) {
      typedef ::METObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::METObservable

//______________________________________________________________________________
void MultiParticleObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class MultiParticleObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MultiParticleObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(MultiParticleObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MultiParticleObservable(void *p) {
      return  p ? new(p) ::MultiParticleObservable : new ::MultiParticleObservable;
   }
   static void *newArray_MultiParticleObservable(Long_t nElements, void *p) {
      return p ? new(p) ::MultiParticleObservable[nElements] : new ::MultiParticleObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_MultiParticleObservable(void *p) {
      delete ((::MultiParticleObservable*)p);
   }
   static void deleteArray_MultiParticleObservable(void *p) {
      delete [] ((::MultiParticleObservable*)p);
   }
   static void destruct_MultiParticleObservable(void *p) {
      typedef ::MultiParticleObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MultiParticleObservable

//______________________________________________________________________________
void MultiTruthParticleObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class MultiTruthParticleObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MultiTruthParticleObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(MultiTruthParticleObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MultiTruthParticleObservable(void *p) {
      return  p ? new(p) ::MultiTruthParticleObservable : new ::MultiTruthParticleObservable;
   }
   static void *newArray_MultiTruthParticleObservable(Long_t nElements, void *p) {
      return p ? new(p) ::MultiTruthParticleObservable[nElements] : new ::MultiTruthParticleObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_MultiTruthParticleObservable(void *p) {
      delete ((::MultiTruthParticleObservable*)p);
   }
   static void deleteArray_MultiTruthParticleObservable(void *p) {
      delete [] ((::MultiTruthParticleObservable*)p);
   }
   static void destruct_MultiTruthParticleObservable(void *p) {
      typedef ::MultiTruthParticleObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MultiTruthParticleObservable

//______________________________________________________________________________
void NTruthLeptons::Streamer(TBuffer &R__b)
{
   // Stream an object of class NTruthLeptons.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NTruthLeptons::Class(),this);
   } else {
      R__b.WriteClassBuffer(NTruthLeptons::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NTruthLeptons(void *p) {
      return  p ? new(p) ::NTruthLeptons : new ::NTruthLeptons;
   }
   static void *newArray_NTruthLeptons(Long_t nElements, void *p) {
      return p ? new(p) ::NTruthLeptons[nElements] : new ::NTruthLeptons[nElements];
   }
   // Wrapper around operator delete
   static void delete_NTruthLeptons(void *p) {
      delete ((::NTruthLeptons*)p);
   }
   static void deleteArray_NTruthLeptons(void *p) {
      delete [] ((::NTruthLeptons*)p);
   }
   static void destruct_NTruthLeptons(void *p) {
      typedef ::NTruthLeptons current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NTruthLeptons

//______________________________________________________________________________
void NWTauDecays::Streamer(TBuffer &R__b)
{
   // Stream an object of class NWTauDecays.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NWTauDecays::Class(),this);
   } else {
      R__b.WriteClassBuffer(NWTauDecays::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NWTauDecays(void *p) {
      return  p ? new(p) ::NWTauDecays : new ::NWTauDecays;
   }
   static void *newArray_NWTauDecays(Long_t nElements, void *p) {
      return p ? new(p) ::NWTauDecays[nElements] : new ::NWTauDecays[nElements];
   }
   // Wrapper around operator delete
   static void delete_NWTauDecays(void *p) {
      delete ((::NWTauDecays*)p);
   }
   static void deleteArray_NWTauDecays(void *p) {
      delete [] ((::NWTauDecays*)p);
   }
   static void destruct_NWTauDecays(void *p) {
      typedef ::NWTauDecays current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NWTauDecays

//______________________________________________________________________________
void PNN_KL::Streamer(TBuffer &R__b)
{
   // Stream an object of class PNN_KL.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PNN_KL::Class(),this);
   } else {
      R__b.WriteClassBuffer(PNN_KL::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PNN_KL(void *p) {
      return  p ? new(p) ::PNN_KL : new ::PNN_KL;
   }
   static void *newArray_PNN_KL(Long_t nElements, void *p) {
      return p ? new(p) ::PNN_KL[nElements] : new ::PNN_KL[nElements];
   }
   // Wrapper around operator delete
   static void delete_PNN_KL(void *p) {
      delete ((::PNN_KL*)p);
   }
   static void deleteArray_PNN_KL(void *p) {
      delete [] ((::PNN_KL*)p);
   }
   static void destruct_PNN_KL(void *p) {
      typedef ::PNN_KL current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PNN_KL

//______________________________________________________________________________
void PUWeight::Streamer(TBuffer &R__b)
{
   // Stream an object of class PUWeight.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PUWeight::Class(),this);
   } else {
      R__b.WriteClassBuffer(PUWeight::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PUWeight(void *p) {
      return  p ? new(p) ::PUWeight : new ::PUWeight;
   }
   static void *newArray_PUWeight(Long_t nElements, void *p) {
      return p ? new(p) ::PUWeight[nElements] : new ::PUWeight[nElements];
   }
   // Wrapper around operator delete
   static void delete_PUWeight(void *p) {
      delete ((::PUWeight*)p);
   }
   static void deleteArray_PUWeight(void *p) {
      delete [] ((::PUWeight*)p);
   }
   static void destruct_PUWeight(void *p) {
      typedef ::PUWeight current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PUWeight

//______________________________________________________________________________
void RegionID::Streamer(TBuffer &R__b)
{
   // Stream an object of class RegionID.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RegionID::Class(),this);
   } else {
      R__b.WriteClassBuffer(RegionID::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RegionID(void *p) {
      return  p ? new(p) ::RegionID : new ::RegionID;
   }
   static void *newArray_RegionID(Long_t nElements, void *p) {
      return p ? new(p) ::RegionID[nElements] : new ::RegionID[nElements];
   }
   // Wrapper around operator delete
   static void delete_RegionID(void *p) {
      delete ((::RegionID*)p);
   }
   static void deleteArray_RegionID(void *p) {
      delete [] ((::RegionID*)p);
   }
   static void destruct_RegionID(void *p) {
      typedef ::RegionID current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RegionID

//______________________________________________________________________________
void SumOfWeight::Streamer(TBuffer &R__b)
{
   // Stream an object of class SumOfWeight.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(SumOfWeight::Class(),this);
   } else {
      R__b.WriteClassBuffer(SumOfWeight::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_SumOfWeight(void *p) {
      return  p ? new(p) ::SumOfWeight : new ::SumOfWeight;
   }
   static void *newArray_SumOfWeight(Long_t nElements, void *p) {
      return p ? new(p) ::SumOfWeight[nElements] : new ::SumOfWeight[nElements];
   }
   // Wrapper around operator delete
   static void delete_SumOfWeight(void *p) {
      delete ((::SumOfWeight*)p);
   }
   static void deleteArray_SumOfWeight(void *p) {
      delete [] ((::SumOfWeight*)p);
   }
   static void destruct_SumOfWeight(void *p) {
      typedef ::SumOfWeight current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SumOfWeight

//______________________________________________________________________________
template <> void TQIParticleDefaultDecorationObservable<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDefaultDecorationObservable<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDefaultDecorationObservable<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDefaultDecorationObservable<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDefaultDecorationObservablelEfloatgR(void *p) {
      return  p ? new(p) ::TQIParticleDefaultDecorationObservable<float> : new ::TQIParticleDefaultDecorationObservable<float>;
   }
   static void *newArray_TQIParticleDefaultDecorationObservablelEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDefaultDecorationObservable<float>[nElements] : new ::TQIParticleDefaultDecorationObservable<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDefaultDecorationObservablelEfloatgR(void *p) {
      delete ((::TQIParticleDefaultDecorationObservable<float>*)p);
   }
   static void deleteArray_TQIParticleDefaultDecorationObservablelEfloatgR(void *p) {
      delete [] ((::TQIParticleDefaultDecorationObservable<float>*)p);
   }
   static void destruct_TQIParticleDefaultDecorationObservablelEfloatgR(void *p) {
      typedef ::TQIParticleDefaultDecorationObservable<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDefaultDecorationObservable<float>

//______________________________________________________________________________
template <> void TQIParticleDefaultDecorationObservable<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDefaultDecorationObservable<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDefaultDecorationObservable<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDefaultDecorationObservable<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDefaultDecorationObservablelEdoublegR(void *p) {
      return  p ? new(p) ::TQIParticleDefaultDecorationObservable<double> : new ::TQIParticleDefaultDecorationObservable<double>;
   }
   static void *newArray_TQIParticleDefaultDecorationObservablelEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDefaultDecorationObservable<double>[nElements] : new ::TQIParticleDefaultDecorationObservable<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDefaultDecorationObservablelEdoublegR(void *p) {
      delete ((::TQIParticleDefaultDecorationObservable<double>*)p);
   }
   static void deleteArray_TQIParticleDefaultDecorationObservablelEdoublegR(void *p) {
      delete [] ((::TQIParticleDefaultDecorationObservable<double>*)p);
   }
   static void destruct_TQIParticleDefaultDecorationObservablelEdoublegR(void *p) {
      typedef ::TQIParticleDefaultDecorationObservable<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDefaultDecorationObservable<double>

//______________________________________________________________________________
template <> void TQIParticleDefaultDecorationObservable<char>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDefaultDecorationObservable<char>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDefaultDecorationObservable<char>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDefaultDecorationObservable<char>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDefaultDecorationObservablelEchargR(void *p) {
      return  p ? new(p) ::TQIParticleDefaultDecorationObservable<char> : new ::TQIParticleDefaultDecorationObservable<char>;
   }
   static void *newArray_TQIParticleDefaultDecorationObservablelEchargR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDefaultDecorationObservable<char>[nElements] : new ::TQIParticleDefaultDecorationObservable<char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDefaultDecorationObservablelEchargR(void *p) {
      delete ((::TQIParticleDefaultDecorationObservable<char>*)p);
   }
   static void deleteArray_TQIParticleDefaultDecorationObservablelEchargR(void *p) {
      delete [] ((::TQIParticleDefaultDecorationObservable<char>*)p);
   }
   static void destruct_TQIParticleDefaultDecorationObservablelEchargR(void *p) {
      typedef ::TQIParticleDefaultDecorationObservable<char> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDefaultDecorationObservable<char>

//______________________________________________________________________________
template <> void TQIParticleDefaultDecorationObservable<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDefaultDecorationObservable<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDefaultDecorationObservable<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDefaultDecorationObservable<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDefaultDecorationObservablelEintgR(void *p) {
      return  p ? new(p) ::TQIParticleDefaultDecorationObservable<int> : new ::TQIParticleDefaultDecorationObservable<int>;
   }
   static void *newArray_TQIParticleDefaultDecorationObservablelEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDefaultDecorationObservable<int>[nElements] : new ::TQIParticleDefaultDecorationObservable<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDefaultDecorationObservablelEintgR(void *p) {
      delete ((::TQIParticleDefaultDecorationObservable<int>*)p);
   }
   static void deleteArray_TQIParticleDefaultDecorationObservablelEintgR(void *p) {
      delete [] ((::TQIParticleDefaultDecorationObservable<int>*)p);
   }
   static void destruct_TQIParticleDefaultDecorationObservablelEintgR(void *p) {
      typedef ::TQIParticleDefaultDecorationObservable<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDefaultDecorationObservable<int>

//______________________________________________________________________________
template <> void TQIParticleDefaultDecorationObservable<unsigned int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDefaultDecorationObservable<unsigned int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDefaultDecorationObservable<unsigned int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDefaultDecorationObservable<unsigned int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDefaultDecorationObservablelEunsignedsPintgR(void *p) {
      return  p ? new(p) ::TQIParticleDefaultDecorationObservable<unsigned int> : new ::TQIParticleDefaultDecorationObservable<unsigned int>;
   }
   static void *newArray_TQIParticleDefaultDecorationObservablelEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDefaultDecorationObservable<unsigned int>[nElements] : new ::TQIParticleDefaultDecorationObservable<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDefaultDecorationObservablelEunsignedsPintgR(void *p) {
      delete ((::TQIParticleDefaultDecorationObservable<unsigned int>*)p);
   }
   static void deleteArray_TQIParticleDefaultDecorationObservablelEunsignedsPintgR(void *p) {
      delete [] ((::TQIParticleDefaultDecorationObservable<unsigned int>*)p);
   }
   static void destruct_TQIParticleDefaultDecorationObservablelEunsignedsPintgR(void *p) {
      typedef ::TQIParticleDefaultDecorationObservable<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDefaultDecorationObservable<unsigned int>

//______________________________________________________________________________
template <> void TQIParticleDefaultDecorationObservable<unsigned short>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDefaultDecorationObservable<unsigned short>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDefaultDecorationObservable<unsigned short>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDefaultDecorationObservable<unsigned short>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR(void *p) {
      return  p ? new(p) ::TQIParticleDefaultDecorationObservable<unsigned short> : new ::TQIParticleDefaultDecorationObservable<unsigned short>;
   }
   static void *newArray_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDefaultDecorationObservable<unsigned short>[nElements] : new ::TQIParticleDefaultDecorationObservable<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR(void *p) {
      delete ((::TQIParticleDefaultDecorationObservable<unsigned short>*)p);
   }
   static void deleteArray_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR(void *p) {
      delete [] ((::TQIParticleDefaultDecorationObservable<unsigned short>*)p);
   }
   static void destruct_TQIParticleDefaultDecorationObservablelEunsignedsPshortgR(void *p) {
      typedef ::TQIParticleDefaultDecorationObservable<unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDefaultDecorationObservable<unsigned short>

//______________________________________________________________________________
template <> void TQIParticleDefaultDecorationObservable<ULong64_t>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIParticleDefaultDecorationObservable<ULong64_t>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIParticleDefaultDecorationObservable<ULong64_t>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIParticleDefaultDecorationObservable<ULong64_t>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIParticleDefaultDecorationObservablelEULong64_tgR(void *p) {
      return  p ? new(p) ::TQIParticleDefaultDecorationObservable<ULong64_t> : new ::TQIParticleDefaultDecorationObservable<ULong64_t>;
   }
   static void *newArray_TQIParticleDefaultDecorationObservablelEULong64_tgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIParticleDefaultDecorationObservable<ULong64_t>[nElements] : new ::TQIParticleDefaultDecorationObservable<ULong64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIParticleDefaultDecorationObservablelEULong64_tgR(void *p) {
      delete ((::TQIParticleDefaultDecorationObservable<ULong64_t>*)p);
   }
   static void deleteArray_TQIParticleDefaultDecorationObservablelEULong64_tgR(void *p) {
      delete [] ((::TQIParticleDefaultDecorationObservable<ULong64_t>*)p);
   }
   static void destruct_TQIParticleDefaultDecorationObservablelEULong64_tgR(void *p) {
      typedef ::TQIParticleDefaultDecorationObservable<ULong64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIParticleDefaultDecorationObservable<ULong64_t>

//______________________________________________________________________________
void TTVASF::Streamer(TBuffer &R__b)
{
   // Stream an object of class TTVASF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TTVASF::Class(),this);
   } else {
      R__b.WriteClassBuffer(TTVASF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TTVASF(void *p) {
      return  p ? new(p) ::TTVASF : new ::TTVASF;
   }
   static void *newArray_TTVASF(Long_t nElements, void *p) {
      return p ? new(p) ::TTVASF[nElements] : new ::TTVASF[nElements];
   }
   // Wrapper around operator delete
   static void delete_TTVASF(void *p) {
      delete ((::TTVASF*)p);
   }
   static void deleteArray_TTVASF(void *p) {
      delete [] ((::TTVASF*)p);
   }
   static void destruct_TTVASF(void *p) {
      typedef ::TTVASF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TTVASF

//______________________________________________________________________________
void TTbarNNLOWeight::Streamer(TBuffer &R__b)
{
   // Stream an object of class TTbarNNLOWeight.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TTbarNNLOWeight::Class(),this);
   } else {
      R__b.WriteClassBuffer(TTbarNNLOWeight::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TTbarNNLOWeight(void *p) {
      return  p ? new(p) ::TTbarNNLOWeight : new ::TTbarNNLOWeight;
   }
   static void *newArray_TTbarNNLOWeight(Long_t nElements, void *p) {
      return p ? new(p) ::TTbarNNLOWeight[nElements] : new ::TTbarNNLOWeight[nElements];
   }
   // Wrapper around operator delete
   static void delete_TTbarNNLOWeight(void *p) {
      delete ((::TTbarNNLOWeight*)p);
   }
   static void deleteArray_TTbarNNLOWeight(void *p) {
      delete [] ((::TTbarNNLOWeight*)p);
   }
   static void destruct_TTbarNNLOWeight(void *p) {
      typedef ::TTbarNNLOWeight current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TTbarNNLOWeight

//______________________________________________________________________________
void TriggerMatching::Streamer(TBuffer &R__b)
{
   // Stream an object of class TriggerMatching.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TriggerMatching::Class(),this);
   } else {
      R__b.WriteClassBuffer(TriggerMatching::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TriggerMatching(void *p) {
      return  p ? new(p) ::TriggerMatching : new ::TriggerMatching;
   }
   static void *newArray_TriggerMatching(Long_t nElements, void *p) {
      return p ? new(p) ::TriggerMatching[nElements] : new ::TriggerMatching[nElements];
   }
   // Wrapper around operator delete
   static void delete_TriggerMatching(void *p) {
      delete ((::TriggerMatching*)p);
   }
   static void deleteArray_TriggerMatching(void *p) {
      delete [] ((::TriggerMatching*)p);
   }
   static void destruct_TriggerMatching(void *p) {
      typedef ::TriggerMatching current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TriggerMatching

//______________________________________________________________________________
void TriggerSF::Streamer(TBuffer &R__b)
{
   // Stream an object of class TriggerSF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TriggerSF::Class(),this);
   } else {
      R__b.WriteClassBuffer(TriggerSF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TriggerSF(void *p) {
      return  p ? new(p) ::TriggerSF : new ::TriggerSF;
   }
   static void *newArray_TriggerSF(Long_t nElements, void *p) {
      return p ? new(p) ::TriggerSF[nElements] : new ::TriggerSF[nElements];
   }
   // Wrapper around operator delete
   static void delete_TriggerSF(void *p) {
      delete ((::TriggerSF*)p);
   }
   static void deleteArray_TriggerSF(void *p) {
      delete [] ((::TriggerSF*)p);
   }
   static void destruct_TriggerSF(void *p) {
      typedef ::TriggerSF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TriggerSF

//______________________________________________________________________________
void TruthHiggsDecayInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class TruthHiggsDecayInfo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TruthHiggsDecayInfo::Class(),this);
   } else {
      R__b.WriteClassBuffer(TruthHiggsDecayInfo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TruthHiggsDecayInfo(void *p) {
      return  p ? new(p) ::TruthHiggsDecayInfo : new ::TruthHiggsDecayInfo;
   }
   static void *newArray_TruthHiggsDecayInfo(Long_t nElements, void *p) {
      return p ? new(p) ::TruthHiggsDecayInfo[nElements] : new ::TruthHiggsDecayInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_TruthHiggsDecayInfo(void *p) {
      delete ((::TruthHiggsDecayInfo*)p);
   }
   static void deleteArray_TruthHiggsDecayInfo(void *p) {
      delete [] ((::TruthHiggsDecayInfo*)p);
   }
   static void destruct_TruthHiggsDecayInfo(void *p) {
      typedef ::TruthHiggsDecayInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TruthHiggsDecayInfo

//______________________________________________________________________________
void TruthMhh::Streamer(TBuffer &R__b)
{
   // Stream an object of class TruthMhh.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TruthMhh::Class(),this);
   } else {
      R__b.WriteClassBuffer(TruthMhh::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TruthMhh(void *p) {
      return  p ? new(p) ::TruthMhh : new ::TruthMhh;
   }
   static void *newArray_TruthMhh(Long_t nElements, void *p) {
      return p ? new(p) ::TruthMhh[nElements] : new ::TruthMhh[nElements];
   }
   // Wrapper around operator delete
   static void delete_TruthMhh(void *p) {
      delete ((::TruthMhh*)p);
   }
   static void deleteArray_TruthMhh(void *p) {
      delete [] ((::TruthMhh*)p);
   }
   static void destruct_TruthMhh(void *p) {
      typedef ::TruthMhh current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TruthMhh

//______________________________________________________________________________
void TruthPthh::Streamer(TBuffer &R__b)
{
   // Stream an object of class TruthPthh.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TruthPthh::Class(),this);
   } else {
      R__b.WriteClassBuffer(TruthPthh::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TruthPthh(void *p) {
      return  p ? new(p) ::TruthPthh : new ::TruthPthh;
   }
   static void *newArray_TruthPthh(Long_t nElements, void *p) {
      return p ? new(p) ::TruthPthh[nElements] : new ::TruthPthh[nElements];
   }
   // Wrapper around operator delete
   static void delete_TruthPthh(void *p) {
      delete ((::TruthPthh*)p);
   }
   static void deleteArray_TruthPthh(void *p) {
      delete [] ((::TruthPthh*)p);
   }
   static void destruct_TruthPthh(void *p) {
      typedef ::TruthPthh current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TruthPthh

//______________________________________________________________________________
void TwoNeutrinoScan::Streamer(TBuffer &R__b)
{
   // Stream an object of class TwoNeutrinoScan.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TwoNeutrinoScan::Class(),this);
   } else {
      R__b.WriteClassBuffer(TwoNeutrinoScan::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TwoNeutrinoScan(void *p) {
      return  p ? new(p) ::TwoNeutrinoScan : new ::TwoNeutrinoScan;
   }
   static void *newArray_TwoNeutrinoScan(Long_t nElements, void *p) {
      return p ? new(p) ::TwoNeutrinoScan[nElements] : new ::TwoNeutrinoScan[nElements];
   }
   // Wrapper around operator delete
   static void delete_TwoNeutrinoScan(void *p) {
      delete ((::TwoNeutrinoScan*)p);
   }
   static void deleteArray_TwoNeutrinoScan(void *p) {
      delete [] ((::TwoNeutrinoScan*)p);
   }
   static void destruct_TwoNeutrinoScan(void *p) {
      typedef ::TwoNeutrinoScan current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TwoNeutrinoScan

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
   static TClass *vectorlETransferFactorConfiggR_Dictionary();
   static void vectorlETransferFactorConfiggR_TClassManip(TClass*);
   static void *new_vectorlETransferFactorConfiggR(void *p = 0);
   static void *newArray_vectorlETransferFactorConfiggR(Long_t size, void *p);
   static void delete_vectorlETransferFactorConfiggR(void *p);
   static void deleteArray_vectorlETransferFactorConfiggR(void *p);
   static void destruct_vectorlETransferFactorConfiggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TransferFactorConfig>*)
   {
      vector<TransferFactorConfig> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TransferFactorConfig>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TransferFactorConfig>", -2, "vector", 339,
                  typeid(vector<TransferFactorConfig>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETransferFactorConfiggR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TransferFactorConfig>) );
      instance.SetNew(&new_vectorlETransferFactorConfiggR);
      instance.SetNewArray(&newArray_vectorlETransferFactorConfiggR);
      instance.SetDelete(&delete_vectorlETransferFactorConfiggR);
      instance.SetDeleteArray(&deleteArray_vectorlETransferFactorConfiggR);
      instance.SetDestructor(&destruct_vectorlETransferFactorConfiggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TransferFactorConfig> >()));

      ::ROOT::AddClassAlternate("vector<TransferFactorConfig>","std::vector<TransferFactorConfig, std::allocator<TransferFactorConfig> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TransferFactorConfig>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETransferFactorConfiggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TransferFactorConfig>*)0x0)->GetClass();
      vectorlETransferFactorConfiggR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETransferFactorConfiggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETransferFactorConfiggR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TransferFactorConfig> : new vector<TransferFactorConfig>;
   }
   static void *newArray_vectorlETransferFactorConfiggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TransferFactorConfig>[nElements] : new vector<TransferFactorConfig>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETransferFactorConfiggR(void *p) {
      delete ((vector<TransferFactorConfig>*)p);
   }
   static void deleteArray_vectorlETransferFactorConfiggR(void *p) {
      delete [] ((vector<TransferFactorConfig>*)p);
   }
   static void destruct_vectorlETransferFactorConfiggR(void *p) {
      typedef vector<TransferFactorConfig> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TransferFactorConfig>

namespace ROOT {
   static TClass *vectorlETQObservablemUgR_Dictionary();
   static void vectorlETQObservablemUgR_TClassManip(TClass*);
   static void *new_vectorlETQObservablemUgR(void *p = 0);
   static void *newArray_vectorlETQObservablemUgR(Long_t size, void *p);
   static void delete_vectorlETQObservablemUgR(void *p);
   static void deleteArray_vectorlETQObservablemUgR(void *p);
   static void destruct_vectorlETQObservablemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TQObservable*>*)
   {
      vector<TQObservable*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TQObservable*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TQObservable*>", -2, "vector", 339,
                  typeid(vector<TQObservable*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETQObservablemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TQObservable*>) );
      instance.SetNew(&new_vectorlETQObservablemUgR);
      instance.SetNewArray(&newArray_vectorlETQObservablemUgR);
      instance.SetDelete(&delete_vectorlETQObservablemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETQObservablemUgR);
      instance.SetDestructor(&destruct_vectorlETQObservablemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TQObservable*> >()));

      ::ROOT::AddClassAlternate("vector<TQObservable*>","std::vector<TQObservable*, std::allocator<TQObservable*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TQObservable*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETQObservablemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TQObservable*>*)0x0)->GetClass();
      vectorlETQObservablemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETQObservablemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETQObservablemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQObservable*> : new vector<TQObservable*>;
   }
   static void *newArray_vectorlETQObservablemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQObservable*>[nElements] : new vector<TQObservable*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETQObservablemUgR(void *p) {
      delete ((vector<TQObservable*>*)p);
   }
   static void deleteArray_vectorlETQObservablemUgR(void *p) {
      delete [] ((vector<TQObservable*>*)p);
   }
   static void destruct_vectorlETQObservablemUgR(void *p) {
      typedef vector<TQObservable*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TQObservable*>

namespace ROOT {
   static TClass *vectorlEMultiParticleConstituentgR_Dictionary();
   static void vectorlEMultiParticleConstituentgR_TClassManip(TClass*);
   static void *new_vectorlEMultiParticleConstituentgR(void *p = 0);
   static void *newArray_vectorlEMultiParticleConstituentgR(Long_t size, void *p);
   static void delete_vectorlEMultiParticleConstituentgR(void *p);
   static void deleteArray_vectorlEMultiParticleConstituentgR(void *p);
   static void destruct_vectorlEMultiParticleConstituentgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<MultiParticleConstituent>*)
   {
      vector<MultiParticleConstituent> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<MultiParticleConstituent>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<MultiParticleConstituent>", -2, "vector", 339,
                  typeid(vector<MultiParticleConstituent>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEMultiParticleConstituentgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<MultiParticleConstituent>) );
      instance.SetNew(&new_vectorlEMultiParticleConstituentgR);
      instance.SetNewArray(&newArray_vectorlEMultiParticleConstituentgR);
      instance.SetDelete(&delete_vectorlEMultiParticleConstituentgR);
      instance.SetDeleteArray(&deleteArray_vectorlEMultiParticleConstituentgR);
      instance.SetDestructor(&destruct_vectorlEMultiParticleConstituentgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<MultiParticleConstituent> >()));

      ::ROOT::AddClassAlternate("vector<MultiParticleConstituent>","std::vector<MultiParticleConstituent, std::allocator<MultiParticleConstituent> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<MultiParticleConstituent>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEMultiParticleConstituentgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<MultiParticleConstituent>*)0x0)->GetClass();
      vectorlEMultiParticleConstituentgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEMultiParticleConstituentgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEMultiParticleConstituentgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<MultiParticleConstituent> : new vector<MultiParticleConstituent>;
   }
   static void *newArray_vectorlEMultiParticleConstituentgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<MultiParticleConstituent>[nElements] : new vector<MultiParticleConstituent>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEMultiParticleConstituentgR(void *p) {
      delete ((vector<MultiParticleConstituent>*)p);
   }
   static void deleteArray_vectorlEMultiParticleConstituentgR(void *p) {
      delete [] ((vector<MultiParticleConstituent>*)p);
   }
   static void destruct_vectorlEMultiParticleConstituentgR(void *p) {
      typedef vector<MultiParticleConstituent> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<MultiParticleConstituent>

namespace ROOT {
   static TClass *setlEstringgR_Dictionary();
   static void setlEstringgR_TClassManip(TClass*);
   static void *new_setlEstringgR(void *p = 0);
   static void *newArray_setlEstringgR(Long_t size, void *p);
   static void delete_setlEstringgR(void *p);
   static void deleteArray_setlEstringgR(void *p);
   static void destruct_setlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const set<string>*)
   {
      set<string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(set<string>));
      static ::ROOT::TGenericClassInfo 
         instance("set<string>", -2, "set", 94,
                  typeid(set<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &setlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(set<string>) );
      instance.SetNew(&new_setlEstringgR);
      instance.SetNewArray(&newArray_setlEstringgR);
      instance.SetDelete(&delete_setlEstringgR);
      instance.SetDeleteArray(&deleteArray_setlEstringgR);
      instance.SetDestructor(&destruct_setlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Insert< set<string> >()));

      ::ROOT::AddClassAlternate("set<string>","std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const set<string>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *setlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const set<string>*)0x0)->GetClass();
      setlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void setlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_setlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<string> : new set<string>;
   }
   static void *newArray_setlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<string>[nElements] : new set<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_setlEstringgR(void *p) {
      delete ((set<string>*)p);
   }
   static void deleteArray_setlEstringgR(void *p) {
      delete [] ((set<string>*)p);
   }
   static void destruct_setlEstringgR(void *p) {
      typedef set<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class set<string>

namespace ROOT {
   static TClass *maplEstringcOTQObservablemUgR_Dictionary();
   static void maplEstringcOTQObservablemUgR_TClassManip(TClass*);
   static void *new_maplEstringcOTQObservablemUgR(void *p = 0);
   static void *newArray_maplEstringcOTQObservablemUgR(Long_t size, void *p);
   static void delete_maplEstringcOTQObservablemUgR(void *p);
   static void deleteArray_maplEstringcOTQObservablemUgR(void *p);
   static void destruct_maplEstringcOTQObservablemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,TQObservable*>*)
   {
      map<string,TQObservable*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,TQObservable*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,TQObservable*>", -2, "map", 100,
                  typeid(map<string,TQObservable*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOTQObservablemUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,TQObservable*>) );
      instance.SetNew(&new_maplEstringcOTQObservablemUgR);
      instance.SetNewArray(&newArray_maplEstringcOTQObservablemUgR);
      instance.SetDelete(&delete_maplEstringcOTQObservablemUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOTQObservablemUgR);
      instance.SetDestructor(&destruct_maplEstringcOTQObservablemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,TQObservable*> >()));

      ::ROOT::AddClassAlternate("map<string,TQObservable*>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, TQObservable*, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, TQObservable*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,TQObservable*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOTQObservablemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,TQObservable*>*)0x0)->GetClass();
      maplEstringcOTQObservablemUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOTQObservablemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOTQObservablemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,TQObservable*> : new map<string,TQObservable*>;
   }
   static void *newArray_maplEstringcOTQObservablemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,TQObservable*>[nElements] : new map<string,TQObservable*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOTQObservablemUgR(void *p) {
      delete ((map<string,TQObservable*>*)p);
   }
   static void deleteArray_maplEstringcOTQObservablemUgR(void *p) {
      delete [] ((map<string,TQObservable*>*)p);
   }
   static void destruct_maplEstringcOTQObservablemUgR(void *p) {
      typedef map<string,TQObservable*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,TQObservable*>

namespace ROOT {
   static TClass *maplETStringcODataVectorlExAODcLcLIParticlegRmUgR_Dictionary();
   static void maplETStringcODataVectorlExAODcLcLIParticlegRmUgR_TClassManip(TClass*);
   static void *new_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR(void *p = 0);
   static void *newArray_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR(Long_t size, void *p);
   static void delete_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR(void *p);
   static void deleteArray_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR(void *p);
   static void destruct_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,DataVector<xAOD::IParticle>*>*)
   {
      map<TString,DataVector<xAOD::IParticle>*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,DataVector<xAOD::IParticle>*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,DataVector<xAOD::IParticle>*>", -2, "map", 100,
                  typeid(map<TString,DataVector<xAOD::IParticle>*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcODataVectorlExAODcLcLIParticlegRmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,DataVector<xAOD::IParticle>*>) );
      instance.SetNew(&new_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR);
      instance.SetNewArray(&newArray_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR);
      instance.SetDelete(&delete_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR);
      instance.SetDestructor(&destruct_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,DataVector<xAOD::IParticle>*> >()));

      ::ROOT::AddClassAlternate("map<TString,DataVector<xAOD::IParticle>*>","std::map<TString, DataVector<xAOD::IParticle, DataModel_detail::NoBase> const*, std::less<TString>, std::allocator<std::pair<TString const, DataVector<xAOD::IParticle, DataModel_detail::NoBase> const*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,DataVector<xAOD::IParticle>*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcODataVectorlExAODcLcLIParticlegRmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,DataVector<xAOD::IParticle>*>*)0x0)->GetClass();
      maplETStringcODataVectorlExAODcLcLIParticlegRmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcODataVectorlExAODcLcLIParticlegRmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,DataVector<xAOD::IParticle>*> : new map<TString,DataVector<xAOD::IParticle>*>;
   }
   static void *newArray_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,DataVector<xAOD::IParticle>*>[nElements] : new map<TString,DataVector<xAOD::IParticle>*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR(void *p) {
      delete ((map<TString,DataVector<xAOD::IParticle>*>*)p);
   }
   static void deleteArray_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR(void *p) {
      delete [] ((map<TString,DataVector<xAOD::IParticle>*>*)p);
   }
   static void destruct_maplETStringcODataVectorlExAODcLcLIParticlegRmUgR(void *p) {
      typedef map<TString,DataVector<xAOD::IParticle>*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,DataVector<xAOD::IParticle>*>

namespace {
  void TriggerDictionaryInitialization_libCAFbbll_Impl() {
    static const char* headers[] = {
"CAFbbll/BBLLBJetCalibStudyAlgorithm.h",
"CAFbbll/BBLLBTaggingStudyAlgorithm.h",
"CAFbbll/BBLLContainerRetrieval.h",
"CAFbbll/BBLLObjectSelectionAlgorithm.h",
"CAFbbll/BTagSF.h",
"CAFbbll/CodeTTBarDecay.h",
"CAFbbll/CombinedQuantities.h",
"CAFbbll/DDFakeWeight.h",
"CAFbbll/FilteredOutputNode.h",
"CAFbbll/JvtSF.h",
"CAFbbll/LeptonFakeType.h",
"CAFbbll/LeptonSF.h",
"CAFbbll/MCWeight.h",
"CAFbbll/METObservable.h",
"CAFbbll/MultiParticleObservable.h",
"CAFbbll/MultiTruthParticleObservable.h",
"CAFbbll/NTruthLeptons.h",
"CAFbbll/NWTauDecays.h",
"CAFbbll/Observables.h",
"CAFbbll/PNN_KL.h",
"CAFbbll/PUWeight.h",
"CAFbbll/RegionID.h",
"CAFbbll/SumOfWeight.h",
"CAFbbll/TQIParticleDefaultDecorationObservable.h",
"CAFbbll/TTVASF.h",
"CAFbbll/TTbarNNLOWeight.h",
"CAFbbll/TriggerMatching.h",
"CAFbbll/TriggerSF.h",
"CAFbbll/TruthHiggsDecayInfo.h",
"CAFbbll/TruthMhh.h",
"CAFbbll/TruthPthh.h",
"CAFbbll/TwoNeutrinoScan.h",
0
    };
    static const char* includePaths[] = {
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
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
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CAFxAODUtils",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CAFxAODUtils",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthToolSupport/AsgTools",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthToolSupport/AsgMessaging",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODParticleEvent",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEgamma",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/DetectorDescription/GeoPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/EventPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCaloEvent",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Calorimeter/CaloGeoHelpers",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTracking",
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
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CxAODTools",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CxAODTools",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/D3PDTools/EventLoop",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/D3PDTools/RootCoreUtils",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/D3PDTools/SampleHandler",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/D3PDTools/AnaAlgorithm",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthToolSupport/AsgServices",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Tools/PathResolver",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
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
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODLuminosity",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonEfficiencyCorrections",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/Interfaces/MuonAnalysisInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/PhysicsAnalysis/AnalysisCommon/PileupReweighting",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Trigger/TrigEvent/TrigDecisionInterface",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Reconstruction/Jet/JetCalibTools",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEventShape",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Reconstruction/Jet/JetInterface",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CalcGenericMT2",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CalcGenericMT2",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/TTbarNNLOReweighter",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/TTbarNNLOReweighter",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CMakeFiles/makeCAFbbllCintDict.yAWLDk/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCAFbbll dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$CAFbbll/BBLLContainerRetrieval.h")))  __attribute__((annotate("$clingAutoload$CAFbbll/BBLLBJetCalibStudyAlgorithm.h")))  BBLLContainerRetrieval;
class __attribute__((annotate("$clingAutoload$CAFbbll/BBLLObjectSelectionAlgorithm.h")))  __attribute__((annotate("$clingAutoload$CAFbbll/BBLLBJetCalibStudyAlgorithm.h")))  BBLLObjectSelectionAlgorithm;
class __attribute__((annotate("$clingAutoload$CAFbbll/BBLLBJetCalibStudyAlgorithm.h")))  BBLLBJetCalibStudyAlgorithm;
class __attribute__((annotate("$clingAutoload$CAFbbll/BBLLBTaggingStudyAlgorithm.h")))  BBLLBTaggingStudyAlgorithm;
class __attribute__((annotate("$clingAutoload$CAFbbll/BTagSF.h")))  BTagSF;
class __attribute__((annotate("$clingAutoload$CAFbbll/CodeTTBarDecay.h")))  CodeTTBarDecay;
class __attribute__((annotate("$clingAutoload$CAFbbll/CombinedQuantities.h")))  CombinedQuantities;
class __attribute__((annotate("$clingAutoload$CAFbbll/DDFakeWeight.h")))  DDFakeWeight;
class __attribute__((annotate("$clingAutoload$CAFbbll/FilteredOutputNode.h")))  FilteredOutputNode;
class __attribute__((annotate("$clingAutoload$CAFbbll/JvtSF.h")))  JvtSF;
class __attribute__((annotate("$clingAutoload$CAFbbll/LeptonFakeType.h")))  LeptonFakeType;
class __attribute__((annotate("$clingAutoload$CAFbbll/LeptonSF.h")))  LeptonSF;
class __attribute__((annotate("$clingAutoload$CAFbbll/MCWeight.h")))  MCWeight;
class __attribute__((annotate("$clingAutoload$CAFbbll/METObservable.h")))  METObservable;
class __attribute__((annotate("$clingAutoload$CAFbbll/MultiParticleObservable.h")))  MultiParticleObservable;
class __attribute__((annotate("$clingAutoload$CAFbbll/MultiTruthParticleObservable.h")))  MultiTruthParticleObservable;
class __attribute__((annotate("$clingAutoload$CAFbbll/NTruthLeptons.h")))  NTruthLeptons;
class __attribute__((annotate("$clingAutoload$CAFbbll/NWTauDecays.h")))  NWTauDecays;
class __attribute__((annotate("$clingAutoload$CAFbbll/PNN_KL.h")))  PNN_KL;
class __attribute__((annotate("$clingAutoload$CAFbbll/PUWeight.h")))  PUWeight;
class __attribute__((annotate("$clingAutoload$CAFbbll/RegionID.h")))  RegionID;
class __attribute__((annotate("$clingAutoload$CAFbbll/SumOfWeight.h")))  SumOfWeight;
template <class Taux> class __attribute__((annotate("$clingAutoload$CAFbbll/TQIParticleDefaultDecorationObservable.h")))  TQIParticleDefaultDecorationObservable;

class __attribute__((annotate("$clingAutoload$CAFbbll/TTVASF.h")))  TTVASF;
class __attribute__((annotate("$clingAutoload$CAFbbll/TTbarNNLOWeight.h")))  TTbarNNLOWeight;
class __attribute__((annotate("$clingAutoload$CAFbbll/TriggerMatching.h")))  TriggerMatching;
class __attribute__((annotate("$clingAutoload$CAFbbll/TriggerSF.h")))  TriggerSF;
class __attribute__((annotate("$clingAutoload$CAFbbll/TruthHiggsDecayInfo.h")))  TruthHiggsDecayInfo;
class __attribute__((annotate("$clingAutoload$CAFbbll/TruthMhh.h")))  TruthMhh;
class __attribute__((annotate("$clingAutoload$CAFbbll/TruthPthh.h")))  TruthPthh;
class __attribute__((annotate("$clingAutoload$CAFbbll/TwoNeutrinoScan.h")))  TwoNeutrinoScan;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCAFbbll dictionary payload"

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
  #define PACKAGE_VERSION "CAFbbll-00-00-00"
#endif
#ifndef PACKAGE_VERSION_UQ
  #define PACKAGE_VERSION_UQ CAFbbll-00-00-00
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CAFbbll/BBLLBJetCalibStudyAlgorithm.h"
#include "CAFbbll/BBLLBTaggingStudyAlgorithm.h"
#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFbbll/BBLLObjectSelectionAlgorithm.h"
#include "CAFbbll/BTagSF.h"
#include "CAFbbll/CodeTTBarDecay.h"
#include "CAFbbll/CombinedQuantities.h"
#include "CAFbbll/DDFakeWeight.h"
#include "CAFbbll/FilteredOutputNode.h"
#include "CAFbbll/JvtSF.h"
#include "CAFbbll/LeptonFakeType.h"
#include "CAFbbll/LeptonSF.h"
#include "CAFbbll/MCWeight.h"
#include "CAFbbll/METObservable.h"
#include "CAFbbll/MultiParticleObservable.h"
#include "CAFbbll/MultiTruthParticleObservable.h"
#include "CAFbbll/NTruthLeptons.h"
#include "CAFbbll/NWTauDecays.h"
#include "CAFbbll/Observables.h"
#include "CAFbbll/PNN_KL.h"
#include "CAFbbll/PUWeight.h"
#include "CAFbbll/RegionID.h"
#include "CAFbbll/SumOfWeight.h"
#include "CAFbbll/TQIParticleDefaultDecorationObservable.h"
#include "CAFbbll/TTVASF.h"
#include "CAFbbll/TTbarNNLOWeight.h"
#include "CAFbbll/TriggerMatching.h"
#include "CAFbbll/TriggerSF.h"
#include "CAFbbll/TruthHiggsDecayInfo.h"
#include "CAFbbll/TruthMhh.h"
#include "CAFbbll/TruthPthh.h"
#include "CAFbbll/TwoNeutrinoScan.h"
//this is an automatically generated -*- c++ -*- file - EDITS WILL BE LOST!

#ifndef __CAFbbllDICT__
#define __CAFbbllDICT__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winconsistent-missing-override"

#include "CAFbbll/BBLLBJetCalibStudyAlgorithm.h"
#include "CAFbbll/BBLLBTaggingStudyAlgorithm.h"
#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFbbll/BBLLObjectSelectionAlgorithm.h"
#include "CAFbbll/BTagSF.h"
#include "CAFbbll/CodeTTBarDecay.h"
#include "CAFbbll/CombinedQuantities.h"
#include "CAFbbll/DDFakeWeight.h"
#include "CAFbbll/FilteredOutputNode.h"
#include "CAFbbll/JvtSF.h"
#include "CAFbbll/LeptonFakeType.h"
#include "CAFbbll/LeptonSF.h"
#include "CAFbbll/MCWeight.h"
#include "CAFbbll/METObservable.h"
#include "CAFbbll/MultiParticleObservable.h"
#include "CAFbbll/MultiTruthParticleObservable.h"
#include "CAFbbll/NTruthLeptons.h"
#include "CAFbbll/NWTauDecays.h"
#include "CAFbbll/Observables.h"
#include "CAFbbll/PNN_KL.h"
#include "CAFbbll/PUWeight.h"
#include "CAFbbll/RegionID.h"
#include "CAFbbll/SumOfWeight.h"
#include "CAFbbll/TQIParticleDefaultDecorationObservable.h"
#include "CAFbbll/TTVASF.h"
#include "CAFbbll/TTbarNNLOWeight.h"
#include "CAFbbll/TriggerMatching.h"
#include "CAFbbll/TriggerSF.h"
#include "CAFbbll/TruthHiggsDecayInfo.h"
#include "CAFbbll/TruthMhh.h"
#include "CAFbbll/TruthPthh.h"
#include "CAFbbll/TwoNeutrinoScan.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
// >>> begin BBLLBJetCalibStudyAlgorithm.h
#pragma link C++ class BBLLBJetCalibStudyAlgorithm+;
// <<< end BBLLBJetCalibStudyAlgorithm.h

// >>> begin BBLLBTaggingStudyAlgorithm.h
#pragma link C++ class BBLLBTaggingStudyAlgorithm+;
// <<< end BBLLBTaggingStudyAlgorithm.h

// >>> begin BBLLContainerRetrieval.h
#pragma link C++ class BBLLContainerRetrieval+;
// <<< end BBLLContainerRetrieval.h

// >>> begin BBLLObjectSelectionAlgorithm.h
#pragma link C++ class BBLLObjectSelectionAlgorithm+;
// <<< end BBLLObjectSelectionAlgorithm.h

// >>> begin BTagSF.h
#pragma link C++ class BTagSF+;
// <<< end BTagSF.h

// >>> begin CodeTTBarDecay.h
#pragma link C++ class CodeTTBarDecay+;
// <<< end CodeTTBarDecay.h

// >>> begin CombinedQuantities.h
#pragma link C++ class CombinedQuantities+;
// <<< end CombinedQuantities.h

// >>> begin DDFakeWeight.h
#pragma link C++ class DDFakeWeight+;
// <<< end DDFakeWeight.h

// >>> begin FilteredOutputNode.h
#pragma link C++ class FilteredOutputNode+;
// <<< end FilteredOutputNode.h

// >>> begin JvtSF.h
#pragma link C++ class JvtSF+;
// <<< end JvtSF.h

// >>> begin LeptonFakeType.h
#pragma link C++ class LeptonFakeType+;
// <<< end LeptonFakeType.h

// >>> begin LeptonSF.h
#pragma link C++ class LeptonSF+;
// <<< end LeptonSF.h

// >>> begin MCWeight.h
#pragma link C++ class MCWeight+;
// <<< end MCWeight.h

// >>> begin METObservable.h
#pragma link C++ class METObservable+;
// <<< end METObservable.h

// >>> begin MultiParticleObservable.h
#pragma link C++ class MultiParticleObservable+;
// <<< end MultiParticleObservable.h

// >>> begin MultiTruthParticleObservable.h
#pragma link C++ class MultiTruthParticleObservable+;
// <<< end MultiTruthParticleObservable.h

// >>> begin NTruthLeptons.h
#pragma link C++ class NTruthLeptons+;
// <<< end NTruthLeptons.h

// >>> begin NWTauDecays.h
#pragma link C++ class NWTauDecays+;
// <<< end NWTauDecays.h

// >>> begin Observables.h
#pragma link C++ namespace TQObservableFactory;
// <<< end Observables.h

// >>> begin PNN_KL.h
#pragma link C++ namespace lwt;
#pragma link C++ namespace TQObservableFactory;
#pragma link C++ class PNN_KL+;
// <<< end PNN_KL.h

// >>> begin PUWeight.h
#pragma link C++ class PUWeight+;
// <<< end PUWeight.h

// >>> begin RegionID.h
#pragma link C++ class RegionID+;
// <<< end RegionID.h

// >>> begin SumOfWeight.h
#pragma link C++ class SumOfWeight+;
// <<< end SumOfWeight.h

// >>> begin TQIParticleDefaultDecorationObservable.h
#pragma link C++ class TQIParticleDefaultDecorationObservable <float>+;
#pragma link C++ class TQIParticleDefaultDecorationObservable <double>+;
#pragma link C++ class TQIParticleDefaultDecorationObservable <char>+;
#pragma link C++ class TQIParticleDefaultDecorationObservable <unsigned int>+;
#pragma link C++ class TQIParticleDefaultDecorationObservable <unsigned short>+;
#pragma link C++ class TQIParticleDefaultDecorationObservable <unsigned long long>+;
#pragma link C++ class TQIParticleDefaultDecorationObservable <int>+;// <<< end TQIParticleDefaultDecorationObservable.h

// >>> begin TTVASF.h
#pragma link C++ class TTVASF+;
// <<< end TTVASF.h

// >>> begin TTbarNNLOWeight.h
#pragma link C++ class TTbarNNLOWeight+;
// <<< end TTbarNNLOWeight.h

// >>> begin TriggerMatching.h
#pragma link C++ class TriggerMatching+;
// <<< end TriggerMatching.h

// >>> begin TriggerSF.h
#pragma link C++ class TriggerSF+;
// <<< end TriggerSF.h

// >>> begin TruthHiggsDecayInfo.h
#pragma link C++ class TruthHiggsDecayInfo+;
// <<< end TruthHiggsDecayInfo.h

// >>> begin TruthMhh.h
#pragma link C++ class TruthMhh+;
// <<< end TruthMhh.h

// >>> begin TruthPthh.h
#pragma link C++ class TruthPthh+;
// <<< end TruthPthh.h

// >>> begin TwoNeutrinoScan.h
#pragma link C++ class TwoNeutrinoScan+;
// <<< end TwoNeutrinoScan.h


#endif //__CINT__
#endif //__CAFbbllDICT__

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"BBLLBJetCalibStudyAlgorithm", payloadCode, "@",
"BBLLBTaggingStudyAlgorithm", payloadCode, "@",
"BBLLContainerRetrieval", payloadCode, "@",
"BBLLObjectSelectionAlgorithm", payloadCode, "@",
"BTagSF", payloadCode, "@",
"CodeTTBarDecay", payloadCode, "@",
"CombinedQuantities", payloadCode, "@",
"DDFakeWeight", payloadCode, "@",
"FilteredOutputNode", payloadCode, "@",
"JvtSF", payloadCode, "@",
"LeptonFakeType", payloadCode, "@",
"LeptonSF", payloadCode, "@",
"MCWeight", payloadCode, "@",
"METObservable", payloadCode, "@",
"MultiParticleObservable", payloadCode, "@",
"MultiTruthParticleObservable", payloadCode, "@",
"NTruthLeptons", payloadCode, "@",
"NWTauDecays", payloadCode, "@",
"PNN_KL", payloadCode, "@",
"PUWeight", payloadCode, "@",
"RegionID", payloadCode, "@",
"SumOfWeight", payloadCode, "@",
"TQIParticleDefaultDecorationObservable<ULong64_t>", payloadCode, "@",
"TQIParticleDefaultDecorationObservable<char>", payloadCode, "@",
"TQIParticleDefaultDecorationObservable<double>", payloadCode, "@",
"TQIParticleDefaultDecorationObservable<float>", payloadCode, "@",
"TQIParticleDefaultDecorationObservable<int>", payloadCode, "@",
"TQIParticleDefaultDecorationObservable<unsigned int>", payloadCode, "@",
"TQIParticleDefaultDecorationObservable<unsigned short>", payloadCode, "@",
"TTVASF", payloadCode, "@",
"TTbarNNLOWeight", payloadCode, "@",
"TriggerMatching", payloadCode, "@",
"TriggerSF", payloadCode, "@",
"TruthHiggsDecayInfo", payloadCode, "@",
"TruthMhh", payloadCode, "@",
"TruthPthh", payloadCode, "@",
"TwoNeutrinoScan", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCAFbbll",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCAFbbll_Impl, {{"namespace DataVector_detail { template <typename T> class DVLEltBaseInit; }", 1},{"namespace DataVector_detail { template <typename B1, typename B2, typename B3> class VirtBases; }", 1},{"template <typename T> class DataVectorBase;", 1},{"template <typename T, typename BASE> class DataVector;", 1}}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCAFbbll_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCAFbbll() {
  TriggerDictionaryInitialization_libCAFbbll_Impl();
}
