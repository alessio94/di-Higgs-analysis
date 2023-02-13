// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME QFrameworkCintDict
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

#include "TBuffer.h"
#include "TVirtualObject.h"
#include <vector>
#include "TSchemaHelper.h"


// Header files passed as explicit arguments
#include "QFramework/ASG.h"
#include "QFramework/TQABCDCalculator.h"
#include "QFramework/TQAlgorithm.h"
#include "QFramework/TQAnalysisAlgorithm.h"
#include "QFramework/TQAnalysisJob.h"
#include "QFramework/TQAnalysisSampleVisitor.h"
#include "QFramework/TQAnalysisSampleVisitorBase.h"
#include "QFramework/TQCompPlotter.h"
#include "QFramework/TQCompiledCut.h"
#include "QFramework/TQConfigReader.h"
#include "QFramework/TQConstObservable.h"
#include "QFramework/TQConstTreeObservable.h"
#include "QFramework/TQCounter.h"
#include "QFramework/TQCut.h"
#include "QFramework/TQCutFactory.h"
#include "QFramework/TQCutflowAnalysisJob.h"
#include "QFramework/TQCutflowPlotter.h"
#include "QFramework/TQCutflowPrinter.h"
#include "QFramework/TQDefaultPlotter.h"
#include "QFramework/TQEventIndexObservable.h"
#include "QFramework/TQEventlistAnalysisJob.h"
#include "QFramework/TQEventlistPrinter.h"
#include "QFramework/TQFilterObservable.h"
#include "QFramework/TQFlags.h"
#include "QFramework/TQFolder.h"
#include "QFramework/TQGraphMakerAnalysisJob.h"
#include "QFramework/TQGridScanBound.h"
#include "QFramework/TQGridScanObservable.h"
#include "QFramework/TQGridScanPoint.h"
#include "QFramework/TQGridScanResults.h"
#include "QFramework/TQGridScanStyle.h"
#include "QFramework/TQGridScanner.h"
#include "QFramework/TQHWWPlotter.h"
#include "QFramework/TQHistComparer.h"
#include "QFramework/TQHistoMakerAnalysisJob.h"
#include "QFramework/TQHistogramObservable.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQImportLink.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQIteratorT.h"
#include "QFramework/TQLibrary.h"
#include "QFramework/TQLink.h"
#include "QFramework/TQListUtils.h"
#include "QFramework/TQMVA.h"
#include "QFramework/TQMVAObservable.h"
#include "QFramework/TQMessageStream.h"
#include "QFramework/TQMultiChannelAnalysisSampleVisitor.h"
#include "QFramework/TQMultiObservable.h"
#include "QFramework/TQNFBase.h"
#include "QFramework/TQNFCalculator.h"
#include "QFramework/TQNFChainloader.h"
#include "QFramework/TQNFCustomCalculator.h"
#include "QFramework/TQNFManualSetter.h"
#include "QFramework/TQNFTop0jetEstimator.h"
#include "QFramework/TQNFTop1jetEstimator.h"
#include "QFramework/TQNFUncertaintyScaler.h"
#include "QFramework/TQNTupleDumperAnalysisJob.h"
#include "QFramework/TQNamedTaggable.h"
#include "QFramework/TQObservable.h"
#include "QFramework/TQPCA.h"
#include "QFramework/TQPCAAnalysisJob.h"
#include "QFramework/TQPathManager.h"
#include "QFramework/TQPlotter.h"
#include "QFramework/TQPresenter.h"
#include "QFramework/TQROOTPlotter.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQSampleDataReader.h"
#include "QFramework/TQSampleFolder.h"
#include "QFramework/TQSampleGroupingVisitor.h"
#include "QFramework/TQSampleInitializer.h"
#include "QFramework/TQSampleInitializerBase.h"
#include "QFramework/TQSampleNormalizationObservable.h"
#include "QFramework/TQSamplePurger.h"
#include "QFramework/TQSampleRevisitor.h"
#include "QFramework/TQSampleVisitor.h"
#include "QFramework/TQSignificanceEvaluator.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQSystematics.h"
#include "QFramework/TQSystematicsHandler.h"
#include "QFramework/TQSystematicsManager.h"
#include "QFramework/TQTHnBaseMakerAnalysisJob.h"
#include "QFramework/TQTHnBaseUtils.h"
#include "QFramework/TQTable.h"
#include "QFramework/TQTaggable.h"
#include "QFramework/TQTikZPlotter.h"
#include "QFramework/TQToken.h"
#include "QFramework/TQTreeFormulaObservable.h"
#include "QFramework/TQTreeObservable.h"
#include "QFramework/TQUniqueCut.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQValue.h"
#include "QFramework/TQVectorAuxObservable.h"
#include "QFramework/TQXSecParser.h"
#include "QFramework/ASG.h"
#include "QFramework/TQABCDCalculator.h"
#include "QFramework/TQAlgorithm.h"
#include "QFramework/TQAnalysisAlgorithm.h"
#include "QFramework/TQAnalysisJob.h"
#include "QFramework/TQAnalysisSampleVisitor.h"
#include "QFramework/TQAnalysisSampleVisitorBase.h"
#include "QFramework/TQCompPlotter.h"
#include "QFramework/TQCompiledCut.h"
#include "QFramework/TQConfigReader.h"
#include "QFramework/TQConstObservable.h"
#include "QFramework/TQConstTreeObservable.h"
#include "QFramework/TQCounter.h"
#include "QFramework/TQCut.h"
#include "QFramework/TQCutFactory.h"
#include "QFramework/TQCutflowAnalysisJob.h"
#include "QFramework/TQCutflowPlotter.h"
#include "QFramework/TQCutflowPrinter.h"
#include "QFramework/TQDefaultPlotter.h"
#include "QFramework/TQEventIndexObservable.h"
#include "QFramework/TQEventlistAnalysisJob.h"
#include "QFramework/TQEventlistPrinter.h"
#include "QFramework/TQFilterObservable.h"
#include "QFramework/TQFlags.h"
#include "QFramework/TQFolder.h"
#include "QFramework/TQGraphMakerAnalysisJob.h"
#include "QFramework/TQGridScanBound.h"
#include "QFramework/TQGridScanObservable.h"
#include "QFramework/TQGridScanPoint.h"
#include "QFramework/TQGridScanResults.h"
#include "QFramework/TQGridScanStyle.h"
#include "QFramework/TQGridScanner.h"
#include "QFramework/TQHWWPlotter.h"
#include "QFramework/TQHistComparer.h"
#include "QFramework/TQHistoMakerAnalysisJob.h"
#include "QFramework/TQHistogramObservable.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQImportLink.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQIteratorT.h"
#include "QFramework/TQLibrary.h"
#include "QFramework/TQLink.h"
#include "QFramework/TQListUtils.h"
#include "QFramework/TQMVA.h"
#include "QFramework/TQMVAObservable.h"
#include "QFramework/TQMessageStream.h"
#include "QFramework/TQMultiChannelAnalysisSampleVisitor.h"
#include "QFramework/TQMultiObservable.h"
#include "QFramework/TQNFBase.h"
#include "QFramework/TQNFCalculator.h"
#include "QFramework/TQNFChainloader.h"
#include "QFramework/TQNFCustomCalculator.h"
#include "QFramework/TQNFManualSetter.h"
#include "QFramework/TQNFTop0jetEstimator.h"
#include "QFramework/TQNFTop1jetEstimator.h"
#include "QFramework/TQNFUncertaintyScaler.h"
#include "QFramework/TQNTupleDumperAnalysisJob.h"
#include "QFramework/TQNamedTaggable.h"
#include "QFramework/TQObservable.h"
#include "QFramework/TQPCA.h"
#include "QFramework/TQPCAAnalysisJob.h"
#include "QFramework/TQPathManager.h"
#include "QFramework/TQPlotter.h"
#include "QFramework/TQPresenter.h"
#include "QFramework/TQROOTPlotter.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQSampleDataReader.h"
#include "QFramework/TQSampleFolder.h"
#include "QFramework/TQSampleGroupingVisitor.h"
#include "QFramework/TQSampleInitializer.h"
#include "QFramework/TQSampleInitializerBase.h"
#include "QFramework/TQSampleNormalizationObservable.h"
#include "QFramework/TQSamplePurger.h"
#include "QFramework/TQSampleRevisitor.h"
#include "QFramework/TQSampleVisitor.h"
#include "QFramework/TQSignificanceEvaluator.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQSystematics.h"
#include "QFramework/TQSystematicsHandler.h"
#include "QFramework/TQSystematicsManager.h"
#include "QFramework/TQTHnBaseMakerAnalysisJob.h"
#include "QFramework/TQTHnBaseUtils.h"
#include "QFramework/TQTable.h"
#include "QFramework/TQTaggable.h"
#include "QFramework/TQTikZPlotter.h"
#include "QFramework/TQToken.h"
#include "QFramework/TQTreeFormulaObservable.h"
#include "QFramework/TQTreeObservable.h"
#include "QFramework/TQUniqueCut.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQValue.h"
#include "QFramework/TQVectorAuxObservable.h"
#include "QFramework/TQXSecParser.h"

// Header files passed via #pragma extra_include
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMVA/Factory.h"
#include "TMVA/DataSetInfo.h"

namespace TQStringUtils {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *TQStringUtils_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("TQStringUtils", 0 /*version*/, "QFramework/TQStringUtils.h", 17,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &TQStringUtils_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *TQStringUtils_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace TQListUtils {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *TQListUtils_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("TQListUtils", 0 /*version*/, "QFramework/TQListUtils.h", 11,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &TQListUtils_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *TQListUtils_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

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

namespace TQGridScanStyle {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *TQGridScanStyle_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("TQGridScanStyle", 0 /*version*/, "QFramework/TQGridScanStyle.h", 15,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &TQGridScanStyle_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *TQGridScanStyle_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace TQHistogramUtils {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *TQHistogramUtils_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("TQHistogramUtils", 0 /*version*/, "QFramework/TQHistogramUtils.h", 30,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &TQHistogramUtils_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *TQHistogramUtils_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace TQNTupleDumperAnalysisJobHelpers {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *TQNTupleDumperAnalysisJobHelpers_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("TQNTupleDumperAnalysisJobHelpers", 0 /*version*/, "QFramework/TQNTupleDumperAnalysisJob.h", 18,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &TQNTupleDumperAnalysisJobHelpers_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *TQNTupleDumperAnalysisJobHelpers_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace TQSystematics {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *TQSystematics_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("TQSystematics", 0 /*version*/, "QFramework/TQSystematics.h", 5,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &TQSystematics_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *TQSystematics_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace TQTHnBaseUtils {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *TQTHnBaseUtils_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("TQTHnBaseUtils", 0 /*version*/, "QFramework/TQTHnBaseUtils.h", 15,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &TQTHnBaseUtils_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *TQTHnBaseUtils_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace TQUtils {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *TQUtils_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("TQUtils", 0 /*version*/, "QFramework/TQUtils.h", 26,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &TQUtils_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *TQUtils_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void delete_TQValue(void *p);
   static void deleteArray_TQValue(void *p);
   static void destruct_TQValue(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQValue*)
   {
      ::TQValue *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQValue >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQValue", ::TQValue::Class_Version(), "QFramework/TQValue.h", 8,
                  typeid(::TQValue), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQValue::Dictionary, isa_proxy, 4,
                  sizeof(::TQValue) );
      instance.SetDelete(&delete_TQValue);
      instance.SetDeleteArray(&deleteArray_TQValue);
      instance.SetDestructor(&destruct_TQValue);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQValue*)
   {
      return GenerateInitInstanceLocal((::TQValue*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQValue*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQValueDouble(void *p = 0);
   static void *newArray_TQValueDouble(Long_t size, void *p);
   static void delete_TQValueDouble(void *p);
   static void deleteArray_TQValueDouble(void *p);
   static void destruct_TQValueDouble(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQValueDouble*)
   {
      ::TQValueDouble *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQValueDouble >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQValueDouble", ::TQValueDouble::Class_Version(), "QFramework/TQValue.h", 80,
                  typeid(::TQValueDouble), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQValueDouble::Dictionary, isa_proxy, 4,
                  sizeof(::TQValueDouble) );
      instance.SetNew(&new_TQValueDouble);
      instance.SetNewArray(&newArray_TQValueDouble);
      instance.SetDelete(&delete_TQValueDouble);
      instance.SetDeleteArray(&deleteArray_TQValueDouble);
      instance.SetDestructor(&destruct_TQValueDouble);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQValueDouble*)
   {
      return GenerateInitInstanceLocal((::TQValueDouble*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQValueDouble*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQValueInteger(void *p = 0);
   static void *newArray_TQValueInteger(Long_t size, void *p);
   static void delete_TQValueInteger(void *p);
   static void deleteArray_TQValueInteger(void *p);
   static void destruct_TQValueInteger(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQValueInteger*)
   {
      ::TQValueInteger *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQValueInteger >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQValueInteger", ::TQValueInteger::Class_Version(), "QFramework/TQValue.h", 122,
                  typeid(::TQValueInteger), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQValueInteger::Dictionary, isa_proxy, 4,
                  sizeof(::TQValueInteger) );
      instance.SetNew(&new_TQValueInteger);
      instance.SetNewArray(&newArray_TQValueInteger);
      instance.SetDelete(&delete_TQValueInteger);
      instance.SetDeleteArray(&deleteArray_TQValueInteger);
      instance.SetDestructor(&destruct_TQValueInteger);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQValueInteger*)
   {
      return GenerateInitInstanceLocal((::TQValueInteger*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQValueInteger*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQValueBool(void *p = 0);
   static void *newArray_TQValueBool(Long_t size, void *p);
   static void delete_TQValueBool(void *p);
   static void deleteArray_TQValueBool(void *p);
   static void destruct_TQValueBool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQValueBool*)
   {
      ::TQValueBool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQValueBool >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQValueBool", ::TQValueBool::Class_Version(), "QFramework/TQValue.h", 164,
                  typeid(::TQValueBool), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQValueBool::Dictionary, isa_proxy, 4,
                  sizeof(::TQValueBool) );
      instance.SetNew(&new_TQValueBool);
      instance.SetNewArray(&newArray_TQValueBool);
      instance.SetDelete(&delete_TQValueBool);
      instance.SetDeleteArray(&deleteArray_TQValueBool);
      instance.SetDestructor(&destruct_TQValueBool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQValueBool*)
   {
      return GenerateInitInstanceLocal((::TQValueBool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQValueBool*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQValueString(void *p = 0);
   static void *newArray_TQValueString(Long_t size, void *p);
   static void delete_TQValueString(void *p);
   static void deleteArray_TQValueString(void *p);
   static void destruct_TQValueString(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQValueString*)
   {
      ::TQValueString *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQValueString >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQValueString", ::TQValueString::Class_Version(), "QFramework/TQValue.h", 206,
                  typeid(::TQValueString), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQValueString::Dictionary, isa_proxy, 4,
                  sizeof(::TQValueString) );
      instance.SetNew(&new_TQValueString);
      instance.SetNewArray(&newArray_TQValueString);
      instance.SetDelete(&delete_TQValueString);
      instance.SetDeleteArray(&deleteArray_TQValueString);
      instance.SetDestructor(&destruct_TQValueString);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQValueString*)
   {
      return GenerateInitInstanceLocal((::TQValueString*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQValueString*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQTaggable(void *p = 0);
   static void *newArray_TQTaggable(Long_t size, void *p);
   static void delete_TQTaggable(void *p);
   static void deleteArray_TQTaggable(void *p);
   static void destruct_TQTaggable(void *p);

   // Schema evolution read functions
   static void read_TQTaggable_0( char* target, TVirtualObject *oldObj )
   {
      //--- Automatically generated variables ---
#if 0
      static Int_t id_fTags = oldObj->GetId("fTags");
#endif
      struct TQTaggable_Onfile {
         TList* &fTags;
         TQTaggable_Onfile(TList* &onfile_fTags ): fTags(onfile_fTags) {}
      };
      static Long_t offset_Onfile_TQTaggable_fTags = oldObj->GetClass()->GetDataMemberOffset("fTags");
      char *onfile_add = (char*)oldObj->GetObject();
      TQTaggable_Onfile onfile(
         *(TList**)(onfile_add+offset_Onfile_TQTaggable_fTags) );

      static TClassRef cls("TQTaggable");
      static Long_t offset_fTags = cls->GetDataMemberOffset("fTags");
      TList*& fTags = *(TList**)(target+offset_fTags);
      TQTaggable* newObj = (TQTaggable*)target;
      // Supress warning message.
      (void)oldObj;

      (void)newObj;

      //--- User's code ---
      fTags = TQTaggable::makeListOfTags(onfile.fTags); 
   }

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQTaggable*)
   {
      ::TQTaggable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQTaggable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQTaggable", ::TQTaggable::Class_Version(), "QFramework/TQTaggable.h", 13,
                  typeid(::TQTaggable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQTaggable::Dictionary, isa_proxy, 4,
                  sizeof(::TQTaggable) );
      instance.SetNew(&new_TQTaggable);
      instance.SetNewArray(&newArray_TQTaggable);
      instance.SetDelete(&delete_TQTaggable);
      instance.SetDeleteArray(&deleteArray_TQTaggable);
      instance.SetDestructor(&destruct_TQTaggable);

      ROOT::Internal::TSchemaHelper* rule;

      // the io read rules
      std::vector<ROOT::Internal::TSchemaHelper> readrules(1);
      rule = &readrules[0];
      rule->fSourceClass = "TQTaggable";
      rule->fTarget      = "fTags";
      rule->fSource      = "TList* fTags";
      rule->fFunctionPtr = (void *)TFunc2void( read_TQTaggable_0);
      rule->fCode        = " fTags = TQTaggable::makeListOfTags(onfile.fTags); ";
      rule->fVersion     = "[1]";
      instance.SetReadRules( readrules );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQTaggable*)
   {
      return GenerateInitInstanceLocal((::TQTaggable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQTaggable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQNamedTaggable(void *p = 0);
   static void *newArray_TQNamedTaggable(Long_t size, void *p);
   static void delete_TQNamedTaggable(void *p);
   static void deleteArray_TQNamedTaggable(void *p);
   static void destruct_TQNamedTaggable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQNamedTaggable*)
   {
      ::TQNamedTaggable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQNamedTaggable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQNamedTaggable", ::TQNamedTaggable::Class_Version(), "QFramework/TQNamedTaggable.h", 8,
                  typeid(::TQNamedTaggable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQNamedTaggable::Dictionary, isa_proxy, 4,
                  sizeof(::TQNamedTaggable) );
      instance.SetNew(&new_TQNamedTaggable);
      instance.SetNewArray(&newArray_TQNamedTaggable);
      instance.SetDelete(&delete_TQNamedTaggable);
      instance.SetDeleteArray(&deleteArray_TQNamedTaggable);
      instance.SetDestructor(&destruct_TQNamedTaggable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQNamedTaggable*)
   {
      return GenerateInitInstanceLocal((::TQNamedTaggable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQNamedTaggable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TQMessageStream_Dictionary();
   static void TQMessageStream_TClassManip(TClass*);
   static void *new_TQMessageStream(void *p = 0);
   static void *newArray_TQMessageStream(Long_t size, void *p);
   static void delete_TQMessageStream(void *p);
   static void deleteArray_TQMessageStream(void *p);
   static void destruct_TQMessageStream(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQMessageStream*)
   {
      ::TQMessageStream *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TQMessageStream));
      static ::ROOT::TGenericClassInfo 
         instance("TQMessageStream", "QFramework/TQMessageStream.h", 14,
                  typeid(::TQMessageStream), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQMessageStream_Dictionary, isa_proxy, 4,
                  sizeof(::TQMessageStream) );
      instance.SetNew(&new_TQMessageStream);
      instance.SetNewArray(&newArray_TQMessageStream);
      instance.SetDelete(&delete_TQMessageStream);
      instance.SetDeleteArray(&deleteArray_TQMessageStream);
      instance.SetDestructor(&destruct_TQMessageStream);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQMessageStream*)
   {
      return GenerateInitInstanceLocal((::TQMessageStream*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQMessageStream*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQMessageStream_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQMessageStream*)0x0)->GetClass();
      TQMessageStream_TClassManip(theClass);
   return theClass;
   }

   static void TQMessageStream_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_TQNFBase(void *p);
   static void deleteArray_TQNFBase(void *p);
   static void destruct_TQNFBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQNFBase*)
   {
      ::TQNFBase *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQNFBase >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQNFBase", ::TQNFBase::Class_Version(), "QFramework/TQNFBase.h", 18,
                  typeid(::TQNFBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQNFBase::Dictionary, isa_proxy, 4,
                  sizeof(::TQNFBase) );
      instance.SetDelete(&delete_TQNFBase);
      instance.SetDeleteArray(&deleteArray_TQNFBase);
      instance.SetDestructor(&destruct_TQNFBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQNFBase*)
   {
      return GenerateInitInstanceLocal((::TQNFBase*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQNFBase*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQCounter(void *p = 0);
   static void *newArray_TQCounter(Long_t size, void *p);
   static void delete_TQCounter(void *p);
   static void deleteArray_TQCounter(void *p);
   static void destruct_TQCounter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCounter*)
   {
      ::TQCounter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCounter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCounter", ::TQCounter::Class_Version(), "QFramework/TQCounter.h", 7,
                  typeid(::TQCounter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQCounter::Dictionary, isa_proxy, 4,
                  sizeof(::TQCounter) );
      instance.SetNew(&new_TQCounter);
      instance.SetNewArray(&newArray_TQCounter);
      instance.SetDelete(&delete_TQCounter);
      instance.SetDeleteArray(&deleteArray_TQCounter);
      instance.SetDestructor(&destruct_TQCounter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCounter*)
   {
      return GenerateInitInstanceLocal((::TQCounter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCounter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQABCDCalculator(void *p = 0);
   static void *newArray_TQABCDCalculator(Long_t size, void *p);
   static void delete_TQABCDCalculator(void *p);
   static void deleteArray_TQABCDCalculator(void *p);
   static void destruct_TQABCDCalculator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQABCDCalculator*)
   {
      ::TQABCDCalculator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQABCDCalculator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQABCDCalculator", ::TQABCDCalculator::Class_Version(), "QFramework/TQABCDCalculator.h", 8,
                  typeid(::TQABCDCalculator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQABCDCalculator::Dictionary, isa_proxy, 4,
                  sizeof(::TQABCDCalculator) );
      instance.SetNew(&new_TQABCDCalculator);
      instance.SetNewArray(&newArray_TQABCDCalculator);
      instance.SetDelete(&delete_TQABCDCalculator);
      instance.SetDeleteArray(&deleteArray_TQABCDCalculator);
      instance.SetDestructor(&destruct_TQABCDCalculator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQABCDCalculator*)
   {
      return GenerateInitInstanceLocal((::TQABCDCalculator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQABCDCalculator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQAlgorithm(void *p);
   static void deleteArray_TQAlgorithm(void *p);
   static void destruct_TQAlgorithm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQAlgorithm*)
   {
      ::TQAlgorithm *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQAlgorithm >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQAlgorithm", ::TQAlgorithm::Class_Version(), "QFramework/TQAlgorithm.h", 11,
                  typeid(::TQAlgorithm), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQAlgorithm::Dictionary, isa_proxy, 4,
                  sizeof(::TQAlgorithm) );
      instance.SetDelete(&delete_TQAlgorithm);
      instance.SetDeleteArray(&deleteArray_TQAlgorithm);
      instance.SetDestructor(&destruct_TQAlgorithm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQAlgorithm*)
   {
      return GenerateInitInstanceLocal((::TQAlgorithm*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQAlgorithm*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETObjectgR_Dictionary();
   static void TQIteratorTlETObjectgR_TClassManip(TClass*);
   static void *new_TQIteratorTlETObjectgR(void *p = 0);
   static void *newArray_TQIteratorTlETObjectgR(Long_t size, void *p);
   static void delete_TQIteratorTlETObjectgR(void *p);
   static void deleteArray_TQIteratorTlETObjectgR(void *p);
   static void destruct_TQIteratorTlETObjectgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TObject>*)
   {
      ::TQIteratorT<TObject> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TObject> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TObject>", ::TQIteratorT<TObject>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TObject>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETObjectgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TObject>) );
      instance.SetNew(&new_TQIteratorTlETObjectgR);
      instance.SetNewArray(&newArray_TQIteratorTlETObjectgR);
      instance.SetDelete(&delete_TQIteratorTlETObjectgR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETObjectgR);
      instance.SetDestructor(&destruct_TQIteratorTlETObjectgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TObject>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TObject>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TObject>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETObjectgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObject>*)0x0)->GetClass();
      TQIteratorTlETObjectgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETObjectgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETQValuegR_Dictionary();
   static void TQIteratorTlETQValuegR_TClassManip(TClass*);
   static void *new_TQIteratorTlETQValuegR(void *p = 0);
   static void *newArray_TQIteratorTlETQValuegR(Long_t size, void *p);
   static void delete_TQIteratorTlETQValuegR(void *p);
   static void deleteArray_TQIteratorTlETQValuegR(void *p);
   static void destruct_TQIteratorTlETQValuegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TQValue>*)
   {
      ::TQIteratorT<TQValue> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TQValue> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TQValue>", ::TQIteratorT<TQValue>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TQValue>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETQValuegR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TQValue>) );
      instance.SetNew(&new_TQIteratorTlETQValuegR);
      instance.SetNewArray(&newArray_TQIteratorTlETQValuegR);
      instance.SetDelete(&delete_TQIteratorTlETQValuegR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETQValuegR);
      instance.SetDestructor(&destruct_TQIteratorTlETQValuegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TQValue>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TQValue>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TQValue>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETQValuegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQValue>*)0x0)->GetClass();
      TQIteratorTlETQValuegR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETQValuegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETQCountergR_Dictionary();
   static void TQIteratorTlETQCountergR_TClassManip(TClass*);
   static void *new_TQIteratorTlETQCountergR(void *p = 0);
   static void *newArray_TQIteratorTlETQCountergR(Long_t size, void *p);
   static void delete_TQIteratorTlETQCountergR(void *p);
   static void deleteArray_TQIteratorTlETQCountergR(void *p);
   static void destruct_TQIteratorTlETQCountergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TQCounter>*)
   {
      ::TQIteratorT<TQCounter> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TQCounter> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TQCounter>", ::TQIteratorT<TQCounter>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TQCounter>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETQCountergR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TQCounter>) );
      instance.SetNew(&new_TQIteratorTlETQCountergR);
      instance.SetNewArray(&newArray_TQIteratorTlETQCountergR);
      instance.SetDelete(&delete_TQIteratorTlETQCountergR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETQCountergR);
      instance.SetDestructor(&destruct_TQIteratorTlETQCountergR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TQCounter>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TQCounter>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TQCounter>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETQCountergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCounter>*)0x0)->GetClass();
      TQIteratorTlETQCountergR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETQCountergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETQCutgR_Dictionary();
   static void TQIteratorTlETQCutgR_TClassManip(TClass*);
   static void *new_TQIteratorTlETQCutgR(void *p = 0);
   static void *newArray_TQIteratorTlETQCutgR(Long_t size, void *p);
   static void delete_TQIteratorTlETQCutgR(void *p);
   static void deleteArray_TQIteratorTlETQCutgR(void *p);
   static void destruct_TQIteratorTlETQCutgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TQCut>*)
   {
      ::TQIteratorT<TQCut> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TQCut> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TQCut>", ::TQIteratorT<TQCut>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TQCut>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETQCutgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TQCut>) );
      instance.SetNew(&new_TQIteratorTlETQCutgR);
      instance.SetNewArray(&newArray_TQIteratorTlETQCutgR);
      instance.SetDelete(&delete_TQIteratorTlETQCutgR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETQCutgR);
      instance.SetDestructor(&destruct_TQIteratorTlETQCutgR);

      ::ROOT::AddClassAlternate("TQIteratorT<TQCut>","TQIteratorT<TQCompiledCut>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TQCut>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TQCut>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TQCut>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETQCutgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCut>*)0x0)->GetClass();
      TQIteratorTlETQCutgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETQCutgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETObjStringgR_Dictionary();
   static void TQIteratorTlETObjStringgR_TClassManip(TClass*);
   static void *new_TQIteratorTlETObjStringgR(void *p = 0);
   static void *newArray_TQIteratorTlETObjStringgR(Long_t size, void *p);
   static void delete_TQIteratorTlETObjStringgR(void *p);
   static void deleteArray_TQIteratorTlETObjStringgR(void *p);
   static void destruct_TQIteratorTlETObjStringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TObjString>*)
   {
      ::TQIteratorT<TObjString> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TObjString> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TObjString>", ::TQIteratorT<TObjString>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TObjString>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETObjStringgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TObjString>) );
      instance.SetNew(&new_TQIteratorTlETObjStringgR);
      instance.SetNewArray(&newArray_TQIteratorTlETObjStringgR);
      instance.SetDelete(&delete_TQIteratorTlETObjStringgR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETObjStringgR);
      instance.SetDestructor(&destruct_TQIteratorTlETObjStringgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TObjString>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TObjString>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TObjString>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETObjStringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObjString>*)0x0)->GetClass();
      TQIteratorTlETObjStringgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETObjStringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETQFoldergR_Dictionary();
   static void TQIteratorTlETQFoldergR_TClassManip(TClass*);
   static void *new_TQIteratorTlETQFoldergR(void *p = 0);
   static void *newArray_TQIteratorTlETQFoldergR(Long_t size, void *p);
   static void delete_TQIteratorTlETQFoldergR(void *p);
   static void deleteArray_TQIteratorTlETQFoldergR(void *p);
   static void destruct_TQIteratorTlETQFoldergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TQFolder>*)
   {
      ::TQIteratorT<TQFolder> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TQFolder> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TQFolder>", ::TQIteratorT<TQFolder>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TQFolder>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETQFoldergR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TQFolder>) );
      instance.SetNew(&new_TQIteratorTlETQFoldergR);
      instance.SetNewArray(&newArray_TQIteratorTlETQFoldergR);
      instance.SetDelete(&delete_TQIteratorTlETQFoldergR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETQFoldergR);
      instance.SetDestructor(&destruct_TQIteratorTlETQFoldergR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TQFolder>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TQFolder>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TQFolder>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETQFoldergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQFolder>*)0x0)->GetClass();
      TQIteratorTlETQFoldergR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETQFoldergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETQSampleFoldergR_Dictionary();
   static void TQIteratorTlETQSampleFoldergR_TClassManip(TClass*);
   static void *new_TQIteratorTlETQSampleFoldergR(void *p = 0);
   static void *newArray_TQIteratorTlETQSampleFoldergR(Long_t size, void *p);
   static void delete_TQIteratorTlETQSampleFoldergR(void *p);
   static void deleteArray_TQIteratorTlETQSampleFoldergR(void *p);
   static void destruct_TQIteratorTlETQSampleFoldergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TQSampleFolder>*)
   {
      ::TQIteratorT<TQSampleFolder> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TQSampleFolder> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TQSampleFolder>", ::TQIteratorT<TQSampleFolder>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TQSampleFolder>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETQSampleFoldergR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TQSampleFolder>) );
      instance.SetNew(&new_TQIteratorTlETQSampleFoldergR);
      instance.SetNewArray(&newArray_TQIteratorTlETQSampleFoldergR);
      instance.SetDelete(&delete_TQIteratorTlETQSampleFoldergR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETQSampleFoldergR);
      instance.SetDestructor(&destruct_TQIteratorTlETQSampleFoldergR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TQSampleFolder>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TQSampleFolder>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TQSampleFolder>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETQSampleFoldergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQSampleFolder>*)0x0)->GetClass();
      TQIteratorTlETQSampleFoldergR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETQSampleFoldergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETQSamplegR_Dictionary();
   static void TQIteratorTlETQSamplegR_TClassManip(TClass*);
   static void *new_TQIteratorTlETQSamplegR(void *p = 0);
   static void *newArray_TQIteratorTlETQSamplegR(Long_t size, void *p);
   static void delete_TQIteratorTlETQSamplegR(void *p);
   static void deleteArray_TQIteratorTlETQSamplegR(void *p);
   static void destruct_TQIteratorTlETQSamplegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TQSample>*)
   {
      ::TQIteratorT<TQSample> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TQSample> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TQSample>", ::TQIteratorT<TQSample>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TQSample>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETQSamplegR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TQSample>) );
      instance.SetNew(&new_TQIteratorTlETQSamplegR);
      instance.SetNewArray(&newArray_TQIteratorTlETQSamplegR);
      instance.SetDelete(&delete_TQIteratorTlETQSamplegR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETQSamplegR);
      instance.SetDestructor(&destruct_TQIteratorTlETQSamplegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TQSample>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TQSample>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TQSample>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETQSamplegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQSample>*)0x0)->GetClass();
      TQIteratorTlETQSamplegR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETQSamplegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETH1gR_Dictionary();
   static void TQIteratorTlETH1gR_TClassManip(TClass*);
   static void *new_TQIteratorTlETH1gR(void *p = 0);
   static void *newArray_TQIteratorTlETH1gR(Long_t size, void *p);
   static void delete_TQIteratorTlETH1gR(void *p);
   static void deleteArray_TQIteratorTlETH1gR(void *p);
   static void destruct_TQIteratorTlETH1gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TH1>*)
   {
      ::TQIteratorT<TH1> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TH1> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TH1>", ::TQIteratorT<TH1>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TH1>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETH1gR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TH1>) );
      instance.SetNew(&new_TQIteratorTlETH1gR);
      instance.SetNewArray(&newArray_TQIteratorTlETH1gR);
      instance.SetDelete(&delete_TQIteratorTlETH1gR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETH1gR);
      instance.SetDestructor(&destruct_TQIteratorTlETH1gR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TH1>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TH1>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TH1>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETH1gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TH1>*)0x0)->GetClass();
      TQIteratorTlETH1gR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETH1gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETQNamedTaggablegR_Dictionary();
   static void TQIteratorTlETQNamedTaggablegR_TClassManip(TClass*);
   static void *new_TQIteratorTlETQNamedTaggablegR(void *p = 0);
   static void *newArray_TQIteratorTlETQNamedTaggablegR(Long_t size, void *p);
   static void delete_TQIteratorTlETQNamedTaggablegR(void *p);
   static void deleteArray_TQIteratorTlETQNamedTaggablegR(void *p);
   static void destruct_TQIteratorTlETQNamedTaggablegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TQNamedTaggable>*)
   {
      ::TQIteratorT<TQNamedTaggable> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TQNamedTaggable> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TQNamedTaggable>", ::TQIteratorT<TQNamedTaggable>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TQNamedTaggable>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETQNamedTaggablegR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TQNamedTaggable>) );
      instance.SetNew(&new_TQIteratorTlETQNamedTaggablegR);
      instance.SetNewArray(&newArray_TQIteratorTlETQNamedTaggablegR);
      instance.SetDelete(&delete_TQIteratorTlETQNamedTaggablegR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETQNamedTaggablegR);
      instance.SetDestructor(&destruct_TQIteratorTlETQNamedTaggablegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TQNamedTaggable>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TQNamedTaggable>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TQNamedTaggable>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETQNamedTaggablegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQNamedTaggable>*)0x0)->GetClass();
      TQIteratorTlETQNamedTaggablegR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETQNamedTaggablegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETQObservablegR_Dictionary();
   static void TQIteratorTlETQObservablegR_TClassManip(TClass*);
   static void *new_TQIteratorTlETQObservablegR(void *p = 0);
   static void *newArray_TQIteratorTlETQObservablegR(Long_t size, void *p);
   static void delete_TQIteratorTlETQObservablegR(void *p);
   static void deleteArray_TQIteratorTlETQObservablegR(void *p);
   static void destruct_TQIteratorTlETQObservablegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TQObservable>*)
   {
      ::TQIteratorT<TQObservable> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TQObservable> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TQObservable>", ::TQIteratorT<TQObservable>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TQObservable>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETQObservablegR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TQObservable>) );
      instance.SetNew(&new_TQIteratorTlETQObservablegR);
      instance.SetNewArray(&newArray_TQIteratorTlETQObservablegR);
      instance.SetDelete(&delete_TQIteratorTlETQObservablegR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETQObservablegR);
      instance.SetDestructor(&destruct_TQIteratorTlETQObservablegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TQObservable>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TQObservable>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TQObservable>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETQObservablegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQObservable>*)0x0)->GetClass();
      TQIteratorTlETQObservablegR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETQObservablegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETQAnalysisJobgR_Dictionary();
   static void TQIteratorTlETQAnalysisJobgR_TClassManip(TClass*);
   static void *new_TQIteratorTlETQAnalysisJobgR(void *p = 0);
   static void *newArray_TQIteratorTlETQAnalysisJobgR(Long_t size, void *p);
   static void delete_TQIteratorTlETQAnalysisJobgR(void *p);
   static void deleteArray_TQIteratorTlETQAnalysisJobgR(void *p);
   static void destruct_TQIteratorTlETQAnalysisJobgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TQAnalysisJob>*)
   {
      ::TQIteratorT<TQAnalysisJob> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TQAnalysisJob> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TQAnalysisJob>", ::TQIteratorT<TQAnalysisJob>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TQAnalysisJob>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETQAnalysisJobgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TQAnalysisJob>) );
      instance.SetNew(&new_TQIteratorTlETQAnalysisJobgR);
      instance.SetNewArray(&newArray_TQIteratorTlETQAnalysisJobgR);
      instance.SetDelete(&delete_TQIteratorTlETQAnalysisJobgR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETQAnalysisJobgR);
      instance.SetDestructor(&destruct_TQIteratorTlETQAnalysisJobgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TQAnalysisJob>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TQAnalysisJob>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TQAnalysisJob>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETQAnalysisJobgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQAnalysisJob>*)0x0)->GetClass();
      TQIteratorTlETQAnalysisJobgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETQAnalysisJobgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETGraphErrorsgR_Dictionary();
   static void TQIteratorTlETGraphErrorsgR_TClassManip(TClass*);
   static void *new_TQIteratorTlETGraphErrorsgR(void *p = 0);
   static void *newArray_TQIteratorTlETGraphErrorsgR(Long_t size, void *p);
   static void delete_TQIteratorTlETGraphErrorsgR(void *p);
   static void deleteArray_TQIteratorTlETGraphErrorsgR(void *p);
   static void destruct_TQIteratorTlETGraphErrorsgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TGraphErrors>*)
   {
      ::TQIteratorT<TGraphErrors> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TGraphErrors> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TGraphErrors>", ::TQIteratorT<TGraphErrors>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TGraphErrors>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETGraphErrorsgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TGraphErrors>) );
      instance.SetNew(&new_TQIteratorTlETGraphErrorsgR);
      instance.SetNewArray(&newArray_TQIteratorTlETGraphErrorsgR);
      instance.SetDelete(&delete_TQIteratorTlETGraphErrorsgR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETGraphErrorsgR);
      instance.SetDestructor(&destruct_TQIteratorTlETGraphErrorsgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TGraphErrors>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TGraphErrors>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TGraphErrors>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETGraphErrorsgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraphErrors>*)0x0)->GetClass();
      TQIteratorTlETGraphErrorsgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETGraphErrorsgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETGraphgR_Dictionary();
   static void TQIteratorTlETGraphgR_TClassManip(TClass*);
   static void *new_TQIteratorTlETGraphgR(void *p = 0);
   static void *newArray_TQIteratorTlETGraphgR(Long_t size, void *p);
   static void delete_TQIteratorTlETGraphgR(void *p);
   static void deleteArray_TQIteratorTlETGraphgR(void *p);
   static void destruct_TQIteratorTlETGraphgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TGraph>*)
   {
      ::TQIteratorT<TGraph> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TGraph> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TGraph>", ::TQIteratorT<TGraph>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TGraph>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETGraphgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TGraph>) );
      instance.SetNew(&new_TQIteratorTlETGraphgR);
      instance.SetNewArray(&newArray_TQIteratorTlETGraphgR);
      instance.SetDelete(&delete_TQIteratorTlETGraphgR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETGraphgR);
      instance.SetDestructor(&destruct_TQIteratorTlETGraphgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TGraph>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TGraph>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TGraph>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETGraphgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraph>*)0x0)->GetClass();
      TQIteratorTlETGraphgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETGraphgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETGraphAsymmErrorsgR_Dictionary();
   static void TQIteratorTlETGraphAsymmErrorsgR_TClassManip(TClass*);
   static void *new_TQIteratorTlETGraphAsymmErrorsgR(void *p = 0);
   static void *newArray_TQIteratorTlETGraphAsymmErrorsgR(Long_t size, void *p);
   static void delete_TQIteratorTlETGraphAsymmErrorsgR(void *p);
   static void deleteArray_TQIteratorTlETGraphAsymmErrorsgR(void *p);
   static void destruct_TQIteratorTlETGraphAsymmErrorsgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TGraphAsymmErrors>*)
   {
      ::TQIteratorT<TGraphAsymmErrors> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TGraphAsymmErrors> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TGraphAsymmErrors>", ::TQIteratorT<TGraphAsymmErrors>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TGraphAsymmErrors>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETGraphAsymmErrorsgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TGraphAsymmErrors>) );
      instance.SetNew(&new_TQIteratorTlETGraphAsymmErrorsgR);
      instance.SetNewArray(&newArray_TQIteratorTlETGraphAsymmErrorsgR);
      instance.SetDelete(&delete_TQIteratorTlETGraphAsymmErrorsgR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETGraphAsymmErrorsgR);
      instance.SetDestructor(&destruct_TQIteratorTlETGraphAsymmErrorsgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TGraphAsymmErrors>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TGraphAsymmErrors>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TGraphAsymmErrors>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETGraphAsymmErrorsgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraphAsymmErrors>*)0x0)->GetClass();
      TQIteratorTlETGraphAsymmErrorsgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETGraphAsymmErrorsgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETCollectiongR_Dictionary();
   static void TQIteratorTlETCollectiongR_TClassManip(TClass*);
   static void *new_TQIteratorTlETCollectiongR(void *p = 0);
   static void *newArray_TQIteratorTlETCollectiongR(Long_t size, void *p);
   static void delete_TQIteratorTlETCollectiongR(void *p);
   static void deleteArray_TQIteratorTlETCollectiongR(void *p);
   static void destruct_TQIteratorTlETCollectiongR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TCollection>*)
   {
      ::TQIteratorT<TCollection> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TCollection> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TCollection>", ::TQIteratorT<TCollection>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TCollection>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETCollectiongR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TCollection>) );
      instance.SetNew(&new_TQIteratorTlETCollectiongR);
      instance.SetNewArray(&newArray_TQIteratorTlETCollectiongR);
      instance.SetDelete(&delete_TQIteratorTlETCollectiongR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETCollectiongR);
      instance.SetDestructor(&destruct_TQIteratorTlETCollectiongR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TCollection>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TCollection>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TCollection>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETCollectiongR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TCollection>*)0x0)->GetClass();
      TQIteratorTlETCollectiongR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETCollectiongR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETListgR_Dictionary();
   static void TQIteratorTlETListgR_TClassManip(TClass*);
   static void *new_TQIteratorTlETListgR(void *p = 0);
   static void *newArray_TQIteratorTlETListgR(Long_t size, void *p);
   static void delete_TQIteratorTlETListgR(void *p);
   static void deleteArray_TQIteratorTlETListgR(void *p);
   static void destruct_TQIteratorTlETListgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TList>*)
   {
      ::TQIteratorT<TList> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TList> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TList>", ::TQIteratorT<TList>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TList>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETListgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TList>) );
      instance.SetNew(&new_TQIteratorTlETListgR);
      instance.SetNewArray(&newArray_TQIteratorTlETListgR);
      instance.SetDelete(&delete_TQIteratorTlETListgR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETListgR);
      instance.SetDestructor(&destruct_TQIteratorTlETListgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TList>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TList>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TList>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETListgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TList>*)0x0)->GetClass();
      TQIteratorTlETListgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETListgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETLegendEntrygR_Dictionary();
   static void TQIteratorTlETLegendEntrygR_TClassManip(TClass*);
   static void *new_TQIteratorTlETLegendEntrygR(void *p = 0);
   static void *newArray_TQIteratorTlETLegendEntrygR(Long_t size, void *p);
   static void delete_TQIteratorTlETLegendEntrygR(void *p);
   static void deleteArray_TQIteratorTlETLegendEntrygR(void *p);
   static void destruct_TQIteratorTlETLegendEntrygR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TLegendEntry>*)
   {
      ::TQIteratorT<TLegendEntry> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TLegendEntry> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TLegendEntry>", ::TQIteratorT<TLegendEntry>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TLegendEntry>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETLegendEntrygR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TLegendEntry>) );
      instance.SetNew(&new_TQIteratorTlETLegendEntrygR);
      instance.SetNewArray(&newArray_TQIteratorTlETLegendEntrygR);
      instance.SetDelete(&delete_TQIteratorTlETLegendEntrygR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETLegendEntrygR);
      instance.SetDestructor(&destruct_TQIteratorTlETLegendEntrygR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TLegendEntry>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TLegendEntry>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TLegendEntry>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETLegendEntrygR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TLegendEntry>*)0x0)->GetClass();
      TQIteratorTlETLegendEntrygR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETLegendEntrygR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETBranchgR_Dictionary();
   static void TQIteratorTlETBranchgR_TClassManip(TClass*);
   static void *new_TQIteratorTlETBranchgR(void *p = 0);
   static void *newArray_TQIteratorTlETBranchgR(Long_t size, void *p);
   static void delete_TQIteratorTlETBranchgR(void *p);
   static void deleteArray_TQIteratorTlETBranchgR(void *p);
   static void destruct_TQIteratorTlETBranchgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TBranch>*)
   {
      ::TQIteratorT<TBranch> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TBranch> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TBranch>", ::TQIteratorT<TBranch>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TBranch>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETBranchgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TBranch>) );
      instance.SetNew(&new_TQIteratorTlETBranchgR);
      instance.SetNewArray(&newArray_TQIteratorTlETBranchgR);
      instance.SetDelete(&delete_TQIteratorTlETBranchgR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETBranchgR);
      instance.SetDestructor(&destruct_TQIteratorTlETBranchgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TBranch>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TBranch>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TBranch>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETBranchgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TBranch>*)0x0)->GetClass();
      TQIteratorTlETBranchgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETBranchgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETKeygR_Dictionary();
   static void TQIteratorTlETKeygR_TClassManip(TClass*);
   static void *new_TQIteratorTlETKeygR(void *p = 0);
   static void *newArray_TQIteratorTlETKeygR(Long_t size, void *p);
   static void delete_TQIteratorTlETKeygR(void *p);
   static void deleteArray_TQIteratorTlETKeygR(void *p);
   static void destruct_TQIteratorTlETKeygR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TKey>*)
   {
      ::TQIteratorT<TKey> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TKey> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TKey>", ::TQIteratorT<TKey>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TKey>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETKeygR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TKey>) );
      instance.SetNew(&new_TQIteratorTlETKeygR);
      instance.SetNewArray(&newArray_TQIteratorTlETKeygR);
      instance.SetDelete(&delete_TQIteratorTlETKeygR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETKeygR);
      instance.SetDestructor(&destruct_TQIteratorTlETKeygR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TKey>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TKey>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TKey>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETKeygR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TKey>*)0x0)->GetClass();
      TQIteratorTlETKeygR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETKeygR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETQCounterGridgR_Dictionary();
   static void TQIteratorTlETQCounterGridgR_TClassManip(TClass*);
   static void *new_TQIteratorTlETQCounterGridgR(void *p = 0);
   static void *newArray_TQIteratorTlETQCounterGridgR(Long_t size, void *p);
   static void delete_TQIteratorTlETQCounterGridgR(void *p);
   static void deleteArray_TQIteratorTlETQCounterGridgR(void *p);
   static void destruct_TQIteratorTlETQCounterGridgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TQCounterGrid>*)
   {
      ::TQIteratorT<TQCounterGrid> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TQCounterGrid> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TQCounterGrid>", ::TQIteratorT<TQCounterGrid>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TQCounterGrid>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETQCounterGridgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TQCounterGrid>) );
      instance.SetNew(&new_TQIteratorTlETQCounterGridgR);
      instance.SetNewArray(&newArray_TQIteratorTlETQCounterGridgR);
      instance.SetDelete(&delete_TQIteratorTlETQCounterGridgR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETQCounterGridgR);
      instance.SetDestructor(&destruct_TQIteratorTlETQCounterGridgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TQCounterGrid>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TQCounterGrid>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TQCounterGrid>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETQCounterGridgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCounterGrid>*)0x0)->GetClass();
      TQIteratorTlETQCounterGridgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETQCounterGridgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETQPCAgR_Dictionary();
   static void TQIteratorTlETQPCAgR_TClassManip(TClass*);
   static void *new_TQIteratorTlETQPCAgR(void *p = 0);
   static void *newArray_TQIteratorTlETQPCAgR(Long_t size, void *p);
   static void delete_TQIteratorTlETQPCAgR(void *p);
   static void deleteArray_TQIteratorTlETQPCAgR(void *p);
   static void destruct_TQIteratorTlETQPCAgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TQPCA>*)
   {
      ::TQIteratorT<TQPCA> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TQPCA> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TQPCA>", ::TQIteratorT<TQPCA>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TQPCA>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETQPCAgR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TQPCA>) );
      instance.SetNew(&new_TQIteratorTlETQPCAgR);
      instance.SetNewArray(&newArray_TQIteratorTlETQPCAgR);
      instance.SetDelete(&delete_TQIteratorTlETQPCAgR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETQPCAgR);
      instance.SetDestructor(&destruct_TQIteratorTlETQPCAgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TQPCA>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TQPCA>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TQPCA>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETQPCAgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQPCA>*)0x0)->GetClass();
      TQIteratorTlETQPCAgR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETQPCAgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETH2gR_Dictionary();
   static void TQIteratorTlETH2gR_TClassManip(TClass*);
   static void *new_TQIteratorTlETH2gR(void *p = 0);
   static void *newArray_TQIteratorTlETH2gR(Long_t size, void *p);
   static void delete_TQIteratorTlETH2gR(void *p);
   static void deleteArray_TQIteratorTlETH2gR(void *p);
   static void destruct_TQIteratorTlETH2gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TH2>*)
   {
      ::TQIteratorT<TH2> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TH2> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TH2>", ::TQIteratorT<TH2>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TH2>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETH2gR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TH2>) );
      instance.SetNew(&new_TQIteratorTlETH2gR);
      instance.SetNewArray(&newArray_TQIteratorTlETH2gR);
      instance.SetDelete(&delete_TQIteratorTlETH2gR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETH2gR);
      instance.SetDestructor(&destruct_TQIteratorTlETH2gR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TH2>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TH2>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TH2>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETH2gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TH2>*)0x0)->GetClass();
      TQIteratorTlETH2gR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETH2gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQIteratorTlETObjArraygR_Dictionary();
   static void TQIteratorTlETObjArraygR_TClassManip(TClass*);
   static void *new_TQIteratorTlETObjArraygR(void *p = 0);
   static void *newArray_TQIteratorTlETObjArraygR(Long_t size, void *p);
   static void delete_TQIteratorTlETObjArraygR(void *p);
   static void deleteArray_TQIteratorTlETObjArraygR(void *p);
   static void destruct_TQIteratorTlETObjArraygR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQIteratorT<TObjArray>*)
   {
      ::TQIteratorT<TObjArray> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQIteratorT<TObjArray> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQIteratorT<TObjArray>", ::TQIteratorT<TObjArray>::Class_Version(), "QFramework/TQIteratorT.h", 54,
                  typeid(::TQIteratorT<TObjArray>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQIteratorTlETObjArraygR_Dictionary, isa_proxy, 4,
                  sizeof(::TQIteratorT<TObjArray>) );
      instance.SetNew(&new_TQIteratorTlETObjArraygR);
      instance.SetNewArray(&newArray_TQIteratorTlETObjArraygR);
      instance.SetDelete(&delete_TQIteratorTlETObjArraygR);
      instance.SetDeleteArray(&deleteArray_TQIteratorTlETObjArraygR);
      instance.SetDestructor(&destruct_TQIteratorTlETObjArraygR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQIteratorT<TObjArray>*)
   {
      return GenerateInitInstanceLocal((::TQIteratorT<TObjArray>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQIteratorT<TObjArray>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQIteratorTlETObjArraygR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObjArray>*)0x0)->GetClass();
      TQIteratorTlETObjArraygR_TClassManip(theClass);
   return theClass;
   }

   static void TQIteratorTlETObjArraygR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_TQPCA(void *p = 0);
   static void *newArray_TQPCA(Long_t size, void *p);
   static void delete_TQPCA(void *p);
   static void deleteArray_TQPCA(void *p);
   static void destruct_TQPCA(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQPCA*)
   {
      ::TQPCA *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQPCA >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQPCA", ::TQPCA::Class_Version(), "QFramework/TQPCA.h", 10,
                  typeid(::TQPCA), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQPCA::Dictionary, isa_proxy, 4,
                  sizeof(::TQPCA) );
      instance.SetNew(&new_TQPCA);
      instance.SetNewArray(&newArray_TQPCA);
      instance.SetDelete(&delete_TQPCA);
      instance.SetDeleteArray(&deleteArray_TQPCA);
      instance.SetDestructor(&destruct_TQPCA);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQPCA*)
   {
      return GenerateInitInstanceLocal((::TQPCA*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQPCA*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQFolder(void *p = 0);
   static void *newArray_TQFolder(Long_t size, void *p);
   static void delete_TQFolder(void *p);
   static void deleteArray_TQFolder(void *p);
   static void destruct_TQFolder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQFolder*)
   {
      ::TQFolder *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQFolder >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQFolder", ::TQFolder::Class_Version(), "QFramework/TQFolder.h", 15,
                  typeid(::TQFolder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQFolder::Dictionary, isa_proxy, 4,
                  sizeof(::TQFolder) );
      instance.SetNew(&new_TQFolder);
      instance.SetNewArray(&newArray_TQFolder);
      instance.SetDelete(&delete_TQFolder);
      instance.SetDeleteArray(&deleteArray_TQFolder);
      instance.SetDestructor(&destruct_TQFolder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQFolder*)
   {
      return GenerateInitInstanceLocal((::TQFolder*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQFolder*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQSampleFolder(void *p = 0);
   static void *newArray_TQSampleFolder(Long_t size, void *p);
   static void delete_TQSampleFolder(void *p);
   static void deleteArray_TQSampleFolder(void *p);
   static void destruct_TQSampleFolder(void *p);

   // Schema evolution read functions
   static void read_TQSampleFolder_0( char* target, TVirtualObject *oldObj )
   {
      //--- Automatically generated variables ---
      TQSampleFolder* newObj = (TQSampleFolder*)target;
      // Supress warning message.
      (void)oldObj;

      (void)newObj;

      //--- User's code ---
      newObj->convertLegacyNFs(); 
   }

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSampleFolder*)
   {
      ::TQSampleFolder *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSampleFolder >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSampleFolder", ::TQSampleFolder::Class_Version(), "QFramework/TQSampleFolder.h", 16,
                  typeid(::TQSampleFolder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSampleFolder::Dictionary, isa_proxy, 4,
                  sizeof(::TQSampleFolder) );
      instance.SetNew(&new_TQSampleFolder);
      instance.SetNewArray(&newArray_TQSampleFolder);
      instance.SetDelete(&delete_TQSampleFolder);
      instance.SetDeleteArray(&deleteArray_TQSampleFolder);
      instance.SetDestructor(&destruct_TQSampleFolder);

      ROOT::Internal::TSchemaHelper* rule;

      // the io read rules
      std::vector<ROOT::Internal::TSchemaHelper> readrules(1);
      rule = &readrules[0];
      rule->fSourceClass = "TQSampleFolder";
      rule->fTarget      = "";
      rule->fSource      = "";
      rule->fFunctionPtr = (void *)TFunc2void( read_TQSampleFolder_0);
      rule->fCode        = " newObj->convertLegacyNFs(); ";
      rule->fVersion     = "[1]";
      instance.SetReadRules( readrules );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSampleFolder*)
   {
      return GenerateInitInstanceLocal((::TQSampleFolder*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSampleFolder*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQSample(void *p = 0);
   static void *newArray_TQSample(Long_t size, void *p);
   static void delete_TQSample(void *p);
   static void deleteArray_TQSample(void *p);
   static void destruct_TQSample(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSample*)
   {
      ::TQSample *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSample >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSample", ::TQSample::Class_Version(), "QFramework/TQSample.h", 20,
                  typeid(::TQSample), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSample::Dictionary, isa_proxy, 4,
                  sizeof(::TQSample) );
      instance.SetNew(&new_TQSample);
      instance.SetNewArray(&newArray_TQSample);
      instance.SetDelete(&delete_TQSample);
      instance.SetDeleteArray(&deleteArray_TQSample);
      instance.SetDestructor(&destruct_TQSample);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSample*)
   {
      return GenerateInitInstanceLocal((::TQSample*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSample*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQObservable(void *p);
   static void deleteArray_TQObservable(void *p);
   static void destruct_TQObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQObservable*)
   {
      ::TQObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQObservable", ::TQObservable::Class_Version(), "QFramework/TQObservable.h", 41,
                  typeid(::TQObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQObservable) );
      instance.SetDelete(&delete_TQObservable);
      instance.SetDeleteArray(&deleteArray_TQObservable);
      instance.SetDestructor(&destruct_TQObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQObservable*)
   {
      return GenerateInitInstanceLocal((::TQObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQAnalysisJob(void *p);
   static void deleteArray_TQAnalysisJob(void *p);
   static void destruct_TQAnalysisJob(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQAnalysisJob*)
   {
      ::TQAnalysisJob *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQAnalysisJob >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQAnalysisJob", ::TQAnalysisJob::Class_Version(), "QFramework/TQAnalysisJob.h", 11,
                  typeid(::TQAnalysisJob), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQAnalysisJob::Dictionary, isa_proxy, 4,
                  sizeof(::TQAnalysisJob) );
      instance.SetDelete(&delete_TQAnalysisJob);
      instance.SetDeleteArray(&deleteArray_TQAnalysisJob);
      instance.SetDestructor(&destruct_TQAnalysisJob);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQAnalysisJob*)
   {
      return GenerateInitInstanceLocal((::TQAnalysisJob*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQAnalysisJob*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQCut(void *p = 0);
   static void *newArray_TQCut(Long_t size, void *p);
   static void delete_TQCut(void *p);
   static void deleteArray_TQCut(void *p);
   static void destruct_TQCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCut*)
   {
      ::TQCut *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCut >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCut", ::TQCut::Class_Version(), "QFramework/TQCut.h", 19,
                  typeid(::TQCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQCut::Dictionary, isa_proxy, 4,
                  sizeof(::TQCut) );
      instance.SetNew(&new_TQCut);
      instance.SetNewArray(&newArray_TQCut);
      instance.SetDelete(&delete_TQCut);
      instance.SetDeleteArray(&deleteArray_TQCut);
      instance.SetDestructor(&destruct_TQCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCut*)
   {
      return GenerateInitInstanceLocal((::TQCut*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCut*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQAnalysisAlgorithm(void *p);
   static void deleteArray_TQAnalysisAlgorithm(void *p);
   static void destruct_TQAnalysisAlgorithm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQAnalysisAlgorithm*)
   {
      ::TQAnalysisAlgorithm *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQAnalysisAlgorithm >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQAnalysisAlgorithm", ::TQAnalysisAlgorithm::Class_Version(), "QFramework/TQAnalysisAlgorithm.h", 12,
                  typeid(::TQAnalysisAlgorithm), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQAnalysisAlgorithm::Dictionary, isa_proxy, 4,
                  sizeof(::TQAnalysisAlgorithm) );
      instance.SetDelete(&delete_TQAnalysisAlgorithm);
      instance.SetDeleteArray(&deleteArray_TQAnalysisAlgorithm);
      instance.SetDestructor(&destruct_TQAnalysisAlgorithm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQAnalysisAlgorithm*)
   {
      return GenerateInitInstanceLocal((::TQAnalysisAlgorithm*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQAnalysisAlgorithm*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQSampleVisitor(void *p = 0);
   static void *newArray_TQSampleVisitor(Long_t size, void *p);
   static void delete_TQSampleVisitor(void *p);
   static void deleteArray_TQSampleVisitor(void *p);
   static void destruct_TQSampleVisitor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSampleVisitor*)
   {
      ::TQSampleVisitor *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSampleVisitor >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSampleVisitor", ::TQSampleVisitor::Class_Version(), "QFramework/TQSampleVisitor.h", 12,
                  typeid(::TQSampleVisitor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSampleVisitor::Dictionary, isa_proxy, 4,
                  sizeof(::TQSampleVisitor) );
      instance.SetNew(&new_TQSampleVisitor);
      instance.SetNewArray(&newArray_TQSampleVisitor);
      instance.SetDelete(&delete_TQSampleVisitor);
      instance.SetDeleteArray(&deleteArray_TQSampleVisitor);
      instance.SetDestructor(&destruct_TQSampleVisitor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSampleVisitor*)
   {
      return GenerateInitInstanceLocal((::TQSampleVisitor*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSampleVisitor*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQAnalysisSampleVisitorBase(void *p);
   static void deleteArray_TQAnalysisSampleVisitorBase(void *p);
   static void destruct_TQAnalysisSampleVisitorBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQAnalysisSampleVisitorBase*)
   {
      ::TQAnalysisSampleVisitorBase *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQAnalysisSampleVisitorBase >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQAnalysisSampleVisitorBase", ::TQAnalysisSampleVisitorBase::Class_Version(), "QFramework/TQAnalysisSampleVisitorBase.h", 10,
                  typeid(::TQAnalysisSampleVisitorBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQAnalysisSampleVisitorBase::Dictionary, isa_proxy, 4,
                  sizeof(::TQAnalysisSampleVisitorBase) );
      instance.SetDelete(&delete_TQAnalysisSampleVisitorBase);
      instance.SetDeleteArray(&deleteArray_TQAnalysisSampleVisitorBase);
      instance.SetDestructor(&destruct_TQAnalysisSampleVisitorBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQAnalysisSampleVisitorBase*)
   {
      return GenerateInitInstanceLocal((::TQAnalysisSampleVisitorBase*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQAnalysisSampleVisitorBase*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQAnalysisSampleVisitor(void *p = 0);
   static void *newArray_TQAnalysisSampleVisitor(Long_t size, void *p);
   static void delete_TQAnalysisSampleVisitor(void *p);
   static void deleteArray_TQAnalysisSampleVisitor(void *p);
   static void destruct_TQAnalysisSampleVisitor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQAnalysisSampleVisitor*)
   {
      ::TQAnalysisSampleVisitor *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQAnalysisSampleVisitor >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQAnalysisSampleVisitor", ::TQAnalysisSampleVisitor::Class_Version(), "QFramework/TQAnalysisSampleVisitor.h", 10,
                  typeid(::TQAnalysisSampleVisitor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQAnalysisSampleVisitor::Dictionary, isa_proxy, 4,
                  sizeof(::TQAnalysisSampleVisitor) );
      instance.SetNew(&new_TQAnalysisSampleVisitor);
      instance.SetNewArray(&newArray_TQAnalysisSampleVisitor);
      instance.SetDelete(&delete_TQAnalysisSampleVisitor);
      instance.SetDeleteArray(&deleteArray_TQAnalysisSampleVisitor);
      instance.SetDestructor(&destruct_TQAnalysisSampleVisitor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQAnalysisSampleVisitor*)
   {
      return GenerateInitInstanceLocal((::TQAnalysisSampleVisitor*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQAnalysisSampleVisitor*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQTable(void *p = 0);
   static void *newArray_TQTable(Long_t size, void *p);
   static void delete_TQTable(void *p);
   static void deleteArray_TQTable(void *p);
   static void destruct_TQTable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQTable*)
   {
      ::TQTable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQTable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQTable", ::TQTable::Class_Version(), "QFramework/TQTable.h", 15,
                  typeid(::TQTable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQTable::Dictionary, isa_proxy, 4,
                  sizeof(::TQTable) );
      instance.SetNew(&new_TQTable);
      instance.SetNewArray(&newArray_TQTable);
      instance.SetDelete(&delete_TQTable);
      instance.SetDeleteArray(&deleteArray_TQTable);
      instance.SetDestructor(&destruct_TQTable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQTable*)
   {
      return GenerateInitInstanceLocal((::TQTable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQTable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQSampleDataReader(void *p = 0);
   static void *newArray_TQSampleDataReader(Long_t size, void *p);
   static void delete_TQSampleDataReader(void *p);
   static void deleteArray_TQSampleDataReader(void *p);
   static void destruct_TQSampleDataReader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSampleDataReader*)
   {
      ::TQSampleDataReader *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSampleDataReader >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSampleDataReader", ::TQSampleDataReader::Class_Version(), "QFramework/TQSampleDataReader.h", 22,
                  typeid(::TQSampleDataReader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSampleDataReader::Dictionary, isa_proxy, 4,
                  sizeof(::TQSampleDataReader) );
      instance.SetNew(&new_TQSampleDataReader);
      instance.SetNewArray(&newArray_TQSampleDataReader);
      instance.SetDelete(&delete_TQSampleDataReader);
      instance.SetDeleteArray(&deleteArray_TQSampleDataReader);
      instance.SetDestructor(&destruct_TQSampleDataReader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSampleDataReader*)
   {
      return GenerateInitInstanceLocal((::TQSampleDataReader*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSampleDataReader*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQPresenter(void *p = 0);
   static void *newArray_TQPresenter(Long_t size, void *p);
   static void delete_TQPresenter(void *p);
   static void deleteArray_TQPresenter(void *p);
   static void destruct_TQPresenter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQPresenter*)
   {
      ::TQPresenter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQPresenter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQPresenter", ::TQPresenter::Class_Version(), "QFramework/TQPresenter.h", 8,
                  typeid(::TQPresenter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQPresenter::Dictionary, isa_proxy, 4,
                  sizeof(::TQPresenter) );
      instance.SetNew(&new_TQPresenter);
      instance.SetNewArray(&newArray_TQPresenter);
      instance.SetDelete(&delete_TQPresenter);
      instance.SetDeleteArray(&deleteArray_TQPresenter);
      instance.SetDestructor(&destruct_TQPresenter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQPresenter*)
   {
      return GenerateInitInstanceLocal((::TQPresenter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQPresenter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQPlotter(void *p);
   static void deleteArray_TQPlotter(void *p);
   static void destruct_TQPlotter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQPlotter*)
   {
      ::TQPlotter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQPlotter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQPlotter", ::TQPlotter::Class_Version(), "QFramework/TQPlotter.h", 18,
                  typeid(::TQPlotter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQPlotter::Dictionary, isa_proxy, 4,
                  sizeof(::TQPlotter) );
      instance.SetDelete(&delete_TQPlotter);
      instance.SetDeleteArray(&deleteArray_TQPlotter);
      instance.SetDestructor(&destruct_TQPlotter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQPlotter*)
   {
      return GenerateInitInstanceLocal((::TQPlotter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQPlotter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQROOTPlotter(void *p);
   static void deleteArray_TQROOTPlotter(void *p);
   static void destruct_TQROOTPlotter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQROOTPlotter*)
   {
      ::TQROOTPlotter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQROOTPlotter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQROOTPlotter", ::TQROOTPlotter::Class_Version(), "QFramework/TQROOTPlotter.h", 7,
                  typeid(::TQROOTPlotter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQROOTPlotter::Dictionary, isa_proxy, 4,
                  sizeof(::TQROOTPlotter) );
      instance.SetDelete(&delete_TQROOTPlotter);
      instance.SetDeleteArray(&deleteArray_TQROOTPlotter);
      instance.SetDestructor(&destruct_TQROOTPlotter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQROOTPlotter*)
   {
      return GenerateInitInstanceLocal((::TQROOTPlotter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQROOTPlotter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQCompPlotter(void *p);
   static void deleteArray_TQCompPlotter(void *p);
   static void destruct_TQCompPlotter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCompPlotter*)
   {
      ::TQCompPlotter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCompPlotter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCompPlotter", ::TQCompPlotter::Class_Version(), "QFramework/TQCompPlotter.h", 26,
                  typeid(::TQCompPlotter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQCompPlotter::Dictionary, isa_proxy, 4,
                  sizeof(::TQCompPlotter) );
      instance.SetDelete(&delete_TQCompPlotter);
      instance.SetDeleteArray(&deleteArray_TQCompPlotter);
      instance.SetDestructor(&destruct_TQCompPlotter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCompPlotter*)
   {
      return GenerateInitInstanceLocal((::TQCompPlotter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCompPlotter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TQConfigReader_Dictionary();
   static void TQConfigReader_TClassManip(TClass*);
   static void delete_TQConfigReader(void *p);
   static void deleteArray_TQConfigReader(void *p);
   static void destruct_TQConfigReader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQConfigReader*)
   {
      ::TQConfigReader *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TQConfigReader));
      static ::ROOT::TGenericClassInfo 
         instance("TQConfigReader", "QFramework/TQConfigReader.h", 16,
                  typeid(::TQConfigReader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQConfigReader_Dictionary, isa_proxy, 4,
                  sizeof(::TQConfigReader) );
      instance.SetDelete(&delete_TQConfigReader);
      instance.SetDeleteArray(&deleteArray_TQConfigReader);
      instance.SetDestructor(&destruct_TQConfigReader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQConfigReader*)
   {
      return GenerateInitInstanceLocal((::TQConfigReader*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQConfigReader*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQConfigReader_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQConfigReader*)0x0)->GetClass();
      TQConfigReader_TClassManip(theClass);
   return theClass;
   }

   static void TQConfigReader_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_TQConstObservable(void *p = 0);
   static void *newArray_TQConstObservable(Long_t size, void *p);
   static void delete_TQConstObservable(void *p);
   static void deleteArray_TQConstObservable(void *p);
   static void destruct_TQConstObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQConstObservable*)
   {
      ::TQConstObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQConstObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQConstObservable", ::TQConstObservable::Class_Version(), "QFramework/TQConstObservable.h", 7,
                  typeid(::TQConstObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQConstObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQConstObservable) );
      instance.SetNew(&new_TQConstObservable);
      instance.SetNewArray(&newArray_TQConstObservable);
      instance.SetDelete(&delete_TQConstObservable);
      instance.SetDeleteArray(&deleteArray_TQConstObservable);
      instance.SetDestructor(&destruct_TQConstObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQConstObservable*)
   {
      return GenerateInitInstanceLocal((::TQConstObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQConstObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQCutFactory(void *p = 0);
   static void *newArray_TQCutFactory(Long_t size, void *p);
   static void delete_TQCutFactory(void *p);
   static void deleteArray_TQCutFactory(void *p);
   static void destruct_TQCutFactory(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCutFactory*)
   {
      ::TQCutFactory *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCutFactory >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCutFactory", ::TQCutFactory::Class_Version(), "QFramework/TQCutFactory.h", 10,
                  typeid(::TQCutFactory), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQCutFactory::Dictionary, isa_proxy, 4,
                  sizeof(::TQCutFactory) );
      instance.SetNew(&new_TQCutFactory);
      instance.SetNewArray(&newArray_TQCutFactory);
      instance.SetDelete(&delete_TQCutFactory);
      instance.SetDeleteArray(&deleteArray_TQCutFactory);
      instance.SetDestructor(&destruct_TQCutFactory);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCutFactory*)
   {
      return GenerateInitInstanceLocal((::TQCutFactory*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCutFactory*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQCutflowAnalysisJob(void *p = 0);
   static void *newArray_TQCutflowAnalysisJob(Long_t size, void *p);
   static void delete_TQCutflowAnalysisJob(void *p);
   static void deleteArray_TQCutflowAnalysisJob(void *p);
   static void destruct_TQCutflowAnalysisJob(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCutflowAnalysisJob*)
   {
      ::TQCutflowAnalysisJob *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCutflowAnalysisJob >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCutflowAnalysisJob", ::TQCutflowAnalysisJob::Class_Version(), "QFramework/TQCutflowAnalysisJob.h", 11,
                  typeid(::TQCutflowAnalysisJob), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQCutflowAnalysisJob::Dictionary, isa_proxy, 4,
                  sizeof(::TQCutflowAnalysisJob) );
      instance.SetNew(&new_TQCutflowAnalysisJob);
      instance.SetNewArray(&newArray_TQCutflowAnalysisJob);
      instance.SetDelete(&delete_TQCutflowAnalysisJob);
      instance.SetDeleteArray(&deleteArray_TQCutflowAnalysisJob);
      instance.SetDestructor(&destruct_TQCutflowAnalysisJob);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCutflowAnalysisJob*)
   {
      return GenerateInitInstanceLocal((::TQCutflowAnalysisJob*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCutflowAnalysisJob*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQCutflowPrinter(void *p = 0);
   static void *newArray_TQCutflowPrinter(Long_t size, void *p);
   static void delete_TQCutflowPrinter(void *p);
   static void deleteArray_TQCutflowPrinter(void *p);
   static void destruct_TQCutflowPrinter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCutflowPrinter*)
   {
      ::TQCutflowPrinter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCutflowPrinter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCutflowPrinter", ::TQCutflowPrinter::Class_Version(), "QFramework/TQCutflowPrinter.h", 8,
                  typeid(::TQCutflowPrinter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQCutflowPrinter::Dictionary, isa_proxy, 4,
                  sizeof(::TQCutflowPrinter) );
      instance.SetNew(&new_TQCutflowPrinter);
      instance.SetNewArray(&newArray_TQCutflowPrinter);
      instance.SetDelete(&delete_TQCutflowPrinter);
      instance.SetDeleteArray(&deleteArray_TQCutflowPrinter);
      instance.SetDestructor(&destruct_TQCutflowPrinter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCutflowPrinter*)
   {
      return GenerateInitInstanceLocal((::TQCutflowPrinter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCutflowPrinter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQCutflowPlotter(void *p);
   static void deleteArray_TQCutflowPlotter(void *p);
   static void destruct_TQCutflowPlotter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQCutflowPlotter*)
   {
      ::TQCutflowPlotter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQCutflowPlotter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQCutflowPlotter", ::TQCutflowPlotter::Class_Version(), "QFramework/TQCutflowPlotter.h", 10,
                  typeid(::TQCutflowPlotter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQCutflowPlotter::Dictionary, isa_proxy, 4,
                  sizeof(::TQCutflowPlotter) );
      instance.SetDelete(&delete_TQCutflowPlotter);
      instance.SetDeleteArray(&deleteArray_TQCutflowPlotter);
      instance.SetDestructor(&destruct_TQCutflowPlotter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQCutflowPlotter*)
   {
      return GenerateInitInstanceLocal((::TQCutflowPlotter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQCutflowPlotter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQDefaultPlotter(void *p);
   static void deleteArray_TQDefaultPlotter(void *p);
   static void destruct_TQDefaultPlotter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQDefaultPlotter*)
   {
      ::TQDefaultPlotter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQDefaultPlotter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQDefaultPlotter", ::TQDefaultPlotter::Class_Version(), "QFramework/TQDefaultPlotter.h", 7,
                  typeid(::TQDefaultPlotter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQDefaultPlotter::Dictionary, isa_proxy, 4,
                  sizeof(::TQDefaultPlotter) );
      instance.SetDelete(&delete_TQDefaultPlotter);
      instance.SetDeleteArray(&deleteArray_TQDefaultPlotter);
      instance.SetDestructor(&destruct_TQDefaultPlotter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQDefaultPlotter*)
   {
      return GenerateInitInstanceLocal((::TQDefaultPlotter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQDefaultPlotter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQTreeObservable(void *p);
   static void deleteArray_TQTreeObservable(void *p);
   static void destruct_TQTreeObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQTreeObservable*)
   {
      ::TQTreeObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQTreeObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQTreeObservable", ::TQTreeObservable::Class_Version(), "QFramework/TQTreeObservable.h", 10,
                  typeid(::TQTreeObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQTreeObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQTreeObservable) );
      instance.SetDelete(&delete_TQTreeObservable);
      instance.SetDeleteArray(&deleteArray_TQTreeObservable);
      instance.SetDestructor(&destruct_TQTreeObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQTreeObservable*)
   {
      return GenerateInitInstanceLocal((::TQTreeObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQTreeObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQEventIndexObservable(void *p = 0);
   static void *newArray_TQEventIndexObservable(Long_t size, void *p);
   static void delete_TQEventIndexObservable(void *p);
   static void deleteArray_TQEventIndexObservable(void *p);
   static void destruct_TQEventIndexObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQEventIndexObservable*)
   {
      ::TQEventIndexObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQEventIndexObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQEventIndexObservable", ::TQEventIndexObservable::Class_Version(), "QFramework/TQEventIndexObservable.h", 6,
                  typeid(::TQEventIndexObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQEventIndexObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQEventIndexObservable) );
      instance.SetNew(&new_TQEventIndexObservable);
      instance.SetNewArray(&newArray_TQEventIndexObservable);
      instance.SetDelete(&delete_TQEventIndexObservable);
      instance.SetDeleteArray(&deleteArray_TQEventIndexObservable);
      instance.SetDestructor(&destruct_TQEventIndexObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQEventIndexObservable*)
   {
      return GenerateInitInstanceLocal((::TQEventIndexObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQEventIndexObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQEventlistAnalysisJob(void *p = 0);
   static void *newArray_TQEventlistAnalysisJob(Long_t size, void *p);
   static void delete_TQEventlistAnalysisJob(void *p);
   static void deleteArray_TQEventlistAnalysisJob(void *p);
   static void destruct_TQEventlistAnalysisJob(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQEventlistAnalysisJob*)
   {
      ::TQEventlistAnalysisJob *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQEventlistAnalysisJob >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQEventlistAnalysisJob", ::TQEventlistAnalysisJob::Class_Version(), "QFramework/TQEventlistAnalysisJob.h", 18,
                  typeid(::TQEventlistAnalysisJob), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQEventlistAnalysisJob::Dictionary, isa_proxy, 4,
                  sizeof(::TQEventlistAnalysisJob) );
      instance.SetNew(&new_TQEventlistAnalysisJob);
      instance.SetNewArray(&newArray_TQEventlistAnalysisJob);
      instance.SetDelete(&delete_TQEventlistAnalysisJob);
      instance.SetDeleteArray(&deleteArray_TQEventlistAnalysisJob);
      instance.SetDestructor(&destruct_TQEventlistAnalysisJob);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQEventlistAnalysisJob*)
   {
      return GenerateInitInstanceLocal((::TQEventlistAnalysisJob*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQEventlistAnalysisJob*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQEventlistPrinter(void *p = 0);
   static void *newArray_TQEventlistPrinter(Long_t size, void *p);
   static void delete_TQEventlistPrinter(void *p);
   static void deleteArray_TQEventlistPrinter(void *p);
   static void destruct_TQEventlistPrinter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQEventlistPrinter*)
   {
      ::TQEventlistPrinter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQEventlistPrinter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQEventlistPrinter", ::TQEventlistPrinter::Class_Version(), "QFramework/TQEventlistPrinter.h", 9,
                  typeid(::TQEventlistPrinter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQEventlistPrinter::Dictionary, isa_proxy, 4,
                  sizeof(::TQEventlistPrinter) );
      instance.SetNew(&new_TQEventlistPrinter);
      instance.SetNewArray(&newArray_TQEventlistPrinter);
      instance.SetDelete(&delete_TQEventlistPrinter);
      instance.SetDeleteArray(&deleteArray_TQEventlistPrinter);
      instance.SetDestructor(&destruct_TQEventlistPrinter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQEventlistPrinter*)
   {
      return GenerateInitInstanceLocal((::TQEventlistPrinter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQEventlistPrinter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQFilterObservable(void *p = 0);
   static void *newArray_TQFilterObservable(Long_t size, void *p);
   static void delete_TQFilterObservable(void *p);
   static void deleteArray_TQFilterObservable(void *p);
   static void destruct_TQFilterObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQFilterObservable*)
   {
      ::TQFilterObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQFilterObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQFilterObservable", ::TQFilterObservable::Class_Version(), "QFramework/TQFilterObservable.h", 6,
                  typeid(::TQFilterObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQFilterObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQFilterObservable) );
      instance.SetNew(&new_TQFilterObservable);
      instance.SetNewArray(&newArray_TQFilterObservable);
      instance.SetDelete(&delete_TQFilterObservable);
      instance.SetDeleteArray(&deleteArray_TQFilterObservable);
      instance.SetDestructor(&destruct_TQFilterObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQFilterObservable*)
   {
      return GenerateInitInstanceLocal((::TQFilterObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQFilterObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQGraphMakerAnalysisJob(void *p = 0);
   static void *newArray_TQGraphMakerAnalysisJob(Long_t size, void *p);
   static void delete_TQGraphMakerAnalysisJob(void *p);
   static void deleteArray_TQGraphMakerAnalysisJob(void *p);
   static void destruct_TQGraphMakerAnalysisJob(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQGraphMakerAnalysisJob*)
   {
      ::TQGraphMakerAnalysisJob *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQGraphMakerAnalysisJob >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQGraphMakerAnalysisJob", ::TQGraphMakerAnalysisJob::Class_Version(), "QFramework/TQGraphMakerAnalysisJob.h", 12,
                  typeid(::TQGraphMakerAnalysisJob), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQGraphMakerAnalysisJob::Dictionary, isa_proxy, 4,
                  sizeof(::TQGraphMakerAnalysisJob) );
      instance.SetNew(&new_TQGraphMakerAnalysisJob);
      instance.SetNewArray(&newArray_TQGraphMakerAnalysisJob);
      instance.SetDelete(&delete_TQGraphMakerAnalysisJob);
      instance.SetDeleteArray(&deleteArray_TQGraphMakerAnalysisJob);
      instance.SetDestructor(&destruct_TQGraphMakerAnalysisJob);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQGraphMakerAnalysisJob*)
   {
      return GenerateInitInstanceLocal((::TQGraphMakerAnalysisJob*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQGraphMakerAnalysisJob*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQGridScanBound(void *p = 0);
   static void *newArray_TQGridScanBound(Long_t size, void *p);
   static void delete_TQGridScanBound(void *p);
   static void deleteArray_TQGridScanBound(void *p);
   static void destruct_TQGridScanBound(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQGridScanBound*)
   {
      ::TQGridScanBound *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQGridScanBound >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQGridScanBound", ::TQGridScanBound::Class_Version(), "QFramework/TQGridScanBound.h", 14,
                  typeid(::TQGridScanBound), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQGridScanBound::Dictionary, isa_proxy, 4,
                  sizeof(::TQGridScanBound) );
      instance.SetNew(&new_TQGridScanBound);
      instance.SetNewArray(&newArray_TQGridScanBound);
      instance.SetDelete(&delete_TQGridScanBound);
      instance.SetDeleteArray(&deleteArray_TQGridScanBound);
      instance.SetDestructor(&destruct_TQGridScanBound);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQGridScanBound*)
   {
      return GenerateInitInstanceLocal((::TQGridScanBound*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQGridScanBound*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQGridScanBounds(void *p = 0);
   static void *newArray_TQGridScanBounds(Long_t size, void *p);
   static void delete_TQGridScanBounds(void *p);
   static void deleteArray_TQGridScanBounds(void *p);
   static void destruct_TQGridScanBounds(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQGridScanBounds*)
   {
      ::TQGridScanBounds *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQGridScanBounds >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQGridScanBounds", ::TQGridScanBounds::Class_Version(), "QFramework/TQGridScanBound.h", 73,
                  typeid(::TQGridScanBounds), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQGridScanBounds::Dictionary, isa_proxy, 4,
                  sizeof(::TQGridScanBounds) );
      instance.SetNew(&new_TQGridScanBounds);
      instance.SetNewArray(&newArray_TQGridScanBounds);
      instance.SetDelete(&delete_TQGridScanBounds);
      instance.SetDeleteArray(&deleteArray_TQGridScanBounds);
      instance.SetDestructor(&destruct_TQGridScanBounds);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQGridScanBounds*)
   {
      return GenerateInitInstanceLocal((::TQGridScanBounds*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQGridScanBounds*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQGridScanObservable(void *p = 0);
   static void *newArray_TQGridScanObservable(Long_t size, void *p);
   static void delete_TQGridScanObservable(void *p);
   static void deleteArray_TQGridScanObservable(void *p);
   static void destruct_TQGridScanObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQGridScanObservable*)
   {
      ::TQGridScanObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQGridScanObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQGridScanObservable", ::TQGridScanObservable::Class_Version(), "QFramework/TQGridScanObservable.h", 21,
                  typeid(::TQGridScanObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQGridScanObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQGridScanObservable) );
      instance.SetNew(&new_TQGridScanObservable);
      instance.SetNewArray(&newArray_TQGridScanObservable);
      instance.SetDelete(&delete_TQGridScanObservable);
      instance.SetDeleteArray(&deleteArray_TQGridScanObservable);
      instance.SetDestructor(&destruct_TQGridScanObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQGridScanObservable*)
   {
      return GenerateInitInstanceLocal((::TQGridScanObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQGridScanObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TQHistParams_Dictionary();
   static void TQHistParams_TClassManip(TClass*);
   static void *new_TQHistParams(void *p = 0);
   static void *newArray_TQHistParams(Long_t size, void *p);
   static void delete_TQHistParams(void *p);
   static void deleteArray_TQHistParams(void *p);
   static void destruct_TQHistParams(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQHistParams*)
   {
      ::TQHistParams *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TQHistParams));
      static ::ROOT::TGenericClassInfo 
         instance("TQHistParams", "QFramework/TQGridScanObservable.h", 84,
                  typeid(::TQHistParams), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQHistParams_Dictionary, isa_proxy, 4,
                  sizeof(::TQHistParams) );
      instance.SetNew(&new_TQHistParams);
      instance.SetNewArray(&newArray_TQHistParams);
      instance.SetDelete(&delete_TQHistParams);
      instance.SetDeleteArray(&deleteArray_TQHistParams);
      instance.SetDestructor(&destruct_TQHistParams);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQHistParams*)
   {
      return GenerateInitInstanceLocal((::TQHistParams*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQHistParams*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQHistParams_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQHistParams*)0x0)->GetClass();
      TQHistParams_TClassManip(theClass);
   return theClass;
   }

   static void TQHistParams_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_TQGridScanNormalObservable(void *p = 0);
   static void *newArray_TQGridScanNormalObservable(Long_t size, void *p);
   static void delete_TQGridScanNormalObservable(void *p);
   static void deleteArray_TQGridScanNormalObservable(void *p);
   static void destruct_TQGridScanNormalObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQGridScanNormalObservable*)
   {
      ::TQGridScanNormalObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQGridScanNormalObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQGridScanNormalObservable", ::TQGridScanNormalObservable::Class_Version(), "QFramework/TQGridScanObservable.h", 128,
                  typeid(::TQGridScanNormalObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQGridScanNormalObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQGridScanNormalObservable) );
      instance.SetNew(&new_TQGridScanNormalObservable);
      instance.SetNewArray(&newArray_TQGridScanNormalObservable);
      instance.SetDelete(&delete_TQGridScanNormalObservable);
      instance.SetDeleteArray(&deleteArray_TQGridScanNormalObservable);
      instance.SetDestructor(&destruct_TQGridScanNormalObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQGridScanNormalObservable*)
   {
      return GenerateInitInstanceLocal((::TQGridScanNormalObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQGridScanNormalObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQGridScanSplitObservable(void *p = 0);
   static void *newArray_TQGridScanSplitObservable(Long_t size, void *p);
   static void delete_TQGridScanSplitObservable(void *p);
   static void deleteArray_TQGridScanSplitObservable(void *p);
   static void destruct_TQGridScanSplitObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQGridScanSplitObservable*)
   {
      ::TQGridScanSplitObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQGridScanSplitObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQGridScanSplitObservable", ::TQGridScanSplitObservable::Class_Version(), "QFramework/TQGridScanObservable.h", 156,
                  typeid(::TQGridScanSplitObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQGridScanSplitObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQGridScanSplitObservable) );
      instance.SetNew(&new_TQGridScanSplitObservable);
      instance.SetNewArray(&newArray_TQGridScanSplitObservable);
      instance.SetDelete(&delete_TQGridScanSplitObservable);
      instance.SetDeleteArray(&deleteArray_TQGridScanSplitObservable);
      instance.SetDestructor(&destruct_TQGridScanSplitObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQGridScanSplitObservable*)
   {
      return GenerateInitInstanceLocal((::TQGridScanSplitObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQGridScanSplitObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQGridScanPoint(void *p = 0);
   static void *newArray_TQGridScanPoint(Long_t size, void *p);
   static void delete_TQGridScanPoint(void *p);
   static void deleteArray_TQGridScanPoint(void *p);
   static void destruct_TQGridScanPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQGridScanPoint*)
   {
      ::TQGridScanPoint *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQGridScanPoint >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQGridScanPoint", ::TQGridScanPoint::Class_Version(), "QFramework/TQGridScanPoint.h", 18,
                  typeid(::TQGridScanPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQGridScanPoint::Dictionary, isa_proxy, 4,
                  sizeof(::TQGridScanPoint) );
      instance.SetNew(&new_TQGridScanPoint);
      instance.SetNewArray(&newArray_TQGridScanPoint);
      instance.SetDelete(&delete_TQGridScanPoint);
      instance.SetDeleteArray(&deleteArray_TQGridScanPoint);
      instance.SetDestructor(&destruct_TQGridScanPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQGridScanPoint*)
   {
      return GenerateInitInstanceLocal((::TQGridScanPoint*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQGridScanPoint*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQGridScanResults(void *p = 0);
   static void *newArray_TQGridScanResults(Long_t size, void *p);
   static void delete_TQGridScanResults(void *p);
   static void deleteArray_TQGridScanResults(void *p);
   static void destruct_TQGridScanResults(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQGridScanResults*)
   {
      ::TQGridScanResults *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQGridScanResults >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQGridScanResults", ::TQGridScanResults::Class_Version(), "QFramework/TQGridScanResults.h", 18,
                  typeid(::TQGridScanResults), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQGridScanResults::Dictionary, isa_proxy, 4,
                  sizeof(::TQGridScanResults) );
      instance.SetNew(&new_TQGridScanResults);
      instance.SetNewArray(&newArray_TQGridScanResults);
      instance.SetDelete(&delete_TQGridScanResults);
      instance.SetDeleteArray(&deleteArray_TQGridScanResults);
      instance.SetDestructor(&destruct_TQGridScanResults);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQGridScanResults*)
   {
      return GenerateInitInstanceLocal((::TQGridScanResults*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQGridScanResults*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQSignificanceEvaluator(void *p);
   static void deleteArray_TQSignificanceEvaluator(void *p);
   static void destruct_TQSignificanceEvaluator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSignificanceEvaluator*)
   {
      ::TQSignificanceEvaluator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSignificanceEvaluator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSignificanceEvaluator", ::TQSignificanceEvaluator::Class_Version(), "QFramework/TQSignificanceEvaluator.h", 17,
                  typeid(::TQSignificanceEvaluator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSignificanceEvaluator::Dictionary, isa_proxy, 4,
                  sizeof(::TQSignificanceEvaluator) );
      instance.SetDelete(&delete_TQSignificanceEvaluator);
      instance.SetDeleteArray(&deleteArray_TQSignificanceEvaluator);
      instance.SetDestructor(&destruct_TQSignificanceEvaluator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSignificanceEvaluator*)
   {
      return GenerateInitInstanceLocal((::TQSignificanceEvaluator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSignificanceEvaluator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQSignificanceEvaluatorBase(void *p = 0);
   static void *newArray_TQSignificanceEvaluatorBase(Long_t size, void *p);
   static void delete_TQSignificanceEvaluatorBase(void *p);
   static void deleteArray_TQSignificanceEvaluatorBase(void *p);
   static void destruct_TQSignificanceEvaluatorBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSignificanceEvaluatorBase*)
   {
      ::TQSignificanceEvaluatorBase *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSignificanceEvaluatorBase >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSignificanceEvaluatorBase", ::TQSignificanceEvaluatorBase::Class_Version(), "QFramework/TQSignificanceEvaluator.h", 77,
                  typeid(::TQSignificanceEvaluatorBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSignificanceEvaluatorBase::Dictionary, isa_proxy, 4,
                  sizeof(::TQSignificanceEvaluatorBase) );
      instance.SetNew(&new_TQSignificanceEvaluatorBase);
      instance.SetNewArray(&newArray_TQSignificanceEvaluatorBase);
      instance.SetDelete(&delete_TQSignificanceEvaluatorBase);
      instance.SetDeleteArray(&deleteArray_TQSignificanceEvaluatorBase);
      instance.SetDestructor(&destruct_TQSignificanceEvaluatorBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSignificanceEvaluatorBase*)
   {
      return GenerateInitInstanceLocal((::TQSignificanceEvaluatorBase*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSignificanceEvaluatorBase*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQSimpleSignificanceEvaluator(void *p = 0);
   static void *newArray_TQSimpleSignificanceEvaluator(Long_t size, void *p);
   static void delete_TQSimpleSignificanceEvaluator(void *p);
   static void deleteArray_TQSimpleSignificanceEvaluator(void *p);
   static void destruct_TQSimpleSignificanceEvaluator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSimpleSignificanceEvaluator*)
   {
      ::TQSimpleSignificanceEvaluator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSimpleSignificanceEvaluator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSimpleSignificanceEvaluator", ::TQSimpleSignificanceEvaluator::Class_Version(), "QFramework/TQSignificanceEvaluator.h", 100,
                  typeid(::TQSimpleSignificanceEvaluator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSimpleSignificanceEvaluator::Dictionary, isa_proxy, 4,
                  sizeof(::TQSimpleSignificanceEvaluator) );
      instance.SetNew(&new_TQSimpleSignificanceEvaluator);
      instance.SetNewArray(&newArray_TQSimpleSignificanceEvaluator);
      instance.SetDelete(&delete_TQSimpleSignificanceEvaluator);
      instance.SetDeleteArray(&deleteArray_TQSimpleSignificanceEvaluator);
      instance.SetDestructor(&destruct_TQSimpleSignificanceEvaluator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSimpleSignificanceEvaluator*)
   {
      return GenerateInitInstanceLocal((::TQSimpleSignificanceEvaluator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQSimpleSignificanceEvaluator2(void *p = 0);
   static void *newArray_TQSimpleSignificanceEvaluator2(Long_t size, void *p);
   static void delete_TQSimpleSignificanceEvaluator2(void *p);
   static void deleteArray_TQSimpleSignificanceEvaluator2(void *p);
   static void destruct_TQSimpleSignificanceEvaluator2(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSimpleSignificanceEvaluator2*)
   {
      ::TQSimpleSignificanceEvaluator2 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSimpleSignificanceEvaluator2 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSimpleSignificanceEvaluator2", ::TQSimpleSignificanceEvaluator2::Class_Version(), "QFramework/TQSignificanceEvaluator.h", 109,
                  typeid(::TQSimpleSignificanceEvaluator2), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSimpleSignificanceEvaluator2::Dictionary, isa_proxy, 4,
                  sizeof(::TQSimpleSignificanceEvaluator2) );
      instance.SetNew(&new_TQSimpleSignificanceEvaluator2);
      instance.SetNewArray(&newArray_TQSimpleSignificanceEvaluator2);
      instance.SetDelete(&delete_TQSimpleSignificanceEvaluator2);
      instance.SetDeleteArray(&deleteArray_TQSimpleSignificanceEvaluator2);
      instance.SetDestructor(&destruct_TQSimpleSignificanceEvaluator2);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSimpleSignificanceEvaluator2*)
   {
      return GenerateInitInstanceLocal((::TQSimpleSignificanceEvaluator2*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator2*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQSimpleSignificanceEvaluator3(void *p = 0);
   static void *newArray_TQSimpleSignificanceEvaluator3(Long_t size, void *p);
   static void delete_TQSimpleSignificanceEvaluator3(void *p);
   static void deleteArray_TQSimpleSignificanceEvaluator3(void *p);
   static void destruct_TQSimpleSignificanceEvaluator3(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSimpleSignificanceEvaluator3*)
   {
      ::TQSimpleSignificanceEvaluator3 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSimpleSignificanceEvaluator3 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSimpleSignificanceEvaluator3", ::TQSimpleSignificanceEvaluator3::Class_Version(), "QFramework/TQSignificanceEvaluator.h", 118,
                  typeid(::TQSimpleSignificanceEvaluator3), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSimpleSignificanceEvaluator3::Dictionary, isa_proxy, 4,
                  sizeof(::TQSimpleSignificanceEvaluator3) );
      instance.SetNew(&new_TQSimpleSignificanceEvaluator3);
      instance.SetNewArray(&newArray_TQSimpleSignificanceEvaluator3);
      instance.SetDelete(&delete_TQSimpleSignificanceEvaluator3);
      instance.SetDeleteArray(&deleteArray_TQSimpleSignificanceEvaluator3);
      instance.SetDestructor(&destruct_TQSimpleSignificanceEvaluator3);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSimpleSignificanceEvaluator3*)
   {
      return GenerateInitInstanceLocal((::TQSimpleSignificanceEvaluator3*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator3*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQPoissonSignificanceEvaluator(void *p = 0);
   static void *newArray_TQPoissonSignificanceEvaluator(Long_t size, void *p);
   static void delete_TQPoissonSignificanceEvaluator(void *p);
   static void deleteArray_TQPoissonSignificanceEvaluator(void *p);
   static void destruct_TQPoissonSignificanceEvaluator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQPoissonSignificanceEvaluator*)
   {
      ::TQPoissonSignificanceEvaluator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQPoissonSignificanceEvaluator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQPoissonSignificanceEvaluator", ::TQPoissonSignificanceEvaluator::Class_Version(), "QFramework/TQSignificanceEvaluator.h", 127,
                  typeid(::TQPoissonSignificanceEvaluator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQPoissonSignificanceEvaluator::Dictionary, isa_proxy, 4,
                  sizeof(::TQPoissonSignificanceEvaluator) );
      instance.SetNew(&new_TQPoissonSignificanceEvaluator);
      instance.SetNewArray(&newArray_TQPoissonSignificanceEvaluator);
      instance.SetDelete(&delete_TQPoissonSignificanceEvaluator);
      instance.SetDeleteArray(&deleteArray_TQPoissonSignificanceEvaluator);
      instance.SetDestructor(&destruct_TQPoissonSignificanceEvaluator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQPoissonSignificanceEvaluator*)
   {
      return GenerateInitInstanceLocal((::TQPoissonSignificanceEvaluator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQPoissonSignificanceEvaluator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQGridScanner(void *p = 0);
   static void *newArray_TQGridScanner(Long_t size, void *p);
   static void delete_TQGridScanner(void *p);
   static void deleteArray_TQGridScanner(void *p);
   static void destruct_TQGridScanner(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQGridScanner*)
   {
      ::TQGridScanner *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQGridScanner >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQGridScanner", ::TQGridScanner::Class_Version(), "QFramework/TQGridScanner.h", 17,
                  typeid(::TQGridScanner), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQGridScanner::Dictionary, isa_proxy, 4,
                  sizeof(::TQGridScanner) );
      instance.SetNew(&new_TQGridScanner);
      instance.SetNewArray(&newArray_TQGridScanner);
      instance.SetDelete(&delete_TQGridScanner);
      instance.SetDeleteArray(&deleteArray_TQGridScanner);
      instance.SetDestructor(&destruct_TQGridScanner);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQGridScanner*)
   {
      return GenerateInitInstanceLocal((::TQGridScanner*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQGridScanner*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQHWWPlotter(void *p = 0);
   static void *newArray_TQHWWPlotter(Long_t size, void *p);
   static void delete_TQHWWPlotter(void *p);
   static void deleteArray_TQHWWPlotter(void *p);
   static void destruct_TQHWWPlotter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQHWWPlotter*)
   {
      ::TQHWWPlotter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQHWWPlotter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQHWWPlotter", ::TQHWWPlotter::Class_Version(), "QFramework/TQHWWPlotter.h", 26,
                  typeid(::TQHWWPlotter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQHWWPlotter::Dictionary, isa_proxy, 4,
                  sizeof(::TQHWWPlotter) );
      instance.SetNew(&new_TQHWWPlotter);
      instance.SetNewArray(&newArray_TQHWWPlotter);
      instance.SetDelete(&delete_TQHWWPlotter);
      instance.SetDeleteArray(&deleteArray_TQHWWPlotter);
      instance.SetDestructor(&destruct_TQHWWPlotter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQHWWPlotter*)
   {
      return GenerateInitInstanceLocal((::TQHWWPlotter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQHWWPlotter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQHistComparer(void *p);
   static void deleteArray_TQHistComparer(void *p);
   static void destruct_TQHistComparer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQHistComparer*)
   {
      ::TQHistComparer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQHistComparer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQHistComparer", ::TQHistComparer::Class_Version(), "QFramework/TQHistComparer.h", 8,
                  typeid(::TQHistComparer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQHistComparer::Dictionary, isa_proxy, 4,
                  sizeof(::TQHistComparer) );
      instance.SetDelete(&delete_TQHistComparer);
      instance.SetDeleteArray(&deleteArray_TQHistComparer);
      instance.SetDestructor(&destruct_TQHistComparer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQHistComparer*)
   {
      return GenerateInitInstanceLocal((::TQHistComparer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQHistComparer*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQHistoMakerAnalysisJob(void *p = 0);
   static void *newArray_TQHistoMakerAnalysisJob(Long_t size, void *p);
   static void delete_TQHistoMakerAnalysisJob(void *p);
   static void deleteArray_TQHistoMakerAnalysisJob(void *p);
   static void destruct_TQHistoMakerAnalysisJob(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQHistoMakerAnalysisJob*)
   {
      ::TQHistoMakerAnalysisJob *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQHistoMakerAnalysisJob >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQHistoMakerAnalysisJob", ::TQHistoMakerAnalysisJob::Class_Version(), "QFramework/TQHistoMakerAnalysisJob.h", 12,
                  typeid(::TQHistoMakerAnalysisJob), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQHistoMakerAnalysisJob::Dictionary, isa_proxy, 4,
                  sizeof(::TQHistoMakerAnalysisJob) );
      instance.SetNew(&new_TQHistoMakerAnalysisJob);
      instance.SetNewArray(&newArray_TQHistoMakerAnalysisJob);
      instance.SetDelete(&delete_TQHistoMakerAnalysisJob);
      instance.SetDeleteArray(&deleteArray_TQHistoMakerAnalysisJob);
      instance.SetDestructor(&destruct_TQHistoMakerAnalysisJob);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQHistoMakerAnalysisJob*)
   {
      return GenerateInitInstanceLocal((::TQHistoMakerAnalysisJob*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQHistoMakerAnalysisJob*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TQHistogramObservablelETH1gR_Dictionary();
   static void TQHistogramObservablelETH1gR_TClassManip(TClass*);
   static void *new_TQHistogramObservablelETH1gR(void *p = 0);
   static void *newArray_TQHistogramObservablelETH1gR(Long_t size, void *p);
   static void delete_TQHistogramObservablelETH1gR(void *p);
   static void deleteArray_TQHistogramObservablelETH1gR(void *p);
   static void destruct_TQHistogramObservablelETH1gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQHistogramObservable<TH1>*)
   {
      ::TQHistogramObservable<TH1> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQHistogramObservable<TH1> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQHistogramObservable<TH1>", ::TQHistogramObservable<TH1>::Class_Version(), "QFramework/TQHistogramObservable.h", 11,
                  typeid(::TQHistogramObservable<TH1>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQHistogramObservablelETH1gR_Dictionary, isa_proxy, 4,
                  sizeof(::TQHistogramObservable<TH1>) );
      instance.SetNew(&new_TQHistogramObservablelETH1gR);
      instance.SetNewArray(&newArray_TQHistogramObservablelETH1gR);
      instance.SetDelete(&delete_TQHistogramObservablelETH1gR);
      instance.SetDeleteArray(&deleteArray_TQHistogramObservablelETH1gR);
      instance.SetDestructor(&destruct_TQHistogramObservablelETH1gR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQHistogramObservable<TH1>*)
   {
      return GenerateInitInstanceLocal((::TQHistogramObservable<TH1>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQHistogramObservable<TH1>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQHistogramObservablelETH1gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH1>*)0x0)->GetClass();
      TQHistogramObservablelETH1gR_TClassManip(theClass);
   return theClass;
   }

   static void TQHistogramObservablelETH1gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQHistogramObservablelETH2gR_Dictionary();
   static void TQHistogramObservablelETH2gR_TClassManip(TClass*);
   static void *new_TQHistogramObservablelETH2gR(void *p = 0);
   static void *newArray_TQHistogramObservablelETH2gR(Long_t size, void *p);
   static void delete_TQHistogramObservablelETH2gR(void *p);
   static void deleteArray_TQHistogramObservablelETH2gR(void *p);
   static void destruct_TQHistogramObservablelETH2gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQHistogramObservable<TH2>*)
   {
      ::TQHistogramObservable<TH2> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQHistogramObservable<TH2> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQHistogramObservable<TH2>", ::TQHistogramObservable<TH2>::Class_Version(), "QFramework/TQHistogramObservable.h", 11,
                  typeid(::TQHistogramObservable<TH2>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQHistogramObservablelETH2gR_Dictionary, isa_proxy, 4,
                  sizeof(::TQHistogramObservable<TH2>) );
      instance.SetNew(&new_TQHistogramObservablelETH2gR);
      instance.SetNewArray(&newArray_TQHistogramObservablelETH2gR);
      instance.SetDelete(&delete_TQHistogramObservablelETH2gR);
      instance.SetDeleteArray(&deleteArray_TQHistogramObservablelETH2gR);
      instance.SetDestructor(&destruct_TQHistogramObservablelETH2gR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQHistogramObservable<TH2>*)
   {
      return GenerateInitInstanceLocal((::TQHistogramObservable<TH2>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQHistogramObservable<TH2>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQHistogramObservablelETH2gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH2>*)0x0)->GetClass();
      TQHistogramObservablelETH2gR_TClassManip(theClass);
   return theClass;
   }

   static void TQHistogramObservablelETH2gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TQHistogramObservablelETH3gR_Dictionary();
   static void TQHistogramObservablelETH3gR_TClassManip(TClass*);
   static void *new_TQHistogramObservablelETH3gR(void *p = 0);
   static void *newArray_TQHistogramObservablelETH3gR(Long_t size, void *p);
   static void delete_TQHistogramObservablelETH3gR(void *p);
   static void deleteArray_TQHistogramObservablelETH3gR(void *p);
   static void destruct_TQHistogramObservablelETH3gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQHistogramObservable<TH3>*)
   {
      ::TQHistogramObservable<TH3> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQHistogramObservable<TH3> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQHistogramObservable<TH3>", ::TQHistogramObservable<TH3>::Class_Version(), "QFramework/TQHistogramObservable.h", 11,
                  typeid(::TQHistogramObservable<TH3>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TQHistogramObservablelETH3gR_Dictionary, isa_proxy, 4,
                  sizeof(::TQHistogramObservable<TH3>) );
      instance.SetNew(&new_TQHistogramObservablelETH3gR);
      instance.SetNewArray(&newArray_TQHistogramObservablelETH3gR);
      instance.SetDelete(&delete_TQHistogramObservablelETH3gR);
      instance.SetDeleteArray(&deleteArray_TQHistogramObservablelETH3gR);
      instance.SetDestructor(&destruct_TQHistogramObservablelETH3gR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQHistogramObservable<TH3>*)
   {
      return GenerateInitInstanceLocal((::TQHistogramObservable<TH3>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQHistogramObservable<TH3>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TQHistogramObservablelETH3gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH3>*)0x0)->GetClass();
      TQHistogramObservablelETH3gR_TClassManip(theClass);
   return theClass;
   }

   static void TQHistogramObservablelETH3gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_TQLink(void *p);
   static void deleteArray_TQLink(void *p);
   static void destruct_TQLink(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQLink*)
   {
      ::TQLink *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQLink >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQLink", ::TQLink::Class_Version(), "QFramework/TQLink.h", 7,
                  typeid(::TQLink), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQLink::Dictionary, isa_proxy, 4,
                  sizeof(::TQLink) );
      instance.SetDelete(&delete_TQLink);
      instance.SetDeleteArray(&deleteArray_TQLink);
      instance.SetDestructor(&destruct_TQLink);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQLink*)
   {
      return GenerateInitInstanceLocal((::TQLink*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQLink*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQImportLink(void *p = 0);
   static void *newArray_TQImportLink(Long_t size, void *p);
   static void delete_TQImportLink(void *p);
   static void deleteArray_TQImportLink(void *p);
   static void destruct_TQImportLink(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQImportLink*)
   {
      ::TQImportLink *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQImportLink >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQImportLink", ::TQImportLink::Class_Version(), "QFramework/TQImportLink.h", 8,
                  typeid(::TQImportLink), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQImportLink::Dictionary, isa_proxy, 4,
                  sizeof(::TQImportLink) );
      instance.SetNew(&new_TQImportLink);
      instance.SetNewArray(&newArray_TQImportLink);
      instance.SetDelete(&delete_TQImportLink);
      instance.SetDeleteArray(&deleteArray_TQImportLink);
      instance.SetDestructor(&destruct_TQImportLink);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQImportLink*)
   {
      return GenerateInitInstanceLocal((::TQImportLink*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQImportLink*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQLibrary(void *p = 0);
   static void *newArray_TQLibrary(Long_t size, void *p);
   static void delete_TQLibrary(void *p);
   static void deleteArray_TQLibrary(void *p);
   static void destruct_TQLibrary(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQLibrary*)
   {
      ::TQLibrary *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQLibrary >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQLibrary", ::TQLibrary::Class_Version(), "QFramework/TQLibrary.h", 78,
                  typeid(::TQLibrary), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQLibrary::Dictionary, isa_proxy, 4,
                  sizeof(::TQLibrary) );
      instance.SetNew(&new_TQLibrary);
      instance.SetNewArray(&newArray_TQLibrary);
      instance.SetDelete(&delete_TQLibrary);
      instance.SetDeleteArray(&deleteArray_TQLibrary);
      instance.SetDestructor(&destruct_TQLibrary);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQLibrary*)
   {
      return GenerateInitInstanceLocal((::TQLibrary*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQLibrary*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQToken(void *p = 0);
   static void *newArray_TQToken(Long_t size, void *p);
   static void delete_TQToken(void *p);
   static void deleteArray_TQToken(void *p);
   static void destruct_TQToken(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQToken*)
   {
      ::TQToken *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQToken >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQToken", ::TQToken::Class_Version(), "QFramework/TQToken.h", 7,
                  typeid(::TQToken), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQToken::Dictionary, isa_proxy, 4,
                  sizeof(::TQToken) );
      instance.SetNew(&new_TQToken);
      instance.SetNewArray(&newArray_TQToken);
      instance.SetDelete(&delete_TQToken);
      instance.SetDeleteArray(&deleteArray_TQToken);
      instance.SetDestructor(&destruct_TQToken);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQToken*)
   {
      return GenerateInitInstanceLocal((::TQToken*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQToken*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQMVA(void *p = 0);
   static void *newArray_TQMVA(Long_t size, void *p);
   static void delete_TQMVA(void *p);
   static void deleteArray_TQMVA(void *p);
   static void destruct_TQMVA(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQMVA*)
   {
      ::TQMVA *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQMVA >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQMVA", ::TQMVA::Class_Version(), "QFramework/TQMVA.h", 27,
                  typeid(::TQMVA), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQMVA::Dictionary, isa_proxy, 4,
                  sizeof(::TQMVA) );
      instance.SetNew(&new_TQMVA);
      instance.SetNewArray(&newArray_TQMVA);
      instance.SetDelete(&delete_TQMVA);
      instance.SetDeleteArray(&deleteArray_TQMVA);
      instance.SetDestructor(&destruct_TQMVA);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQMVA*)
   {
      return GenerateInitInstanceLocal((::TQMVA*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQMVA*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQMVAObservable(void *p = 0);
   static void *newArray_TQMVAObservable(Long_t size, void *p);
   static void delete_TQMVAObservable(void *p);
   static void deleteArray_TQMVAObservable(void *p);
   static void destruct_TQMVAObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQMVAObservable*)
   {
      ::TQMVAObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQMVAObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQMVAObservable", ::TQMVAObservable::Class_Version(), "QFramework/TQMVAObservable.h", 9,
                  typeid(::TQMVAObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQMVAObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQMVAObservable) );
      instance.SetNew(&new_TQMVAObservable);
      instance.SetNewArray(&newArray_TQMVAObservable);
      instance.SetDelete(&delete_TQMVAObservable);
      instance.SetDeleteArray(&deleteArray_TQMVAObservable);
      instance.SetDestructor(&destruct_TQMVAObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQMVAObservable*)
   {
      return GenerateInitInstanceLocal((::TQMVAObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQMVAObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQMultiChannelAnalysisSampleVisitor(void *p = 0);
   static void *newArray_TQMultiChannelAnalysisSampleVisitor(Long_t size, void *p);
   static void delete_TQMultiChannelAnalysisSampleVisitor(void *p);
   static void deleteArray_TQMultiChannelAnalysisSampleVisitor(void *p);
   static void destruct_TQMultiChannelAnalysisSampleVisitor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQMultiChannelAnalysisSampleVisitor*)
   {
      ::TQMultiChannelAnalysisSampleVisitor *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQMultiChannelAnalysisSampleVisitor >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQMultiChannelAnalysisSampleVisitor", ::TQMultiChannelAnalysisSampleVisitor::Class_Version(), "QFramework/TQMultiChannelAnalysisSampleVisitor.h", 13,
                  typeid(::TQMultiChannelAnalysisSampleVisitor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQMultiChannelAnalysisSampleVisitor::Dictionary, isa_proxy, 4,
                  sizeof(::TQMultiChannelAnalysisSampleVisitor) );
      instance.SetNew(&new_TQMultiChannelAnalysisSampleVisitor);
      instance.SetNewArray(&newArray_TQMultiChannelAnalysisSampleVisitor);
      instance.SetDelete(&delete_TQMultiChannelAnalysisSampleVisitor);
      instance.SetDeleteArray(&deleteArray_TQMultiChannelAnalysisSampleVisitor);
      instance.SetDestructor(&destruct_TQMultiChannelAnalysisSampleVisitor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQMultiChannelAnalysisSampleVisitor*)
   {
      return GenerateInitInstanceLocal((::TQMultiChannelAnalysisSampleVisitor*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQMultiChannelAnalysisSampleVisitor*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQMultiObservable(void *p = 0);
   static void *newArray_TQMultiObservable(Long_t size, void *p);
   static void delete_TQMultiObservable(void *p);
   static void deleteArray_TQMultiObservable(void *p);
   static void destruct_TQMultiObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQMultiObservable*)
   {
      ::TQMultiObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQMultiObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQMultiObservable", ::TQMultiObservable::Class_Version(), "QFramework/TQMultiObservable.h", 8,
                  typeid(::TQMultiObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQMultiObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQMultiObservable) );
      instance.SetNew(&new_TQMultiObservable);
      instance.SetNewArray(&newArray_TQMultiObservable);
      instance.SetDelete(&delete_TQMultiObservable);
      instance.SetDeleteArray(&deleteArray_TQMultiObservable);
      instance.SetDestructor(&destruct_TQMultiObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQMultiObservable*)
   {
      return GenerateInitInstanceLocal((::TQMultiObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQMultiObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQNFCalculator(void *p = 0);
   static void *newArray_TQNFCalculator(Long_t size, void *p);
   static void delete_TQNFCalculator(void *p);
   static void deleteArray_TQNFCalculator(void *p);
   static void destruct_TQNFCalculator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQNFCalculator*)
   {
      ::TQNFCalculator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQNFCalculator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQNFCalculator", ::TQNFCalculator::Class_Version(), "QFramework/TQNFCalculator.h", 13,
                  typeid(::TQNFCalculator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQNFCalculator::Dictionary, isa_proxy, 4,
                  sizeof(::TQNFCalculator) );
      instance.SetNew(&new_TQNFCalculator);
      instance.SetNewArray(&newArray_TQNFCalculator);
      instance.SetDelete(&delete_TQNFCalculator);
      instance.SetDeleteArray(&deleteArray_TQNFCalculator);
      instance.SetDestructor(&destruct_TQNFCalculator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQNFCalculator*)
   {
      return GenerateInitInstanceLocal((::TQNFCalculator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQNFCalculator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQNFChainloader(void *p = 0);
   static void *newArray_TQNFChainloader(Long_t size, void *p);
   static void delete_TQNFChainloader(void *p);
   static void deleteArray_TQNFChainloader(void *p);
   static void destruct_TQNFChainloader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQNFChainloader*)
   {
      ::TQNFChainloader *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQNFChainloader >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQNFChainloader", ::TQNFChainloader::Class_Version(), "QFramework/TQNFChainloader.h", 16,
                  typeid(::TQNFChainloader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQNFChainloader::Dictionary, isa_proxy, 4,
                  sizeof(::TQNFChainloader) );
      instance.SetNew(&new_TQNFChainloader);
      instance.SetNewArray(&newArray_TQNFChainloader);
      instance.SetDelete(&delete_TQNFChainloader);
      instance.SetDeleteArray(&deleteArray_TQNFChainloader);
      instance.SetDestructor(&destruct_TQNFChainloader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQNFChainloader*)
   {
      return GenerateInitInstanceLocal((::TQNFChainloader*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQNFChainloader*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQNFCustomCalculator(void *p = 0);
   static void *newArray_TQNFCustomCalculator(Long_t size, void *p);
   static void delete_TQNFCustomCalculator(void *p);
   static void deleteArray_TQNFCustomCalculator(void *p);
   static void destruct_TQNFCustomCalculator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQNFCustomCalculator*)
   {
      ::TQNFCustomCalculator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQNFCustomCalculator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQNFCustomCalculator", ::TQNFCustomCalculator::Class_Version(), "QFramework/TQNFCustomCalculator.h", 9,
                  typeid(::TQNFCustomCalculator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQNFCustomCalculator::Dictionary, isa_proxy, 4,
                  sizeof(::TQNFCustomCalculator) );
      instance.SetNew(&new_TQNFCustomCalculator);
      instance.SetNewArray(&newArray_TQNFCustomCalculator);
      instance.SetDelete(&delete_TQNFCustomCalculator);
      instance.SetDeleteArray(&deleteArray_TQNFCustomCalculator);
      instance.SetDestructor(&destruct_TQNFCustomCalculator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQNFCustomCalculator*)
   {
      return GenerateInitInstanceLocal((::TQNFCustomCalculator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQNFCustomCalculator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQNFManualSetter(void *p = 0);
   static void *newArray_TQNFManualSetter(Long_t size, void *p);
   static void delete_TQNFManualSetter(void *p);
   static void deleteArray_TQNFManualSetter(void *p);
   static void destruct_TQNFManualSetter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQNFManualSetter*)
   {
      ::TQNFManualSetter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQNFManualSetter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQNFManualSetter", ::TQNFManualSetter::Class_Version(), "QFramework/TQNFManualSetter.h", 10,
                  typeid(::TQNFManualSetter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQNFManualSetter::Dictionary, isa_proxy, 4,
                  sizeof(::TQNFManualSetter) );
      instance.SetNew(&new_TQNFManualSetter);
      instance.SetNewArray(&newArray_TQNFManualSetter);
      instance.SetDelete(&delete_TQNFManualSetter);
      instance.SetDeleteArray(&deleteArray_TQNFManualSetter);
      instance.SetDestructor(&destruct_TQNFManualSetter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQNFManualSetter*)
   {
      return GenerateInitInstanceLocal((::TQNFManualSetter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQNFManualSetter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQNFTop0jetEstimator(void *p = 0);
   static void *newArray_TQNFTop0jetEstimator(Long_t size, void *p);
   static void delete_TQNFTop0jetEstimator(void *p);
   static void deleteArray_TQNFTop0jetEstimator(void *p);
   static void destruct_TQNFTop0jetEstimator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQNFTop0jetEstimator*)
   {
      ::TQNFTop0jetEstimator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQNFTop0jetEstimator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQNFTop0jetEstimator", ::TQNFTop0jetEstimator::Class_Version(), "QFramework/TQNFTop0jetEstimator.h", 9,
                  typeid(::TQNFTop0jetEstimator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQNFTop0jetEstimator::Dictionary, isa_proxy, 4,
                  sizeof(::TQNFTop0jetEstimator) );
      instance.SetNew(&new_TQNFTop0jetEstimator);
      instance.SetNewArray(&newArray_TQNFTop0jetEstimator);
      instance.SetDelete(&delete_TQNFTop0jetEstimator);
      instance.SetDeleteArray(&deleteArray_TQNFTop0jetEstimator);
      instance.SetDestructor(&destruct_TQNFTop0jetEstimator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQNFTop0jetEstimator*)
   {
      return GenerateInitInstanceLocal((::TQNFTop0jetEstimator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQNFTop0jetEstimator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQNFTop1jetEstimator(void *p = 0);
   static void *newArray_TQNFTop1jetEstimator(Long_t size, void *p);
   static void delete_TQNFTop1jetEstimator(void *p);
   static void deleteArray_TQNFTop1jetEstimator(void *p);
   static void destruct_TQNFTop1jetEstimator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQNFTop1jetEstimator*)
   {
      ::TQNFTop1jetEstimator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQNFTop1jetEstimator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQNFTop1jetEstimator", ::TQNFTop1jetEstimator::Class_Version(), "QFramework/TQNFTop1jetEstimator.h", 9,
                  typeid(::TQNFTop1jetEstimator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQNFTop1jetEstimator::Dictionary, isa_proxy, 4,
                  sizeof(::TQNFTop1jetEstimator) );
      instance.SetNew(&new_TQNFTop1jetEstimator);
      instance.SetNewArray(&newArray_TQNFTop1jetEstimator);
      instance.SetDelete(&delete_TQNFTop1jetEstimator);
      instance.SetDeleteArray(&deleteArray_TQNFTop1jetEstimator);
      instance.SetDestructor(&destruct_TQNFTop1jetEstimator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQNFTop1jetEstimator*)
   {
      return GenerateInitInstanceLocal((::TQNFTop1jetEstimator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQNFTop1jetEstimator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQNFUncertaintyScaler(void *p = 0);
   static void *newArray_TQNFUncertaintyScaler(Long_t size, void *p);
   static void delete_TQNFUncertaintyScaler(void *p);
   static void deleteArray_TQNFUncertaintyScaler(void *p);
   static void destruct_TQNFUncertaintyScaler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQNFUncertaintyScaler*)
   {
      ::TQNFUncertaintyScaler *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQNFUncertaintyScaler >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQNFUncertaintyScaler", ::TQNFUncertaintyScaler::Class_Version(), "QFramework/TQNFUncertaintyScaler.h", 10,
                  typeid(::TQNFUncertaintyScaler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQNFUncertaintyScaler::Dictionary, isa_proxy, 4,
                  sizeof(::TQNFUncertaintyScaler) );
      instance.SetNew(&new_TQNFUncertaintyScaler);
      instance.SetNewArray(&newArray_TQNFUncertaintyScaler);
      instance.SetDelete(&delete_TQNFUncertaintyScaler);
      instance.SetDeleteArray(&deleteArray_TQNFUncertaintyScaler);
      instance.SetDestructor(&destruct_TQNFUncertaintyScaler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQNFUncertaintyScaler*)
   {
      return GenerateInitInstanceLocal((::TQNFUncertaintyScaler*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQNFUncertaintyScaler*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQNTupleDumperAnalysisJob(void *p);
   static void deleteArray_TQNTupleDumperAnalysisJob(void *p);
   static void destruct_TQNTupleDumperAnalysisJob(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQNTupleDumperAnalysisJob*)
   {
      ::TQNTupleDumperAnalysisJob *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQNTupleDumperAnalysisJob >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQNTupleDumperAnalysisJob", ::TQNTupleDumperAnalysisJob::Class_Version(), "QFramework/TQNTupleDumperAnalysisJob.h", 24,
                  typeid(::TQNTupleDumperAnalysisJob), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQNTupleDumperAnalysisJob::Dictionary, isa_proxy, 4,
                  sizeof(::TQNTupleDumperAnalysisJob) );
      instance.SetDelete(&delete_TQNTupleDumperAnalysisJob);
      instance.SetDeleteArray(&deleteArray_TQNTupleDumperAnalysisJob);
      instance.SetDestructor(&destruct_TQNTupleDumperAnalysisJob);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQNTupleDumperAnalysisJob*)
   {
      return GenerateInitInstanceLocal((::TQNTupleDumperAnalysisJob*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQNTupleDumperAnalysisJob*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQPCAAnalysisJob(void *p = 0);
   static void *newArray_TQPCAAnalysisJob(Long_t size, void *p);
   static void delete_TQPCAAnalysisJob(void *p);
   static void deleteArray_TQPCAAnalysisJob(void *p);
   static void destruct_TQPCAAnalysisJob(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQPCAAnalysisJob*)
   {
      ::TQPCAAnalysisJob *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQPCAAnalysisJob >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQPCAAnalysisJob", ::TQPCAAnalysisJob::Class_Version(), "QFramework/TQPCAAnalysisJob.h", 12,
                  typeid(::TQPCAAnalysisJob), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQPCAAnalysisJob::Dictionary, isa_proxy, 4,
                  sizeof(::TQPCAAnalysisJob) );
      instance.SetNew(&new_TQPCAAnalysisJob);
      instance.SetNewArray(&newArray_TQPCAAnalysisJob);
      instance.SetDelete(&delete_TQPCAAnalysisJob);
      instance.SetDeleteArray(&deleteArray_TQPCAAnalysisJob);
      instance.SetDestructor(&destruct_TQPCAAnalysisJob);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQPCAAnalysisJob*)
   {
      return GenerateInitInstanceLocal((::TQPCAAnalysisJob*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQPCAAnalysisJob*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQPathManager(void *p = 0);
   static void *newArray_TQPathManager(Long_t size, void *p);
   static void delete_TQPathManager(void *p);
   static void deleteArray_TQPathManager(void *p);
   static void destruct_TQPathManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQPathManager*)
   {
      ::TQPathManager *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQPathManager >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQPathManager", ::TQPathManager::Class_Version(), "QFramework/TQPathManager.h", 9,
                  typeid(::TQPathManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQPathManager::Dictionary, isa_proxy, 4,
                  sizeof(::TQPathManager) );
      instance.SetNew(&new_TQPathManager);
      instance.SetNewArray(&newArray_TQPathManager);
      instance.SetDelete(&delete_TQPathManager);
      instance.SetDeleteArray(&deleteArray_TQPathManager);
      instance.SetDestructor(&destruct_TQPathManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQPathManager*)
   {
      return GenerateInitInstanceLocal((::TQPathManager*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQPathManager*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQSampleGroupingVisitor(void *p);
   static void deleteArray_TQSampleGroupingVisitor(void *p);
   static void destruct_TQSampleGroupingVisitor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSampleGroupingVisitor*)
   {
      ::TQSampleGroupingVisitor *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSampleGroupingVisitor >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSampleGroupingVisitor", ::TQSampleGroupingVisitor::Class_Version(), "QFramework/TQSampleGroupingVisitor.h", 14,
                  typeid(::TQSampleGroupingVisitor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSampleGroupingVisitor::Dictionary, isa_proxy, 4,
                  sizeof(::TQSampleGroupingVisitor) );
      instance.SetDelete(&delete_TQSampleGroupingVisitor);
      instance.SetDeleteArray(&deleteArray_TQSampleGroupingVisitor);
      instance.SetDestructor(&destruct_TQSampleGroupingVisitor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSampleGroupingVisitor*)
   {
      return GenerateInitInstanceLocal((::TQSampleGroupingVisitor*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSampleGroupingVisitor*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQSampleInitializerBase(void *p);
   static void deleteArray_TQSampleInitializerBase(void *p);
   static void destruct_TQSampleInitializerBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSampleInitializerBase*)
   {
      ::TQSampleInitializerBase *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSampleInitializerBase >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSampleInitializerBase", ::TQSampleInitializerBase::Class_Version(), "QFramework/TQSampleInitializerBase.h", 7,
                  typeid(::TQSampleInitializerBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSampleInitializerBase::Dictionary, isa_proxy, 4,
                  sizeof(::TQSampleInitializerBase) );
      instance.SetDelete(&delete_TQSampleInitializerBase);
      instance.SetDeleteArray(&deleteArray_TQSampleInitializerBase);
      instance.SetDestructor(&destruct_TQSampleInitializerBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSampleInitializerBase*)
   {
      return GenerateInitInstanceLocal((::TQSampleInitializerBase*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSampleInitializerBase*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQSampleInitializer(void *p = 0);
   static void *newArray_TQSampleInitializer(Long_t size, void *p);
   static void delete_TQSampleInitializer(void *p);
   static void deleteArray_TQSampleInitializer(void *p);
   static void destruct_TQSampleInitializer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSampleInitializer*)
   {
      ::TQSampleInitializer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSampleInitializer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSampleInitializer", ::TQSampleInitializer::Class_Version(), "QFramework/TQSampleInitializer.h", 10,
                  typeid(::TQSampleInitializer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSampleInitializer::Dictionary, isa_proxy, 4,
                  sizeof(::TQSampleInitializer) );
      instance.SetNew(&new_TQSampleInitializer);
      instance.SetNewArray(&newArray_TQSampleInitializer);
      instance.SetDelete(&delete_TQSampleInitializer);
      instance.SetDeleteArray(&deleteArray_TQSampleInitializer);
      instance.SetDestructor(&destruct_TQSampleInitializer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSampleInitializer*)
   {
      return GenerateInitInstanceLocal((::TQSampleInitializer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSampleInitializer*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQSampleNormalizationObservable(void *p = 0);
   static void *newArray_TQSampleNormalizationObservable(Long_t size, void *p);
   static void delete_TQSampleNormalizationObservable(void *p);
   static void deleteArray_TQSampleNormalizationObservable(void *p);
   static void destruct_TQSampleNormalizationObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSampleNormalizationObservable*)
   {
      ::TQSampleNormalizationObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSampleNormalizationObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSampleNormalizationObservable", ::TQSampleNormalizationObservable::Class_Version(), "QFramework/TQSampleNormalizationObservable.h", 6,
                  typeid(::TQSampleNormalizationObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSampleNormalizationObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQSampleNormalizationObservable) );
      instance.SetNew(&new_TQSampleNormalizationObservable);
      instance.SetNewArray(&newArray_TQSampleNormalizationObservable);
      instance.SetDelete(&delete_TQSampleNormalizationObservable);
      instance.SetDeleteArray(&deleteArray_TQSampleNormalizationObservable);
      instance.SetDestructor(&destruct_TQSampleNormalizationObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSampleNormalizationObservable*)
   {
      return GenerateInitInstanceLocal((::TQSampleNormalizationObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSampleNormalizationObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQSamplePurger(void *p);
   static void deleteArray_TQSamplePurger(void *p);
   static void destruct_TQSamplePurger(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSamplePurger*)
   {
      ::TQSamplePurger *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSamplePurger >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSamplePurger", ::TQSamplePurger::Class_Version(), "QFramework/TQSamplePurger.h", 7,
                  typeid(::TQSamplePurger), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSamplePurger::Dictionary, isa_proxy, 4,
                  sizeof(::TQSamplePurger) );
      instance.SetDelete(&delete_TQSamplePurger);
      instance.SetDeleteArray(&deleteArray_TQSamplePurger);
      instance.SetDestructor(&destruct_TQSamplePurger);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSamplePurger*)
   {
      return GenerateInitInstanceLocal((::TQSamplePurger*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSamplePurger*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQSampleRevisitor(void *p = 0);
   static void *newArray_TQSampleRevisitor(Long_t size, void *p);
   static void delete_TQSampleRevisitor(void *p);
   static void deleteArray_TQSampleRevisitor(void *p);
   static void destruct_TQSampleRevisitor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSampleRevisitor*)
   {
      ::TQSampleRevisitor *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSampleRevisitor >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSampleRevisitor", ::TQSampleRevisitor::Class_Version(), "QFramework/TQSampleRevisitor.h", 10,
                  typeid(::TQSampleRevisitor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSampleRevisitor::Dictionary, isa_proxy, 4,
                  sizeof(::TQSampleRevisitor) );
      instance.SetNew(&new_TQSampleRevisitor);
      instance.SetNewArray(&newArray_TQSampleRevisitor);
      instance.SetDelete(&delete_TQSampleRevisitor);
      instance.SetDeleteArray(&deleteArray_TQSampleRevisitor);
      instance.SetDestructor(&destruct_TQSampleRevisitor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSampleRevisitor*)
   {
      return GenerateInitInstanceLocal((::TQSampleRevisitor*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSampleRevisitor*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQSystematicsManager(void *p = 0);
   static void *newArray_TQSystematicsManager(Long_t size, void *p);
   static void delete_TQSystematicsManager(void *p);
   static void deleteArray_TQSystematicsManager(void *p);
   static void destruct_TQSystematicsManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSystematicsManager*)
   {
      ::TQSystematicsManager *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSystematicsManager >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSystematicsManager", ::TQSystematicsManager::Class_Version(), "QFramework/TQSystematicsManager.h", 9,
                  typeid(::TQSystematicsManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSystematicsManager::Dictionary, isa_proxy, 4,
                  sizeof(::TQSystematicsManager) );
      instance.SetNew(&new_TQSystematicsManager);
      instance.SetNewArray(&newArray_TQSystematicsManager);
      instance.SetDelete(&delete_TQSystematicsManager);
      instance.SetDeleteArray(&deleteArray_TQSystematicsManager);
      instance.SetDestructor(&destruct_TQSystematicsManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSystematicsManager*)
   {
      return GenerateInitInstanceLocal((::TQSystematicsManager*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSystematicsManager*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQSystematicsHandler(void *p);
   static void deleteArray_TQSystematicsHandler(void *p);
   static void destruct_TQSystematicsHandler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQSystematicsHandler*)
   {
      ::TQSystematicsHandler *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQSystematicsHandler >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQSystematicsHandler", ::TQSystematicsHandler::Class_Version(), "QFramework/TQSystematicsHandler.h", 10,
                  typeid(::TQSystematicsHandler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQSystematicsHandler::Dictionary, isa_proxy, 4,
                  sizeof(::TQSystematicsHandler) );
      instance.SetDelete(&delete_TQSystematicsHandler);
      instance.SetDeleteArray(&deleteArray_TQSystematicsHandler);
      instance.SetDestructor(&destruct_TQSystematicsHandler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQSystematicsHandler*)
   {
      return GenerateInitInstanceLocal((::TQSystematicsHandler*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQSystematicsHandler*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQTHnBaseMakerAnalysisJob(void *p = 0);
   static void *newArray_TQTHnBaseMakerAnalysisJob(Long_t size, void *p);
   static void delete_TQTHnBaseMakerAnalysisJob(void *p);
   static void deleteArray_TQTHnBaseMakerAnalysisJob(void *p);
   static void destruct_TQTHnBaseMakerAnalysisJob(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQTHnBaseMakerAnalysisJob*)
   {
      ::TQTHnBaseMakerAnalysisJob *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQTHnBaseMakerAnalysisJob >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQTHnBaseMakerAnalysisJob", ::TQTHnBaseMakerAnalysisJob::Class_Version(), "QFramework/TQTHnBaseMakerAnalysisJob.h", 12,
                  typeid(::TQTHnBaseMakerAnalysisJob), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQTHnBaseMakerAnalysisJob::Dictionary, isa_proxy, 4,
                  sizeof(::TQTHnBaseMakerAnalysisJob) );
      instance.SetNew(&new_TQTHnBaseMakerAnalysisJob);
      instance.SetNewArray(&newArray_TQTHnBaseMakerAnalysisJob);
      instance.SetDelete(&delete_TQTHnBaseMakerAnalysisJob);
      instance.SetDeleteArray(&deleteArray_TQTHnBaseMakerAnalysisJob);
      instance.SetDestructor(&destruct_TQTHnBaseMakerAnalysisJob);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQTHnBaseMakerAnalysisJob*)
   {
      return GenerateInitInstanceLocal((::TQTHnBaseMakerAnalysisJob*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQTHnBaseMakerAnalysisJob*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TQTikZPlotter(void *p);
   static void deleteArray_TQTikZPlotter(void *p);
   static void destruct_TQTikZPlotter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQTikZPlotter*)
   {
      ::TQTikZPlotter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQTikZPlotter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQTikZPlotter", ::TQTikZPlotter::Class_Version(), "QFramework/TQTikZPlotter.h", 9,
                  typeid(::TQTikZPlotter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQTikZPlotter::Dictionary, isa_proxy, 4,
                  sizeof(::TQTikZPlotter) );
      instance.SetDelete(&delete_TQTikZPlotter);
      instance.SetDeleteArray(&deleteArray_TQTikZPlotter);
      instance.SetDestructor(&destruct_TQTikZPlotter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQTikZPlotter*)
   {
      return GenerateInitInstanceLocal((::TQTikZPlotter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQTikZPlotter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQTreeFormulaObservable(void *p = 0);
   static void *newArray_TQTreeFormulaObservable(Long_t size, void *p);
   static void delete_TQTreeFormulaObservable(void *p);
   static void deleteArray_TQTreeFormulaObservable(void *p);
   static void destruct_TQTreeFormulaObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQTreeFormulaObservable*)
   {
      ::TQTreeFormulaObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQTreeFormulaObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQTreeFormulaObservable", ::TQTreeFormulaObservable::Class_Version(), "QFramework/TQTreeFormulaObservable.h", 8,
                  typeid(::TQTreeFormulaObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQTreeFormulaObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQTreeFormulaObservable) );
      instance.SetNew(&new_TQTreeFormulaObservable);
      instance.SetNewArray(&newArray_TQTreeFormulaObservable);
      instance.SetDelete(&delete_TQTreeFormulaObservable);
      instance.SetDeleteArray(&deleteArray_TQTreeFormulaObservable);
      instance.SetDestructor(&destruct_TQTreeFormulaObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQTreeFormulaObservable*)
   {
      return GenerateInitInstanceLocal((::TQTreeFormulaObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQTreeFormulaObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQUniqueCut(void *p = 0);
   static void *newArray_TQUniqueCut(Long_t size, void *p);
   static void delete_TQUniqueCut(void *p);
   static void deleteArray_TQUniqueCut(void *p);
   static void destruct_TQUniqueCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQUniqueCut*)
   {
      ::TQUniqueCut *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQUniqueCut >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQUniqueCut", ::TQUniqueCut::Class_Version(), "QFramework/TQUniqueCut.h", 8,
                  typeid(::TQUniqueCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQUniqueCut::Dictionary, isa_proxy, 4,
                  sizeof(::TQUniqueCut) );
      instance.SetNew(&new_TQUniqueCut);
      instance.SetNewArray(&newArray_TQUniqueCut);
      instance.SetDelete(&delete_TQUniqueCut);
      instance.SetDeleteArray(&deleteArray_TQUniqueCut);
      instance.SetDestructor(&destruct_TQUniqueCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQUniqueCut*)
   {
      return GenerateInitInstanceLocal((::TQUniqueCut*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQUniqueCut*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQVectorAuxObservable(void *p = 0);
   static void *newArray_TQVectorAuxObservable(Long_t size, void *p);
   static void delete_TQVectorAuxObservable(void *p);
   static void deleteArray_TQVectorAuxObservable(void *p);
   static void destruct_TQVectorAuxObservable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQVectorAuxObservable*)
   {
      ::TQVectorAuxObservable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQVectorAuxObservable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQVectorAuxObservable", ::TQVectorAuxObservable::Class_Version(), "QFramework/TQVectorAuxObservable.h", 6,
                  typeid(::TQVectorAuxObservable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQVectorAuxObservable::Dictionary, isa_proxy, 4,
                  sizeof(::TQVectorAuxObservable) );
      instance.SetNew(&new_TQVectorAuxObservable);
      instance.SetNewArray(&newArray_TQVectorAuxObservable);
      instance.SetDelete(&delete_TQVectorAuxObservable);
      instance.SetDeleteArray(&deleteArray_TQVectorAuxObservable);
      instance.SetDestructor(&destruct_TQVectorAuxObservable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQVectorAuxObservable*)
   {
      return GenerateInitInstanceLocal((::TQVectorAuxObservable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQVectorAuxObservable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TQXSecParser(void *p = 0);
   static void *newArray_TQXSecParser(Long_t size, void *p);
   static void delete_TQXSecParser(void *p);
   static void deleteArray_TQXSecParser(void *p);
   static void destruct_TQXSecParser(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TQXSecParser*)
   {
      ::TQXSecParser *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TQXSecParser >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TQXSecParser", ::TQXSecParser::Class_Version(), "QFramework/TQXSecParser.h", 13,
                  typeid(::TQXSecParser), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TQXSecParser::Dictionary, isa_proxy, 4,
                  sizeof(::TQXSecParser) );
      instance.SetNew(&new_TQXSecParser);
      instance.SetNewArray(&newArray_TQXSecParser);
      instance.SetDelete(&delete_TQXSecParser);
      instance.SetDeleteArray(&deleteArray_TQXSecParser);
      instance.SetDestructor(&destruct_TQXSecParser);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TQXSecParser*)
   {
      return GenerateInitInstanceLocal((::TQXSecParser*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TQXSecParser*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TQValue::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQValue::Class_Name()
{
   return "TQValue";
}

//______________________________________________________________________________
const char *TQValue::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQValue*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQValue::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQValue*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQValue::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQValue*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQValue::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQValue*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQValueDouble::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQValueDouble::Class_Name()
{
   return "TQValueDouble";
}

//______________________________________________________________________________
const char *TQValueDouble::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQValueDouble*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQValueDouble::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQValueDouble*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQValueDouble::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQValueDouble*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQValueDouble::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQValueDouble*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQValueInteger::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQValueInteger::Class_Name()
{
   return "TQValueInteger";
}

//______________________________________________________________________________
const char *TQValueInteger::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQValueInteger*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQValueInteger::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQValueInteger*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQValueInteger::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQValueInteger*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQValueInteger::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQValueInteger*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQValueBool::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQValueBool::Class_Name()
{
   return "TQValueBool";
}

//______________________________________________________________________________
const char *TQValueBool::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQValueBool*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQValueBool::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQValueBool*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQValueBool::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQValueBool*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQValueBool::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQValueBool*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQValueString::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQValueString::Class_Name()
{
   return "TQValueString";
}

//______________________________________________________________________________
const char *TQValueString::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQValueString*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQValueString::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQValueString*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQValueString::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQValueString*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQValueString::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQValueString*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQTaggable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQTaggable::Class_Name()
{
   return "TQTaggable";
}

//______________________________________________________________________________
const char *TQTaggable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTaggable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQTaggable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTaggable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQTaggable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTaggable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQTaggable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTaggable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQNamedTaggable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQNamedTaggable::Class_Name()
{
   return "TQNamedTaggable";
}

//______________________________________________________________________________
const char *TQNamedTaggable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNamedTaggable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQNamedTaggable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNamedTaggable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQNamedTaggable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNamedTaggable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQNamedTaggable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNamedTaggable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQNFBase::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQNFBase::Class_Name()
{
   return "TQNFBase";
}

//______________________________________________________________________________
const char *TQNFBase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFBase*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQNFBase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFBase*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQNFBase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFBase*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQNFBase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFBase*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQCounter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQCounter::Class_Name()
{
   return "TQCounter";
}

//______________________________________________________________________________
const char *TQCounter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCounter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQCounter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCounter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQCounter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCounter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQCounter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCounter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQABCDCalculator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQABCDCalculator::Class_Name()
{
   return "TQABCDCalculator";
}

//______________________________________________________________________________
const char *TQABCDCalculator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQABCDCalculator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQABCDCalculator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQABCDCalculator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQABCDCalculator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQABCDCalculator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQABCDCalculator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQABCDCalculator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQAlgorithm::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQAlgorithm::Class_Name()
{
   return "TQAlgorithm";
}

//______________________________________________________________________________
const char *TQAlgorithm::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQAlgorithm*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQAlgorithm::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQAlgorithm*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQAlgorithm::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQAlgorithm*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQAlgorithm::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQAlgorithm*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TObject>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TObject>::Class_Name()
{
   return "TQIteratorT<TObject>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TObject>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObject>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TObject>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObject>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TObject>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObject>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TObject>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObject>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TQValue>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TQValue>::Class_Name()
{
   return "TQIteratorT<TQValue>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TQValue>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQValue>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TQValue>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQValue>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQValue>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQValue>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQValue>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQValue>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TQCounter>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TQCounter>::Class_Name()
{
   return "TQIteratorT<TQCounter>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TQCounter>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCounter>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TQCounter>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCounter>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQCounter>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCounter>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQCounter>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCounter>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TQCut>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TQCut>::Class_Name()
{
   return "TQIteratorT<TQCut>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TQCut>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCut>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TQCut>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCut>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQCut>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCut>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQCut>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCut>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TObjString>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TObjString>::Class_Name()
{
   return "TQIteratorT<TObjString>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TObjString>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObjString>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TObjString>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObjString>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TObjString>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObjString>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TObjString>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObjString>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TQFolder>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TQFolder>::Class_Name()
{
   return "TQIteratorT<TQFolder>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TQFolder>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQFolder>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TQFolder>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQFolder>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQFolder>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQFolder>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQFolder>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQFolder>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TQSampleFolder>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TQSampleFolder>::Class_Name()
{
   return "TQIteratorT<TQSampleFolder>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TQSampleFolder>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQSampleFolder>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TQSampleFolder>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQSampleFolder>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQSampleFolder>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQSampleFolder>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQSampleFolder>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQSampleFolder>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TQSample>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TQSample>::Class_Name()
{
   return "TQIteratorT<TQSample>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TQSample>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQSample>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TQSample>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQSample>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQSample>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQSample>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQSample>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQSample>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TH1>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TH1>::Class_Name()
{
   return "TQIteratorT<TH1>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TH1>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TH1>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TH1>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TH1>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TH1>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TH1>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TH1>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TH1>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TQNamedTaggable>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TQNamedTaggable>::Class_Name()
{
   return "TQIteratorT<TQNamedTaggable>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TQNamedTaggable>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQNamedTaggable>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TQNamedTaggable>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQNamedTaggable>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQNamedTaggable>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQNamedTaggable>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQNamedTaggable>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQNamedTaggable>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TQObservable>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TQObservable>::Class_Name()
{
   return "TQIteratorT<TQObservable>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TQObservable>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQObservable>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TQObservable>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQObservable>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQObservable>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQObservable>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQObservable>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQObservable>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TQAnalysisJob>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TQAnalysisJob>::Class_Name()
{
   return "TQIteratorT<TQAnalysisJob>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TQAnalysisJob>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQAnalysisJob>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TQAnalysisJob>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQAnalysisJob>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQAnalysisJob>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQAnalysisJob>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQAnalysisJob>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQAnalysisJob>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TGraphErrors>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TGraphErrors>::Class_Name()
{
   return "TQIteratorT<TGraphErrors>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TGraphErrors>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraphErrors>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TGraphErrors>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraphErrors>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TGraphErrors>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraphErrors>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TGraphErrors>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraphErrors>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TGraph>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TGraph>::Class_Name()
{
   return "TQIteratorT<TGraph>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TGraph>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraph>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TGraph>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraph>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TGraph>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraph>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TGraph>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraph>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TGraphAsymmErrors>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TGraphAsymmErrors>::Class_Name()
{
   return "TQIteratorT<TGraphAsymmErrors>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TGraphAsymmErrors>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraphAsymmErrors>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TGraphAsymmErrors>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraphAsymmErrors>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TGraphAsymmErrors>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraphAsymmErrors>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TGraphAsymmErrors>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TGraphAsymmErrors>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TCollection>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TCollection>::Class_Name()
{
   return "TQIteratorT<TCollection>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TCollection>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TCollection>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TCollection>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TCollection>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TCollection>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TCollection>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TCollection>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TCollection>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TList>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TList>::Class_Name()
{
   return "TQIteratorT<TList>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TList>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TList>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TList>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TList>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TList>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TList>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TList>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TList>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TLegendEntry>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TLegendEntry>::Class_Name()
{
   return "TQIteratorT<TLegendEntry>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TLegendEntry>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TLegendEntry>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TLegendEntry>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TLegendEntry>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TLegendEntry>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TLegendEntry>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TLegendEntry>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TLegendEntry>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TBranch>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TBranch>::Class_Name()
{
   return "TQIteratorT<TBranch>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TBranch>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TBranch>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TBranch>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TBranch>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TBranch>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TBranch>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TBranch>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TBranch>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TKey>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TKey>::Class_Name()
{
   return "TQIteratorT<TKey>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TKey>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TKey>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TKey>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TKey>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TKey>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TKey>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TKey>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TKey>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TQCounterGrid>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TQCounterGrid>::Class_Name()
{
   return "TQIteratorT<TQCounterGrid>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TQCounterGrid>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCounterGrid>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TQCounterGrid>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCounterGrid>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQCounterGrid>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCounterGrid>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQCounterGrid>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQCounterGrid>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TQPCA>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TQPCA>::Class_Name()
{
   return "TQIteratorT<TQPCA>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TQPCA>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQPCA>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TQPCA>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQPCA>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQPCA>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQPCA>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TQPCA>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TQPCA>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TH2>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TH2>::Class_Name()
{
   return "TQIteratorT<TH2>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TH2>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TH2>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TH2>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TH2>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TH2>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TH2>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TH2>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TH2>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQIteratorT<TObjArray>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQIteratorT<TObjArray>::Class_Name()
{
   return "TQIteratorT<TObjArray>";
}

//______________________________________________________________________________
template <> const char *TQIteratorT<TObjArray>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObjArray>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQIteratorT<TObjArray>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObjArray>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TObjArray>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObjArray>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQIteratorT<TObjArray>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQIteratorT<TObjArray>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQPCA::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQPCA::Class_Name()
{
   return "TQPCA";
}

//______________________________________________________________________________
const char *TQPCA::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQPCA*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQPCA::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQPCA*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQPCA::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQPCA*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQPCA::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQPCA*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQFolder::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQFolder::Class_Name()
{
   return "TQFolder";
}

//______________________________________________________________________________
const char *TQFolder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQFolder*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQFolder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQFolder*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQFolder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQFolder*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQFolder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQFolder*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSampleFolder::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSampleFolder::Class_Name()
{
   return "TQSampleFolder";
}

//______________________________________________________________________________
const char *TQSampleFolder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleFolder*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSampleFolder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleFolder*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSampleFolder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleFolder*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSampleFolder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleFolder*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSample::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSample::Class_Name()
{
   return "TQSample";
}

//______________________________________________________________________________
const char *TQSample::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSample*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSample::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSample*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSample::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSample*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSample::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSample*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQObservable::Class_Name()
{
   return "TQObservable";
}

//______________________________________________________________________________
const char *TQObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQAnalysisJob::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQAnalysisJob::Class_Name()
{
   return "TQAnalysisJob";
}

//______________________________________________________________________________
const char *TQAnalysisJob::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisJob*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQAnalysisJob::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisJob*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQAnalysisJob::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisJob*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQAnalysisJob::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisJob*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQCut::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQCut::Class_Name()
{
   return "TQCut";
}

//______________________________________________________________________________
const char *TQCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCut*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCut*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCut*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCut*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQAnalysisAlgorithm::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQAnalysisAlgorithm::Class_Name()
{
   return "TQAnalysisAlgorithm";
}

//______________________________________________________________________________
const char *TQAnalysisAlgorithm::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisAlgorithm*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQAnalysisAlgorithm::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisAlgorithm*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQAnalysisAlgorithm::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisAlgorithm*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQAnalysisAlgorithm::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisAlgorithm*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSampleVisitor::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSampleVisitor::Class_Name()
{
   return "TQSampleVisitor";
}

//______________________________________________________________________________
const char *TQSampleVisitor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleVisitor*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSampleVisitor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleVisitor*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSampleVisitor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleVisitor*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSampleVisitor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleVisitor*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQAnalysisSampleVisitorBase::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQAnalysisSampleVisitorBase::Class_Name()
{
   return "TQAnalysisSampleVisitorBase";
}

//______________________________________________________________________________
const char *TQAnalysisSampleVisitorBase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisSampleVisitorBase*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQAnalysisSampleVisitorBase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisSampleVisitorBase*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQAnalysisSampleVisitorBase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisSampleVisitorBase*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQAnalysisSampleVisitorBase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisSampleVisitorBase*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQAnalysisSampleVisitor::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQAnalysisSampleVisitor::Class_Name()
{
   return "TQAnalysisSampleVisitor";
}

//______________________________________________________________________________
const char *TQAnalysisSampleVisitor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisSampleVisitor*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQAnalysisSampleVisitor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisSampleVisitor*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQAnalysisSampleVisitor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisSampleVisitor*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQAnalysisSampleVisitor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQAnalysisSampleVisitor*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQTable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQTable::Class_Name()
{
   return "TQTable";
}

//______________________________________________________________________________
const char *TQTable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQTable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQTable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQTable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSampleDataReader::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSampleDataReader::Class_Name()
{
   return "TQSampleDataReader";
}

//______________________________________________________________________________
const char *TQSampleDataReader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleDataReader*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSampleDataReader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleDataReader*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSampleDataReader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleDataReader*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSampleDataReader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleDataReader*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQPresenter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQPresenter::Class_Name()
{
   return "TQPresenter";
}

//______________________________________________________________________________
const char *TQPresenter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQPresenter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQPresenter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQPresenter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQPresenter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQPresenter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQPresenter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQPresenter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQPlotter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQPlotter::Class_Name()
{
   return "TQPlotter";
}

//______________________________________________________________________________
const char *TQPlotter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQPlotter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQPlotter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQPlotter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQPlotter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQPlotter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQPlotter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQPlotter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQROOTPlotter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQROOTPlotter::Class_Name()
{
   return "TQROOTPlotter";
}

//______________________________________________________________________________
const char *TQROOTPlotter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQROOTPlotter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQROOTPlotter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQROOTPlotter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQROOTPlotter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQROOTPlotter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQROOTPlotter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQROOTPlotter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQCompPlotter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQCompPlotter::Class_Name()
{
   return "TQCompPlotter";
}

//______________________________________________________________________________
const char *TQCompPlotter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCompPlotter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQCompPlotter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCompPlotter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQCompPlotter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCompPlotter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQCompPlotter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCompPlotter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQConstObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQConstObservable::Class_Name()
{
   return "TQConstObservable";
}

//______________________________________________________________________________
const char *TQConstObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQConstObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQConstObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQConstObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQConstObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQConstObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQConstObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQConstObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQCutFactory::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQCutFactory::Class_Name()
{
   return "TQCutFactory";
}

//______________________________________________________________________________
const char *TQCutFactory::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCutFactory*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQCutFactory::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCutFactory*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQCutFactory::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCutFactory*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQCutFactory::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCutFactory*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQCutflowAnalysisJob::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQCutflowAnalysisJob::Class_Name()
{
   return "TQCutflowAnalysisJob";
}

//______________________________________________________________________________
const char *TQCutflowAnalysisJob::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCutflowAnalysisJob*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQCutflowAnalysisJob::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCutflowAnalysisJob*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQCutflowAnalysisJob::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCutflowAnalysisJob*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQCutflowAnalysisJob::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCutflowAnalysisJob*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQCutflowPrinter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQCutflowPrinter::Class_Name()
{
   return "TQCutflowPrinter";
}

//______________________________________________________________________________
const char *TQCutflowPrinter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCutflowPrinter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQCutflowPrinter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCutflowPrinter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQCutflowPrinter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCutflowPrinter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQCutflowPrinter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCutflowPrinter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQCutflowPlotter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQCutflowPlotter::Class_Name()
{
   return "TQCutflowPlotter";
}

//______________________________________________________________________________
const char *TQCutflowPlotter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCutflowPlotter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQCutflowPlotter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQCutflowPlotter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQCutflowPlotter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCutflowPlotter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQCutflowPlotter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQCutflowPlotter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQDefaultPlotter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQDefaultPlotter::Class_Name()
{
   return "TQDefaultPlotter";
}

//______________________________________________________________________________
const char *TQDefaultPlotter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQDefaultPlotter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQDefaultPlotter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQDefaultPlotter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQDefaultPlotter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQDefaultPlotter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQDefaultPlotter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQDefaultPlotter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQTreeObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQTreeObservable::Class_Name()
{
   return "TQTreeObservable";
}

//______________________________________________________________________________
const char *TQTreeObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTreeObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQTreeObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTreeObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQTreeObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTreeObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQTreeObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTreeObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQEventIndexObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQEventIndexObservable::Class_Name()
{
   return "TQEventIndexObservable";
}

//______________________________________________________________________________
const char *TQEventIndexObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQEventIndexObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQEventIndexObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQEventIndexObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQEventIndexObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQEventIndexObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQEventIndexObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQEventIndexObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQEventlistAnalysisJob::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQEventlistAnalysisJob::Class_Name()
{
   return "TQEventlistAnalysisJob";
}

//______________________________________________________________________________
const char *TQEventlistAnalysisJob::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQEventlistAnalysisJob*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQEventlistAnalysisJob::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQEventlistAnalysisJob*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQEventlistAnalysisJob::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQEventlistAnalysisJob*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQEventlistAnalysisJob::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQEventlistAnalysisJob*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQEventlistPrinter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQEventlistPrinter::Class_Name()
{
   return "TQEventlistPrinter";
}

//______________________________________________________________________________
const char *TQEventlistPrinter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQEventlistPrinter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQEventlistPrinter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQEventlistPrinter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQEventlistPrinter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQEventlistPrinter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQEventlistPrinter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQEventlistPrinter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQFilterObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQFilterObservable::Class_Name()
{
   return "TQFilterObservable";
}

//______________________________________________________________________________
const char *TQFilterObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQFilterObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQFilterObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQFilterObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQFilterObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQFilterObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQFilterObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQFilterObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQGraphMakerAnalysisJob::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQGraphMakerAnalysisJob::Class_Name()
{
   return "TQGraphMakerAnalysisJob";
}

//______________________________________________________________________________
const char *TQGraphMakerAnalysisJob::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGraphMakerAnalysisJob*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQGraphMakerAnalysisJob::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGraphMakerAnalysisJob*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQGraphMakerAnalysisJob::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGraphMakerAnalysisJob*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQGraphMakerAnalysisJob::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGraphMakerAnalysisJob*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQGridScanBound::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQGridScanBound::Class_Name()
{
   return "TQGridScanBound";
}

//______________________________________________________________________________
const char *TQGridScanBound::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanBound*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQGridScanBound::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanBound*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQGridScanBound::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanBound*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQGridScanBound::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanBound*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQGridScanBounds::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQGridScanBounds::Class_Name()
{
   return "TQGridScanBounds";
}

//______________________________________________________________________________
const char *TQGridScanBounds::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanBounds*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQGridScanBounds::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanBounds*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQGridScanBounds::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanBounds*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQGridScanBounds::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanBounds*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQGridScanObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQGridScanObservable::Class_Name()
{
   return "TQGridScanObservable";
}

//______________________________________________________________________________
const char *TQGridScanObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQGridScanObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQGridScanObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQGridScanObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQGridScanNormalObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQGridScanNormalObservable::Class_Name()
{
   return "TQGridScanNormalObservable";
}

//______________________________________________________________________________
const char *TQGridScanNormalObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanNormalObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQGridScanNormalObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanNormalObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQGridScanNormalObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanNormalObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQGridScanNormalObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanNormalObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQGridScanSplitObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQGridScanSplitObservable::Class_Name()
{
   return "TQGridScanSplitObservable";
}

//______________________________________________________________________________
const char *TQGridScanSplitObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanSplitObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQGridScanSplitObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanSplitObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQGridScanSplitObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanSplitObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQGridScanSplitObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanSplitObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQGridScanPoint::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQGridScanPoint::Class_Name()
{
   return "TQGridScanPoint";
}

//______________________________________________________________________________
const char *TQGridScanPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanPoint*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQGridScanPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanPoint*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQGridScanPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanPoint*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQGridScanPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanPoint*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQGridScanResults::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQGridScanResults::Class_Name()
{
   return "TQGridScanResults";
}

//______________________________________________________________________________
const char *TQGridScanResults::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanResults*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQGridScanResults::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanResults*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQGridScanResults::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanResults*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQGridScanResults::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanResults*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSignificanceEvaluator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSignificanceEvaluator::Class_Name()
{
   return "TQSignificanceEvaluator";
}

//______________________________________________________________________________
const char *TQSignificanceEvaluator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSignificanceEvaluator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSignificanceEvaluator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSignificanceEvaluator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSignificanceEvaluator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSignificanceEvaluator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSignificanceEvaluator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSignificanceEvaluator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSignificanceEvaluatorBase::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSignificanceEvaluatorBase::Class_Name()
{
   return "TQSignificanceEvaluatorBase";
}

//______________________________________________________________________________
const char *TQSignificanceEvaluatorBase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSignificanceEvaluatorBase*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSignificanceEvaluatorBase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSignificanceEvaluatorBase*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSignificanceEvaluatorBase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSignificanceEvaluatorBase*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSignificanceEvaluatorBase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSignificanceEvaluatorBase*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSimpleSignificanceEvaluator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSimpleSignificanceEvaluator::Class_Name()
{
   return "TQSimpleSignificanceEvaluator";
}

//______________________________________________________________________________
const char *TQSimpleSignificanceEvaluator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSimpleSignificanceEvaluator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSimpleSignificanceEvaluator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSimpleSignificanceEvaluator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSimpleSignificanceEvaluator2::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSimpleSignificanceEvaluator2::Class_Name()
{
   return "TQSimpleSignificanceEvaluator2";
}

//______________________________________________________________________________
const char *TQSimpleSignificanceEvaluator2::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator2*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSimpleSignificanceEvaluator2::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator2*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSimpleSignificanceEvaluator2::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator2*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSimpleSignificanceEvaluator2::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator2*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSimpleSignificanceEvaluator3::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSimpleSignificanceEvaluator3::Class_Name()
{
   return "TQSimpleSignificanceEvaluator3";
}

//______________________________________________________________________________
const char *TQSimpleSignificanceEvaluator3::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator3*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSimpleSignificanceEvaluator3::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator3*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSimpleSignificanceEvaluator3::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator3*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSimpleSignificanceEvaluator3::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSimpleSignificanceEvaluator3*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQPoissonSignificanceEvaluator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQPoissonSignificanceEvaluator::Class_Name()
{
   return "TQPoissonSignificanceEvaluator";
}

//______________________________________________________________________________
const char *TQPoissonSignificanceEvaluator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQPoissonSignificanceEvaluator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQPoissonSignificanceEvaluator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQPoissonSignificanceEvaluator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQPoissonSignificanceEvaluator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQPoissonSignificanceEvaluator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQPoissonSignificanceEvaluator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQPoissonSignificanceEvaluator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQGridScanner::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQGridScanner::Class_Name()
{
   return "TQGridScanner";
}

//______________________________________________________________________________
const char *TQGridScanner::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanner*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQGridScanner::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanner*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQGridScanner::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanner*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQGridScanner::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQGridScanner*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQHWWPlotter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQHWWPlotter::Class_Name()
{
   return "TQHWWPlotter";
}

//______________________________________________________________________________
const char *TQHWWPlotter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQHWWPlotter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQHWWPlotter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQHWWPlotter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQHWWPlotter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQHWWPlotter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQHWWPlotter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQHWWPlotter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQHistComparer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQHistComparer::Class_Name()
{
   return "TQHistComparer";
}

//______________________________________________________________________________
const char *TQHistComparer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQHistComparer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQHistComparer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQHistComparer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQHistComparer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQHistComparer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQHistComparer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQHistComparer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQHistoMakerAnalysisJob::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQHistoMakerAnalysisJob::Class_Name()
{
   return "TQHistoMakerAnalysisJob";
}

//______________________________________________________________________________
const char *TQHistoMakerAnalysisJob::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQHistoMakerAnalysisJob*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQHistoMakerAnalysisJob::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQHistoMakerAnalysisJob*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQHistoMakerAnalysisJob::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQHistoMakerAnalysisJob*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQHistoMakerAnalysisJob::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQHistoMakerAnalysisJob*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQHistogramObservable<TH1>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQHistogramObservable<TH1>::Class_Name()
{
   return "TQHistogramObservable<TH1>";
}

//______________________________________________________________________________
template <> const char *TQHistogramObservable<TH1>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH1>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQHistogramObservable<TH1>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH1>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQHistogramObservable<TH1>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH1>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQHistogramObservable<TH1>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH1>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQHistogramObservable<TH2>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQHistogramObservable<TH2>::Class_Name()
{
   return "TQHistogramObservable<TH2>";
}

//______________________________________________________________________________
template <> const char *TQHistogramObservable<TH2>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH2>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQHistogramObservable<TH2>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH2>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQHistogramObservable<TH2>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH2>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQHistogramObservable<TH2>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH2>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TQHistogramObservable<TH3>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TQHistogramObservable<TH3>::Class_Name()
{
   return "TQHistogramObservable<TH3>";
}

//______________________________________________________________________________
template <> const char *TQHistogramObservable<TH3>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH3>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TQHistogramObservable<TH3>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH3>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TQHistogramObservable<TH3>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH3>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TQHistogramObservable<TH3>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQHistogramObservable<TH3>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQLink::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQLink::Class_Name()
{
   return "TQLink";
}

//______________________________________________________________________________
const char *TQLink::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQLink*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQLink::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQLink*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQLink::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQLink*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQLink::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQLink*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQImportLink::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQImportLink::Class_Name()
{
   return "TQImportLink";
}

//______________________________________________________________________________
const char *TQImportLink::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQImportLink*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQImportLink::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQImportLink*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQImportLink::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQImportLink*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQImportLink::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQImportLink*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQLibrary::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQLibrary::Class_Name()
{
   return "TQLibrary";
}

//______________________________________________________________________________
const char *TQLibrary::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQLibrary*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQLibrary::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQLibrary*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQLibrary::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQLibrary*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQLibrary::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQLibrary*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQToken::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQToken::Class_Name()
{
   return "TQToken";
}

//______________________________________________________________________________
const char *TQToken::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQToken*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQToken::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQToken*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQToken::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQToken*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQToken::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQToken*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQMVA::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQMVA::Class_Name()
{
   return "TQMVA";
}

//______________________________________________________________________________
const char *TQMVA::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQMVA*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQMVA::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQMVA*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQMVA::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQMVA*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQMVA::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQMVA*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQMVAObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQMVAObservable::Class_Name()
{
   return "TQMVAObservable";
}

//______________________________________________________________________________
const char *TQMVAObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQMVAObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQMVAObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQMVAObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQMVAObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQMVAObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQMVAObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQMVAObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQMultiChannelAnalysisSampleVisitor::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQMultiChannelAnalysisSampleVisitor::Class_Name()
{
   return "TQMultiChannelAnalysisSampleVisitor";
}

//______________________________________________________________________________
const char *TQMultiChannelAnalysisSampleVisitor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQMultiChannelAnalysisSampleVisitor*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQMultiChannelAnalysisSampleVisitor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQMultiChannelAnalysisSampleVisitor*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQMultiChannelAnalysisSampleVisitor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQMultiChannelAnalysisSampleVisitor*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQMultiChannelAnalysisSampleVisitor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQMultiChannelAnalysisSampleVisitor*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQMultiObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQMultiObservable::Class_Name()
{
   return "TQMultiObservable";
}

//______________________________________________________________________________
const char *TQMultiObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQMultiObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQMultiObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQMultiObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQMultiObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQMultiObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQMultiObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQMultiObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQNFCalculator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQNFCalculator::Class_Name()
{
   return "TQNFCalculator";
}

//______________________________________________________________________________
const char *TQNFCalculator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFCalculator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQNFCalculator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFCalculator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQNFCalculator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFCalculator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQNFCalculator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFCalculator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQNFChainloader::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQNFChainloader::Class_Name()
{
   return "TQNFChainloader";
}

//______________________________________________________________________________
const char *TQNFChainloader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFChainloader*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQNFChainloader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFChainloader*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQNFChainloader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFChainloader*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQNFChainloader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFChainloader*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQNFCustomCalculator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQNFCustomCalculator::Class_Name()
{
   return "TQNFCustomCalculator";
}

//______________________________________________________________________________
const char *TQNFCustomCalculator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFCustomCalculator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQNFCustomCalculator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFCustomCalculator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQNFCustomCalculator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFCustomCalculator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQNFCustomCalculator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFCustomCalculator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQNFManualSetter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQNFManualSetter::Class_Name()
{
   return "TQNFManualSetter";
}

//______________________________________________________________________________
const char *TQNFManualSetter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFManualSetter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQNFManualSetter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFManualSetter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQNFManualSetter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFManualSetter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQNFManualSetter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFManualSetter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQNFTop0jetEstimator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQNFTop0jetEstimator::Class_Name()
{
   return "TQNFTop0jetEstimator";
}

//______________________________________________________________________________
const char *TQNFTop0jetEstimator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFTop0jetEstimator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQNFTop0jetEstimator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFTop0jetEstimator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQNFTop0jetEstimator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFTop0jetEstimator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQNFTop0jetEstimator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFTop0jetEstimator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQNFTop1jetEstimator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQNFTop1jetEstimator::Class_Name()
{
   return "TQNFTop1jetEstimator";
}

//______________________________________________________________________________
const char *TQNFTop1jetEstimator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFTop1jetEstimator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQNFTop1jetEstimator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFTop1jetEstimator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQNFTop1jetEstimator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFTop1jetEstimator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQNFTop1jetEstimator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFTop1jetEstimator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQNFUncertaintyScaler::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQNFUncertaintyScaler::Class_Name()
{
   return "TQNFUncertaintyScaler";
}

//______________________________________________________________________________
const char *TQNFUncertaintyScaler::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFUncertaintyScaler*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQNFUncertaintyScaler::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNFUncertaintyScaler*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQNFUncertaintyScaler::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFUncertaintyScaler*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQNFUncertaintyScaler::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNFUncertaintyScaler*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQNTupleDumperAnalysisJob::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQNTupleDumperAnalysisJob::Class_Name()
{
   return "TQNTupleDumperAnalysisJob";
}

//______________________________________________________________________________
const char *TQNTupleDumperAnalysisJob::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNTupleDumperAnalysisJob*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQNTupleDumperAnalysisJob::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQNTupleDumperAnalysisJob*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQNTupleDumperAnalysisJob::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNTupleDumperAnalysisJob*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQNTupleDumperAnalysisJob::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQNTupleDumperAnalysisJob*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQPCAAnalysisJob::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQPCAAnalysisJob::Class_Name()
{
   return "TQPCAAnalysisJob";
}

//______________________________________________________________________________
const char *TQPCAAnalysisJob::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQPCAAnalysisJob*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQPCAAnalysisJob::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQPCAAnalysisJob*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQPCAAnalysisJob::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQPCAAnalysisJob*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQPCAAnalysisJob::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQPCAAnalysisJob*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQPathManager::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQPathManager::Class_Name()
{
   return "TQPathManager";
}

//______________________________________________________________________________
const char *TQPathManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQPathManager*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQPathManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQPathManager*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQPathManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQPathManager*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQPathManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQPathManager*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSampleGroupingVisitor::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSampleGroupingVisitor::Class_Name()
{
   return "TQSampleGroupingVisitor";
}

//______________________________________________________________________________
const char *TQSampleGroupingVisitor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleGroupingVisitor*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSampleGroupingVisitor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleGroupingVisitor*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSampleGroupingVisitor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleGroupingVisitor*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSampleGroupingVisitor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleGroupingVisitor*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSampleInitializerBase::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSampleInitializerBase::Class_Name()
{
   return "TQSampleInitializerBase";
}

//______________________________________________________________________________
const char *TQSampleInitializerBase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleInitializerBase*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSampleInitializerBase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleInitializerBase*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSampleInitializerBase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleInitializerBase*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSampleInitializerBase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleInitializerBase*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSampleInitializer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSampleInitializer::Class_Name()
{
   return "TQSampleInitializer";
}

//______________________________________________________________________________
const char *TQSampleInitializer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleInitializer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSampleInitializer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleInitializer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSampleInitializer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleInitializer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSampleInitializer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleInitializer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSampleNormalizationObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSampleNormalizationObservable::Class_Name()
{
   return "TQSampleNormalizationObservable";
}

//______________________________________________________________________________
const char *TQSampleNormalizationObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleNormalizationObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSampleNormalizationObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleNormalizationObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSampleNormalizationObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleNormalizationObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSampleNormalizationObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleNormalizationObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSamplePurger::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSamplePurger::Class_Name()
{
   return "TQSamplePurger";
}

//______________________________________________________________________________
const char *TQSamplePurger::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSamplePurger*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSamplePurger::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSamplePurger*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSamplePurger::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSamplePurger*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSamplePurger::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSamplePurger*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSampleRevisitor::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSampleRevisitor::Class_Name()
{
   return "TQSampleRevisitor";
}

//______________________________________________________________________________
const char *TQSampleRevisitor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleRevisitor*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSampleRevisitor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSampleRevisitor*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSampleRevisitor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleRevisitor*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSampleRevisitor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSampleRevisitor*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSystematicsManager::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSystematicsManager::Class_Name()
{
   return "TQSystematicsManager";
}

//______________________________________________________________________________
const char *TQSystematicsManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSystematicsManager*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSystematicsManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSystematicsManager*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSystematicsManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSystematicsManager*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSystematicsManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSystematicsManager*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQSystematicsHandler::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQSystematicsHandler::Class_Name()
{
   return "TQSystematicsHandler";
}

//______________________________________________________________________________
const char *TQSystematicsHandler::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSystematicsHandler*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQSystematicsHandler::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQSystematicsHandler*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQSystematicsHandler::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSystematicsHandler*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQSystematicsHandler::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQSystematicsHandler*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQTHnBaseMakerAnalysisJob::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQTHnBaseMakerAnalysisJob::Class_Name()
{
   return "TQTHnBaseMakerAnalysisJob";
}

//______________________________________________________________________________
const char *TQTHnBaseMakerAnalysisJob::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTHnBaseMakerAnalysisJob*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQTHnBaseMakerAnalysisJob::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTHnBaseMakerAnalysisJob*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQTHnBaseMakerAnalysisJob::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTHnBaseMakerAnalysisJob*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQTHnBaseMakerAnalysisJob::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTHnBaseMakerAnalysisJob*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQTikZPlotter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQTikZPlotter::Class_Name()
{
   return "TQTikZPlotter";
}

//______________________________________________________________________________
const char *TQTikZPlotter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTikZPlotter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQTikZPlotter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTikZPlotter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQTikZPlotter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTikZPlotter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQTikZPlotter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTikZPlotter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQTreeFormulaObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQTreeFormulaObservable::Class_Name()
{
   return "TQTreeFormulaObservable";
}

//______________________________________________________________________________
const char *TQTreeFormulaObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTreeFormulaObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQTreeFormulaObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQTreeFormulaObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQTreeFormulaObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTreeFormulaObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQTreeFormulaObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQTreeFormulaObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQUniqueCut::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQUniqueCut::Class_Name()
{
   return "TQUniqueCut";
}

//______________________________________________________________________________
const char *TQUniqueCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQUniqueCut*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQUniqueCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQUniqueCut*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQUniqueCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQUniqueCut*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQUniqueCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQUniqueCut*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQVectorAuxObservable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQVectorAuxObservable::Class_Name()
{
   return "TQVectorAuxObservable";
}

//______________________________________________________________________________
const char *TQVectorAuxObservable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQVectorAuxObservable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQVectorAuxObservable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQVectorAuxObservable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQVectorAuxObservable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQVectorAuxObservable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQVectorAuxObservable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQVectorAuxObservable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TQXSecParser::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TQXSecParser::Class_Name()
{
   return "TQXSecParser";
}

//______________________________________________________________________________
const char *TQXSecParser::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQXSecParser*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TQXSecParser::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TQXSecParser*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TQXSecParser::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQXSecParser*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TQXSecParser::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TQXSecParser*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TQValue::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQValue.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQValue::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQValue::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQValue(void *p) {
      delete ((::TQValue*)p);
   }
   static void deleteArray_TQValue(void *p) {
      delete [] ((::TQValue*)p);
   }
   static void destruct_TQValue(void *p) {
      typedef ::TQValue current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQValue

//______________________________________________________________________________
void TQValueDouble::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQValueDouble.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQValueDouble::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQValueDouble::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQValueDouble(void *p) {
      return  p ? new(p) ::TQValueDouble : new ::TQValueDouble;
   }
   static void *newArray_TQValueDouble(Long_t nElements, void *p) {
      return p ? new(p) ::TQValueDouble[nElements] : new ::TQValueDouble[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQValueDouble(void *p) {
      delete ((::TQValueDouble*)p);
   }
   static void deleteArray_TQValueDouble(void *p) {
      delete [] ((::TQValueDouble*)p);
   }
   static void destruct_TQValueDouble(void *p) {
      typedef ::TQValueDouble current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQValueDouble

//______________________________________________________________________________
void TQValueInteger::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQValueInteger.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQValueInteger::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQValueInteger::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQValueInteger(void *p) {
      return  p ? new(p) ::TQValueInteger : new ::TQValueInteger;
   }
   static void *newArray_TQValueInteger(Long_t nElements, void *p) {
      return p ? new(p) ::TQValueInteger[nElements] : new ::TQValueInteger[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQValueInteger(void *p) {
      delete ((::TQValueInteger*)p);
   }
   static void deleteArray_TQValueInteger(void *p) {
      delete [] ((::TQValueInteger*)p);
   }
   static void destruct_TQValueInteger(void *p) {
      typedef ::TQValueInteger current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQValueInteger

//______________________________________________________________________________
void TQValueBool::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQValueBool.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQValueBool::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQValueBool::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQValueBool(void *p) {
      return  p ? new(p) ::TQValueBool : new ::TQValueBool;
   }
   static void *newArray_TQValueBool(Long_t nElements, void *p) {
      return p ? new(p) ::TQValueBool[nElements] : new ::TQValueBool[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQValueBool(void *p) {
      delete ((::TQValueBool*)p);
   }
   static void deleteArray_TQValueBool(void *p) {
      delete [] ((::TQValueBool*)p);
   }
   static void destruct_TQValueBool(void *p) {
      typedef ::TQValueBool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQValueBool

//______________________________________________________________________________
void TQValueString::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQValueString.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQValueString::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQValueString::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQValueString(void *p) {
      return  p ? new(p) ::TQValueString : new ::TQValueString;
   }
   static void *newArray_TQValueString(Long_t nElements, void *p) {
      return p ? new(p) ::TQValueString[nElements] : new ::TQValueString[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQValueString(void *p) {
      delete ((::TQValueString*)p);
   }
   static void deleteArray_TQValueString(void *p) {
      delete [] ((::TQValueString*)p);
   }
   static void destruct_TQValueString(void *p) {
      typedef ::TQValueString current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQValueString

//______________________________________________________________________________
void TQTaggable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQTaggable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQTaggable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQTaggable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQTaggable(void *p) {
      return  p ? new(p) ::TQTaggable : new ::TQTaggable;
   }
   static void *newArray_TQTaggable(Long_t nElements, void *p) {
      return p ? new(p) ::TQTaggable[nElements] : new ::TQTaggable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQTaggable(void *p) {
      delete ((::TQTaggable*)p);
   }
   static void deleteArray_TQTaggable(void *p) {
      delete [] ((::TQTaggable*)p);
   }
   static void destruct_TQTaggable(void *p) {
      typedef ::TQTaggable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQTaggable

//______________________________________________________________________________
void TQNamedTaggable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQNamedTaggable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQNamedTaggable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQNamedTaggable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQNamedTaggable(void *p) {
      return  p ? new(p) ::TQNamedTaggable : new ::TQNamedTaggable;
   }
   static void *newArray_TQNamedTaggable(Long_t nElements, void *p) {
      return p ? new(p) ::TQNamedTaggable[nElements] : new ::TQNamedTaggable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQNamedTaggable(void *p) {
      delete ((::TQNamedTaggable*)p);
   }
   static void deleteArray_TQNamedTaggable(void *p) {
      delete [] ((::TQNamedTaggable*)p);
   }
   static void destruct_TQNamedTaggable(void *p) {
      typedef ::TQNamedTaggable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQNamedTaggable

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQMessageStream(void *p) {
      return  p ? new(p) ::TQMessageStream : new ::TQMessageStream;
   }
   static void *newArray_TQMessageStream(Long_t nElements, void *p) {
      return p ? new(p) ::TQMessageStream[nElements] : new ::TQMessageStream[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQMessageStream(void *p) {
      delete ((::TQMessageStream*)p);
   }
   static void deleteArray_TQMessageStream(void *p) {
      delete [] ((::TQMessageStream*)p);
   }
   static void destruct_TQMessageStream(void *p) {
      typedef ::TQMessageStream current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQMessageStream

//______________________________________________________________________________
void TQNFBase::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQNFBase.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQNFBase::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQNFBase::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQNFBase(void *p) {
      delete ((::TQNFBase*)p);
   }
   static void deleteArray_TQNFBase(void *p) {
      delete [] ((::TQNFBase*)p);
   }
   static void destruct_TQNFBase(void *p) {
      typedef ::TQNFBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQNFBase

//______________________________________________________________________________
void TQCounter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCounter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCounter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCounter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCounter(void *p) {
      return  p ? new(p) ::TQCounter : new ::TQCounter;
   }
   static void *newArray_TQCounter(Long_t nElements, void *p) {
      return p ? new(p) ::TQCounter[nElements] : new ::TQCounter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCounter(void *p) {
      delete ((::TQCounter*)p);
   }
   static void deleteArray_TQCounter(void *p) {
      delete [] ((::TQCounter*)p);
   }
   static void destruct_TQCounter(void *p) {
      typedef ::TQCounter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCounter

//______________________________________________________________________________
void TQABCDCalculator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQABCDCalculator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQABCDCalculator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQABCDCalculator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQABCDCalculator(void *p) {
      return  p ? new(p) ::TQABCDCalculator : new ::TQABCDCalculator;
   }
   static void *newArray_TQABCDCalculator(Long_t nElements, void *p) {
      return p ? new(p) ::TQABCDCalculator[nElements] : new ::TQABCDCalculator[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQABCDCalculator(void *p) {
      delete ((::TQABCDCalculator*)p);
   }
   static void deleteArray_TQABCDCalculator(void *p) {
      delete [] ((::TQABCDCalculator*)p);
   }
   static void destruct_TQABCDCalculator(void *p) {
      typedef ::TQABCDCalculator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQABCDCalculator

//______________________________________________________________________________
void TQAlgorithm::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQAlgorithm.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQAlgorithm::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQAlgorithm::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQAlgorithm(void *p) {
      delete ((::TQAlgorithm*)p);
   }
   static void deleteArray_TQAlgorithm(void *p) {
      delete [] ((::TQAlgorithm*)p);
   }
   static void destruct_TQAlgorithm(void *p) {
      typedef ::TQAlgorithm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQAlgorithm

//______________________________________________________________________________
template <> void TQIteratorT<TObject>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TObject>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TObject>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TObject>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETObjectgR(void *p) {
      return  p ? new(p) ::TQIteratorT<TObject> : new ::TQIteratorT<TObject>;
   }
   static void *newArray_TQIteratorTlETObjectgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TObject>[nElements] : new ::TQIteratorT<TObject>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETObjectgR(void *p) {
      delete ((::TQIteratorT<TObject>*)p);
   }
   static void deleteArray_TQIteratorTlETObjectgR(void *p) {
      delete [] ((::TQIteratorT<TObject>*)p);
   }
   static void destruct_TQIteratorTlETObjectgR(void *p) {
      typedef ::TQIteratorT<TObject> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TObject>

//______________________________________________________________________________
template <> void TQIteratorT<TQValue>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TQValue>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TQValue>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TQValue>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETQValuegR(void *p) {
      return  p ? new(p) ::TQIteratorT<TQValue> : new ::TQIteratorT<TQValue>;
   }
   static void *newArray_TQIteratorTlETQValuegR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TQValue>[nElements] : new ::TQIteratorT<TQValue>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETQValuegR(void *p) {
      delete ((::TQIteratorT<TQValue>*)p);
   }
   static void deleteArray_TQIteratorTlETQValuegR(void *p) {
      delete [] ((::TQIteratorT<TQValue>*)p);
   }
   static void destruct_TQIteratorTlETQValuegR(void *p) {
      typedef ::TQIteratorT<TQValue> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TQValue>

//______________________________________________________________________________
template <> void TQIteratorT<TQCounter>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TQCounter>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TQCounter>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TQCounter>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETQCountergR(void *p) {
      return  p ? new(p) ::TQIteratorT<TQCounter> : new ::TQIteratorT<TQCounter>;
   }
   static void *newArray_TQIteratorTlETQCountergR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TQCounter>[nElements] : new ::TQIteratorT<TQCounter>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETQCountergR(void *p) {
      delete ((::TQIteratorT<TQCounter>*)p);
   }
   static void deleteArray_TQIteratorTlETQCountergR(void *p) {
      delete [] ((::TQIteratorT<TQCounter>*)p);
   }
   static void destruct_TQIteratorTlETQCountergR(void *p) {
      typedef ::TQIteratorT<TQCounter> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TQCounter>

//______________________________________________________________________________
template <> void TQIteratorT<TQCut>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TQCut>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TQCut>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TQCut>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETQCutgR(void *p) {
      return  p ? new(p) ::TQIteratorT<TQCut> : new ::TQIteratorT<TQCut>;
   }
   static void *newArray_TQIteratorTlETQCutgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TQCut>[nElements] : new ::TQIteratorT<TQCut>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETQCutgR(void *p) {
      delete ((::TQIteratorT<TQCut>*)p);
   }
   static void deleteArray_TQIteratorTlETQCutgR(void *p) {
      delete [] ((::TQIteratorT<TQCut>*)p);
   }
   static void destruct_TQIteratorTlETQCutgR(void *p) {
      typedef ::TQIteratorT<TQCut> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TQCut>

//______________________________________________________________________________
template <> void TQIteratorT<TObjString>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TObjString>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TObjString>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TObjString>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETObjStringgR(void *p) {
      return  p ? new(p) ::TQIteratorT<TObjString> : new ::TQIteratorT<TObjString>;
   }
   static void *newArray_TQIteratorTlETObjStringgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TObjString>[nElements] : new ::TQIteratorT<TObjString>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETObjStringgR(void *p) {
      delete ((::TQIteratorT<TObjString>*)p);
   }
   static void deleteArray_TQIteratorTlETObjStringgR(void *p) {
      delete [] ((::TQIteratorT<TObjString>*)p);
   }
   static void destruct_TQIteratorTlETObjStringgR(void *p) {
      typedef ::TQIteratorT<TObjString> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TObjString>

//______________________________________________________________________________
template <> void TQIteratorT<TQFolder>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TQFolder>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TQFolder>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TQFolder>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETQFoldergR(void *p) {
      return  p ? new(p) ::TQIteratorT<TQFolder> : new ::TQIteratorT<TQFolder>;
   }
   static void *newArray_TQIteratorTlETQFoldergR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TQFolder>[nElements] : new ::TQIteratorT<TQFolder>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETQFoldergR(void *p) {
      delete ((::TQIteratorT<TQFolder>*)p);
   }
   static void deleteArray_TQIteratorTlETQFoldergR(void *p) {
      delete [] ((::TQIteratorT<TQFolder>*)p);
   }
   static void destruct_TQIteratorTlETQFoldergR(void *p) {
      typedef ::TQIteratorT<TQFolder> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TQFolder>

//______________________________________________________________________________
template <> void TQIteratorT<TQSampleFolder>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TQSampleFolder>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TQSampleFolder>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TQSampleFolder>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETQSampleFoldergR(void *p) {
      return  p ? new(p) ::TQIteratorT<TQSampleFolder> : new ::TQIteratorT<TQSampleFolder>;
   }
   static void *newArray_TQIteratorTlETQSampleFoldergR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TQSampleFolder>[nElements] : new ::TQIteratorT<TQSampleFolder>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETQSampleFoldergR(void *p) {
      delete ((::TQIteratorT<TQSampleFolder>*)p);
   }
   static void deleteArray_TQIteratorTlETQSampleFoldergR(void *p) {
      delete [] ((::TQIteratorT<TQSampleFolder>*)p);
   }
   static void destruct_TQIteratorTlETQSampleFoldergR(void *p) {
      typedef ::TQIteratorT<TQSampleFolder> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TQSampleFolder>

//______________________________________________________________________________
template <> void TQIteratorT<TQSample>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TQSample>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TQSample>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TQSample>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETQSamplegR(void *p) {
      return  p ? new(p) ::TQIteratorT<TQSample> : new ::TQIteratorT<TQSample>;
   }
   static void *newArray_TQIteratorTlETQSamplegR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TQSample>[nElements] : new ::TQIteratorT<TQSample>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETQSamplegR(void *p) {
      delete ((::TQIteratorT<TQSample>*)p);
   }
   static void deleteArray_TQIteratorTlETQSamplegR(void *p) {
      delete [] ((::TQIteratorT<TQSample>*)p);
   }
   static void destruct_TQIteratorTlETQSamplegR(void *p) {
      typedef ::TQIteratorT<TQSample> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TQSample>

//______________________________________________________________________________
template <> void TQIteratorT<TH1>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TH1>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TH1>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TH1>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETH1gR(void *p) {
      return  p ? new(p) ::TQIteratorT<TH1> : new ::TQIteratorT<TH1>;
   }
   static void *newArray_TQIteratorTlETH1gR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TH1>[nElements] : new ::TQIteratorT<TH1>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETH1gR(void *p) {
      delete ((::TQIteratorT<TH1>*)p);
   }
   static void deleteArray_TQIteratorTlETH1gR(void *p) {
      delete [] ((::TQIteratorT<TH1>*)p);
   }
   static void destruct_TQIteratorTlETH1gR(void *p) {
      typedef ::TQIteratorT<TH1> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TH1>

//______________________________________________________________________________
template <> void TQIteratorT<TQNamedTaggable>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TQNamedTaggable>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TQNamedTaggable>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TQNamedTaggable>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETQNamedTaggablegR(void *p) {
      return  p ? new(p) ::TQIteratorT<TQNamedTaggable> : new ::TQIteratorT<TQNamedTaggable>;
   }
   static void *newArray_TQIteratorTlETQNamedTaggablegR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TQNamedTaggable>[nElements] : new ::TQIteratorT<TQNamedTaggable>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETQNamedTaggablegR(void *p) {
      delete ((::TQIteratorT<TQNamedTaggable>*)p);
   }
   static void deleteArray_TQIteratorTlETQNamedTaggablegR(void *p) {
      delete [] ((::TQIteratorT<TQNamedTaggable>*)p);
   }
   static void destruct_TQIteratorTlETQNamedTaggablegR(void *p) {
      typedef ::TQIteratorT<TQNamedTaggable> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TQNamedTaggable>

//______________________________________________________________________________
template <> void TQIteratorT<TQObservable>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TQObservable>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TQObservable>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TQObservable>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETQObservablegR(void *p) {
      return  p ? new(p) ::TQIteratorT<TQObservable> : new ::TQIteratorT<TQObservable>;
   }
   static void *newArray_TQIteratorTlETQObservablegR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TQObservable>[nElements] : new ::TQIteratorT<TQObservable>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETQObservablegR(void *p) {
      delete ((::TQIteratorT<TQObservable>*)p);
   }
   static void deleteArray_TQIteratorTlETQObservablegR(void *p) {
      delete [] ((::TQIteratorT<TQObservable>*)p);
   }
   static void destruct_TQIteratorTlETQObservablegR(void *p) {
      typedef ::TQIteratorT<TQObservable> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TQObservable>

//______________________________________________________________________________
template <> void TQIteratorT<TQAnalysisJob>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TQAnalysisJob>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TQAnalysisJob>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TQAnalysisJob>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETQAnalysisJobgR(void *p) {
      return  p ? new(p) ::TQIteratorT<TQAnalysisJob> : new ::TQIteratorT<TQAnalysisJob>;
   }
   static void *newArray_TQIteratorTlETQAnalysisJobgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TQAnalysisJob>[nElements] : new ::TQIteratorT<TQAnalysisJob>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETQAnalysisJobgR(void *p) {
      delete ((::TQIteratorT<TQAnalysisJob>*)p);
   }
   static void deleteArray_TQIteratorTlETQAnalysisJobgR(void *p) {
      delete [] ((::TQIteratorT<TQAnalysisJob>*)p);
   }
   static void destruct_TQIteratorTlETQAnalysisJobgR(void *p) {
      typedef ::TQIteratorT<TQAnalysisJob> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TQAnalysisJob>

//______________________________________________________________________________
template <> void TQIteratorT<TGraphErrors>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TGraphErrors>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TGraphErrors>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TGraphErrors>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETGraphErrorsgR(void *p) {
      return  p ? new(p) ::TQIteratorT<TGraphErrors> : new ::TQIteratorT<TGraphErrors>;
   }
   static void *newArray_TQIteratorTlETGraphErrorsgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TGraphErrors>[nElements] : new ::TQIteratorT<TGraphErrors>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETGraphErrorsgR(void *p) {
      delete ((::TQIteratorT<TGraphErrors>*)p);
   }
   static void deleteArray_TQIteratorTlETGraphErrorsgR(void *p) {
      delete [] ((::TQIteratorT<TGraphErrors>*)p);
   }
   static void destruct_TQIteratorTlETGraphErrorsgR(void *p) {
      typedef ::TQIteratorT<TGraphErrors> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TGraphErrors>

//______________________________________________________________________________
template <> void TQIteratorT<TGraph>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TGraph>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TGraph>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TGraph>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETGraphgR(void *p) {
      return  p ? new(p) ::TQIteratorT<TGraph> : new ::TQIteratorT<TGraph>;
   }
   static void *newArray_TQIteratorTlETGraphgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TGraph>[nElements] : new ::TQIteratorT<TGraph>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETGraphgR(void *p) {
      delete ((::TQIteratorT<TGraph>*)p);
   }
   static void deleteArray_TQIteratorTlETGraphgR(void *p) {
      delete [] ((::TQIteratorT<TGraph>*)p);
   }
   static void destruct_TQIteratorTlETGraphgR(void *p) {
      typedef ::TQIteratorT<TGraph> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TGraph>

//______________________________________________________________________________
template <> void TQIteratorT<TGraphAsymmErrors>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TGraphAsymmErrors>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TGraphAsymmErrors>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TGraphAsymmErrors>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETGraphAsymmErrorsgR(void *p) {
      return  p ? new(p) ::TQIteratorT<TGraphAsymmErrors> : new ::TQIteratorT<TGraphAsymmErrors>;
   }
   static void *newArray_TQIteratorTlETGraphAsymmErrorsgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TGraphAsymmErrors>[nElements] : new ::TQIteratorT<TGraphAsymmErrors>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETGraphAsymmErrorsgR(void *p) {
      delete ((::TQIteratorT<TGraphAsymmErrors>*)p);
   }
   static void deleteArray_TQIteratorTlETGraphAsymmErrorsgR(void *p) {
      delete [] ((::TQIteratorT<TGraphAsymmErrors>*)p);
   }
   static void destruct_TQIteratorTlETGraphAsymmErrorsgR(void *p) {
      typedef ::TQIteratorT<TGraphAsymmErrors> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TGraphAsymmErrors>

//______________________________________________________________________________
template <> void TQIteratorT<TCollection>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TCollection>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TCollection>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TCollection>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETCollectiongR(void *p) {
      return  p ? new(p) ::TQIteratorT<TCollection> : new ::TQIteratorT<TCollection>;
   }
   static void *newArray_TQIteratorTlETCollectiongR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TCollection>[nElements] : new ::TQIteratorT<TCollection>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETCollectiongR(void *p) {
      delete ((::TQIteratorT<TCollection>*)p);
   }
   static void deleteArray_TQIteratorTlETCollectiongR(void *p) {
      delete [] ((::TQIteratorT<TCollection>*)p);
   }
   static void destruct_TQIteratorTlETCollectiongR(void *p) {
      typedef ::TQIteratorT<TCollection> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TCollection>

//______________________________________________________________________________
template <> void TQIteratorT<TList>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TList>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TList>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TList>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETListgR(void *p) {
      return  p ? new(p) ::TQIteratorT<TList> : new ::TQIteratorT<TList>;
   }
   static void *newArray_TQIteratorTlETListgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TList>[nElements] : new ::TQIteratorT<TList>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETListgR(void *p) {
      delete ((::TQIteratorT<TList>*)p);
   }
   static void deleteArray_TQIteratorTlETListgR(void *p) {
      delete [] ((::TQIteratorT<TList>*)p);
   }
   static void destruct_TQIteratorTlETListgR(void *p) {
      typedef ::TQIteratorT<TList> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TList>

//______________________________________________________________________________
template <> void TQIteratorT<TLegendEntry>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TLegendEntry>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TLegendEntry>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TLegendEntry>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETLegendEntrygR(void *p) {
      return  p ? new(p) ::TQIteratorT<TLegendEntry> : new ::TQIteratorT<TLegendEntry>;
   }
   static void *newArray_TQIteratorTlETLegendEntrygR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TLegendEntry>[nElements] : new ::TQIteratorT<TLegendEntry>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETLegendEntrygR(void *p) {
      delete ((::TQIteratorT<TLegendEntry>*)p);
   }
   static void deleteArray_TQIteratorTlETLegendEntrygR(void *p) {
      delete [] ((::TQIteratorT<TLegendEntry>*)p);
   }
   static void destruct_TQIteratorTlETLegendEntrygR(void *p) {
      typedef ::TQIteratorT<TLegendEntry> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TLegendEntry>

//______________________________________________________________________________
template <> void TQIteratorT<TBranch>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TBranch>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TBranch>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TBranch>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETBranchgR(void *p) {
      return  p ? new(p) ::TQIteratorT<TBranch> : new ::TQIteratorT<TBranch>;
   }
   static void *newArray_TQIteratorTlETBranchgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TBranch>[nElements] : new ::TQIteratorT<TBranch>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETBranchgR(void *p) {
      delete ((::TQIteratorT<TBranch>*)p);
   }
   static void deleteArray_TQIteratorTlETBranchgR(void *p) {
      delete [] ((::TQIteratorT<TBranch>*)p);
   }
   static void destruct_TQIteratorTlETBranchgR(void *p) {
      typedef ::TQIteratorT<TBranch> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TBranch>

//______________________________________________________________________________
template <> void TQIteratorT<TKey>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TKey>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TKey>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TKey>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETKeygR(void *p) {
      return  p ? new(p) ::TQIteratorT<TKey> : new ::TQIteratorT<TKey>;
   }
   static void *newArray_TQIteratorTlETKeygR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TKey>[nElements] : new ::TQIteratorT<TKey>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETKeygR(void *p) {
      delete ((::TQIteratorT<TKey>*)p);
   }
   static void deleteArray_TQIteratorTlETKeygR(void *p) {
      delete [] ((::TQIteratorT<TKey>*)p);
   }
   static void destruct_TQIteratorTlETKeygR(void *p) {
      typedef ::TQIteratorT<TKey> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TKey>

//______________________________________________________________________________
template <> void TQIteratorT<TQCounterGrid>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TQCounterGrid>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TQCounterGrid>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TQCounterGrid>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETQCounterGridgR(void *p) {
      return  p ? new(p) ::TQIteratorT<TQCounterGrid> : new ::TQIteratorT<TQCounterGrid>;
   }
   static void *newArray_TQIteratorTlETQCounterGridgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TQCounterGrid>[nElements] : new ::TQIteratorT<TQCounterGrid>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETQCounterGridgR(void *p) {
      delete ((::TQIteratorT<TQCounterGrid>*)p);
   }
   static void deleteArray_TQIteratorTlETQCounterGridgR(void *p) {
      delete [] ((::TQIteratorT<TQCounterGrid>*)p);
   }
   static void destruct_TQIteratorTlETQCounterGridgR(void *p) {
      typedef ::TQIteratorT<TQCounterGrid> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TQCounterGrid>

//______________________________________________________________________________
template <> void TQIteratorT<TQPCA>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TQPCA>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TQPCA>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TQPCA>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETQPCAgR(void *p) {
      return  p ? new(p) ::TQIteratorT<TQPCA> : new ::TQIteratorT<TQPCA>;
   }
   static void *newArray_TQIteratorTlETQPCAgR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TQPCA>[nElements] : new ::TQIteratorT<TQPCA>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETQPCAgR(void *p) {
      delete ((::TQIteratorT<TQPCA>*)p);
   }
   static void deleteArray_TQIteratorTlETQPCAgR(void *p) {
      delete [] ((::TQIteratorT<TQPCA>*)p);
   }
   static void destruct_TQIteratorTlETQPCAgR(void *p) {
      typedef ::TQIteratorT<TQPCA> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TQPCA>

//______________________________________________________________________________
template <> void TQIteratorT<TH2>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TH2>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TH2>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TH2>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETH2gR(void *p) {
      return  p ? new(p) ::TQIteratorT<TH2> : new ::TQIteratorT<TH2>;
   }
   static void *newArray_TQIteratorTlETH2gR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TH2>[nElements] : new ::TQIteratorT<TH2>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETH2gR(void *p) {
      delete ((::TQIteratorT<TH2>*)p);
   }
   static void deleteArray_TQIteratorTlETH2gR(void *p) {
      delete [] ((::TQIteratorT<TH2>*)p);
   }
   static void destruct_TQIteratorTlETH2gR(void *p) {
      typedef ::TQIteratorT<TH2> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TH2>

//______________________________________________________________________________
template <> void TQIteratorT<TObjArray>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQIteratorT<TObjArray>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQIteratorT<TObjArray>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQIteratorT<TObjArray>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQIteratorTlETObjArraygR(void *p) {
      return  p ? new(p) ::TQIteratorT<TObjArray> : new ::TQIteratorT<TObjArray>;
   }
   static void *newArray_TQIteratorTlETObjArraygR(Long_t nElements, void *p) {
      return p ? new(p) ::TQIteratorT<TObjArray>[nElements] : new ::TQIteratorT<TObjArray>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQIteratorTlETObjArraygR(void *p) {
      delete ((::TQIteratorT<TObjArray>*)p);
   }
   static void deleteArray_TQIteratorTlETObjArraygR(void *p) {
      delete [] ((::TQIteratorT<TObjArray>*)p);
   }
   static void destruct_TQIteratorTlETObjArraygR(void *p) {
      typedef ::TQIteratorT<TObjArray> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQIteratorT<TObjArray>

//______________________________________________________________________________
void TQPCA::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQPCA.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQPCA::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQPCA::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQPCA(void *p) {
      return  p ? new(p) ::TQPCA : new ::TQPCA;
   }
   static void *newArray_TQPCA(Long_t nElements, void *p) {
      return p ? new(p) ::TQPCA[nElements] : new ::TQPCA[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQPCA(void *p) {
      delete ((::TQPCA*)p);
   }
   static void deleteArray_TQPCA(void *p) {
      delete [] ((::TQPCA*)p);
   }
   static void destruct_TQPCA(void *p) {
      typedef ::TQPCA current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQPCA

//______________________________________________________________________________
void TQFolder::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQFolder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQFolder::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQFolder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQFolder(void *p) {
      return  p ? new(p) ::TQFolder : new ::TQFolder;
   }
   static void *newArray_TQFolder(Long_t nElements, void *p) {
      return p ? new(p) ::TQFolder[nElements] : new ::TQFolder[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQFolder(void *p) {
      delete ((::TQFolder*)p);
   }
   static void deleteArray_TQFolder(void *p) {
      delete [] ((::TQFolder*)p);
   }
   static void destruct_TQFolder(void *p) {
      typedef ::TQFolder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQFolder

//______________________________________________________________________________
void TQSampleFolder::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSampleFolder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSampleFolder::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSampleFolder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSampleFolder(void *p) {
      return  p ? new(p) ::TQSampleFolder : new ::TQSampleFolder;
   }
   static void *newArray_TQSampleFolder(Long_t nElements, void *p) {
      return p ? new(p) ::TQSampleFolder[nElements] : new ::TQSampleFolder[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSampleFolder(void *p) {
      delete ((::TQSampleFolder*)p);
   }
   static void deleteArray_TQSampleFolder(void *p) {
      delete [] ((::TQSampleFolder*)p);
   }
   static void destruct_TQSampleFolder(void *p) {
      typedef ::TQSampleFolder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSampleFolder

//______________________________________________________________________________
void TQSample::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSample.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSample::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSample::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSample(void *p) {
      return  p ? new(p) ::TQSample : new ::TQSample;
   }
   static void *newArray_TQSample(Long_t nElements, void *p) {
      return p ? new(p) ::TQSample[nElements] : new ::TQSample[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSample(void *p) {
      delete ((::TQSample*)p);
   }
   static void deleteArray_TQSample(void *p) {
      delete [] ((::TQSample*)p);
   }
   static void destruct_TQSample(void *p) {
      typedef ::TQSample current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSample

//______________________________________________________________________________
void TQObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQObservable(void *p) {
      delete ((::TQObservable*)p);
   }
   static void deleteArray_TQObservable(void *p) {
      delete [] ((::TQObservable*)p);
   }
   static void destruct_TQObservable(void *p) {
      typedef ::TQObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQObservable

//______________________________________________________________________________
void TQAnalysisJob::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQAnalysisJob.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQAnalysisJob::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQAnalysisJob::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQAnalysisJob(void *p) {
      delete ((::TQAnalysisJob*)p);
   }
   static void deleteArray_TQAnalysisJob(void *p) {
      delete [] ((::TQAnalysisJob*)p);
   }
   static void destruct_TQAnalysisJob(void *p) {
      typedef ::TQAnalysisJob current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQAnalysisJob

//______________________________________________________________________________
void TQCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCut(void *p) {
      return  p ? new(p) ::TQCut : new ::TQCut;
   }
   static void *newArray_TQCut(Long_t nElements, void *p) {
      return p ? new(p) ::TQCut[nElements] : new ::TQCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCut(void *p) {
      delete ((::TQCut*)p);
   }
   static void deleteArray_TQCut(void *p) {
      delete [] ((::TQCut*)p);
   }
   static void destruct_TQCut(void *p) {
      typedef ::TQCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCut

//______________________________________________________________________________
void TQAnalysisAlgorithm::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQAnalysisAlgorithm.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQAnalysisAlgorithm::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQAnalysisAlgorithm::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQAnalysisAlgorithm(void *p) {
      delete ((::TQAnalysisAlgorithm*)p);
   }
   static void deleteArray_TQAnalysisAlgorithm(void *p) {
      delete [] ((::TQAnalysisAlgorithm*)p);
   }
   static void destruct_TQAnalysisAlgorithm(void *p) {
      typedef ::TQAnalysisAlgorithm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQAnalysisAlgorithm

//______________________________________________________________________________
void TQSampleVisitor::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSampleVisitor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSampleVisitor::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSampleVisitor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSampleVisitor(void *p) {
      return  p ? new(p) ::TQSampleVisitor : new ::TQSampleVisitor;
   }
   static void *newArray_TQSampleVisitor(Long_t nElements, void *p) {
      return p ? new(p) ::TQSampleVisitor[nElements] : new ::TQSampleVisitor[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSampleVisitor(void *p) {
      delete ((::TQSampleVisitor*)p);
   }
   static void deleteArray_TQSampleVisitor(void *p) {
      delete [] ((::TQSampleVisitor*)p);
   }
   static void destruct_TQSampleVisitor(void *p) {
      typedef ::TQSampleVisitor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSampleVisitor

//______________________________________________________________________________
void TQAnalysisSampleVisitorBase::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQAnalysisSampleVisitorBase.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQAnalysisSampleVisitorBase::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQAnalysisSampleVisitorBase::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQAnalysisSampleVisitorBase(void *p) {
      delete ((::TQAnalysisSampleVisitorBase*)p);
   }
   static void deleteArray_TQAnalysisSampleVisitorBase(void *p) {
      delete [] ((::TQAnalysisSampleVisitorBase*)p);
   }
   static void destruct_TQAnalysisSampleVisitorBase(void *p) {
      typedef ::TQAnalysisSampleVisitorBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQAnalysisSampleVisitorBase

//______________________________________________________________________________
void TQAnalysisSampleVisitor::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQAnalysisSampleVisitor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQAnalysisSampleVisitor::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQAnalysisSampleVisitor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQAnalysisSampleVisitor(void *p) {
      return  p ? new(p) ::TQAnalysisSampleVisitor : new ::TQAnalysisSampleVisitor;
   }
   static void *newArray_TQAnalysisSampleVisitor(Long_t nElements, void *p) {
      return p ? new(p) ::TQAnalysisSampleVisitor[nElements] : new ::TQAnalysisSampleVisitor[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQAnalysisSampleVisitor(void *p) {
      delete ((::TQAnalysisSampleVisitor*)p);
   }
   static void deleteArray_TQAnalysisSampleVisitor(void *p) {
      delete [] ((::TQAnalysisSampleVisitor*)p);
   }
   static void destruct_TQAnalysisSampleVisitor(void *p) {
      typedef ::TQAnalysisSampleVisitor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQAnalysisSampleVisitor

//______________________________________________________________________________
void TQTable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQTable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQTable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQTable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQTable(void *p) {
      return  p ? new(p) ::TQTable : new ::TQTable;
   }
   static void *newArray_TQTable(Long_t nElements, void *p) {
      return p ? new(p) ::TQTable[nElements] : new ::TQTable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQTable(void *p) {
      delete ((::TQTable*)p);
   }
   static void deleteArray_TQTable(void *p) {
      delete [] ((::TQTable*)p);
   }
   static void destruct_TQTable(void *p) {
      typedef ::TQTable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQTable

//______________________________________________________________________________
void TQSampleDataReader::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSampleDataReader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSampleDataReader::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSampleDataReader::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSampleDataReader(void *p) {
      return  p ? new(p) ::TQSampleDataReader : new ::TQSampleDataReader;
   }
   static void *newArray_TQSampleDataReader(Long_t nElements, void *p) {
      return p ? new(p) ::TQSampleDataReader[nElements] : new ::TQSampleDataReader[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSampleDataReader(void *p) {
      delete ((::TQSampleDataReader*)p);
   }
   static void deleteArray_TQSampleDataReader(void *p) {
      delete [] ((::TQSampleDataReader*)p);
   }
   static void destruct_TQSampleDataReader(void *p) {
      typedef ::TQSampleDataReader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSampleDataReader

//______________________________________________________________________________
void TQPresenter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQPresenter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQPresenter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQPresenter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQPresenter(void *p) {
      return  p ? new(p) ::TQPresenter : new ::TQPresenter;
   }
   static void *newArray_TQPresenter(Long_t nElements, void *p) {
      return p ? new(p) ::TQPresenter[nElements] : new ::TQPresenter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQPresenter(void *p) {
      delete ((::TQPresenter*)p);
   }
   static void deleteArray_TQPresenter(void *p) {
      delete [] ((::TQPresenter*)p);
   }
   static void destruct_TQPresenter(void *p) {
      typedef ::TQPresenter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQPresenter

//______________________________________________________________________________
void TQPlotter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQPlotter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQPlotter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQPlotter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQPlotter(void *p) {
      delete ((::TQPlotter*)p);
   }
   static void deleteArray_TQPlotter(void *p) {
      delete [] ((::TQPlotter*)p);
   }
   static void destruct_TQPlotter(void *p) {
      typedef ::TQPlotter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQPlotter

//______________________________________________________________________________
void TQROOTPlotter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQROOTPlotter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQROOTPlotter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQROOTPlotter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQROOTPlotter(void *p) {
      delete ((::TQROOTPlotter*)p);
   }
   static void deleteArray_TQROOTPlotter(void *p) {
      delete [] ((::TQROOTPlotter*)p);
   }
   static void destruct_TQROOTPlotter(void *p) {
      typedef ::TQROOTPlotter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQROOTPlotter

//______________________________________________________________________________
void TQCompPlotter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCompPlotter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCompPlotter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCompPlotter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQCompPlotter(void *p) {
      delete ((::TQCompPlotter*)p);
   }
   static void deleteArray_TQCompPlotter(void *p) {
      delete [] ((::TQCompPlotter*)p);
   }
   static void destruct_TQCompPlotter(void *p) {
      typedef ::TQCompPlotter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCompPlotter

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQConfigReader(void *p) {
      delete ((::TQConfigReader*)p);
   }
   static void deleteArray_TQConfigReader(void *p) {
      delete [] ((::TQConfigReader*)p);
   }
   static void destruct_TQConfigReader(void *p) {
      typedef ::TQConfigReader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQConfigReader

//______________________________________________________________________________
void TQConstObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQConstObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQConstObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQConstObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQConstObservable(void *p) {
      return  p ? new(p) ::TQConstObservable : new ::TQConstObservable;
   }
   static void *newArray_TQConstObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQConstObservable[nElements] : new ::TQConstObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQConstObservable(void *p) {
      delete ((::TQConstObservable*)p);
   }
   static void deleteArray_TQConstObservable(void *p) {
      delete [] ((::TQConstObservable*)p);
   }
   static void destruct_TQConstObservable(void *p) {
      typedef ::TQConstObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQConstObservable

//______________________________________________________________________________
void TQCutFactory::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCutFactory.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCutFactory::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCutFactory::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCutFactory(void *p) {
      return  p ? new(p) ::TQCutFactory : new ::TQCutFactory;
   }
   static void *newArray_TQCutFactory(Long_t nElements, void *p) {
      return p ? new(p) ::TQCutFactory[nElements] : new ::TQCutFactory[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCutFactory(void *p) {
      delete ((::TQCutFactory*)p);
   }
   static void deleteArray_TQCutFactory(void *p) {
      delete [] ((::TQCutFactory*)p);
   }
   static void destruct_TQCutFactory(void *p) {
      typedef ::TQCutFactory current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCutFactory

//______________________________________________________________________________
void TQCutflowAnalysisJob::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCutflowAnalysisJob.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCutflowAnalysisJob::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCutflowAnalysisJob::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCutflowAnalysisJob(void *p) {
      return  p ? new(p) ::TQCutflowAnalysisJob : new ::TQCutflowAnalysisJob;
   }
   static void *newArray_TQCutflowAnalysisJob(Long_t nElements, void *p) {
      return p ? new(p) ::TQCutflowAnalysisJob[nElements] : new ::TQCutflowAnalysisJob[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCutflowAnalysisJob(void *p) {
      delete ((::TQCutflowAnalysisJob*)p);
   }
   static void deleteArray_TQCutflowAnalysisJob(void *p) {
      delete [] ((::TQCutflowAnalysisJob*)p);
   }
   static void destruct_TQCutflowAnalysisJob(void *p) {
      typedef ::TQCutflowAnalysisJob current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCutflowAnalysisJob

//______________________________________________________________________________
void TQCutflowPrinter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCutflowPrinter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCutflowPrinter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCutflowPrinter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQCutflowPrinter(void *p) {
      return  p ? new(p) ::TQCutflowPrinter : new ::TQCutflowPrinter;
   }
   static void *newArray_TQCutflowPrinter(Long_t nElements, void *p) {
      return p ? new(p) ::TQCutflowPrinter[nElements] : new ::TQCutflowPrinter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQCutflowPrinter(void *p) {
      delete ((::TQCutflowPrinter*)p);
   }
   static void deleteArray_TQCutflowPrinter(void *p) {
      delete [] ((::TQCutflowPrinter*)p);
   }
   static void destruct_TQCutflowPrinter(void *p) {
      typedef ::TQCutflowPrinter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCutflowPrinter

//______________________________________________________________________________
void TQCutflowPlotter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQCutflowPlotter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQCutflowPlotter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQCutflowPlotter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQCutflowPlotter(void *p) {
      delete ((::TQCutflowPlotter*)p);
   }
   static void deleteArray_TQCutflowPlotter(void *p) {
      delete [] ((::TQCutflowPlotter*)p);
   }
   static void destruct_TQCutflowPlotter(void *p) {
      typedef ::TQCutflowPlotter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQCutflowPlotter

//______________________________________________________________________________
void TQDefaultPlotter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQDefaultPlotter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQDefaultPlotter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQDefaultPlotter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQDefaultPlotter(void *p) {
      delete ((::TQDefaultPlotter*)p);
   }
   static void deleteArray_TQDefaultPlotter(void *p) {
      delete [] ((::TQDefaultPlotter*)p);
   }
   static void destruct_TQDefaultPlotter(void *p) {
      typedef ::TQDefaultPlotter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQDefaultPlotter

//______________________________________________________________________________
void TQTreeObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQTreeObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQTreeObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQTreeObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQTreeObservable(void *p) {
      delete ((::TQTreeObservable*)p);
   }
   static void deleteArray_TQTreeObservable(void *p) {
      delete [] ((::TQTreeObservable*)p);
   }
   static void destruct_TQTreeObservable(void *p) {
      typedef ::TQTreeObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQTreeObservable

//______________________________________________________________________________
void TQEventIndexObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQEventIndexObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQEventIndexObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQEventIndexObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQEventIndexObservable(void *p) {
      return  p ? new(p) ::TQEventIndexObservable : new ::TQEventIndexObservable;
   }
   static void *newArray_TQEventIndexObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQEventIndexObservable[nElements] : new ::TQEventIndexObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQEventIndexObservable(void *p) {
      delete ((::TQEventIndexObservable*)p);
   }
   static void deleteArray_TQEventIndexObservable(void *p) {
      delete [] ((::TQEventIndexObservable*)p);
   }
   static void destruct_TQEventIndexObservable(void *p) {
      typedef ::TQEventIndexObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQEventIndexObservable

//______________________________________________________________________________
void TQEventlistAnalysisJob::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQEventlistAnalysisJob.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQEventlistAnalysisJob::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQEventlistAnalysisJob::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQEventlistAnalysisJob(void *p) {
      return  p ? new(p) ::TQEventlistAnalysisJob : new ::TQEventlistAnalysisJob;
   }
   static void *newArray_TQEventlistAnalysisJob(Long_t nElements, void *p) {
      return p ? new(p) ::TQEventlistAnalysisJob[nElements] : new ::TQEventlistAnalysisJob[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQEventlistAnalysisJob(void *p) {
      delete ((::TQEventlistAnalysisJob*)p);
   }
   static void deleteArray_TQEventlistAnalysisJob(void *p) {
      delete [] ((::TQEventlistAnalysisJob*)p);
   }
   static void destruct_TQEventlistAnalysisJob(void *p) {
      typedef ::TQEventlistAnalysisJob current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQEventlistAnalysisJob

//______________________________________________________________________________
void TQEventlistPrinter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQEventlistPrinter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQEventlistPrinter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQEventlistPrinter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQEventlistPrinter(void *p) {
      return  p ? new(p) ::TQEventlistPrinter : new ::TQEventlistPrinter;
   }
   static void *newArray_TQEventlistPrinter(Long_t nElements, void *p) {
      return p ? new(p) ::TQEventlistPrinter[nElements] : new ::TQEventlistPrinter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQEventlistPrinter(void *p) {
      delete ((::TQEventlistPrinter*)p);
   }
   static void deleteArray_TQEventlistPrinter(void *p) {
      delete [] ((::TQEventlistPrinter*)p);
   }
   static void destruct_TQEventlistPrinter(void *p) {
      typedef ::TQEventlistPrinter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQEventlistPrinter

//______________________________________________________________________________
void TQFilterObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQFilterObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQFilterObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQFilterObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQFilterObservable(void *p) {
      return  p ? new(p) ::TQFilterObservable : new ::TQFilterObservable;
   }
   static void *newArray_TQFilterObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQFilterObservable[nElements] : new ::TQFilterObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQFilterObservable(void *p) {
      delete ((::TQFilterObservable*)p);
   }
   static void deleteArray_TQFilterObservable(void *p) {
      delete [] ((::TQFilterObservable*)p);
   }
   static void destruct_TQFilterObservable(void *p) {
      typedef ::TQFilterObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQFilterObservable

//______________________________________________________________________________
void TQGraphMakerAnalysisJob::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQGraphMakerAnalysisJob.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQGraphMakerAnalysisJob::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQGraphMakerAnalysisJob::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQGraphMakerAnalysisJob(void *p) {
      return  p ? new(p) ::TQGraphMakerAnalysisJob : new ::TQGraphMakerAnalysisJob;
   }
   static void *newArray_TQGraphMakerAnalysisJob(Long_t nElements, void *p) {
      return p ? new(p) ::TQGraphMakerAnalysisJob[nElements] : new ::TQGraphMakerAnalysisJob[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQGraphMakerAnalysisJob(void *p) {
      delete ((::TQGraphMakerAnalysisJob*)p);
   }
   static void deleteArray_TQGraphMakerAnalysisJob(void *p) {
      delete [] ((::TQGraphMakerAnalysisJob*)p);
   }
   static void destruct_TQGraphMakerAnalysisJob(void *p) {
      typedef ::TQGraphMakerAnalysisJob current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQGraphMakerAnalysisJob

//______________________________________________________________________________
void TQGridScanBound::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQGridScanBound.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQGridScanBound::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQGridScanBound::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQGridScanBound(void *p) {
      return  p ? new(p) ::TQGridScanBound : new ::TQGridScanBound;
   }
   static void *newArray_TQGridScanBound(Long_t nElements, void *p) {
      return p ? new(p) ::TQGridScanBound[nElements] : new ::TQGridScanBound[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQGridScanBound(void *p) {
      delete ((::TQGridScanBound*)p);
   }
   static void deleteArray_TQGridScanBound(void *p) {
      delete [] ((::TQGridScanBound*)p);
   }
   static void destruct_TQGridScanBound(void *p) {
      typedef ::TQGridScanBound current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQGridScanBound

//______________________________________________________________________________
void TQGridScanBounds::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQGridScanBounds.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQGridScanBounds::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQGridScanBounds::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQGridScanBounds(void *p) {
      return  p ? new(p) ::TQGridScanBounds : new ::TQGridScanBounds;
   }
   static void *newArray_TQGridScanBounds(Long_t nElements, void *p) {
      return p ? new(p) ::TQGridScanBounds[nElements] : new ::TQGridScanBounds[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQGridScanBounds(void *p) {
      delete ((::TQGridScanBounds*)p);
   }
   static void deleteArray_TQGridScanBounds(void *p) {
      delete [] ((::TQGridScanBounds*)p);
   }
   static void destruct_TQGridScanBounds(void *p) {
      typedef ::TQGridScanBounds current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQGridScanBounds

//______________________________________________________________________________
void TQGridScanObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQGridScanObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQGridScanObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQGridScanObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQGridScanObservable(void *p) {
      return  p ? new(p) ::TQGridScanObservable : new ::TQGridScanObservable;
   }
   static void *newArray_TQGridScanObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQGridScanObservable[nElements] : new ::TQGridScanObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQGridScanObservable(void *p) {
      delete ((::TQGridScanObservable*)p);
   }
   static void deleteArray_TQGridScanObservable(void *p) {
      delete [] ((::TQGridScanObservable*)p);
   }
   static void destruct_TQGridScanObservable(void *p) {
      typedef ::TQGridScanObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQGridScanObservable

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQHistParams(void *p) {
      return  p ? new(p) ::TQHistParams : new ::TQHistParams;
   }
   static void *newArray_TQHistParams(Long_t nElements, void *p) {
      return p ? new(p) ::TQHistParams[nElements] : new ::TQHistParams[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQHistParams(void *p) {
      delete ((::TQHistParams*)p);
   }
   static void deleteArray_TQHistParams(void *p) {
      delete [] ((::TQHistParams*)p);
   }
   static void destruct_TQHistParams(void *p) {
      typedef ::TQHistParams current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQHistParams

//______________________________________________________________________________
void TQGridScanNormalObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQGridScanNormalObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQGridScanNormalObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQGridScanNormalObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQGridScanNormalObservable(void *p) {
      return  p ? new(p) ::TQGridScanNormalObservable : new ::TQGridScanNormalObservable;
   }
   static void *newArray_TQGridScanNormalObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQGridScanNormalObservable[nElements] : new ::TQGridScanNormalObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQGridScanNormalObservable(void *p) {
      delete ((::TQGridScanNormalObservable*)p);
   }
   static void deleteArray_TQGridScanNormalObservable(void *p) {
      delete [] ((::TQGridScanNormalObservable*)p);
   }
   static void destruct_TQGridScanNormalObservable(void *p) {
      typedef ::TQGridScanNormalObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQGridScanNormalObservable

//______________________________________________________________________________
void TQGridScanSplitObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQGridScanSplitObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQGridScanSplitObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQGridScanSplitObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQGridScanSplitObservable(void *p) {
      return  p ? new(p) ::TQGridScanSplitObservable : new ::TQGridScanSplitObservable;
   }
   static void *newArray_TQGridScanSplitObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQGridScanSplitObservable[nElements] : new ::TQGridScanSplitObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQGridScanSplitObservable(void *p) {
      delete ((::TQGridScanSplitObservable*)p);
   }
   static void deleteArray_TQGridScanSplitObservable(void *p) {
      delete [] ((::TQGridScanSplitObservable*)p);
   }
   static void destruct_TQGridScanSplitObservable(void *p) {
      typedef ::TQGridScanSplitObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQGridScanSplitObservable

//______________________________________________________________________________
void TQGridScanPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQGridScanPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQGridScanPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQGridScanPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQGridScanPoint(void *p) {
      return  p ? new(p) ::TQGridScanPoint : new ::TQGridScanPoint;
   }
   static void *newArray_TQGridScanPoint(Long_t nElements, void *p) {
      return p ? new(p) ::TQGridScanPoint[nElements] : new ::TQGridScanPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQGridScanPoint(void *p) {
      delete ((::TQGridScanPoint*)p);
   }
   static void deleteArray_TQGridScanPoint(void *p) {
      delete [] ((::TQGridScanPoint*)p);
   }
   static void destruct_TQGridScanPoint(void *p) {
      typedef ::TQGridScanPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQGridScanPoint

//______________________________________________________________________________
void TQGridScanResults::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQGridScanResults.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQGridScanResults::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQGridScanResults::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQGridScanResults(void *p) {
      return  p ? new(p) ::TQGridScanResults : new ::TQGridScanResults;
   }
   static void *newArray_TQGridScanResults(Long_t nElements, void *p) {
      return p ? new(p) ::TQGridScanResults[nElements] : new ::TQGridScanResults[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQGridScanResults(void *p) {
      delete ((::TQGridScanResults*)p);
   }
   static void deleteArray_TQGridScanResults(void *p) {
      delete [] ((::TQGridScanResults*)p);
   }
   static void destruct_TQGridScanResults(void *p) {
      typedef ::TQGridScanResults current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQGridScanResults

//______________________________________________________________________________
void TQSignificanceEvaluator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSignificanceEvaluator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSignificanceEvaluator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSignificanceEvaluator::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQSignificanceEvaluator(void *p) {
      delete ((::TQSignificanceEvaluator*)p);
   }
   static void deleteArray_TQSignificanceEvaluator(void *p) {
      delete [] ((::TQSignificanceEvaluator*)p);
   }
   static void destruct_TQSignificanceEvaluator(void *p) {
      typedef ::TQSignificanceEvaluator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSignificanceEvaluator

//______________________________________________________________________________
void TQSignificanceEvaluatorBase::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSignificanceEvaluatorBase.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSignificanceEvaluatorBase::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSignificanceEvaluatorBase::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSignificanceEvaluatorBase(void *p) {
      return  p ? new(p) ::TQSignificanceEvaluatorBase : new ::TQSignificanceEvaluatorBase;
   }
   static void *newArray_TQSignificanceEvaluatorBase(Long_t nElements, void *p) {
      return p ? new(p) ::TQSignificanceEvaluatorBase[nElements] : new ::TQSignificanceEvaluatorBase[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSignificanceEvaluatorBase(void *p) {
      delete ((::TQSignificanceEvaluatorBase*)p);
   }
   static void deleteArray_TQSignificanceEvaluatorBase(void *p) {
      delete [] ((::TQSignificanceEvaluatorBase*)p);
   }
   static void destruct_TQSignificanceEvaluatorBase(void *p) {
      typedef ::TQSignificanceEvaluatorBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSignificanceEvaluatorBase

//______________________________________________________________________________
void TQSimpleSignificanceEvaluator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSimpleSignificanceEvaluator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSimpleSignificanceEvaluator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSimpleSignificanceEvaluator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSimpleSignificanceEvaluator(void *p) {
      return  p ? new(p) ::TQSimpleSignificanceEvaluator : new ::TQSimpleSignificanceEvaluator;
   }
   static void *newArray_TQSimpleSignificanceEvaluator(Long_t nElements, void *p) {
      return p ? new(p) ::TQSimpleSignificanceEvaluator[nElements] : new ::TQSimpleSignificanceEvaluator[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSimpleSignificanceEvaluator(void *p) {
      delete ((::TQSimpleSignificanceEvaluator*)p);
   }
   static void deleteArray_TQSimpleSignificanceEvaluator(void *p) {
      delete [] ((::TQSimpleSignificanceEvaluator*)p);
   }
   static void destruct_TQSimpleSignificanceEvaluator(void *p) {
      typedef ::TQSimpleSignificanceEvaluator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSimpleSignificanceEvaluator

//______________________________________________________________________________
void TQSimpleSignificanceEvaluator2::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSimpleSignificanceEvaluator2.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSimpleSignificanceEvaluator2::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSimpleSignificanceEvaluator2::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSimpleSignificanceEvaluator2(void *p) {
      return  p ? new(p) ::TQSimpleSignificanceEvaluator2 : new ::TQSimpleSignificanceEvaluator2;
   }
   static void *newArray_TQSimpleSignificanceEvaluator2(Long_t nElements, void *p) {
      return p ? new(p) ::TQSimpleSignificanceEvaluator2[nElements] : new ::TQSimpleSignificanceEvaluator2[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSimpleSignificanceEvaluator2(void *p) {
      delete ((::TQSimpleSignificanceEvaluator2*)p);
   }
   static void deleteArray_TQSimpleSignificanceEvaluator2(void *p) {
      delete [] ((::TQSimpleSignificanceEvaluator2*)p);
   }
   static void destruct_TQSimpleSignificanceEvaluator2(void *p) {
      typedef ::TQSimpleSignificanceEvaluator2 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSimpleSignificanceEvaluator2

//______________________________________________________________________________
void TQSimpleSignificanceEvaluator3::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSimpleSignificanceEvaluator3.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSimpleSignificanceEvaluator3::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSimpleSignificanceEvaluator3::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSimpleSignificanceEvaluator3(void *p) {
      return  p ? new(p) ::TQSimpleSignificanceEvaluator3 : new ::TQSimpleSignificanceEvaluator3;
   }
   static void *newArray_TQSimpleSignificanceEvaluator3(Long_t nElements, void *p) {
      return p ? new(p) ::TQSimpleSignificanceEvaluator3[nElements] : new ::TQSimpleSignificanceEvaluator3[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSimpleSignificanceEvaluator3(void *p) {
      delete ((::TQSimpleSignificanceEvaluator3*)p);
   }
   static void deleteArray_TQSimpleSignificanceEvaluator3(void *p) {
      delete [] ((::TQSimpleSignificanceEvaluator3*)p);
   }
   static void destruct_TQSimpleSignificanceEvaluator3(void *p) {
      typedef ::TQSimpleSignificanceEvaluator3 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSimpleSignificanceEvaluator3

//______________________________________________________________________________
void TQPoissonSignificanceEvaluator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQPoissonSignificanceEvaluator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQPoissonSignificanceEvaluator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQPoissonSignificanceEvaluator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQPoissonSignificanceEvaluator(void *p) {
      return  p ? new(p) ::TQPoissonSignificanceEvaluator : new ::TQPoissonSignificanceEvaluator;
   }
   static void *newArray_TQPoissonSignificanceEvaluator(Long_t nElements, void *p) {
      return p ? new(p) ::TQPoissonSignificanceEvaluator[nElements] : new ::TQPoissonSignificanceEvaluator[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQPoissonSignificanceEvaluator(void *p) {
      delete ((::TQPoissonSignificanceEvaluator*)p);
   }
   static void deleteArray_TQPoissonSignificanceEvaluator(void *p) {
      delete [] ((::TQPoissonSignificanceEvaluator*)p);
   }
   static void destruct_TQPoissonSignificanceEvaluator(void *p) {
      typedef ::TQPoissonSignificanceEvaluator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQPoissonSignificanceEvaluator

//______________________________________________________________________________
void TQGridScanner::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQGridScanner.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQGridScanner::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQGridScanner::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQGridScanner(void *p) {
      return  p ? new(p) ::TQGridScanner : new ::TQGridScanner;
   }
   static void *newArray_TQGridScanner(Long_t nElements, void *p) {
      return p ? new(p) ::TQGridScanner[nElements] : new ::TQGridScanner[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQGridScanner(void *p) {
      delete ((::TQGridScanner*)p);
   }
   static void deleteArray_TQGridScanner(void *p) {
      delete [] ((::TQGridScanner*)p);
   }
   static void destruct_TQGridScanner(void *p) {
      typedef ::TQGridScanner current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQGridScanner

//______________________________________________________________________________
void TQHWWPlotter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQHWWPlotter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQHWWPlotter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQHWWPlotter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQHWWPlotter(void *p) {
      return  p ? new(p) ::TQHWWPlotter : new ::TQHWWPlotter;
   }
   static void *newArray_TQHWWPlotter(Long_t nElements, void *p) {
      return p ? new(p) ::TQHWWPlotter[nElements] : new ::TQHWWPlotter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQHWWPlotter(void *p) {
      delete ((::TQHWWPlotter*)p);
   }
   static void deleteArray_TQHWWPlotter(void *p) {
      delete [] ((::TQHWWPlotter*)p);
   }
   static void destruct_TQHWWPlotter(void *p) {
      typedef ::TQHWWPlotter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQHWWPlotter

//______________________________________________________________________________
void TQHistComparer::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQHistComparer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQHistComparer::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQHistComparer::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQHistComparer(void *p) {
      delete ((::TQHistComparer*)p);
   }
   static void deleteArray_TQHistComparer(void *p) {
      delete [] ((::TQHistComparer*)p);
   }
   static void destruct_TQHistComparer(void *p) {
      typedef ::TQHistComparer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQHistComparer

//______________________________________________________________________________
void TQHistoMakerAnalysisJob::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQHistoMakerAnalysisJob.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQHistoMakerAnalysisJob::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQHistoMakerAnalysisJob::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQHistoMakerAnalysisJob(void *p) {
      return  p ? new(p) ::TQHistoMakerAnalysisJob : new ::TQHistoMakerAnalysisJob;
   }
   static void *newArray_TQHistoMakerAnalysisJob(Long_t nElements, void *p) {
      return p ? new(p) ::TQHistoMakerAnalysisJob[nElements] : new ::TQHistoMakerAnalysisJob[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQHistoMakerAnalysisJob(void *p) {
      delete ((::TQHistoMakerAnalysisJob*)p);
   }
   static void deleteArray_TQHistoMakerAnalysisJob(void *p) {
      delete [] ((::TQHistoMakerAnalysisJob*)p);
   }
   static void destruct_TQHistoMakerAnalysisJob(void *p) {
      typedef ::TQHistoMakerAnalysisJob current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQHistoMakerAnalysisJob

//______________________________________________________________________________
template <> void TQHistogramObservable<TH1>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQHistogramObservable<TH1>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQHistogramObservable<TH1>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQHistogramObservable<TH1>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQHistogramObservablelETH1gR(void *p) {
      return  p ? new(p) ::TQHistogramObservable<TH1> : new ::TQHistogramObservable<TH1>;
   }
   static void *newArray_TQHistogramObservablelETH1gR(Long_t nElements, void *p) {
      return p ? new(p) ::TQHistogramObservable<TH1>[nElements] : new ::TQHistogramObservable<TH1>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQHistogramObservablelETH1gR(void *p) {
      delete ((::TQHistogramObservable<TH1>*)p);
   }
   static void deleteArray_TQHistogramObservablelETH1gR(void *p) {
      delete [] ((::TQHistogramObservable<TH1>*)p);
   }
   static void destruct_TQHistogramObservablelETH1gR(void *p) {
      typedef ::TQHistogramObservable<TH1> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQHistogramObservable<TH1>

//______________________________________________________________________________
template <> void TQHistogramObservable<TH2>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQHistogramObservable<TH2>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQHistogramObservable<TH2>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQHistogramObservable<TH2>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQHistogramObservablelETH2gR(void *p) {
      return  p ? new(p) ::TQHistogramObservable<TH2> : new ::TQHistogramObservable<TH2>;
   }
   static void *newArray_TQHistogramObservablelETH2gR(Long_t nElements, void *p) {
      return p ? new(p) ::TQHistogramObservable<TH2>[nElements] : new ::TQHistogramObservable<TH2>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQHistogramObservablelETH2gR(void *p) {
      delete ((::TQHistogramObservable<TH2>*)p);
   }
   static void deleteArray_TQHistogramObservablelETH2gR(void *p) {
      delete [] ((::TQHistogramObservable<TH2>*)p);
   }
   static void destruct_TQHistogramObservablelETH2gR(void *p) {
      typedef ::TQHistogramObservable<TH2> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQHistogramObservable<TH2>

//______________________________________________________________________________
template <> void TQHistogramObservable<TH3>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQHistogramObservable<TH3>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQHistogramObservable<TH3>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQHistogramObservable<TH3>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQHistogramObservablelETH3gR(void *p) {
      return  p ? new(p) ::TQHistogramObservable<TH3> : new ::TQHistogramObservable<TH3>;
   }
   static void *newArray_TQHistogramObservablelETH3gR(Long_t nElements, void *p) {
      return p ? new(p) ::TQHistogramObservable<TH3>[nElements] : new ::TQHistogramObservable<TH3>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQHistogramObservablelETH3gR(void *p) {
      delete ((::TQHistogramObservable<TH3>*)p);
   }
   static void deleteArray_TQHistogramObservablelETH3gR(void *p) {
      delete [] ((::TQHistogramObservable<TH3>*)p);
   }
   static void destruct_TQHistogramObservablelETH3gR(void *p) {
      typedef ::TQHistogramObservable<TH3> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQHistogramObservable<TH3>

//______________________________________________________________________________
void TQLink::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQLink.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQLink::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQLink::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQLink(void *p) {
      delete ((::TQLink*)p);
   }
   static void deleteArray_TQLink(void *p) {
      delete [] ((::TQLink*)p);
   }
   static void destruct_TQLink(void *p) {
      typedef ::TQLink current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQLink

//______________________________________________________________________________
void TQImportLink::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQImportLink.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQImportLink::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQImportLink::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQImportLink(void *p) {
      return  p ? new(p) ::TQImportLink : new ::TQImportLink;
   }
   static void *newArray_TQImportLink(Long_t nElements, void *p) {
      return p ? new(p) ::TQImportLink[nElements] : new ::TQImportLink[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQImportLink(void *p) {
      delete ((::TQImportLink*)p);
   }
   static void deleteArray_TQImportLink(void *p) {
      delete [] ((::TQImportLink*)p);
   }
   static void destruct_TQImportLink(void *p) {
      typedef ::TQImportLink current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQImportLink

//______________________________________________________________________________
void TQLibrary::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQLibrary.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQLibrary::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQLibrary::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQLibrary(void *p) {
      return  p ? new(p) ::TQLibrary : new ::TQLibrary;
   }
   static void *newArray_TQLibrary(Long_t nElements, void *p) {
      return p ? new(p) ::TQLibrary[nElements] : new ::TQLibrary[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQLibrary(void *p) {
      delete ((::TQLibrary*)p);
   }
   static void deleteArray_TQLibrary(void *p) {
      delete [] ((::TQLibrary*)p);
   }
   static void destruct_TQLibrary(void *p) {
      typedef ::TQLibrary current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQLibrary

//______________________________________________________________________________
void TQToken::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQToken.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQToken::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQToken::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQToken(void *p) {
      return  p ? new(p) ::TQToken : new ::TQToken;
   }
   static void *newArray_TQToken(Long_t nElements, void *p) {
      return p ? new(p) ::TQToken[nElements] : new ::TQToken[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQToken(void *p) {
      delete ((::TQToken*)p);
   }
   static void deleteArray_TQToken(void *p) {
      delete [] ((::TQToken*)p);
   }
   static void destruct_TQToken(void *p) {
      typedef ::TQToken current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQToken

//______________________________________________________________________________
void TQMVA::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQMVA.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQMVA::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQMVA::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQMVA(void *p) {
      return  p ? new(p) ::TQMVA : new ::TQMVA;
   }
   static void *newArray_TQMVA(Long_t nElements, void *p) {
      return p ? new(p) ::TQMVA[nElements] : new ::TQMVA[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQMVA(void *p) {
      delete ((::TQMVA*)p);
   }
   static void deleteArray_TQMVA(void *p) {
      delete [] ((::TQMVA*)p);
   }
   static void destruct_TQMVA(void *p) {
      typedef ::TQMVA current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQMVA

//______________________________________________________________________________
void TQMVAObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQMVAObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQMVAObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQMVAObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQMVAObservable(void *p) {
      return  p ? new(p) ::TQMVAObservable : new ::TQMVAObservable;
   }
   static void *newArray_TQMVAObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQMVAObservable[nElements] : new ::TQMVAObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQMVAObservable(void *p) {
      delete ((::TQMVAObservable*)p);
   }
   static void deleteArray_TQMVAObservable(void *p) {
      delete [] ((::TQMVAObservable*)p);
   }
   static void destruct_TQMVAObservable(void *p) {
      typedef ::TQMVAObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQMVAObservable

//______________________________________________________________________________
void TQMultiChannelAnalysisSampleVisitor::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQMultiChannelAnalysisSampleVisitor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQMultiChannelAnalysisSampleVisitor::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQMultiChannelAnalysisSampleVisitor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQMultiChannelAnalysisSampleVisitor(void *p) {
      return  p ? new(p) ::TQMultiChannelAnalysisSampleVisitor : new ::TQMultiChannelAnalysisSampleVisitor;
   }
   static void *newArray_TQMultiChannelAnalysisSampleVisitor(Long_t nElements, void *p) {
      return p ? new(p) ::TQMultiChannelAnalysisSampleVisitor[nElements] : new ::TQMultiChannelAnalysisSampleVisitor[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQMultiChannelAnalysisSampleVisitor(void *p) {
      delete ((::TQMultiChannelAnalysisSampleVisitor*)p);
   }
   static void deleteArray_TQMultiChannelAnalysisSampleVisitor(void *p) {
      delete [] ((::TQMultiChannelAnalysisSampleVisitor*)p);
   }
   static void destruct_TQMultiChannelAnalysisSampleVisitor(void *p) {
      typedef ::TQMultiChannelAnalysisSampleVisitor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQMultiChannelAnalysisSampleVisitor

//______________________________________________________________________________
void TQMultiObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQMultiObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQMultiObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQMultiObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQMultiObservable(void *p) {
      return  p ? new(p) ::TQMultiObservable : new ::TQMultiObservable;
   }
   static void *newArray_TQMultiObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQMultiObservable[nElements] : new ::TQMultiObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQMultiObservable(void *p) {
      delete ((::TQMultiObservable*)p);
   }
   static void deleteArray_TQMultiObservable(void *p) {
      delete [] ((::TQMultiObservable*)p);
   }
   static void destruct_TQMultiObservable(void *p) {
      typedef ::TQMultiObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQMultiObservable

//______________________________________________________________________________
void TQNFCalculator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQNFCalculator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQNFCalculator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQNFCalculator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQNFCalculator(void *p) {
      return  p ? new(p) ::TQNFCalculator : new ::TQNFCalculator;
   }
   static void *newArray_TQNFCalculator(Long_t nElements, void *p) {
      return p ? new(p) ::TQNFCalculator[nElements] : new ::TQNFCalculator[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQNFCalculator(void *p) {
      delete ((::TQNFCalculator*)p);
   }
   static void deleteArray_TQNFCalculator(void *p) {
      delete [] ((::TQNFCalculator*)p);
   }
   static void destruct_TQNFCalculator(void *p) {
      typedef ::TQNFCalculator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQNFCalculator

//______________________________________________________________________________
void TQNFChainloader::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQNFChainloader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQNFChainloader::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQNFChainloader::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQNFChainloader(void *p) {
      return  p ? new(p) ::TQNFChainloader : new ::TQNFChainloader;
   }
   static void *newArray_TQNFChainloader(Long_t nElements, void *p) {
      return p ? new(p) ::TQNFChainloader[nElements] : new ::TQNFChainloader[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQNFChainloader(void *p) {
      delete ((::TQNFChainloader*)p);
   }
   static void deleteArray_TQNFChainloader(void *p) {
      delete [] ((::TQNFChainloader*)p);
   }
   static void destruct_TQNFChainloader(void *p) {
      typedef ::TQNFChainloader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQNFChainloader

//______________________________________________________________________________
void TQNFCustomCalculator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQNFCustomCalculator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQNFCustomCalculator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQNFCustomCalculator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQNFCustomCalculator(void *p) {
      return  p ? new(p) ::TQNFCustomCalculator : new ::TQNFCustomCalculator;
   }
   static void *newArray_TQNFCustomCalculator(Long_t nElements, void *p) {
      return p ? new(p) ::TQNFCustomCalculator[nElements] : new ::TQNFCustomCalculator[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQNFCustomCalculator(void *p) {
      delete ((::TQNFCustomCalculator*)p);
   }
   static void deleteArray_TQNFCustomCalculator(void *p) {
      delete [] ((::TQNFCustomCalculator*)p);
   }
   static void destruct_TQNFCustomCalculator(void *p) {
      typedef ::TQNFCustomCalculator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQNFCustomCalculator

//______________________________________________________________________________
void TQNFManualSetter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQNFManualSetter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQNFManualSetter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQNFManualSetter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQNFManualSetter(void *p) {
      return  p ? new(p) ::TQNFManualSetter : new ::TQNFManualSetter;
   }
   static void *newArray_TQNFManualSetter(Long_t nElements, void *p) {
      return p ? new(p) ::TQNFManualSetter[nElements] : new ::TQNFManualSetter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQNFManualSetter(void *p) {
      delete ((::TQNFManualSetter*)p);
   }
   static void deleteArray_TQNFManualSetter(void *p) {
      delete [] ((::TQNFManualSetter*)p);
   }
   static void destruct_TQNFManualSetter(void *p) {
      typedef ::TQNFManualSetter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQNFManualSetter

//______________________________________________________________________________
void TQNFTop0jetEstimator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQNFTop0jetEstimator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQNFTop0jetEstimator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQNFTop0jetEstimator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQNFTop0jetEstimator(void *p) {
      return  p ? new(p) ::TQNFTop0jetEstimator : new ::TQNFTop0jetEstimator;
   }
   static void *newArray_TQNFTop0jetEstimator(Long_t nElements, void *p) {
      return p ? new(p) ::TQNFTop0jetEstimator[nElements] : new ::TQNFTop0jetEstimator[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQNFTop0jetEstimator(void *p) {
      delete ((::TQNFTop0jetEstimator*)p);
   }
   static void deleteArray_TQNFTop0jetEstimator(void *p) {
      delete [] ((::TQNFTop0jetEstimator*)p);
   }
   static void destruct_TQNFTop0jetEstimator(void *p) {
      typedef ::TQNFTop0jetEstimator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQNFTop0jetEstimator

//______________________________________________________________________________
void TQNFTop1jetEstimator::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQNFTop1jetEstimator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQNFTop1jetEstimator::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQNFTop1jetEstimator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQNFTop1jetEstimator(void *p) {
      return  p ? new(p) ::TQNFTop1jetEstimator : new ::TQNFTop1jetEstimator;
   }
   static void *newArray_TQNFTop1jetEstimator(Long_t nElements, void *p) {
      return p ? new(p) ::TQNFTop1jetEstimator[nElements] : new ::TQNFTop1jetEstimator[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQNFTop1jetEstimator(void *p) {
      delete ((::TQNFTop1jetEstimator*)p);
   }
   static void deleteArray_TQNFTop1jetEstimator(void *p) {
      delete [] ((::TQNFTop1jetEstimator*)p);
   }
   static void destruct_TQNFTop1jetEstimator(void *p) {
      typedef ::TQNFTop1jetEstimator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQNFTop1jetEstimator

//______________________________________________________________________________
void TQNFUncertaintyScaler::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQNFUncertaintyScaler.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQNFUncertaintyScaler::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQNFUncertaintyScaler::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQNFUncertaintyScaler(void *p) {
      return  p ? new(p) ::TQNFUncertaintyScaler : new ::TQNFUncertaintyScaler;
   }
   static void *newArray_TQNFUncertaintyScaler(Long_t nElements, void *p) {
      return p ? new(p) ::TQNFUncertaintyScaler[nElements] : new ::TQNFUncertaintyScaler[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQNFUncertaintyScaler(void *p) {
      delete ((::TQNFUncertaintyScaler*)p);
   }
   static void deleteArray_TQNFUncertaintyScaler(void *p) {
      delete [] ((::TQNFUncertaintyScaler*)p);
   }
   static void destruct_TQNFUncertaintyScaler(void *p) {
      typedef ::TQNFUncertaintyScaler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQNFUncertaintyScaler

//______________________________________________________________________________
void TQNTupleDumperAnalysisJob::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQNTupleDumperAnalysisJob.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQNTupleDumperAnalysisJob::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQNTupleDumperAnalysisJob::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQNTupleDumperAnalysisJob(void *p) {
      delete ((::TQNTupleDumperAnalysisJob*)p);
   }
   static void deleteArray_TQNTupleDumperAnalysisJob(void *p) {
      delete [] ((::TQNTupleDumperAnalysisJob*)p);
   }
   static void destruct_TQNTupleDumperAnalysisJob(void *p) {
      typedef ::TQNTupleDumperAnalysisJob current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQNTupleDumperAnalysisJob

//______________________________________________________________________________
void TQPCAAnalysisJob::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQPCAAnalysisJob.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQPCAAnalysisJob::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQPCAAnalysisJob::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQPCAAnalysisJob(void *p) {
      return  p ? new(p) ::TQPCAAnalysisJob : new ::TQPCAAnalysisJob;
   }
   static void *newArray_TQPCAAnalysisJob(Long_t nElements, void *p) {
      return p ? new(p) ::TQPCAAnalysisJob[nElements] : new ::TQPCAAnalysisJob[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQPCAAnalysisJob(void *p) {
      delete ((::TQPCAAnalysisJob*)p);
   }
   static void deleteArray_TQPCAAnalysisJob(void *p) {
      delete [] ((::TQPCAAnalysisJob*)p);
   }
   static void destruct_TQPCAAnalysisJob(void *p) {
      typedef ::TQPCAAnalysisJob current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQPCAAnalysisJob

//______________________________________________________________________________
void TQPathManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQPathManager.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQPathManager::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQPathManager::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQPathManager(void *p) {
      return  p ? new(p) ::TQPathManager : new ::TQPathManager;
   }
   static void *newArray_TQPathManager(Long_t nElements, void *p) {
      return p ? new(p) ::TQPathManager[nElements] : new ::TQPathManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQPathManager(void *p) {
      delete ((::TQPathManager*)p);
   }
   static void deleteArray_TQPathManager(void *p) {
      delete [] ((::TQPathManager*)p);
   }
   static void destruct_TQPathManager(void *p) {
      typedef ::TQPathManager current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQPathManager

//______________________________________________________________________________
void TQSampleGroupingVisitor::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSampleGroupingVisitor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSampleGroupingVisitor::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSampleGroupingVisitor::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQSampleGroupingVisitor(void *p) {
      delete ((::TQSampleGroupingVisitor*)p);
   }
   static void deleteArray_TQSampleGroupingVisitor(void *p) {
      delete [] ((::TQSampleGroupingVisitor*)p);
   }
   static void destruct_TQSampleGroupingVisitor(void *p) {
      typedef ::TQSampleGroupingVisitor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSampleGroupingVisitor

//______________________________________________________________________________
void TQSampleInitializerBase::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSampleInitializerBase.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSampleInitializerBase::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSampleInitializerBase::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQSampleInitializerBase(void *p) {
      delete ((::TQSampleInitializerBase*)p);
   }
   static void deleteArray_TQSampleInitializerBase(void *p) {
      delete [] ((::TQSampleInitializerBase*)p);
   }
   static void destruct_TQSampleInitializerBase(void *p) {
      typedef ::TQSampleInitializerBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSampleInitializerBase

//______________________________________________________________________________
void TQSampleInitializer::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSampleInitializer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSampleInitializer::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSampleInitializer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSampleInitializer(void *p) {
      return  p ? new(p) ::TQSampleInitializer : new ::TQSampleInitializer;
   }
   static void *newArray_TQSampleInitializer(Long_t nElements, void *p) {
      return p ? new(p) ::TQSampleInitializer[nElements] : new ::TQSampleInitializer[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSampleInitializer(void *p) {
      delete ((::TQSampleInitializer*)p);
   }
   static void deleteArray_TQSampleInitializer(void *p) {
      delete [] ((::TQSampleInitializer*)p);
   }
   static void destruct_TQSampleInitializer(void *p) {
      typedef ::TQSampleInitializer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSampleInitializer

//______________________________________________________________________________
void TQSampleNormalizationObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSampleNormalizationObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSampleNormalizationObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSampleNormalizationObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSampleNormalizationObservable(void *p) {
      return  p ? new(p) ::TQSampleNormalizationObservable : new ::TQSampleNormalizationObservable;
   }
   static void *newArray_TQSampleNormalizationObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQSampleNormalizationObservable[nElements] : new ::TQSampleNormalizationObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSampleNormalizationObservable(void *p) {
      delete ((::TQSampleNormalizationObservable*)p);
   }
   static void deleteArray_TQSampleNormalizationObservable(void *p) {
      delete [] ((::TQSampleNormalizationObservable*)p);
   }
   static void destruct_TQSampleNormalizationObservable(void *p) {
      typedef ::TQSampleNormalizationObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSampleNormalizationObservable

//______________________________________________________________________________
void TQSamplePurger::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSamplePurger.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSamplePurger::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSamplePurger::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQSamplePurger(void *p) {
      delete ((::TQSamplePurger*)p);
   }
   static void deleteArray_TQSamplePurger(void *p) {
      delete [] ((::TQSamplePurger*)p);
   }
   static void destruct_TQSamplePurger(void *p) {
      typedef ::TQSamplePurger current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSamplePurger

//______________________________________________________________________________
void TQSampleRevisitor::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSampleRevisitor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSampleRevisitor::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSampleRevisitor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSampleRevisitor(void *p) {
      return  p ? new(p) ::TQSampleRevisitor : new ::TQSampleRevisitor;
   }
   static void *newArray_TQSampleRevisitor(Long_t nElements, void *p) {
      return p ? new(p) ::TQSampleRevisitor[nElements] : new ::TQSampleRevisitor[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSampleRevisitor(void *p) {
      delete ((::TQSampleRevisitor*)p);
   }
   static void deleteArray_TQSampleRevisitor(void *p) {
      delete [] ((::TQSampleRevisitor*)p);
   }
   static void destruct_TQSampleRevisitor(void *p) {
      typedef ::TQSampleRevisitor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSampleRevisitor

//______________________________________________________________________________
void TQSystematicsManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSystematicsManager.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSystematicsManager::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSystematicsManager::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQSystematicsManager(void *p) {
      return  p ? new(p) ::TQSystematicsManager : new ::TQSystematicsManager;
   }
   static void *newArray_TQSystematicsManager(Long_t nElements, void *p) {
      return p ? new(p) ::TQSystematicsManager[nElements] : new ::TQSystematicsManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQSystematicsManager(void *p) {
      delete ((::TQSystematicsManager*)p);
   }
   static void deleteArray_TQSystematicsManager(void *p) {
      delete [] ((::TQSystematicsManager*)p);
   }
   static void destruct_TQSystematicsManager(void *p) {
      typedef ::TQSystematicsManager current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSystematicsManager

//______________________________________________________________________________
void TQSystematicsHandler::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQSystematicsHandler.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQSystematicsHandler::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQSystematicsHandler::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQSystematicsHandler(void *p) {
      delete ((::TQSystematicsHandler*)p);
   }
   static void deleteArray_TQSystematicsHandler(void *p) {
      delete [] ((::TQSystematicsHandler*)p);
   }
   static void destruct_TQSystematicsHandler(void *p) {
      typedef ::TQSystematicsHandler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQSystematicsHandler

//______________________________________________________________________________
void TQTHnBaseMakerAnalysisJob::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQTHnBaseMakerAnalysisJob.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQTHnBaseMakerAnalysisJob::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQTHnBaseMakerAnalysisJob::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQTHnBaseMakerAnalysisJob(void *p) {
      return  p ? new(p) ::TQTHnBaseMakerAnalysisJob : new ::TQTHnBaseMakerAnalysisJob;
   }
   static void *newArray_TQTHnBaseMakerAnalysisJob(Long_t nElements, void *p) {
      return p ? new(p) ::TQTHnBaseMakerAnalysisJob[nElements] : new ::TQTHnBaseMakerAnalysisJob[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQTHnBaseMakerAnalysisJob(void *p) {
      delete ((::TQTHnBaseMakerAnalysisJob*)p);
   }
   static void deleteArray_TQTHnBaseMakerAnalysisJob(void *p) {
      delete [] ((::TQTHnBaseMakerAnalysisJob*)p);
   }
   static void destruct_TQTHnBaseMakerAnalysisJob(void *p) {
      typedef ::TQTHnBaseMakerAnalysisJob current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQTHnBaseMakerAnalysisJob

//______________________________________________________________________________
void TQTikZPlotter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQTikZPlotter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQTikZPlotter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQTikZPlotter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TQTikZPlotter(void *p) {
      delete ((::TQTikZPlotter*)p);
   }
   static void deleteArray_TQTikZPlotter(void *p) {
      delete [] ((::TQTikZPlotter*)p);
   }
   static void destruct_TQTikZPlotter(void *p) {
      typedef ::TQTikZPlotter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQTikZPlotter

//______________________________________________________________________________
void TQTreeFormulaObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQTreeFormulaObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQTreeFormulaObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQTreeFormulaObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQTreeFormulaObservable(void *p) {
      return  p ? new(p) ::TQTreeFormulaObservable : new ::TQTreeFormulaObservable;
   }
   static void *newArray_TQTreeFormulaObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQTreeFormulaObservable[nElements] : new ::TQTreeFormulaObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQTreeFormulaObservable(void *p) {
      delete ((::TQTreeFormulaObservable*)p);
   }
   static void deleteArray_TQTreeFormulaObservable(void *p) {
      delete [] ((::TQTreeFormulaObservable*)p);
   }
   static void destruct_TQTreeFormulaObservable(void *p) {
      typedef ::TQTreeFormulaObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQTreeFormulaObservable

//______________________________________________________________________________
void TQUniqueCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQUniqueCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQUniqueCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQUniqueCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQUniqueCut(void *p) {
      return  p ? new(p) ::TQUniqueCut : new ::TQUniqueCut;
   }
   static void *newArray_TQUniqueCut(Long_t nElements, void *p) {
      return p ? new(p) ::TQUniqueCut[nElements] : new ::TQUniqueCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQUniqueCut(void *p) {
      delete ((::TQUniqueCut*)p);
   }
   static void deleteArray_TQUniqueCut(void *p) {
      delete [] ((::TQUniqueCut*)p);
   }
   static void destruct_TQUniqueCut(void *p) {
      typedef ::TQUniqueCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQUniqueCut

//______________________________________________________________________________
void TQVectorAuxObservable::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQVectorAuxObservable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQVectorAuxObservable::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQVectorAuxObservable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQVectorAuxObservable(void *p) {
      return  p ? new(p) ::TQVectorAuxObservable : new ::TQVectorAuxObservable;
   }
   static void *newArray_TQVectorAuxObservable(Long_t nElements, void *p) {
      return p ? new(p) ::TQVectorAuxObservable[nElements] : new ::TQVectorAuxObservable[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQVectorAuxObservable(void *p) {
      delete ((::TQVectorAuxObservable*)p);
   }
   static void deleteArray_TQVectorAuxObservable(void *p) {
      delete [] ((::TQVectorAuxObservable*)p);
   }
   static void destruct_TQVectorAuxObservable(void *p) {
      typedef ::TQVectorAuxObservable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQVectorAuxObservable

//______________________________________________________________________________
void TQXSecParser::Streamer(TBuffer &R__b)
{
   // Stream an object of class TQXSecParser.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TQXSecParser::Class(),this);
   } else {
      R__b.WriteClassBuffer(TQXSecParser::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TQXSecParser(void *p) {
      return  p ? new(p) ::TQXSecParser : new ::TQXSecParser;
   }
   static void *newArray_TQXSecParser(Long_t nElements, void *p) {
      return p ? new(p) ::TQXSecParser[nElements] : new ::TQXSecParser[nElements];
   }
   // Wrapper around operator delete
   static void delete_TQXSecParser(void *p) {
      delete ((::TQXSecParser*)p);
   }
   static void deleteArray_TQXSecParser(void *p) {
      delete [] ((::TQXSecParser*)p);
   }
   static void destruct_TQXSecParser(void *p) {
      typedef ::TQXSecParser current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TQXSecParser

namespace ROOT {
   static TClass *vectorlEpairlEintcOintgRsPgR_Dictionary();
   static void vectorlEpairlEintcOintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEintcOintgRsPgR(void *p = 0);
   static void *newArray_vectorlEpairlEintcOintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEintcOintgRsPgR(void *p);
   static void deleteArray_vectorlEpairlEintcOintgRsPgR(void *p);
   static void destruct_vectorlEpairlEintcOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<int,int> >*)
   {
      vector<pair<int,int> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<int,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<int,int> >", -2, "vector", 339,
                  typeid(vector<pair<int,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlEintcOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<int,int> >) );
      instance.SetNew(&new_vectorlEpairlEintcOintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEintcOintgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEintcOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEintcOintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEintcOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<int,int> > >()));

      ::ROOT::AddClassAlternate("vector<pair<int,int> >","std::vector<std::pair<int, int>, std::allocator<std::pair<int, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<int,int> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEintcOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<int,int> >*)0x0)->GetClass();
      vectorlEpairlEintcOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEintcOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEintcOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<int,int> > : new vector<pair<int,int> >;
   }
   static void *newArray_vectorlEpairlEintcOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<int,int> >[nElements] : new vector<pair<int,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEintcOintgRsPgR(void *p) {
      delete ((vector<pair<int,int> >*)p);
   }
   static void deleteArray_vectorlEpairlEintcOintgRsPgR(void *p) {
      delete [] ((vector<pair<int,int> >*)p);
   }
   static void destruct_vectorlEpairlEintcOintgRsPgR(void *p) {
      typedef vector<pair<int,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<int,int> >

namespace ROOT {
   static TClass *vectorlEpairlEdoublecOdoublegRsPgR_Dictionary();
   static void vectorlEpairlEdoublecOdoublegRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEdoublecOdoublegRsPgR(void *p = 0);
   static void *newArray_vectorlEpairlEdoublecOdoublegRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEdoublecOdoublegRsPgR(void *p);
   static void deleteArray_vectorlEpairlEdoublecOdoublegRsPgR(void *p);
   static void destruct_vectorlEpairlEdoublecOdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<double,double> >*)
   {
      vector<pair<double,double> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<double,double> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<double,double> >", -2, "vector", 339,
                  typeid(vector<pair<double,double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlEdoublecOdoublegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<double,double> >) );
      instance.SetNew(&new_vectorlEpairlEdoublecOdoublegRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEdoublecOdoublegRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEdoublecOdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEdoublecOdoublegRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEdoublecOdoublegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<double,double> > >()));

      ::ROOT::AddClassAlternate("vector<pair<double,double> >","std::vector<std::pair<double, double>, std::allocator<std::pair<double, double> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<double,double> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEdoublecOdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<double,double> >*)0x0)->GetClass();
      vectorlEpairlEdoublecOdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEdoublecOdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEdoublecOdoublegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<double,double> > : new vector<pair<double,double> >;
   }
   static void *newArray_vectorlEpairlEdoublecOdoublegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<double,double> >[nElements] : new vector<pair<double,double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEdoublecOdoublegRsPgR(void *p) {
      delete ((vector<pair<double,double> >*)p);
   }
   static void deleteArray_vectorlEpairlEdoublecOdoublegRsPgR(void *p) {
      delete [] ((vector<pair<double,double> >*)p);
   }
   static void destruct_vectorlEpairlEdoublecOdoublegRsPgR(void *p) {
      typedef vector<pair<double,double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<double,double> >

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
   static TClass *vectorlETStringgR_Dictionary();
   static void vectorlETStringgR_TClassManip(TClass*);
   static void *new_vectorlETStringgR(void *p = 0);
   static void *newArray_vectorlETStringgR(Long_t size, void *p);
   static void delete_vectorlETStringgR(void *p);
   static void deleteArray_vectorlETStringgR(void *p);
   static void destruct_vectorlETStringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TString>*)
   {
      vector<TString> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TString>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TString>", -2, "vector", 339,
                  typeid(vector<TString>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETStringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TString>) );
      instance.SetNew(&new_vectorlETStringgR);
      instance.SetNewArray(&newArray_vectorlETStringgR);
      instance.SetDelete(&delete_vectorlETStringgR);
      instance.SetDeleteArray(&deleteArray_vectorlETStringgR);
      instance.SetDestructor(&destruct_vectorlETStringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TString> >()));

      ::ROOT::AddClassAlternate("vector<TString>","std::vector<TString, std::allocator<TString> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TString>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETStringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TString>*)0x0)->GetClass();
      vectorlETStringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETStringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETStringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TString> : new vector<TString>;
   }
   static void *newArray_vectorlETStringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TString>[nElements] : new vector<TString>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETStringgR(void *p) {
      delete ((vector<TString>*)p);
   }
   static void deleteArray_vectorlETStringgR(void *p) {
      delete [] ((vector<TString>*)p);
   }
   static void destruct_vectorlETStringgR(void *p) {
      typedef vector<TString> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TString>

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
   static TClass *vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR_Dictionary();
   static void vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR_TClassManip(TClass*);
   static void *new_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR(void *p = 0);
   static void *newArray_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR(Long_t size, void *p);
   static void delete_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR(void *p);
   static void deleteArray_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR(void *p);
   static void destruct_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TQNTupleDumperAnalysisJob::VarType>*)
   {
      vector<TQNTupleDumperAnalysisJob::VarType> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TQNTupleDumperAnalysisJob::VarType>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TQNTupleDumperAnalysisJob::VarType>", -2, "vector", 339,
                  typeid(vector<TQNTupleDumperAnalysisJob::VarType>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TQNTupleDumperAnalysisJob::VarType>) );
      instance.SetNew(&new_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR);
      instance.SetNewArray(&newArray_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR);
      instance.SetDelete(&delete_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR);
      instance.SetDeleteArray(&deleteArray_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR);
      instance.SetDestructor(&destruct_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TQNTupleDumperAnalysisJob::VarType> >()));

      ::ROOT::AddClassAlternate("vector<TQNTupleDumperAnalysisJob::VarType>","std::vector<TQNTupleDumperAnalysisJob::VarType, std::allocator<TQNTupleDumperAnalysisJob::VarType> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TQNTupleDumperAnalysisJob::VarType>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TQNTupleDumperAnalysisJob::VarType>*)0x0)->GetClass();
      vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQNTupleDumperAnalysisJob::VarType> : new vector<TQNTupleDumperAnalysisJob::VarType>;
   }
   static void *newArray_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQNTupleDumperAnalysisJob::VarType>[nElements] : new vector<TQNTupleDumperAnalysisJob::VarType>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR(void *p) {
      delete ((vector<TQNTupleDumperAnalysisJob::VarType>*)p);
   }
   static void deleteArray_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR(void *p) {
      delete [] ((vector<TQNTupleDumperAnalysisJob::VarType>*)p);
   }
   static void destruct_vectorlETQNTupleDumperAnalysisJobcLcLVarTypegR(void *p) {
      typedef vector<TQNTupleDumperAnalysisJob::VarType> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TQNTupleDumperAnalysisJob::VarType>

namespace ROOT {
   static TClass *vectorlETQNFCalculatorcLcLMethodgR_Dictionary();
   static void vectorlETQNFCalculatorcLcLMethodgR_TClassManip(TClass*);
   static void *new_vectorlETQNFCalculatorcLcLMethodgR(void *p = 0);
   static void *newArray_vectorlETQNFCalculatorcLcLMethodgR(Long_t size, void *p);
   static void delete_vectorlETQNFCalculatorcLcLMethodgR(void *p);
   static void deleteArray_vectorlETQNFCalculatorcLcLMethodgR(void *p);
   static void destruct_vectorlETQNFCalculatorcLcLMethodgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TQNFCalculator::Method>*)
   {
      vector<TQNFCalculator::Method> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TQNFCalculator::Method>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TQNFCalculator::Method>", -2, "vector", 339,
                  typeid(vector<TQNFCalculator::Method>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETQNFCalculatorcLcLMethodgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TQNFCalculator::Method>) );
      instance.SetNew(&new_vectorlETQNFCalculatorcLcLMethodgR);
      instance.SetNewArray(&newArray_vectorlETQNFCalculatorcLcLMethodgR);
      instance.SetDelete(&delete_vectorlETQNFCalculatorcLcLMethodgR);
      instance.SetDeleteArray(&deleteArray_vectorlETQNFCalculatorcLcLMethodgR);
      instance.SetDestructor(&destruct_vectorlETQNFCalculatorcLcLMethodgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TQNFCalculator::Method> >()));

      ::ROOT::AddClassAlternate("vector<TQNFCalculator::Method>","std::vector<TQNFCalculator::Method, std::allocator<TQNFCalculator::Method> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TQNFCalculator::Method>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETQNFCalculatorcLcLMethodgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TQNFCalculator::Method>*)0x0)->GetClass();
      vectorlETQNFCalculatorcLcLMethodgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETQNFCalculatorcLcLMethodgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETQNFCalculatorcLcLMethodgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQNFCalculator::Method> : new vector<TQNFCalculator::Method>;
   }
   static void *newArray_vectorlETQNFCalculatorcLcLMethodgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQNFCalculator::Method>[nElements] : new vector<TQNFCalculator::Method>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETQNFCalculatorcLcLMethodgR(void *p) {
      delete ((vector<TQNFCalculator::Method>*)p);
   }
   static void deleteArray_vectorlETQNFCalculatorcLcLMethodgR(void *p) {
      delete [] ((vector<TQNFCalculator::Method>*)p);
   }
   static void destruct_vectorlETQNFCalculatorcLcLMethodgR(void *p) {
      typedef vector<TQNFCalculator::Method> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TQNFCalculator::Method>

namespace ROOT {
   static TClass *vectorlETQGridScanSplitObservablemUgR_Dictionary();
   static void vectorlETQGridScanSplitObservablemUgR_TClassManip(TClass*);
   static void *new_vectorlETQGridScanSplitObservablemUgR(void *p = 0);
   static void *newArray_vectorlETQGridScanSplitObservablemUgR(Long_t size, void *p);
   static void delete_vectorlETQGridScanSplitObservablemUgR(void *p);
   static void deleteArray_vectorlETQGridScanSplitObservablemUgR(void *p);
   static void destruct_vectorlETQGridScanSplitObservablemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TQGridScanSplitObservable*>*)
   {
      vector<TQGridScanSplitObservable*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TQGridScanSplitObservable*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TQGridScanSplitObservable*>", -2, "vector", 339,
                  typeid(vector<TQGridScanSplitObservable*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETQGridScanSplitObservablemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TQGridScanSplitObservable*>) );
      instance.SetNew(&new_vectorlETQGridScanSplitObservablemUgR);
      instance.SetNewArray(&newArray_vectorlETQGridScanSplitObservablemUgR);
      instance.SetDelete(&delete_vectorlETQGridScanSplitObservablemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETQGridScanSplitObservablemUgR);
      instance.SetDestructor(&destruct_vectorlETQGridScanSplitObservablemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TQGridScanSplitObservable*> >()));

      ::ROOT::AddClassAlternate("vector<TQGridScanSplitObservable*>","std::vector<TQGridScanSplitObservable*, std::allocator<TQGridScanSplitObservable*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TQGridScanSplitObservable*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETQGridScanSplitObservablemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TQGridScanSplitObservable*>*)0x0)->GetClass();
      vectorlETQGridScanSplitObservablemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETQGridScanSplitObservablemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETQGridScanSplitObservablemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQGridScanSplitObservable*> : new vector<TQGridScanSplitObservable*>;
   }
   static void *newArray_vectorlETQGridScanSplitObservablemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQGridScanSplitObservable*>[nElements] : new vector<TQGridScanSplitObservable*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETQGridScanSplitObservablemUgR(void *p) {
      delete ((vector<TQGridScanSplitObservable*>*)p);
   }
   static void deleteArray_vectorlETQGridScanSplitObservablemUgR(void *p) {
      delete [] ((vector<TQGridScanSplitObservable*>*)p);
   }
   static void destruct_vectorlETQGridScanSplitObservablemUgR(void *p) {
      typedef vector<TQGridScanSplitObservable*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TQGridScanSplitObservable*>

namespace ROOT {
   static TClass *vectorlETQGridScanResultscLcLInputHistsgR_Dictionary();
   static void vectorlETQGridScanResultscLcLInputHistsgR_TClassManip(TClass*);
   static void *new_vectorlETQGridScanResultscLcLInputHistsgR(void *p = 0);
   static void *newArray_vectorlETQGridScanResultscLcLInputHistsgR(Long_t size, void *p);
   static void delete_vectorlETQGridScanResultscLcLInputHistsgR(void *p);
   static void deleteArray_vectorlETQGridScanResultscLcLInputHistsgR(void *p);
   static void destruct_vectorlETQGridScanResultscLcLInputHistsgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TQGridScanResults::InputHists>*)
   {
      vector<TQGridScanResults::InputHists> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TQGridScanResults::InputHists>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TQGridScanResults::InputHists>", -2, "vector", 339,
                  typeid(vector<TQGridScanResults::InputHists>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETQGridScanResultscLcLInputHistsgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TQGridScanResults::InputHists>) );
      instance.SetNew(&new_vectorlETQGridScanResultscLcLInputHistsgR);
      instance.SetNewArray(&newArray_vectorlETQGridScanResultscLcLInputHistsgR);
      instance.SetDelete(&delete_vectorlETQGridScanResultscLcLInputHistsgR);
      instance.SetDeleteArray(&deleteArray_vectorlETQGridScanResultscLcLInputHistsgR);
      instance.SetDestructor(&destruct_vectorlETQGridScanResultscLcLInputHistsgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TQGridScanResults::InputHists> >()));

      ::ROOT::AddClassAlternate("vector<TQGridScanResults::InputHists>","std::vector<TQGridScanResults::InputHists, std::allocator<TQGridScanResults::InputHists> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TQGridScanResults::InputHists>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETQGridScanResultscLcLInputHistsgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TQGridScanResults::InputHists>*)0x0)->GetClass();
      vectorlETQGridScanResultscLcLInputHistsgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETQGridScanResultscLcLInputHistsgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETQGridScanResultscLcLInputHistsgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQGridScanResults::InputHists> : new vector<TQGridScanResults::InputHists>;
   }
   static void *newArray_vectorlETQGridScanResultscLcLInputHistsgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQGridScanResults::InputHists>[nElements] : new vector<TQGridScanResults::InputHists>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETQGridScanResultscLcLInputHistsgR(void *p) {
      delete ((vector<TQGridScanResults::InputHists>*)p);
   }
   static void deleteArray_vectorlETQGridScanResultscLcLInputHistsgR(void *p) {
      delete [] ((vector<TQGridScanResults::InputHists>*)p);
   }
   static void destruct_vectorlETQGridScanResultscLcLInputHistsgR(void *p) {
      typedef vector<TQGridScanResults::InputHists> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TQGridScanResults::InputHists>

namespace ROOT {
   static TClass *vectorlETQGridScanPointgR_Dictionary();
   static void vectorlETQGridScanPointgR_TClassManip(TClass*);
   static void *new_vectorlETQGridScanPointgR(void *p = 0);
   static void *newArray_vectorlETQGridScanPointgR(Long_t size, void *p);
   static void delete_vectorlETQGridScanPointgR(void *p);
   static void deleteArray_vectorlETQGridScanPointgR(void *p);
   static void destruct_vectorlETQGridScanPointgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TQGridScanPoint>*)
   {
      vector<TQGridScanPoint> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TQGridScanPoint>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TQGridScanPoint>", -2, "vector", 339,
                  typeid(vector<TQGridScanPoint>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETQGridScanPointgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TQGridScanPoint>) );
      instance.SetNew(&new_vectorlETQGridScanPointgR);
      instance.SetNewArray(&newArray_vectorlETQGridScanPointgR);
      instance.SetDelete(&delete_vectorlETQGridScanPointgR);
      instance.SetDeleteArray(&deleteArray_vectorlETQGridScanPointgR);
      instance.SetDestructor(&destruct_vectorlETQGridScanPointgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TQGridScanPoint> >()));

      ::ROOT::AddClassAlternate("vector<TQGridScanPoint>","std::vector<TQGridScanPoint, std::allocator<TQGridScanPoint> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TQGridScanPoint>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETQGridScanPointgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TQGridScanPoint>*)0x0)->GetClass();
      vectorlETQGridScanPointgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETQGridScanPointgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETQGridScanPointgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQGridScanPoint> : new vector<TQGridScanPoint>;
   }
   static void *newArray_vectorlETQGridScanPointgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQGridScanPoint>[nElements] : new vector<TQGridScanPoint>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETQGridScanPointgR(void *p) {
      delete ((vector<TQGridScanPoint>*)p);
   }
   static void deleteArray_vectorlETQGridScanPointgR(void *p) {
      delete [] ((vector<TQGridScanPoint>*)p);
   }
   static void destruct_vectorlETQGridScanPointgR(void *p) {
      typedef vector<TQGridScanPoint> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TQGridScanPoint>

namespace ROOT {
   static TClass *vectorlETQGridScanObservablemUgR_Dictionary();
   static void vectorlETQGridScanObservablemUgR_TClassManip(TClass*);
   static void *new_vectorlETQGridScanObservablemUgR(void *p = 0);
   static void *newArray_vectorlETQGridScanObservablemUgR(Long_t size, void *p);
   static void delete_vectorlETQGridScanObservablemUgR(void *p);
   static void deleteArray_vectorlETQGridScanObservablemUgR(void *p);
   static void destruct_vectorlETQGridScanObservablemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TQGridScanObservable*>*)
   {
      vector<TQGridScanObservable*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TQGridScanObservable*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TQGridScanObservable*>", -2, "vector", 339,
                  typeid(vector<TQGridScanObservable*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETQGridScanObservablemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TQGridScanObservable*>) );
      instance.SetNew(&new_vectorlETQGridScanObservablemUgR);
      instance.SetNewArray(&newArray_vectorlETQGridScanObservablemUgR);
      instance.SetDelete(&delete_vectorlETQGridScanObservablemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETQGridScanObservablemUgR);
      instance.SetDestructor(&destruct_vectorlETQGridScanObservablemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TQGridScanObservable*> >()));

      ::ROOT::AddClassAlternate("vector<TQGridScanObservable*>","std::vector<TQGridScanObservable*, std::allocator<TQGridScanObservable*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TQGridScanObservable*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETQGridScanObservablemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TQGridScanObservable*>*)0x0)->GetClass();
      vectorlETQGridScanObservablemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETQGridScanObservablemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETQGridScanObservablemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQGridScanObservable*> : new vector<TQGridScanObservable*>;
   }
   static void *newArray_vectorlETQGridScanObservablemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQGridScanObservable*>[nElements] : new vector<TQGridScanObservable*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETQGridScanObservablemUgR(void *p) {
      delete ((vector<TQGridScanObservable*>*)p);
   }
   static void deleteArray_vectorlETQGridScanObservablemUgR(void *p) {
      delete [] ((vector<TQGridScanObservable*>*)p);
   }
   static void destruct_vectorlETQGridScanObservablemUgR(void *p) {
      typedef vector<TQGridScanObservable*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TQGridScanObservable*>

namespace ROOT {
   static TClass *vectorlETQGridScanNormalObservablemUgR_Dictionary();
   static void vectorlETQGridScanNormalObservablemUgR_TClassManip(TClass*);
   static void *new_vectorlETQGridScanNormalObservablemUgR(void *p = 0);
   static void *newArray_vectorlETQGridScanNormalObservablemUgR(Long_t size, void *p);
   static void delete_vectorlETQGridScanNormalObservablemUgR(void *p);
   static void deleteArray_vectorlETQGridScanNormalObservablemUgR(void *p);
   static void destruct_vectorlETQGridScanNormalObservablemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TQGridScanNormalObservable*>*)
   {
      vector<TQGridScanNormalObservable*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TQGridScanNormalObservable*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TQGridScanNormalObservable*>", -2, "vector", 339,
                  typeid(vector<TQGridScanNormalObservable*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETQGridScanNormalObservablemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TQGridScanNormalObservable*>) );
      instance.SetNew(&new_vectorlETQGridScanNormalObservablemUgR);
      instance.SetNewArray(&newArray_vectorlETQGridScanNormalObservablemUgR);
      instance.SetDelete(&delete_vectorlETQGridScanNormalObservablemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETQGridScanNormalObservablemUgR);
      instance.SetDestructor(&destruct_vectorlETQGridScanNormalObservablemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TQGridScanNormalObservable*> >()));

      ::ROOT::AddClassAlternate("vector<TQGridScanNormalObservable*>","std::vector<TQGridScanNormalObservable*, std::allocator<TQGridScanNormalObservable*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TQGridScanNormalObservable*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETQGridScanNormalObservablemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TQGridScanNormalObservable*>*)0x0)->GetClass();
      vectorlETQGridScanNormalObservablemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETQGridScanNormalObservablemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETQGridScanNormalObservablemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQGridScanNormalObservable*> : new vector<TQGridScanNormalObservable*>;
   }
   static void *newArray_vectorlETQGridScanNormalObservablemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQGridScanNormalObservable*>[nElements] : new vector<TQGridScanNormalObservable*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETQGridScanNormalObservablemUgR(void *p) {
      delete ((vector<TQGridScanNormalObservable*>*)p);
   }
   static void deleteArray_vectorlETQGridScanNormalObservablemUgR(void *p) {
      delete [] ((vector<TQGridScanNormalObservable*>*)p);
   }
   static void destruct_vectorlETQGridScanNormalObservablemUgR(void *p) {
      typedef vector<TQGridScanNormalObservable*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TQGridScanNormalObservable*>

namespace ROOT {
   static TClass *vectorlETQCountermUgR_Dictionary();
   static void vectorlETQCountermUgR_TClassManip(TClass*);
   static void *new_vectorlETQCountermUgR(void *p = 0);
   static void *newArray_vectorlETQCountermUgR(Long_t size, void *p);
   static void delete_vectorlETQCountermUgR(void *p);
   static void deleteArray_vectorlETQCountermUgR(void *p);
   static void destruct_vectorlETQCountermUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TQCounter*>*)
   {
      vector<TQCounter*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TQCounter*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TQCounter*>", -2, "vector", 339,
                  typeid(vector<TQCounter*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETQCountermUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TQCounter*>) );
      instance.SetNew(&new_vectorlETQCountermUgR);
      instance.SetNewArray(&newArray_vectorlETQCountermUgR);
      instance.SetDelete(&delete_vectorlETQCountermUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETQCountermUgR);
      instance.SetDestructor(&destruct_vectorlETQCountermUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TQCounter*> >()));

      ::ROOT::AddClassAlternate("vector<TQCounter*>","std::vector<TQCounter*, std::allocator<TQCounter*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TQCounter*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETQCountermUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TQCounter*>*)0x0)->GetClass();
      vectorlETQCountermUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETQCountermUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETQCountermUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQCounter*> : new vector<TQCounter*>;
   }
   static void *newArray_vectorlETQCountermUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TQCounter*>[nElements] : new vector<TQCounter*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETQCountermUgR(void *p) {
      delete ((vector<TQCounter*>*)p);
   }
   static void deleteArray_vectorlETQCountermUgR(void *p) {
      delete [] ((vector<TQCounter*>*)p);
   }
   static void destruct_vectorlETQCountermUgR(void *p) {
      typedef vector<TQCounter*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TQCounter*>

namespace ROOT {
   static TClass *vectorlETHnBasemUgR_Dictionary();
   static void vectorlETHnBasemUgR_TClassManip(TClass*);
   static void *new_vectorlETHnBasemUgR(void *p = 0);
   static void *newArray_vectorlETHnBasemUgR(Long_t size, void *p);
   static void delete_vectorlETHnBasemUgR(void *p);
   static void deleteArray_vectorlETHnBasemUgR(void *p);
   static void destruct_vectorlETHnBasemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<THnBase*>*)
   {
      vector<THnBase*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<THnBase*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<THnBase*>", -2, "vector", 339,
                  typeid(vector<THnBase*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETHnBasemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<THnBase*>) );
      instance.SetNew(&new_vectorlETHnBasemUgR);
      instance.SetNewArray(&newArray_vectorlETHnBasemUgR);
      instance.SetDelete(&delete_vectorlETHnBasemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETHnBasemUgR);
      instance.SetDestructor(&destruct_vectorlETHnBasemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<THnBase*> >()));

      ::ROOT::AddClassAlternate("vector<THnBase*>","std::vector<THnBase*, std::allocator<THnBase*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<THnBase*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETHnBasemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<THnBase*>*)0x0)->GetClass();
      vectorlETHnBasemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETHnBasemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETHnBasemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<THnBase*> : new vector<THnBase*>;
   }
   static void *newArray_vectorlETHnBasemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<THnBase*>[nElements] : new vector<THnBase*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETHnBasemUgR(void *p) {
      delete ((vector<THnBase*>*)p);
   }
   static void deleteArray_vectorlETHnBasemUgR(void *p) {
      delete [] ((vector<THnBase*>*)p);
   }
   static void destruct_vectorlETHnBasemUgR(void *p) {
      typedef vector<THnBase*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<THnBase*>

namespace ROOT {
   static TClass *vectorlETH1FgR_Dictionary();
   static void vectorlETH1FgR_TClassManip(TClass*);
   static void *new_vectorlETH1FgR(void *p = 0);
   static void *newArray_vectorlETH1FgR(Long_t size, void *p);
   static void delete_vectorlETH1FgR(void *p);
   static void deleteArray_vectorlETH1FgR(void *p);
   static void destruct_vectorlETH1FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TH1F>*)
   {
      vector<TH1F> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TH1F>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TH1F>", -2, "vector", 339,
                  typeid(vector<TH1F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETH1FgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TH1F>) );
      instance.SetNew(&new_vectorlETH1FgR);
      instance.SetNewArray(&newArray_vectorlETH1FgR);
      instance.SetDelete(&delete_vectorlETH1FgR);
      instance.SetDeleteArray(&deleteArray_vectorlETH1FgR);
      instance.SetDestructor(&destruct_vectorlETH1FgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TH1F> >()));

      ::ROOT::AddClassAlternate("vector<TH1F>","std::vector<TH1F, std::allocator<TH1F> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TH1F>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETH1FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TH1F>*)0x0)->GetClass();
      vectorlETH1FgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETH1FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETH1FgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH1F> : new vector<TH1F>;
   }
   static void *newArray_vectorlETH1FgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH1F>[nElements] : new vector<TH1F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETH1FgR(void *p) {
      delete ((vector<TH1F>*)p);
   }
   static void deleteArray_vectorlETH1FgR(void *p) {
      delete [] ((vector<TH1F>*)p);
   }
   static void destruct_vectorlETH1FgR(void *p) {
      typedef vector<TH1F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TH1F>

namespace ROOT {
   static TClass *vectorlETAxismUgR_Dictionary();
   static void vectorlETAxismUgR_TClassManip(TClass*);
   static void *new_vectorlETAxismUgR(void *p = 0);
   static void *newArray_vectorlETAxismUgR(Long_t size, void *p);
   static void delete_vectorlETAxismUgR(void *p);
   static void deleteArray_vectorlETAxismUgR(void *p);
   static void destruct_vectorlETAxismUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TAxis*>*)
   {
      vector<TAxis*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TAxis*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TAxis*>", -2, "vector", 339,
                  typeid(vector<TAxis*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETAxismUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TAxis*>) );
      instance.SetNew(&new_vectorlETAxismUgR);
      instance.SetNewArray(&newArray_vectorlETAxismUgR);
      instance.SetDelete(&delete_vectorlETAxismUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETAxismUgR);
      instance.SetDestructor(&destruct_vectorlETAxismUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TAxis*> >()));

      ::ROOT::AddClassAlternate("vector<TAxis*>","std::vector<TAxis*, std::allocator<TAxis*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TAxis*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETAxismUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TAxis*>*)0x0)->GetClass();
      vectorlETAxismUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETAxismUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETAxismUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TAxis*> : new vector<TAxis*>;
   }
   static void *newArray_vectorlETAxismUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TAxis*>[nElements] : new vector<TAxis*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETAxismUgR(void *p) {
      delete ((vector<TAxis*>*)p);
   }
   static void deleteArray_vectorlETAxismUgR(void *p) {
      delete [] ((vector<TAxis*>*)p);
   }
   static void destruct_vectorlETAxismUgR(void *p) {
      typedef vector<TAxis*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TAxis*>

namespace ROOT {
   static TClass *setlETStringgR_Dictionary();
   static void setlETStringgR_TClassManip(TClass*);
   static void *new_setlETStringgR(void *p = 0);
   static void *newArray_setlETStringgR(Long_t size, void *p);
   static void delete_setlETStringgR(void *p);
   static void deleteArray_setlETStringgR(void *p);
   static void destruct_setlETStringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const set<TString>*)
   {
      set<TString> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(set<TString>));
      static ::ROOT::TGenericClassInfo 
         instance("set<TString>", -2, "set", 94,
                  typeid(set<TString>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &setlETStringgR_Dictionary, isa_proxy, 0,
                  sizeof(set<TString>) );
      instance.SetNew(&new_setlETStringgR);
      instance.SetNewArray(&newArray_setlETStringgR);
      instance.SetDelete(&delete_setlETStringgR);
      instance.SetDeleteArray(&deleteArray_setlETStringgR);
      instance.SetDestructor(&destruct_setlETStringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Insert< set<TString> >()));

      ::ROOT::AddClassAlternate("set<TString>","std::set<TString, std::less<TString>, std::allocator<TString> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const set<TString>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *setlETStringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const set<TString>*)0x0)->GetClass();
      setlETStringgR_TClassManip(theClass);
   return theClass;
   }

   static void setlETStringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_setlETStringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<TString> : new set<TString>;
   }
   static void *newArray_setlETStringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<TString>[nElements] : new set<TString>[nElements];
   }
   // Wrapper around operator delete
   static void delete_setlETStringgR(void *p) {
      delete ((set<TString>*)p);
   }
   static void deleteArray_setlETStringgR(void *p) {
      delete [] ((set<TString>*)p);
   }
   static void destruct_setlETStringgR(void *p) {
      typedef set<TString> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class set<TString>

namespace ROOT {
   static TClass *maplETStringcOvectorlEdoublegRsPgR_Dictionary();
   static void maplETStringcOvectorlEdoublegRsPgR_TClassManip(TClass*);
   static void *new_maplETStringcOvectorlEdoublegRsPgR(void *p = 0);
   static void *newArray_maplETStringcOvectorlEdoublegRsPgR(Long_t size, void *p);
   static void delete_maplETStringcOvectorlEdoublegRsPgR(void *p);
   static void deleteArray_maplETStringcOvectorlEdoublegRsPgR(void *p);
   static void destruct_maplETStringcOvectorlEdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,vector<double> >*)
   {
      map<TString,vector<double> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,vector<double> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,vector<double> >", -2, "map", 100,
                  typeid(map<TString,vector<double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOvectorlEdoublegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,vector<double> >) );
      instance.SetNew(&new_maplETStringcOvectorlEdoublegRsPgR);
      instance.SetNewArray(&newArray_maplETStringcOvectorlEdoublegRsPgR);
      instance.SetDelete(&delete_maplETStringcOvectorlEdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOvectorlEdoublegRsPgR);
      instance.SetDestructor(&destruct_maplETStringcOvectorlEdoublegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,vector<double> > >()));

      ::ROOT::AddClassAlternate("map<TString,vector<double> >","std::map<TString, std::vector<double, std::allocator<double> >, std::less<TString>, std::allocator<std::pair<TString const, std::vector<double, std::allocator<double> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,vector<double> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOvectorlEdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,vector<double> >*)0x0)->GetClass();
      maplETStringcOvectorlEdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOvectorlEdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOvectorlEdoublegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,vector<double> > : new map<TString,vector<double> >;
   }
   static void *newArray_maplETStringcOvectorlEdoublegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,vector<double> >[nElements] : new map<TString,vector<double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOvectorlEdoublegRsPgR(void *p) {
      delete ((map<TString,vector<double> >*)p);
   }
   static void deleteArray_maplETStringcOvectorlEdoublegRsPgR(void *p) {
      delete [] ((map<TString,vector<double> >*)p);
   }
   static void destruct_maplETStringcOvectorlEdoublegRsPgR(void *p) {
      typedef map<TString,vector<double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,vector<double> >

namespace ROOT {
   static TClass *maplETStringcOdoublegR_Dictionary();
   static void maplETStringcOdoublegR_TClassManip(TClass*);
   static void *new_maplETStringcOdoublegR(void *p = 0);
   static void *newArray_maplETStringcOdoublegR(Long_t size, void *p);
   static void delete_maplETStringcOdoublegR(void *p);
   static void deleteArray_maplETStringcOdoublegR(void *p);
   static void destruct_maplETStringcOdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,double>*)
   {
      map<TString,double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,double>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,double>", -2, "map", 100,
                  typeid(map<TString,double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,double>) );
      instance.SetNew(&new_maplETStringcOdoublegR);
      instance.SetNewArray(&newArray_maplETStringcOdoublegR);
      instance.SetDelete(&delete_maplETStringcOdoublegR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOdoublegR);
      instance.SetDestructor(&destruct_maplETStringcOdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,double> >()));

      ::ROOT::AddClassAlternate("map<TString,double>","std::map<TString, double, std::less<TString>, std::allocator<std::pair<TString const, double> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,double>*)0x0)->GetClass();
      maplETStringcOdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,double> : new map<TString,double>;
   }
   static void *newArray_maplETStringcOdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,double>[nElements] : new map<TString,double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOdoublegR(void *p) {
      delete ((map<TString,double>*)p);
   }
   static void deleteArray_maplETStringcOdoublegR(void *p) {
      delete [] ((map<TString,double>*)p);
   }
   static void destruct_maplETStringcOdoublegR(void *p) {
      typedef map<TString,double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,double>

namespace ROOT {
   static TClass *maplETStringcOboolgR_Dictionary();
   static void maplETStringcOboolgR_TClassManip(TClass*);
   static void *new_maplETStringcOboolgR(void *p = 0);
   static void *newArray_maplETStringcOboolgR(Long_t size, void *p);
   static void delete_maplETStringcOboolgR(void *p);
   static void deleteArray_maplETStringcOboolgR(void *p);
   static void destruct_maplETStringcOboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,bool>*)
   {
      map<TString,bool> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,bool>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,bool>", -2, "map", 100,
                  typeid(map<TString,bool>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOboolgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,bool>) );
      instance.SetNew(&new_maplETStringcOboolgR);
      instance.SetNewArray(&newArray_maplETStringcOboolgR);
      instance.SetDelete(&delete_maplETStringcOboolgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOboolgR);
      instance.SetDestructor(&destruct_maplETStringcOboolgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,bool> >()));

      ::ROOT::AddClassAlternate("map<TString,bool>","std::map<TString, bool, std::less<TString>, std::allocator<std::pair<TString const, bool> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,bool>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,bool>*)0x0)->GetClass();
      maplETStringcOboolgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOboolgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,bool> : new map<TString,bool>;
   }
   static void *newArray_maplETStringcOboolgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,bool>[nElements] : new map<TString,bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOboolgR(void *p) {
      delete ((map<TString,bool>*)p);
   }
   static void deleteArray_maplETStringcOboolgR(void *p) {
      delete [] ((map<TString,bool>*)p);
   }
   static void destruct_maplETStringcOboolgR(void *p) {
      typedef map<TString,bool> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,bool>

namespace ROOT {
   static TClass *maplETStringcOTQGridScanObservablemUgR_Dictionary();
   static void maplETStringcOTQGridScanObservablemUgR_TClassManip(TClass*);
   static void *new_maplETStringcOTQGridScanObservablemUgR(void *p = 0);
   static void *newArray_maplETStringcOTQGridScanObservablemUgR(Long_t size, void *p);
   static void delete_maplETStringcOTQGridScanObservablemUgR(void *p);
   static void deleteArray_maplETStringcOTQGridScanObservablemUgR(void *p);
   static void destruct_maplETStringcOTQGridScanObservablemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,TQGridScanObservable*>*)
   {
      map<TString,TQGridScanObservable*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,TQGridScanObservable*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,TQGridScanObservable*>", -2, "map", 100,
                  typeid(map<TString,TQGridScanObservable*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOTQGridScanObservablemUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,TQGridScanObservable*>) );
      instance.SetNew(&new_maplETStringcOTQGridScanObservablemUgR);
      instance.SetNewArray(&newArray_maplETStringcOTQGridScanObservablemUgR);
      instance.SetDelete(&delete_maplETStringcOTQGridScanObservablemUgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOTQGridScanObservablemUgR);
      instance.SetDestructor(&destruct_maplETStringcOTQGridScanObservablemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,TQGridScanObservable*> >()));

      ::ROOT::AddClassAlternate("map<TString,TQGridScanObservable*>","std::map<TString, TQGridScanObservable*, std::less<TString>, std::allocator<std::pair<TString const, TQGridScanObservable*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,TQGridScanObservable*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOTQGridScanObservablemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,TQGridScanObservable*>*)0x0)->GetClass();
      maplETStringcOTQGridScanObservablemUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOTQGridScanObservablemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOTQGridScanObservablemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TQGridScanObservable*> : new map<TString,TQGridScanObservable*>;
   }
   static void *newArray_maplETStringcOTQGridScanObservablemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TQGridScanObservable*>[nElements] : new map<TString,TQGridScanObservable*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOTQGridScanObservablemUgR(void *p) {
      delete ((map<TString,TQGridScanObservable*>*)p);
   }
   static void deleteArray_maplETStringcOTQGridScanObservablemUgR(void *p) {
      delete [] ((map<TString,TQGridScanObservable*>*)p);
   }
   static void destruct_maplETStringcOTQGridScanObservablemUgR(void *p) {
      typedef map<TString,TQGridScanObservable*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,TQGridScanObservable*>

namespace ROOT {
   static TClass *maplETStringcOTQFoldermUgR_Dictionary();
   static void maplETStringcOTQFoldermUgR_TClassManip(TClass*);
   static void *new_maplETStringcOTQFoldermUgR(void *p = 0);
   static void *newArray_maplETStringcOTQFoldermUgR(Long_t size, void *p);
   static void delete_maplETStringcOTQFoldermUgR(void *p);
   static void deleteArray_maplETStringcOTQFoldermUgR(void *p);
   static void destruct_maplETStringcOTQFoldermUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,TQFolder*>*)
   {
      map<TString,TQFolder*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,TQFolder*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,TQFolder*>", -2, "map", 100,
                  typeid(map<TString,TQFolder*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOTQFoldermUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,TQFolder*>) );
      instance.SetNew(&new_maplETStringcOTQFoldermUgR);
      instance.SetNewArray(&newArray_maplETStringcOTQFoldermUgR);
      instance.SetDelete(&delete_maplETStringcOTQFoldermUgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOTQFoldermUgR);
      instance.SetDestructor(&destruct_maplETStringcOTQFoldermUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,TQFolder*> >()));

      ::ROOT::AddClassAlternate("map<TString,TQFolder*>","std::map<TString, TQFolder*, std::less<TString>, std::allocator<std::pair<TString const, TQFolder*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,TQFolder*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOTQFoldermUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,TQFolder*>*)0x0)->GetClass();
      maplETStringcOTQFoldermUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOTQFoldermUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOTQFoldermUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TQFolder*> : new map<TString,TQFolder*>;
   }
   static void *newArray_maplETStringcOTQFoldermUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TQFolder*>[nElements] : new map<TString,TQFolder*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOTQFoldermUgR(void *p) {
      delete ((map<TString,TQFolder*>*)p);
   }
   static void deleteArray_maplETStringcOTQFoldermUgR(void *p) {
      delete [] ((map<TString,TQFolder*>*)p);
   }
   static void destruct_maplETStringcOTQFoldermUgR(void *p) {
      typedef map<TString,TQFolder*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,TQFolder*>

namespace ROOT {
   static TClass *maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR_Dictionary();
   static void maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR_TClassManip(TClass*);
   static void *new_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR(void *p = 0);
   static void *newArray_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR(Long_t size, void *p);
   static void delete_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR(void *p);
   static void deleteArray_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR(void *p);
   static void destruct_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TQMessageStream::MessageType,unsigned int>*)
   {
      map<TQMessageStream::MessageType,unsigned int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TQMessageStream::MessageType,unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TQMessageStream::MessageType,unsigned int>", -2, "map", 100,
                  typeid(map<TQMessageStream::MessageType,unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TQMessageStream::MessageType,unsigned int>) );
      instance.SetNew(&new_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR);
      instance.SetNewArray(&newArray_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR);
      instance.SetDelete(&delete_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR);
      instance.SetDestructor(&destruct_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TQMessageStream::MessageType,unsigned int> >()));

      ::ROOT::AddClassAlternate("map<TQMessageStream::MessageType,unsigned int>","std::map<TQMessageStream::MessageType, unsigned int, std::less<TQMessageStream::MessageType>, std::allocator<std::pair<TQMessageStream::MessageType const, unsigned int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TQMessageStream::MessageType,unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TQMessageStream::MessageType,unsigned int>*)0x0)->GetClass();
      maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TQMessageStream::MessageType,unsigned int> : new map<TQMessageStream::MessageType,unsigned int>;
   }
   static void *newArray_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TQMessageStream::MessageType,unsigned int>[nElements] : new map<TQMessageStream::MessageType,unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR(void *p) {
      delete ((map<TQMessageStream::MessageType,unsigned int>*)p);
   }
   static void deleteArray_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR(void *p) {
      delete [] ((map<TQMessageStream::MessageType,unsigned int>*)p);
   }
   static void destruct_maplETQMessageStreamcLcLMessageTypecOunsignedsPintgR(void *p) {
      typedef map<TQMessageStream::MessageType,unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TQMessageStream::MessageType,unsigned int>

namespace {
  void TriggerDictionaryInitialization_libQFramework_Impl() {
    static const char* headers[] = {
"QFramework/ASG.h",
"QFramework/TQABCDCalculator.h",
"QFramework/TQAlgorithm.h",
"QFramework/TQAnalysisAlgorithm.h",
"QFramework/TQAnalysisJob.h",
"QFramework/TQAnalysisSampleVisitor.h",
"QFramework/TQAnalysisSampleVisitorBase.h",
"QFramework/TQCompPlotter.h",
"QFramework/TQCompiledCut.h",
"QFramework/TQConfigReader.h",
"QFramework/TQConstObservable.h",
"QFramework/TQConstTreeObservable.h",
"QFramework/TQCounter.h",
"QFramework/TQCut.h",
"QFramework/TQCutFactory.h",
"QFramework/TQCutflowAnalysisJob.h",
"QFramework/TQCutflowPlotter.h",
"QFramework/TQCutflowPrinter.h",
"QFramework/TQDefaultPlotter.h",
"QFramework/TQEventIndexObservable.h",
"QFramework/TQEventlistAnalysisJob.h",
"QFramework/TQEventlistPrinter.h",
"QFramework/TQFilterObservable.h",
"QFramework/TQFlags.h",
"QFramework/TQFolder.h",
"QFramework/TQGraphMakerAnalysisJob.h",
"QFramework/TQGridScanBound.h",
"QFramework/TQGridScanObservable.h",
"QFramework/TQGridScanPoint.h",
"QFramework/TQGridScanResults.h",
"QFramework/TQGridScanStyle.h",
"QFramework/TQGridScanner.h",
"QFramework/TQHWWPlotter.h",
"QFramework/TQHistComparer.h",
"QFramework/TQHistoMakerAnalysisJob.h",
"QFramework/TQHistogramObservable.h",
"QFramework/TQHistogramUtils.h",
"QFramework/TQImportLink.h",
"QFramework/TQIterator.h",
"QFramework/TQIteratorT.h",
"QFramework/TQLibrary.h",
"QFramework/TQLink.h",
"QFramework/TQListUtils.h",
"QFramework/TQMVA.h",
"QFramework/TQMVAObservable.h",
"QFramework/TQMessageStream.h",
"QFramework/TQMultiChannelAnalysisSampleVisitor.h",
"QFramework/TQMultiObservable.h",
"QFramework/TQNFBase.h",
"QFramework/TQNFCalculator.h",
"QFramework/TQNFChainloader.h",
"QFramework/TQNFCustomCalculator.h",
"QFramework/TQNFManualSetter.h",
"QFramework/TQNFTop0jetEstimator.h",
"QFramework/TQNFTop1jetEstimator.h",
"QFramework/TQNFUncertaintyScaler.h",
"QFramework/TQNTupleDumperAnalysisJob.h",
"QFramework/TQNamedTaggable.h",
"QFramework/TQObservable.h",
"QFramework/TQPCA.h",
"QFramework/TQPCAAnalysisJob.h",
"QFramework/TQPathManager.h",
"QFramework/TQPlotter.h",
"QFramework/TQPresenter.h",
"QFramework/TQROOTPlotter.h",
"QFramework/TQSample.h",
"QFramework/TQSampleDataReader.h",
"QFramework/TQSampleFolder.h",
"QFramework/TQSampleGroupingVisitor.h",
"QFramework/TQSampleInitializer.h",
"QFramework/TQSampleInitializerBase.h",
"QFramework/TQSampleNormalizationObservable.h",
"QFramework/TQSamplePurger.h",
"QFramework/TQSampleRevisitor.h",
"QFramework/TQSampleVisitor.h",
"QFramework/TQSignificanceEvaluator.h",
"QFramework/TQStringUtils.h",
"QFramework/TQSystematics.h",
"QFramework/TQSystematicsHandler.h",
"QFramework/TQSystematicsManager.h",
"QFramework/TQTHnBaseMakerAnalysisJob.h",
"QFramework/TQTHnBaseUtils.h",
"QFramework/TQTable.h",
"QFramework/TQTaggable.h",
"QFramework/TQTikZPlotter.h",
"QFramework/TQToken.h",
"QFramework/TQTreeFormulaObservable.h",
"QFramework/TQTreeObservable.h",
"QFramework/TQUniqueCut.h",
"QFramework/TQUtils.h",
"QFramework/TQValue.h",
"QFramework/TQVectorAuxObservable.h",
"QFramework/TQXSecParser.h",
0
    };
    static const char* includePaths[] = {
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/QFramework",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/libxml2",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
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
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/libxml2",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/",
"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/QFramework/CMakeFiles/makeQFrameworkCintDict.ms38LX/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libQFramework dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$QFramework/TQValue.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TQValue;
class __attribute__((annotate("$clingAutoload$QFramework/TQValue.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TQValueDouble;
class __attribute__((annotate("$clingAutoload$QFramework/TQValue.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TQValueInteger;
class __attribute__((annotate("$clingAutoload$QFramework/TQValue.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TQValueBool;
class __attribute__((annotate("$clingAutoload$QFramework/TQValue.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TQValueString;
class __attribute__((annotate(R"ATTRDUMP(storage class for meta-information)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(storage class for meta-information)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(storage class for meta-information)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(storage class for meta-information)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$QFramework/TQTaggable.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TQTaggable;
class __attribute__((annotate("$clingAutoload$QFramework/TQNamedTaggable.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TQNamedTaggable;
class __attribute__((annotate("$clingAutoload$QFramework/TQMessageStream.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TQMessageStream;
class __attribute__((annotate(R"ATTRDUMP(abstract base class for normalization factor calculators)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(abstract base class for normalization factor calculators)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(abstract base class for normalization factor calculators)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(abstract base class for normalization factor calculators)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$QFramework/TQNFBase.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TQNFBase;
class __attribute__((annotate("$clingAutoload$QFramework/TQCounter.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TQCounter;
class __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TQABCDCalculator;
class __attribute__((annotate("$clingAutoload$QFramework/TQAlgorithm.h")))  TQAlgorithm;
class __attribute__((annotate("$clingAutoload$TObject.h")))  TObject;
template <class T> class __attribute__((annotate("$clingAutoload$QFramework/TQIteratorT.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TQIteratorT;

class __attribute__((annotate("$clingAutoload$QFramework/TQCut.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TQCut;
class __attribute__((annotate("$clingAutoload$TObjString.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TObjString;
class __attribute__((annotate("$clingAutoload$QFramework/TQFolder.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TQFolder;
class __attribute__((annotate("$clingAutoload$QFramework/TQSampleFolder.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TQSampleFolder;
class __attribute__((annotate("$clingAutoload$QFramework/TQSample.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TQSample;
class __attribute__((annotate("$clingAutoload$TH1.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TH1;
class __attribute__((annotate("$clingAutoload$QFramework/TQObservable.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TQObservable;
class __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisJob.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TQAnalysisJob;
class __attribute__((annotate("$clingAutoload$TGraphErrors.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TGraphErrors;
class __attribute__((annotate("$clingAutoload$TGraph.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TGraph;
class __attribute__((annotate("$clingAutoload$TGraphAsymmErrors.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TGraphAsymmErrors;
class __attribute__((annotate("$clingAutoload$TCollection.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TCollection;
class __attribute__((annotate("$clingAutoload$TList.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TList;
class __attribute__((annotate("$clingAutoload$TLegendEntry.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TLegendEntry;
class __attribute__((annotate("$clingAutoload$TBranch.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TBranch;
class __attribute__((annotate("$clingAutoload$TKey.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TKey;
class __attribute__((annotate("$clingAutoload$QFramework/TQIteratorT.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TQCounterGrid;
class __attribute__((annotate("$clingAutoload$QFramework/TQPCA.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TQPCA;
class __attribute__((annotate("$clingAutoload$TH2.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TH2;
class __attribute__((annotate("$clingAutoload$TObjArray.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQABCDCalculator.h")))  TObjArray;
class __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h")))  TQAnalysisAlgorithm;
class __attribute__((annotate("$clingAutoload$QFramework/TQSampleVisitor.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisSampleVisitor.h")))  TQSampleVisitor;
class __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisSampleVisitorBase.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisSampleVisitor.h")))  TQAnalysisSampleVisitorBase;
class __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisSampleVisitor.h")))  TQAnalysisSampleVisitor;
class __attribute__((annotate("$clingAutoload$QFramework/TQTable.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQCompPlotter.h")))  TQTable;
class __attribute__((annotate(R"ATTRDUMP(helper class for data retrieval from sample folders)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(helper class for data retrieval from sample folders)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(helper class for data retrieval from sample folders)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(helper class for data retrieval from sample folders)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$QFramework/TQSampleDataReader.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQCompPlotter.h")))  TQSampleDataReader;
class __attribute__((annotate("$clingAutoload$QFramework/TQPresenter.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQCompPlotter.h")))  TQPresenter;
class __attribute__((annotate("$clingAutoload$QFramework/TQPlotter.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQCompPlotter.h")))  TQPlotter;
class __attribute__((annotate("$clingAutoload$QFramework/TQROOTPlotter.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQCompPlotter.h")))  TQROOTPlotter;
class __attribute__((annotate("$clingAutoload$QFramework/TQCompPlotter.h")))  TQCompPlotter;
class __attribute__((annotate("$clingAutoload$QFramework/TQConfigReader.h")))  TQConfigReader;
class __attribute__((annotate("$clingAutoload$QFramework/TQConstObservable.h")))  TQConstObservable;
class __attribute__((annotate("$clingAutoload$QFramework/TQCutFactory.h")))  TQCutFactory;
class __attribute__((annotate("$clingAutoload$QFramework/TQCutflowAnalysisJob.h")))  TQCutflowAnalysisJob;
class __attribute__((annotate("$clingAutoload$QFramework/TQCutflowPrinter.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQCutflowPlotter.h")))  TQCutflowPrinter;
class __attribute__((annotate("$clingAutoload$QFramework/TQCutflowPlotter.h")))  TQCutflowPlotter;
class __attribute__((annotate("$clingAutoload$QFramework/TQDefaultPlotter.h")))  TQDefaultPlotter;
class __attribute__((annotate("$clingAutoload$QFramework/TQTreeObservable.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQEventIndexObservable.h")))  TQTreeObservable;
class __attribute__((annotate("$clingAutoload$QFramework/TQEventIndexObservable.h")))  TQEventIndexObservable;
class __attribute__((annotate("$clingAutoload$QFramework/TQEventlistAnalysisJob.h")))  TQEventlistAnalysisJob;
class __attribute__((annotate(R"ATTRDUMP(presenter class to print event lists)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$QFramework/TQEventlistPrinter.h")))  TQEventlistPrinter;
class __attribute__((annotate("$clingAutoload$QFramework/TQFilterObservable.h")))  TQFilterObservable;
class __attribute__((annotate("$clingAutoload$QFramework/TQGraphMakerAnalysisJob.h")))  TQGraphMakerAnalysisJob;
class __attribute__((annotate("$clingAutoload$QFramework/TQGridScanBound.h")))  TQGridScanBound;
class __attribute__((annotate("$clingAutoload$QFramework/TQGridScanBound.h")))  TQGridScanBounds;
class __attribute__((annotate("$clingAutoload$QFramework/TQGridScanObservable.h")))  TQGridScanObservable;
class __attribute__((annotate("$clingAutoload$QFramework/TQGridScanObservable.h")))  TQHistParams;
class __attribute__((annotate("$clingAutoload$QFramework/TQGridScanObservable.h")))  TQGridScanNormalObservable;
class __attribute__((annotate("$clingAutoload$QFramework/TQGridScanObservable.h")))  TQGridScanSplitObservable;
class __attribute__((annotate("$clingAutoload$QFramework/TQGridScanPoint.h")))  TQGridScanPoint;
class __attribute__((annotate("$clingAutoload$QFramework/TQGridScanResults.h")))  TQGridScanResults;
class __attribute__((annotate("$clingAutoload$QFramework/TQSignificanceEvaluator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQGridScanStyle.h")))  TQSignificanceEvaluator;
class __attribute__((annotate("$clingAutoload$QFramework/TQSignificanceEvaluator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQGridScanStyle.h")))  TQSignificanceEvaluatorBase;
class __attribute__((annotate("$clingAutoload$QFramework/TQSignificanceEvaluator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQGridScanStyle.h")))  TQSimpleSignificanceEvaluator;
class __attribute__((annotate("$clingAutoload$QFramework/TQSignificanceEvaluator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQGridScanStyle.h")))  TQSimpleSignificanceEvaluator2;
class __attribute__((annotate("$clingAutoload$QFramework/TQSignificanceEvaluator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQGridScanStyle.h")))  TQSimpleSignificanceEvaluator3;
class __attribute__((annotate("$clingAutoload$QFramework/TQSignificanceEvaluator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQGridScanStyle.h")))  TQPoissonSignificanceEvaluator;
class __attribute__((annotate("$clingAutoload$QFramework/TQGridScanner.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQGridScanStyle.h")))  TQGridScanner;
class __attribute__((annotate("$clingAutoload$QFramework/TQHWWPlotter.h")))  TQHWWPlotter;
class __attribute__((annotate("$clingAutoload$QFramework/TQHistComparer.h")))  TQHistComparer;
class __attribute__((annotate("$clingAutoload$QFramework/TQHistoMakerAnalysisJob.h")))  TQHistoMakerAnalysisJob;
template <class T> class __attribute__((annotate("$clingAutoload$QFramework/TQHistogramObservable.h")))  TQHistogramObservable;

class __attribute__((annotate("$clingAutoload$TH3.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQHistogramObservable.h")))  TH3;
class __attribute__((annotate("$clingAutoload$QFramework/TQLink.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQImportLink.h")))  TQLink;
class __attribute__((annotate("$clingAutoload$QFramework/TQImportLink.h")))  TQImportLink;
class __attribute__((annotate("$clingAutoload$QFramework/TQLibrary.h")))  TQLibrary;
class __attribute__((annotate("$clingAutoload$QFramework/TQToken.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQMVA.h")))  TQToken;
class __attribute__((annotate("$clingAutoload$QFramework/TQMVA.h")))  TQMVA;
class __attribute__((annotate("$clingAutoload$QFramework/TQMVAObservable.h")))  TQMVAObservable;
class __attribute__((annotate("$clingAutoload$QFramework/TQMultiChannelAnalysisSampleVisitor.h")))  TQMultiChannelAnalysisSampleVisitor;
class __attribute__((annotate("$clingAutoload$QFramework/TQMultiObservable.h")))  TQMultiObservable;
class __attribute__((annotate(R"ATTRDUMP(calculator class for normalization factors)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$QFramework/TQNFCalculator.h")))  TQNFCalculator;
class __attribute__((annotate("$clingAutoload$QFramework/TQNFChainloader.h")))  TQNFChainloader;
class __attribute__((annotate("$clingAutoload$QFramework/TQNFCustomCalculator.h")))  TQNFCustomCalculator;
class __attribute__((annotate("$clingAutoload$QFramework/TQNFManualSetter.h")))  TQNFManualSetter;
class __attribute__((annotate("$clingAutoload$QFramework/TQNFTop0jetEstimator.h")))  TQNFTop0jetEstimator;
class __attribute__((annotate("$clingAutoload$QFramework/TQNFTop1jetEstimator.h")))  TQNFTop1jetEstimator;
class __attribute__((annotate("$clingAutoload$QFramework/TQNFUncertaintyScaler.h")))  TQNFUncertaintyScaler;
class __attribute__((annotate("$clingAutoload$QFramework/TQNTupleDumperAnalysisJob.h")))  TQNTupleDumperAnalysisJob;
class __attribute__((annotate("$clingAutoload$QFramework/TQPCAAnalysisJob.h")))  TQPCAAnalysisJob;
class __attribute__((annotate("$clingAutoload$QFramework/TQPathManager.h")))  TQPathManager;
class __attribute__((annotate("$clingAutoload$QFramework/TQSampleGroupingVisitor.h")))  TQSampleGroupingVisitor;
class __attribute__((annotate("$clingAutoload$QFramework/TQSampleInitializerBase.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQSampleInitializer.h")))  TQSampleInitializerBase;
class __attribute__((annotate("$clingAutoload$QFramework/TQSampleInitializer.h")))  TQSampleInitializer;
class __attribute__((annotate("$clingAutoload$QFramework/TQSampleNormalizationObservable.h")))  TQSampleNormalizationObservable;
class __attribute__((annotate("$clingAutoload$QFramework/TQSamplePurger.h")))  TQSamplePurger;
class __attribute__((annotate("$clingAutoload$QFramework/TQSampleRevisitor.h")))  TQSampleRevisitor;
class __attribute__((annotate("$clingAutoload$QFramework/TQSystematicsManager.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQSystematicsHandler.h")))  TQSystematicsManager;
class __attribute__((annotate("$clingAutoload$QFramework/TQSystematicsHandler.h")))  TQSystematicsHandler;
class __attribute__((annotate("$clingAutoload$QFramework/TQTHnBaseMakerAnalysisJob.h")))  TQTHnBaseMakerAnalysisJob;
class __attribute__((annotate("$clingAutoload$QFramework/TQTikZPlotter.h")))  TQTikZPlotter;
class __attribute__((annotate("$clingAutoload$QFramework/TQTreeFormulaObservable.h")))  TQTreeFormulaObservable;
class __attribute__((annotate("$clingAutoload$QFramework/TQUniqueCut.h")))  TQUniqueCut;
class __attribute__((annotate("$clingAutoload$QFramework/TQVectorAuxObservable.h")))  TQVectorAuxObservable;
class __attribute__((annotate("$clingAutoload$QFramework/TQXSecParser.h")))  TQXSecParser;
typedef TQIteratorT<TObject> TQIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TQValue> TQValueIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TQCounter> TQCounterIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TQCut> TQCutIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TObjString> TQStringIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TQFolder> TQFolderIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TQSampleFolder> TQSampleFolderIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TQSample> TQSampleIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TH1> TQTH1Iterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TQNamedTaggable> TQTaggableIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TQObservable> TQObservableIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TQAnalysisJob> TQAnalysisJobIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TGraphErrors> TQGraphErrorsIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TGraph> TQGraphIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TGraphAsymmErrors> TQGraphAsymmErrorsIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TCollection> TQCollectionIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TList> TQListIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TLegendEntry> TQLegendEntryIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TBranch> TQBranchIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQIteratorT<TKey> TQKeyIterator __attribute__((annotate("$clingAutoload$QFramework/TQIterator.h")))  __attribute__((annotate("$clingAutoload$QFramework/TQAnalysisAlgorithm.h"))) ;
typedef TQCut TQCompiledCut __attribute__((annotate("$clingAutoload$QFramework/TQCompiledCut.h"))) ;
typedef TQConstObservable TQConstTreeObservable __attribute__((annotate("$clingAutoload$QFramework/TQConstTreeObservable.h"))) ;
typedef TQHWWPlotter TQHWWPlotter2 __attribute__((annotate("$clingAutoload$QFramework/TQHWWPlotter.h"))) ;
typedef TQHistogramObservable<TH1> TQTH1Observable __attribute__((annotate("$clingAutoload$QFramework/TQHistogramObservable.h"))) ;
typedef TQHistogramObservable<TH2> TQTH2Observable __attribute__((annotate("$clingAutoload$QFramework/TQHistogramObservable.h"))) ;
typedef TQHistogramObservable<TH3> TQTH3Observable __attribute__((annotate("$clingAutoload$QFramework/TQHistogramObservable.h"))) ;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libQFramework dictionary payload"

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
  #define PACKAGE_VERSION "QFramework-00-00-00"
#endif
#ifndef PACKAGE_VERSION_UQ
  #define PACKAGE_VERSION_UQ QFramework-00-00-00
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "QFramework/ASG.h"
#include "QFramework/TQABCDCalculator.h"
#include "QFramework/TQAlgorithm.h"
#include "QFramework/TQAnalysisAlgorithm.h"
#include "QFramework/TQAnalysisJob.h"
#include "QFramework/TQAnalysisSampleVisitor.h"
#include "QFramework/TQAnalysisSampleVisitorBase.h"
#include "QFramework/TQCompPlotter.h"
#include "QFramework/TQCompiledCut.h"
#include "QFramework/TQConfigReader.h"
#include "QFramework/TQConstObservable.h"
#include "QFramework/TQConstTreeObservable.h"
#include "QFramework/TQCounter.h"
#include "QFramework/TQCut.h"
#include "QFramework/TQCutFactory.h"
#include "QFramework/TQCutflowAnalysisJob.h"
#include "QFramework/TQCutflowPlotter.h"
#include "QFramework/TQCutflowPrinter.h"
#include "QFramework/TQDefaultPlotter.h"
#include "QFramework/TQEventIndexObservable.h"
#include "QFramework/TQEventlistAnalysisJob.h"
#include "QFramework/TQEventlistPrinter.h"
#include "QFramework/TQFilterObservable.h"
#include "QFramework/TQFlags.h"
#include "QFramework/TQFolder.h"
#include "QFramework/TQGraphMakerAnalysisJob.h"
#include "QFramework/TQGridScanBound.h"
#include "QFramework/TQGridScanObservable.h"
#include "QFramework/TQGridScanPoint.h"
#include "QFramework/TQGridScanResults.h"
#include "QFramework/TQGridScanStyle.h"
#include "QFramework/TQGridScanner.h"
#include "QFramework/TQHWWPlotter.h"
#include "QFramework/TQHistComparer.h"
#include "QFramework/TQHistoMakerAnalysisJob.h"
#include "QFramework/TQHistogramObservable.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQImportLink.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQIteratorT.h"
#include "QFramework/TQLibrary.h"
#include "QFramework/TQLink.h"
#include "QFramework/TQListUtils.h"
#include "QFramework/TQMVA.h"
#include "QFramework/TQMVAObservable.h"
#include "QFramework/TQMessageStream.h"
#include "QFramework/TQMultiChannelAnalysisSampleVisitor.h"
#include "QFramework/TQMultiObservable.h"
#include "QFramework/TQNFBase.h"
#include "QFramework/TQNFCalculator.h"
#include "QFramework/TQNFChainloader.h"
#include "QFramework/TQNFCustomCalculator.h"
#include "QFramework/TQNFManualSetter.h"
#include "QFramework/TQNFTop0jetEstimator.h"
#include "QFramework/TQNFTop1jetEstimator.h"
#include "QFramework/TQNFUncertaintyScaler.h"
#include "QFramework/TQNTupleDumperAnalysisJob.h"
#include "QFramework/TQNamedTaggable.h"
#include "QFramework/TQObservable.h"
#include "QFramework/TQPCA.h"
#include "QFramework/TQPCAAnalysisJob.h"
#include "QFramework/TQPathManager.h"
#include "QFramework/TQPlotter.h"
#include "QFramework/TQPresenter.h"
#include "QFramework/TQROOTPlotter.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQSampleDataReader.h"
#include "QFramework/TQSampleFolder.h"
#include "QFramework/TQSampleGroupingVisitor.h"
#include "QFramework/TQSampleInitializer.h"
#include "QFramework/TQSampleInitializerBase.h"
#include "QFramework/TQSampleNormalizationObservable.h"
#include "QFramework/TQSamplePurger.h"
#include "QFramework/TQSampleRevisitor.h"
#include "QFramework/TQSampleVisitor.h"
#include "QFramework/TQSignificanceEvaluator.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQSystematics.h"
#include "QFramework/TQSystematicsHandler.h"
#include "QFramework/TQSystematicsManager.h"
#include "QFramework/TQTHnBaseMakerAnalysisJob.h"
#include "QFramework/TQTHnBaseUtils.h"
#include "QFramework/TQTable.h"
#include "QFramework/TQTaggable.h"
#include "QFramework/TQTikZPlotter.h"
#include "QFramework/TQToken.h"
#include "QFramework/TQTreeFormulaObservable.h"
#include "QFramework/TQTreeObservable.h"
#include "QFramework/TQUniqueCut.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQValue.h"
#include "QFramework/TQVectorAuxObservable.h"
#include "QFramework/TQXSecParser.h"
//this is an automatically generated -*- c++ -*- file - EDITS WILL BE LOST!

#ifndef __QFrameworkDICT__
#define __QFrameworkDICT__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winconsistent-missing-override"

#include "QFramework/ASG.h"
#include "QFramework/TQABCDCalculator.h"
#include "QFramework/TQAlgorithm.h"
#include "QFramework/TQAnalysisAlgorithm.h"
#include "QFramework/TQAnalysisJob.h"
#include "QFramework/TQAnalysisSampleVisitor.h"
#include "QFramework/TQAnalysisSampleVisitorBase.h"
#include "QFramework/TQCompPlotter.h"
#include "QFramework/TQCompiledCut.h"
#include "QFramework/TQConfigReader.h"
#include "QFramework/TQConstObservable.h"
#include "QFramework/TQConstTreeObservable.h"
#include "QFramework/TQCounter.h"
#include "QFramework/TQCut.h"
#include "QFramework/TQCutFactory.h"
#include "QFramework/TQCutflowAnalysisJob.h"
#include "QFramework/TQCutflowPlotter.h"
#include "QFramework/TQCutflowPrinter.h"
#include "QFramework/TQDefaultPlotter.h"
#include "QFramework/TQEventIndexObservable.h"
#include "QFramework/TQEventlistAnalysisJob.h"
#include "QFramework/TQEventlistPrinter.h"
#include "QFramework/TQFilterObservable.h"
#include "QFramework/TQFlags.h"
#include "QFramework/TQFolder.h"
#include "QFramework/TQGraphMakerAnalysisJob.h"
#include "QFramework/TQGridScanBound.h"
#include "QFramework/TQGridScanObservable.h"
#include "QFramework/TQGridScanPoint.h"
#include "QFramework/TQGridScanResults.h"
#include "QFramework/TQGridScanStyle.h"
#include "QFramework/TQGridScanner.h"
#include "QFramework/TQHWWPlotter.h"
#include "QFramework/TQHistComparer.h"
#include "QFramework/TQHistoMakerAnalysisJob.h"
#include "QFramework/TQHistogramObservable.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQImportLink.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQIteratorT.h"
#include "QFramework/TQLibrary.h"
#include "QFramework/TQLink.h"
#include "QFramework/TQListUtils.h"
#include "QFramework/TQMVA.h"
#include "QFramework/TQMVAObservable.h"
#include "QFramework/TQMessageStream.h"
#include "QFramework/TQMultiChannelAnalysisSampleVisitor.h"
#include "QFramework/TQMultiObservable.h"
#include "QFramework/TQNFBase.h"
#include "QFramework/TQNFCalculator.h"
#include "QFramework/TQNFChainloader.h"
#include "QFramework/TQNFCustomCalculator.h"
#include "QFramework/TQNFManualSetter.h"
#include "QFramework/TQNFTop0jetEstimator.h"
#include "QFramework/TQNFTop1jetEstimator.h"
#include "QFramework/TQNFUncertaintyScaler.h"
#include "QFramework/TQNTupleDumperAnalysisJob.h"
#include "QFramework/TQNamedTaggable.h"
#include "QFramework/TQObservable.h"
#include "QFramework/TQPCA.h"
#include "QFramework/TQPCAAnalysisJob.h"
#include "QFramework/TQPathManager.h"
#include "QFramework/TQPlotter.h"
#include "QFramework/TQPresenter.h"
#include "QFramework/TQROOTPlotter.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQSampleDataReader.h"
#include "QFramework/TQSampleFolder.h"
#include "QFramework/TQSampleGroupingVisitor.h"
#include "QFramework/TQSampleInitializer.h"
#include "QFramework/TQSampleInitializerBase.h"
#include "QFramework/TQSampleNormalizationObservable.h"
#include "QFramework/TQSamplePurger.h"
#include "QFramework/TQSampleRevisitor.h"
#include "QFramework/TQSampleVisitor.h"
#include "QFramework/TQSignificanceEvaluator.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQSystematics.h"
#include "QFramework/TQSystematicsHandler.h"
#include "QFramework/TQSystematicsManager.h"
#include "QFramework/TQTHnBaseMakerAnalysisJob.h"
#include "QFramework/TQTHnBaseUtils.h"
#include "QFramework/TQTable.h"
#include "QFramework/TQTaggable.h"
#include "QFramework/TQTikZPlotter.h"
#include "QFramework/TQToken.h"
#include "QFramework/TQTreeFormulaObservable.h"
#include "QFramework/TQTreeObservable.h"
#include "QFramework/TQUniqueCut.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQValue.h"
#include "QFramework/TQVectorAuxObservable.h"
#include "QFramework/TQXSecParser.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
// >>> begin ASG.h
// <<< end ASG.h

// >>> begin TQABCDCalculator.h
#pragma link C++ class TQABCDCalculator+;
// <<< end TQABCDCalculator.h

// >>> begin TQAlgorithm.h
#pragma link C++ class TQAlgorithm+;
// <<< end TQAlgorithm.h

// >>> begin TQAnalysisAlgorithm.h
#pragma link C++ class TQAnalysisAlgorithm+;
// <<< end TQAnalysisAlgorithm.h

// >>> begin TQAnalysisJob.h
#pragma link C++ class TQAnalysisJob+;
// <<< end TQAnalysisJob.h

// >>> begin TQAnalysisSampleVisitor.h
#pragma link C++ class TQAnalysisSampleVisitor+;
// <<< end TQAnalysisSampleVisitor.h

// >>> begin TQAnalysisSampleVisitorBase.h
#pragma link C++ class TQAnalysisSampleVisitorBase+;
// <<< end TQAnalysisSampleVisitorBase.h

// >>> begin TQCompPlotter.h
#pragma link C++ class TQCompPlotter+;
// <<< end TQCompPlotter.h

// >>> begin TQCompiledCut.h
#pragma link C++ typedef TQCompiledCut;
// <<< end TQCompiledCut.h

// >>> begin TQConfigReader.h
#pragma link C++ class TQConfigReader+;
// <<< end TQConfigReader.h

// >>> begin TQConstObservable.h
#pragma link C++ class TQConstObservable+;
// <<< end TQConstObservable.h

// >>> begin TQConstTreeObservable.h
#pragma link C++ typedef TQConstTreeObservable;
// <<< end TQConstTreeObservable.h

// >>> begin TQCounter.h
#pragma link C++ class TQCounter+;
// <<< end TQCounter.h

// >>> begin TQCut.h
#pragma link C++ class TQCut+;
// <<< end TQCut.h

// >>> begin TQCutFactory.h
#pragma link C++ class TQCutFactory+;
// <<< end TQCutFactory.h

// >>> begin TQCutflowAnalysisJob.h
#pragma link C++ class TQCutflowAnalysisJob+;
// <<< end TQCutflowAnalysisJob.h

// >>> begin TQCutflowPlotter.h
#pragma link C++ class TQCutflowPlotter+;
// <<< end TQCutflowPlotter.h

// >>> begin TQCutflowPrinter.h
#pragma link C++ class TQCutflowPrinter+;
// <<< end TQCutflowPrinter.h

// >>> begin TQDefaultPlotter.h
#pragma link C++ class TQDefaultPlotter+;
// <<< end TQDefaultPlotter.h

// >>> begin TQEventIndexObservable.h
#pragma link C++ class TQEventIndexObservable+;
// <<< end TQEventIndexObservable.h

// >>> begin TQEventlistAnalysisJob.h
#pragma link C++ class TQEventlistAnalysisJob+;
// <<< end TQEventlistAnalysisJob.h

// >>> begin TQEventlistPrinter.h
#pragma link C++ class TQEventlistPrinter+;
// <<< end TQEventlistPrinter.h

// >>> begin TQFilterObservable.h
#pragma link C++ class TQFilterObservable+;
// <<< end TQFilterObservable.h

// >>> begin TQFlags.h
// <<< end TQFlags.h

// >>> begin TQFolder.h
#pragma link C++ class TQFolder+;
// <<< end TQFolder.h

// >>> begin TQGraphMakerAnalysisJob.h
#pragma link C++ class TQGraphMakerAnalysisJob+;
// <<< end TQGraphMakerAnalysisJob.h

// >>> begin TQGridScanBound.h
#pragma link C++ class TQGridScanBound+;
#pragma link C++ class TQGridScanBounds+;
// <<< end TQGridScanBound.h

// >>> begin TQGridScanObservable.h
#pragma link C++ class TQGridScanObservable+;
#pragma link C++ class TQHistParams+;
#pragma link C++ class TQGridScanNormalObservable+;
#pragma link C++ class TQGridScanSplitObservable+;
// <<< end TQGridScanObservable.h

// >>> begin TQGridScanPoint.h
#pragma link C++ class TQGridScanPoint+;
// <<< end TQGridScanPoint.h

// >>> begin TQGridScanResults.h
#pragma link C++ class TQGridScanResults+;
// <<< end TQGridScanResults.h

// >>> begin TQGridScanStyle.h
#pragma link C++ namespace TQGridScanStyle;
// <<< end TQGridScanStyle.h

// >>> begin TQGridScanner.h
#pragma link C++ class TQGridScanner+;
// <<< end TQGridScanner.h

// >>> begin TQHWWPlotter.h
#pragma link C++ class TQHWWPlotter+;
#pragma link C++ typedef TQHWWPlotter2;
// <<< end TQHWWPlotter.h

// >>> begin TQHistComparer.h
#pragma link C++ class TQHistComparer+;
// <<< end TQHistComparer.h

// >>> begin TQHistoMakerAnalysisJob.h
#pragma link C++ class TQHistoMakerAnalysisJob+;
// <<< end TQHistoMakerAnalysisJob.h

// >>> begin TQHistogramObservable.h
#pragma link C++ typedef TQTH1Observable;
#pragma link C++ typedef TQTH2Observable;
#pragma link C++ typedef TQTH3Observable;
#pragma extra_include "TH1.h";
#pragma extra_include "TH2.h";
#pragma extra_include "TH3.h";
#pragma link C++ class TQHistogramObservable<TH1>+;
#pragma link C++ class TQHistogramObservable<TH2>+;
#pragma link C++ class TQHistogramObservable<TH3>+;
// <<< end TQHistogramObservable.h

// >>> begin TQHistogramUtils.h
#pragma link C++ namespace TQHistogramUtils;
// <<< end TQHistogramUtils.h

// >>> begin TQImportLink.h
#pragma link C++ class TQImportLink+;
// <<< end TQImportLink.h

// >>> begin TQIterator.h
#pragma link C++ typedef TQIterator;
#pragma link C++ typedef TQValueIterator;
#pragma link C++ typedef TQCounterIterator;
#pragma link C++ typedef TQCutIterator;
#pragma link C++ typedef TQStringIterator;
#pragma link C++ typedef TQFolderIterator;
#pragma link C++ typedef TQSampleFolderIterator;
#pragma link C++ typedef TQSampleIterator;
#pragma link C++ typedef TQTH1Iterator;
#pragma link C++ typedef TQTaggableIterator;
#pragma link C++ typedef TQObservableIterator;
#pragma link C++ typedef TQAnalysisJobIterator;
#pragma link C++ typedef TQGraphErrorsIterator;
#pragma link C++ typedef TQGraphIterator;
#pragma link C++ typedef TQGraphAsymmErrorsIterator;
#pragma link C++ typedef TQCollectionIterator;
#pragma link C++ typedef TQListIterator;
#pragma link C++ typedef TQLegendEntryIterator;
#pragma link C++ typedef TQBranchIterator;
#pragma link C++ typedef TQKeyIterator;
// <<< end TQIterator.h

// >>> begin TQIteratorT.h
//////////////////////-*- mode: c++ -*-////////////////////////////////
//
// IMPORTANT NOTICE for adding custom class iterators:
//
// If you want to add a typedef and/or be able to access a TQ...Iterator
// fromt the ROOT shell, you need to undertake the following steps:
//    - add a line 
//          typedef TQIteratorT<MyClass> TQMyClassIterator; 
//      to TQIterator.h
//    - add an include statement
//          #include <MyClass.h> 
//      in the 'include'-section of TQIteratorT.h
//    - add the line 
//          #pragma link C++ class TQIteratorT<TQValue>+;
//      to TQIteratorTLinkDef.h.add
// 
//////////////////////////////////////////////////////////////////////

#pragma link C++ class TQIteratorT<TObject>+;
#pragma link C++ class TQIteratorT<TList>+;
#pragma link C++ class TQIteratorT<TCollection>+;
#pragma link C++ class TQIteratorT<TObjArray>+;
#pragma link C++ class TQIteratorT<TObjString>+;
#pragma link C++ class TQIteratorT<TH1>+;
#pragma link C++ class TQIteratorT<TH2>+;
#pragma link C++ class TQIteratorT<TQPCA>+;
#pragma link C++ class TQIteratorT<TQCounter>+;
#pragma link C++ class TQIteratorT<TQCounterGrid>+;
#pragma link C++ class TQIteratorT<TQFolder>+;
#pragma link C++ class TQIteratorT<TQSampleFolder>+;
#pragma link C++ class TQIteratorT<TQSample>+;
#pragma link C++ class TQIteratorT<TQValue>+;
#pragma link C++ class TQIteratorT<TQNamedTaggable>+;
#pragma link C++ class TQIteratorT<TQCompiledCut>+;
#pragma link C++ class TQIteratorT<TQAnalysisJob>+;
#pragma link C++ class TQIteratorT<TGraphErrors>+;
#pragma link C++ class TQIteratorT<TGraph>+;
#pragma link C++ class TQIteratorT<TGraphAsymmErrors>+;
#pragma link C++ class TQIteratorT<TQObservable>+;
#pragma link C++ class TQIteratorT<TQCut>+;
#pragma link C++ class TQIteratorT<TList>+;
#pragma link C++ class TQIteratorT<TCollection>+;
#pragma link C++ class TQIteratorT<TLegendEntry>+;
#pragma link C++ class TQIteratorT<TBranch>+;
#pragma link C++ class TQIteratorT<TKey>+;
// <<< end TQIteratorT.h

// >>> begin TQLibrary.h
#pragma link C++ class TQLibrary+;
// <<< end TQLibrary.h

// >>> begin TQLink.h
#pragma link C++ class TQLink+;
// <<< end TQLink.h

// >>> begin TQListUtils.h
#pragma link C++ namespace TQListUtils;
// <<< end TQListUtils.h

// >>> begin TQMVA.h
#pragma link C++ class TQMVA+;
#include "RVersion.h"
#pragma extra_include "TMVA/Factory.h";
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,07,00)
#pragma extra_include "TMVA/DataSetInfo.h";
#endif
// <<< end TQMVA.h

// >>> begin TQMVAObservable.h
#pragma link C++ class TQMVAObservable+;
// <<< end TQMVAObservable.h

// >>> begin TQMessageStream.h
#pragma link C++ class TQMessageStream+;
// <<< end TQMessageStream.h

// >>> begin TQMultiChannelAnalysisSampleVisitor.h
#pragma link C++ class TQMultiChannelAnalysisSampleVisitor+;
// <<< end TQMultiChannelAnalysisSampleVisitor.h

// >>> begin TQMultiObservable.h
#pragma link C++ class TQMultiObservable+;
// <<< end TQMultiObservable.h

// >>> begin TQNFBase.h
#pragma link C++ class TQNFBase+;
// <<< end TQNFBase.h

// >>> begin TQNFCalculator.h
#pragma link C++ class TQNFCalculator+;
// <<< end TQNFCalculator.h

// >>> begin TQNFChainloader.h
#pragma link C++ class TQNFChainloader+;
// <<< end TQNFChainloader.h

// >>> begin TQNFCustomCalculator.h
#pragma link C++ class TQNFCustomCalculator+;
// <<< end TQNFCustomCalculator.h

// >>> begin TQNFManualSetter.h
#pragma link C++ class TQNFManualSetter+;
// <<< end TQNFManualSetter.h

// >>> begin TQNFTop0jetEstimator.h
#pragma link C++ class TQNFTop0jetEstimator+;
// <<< end TQNFTop0jetEstimator.h

// >>> begin TQNFTop1jetEstimator.h
#pragma link C++ class TQNFTop1jetEstimator+;
// <<< end TQNFTop1jetEstimator.h

// >>> begin TQNFUncertaintyScaler.h
#pragma link C++ class TQNFUncertaintyScaler+;
// <<< end TQNFUncertaintyScaler.h

// >>> begin TQNTupleDumperAnalysisJob.h
#pragma link C++ namespace TQNTupleDumperAnalysisJobHelpers;
#pragma link C++ class TQNTupleDumperAnalysisJob+;
// <<< end TQNTupleDumperAnalysisJob.h

// >>> begin TQNamedTaggable.h
#pragma link C++ class TQNamedTaggable+;
// <<< end TQNamedTaggable.h

// >>> begin TQObservable.h
#pragma link C++ namespace TQObservableFactory;
#pragma link C++ class TQObservable+;
// <<< end TQObservable.h

// >>> begin TQPCA.h
#pragma link C++ class TQPCA+;
// <<< end TQPCA.h

// >>> begin TQPCAAnalysisJob.h
#pragma link C++ class TQPCAAnalysisJob+;
// <<< end TQPCAAnalysisJob.h

// >>> begin TQPathManager.h
#pragma link C++ class TQPathManager+;
// <<< end TQPathManager.h

// >>> begin TQPlotter.h
#pragma link C++ class TQPlotter+;
// <<< end TQPlotter.h

// >>> begin TQPresenter.h
#pragma link C++ class TQPresenter+;
// <<< end TQPresenter.h

// >>> begin TQROOTPlotter.h
#pragma link C++ class TQROOTPlotter+;
// <<< end TQROOTPlotter.h

// >>> begin TQSample.h
#pragma link C++ class TQSample+;
// <<< end TQSample.h

// >>> begin TQSampleDataReader.h
#pragma link C++ class TQSampleDataReader+;
// <<< end TQSampleDataReader.h

// >>> begin TQSampleFolder.h
#pragma link C++ class TQSampleFolder+;
#pragma read sourceClass="TQSampleFolder" version="[1]" targetClass="TQSampleFolder"  source="" target="" code="{ newObj->convertLegacyNFs(); }"// <<< end TQSampleFolder.h

// >>> begin TQSampleGroupingVisitor.h
#pragma link C++ class TQSampleGroupingVisitor+;
// <<< end TQSampleGroupingVisitor.h

// >>> begin TQSampleInitializer.h
#pragma link C++ class TQSampleInitializer+;
// <<< end TQSampleInitializer.h

// >>> begin TQSampleInitializerBase.h
#pragma link C++ class TQSampleInitializerBase+;
// <<< end TQSampleInitializerBase.h

// >>> begin TQSampleNormalizationObservable.h
#pragma link C++ class TQSampleNormalizationObservable+;
// <<< end TQSampleNormalizationObservable.h

// >>> begin TQSamplePurger.h
#pragma link C++ class TQSamplePurger+;
// <<< end TQSamplePurger.h

// >>> begin TQSampleRevisitor.h
#pragma link C++ class TQSampleRevisitor+;
// <<< end TQSampleRevisitor.h

// >>> begin TQSampleVisitor.h
#pragma link C++ class TQSampleVisitor+;
// <<< end TQSampleVisitor.h

// >>> begin TQSignificanceEvaluator.h
#pragma link C++ class TQSignificanceEvaluator+;
#pragma link C++ class TQSignificanceEvaluatorBase+;
#pragma link C++ class TQSimpleSignificanceEvaluator+;
#pragma link C++ class TQSimpleSignificanceEvaluator2+;
#pragma link C++ class TQSimpleSignificanceEvaluator3+;
#pragma link C++ class TQPoissonSignificanceEvaluator+;
// <<< end TQSignificanceEvaluator.h

// >>> begin TQStringUtils.h
#pragma link C++ namespace TQStringUtils;
// <<< end TQStringUtils.h

// >>> begin TQSystematics.h
#pragma link C++ namespace TQSystematics;
// <<< end TQSystematics.h

// >>> begin TQSystematicsHandler.h
#pragma link C++ class TQSystematicsHandler+;
// <<< end TQSystematicsHandler.h

// >>> begin TQSystematicsManager.h
#pragma link C++ class TQSystematicsManager+;
// <<< end TQSystematicsManager.h

// >>> begin TQTHnBaseMakerAnalysisJob.h
#pragma link C++ class TQTHnBaseMakerAnalysisJob+;
// <<< end TQTHnBaseMakerAnalysisJob.h

// >>> begin TQTHnBaseUtils.h
#pragma link C++ namespace TQTHnBaseUtils;
// <<< end TQTHnBaseUtils.h

// >>> begin TQTable.h
#pragma link C++ class TQTable+;
// <<< end TQTable.h

// >>> begin TQTaggable.h
#pragma link C++ class TQTaggable+;
#pragma read sourceClass="TQTaggable" version="[1]" targetClass="TQTaggable" \
 source="TList* fTags" target="fTags" \
 code="{ fTags = TQTaggable::makeListOfTags(onfile.fTags); }"
// <<< end TQTaggable.h

// >>> begin TQTikZPlotter.h
#pragma link C++ class TQTikZPlotter+;
// <<< end TQTikZPlotter.h

// >>> begin TQToken.h
#pragma link C++ class TQToken+;
// <<< end TQToken.h

// >>> begin TQTreeFormulaObservable.h
#pragma link C++ class TQTreeFormulaObservable+;
// <<< end TQTreeFormulaObservable.h

// >>> begin TQTreeObservable.h
#pragma link C++ class TQTreeObservable+;
// <<< end TQTreeObservable.h

// >>> begin TQUniqueCut.h
#pragma link C++ class TQUniqueCut+;
// <<< end TQUniqueCut.h

// >>> begin TQUtils.h
#pragma link C++ namespace TQUtils;
// <<< end TQUtils.h

// >>> begin TQValue.h
#pragma link C++ class TQValue+;
#pragma link C++ class TQValueDouble+;
#pragma link C++ class TQValueInteger+;
#pragma link C++ class TQValueBool+;
#pragma link C++ class TQValueString+;
// <<< end TQValue.h

// >>> begin TQVectorAuxObservable.h
#pragma link C++ class TQVectorAuxObservable+;
// <<< end TQVectorAuxObservable.h

// >>> begin TQXSecParser.h
#pragma link C++ class TQXSecParser+;
// <<< end TQXSecParser.h


#endif //__CINT__
#endif //__QFrameworkDICT__

// Extra includes
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMVA/Factory.h"
#include "TMVA/DataSetInfo.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TQABCDCalculator", payloadCode, "@",
"TQAlgorithm", payloadCode, "@",
"TQAnalysisAlgorithm", payloadCode, "@",
"TQAnalysisJob", payloadCode, "@",
"TQAnalysisJobIterator", payloadCode, "@",
"TQAnalysisSampleVisitor", payloadCode, "@",
"TQAnalysisSampleVisitorBase", payloadCode, "@",
"TQBranchIterator", payloadCode, "@",
"TQCollectionIterator", payloadCode, "@",
"TQCompPlotter", payloadCode, "@",
"TQCompiledCut", payloadCode, "@",
"TQConfigReader", payloadCode, "@",
"TQConstObservable", payloadCode, "@",
"TQConstTreeObservable", payloadCode, "@",
"TQCounter", payloadCode, "@",
"TQCounterIterator", payloadCode, "@",
"TQCut", payloadCode, "@",
"TQCutFactory", payloadCode, "@",
"TQCutIterator", payloadCode, "@",
"TQCutflowAnalysisJob", payloadCode, "@",
"TQCutflowPlotter", payloadCode, "@",
"TQCutflowPrinter", payloadCode, "@",
"TQDefaultPlotter", payloadCode, "@",
"TQEventIndexObservable", payloadCode, "@",
"TQEventlistAnalysisJob", payloadCode, "@",
"TQEventlistPrinter", payloadCode, "@",
"TQFilterObservable", payloadCode, "@",
"TQFolder", payloadCode, "@",
"TQFolderIterator", payloadCode, "@",
"TQGraphAsymmErrorsIterator", payloadCode, "@",
"TQGraphErrorsIterator", payloadCode, "@",
"TQGraphIterator", payloadCode, "@",
"TQGraphMakerAnalysisJob", payloadCode, "@",
"TQGridScanBound", payloadCode, "@",
"TQGridScanBounds", payloadCode, "@",
"TQGridScanNormalObservable", payloadCode, "@",
"TQGridScanObservable", payloadCode, "@",
"TQGridScanPoint", payloadCode, "@",
"TQGridScanResults", payloadCode, "@",
"TQGridScanSplitObservable", payloadCode, "@",
"TQGridScanner", payloadCode, "@",
"TQHWWPlotter", payloadCode, "@",
"TQHWWPlotter2", payloadCode, "@",
"TQHistComparer", payloadCode, "@",
"TQHistParams", payloadCode, "@",
"TQHistoMakerAnalysisJob", payloadCode, "@",
"TQHistogramObservable<TH1>", payloadCode, "@",
"TQHistogramObservable<TH2>", payloadCode, "@",
"TQHistogramObservable<TH3>", payloadCode, "@",
"TQImportLink", payloadCode, "@",
"TQIterator", payloadCode, "@",
"TQIteratorT<TBranch>", payloadCode, "@",
"TQIteratorT<TCollection>", payloadCode, "@",
"TQIteratorT<TGraph>", payloadCode, "@",
"TQIteratorT<TGraphAsymmErrors>", payloadCode, "@",
"TQIteratorT<TGraphErrors>", payloadCode, "@",
"TQIteratorT<TH1>", payloadCode, "@",
"TQIteratorT<TH2>", payloadCode, "@",
"TQIteratorT<TKey>", payloadCode, "@",
"TQIteratorT<TLegendEntry>", payloadCode, "@",
"TQIteratorT<TList>", payloadCode, "@",
"TQIteratorT<TObjArray>", payloadCode, "@",
"TQIteratorT<TObjString>", payloadCode, "@",
"TQIteratorT<TObject>", payloadCode, "@",
"TQIteratorT<TQAnalysisJob>", payloadCode, "@",
"TQIteratorT<TQCompiledCut>", payloadCode, "@",
"TQIteratorT<TQCounter>", payloadCode, "@",
"TQIteratorT<TQCounterGrid>", payloadCode, "@",
"TQIteratorT<TQCut>", payloadCode, "@",
"TQIteratorT<TQFolder>", payloadCode, "@",
"TQIteratorT<TQNamedTaggable>", payloadCode, "@",
"TQIteratorT<TQObservable>", payloadCode, "@",
"TQIteratorT<TQPCA>", payloadCode, "@",
"TQIteratorT<TQSample>", payloadCode, "@",
"TQIteratorT<TQSampleFolder>", payloadCode, "@",
"TQIteratorT<TQValue>", payloadCode, "@",
"TQKeyIterator", payloadCode, "@",
"TQLegendEntryIterator", payloadCode, "@",
"TQLibrary", payloadCode, "@",
"TQLink", payloadCode, "@",
"TQListIterator", payloadCode, "@",
"TQMVA", payloadCode, "@",
"TQMVAObservable", payloadCode, "@",
"TQMessageStream", payloadCode, "@",
"TQMultiChannelAnalysisSampleVisitor", payloadCode, "@",
"TQMultiObservable", payloadCode, "@",
"TQNFBase", payloadCode, "@",
"TQNFCalculator", payloadCode, "@",
"TQNFChainloader", payloadCode, "@",
"TQNFCustomCalculator", payloadCode, "@",
"TQNFManualSetter", payloadCode, "@",
"TQNFTop0jetEstimator", payloadCode, "@",
"TQNFTop1jetEstimator", payloadCode, "@",
"TQNFUncertaintyScaler", payloadCode, "@",
"TQNTupleDumperAnalysisJob", payloadCode, "@",
"TQNamedTaggable", payloadCode, "@",
"TQObservable", payloadCode, "@",
"TQObservableIterator", payloadCode, "@",
"TQPCA", payloadCode, "@",
"TQPCAAnalysisJob", payloadCode, "@",
"TQPathManager", payloadCode, "@",
"TQPlotter", payloadCode, "@",
"TQPoissonSignificanceEvaluator", payloadCode, "@",
"TQPresenter", payloadCode, "@",
"TQROOTPlotter", payloadCode, "@",
"TQSample", payloadCode, "@",
"TQSampleDataReader", payloadCode, "@",
"TQSampleFolder", payloadCode, "@",
"TQSampleFolderIterator", payloadCode, "@",
"TQSampleGroupingVisitor", payloadCode, "@",
"TQSampleInitializer", payloadCode, "@",
"TQSampleInitializerBase", payloadCode, "@",
"TQSampleIterator", payloadCode, "@",
"TQSampleNormalizationObservable", payloadCode, "@",
"TQSamplePurger", payloadCode, "@",
"TQSampleRevisitor", payloadCode, "@",
"TQSampleVisitor", payloadCode, "@",
"TQSignificanceEvaluator", payloadCode, "@",
"TQSignificanceEvaluatorBase", payloadCode, "@",
"TQSimpleSignificanceEvaluator", payloadCode, "@",
"TQSimpleSignificanceEvaluator2", payloadCode, "@",
"TQSimpleSignificanceEvaluator3", payloadCode, "@",
"TQStringIterator", payloadCode, "@",
"TQSystematicsHandler", payloadCode, "@",
"TQSystematicsManager", payloadCode, "@",
"TQTH1Iterator", payloadCode, "@",
"TQTH1Observable", payloadCode, "@",
"TQTH2Observable", payloadCode, "@",
"TQTH3Observable", payloadCode, "@",
"TQTHnBaseMakerAnalysisJob", payloadCode, "@",
"TQTable", payloadCode, "@",
"TQTaggable", payloadCode, "@",
"TQTaggableIterator", payloadCode, "@",
"TQTikZPlotter", payloadCode, "@",
"TQToken", payloadCode, "@",
"TQTreeFormulaObservable", payloadCode, "@",
"TQTreeObservable", payloadCode, "@",
"TQUniqueCut", payloadCode, "@",
"TQValue", payloadCode, "@",
"TQValueBool", payloadCode, "@",
"TQValueDouble", payloadCode, "@",
"TQValueInteger", payloadCode, "@",
"TQValueIterator", payloadCode, "@",
"TQValueString", payloadCode, "@",
"TQVectorAuxObservable", payloadCode, "@",
"TQXSecParser", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libQFramework",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libQFramework_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libQFramework_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libQFramework() {
  TriggerDictionaryInitialization_libQFramework_Impl();
}
