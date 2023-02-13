//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQHistogramUtils__
#define __TQHistogramUtils__

class TH1;
class TH2;
class THStack;
class TGraph;
class TMultiGraph;
class TPrincipal;
class TGraphAsymmErrors;
class TGraph2D;
class TList;
class TAxis;
class TLine;
class TStyle;
class TCanvas;

class TQTaggable;
class TQCounter;

#include "TH1.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"
#include "TString.h"

#include <vector>
#include <limits>

namespace TQHistogramUtils {

  enum FOM {
    kSoSqB,
    kSoSqBpdB,
    kSoSqSpB,
    kSoB,
    kPoisson,
    kPoissonWErr,
    kUndefined
  };

  enum Axes {
    X=0,
    Y=1,
    Z=2
  };

  bool hasBinLabels(const TH1* h);
  bool hasBinLabels(const TAxis* a);
  TAxis* getAxis(TNamed* obj, int idx);
  const TAxis* getAxis(const TNamed* obj, int idx);  

  std::vector<double> getBinLowEdges(const TH1* histo, const std::vector<int>& binBorders);
  std::vector<double> getBinLowEdges(const TAxis* histo, const std::vector<int>& binBorders);
  std::vector<int> getBinBorders(const TH1* histo, const std::vector<double>& lowEdges);
  std::vector<int> getBinBorders(const TAxis* histo, const std::vector<double>& lowEdges);
  std::vector<double> getUniformBinEdges(int nBins, double min, double max);
  bool hasUniformBinning(const TH1 * hist);
  bool hasUniformBinning(const TAxis* axis);

  void scaleErrors(TH1* hist, double scale);

  bool extractBinning(TQTaggable * p, int &index, int &nBins, double &min, double &max, std::vector<double> &edges, TString &errMsg);
  bool extractRange(TQTaggable * p, int &index, double &min, double &max, TString &errMsg);
  std::vector<TString> histoBinsToCutStrings(TH1* hist, const TString& varexpr, TString cutname = "", const TString& basecutname = "");

  int countHoles(TH2* hist);
  int fillHoles2DAverage(TH2* hist, double threshold, bool allowUnderflowOverflow = false, int dx=1, int dy=1, int minSources=1);
  int fillHoles2DMax    (TH2* hist, double threshold, bool allowUnderflowOverflow = false, int dx=1, int dy=1, int minSources=1);

  TString getHistogramContents(const TH1 * histo);
  TString getHistogramDefinition(const TH1 * histo);
  TString getGraphDefinition(const TNamed * graph);
  TString getBinningDefinition(const TAxis * axis);
  bool setHistogramContents(TH1 * histo, const TString& contents);
  TString convertToText(const TH1 * histo, int detailLevel);
  TH1* convertFromText(TString input);

  TH1* createHistogram(TQTaggable* values, const TString& prefix);
  TH2* createHistogram(const TGraph2D* graph, int precision);
  TH2* createHistogram(const TGraph2D* g, int nbinsx, int nbinsy);  
  std::vector<TList*> getContourList(TH2* hist, const std::vector<double>& thresholds);

  bool isEmpty(const TH1 * hist, double threshold, double mean=0);

  inline int getNbinsGlobal(const TH1* hist, bool ignoreLabels = false){
    if(!hist) return -1;
    if(TQHistogramUtils::hasBinLabels(hist) && !ignoreLabels) return hist->GetNbinsX()*hist->GetNbinsY()*hist->GetNbinsZ();
    return const_cast<TH1*>(hist)->FindBin(std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity()) +1;
  }

  TH1 * createHistogram(const TString& definition, bool printErrMsg = false);
  TH1 * createHistogram(TString definition, TString &errMsg);
  TNamed * createGraph(const TString& definition, bool printErrMsg = false);
  TNamed * createGraph(TString definition, TString &errMsg);

  TString getDetailsAsString(const TNamed * obj, int option = 1);
  TString getDetailsAsString(const TH1 * histo, int option = 1);
  TString getDetailsAsString(const TAxis * axis, int option = 1);
  TString getDetailsAsString(const TGraph * g, int option = 1);
  TString getDetailsAsString(const TGraph2D * g, int option = 1);
  TString getDetailsAsString(const TPrincipal * p, int option = 1);
  int dumpData(TPrincipal * p, int cutoff = 100);
  bool isAlphanumeric(const TAxis* ax);
  
  /* significance calculation */

  double getPoisson(double b, double s);
  double getPoissonError(double b, double s, double db, double ds);

  double getPoissonWithError(double b, double db, double s);
  double getPoissonWithErrorError(double b, double s, double db, double ds);

  double getSoverSqrtB(double b, double s);
  double getSoverSqrtSplusB(double b, double s);

  double getSoverB(double b, double s);

  double getSignificance(double b, double s, TString sgnfName, double berr, TString * sgnfTitle = 0);

  void getSgnfAndErr(double b, double bErr, double s, double sErr,
                     double & sgnf, double & sgnfErr, TString sgnfName, TString * sgnfTitle = 0);

  double getFOM(FOM fom, double b, double berr, double s, double serr);
  TString getFOMTitleROOT(FOM fom);
  TString getFOMTitleLaTeX(FOM fom);
  TH1* getFOMScan(TQHistogramUtils::FOM fom, TH1* sig, TH1* bkg, bool fromleft, double cutoff, bool verbose, const std::vector<TH1*>& bkgSystHistos = std::vector<TH1*>{});
  TH1* getFOMHistogram(TQHistogramUtils::FOM fom, TH1* sig, TH1* bkg, double cutoff = 0, const std::vector<TH1*>& bkgSystHistos = std::vector<TH1*>{} );
  FOM readFOM(TString fom);

  double pValuePoisson(unsigned nObs, double nExp);
  double pValuePoissonError(unsigned nObs, double E=1, double V=1);
  double pValueToSignificance(double p, bool excess=true);
  TH1* pcmpObsVsExp(TH1* hObs, TH1* hExp, bool ignoreExpUnc=false);

  TH1* getUncertaintyHistogram(TH1* hist);
  TGraph* scatterPlot(const TString& name, double* vec1, double* vec2, int vLength, const TString& labelX = "x", const TString& labelY = "y");
  TGraph* scatterPlot(const TString& name, std::vector<double>& vec1, std::vector<double>& vec2, const TString& labelX = "x", const TString& labelY = "y");

  TMultiGraph* makeMultiColorGraph(const std::vector<double>& vecX, const std::vector<double>& vecY, const std::vector<short>& vecColors);

  /* ===== histogram utils===== */


  TH1 * invertShift(TH1 * var, TH1 * nom);
  TH1 * invertShiftWithoutError(TH1 * var, TH1 * nom);

  TH1* symmetrizeFromTwo(const TH1* var1, const TH1* var2, TH1* nom);

  bool applyPoissonErrors(TH1 * histo);

  TH1 * copyHistogram(const TH1 * histo, const TString& newName = "");
  TNamed * copyGraph(const TNamed * histo, const TString& newName = "");
  void copyGraphAxisTitles(TNamed* copy, const TNamed* graph);

  TH1 * convertTo1D(const TH2 * histo, bool alongX = true, bool includeUnderflowsX = true,
                    bool includeOverflowsX = true, bool includeUnderflowsY = true, bool includeOverflowsY = true);

  int getNDips(TH1 * histo);

  void interpolateGraph(TGraph* g, size_t increasePoints, const char* option="S");

  bool transferRelativeErrors(TH1* sourceHist, TH1* targetHist);
  bool transferRelativeErrors(TQCounter* sourceHist, TQCounter* targetHist);

  TH1* sumHistograms(TCollection* histos);
  TH1* sumHistograms(const std::vector<TH1*>& histos);


  /* ===== rebinning/remapping of histograms===== */

  std::vector<int> getBinBordersFlat(TH1 * histo, int nBins, bool includeOverflows);
  std::vector<int> getBinBordersFlat2D(TH2 * histo, int nBins, bool remapX, bool includeOverflows, bool remapSlices = true);

  std::vector<int> getOptimizedBinBorders(TH1 * hsig, TH1 * hbkg, double minSignal = 10, double minBkg = 10, double maxBkgUnc = 0.2,  bool significanceAgnostic = false, double maxSignal = 30, double estimatedMaxSignInBin = 6, bool verbose = false);


  void remap(TAxis* ax, double min=0, double max=1);
  bool rebin(TH1*& hist, const std::vector<double>& boundaries, bool doRemap = false);
  bool rebin(TH1*& hist, const std::vector<int>& boundaries, bool doRemap = false);
  bool rebin(TH1*& hist, const std::vector<double>& boundariesX, const std::vector<double>& boundariesY, bool doRemap = false);
  bool rebin(TH1*& hist, const std::vector<int>& boundariesX, const std::vector<int>& boundariesY, bool doRemap = false);
  bool rebin(TH1* hist, int rebinX, int rebinY, int rebinZ, bool doRemap = false);

  void reorderBins(TH2* hist, const std::vector<TString>& ordering);


  std::vector<int> getBinsSortedByRatio(TH1* sig, TH1* bkg, double epsilon = 1e-12);
  void reorder(TH1* hist, const std::vector<int>& bins);

  TH1 * getRebinned(const TH1 * histo, const std::vector<int>& binBorders, bool doRemap = false, bool keepOverFlowUnderFlowSeparate = false);
  TH1 * getRebinned(const TH1 * histo, const std::vector<double>& lowEdges, bool doRemap = false, bool keepOverFlowUnderFlowSeparate = false);
  TH1 * getRebinned(const TH1 * histo, const std::vector<int>& binBordersX, const std::vector<int>& binBordersY, bool doRemap = false, bool keepOverFlowUnderFlowSeparate = false);
  TH1 * getRebinned(const TH1 * histo, const std::vector<double>& lowEdgesX, const std::vector<double>& lowEdgesY, bool doRemap = false, bool keepOverFlowUnderFlowSeparate = false);
  TH1 * getRebinned(const TH1 * histo, const std::vector<int>& binBorders, const std::vector<double>& lowEdges, bool doRemap = false, bool keepOverFlowUnderFlowSeparate = false);

  TH1 * getSmoothedManually(const TH1 * histo, const std::vector<double>& lowEdges, bool keepOverFlowUnderFlowSeparate = true);
  TH1 * getSmoothedManually(const TH1 * histo, const std::vector<int>& binBorders, bool keepOverFlowUnderFlowSeparate = true);
  TH1 * getSmoothedVariation(const TH1 * h_nom, TH1 * h_var, const std::vector<int>& binBorders, bool keepOverFlowUnderFlowSeparate = true);

  TH2 * getRemapped2D(TH2 * histo, const std::vector<int>& binBorders, bool remapX = true);

  TH1 * getRebinnedFlat(TH1 * histo, int nBins);
  TH1 * getRemappedFlat(TH1 * histo, int nBins);
  TH2 * getRemappedFlat2D(TH2 * histo, int nBins, bool remapX = true);


  TH1 * cutHistogram(TH1 * histo, int xBinLow, int xBinHigh, int yBinLow = -1, int yBinHigh = -1,
                     bool keepInUVX = false, bool keepInOVX = false, bool keepInUVY = false, bool keepInOVY = false);
  TH1 * cutAndZoomHistogram(TH1 * histo, int cutBinLowX, int cutBinHighX, int cutBinLowY = -1, int cutBinHighY = -1,
                            int zoomBinLowX = -1, int zoomBinHighX = -1, int zoomBinLowY = -1, int zoomBinHighY = -1);

  TH2 * removeBins(TH2* in, const std::vector<TString>& blackList);
  TH2 * removeBins(TH2* in, TString blackList);


  bool includeSystematics(TH1 * histo, TH1 * systematics);

  TH1 * getSystematics(TH1 * h_nominal, TList * singleVars, TList * pairVars = 0);

  bool addHistogram(TH1 * histo1, const TH1 * histo2, double scale = 1., double scaleUncertainty = 0., double corr12 = 0., bool includeScaleUncertainty=true);
  bool addHistogram(TH1 * histo1, const TH1 * histo2, TQCounter* scale, double corr12 = 0., bool includeScaleUncertainty=false);

  bool addHistogramAsError(TH1 * hist, const TH1 * errorhist, bool isRelative = false);
  
  bool addHistogramInQuadrature(TH1 * histo1, const TH1 * histo2, double factor=1);
  bool addHistogramInQuadrature(TH1 * histo1, double val, double factor=1);  
  bool replaceBins(TH1 * histo1, TH1 * histo2, std::vector<int> bins);  
  
  TH1* sumOfSquares(const std::vector<TH1*>& histograms); //returns a*a+b*b+... (bin-by-bin)
  TH1* sumHistogramsInQuadrature(const std::vector<TH1*>& histograms); //returns sqrt(a*a+b*b+...) (bin-by-bin)


  bool addHistogramWithoutError(TH1 * histo1, const TH1 * histo2, double scale=1.);
  bool divideHistogramWithoutError(TH1 * histo1, const TH1 * histo2);
  bool multiplyHistogramWithoutError(TH1 * histo1, const TH1 * histo2);

  bool scaleHistogram(TH1 * histo1, double scale = 1., double scaleUncertainty = 0., bool includeScaleUncertainty = true);
  bool scaleHistogram(TH1 * histo1, TQCounter* scale, bool includeScaleUncertainty = false);
  bool addGraph(TGraph * graph1, const TGraph * graph2);
  bool addGraph(TGraph2D * graph1, const TGraph2D * graph2);
  bool addGraph(TNamed * graph1, const TNamed * graph2);

  bool drawHistograms(TList * histograms, TString drawOption = "", TString extOptions = "");

  bool resetBinErrors(TH1 * histo);
  int fixHoles1D(TH1* hist, double threshold);

  int getDimension(const TH1 * histo);

  int getNBins(const TH1 * histo, bool includeUnderflowOverflow = true);

  int getSizePerBin(const TH1 * histo);

  int estimateSize(const TH1 * histo);

  bool checkConsistency(const TH1 * histo1, const TH1 * histo2, bool verbose=false);
  bool areEqual(TH1* first, TH1* second, bool includeUnderflowOverflow = true, bool compareErrors = false, double tolerance=0.01);

  /* */

  TH1 * getCutEfficiencyHisto(TH1 * histo, TString options = "");

  TH1 * getSignificanceHisto(TH1 * histo_bkg, TH1 * histo_sig, TString options = "");

  double getSummedBinByBinSignificance(TH1 * histo_bkg, TH1 * histo_sig, TString options = "sgnf:poisson");

  TGraphAsymmErrors * getROCGraph(TH1 * h_bkg, TH1 * h_sig, bool lowerBound);

  TList * getProjectionHistograms(TH2 * histo, bool projectOnX, bool normalize = false);
  TList * getProjectionHistogramsX(TH2 * histo, bool normalize = false);
  TList * getProjectionHistogramsY(TH2 * histo, bool normalize = false);

  TH1 * getReweightedHistogram(TH2 * histo_input, TH1 * histo_weights, bool projectOnX);
  TH1 * getReweightedHistogramX(TH2 * histo_input, TH1 * histo_weights);
  TH1 * getReweightedHistogramY(TH2 * histo_input, TH1 * histo_weights);

  TQCounter * histogramToCounter(TH1 * histo);

  TH1 * counterToHistogram(TQCounter * counter);
  TH1 * countersToHistogram(TList * counters);

  TH1 * getEfficiencyHistogram(TH1* numerator, TH1* denominator);

  TH1 * getRelativeVariationHistogram(TH1* variation, TH1* nominal, double epsilon = 0.);
  /* projections */

  TH1 * getProjection(TH1 * histo, bool onX, int binLow = -1, int binHigh = -1);
  TH1 * getProjectionX(TH1 * histo, int binLow = -1, int binHigh = -1);
  TH1 * getProjectionY(TH1 * histo, int binLow = -1, int binHigh = -1);


  /* */

  double getIntegral(const TH1 * histo, bool userUnderOverflow = true);
  double getIntegralError(const TH1 * histo);
  double getIntegralAndError(const TH1 * histo, double &error, bool useUnderflowOverflow = true);

  TH1 * normalize(TH1 * histo, double normalizeTo = 1.);
  TH1 * normalize(TH1 * histo, const TH1 * normalizeToHisto);

  TH1 * power(TH1 * histo, double exp);

  TH1 * getSlopeHistogram(TH1 * input, double slope);
  TH1 * applySlopeToHistogram(TH1 * input, double slope);

  double getChi2(TH1 * histo1, TH1 * histo2);

  TH1 * includeOverflowBins(TH1 * histo, bool underflow = true, bool overflow = true);
  TH1 * extraOverflowBins(TH1*& histo, bool underflow = true, bool overflow = true, bool remap = false);

  void unifyMinMax(TCollection * histograms, double vetoFraction = .9);
  void unifyMinMax(TH1 * h1, TH1 * h2, TH1 * h3, double vetoFraction = .9);
  void unifyMinMax(TH1 * h1, TH1 * h2, double vetoFraction = .9);

  bool getMinMaxBin(const TH1 * histo, int &minBin, int &maxBin, bool includeError = false, bool includeUnderflowOverflow = true, double minMin = -std::numeric_limits<double>::infinity(), double maxMax = std::numeric_limits<double>::infinity());
  bool getMinMax(const TH1 * histo, double &min, double &max, bool includeError = false, bool includeUnderflowOverflow = true, double minMin = -std::numeric_limits<double>::infinity(), double maxMax = std::numeric_limits<double>::infinity());
  int getMinBin(const TH1 * histo, bool includeError = false, bool includeUnderflowOverflow = true, double minMin = -std::numeric_limits<double>::infinity());
  int getMaxBin(const TH1 * histo, bool includeError = false, bool includeUnderflowOverflow = true, double maxMax = std::numeric_limits<double>::infinity());
  double getMin(const TH1 * histo, bool includeError = false, bool includeUnderflowOverflow = true, double minMin = -std::numeric_limits<double>::infinity());
  double getMax(const TH1 * histo, bool includeError = false, bool includeUnderflowOverflow = true, double maxMax = std::numeric_limits<double>::infinity());
  double getMax(const TCollection* c, bool includeUnderflowOverflow = true, double maxMax = std::numeric_limits<double>::infinity() );
  double getMin(const TCollection* c, bool includeUnderflowOverflow = true, double minMin = -std::numeric_limits<double>::infinity() );

  bool getMaxArea2D(const TH2 * histo, double frac, int &maxX, int &maxY,
                    int &maxX_low, int &maxX_high, int &maxY_low, int &maxY_high);

  bool extractStyle(const TH1 * histo, TQTaggable * tags, const TString& styleScheme = "");
  bool extractStyle(const TGraph * graph, TQTaggable * tags, const TString& styleScheme = "");
  int applyStyle(TH1 * histo, TQTaggable * tags, const TString& styleScheme = "", bool allowRecursion = true);
  bool copyStyle(TH1 * dest, const TH1 * src);
  bool copyStyle(TGraph * dest, const TH1 * src);
  bool copyStyle(TGraph * dest, const TGraph * src);
  bool copyStyle(TGraph2D * dest, const TGraph2D * src);
  bool copyStyle(TNamed * dest, const TNamed * src);

  bool copyBinLabels(const TH1* source, TH1* target);
  bool copyBinLabels(const TAxis* source, TAxis* target);
  bool copyAxisStyle(const TH1* source, TH1* target);
  bool copyAxisStyle(const TAxis* source, TAxis* target);

  TGraphAsymmErrors * getGraph(const TH1 * histo);
  TGraphAsymmErrors * getGraph(const TH1* nom, TObjArray* sys);

  bool isCloseToOneRel(double val, double rel);
  bool areEqualRel(double val1, double val2, double rel);
  bool haveEqualShapeRel(TH1 * h1, TH1 * h2, double rel);

  void setRelativeUncertainties(TH1* hist,double relunc,bool addToInitialValue);

  bool ensureMinimumContent(double &content, double &uncertainty, double min = 1E-12, bool ignoreNegative = false, bool flipToPositive = false, bool assignUncertainty = false);
  int ensureMinimumBinContent(TH1 * histo, double min = 1E-12, bool ignoreNegative = false, bool flipToPositive = false, bool assignUncertainty = false);

  template<class TMatrixTT> TH2* convertMatrixToHistogram(const TMatrixTT* matrix, const TString& name);
  TMatrixD* convertHistogramToMatrix(TH2* hist);
  TMatrixDSym* convertHistogramToSymMatrix(TH2* hist, double relThreshold=1e-9, double absThreshold=1e-9);
  TMatrixDSym* convertHistogramToSymMatrix(TH2* hist, const std::vector<TString>& ordering, double relThreshold=1e-9, double absThreshold=1e-9);

  bool envelopeUpper(TH1* hist, TH1* otherhist);
  bool envelopeLower(TH1* hist, TH1* otherhist);

  void setSliceX(TH2* hist2d, TH1* hist, double value);
  void setSliceY(TH2* hist2d, TH1* hist, double value);

  void getFilteredRange(TCollection* histograms, double xmin, double xmax, double minContent, double maxContent, double& lower, double& upper, double logMin);
  void getUnFilteredRange(TCollection* histograms, double xmin, double xmax, double minContent, double maxContent, double& lower, double& upper);
  double getMinimumBinValue(TH1* hist, double xmin, double xmax, bool includeErrors=false, double minContent=-std::numeric_limits<double>::infinity(), double maxContent=std::numeric_limits<double>::infinity());
  double getMaximumBinValue(TH1* hist, double xmin, double xmax, bool includeErrors=false, double minContent=-std::numeric_limits<double>::infinity(), double maxContent=std::numeric_limits<double>::infinity());
  double getMinimumBinValue(TCollection* hist, double xmin, double xmax, bool includeErrors=false, double minContent=-std::numeric_limits<double>::infinity(), double maxContent=std::numeric_limits<double>::infinity());
  double getMaximumBinValue(TCollection* hist, double xmin, double xmax, bool includeErrors=false, double minContent=-std::numeric_limits<double>::infinity(), double maxContent=std::numeric_limits<double>::infinity());
  double getHistogramMaximum(size_t n, ...);
  double getHistogramMinimum(size_t n, ...);
  double getHistogramXmax(size_t n, ...);
  double getHistogramXmin(size_t n, ...);
  double getHistogramYmax(size_t n, ...);
  double getHistogramYmin(size_t n, ...);

  double getHistogramBinContentFromFile(const TString& fname, const TString& hname, const TString binlabel);
  double getHistogramBinContentFromFile(const TString& fname, const TString& hname, int);
  int edge(TH1* hist, double cutoff = std::numeric_limits<double>::epsilon());
  int edge(TH2* hist, double cutoff = std::numeric_limits<double>::epsilon());

  double getAxisYmin(const TH1* hist);
  double getAxisYmax(const TH1* hist);
  double getAxisXmin(const TH1* hist);
  double getAxisXmax(const TH1* hist);
  double getAxisXmin(const TGraph* graph);
  double getAxisXmax(const TGraph* graph);

  double getContourArea(const TGraph* g);
  double getContourJump(const TGraph* g);

  double getMinBinWidth(TAxis*a);
  double getMinBinArea(TH2* hist);


  TH1* getSoverSqrtBScan(TH1* signal, TH1* bkg, bool fromleft, double cutoff = 0.05, bool verbose=false);
  TH1* getSoverSqrtB(TH1* sig, TH1* bkg);

  void print(THStack* s, TString options="");

  int addPCA(TPrincipal* orig, TPrincipal* add);

  double sumLineDistSquares(TH2* hist, double a, double b, bool useUnderflowOverflow = false);
  TH1* getLineDistSquares(TH2* hist, double a, double b, bool useUnderflowOverflow = false);
  double calculateBinLineDistSquare(TH2* hist, double a, double b, int i, int j);
  TH1* rotationProfile(TH2* hist, double xUnit = -1, double yUnit = -1, int nStep = 36, double xOrig = 0, double yOrig = 0);
  TH2* rotationYtranslationProfile(TH2* hist, double xUnit = -1, double yUnit = -1, int nStepAngle = 36, int nStepOffset = -1, double xOrig = 0, double y0 = std::numeric_limits<double>::quiet_NaN());
  TH2* rotationXtranslationProfile(TH2* hist, double xUnit = -1, double yUnit = -1, int nStepAngle = 36, int nStepOffset = -1, double yOrig = 0, double x0 = std::numeric_limits<double>::quiet_NaN());
  TLine* makeBisectorLine(TH1* hist, double angle = 45, double xUnit = -1, double yUnit = -1, double xOrig = 0, double yOrig = 0);

  double clearBinsAboveX(TH1* hist, double xMax);
  double clearBinsBelowX(TH1* hist, double xMin);
  double clearBinsAboveX(TH2* hist, double xMax);
  double clearBinsBelowX(TH2* hist, double xMin);
  double clearBinsAboveY(TH2* hist, double yMax);
  double clearBinsBelowY(TH2* hist, double yMin);

  bool cropLine(TH1* hist, TLine* l);
  bool isGreaterThan(TH1* hist1, TH1* hist2);
  bool isUnderflowOverflowBin(TH1* hist, int bin);
  bool hasGreaterMaximumThan(TH1* hist1, TH1* hist2);

  void printHistogramASCII(TH1* hist, const TString& tags = "");
  void printHistogramASCII(TH1* hist, TQTaggable& tags);
  void printHistogramASCII(std::ostream&, TH1* hist, const TString& tags);
  void printHistogramASCII(std::ostream&, TH1* hist, TQTaggable& tags);

  TH1* unrollHistogram(const TH2* input, bool firstX = true, bool includeUnderflowOverflow = false);
  TObjArray* getSlices(const TH2* input, bool alongX = true);

  void rerollGauss(TH1* hist, double zvalue = 1);
  void rerollPoisson(TH1* hist,int seed=0);

  TStyle* ATLASstyle();
  TCanvas* applyATLASstyle(TH1* histo, const TString& label = "Internal", double relPosX = 0.1, double relPosY = 0.1, double yResize = 1., const TString& xTitle = "none", const TString& yTitle = "none", bool square = false);
  TCanvas* applyATLASstyle(TGraph* graph, const TString& label = "Internal", double relPosX = 0.1, double relPosY = 0.1, double yResize = 1., const TString& xTitle = "none", const TString& yTitle = "none", bool square = false);
  TCanvas* applyATLASstyle(TMultiGraph* graph, const TString& label = "Internal", double relPosX = 0.1, double relPosY = 0.1, double yResize = 1., const TString& xTitle = "none", const TString& yTitle = "none", bool square = false);
}


#endif
