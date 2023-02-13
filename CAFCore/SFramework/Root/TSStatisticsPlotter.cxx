#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iterator>

#include "SFramework/TSStatisticsPlotter.h"
#include "QFramework/TQStringUtils.h"
#include "QFramework/TQTaggable.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQHistogramUtils.h"

#include "TLatex.h"
#include "TSystem.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphAsymmErrors.h"
#include "TGaxis.h"
#include "TStyle.h"
#include "TLine.h"
#include "TBox.h"
#include "TROOT.h"
#include "TMath.h"
#include "TLegend.h"

ClassImp(TSStatisticsPlotter)

#define inf std::numeric_limits<double>::infinity()

namespace {
  template<class GraphT> void configureFirstGraph(GraphT* graph, TQTaggable* input, double xMin, double xMax, double yMin, double yMax, double width, double height, const TString& opt){
    graph->GetXaxis()->SetTitle(input->getTagStringDefault("style.title.xAxis"));
    graph->GetYaxis()->SetTitle(input->getTagStringDefault("style.title.yAxis"));
    double xMinBreakdown = input->getTagDoubleDefault("style.breakdown.xMin", xMin);
    double xMaxBreakdown = input->getTagDoubleDefault("style.breakdown.xMax", xMax);
    graph->GetHistogram()->SetMinimum(yMin);
    graph->GetHistogram()->SetMaximum(yMax);
    graph->GetXaxis()->SetLimits(xMinBreakdown, xMaxBreakdown);
    double titleOffset = input->getTagDoubleDefault("style.titleOffset",1);
    double labelOffset = input->getTagDoubleDefault("style.labelOffset",1);
    double scaling = min(width,height);
    double yLabelSize = input->getTagDoubleDefault("style.yLabelSize", 0.025);
    double xLabelSize = input->getTagDoubleDefault("style.xLabelSize", 0.025);
    double yTitleSize = input->getTagDoubleDefault("style.yTitleSize", 0.05);
    double xTitleSize = input->getTagDoubleDefault("style.xTitleSize", 0.05);
    double yTickLength = input->getTagDoubleDefault("style.yTickLength", 0.025);
    double xTickLength = input->getTagDoubleDefault("style.xTickLength", 0.025);
    graph->GetXaxis()->SetTitleOffset((width/scaling)*titleOffset);
    graph->GetYaxis()->SetTitleOffset((height/scaling)*titleOffset);
    graph->GetYaxis()->SetLabelOffset(0.005*800/height*labelOffset);
    graph->GetXaxis()->SetLabelOffset(0.005*800/width*labelOffset);
    graph->GetYaxis()->SetLabelSize(yLabelSize);
    graph->GetXaxis()->SetLabelSize(xLabelSize);
    graph->GetYaxis()->SetTitleSize(yTitleSize);
    graph->GetXaxis()->SetTitleSize(xTitleSize);
    graph->GetYaxis()->SetTickLength(yTickLength);
    graph->GetXaxis()->SetTickLength(xTickLength);
    graph->Draw(TString("A") + opt);

    if(input->getTagBoolDefault("style.axis.x.blank",false)){
      TAxis* xax = graph->GetXaxis();
      xax->SetDrawOption("B");
    }
    if(input->getTagBoolDefault("style.axis.y.blank",false)){
      TAxis* yax = graph->GetYaxis();
      yax->SetDrawOption("B");
    }
  }
}

//__________________________________________________________________________________|___________

TQFolder* TSStatisticsPlotter::convertGraph(TGraph* g){
  // convert a TGraph into its TSStatisticsPlotter TQFolder representation
  if(!g || g->GetN() < 1) return NULL;
  TQFolder* result = new TQFolder(g->GetName());
  const int n(g->GetN());
  for(int i=0; i<n; ++i){
    double x,y;
    if(i != g->GetPoint(i,x,y)){
      throw std::runtime_error("graph length inconsistency detected!");
    }
    TQFolder* point = result->getFolder(TString::Format("p.%d+",i));
    point->setTagDouble("x",x);
    point->setTagDouble("y",y);
  }
  return result;
}

//__________________________________________________________________________________|___________

TGraph2D* TSStatisticsPlotter::createGraph2D(TQFolder* source,TQFolder* config){
  // convert a TSStatisticsPlotter TQFolder representation of a 2D graph into a TGraph2D
  TQFolderIterator points(source->getListOfFolders("?"),true);
  TGraph2D* helperGraph = new TGraph2D(points.flush());
  TString xTag = config ? config->getTagStringDefault("xTag","x") : "x";
  TString yTag = config ? config->getTagStringDefault("yTag","y") : "y";
  TString zTag = config ? config->getTagStringDefault("zTag","z") : "z";
  double xScale = config ? config->getTagDoubleDefault("xScale",1.) : 1.;
  double yScale = config ? config->getTagDoubleDefault("yScale",1.) : 1.;
  double zScale = config ? config->getTagDoubleDefault("zScale",1.) : 1.;
  helperGraph->SetDirectory(NULL);
  points.reset();
  int i = 0;
  while(points.hasNext()){
    TQFolder* point = points.readNext();
    if(!point) continue;
    double x,y,z;
    if(point->getTagDouble(xTag,x) && point->getTagDouble(yTag,y) && point->getTagDouble(zTag,z)){
      helperGraph->SetPoint(i,xScale*x,yScale*y,zScale*z);
    }
    ++i;
  }
  helperGraph->SetName(source->getTagStringDefault("name",source->GetName()));
  helperGraph->SetTitle(source->getTagStringDefault("title",source->GetName()));
  return helperGraph;
}

//__________________________________________________________________________________|___________

TQFolder* TSStatisticsPlotter::findGraphMinimum(TQFolder* source){
  // find the minimum in a TSStatisticsPlotter TQFolder representation of a graph
  TQFolderIterator points(source->getListOfFolders("?"),true);
  TQFolder* min = NULL;
  while(points.hasNext()){
    TQFolder* point = points.readNext();
    if(!point) continue;
    if(!min || point->getTagDoubleDefault("y",inf) < min->getTagDoubleDefault("y")) min = point;
  }
  return min;
}

//__________________________________________________________________________________|___________

TSStatisticsPlotter::TSStatisticsPlotter() {

  /* set the official ATLAS style */
  setStyleAtlas();

  /* initialize templates */
  fTemplates = NULL;
}


//__________________________________________________________________________________|___________

void TSStatisticsPlotter::setTemplates(TQFolder * templates) {

  fTemplates = templates;
}


//__________________________________________________________________________________|___________

TCanvas * TSStatisticsPlotter::plot(TQFolder * input) {

  /* stop if input is invalid */
  if (!input)
    return NULL;

  /* collections of elements to draw */
  TList * graphsToDraw  = new TList();
  TList * backElements  = new TList();
  TList * frontElements  = new TList();
  TList * lineYLabels    = new TList();

  /* log scale on y axis? */
  Bool_t logScale = input->getTagBoolDefault("style.logScale", false);

  TQIterator colormaps(input->getListOfObjects("Colormaps/*"),true);
  while(colormaps.hasNext()){
    TH2* colormap = dynamic_cast<TH2*>(colormaps.readNext());
    if(!colormap) continue;
    TColor* thecolor = gROOT->GetColor(colormap->GetFillColor());
    const Int_t Number = 4;
    Double_t Red   [Number] = { 1.00*thecolor->GetRed()  , 1.00*thecolor->GetRed()  , 1.00, 1.00};
    Double_t Green [Number] = { 1.00*thecolor->GetGreen(), 1.00*thecolor->GetGreen(), 1.00, 1.00};
    Double_t Blue  [Number] = { 1.00*thecolor->GetBlue() , 1.00*thecolor->GetBlue() , 1.00, 1.00};
    Double_t Length[Number] = { 0.00, 0.1, 1.0-1e-9, 1.00}; 
    const int nb=40;
    TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb,0.5);
    TQHistogramUtils::ensureMinimumBinContent(colormap,1e-6);
    backElements->Add(colormap,"COLSAME");
  }
  
  /* x and y range to plot */
  Double_t yMin = input->getTagDoubleDefault("style.yMinMax", inf);
  Double_t yMax = input->getTagDoubleDefault("style.yMaxMin", -inf);
  Double_t xMin = input->getTagDoubleDefault("style.xMinMax", inf);
  Double_t xMax = input->getTagDoubleDefault("style.xMaxMin", -inf);

  double xMinMin = input->getTagDoubleDefault("style.xMinMin", -inf);
  double xMaxMax = input->getTagDoubleDefault("style.xMaxMax", inf);
  double yMinMin = input->getTagDoubleDefault("style.yMinMin", -inf);
  double yMaxMax = input->getTagDoubleDefault("style.yMaxMax", inf);

  /* the legend */
  TLegend * legend =NULL;
  if(input->getTagBoolDefault("style.showLegend",true)){
    legend= new TLegend(input->getTagDoubleDefault("style.legend.x1",0.65),
                        input->getTagDoubleDefault("style.legend.y1",0.62),
                        input->getTagDoubleDefault("style.legend.x2",0.90),
                        input->getTagDoubleDefault("style.legend.y2",0.90));
  }

  /* loop over overlays */
  TQFolderIterator overlays(input->getListOfFolders("Overlay.*"), "!.*", true);
  while (overlays.hasNext()) {
    TQFolder * element = overlays.readNext();
    if(!element) continue;
    TString elementName = element->GetName();
    TQTaggable tagsElement;

    /* use style template? */
    TString templateName;
    if (tagsElement.getTagString("template",templateName) && fTemplates) {
      TQFolder * styleTemplate = fTemplates->getFolder(templateName);
      tagsElement.importTags(styleTemplate, false);
    }
    tagsElement.importTags(element,true,true);

    if (TQStringUtils::removeLeadingText(elementName, "Overlay.")) {

      /* count the number of points */
      TQIterator itrTmp = TQIterator(element->getListOfFolders("?"), "p.*", true);
      Int_t n = itrTmp.flush();

      /* create the graph using the number of points */
      TGraphAsymmErrors * graph = new TGraphAsymmErrors(n);
      graph->SetName(elementName.ReplaceAll(".","_"));

      /* loop over points */
      Int_t i = 0;

      double yMinLocal = +inf;
      double yMaxLocal = -inf;

      TQFolderIterator itrPoints(element->getListOfFolders("?"), "p.*", true);
      while (itrPoints.hasNext()) {
        TQFolder * point = itrPoints.readNext();

        Double_t xd  = point->getTagDoubleDefault("x");
        Double_t xp  = point->getTagDoubleDefault("xp", xd);
        Double_t xn  = point->getTagDoubleDefault("xn", xd);
        Double_t x  = point->getTagDoubleDefault("x", (xp + xn) / 2.);

        Double_t yd  = point->getTagDoubleDefault("y");
        Double_t yp  = point->getTagDoubleDefault("yp", yd);
        Double_t yn  = point->getTagDoubleDefault("yn", yd);
        Double_t y  = point->getTagDoubleDefault("y", (yp + yn) / 2.);

        graph->SetPoint(i, x, y);
        graph->SetPointError(i, x - xn, xp - x, y - yn, yp - y);

        xMin = TMath::Min(xMin, TMath::Min(x, TMath::Min(xp, xn)));
        xMax = TMath::Max(xMax, TMath::Max(x, TMath::Max(xp, xn)));

        if(x > xMinMin && x < xMaxMax){
          yMinLocal = TMath::Min(yMinLocal, TMath::Min(y, TMath::Min(yp, yn)));
          yMaxLocal = TMath::Max(yMaxLocal, TMath::Max(y, TMath::Max(yp, yn)));
        }

        i++;
      }

      if(tagsElement.getTagBoolDefault("style.shift0",false)){
        double x,y;
        int j=0;
        while(graph->GetPoint(j,x,y) ==j){
          double ynew = y-yMinLocal+0.01; // this +0.01 is just to avoid drawing problems
          graph->SetPoint(j,x,ynew);
          j++;
        }
        yMax = std::max(yMax,yMaxLocal-yMinLocal);
        yMin = std::min(0.,yMin);
      } else {
        yMax = std::max(yMax,yMaxLocal);
        yMin = std::min(yMin,yMinLocal);
      }

      int interpolate;
      if(tagsElement.getTagInteger("style.lininterpolate",interpolate)){
        TQHistogramUtils::interpolateGraph(graph,interpolate,"");
      }
      if(tagsElement.getTagInteger("style.interpolate",interpolate)){
        TQHistogramUtils::interpolateGraph(graph,interpolate,"S");
      }

      Int_t color = tagsElement.getTagIntegerDefault("style.color", 1);

      /* fill properties */
      int fillcolor = tagsElement.getTagIntegerDefault("style.fillColor", color);
      double fillalpha = tagsElement.getTagIntegerDefault("style.fillAlpha", 0.);
      graph->SetFillStyle(tagsElement.getTagIntegerDefault("style.fillStyle", 1001));
      graph->SetLineColorAlpha(fillcolor,fillalpha);
      graph->SetFillColor(fillcolor);

      /* line properties */
      int linecolor = tagsElement.getTagIntegerDefault("style.lineColor", color);
      double linealpha = tagsElement.getTagDoubleDefault("style.lineAlpha",1.);
      double linewidth = tagsElement.getTagDoubleDefault("style.lineWidth", 2.);
      int linestyle = tagsElement.getTagIntegerDefault("style.lineStyle",1);
      graph->SetLineWidth(linewidth);
      graph->SetLineStyle(linestyle);
      graph->SetLineColorAlpha(linecolor,linealpha);

      /* marker properties */
      graph->SetMarkerColor(tagsElement.getTagIntegerDefault("style.markerColor", color));
      graph->SetMarkerStyle(tagsElement.getTagIntegerDefault("style.markerStyle"));
      graph->SetMarkerSize(tagsElement.getTagDoubleDefault("style.markerSize", 2.));

      applyStyle(graph->GetHistogram());

      /* add entry to legend */
      if (legend && tagsElement.getTagBoolDefault("style.addToLegend", true)) {
        legend->AddEntry(graph,
                         tagsElement.getTagStringDefault("style.title", elementName).Prepend(" "),
                         tagsElement.getTagStringDefault("style.legendDrawOptions"));
      }

      TString drawopt = tagsElement.getTagStringDefault("style.drawOption");
      graphsToDraw->Add(graph, drawopt);
      if(drawopt == "E5" && graph->GetFillStyle() > 3000){
        TGraph* other = (TGraph*)(graph->Clone());
        other->SetFillStyle(0);
        graphsToDraw->Add(other,"E5");
      }

      for(auto x:tagsElement.getTagVDouble("action.markX")){
        double y = graph->Eval(x);
        TQFolder* line = input->getFolder(TString::Format("Line.%s.%g+",graph->GetName(),x));
        line->setTagDouble("style.lineWidth",linewidth);
        line->setTagInteger("style.lineStyle",linestyle);
        line->setTagInteger("style.lineColor",linecolor);
        line->setTagDouble("x1",x);
        line->setTagDouble("x2",x);
        line->setTagDouble("y2",y);
        line->setTagString("level","back");
      }
    }
  }

  /* apply a margin on x and y range */
  Double_t xMargin = (xMax - xMin) * 0.1;
  xMin -= xMargin;
  xMax += xMargin;
  if (logScale) {
    yMin /= TMath::Power(yMax / yMin, 0.1);
  } else {
    double d = (yMax - yMin) * 0.1;
    yMin -= d;
    yMax += d;
  }
  xMin = std::max(xMinMin,xMin);
  xMax = std::min(xMaxMax,xMax);
  yMin = std::max(yMinMin,yMin);
  yMax = std::min(yMaxMax,yMax);


  /* loop over overlays */
  TQFolderIterator all(input->getListOfFolders("?"), "!.*", true);
  while (all.hasNext()) {
    TQFolder * element = all.readNext();
    if(!element) continue;
    TString elementName = element->GetName();
    TQTaggable tagsElement;

    /* use style template? */
    TString templateName;
    if (tagsElement.getTagString("template",templateName) && fTemplates) {
      TQFolder * styleTemplate = fTemplates->getFolder(templateName);
      tagsElement.importTags(styleTemplate, false);
    }
    tagsElement.importTags(element);

    if (TQStringUtils::removeLeadingText(elementName, "Line.")) {

      Double_t x1 = tagsElement.getTagDoubleDefault("x1",xMin);
      Double_t y1 = tagsElement.getTagDoubleDefault("y1",yMin);
      Double_t x2 = tagsElement.getTagDoubleDefault("x2",xMax);
      Double_t y2 = tagsElement.getTagDoubleDefault("y2",yMax);

      if (tagsElement.getTagDouble("x", x1)) {
        x2 = x1;
        y1 = yMin;
        y2 = yMax;
      } else if (tagsElement.getTagDouble("y", y1)) {
        y2 = y1;
        x1 = xMin;
        x2 = xMax;
      }

      if ((x1 <= xMax && x1 >= xMin && y1 <= yMax && y1 >= yMin)
          || (x2 <= xMax && x2 >= xMin && y2 <= yMax && y2 >= yMin)) {
        y2 = y2 - input->getTagDoubleDefault("style.breakdown.vertLinesReduceYmax",0);
        TLine * line = new TLine(x1, y1, x2, y2);

        /* line properties */
        Int_t color = tagsElement.getTagIntegerDefault("style.color", 1);
        line->SetLineColor(tagsElement.getTagIntegerDefault("style.lineColor", color));
        line->SetLineWidth(tagsElement.getTagDoubleDefault("style.lineWidth", 2.));
        line->SetLineStyle(tagsElement.getTagIntegerDefault("style.lineStyle"));

        TLatex * lineYLabel = NULL;
        TString lineYLabelStr;
        if (tagsElement.getTagString("yLabel", lineYLabelStr)) {
          lineYLabel = new TLatex(0., y2, lineYLabelStr.Data());
          lineYLabel->SetTextFont(42);
          lineYLabel->SetTextSize(gStyle->GetTextSize() * 0.8);
          lineYLabel->SetTextColor(1);
          lineYLabels->Add(lineYLabel);
        }

        TString level = tagsElement.getTagStringDefault("level", "front");
        if (level.CompareTo("front", TString::kIgnoreCase) == 0) {
          frontElements->Add(line);
        } else if (level.CompareTo("back", TString::kIgnoreCase) == 0) {
          backElements->Add(line);
        }
      }
    } else if (TQStringUtils::removeLeadingText(elementName, "Box.")) {

      Double_t x1 = tagsElement.getTagDoubleDefault("x1",xMin);
      Double_t y1 = tagsElement.getTagDoubleDefault("y1",yMin);
      Double_t x2 = tagsElement.getTagDoubleDefault("x2",xMax);
      Double_t y2 = tagsElement.getTagDoubleDefault("y2",yMax);

      if (tagsElement.getTagDouble("x", x1)) {
        x2 = x1;
        y1 = yMin;
        y2 = yMax;
      } else if (tagsElement.getTagDouble("y", y1)) {
        y2 = y1;
        x1 = xMin;
        x2 = xMax;
      }

      if ((x1 <= xMax && x1 >= xMin && y1 <= yMax && y1 >= yMin)
          || (x2 <= xMax && x2 >= xMin && y2 <= yMax && y2 >= yMin)) {

        double xMinBreakdown = input->getTagDoubleDefault("style.breakdown.xMin", xMin);
        double xMaxBreakdown = input->getTagDoubleDefault("style.breakdown.xMax", xMax);
        x1 = xMinBreakdown;
        x2 = xMaxBreakdown;
        TBox * box = new TBox(x1, y1, x2, y2);

        /* line properties */
        Int_t color = tagsElement.getTagIntegerDefault("style.color", 1);
        box->SetFillColor(color);
        box->SetLineColor(tagsElement.getTagIntegerDefault("style.lineColor", color));
        box->SetLineWidth(tagsElement.getTagDoubleDefault("style.lineWidth", 2.));
        box->SetLineStyle(tagsElement.getTagIntegerDefault("style.lineStyle"));

        TString level = tagsElement.getTagStringDefault("level", "front");
        if (level.CompareTo("front", TString::kIgnoreCase) == 0) {
          frontElements->Add(box);
        } else if (level.CompareTo("back", TString::kIgnoreCase) == 0) {
          backElements->Add(box);
        }
      }
    } else if (TQStringUtils::removeLeadingText(elementName, "Axis.")) {

      Double_t x1 = tagsElement.getTagDoubleDefault("x1");
      Double_t y1 = tagsElement.getTagDoubleDefault("y1");
      Double_t x2 = tagsElement.getTagDoubleDefault("x2");
      Double_t y2 = tagsElement.getTagDoubleDefault("y2");

      Double_t wmin = tagsElement.getTagDoubleDefault("wmin",-1);
      Double_t wmax = tagsElement.getTagDoubleDefault("wmax",1);
      Int_t ndiv = tagsElement.getTagIntegerDefault("ndiv",510);

      if (tagsElement.getTagDouble("x", x1)) {
        x2 = x1;
      } else if (tagsElement.getTagDouble("y", y1)) {
        y2 = y1;
      }

      TString drawOption = tagsElement.getTagStringDefault("style.drawOption","");
      TGaxis * axis = new TGaxis(x1, y1, x2, y2,wmin,wmax,ndiv,drawOption);
      axis->SetNDC(false);
      axis->SetTitle(tagsElement.getTagStringDefault("style.title"));

      /* axis properties */
      Int_t color = tagsElement.getTagIntegerDefault("style.color", 1);
      axis->SetLineColor(tagsElement.getTagIntegerDefault("style.lineColor", color));
      axis->SetLineWidth(tagsElement.getTagDoubleDefault("style.lineWidth", 2.));
      axis->SetLabelSize(tagsElement.getTagDoubleDefault("style.labelSize",0.025));
      axis->SetTitleSize(tagsElement.getTagDoubleDefault("style.textSize",0.025));
      axis->SetTitleOffset(tagsElement.getTagDoubleDefault("style.titleOffset",1));

      TString level = tagsElement.getTagStringDefault("level", "front");
      if (level.CompareTo("front", TString::kIgnoreCase) == 0) {
        frontElements->Add(axis);
      } else if (level.CompareTo("back", TString::kIgnoreCase) == 0) {
        backElements->Add(axis);
      }
    } else if (TQStringUtils::removeLeadingText(elementName, "Text.")) {

      Double_t x,y;
      if(!tagsElement.getTagDouble("x",x)) x = xMax;
      if(!tagsElement.getTagDouble("y",y)) y = yMax;

      if(tagsElement.getTagBoolDefault("restrictX",false)){
        if(x < xMin || x>xMax) continue;
      }
      if(tagsElement.getTagBoolDefault("restrictY",false)){
        if(y < yMin || y>yMax) continue;
      }

      y += tagsElement.getTagDoubleDefault("yShift",0.);
      x += tagsElement.getTagDoubleDefault("xShift",0.);

      TString s = tagsElement.getTagStringDefault("text");
      TLatex * text = new TLatex(x, y, s.Data());
      text->SetTextSize(tagsElement.getTagDoubleDefault("style.textSize", gStyle->GetTextSize()));
      text->SetTextAngle(tagsElement.getTagIntegerDefault("style.textAngle", 0));
      text->SetTextColor(tagsElement.getTagIntegerDefault("style.textColor", kBlack));

      TString level = tagsElement.getTagStringDefault("level", "front");
      if (level.CompareTo("front", TString::kIgnoreCase) == 0) {
        frontElements->Add(text);
      } else if (level.CompareTo("back", TString::kIgnoreCase) == 0) {
        backElements->Add(text);
      }
    }

  }

  Double_t x = input->getTagDoubleDefault("labels.x",0.2);
  Double_t y = input->getTagDoubleDefault("labels.y",0.86);

  /* prepare additional labels */
  TString strLabel;
  Int_t iLabel = 0;
  while (input->getTagString(TString::Format("labels.%d", iLabel++), strLabel)) {
    TLatex * label = new TLatex(x, y - 0.07 * (Double_t)(iLabel), strLabel.Data());
    label->SetNDC();
    label->SetTextFont(42);
    label->SetTextSize(gStyle->GetTextSize() * 0.85);
    label->SetTextColor(1);
    frontElements->Add(label);
  }

  /* determine y range to avoid collisions */
  double yOffset = input->getTagDoubleDefault("style.breakdown.yUpperOffset", 0.07);
  Double_t vetoFrac = input->getTagDoubleDefault("style.allowVetoFrac",true) ? y - yOffset * (Double_t)(iLabel) : 1.0;
  yMax = getMaxRange(yMin, yMax, vetoFrac, logScale);


  /* create the canvas */
  double height = input->getTagIntegerDefault("style.height",768);
  double width = input->getTagIntegerDefault("style.width",1024);
  TCanvas * canvas = new TCanvas("canvas", "canvas", width, height);


  canvas->SetMargin(input->getTagDoubleDefault("style.margin.left",0.1),
                    input->getTagDoubleDefault("style.margin.right",0.1),
                    input->getTagDoubleDefault("style.margin.bottom",0.1),
                    input->getTagDoubleDefault("style.margin.top",0.1));

  if(legend){
    /* adapt size of legend according to number of overlays */
    if (legend->GetListOfPrimitives()) {
      legend->GetListOfPrimitives()->GetEntries();
    }
    legend->SetFillStyle(0);
    legend->SetX1NDC(input->getTagDoubleDefault("style.legend.x1",0.65));
    legend->SetY1NDC(input->getTagDoubleDefault("style.legend.y1",0.62));
    legend->SetX2NDC(input->getTagDoubleDefault("style.legend.x2",0.9));
    legend->SetY2NDC(input->getTagDoubleDefault("style.legend.y2",0.9));
    legend->SetEntrySeparation(legend->GetEntrySeparation()*input->getTagDoubleDefault("style.legend.scaleSeparation",1.));
  }

  /* ===== draw graphs ===== */

  Bool_t firstGraph = true;
  TObjLink * lnk = graphsToDraw->FirstLink();
  while (lnk) {
    /* get this graph */
    TGraph * graph = dynamic_cast<TGraph*>(lnk->GetObject());
    TGraph2D* graph2D = dynamic_cast<TGraph2D*>(lnk->GetObject());
    /* draw graph */
    if (firstGraph) {
      if(graph)        configureFirstGraph(graph,  input,xMin,xMax,yMin,yMax,width,height,lnk->GetOption());
      else if(graph2D) configureFirstGraph(graph2D,input,xMin,xMax,yMin,yMax,width,height,lnk->GetOption());

      /* ===== draw back elements ===== */
      TObjLink * bkglnk = backElements->FirstLink();
      while (bkglnk) {
        bkglnk->GetObject()->Draw(bkglnk->GetOption());
        bkglnk = bkglnk->Next();
      };        
      delete backElements;

      canvas->RedrawAxis();

      /* redraw first graph */
      if(graph)        graph->Draw(TString(lnk->GetOption()));
      else if(graph2D) graph2D->Draw(TString(lnk->GetOption()));

    } else {
      if(graph)        graph->Draw(TString("SAME") + TString(lnk->GetOption()));
      else if(graph2D) graph2D->Draw(TString("SAME") + TString(lnk->GetOption()));
    }

    /* get next graph */
    lnk = lnk->Next();
    firstGraph = false;
  }
  delete graphsToDraw;


  /* ===== draw legend ===== */

  if(legend){
    legend->SetBorderSize(0);
    legend->SetTextSize(input->getTagDoubleDefault("style.legend.textSize",gStyle->GetTextSize() * 0.8));
    legend->SetFillColor(0);
    legend->Draw("same");
  }

  if (logScale) {
    canvas->SetLogy();
    canvas->RedrawAxis();
    canvas->Update();
  }

  /* ===== draw front elements ===== */

  frontElements->Draw();
  delete frontElements;


  /* ===== draw labels ===== */

  /* draw the ATLAS label */
  if (input->getTagBoolDefault("labels.drawAtlas", true)) {
    TLatex * l = new TLatex();
    l->SetNDC();
    l->SetTextFont(72);
    l->SetTextSize(gStyle->GetTextSize() * 1.25);
    l->SetTextColor(1);
    l->DrawLatex(x, y, "ATLAS");
  }

  /* draw the ATLAS label addition */
  TString atlasLabel = "Private";
  input->getTagString("labels.atlas", atlasLabel);
  if (atlasLabel.Length() > 0) {
    TLatex * p = new TLatex(x + input->getTagDoubleDefault("labels.atlas.xOffset",0.23), y, atlasLabel.Data());
    p->SetNDC();
    p->SetTextFont(42);
    p->SetTextSize(gStyle->GetTextSize() * 1.25);
    p->SetTextColor(1);
    p->Draw();
  }

  /* draw info label */
  TString infoLabel;
  if (input->getTagString("labels.info", infoLabel)) {
    TLatex * l0 = new TLatex(0., 1. - 0.04, infoLabel.Data());
    l0->SetNDC();
    l0->SetTextFont(42);
    l0->SetTextSize(gStyle->GetTextSize() * 0.6);
    l0->SetTextColor(1);
    l0->SetX(1. - canvas->GetRightMargin() - l0->GetXsize());
    l0->Draw();
  }

  TQIterator itrLabels(lineYLabels);
  while (itrLabels.hasNext()) {
    TLatex * label = (TLatex*)itrLabels.readNext();
    label->SetX(xMax + (xMax - xMin) * 0.01);
    label->Draw();
  }
  delete lineYLabels;


  /* finally force canvas to redraw itself */
  canvas->Update();

  /* return cancas */
  return canvas;
}


//__________________________________________________________________________________|___________

Double_t TSStatisticsPlotter::getMaxRange(Double_t min,
                                          Double_t max, Double_t vetoFrac, Bool_t logScale) {

  if (logScale)
    return TMath::Power(max * TMath::Power(min, vetoFrac - 1), (1. / vetoFrac));
  else
    return max / vetoFrac + min * (1 - 1 / vetoFrac);
}


//__________________________________________________________________________________|___________

void TSStatisticsPlotter::applyStyle(TH1 * /*h*/) {
  // CURRENTLY NOT IMPLEMENTED

}


//__________________________________________________________________________________|___________

void TSStatisticsPlotter::setStyleAtlas() {

  Int_t icol = 0;
  gStyle->SetFrameBorderMode(icol);
  gStyle->SetFrameFillColor(icol);
  gStyle->SetCanvasBorderMode(icol);
  gStyle->SetCanvasColor(icol);
  gStyle->SetPadBorderMode(icol);
  gStyle->SetPadColor(icol);
  gStyle->SetStatColor(icol);
  //gStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

  // set the paper & margin sizes
  gStyle->SetPaperSize(20,26);

  // set margin sizes
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  gStyle->SetTitleXOffset(1.4);
  gStyle->SetTitleYOffset(1.4);

  // use large fonts
  //Int_t font=72; // Helvetica italics
  Int_t font=42; // Helvetica
  Double_t tsize=0.05;
  gStyle->SetTextFont(font);

  gStyle->SetTextSize(tsize);
  gStyle->SetLabelFont(font,"x");
  gStyle->SetTitleFont(font,"x");
  gStyle->SetLabelFont(font,"y");
  gStyle->SetTitleFont(font,"y");
  gStyle->SetLabelFont(font,"z");
  gStyle->SetTitleFont(font,"z");

  gStyle->SetLabelSize(tsize,"x");
  gStyle->SetTitleSize(tsize,"x");
  gStyle->SetLabelSize(tsize,"y");
  gStyle->SetTitleSize(tsize,"y");
  gStyle->SetLabelSize(tsize,"z");
  gStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);
  gStyle->SetHistLineWidth((Width_t)2.);
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars
  //gStyle->SetErrorX(0.001);
  // get rid of error bar caps
  //gStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  gStyle->SetOptTitle(0);
  //gStyle->SetOptStat(1111);
  gStyle->SetOptStat(0);
  //gStyle->SetOptFit(1111);
  gStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

}


//__________________________________________________________________________________|___________

TSStatisticsPlotter::~TSStatisticsPlotter() {
}

