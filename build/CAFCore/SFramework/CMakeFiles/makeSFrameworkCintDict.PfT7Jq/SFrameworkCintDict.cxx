// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SFrameworkCintDict
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
#include "SFramework/RooFitUtils.h"
#include "SFramework/Run_Freiburg_HWW_Statistics.h"
#include "SFramework/TSBreakdownCalculator.h"
#include "SFramework/TSCLSignificanceEvaluator.h"
#include "SFramework/TSContourScanner.h"
#include "SFramework/TSFitter.h"
#include "SFramework/TSHistogramExporter.h"
#include "SFramework/TSHypothesisTest.h"
#include "SFramework/TSImpactCalculator.h"
#include "SFramework/TSIterator.h"
#include "SFramework/TSLikelihoodScanner.h"
#include "SFramework/TSLimitCalculator.h"
#include "SFramework/TSModelBuilder.h"
#include "SFramework/TSModelFactory.h"
#include "SFramework/TSSignificanceCalculator.h"
#include "SFramework/TSStatisticsCalculator.h"
#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSStatisticsPlotter.h"
#include "SFramework/TSSystematicsManager.h"
#include "SFramework/TSUtils.h"
#include "SFramework/RooFitUtils.h"
#include "SFramework/Run_Freiburg_HWW_Statistics.h"
#include "SFramework/TSBreakdownCalculator.h"
#include "SFramework/TSCLSignificanceEvaluator.h"
#include "SFramework/TSContourScanner.h"
#include "SFramework/TSFitter.h"
#include "SFramework/TSHistogramExporter.h"
#include "SFramework/TSHypothesisTest.h"
#include "SFramework/TSImpactCalculator.h"
#include "SFramework/TSIterator.h"
#include "SFramework/TSLikelihoodScanner.h"
#include "SFramework/TSLimitCalculator.h"
#include "SFramework/TSModelBuilder.h"
#include "SFramework/TSModelFactory.h"
#include "SFramework/TSSignificanceCalculator.h"
#include "SFramework/TSStatisticsCalculator.h"
#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSStatisticsPlotter.h"
#include "SFramework/TSSystematicsManager.h"
#include "SFramework/TSUtils.h"

// Header files passed via #pragma extra_include

namespace TSUtils {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *TSUtils_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("TSUtils", 0 /*version*/, "SFramework/TSUtils.h", 96,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &TSUtils_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *TSUtils_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_TSModelFactory(void *p = 0);
   static void *newArray_TSModelFactory(Long_t size, void *p);
   static void delete_TSModelFactory(void *p);
   static void deleteArray_TSModelFactory(void *p);
   static void destruct_TSModelFactory(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSModelFactory*)
   {
      ::TSModelFactory *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSModelFactory >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSModelFactory", ::TSModelFactory::Class_Version(), "SFramework/TSModelFactory.h", 11,
                  typeid(::TSModelFactory), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSModelFactory::Dictionary, isa_proxy, 4,
                  sizeof(::TSModelFactory) );
      instance.SetNew(&new_TSModelFactory);
      instance.SetNewArray(&newArray_TSModelFactory);
      instance.SetDelete(&delete_TSModelFactory);
      instance.SetDeleteArray(&deleteArray_TSModelFactory);
      instance.SetDestructor(&destruct_TSModelFactory);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSModelFactory*)
   {
      return GenerateInitInstanceLocal((::TSModelFactory*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSModelFactory*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TSSystematicsManager_Dictionary();
   static void TSSystematicsManager_TClassManip(TClass*);
   static void *new_TSSystematicsManager(void *p = 0);
   static void *newArray_TSSystematicsManager(Long_t size, void *p);
   static void delete_TSSystematicsManager(void *p);
   static void deleteArray_TSSystematicsManager(void *p);
   static void destruct_TSSystematicsManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSSystematicsManager*)
   {
      ::TSSystematicsManager *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TSSystematicsManager));
      static ::ROOT::TGenericClassInfo 
         instance("TSSystematicsManager", "SFramework/TSSystematicsManager.h", 7,
                  typeid(::TSSystematicsManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TSSystematicsManager_Dictionary, isa_proxy, 4,
                  sizeof(::TSSystematicsManager) );
      instance.SetNew(&new_TSSystematicsManager);
      instance.SetNewArray(&newArray_TSSystematicsManager);
      instance.SetDelete(&delete_TSSystematicsManager);
      instance.SetDeleteArray(&deleteArray_TSSystematicsManager);
      instance.SetDestructor(&destruct_TSSystematicsManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSSystematicsManager*)
   {
      return GenerateInitInstanceLocal((::TSSystematicsManager*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSSystematicsManager*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TSSystematicsManager_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TSSystematicsManager*)0x0)->GetClass();
      TSSystematicsManager_TClassManip(theClass);
   return theClass;
   }

   static void TSSystematicsManager_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_TSModelBuilder(void *p = 0);
   static void *newArray_TSModelBuilder(Long_t size, void *p);
   static void delete_TSModelBuilder(void *p);
   static void deleteArray_TSModelBuilder(void *p);
   static void destruct_TSModelBuilder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSModelBuilder*)
   {
      ::TSModelBuilder *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSModelBuilder >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSModelBuilder", ::TSModelBuilder::Class_Version(), "SFramework/TSModelBuilder.h", 10,
                  typeid(::TSModelBuilder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSModelBuilder::Dictionary, isa_proxy, 4,
                  sizeof(::TSModelBuilder) );
      instance.SetNew(&new_TSModelBuilder);
      instance.SetNewArray(&newArray_TSModelBuilder);
      instance.SetDelete(&delete_TSModelBuilder);
      instance.SetDeleteArray(&deleteArray_TSModelBuilder);
      instance.SetDestructor(&destruct_TSModelBuilder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSModelBuilder*)
   {
      return GenerateInitInstanceLocal((::TSModelBuilder*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSModelBuilder*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TSStatisticsCalculator(void *p);
   static void deleteArray_TSStatisticsCalculator(void *p);
   static void destruct_TSStatisticsCalculator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSStatisticsCalculator*)
   {
      ::TSStatisticsCalculator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSStatisticsCalculator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSStatisticsCalculator", ::TSStatisticsCalculator::Class_Version(), "SFramework/TSStatisticsCalculator.h", 27,
                  typeid(::TSStatisticsCalculator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSStatisticsCalculator::Dictionary, isa_proxy, 4,
                  sizeof(::TSStatisticsCalculator) );
      instance.SetDelete(&delete_TSStatisticsCalculator);
      instance.SetDeleteArray(&deleteArray_TSStatisticsCalculator);
      instance.SetDestructor(&destruct_TSStatisticsCalculator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSStatisticsCalculator*)
   {
      return GenerateInitInstanceLocal((::TSStatisticsCalculator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSStatisticsCalculator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TSBreakdownCalculator(void *p);
   static void deleteArray_TSBreakdownCalculator(void *p);
   static void destruct_TSBreakdownCalculator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSBreakdownCalculator*)
   {
      ::TSBreakdownCalculator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSBreakdownCalculator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSBreakdownCalculator", ::TSBreakdownCalculator::Class_Version(), "SFramework/TSBreakdownCalculator.h", 8,
                  typeid(::TSBreakdownCalculator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSBreakdownCalculator::Dictionary, isa_proxy, 4,
                  sizeof(::TSBreakdownCalculator) );
      instance.SetDelete(&delete_TSBreakdownCalculator);
      instance.SetDeleteArray(&deleteArray_TSBreakdownCalculator);
      instance.SetDestructor(&destruct_TSBreakdownCalculator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSBreakdownCalculator*)
   {
      return GenerateInitInstanceLocal((::TSBreakdownCalculator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSBreakdownCalculator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSStatisticsManager(void *p = 0);
   static void *newArray_TSStatisticsManager(Long_t size, void *p);
   static void delete_TSStatisticsManager(void *p);
   static void deleteArray_TSStatisticsManager(void *p);
   static void destruct_TSStatisticsManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSStatisticsManager*)
   {
      ::TSStatisticsManager *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSStatisticsManager >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSStatisticsManager", ::TSStatisticsManager::Class_Version(), "SFramework/TSStatisticsManager.h", 13,
                  typeid(::TSStatisticsManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSStatisticsManager::Dictionary, isa_proxy, 4,
                  sizeof(::TSStatisticsManager) );
      instance.SetNew(&new_TSStatisticsManager);
      instance.SetNewArray(&newArray_TSStatisticsManager);
      instance.SetDelete(&delete_TSStatisticsManager);
      instance.SetDeleteArray(&deleteArray_TSStatisticsManager);
      instance.SetDestructor(&destruct_TSStatisticsManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSStatisticsManager*)
   {
      return GenerateInitInstanceLocal((::TSStatisticsManager*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSStatisticsManager*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSCLSignificanceEvaluator(void *p = 0);
   static void *newArray_TSCLSignificanceEvaluator(Long_t size, void *p);
   static void delete_TSCLSignificanceEvaluator(void *p);
   static void deleteArray_TSCLSignificanceEvaluator(void *p);
   static void destruct_TSCLSignificanceEvaluator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSCLSignificanceEvaluator*)
   {
      ::TSCLSignificanceEvaluator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSCLSignificanceEvaluator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSCLSignificanceEvaluator", ::TSCLSignificanceEvaluator::Class_Version(), "SFramework/TSCLSignificanceEvaluator.h", 10,
                  typeid(::TSCLSignificanceEvaluator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSCLSignificanceEvaluator::Dictionary, isa_proxy, 4,
                  sizeof(::TSCLSignificanceEvaluator) );
      instance.SetNew(&new_TSCLSignificanceEvaluator);
      instance.SetNewArray(&newArray_TSCLSignificanceEvaluator);
      instance.SetDelete(&delete_TSCLSignificanceEvaluator);
      instance.SetDeleteArray(&deleteArray_TSCLSignificanceEvaluator);
      instance.SetDestructor(&destruct_TSCLSignificanceEvaluator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSCLSignificanceEvaluator*)
   {
      return GenerateInitInstanceLocal((::TSCLSignificanceEvaluator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSCLSignificanceEvaluator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TSContourScanner(void *p);
   static void deleteArray_TSContourScanner(void *p);
   static void destruct_TSContourScanner(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSContourScanner*)
   {
      ::TSContourScanner *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSContourScanner >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSContourScanner", ::TSContourScanner::Class_Version(), "SFramework/TSContourScanner.h", 11,
                  typeid(::TSContourScanner), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSContourScanner::Dictionary, isa_proxy, 4,
                  sizeof(::TSContourScanner) );
      instance.SetDelete(&delete_TSContourScanner);
      instance.SetDeleteArray(&deleteArray_TSContourScanner);
      instance.SetDestructor(&destruct_TSContourScanner);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSContourScanner*)
   {
      return GenerateInitInstanceLocal((::TSContourScanner*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSContourScanner*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSFitter(void *p = 0);
   static void *newArray_TSFitter(Long_t size, void *p);
   static void delete_TSFitter(void *p);
   static void deleteArray_TSFitter(void *p);
   static void destruct_TSFitter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSFitter*)
   {
      ::TSFitter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSFitter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSFitter", ::TSFitter::Class_Version(), "SFramework/TSFitter.h", 10,
                  typeid(::TSFitter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSFitter::Dictionary, isa_proxy, 4,
                  sizeof(::TSFitter) );
      instance.SetNew(&new_TSFitter);
      instance.SetNewArray(&newArray_TSFitter);
      instance.SetDelete(&delete_TSFitter);
      instance.SetDeleteArray(&deleteArray_TSFitter);
      instance.SetDestructor(&destruct_TSFitter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSFitter*)
   {
      return GenerateInitInstanceLocal((::TSFitter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSFitter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_RooExpectedEvents(void *p);
   static void deleteArray_RooExpectedEvents(void *p);
   static void destruct_RooExpectedEvents(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooExpectedEvents*)
   {
      ::RooExpectedEvents *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooExpectedEvents >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooExpectedEvents", ::RooExpectedEvents::Class_Version(), "SFramework/TSHistogramExporter.h", 32,
                  typeid(::RooExpectedEvents), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooExpectedEvents::Dictionary, isa_proxy, 4,
                  sizeof(::RooExpectedEvents) );
      instance.SetDelete(&delete_RooExpectedEvents);
      instance.SetDeleteArray(&deleteArray_RooExpectedEvents);
      instance.SetDestructor(&destruct_RooExpectedEvents);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooExpectedEvents*)
   {
      return GenerateInitInstanceLocal((::RooExpectedEvents*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RooExpectedEvents*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSHistogramExporter(void *p = 0);
   static void *newArray_TSHistogramExporter(Long_t size, void *p);
   static void delete_TSHistogramExporter(void *p);
   static void deleteArray_TSHistogramExporter(void *p);
   static void destruct_TSHistogramExporter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSHistogramExporter*)
   {
      ::TSHistogramExporter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSHistogramExporter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSHistogramExporter", ::TSHistogramExporter::Class_Version(), "SFramework/TSHistogramExporter.h", 89,
                  typeid(::TSHistogramExporter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSHistogramExporter::Dictionary, isa_proxy, 4,
                  sizeof(::TSHistogramExporter) );
      instance.SetNew(&new_TSHistogramExporter);
      instance.SetNewArray(&newArray_TSHistogramExporter);
      instance.SetDelete(&delete_TSHistogramExporter);
      instance.SetDeleteArray(&deleteArray_TSHistogramExporter);
      instance.SetDestructor(&destruct_TSHistogramExporter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSHistogramExporter*)
   {
      return GenerateInitInstanceLocal((::TSHistogramExporter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSHistogramExporter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TSHypothesisTest(void *p);
   static void deleteArray_TSHypothesisTest(void *p);
   static void destruct_TSHypothesisTest(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSHypothesisTest*)
   {
      ::TSHypothesisTest *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSHypothesisTest >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSHypothesisTest", ::TSHypothesisTest::Class_Version(), "SFramework/TSHypothesisTest.h", 7,
                  typeid(::TSHypothesisTest), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSHypothesisTest::Dictionary, isa_proxy, 4,
                  sizeof(::TSHypothesisTest) );
      instance.SetDelete(&delete_TSHypothesisTest);
      instance.SetDeleteArray(&deleteArray_TSHypothesisTest);
      instance.SetDestructor(&destruct_TSHypothesisTest);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSHypothesisTest*)
   {
      return GenerateInitInstanceLocal((::TSHypothesisTest*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSHypothesisTest*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TSImpactCalculator(void *p);
   static void deleteArray_TSImpactCalculator(void *p);
   static void destruct_TSImpactCalculator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSImpactCalculator*)
   {
      ::TSImpactCalculator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSImpactCalculator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSImpactCalculator", ::TSImpactCalculator::Class_Version(), "SFramework/TSImpactCalculator.h", 8,
                  typeid(::TSImpactCalculator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSImpactCalculator::Dictionary, isa_proxy, 4,
                  sizeof(::TSImpactCalculator) );
      instance.SetDelete(&delete_TSImpactCalculator);
      instance.SetDeleteArray(&deleteArray_TSImpactCalculator);
      instance.SetDestructor(&destruct_TSImpactCalculator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSImpactCalculator*)
   {
      return GenerateInitInstanceLocal((::TSImpactCalculator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSImpactCalculator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlERooWorkspacegR_Dictionary();
   static void TQIteratorTlERooWorkspacegR_TClassManip(TClass*);
   static void *new_TQIteratorTlERooWorkspacegR(void *p = 0);
   static void *newArray_TQIteratorTlERooWorkspacegR(Long_t size, void *p);
   static void delete_TQIteratorTlERooWorkspacegR(void *p);
   static void deleteArray_TQIteratorTlERooWorkspacegR(void *p);
   static void destruct_TQIteratorTlERooWorkspacegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<RooWorkspace>*)
   {
      ::TQIteratorT<RooWorkspace> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<RooWorkspace> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<RooWorkspace>", ::TQIteratorT<RooWorkspace>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<RooWorkspace>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlERooWorkspacegR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<RooWorkspace>) );
      instance.SetNew(&new_TQIteratorTlERooWorkspacegR);
      instance.SetNewArray(&newArray_TQIteratorTlERooWorkspacegR);
      instance.SetDelete(&delete_TQIteratorTlERooWorkspacegR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlERooWorkspacegR);
      instance.SetDestructor(&destruct_TQIteratorTlERooWorkspacegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<RooWorkspace>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<RooWorkspace>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<RooWorkspace>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlERooWorkspacegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<RooWorkspace>*)0x0)->GetClass();
      TQIteratorTlERooWorkspacegR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlERooWorkspacegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlERooRealVargR_Dictionary();
   static void TQIteratorTlERooRealVargR_TClassManip(TClass*);
   static void *new_TQIteratorTlERooRealVargR(void *p = 0);
   static void *newArray_TQIteratorTlERooRealVargR(Long_t size, void *p);
   static void delete_TQIteratorTlERooRealVargR(void *p);
   static void deleteArray_TQIteratorTlERooRealVargR(void *p);
   static void destruct_TQIteratorTlERooRealVargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<RooRealVar>*)
   {
      ::TQIteratorT<RooRealVar> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<RooRealVar> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<RooRealVar>", ::TQIteratorT<RooRealVar>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<RooRealVar>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlERooRealVargR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<RooRealVar>) );
      instance.SetNew(&new_TQIteratorTlERooRealVargR);
      instance.SetNewArray(&newArray_TQIteratorTlERooRealVargR);
      instance.SetDelete(&delete_TQIteratorTlERooRealVargR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlERooRealVargR);
      instance.SetDestructor(&destruct_TQIteratorTlERooRealVargR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<RooRealVar>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<RooRealVar>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<RooRealVar>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlERooRealVargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<RooRealVar>*)0x0)->GetClass();
      TQIteratorTlERooRealVargR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlERooRealVargR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_TSLikelihoodScanner(void *p);
   static void deleteArray_TSLikelihoodScanner(void *p);
   static void destruct_TSLikelihoodScanner(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSLikelihoodScanner*)
   {
      ::TSLikelihoodScanner *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSLikelihoodScanner >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSLikelihoodScanner", ::TSLikelihoodScanner::Class_Version(), "SFramework/TSLikelihoodScanner.h", 9,
                  typeid(::TSLikelihoodScanner), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSLikelihoodScanner::Dictionary, isa_proxy, 4,
                  sizeof(::TSLikelihoodScanner) );
      instance.SetDelete(&delete_TSLikelihoodScanner);
      instance.SetDeleteArray(&deleteArray_TSLikelihoodScanner);
      instance.SetDestructor(&destruct_TSLikelihoodScanner);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSLikelihoodScanner*)
   {
      return GenerateInitInstanceLocal((::TSLikelihoodScanner*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSLikelihoodScanner*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSLimitCalculator(void *p = 0);
   static void *newArray_TSLimitCalculator(Long_t size, void *p);
   static void delete_TSLimitCalculator(void *p);
   static void deleteArray_TSLimitCalculator(void *p);
   static void destruct_TSLimitCalculator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSLimitCalculator*)
   {
      ::TSLimitCalculator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSLimitCalculator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSLimitCalculator", ::TSLimitCalculator::Class_Version(), "SFramework/TSLimitCalculator.h", 10,
                  typeid(::TSLimitCalculator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSLimitCalculator::Dictionary, isa_proxy, 4,
                  sizeof(::TSLimitCalculator) );
      instance.SetNew(&new_TSLimitCalculator);
      instance.SetNewArray(&newArray_TSLimitCalculator);
      instance.SetDelete(&delete_TSLimitCalculator);
      instance.SetDeleteArray(&deleteArray_TSLimitCalculator);
      instance.SetDestructor(&destruct_TSLimitCalculator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSLimitCalculator*)
   {
      return GenerateInitInstanceLocal((::TSLimitCalculator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSLimitCalculator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSSignificanceCalculator(void *p = 0);
   static void *newArray_TSSignificanceCalculator(Long_t size, void *p);
   static void delete_TSSignificanceCalculator(void *p);
   static void deleteArray_TSSignificanceCalculator(void *p);
   static void destruct_TSSignificanceCalculator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSSignificanceCalculator*)
   {
      ::TSSignificanceCalculator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSSignificanceCalculator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSSignificanceCalculator", ::TSSignificanceCalculator::Class_Version(), "SFramework/TSSignificanceCalculator.h", 10,
                  typeid(::TSSignificanceCalculator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSSignificanceCalculator::Dictionary, isa_proxy, 4,
                  sizeof(::TSSignificanceCalculator) );
      instance.SetNew(&new_TSSignificanceCalculator);
      instance.SetNewArray(&newArray_TSSignificanceCalculator);
      instance.SetDelete(&delete_TSSignificanceCalculator);
      instance.SetDeleteArray(&deleteArray_TSSignificanceCalculator);
      instance.SetDestructor(&destruct_TSSignificanceCalculator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSSignificanceCalculator*)
   {
      return GenerateInitInstanceLocal((::TSSignificanceCalculator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSSignificanceCalculator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSStatisticsPlotter(void *p = 0);
   static void *newArray_TSStatisticsPlotter(Long_t size, void *p);
   static void delete_TSStatisticsPlotter(void *p);
   static void deleteArray_TSStatisticsPlotter(void *p);
   static void destruct_TSStatisticsPlotter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSStatisticsPlotter*)
   {
      ::TSStatisticsPlotter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSStatisticsPlotter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSStatisticsPlotter", ::TSStatisticsPlotter::Class_Version(), "SFramework/TSStatisticsPlotter.h", 14,
                  typeid(::TSStatisticsPlotter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSStatisticsPlotter::Dictionary, isa_proxy, 4,
                  sizeof(::TSStatisticsPlotter) );
      instance.SetNew(&new_TSStatisticsPlotter);
      instance.SetNewArray(&newArray_TSStatisticsPlotter);
      instance.SetDelete(&delete_TSStatisticsPlotter);
      instance.SetDeleteArray(&deleteArray_TSStatisticsPlotter);
      instance.SetDestructor(&destruct_TSStatisticsPlotter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSStatisticsPlotter*)
   {
      return GenerateInitInstanceLocal((::TSStatisticsPlotter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSStatisticsPlotter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TSModelFactory::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSModelFactory::Class_Name()
{
   return "TSModelFactory";
}

//______________________________________________________________________________
const char *TSModelFactory::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSModelFactory*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSModelFactory::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSModelFactory*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSModelFactory::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSModelFactory*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSModelFactory::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSModelFactory*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSModelBuilder::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSModelBuilder::Class_Name()
{
   return "TSModelBuilder";
}

//______________________________________________________________________________
const char *TSModelBuilder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSModelBuilder*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSModelBuilder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSModelBuilder*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSModelBuilder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSModelBuilder*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSModelBuilder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSModelBuilder*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSStatisticsCalculator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSStatisticsCalculator::Class_Name()
{
   return "TSStatisticsCalculator";
}

//______________________________________________________________________________
const char *TSStatisticsCalculator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSStatisticsCalculator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSStatisticsCalculator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSStatisticsCalculator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSStatisticsCalculator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSStatisticsCalculator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSStatisticsCalculator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSStatisticsCalculator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSBreakdownCalculator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSBreakdownCalculator::Class_Name()
{
   return "TSBreakdownCalculator";
}

//______________________________________________________________________________
const char *TSBreakdownCalculator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSBreakdownCalculator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSBreakdownCalculator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSBreakdownCalculator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSBreakdownCalculator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSBreakdownCalculator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSBreakdownCalculator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSBreakdownCalculator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSStatisticsManager::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSStatisticsManager::Class_Name()
{
   return "TSStatisticsManager";
}

//______________________________________________________________________________
const char *TSStatisticsManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSStatisticsManager*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSStatisticsManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSStatisticsManager*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSStatisticsManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSStatisticsManager*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSStatisticsManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSStatisticsManager*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSCLSignificanceEvaluator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSCLSignificanceEvaluator::Class_Name()
{
   return "TSCLSignificanceEvaluator";
}

//______________________________________________________________________________
const char *TSCLSignificanceEvaluator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCLSignificanceEvaluator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSCLSignificanceEvaluator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCLSignificanceEvaluator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSCLSignificanceEvaluator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCLSignificanceEvaluator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSCLSignificanceEvaluator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCLSignificanceEvaluator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSContourScanner::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSContourScanner::Class_Name()
{
   return "TSContourScanner";
}

//______________________________________________________________________________
const char *TSContourScanner::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSContourScanner*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSContourScanner::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSContourScanner*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSContourScanner::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSContourScanner*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSContourScanner::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSContourScanner*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSFitter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSFitter::Class_Name()
{
   return "TSFitter";
}

//______________________________________________________________________________
const char *TSFitter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSFitter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSFitter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSFitter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSFitter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSFitter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSFitter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSFitter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RooExpectedEvents::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooExpectedEvents::Class_Name()
{
   return "RooExpectedEvents";
}

//______________________________________________________________________________
const char *RooExpectedEvents::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooExpectedEvents*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooExpectedEvents::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooExpectedEvents*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooExpectedEvents::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooExpectedEvents*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooExpectedEvents::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooExpectedEvents*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSHistogramExporter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSHistogramExporter::Class_Name()
{
   return "TSHistogramExporter";
}

//______________________________________________________________________________
const char *TSHistogramExporter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSHistogramExporter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSHistogramExporter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSHistogramExporter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSHistogramExporter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSHistogramExporter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSHistogramExporter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSHistogramExporter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSHypothesisTest::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSHypothesisTest::Class_Name()
{
   return "TSHypothesisTest";
}

//______________________________________________________________________________
const char *TSHypothesisTest::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSHypothesisTest*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSHypothesisTest::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSHypothesisTest*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSHypothesisTest::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSHypothesisTest*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSHypothesisTest::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSHypothesisTest*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSImpactCalculator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSImpactCalculator::Class_Name()
{
   return "TSImpactCalculator";
}

//______________________________________________________________________________
const char *TSImpactCalculator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSImpactCalculator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSImpactCalculator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSImpactCalculator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSImpactCalculator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSImpactCalculator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSImpactCalculator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSImpactCalculator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<RooWorkspace>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<RooWorkspace>::Class_Name()
{
   return "TQIteratorT<RooWorkspace>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<RooWorkspace>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<RooWorkspace>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<RooWorkspace>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<RooWorkspace>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<RooWorkspace>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<RooWorkspace>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<RooWorkspace>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<RooWorkspace>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<RooRealVar>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<RooRealVar>::Class_Name()
{
   return "TQIteratorT<RooRealVar>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<RooRealVar>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<RooRealVar>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<RooRealVar>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<RooRealVar>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<RooRealVar>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<RooRealVar>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<RooRealVar>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<RooRealVar>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSLikelihoodScanner::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSLikelihoodScanner::Class_Name()
{
   return "TSLikelihoodScanner";
}

//______________________________________________________________________________
const char *TSLikelihoodScanner::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSLikelihoodScanner*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSLikelihoodScanner::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSLikelihoodScanner*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSLikelihoodScanner::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSLikelihoodScanner*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSLikelihoodScanner::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSLikelihoodScanner*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSLimitCalculator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSLimitCalculator::Class_Name()
{
   return "TSLimitCalculator";
}

//______________________________________________________________________________
const char *TSLimitCalculator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSLimitCalculator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSLimitCalculator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSLimitCalculator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSLimitCalculator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSLimitCalculator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSLimitCalculator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSLimitCalculator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSSignificanceCalculator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSSignificanceCalculator::Class_Name()
{
   return "TSSignificanceCalculator";
}

//______________________________________________________________________________
const char *TSSignificanceCalculator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSSignificanceCalculator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSSignificanceCalculator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSSignificanceCalculator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSSignificanceCalculator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSSignificanceCalculator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSSignificanceCalculator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSSignificanceCalculator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSStatisticsPlotter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSStatisticsPlotter::Class_Name()
{
   return "TSStatisticsPlotter";
}

//______________________________________________________________________________
const char *TSStatisticsPlotter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSStatisticsPlotter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSStatisticsPlotter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSStatisticsPlotter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSStatisticsPlotter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSStatisticsPlotter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSStatisticsPlotter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSStatisticsPlotter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TSModelFactory::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSModelFactory.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSModelFactory::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSModelFactory::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSModelFactory(void *p) {
      return  p ? new(p) ::TSModelFactory : new ::TSModelFactory;
   }
   static void *newArray_TSModelFactory(Long_t nElements, void *p) {
      return p ? new(p) ::TSModelFactory[nElements] : new ::TSModelFactory[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSModelFactory(void *p) {
      delete ((::TSModelFactory*)p);
   }
   static void deleteArray_TSModelFactory(void *p) {
      delete [] ((::TSModelFactory*)p);
   }
   static void destruct_TSModelFactory(void *p) {
      typedef ::TSModelFactory current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSModelFactory

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSSystematicsManager(void *p) {
      return  p ? new(p) ::TSSystematicsManager : new ::TSSystematicsManager;
   }
   static void *newArray_TSSystematicsManager(Long_t nElements, void *p) {
      return p ? new(p) ::TSSystematicsManager[nElements] : new ::TSSystematicsManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSSystematicsManager(void *p) {
      delete ((::TSSystematicsManager*)p);
   }
   static void deleteArray_TSSystematicsManager(void *p) {
      delete [] ((::TSSystematicsManager*)p);
   }
   static void destruct_TSSystematicsManager(void *p) {
      typedef ::TSSystematicsManager current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSSystematicsManager

//______________________________________________________________________________
void TSModelBuilder::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSModelBuilder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSModelBuilder::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSModelBuilder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSModelBuilder(void *p) {
      return  p ? new(p) ::TSModelBuilder : new ::TSModelBuilder;
   }
   static void *newArray_TSModelBuilder(Long_t nElements, void *p) {
      return p ? new(p) ::TSModelBuilder[nElements] : new ::TSModelBuilder[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSModelBuilder(void *p) {
      delete ((::TSModelBuilder*)p);
   }
   static void deleteArray_TSModelBuilder(void *p) {
      delete [] ((::TSModelBuilder*)p);
   }
   static void destruct_TSModelBuilder(void *p) {
      typedef ::TSModelBuilder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSModelBuilder

//______________________________________________________________________________
void TSStatisticsCalculator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSStatisticsCalculator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSStatisticsCalculator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSStatisticsCalculator::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TSStatisticsCalculator(void *p) {
      delete ((::TSStatisticsCalculator*)p);
   }
   static void deleteArray_TSStatisticsCalculator(void *p) {
      delete [] ((::TSStatisticsCalculator*)p);
   }
   static void destruct_TSStatisticsCalculator(void *p) {
      typedef ::TSStatisticsCalculator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSStatisticsCalculator

//______________________________________________________________________________
void TSBreakdownCalculator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSBreakdownCalculator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSBreakdownCalculator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSBreakdownCalculator::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TSBreakdownCalculator(void *p) {
      delete ((::TSBreakdownCalculator*)p);
   }
   static void deleteArray_TSBreakdownCalculator(void *p) {
      delete [] ((::TSBreakdownCalculator*)p);
   }
   static void destruct_TSBreakdownCalculator(void *p) {
      typedef ::TSBreakdownCalculator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSBreakdownCalculator

//______________________________________________________________________________
void TSStatisticsManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSStatisticsManager.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSStatisticsManager::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSStatisticsManager::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSStatisticsManager(void *p) {
      return  p ? new(p) ::TSStatisticsManager : new ::TSStatisticsManager;
   }
   static void *newArray_TSStatisticsManager(Long_t nElements, void *p) {
      return p ? new(p) ::TSStatisticsManager[nElements] : new ::TSStatisticsManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSStatisticsManager(void *p) {
      delete ((::TSStatisticsManager*)p);
   }
   static void deleteArray_TSStatisticsManager(void *p) {
      delete [] ((::TSStatisticsManager*)p);
   }
   static void destruct_TSStatisticsManager(void *p) {
      typedef ::TSStatisticsManager current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSStatisticsManager

//______________________________________________________________________________
void TSCLSignificanceEvaluator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSCLSignificanceEvaluator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSCLSignificanceEvaluator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSCLSignificanceEvaluator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSCLSignificanceEvaluator(void *p) {
      return  p ? new(p) ::TSCLSignificanceEvaluator : new ::TSCLSignificanceEvaluator;
   }
   static void *newArray_TSCLSignificanceEvaluator(Long_t nElements, void *p) {
      return p ? new(p) ::TSCLSignificanceEvaluator[nElements] : new ::TSCLSignificanceEvaluator[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSCLSignificanceEvaluator(void *p) {
      delete ((::TSCLSignificanceEvaluator*)p);
   }
   static void deleteArray_TSCLSignificanceEvaluator(void *p) {
      delete [] ((::TSCLSignificanceEvaluator*)p);
   }
   static void destruct_TSCLSignificanceEvaluator(void *p) {
      typedef ::TSCLSignificanceEvaluator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSCLSignificanceEvaluator

//______________________________________________________________________________
void TSContourScanner::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSContourScanner.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSContourScanner::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSContourScanner::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TSContourScanner(void *p) {
      delete ((::TSContourScanner*)p);
   }
   static void deleteArray_TSContourScanner(void *p) {
      delete [] ((::TSContourScanner*)p);
   }
   static void destruct_TSContourScanner(void *p) {
      typedef ::TSContourScanner current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSContourScanner

//______________________________________________________________________________
void TSFitter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSFitter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSFitter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSFitter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSFitter(void *p) {
      return  p ? new(p) ::TSFitter : new ::TSFitter;
   }
   static void *newArray_TSFitter(Long_t nElements, void *p) {
      return p ? new(p) ::TSFitter[nElements] : new ::TSFitter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSFitter(void *p) {
      delete ((::TSFitter*)p);
   }
   static void deleteArray_TSFitter(void *p) {
      delete [] ((::TSFitter*)p);
   }
   static void destruct_TSFitter(void *p) {
      typedef ::TSFitter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSFitter

//______________________________________________________________________________
void RooExpectedEvents::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooExpectedEvents.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RooExpectedEvents::Class(),this);
   } else {
      R__b.WriteClassBuffer(RooExpectedEvents::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_RooExpectedEvents(void *p) {
      delete ((::RooExpectedEvents*)p);
   }
   static void deleteArray_RooExpectedEvents(void *p) {
      delete [] ((::RooExpectedEvents*)p);
   }
   static void destruct_RooExpectedEvents(void *p) {
      typedef ::RooExpectedEvents current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RooExpectedEvents

//______________________________________________________________________________
void TSHistogramExporter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSHistogramExporter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSHistogramExporter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSHistogramExporter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSHistogramExporter(void *p) {
      return  p ? new(p) ::TSHistogramExporter : new ::TSHistogramExporter;
   }
   static void *newArray_TSHistogramExporter(Long_t nElements, void *p) {
      return p ? new(p) ::TSHistogramExporter[nElements] : new ::TSHistogramExporter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSHistogramExporter(void *p) {
      delete ((::TSHistogramExporter*)p);
   }
   static void deleteArray_TSHistogramExporter(void *p) {
      delete [] ((::TSHistogramExporter*)p);
   }
   static void destruct_TSHistogramExporter(void *p) {
      typedef ::TSHistogramExporter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSHistogramExporter

//______________________________________________________________________________
void TSHypothesisTest::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSHypothesisTest.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSHypothesisTest::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSHypothesisTest::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TSHypothesisTest(void *p) {
      delete ((::TSHypothesisTest*)p);
   }
   static void deleteArray_TSHypothesisTest(void *p) {
      delete [] ((::TSHypothesisTest*)p);
   }
   static void destruct_TSHypothesisTest(void *p) {
      typedef ::TSHypothesisTest current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSHypothesisTest

//______________________________________________________________________________
void TSImpactCalculator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSImpactCalculator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSImpactCalculator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSImpactCalculator::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TSImpactCalculator(void *p) {
      delete ((::TSImpactCalculator*)p);
   }
   static void deleteArray_TSImpactCalculator(void *p) {
      delete [] ((::TSImpactCalculator*)p);
   }
   static void destruct_TSImpactCalculator(void *p) {
      typedef ::TSImpactCalculator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSImpactCalculator

//______________________________________________________________________________
template <> void TQIteratorT<RooWorkspace>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<RooWorkspace>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<RooWorkspace>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<RooWorkspace>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlERooWorkspacegR(void *p) {
      return  p ? new(p) ::TQIteratorT<RooWorkspace> : new ::TQIteratorT<RooWorkspace>;
   }
   static void *newArray_TQIteratorTlERooWorkspacegR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<RooWorkspace>[nElements] : new ::TQIteratorT<RooWorkspace>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlERooWorkspacegR(void *p) {
      delete ((::TQIteratorT<RooWorkspace>*)p);
   }
   static void deleteArray_TQIteratorTlERooWorkspacegR(void *p) {
      delete [] ((::TQIteratorT<RooWorkspace>*)p);
   }
   static void destruct_TQIteratorTlERooWorkspacegR(void *p) {
      typedef ::TQIteratorT<RooWorkspace> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<RooWorkspace>

//______________________________________________________________________________
template <> void TQIteratorT<RooRealVar>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<RooRealVar>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<RooRealVar>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<RooRealVar>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlERooRealVargR(void *p) {
      return  p ? new(p) ::TQIteratorT<RooRealVar> : new ::TQIteratorT<RooRealVar>;
   }
   static void *newArray_TQIteratorTlERooRealVargR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<RooRealVar>[nElements] : new ::TQIteratorT<RooRealVar>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlERooRealVargR(void *p) {
      delete ((::TQIteratorT<RooRealVar>*)p);
   }
   static void deleteArray_TQIteratorTlERooRealVargR(void *p) {
      delete [] ((::TQIteratorT<RooRealVar>*)p);
   }
   static void destruct_TQIteratorTlERooRealVargR(void *p) {
      typedef ::TQIteratorT<RooRealVar> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<RooRealVar>

//______________________________________________________________________________
void TSLikelihoodScanner::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSLikelihoodScanner.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSLikelihoodScanner::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSLikelihoodScanner::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TSLikelihoodScanner(void *p) {
      delete ((::TSLikelihoodScanner*)p);
   }
   static void deleteArray_TSLikelihoodScanner(void *p) {
      delete [] ((::TSLikelihoodScanner*)p);
   }
   static void destruct_TSLikelihoodScanner(void *p) {
      typedef ::TSLikelihoodScanner current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSLikelihoodScanner

//______________________________________________________________________________
void TSLimitCalculator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSLimitCalculator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSLimitCalculator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSLimitCalculator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSLimitCalculator(void *p) {
      return  p ? new(p) ::TSLimitCalculator : new ::TSLimitCalculator;
   }
   static void *newArray_TSLimitCalculator(Long_t nElements, void *p) {
      return p ? new(p) ::TSLimitCalculator[nElements] : new ::TSLimitCalculator[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSLimitCalculator(void *p) {
      delete ((::TSLimitCalculator*)p);
   }
   static void deleteArray_TSLimitCalculator(void *p) {
      delete [] ((::TSLimitCalculator*)p);
   }
   static void destruct_TSLimitCalculator(void *p) {
      typedef ::TSLimitCalculator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSLimitCalculator

//______________________________________________________________________________
void TSSignificanceCalculator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSSignificanceCalculator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSSignificanceCalculator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSSignificanceCalculator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSSignificanceCalculator(void *p) {
      return  p ? new(p) ::TSSignificanceCalculator : new ::TSSignificanceCalculator;
   }
   static void *newArray_TSSignificanceCalculator(Long_t nElements, void *p) {
      return p ? new(p) ::TSSignificanceCalculator[nElements] : new ::TSSignificanceCalculator[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSSignificanceCalculator(void *p) {
      delete ((::TSSignificanceCalculator*)p);
   }
   static void deleteArray_TSSignificanceCalculator(void *p) {
      delete [] ((::TSSignificanceCalculator*)p);
   }
   static void destruct_TSSignificanceCalculator(void *p) {
      typedef ::TSSignificanceCalculator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSSignificanceCalculator

//______________________________________________________________________________
void TSStatisticsPlotter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSStatisticsPlotter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSStatisticsPlotter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSStatisticsPlotter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSStatisticsPlotter(void *p) {
      return  p ? new(p) ::TSStatisticsPlotter : new ::TSStatisticsPlotter;
   }
   static void *newArray_TSStatisticsPlotter(Long_t nElements, void *p) {
      return p ? new(p) ::TSStatisticsPlotter[nElements] : new ::TSStatisticsPlotter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSStatisticsPlotter(void *p) {
      delete ((::TSStatisticsPlotter*)p);
   }
   static void deleteArray_TSStatisticsPlotter(void *p) {
      delete [] ((::TSStatisticsPlotter*)p);
   }
   static void destruct_TSStatisticsPlotter(void *p) {
      typedef ::TSStatisticsPlotter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSStatisticsPlotter

namespace {
  void TriggerDictionaryInitialization_libSFramework_Impl() {
    static const char* headers[] = {
"SFramework/RooFitUtils.h",
"SFramework/Run_Freiburg_HWW_Statistics.h",
"SFramework/TSBreakdownCalculator.h",
"SFramework/TSCLSignificanceEvaluator.h",
"SFramework/TSContourScanner.h",
"SFramework/TSFitter.h",
"SFramework/TSHistogramExporter.h",
"SFramework/TSHypothesisTest.h",
"SFramework/TSImpactCalculator.h",
"SFramework/TSIterator.h",
"SFramework/TSLikelihoodScanner.h",
"SFramework/TSLimitCalculator.h",
"SFramework/TSModelBuilder.h",
"SFramework/TSModelFactory.h",
"SFramework/TSSignificanceCalculator.h",
"SFramework/TSStatisticsCalculator.h",
"SFramework/TSStatisticsManager.h",
"SFramework/TSStatisticsPlotter.h",
"SFramework/TSSystematicsManager.h",
"SFramework/TSUtils.h",
0
    };
    static const char* includePaths[] = {
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/SFramework",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/SFramework",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/SFramework",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/SFramework",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/SFramework",
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
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/SFramework",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/SFramework/CMakeFiles/makeSFrameworkCintDict.PfT7Jq/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libSFramework dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$SFramework/Run_Freiburg_HWW_Statistics.h")))  TSModelFactory;
class __attribute__((annotate("$clingAutoload$SFramework/TSSystematicsManager.h")))  __attribute__((annotate("$clingAutoload$SFramework/Run_Freiburg_HWW_Statistics.h")))  TSSystematicsManager;
class __attribute__((annotate("$clingAutoload$SFramework/Run_Freiburg_HWW_Statistics.h")))  TSModelBuilder;
class __attribute__((annotate("$clingAutoload$SFramework/TSBreakdownCalculator.h")))  TSStatisticsCalculator;
class __attribute__((annotate("$clingAutoload$SFramework/TSBreakdownCalculator.h")))  TSBreakdownCalculator;
class __attribute__((annotate("$clingAutoload$SFramework/TSCLSignificanceEvaluator.h")))  TSStatisticsManager;
class __attribute__((annotate("$clingAutoload$SFramework/TSCLSignificanceEvaluator.h")))  TSCLSignificanceEvaluator;
class __attribute__((annotate("$clingAutoload$SFramework/TSContourScanner.h")))  TSContourScanner;
class __attribute__((annotate("$clingAutoload$SFramework/TSFitter.h")))  TSFitter;
class __attribute__((annotate("$clingAutoload$SFramework/TSHistogramExporter.h")))  RooExpectedEvents;
class __attribute__((annotate("$clingAutoload$SFramework/TSHistogramExporter.h")))  TSHistogramExporter;
class __attribute__((annotate("$clingAutoload$SFramework/TSHypothesisTest.h")))  TSHypothesisTest;
class __attribute__((annotate("$clingAutoload$SFramework/TSImpactCalculator.h")))  TSImpactCalculator;
class __attribute__((annotate("$clingAutoload$RooWorkspace.h")))  __attribute__((annotate("$clingAutoload$SFramework/Run_Freiburg_HWW_Statistics.h")))  RooWorkspace;
template <class T> class __attribute__((annotate("$clingAutoload$QFramework/TQIteratorT.h")))  __attribute__((annotate("$clingAutoload$SFramework/TSIterator.h")))  TQIteratorT;

class __attribute__((annotate("$clingAutoload$RooRealVar.h")))  __attribute__((annotate("$clingAutoload$SFramework/TSBreakdownCalculator.h")))  RooRealVar;
class __attribute__((annotate("$clingAutoload$SFramework/TSLikelihoodScanner.h")))  TSLikelihoodScanner;
class __attribute__((annotate("$clingAutoload$SFramework/TSLimitCalculator.h")))  TSLimitCalculator;
class __attribute__((annotate("$clingAutoload$SFramework/TSSignificanceCalculator.h")))  TSSignificanceCalculator;
class __attribute__((annotate("$clingAutoload$SFramework/TSStatisticsPlotter.h")))  TSStatisticsPlotter;
typedef TQIteratorT<RooWorkspace> RooWorkspaceIterator __attribute__((annotate("$clingAutoload$SFramework/TSIterator.h"))) ;
typedef TQIteratorT<RooRealVar> RooRealVarIterator __attribute__((annotate("$clingAutoload$SFramework/TSIterator.h"))) ;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libSFramework dictionary payload"

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
  #define PACKAGE_VERSION "SFramework-00-00-00"
#endif
#ifndef PACKAGE_VERSION_UQ
  #define PACKAGE_VERSION_UQ SFramework-00-00-00
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "SFramework/RooFitUtils.h"
#include "SFramework/Run_Freiburg_HWW_Statistics.h"
#include "SFramework/TSBreakdownCalculator.h"
#include "SFramework/TSCLSignificanceEvaluator.h"
#include "SFramework/TSContourScanner.h"
#include "SFramework/TSFitter.h"
#include "SFramework/TSHistogramExporter.h"
#include "SFramework/TSHypothesisTest.h"
#include "SFramework/TSImpactCalculator.h"
#include "SFramework/TSIterator.h"
#include "SFramework/TSLikelihoodScanner.h"
#include "SFramework/TSLimitCalculator.h"
#include "SFramework/TSModelBuilder.h"
#include "SFramework/TSModelFactory.h"
#include "SFramework/TSSignificanceCalculator.h"
#include "SFramework/TSStatisticsCalculator.h"
#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSStatisticsPlotter.h"
#include "SFramework/TSSystematicsManager.h"
#include "SFramework/TSUtils.h"
//this is an automatically generated -*- c++ -*- file - EDITS WILL BE LOST!

#ifndef __SFrameworkDICT__
#define __SFrameworkDICT__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winconsistent-missing-override"

#include "SFramework/RooFitUtils.h"
#include "SFramework/Run_Freiburg_HWW_Statistics.h"
#include "SFramework/TSBreakdownCalculator.h"
#include "SFramework/TSCLSignificanceEvaluator.h"
#include "SFramework/TSContourScanner.h"
#include "SFramework/TSFitter.h"
#include "SFramework/TSHistogramExporter.h"
#include "SFramework/TSHypothesisTest.h"
#include "SFramework/TSImpactCalculator.h"
#include "SFramework/TSIterator.h"
#include "SFramework/TSLikelihoodScanner.h"
#include "SFramework/TSLimitCalculator.h"
#include "SFramework/TSModelBuilder.h"
#include "SFramework/TSModelFactory.h"
#include "SFramework/TSSignificanceCalculator.h"
#include "SFramework/TSStatisticsCalculator.h"
#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSStatisticsPlotter.h"
#include "SFramework/TSSystematicsManager.h"
#include "SFramework/TSUtils.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
// >>> begin RooFitUtils.h
// <<< end RooFitUtils.h

// >>> begin Run_Freiburg_HWW_Statistics.h
// <<< end Run_Freiburg_HWW_Statistics.h

// >>> begin TSBreakdownCalculator.h
#pragma link C++ class TSBreakdownCalculator+;
// <<< end TSBreakdownCalculator.h

// >>> begin TSCLSignificanceEvaluator.h
#pragma link C++ class TSCLSignificanceEvaluator+;
// <<< end TSCLSignificanceEvaluator.h

// >>> begin TSContourScanner.h
#pragma link C++ class TSContourScanner+;
// <<< end TSContourScanner.h

// >>> begin TSFitter.h
#pragma link C++ class TSFitter+;
// <<< end TSFitter.h

// >>> begin TSHistogramExporter.h
#pragma link C++ class RooExpectedEvents+;
#pragma link C++ class TSHistogramExporter+;
// <<< end TSHistogramExporter.h

// >>> begin TSHypothesisTest.h
#pragma link C++ class TSHypothesisTest+;
// <<< end TSHypothesisTest.h

// >>> begin TSImpactCalculator.h
#pragma link C++ class TSImpactCalculator+;
// <<< end TSImpactCalculator.h

// >>> begin TSIterator.h
#pragma link C++ typedef RooWorkspaceIterator;
#pragma link C++ typedef RooRealVarIterator;
//////////////////////-*- mode: c++ -*-////////////////////////////////
#pragma link C++ class TQIteratorT<RooRealVar>+;
#pragma link C++ class TQIteratorT<RooWorkspace>+;
// <<< end TSIterator.h

// >>> begin TSLikelihoodScanner.h
#pragma link C++ class TSLikelihoodScanner+;
// <<< end TSLikelihoodScanner.h

// >>> begin TSLimitCalculator.h
#pragma link C++ class TSLimitCalculator+;
// <<< end TSLimitCalculator.h

// >>> begin TSModelBuilder.h
#pragma link C++ class TSModelBuilder+;
// <<< end TSModelBuilder.h

// >>> begin TSModelFactory.h
#pragma link C++ class TSModelFactory+;
// <<< end TSModelFactory.h

// >>> begin TSSignificanceCalculator.h
#pragma link C++ class TSSignificanceCalculator+;
// <<< end TSSignificanceCalculator.h

// >>> begin TSStatisticsCalculator.h
#pragma link C++ class TSStatisticsCalculator+;
#pragma link C++ class TSStatisticsCalculator+;
// <<< end TSStatisticsCalculator.h

// >>> begin TSStatisticsManager.h
#pragma link C++ class TSStatisticsManager+;
// <<< end TSStatisticsManager.h

// >>> begin TSStatisticsPlotter.h
#pragma link C++ class TSStatisticsPlotter+;
// <<< end TSStatisticsPlotter.h

// >>> begin TSSystematicsManager.h
#pragma link C++ class TSSystematicsManager+;
// <<< end TSSystematicsManager.h

// >>> begin TSUtils.h
#pragma link C++ namespace TSUtils;
// <<< end TSUtils.h


#endif //__CINT__
#endif //__SFrameworkDICT__

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"RooExpectedEvents", payloadCode, "@",
"RooRealVarIterator", payloadCode, "@",
"RooWorkspaceIterator", payloadCode, "@",
"TQIteratorT<RooRealVar>", payloadCode, "@",
"TQIteratorT<RooWorkspace>", payloadCode, "@",
"TSBreakdownCalculator", payloadCode, "@",
"TSCLSignificanceEvaluator", payloadCode, "@",
"TSContourScanner", payloadCode, "@",
"TSFitter", payloadCode, "@",
"TSHistogramExporter", payloadCode, "@",
"TSHypothesisTest", payloadCode, "@",
"TSImpactCalculator", payloadCode, "@",
"TSLikelihoodScanner", payloadCode, "@",
"TSLimitCalculator", payloadCode, "@",
"TSModelBuilder", payloadCode, "@",
"TSModelFactory", payloadCode, "@",
"TSSignificanceCalculator", payloadCode, "@",
"TSStatisticsCalculator", payloadCode, "@",
"TSStatisticsManager", payloadCode, "@",
"TSStatisticsPlotter", payloadCode, "@",
"TSSystematicsManager", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libSFramework",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libSFramework_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libSFramework_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libSFramework() {
  TriggerDictionaryInitialization_libSFramework_Impl();
}
