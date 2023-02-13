#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

#include <algorithm>

#include "QFramework/TQIterator.h"
#include "QFramework/TQTikZPlotter.h"
#include "QFramework/TQUtils.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQTikZPlotter:
//
// The TQTikZPlotter is a plotter implemention utilizing TikZ/PGFplots instead of ROOT
//
////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(TQTikZPlotter)

//______________________________________________________________________________________________

bool TQTikZPlotter::plotAndSaveAsInternal(const TString& histogram, const TString& saveAs, TQTaggable& tags) {
  // plot and save the given histogram using the given tags
  // the tags are forwarded to and interpreted by the makeplot function

  //@tags: printProcesses: print processes to the console before plotting
  if(tags.getTagBoolDefault("printProcesses",false)){
    this->printProcesses();
  }

  //@tags: useNamePrefix: prefix histogram names with the variable names. will not affect the look of the plot, but possibly required for elaborate plots to be saved in .C format
  if(tags.getTagBoolDefault("useNamePrefix",true)){
    TString tmp(histogram);
    TString prefix = TQFolder::getPathTail(tmp);
    TQStringUtils::ensureTrailingText(prefix,".");
    tags.importTagsWithoutPrefix(tags,prefix);
  }
  tags.importTags(this);
  if(histogram.Contains("=")){
    tags.importTagsWithPrefix(histogram,"input");
  } else {
    tags.setTagString("input.histogram",histogram);
  }
  
  bool success = false;
  try {
    std::ofstream os(saveAs.Data());
    if(os.good()){
      success = makePlot(os,tags);
    }
    os.close();
  } catch (const std::exception& err){
    std::cout << err.what() << std::endl;
  }

  if(tags.getTagBoolDefault("deleteObjects",true)){
    //@tags: deleteObjects: control whether plotting objects will be kept in memory after plotting (default: false for plotAndSaveAs, true for plot)
    this->deleteObjects();
  } 
  return success;
}

//______________________________________________________________________________________________

void TQTikZPlotter::histogramToTikZCoordinatesMC(std::ostream& os, TH1* hist){
  // output an MC const plot
  unsigned nbins = hist->GetNbinsX()+1;
  for(size_t i=1; i<nbins; ++i){
    os << "    ( " << hist->GetXaxis()->GetBinLowEdge(i) << " , " << hist->GetBinContent(i) << " )\n"; 
  }
  os << "    ( " << hist->GetXaxis()->GetBinLowEdge(nbins) << " , " << hist->GetBinContent(nbins-1) << " )\n"; 
}
void TQTikZPlotter::histogramErrorsToTikZCoordinates(std::ostream& os, TH1* hist, bool up){
  // output the +1sigma / -1sigma variation of an MC const plot
  unsigned nbins = hist->GetNbinsX()+1;
  for(size_t i=1; i<nbins; ++i){
    os << "    ( " << hist->GetXaxis()->GetBinLowEdge(i) << " , " << hist->GetBinContent(i) + (up ? hist->GetBinError(i) : -hist->GetBinError(i)) << " )\n"; 
  }
  os << "    ( " << hist->GetXaxis()->GetBinLowEdge(nbins) << " , " << hist->GetBinContent(nbins-1) + (up ? hist->GetBinError(nbins-1) : -hist->GetBinError(nbins-1)) << " )\n"; 
}

void TQTikZPlotter::histogramToTikZCoordinatesData(std::ostream& os, TH1* hist){
  // output a data plot with error bars
  unsigned nbins = hist->GetNbinsX()+1;
  for(size_t i=1; i<nbins; ++i){
    os << "    ( " << hist->GetXaxis()->GetBinCenter(i) << " , " << hist->GetBinContent(i) << " ) +- ( " << 0.5*(hist->GetXaxis()->GetBinWidth(i)) << " , " << hist->GetBinError(i) << " )\n"; 
  }
}

//______________________________________________________________________________________________

bool TQTikZPlotter::makePlot(std::ostream& output, TQTaggable& tags){
  // master-function controlling the plotting

  // read a few tags
  // bool verbose = tags.getTagBoolDefault("verbose",false); // BW: hashed (unused variable)
  bool standalone  = tags.getTagBoolDefault("standalone",true);
  bool showSub = false;
  bool showRatio = tags.getTagBoolDefault ("style.showRatio",false);
  bool showDminusB = tags.getTagBoolDefault ("style.showDminusB",false);
  bool showOptScan = tags.getTagBoolDefault ("style.showOptScan",false);
  if(showRatio || showDminusB || showOptScan){
    showSub = true;
  }
  bool showErrorEnds = tags.getTagBoolDefault("style.showErrorEnds",false);
  
  // collect the objects
  //TObjArray* histos = this->collectHistograms(tags); // BW: hashed (unused variable)
  TH1* hMaster = this->getObject<TH1>("Graph_master");
  if(!hMaster) return false;
  this->setAxisLabels(tags);

  // create header
  if(standalone) output << "\\documentclass[tikz,border=3pt]{standalone}\n";
  output << "%%% this plot was automatically generated on " << TQUtils::getTimeStamp() << " using CAFCore\n";
  if(!standalone) output << "% add this to your document preamble\n";
  // packages and libraries
  output << (standalone ? "" : "%") << "\\usepackage{pgfplots}\n";
  output << (standalone ? "" : "%") << "\\usepackage{sansmath}\n";
  output << (standalone ? "" : "%") << "\\usepgfplotslibrary{fillbetween}\n";
  output << (standalone ? "" : "%") << "\\usetikzlibrary{patterns}\n";
  // plot styles
  output << (standalone ? "" : "%") << "\\tikzset{datapoints/.style={draw=black,solid,/pgfplots/.cd,only marks,error bars/y dir = both,error bars/y explicit,";
  if(!showErrorEnds)  output << "error bars/error mark options = {rotate=90,mark size=0pt}}}";
  output << "\n";
  output << (standalone ? "" : "%") << "\\tikzset{mcstack/.style={/pgfplots/.cd,stack plots=y,const plot, no marks}}\n";
  output << (standalone ? "" : "%") << "\\tikzset{mcnostack/.style={/pgfplots/.cd,const plot, no marks}}\n";
  // legend entry styles
  output << (standalone ? "" : "%") << "\\pgfplotsset{/pgfplots/fillbox/.style={legend image code/.code={\\fill[draw=none,no markers] (.0em,-.5em) rectangle (1.7em,.5em); } } }\n";
  output << (standalone ? "" : "%") << "\\pgfplotsset{/pgfplots/fillboxline/.style={legend image code/.code={\\draw[draw=none,no markers] (.0em,-.5em) rectangle (1.7em,.5em); \\draw[fill=none] (0.0em,0.0em) -- (1.7em,0.0em); } } }\n";
  output << (standalone ? "" : "%") << "\\pgfplotsset{/pgfplots/datapoint/.style={legend image code/.code={\\draw";
  if(showErrorEnds) output << "[|-|]";
  output << " (.85em,.5em) -- (.85em,-.5em) node[pos=0.5,anchor=center,circle,fill,inner sep=1pt] {} ; } } }\n";
  // begin the document
  if(standalone) output <<"\\begin{document}\n";

  // obtain the geometry
  int width  = tags.getTagIntegerDefault("geometry.canvas.width",400);
  int height = tags.getTagIntegerDefault("geometry.canvas.height",300);
  double subpadRatio = tags.getTagDoubleDefault("geometry.sub.height",0.35);

  // initialize the picture environment
  output << "\\begin{tikzpicture}[font={\\fontfamily{qhv}\\selectfont},fill between/on layer={main}]\n";
  output << "\\pgfplotsset{axis on top}\n";
  output << "\\sansmath\n";

  // collect the styles and write out the color definitions
  makeStyles(tags,output);
  output << TQStringUtils::getColorDefStringLaTeX("mainErrorBand",14) << "\n";
  output << TQStringUtils::getColorDefStringLaTeX("subErrorBand",kOrange-2) << "\n";
  
  // create the axis of the main pad
  output << "\\begin{axis}[name=main,height=" << height << "pt,width=" << width << "pt,ymin=0,enlarge x limits=false,clip=false,\n";
  output << "    domain=" << hMaster->GetXaxis()->GetXmin() << ":" << hMaster->GetXaxis()->GetXmax() << ",\n";
  output << "    legend style={legend columns=" << tags.getTagIntegerDefault ("style.nLegendCols",1) << ",fill=none,draw=none},legend cell align={left},\n";
  if(!showSub) output << "    xlabel={"<<hMaster->GetXaxis()->GetTitle()<<"},x label style={at={(axis description cs:1.0,-0.01)},anchor=north east},\n";
  else         output << "    xticklabels={},\n";
  output << "    ylabel={"<<hMaster->GetYaxis()->GetTitle()<<"},y label style={at={(axis description cs:0.00,1.0)},anchor=south east},\n";
  output << "]\n";
  
  // draw the contents of the main pad
  drawStack(tags,output);

  // draw the labels
  drawLabels(tags,output);

  // close the main pad
  output << "\\end{axis}\n";

  if(showSub){
    // special if for optimization scans
    TQHistogramUtils::FOM FOMmode = TQHistogramUtils::kUndefined;
    //@tag:optScan.FOMmode: figure of merit to be used. currently available are: s/sqrt(s+b),s/b,poisson,s/sqrt(b),s/sqrt(b+db2)
    //@tag:style.FOMmode: deprecated, use optScan.FOMmode
    if(showOptScan){
      TString fommodestr = tags.getTagStringDefault("optScan.FOMmode",tags.getTagStringDefault ("style.FOMmode","s/sqrt(b)"));
      FOMmode = TQHistogramUtils::readFOM(fommodestr);
      if(FOMmode == TQHistogramUtils::kUndefined){
        WARNclass("unknown figure of merit '%s'!",fommodestr.Data());
        showOptScan = false;
      }
    }

    // create the axis of the sub pad
    output << "\\begin{axis}[name=sub,at=(main.south west), anchor=north west,height=" << subpadRatio*height << "pt,width=" << width << "pt,enlarge x limits=false,clip=false,\n";
    output << "    domain=" << hMaster->GetXaxis()->GetXmin() << ":" << hMaster->GetXaxis()->GetXmax() << ",\n";
    if(showOptScan)   output << "    legend style={legend columns=1,fill=none,draw=none},legend cell align={left},\n";
    output << "    xlabel={"<<hMaster->GetXaxis()->GetTitle()<<"},x label style={at={(axis description cs:1.0,-0.01)},anchor=north east},\n";
    output << "    ylabel={";
    // choose the label dynamically
    if(showRatio)        output << "Data/SM";
    else if(showDminusB) output << "Data-Bkg.";
    else if(showOptScan) output << TQHistogramUtils::getFOMTitleLaTeX(FOMmode);
    output <<"},y label style={at={(axis description cs:0.00,1.0)},anchor=south east},\n";
    output << "]\n";
    
    // draw the contents of the sub pad
    if     (showRatio)   drawRatio  (tags,output);
    else if(showDminusB) drawDminusB(tags,output);
    else if(showOptScan) drawOptScan(tags,output,FOMmode);

    // close the sub pad
    output << "\\end{axis}\n";
  }
  
  // finalize the picture
  output << "\\end{tikzpicture}\n";
  if(standalone){
    output << "\\end{document}\n";
  }

  // cleanup temporary objects
  this->f_stack.clear();
  this->f_data.clear();
  this->f_signal.clear();

  return true;
}

//______________________________________________________________________________________________

void TQTikZPlotter::makeStyles(TQTaggable& tags,std::ostream& output){
  // collect all the styles and definitions
  TH1* hMaster = this->getObject<TH1>("Graph_master");

  // the total stack and error band histograms
  TH1* hTotalStack = TQHistogramUtils::copyHistogram(hMaster,"totalStack");
  TH1* hTotalBkg = TQHistogramUtils::copyHistogram(hMaster,"totalBkg");
  TString bandStyle = "color=mainErrorBand";

  bandStyle.Append(",pattern=");  
  int pattern = tags.getTagIntegerDefault("style.main.totalStackError.fillStyle",3254);
  switch(pattern % 100){
  case  5: bandStyle.Append("horizontal lines"); break;
  case 50: bandStyle.Append("vertical lines"); break;
  case 44: bandStyle.Append("crosshatch"); break;
  case 54: bandStyle.Append("north west lines"); break;
  case 45: bandStyle.Append("north east lines"); break;
  default: 
    WARNclass("pattern not implemented: %d",pattern);
    bandStyle.Append("none");
  }

  bandStyle.Append(",draw=");
  int color = tags.setTagInteger("style.main.totalStack.lineColor",kBlue);
  if(color == kWhite){
    bandStyle.Append("none");
  } else if(color == kBlue){
    bandStyle.Append("blue");;
  } else if(color == kBlack){
    bandStyle.Append("black");
  } else {
    output << TQStringUtils::getColorDefStringLaTeX("drawtotalStack",color) << "\n";
    bandStyle.Append("drawtotalStack");
  }
  bandStyle.Append(",thick,");
  
  f_styles[hTotalStack] = bandStyle;
  f_styles[hTotalBkg]   = bandStyle;


  TQTaggableIterator itr(this->fProcesses);
  while(itr.hasNext()){
    // loop over all processes
    TQNamedTaggable* process = itr.readNext();
    if(!process) continue;
    bool isBkg = process->getTagBoolDefault(".isBackground",false);
    bool isSig =  process->getTagBoolDefault(".isSignal",false);
    // bool isData =  process->getTagBoolDefault(".isData",false); // BW: hashed (unused variable)
    if(process->getTagBoolDefault("stack", isBkg) || (isSig && tags.getTagBoolDefault ("style.autoStackSignal",false))){
      TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
      if (!h) continue;
      // if this is a background or otherwise stacked
      // this is an MC histogram, collect draw and fill color
      TString processStyle;
      TString processName = process->getTagStringDefault(".name",process->GetName());
      if(h->GetFillStyle() == 1001){
        int fillColor = h->GetFillColor();
        TString fillColorName = processName;
        fillColorName.Prepend("fill");
        TString fillColorDef = TQStringUtils::getColorDefStringLaTeX(fillColorName,fillColor);
        output << fillColorDef << "\n";
        processStyle.Append("fill=");
        processStyle.Append(fillColorName);
        processStyle.Append(",");
      } else {
        processStyle.Append("fill=none,");
      }
      int drawColor = h->GetLineColor();
      if(drawColor != kBlack){
        TString drawColorName = processName;
        drawColorName.Prepend("draw");
        TString drawColorDef = TQStringUtils::getColorDefStringLaTeX(drawColorName,drawColor);
        output << drawColorDef << "\n";
        processStyle.Append("draw=");
        processStyle.Append(drawColorName);
        processStyle.Append(",");
      } else {
        processStyle.Append("draw=black,");
      }
      f_stack.push_back(h);
      f_styles[h] = processStyle;
      hTotalStack->Add(h);
      if(isBkg) hTotalBkg->Add(h);
    } else if (process->getTagBoolDefault(".isSignal",false)){
      TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
      if(!h) continue;
      // if this is a signal
      // this is an MC histogram, collect draw and fill color
      TString sigStyle;
      int drawcolor = h->GetLineColor();
      if(drawcolor != kBlack){
        TString drawColorName = tags.getTagStringDefault(".name",TQFolder::makeValidIdentifier(process->GetName()));
        drawColorName.Prepend("draw");
        TString style = "draw=";
        style.Append(drawColorName);
        style.Append(",");
        TString drawColorDef = TQStringUtils::getColorDefStringLaTeX(drawColorName,drawcolor);
        output << drawColorDef << "\n";
        sigStyle.Append(style);
      } else {
        sigStyle.Append("draw=black,");
      }
      if(h->GetFillStyle() == 1001){
        int fillcolor = h->GetFillColor();
        TString drawColorName = tags.getTagStringDefault(".name",TQFolder::makeValidIdentifier(process->GetName()));
        drawColorName.Prepend("fill");
        TString style = "fill=";
        style.Append(drawColorName);
        style.Append(",");
        TString drawColorDef = TQStringUtils::getColorDefStringLaTeX(drawColorName,fillcolor);
        output << drawColorDef << "\n";
        sigStyle.Append(style);
      } else {
        sigStyle.Append("fill=none,");
      }
      f_signal.push_back(h);
      f_styles[h] = sigStyle;
      if(tags.getTagBoolDefault("style.stackSignal")) hTotalStack->Add(h);
    } else if(process->getTagBoolDefault(".isData",false)){
      TH1 * h = this->getObject<TH1>(this->makeHistogramIdentifier(process));
      f_data.push_back(h);
      if(!h) continue;
      this->applyStyle(tags,h,"main.data");
      // if this is a data histogram
      // we only care for the marker color
      int color = h->GetMarkerColor();
      TString style;
      if(color != kBlack){
        TString drawColorName = tags.getTagStringDefault(".name",TQFolder::makeValidIdentifier(process->GetName()));
        drawColorName.Prepend("mark");
        TString style = "draw=";
        style.Append(drawColorName);
        style.Append(",");
        style.Append("mark=*,");
        style.Append("mark options={fill=");
        style.Append(drawColorName);
        style.Append("},");
        style.Append("error bars/error mark options/.append style={solid,draw=");
        style.Append(drawColorName);
        style.Append("},");
        TString drawColorDef = TQStringUtils::getColorDefStringLaTeX(drawColorName,color);
        output << drawColorDef << "\n";
      } else {
        style = "draw=black,mark=*,mark options={fill=black},error bars/error mark options/.append style={solid,draw=black},";
      }
      f_styles[h] = style;
    }
  }
}

//______________________________________________________________________________________________

void TQTikZPlotter::drawStack(TQTaggable& tags,std::ostream& output){
  // draw the stack on the main pad
  TH1* hMaster = this->getObject<TH1>("Graph_master");
  TH1* hTotalStack = this->getObject<TH1>("totalStack");
  if(!hMaster) return;
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // preparations 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // set the errors
  if(tags.hasTag("errors.showSys")){
    this->includeSystematics(tags);
  }
  this->setErrors(tags,tags.getTagStringDefault("errors.source","totalBkg"));

  std::vector<TH1*> v_stack(f_stack);
  // sort the histograms to be stacked by ascending integral (sum of weights)
  if (!tags.getTagBoolDefault("style.manualStacking",false) && tags.getTagBoolDefault("style.autoStack",true)){
    std::sort(v_stack.begin(),v_stack.end(),[&](TH1* a, TH1* b){ return a->GetSumOfWeights() <= b->GetSumOfWeights();});
  }
  bool reverseStacking = tags.getTagBoolDefault ("style.reverseStacking",false );

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // legend
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // add the data to the legend
  std::vector<std::pair<TString,TString> > legend;
  for (size_t iHist = 0; iHist < f_data.size() ; iHist++){
    TH1* h = f_data[iHist];
    TString style = f_styles[h];
    legend.push_back(std::make_pair("datapoint,"+style,h->GetTitle()));
  }
  
  // add the error band to the legend
  bool statMcErrors = tags.getTagBoolDefault("errors.drawStatMC",true );
  bool sysMcErrors = tags.getTagBoolDefault("errors.drawSysMC",false ) || tags.hasTag("errors.showSys");
  TString legendTotalBkgLabel = tags.getTagStringDefault ("labels.totalStack", "SM");
  legendTotalBkgLabel = " " + tags.getTagStringDefault ("labels.legendTotalBkg", legendTotalBkgLabel); // overwrite if you explicitly want it different in legend
  if(tags.getTagBoolDefault("legend.showTotalBkgErrorType",true)){
    if (statMcErrors && sysMcErrors)
      legendTotalBkgLabel.Append(" (sys $\\oplus$ stat)");
    else if (sysMcErrors)
      legendTotalBkgLabel.Append(" (sys)");
    else if (statMcErrors)
      legendTotalBkgLabel.Append(" (stat)");
  }
  legend.push_back(std::make_pair(TString(tags.setTagInteger("style.main.totalStack.lineColor",kBlue) == kWhite ? "fillbox" : "fillboxline")+","+f_styles[hTotalStack],legendTotalBkgLabel));
  
  // add the stack to the legend
  for (int iHist = reverseStacking ? (int) v_stack.size() : 0; reverseStacking ? (iHist >= 0) : (iHist < (int) v_stack.size()) ; reverseStacking ? iHist-- : iHist++){
    TH1* h = dynamic_cast<TH1*>(v_stack.at(iHist));
    TString style = f_styles[h];
    legend.push_back(std::make_pair("fillbox,"+style,h->GetTitle()));
  } 

  // add the signal to the legend
  for (size_t iHist = 0; iHist < f_signal.size() ; iHist++){
    TH1* h = f_signal[iHist];
    TString style = f_styles[h];
    legend.push_back(std::make_pair("fillbox,"+style,h->GetTitle()));
  }

  // output all legend entries
  for(const auto& entry:legend){
    output << "\\addlegendimage{" << entry.first << "}\n";
    output << "\\addlegendentry{" << entry.second << "}\n";
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // draw stack, signal and data
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // draw stack
  for (int iHist = reverseStacking ? (int) v_stack.size() : 0; reverseStacking ? (iHist >= 0) : (iHist < (int) v_stack.size()) ; reverseStacking ? iHist-- : iHist++){
    TH1* h = v_stack[iHist];
    TString style = f_styles[h];
    output << "\\addplot+[mcstack," << style <<"] coordinates {\n";
    histogramToTikZCoordinatesMC(output,h);
    output << "}\\closedcycle;\n";
  } 
  // draw signal
  for (size_t iHist = 0; iHist < f_signal.size() ; iHist++){
    TH1* h = f_signal[iHist];
    TString style = f_styles[h];
    output << "\\addplot[";
    if(tags.getTagBoolDefault("style.stackSignal",false)){
      output << "mcstack";
    } else {
      output << "mcnostack";
    }
    output << "," << style <<"] coordinates {\n";
    histogramToTikZCoordinatesMC(output,h);
    output << "}\\closedcycle;\n";
  }

  // draw error band
  output << "\\addplot[const plot,draw=none,fill=none,name path=lower] coordinates {\n";
  histogramErrorsToTikZCoordinates(output,hTotalStack,false);
  output << "};\n";
  output << "\\addplot[const plot,draw=none,fill=none,name path=upper] coordinates {\n";
  histogramErrorsToTikZCoordinates(output,hTotalStack,true);
  output << "};\n";
  output << "\\addplot[const plot," << f_styles[hTotalStack] << ",draw=none] fill between [of=lower and upper];\n";
  bool showTotalBkg = tags.getTagBoolDefault ("style.showTotalBkg",true);
  if(showTotalBkg) {
    output << "\\addplot[const plot," << f_styles[hTotalStack] << ",fill=none] coordinates {\n";
    histogramToTikZCoordinatesMC(output,hTotalStack);
    output << "};\n";
  }
  
  // draw data
  for (size_t iHist = 0; iHist < f_data.size() ; iHist++){
    TH1* h = f_data[iHist];
    TString style = f_styles[h];
    output << "\\addplot[datapoints," << style <<"] coordinates {\n";
    histogramToTikZCoordinatesData(output,h);
    output << "}\\closedcycle;\n";
  }

}

//______________________________________________________________________________________________

void TQTikZPlotter::drawLabels(TQTaggable& tags,std::ostream& output){
  // draw the labels given by the tags
  bool drawlabels = tags.getTagBoolDefault("style.showLabels",true);

  // locations of the labels
  double x = tags.getTagDoubleDefault("style.labels.xOffset",0.05);
  double y = tags.getTagDoubleDefault("style.labels.yPos",0.95);
 
  // the ATLAS labels
  //@tag:labels.drawATLAS: decide whether to draw the 'ATLAS' label
  //@tag:labels.atlas: which ATLAS label to use (Private, work in progress, Internal, Preliminary, ... - default:'Private')
  //@tag:labels.atlas.xOffset : horizontal offset between ATLAS label and its addition. (default: 0.16)
  bool drawATLAS = tags.getTagBoolDefault ("labels.drawATLAS",drawlabels);
  if (drawATLAS) {
    output << "\\node[anchor = west, font={\\fontfamily{phv}\\fontseries{b}\\selectfont}] at (axis description cs:" << x << "," << y << ") {" << tags.getTagStringDefault("labels.drawATLAS.text","ATLAS") << "};\n";
  }
  TString atlasLabel = tags.getTagStringDefault ("labels.atlas","Private");
  if (drawATLAS && !atlasLabel.IsNull()){
    output << "\\node[anchor = west] at (axis description cs:" << x + tags.getTagDoubleDefault("labels.atlas.xOffset",0.16) << "," << y << ") {" << atlasLabel << "};\n";
  }

  // collect the NF information
  TString nfLabel = "";
  //@tag:labels.drawNFInfo: decide whether to draw information on which NFs were applied
  if(tags.getTagBoolDefault ("labels.drawNFInfo",false)){
    TString tmpLabel = tags.getTagStringDefault("labels.nfInfo","\\textcolor{red}{(NF applied for %s)}");
    TString nflist = this->getScaleFactorList(tags.getTagStringDefault("input.bkg",""));
    if(!nflist.IsNull()){
      nfLabel = TString::Format(tmpLabel.Data(),nflist.Data());
    }
  }

  // draw the info label 
  //@tag:labels.drawInfo: decide whether to draw the technical info tag on the top right of the plot
  TString infoLabel = tags.getTagBoolDefault ("labels.drawInfo",drawlabels) ? tags.getTagStringDefault ("labels.info",TString::Format("Plot: \"%s\"", tags.getTagStringDefault("input.name","histogram").Data())) : ""; 
  if (!infoLabel.IsNull()){
    // draw the info label
    if(!nfLabel.IsNull()){
      infoLabel.Prepend(" ");
      infoLabel.Prepend(nfLabel);
    }
    output << "\\node[anchor = south east] at (axis description cs:1,1) {" << infoLabel.Data() << "};\n";    
  }
  
  // draw all remaining lables
  double marginStep = tags.getTagDoubleDefault("style.labels.marginStep",0.06);
  // double labelTextScale = tags.getTagDoubleDefault("style.labels.scale",0.85); // BW: hashed (unused variable)
  if(drawlabels){
    TQIterator itr(tags.getTagList("labels"),true);
    size_t index = 1;
    while(itr.hasNext()){
      TObject* obj = itr.readNext();
      if(!obj) break;
      output << "\\node[anchor = west] at (axis description cs:" << x << "," << y - marginStep * index << ") {" << TQStringUtils::convertROOTTeX2LaTeX(obj->GetName()) << "};\n";
      index++;
    }
  }
}

//______________________________________________________________________________________________

void TQTikZPlotter::drawRatio  (TQTaggable& tags,std::ostream& output){
  // draw a ratio-plot in the sub-pad

  bool invertRatio = tags.getTagBoolDefault("style.invertRatio",false);
  TString totalStackLabel = tags.getTagStringDefault ("labels.totalStack", "SM");
  // get the denominator
  TString ratioDenominator = tags.getTagStringDefault("style.ratio.denominator","totalStack");
  TH1* denominator = this->getObject<TH1>(ratioDenominator);
  if(!denominator) return;

  // get the error band
  TH1* error = TQHistogramUtils::copyHistogram(denominator,"ratio_error");
  TQHistogramUtils::divideHistogramWithoutError(error,denominator);
  output << "\\addplot[const plot,draw=none,fill=none,name path=ratiolower] coordinates {\n";
  histogramErrorsToTikZCoordinates(output,error,false);
  output << "};\n";
  output << "\\addplot[const plot,draw=none,fill=none,name path=ratioupper] coordinates {\n";
  histogramErrorsToTikZCoordinates(output,error,true);
  output << "};\n";
  output << "\\addplot[const plot,draw=none,fill=subErrorBand] fill between [of=ratiolower and ratioupper];\n";
  output << "\\addplot[draw=red] {1};\n";
  output << "\\addplot[draw=black,dashed] {1.05};\n";
  output << "\\addplot[draw=black,dashed] {0.95};\n";

  // get the numerator(s)
  TString dataLabel("");
  tags.getTagString("labels.numerator",dataLabel);
  for(size_t i=0; i<f_data.size(); ++i){
    TH1 * h_data = f_data[i];
    // get style and label
    TString style = f_styles[h_data];
    if(dataLabel.IsNull()) dataLabel = h_data->GetTitle();
    tags.getTagString("labels.data",dataLabel);
    
    // calculate the ratio
    TH1* h_ratio = TQHistogramUtils::copyHistogram(h_data,TString::Format("%s_ratio",h_data->GetName()));
    TQHistogramUtils::divideHistogramWithoutError(h_ratio,denominator);
    if(invertRatio) TQHistogramUtils::power(h_ratio,-1);
    
    // plot the ratio
    output << "\\addplot[datapoints," << style <<"] coordinates {\n";
    histogramToTikZCoordinatesData(output,h_ratio);
    output << "}\\closedcycle;\n";
  }
}
 
void TQTikZPlotter::drawDminusB(TQTaggable& tags,std::ostream& output){
  // draw a data-minus-background plot on the sub pad

  // the subtraction is usually the background
  TString subtractionName = tags.getTagStringDefault("style.DminusB.subtraction","totalBkg");
  TH1* subtraction = this->getObject<TH1>(subtractionName);
  if(!subtraction) return;
  // the source of the error band is usually also the background
  TString errorSourceName = tags.getTagStringDefault("style.DminusB.errors","totalBkg");
  TH1* errorSource = this->getObject<TH1>(errorSourceName);
  if(!errorSource) return;
  
  // sort signals by integral to have smallest contribution in front
  std::vector<TH1*> v_signal(f_signal);;
  std::sort(v_signal.begin(),v_signal.end(),[&](TH1* a, TH1* b){ return a->GetSumOfWeights() >= b->GetSumOfWeights();});
  
  // loop over all signals and plot them
  for(auto h:v_signal){
    TString style = f_styles[h];
    output << "\\addplot+[mcnostack," << style <<"] coordinates {\n";
    histogramToTikZCoordinatesMC(output,h);
    output << "}\\closedcycle;\n";
  } 
   
  // create the error histogram 
  TH1* error = TQHistogramUtils::copyHistogram(errorSource);
  TQHistogramUtils::addHistogramWithoutError(error,subtraction,-1.);
  // plot the error band
  output << "\\addplot[const plot,draw=none,fill=none,name path=dmblower] coordinates {\n";
  histogramErrorsToTikZCoordinates(output,error,false);
  output << "};\n";
  output << "\\addplot[const plot,draw=none,fill=none,name path=dmbupper] coordinates {\n";
  histogramErrorsToTikZCoordinates(output,error,true);
  output << "};\n";
  output << "\\addplot[const plot," << f_styles[errorSource] << ",draw=none] fill between [of=dmblower and dmbupper];\n";
  bool showTotalBkg = tags.getTagBoolDefault ("style.showTotalBkg",true);
  if(showTotalBkg) {
    output << "\\addplot[const plot," << f_styles[errorSource] << ",fill=none] coordinates {\n";
    histogramToTikZCoordinatesMC(output,error);
    output << "};\n";
  }

  // loop over all data histograms and plot them
  for(auto h:f_data){
    TH1 * h_data = TQHistogramUtils::copyHistogram(h,TString::Format("%s_minus_bkg",h->GetName()));
    TQHistogramUtils::addHistogramWithoutError(h_data,subtraction,-1.);
    TString style = f_styles[h];
    output << "\\addplot+[datapoints," << style <<"] coordinates {\n";
    histogramToTikZCoordinatesData(output,h_data);
    output << "}\\closedcycle;\n";
    delete h_data;
  }
}

//__________________________________________________________________________________|___________

void TQTikZPlotter::drawOptScan(TQTaggable& tags,std::ostream& output,TQHistogramUtils::FOM FOMmode){
  // draw a cut optimization scan in the sub-pad
  bool verbose = tags.getTagBoolDefault("verbose",false);

  if(f_signal.size() < 1){
    ERRORclass("cannot perform optimization scan unless at least one signal contribution is scheduled!");
    return;
  }
  if(f_signal.size() > 1){
    WARNclass("more than one signal contribution scheduled, using first one arbitrarily!");
  }

  TH1* hTotalStack = this->getObject<TH1>("totalStack");
  if(!hTotalStack) return;
  TH1* hSig = f_signal[0];
  if(!hSig) return;

  // figure of merit (FOM) plot. e.g. S/sqrt(B) and etc. one can implement more
  TH1* hFOMl = 0;
  TH1* hFOMr = 0;
  TH1* hFOM = 0;

  //@tag:optScan.FOMbbb: evaluate the figure-of-merit bin-by-bin instead of integrated left and right (default:false)
  //@tag:style.FOMbbb: deprecated, use optScan.FOMbbb
  bool binByBin = tags.getTagBoolDefault("optScan.FOMbbb",tags.getTagBoolDefault ("style.FOMbbb",false));
  bool drawLegend = !binByBin;
  
  std::vector<TH1*> bkgSystHistos;
  collectOptScanSimplifiedSystHistograms(bkgSystHistos, tags); //TODO? if this returns false something was wrong and no syst histograms are provided -> should abort?
  
  if(binByBin){
    if(verbose){
      VERBOSEclass("drawing bin-by-bin significances with FOM=%s for S=%s and B=%s",TQHistogramUtils::getFOMTitleROOT(FOMmode).Data(),hSig->GetTitle(),hTotalStack->GetTitle());
    }
    hFOM = TQHistogramUtils::getFOMHistogram(FOMmode,hSig, hTotalStack, 0, bkgSystHistos);
    this->addObject(hFOM,"hist_FOM_bbb");
    this->f_styles[hFOM] = "draw=green";
  } else {
    if(verbose){
      VERBOSEclass("drawing optimization scan with FOM=%s for S=%s and B=%s",TQHistogramUtils::getFOMTitleROOT(FOMmode).Data(),hSig->GetTitle(),hTotalStack->GetTitle());
    }
    hFOMl = TQHistogramUtils::getFOMScan(FOMmode,hSig, hTotalStack, true,0.05,verbose, bkgSystHistos);
    this->f_styles[hFOMl] = "draw=red";
    hFOMr = TQHistogramUtils::getFOMScan(FOMmode,hSig, hTotalStack, false,0.05,verbose, bkgSystHistos);
    this->f_styles[hFOMr] = "draw=blue";
    //@tag:optScan.autoselect: select the optimization scan (left,right) that is better suited for every histogram and only show that one (default:false)
    if(tags.getTagBoolDefault("optScan.autoselect",false)){
      if(verbose) VERBOSEclass("autoselecting opt scan");
      if(TQHistogramUtils::hasGreaterMaximumThan(hFOMr,hFOMl)){
        if(verbose) VERBOSEclass("removing left-hand FOM histogram");
        this->addObject(hFOMr,"hist_FOM");
        delete hFOMl;
        hFOMl = NULL;
      } else {
        if(verbose) VERBOSEclass("removing right-hand FOM histogram");
        this->addObject(hFOMl,"hist_FOM");
        delete hFOMr;
        hFOMr = NULL;
      }
    } else {
      if(verbose) VERBOSEclass("using all opt scans");
      this->addObject(hFOMl,"hist_FOM_left");
      this->addObject(hFOMr,"hist_FOM_right");
    }
  }
  
  //cleanup
  for (TH1* toDel : bkgSystHistos) {
    delete toDel;
  }
  bkgSystHistos.clear(); //just to make sure the otherwise dangling pointers can not accessed anymore afterwards...


  if(drawLegend){
    std::vector<std::pair<TString,TString> > legend;
    if(hFOM)  legend.push_back(std::make_pair("no markers,green","bin-by-bin"));
    if(hFOMl) legend.push_back(std::make_pair("no markers,red","$\\rightarrow$ cut"));
    if(hFOMr) legend.push_back(std::make_pair("no markers,blue","$\\leftarrow$ cut"));
    
    // output all legend entries
    for(const auto& entry:legend){
      output << "\\addlegendimage{" << entry.first << "}\n";
      output << "\\addlegendentry{" << entry.second << "}\n";
    }
  }

  // set style
  if (hFOM) {
    //@tag:style.optScan.default.*: control styling of the auto-selected FOM graph
    if(verbose) VERBOSEclass("drawing FOM histogram");
    output << "\\addplot[const plot," << f_styles[hFOM] << ",fill=none] coordinates {\n";
    histogramToTikZCoordinatesMC(output,hFOM);
    output << "};\n";
  }
  if (hFOMl) {
    //@tag:style.optScan.left.*: control styling of the left-hand-side (lower) FOM graph
    if(verbose) VERBOSEclass("drawing FOM histogram (lhs)");
    output << "\\addplot[const plot," << f_styles[hFOMl] << ",fill=none] coordinates {\n";
    histogramToTikZCoordinatesMC(output,hFOMl);
    output << "};\n";
  }
  if (hFOMr) {
    //@tag:style.optScan.right.*: control styling of the right-hand-side (upper) FOM graph
    if(verbose) VERBOSEclass("drawing FOM histogram (rhs)");
    output << "\\addplot[const plot," << f_styles[hFOMr] << ",fill=none] coordinates {\n";
    histogramToTikZCoordinatesMC(output,hFOMr);
    output << "};\n";
  }

}
