#include "RooAbsReal.h"
#include "RooWorkspace.h"

#include "SFramework/TSUtils.h"
#include "QFramework/TQFolder.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQStringUtils.h"

#include "RooRealVar.h"
#include "RooRealSumPdf.h"
#include "RooProdPdf.h"
#include "RooProduct.h"
#include "RooMultiVarGaussian.h"

#include "RooCategory.h"
#include "RooFitResult.h"
#include "RooConstVar.h"
#include "RooSimultaneous.h"
#include "RooDataSet.h"
#include "RooLinkedList.h"
#include "RooStats/ModelConfig.h"
#include "RooAbsDataStore.h"

#include "Fit/FitConfig.h"

#include "RooHistFunc.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooSimultaneous.h"
#include "RooMinimizerFcn.h"
#include "RooStats/HistFactory/PiecewiseInterpolation.h"
//#include "RooStats/HistFactory/FlexibleInterpVar.h" //already included in the header (reason: ROOT/Cling/dictionaries)

#include "TH2.h"
#include "TH2F.h"
#include "TH1.h"

#include "TMath.h"
#include "TMatrixD.h"
#include "TDecompLU.h"
#include "TMatrixDSym.h"

#include <assert.h>
#include <utility>
#include <limits>

#include "QFramework/TQLibrary.h"

#include "SFramework/TSIterator.h"
#include "SFramework/TSStatisticsManager.h"

// iterator helpers for older ROOT versions

#if ROOT_VERSION_CODE < ROOT_VERSION(6,18,0)
RooAbsCollection_IteratorHelper::RooAbsCollection_IteratorHelper(const RooAbsCollection& c, bool end) : itr(c.fwdIterator()), nextItem(end ? NULL : itr.next()) {}
RooAbsArg* RooAbsCollection_IteratorHelper::operator++(){ nextItem = itr.next(); return nextItem; }
bool RooAbsCollection_IteratorHelper::operator!=(const RooAbsCollection_IteratorHelper& other){ return this->nextItem != other.nextItem; }
bool RooAbsCollection_IteratorHelper::operator!=(const RooAbsArg* other){  return this->nextItem != other; }
RooAbsArg* RooAbsCollection_IteratorHelper::operator*(){ return nextItem; }
RooAbsCollection_IteratorHelper begin(const RooAbsCollection& c){
  return RooAbsCollection_IteratorHelper(c,false);
}
RooAbsCollection_IteratorHelper end(const RooAbsCollection& c){
  return RooAbsCollection_IteratorHelper(c,true);
}
#endif
RooLinkedList_IteratorHelper::RooLinkedList_IteratorHelper(const RooLinkedList& c, bool end) : itr(c.fwdIterator()), nextItem(end ? NULL : itr.next()) {}
RooAbsArg* RooLinkedList_IteratorHelper::operator++(){ nextItem = itr.next(); return nextItem; }
bool RooLinkedList_IteratorHelper::operator!=(const RooLinkedList_IteratorHelper& other){ return this->nextItem != other.nextItem; }
bool RooLinkedList_IteratorHelper::operator!=(const RooAbsArg* other){  return this->nextItem != other; }
RooAbsArg* RooLinkedList_IteratorHelper::operator*(){ return nextItem; }
RooLinkedList_IteratorHelper begin(const RooLinkedList& c){
  return RooLinkedList_IteratorHelper(c,false);
}
RooLinkedList_IteratorHelper end(const RooLinkedList& c){
  return RooLinkedList_IteratorHelper(c,true);
}
RooFIter_IteratorHelper::RooFIter_IteratorHelper(RooFIter& it, bool end) : itr(&it), nextItem(end ? NULL : itr->next()) {}
RooAbsArg* RooFIter_IteratorHelper::operator++(){ nextItem = itr->next(); return nextItem; }
bool RooFIter_IteratorHelper::operator!=(const RooFIter_IteratorHelper& other){ return this->nextItem != other.nextItem; }
bool RooFIter_IteratorHelper::operator!=(const RooAbsArg* other){  return this->nextItem != other; }
RooAbsArg* RooFIter_IteratorHelper::operator*(){ return nextItem; }
RooFIter_IteratorHelper begin(RooFIter& it){
  return RooFIter_IteratorHelper(it,false);
}
RooFIter_IteratorHelper end(RooFIter& it){
  return RooFIter_IteratorHelper(it,true);
}
TIterator_IteratorHelper::TIterator_IteratorHelper(TIterator* it, bool end) : itr(it), nextItem(end ? NULL : itr->Next()) {}
TObject* TIterator_IteratorHelper::operator++(){ nextItem = itr->Next(); return nextItem; }
bool TIterator_IteratorHelper::operator!=(const TIterator_IteratorHelper& other){ return this->nextItem != other.nextItem; }
bool TIterator_IteratorHelper::operator!=(const TObject* other){  return this->nextItem != other; }
TObject* TIterator_IteratorHelper::operator*(){ return nextItem; }
TIterator_IteratorHelper begin(TIterator* it){
  return TIterator_IteratorHelper(it,false);
}
TIterator_IteratorHelper end(TIterator* it){
  return TIterator_IteratorHelper(it,true);
}



//we need to free some members from their private/protected prison since there are no getters implemented for them:
//RooAbsReal::_plotMin
//RooAbsReal::_plotBins
//RooWorkspace::_snapshots
//RooFitResult::_CM

#define nan std::numeric_limits<double>::quiet_NaN()
#define inf std::numeric_limits<double>::infinity()

namespace {
  //c++ standard conform alternative to a public-private hack:
  //freeing members from RooAbsReal:
  template <typename RooAbsRealTag>
  struct RooAbsRealHackResult {
    typedef typename RooAbsRealTag::type type;
    static type ptr;
  };

  template <typename RooAbsRealTag>
  typename RooAbsRealHackResult<RooAbsRealTag>::type RooAbsRealHackResult<RooAbsRealTag>::ptr;

  template<typename RooAbsRealTag, typename RooAbsRealTag::type p>
  struct RooAbsRealRob : RooAbsRealHackResult<RooAbsRealTag> {
    struct RooAbsRealFiller {
      RooAbsRealFiller() {RooAbsRealHackResult<RooAbsRealTag>::ptr = p;}
    };
    static RooAbsRealFiller rooabsrealfiller_obj;
  };

  template<typename RooAbsRealTag, typename RooAbsRealTag::type p>
  typename RooAbsRealRob<RooAbsRealTag, p>::RooAbsRealFiller RooAbsRealRob<RooAbsRealTag, p>::rooabsrealfiller_obj;

  //now expose some members of RooAbsReal that we need to access
  struct RooAbsReal_plotMin { typedef Double_t(RooAbsReal::*type); };
  template class RooAbsRealRob<RooAbsReal_plotMin, &RooAbsReal::_plotMin>;

  struct RooAbsReal_plotMax { typedef Double_t(RooAbsReal::*type); };
  template class RooAbsRealRob<RooAbsReal_plotMax, &RooAbsReal::_plotMax>;

  struct RooAbsReal_plotBins { typedef Int_t(RooAbsReal::*type); };
  template class RooAbsRealRob<RooAbsReal_plotBins, &RooAbsReal::_plotBins>;

  //freeing members from RooWorkspace:
  template <typename RooWorkspaceTag>
  struct RooWorkspaceHackResult {
    typedef typename RooWorkspaceTag::type type;
    static type ptr;
  };

  template <typename RooWorkspaceTag>
  typename RooWorkspaceHackResult<RooWorkspaceTag>::type RooWorkspaceHackResult<RooWorkspaceTag>::ptr;

  template<typename RooWorkspaceTag, typename RooWorkspaceTag::type p>
  struct RooWorkspaceRob : RooWorkspaceHackResult<RooWorkspaceTag> {
    struct RooWorkspaceFiller {
      RooWorkspaceFiller() {RooWorkspaceHackResult<RooWorkspaceTag>::ptr = p;}
    };
    static RooWorkspaceFiller rooworkspacefiller_obj;
  };

  template<typename RooWorkspaceTag, typename RooWorkspaceTag::type p>
  typename RooWorkspaceRob<RooWorkspaceTag, p>::RooWorkspaceFiller RooWorkspaceRob<RooWorkspaceTag, p>::rooworkspacefiller_obj;

  //now expose some members of TAxis that we need to access
  struct RooWorkspace_snapshots { typedef RooLinkedList(RooWorkspace::*type); };
  template class RooWorkspaceRob<RooWorkspace_snapshots, &RooWorkspace::_snapshots>;
  struct RooWorkspace_namedSets { typedef std::map<std::string, RooArgSet>(RooWorkspace::*type); };
  template class RooWorkspaceRob<RooWorkspace_namedSets, &RooWorkspace::_namedSets>;



  // hack to get access to RooFitResult::_CM
  template <typename RooFitResultTag>
  struct RooFitResultHackResult {
    typedef typename RooFitResultTag::type type;
    static type ptr;
  };

  template <typename RooFitResultTag>
  typename RooFitResultHackResult<RooFitResultTag>::type RooFitResultHackResult<RooFitResultTag>::ptr;

  template<typename RooFitResultTag, typename RooFitResultTag::type p>
  struct RooFitResultRob : RooFitResultHackResult<RooFitResultTag> {
    struct RooFitResultFiller {
      RooFitResultFiller() {RooFitResultHackResult<RooFitResultTag>::ptr = p;}
    };
    static RooFitResultFiller roofitresultfiller_obj;
  };

  template<typename RooFitResultTag, typename RooFitResultTag::type p>
  typename RooFitResultRob<RooFitResultTag, p>::RooFitResultFiller RooFitResultRob<RooFitResultTag, p>::roofitresultfiller_obj;

  //now expose some members of RooFitResult that we need to access
  struct RooFitResultCM { typedef TMatrixDSym*(RooFitResult::*type); };
  template class RooFitResultRob<RooFitResultCM, &RooFitResult::_CM>;
  struct RooFitResultVM { typedef TMatrixDSym*(RooFitResult::*type); };
  template class RooFitResultRob<RooFitResultVM, &RooFitResult::_VM>;
}


namespace {
  //somewhat complex but apparently standard conform hack to access
  template <typename RooMinimizerTag>
  struct RooMinimizerHackResult {
    typedef typename RooMinimizerTag::type type;
    static type ptr;
  };

  template <typename RooMinimizerTag>
  typename RooMinimizerHackResult<RooMinimizerTag>::type RooMinimizerHackResult<RooMinimizerTag>::ptr;

  template<typename RooMinimizerTag, typename RooMinimizerTag::type p>
  struct RooMinimizerRob : RooMinimizerHackResult<RooMinimizerTag> {
    struct RooMinimizerFiller {
      RooMinimizerFiller() {RooMinimizerHackResult<RooMinimizerTag>::ptr = p;}
    };
    static RooMinimizerFiller RooMinimizerfiller_obj;
  };

  template<typename RooMinimizerTag, typename RooMinimizerTag::type p>
  typename RooMinimizerRob<RooMinimizerTag, p>::RooMinimizerFiller RooMinimizerRob<RooMinimizerTag, p>::RooMinimizerfiller_obj;

  //now expose some members of RooMinimizer that we need to access
  struct RooMinimizer_getNPar      { typedef Int_t           (RooMinimizer::*type)() const; };
  template class RooMinimizerRob<RooMinimizer_getNPar,      &RooMinimizer::getNPar  >;
}

namespace {
  //somewhat complex but apparently standard conform hack to access
  template <typename RooProdPdfTag>
  struct RooProdPdfHackResult {
    typedef typename RooProdPdfTag::type type;
    static type ptr;
  };

  template <typename RooProdPdfTag>
  typename RooProdPdfHackResult<RooProdPdfTag>::type RooProdPdfHackResult<RooProdPdfTag>::ptr;

  template<typename RooProdPdfTag, typename RooProdPdfTag::type p>
  struct RooProdPdfRob : RooProdPdfHackResult<RooProdPdfTag> {
    struct RooProdPdfFiller {
      RooProdPdfFiller() {RooProdPdfHackResult<RooProdPdfTag>::ptr = p;}
    };
    static RooProdPdfFiller RooProdPdffiller_obj;
  };

  template<typename RooProdPdfTag, typename RooProdPdfTag::type p>
  typename RooProdPdfRob<RooProdPdfTag, p>::RooProdPdfFiller RooProdPdfRob<RooProdPdfTag, p>::RooProdPdffiller_obj;

  //now expose some members of RooProdPdf that we need to access
  struct RooProdPdf_pdfList { typedef RooListProxy (RooProdPdf::*type); };
  template class RooProdPdfRob<RooProdPdf_pdfList, &RooProdPdf::_pdfList>;

  struct RooProdPdf_extendedIndex { typedef int (RooProdPdf::*type); };
  template class RooProdPdfRob<RooProdPdf_extendedIndex, &RooProdPdf::_extendedIndex>;
}



//__________________________________________________________________________________|___________

int TSUtils::nParameters(const RooMinimizer& minimizer){
  return (minimizer.*RooMinimizerHackResult<RooMinimizer_getNPar>::ptr)();
}

//__________________________________________________________________________________|___________

RooMinimizerFcn* TSUtils::getFunction(RooMinimizer& minimizer){
  return (RooMinimizerFcn*) minimizer.fitter()->GetFCN();
}

//__________________________________________________________________________________|___________

class PiecewiseInterpolationHelper : public ::PiecewiseInterpolation {
  struct Components {
    size_t index;
    RooAbsArg *low,*high,*param;
    Components(size_t index_, RooAbsArg* low_, RooAbsArg* high_, RooAbsArg* param_) : index(index_), low(low_), high(high_), param(param_) {}
  };
  public:
  static void printInfo(const PiecewiseInterpolation* interp_) {
    const PiecewiseInterpolationHelper* interp = (const PiecewiseInterpolationHelper*) interp_;
    if (!interp) return;
    std::cout<<"nominal:"; interp->_nominal.arg().Print();
    interp->_nominal.arg().Print();
    std::cout<<"components:"<<std::endl;
    RooFIter paramIter(interp->_paramSet.fwdIterator()) ;
    RooFIter highIter(interp->_highSet.fwdIterator()) ;
    RooFIter lowIter(interp->_lowSet.fwdIterator()) ;
    RooAbsArg* parAbs = nullptr;
    RooAbsArg* highAbs = nullptr;
    RooAbsArg* lowAbs = nullptr;
    while((parAbs=paramIter.next())) {
      lowAbs = lowIter.next() ;
      highAbs = highIter.next();
      std::cout<<parAbs->GetName()<<std::endl;
      std::cout<<"par : ";parAbs->Print();
      std::cout<<"high: ";highAbs->Print();
      std::cout<<"low : ";lowAbs->Print();
    }

  }
  static RooAbsArg* getNominalMember(const PiecewiseInterpolation* pw_) {
    if (!pw_) return nullptr;
    PiecewiseInterpolationHelper* pw = (PiecewiseInterpolationHelper*) pw_;
    return pw->_nominal.absArg();
  }
  //static const RooAbsArg* getNominalMember(const PiecewiseInterpolation* _interp) {
  //  const PiecewiseInterpolationHelper* interp = (const PiecewiseInterpolationHelper*) _interp;
  //  if (!interp) return nullptr;
  //  return &interp->_nominal.arg();
  //}
  static RooListProxy* getParamSetMember(const PiecewiseInterpolation* pw_) {
    if (!pw_) return nullptr;
    PiecewiseInterpolationHelper* pw = (PiecewiseInterpolationHelper*) pw_;
    return &(pw->_paramSet);
  }
  static RooListProxy* getHighSetMember(const PiecewiseInterpolation* pw_) {
    if (!pw_) return nullptr;
    PiecewiseInterpolationHelper* pw = (PiecewiseInterpolationHelper*) pw_;
    return &(pw->_highSet);
  }
  static RooListProxy* getLowSetMember(const PiecewiseInterpolation* pw_) {
    if (!pw_) return nullptr;
    PiecewiseInterpolationHelper* pw = (PiecewiseInterpolationHelper*) pw_;
    return &(pw->_lowSet);
  }
  static void removeConstituents(PiecewiseInterpolation* pw_, const TString& filter) {
    if (!pw_) return;
    PiecewiseInterpolationHelper* pw = (PiecewiseInterpolationHelper*) pw_;
    //members modified/considered in the following:
    //_nominal //don't remove, this wouldn't make sense
    //_normSet //seems to be unused in entire PiecewiseInterpolation class...

    //_ownedList //only for cleanup, no active searching

    //_lowSet
    //_highSet
    //_paramSet
    //_interpCode
    RooFIter paramIter(pw->_paramSet.fwdIterator()) ;
    RooFIter highIter(pw->_highSet.fwdIterator()) ;
    RooFIter lowIter(pw->_lowSet.fwdIterator()) ;

    std::vector<Components> toRemove;
    RooAbsArg* parAbs = nullptr;
    RooAbsArg* highAbs = nullptr;
    RooAbsArg* lowAbs = nullptr;
    size_t index = 0;
    while((parAbs=paramIter.next())) {
      lowAbs = lowIter.next() ;
      highAbs = highIter.next();
      if (TQStringUtils::matchesFilter(parAbs->GetName(),filter,",")
          || TQStringUtils::matchesFilter(highAbs->GetName(),filter,",")
          || TQStringUtils::matchesFilter(lowAbs->GetName(),filter,",")  ) {
        Components comp(index, lowAbs, highAbs, parAbs);
        toRemove.push_back(comp);
      }
      ++index;
    }
    int indexOffset = 0;
    for (Components& comp : toRemove) {
      pw->_paramSet.remove(*(comp.param));
      pw->_highSet.remove(*(comp.high));
      pw->_lowSet.remove(*(comp.low));
      pw->_ownedList.remove(*(comp.param), true);
      pw->_ownedList.remove(*(comp.high), true);
      pw->_ownedList.remove(*(comp.low), true);
      pw->_interpCode.erase(pw->_interpCode.begin() + (comp.index - indexOffset));
      ++indexOffset;
    }

  }


};
//expose to the outer world:
void TSUtils::printInfo(const PiecewiseInterpolation* _interp) {
  PiecewiseInterpolationHelper::printInfo(_interp);
}
RooAbsArg* TSUtils::getNominalMember(const PiecewiseInterpolation* _interp) {
  return PiecewiseInterpolationHelper::getNominalMember(_interp);
}
RooListProxy* TSUtils::getParamSetMember(const PiecewiseInterpolation* _interp) {
  return PiecewiseInterpolationHelper::getParamSetMember(_interp);
}
RooListProxy* TSUtils::getHighSetMember(const PiecewiseInterpolation* _interp) {
  return PiecewiseInterpolationHelper::getHighSetMember(_interp);
}
RooListProxy* TSUtils::getLowSetMember(const PiecewiseInterpolation* _interp) {
  return PiecewiseInterpolationHelper::getLowSetMember(_interp);
}
void TSUtils::removeConstituentsFromPiecewiseInterpolation(PiecewiseInterpolation* interp, const TString& filter) {
  PiecewiseInterpolationHelper::removeConstituents(interp,filter);
}

//__________________________________________________________________________________|___________

class FlexibleInterpVarHelper : RooStats::HistFactory::FlexibleInterpVar {
  public:
  static Double_t* getNominalMemberPtr(const RooStats::HistFactory::FlexibleInterpVar* interp_) {
    //exposing as pointer in case we need to modify it
    FlexibleInterpVarHelper* interp = (FlexibleInterpVarHelper*) interp_;
    if (!interp) return nullptr;
    return &(interp->_nominal);
  }

  static Double_t getNominalMember(const RooStats::HistFactory::FlexibleInterpVar* interp_) {
    //convenience wrapper to only get the plain value
    Double_t* ptr = getNominalMemberPtr(interp_);
    if (!ptr) return std::numeric_limits<double>::quiet_NaN();
    return *ptr;
  }

  static std::vector<double>* getHighMember(const RooStats::HistFactory::FlexibleInterpVar* interp_) {
    FlexibleInterpVarHelper* interp = (FlexibleInterpVarHelper*) interp_;
    if (!interp) return nullptr;
    return &(interp->_high);
  }

  static std::vector<double>* getLowMember(const RooStats::HistFactory::FlexibleInterpVar* interp_) {
    FlexibleInterpVarHelper* interp = (FlexibleInterpVarHelper*) interp_;
    if (!interp) return nullptr;
    return &(interp->_low);
  }

  static RooListProxy* getParamListMember(const RooStats::HistFactory::FlexibleInterpVar* interp_) {
    FlexibleInterpVarHelper* interp = (FlexibleInterpVarHelper*) interp_;
    if (!interp) return nullptr;
    return &(interp->_paramList);
  }

  static void removeConstituents(RooStats::HistFactory::FlexibleInterpVar* interp_, const TString& filter) {
    if (!interp_) return;
    FlexibleInterpVarHelper* interp = (FlexibleInterpVarHelper*) interp_;
    //members to modify:
    //_paramList
    //_low
    //_high
    //_interpCode
    std::vector<std::pair<size_t,RooAbsArg*>> toRemove;
    size_t index = 0;
    for(auto buff:interp->_paramList.createIterator()){
      if (buff && (TQStringUtils::matchesFilter(buff->GetName(),filter,","))) {
        std::pair<size_t,RooAbsArg*> rem(index,dynamic_cast<RooAbsArg*>(buff));
        toRemove.push_back(rem);
      }
      ++index;
    }
    int indexOffset = 0;
    for (auto& rem : toRemove) {
      interp->_interpCode.erase(interp->_interpCode.begin() + (rem.first - indexOffset));
      interp->_low.erase(interp->_low.begin() + (rem.first - indexOffset));
      interp->_high.erase(interp->_high.begin() + (rem.first - indexOffset));
      interp->_paramList.remove(*(rem.second));
      ++indexOffset; //increment index offset
    }

  }
};
  //expose to outside world:
Double_t* TSUtils::getNominalMemberPtr(const RooStats::HistFactory::FlexibleInterpVar* interp) {
  return FlexibleInterpVarHelper::getNominalMemberPtr(interp);
}
Double_t TSUtils::getNominalMember(const RooStats::HistFactory::FlexibleInterpVar* interp) {
  return FlexibleInterpVarHelper::getNominalMember(interp);
}
std::vector<double>* TSUtils::getHighMember(const RooStats::HistFactory::FlexibleInterpVar* interp) {
  return FlexibleInterpVarHelper::getHighMember(interp);
}
std::vector<double>* TSUtils::getLowMember(const RooStats::HistFactory::FlexibleInterpVar* interp) {
  return FlexibleInterpVarHelper::getLowMember(interp);
}
RooListProxy* TSUtils::getParamListMember(const RooStats::HistFactory::FlexibleInterpVar* interp) {
  return FlexibleInterpVarHelper::getParamListMember(interp);
}
void TSUtils::removeConstituentsFromFlexibleInterpVar(RooStats::HistFactory::FlexibleInterpVar* interp, const TString& filter) {
  FlexibleInterpVarHelper::removeConstituents(interp, filter);
}


//__________________________________________________________________________________|___________

class RooDataSetHelper : public ::RooDataSet {
  public:
  static void printInfo(RooDataSet* _ds) {
    RooDataSetHelper* ds = (RooDataSetHelper*) _ds;
    if (!ds) return;
    for(auto* arg:ds->_varsNoWgt){
      std::cout<<arg->GetName()<<", type: "<<arg->IsA()->GetName()<<std::endl;
      arg->Print();
    }
    ds->store()->Print();
  }
};

class RooAbsDataHelper : public ::RooAbsData {
  public:
  static const RooArgSet& getVars(const RooAbsData* _ds){
    const RooAbsDataHelper* ds = (const RooAbsDataHelper*) _ds;
    if (!ds) throw std::runtime_error("unable to obtain dataset!");
    return ds->_vars;
  }
};

void TSUtils::printInfo(RooDataSet* _ds) {
  RooDataSetHelper::printInfo(_ds);
}

const RooArgSet& TSUtils::getVariables(const RooAbsData* _ds) {
  return RooAbsDataHelper::getVars(_ds);
}


//__________________________________________________________________________________|___________

class RooDataHistHelper : public ::RooDataHist {
  public:
  static void printBins(RooDataHist* _dh) {
    RooDataHistHelper* dh = (RooDataHistHelper*) _dh;
    if (!dh) return;
    std::cout<<"DH '"<<dh->GetName()<<"':"<<std::endl;
    for (Int_t i = 0; i<dh->_arrSize; ++i) {
      std::cout<<"  bin "<<i<<": "<< dh->_wgt[i] <<" / errLo = "<< dh->_errLo[i] <<" / errHi = "<< dh->_errHi[i] <<" / sumw2 = "<< dh->_sumw2[i] <<" / binv = "<< dh->_binv[i]<<std::endl;
    }
  }
  static void scaleHist(RooDataHist* _dh, double scale) {
    RooDataHistHelper* dh = (RooDataHistHelper*) _dh;
    if (!dh) return;
    for (Int_t i = 0; i<dh->_arrSize; ++i) {
      dh->_wgt[i] *= scale;
      //dh->_errLo[i] // does not seem to be relevant
      //dh->_errHi[i] // does not seem to be relevant
      dh->_sumw2[i] *= scale*scale; //quadratic scaling
      //invalidate cache
      //dh->_curIndex = -1;
      //dh->_cache_sum_valid = false;
    }
  }
  static void scaleDifferenceToNominal(RooDataHist* _var, const RooDataHist* _nom, double scale) {
    if (!_var || !_nom) return;

    RooDataHistHelper* var = (RooDataHistHelper*) _var;
    const RooDataHistHelper* nom = (const RooDataHistHelper*) _nom;
    if (var->_arrSize != nom->_arrSize) {
      std::cout<<"Inconsistent array size between variation and nominal DataHist found, will not modify them!"<<std::endl;
    }
    for (Int_t i = 0; i<nom->_arrSize; ++i) {
      double newWeight = nom->_wgt[i] + scale * (var->_wgt[i] - nom->_wgt[i]);
      double ratio = var->_wgt[i]!= 0 ? newWeight / var->_wgt[i] : std::numeric_limits<double>::quiet_NaN();
      var->_wgt[i] = newWeight;
      //dh->_errLo[i] // does not seem to be relevant
      //dh->_errHi[i] // does not seem to be relevant
      var->_sumw2[i] *= ratio==ratio ? pow(ratio,2) : 1.; //quadratic scaling, assuming the reduction is flat accros individual weights (most likely irrelavant anyways)
      //invalidate cache
      //dh->_curIndex = -1;
      //dh->_cache_sum_valid = false;
    }

  }
};

void TSUtils::printBins(RooDataHist* _dh) {
  RooDataHistHelper::printBins(_dh);
}
void TSUtils::scaleHist(RooDataHist* _dh, double scale) {
  RooDataHistHelper::scaleHist(_dh,scale);
}
void TSUtils::scaleDifferenceToNominal(RooDataHist* _var, const RooDataHist* _nom, double scale) {
  RooDataHistHelper::scaleDifferenceToNominal(_var, _nom, scale);
}

//__________________________________________________________________________________|___________

class RooSimultaneousHelper : public ::RooSimultaneous {
  public:
  static std::vector<const RooAbsArg*>* getConstituents(const RooSimultaneous* sim) {
    if (!sim) return nullptr;
    std::vector<const RooAbsArg*>* retVec = new std::vector<const RooAbsArg*>();
    for (TObject *obj : ((const RooSimultaneousHelper*)sim)->_pdfProxyList) {
      RooRealProxy* prox = dynamic_cast<RooRealProxy*>(obj);
      if (!prox) continue;

      const RooAbsArg* p =  &(prox->arg());
      if (!p) continue;

      retVec->push_back(p);
    }
    return retVec;
  }
  static void removeConstituents(RooSimultaneous* sim, const TString& filter) {
    std::vector<RooRealProxy*> parts;
    RooSimultaneousHelper* sh = (RooSimultaneousHelper*) sim;
    for (TObject *obj : sh->_pdfProxyList) {
      RooRealProxy* prox = dynamic_cast<RooRealProxy*>(obj);
      if (!prox) continue;
      if (!TQStringUtils::matchesFilter(prox->arg().GetName(),filter,",")) continue;
      parts.push_back(prox);
    }
    for (RooRealProxy* const& part: parts) {
      sh->_pdfProxyList.Remove(part);
    }
  }
};

//std::vector<const RooAbsArg*>* TSUtils::getConstituents(const RooSimultaneous* sim) {
//  return RooSimultaneousHelper::getConstituents(sim);
//}

void TSUtils::removeConstituentsFromRooSimultaneous(RooSimultaneous* sim, const TString& filter) {
  RooSimultaneousHelper::removeConstituents(sim,filter);
}
//__________________________________________________________________________________|___________

class RooHistFuncHelper : public RooHistFunc {
  public:
  static RooDataHist* getDataHist(const RooHistFunc* h) {
    if (!h) return nullptr;
    return ((const RooHistFuncHelper*)h)->_dataHist;
  }
};

RooDataHist* TSUtils::getDataHist(const RooHistFunc* h) {
  return RooHistFuncHelper::getDataHist(h);
}

//__________________________________________________________________________________|___________

class RooRealSumPdfHelper : public RooRealSumPdf {
  public:
  static void removeConstituents(RooRealSumPdf* pdf, const TString& filter) {
    if (!pdf) return;
    std::vector<RooAbsArg*> parts;
    RooRealSumPdfHelper* helper = (RooRealSumPdfHelper*) pdf;
    for(auto arg:helper->_funcList.fwdIterator()){
      if (!arg) continue;
      if (!TQStringUtils::matchesFilter(arg->GetName(),filter,",")) continue;
      parts.push_back(arg);
    }

    for (RooAbsArg* const& part: parts) {
      helper->_funcList.RecursiveRemove(part);
    }
  }

};

void TSUtils::removeConstituentsFromRooRealSumPdf(RooRealSumPdf* pdf, const TString& filter) {
  RooRealSumPdfHelper::removeConstituents(pdf,filter);
}

//__________________________________________________________________________________|___________

namespace {
  void removeConstituents(RooProdPdf* pdf, const TString& filter) {
    if (!pdf) return;
    std::vector<RooAbsArg*> parts;
    for(auto* arg:(*pdf).*RooProdPdfHackResult<RooProdPdf_pdfList>::ptr){
      if (!arg) continue;
      if (!TQStringUtils::matchesFilter(arg->GetName(),filter,",")) continue;
      parts.push_back(arg);
    }
    
    for (RooAbsArg* const& part: parts) {
      int index = ((*pdf).*RooProdPdfHackResult<RooProdPdf_pdfList>::ptr).index(part);
      if (((*pdf).*RooProdPdfHackResult<RooProdPdf_extendedIndex>::ptr) == index) { //if this element was the "extendible" sub-component we need to invalidate this index to prevent segfaults
        //std::cout<<"Modifying extendedIndex"<<std::endl;
        (((*pdf).*RooProdPdfHackResult<RooProdPdf_extendedIndex>::ptr)) = -1;
      } else if (((*pdf).*RooProdPdfHackResult<RooProdPdf_extendedIndex>::ptr) > index) {//reduce the index by 1 if we remove an element which is at a lower position
        --((*pdf).*RooProdPdfHackResult<RooProdPdf_extendedIndex>::ptr);
        //std::cout<<"ExtendedIndex="<<helper->_extendedIndex<<", listIndex="<<helper->_pdfList.index(part)<<std::endl;
      }
      ((*pdf).*RooProdPdfHackResult<RooProdPdf_pdfList>::ptr).RecursiveRemove(part);
    }
  }
  
}

void TSUtils::removeConstituentsFromRooProdPdf(RooProdPdf* pdf, const TString& filter) {
  ::removeConstituents(pdf,filter);
}

//__________________________________________________________________________________|___________

class RooProductHelper : public RooProduct {
  public:
  static void removeConstituents(RooProduct* prod, const TString& filter) {
    if (!prod) return;
    std::vector<RooAbsArg*> parts;
    RooProductHelper* helper = (RooProductHelper*) prod;
    for(auto arg:helper->_compRSet){
      if (!arg) continue;
      if (!TQStringUtils::matchesFilter(arg->GetName(),filter,",")) continue;
      parts.push_back(arg);
    }
    for (RooAbsArg* const& part: parts) {
      helper->_compRSet.RecursiveRemove(part);
    }
    
    for(auto* arg: helper->_compCSet){
      if (!arg) continue;
      if (!TQStringUtils::matchesFilter(arg->GetName(),filter,",")) continue;
      parts.push_back(arg);
    }
    for (auto* part: parts) {
      helper->_compCSet.RecursiveRemove(part);
    }
    
  }

};

void TSUtils::removeConstituentsFromRooProduct(RooProduct* prod, const TString& filter) {
  RooProductHelper::removeConstituents(prod,filter);
}





//__________________________________________________________________________________|___________

namespace {
  void info(const TString& message) {
    std::cout << "SFramework/TSUtils: " << message.Data() << std::endl;
  }
  void error(const TString& message) {
    info(TQStringUtils::makeBoldRed(TString("ERROR: ") + message));
  }
  void warn(const TString& message) {
    info(TQStringUtils::makeBoldYellow(TString("WARNING: ") + message));
  }

}

//__________________________________________________________________________________|___________


TQFolder * TSUtils::convertParameterList(const RooAbsCollection * list, const TString& filter) {
  // convert a parameter list into a TQFolder
  if (!list) {
    return NULL;
  }

  TQFolder * result_list = TQFolder::newFolder("parameterList");

  convertParameterList(list,result_list,filter);

  return result_list;
}

//__________________________________________________________________________________|___________

void TSUtils::removeConstantParameters(RooAbsCollection& set){
  RooArgSet constSet;
  RooLinkedListIter it = set.iterator();
  RooRealVar *myarg;
  while ((myarg = (RooRealVar *)it.Next())) {
    if(myarg->isConstant()) constSet.add(*myarg);
  }
  set.remove(constSet);
}

//__________________________________________________________________________________|___________

void TSUtils::removeFloatingParameters(RooAbsCollection& set){
  RooArgSet floatSet;
  RooLinkedListIter it = set.iterator();
  RooRealVar *myarg;
  while ((myarg = (RooRealVar *)it.Next())) {
    if(!myarg->isConstant()) floatSet.add(*myarg);
  }
  set.remove(floatSet);
}

//__________________________________________________________________________________|___________

int TSUtils::convertParameterList(const RooAbsCollection * list, TQFolder* result_list, const TString& filter) {
  int n = 0;

  // iterate of parameters in list
  RooRealVarIterator itr(list->createIterator());
  while (itr.hasNext()) {
    RooRealVar * var = itr.readNext();
    if(!var) continue;
    
    if(!TQStringUtils::matchesFilter(var->GetName(),filter)) continue;

    TString name = TQFolder::makeValidIdentifier(var->GetName());
    TQFolder * result_arg = result_list->getFolder(name + "+");
    n++;

    // value
    result_arg->setTagDouble("val", var->getVal());

    // std::cout << var->GetName() << " = " << var->getVal() << " + " << var->getAsymErrorHi() << " - " << var->getAsymErrorLo() << std::endl;

    // errors
    if (var->hasError() && !var->hasAsymError()) {
      result_arg->setTagDouble("err", var->getError());
    } else if (var->hasAsymError()) {
      result_arg->setTagDouble("errLow", var->getAsymErrorLo());
      result_arg->setTagDouble("errHigh", var->getAsymErrorHi());
    }

    // is constant?
    result_arg->setTagBool("const", var->isConstant());

    // fit limits
    if (var->hasMin()) {
      result_arg->setTagDouble("min", var->getMin());
    }
    if (var->hasMax()) {
      result_arg->setTagDouble("max", var->getMax());
    }

    // title (if different from name)
    TString title = var->GetTitle();
    if (name.CompareTo(title) != 0) {
      result_arg->setTagString("title", var->GetTitle());
    }
  }
  return n;
}

//__________________________________________________________________________________|___________

RooArgList* TSUtils::convertParameterList(TQFolder* folder, RooAbsCollection * params) {
  TQFolderIterator itr(folder->getListOfFolders("?"),true);
  RooArgList* retval = new RooArgList();
  while(itr.hasNext()){
    TQFolder* f = itr.readNext();
    if(!f) continue;
    RooRealVar* var = params ? dynamic_cast<RooRealVar*>(params->find(f->GetName())) : new RooRealVar(f->GetName(),f->GetName(),f->getTagDoubleDefault("val",0.));
    if(!var) continue;
    double max;
    if(f->getTagDouble("max",max)){
      var->setMax(max);
    }
    double min;
    if(f->getTagDouble("min",min)){
      var->setMin(min);
    }
    double val;
    if(f->getTagDouble("val",val)){
      var->setVal(val);
    }
    //    std::cout<<TString::Format("Setting parameter '%s' to %s",var->GetName(), f->getTagBoolDefault("const",false)? "constant" : "floating").Data()<<std::endl;
    var->setConstant(f->getTagBoolDefault("const",false));
    double err,errLow,errHigh;
    if(f->getTagDouble("errLow",errLow) && f->getTagDouble("errHigh",errHigh)){
      var->setError(0.5*(fabs(errLow)+fabs(errHigh)));      
      var->setAsymError(errLow,errHigh);
    } else if(f->getTagDouble("err",err)){
      var->setError(err);
    }
    retval->add(*var);
  }
  retval->sort();
  return retval;
}

//__________________________________________________________________________________|___________

namespace {
  TH2* makeHist(const TMatrixDSym* mat, const std::vector<TString>& names, const RooAbsCollection& parameters, const char* name, bool normalize){
    std::vector<RooRealVar*> params;
    std::map<TString,int> indices;
    RooRealVarIterator itr(parameters.createIterator(),true);
    while(itr.hasNext()){
      RooRealVar* v = itr.readNext();
      TString name(v->GetName());
      int index = -1;
      for(size_t i=0; i<names.size(); ++i){
        if(names[i] == name) index = i;
      }
      if(index<0) continue;
      params.push_back(v);
      indices[name] = index;
    }

    int n = params.size();
    TH2D* hh = new TH2D(name,name,n,0,n,n,0,n) ;
    hh->SetDirectory(NULL);
    double min = 0;
    double max = 0;
    for (Int_t i = 0 ; i<n ; i++) {
      int ii = indices.at(params[i]->GetName());
      for (Int_t j = 0 ; j<n; j++) {
        int ij = indices.at(params[j]->GetName());
        double val = (*mat)(ii,ij);
        double vii = (*mat)(ii,ii);
        double vij = (*mat)(ij,ij);
        min = std::min(min,val);
        max = std::max(max,val);
        if(normalize){
          hh->SetBinContent(i+1,n-j,val / sqrt(vii * vij));
        } else {
          hh->SetBinContent(i+1,n-j,val);
        }
      }
      hh->GetXaxis()->SetBinLabel(i+1,params[i]->GetName()) ;
      hh->GetYaxis()->SetBinLabel(n-i,params[i]->GetName()) ;
    }
    if(normalize){
      hh->SetMinimum(-1.) ;
      hh->SetMaximum(1.) ;
    } else {
      hh->SetMinimum(min) ;
      hh->SetMaximum(max) ;
    }

    return hh ;
  }

  TMatrixDSym reduce(const TMatrixDSym& mat){
    std::vector<int> keepRows;
    for(int i=0; i<mat.GetNcols(); ++i){
      if(fabs(mat(i,i)) > 1e-14){
        keepRows.push_back(i);
      }
    }
    TMatrixDSym reduced(keepRows.size());
    for(size_t i=0; i<keepRows.size(); ++i){
      for(size_t j=0; j<keepRows.size(); ++j){
        reduced(i,j) = mat(keepRows[i],keepRows[j]);
      }
    }

    return reduced;
  }
}
//__________________________________________________________________________________|___________

RooAbsPdf* TSUtils::stripConstraints(RooProdPdf* product, const RooArgSet& observables, RooArgSet& constraints){
  //! strip the constraints from a product pdf
  RooAbsPdf* pdf = NULL;
  RooArgSet components(*product->getComponents());
  for(auto comp:components){
    if(!comp->InheritsFrom(RooAbsPdf::Class())) continue;
    RooAbsPdf* subpdf = NULL;
    if(comp == product) continue;
    if(comp->InheritsFrom(RooProdPdf::Class())){
      subpdf = stripConstraints((RooProdPdf*)comp,observables,constraints);
    } else if(comp->dependsOn(observables)){
      subpdf = (RooAbsPdf*)comp;
    } else {
      constraints.add(*comp);
    }
    if(subpdf){
      if(!pdf) pdf = subpdf;
      else {
        throw std::runtime_error("found several non-constraint like pdfs in a single product!");
      }
    }
  }
  return pdf;
}

//__________________________________________________________________________________|___________

TQFolder * TSUtils::convertFitResults(RooFitResult * fitResult, TQTaggable* options, ROOT::Fit::FitConfig* fitConfig) {
  // convert a RooFitResult into a TQFolder
  if (!fitResult) {
    return NULL;
  }

  TString name = TQFolder::makeValidIdentifier(fitResult->GetName());
  TQFolder * result = TQFolder::newFolder(name);

  // set title if different from name
  if (name.CompareTo(fitResult->GetTitle()) != 0) {
    result->setTagString("title", fitResult->GetTitle());
  }

  // minimized -log(L) value
  result->setTagDouble("minNll", fitResult->minNll());
  // estimated distance to minimum
  result->setTagDouble("edm", fitResult->edm());
  // status
  result->setTagInteger("status", fitResult->status());
  for (UInt_t i = 0; i < fitResult->numStatusHistory(); i++) {
    result->setTagInteger(TString::Format("status.%s", fitResult->statusLabelHistory(i)),
                          fitResult->statusCodeHistory(i));
  }
  // MINUIT quality code of covariance matrix
  result->setTagInteger("covQual", fitResult->covQual()); // for meanings of these codes, see https://root.cern.ch/root/html/ROOT__Minuit2__Minuit2Minimizer.html#ROOT__Minuit2__Minuit2Minimizer:CovMatrixStatus
  // number of NLL evaluations with problems
  result->setTagInteger("numInvalidNLL", fitResult->numInvalidNLL());

  // parameters
  TQFolder* constPars = convertParameterList(&fitResult->constPars());
  TQFolder* floatParsInit = convertParameterList(&fitResult->floatParsInit());
  TQFolder* floatParsFinal = convertParameterList(&fitResult->floatParsFinal());

  int floatPars = floatParsFinal->getNElements(false,TQFolder::Class());

  std::vector<TString> paramNames;
  if(fitConfig){
    for(auto p:fitConfig->ParamsSettings()){
      paramNames.push_back(p.Name());
    }
  } else {
    TQFolderIterator itr(floatParsFinal->getListOfFolders("?"),true);
    while(itr.hasNext()){
      TQFolder* f = itr.readNext();
      paramNames.push_back(f->GetName());
    }
  }

  result->addObject(constPars,"::constPars");
  result->addObject(floatParsInit,"::floatParsInit");
  result->addObject(floatParsFinal,"::floatParsFinal");

  if(fitResult->covQual() > 0){
    Double_t determ = 0;
    // these use a hack to access the fitResults internal member. We need this to check if this member isn't a nullptr as RooFit doesn't check this in some calls...
    const TMatrixDSym* covMatptr = TSUtils::getCovarianceMatrix(fitResult);
    const TMatrixDSym* corMatptr = TSUtils::getCorrelationMatrix(fitResult);
    if(corMatptr && covMatptr && covMatptr->GetNcols()>1 && corMatptr->GetNcols()>1){
      TMatrixDSym covMat(::reduce(*covMatptr));
      TMatrixDSym corMat(::reduce(*corMatptr));

      TH2* hCovariance = ::makeHist(covMatptr,paramNames,fitResult->floatParsFinal(),"covariance",false);
      if (hCovariance) {
        result->addObject(hCovariance);
      }

      // some checks
      if(hCovariance->GetNbinsX() != hCovariance->GetNbinsY()){
        throw std::runtime_error("internal consistency check failed, covariance matrix is not square!");
      }
      for(size_t i=0; i<(unsigned) hCovariance->GetNbinsX(); ++i){
        TString xlabel(hCovariance->GetXaxis()->GetBinLabel(i+1));
        int j = hCovariance->GetYaxis()->GetNbins()-i-1;
        TString ylabel(hCovariance->GetYaxis()->GetBinLabel(j+1));
        if(xlabel != ylabel) throw std::runtime_error(TString::Format("internal consistency check failed, labeling is inconsistent: %lu='%s' vs. %d='%s'!",i,xlabel.Data(),j,ylabel.Data()).Data());
        RooRealVar* par = dynamic_cast<RooRealVar*>(fitResult->floatParsFinal().find(xlabel));
        if(!par) throw std::runtime_error("internal consistency check failed, parameter listed in covariance matrix not found in fit result!");
        double err = par->getError();
        double cov = hCovariance->GetBinContent(i+1,j+1);
        if(options && options->getTagBoolDefault("runHesse",false) && fitResult->covQual() > 2){
          if(!TMath::AreEqualRel(cov,err*err,0.025)){
            warn(TString::Format("internal consistency check failed, error on parameter %lu %s=%.3f+/-%.3f does not agree with entry in covariance matrix %.3f (sqrt = %.3f)!",
                                 i,par->GetName(),par->getVal(),err,cov,sqrt(cov)).Data());
          }
        }
      }

      TH2* hCorrelation = ::makeHist(corMatptr,paramNames,fitResult->floatParsFinal(),"correlation",true);
      if (hCorrelation) {
        result->addObject(hCorrelation);
      }

      if(hCovariance->GetNbinsX()  != floatPars) throw std::runtime_error(TString::Format( "covariance matrix size %d inconsistent with float parameter list length %d!",hCovariance->GetNbinsX(),floatPars).Data());
      if(hCorrelation->GetNbinsX() != floatPars) throw std::runtime_error(TString::Format("correlation matrix size %d inconsistent with float parameter list length %d!",hCorrelation->GetNbinsX(),floatPars).Data());

      TMatrixDSym G = covMat.Invert(&determ);
      result->setTagDouble("hesse.determinant",determ);
      TH2* hist = TQHistogramUtils::convertMatrixToHistogram(&G,"hesse");
      if(hist){
        RooRealVarIterator itr(fitResult->floatParsFinal().createIterator(),true);
        int i = 1;
        while(itr.hasNext()){
          RooRealVar* var = itr.readNext();
          if(!var) continue;
          const TString name(var->GetName());
          hist          ->GetXaxis()->SetBinLabel(i,name);
          hist          ->GetYaxis()->SetBinLabel(i,name);
          i++;
        }
        if(!result->addObject(hist)){
          ERRORfunc("unable to add histogram to folder");
        }
      } else {
        ERRORfunc("histogram conversion of hesse matrix failed!");
      }
    }
  }

  return result;
}

namespace {
  struct MyFitResult : public RooFitResult {
    MyFitResult(const RooFitResult* other) : RooFitResult(*other) {}
    MyFitResult(const RooFitResult& other) : RooFitResult(other) {}
    MyFitResult(RooAbsCollection* paramList) :
      RooFitResult("prefitResult","prefitResult")
    {
      RooArgList constPars("constPars");
      RooArgList floatPars("floatPars");
      
      for(auto* obj:*paramList){
        RooRealVar* arg = dynamic_cast<RooRealVar*>(obj);
        if(!arg) continue;
        if (arg->isConstant()) {
          constPars.addClone(*arg);
        } else {
          if(arg->getError() == 0) arg->setError(1);
          floatPars.addClone(*arg);
        }
      }

      TMatrixDSym mat(floatPars.getSize());
      for(int i=0; i<floatPars.getSize(); ++i){
        mat(i,i) = 1;
      }

      this->setConstParList(constPars);
      this->setInitParList(floatPars);
      this->setFinalParList(floatPars);
      this->setMinNLL(0);
      this->setEDM(0);
      this->setCovQual(0);
      this->setStatus(0);

      this->_CM = new TMatrixDSym(mat);

      int i = 0;
      for(auto obj:floatPars){
        RooRealVar* roorealVar = dynamic_cast<RooRealVar*>(obj);
        double err = roorealVar->getError();
        if(err > 0){
          mat(i,i) = mat(i,i) * err * err;
        }
        i++;
      }

      this->_VM = new TMatrixDSym(mat);
    }


    MyFitResult(TQFolder* fitResult, RooAbsCollection* params) :
      RooFitResult(fitResult->GetName(),fitResult->GetTitle())
    {
      // minimized -log(L) value
      this->setMinNLL(fitResult->getTagDoubleDefault("minNll",nan));
      // estimated distance to minimum
      this->setEDM(fitResult->getTagDoubleDefault("edm",nan));
      // status
      this->setStatus(fitResult->getTagIntegerDefault("status",-1));
      // MINUIT quality code of covariance matrix
      this->setCovQual(fitResult->getTagIntegerDefault("covQual",0));

      RooArgList* floatParsInit  = TSUtils::convertParameterList(fitResult->getFolder("floatParsInit"),params);
      RooArgList* floatParsFinal = TSUtils::convertParameterList(fitResult->getFolder("floatParsFinal"),params);
      RooArgList* constPars      = TSUtils::convertParameterList(fitResult->getFolder("constPars"),     params);

      if(floatParsInit->getSize() != floatParsInit->getSize()){
        throw std::runtime_error("found inconsistent number of parameters before and after fit while constructing fit result");
      }

      floatParsInit->sort();
      floatParsFinal->sort();
      constPars->sort();

      std::vector<TString> names;
      TSUtils::getParameterNames(floatParsFinal,names);

      this->setInitParList (*floatParsInit);
      this->setFinalParList(*floatParsFinal);
      this->setConstParList(*constPars);

      const double relThreshold = 1e-4;
      const double absThreshold = 1e-6;
      
      this->_VM = TQHistogramUtils::convertHistogramToSymMatrix((TH2*)fitResult->getObject("covariance"), names,relThreshold,absThreshold);
      this->_CM = TQHistogramUtils::convertHistogramToSymMatrix((TH2*)fitResult->getObject("correlation"),names,relThreshold,absThreshold);

      // sanity checks      
      if(!this->_CM){
        throw std::runtime_error("unable to obtain correlation matrix!");
      }
      if(!this->_VM){
        throw std::runtime_error("unable to obtain covariance matrix!");
      }      
      if((int)this->_CM->GetNcols() != (int)this->_VM->GetNcols()){
        throw std::runtime_error("correlation and covariance matrices have different size!");
      }
      if((int)this->_CM->GetNcols() != (int)names.size()){
        throw std::runtime_error("matrix dimensions doesn't match number of floating parameters!");
      }      

      double minVar = 1;
      double maxVar = 1;
      for(int i=0; i<this->_CM->GetNcols(); ++i){
        if( (*(this->_CM))(i,i) <= 0){
          throw std::runtime_error("internal error in correlation matrix!");
        }
        if( (*(this->_VM))(i,i) <= 0){
          throw std::runtime_error("internal error in covariance matrix!");
        }
        minVar = std::min(minVar,(*(this->_VM))(i,i));
        maxVar = std::max(maxVar,(*(this->_VM))(i,i));
        for(int j=0; j<this->_CM->GetNcols(); ++j){
          double cov = (*(this->_VM))(i,j);
          double vari = (*(this->_VM))(i,i);
          double varj = (*(this->_VM))(j,j);
          double corrval = cov / sqrt(vari * varj);
          double corr = (*(this->_CM))(i,j);
          if(!TQHistogramUtils::areEqualRel(corrval,corr,relThreshold) && !(fabs(corrval) + fabs(corr) < absThreshold)){
            throw std::runtime_error(TString::Format("correlation embedded in covariance matrix does not match correlation matrix values for parameters '%s' and '%s': corr(i,j)=%g, but cov(i,j)=%g => corr(i,j)=%g with var(i)=%g and var(j)=%g!",names[i].Data(),names[j].Data(),corr,cov,corrval,vari,varj).Data());
          }
        }
      }
      if(minVar == maxVar){
        throw std::runtime_error("covariance matrix is identical to correlation matrix, all variances are unity - are you sure your fit result is correct?");
      }
    }
    virtual ~MyFitResult(){};

    ////////////////////////////////////////////////////////////////////////////////
    /// Return a p.d.f that represents the fit result as a multi-variate probability densisty
    /// function on the floating fit parameters, including correlations

    RooAbsPdf* robustCreateHessePdf(const RooAbsCollection* params) const {
      const TMatrixDSym& V = covarianceMatrix() ;

      TDecompLU lu(V);
      Double_t d1,d2;
      lu.Det(d1,d2);
      double det = d1*TMath::Power(2.,d2);
      int nOffDiag = 0;
      for(int i=0; i<V.GetNcols(); ++i){
        for(int j=0; j<V.GetNcols(); ++j){
          if(i==j) continue;
          if(V(i,j)!=0.) nOffDiag++;
        }
      }

      if(det<=0){
        std::stringstream ss;
        ss << TString::Format("cannot create hessian Pdf, covariance matrix is not pos. def., determinant is %g (d1=%g, d2=%g), condition is %g, tolerance is %g!",det,d1,d2,lu.Condition(),lu.GetTol()) << "\n";
        for(int i=0; i<V.GetNcols(); ++i){
          if(!(V(i,i)>0)){
            ss << TString::Format("  entry (%d,%d) = %g!",i,i,V(i,i)) << "\n";
          }
          for(int j=0; j<V.GetNcols(); ++j){
            if(i==j) continue;
            if(V(i,j)!=0.) nOffDiag++;
            if(V(i,i) <= V(i,j)){
              ss << TString::Format("  entry (%d,%d) = %g is greather than corresponding diagonal entry (%d,%d) = %g!",i,j,V(i,j),i,i,V(i,i)) << "\n";
            }
          }
        }
        ss << TString::Format("  total number of off-diagonal entries is %d!",nOffDiag) << "\n";
        if(nOffDiag>0){
          throw std::runtime_error(ss.str());
        }
      }

      // Make sure that all given params were floating parameters in the represented fit
      RooArgList params2 ;
      for(RooAbsArg* arg:*params){
        if (_finalPars->find(arg->GetName())) {
          params2.add(*arg) ;
        } else {
          std::stringstream ss;
          ss  << "RooFitResult::createHessePdf(" << GetName() << ") WARNING input variable "
              << arg->GetName() << " was not a floating parameters in fit result and is ignored" << std::endl ;
          warn(ss.str());
        }
      }
      
      // Need to order params in vector in same order as in covariance matrix
      RooArgList params3 ;
      for(auto* obj:*_finalPars){
        RooAbsReal* arg = dynamic_cast<RooAbsReal*>(obj);
        if (params2.find(arg->GetName())) {
          params3.add(*arg) ;
        }
      }


      // Handle special case of representing full covariance matrix here
      if (params3.getSize()==_finalPars->getSize()) {

        RooArgList mu ;
        for (Int_t i=0 ; i<_finalPars->getSize() ; i++) {
          RooRealVar* parclone = (RooRealVar*) _finalPars->at(i)->Clone(Form("%s_centralvalue",_finalPars->at(i)->GetName())) ;
          parclone->setConstant(kTRUE) ;
          mu.add(*parclone) ;
        }

        std::string name  = Form("pdf_%s",GetName()) ;
        std::string title = Form("P.d.f of %s",GetTitle()) ;

        // Create p.d.f.
        RooAbsPdf* mvg = new RooMultiVarGaussian(name.c_str(),title.c_str(),params3,mu,V) ;
        mvg->addOwnedComponents(mu) ;
        return  mvg ;
      }

      //                                       -> ->
      // Handle case of conditional p.d.f. MVG(p1|p2) here

      // Find (subset) of parameters that are stored in the covariance matrix
      std::vector<int> map1, map2 ;
      for (int i=0 ; i<_finalPars->getSize() ; i++) {
        if (params3.find(_finalPars->at(i)->GetName())) {
          map1.push_back(i) ;
        } else {
          map2.push_back(i) ;
        }
      }

      // Calculate offset vectors mu1 and mu2
      RooArgList mu1 ;
      for (UInt_t i=0 ; i<map1.size() ; i++) {
        RooRealVar* parclone = (RooRealVar*) _finalPars->at(map1[i])->Clone(Form("%s_centralvalue",_finalPars->at(map1[i])->GetName())) ;
        parclone->setConstant(kTRUE) ;
        mu1.add(*parclone) ;
      }

      TMatrixDSym Vred;
      if(nOffDiag>0){
        // Rearrange matrix in block form with 'params' first and 'others' last
        // (preserving relative order)
        TMatrixDSym S11, S22 ;
        TMatrixD S12, S21 ;
        RooMultiVarGaussian::blockDecompose(V,map1,map2,S11,S12,S21,S22) ;

        // Constructed conditional matrix form         -1
        // F(X1|X2) --> CovI --> S22bar = S11 - S12 S22  S21

        // Do eigenvalue decomposition
        TMatrixD S22Inv(TMatrixD::kInverted,S22) ;
        TMatrixD S22bar =  S11 - S12 * (S22Inv * S21) ;

        // Convert explicitly to symmetric form
        Vred.ResizeTo(S22bar.GetNcols(),S22bar.GetNcols()) ;
        for (int i=0 ; i<Vred.GetNcols() ; i++) {
          for (int j=i ; j<Vred.GetNcols() ; j++) {
            Vred(i,j) = (S22bar(i,j) + S22bar(j,i))/2 ;
            Vred(j,i) = Vred(i,j) ;
          }
        }
      } else {
        Vred.ResizeTo(params3.getSize(),params3.getSize());
        for (int i=0 ; i<Vred.GetNcols() ; i++) {
          int j = map1.at(i);
          Vred(i,i) = V(j,j);
        }
      }
      std::string name  = Form("pdf_%s",GetName()) ;
      std::string title = Form("P.d.f of %s",GetTitle()) ;

      // Create p.d.f.
      RooAbsPdf* ret =  new RooMultiVarGaussian(name.c_str(),title.c_str(),params3,mu1,Vred) ;
      ret->addOwnedComponents(mu1) ;
      return ret ;
    }
  };
}

//__________________________________________________________________________________|___________

template<class T> RooFitResult * TSUtils::convertFitResults(TQFolder* fitResult, const std::vector<T>& params) {
  RooArgList l;
  for(auto p:params) l.add(*p);
  return convertFitResults(fitResult,&l);
}
namespace TSUtils {
  template<>  RooFitResult * convertFitResults<RooAbsArg*> (TQFolder* fitResult, const std::vector<RooAbsArg*>&  params);
  template<>  RooFitResult * convertFitResults<RooRealVar*>(TQFolder* fitResult, const std::vector<RooRealVar*>& params);
  template<>  RooFitResult * convertFitResults<RooAbsPdf*> (TQFolder* fitResult, const std::vector<RooAbsPdf*>&  params);
}

//__________________________________________________________________________________|___________

RooFitResult * TSUtils::convertFitResults(TQFolder* fitResult, RooAbsCollection* params) {

  // convert a RooFitResult into a TQFolder
  if (!fitResult) {
    return NULL;
  }

  MyFitResult result(fitResult,params);
  RooFitResult* r = new RooFitResult(result);

  return r;
}

//__________________________________________________________________________________|___________

TQFolder* TSUtils::modifyFitResults(TQFolder* fitResults, TQFolder* changes){
  TQFolder* newResult = new TQFolder(fitResults->getName());
  newResult->importTags(fitResults);

  std::vector<TString> constParList = changes->getTagVString("constPars");

  TQFolder* oldfloatParsFinal = fitResults->getFolder("floatParsFinal");
  TQFolder* oldfloatParsInit  = fitResults->getFolder("floatParsInit");
  TQFolder* oldconstPars      = fitResults->getFolder("constPars");
  TQFolder* newfloatParsFinal = newResult->getFolder("floatParsFinal+");
  TQFolder* newfloatParsInit  = newResult->getFolder("floatParsInit+");
  TQFolder* newconstPars      = oldconstPars->copy();
  newResult->addFolder(newconstPars);

  std::vector<TString> removeBins;

  TQFolderIterator itr(oldfloatParsInit->getListOfFolders("?"),true);
  while(itr.hasNext()){
    TQFolder* par = itr.readNext();
    if(!par) continue;
    bool setConst = false;
    for(const auto& p:constParList){
      if(TQStringUtils::matches(par->getName(),p)){
        setConst = true;
      }
    }
    TQFolder* finalpar = oldfloatParsFinal->getFolder(par->getName());
    if(!finalpar) continue;
    if(setConst){
      TQFolder* copy = finalpar->copy();
      copy->setTagBool("const",true);
      newconstPars->addFolder(copy);
      removeBins.push_back(finalpar->GetName());
    } else {
      newfloatParsInit ->addFolder(par->copy());
      newfloatParsFinal->addFolder(finalpar->copy());
    }
  }

  TH2* corMat  = dynamic_cast<TH2*>(fitResults->getObject("correlation"));
  TH2* covMat  = dynamic_cast<TH2*>(fitResults->getObject("covariance"));
  TH2* hesse  = dynamic_cast<TH2*>(fitResults->getObject("hesse"));
  newResult->addObject(TQHistogramUtils::removeBins(corMat,removeBins));
  newResult->addObject(TQHistogramUtils::removeBins(covMat,removeBins));
  newResult->addObject(TQHistogramUtils::removeBins(hesse ,removeBins));

  return newResult;
}


//__________________________________________________________________________________|___________

TQFolder* TSUtils::applyFitResult(RooAbsCollection* vars, TQFolder* fitResults){
  TQFolder* newResult = new TQFolder(fitResults->getName());
  newResult->importTags(fitResults);

  TQFolder* oldfloatParsFinal = fitResults->getFolder("floatParsFinal");
  TQFolder* oldfloatParsInit  = fitResults->getFolder("floatParsInit");
  TQFolder* oldconstPars      = fitResults->getFolder("constPars");
  TQFolder* newfloatParsFinal = newResult->getFolder("floatParsFinal+");
  TQFolder* newfloatParsInit  = newResult->getFolder("floatParsInit+");
  TQFolder* newconstPars      = newResult->getFolder("constPars+");

  std::vector<TString> removeBins;
  TQFolderIterator float_itr(oldfloatParsFinal->getListOfFolders("?"),true);
  while(float_itr.hasNext()){
    TQFolder* p_float = float_itr.readNext();
    RooRealVar* v_float = dynamic_cast<RooRealVar*>(vars->find(p_float->GetName()));
    if(!v_float) {
      info(TString::Format("unable to find parameter '%s' in workspace!",p_float->GetName()));
      removeBins.push_back(p_float->GetName());
      continue;
    }
    v_float->setVal(p_float->getTagDoubleDefault("val",0.));
    double err;
    if(p_float->getTagDouble("err",err)) v_float->setError(err);
    double errLo, errHi;
    if(p_float->getTagDouble("errHigh",errHi) && p_float->getTagDouble("errLow",err)) v_float->setAsymError(errLo,errHi);
    v_float->setConstant(false);
    newfloatParsFinal->addFolder(p_float->copy());
   }
  TQFolderIterator floatInit_itr(oldfloatParsInit->getListOfFolders("?"),true);
  while(floatInit_itr.hasNext()){
    TQFolder* p_floatInit = floatInit_itr.readNext();
    RooRealVar* v_floatInit = dynamic_cast<RooRealVar*>(vars->find(p_floatInit->GetName()));
    if(!v_floatInit) {
      info(TString::Format("unable to find parameter '%s' in workspace!",p_floatInit->GetName()));
      removeBins.push_back(p_floatInit->GetName());
      continue;
    }
    v_floatInit->setVal(p_floatInit->getTagDoubleDefault("val",0.));
    double err;
    if(p_floatInit->getTagDouble("err",err)) v_floatInit->setError(err);
    double errLo, errHi;
    if(p_floatInit->getTagDouble("errHigh",errHi) && p_floatInit->getTagDouble("errLow",err)) v_floatInit->setAsymError(errLo,errHi);
    v_floatInit->setConstant(false);
    newfloatParsInit->addFolder(p_floatInit->copy());
   }
  TQFolderIterator const_itr(oldconstPars->getListOfFolders("?"),true);
  while(const_itr.hasNext()){
    TQFolder* p_const = const_itr.readNext();
    RooRealVar* v_const = dynamic_cast<RooRealVar*>(vars->find(p_const->GetName()));
    if(!v_const) {
      info(TString::Format("unable to find parameter '%s' in workspace!",p_const->GetName()));
      removeBins.push_back(p_const->GetName());
      continue;
    }
    v_const->setVal(p_const->getTagDoubleDefault("val",0.));
    double err;
    if(p_const->getTagDouble("err",err)) v_const->setError(err);
    double errLo, errHi;
    if(p_const->getTagDouble("errHigh",errHi) && p_const->getTagDouble("errLow",err)) v_const->setAsymError(errLo,errHi);
    v_const->setConstant(false);
    newconstPars->addFolder(p_const->copy());
   }
  TH2* corMat  = dynamic_cast<TH2*>(fitResults->getObject("correlation"));
  TH2* covMat  = dynamic_cast<TH2*>(fitResults->getObject("covariance"));
  TH2* hesse  = dynamic_cast<TH2*>(fitResults->getObject("hesse"));
  newResult->addObject(TQHistogramUtils::removeBins(corMat,removeBins));
  newResult->addObject(TQHistogramUtils::removeBins(covMat,removeBins));
  newResult->addObject(TQHistogramUtils::removeBins(hesse ,removeBins));

  return newResult;
}

//__________________________________________________________________________________|___________

RooFitResult * TSUtils::prefitResults(RooAbsCollection* params) {
  // create an empty RooFitResult
  MyFitResult result(params);
  return new RooFitResult(result);
}

//__________________________________________________________________________________|___________

RooAbsPdf * TSUtils::createHessePdf(const RooFitResult* fr, const RooAbsCollection* params) {
  // create an empty RooFitResult
  MyFitResult result(*fr);
  RooAbsPdf* hesse = result.robustCreateHessePdf(params);
  hesse->recursiveRedirectServers(fr->floatParsFinal(),true);
  return hesse;
}

//__________________________________________________________________________________|___________

const TMatrixDSym* TSUtils::getCovarianceMatrix(const RooFitResult* fitResult){
  // get a (const) pointer to the covariance matrix of a fit result
  return fitResult->*RooFitResultHackResult<RooFitResultVM>::ptr;
}

//__________________________________________________________________________________|___________

const TMatrixDSym* TSUtils::getCorrelationMatrix(const RooFitResult* fitResult){
  // get a (const) pointer to the covariance matrix of a fit result
  return fitResult->*RooFitResultHackResult<RooFitResultCM>::ptr;
}

//__________________________________________________________________________________|___________

void TSUtils::getPlotRange(RooAbsReal* rv, double& min, double& max, int& nbins){
  // return the range set for plotting in a RooAbsReal
  if(!rv) return;
  //the following lines are nothing but fancy ways to do what the comments above each line do. This is required in order to avoid a public-private hack (this variant should be way less compiler dependent or ideally compiler independent)
  //min = rv->_plotMin;
  min = (*rv).*RooAbsRealHackResult<RooAbsReal_plotMin>::ptr;
  //max = rv->_plotMin;
  max = (*rv).*RooAbsRealHackResult<RooAbsReal_plotMax>::ptr;

  //nbins = rv->_plotBins;
  nbins = (*rv).*RooAbsRealHackResult<RooAbsReal_plotBins>::ptr;
}

//__________________________________________________________________________________|___________

TQFolder* TSUtils::convertGraph(TGraph* g){
  // convert an instance of TGraph into an instance of TQFolder such that the TSStatisticsPlotter can handle it
  int i=0;
  TQFolder* retval = TQFolder::newFolder(g->GetName());
  double x,y;
  while(g->GetPoint(i,x,y) > 0){
    TQFolder* p = retval->getFolder(TString::Format("p.%d+",i));
    p->setTag("x",x);
    p->setTag("y",x);
    i++;
  }
  return retval;
}

//__________________________________________________________________________________|___________

TQFolder* TSUtils::makeGraphFolder(const char* name,const std::map<double,double>& values){
  // convert a map<double,double> into an instance of TQFolder such that the TSStatisticsPlotter can handle it
  int i=0;
  TQFolder* retval = TQFolder::newFolder(name);
  for(auto it:values){
    TQFolder* p = retval->getFolder(TString::Format("p.%d+",i));
    p->setTag("x",it.first);
    p->setTag("y",it.second);
    i++;
  }
  return retval;
}

//__________________________________________________________________________________|___________

void TSUtils::setParametersConstant(const RooAbsCollection* params, bool constVal){
  // set a list of parameters to const/non-const
  if(!params) return;
  TQIterator itr(params->createIterator(),true);
  while (itr.hasNext()) {
    RooRealVar * poi = dynamic_cast<RooRealVar*>(itr.readNext());
    if (poi) {
      std::cout<<TString::Format("Setting parameter '%s' to %s",poi->GetName(),constVal? "constant" : "floating" ).Data()<<std::endl;
      poi->setConstant(constVal);
    }
  }
}
//__________________________________________________________________________________|___________

int TSUtils::countConstParameters(const RooAbsCollection& params){
  return countConstParameters(&params);
}

//__________________________________________________________________________________|___________

int TSUtils::countConstParameters(const RooAbsCollection* params){
  // count how many parameters are constant
  if(!params) return -1;
  TQIterator itr(params->createIterator(),true);
	int n = 0;
  while (itr.hasNext()) {
    RooRealVar * var = dynamic_cast<RooRealVar*>(itr.readNext());
    if (!var) continue;
		if(var->isConstant()) n++;
	}
	return n;
}

//__________________________________________________________________________________|___________

int TSUtils::countFloatParameters(const RooAbsCollection& params){
  return countFloatParameters(&params);
}

//__________________________________________________________________________________|___________

int TSUtils::countFloatParameters(const RooAbsCollection* params){
  // count how many parameters are floating
  if(!params) return -1;
  TQIterator itr(params->createIterator(),true);
	int n = 0;
  while (itr.hasNext()) {
    RooRealVar * var = dynamic_cast<RooRealVar*>(itr.readNext());
    if (!var) continue;
		if(!var->isConstant()) n++;
	}
	return n;
}

//__________________________________________________________________________________|___________

template<class listT, class stringT>
void TSUtils::getParameterNames(const listT* l,std::vector<stringT>& names){
	// extract the parameter names from a list
  if(!l) return;
  for(auto* obj:*l){
    names.push_back(obj->GetName());
  }
}
template void TSUtils::getParameterNames<RooAbsCollection, TString>    (const RooAbsCollection* l,std::vector<TString>& names);
template void TSUtils::getParameterNames<RooAbsCollection, std::string>(const RooAbsCollection* l,std::vector<std::string>& names);
template void TSUtils::getParameterNames<RooArgSet, TString>    (const RooArgSet* l,std::vector<TString>& names);
template void TSUtils::getParameterNames<RooArgSet, std::string>(const RooArgSet* l,std::vector<std::string>& names);
template void TSUtils::getParameterNames<RooArgList, TString>    (const RooArgList* l,std::vector<TString>& names);
template void TSUtils::getParameterNames<RooArgList, std::string>(const RooArgList* l,std::vector<std::string>& names);

//__________________________________________________________________________________|___________

namespace {
  void getArgs(RooWorkspace* ws, const std::vector<TString>& names, RooArgSet& args){
    for(const auto& p:names){
      RooAbsArg* v =(RooAbsArg*) ws->obj(p);
      if(v){
        args.add(*v);
      }
    }
  }
  void getArgs(RooWorkspace* ws, const std::vector<std::string>& names, RooArgSet& args){
    for(const auto& p:names){
      RooRealVar* v = ws->var(p.c_str());
      if(v){
        args.add(*v);
      }
      RooAbsCategory* c = ws->cat(p.c_str());
      if(c){
        args.add(*c);
      }
    }
  }
  const char* name(const TString& s){
    return s.Data();
  }
  const char* name(const TObject* o){
    return o->GetName();
  }  
  bool samename(const char* a, const char*  b){
    return strcmp(a,b)==0;
  }    
  template<class ListT1,class ListT2> void myAssertList(const ListT1& n1, const ListT2& n2, const char* msg, bool throwCatch){
    if(n1.size() != n2.size()){
      TString message = msg;
      message += TString::Format(": %d -> %d",(int)n1.size(),(int)n2.size());
      for(const auto& item1:n1){
        bool found = false;
        for(const auto& item2:n2){
          if(samename(name(item1),name(item2))){
            found = true;
            break;
          }
        }
        if(!found){
          message += TString::Format("\n  missing: %s",name(item1));
        }
      }
      for(const auto& item1:n2){
        bool found = false;
        for(const auto& item2:n1){
          if(samename(name(item1),name(item2))){          
            found = true;
            break;
          }
        }
        if(!found){
          message += TString::Format("\n  added: %s",name(item1));
        }
      }      
      if(throwCatch) throw std::runtime_error((TString("error: ")+message).Data());
      else warn(message);
    }
  }
  template<class ListT1,class ListT2> void myAssertList(const ListT1* n1, const ListT2* n2, const char* msg, bool throwCatch){
    myAssertList(*n1,*n2,msg,throwCatch);
  }
  template<class ListT1,class ListT2> void myAssertList(const ListT1* n1, const ListT2& n2, const char* msg, bool throwCatch){
    myAssertList(*n1,n2,msg,throwCatch);
  }
  template<class ListT1,class ListT2> void myAssertList(const ListT1& n1, const ListT2* n2, const char* msg, bool throwCatch){
    myAssertList(n1,*n2,msg,throwCatch);
  }
    
  
}

bool TSUtils::importObjectToWorkspace(RooWorkspace* ws, RooAbsArg* obj){
  // wrapper for RooWorkspace::import
  if(!ws || !obj) return false;
  return !ws->import(*obj);
}

bool TSUtils::importObjectToWorkspace(RooWorkspace* ws, RooAbsData* obj){
  // wrapper for RooWorkspace::import
  if(!ws || !obj) return false;
  return !ws->import(*obj);
}


RooWorkspace* TSUtils::makeCleanWorkspace(RooWorkspace* oldWS, const char* newName, const char* mcname, bool copySnapshots, bool catchUnaccountedParameters){
  // clone a workspace, copying all needed components and discarding all others

  // butcher the old workspace
  auto objects = oldWS->allGenericObjects();
  RooStats::ModelConfig* oldMC = dynamic_cast<RooStats::ModelConfig*>(oldWS->obj(mcname));
  RooAbsPdf* pdf = NULL;
  auto data = oldWS->allData();
  for(auto it:objects){
    if(!oldMC){
      oldMC = dynamic_cast<RooStats::ModelConfig*>(it);
    }
    pdf = dynamic_cast<RooSimultaneous*>(it);
  }

  // butcher the old modelconfig
  std::vector<TString> poilist;
  std::vector<TString> nplist;
  std::vector<TString> obslist;
  std::vector<TString> globobslist;
  if(oldMC){
    pdf = oldMC->GetPdf();
    TSUtils::getParameterNames(oldMC->GetParametersOfInterest(),poilist);
    TSUtils::getParameterNames(oldMC->GetNuisanceParameters(),nplist);
    TSUtils::getParameterNames(oldMC->GetObservables(),obslist);
    TSUtils::getParameterNames(oldMC->GetGlobalObservables(),globobslist);
    myAssertList(oldMC->GetParametersOfInterest(),poilist,"exporting POI list",catchUnaccountedParameters);
    myAssertList(oldMC->GetNuisanceParameters(),nplist,"exporting NP list!",catchUnaccountedParameters);
    myAssertList(oldMC->GetObservables(),obslist,"exporting Obs list",catchUnaccountedParameters);
    myAssertList(oldMC->GetGlobalObservables(),globobslist,"exporting Globs list",catchUnaccountedParameters);
  } else if(!pdf){
    pdf = oldWS->pdf(mcname);
  }

  if(!pdf){
    return NULL;
  }

  // create them anew
  RooWorkspace* newWS = new RooWorkspace(newName);
  newWS->autoImportClassCode(true);
  RooStats::ModelConfig* newMC = new RooStats::ModelConfig("ModelConfig", newWS);

  newWS->import(*pdf, RooFit::RecycleConflictNodes());
  RooAbsPdf* newPdf = newWS->pdf(pdf->GetName());
  newMC->SetPdf(*newPdf);

  for(auto d:data){
    newWS->import(*d);
  }

  RooArgSet poiset; getArgs(newWS,poilist,poiset);
  RooArgSet npset; getArgs(newWS,nplist,npset);
  RooArgSet obsset; getArgs(newWS,obslist,obsset);
  RooArgSet globobsset; getArgs(newWS,globobslist,globobsset);

  myAssertList(poilist,poiset,"setting POI list",catchUnaccountedParameters);
  myAssertList(nplist,npset,"setting NP list",catchUnaccountedParameters);
  myAssertList(obslist,obsset,"setting Obs list",catchUnaccountedParameters);
  myAssertList(globobslist,globobsset,"setting Globs list",catchUnaccountedParameters);
  
  newMC->SetParametersOfInterest(poiset);
  newMC->SetNuisanceParameters  (npset);
  newMC->SetObservables         (obsset);
  newMC->SetGlobalObservables   (globobsset);

  myAssertList(poiset,newMC->GetParametersOfInterest(),"setting POI list",catchUnaccountedParameters);
  myAssertList(npset,newMC->GetNuisanceParameters(),"setting NP list",catchUnaccountedParameters);
  myAssertList(obsset,newMC->GetObservables(),"setting Obs list",catchUnaccountedParameters);
  myAssertList(globobsset,newMC->GetGlobalObservables(),"setting Globs list",catchUnaccountedParameters);

  newWS->import(*newMC);

  RooStats::ModelConfig* importedMC = (RooStats::ModelConfig*)(newWS->obj(newMC->GetName()));

  myAssertList(newMC->GetParametersOfInterest(),importedMC->GetParametersOfInterest(),"importing POI list",catchUnaccountedParameters);
  myAssertList(newMC->GetNuisanceParameters()  ,importedMC->GetNuisanceParameters()  ,"importing NP list",catchUnaccountedParameters);
  myAssertList(newMC->GetObservables()         ,importedMC->GetObservables()         ,"importing Obs list",catchUnaccountedParameters);
  myAssertList(newMC->GetGlobalObservables()   ,importedMC->GetGlobalObservables()   ,"importing Globs list",catchUnaccountedParameters);


  if(copySnapshots){
    // Copy snapshots
    // Fancy ways to avoid public-private hack used in the following, simplified version in comments above the respective lines
    //  RooFIter itr(oldWS->_snapshots.fwdIterator());
    auto snapshots(TSUtils::getListOfSnapshots(oldWS));
    for(size_t i=0; i<(size_t)(snapshots.GetSize()); ++i){
      RooArgSet* snap = dynamic_cast<RooArgSet*>(snapshots.At(i));
      newWS->saveSnapshot(snap->GetName(),*snap,true);
    }
  }

  return newWS;
}

//__________________________________________________________________________________|___________

RooLinkedList& TSUtils::getListOfSnapshots(RooWorkspace* ws){
  if (!ws) {
    throw std::runtime_error("invalid workspace pointer passed!");
  }
  return ws->*RooWorkspaceHackResult<RooWorkspace_snapshots>::ptr;
}


//__________________________________________________________________________________|___________

std::vector<TString> TSUtils::getListOfSnapshotNames(RooWorkspace* ws){
  // retrieve the list of snapshots
  std::vector<TString> names;
  if(!ws) return names;
  for(auto* snap:getListOfSnapshots(ws).fwdIterator()){
    names.push_back(snap->GetName());
  }
  return names;
}

//__________________________________________________________________________________|___________

bool TSUtils::takeSnapshot(RooWorkspace* workspace, const TString& snShName, TQTaggable* config, TSStatisticsManager* manager) {

  if (!workspace) return false;
  //create a dummy config object if none was provided (-> always using default values)
  bool transientConfig = !config;
  if (transientConfig) config = new TQTaggable();

  //local method definition to print infos with or without a manager
  void (*printInfo)(TSStatisticsManager*, const TString&) = [](TSStatisticsManager* manager, const TString& msg) {
    if (manager) manager->info(msg);
    else std::cout<<"[INFO] "<<msg.Data()<<std::endl;
  };

  RooStats::ModelConfig* mc = dynamic_cast<RooStats::ModelConfig*>(workspace->obj(config->getTagStringDefault("~modelConfig","ModelConfig")));
  RooArgSet allVars(workspace->allVars());

  bool all    = config->getTagBoolDefault("all",false);
  bool pois   = config->getTagBoolDefault("POIs",true);
  bool nps    = config->getTagBoolDefault("NPs",true);
  bool obs    = config->getTagBoolDefault("obs",false);
  bool globs  = config->getTagBoolDefault("globs",true);
  RooArgSet vars;
  if(all){
    vars.add(allVars);
    printInfo(manager, TString::Format("taking snapshot '%s' of configuration, including all variables",snShName.Data()));
  } else if(mc){
    if(pois ) vars.add(*(mc->GetParametersOfInterest()));
    if(nps  ) vars.add(*(mc->GetNuisanceParameters()));
    if(obs  ) vars.add(*(mc->GetObservables()));
    if(globs) vars.add(*(mc->GetGlobalObservables()));
    int npars = vars.getSize();
    TSUtils::addParameters(&allVars,config->getTagVString("constPars"),vars);
    TSUtils::addParameters(&allVars,config->getTagVString("floatPars"),vars);
    TSUtils::addParameters(&allVars,TSUtils::getParameterKeys(config,"setPars"),vars);
    int addpars = vars.getSize() - npars;

    printInfo(manager,TString::Format("taking snapshot '%s' of configuration, including POIs (%s), NPs (%s), Observables (%s) and Global Observables (%s), plus %d additional parameters",snShName.Data(),
                                  pois ? "yes" : "no",
                                  nps ? "yes" : "no",
                                  obs ? "yes" : "no",
                                  globs ? "yes" : "no",
                                  addpars));
  }

  if(vars.getSize() > 0){
    workspace->saveSnapshot(snShName,vars);
    if (transientConfig) delete config;
    return true;
  } else {
    //single occurance, so no convenience function defined
    if (manager) manager->warn("skipping snapshot creation, no variables listed...");
    else std::cout<<"[WARN] skipping snapshot creation, no variables listed..."<<std::endl;
  }

  if (transientConfig) delete config;
  return false;

}

//__________________________________________________________________________________|___________

std::vector<TString> TSUtils::expandNames(const RooAbsCollection* nuis, const TString& filter, const TString& exception) {
  // expand a filter
  std::vector<TString> parnames;
  TQIterator itrNuis(nuis->createIterator(), true);
  while (itrNuis.hasNext()) {
    TObject * obj = itrNuis.readNext();
    if (!obj->InheritsFrom(RooRealVar::Class())) {
      continue;
    }
    RooRealVar * var = (RooRealVar*)obj;
    TString varName = var->GetName();
    if (TQStringUtils::matchesFilter(varName, filter, ",", true) && !TQStringUtils::matchesFilter(varName, exception, ",", true)) {
      parnames.push_back(varName);
    }
  }
  return parnames;
}

//__________________________________________________________________________________|___________

void TSUtils::expandKeys(const RooAbsCollection* nuis, TQFolder * options) {
  // convert singles into groups
  TQIterator itrSingles(options->getListOfKeys("singles.*"), true);
  while (itrSingles.hasNext()) {
    TString name = itrSingles.readNext()->GetName();
    TString filter = options->getTagStringDefault(name, "!*");
    TQStringUtils::removeLeadingText(name, "singles.");

    TQIterator itrNuis(nuis->createIterator(), true);
    while (itrNuis.hasNext()) {
      TObject * obj = itrNuis.readNext();
      if (!obj->InheritsFrom(RooRealVar::Class())) {
        continue;
      }
      RooRealVar * var = (RooRealVar*)obj;
      TString varName = var->GetName();
      if (TQStringUtils::matchesFilter(varName, filter, ",", true)) {
        // add as group with one element
        TString groupName = TString("group.") + varName;
        if (!options->hasTag(groupName)) {
          options->setTagString(groupName, varName);
        }
      }
    }
  }
}


//__________________________________________________________________________________|___________

std::map<TString,double> TSUtils::getParameterValues(const RooAbsCollection& parameters){
  // obtain the values of parameters
  return getParameterValues(&parameters);
}

//__________________________________________________________________________________|___________

std::map<TString,double> TSUtils::getParameterValues(const RooAbsCollection* parameters){
  // obtain the values of parameters
  std::map<TString,double> values;
  for(auto* obj:*parameters){
    RooRealVar* poi = dynamic_cast<RooRealVar*>(obj);
    values[obj->GetName()] = poi->getVal();
  }
  return values;
}

//__________________________________________________________________________________|___________

void TSUtils::setParameterValues(const RooAbsCollection* parameters, double val){
  // set a set of parameters all to the same value
  for(auto* obj:*parameters){
    RooRealVar* poi = dynamic_cast<RooRealVar*>(obj);
    poi->setVal(val);
  }
}

//__________________________________________________________________________________|___________

void TSUtils::setParameterValues(const RooAbsCollection& parameters, const RooAbsCollection& values){
  // set a set of parameters all to the same value
  TSUtils::setParameterValues(&parameters,&values);
}

//__________________________________________________________________________________|___________

void TSUtils::setParameterValues(const RooAbsCollection& parameters, const RooAbsCollection* values){
  // set a set of parameters all to the same value
  TSUtils::setParameterValues(&parameters,values);
}

//__________________________________________________________________________________|___________

void TSUtils::setParameterValues(const RooAbsCollection* parameters, const RooAbsCollection& values){
  // set a set of parameters all to the same value
  TSUtils::setParameterValues(parameters,&values);
}

//__________________________________________________________________________________|___________

void TSUtils::setParameterValues(const RooAbsCollection* parameters, const RooAbsCollection* values){
  // set a set of parameters all to the same value
  if(!values || !parameters) return;
  for(auto* obj:*parameters){
    RooRealVar* poi = dynamic_cast<RooRealVar*>(obj);
    if(!poi) continue;
    RooAbsReal* val = dynamic_cast<RooAbsReal*>(values->find(poi->GetName()));
    if(!val) continue;
    poi->setVal(val->getVal());
  }
}

//__________________________________________________________________________________|___________

void TSUtils::setParametersConstFloat(const RooAbsCollection& parameters, const RooAbsCollection& values){
  // set a set of parameters all to the same value
  TSUtils::setParametersConstFloat(&parameters,&values);
}

//__________________________________________________________________________________|___________

void TSUtils::setParametersConstFloat(const RooAbsCollection& parameters, const RooAbsCollection* values){
  // set a set of parameters all to the same value
  TSUtils::setParametersConstFloat(&parameters,values);
}

//__________________________________________________________________________________|___________

void TSUtils::setParametersConstFloat(const RooAbsCollection* parameters, const RooAbsCollection& values){
  // set a set of parameters all to the same value
  TSUtils::setParametersConstFloat(parameters,&values);
}

//__________________________________________________________________________________|___________

void TSUtils::setParametersConstFloat(const RooAbsCollection* parameters, const RooAbsCollection* values){
  // set a set of parameters all to the same value
  if(!values || !parameters) return;
  for(auto* obj:*parameters){
    RooRealVar* poi = dynamic_cast<RooRealVar*>(obj);
    if(!poi) continue;
    RooAbsReal* val = dynamic_cast<RooAbsReal*>(values->find(poi->GetName()));
    if(!val) continue;
    poi->setConstant(val->isConstant());
  }
}

//__________________________________________________________________________________|___________

std::vector<TString> TSUtils::getParameterKeys(TQTaggable* tags, const TString& prefix){
  TQIterator params(tags->getListOfKeys(prefix+".*"),true);
  std::vector<TString> retval;
  while(params.hasNext()){
    TObject* key = params.readNext();
    if(!key) continue;
    TString name(key->GetName());
    TQStringUtils::removeLeadingText(name,prefix+".");
    retval.push_back(name);
  }
  return retval;
}

//__________________________________________________________________________________|___________

void TSUtils::setParameterValues(const RooAbsCollection* parameters, TQTaggable* options, const TString& prefix, bool forceSetConst){
  // the parameters in the workspace to the values given by the tags
  DEBUGfunc("entering function");

  TQIterator params(options->getListOfKeys(prefix+".*"),true);
  while(params.hasNext()){
    TObject* key = params.readNext();
    if(!key) continue;
    TString name(key->GetName());
    TQStringUtils::removeLeadingText(name,prefix+".");
    RooRealVar* var = dynamic_cast<RooRealVar*>(parameters->find(name));
    double val;
    if(!var){
      error(TString::Format("unable to initialize parameter '%s' - not found!",name.Data()));
    } else if(!options->getTagDouble(key->GetName(),val)){
      error(TString::Format("internal error - no value set for parameter '%s'",name.Data()));
    } else if(var->isConstant() && !forceSetConst){
      warn(TString::Format("skipping constant parameter '%s'",name.Data()));
    } else {
      if(val > var->getMax()){
        var->setMax(val);
      } else if(val < var->getMin()){
        var->setMin(val);
      }
      var->setVal(val);
      if(val == var->getVal()){
        info(TString::Format("initializing parameter '%s' to '%g'",name.Data(),val));
      } else {
        info(TString::Format("failed to initialize parameter '%s' to '%g'",name.Data(),val));
      }
    }
  }
}

void TSUtils::setParameterErrors(const RooAbsCollection* parameters, TQTaggable* options, const TString& prefix, bool forceSetConst){
  // the parameters in the workspace to the values given by the tags
  DEBUGfunc("entering function");
  TQIterator params(options->getListOfKeys(prefix+".*"),true);
  while(params.hasNext()){
    TObject* key = params.readNext();
    if(!key) continue;
    TString name(key->GetName());
    TQStringUtils::removeLeadingText(name,prefix+".");
    RooRealVar* var = dynamic_cast<RooRealVar*>(parameters->find(name));
    double val;
    if(!var){
      error(TString::Format("unable to initialize parameter error '%s' - not found!",name.Data()));
    } else if(!options->getTagDouble(key->GetName(),val)){
      error(TString::Format("internal error - no value set for parameter '%s'",name.Data()));
    } else if(var->isConstant() && !forceSetConst){
      DEBUGfunc("skipping constant parameter");
      continue;
    } else {
      info(TString::Format("initializing parameter error '%s' to '%g'",name.Data(),val));
      var->setError(val);
    }
  }
}

//__________________________________________________________________________________|___________

void TSUtils::setParameterValues(const RooAbsCollection& parameters, TQFolder* fitResult){
  // the parameters in the workspace to the values given by the fit result
  if(!fitResult) return;
  TQFolderIterator pars(fitResult->getListOfFolders("?"),true);
  while(pars.hasNext()){
    TQFolder* par = pars.readNext();
    RooRealVar* p = dynamic_cast<RooRealVar*>(parameters.find(par->GetName()));
    if(!p){
      warn(TString::Format("unable to find parameter '%s' in set '%s'",par->GetName(),parameters.GetName()));
      continue;
    }
    p->setVal(par->getTagDoubleDefault("val",0.));
  }
}


//__________________________________________________________________________________|___________

void TSUtils::setParametersConstant(const RooAbsCollection* parameters, const std::vector<TString>& parnames, bool constval){
  // the parameters in the workspace to the constant o given by the tags
  DEBUGfunc("entering function");
  for(const auto& name:parnames){
    for(auto* obj:*parameters){
      if(!obj) continue;
      RooRealVar* var = dynamic_cast<RooRealVar*>(obj);
      if(!var) continue;
      if(TQStringUtils::matches(var->GetName(),name)){
        if(var->isConstant() != constval){
          //          info(TString::Format("setting parameter '%s' to %s",var->GetName(),constval ? "constant" : "floating"));
          var->setConstant(constval);
        }
      }
    }
  }
}

//__________________________________________________________________________________|___________

void TSUtils::addParameters(const RooAbsCollection* parameters, const std::vector<TString>& parnames, RooAbsCollection& pars){
  // the parameters in the workspace to the constant o given by the tags
  DEBUGfunc("entering function");
  TString buf;
  for(const auto& name:parnames){
    for(auto* obj:*parameters){
      if(!obj) continue;
      RooRealVar* var = dynamic_cast<RooRealVar*>(obj);
      if(!var) continue;
      if(TQStringUtils::matches(var->GetName(),name)){
        pars.add(*var);
      }
    }
  }
}

//__________________________________________________________________________________|___________


RooDataSet* TSUtils::getDataHistogram(RooAbsPdf* pdf, RooArgSet* observables, RooRealVar* weightVar, RooAbsArg* moreArgs){
  // obtain a RooDataHist from a PDF
  const double infty = pow(10., 18);
  TString name = TString::Format("data_%s", pdf->GetName());

  RooArgSet * obs = pdf->getObservables(observables);
  RooRealVar* thisObs = dynamic_cast<RooRealVar*>(obs->first());

  RooArgSet allVars;
  allVars.add(*obs);
  if(moreArgs) allVars.add(*moreArgs);

  RooDataSet * data = NULL;
  if(weightVar){
    allVars.add(*weightVar);
    data = new RooDataSet(name,name,allVars,RooFit::WeightVar(*weightVar));
  } else {
    data = new RooDataSet(name,name,allVars);
  }


  double expectedEvents = pdf->expectedEvents(obs);
  for (Int_t jj = 0; jj < thisObs->numBins(); ++jj) {
    thisObs->setBin(jj);
    double thisNorm = pdf->getVal(obs) * thisObs->getBinWidth(jj);
    double val = thisNorm * expectedEvents;
    if (val == val && val >= 0. && val < infty){
      data->add(*obs, val);
    } else {
      throw std::runtime_error(TString::Format("Invalid entries for bin %d of '%s': %g",jj,pdf->GetName(),val).Data());
    }
  }
  return data;
}


//__________________________________________________________________________________|___________

TH1* TSUtils::getHistogram(RooAbsPdf* pdf, RooArgSet* observables){
  // obtain a TH1 from a PDF
  const double infty = pow(10., 18);
  TString name = TString::Format("h_data_%s", pdf->GetName());

  RooArgSet * obs = pdf->getObservables(observables);
  RooRealVar* thisObs = dynamic_cast<RooRealVar*>(obs->first());

  std::vector<double> bins = TSUtils::getBinning(pdf,thisObs);
  TH1* data = new TH1F(name,name,bins.size()-1,&bins[0]);
  data->SetDirectory(NULL);

  double expectedEvents = pdf->expectedEvents(obs);
  for (Int_t jj = 0; jj < thisObs->numBins(); ++jj) {
    thisObs->setBin(jj);
    double thisNorm = pdf->getVal(obs) * thisObs->getBinWidth(jj);
    double val = thisNorm * expectedEvents;
    if (val > 0. && val < infty){
      data->SetBinContent(jj+1,val);
    } else {
      throw std::runtime_error(TString::Format("Invalid entries for bin %d",jj).Data());
    }
  }
  return data;
}

//__________________________________________________________________________________|___________

std::vector<double> TSUtils::getBinning(RooAbsPdf* pdf, RooRealVar* obs){
  // retrieve the list of bin boundaries
  auto bl = pdf->binBoundaries(*obs,obs->getMin(),obs->getMax());
  if(!bl){
    throw std::runtime_error(TString::Format("unable to retrieve bin boundaries from pdf segment '%s' for observable '%s'!",pdf->GetName(),obs->GetName()).Data());
  }
  std::vector<double> bins;
  for (auto it=bl->begin() ; it!=bl->end() ; ++it) {
    bins.push_back(*it);
  }
  if(bins[bins.size()-1] < obs->getMax()){
    bins.push_back(obs->getMax());
  }
  return bins;
}

//__________________________________________________________________________________|___________

double TSUtils::diffParameterSets(TQFolder* p1, TQFolder* p2, bool /*verbose*/){
  // get the numerical difference(s) between two parameter sets
  TQFolderIterator itr(p2->getListOfFolders("?"),true);
  double d = 0;
  while(itr.hasNext()){
    TQFolder* f2 = itr.readNext();
    if(!f2) continue;
    TQFolder* f1 = p1->getFolder(f2->GetName());
    if(!f1) continue;
    double v1 = f1->getTagDoubleDefault("val",0.);
    double v2 = f2->getTagDoubleDefault("val",0.);
    d += pow(v1-v2,2);
  }
  return sqrt(d);
}

//__________________________________________________________________________________|___________

int TSUtils::nParameters(RooAbsReal* func){
  // retrun the number of floating parameters a function depends on
   RooArgSet* paramSet = func->getParameters(RooArgSet());
   int n = 0;
   for(auto* arg:*paramSet){
     if (arg->IsA()->InheritsFrom(RooAbsRealLValue::Class()) && !arg->isConstant()){
       n++;
     }
   }
   delete paramSet;
   return n;
}

//__________________________________________________________________________________|___________

TMatrixDSym TSUtils::getCorrelationMatrix(const TMatrixDSym& V) {
  // convert a covariance matrix to a correlation matrix
  TMatrixDSym C(V.GetNcols()) ;
  for (int i=0 ; i<V.GetNrows() ; ++i) {
    for (int j=0 ; j<V.GetNcols() ; ++j) {
      const double corr = V(i,j)/sqrt(V(i,i)*V(j,j)) ;
      C(i,j) = corr;
      C(j,i) = corr;
    }
  }
  return C;
}

//__________________________________________________________________________________|___________

void TSUtils::applySettings(TQTaggable* config, const RooArgSet& allVars, RooStats::ModelConfig* mc){

  std::vector<std::string> pois;
  std::vector<std::string> nps;
  std::vector<std::string> globs;
  std::vector<std::string> obs;

  if(mc){
    TSUtils::getParameterNames(mc->GetNuisanceParameters(),nps);
    if(mc->GetNuisanceParameters())
      assert(mc->GetNuisanceParameters()->getSize() == nps.size());
    TSUtils::getParameterNames(mc->GetParametersOfInterest(),pois);
    if(mc->GetParametersOfInterest())
      assert(mc->GetParametersOfInterest()->getSize() == pois.size());
    TSUtils::getParameterNames(mc->GetGlobalObservables(),globs);
    if(mc->GetGlobalObservables())
      assert(mc->GetGlobalObservables()->getSize() == globs.size());
    TSUtils::getParameterNames(mc->GetObservables(),obs);
    if(mc->GetObservables())
      assert(mc->GetObservables()->getSize() == obs.size());
  }
  TString constParsFilter   = TQStringUtils::concat(config->getTagVString("constPars"));
  TString constExcept = TQStringUtils::concat(config->getTagVString("constPars.except"));
  TString floatParsFilter   = TQStringUtils::concat(config->getTagVString("floatPars"));
  TString floatExcept = TQStringUtils::concat(config->getTagVString("floatPars.except"));
  std::vector<std::string> addNPs = config->getTagVStandardString("addNPs");
  std::vector<std::string> removeNPs = config->getTagVStandardString("removeNPs");
  std::vector<std::string> addPOIs = config->getTagVStandardString("addPOIs");
  std::vector<std::string> removePOIs = config->getTagVStandardString("removePOIs");
  std::vector<std::string> addObs = config->getTagVStandardString("addObs");
  std::vector<std::string> removeObs = config->getTagVStandardString("removeObs");
  std::vector<std::string> addGlobs = config->getTagVStandardString("addGlobs");
  std::vector<std::string> removeGlobs = config->getTagVStandardString("removeGlobs");
  TSUtils::setParameterValues(&(allVars),config,"setPars",true);
  TSUtils::setParameterErrors(&(allVars),config,"setParErrors",true);

  TQIterator itr(allVars.createIterator());
  while (itr.hasNext()) {
    TObject* obj = itr.readNext();
    if(!obj) continue;
    RooRealVar * var = dynamic_cast<RooRealVar*>(obj);
    if(!var) continue;
    bool setConst = /*is in whitelist*/ TQStringUtils::matchesFilter(var->GetName(), constParsFilter, ",", true) &&
      /*and not in blacklist*/ !TQStringUtils::matchesFilter(var->GetName(), constExcept, ",", true);
    bool setFloat = /*is in whitelist*/ TQStringUtils::matchesFilter(var->GetName(), floatParsFilter, ",", true) &&
      /*and not in blacklist*/ !TQStringUtils::matchesFilter(var->GetName(), floatExcept, ",", true);
    bool addNP = false;
    bool removeNP = false;
    bool addPOI = false;
    bool removePOI = false;
    bool addOb = false;
    bool removeOb = false;
    bool addGlob = false;
    bool removeGlob = false;
    for(const auto& vname:addNPs){
      if(TQStringUtils::matchesFilter(var->GetName(),vname)){
        addNP = true;
      }
    }
    for(const auto& vname:removeNPs){
      if(TQStringUtils::matchesFilter(var->GetName(),vname)){
        removeNP = true;
      }
    }
    for(const auto& vname:addPOIs){
      if(TQStringUtils::matchesFilter(var->GetName(),vname)){
        addPOI = true;
      }
    }
    for(const auto& vname:removePOIs){
      if(TQStringUtils::matchesFilter(var->GetName(),vname)){
        removePOI = true;
      }
    }
    for(const auto& vname:addObs){
      if(TQStringUtils::matchesFilter(var->GetName(),vname)){
        addOb = true;
      }
    }
    for(const auto& vname:removeObs){
      if(TQStringUtils::matchesFilter(var->GetName(),vname)){
        removeOb = true;
      }
    }
    for(const auto& vname:addGlobs){
      if(TQStringUtils::matchesFilter(var->GetName(),vname)){
        addGlob = true;
      }
    }
    for(const auto& vname:removeGlobs){
      if(TQStringUtils::matchesFilter(var->GetName(),vname)){
        removeGlob = true;
      }
    }
    if(setConst && !setFloat){
      var->setConstant(true);
      info(TString::Format("setting parameter '%s' constant",var->GetName()));
    }
    //add/remove NP
    if(addNP){
      auto found_np = std::find(nps.begin(),nps.end(),std::string(var->GetName()));
      if(found_np == nps.end()){
        nps.push_back(var->GetName());
        info(TString::Format("adding parameter '%s' to the list of nuisance parameters",var->GetName()));
      }
    } else if(removeNP){
      info(TString::Format("removing parameter '%s' from the list of nuisance parameters",var->GetName()));
      auto found_np = std::find(nps.begin(),nps.end(),std::string(var->GetName()));
      if(found_np != nps.end()) nps.erase(found_np);
    }
    //add/remove POI
    if(addPOI){
      auto found_poi = std::find(pois.begin(),pois.end(),std::string(var->GetName()));
      if(found_poi == pois.end()){
        pois.push_back(var->GetName());
        info(TString::Format("adding parameter '%s' to the list of parameters of interest",var->GetName()));
      }
    } else if(removePOI){
      info(TString::Format("removing parameter '%s' from the list of parameters of interest",var->GetName()));
      auto found_poi = std::find(pois.begin(),pois.end(),std::string(var->GetName()));
      if(found_poi != pois.end()) pois.erase(found_poi);
    }
    //add/remove Obs (Observable)
    if(addOb){
      auto found_obs = std::find(obs.begin(),obs.end(),std::string(var->GetName()));
      if(found_obs == obs.end()){
        obs.push_back(var->GetName());
        info(TString::Format("adding parameter '%s' to the list of observables",var->GetName()));
      }
    } else if(removeOb){
      info(TString::Format("removing parameter '%s' from the list of observables",var->GetName()));
      auto found_obs = std::find(obs.begin(),obs.end(),std::string(var->GetName()));
      if(found_obs != obs.end()) obs.erase(found_obs);
    }
    //add/remove Glob (GlobalObservable)
    if(addGlob){
      auto found_glob = std::find(globs.begin(),globs.end(),std::string(var->GetName()));
      if(found_glob == globs.end()){
        globs.push_back(var->GetName());
        info(TString::Format("adding parameter '%s' to the list of global observables",var->GetName()));
      }
    } else if(removeGlob){
      info(TString::Format("removing parameter '%s' from the list of global observables",var->GetName()));
      auto found_glob = std::find(globs.begin(),globs.end(),std::string(var->GetName()));
      if(found_glob != globs.end()) globs.erase(found_glob);
    }

    if(setFloat && !setConst){
      info(TString::Format("setting parameter '%s' floating",var->GetName()));
      var->setConstant(false);
    }
    if(setFloat && setConst){
      warn(TString::Format("refusing to change const property of parameter '%s' matching both constPars='%s' and floatPars='%s'",var->GetName(),constParsFilter.Data(),floatParsFilter.Data()));
    }
  }

  if(mc){
    RooWorkspace* ws = mc->GetWorkspace();
    if(!ws) throw std::runtime_error("ModelConfig has no workspace!");

    RooArgSet npset; getArgs(ws,nps,npset);
    mc->SetNuisanceParameters(npset);
    assert(mc->GetNuisanceParameters()->getSize() == nps.size());
    RooArgSet poiset; getArgs(ws,pois,poiset);
    mc->SetParametersOfInterest(poiset);
    assert(mc->GetParametersOfInterest()->getSize() == pois.size());
    RooArgSet obsset; getArgs(ws,obs,obsset);
    mc->SetObservables(obsset);
    assert(mc->GetObservables()->getSize() == obs.size());
    RooArgSet globset; getArgs(ws,globs,globset);
    mc->SetGlobalObservables(globset);
    assert(mc->GetGlobalObservables()->getSize() == globs.size());
  }

}


//hack section (advanced stuff to tame RooFit)

//__________________________________________________________________________________|___________

std::vector<const RooAbsArg*> * TSUtils::getConstituents(const RooAbsArg* parent) {
  //convenience wrapper to extract constituent terms from various RooFit classes
  //"... because uniform interfaces are too mainstream ..."
  //caller takes ownership of returned vector object (but not its contents)

  if (!parent) return nullptr;

  const TClass* parentClass = parent->IsA();
  std::vector<const RooAbsArg*>* children = nullptr;

  if (parentClass == RooSimultaneous::Class()) {
    //case: it's a RooSimultaneous
    //since the pdfs inside a RooSimultaneous are in a protected member we use a small wrapper class to access them
    return ::RooSimultaneousHelper::getConstituents(dynamic_cast<const RooSimultaneous*>(parent));
  } else if (parentClass == RooRealSumPdf::Class()) {
    const RooRealSumPdf* sum = dynamic_cast<const RooRealSumPdf*>(parent);
    if (sum) {
      children = new std::vector<const RooAbsArg*>();
      for(auto* arg:sum->funcList()){
        if (!arg) continue;
        children->push_back(arg);
      }
    }
    return children;
  } else if (parentClass == RooProdPdf::Class()) {
    //case: RooProdPdf
    const RooProdPdf* prod = dynamic_cast<const RooProdPdf*>(parent);
    if (prod) {
      children = new std::vector<const RooAbsArg*>();
      for(auto* arg:prod->pdfList()){
        if (!arg) continue;
        children->push_back(arg);
      }
    }
    return children;
  } else if (parentClass == RooProduct::Class()) {
    //case: RooProduct
    const RooProduct* prod = dynamic_cast<const RooProduct*>(parent);
    if (prod) {
      children = new std::vector<const RooAbsArg*>();
      //thanks to RooProduct::components not being marked as const, we need this ugly cast here...
      RooArgList al = const_cast<RooProduct*>(prod)->components(); //we need to store the RooArgsList in some object as it's returned by value, not by reference
      for(auto* arg:al){
        if (!arg) continue;
        children->push_back(arg);
      }
    }
    return children;
  } else if (parentClass == PiecewiseInterpolation::Class()) {
    //case: PiecewiseInterpolation
    const PiecewiseInterpolation* interp = dynamic_cast<const PiecewiseInterpolation*>(parent);
    if (interp) {
      children = new std::vector<const RooAbsArg*>();
      const RooAbsArg* nominal = ::PiecewiseInterpolationHelper::getNominalMember(interp);
      if (nominal) children->push_back(nominal); //add the "_nominal" member (typically a RooHistFunc)
      for(auto* arg:interp->paramList()){
        if (!arg) continue;
        children->push_back(arg);
      }
    }
    return children;
  } /*else {
    std::cout<<"Unsupported Class? "<<parent->IsA()->GetName()<<std::endl;
    if (parent->IsA() == ParamHistFunc::Class()) parent->Print();
    if (parent->IsA() == RooStats::HistFactory::FlexibleInterpVar::Class()) parent->Print();
  }*/
  //eventually return a nullptr if we didn't get a class that has constituents (or is not supported)
  return children;
} // end getConstituents

//__________________________________________________________________________________|___________

std::set<const RooAbsArg*>* TSUtils::getContributionsRecursive(const RooAbsArg* rootNode, TClass* cl, bool recurseMatching, std::set<const RooAbsArg*>* elements) {
  if (!rootNode) return elements; //possibly nullptr

  std::vector<const RooAbsArg*>* constituents = getConstituents(rootNode);
  if (!constituents) return elements; //possibly nullptr / nothing to do here, can't recurse

  if (!elements) elements = new std::set<const RooAbsArg*>();
  recurseMatching = recurseMatching || !cl; //if not explicit class is specified we always recurse

  for (const RooAbsArg* node : *constituents) {
    if (!node) continue;
    if (!cl || node->IsA() == cl) { //matching type
      elements->insert(node);
    }
    if (recurseMatching || !(node->IsA() == cl)) { //not a matching type or we should also recurse within matching types (this includes also not having an explicit class specified)
      getContributionsRecursive(node, cl, recurseMatching, elements);
    }
  }
  delete constituents;
  constituents = nullptr;

  return elements;
}

std::map<std::string,RooArgSet>* TSUtils::getNamedSets(RooWorkspace* ws) {
  return &((*ws).*RooWorkspaceHackResult<RooWorkspace_namedSets>::ptr);
}


namespace {
  enum MatrixStatus {
    DIAG_UP_OK,
    DIAG_DOWN_OK,
    DIAG_UP_FLIPPED,
    DIAG_DOWN_FLIPPED,
    NOT_OK
  };

  // test the orientation of the matrix
  MatrixStatus validateMatrix(TH2* hist, bool isCorrelation){
    if(!hist) return NOT_OK;
    size_t n = hist->GetNbinsX();
    if(n != (size_t) hist->GetNbinsY()) return NOT_OK;

    bool diag_down_vals = true;
    bool diag_up_vals = true;

    // check if the values are distributed sanely
    // for correlation matrices, just check that a diagonal is 1 (and which one it is)
    // for covariance matrix, try converting it into a correlation
    // matrix using both possible orientations and check which one
    // fits the above criterion
    for(size_t i=0; i<n; ++i){
      double maxval_v1 = -inf;
      double maxval_v2 = -inf;
      int maxidx_v1 = -1;
      int maxidx_v2 = -1;

      double ii1 = (hist->GetBinContent(i+1,i+1));
      double ii2 = (hist->GetBinContent(i+1,n-i));

      for(size_t j=0; j<n; ++j){
        double val = hist->GetBinContent(i+1,j+1);
        double v1,v2;
        if(isCorrelation){
          v1 = val;
          v2 = val;
        } else {
          double jj1 = (hist->GetBinContent(j+1,j+1));
          double jj2 = (hist->GetBinContent(n-j,j+1));

          v1 = val / sqrt(ii1 * jj1);
          v2 = val / sqrt(ii2 * jj2);
        }
        if(v1 > maxval_v1){
          maxval_v1 = v1;
          maxidx_v1 = j;
        }
        if(v2 > maxval_v2){
          maxval_v2 = v2;
          maxidx_v2 = (int) j;
        }
      }
      if(! (TMath::AreEqualRel(maxval_v1,1.,1e-3) && (maxidx_v1 ==   (int) i  )) ){
        diag_up_vals = false;
      }
      if(! (TMath::AreEqualRel(maxval_v2,1.,1e-3) && (maxidx_v2 == (int) (n-i-1))) ){
        diag_down_vals = false;
      }
    }


    // check if the labels are distributed sanely
    bool diag_down_labels = true;
    bool diag_up_labels = true;
    for(size_t i=0; i<n; ++i){
      if(!TQStringUtils::equal(hist->GetXaxis()->GetBinLabel(i+1),hist->GetYaxis()->GetBinLabel(i+1))){
        diag_up_labels = false;
      }
      if(!TQStringUtils::equal(hist->GetXaxis()->GetBinLabel(i+1),hist->GetYaxis()->GetBinLabel(n-i))){
        diag_down_labels = false;
      }
    }


    if(diag_up_vals   && diag_up_labels  ) return DIAG_UP_OK;
    if(diag_up_vals   && diag_down_labels) return DIAG_UP_FLIPPED;
    if(diag_down_vals && diag_down_labels) return DIAG_DOWN_OK;
    if(diag_down_vals && diag_up_labels  ) return DIAG_DOWN_FLIPPED;

    return NOT_OK;
  }
}

  TH2* TSUtils::makeValidMatrix(TH2* orig, bool up, bool isCovariance, bool makeCorrelation, bool verbose){
    size_t n = orig->GetXaxis()->GetNbins();
    TH2* correlations = new TH2F("correlations","correlations",n,0,n,n,0,n);
    correlations->SetDirectory(NULL);

    MatrixStatus status = validateMatrix(orig,!isCovariance);
    bool down = !up;
    bool flipXlabels = false;
    // bool flipXvalues = false; // BW: hashed (set but not used, also hashed below)
    // bool diag_down = false;   // BW: hashed (unused variable)
    /*   */if(down && status == DIAG_DOWN_OK){
      if(verbose) info("using matrix as-is");
    } else if(down && status == DIAG_DOWN_FLIPPED){
      if(verbose) info("flipping X labels");
      flipXlabels = true;
    } else if(up && status == DIAG_UP_OK){
      if(verbose) info("using matrix as-is");
    } else if(up && status == DIAG_UP_FLIPPED){
      if(verbose) info("flipping X labels");
      flipXlabels = true;
    } else if(down && status == DIAG_UP_OK){
      if(verbose) info("flipping X values & labels");
      flipXlabels = true;
      //flipXvalues = true;
    } else if(down && status == DIAG_UP_FLIPPED){
      if(verbose) info("flipping X values");
      //flipXvalues = true;
    } else if(up && status == DIAG_DOWN_OK){
      if(verbose) info("flipping X values & labels");
      flipXlabels = true;
      //flipXvalues = true;
    } else if(up && status == DIAG_DOWN_FLIPPED){
      if(verbose) if(verbose) info("flipping X values");
      //flipXvalues = true;
    } else {
      throw std::runtime_error("matrix is broken, aborting!");
    }

    for(size_t i=0; i<n; ++i){
      double xx = orig->GetBinContent(down?(i+1):(n-i),down?(n-i):(i+1));
      for(size_t j=0; j<n; ++j){
        double xy = orig->GetBinContent(down?(i+1):(n-i),j+1);
        double yy = orig->GetBinContent(n-j,j+1);
        double corr = ( isCovariance && makeCorrelation ) ? (xy / sqrt(xx * yy)) : xy;
        correlations->SetBinContent(i+1,j+1,corr);
      }
    }

    for(size_t i=0; i<n; ++i){
      int x = flipXlabels ? n-i : i+1;
      int y = i+1;
      correlations->GetXaxis()->SetBinLabel(i+1,orig->GetXaxis()->GetBinLabel(x));
      correlations->GetYaxis()->SetBinLabel(i+1,orig->GetYaxis()->GetBinLabel(y));
    }

    return correlations;

}

TVectorD TSUtils::getErrors(const TMatrixD& covariance){
  TVectorD errors(covariance.GetNcols());
  for(int i=0; i<covariance.GetNcols(); ++i){
    errors(i) = sqrt(covariance(i,i));
  }
  return errors;
}
TVectorD TSUtils::getErrors(const RooAbsCollection* parameters){
  TVectorD errors(parameters->getSize());
  int i=0; 
  for(auto arg:*parameters){
    RooRealVar* p = dynamic_cast<RooRealVar*>(arg);
    if(!p) continue;
    double e = p->getError();
    if(e <= 0){
      e = 0.5*(p->getErrorHi() + p->getErrorLo());
    }
    errors(i) = e;
  }
  return errors;
}
TMatrixDSym TSUtils::makeCovarianceMatrix(const TMatrixD& correlation, const TVectorD& errors){
  TMatrixDSym covariance(correlation.GetNcols());
  for(int i=0; i<correlation.GetNcols(); ++i){
    for(int j=0; j<correlation.GetNrows(); ++j){
      covariance(i,j) = correlation(i,j) * errors(i) * errors(j);
    }
  }
  return covariance;
}
TMatrixDSym TSUtils::makeCovarianceMatrix(const TMatrixD& correlation, const std::vector<double>& errors){
  TMatrixDSym covariance(correlation.GetNcols());
  for(int i=0; i<correlation.GetNcols(); ++i){
    for(int j=0; j<correlation.GetNrows(); ++j){
      covariance(i,j) = correlation(i,j) * errors[i] * errors[j];
    }
  }
  return covariance;
}
TMatrixDSym TSUtils::makeCorrelationMatrix(const TMatrixD& covariance){
  TVectorD errors(TSUtils::getErrors(covariance));
  TMatrixDSym correlation(covariance.GetNcols());
  for(int i=0; i<covariance.GetNcols(); ++i){
    for(int j=0; j<covariance.GetNrows(); ++j){
      correlation(i,j) = covariance(i,j) / errors(i) / errors(j);
    }
  }
  return correlation;
}


namespace {
  void getMeshHelper(const std::vector<RooRealVar*>& observables,std::vector<double>& currentValues, std::vector<std::vector<double> >& retval,size_t idx){
    if(idx == currentValues.size()){
      retval.push_back(currentValues);
    } else {
      RooRealVar* obs = observables[idx];
      size_t nbins = obs->getBinning().numBins();
      for(size_t i=0; i<nbins; ++i){
        currentValues[idx] = obs->getBinning().binCenter(i);
        getMeshHelper(observables,currentValues,retval,idx+1);
      }
    }
  }
  
  std::vector<std::vector<double> > getMesh(const std::vector<RooRealVar*>& observables){
    std::vector<std::vector<double> > retval;
    std::vector<double> currentValues(observables.size());
    getMeshHelper(observables,currentValues,retval,0);
    return retval;
  }

  void getVariations(RooAbsReal* func, RooArgList& params, std::vector<double>& up, std::vector<double>& dn,double relThreshold){
    RooArgSet* servers = func->getParameters((RooArgSet*)0);
    Double_t nom = func->getVal();
    if(!TQUtils::isNum(nom)){
      throw std::runtime_error("unable to process non-numeric nominal value!");
    }
    for (Int_t ivar=params.getSize()-1 ; ivar>=0; --ivar) {
      RooRealVar* p = (RooRealVar*)(params.at(ivar));
      Double_t cenVal = p->getVal() ;
      Double_t errVal = p->getError();
      RooRealVar* v = (RooRealVar*)(servers->find(p->GetName()));
      if(!v || !TQUtils::isNum(errVal)){
        params.remove(*p,true,true);
        continue;
      }
      // Make Plus variation
      v->setVal(cenVal+errVal) ;
      v->setValueDirty();
      const double plusVar = func->getVal();
      // Make Minus variation
      v->setVal(cenVal-errVal) ;
      v->setValueDirty();
      const double minusVar = func->getVal();
      v->setVal(cenVal) ;
      //      std::cout << func->GetName() << " " << func->ClassName() << " " << v->GetName() << " " << plusVar << " " << nom << " " << minusVar << std::endl;
      if(fabs(plusVar/nom - 1) > relThreshold || fabs(minusVar/nom - 1) > relThreshold){
        //        std::cout << " acc" << std::endl;
        up.push_back(plusVar);
        dn.push_back(minusVar);
      } else {
        //        std::cout << " dec" << std::endl;        
        params.remove(*v,true,true);
      }
    }
    delete servers;
  }
}

std::map<std::vector<double> ,RooAbsReal*> TSUtils::slice(RooAbsReal* func, const std::vector<RooRealVar*>& observables, const RooArgList& nps, double relThreshold){
  // slice a function, providing an array of separate functions, one for every bin in the n-dim grid of observables given
  // std::cout << "slicing " << func->GetName() << std::endl;
  RooArgSet obsset;
  for(auto obs:observables) obsset.add(*obs);
  std::map<std::vector<double> , RooAbsReal* > slices;
  if(!func->dependsOn(obsset)){
    auto mesh = ::getMesh(observables);
    for(const auto& coordinates:mesh){
      slices[coordinates] = func;
    }
  } else if(func->InheritsFrom(RooAbsPdf::Class())){
    RooWorkspace tmpws("tmpws");
    tmpws.import(*func);
    std::map<std::vector<double> , std::vector<RooAbsReal*> > compslices;
    std::vector<RooAbsArg*> servers;
    for(auto s:func->serverMIterator()){
      if(!s->InheritsFrom(RooAbsReal::Class())) continue;
      servers.push_back(s);
      auto slices = slice((RooAbsReal*)s,observables,nps,relThreshold);
      if(compslices.size() == 0){
        for(const auto& slice:slices){
          compslices[slice.first] = {slice.second};
          tmpws.import(*slice.second);
        }
      } else {
        for(const auto& slice:slices){
          compslices[slice.first].push_back(slice.second);
          tmpws.import(*slice.second);          
        }
      }
    }
    for(const auto& slice:compslices){
      std::stringstream edit;
      edit << "EDIT::" << func->GetName();
      for(size_t i=0; i<slice.first.size(); ++i){
        edit << "_" << observables[i]->GetName() << "_" << observables[i]->getBinning().binNumber(slice.first[i]);
        observables[i]->setVal(slice.first[i]);
      }
      edit << "(" << func->GetName();
      for(size_t i=0; i<slice.first.size(); ++i){
        edit << "," << servers[i]->GetName() << "=" << slice.second[i]->GetName();
      }
      edit << ")";
      TString editstr(edit.str().c_str());
      RooAbsReal* slicefunc = (RooAbsReal*)(tmpws.factory(editstr.Data()));
      slices[slice.first] = (RooAbsReal*)(slicefunc->cloneTree());
    }
  } else {
    auto mesh = ::getMesh(observables);
    for(const auto& coordinates:mesh){
      std::stringstream ss;
      ss << func->GetName();
      for(size_t i=0; i<coordinates.size(); ++i){
        ss << "_" << observables[i]->GetName() << "_" << observables[i]->getBinning().binNumber(coordinates[i]);
        observables[i]->setVal(coordinates[i]);
      }
      double central = func->getVal();
      TString name(ss.str());
      std::vector<double> up,dn;
      RooArgList localnps(nps);
      ::getVariations(func,localnps,up,dn,relThreshold);
      RooAbsReal* slice = NULL;
      if(localnps.getSize() > 0){
        slice = new RooStats::HistFactory::FlexibleInterpVar(name.Data(),name.Data(),localnps,central,up,dn);
      } else {
        slice = new RooConstVar(name.Data(),name.Data(),central);
      }
      slices[coordinates] = slice;
    }
  }
  // std::cout << "done slicing " << func->GetName() << std::endl;
  return slices;
}
