#include "TCanvas.h"
#include "TH1.h"
#include "TLine.h"
#include "TString.h"

#include "QFramework/TQGridScanner.h"
#include "QFramework/TQGridScanResults.h"
#include "QFramework/TQGridScanObservable.h"
#include "QFramework/TQTaggable.h"

using std::unique_ptr;

using BoundDirection = TQGridScanBound::Direction;

namespace TQGridScanStyle {
  
  unique_ptr<TH1F> rebinHist(unique_ptr<TH1F> hist, TQGridScanBound::Range range, BoundDirection direction);
  unique_ptr<TCanvas> defaultCanvas();
  void drawLabels(TQTaggable& tags);
  void setStyleAndRangeSignifProfile(TH1F* hist, TH1F* histmax, TQTaggable& tags);
  void setStyleSignifProfileGraphs(TGraph* graph, TGraph* graphmax, TQTaggable& tags);
  void setStyleInputHists(TH1F* hist_sig, TH1F* hist_bkg, TQTaggable& tags);
  void setStyle(TPad* pad, TH1F* hist, TQTaggable& tags);
  void drawLegend(TH1F* hist, TH1F* histmax, TQTaggable& tags, const TString& histlabel, TLine* cutline, TGraph* graph, TGraph* graphmax);
  TString getObsTitleSuffix(TString obsName, BoundDirection direction);
  void fillHists(TH1F* hist, TH1F* histSquares, TH1F* histFills, int nBin, double significance, int topNumber, bool showUnderflow, bool showOverflow);
  void fillErrors(TH1F* hist, TH1F* histSquares, TH1F* histFills);
  void excludeOverflow(TAxis* axis);
  void printDelimiter(int i, int n);
#if 0
  Bound* getBound(TQGridScanObservable* obs, BoundDirection direction);
  Bound* getSplitBound(TQGridScanObservable* obs);
#endif
  
}

