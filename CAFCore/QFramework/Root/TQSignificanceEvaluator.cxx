#include "QFramework/TQSignificanceEvaluator.h"
#include "QFramework/TQHistogramUtils.h"
#include <iostream>
#include <functional>
#include <math.h>
#include "QFramework/TQGridScanner.h"
#include "QFramework/TQSampleFolder.h"
#include "QFramework/TQSampleDataReader.h"
#include "QFramework/TQLibrary.h"
#include "QFramework/TQHistogramUtils.h"

#include "THnBase.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQSignificanceEvaluator
//
// The TQSignificaneEvaluator is an abstract class that provides an interface
// for significance evaluators, that is, classes which provide functionality
// to calculate a significance for the purpose of optimization studies
// currently (Apr/May 2013) these are primarily aimed at supporting the mechanism
// provided by the TQGridScanner -- but there is really nothing that should stop you from
// changing these classes to support other optimization techniques
//
// the interface is kept as general as possible to provide maximum flexibility
// thus, please read the documentation to the class members carefully before
// deciding on how to proceed implementing compatibility for different
// optimization techniques
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include "QFramework/TQGridScanObservable.h"
using BinExtrema = TQGridScanBound::BinExtrema;

using std::vector;
using std::pair;

namespace {

  auto simpleSignif = [](double b, double s){ return s/sqrt(b); };
  auto simpleSignif2 = [](double b, double s){ return s*s/b; };
  auto simple2Signif = [](double b, double s){ return s/sqrt(s+b); };
  auto simple2Signif2 = [](double b, double s){ return s*s/(s+b); };
  auto simple3Signif = [](double b, double s){ return s/b; };
  auto simple3Signif2 = [](double b, double s){ return s*s/(b*b); };

  std::unique_ptr<TH1F> projection(THnBase* hist) {
    // This is probably the most time consuming step for the simple significance evaluators
    // since here the specified ranges are applied and then one dimension is projected out!
    return std::unique_ptr<TH1F>(reinterpret_cast<TH1F*>(hist->Projection(0)));
  }

  // Ranges set by the user have already been accounted for in the projection
  double count(THnBase* hist) {
    auto proj = projection(hist);
    return proj->Integral(BinExtrema::min, BinExtrema::max);
  }

  pair<double, double> countAndError(THnBase* hist) {
    auto proj = projection(hist);
    double error = 0.0;
    auto count = proj->IntegralAndError(BinExtrema::min, BinExtrema::max, error);
    return std::make_pair(count, error);
  }

}

ClassImp(TQSignificanceEvaluator)

TQSignificanceEvaluator::TQSignificanceEvaluator(const TString& name) :
  TNamed(name,name),
  initialization(NULL),
  reader(NULL),
  luminosity(0),
  luminosityScale(1),
  fileIdentifier(name),
  verbose(false)
{}

void TQSignificanceEvaluator::setFileIdentifier(TString s){
  // set a file identifider
  this->fileIdentifier = s;
}

TQSampleDataReader* TQSignificanceEvaluator::getReader(){
  // retrieve TQSampleDataReader object
  return this->reader;
}

bool TQSignificanceEvaluator::setRangeAxis(int /*axis*/, double /*low*/, double /*up*/) {
  return true;
}

bool TQSignificanceEvaluator::updateHists(vector<int> /*axesToScan*/, TQGridScanner* /*scanner*/, int /*axisToEvaluate*/) {
  return true;
}

void TQSignificanceEvaluator::setVerbose(bool v){
  // toggle verbosity
  this->verbose = v;
}

void TQSignificanceEvaluator::bookNF(const TString& path){
  // book an NF
  this->autoNFs.push_back(path);
}

void TQSignificanceEvaluator::addRegion(const TString& cutname){
  // add a region (cut)
  m_regions.push_back(cutname);
}

bool TQSignificanceEvaluator::hasNativeRegionSetHandling(){
  // returns true if this evaluator type supports region set handling
  return false;
}
bool TQSignificanceEvaluator::prepareNextRegionSet(const TString& /*suffix*/){
  // prepare the next region set
  WARNclass("prepareRegion was called - this evaluator type does not support native region handling. Something went wrong!");
  return false;
}
bool TQSignificanceEvaluator::isPrepared(){
  // returns true if the evaluator is prepared, false otherwise
  return false;
}

TQSignificanceEvaluator::TQSignificanceEvaluator(const TString& name, TQSampleFolder* sf) :
  TNamed(name,name),
  initialization(NULL),
  reader(sf ? new TQSampleDataReader(sf) : NULL),
  luminosity(0),
  luminosityScale(1),
  fileIdentifier(name)
{
  // In this constructor variant, the significance evaluator is created
  // with a pointer to a TQSampleFolder which can be used
  // to retrieve the data necessary for significance evaluatioin
  // the mechanisms of retrievial and calculation are entirely up to the derived class
  this->getLuminosity();
}

double TQSignificanceEvaluator::getLuminosity(TString folderName, TString tagName){
  // retrieve the luminosity value from the given location (default info/luminosity)
  // within the sample folder structure (and save it internally)
  if(this->reader && this->reader->getSampleFolder() && this->reader->getSampleFolder()->getFolder(folderName))
    this->reader->getSampleFolder()->getFolder(folderName)->getTagDouble(tagName,this->luminosity);
  return this->luminosity;
}

bool TQSignificanceEvaluator::scaleLuminosity(double lumi){
  // set the luminosity scale to an arbitrary value -- this will affect the significnce!
  // this will only have an effect if TQSignificanceEvaluator::getLuminosity was called previously!
  if(this->luminosity <= 0)
    this->getLuminosity();
  if(this->luminosity > 0){
    this->luminosityScale = lumi/(this->luminosity);
    return true;
  }
  return false;
}

std::vector<double> TQSignificanceEvaluator::getFOMs(size_t iregion) {
  // This function will calculate the FOMs based on the configured formulas in m_functionsFOM and m_functionsFOMWithBkgUnc
  // If vector is empty an empty vector is returned
  if(this->initialization == TQGridScanner::Class()){
    std::vector<double> fom_vector;
    if (m_functionsFOM.size() == 0 && m_functionsFOMWithUnc.size() == 0 && m_functionsFOMWithBkgUnc.size() == 0) {
      WARNclass("Figure of merits cannot be returned because no functions are specified!");
      return {};
    }

     // if only FOMs are specified where no stat. uncertainties are used simply
    // get the yields with signal() and bkg() function.
    // (This will probably be the most often usecase with estimates like s/sqrt(b)
    // or the poisson significance.)
    // If at least one FOM where stat. uncertainties are regarded is
    // configured, jump to other if clause and directly calculate signal and bkg yield
    // with errors (signalAndError() and bkgAndError() functions).
    // This avoids having to project out the multidimensional histogram multiple times
    // which is the most time consuming bit in the gridscanner feature
    double s, b, serr=0., berr=0.;
    if (m_functionsFOMWithUnc.size() == 0 and m_functionsFOMWithBkgUnc.size() == 0) {
      s = signal(iregion); // signal yield
      b = bkg(iregion); // background yiel
    } else { // if also FOMs are specified where stat. uncertainties are used
      auto sAndErr = signalAndError(iregion); //  pair of signal yield and uncertainty
      auto bAndErr = bkgAndError(iregion); //  pair of bkg yield and uncertainty
      s = sAndErr.first;
      serr = sAndErr.second;
      b = bAndErr.first;
      berr = bAndErr.second;
    }

    // Check if we are in a low stat region
    if(b < getTagDoubleDefault("cutoff",0)) {
      this->info += m_regions[iregion] + " skipped: low stats ; ";
      return {};
    }

    // evaluate all FOMs
    for (auto f : m_functionsFOM) {
      auto fom = f(b, s);
      fom_vector.push_back(fom);
    }
    for (auto f : m_functionsFOMWithUnc) {
      auto fom = f(b, s, berr, serr);
      fom_vector.push_back(fom);
    }
    for (auto f : m_functionsFOMWithBkgUnc) {
      auto fom = f(b, berr, s);
      fom_vector.push_back(fom);
    }

    if (not this->scanner->hasSplitObs) {// if Gridscanner has split obs don't write out yields and write out significance later
      this->info += TString::Format("%s: s=%g, b=%g, FOM(%s)=%g, ", m_regions[iregion].Data(), s, b, m_FOMDefinitions[0].Data(), fom_vector[0]);
      for (unsigned int i = 1; i < fom_vector.size(); i++) {
        this->info += TString::Format("FOM(%s)=%g", m_FOMDefinitions[i].Data(), fom_vector[i]);
        if (i == fom_vector.size()-1) {this->info += "; ";}
        else {this->info += ", ";}
      }
    }
    return fom_vector;
  }
  return {};
}

bool TQSignificanceEvaluator::addFunctionsFOM(std::vector<TString> fomDefinitions, std::vector<double>* parameter) {

  TString definitionString = "";
  for (auto d : fomDefinitions) definitionString += d+", ";
  definitionString.Chop(); definitionString.Chop();
  INFO("Add figure of merits from expression: '%s'", definitionString.Data());
  // ... can add any function here

  // std::vector<TString> definitions = TQStringUtils::split(fomDefinitions,",");
  unsigned i = 0;
  for (auto def : fomDefinitions) {
    TQStringUtils::removeLeading(def," ");
    TQStringUtils::removeTrailing(def," ");
    def.ToLower(); // to lower cases
    if (def.EqualTo("poisson")) {
      m_functionsFOM.push_back(TQHistogramUtils::getPoisson);
      m_FOMDefinitions.push_back("poisson");
    }
    else if (def.EqualTo("s/sqrt(b)")) {
      m_functionsFOM.push_back(simpleSignif);
      m_FOMDefinitions.push_back("s/sqrt(b)");
    }
    else if (def.EqualTo("s*s/b")) {
      m_functionsFOM.push_back(simpleSignif2);
      m_FOMDefinitions.push_back("s*s/b");
    }
    else if (def.EqualTo("s/sqrt(s+b)")) {
      m_functionsFOM.push_back(simple2Signif);
      m_FOMDefinitions.push_back("s/sqrt(s+b)");
    }
    else if (def.EqualTo("s*s/(s+b)")) {
      m_functionsFOM.push_back( simple2Signif2);
      m_FOMDefinitions.push_back("s*s/(s+b)");
    }
    else if (def.EqualTo("s/b")) {
      m_functionsFOM.push_back(simple3Signif);
      m_FOMDefinitions.push_back("s/b");
    }
    else if (def.EqualTo("s*s/(b*b)")) {
      m_functionsFOM.push_back(simple3Signif2);
      m_FOMDefinitions.push_back("s*s/(b*b)");
    }
    else if (def.EqualTo("s/const")) {
      double par = 0;
      if (!parameter || parameter->size()-1 < i ) {
        WARNclass("The following definition of a figure of merit is used but no parameter is specified: '%s'! Using a value of '0' for 'const'!", def.Data());
      } else {
        par = parameter->at(i);
      }
      auto signif = [par](double /*b*/, double s){ return s/par; };
      m_functionsFOM.push_back(signif);
      m_FOMDefinitions.push_back("s/const");
    }
    else if (def.EqualTo("s/sqrt(s+b+const)")) {
      double par = 0;
      if (!parameter || parameter->size()-1 < i ) {
        WARNclass("The following definition of a figure of merit is used but no parameter is specified: '%s'! Using a value of '0' for 'const'!", def.Data());
      } else {
        par = parameter->at(i);
      }
      auto signif = [par](double b, double s){ return s/sqrt(s+b+par); };
      m_functionsFOM.push_back(signif);
      m_FOMDefinitions.push_back("s/sqrt(s+b+const)");
    }
    else if (def.EqualTo("s/sqrt(b+berr2)")) {
      auto signif = [](double b, double s, double berr, double /*serr*/){ return s/sqrt(b+berr*berr); };
      m_functionsFOMWithUnc.push_back(signif);
      m_FOMDefinitions.push_back("s/sqrt(b+berr2)");
    }
    else if (def.EqualTo("s/sqrt(b+berr2+serr2)")) {
      auto signif = [](double b, double s, double berr, double serr){ return s/sqrt(b+berr*berr+serr*serr); };
      m_functionsFOMWithUnc.push_back(signif);
      m_FOMDefinitions.push_back("s/sqrt(b+berr2+serr2)");
    }
    else if (def.EqualTo("s/sqrt(s+b+berr2)")) {
      auto signif = [](double b, double s, double berr, double /*serr = 0*/){ return s/sqrt(s+b+berr*berr); };
      m_functionsFOMWithUnc.push_back(signif);
      m_FOMDefinitions.push_back("s/sqrt(s+b+berr2)");
    }
    else if (def.EqualTo("s/sqrt(s+b+serr2+berr2)")) {
      auto signif = [](double b, double s, double berr, double serr){ return s/sqrt(s+b+berr*berr+serr*serr); };
      m_functionsFOMWithUnc.push_back(signif);
      m_FOMDefinitions.push_back("s/sqrt(s+b+serr2+berr2)");
    }
    else if (def.EqualTo("poissonwerr")) {
      m_functionsFOMWithBkgUnc.push_back(TQHistogramUtils::getPoissonWithError);
      m_FOMDefinitions.push_back("poissonwerr");
    }
    else if (def.EqualTo("poissonwerrandrelerr")) {
      double par = 0;
      if (!parameter || parameter->size()-1 < i ) {
        WARNclass("The following definition of a figure of merit is used but no parameter is specified: '%s'! Using a value of '0' for 'relative bkg syst err'!", def.Data());
      } else {
        par = parameter->at(i);
      }
      auto signif = [par](double b, double berr, double s){ 
        double bSystErr = par * b;
        double bTotErr = TMath::Sqrt( TMath::Power(bSystErr, 2) + TMath::Power(berr, 2) );
        return TQHistogramUtils::getPoissonWithError(b, bTotErr, s);
      };
      m_FOMDefinitions.push_back("poissonwerrandrelerr");
      m_functionsFOMWithBkgUnc.push_back(signif);
    }
    else {
      WARNclass("The following definition of a figure of merit is not known: '%s'! Feel free to implement it in TQSignificanceEvaluator::addFunctionsFOM()! For now, this is just ignored and not used...", def.Data());
    }
    i++;
  }
  if  (m_functionsFOM.size() == 0 && m_functionsFOMWithUnc.size() == 0 && m_functionsFOMWithBkgUnc.size() == 0) {
    ERRORclass("No FOM function could be added, please check your configurations!");
    throw std::runtime_error("No FOM function could be added, exiting!");
  }
  m_multipleFOMsCompatible = true;
  return true;
}

double TQSignificanceEvaluator::signal(size_t iregion) {
  return count(m_signalHists[iregion]);
}
pair<double, double> TQSignificanceEvaluator::signalAndError(size_t iregion) {
  return countAndError(m_signalHists[iregion]);
}
double TQSignificanceEvaluator::bkg(size_t iregion) {
  return count(m_bkgHists[iregion]);
}
pair<double, double> TQSignificanceEvaluator::bkgAndError(size_t iregion) {
  return countAndError(m_bkgHists[iregion]);
}


ClassImp(TQSignificanceEvaluatorBase)

TQSignificanceEvaluatorBase::TQSignificanceEvaluatorBase(
    TQSampleFolder* sf,
    TString signal,
    TString background,
    TString name
) :
  TQSignificanceEvaluator(name, sf),
  signalPath(signal),
  backgroundPath(background)
{}

double TQSignificanceEvaluatorBase::evaluate(){
  // in the evaluation method, we will ask for different types of initialzation
  // and perform the corresponding steps needed to return a value
  if(this->initialization == TQGridScanner::Class()){
    // if the initialization was done with a GridScanner
    // we have pointers to grids containing the information we need
    double significance = 0;
    // TMatrix<double>* m_NF = this->calculateNFs();
    this->info = "";
    for(size_t i=0; i<m_regions.size(); i++){
      significance += getSignificance2(i);
    }
    if (m_regions.size() > 1) {
      this->info += TString::Format("FOM(combined)=%g ; ", sqrt(significance));
    }
    return sqrt(significance);
  }
  // if we didn't find a valid initialization method
  // we return zero and issue an error message
  ERRORclass("cannot evaluate without valid initialization!");
  return 0;
}

// Dummy function that shouldn't be used
double TQSignificanceEvaluatorBase::getSignificance(size_t iregion) {
  return significance(iregion, simpleSignif);
}

double TQSignificanceEvaluatorBase::getSignificance2(size_t iregion) {
  auto signif = getSignificance(iregion);
  return signif*signif;
}

vector<double> TQSignificanceEvaluatorBase::evaluateMultiple(){
  // in the evaluation method where multiple FOMs are evaluated;
  // we will ask for different types of initialzation
  // and perform the corresponding steps needed to return a value
  if(this->initialization == TQGridScanner::Class()){
    // if the initialization was done with a GridScanner
    // we have pointers to grids containing the information we need
    if (m_functionsFOM.size() == 0 && m_functionsFOMWithUnc.size() == 0 && m_functionsFOMWithBkgUnc.size() == 0) {
      WARNclass("Figure of merits cannot be returned because no functions are specified!");
      return {};
    }

    this->info = "";
    int length = m_functionsFOM.size();
    length +=  m_functionsFOMWithUnc.size();
    length +=  m_functionsFOMWithBkgUnc.size();
    std::vector<double> fom2(length, 0.0);
    for (size_t i=0; i<m_regions.size(); i++) {
      auto foms =  getFOMs(i);
      if (!foms.empty()) {
        for (int j = 0; j < length; j++ ) {
          fom2[j] += foms[j]*foms[j];
        }
      }
    }
    std::vector<double> fom;
    for (int j = 0; j < length; j++ ) {
      double final_fom = sqrt(fom2[j]);
      fom.push_back(final_fom);
      if (not this->scanner->hasSplitObs) {// if Gridscanner has split obs don't write out yields and write out significance later
        if (m_regions.size() > 1) {
          this->info += TString::Format("FOM(%s, comb.)=%g ; ", m_FOMDefinitions[j].Data(), final_fom);
        }
      }
    }
    return fom;
  }

  // if we didn't find a valid initialization method
  // we return zero and issue an error message
  ERRORclass("cannot evaluate without valid initialization!");
  return {};
}


void TQSignificanceEvaluatorBase::printHistogramAxis() {
  int nHists = m_signalHists.size();
  INFO(TString::Format("provided %d signal/backgroud hists with axis:", nHists));
  for (int i=0; i<nHists; i++) {
    INFO(TString::Format("histogram %d", i));
    for (int j=0; j<m_signalHists[i]->GetNdimensions(); j++) {
      INFO(TString::Format("name of axis %d: %s", j, m_signalHists[i]->GetAxis(j)->GetName()));
    }
  }
}

bool TQSignificanceEvaluatorBase::initialize(TQGridScanner* scanner){
  // if the initialization was done with a TQGridScanner
  // we remember this and retrieve the grids from the reader now
  // to be able to use them later on
  if(!scanner){
    ERRORclass("no TQGridScanner appointed to initialization!");
    return false;
  }
  if(!reader){
    ERRORclass("no TQSampleDataReader available!");
    return false;
  }
  this->initialization = scanner->Class();
  this->scanner = scanner;
  if(m_regions.size() < 1){
    ERRORclass("no regions set!");
    return false;
  }
  if(this->verbose) VERBOSEclass("initializing with %i regions",m_regions.size());
  for(size_t i=0; i<m_regions.size(); i++){
    THnBase* s = this->reader->getTHnBase(this->signalPath, TQFolder::concatPaths(m_regions[i], scanner->nDimHistName().Data()));
    THnBase* b = this->reader->getTHnBase(this->backgroundPath, TQFolder::concatPaths(m_regions[i],scanner->nDimHistName().Data()));
    if(!s || !b){
      ERRORclass("Unable to retrieve THnBase '%s/%s' from '%s' and/or '%s' in folder '%s'",m_regions[i].Data(),scanner->nDimHistName().Data(),this->signalPath.Data(),this->backgroundPath.Data(),this->reader->getSampleFolder()->GetName());
      return false;
    }

    // First process multidimensional histograms to only regard dimensions that are actually used in gridscan
    if (!scanner->obsNamesToScan().empty()) {
      std::vector<int> axesIndizesToRegard;
      for (TString name : scanner->obsNamesToScan()) {
        bool found = false;
        for (int idim=0; idim<s->GetNdimensions(); idim++) {
          if (name.EqualTo(s->GetAxis(idim)->GetName())) {
            found = true;
            axesIndizesToRegard.push_back(idim);
          }
        }
        if (!found) {
          BREAKclass("Observable with name %s not found in input histogram! Please check your input configuration and try again.", name.Data());
        }
      }
      // project out only relevant dimensions!
      s = s->ProjectionND(axesIndizesToRegard.size(), axesIndizesToRegard.data(), "E");
      b = b->ProjectionND(axesIndizesToRegard.size(), axesIndizesToRegard.data(), "E");
    }

    m_signalHists.push_back(s);
    m_bkgHists.push_back(b);
    bool addSig = scanner->addSignalHist(s);
    bool addBkg = scanner->addBkgHist(b);
    if (!addSig || !addBkg) {
      ERRORclass("Unable to add THnBase '%s/%s' from '%s' and/or '%s' in folder '%s' because it is not consistent with the previously added THnBase from another region!",m_regions[i].Data(),scanner->nDimHistName().Data(),this->signalPath.Data(),this->backgroundPath.Data(),this->reader->getSampleFolder()->GetName());
      throw std::runtime_error("Defined multidimensional histograms not consistent! Exiting");
    }
    // pass FOM definitions directly to TQGridscanResults! Kind of convoluted relationships
    scanner->addFOMDefinitions(m_FOMDefinitions);
  }
  if(this->verbose) printHistogramAxis();
  return true;
}

bool TQSignificanceEvaluatorBase::setRangeAxis(int axis, double low, double up) {
  // restrict the range for the specified axis in
  for (unsigned int i=0; i<m_signalHists.size(); i++) {
    if (!m_signalHists[i] || !m_bkgHists[i]) {
      WARNclass(TString::Format("cannot set range for axis %d for histograms", axis));
      return false;
    }
    m_signalHists[i]->GetAxis(axis)->SetRangeUser(low, up);
    m_bkgHists[i]->GetAxis(axis)->SetRangeUser(low, up);
  }
  return true;
}

bool TQSignificanceEvaluatorBase::updateHists(
    vector<int> axesToScan,
    TQGridScanner* scanner,
    int axisToEvaluate
) {
  // Project out the axis which are actually scanned for optimization.
  // This will improve the performance during the actual scan

  // always calculate the significance on the same axis to not risk any discrepancies when
  // calculating the significance in different distributions
  vector<int> axis;
  axis.push_back(axisToEvaluate);  // must be set in first position of vector!
  for (int i : axesToScan) {
    if (i != axisToEvaluate) axis.push_back(i);
  }
  axesToScan = axis;
  for (unsigned int i=0; i<m_signalHists.size(); i++) {
    if (!m_signalHists[i] || !m_bkgHists[i]) {
      WARNclass("cannot update hist");
      return false;
    }
    m_signalHists[i] = m_signalHists[i]->ProjectionND(axesToScan.size(), axesToScan.data(), "E");
    m_bkgHists[i] = m_bkgHists[i]->ProjectionND(axesToScan.size(), axesToScan.data(), "E");
    // WARNING: TQGridScanner only holds a single signal and single background hist
    scanner->addSignalHist(m_signalHists[i]);
    scanner->addBkgHist(m_bkgHists[i]);
  }
  return true;
}

double TQSignificanceEvaluatorBase::significance(size_t iregion, const std::function<double (double, double)>& signifFunc) {
  auto b = bkg(iregion);
  if(b < getTagDoubleDefault("cutoff",0)) {
    info += m_regions[iregion] + " skipped: low stats ; ";
    return 0;
  }
  auto s = signal(iregion);
  auto signif = signifFunc(b, s);
  if (not this->scanner->hasSplitObs) {// if Gridscanner has split obs don't write out yields and write out significance later
    info += TString::Format("%s: s=%g, b=%g, FOM(%s)=%g ; ", m_regions[iregion].Data(), s, b, GetName(), signif);
  }
  return signif;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQSimpleSignificanceEvaluator
//
// The TQSimpleSignificanceCalculator is a very basic implementation of a
// TQSignificanceEvaluator that simply calculates the signal/sqrt(background) ratio
//
////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(TQSimpleSignificanceEvaluator)

TQSimpleSignificanceEvaluator::TQSimpleSignificanceEvaluator(
    TQSampleFolder* sf,
    TString signal,
    TString background,
    TString name
) :
  TQSignificanceEvaluatorBase(sf,signal,background,name)
{}

double TQSimpleSignificanceEvaluator::getSignificance(size_t iregion){
  return significance(iregion, simpleSignif);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQPoissonSignificanceEvaluator
//
// The TQPoissonSignificanceCalculator is a rather basic implementation of a
// TQSignificanceEvaluator that calculates the Poisson significance
// Since the initialization is identical to the TQSimpleSignificanceEvaluator
// we inherit from that as a base class and only replace the name
// and the evaluation method.
//
////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(TQPoissonSignificanceEvaluator)

TQPoissonSignificanceEvaluator::TQPoissonSignificanceEvaluator(
    TQSampleFolder* sf,
    TString signal,
    TString background,
    TString name
) :
  TQSignificanceEvaluatorBase(sf,signal,background,name)
{}

double TQPoissonSignificanceEvaluator::getSignificance(size_t iregion){
  return significance(iregion, TQHistogramUtils::getPoisson);
}

ClassImp(TQSimpleSignificanceEvaluator2)

TQSimpleSignificanceEvaluator2::TQSimpleSignificanceEvaluator2(
    TQSampleFolder* sf,
    TString signal,
    TString background,
    TString name
) :
  TQSignificanceEvaluatorBase(sf,signal,background,name)
{}

double TQSimpleSignificanceEvaluator2::getSignificance(size_t iregion){
  return significance(iregion, simple2Signif);
}


ClassImp(TQSimpleSignificanceEvaluator3)

TQSimpleSignificanceEvaluator3::TQSimpleSignificanceEvaluator3(
    TQSampleFolder* sf,
    TString signal,
    TString background,
    TString name
) :
  TQSignificanceEvaluatorBase(sf,signal,background,name)
{}

double TQSimpleSignificanceEvaluator3::getSignificance(size_t iregion){
  return significance(iregion, simple3Signif);
}
