#include <SFramework/TSHistogramExporter.h>
#include <SFramework/TSUtils.h>

#include "QFramework/TQStringUtils.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQIterator.h"

#include "RooProduct.h"
#include "RooRealIntegral.h"
#include "RooSimultaneous.h"
#include "RooAddPdf.h"
#include "RooConstVar.h"
#include "RooProdPdf.h"
#include "RooBinning.h"

#include "TH1.h"
#include "TAxis.h"
#include "TMath.h"

// #define _DEBUG_

#ifdef __cpp_generic_lambdas
#if __cpp_generic_lambdas >= 201304
#define HAS_GENERIC_LAMBDAS
#endif
#endif

namespace {
  void info(const TString& message) {
    std::cout << "SFramework/TSUtils: " << message.Data() << std::endl;
  }
  // void error(const TString& message) {
  //   info(TQStringUtils::makeBoldRed(TString("ERROR: ") + message));
  // }
  void warn(const TString& message) {
    info(TQStringUtils::makeBoldYellow(TString("WARNING: ") + message));
  }

}

#include "QFramework/TQLibrary.h"

ClassImp(TSHistogramExporter)

RooExpectedEvents::RooExpectedEvents(const char* name, const char* title, RooAbsPdf& pdf, const RooArgSet& normSet) : RooAbsReal(name,title),
  _pdf(pdf),
  _normSet(normSet)
{
  addServer(_pdf);
}

RooExpectedEvents::~RooExpectedEvents(){}

double RooExpectedEvents::evaluate() const {
  return this->_pdf.expectedEvents(_normSet);
}

TObject* RooExpectedEvents::clone(const char*) const {
  return new RooExpectedEvents(this->GetName(),this->GetTitle(),_pdf,_normSet);
}

ClassImp(RooExpectedEvents)

#ifdef HAS_GENERIC_LAMBDAS
namespace {
  namespace SFINAE {
    // SFINAE test
    // https://stackoverflow.com/questions/257288/is-it-possible-to-write-a-template-to-check-for-a-functions-existence


    class true_val    {char dummy[1];};
    class false_val   {char dummy[2];};

    template <typename T>
    class hasCompSelectSwitch {
    public:
      template <typename C> static true_val    test(decltype(&C::setAllowComponentSelection));
      template <typename C> static false_val   test(...);

      enum { value = sizeof(test<T>(0)) == sizeof(true_val) };
    };

    // static_if
    //https://stackoverflow.com/questions/37617677/implementing-a-compile-time-static-if-logic-for-different-string-types-in-a-co
    template <typename T, typename F>          auto static_if(std::true_type, T t, F /*f*/) { return t; }
    template <typename T, typename F>          auto static_if(std::false_type, T /*t*/, F f) { return f; }
    template <bool B, typename T, typename F>  auto static_if(T t, F f) { return static_if(std::integral_constant<bool, B>{}, t, f); }
    template <bool B, typename T>              auto static_if(T t) { return static_if(std::integral_constant<bool, B>{}, t, [](auto&&...){}); }

    template <class C> inline void trySetAllowComponentSelection(C* integral, bool val){
      static_if<hasCompSelectSwitch<C>::value>
        ([&](auto* ri) {ri->setAllowComponentSelection(val); })
        (integral);
    }
  }
}
#endif

namespace {


  struct Exposer : RooAbsReal {
    using RooAbsReal::plotOnCompSelect;
  };

  template<class T> bool has(const std::vector<T> v,T x){
    for(const auto& e:v){
      if(e==x) return true;
    }
    return false;
  }

  void plotOnCompSelect(const RooAbsReal* thisObj,RooArgSet* selNodes){
    (thisObj->*&Exposer::plotOnCompSelect)(selNodes);
  }


  void findUniqueProdComponents(RooProdPdf* Pdf, RooArgSet& components, const TString& filter) {
    static int counter = 0;
    counter++;

    if (counter > 50) {
      throw std::runtime_error("FindUniqueProdComponents detected infinite loop. Please check.");
    }

    RooArgList pdfList = Pdf->pdfList();
    if (pdfList.getSize() == 1) {
      components.add(pdfList);
    } else {
      TIterator* pdfItr = pdfList.createIterator();
      RooAbsArg* nextArg;
      while ((nextArg = (RooAbsArg*)pdfItr->Next())) {
        RooProdPdf* pdf = (RooProdPdf*)nextArg;
        if(pdf->IsA() != RooProdPdf::Class()){
          if(TQStringUtils::matches(pdf->GetName(),filter)) components.add(*pdf);
          continue;
        }
        findUniqueProdComponents(pdf, components, filter);
      }
      delete pdfItr;
    }
    counter = 0;
  }
}


TSHistogramExporter::TSHistogramExporter(TQFolder* style, RooFitResult* fr) :
  styleModel(style),
  fitResult(fr)
{
  // default constructor
}

TSHistogramExporter::~TSHistogramExporter(){
}

RooCategory* TSHistogramExporter::Region::getChannelCategory(){
  return TSHistogramExporter::getCategories(this->simPdf);
}

void TSHistogramExporter::Region::select(){
  if(this->getChannelCategory()){
    this->getChannelCategory()->setLabel(this->getName());
  }
}

TSHistogramExporter::Region::~Region(){
}

bool TSHistogramExporter::Region::isCombined() const {
  return this->combined;
}
const RooArgSet& TSHistogramExporter::Region::getObservables() const { return this->observables; }
const TString& TSHistogramExporter::Region::getName() const { return this->name; }
TSHistogramExporter::Region::Region(const TString& n, RooAbsPdf* p, RooArgSet* obs, bool combined) : name(n), func(p), observables(*obs), simPdf(0), combined(combined)
{
  // do nothing
}
TSHistogramExporter::Region::Region(const TString& n, RooSimultaneous* sim, RooRealSumPdf* p, RooArgSet* obs, bool combined) : Region(n,(RooAbsPdf*)p,obs,combined) {
  this->simPdf = sim;
  addSamples(p);
}
RooDataSet* TSHistogramExporter::Region::getData(const TString& name, int index) const {
  return this->datasets.at(name)[index];
}
int TSHistogramExporter::Region::nData(const TString& name) const {
  return this->datasets.at(name).size();
}

std::vector<TString> TSHistogramExporter::Region::allData() const {
  std::vector<TString> retval;
  for(const auto it:datasets){
    retval.push_back(it.first);
  }
  return retval;
}
std::vector<TString> TSHistogramExporter::Region::allSamples() const {
  std::vector<TString> retval;
  for(const auto it:samples){
    retval.push_back(it.first);
  }
  return retval;
}
void TSHistogramExporter::Region::print() const {
  std::cout << "Region '" << this->name << "': " << func->GetName() << " " << ( this->combined ? "combined " : "single" ) << " observables: ";
  RooAbsArg* obj = NULL;
  RooFIter itr(observables.fwdIterator());
  while((obj = itr.next())){
    RooRealVar* var = dynamic_cast<RooRealVar*>(obj);
    if(var){
      std::cout << var->GetName() << ",";
    }
  }
  std::cout <<std::endl;
  for(const auto& it:this->samples){
    std::cout << "  " << it.first << ": ";
    for(const auto& s:it.second){
      std::cout << s->GetName() << " ";
    }
    std::cout << std::endl;
  }
  for(const auto& it:this->datasets){
    std::cout << "  " << it.first << ": ";
    for(const auto& d:it.second){
      std::cout << d->GetName() << " ";
    }
    std::cout << std::endl;
  }
}
RooAbsPdf* TSHistogramExporter::Region::getPdf() const { return const_cast<RooAbsPdf*>(func); }
void TSHistogramExporter::Region::addSample(const TString& name, RooAbsReal* obj){
  if(!has(this->samples[name],obj))
    this->samples[name].push_back(obj);
}
void TSHistogramExporter::Region::addData(const TString& name, RooDataSet* obj){
  if(!has(this->datasets[name],obj))
    this->datasets[name].push_back(obj);
}
void TSHistogramExporter::Region::addSamples(const Region* other){
  if(!other) return;
  for(const auto& it:other->samples){
    for(const auto& s:it.second){
      this->addSample(it.first,s);
    }
  }
}
void TSHistogramExporter::Region::addSamples(RooRealSumPdf* sumPdf){
  if(!sumPdf) return;
  RooAbsArg* obj = NULL;
  RooFIter itr(sumPdf->funcList().fwdIterator());
  TString regionname(sumPdf->GetName());
  TQStringUtils::removeTrailing(regionname,"_model");
  while((obj = itr.next())){
    RooProduct* prod = dynamic_cast<RooProduct*>(obj);
    if(!prod){
      throw std::runtime_error(TString::Format("object %s is not of type 'RooProduct'",obj->GetName()).Data());
    }
    TString pname(prod->GetName());
    pname.Remove(0,4);
    int start = pname.Index(regionname);
    if(start < 0){
      start = pname.Index(regionname(0,regionname.First("_")));
    }
    if(start >= 0){
      pname.Remove(start-1,pname.Length()-start+1);
    } else {
      throw std::runtime_error(TString::Format("unable to isolate region name '%s' from '%s'",regionname.Data(),pname.Data()).Data());
    }
    addSample(pname,prod);
  }
}
double TSHistogramExporter::Region::expectedEvents() const {
  return func->expectedEvents(this->getObservables());
}

TH1* TSHistogramExporter::Region::makeHistogram(const TString& n,RooArgList& mylist) const {
  if(this->isCombined()) return NULL;
  if (mylist.getSize()==1){
    RooRealVar* xvar= dynamic_cast<RooRealVar*> (mylist.at(0));
    std::list<Double_t>* bl = func->binBoundaries(*xvar,xvar->getMin(),xvar->getMax()) ;
    Double_t* ba = new Double_t[bl->size()+1] ; int i=0 ;
    for (auto it=bl->begin() ; it!=bl->end() ; ++it) {
      ba[i++] = *it ;
    }
    if(ba[i-1] < xvar->getMax()*(1-1e-8)){
      ba[i]=xvar->getMax();
      i++;
    }

    TH1F* histo =  new TH1F(n, xvar->GetTitle(), i-1, ba);
    histo->GetXaxis()->SetTitle(xvar->GetTitle());
    delete[] ba ;
    delete bl;
    return histo;

  } else if (mylist.getSize()==2){
    RooRealVar* xvar= dynamic_cast<RooRealVar*> (mylist.at(0));
    std::list<Double_t>* bl = func->binBoundaries(*xvar,xvar->getMin(),xvar->getMax()) ;
    Double_t* ba = new Double_t[bl->size()+1] ; int i=0 ;
    RooRealVar* yvar= dynamic_cast<RooRealVar*> (mylist.at(1));
    std::list<Double_t>* bl_y = func->binBoundaries(*yvar,yvar->getMin(),yvar->getMax()) ;
    Double_t* ba_y = new Double_t[bl_y->size()+1] ; int j=0 ;
    for (auto it=bl->begin() ; it!=bl->end() ; ++it) {
      ba[i++] = *it ;
    }
    if(ba[i-1] < xvar->getMax()*(1-1e-8)){
      ba[i]=xvar->getMax();
      i++;
    }
    for (auto it_y=bl_y->begin() ; it_y!=bl_y->end() ; ++it_y) {
      ba_y[j++] = *it_y ;
    }
    if(ba_y[j-1] < yvar->getMax()*(1-1e-8)){
      ba_y[j]=yvar->getMax();
      j++;
    }
    TH2F* histo2D =  new TH2F(n, n, i-1, ba, j-1, ba_y);
    histo2D->GetXaxis()->SetTitle(xvar->GetTitle());
    histo2D->GetYaxis()->SetTitle(yvar->GetTitle());
    delete[] ba ;
    delete bl;
    delete[] ba_y ;
    delete bl_y;
    return histo2D;
  }
  return NULL;
}

RooAbsReal* RooPdfEvaluator::getFunc(){
  return this->_func;
}

RooPdfEvaluator::RooPdfEvaluator(const RooPdfEvaluator& other):RooAbsOptTestStatistic(other),
  nset(other.nset),
  params(other.params),
  covMat(other.covMat),
  corrMat(other.corrMat),
  threshold(other.threshold),
  ccheck(other.ccheck)
{
  this->_func = other._func;
  this->_data = NULL;
  this->_nEvents = 1;
  this->_setNum = 1;
  this->_numSets = 1;
  this->_hesse = (RooAbsPdf*)(other._hesse->clone());
}


RooPdfEvaluator::RooPdfEvaluator(RooAbsReal* f, const RooFitResult* fr, double t,double _ccheck) :
  _hesse(NULL),
  nset(NULL),
  params(NULL),
  threshold(t),
  ccheck(_ccheck)
{
  this->_data = NULL;
  this->_nEvents = 1;
  this->_setNum = 1;
  this->_numSets = 1;
  if(f && fr){
    this->_func = f;
    RooArgSet* parset = _func->getObservables(fr->floatParsFinal());
    this->params = new RooArgList();
    RooFIter itr(fr->floatParsFinal().fwdIterator());
    RooAbsArg* obj = NULL;;
    while((obj = itr.next())){
      const RooRealVar* comp = dynamic_cast<const RooRealVar*>(obj);
      if(!comp) continue;
      RooRealVar* par = dynamic_cast<RooRealVar*>(parset->find(comp->GetName()));
      if(!par){
        continue;
      }
      this->params->add(*par) ;
      par->setVal(comp->getVal());
      if (comp->getError() > 0.) {
        par->setError(comp->getError());
      }
      if (comp->getErrorLo() > 0. and comp->getErrorHi() > 0.) {
        par->setAsymError(comp->getErrorLo(),comp->getErrorHi());
      }
    }
    const int n(params->getSize());
    this->covMat.ResizeTo(n,n);
    this->corrMat.ResizeTo(n,n);

    this->covMat = n==fr->floatParsFinal().getSize()?fr->covarianceMatrix():fr->reducedCovarianceMatrix(*(this->params));
    for(int i=0; i<n; ++i){
      RooRealVar* par = dynamic_cast<RooRealVar*>(params->at(i));
      if(!par){
        throw std::runtime_error("internal inconsistency detected, invalid parameter encountered!");
      }
      RooRealVar* var = dynamic_cast<RooRealVar*>(fr->floatParsFinal().find(par->GetName()));
      if(!var){
        throw std::runtime_error(TString::Format("internal inconsistency detected, unable to find parameter '%s' in fit result!",par->GetName()).Data());
      }
      double err = var->getError();
      double errHi = var->getErrorHi();
      double errLo = var->getErrorLo();
      double cov = this->covMat(i,i);
      if(!TMath::AreEqualRel(cov,err*err,ccheck) && !TMath::AreEqualRel(cov,errHi*errHi,ccheck) && !TMath::AreEqualRel(cov,errLo*errLo,ccheck)){
        ::warn(TString::Format("internal consistency check failed for parameter %d '%s': pow(err,2)=%g, pow(errHi,2)=%g, pow(errLo,2)=%g,  cov(i,i)=%g!",i,var->GetName(),err*err,errHi*errHi,errLo*errLo,cov).Data());
      }
    }
    this->corrMat = TSUtils::getCorrelationMatrix(this->covMat);
    delete parset;
    this->_hesse = TSUtils::createHessePdf(fr,this->params);
  } else if(!fr){
    throw std::runtime_error("NULL fit result passed to RooPdfEvaluator!");
  }
}

TObject* RooPdfEvaluator::clone(const char*) const {
  return new RooPdfEvaluator(*this);
}

RooAbsTestStatistic* RooPdfEvaluator::create(const char *, const char *, RooAbsReal&, RooAbsData&,
                                                     const RooArgSet&, const char*, const char*,
                                                     Int_t, RooFit::MPSplit, Bool_t, Bool_t, Bool_t){
  // not applicable
  return NULL;
}

Double_t RooPdfEvaluator::evaluatePartition(
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,20,00)
                                            size_t /*firstEvent*/, size_t /*lastEvent*/, size_t /*stepSize*/
#else
                                            Int_t /*firstEvent*/, Int_t /*lastEvent*/, Int_t /*stepSize*/
#endif
                                            ) const {
  return this->_func->getVal(this->nset);
}


int RooPdfEvaluator::getParameterIndex(const char* pname) const{
  if(!this->params) return -1;
  for(int i=0; i<this->params->getSize(); ++i){
    if(TQStringUtils::equal(pname,params->at(i)->GetName())) return i;
  }
  return -1;
}

void RooPdfEvaluator::setCorrelation(const char* p1, const char* p2, double val){
  if(!this->params) return;
  for(int i=0; i<this->params->getSize(); ++i){
    for(int j=0; j<this->params->getSize(); ++j){
      if(i==j) continue;
      if(TQStringUtils::matches(params->at(i)->GetName(),p1) && TQStringUtils::matches(params->at(j)->GetName(),p2)){
        double v1 = sqrt(this->covMat(i,i));
        double v2 = sqrt(this->covMat(j,j));
        this->covMat (i,j) = v1*v2*val;;
        this->covMat (j,i) = v1*v2*val;;
        this->corrMat(i,j) = val;
        this->corrMat(j,i) = val;
      }
    }
  }
}

double RooPdfEvaluator::getCorrelation(const char* p1, const char* p2) const {
  int i1 = this->getParameterIndex(p1);
  int i2 = this->getParameterIndex(p2);
  if(i1<0 || i2 < 0) return 0;
  return this->corrMat(i1,i2);
}

RooPdfEvaluator::~RooPdfEvaluator(){
  if(this->params) delete this->params ;
  if(this->nset)   delete this->nset ;
  if(!this->_hesse) delete this->_hesse;
}

namespace {
  void getAxes(TH1* hist, const RooArgList& plotVars, int& xbins, RooRealVar*&xvar, TAxis*& xaxis, int& ybins, RooRealVar*&yvar, TAxis*& yaxis, int& zbins, RooRealVar*&zvar, TAxis*& zaxis){
    // Check that the number of plotVars matches the input histogram's dimension
    xbins=0;
    ybins=1;
    zbins=1;
    switch(hist->GetDimension()) {
    case 3:
      zbins= hist->GetNbinsZ();
      zvar = (RooRealVar*)(plotVars.at(2));
      zvar->setValueDirty();
      zaxis= hist->GetZaxis();
      assert(0 != zvar && 0 != zaxis);
      // [[fallthrough]];
      // fall through
    case 2:
      ybins= hist->GetNbinsY();
      yvar = (RooRealVar*)(plotVars.at(1));
      yvar->setValueDirty();
      yaxis= hist->GetYaxis();
      assert(0 != yvar && 0 != yaxis);
      // [[fallthrough]];
      // fall through
    case 1:
      xbins= hist->GetNbinsX();
      xvar = (RooRealVar*)(plotVars.at(0));
      xvar->setValueDirty();
      xaxis= hist->GetXaxis();
      assert(0 != xvar && 0 != xaxis);
      break;
    default:
      throw std::runtime_error("fillHistogram: wrong dimensionality");
      break;
    }

  }

  void setNextBin(TH1* hist, int bin, int& xbin, int xbins, RooRealVar*xvar, TAxis* xaxis, int& ybin, int ybins, RooRealVar*yvar, TAxis* yaxis, int& zbin, int /*zbins*/, RooRealVar*zvar, TAxis* zaxis){
    switch(hist->GetDimension()) {
    case 3:
      if(bin % (xbins*ybins) == 0) {
        zbin++;
        zvar->setVal(zaxis->GetBinCenter(zbin));
      }
      // [[fallthrough]];
      // fall through
    case 2:
      if(bin % xbins == 0) {
        ybin= (ybin%ybins) + 1;
        yvar->setVal(yaxis->GetBinCenter(ybin));
      }
      // [[fallthrough]];
      // fall through
    case 1:
      xbin= (xbin%xbins) + 1;
      xvar->setVal(xaxis->GetBinCenter(xbin));
      break;
    default:
      throw std::runtime_error("RooAbsReal::fillHistogram: Internal Error!");
      break;
    }
  }
}


//void RooPdfEvaluator::fillHistogram(TH1 *hist,const RooArgList& plotVars) const {
void RooPdfEvaluator::fillHistogram(TH1 *hist, const RooArgList& plotVars, ErrorCalculation mode) {
  // Prepare to loop over the histogram bins
  Int_t xbins,ybins,zbins;
  RooRealVar *xvar = NULL;
  RooRealVar *yvar = NULL;
  RooRealVar *zvar = NULL;
  TAxis *xaxis = NULL;
  TAxis *yaxis = NULL;
  TAxis *zaxis = NULL;
  getAxes(hist,plotVars,xbins,xvar,xaxis,ybins,yvar,yaxis,zbins,zvar,zaxis);
  Int_t bins= xbins*ybins*zbins;
  if(mode == SamplingErrors && _hesse){
    double Z = 1;
    int n = Int_t(100./TMath::Erfc(Z/sqrt(2.))) ;
    if (n<100) n=100 ;
    RooAbsCollection* origVals = this->params->snapshot();
    DEBUGclass("generating %d samples",n);
    RooDataSet* samples = _hesse->generate(*(this->params),n) ;
    std::vector<double> sum2(bins);
    DEBUGclass("evaluating %d samples",n);
    for (int i=0 ; i<samples->numEntries() ; ++i) {
      *(this->params) = (*samples->get(i));
      int xbin=0,ybin=0,zbin=0;
      for(Int_t bin= 0; bin < bins; bin++) {
        setNextBin(hist,bin,xbin,xbins,xvar,xaxis,ybin,ybins,yvar,yaxis,zbin,zbins,zvar,zaxis);
        this->setValueDirty();
        Double_t result = this->getVal();
        sum2[bin] += result*result;
      }
    }
    for(int i=0; i<this->params->getSize(); ++i){
      RooRealVar* v = (RooRealVar*)(this->params->at(i));
      v->setVal(((RooRealVar*)origVals->find(v->GetName()))->getVal());
    }
    delete samples;
    delete origVals;
    int xbin=0,ybin=0,zbin=0;
    for(Int_t bin= 0; bin < bins; bin++) {
      setNextBin(hist,bin,xbin,xbins,xvar,xaxis,ybin,ybins,yvar,yaxis,zbin,zbins,zvar,zaxis);
      this->setValueDirty();
      Double_t result = this->getVal();
      int binid = hist->GetBin(xbin,ybin,zbin);
      hist->SetBinContent(binid,result);
      hist->SetBinError(binid,sqrt(sum2[bin])/n);
    }
  } else {
    int xbin=0,ybin=0,zbin=0;
    for(Int_t bin= 0; bin < bins; bin++) {
      setNextBin(hist,bin,xbin,xbins,xvar,xaxis,ybin,ybins,yvar,yaxis,zbin,zbins,zvar,zaxis);
      this->setValueDirty();
      Double_t result = this->getVal();
      Double_t unc = (mode == NoErrors || result < 1e-6) ? 0. : this->getError(mode);
      int binid = hist->GetBin(xbin,ybin,zbin);
      hist->SetBinContent(binid,result);
      hist->SetBinError  (binid,unc);
    }
  }
}

double  RooPdfEvaluator::getValue() const {
  return this->getVal();
}

double RooPdfEvaluator::getError(ErrorCalculation mode) {
  // Calculate error on self by propagated errors on parameters with correlations as given by fit result
  // Mode "NoErrors":
  // return 0 always
  //
  // Mode "LinearErrors"
  // The linearly propagated error is calculated as follows
  //                                    T
  // error(x) = F_a(x) * Corr(a,a') F_a'(x)
  //
  // where     F_a(x) = [ f(x,a+da) - f(x,a-da) ] / 2, with f(x) this function and 'da' taken from the fit result
  //       Corr(a,a') = the correlation matrix from the fit result
  //
  // side effects: after application of this function, all parameters will be set to the fit result values
  //
  // Mode "SamplingErrors"
  // Throw toys to determine the uncertainty on the result

  switch(mode){
  case NoErrors: {
    return 0.;
    break;
  }
  case LinearErrors: {
    DEBUGclass("getting variations");
    const TVectorD F(this->getVariations());
    if(this->allEntriesZero(F)) return 0.;
    // Calculate error in linear approximation from variations and correlation coefficient
    DEBUGclass("multiplying with covariance matrix");
    Double_t sum = F*(this->corrMat*F) ;
    // nExpected already contained in sum, as getVariations uses getVal internally
    DEBUGclass("returning");
    return sqrt(sum) ;
    break;
  }
  case SamplingErrors: {
    if(!_hesse) return 0.;
     // Generate 100 random parameter points distributed according to fit result covariance matrix
    double Z = 1;
    int n = Int_t(100./TMath::Erfc(Z/sqrt(2.))) ;
    if (n<100) n=100 ;

    RooAbsCollection* origVals = this->params->snapshot();
    DEBUGclass("generating %d samples",n);
    RooDataSet* d = _hesse->generate(*(this->params),n) ;
    DEBUGclass("evaluating %d samples",n);
    double sum=0;
    double sum2=0;
    for (int i=0 ; i<d->numEntries() ; ++i) {
      *(this->params) = (*d->get(i));
      double val = this->getValue();
      sum+=val;
      sum2+=(val*val);
    }
    for(int i=0; i<this->params->getSize(); ++i){
      RooRealVar* v = (RooRealVar*)(this->params->at(i));
      v->setVal(((RooRealVar*)origVals->find(v->GetName()))->getVal());
    }
    DEBUGclass("returning");
    delete d;
    delete origVals;
    return sqrt(sum2/n);
    break;
  }
  }
  return 0.;
}

bool RooPdfEvaluator::allEntriesZero(const TVectorD& v) const {
  for(int i=0; i<v.GetNrows(); ++i){
    if(v[i]>this->threshold) return false;
  }
  return true;
}


void RooPdfEvaluator::setDirty() {
  if(!this->_func) return;
  if(!this->params){
    throw std::runtime_error("no parameters available!");
  }
  for (Int_t ivar=0 ; ivar<this->params->getSize() ; ++ivar) {
    RooRealVar* v = (RooRealVar*)(this->params->at(ivar));
    v->setValueDirty();
  }
  RooArgSet* cache = const_cast<RooArgSet*>(&(this->_cachedNodes));
  cache->removeAll();
  this->setValueDirty();
  this->_func->setValueDirty();
  this->_func->getVal();
}

TVectorD RooPdfEvaluator::getVariations() {
  // Make vector of variations
  if(!this->params){
    throw std::runtime_error("no parameters available!");
  }
  TVectorD F(this->params->getSize()) ;
  this->setValueDirty();
  Double_t nom = this->getVal();
  if(!TQUtils::isNum(nom)){
    throw std::runtime_error("unable to process non-numeric nominal value!");
  }
  for (Int_t ivar=0 ; ivar<this->params->getSize() ; ++ivar) {
    RooRealVar* v = (RooRealVar*)(this->params->at(ivar));
    Double_t cenVal = v->getVal() ;
    Double_t errVal = sqrt(this->covMat(ivar,ivar));
    double var = 0;
    if(TQUtils::isNum(errVal)){
      // Make Plus variation
      v->setVal(cenVal+errVal) ;
      this->setValueDirty();
      const double plusVar = this->getVal();
      // Make Minus variation
      v->setVal(cenVal-errVal) ;
      this->setValueDirty();
      const double minusVar = this->getVal();
      v->setVal(cenVal) ;
      DEBUG("  varying %s from %f by +/-%f: %f %f %f",v->GetName(),cenVal,errVal,nom,plusVar,minusVar);
      if(TQUtils::isNum(plusVar) && TQUtils::isNum(minusVar)){
        var = (plusVar-minusVar)/2 ;
      } else if(TQUtils::isNum(plusVar)){
        var = plusVar-nom;
      } else if (TQUtils::isNum(minusVar)){
        var = nom-minusVar;
      }
    }
    F[ivar] = var;
  }
  return F;
}



void TSHistogramExporter::Region::collectSelectionVars(const TString& nfPattern) {
  RooArgSet* allVars(this->getPdf()->getParameters((RooArgSet*)0));
  //allVars->Print("v");
  for(const auto& s:this->samples){
    TString sname(s.first);
    TString pattern(nfPattern);
    pattern.ReplaceAll("$(SAMPLENAME)",sname);
    RooAbsArg* arg = allVars->find(pattern);
    if(!arg) throw std::runtime_error(TString::Format("unable to find selection var '%s' for sample '%s'",pattern.Data(),sname.Data()).Data());
    RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
    if(!var) throw std::runtime_error(TString::Format("find selection var '%s' for sample '%s' is not of type RooRealVar!",pattern.Data(),sname.Data()).Data());
    this->selectionVars[sname] = var;
  }
  delete allVars;
}

TString TSHistogramExporter::Region::getSelectionVariableNames(const std::vector<TString>& keys) const {
  // in legacy mode, we expect there to be an "NF" parameter that we can use to turn on/off parts of the PDF
  TString compNames;
  bool first = true;
  for(const auto& p:keys){
    RooRealVar* v = selectionVars.at(p);
    if(!first){
      compNames.Append(",");
    }
    first = false;
    compNames.Append(v->GetName());
  }
  return compNames;
}

void TSHistogramExporter::Region::selectComponentsLegacy(const std::vector<TString>& keys) const {
  // in legacy mode, we expect there to be an "NF" parameter that we can use to turn on/off parts of the PDF
  for(const auto& v:selectionVars){
    v.second->setVal(0);
  }
  for(const auto& k:keys){
    RooRealVar* var = selectionVars.at(k);
    if(!var){
      throw std::runtime_error(TString::Format("unable to retrieve parameter for sample '%s'",k.Data()).Data());
    }
    var->setVal(1);
  }
}

TString TSHistogramExporter::Region::getComponentNames(const std::vector<TString>& keys) const {
  TString compNames;
  bool first = true;
  for(const auto& p:keys){
    for(const auto& s:samples.at(p)){
      if(!first){
        compNames.Append(",");
      }
      first = false;
      compNames.Append(s->GetName());
    }
  }
  return compNames;
}

RooArgSet* TSHistogramExporter::selectBranchNodes(RooAbsReal* func, const TString& compNames){
 // Get complete set of tree branch nodes
  RooArgSet branchNodeSet ;
  func->branchNodeServerList(&branchNodeSet) ;

  // Discard any non-RooAbsReal nodes
  TIterator* iter = branchNodeSet.createIterator() ;
  RooAbsArg* arg ;
  while((arg=(RooAbsArg*)iter->Next())) {
    if (!dynamic_cast<RooAbsReal*>(arg)) {
      branchNodeSet.remove(*arg) ;
    }
  }
  delete iter ;

  return (RooArgSet*)(branchNodeSet.selectByName(compNames));
}

void TSHistogramExporter::Region::selectComponents(const std::vector<TString>& keys) const {
  TString compNames = this->getComponentNames(keys);
  TSHistogramExporter::selectComponents(this->func,compNames);

}

void TSHistogramExporter::selectComponents(RooAbsReal* func, const TString& compNames) {
  // in the component selection mode, we use RooFits internal component selection mechanism to select parts of the PDF
  RooArgSet* selected = TSHistogramExporter::selectBranchNodes(func,compNames);
  plotOnCompSelect(func,selected);
  delete selected;
}

void TSHistogramExporter::Region::unselectComponentsLegacy() const {
  for(const auto& v:selectionVars){
    v.second->setVal(1);
  }
}
void TSHistogramExporter::Region::unselectComponents() const {
  TSHistogramExporter::unselectComponents(func);
}

void TSHistogramExporter::unselectComponents(RooAbsReal* func) {
  plotOnCompSelect(func,0) ;
}


RooAbsReal* TSHistogramExporter::Region::createIntegral(const TString& nameAppend, const RooArgSet& normSet, const RooArgSet& projectedVars) const {
  TString resultname = this->getName()+nameAppend;
  this->func->getVal(normSet);
  TString nEventsName = this->getName()+"_nEvents";
  RooExpectedEvents* nEvents = new RooExpectedEvents(nEventsName,nEventsName,*(this->func),normSet);
  TString intname=TString::Format("%s_Int",this->func->GetName());
  RooArgList terms;
  RooRealIntegral* integral = new RooRealIntegral(intname,intname,*(this->func),projectedVars,&normSet);
#ifdef HAS_GENERIC_LAMBDAS
  ::SFINAE::trySetAllowComponentSelection(integral,true);
#endif
  terms.add(*integral);
  terms.add(*nEvents);
  RooProduct* prod = new RooProduct(resultname.Data(),resultname.Data(),terms);
  return prod;
}

RooAbsReal* TSHistogramExporter::Region::createIntegral() const {
  TString resultname = this->getName()+"_yield";
  this->func->getVal(this->getObservables());
  TString nEventsName = this->getName()+"_nEvents";
  RooExpectedEvents* nEvents = new RooExpectedEvents(nEventsName,nEventsName,*(this->func),this->getObservables());
  TString intname=TString::Format("%s_Int",this->func->GetName());
  RooArgList terms;
  RooRealIntegral* integral = new RooRealIntegral(intname,intname,*(this->func),this->getObservables(),&(this->getObservables()));
#ifdef HAS_GENERIC_LAMBDAS
  ::SFINAE::trySetAllowComponentSelection(integral,true);
#endif
  terms.add(*integral);
  terms.add(*nEvents);
  RooProduct* prod = new RooProduct(resultname.Data(),resultname.Data(),terms);
  return prod;
}

bool TSHistogramExporter::getMC(TDirectory* dir, Region* region, std::map< const TString, std::vector<TString> > samples, int calculateErrors, double checkThreshold){
  // obtain the MC histograms
  DEBUGclass("initializing %s with calculate errors %d",region->getName().Data(), calculateErrors);
  #ifdef _DEBUG_
  region->print();
  #endif

  bool legacyMode = !nfPattern.IsNull();
  if(legacyMode){
    region->collectSelectionVars(nfPattern);
    region->unselectComponentsLegacy();
#ifdef HAS_GENERIC_LAMBDAS
  } else if(::SFINAE::hasCompSelectSwitch<RooRealIntegral>::value){
    region->unselectComponents();
#endif
  } else {
    throw std::runtime_error("RooRealIntegral does not have a setAllowComponentSelection member function, cannot use CompSelect mechanism - please use legacy mechanism suppling an NF pattern!");
  }

  // get the number of events
  double nExp = region->expectedEvents();
  if(nExp < 0){
    throw std::runtime_error(TString::Format("number of expected events in region %s is %g < 0!",region->getName().Data(),nExp).Data());
  }


  // sort out the observables
  RooArgSet observables(region->getObservables());
  RooArgSet projectedVars;
  RooArgList x(region->getObservables());

  // create helper objects
  RooAbsReal *projected = NULL;
  RooArgSet normSet;
  if(!legacyMode) normSet.add(observables);
  if(!region->isCombined()){
    projected               = region->createIntegral("_projected",normSet,projectedVars);
  }
  RooAbsReal* integral = region->createIntegral();

  double epsilon = 1e-12;
  RooPdfEvaluator intReader (integral, fitResult,epsilon,checkThreshold);
  RooPdfEvaluator histReader(projected,fitResult,epsilon,checkThreshold);

  std::vector<TString> decorrelatePars = this->getTagVString("decorrelate");
  for(const auto& p1:decorrelatePars){
    for(const auto& p2:decorrelatePars){
      intReader .setCorrelation(p1,p2,0);
      histReader.setCorrelation(p1,p2,0);
    }
  }
  std::vector<TString> correlatePars = this->getTagVString("correlate");
  for(const auto& p1:correlatePars){
    for(const auto& p2:correlatePars){
      intReader .setCorrelation(p1,p2,1);
      histReader.setCorrelation(p1,p2,1);
    }
  }

  // loop over the regions
  double sumEvents = 0;
  std::vector<TH1*> histograms;
  for(const auto& sample:samples){
    RooLinkedList args;

    DEBUGclass(" evaluating sample %s",sample.first.Data());

    /////////////////////////////////////
    // obtain the results from the Pdf
    /////////////////////////////////////
    TH1* hist = 0;
    if(!region->isCombined()){
      DEBUGclass("   creating histogram");
      hist = region->makeHistogram(sample.first,x);
      if (!hist){
        std::cout << "  WARNING: unable to create histogram " << sample.first << std::endl;
      } else {
        hist->SetDirectory(dir);
        //checkConsistency(hist,x);
      }
    }

    DEBUGclass("   selecting components");
    if(!legacyMode){
      region->selectComponents      (sample.second);
    } else {
      region->selectComponentsLegacy(sample.second);
    }

    intReader.setDirty();
    DEBUGclass("   evaluating integral");
    double yieldVal = intReader.getVal();
    DEBUGclass("   evaluating integral uncertainty");

    double yieldErr = intReader.getError((RooPdfEvaluator::ErrorCalculation)calculateErrors);
    if(!TQUtils::isNum(yieldErr)){
      throw std::runtime_error(TString::Format("encountered non-numeric error in %s using fitResult '%s'",integral->GetName(),fitResult->GetName()).Data());
    }

    histReader.setDirty();
    if(hist){
      DEBUGclass("   filling histogram");
      histReader.fillHistogram(hist,x, (RooPdfEvaluator::ErrorCalculation)calculateErrors);
    }

    if(!legacyMode){
      region->unselectComponents();
    } else {
      region->unselectComponentsLegacy();
    }

    /////////////////////////////////////
    // sanitize the histograms
    /////////////////////////////////////

    DEBUGclass("   finalizing");

    if(hist){
      histograms.push_back(hist);
      hist->SetName(sample.first);
      hist->SetTitle(sample.first);

      // apply the bin width correction
      double nevents_hist = hist->Integral();
      if(nevents_hist>0){
        double density_hist = hist->Integral("WIDTH");
        double scaling = density_hist/nevents_hist;
        hist->Scale(scaling);
      } else {
        hist->Reset();
      }

      double epsilon_total = epsilon * hist->GetNbinsX() * samples.size();
      if(hist->Integral() <= epsilon_total && yieldVal <= epsilon_total){
        // check if we have any events
        hist->Reset();
        yieldVal = 0;
        yieldErr = 0;
      } else if(!TMath::AreEqualRel(hist->Integral(),yieldVal,1E-04)){
        // check if the yield and differential histograms agree
        std::cout << TString::Format("  WARNING: integral of histogram '%s' is %g and does not match total yield %g, correcting total yield and error neglecting bin-to-bin correlations!",hist->GetName(),hist->Integral(),yieldVal) << std::endl;
        yieldVal = TQHistogramUtils::getIntegralAndError(hist,yieldErr);
      } else {
        // if yield and differential agree in integral and the histogram has only one bin, we can run a few additional checks
        if(hist->GetNbinsX() == 1){
          double binErr = hist->GetBinError(1);
          if(binErr == 0){
//            std::cout << TString::Format("  WARNING: single-bin histogram '%s' does not have error, recycling %.1f%% uncertainty from total yield!",hist->GetName(),100*yieldErr/yieldVal) << std::endl;
//            hist->SetBinError(1,yieldErr);
            double err = sqrt(yieldVal);
            std::cout << TString::Format("  WARNING: single-bin histogram '%s' does not have error, using sqrt(n)=%.1f%% uncertainty!",hist->GetName(),100*err/yieldVal) << std::endl;
            hist->SetBinError(1,err);
            binErr = err;
          }
          if(!TMath::AreEqualRel(binErr/yieldVal,yieldErr/yieldVal,1E-06)){
            std::cout << TString::Format("  WARNING: in single-bin histogram '%s', uncertainties on bin and integral do not match: d(bin)=%g but d(integral)=%g - using single bin uncertainty!",hist->GetName(),binErr/yieldVal,yieldErr/yieldVal) << std::endl;
            yieldErr = binErr;
          } else {
            //            std::cout << TString::Format("  INFO: in single-bin histogram '%s', uncertainties on bin and integral: d(bin)=%g and d(integral)=%g!",hist->GetName(),binErr/yieldVal,yieldErr/yieldVal) << std::endl;
          }
        }
      }

      // check if any bin contents come out <=0 and print a warning if that happens
      for(int i=1; i<TQHistogramUtils::getNbinsGlobal(hist)-1; ++i){
        if(hist->IsBinOverflow(i) || hist->IsBinUnderflow(i)) continue;
        if(hist->GetBinContent(i) <= 0 ){
          std::cout << TString::Format("  WARNING: histogram '%s' has null/negative bin contents in bin %d",sample.first.Data(),i) << std::endl;
        }
        if((calculateErrors>0) && hist->GetBinError(i) < 1e-12){
          std::cout << TString::Format("  WARNING: histogram '%s' has close-to-zero uncertainty in bin %d",sample.first.Data(),i) << std::endl;
        }
      }
    }

    DEBUGclass("   saving integral");

    // create a single-bin histogram to save the yield
    TH1* yield = new TH1F(sample.first+"_count",sample.first,1,0,1);
    yield->SetDirectory(dir);
    yield->SetBinContent(1,yieldVal);
    yield->SetBinError  (1,yieldErr);
    histograms.push_back(yield);

    // we only add the samples to the sumEvents that are indeed a single sample to avoid double counting
    if(sample.second.size() == 1){
      sumEvents += yieldVal;
    }

    // look if there's anything in the model that we can use to style the histogram
    if(styleModel){
      TH1* otherHist = dynamic_cast<TH1*>(styleModel->getObject(sample.first,region->getName()));
      if(otherHist){
        TQHistogramUtils::copyStyle(yield,otherHist);
        yield->SetTitle(otherHist->GetTitle());
        if(hist){
          TQHistogramUtils::copyStyle(hist,otherHist);
          hist->SetTitle(otherHist->GetTitle());
          hist->GetXaxis()->SetTitle(otherHist->GetXaxis()->GetTitle());
        }
      }
    }
  }

  intReader.setDirty();
  histReader.setDirty();

  if(sumEvents <= 0){
    throw std::runtime_error("sum over all samples in this region is <= 0!");
  }

  // in order to avoid normalization problems, we normalize the sum of integrals to the number of expected events in the region
  // if everything worked out fine, this shouldn't do anything, but unfortunately, this is sometimes needed
 // double scale = nExp/sumEvents;
  //if(!TMath::AreEqualRel(scale, 1., 1E-06)){
   // std::cout << TString::Format("WARNING: correcting scaling of histograms in region '%s' by a factor of %.16f",region->getName().Data(),scale) << std::endl;
    //for(auto hist:histograms){
     // hist->Scale(scale);
     // hist->SetEntries(1);
   // }
  //}

  // add everything to the output file
  for(auto hist:histograms){
    hist->SetDirectory(dir);
  }

  delete integral;
  delete projected;

  return true;
}

int TSHistogramExporter::getData(TDirectory* dir, const TString& selector, const TString& histname, const Region* region) {
  TH1* data = NULL;
  TH1* data_count = new TH1F(histname+"_count",histname,1,0,1);
  data_count->SetDirectory(dir);
  int ndata = 0;
  RooArgList x(region->getObservables());
  std::vector<TString> varnames;
  TSUtils::getParameterNames(&x,varnames);

  // loop over datasets
  std::vector<TString> dataNames(region->allData());
  for(const auto& dsname:dataNames){
    if(!TQStringUtils::matches(dsname,selector)) continue;
    int n = region->nData(dsname);
    RooDataSet* dataset = NULL;
    for(int i=0; i<n; ++i){
      dataset = region->getData(dsname,i);
      if(!dataset) throw std::runtime_error("error retrieving dataset!");
      double entries = dataset->sumEntries();
      data_count->AddBinContent(1,entries);
    }
    ndata++;
    if(region->isCombined() || n!=1) continue;
    TH1* thisdata = dataset->createHistogram(TQStringUtils::concat(varnames));
   if(!thisdata) continue;
    //checkConsistency(thisdata,x);
    if(data){
      data->Add(thisdata);
      thisdata->SetDirectory(NULL);
      delete thisdata;
    } else {
      data = thisdata;
      data->SetDirectory(dir);
      data->SetName(histname);
      data->Sumw2(false);
      if(styleModel){
        TH1* otherHist = dynamic_cast<TH1*>(styleModel->getObject("Data",dataNames[0]));
        if(otherHist){
          TQHistogramUtils::copyStyle(data,otherHist);
          data->SetTitle(otherHist->GetTitle());
          data->GetXaxis()->SetTitle(otherHist->GetXaxis()->GetTitle());
        }
      } else {
        data->SetTitle(histname);
      }
      ndata = 1;
    }
  }
  if(ndata>1){
    if(data){
      data->Scale(1./ndata);
      TQHistogramUtils::applyPoissonErrors(data);
    }
    data_count->Scale(1./ndata);
    TQHistogramUtils::applyPoissonErrors(data_count);
  }
  if(data){
    data->SetEntries(ndata);
  }
  data_count->SetEntries(ndata);
  return ndata;
}

RooCategory* TSHistogramExporter::getCategories(RooSimultaneous* simPdf){
  if(!simPdf) return NULL;
  return const_cast<RooCategory*>(dynamic_cast<const RooCategory*>(&(simPdf->indexCat())));
}

std::vector<TSHistogramExporter::Region*> TSHistogramExporter::makeRegions(RooSimultaneous* pdf, const RooArgSet* observables, const std::list<RooAbsData*>& datasets){
  RooCategory* channelCat = TSHistogramExporter::getCategories(pdf);
  std::map<const TString,TList*> allSplitData;
  for(auto ds:datasets){
    TList* dataList = ds->split(*(channelCat), true);
    if(!dataList){
      throw std::runtime_error(TString::Format("unable to split dataset '%s' at '%s'",ds->GetName(),channelCat->GetName()).Data());
    }
    allSplitData[ds->GetName()] = dataList;
  }
  std::vector<TSHistogramExporter::Region*> regions;
  std::map<TString,RooRealSumPdf*> cats = TSHistogramExporter::getComponents(pdf);
  for(const auto& cat:cats){
    RooArgSet* obs = cat.second->getObservables(*observables);
    TSHistogramExporter::Region* region = new TSHistogramExporter::Region(cat.first,pdf,cat.second,obs,false);
    if(region->isCombined()) throw std::runtime_error("single region is combined!");
    for(auto ds:datasets){
      RooDataSet* data = dynamic_cast<RooDataSet*>(allSplitData[ds->GetName()]->FindObject(cat.first));
      region->addData(ds->GetName(),data);
    }
    regions.push_back(region);
  }
  for(auto it:allSplitData){
    it.second->SetOwner(false);
    delete it.second;
  }
  return regions;
}

std::map<TString,RooRealSumPdf*> TSHistogramExporter::getComponents(RooSimultaneous* pdf){
  std::map<TString,RooRealSumPdf*> cats;
  RooCategory* channelCat = getCategories(pdf);
  TIterator* iter = channelCat->typeIterator() ;
  RooCatType* tt = NULL;
  while((tt=(RooCatType*) iter->Next())) {
    TString catname(tt->GetName());
    RooProdPdf* prodpdf = dynamic_cast<RooProdPdf*>(pdf->getPdf(catname));
    if(!prodpdf){
      throw std::runtime_error(TString::Format("unable to find obtain prodPdf of region '%s'",catname.Data()).Data());
    }
    RooRealSumPdf* sumpdf = NULL;
    RooArgSet thisComponents;
    ::findUniqueProdComponents(prodpdf, thisComponents, "*");
    for (RooLinkedListIter compItr = thisComponents.iterator(); RooAbsPdf* comp = dynamic_cast<RooAbsPdf*>(compItr.Next());) {
      if (comp->IsA() == RooRealSumPdf::Class()) {
        sumpdf = dynamic_cast<RooRealSumPdf*>(comp);
        break;
      }
    }
    if(!sumpdf){
      throw std::runtime_error(TString::Format("unable to find sumPdf of region '%s'",catname.Data()).Data());
    }
    cats[catname] = sumpdf;
  }
  return cats;
}

int TSHistogramExporter::addCombinedRegions(TCollection* input, std::vector<TSHistogramExporter::Region*>& regions, const std::vector<TSHistogramExporter::Region*>& origregions){
  TQFolderIterator ritr(input,true);
  int n = 0;
  while(ritr.hasNext()){
    TQFolder* combination = ritr.readNext();
    TString name(combination->GetName());
    std::vector<TString> selection = combination->getTagVString("Components");
    RooArgSet allObs;
    RooArgList funcs;
    RooArgList coefs;
    RooConstVar* unity = new RooConstVar("1","1",1);
    std::vector<Region*> selectedRegions;
    for(const auto& r:origregions){
      bool selected = false;
      for(const auto& n:selection){
        if(TQStringUtils::matches(r->getName(),n)){
          selected = true;
        }
      }
      if(!selected) continue;
      selectedRegions.push_back(r);
    }
    for(const auto r:selectedRegions){
      allObs.add(r->getObservables());
      funcs.add(*r->getPdf());
      coefs.add(*unity);
    }
    RooAbsPdf * pdf= new RooRealSumPdf(name,"",funcs,coefs);
    Region* result = new Region(name,pdf,&allObs,true);
    if(!result->isCombined()) throw std::runtime_error("combined region is not combined!");
    for(const auto r:selectedRegions){
      result->addSamples(r);
      for(const auto& d:r->allData()){
        for(int i=0; i<r->nData(d); ++i){
          result->addData(d,r->getData(d,i));
        }
      }
    }
    regions.push_back(result);
    n++;
  }
  return n;
}

int TSHistogramExporter::addCombinedSamples(TCollection* input,std::map< const TString, std::vector<TString> >& samples, const std::vector<TString>& allSamples){
  TQFolderIterator sitr(input,true);
  int n = 0;
  while(sitr.hasNext()){
    TQFolder* combination = sitr.readNext();
    std::vector<TString> keys = combination->getTagVString("Components");
    std::vector<TString> blacklist = combination->getTagVString("Blacklist");
    std::vector<TString> samplelist;
    for(const auto& s:allSamples){
      bool add = false;
      for(const auto& k:keys){
        if(TQStringUtils::matches(s,k)){
          add = true;
        }
      }
      for(const auto& k:blacklist){
        if(TQStringUtils::matches(s,k)){
          add = false;
        }
      }
      if(add){
        samplelist.push_back(s);
      }
    }
    if(samplelist.size() > 0){
      samples[combination->GetName()] = samplelist;
      n++;
    } else {
      std::cout << "ERROR: expression for sample '" << combination->GetName() << "' does not match any!" << std::endl;
    }
  }
  return n;
}
