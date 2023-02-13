#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQTable.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include "SFramework/TSStatisticsManager.h"

#include <fstream>
#include <algorithm>

#include "TStyle.h"
#include "TMath.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TLatex.h"

#include <limits>

#define inf std::numeric_limits<double>::infinity()

namespace {
  enum MatrixStatus {
    NOT_OK = 0,
    DIAG_UP_OK = 1,
    DIAG_DOWN_OK = 2,
    DIAG_UP_FLIPPED = 3,
    DIAG_DOWN_FLIPPED = 4
  };


  // test the orientation of the matrix
  MatrixStatus validateMatrix(TH2* hist, bool isCorrelation){
    if(!hist) return NOT_OK;
    size_t n = (size_t) hist->GetNbinsX();
    if(n != (unsigned) hist->GetNbinsY()) return NOT_OK;

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
      if(! (TMath::AreEqualRel(maxval_v1,1.,1e-3) && (maxidx_v1 ==  (int) i  )) ){
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


/*<cafdoc name=PlotCorrelations>
  PlotCorrelations
  ===========================

  Plot a correlation matrix. The value of the correlation will be
  color-coded and additionally printed onto each cell of the 2D plot.

  Usage:
  ---------------
  ```
  # correlation plots with 5% cutoff
  +PlotCorrelations.asimov.medium {
    +HWWRun2GGF {
      <outputFile="workspaces/run2-ggf-$(fitLabel)/correlations-asimov-medium.pdf">
    }
  }
  +PlotCorrelations.observed.medium {
    +HWWRun2GGF {
      <outputFile="workspaces/run2-ggf-$(fitLabel)/correlations-observed-medium.pdf">
    }
  }
  @PlotCorrelations.*.medium/? {
    <blacklist={"mu"}>
    <whitelist={"gamma_*"}>
    <cutoff=0.05>
    <style.showNumeric=true>
    <style.numeric.textSize=0.25>
    <style.textSize=0.01>
  }
  ```

  The name of the output file can be determined with the `outputFile` tag.
  The source of the correlation matrix is identified with the `fitresult` tag.

  Some additional options exist to control the styling:

    * `orientation="up"/"down"` choose whether the diagonal should be
       pointing up or down
    * `blacklist`: List expressions matching parameters that should
       not be included
    * `whitelist`: List expressions matching parameters that should be
       included, even if also matching the blacklist
    * `cutoff`: Numeric value of the minimum correlation to be
       plotted. All rows and columns not including any entry above this
       value will be dropped.
    *  `writeCorrelations.outputFile="filename.txt"`,
       `writeCorrelations.threshold=0.6`: Write all entries of the
       correlation matrix exceeding a given threshold to a log file
       for further studies.
    * `style.showNumeric` Set to false to suppress numbers printed on
       cells. The text size of the numbers can be controlled with
      `style.numeric.textSize`.
    * `style.textSize` : Set the text size of everything on the canvas.
    * `outputFile.csv`, `outputFile.plain`: Give filenames to also
       print the correlation matrix to a CSV or plain text file.
    * `style.canvasScale`: Control the scale of the canvas.
    * `style.marginScale`,`style.marginScale.top`,`style.marginScale.bottom`,`style.marginScale.left`,`style.marginScale.right`: Control the scale of the margins.

</cafdoc> */




namespace TSBaseActions {

  class PlotCorrelations : public TSStatisticsManager::Action {
    static bool XOR(bool a, bool b){
      return a != b;
    }
    
    static TString sanitize(TString orig){
      return orig.ReplaceAll("_","\\_");
    }


    bool plotPGF(TQFolder* config, TH1* reduced, const TString& outfilename) const {

      std::ofstream out(outfilename.Data());

      std::vector<TString> parnames;
      for(int i=1; i<reduced->GetNbinsX()+1; ++i){
        parnames.push_back(sanitize(reduced->GetXaxis()->GetBinLabel(i)));
      }
      std::vector<TString> parnames_r(parnames);
      std::reverse(parnames_r.begin(),parnames_r.end());
      
      TString concat_labels   = TQStringUtils::concat(parnames,",");
      TString concat_labels_r = TQStringUtils::concat(parnames_r,",");

      bool standalone = config->getTagBoolDefault("standalone",true);
      out << "% this plot was automatically generated by SFramework ActionPlotCorrelations\n";
      out << "% correlation matrices written this way can occasionally become very large\n";
      out << "% should you encounter an error like this one while trying to compile this file\n";
      out << "%     ! TeX capacity exceeded, sorry [main memory size=12000000].\n";
      out << "% try the following:\n";
      out << "%  - use lualatex or xelatex\n";
      out << "%  - increase your tex main memory size by editing your 'texmf.cnf'\n";
      out << "%    which you can find by typing 'kpsewhich -a texmf.cnf' in your terminal\n";
      out << "%    and change the entry 'main_memory' to some very large number, e.g. 'main_memory.xelatex = 500000000'\n";
      out << "%    finally, run 'fmtutil-sys --all' to update your configuration\n";
      if(standalone){
        out << "\\documentclass[tikz]{standalone}\n";
        out << "\\usepackage{pgfplots}\n";
        out << "\\ifpdftex\\usepackage[scaled=1]{helvet}\\fi\n";
        out << "\\ifxetex\\usepackage{fontspec}\\setsansfont{TeX Gyre Heros}\\fi\n";
        out << "\\begin{document}";
      }
      out << "\\ifpdftex\n";
      out << "\\renewcommand\\sfdefault{phv}\n";
      out << "\\renewcommand\\rmdefault{phv}\n";
      out << "\\renewcommand\\ttdefault{pcr}\n";
      out << "\\fi\n";
      out<<"\\begin{tikzpicture}\n";
      out<<"\\begin{axis}[\n";
      out<<"    colormap={bluewhitered}{color=(blue) color=(white) color=(red)},\n";
      out<<"    clip=false,\n";
      out<<"    colorbar,\n";
      out<<"    colormap name={bluewhitered},\n";
      out<<"    x=1em,\n";
      out<<"    y=1em,\n";
      out<<"    xtick=data,\n";
      out<<"    ytick=data,\n";
      out<<"    ymin=" << parnames_r[0] << ",\n";
      out<<"    ymax=" << parnames[0] << ",\n";
      out<<"    xmin=" << parnames[0] << ",\n";
      out<<"    xmax=" << parnames_r[0] << ",\n";
      out<<"    enlarge x limits={abs=0.5em},\n";
      out<<"    enlarge y limits={abs=0.5em},\n";
      out<<"    point meta min=-1,\n";
      out<<"    point meta max=+1,\n";
      out<<"    grid=both,\n";
      out<<"    major grid style={draw=none},\n";
      if (config->getTagBoolDefault("style.showNumeric",false)) {
        out<<"    nodes near coords,\n";
        out<<"    every node near coord/.append style={anchor=center,scale=0.25,/pgf/number format/.cd,fixed,precision=2},\n";
      }
      out<<"    minor tick num=1,\n";
      out<<"    symbolic x coords={" << concat_labels << "},\n";
      out<<"    symbolic y coords={" << concat_labels_r << "},\n";
      out<<"    axis on top,\n";
      out<<"    x tick label style={rotate=90},\n";
      out<<"    tick style={draw=none}\n";
      out<<"]\n";
      out<<"\\addplot [matrix plot*,point meta=explicit,mesh/cols=" << parnames.size() << "] table [meta=" << reduced->GetName() << "] {\n x  y  " << reduced->GetName() << "\n";
      for(int i=1; i<reduced->GetNbinsX()+1; ++i){
        for(int j=1; j<reduced->GetNbinsY()+1; ++j){
          out << " " << sanitize(reduced->GetXaxis()->GetBinLabel(i)) << " " << sanitize(reduced->GetYaxis()->GetBinLabel(j)) << " " << reduced->GetBinContent(i,j) << "\n";
        }
      }
      out << "};\n";

      bool drawATLAS = config->getTagBoolDefault ("labels.drawATLAS",true);
      if (drawATLAS) {
        out << "\\node (atlas) [above right, font={\\sffamily\\bfseries}] at (rel axis cs:0,1) {" << config->getTagStringDefault("labels.drawATLAS.text","ATLAS") << "};\n";
      }
      TString atlasLabel = config->getTagStringDefault ("labels.atlas","Private");
      if (drawATLAS && !atlasLabel.IsNull()){
        out << "\\node [anchor=west] at (atlas.east) {" << atlasLabel << "};\n";
      }

      std::vector<TString> labels;
      int nMax = config->getTagIntegerDefault("nMax",-1);
      if(nMax>0){
        labels.push_back(TString::Format("%d largest",nMax));
      }
      double cutoff = config->getTagDoubleDefault("cutoff",0);
      if(cutoff > 0){
        labels.push_back(TString::Format("correlation$>%g$",cutoff));
      }
      if(labels.size()>0){
        out<<"\\node[above left]  at (rel axis cs:1,1){ " << TQStringUtils::concat(labels,", ") << "};\n";
      }
      out<<"\\end{axis}\n";
      out<<"\\end{tikzpicture}\n";
      if(standalone){
        out<<"\\end{document}\n";
      }
      return true;
    }

    bool plotROOT(TQFolder* config, TH1* reduced, const TString& outputfile) const {
      gStyle->SetOptStat(false);
      gStyle->SetOptTitle(false);
      gStyle->SetLineWidth(0.5);
      double canvasScale = config->getTagDoubleDefault("style.canvasScale",1.);
      TCanvas* c = new TCanvas("correlations","Correlation Matrix",1024*canvasScale,1024*canvasScale);
      c->SetMargin( 0.2*config->getTagDoubleDefault("style.marginScale.left",config->getTagDoubleDefault("style.marginScale",1.)),
                    0.2*config->getTagDoubleDefault("style.marginScale.right",config->getTagDoubleDefault("style.marginScale",1.)),
                    0.2*config->getTagDoubleDefault("style.marginScale.bottom",config->getTagDoubleDefault("style.marginScale",1.)),
                    0.05*config->getTagDoubleDefault("style.marginScale.top",config->getTagDoubleDefault("style.marginScale",1.))
                    );
      double textsize = config->getTagDoubleDefault("style.textSize",0.04);
      reduced->GetXaxis()->SetLabelSize(textsize);
      reduced->GetXaxis()->SetTickLength(0);
      reduced->GetYaxis()->SetLabelSize(textsize);
      reduced->GetYaxis()->SetTickLength(0);
      reduced->GetXaxis()->LabelsOption("v"); //draw labels vertically

      c->cd();

      const Int_t Number = 4;
      Double_t Red[Number]    = { 0.00, 1.00,1.00, 1.00};
      Double_t Green[Number]  = { 0.00, 1.00,1.00, 0.00};
      Double_t Blue[Number]   = { 1.00, 1.00,1.00, 0.00};
      Double_t Length[Number] = { 0.00, 0.475,0.52499999, 1.00 }; 
      const int nb=40;
      TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);

      Double_t contours[nb+1];
      for (int i=0;i<nb+1;i++) {
        contours[i] = reduced->GetMinimum() + i*(reduced->GetMaximum()-(reduced->GetMinimum()))/nb;
      }
      reduced->SetContour(nb+1,contours);

      reduced->Draw("COLZ");
      TH2* copy = NULL;
      if (config->getTagBoolDefault("style.showNumeric",false)) {
        copy = static_cast<TH2*>(reduced->Clone());
        copy->Scale(100.);
        gStyle->SetPaintTextFormat(".0f");
        copy->SetMarkerSize(config->getTagDoubleDefault("style.numeric.textSize",1.));
        if (!copy) {manager->error("Failed to copy histogram for numeric overlay"); return false;}
        copy->Draw("TEXT,SAME");
      }

      float xposLabel = config->getTagDoubleDefault("labels.atlas.xpos", 0.3);
      float yposLabel = config->getTagDoubleDefault("labels.atlas.ypos", 0.955);
      if (config->getTagBoolDefault("labels.drawATLAS",true)) {
        // draw the ATLAS label
        TLatex l;
        l.SetNDC();
        l.SetTextFont(72);
        l.SetTextSize(config->getTagDoubleDefault("labels.drawATLAS.textsize", 0.02) );
        l.SetTextColor(1);
        l.DrawLatex(xposLabel, yposLabel, config->getTagStringDefault("labels.drawATLAS.text","ATLAS"));
      }
      TString atlasLabel = config->getTagStringDefault ("labels.atlas","Private");
      if (!atlasLabel.IsNull() && config->getTagBoolDefault("labels.drawATLAS",true)) {
        // draw the ATLAS label addition
        TLatex p;
        p.SetNDC();
        p.SetTextFont(42);
        p.SetTextColor(1);
        p.SetTextSize( config->getTagDoubleDefault("labels.drawATLAS.textsize", 0.02) );
        p.DrawLatex(xposLabel + config->getTagDoubleDefault("labels.atlas.xOffset",0.068), yposLabel, atlasLabel.Data());
      }

      c->SaveAs(TQPathManager::getPathManager()->getTargetPath(outputfile).c_str());

      if(copy) delete copy;
      delete c;

      return true;
    }

    bool execute(TQFolder * config) const override {

      TQFolder* result = results()->getFolder(config->GetName());
      if(!result){
        manager->error(TString::Format("no such result available: '%s'",config->GetName()));
        return false;
      }

      TString path;
      if(!config->getTagString("fitresult",path)){
        manager->error("you have to give a path via the 'fitresult' option!");
        return false;
      }

      TQFolder* fit = result->getFolder(path);
      if(!fit){
        manager->error(TString::Format("unable to retrieve fitresult '%s'",path.Data()));
        return false;
      } 

      bool showCovariance = config->getTagBoolDefault("showCovariance",false);      
      bool useCovariance = config->getTagBoolDefault("fromCovariance",showCovariance);
      if(showCovariance && !useCovariance){
        manager->error("can not show covariance with 'fromCovariance=false'");
        return false;
      }

      TString objectname = config->getTagStringDefault("object",useCovariance ? "covariance" : "correlation");
      manager->info(TString::Format("Retrieving object with name '%s'",objectname.Data()));
      TH2* orig = dynamic_cast<TH2*>(fit->getObject(objectname));
      if(!orig){
        manager->error(TString::Format("unable to retrieve covariance/correlation matrix '%s' from '%s'",objectname.Data(),path.Data()));
        return false;
      }

      size_t n = orig->GetXaxis()->GetNbins();

      TH2* correlations = new TH2F("correlations","correlations",n,0,n,n,0,n);
      correlations->SetDirectory(NULL);

      TString style = config->getTagStringDefault("orientation","down");
      bool down = false;
      bool up = false;
      if(style == "down") down=true;
      else if(style == "up") up = true;
      else {
        manager->error(TString::Format("unknown style option '%s'",style.Data()));
        return false;
      }
      MatrixStatus status = validateMatrix(orig,!useCovariance);

      bool autofix = config->getTagBoolDefault("autofix",true);
      bool flipXlabels = config->getTagBoolDefault("flipXlabels",false);
      bool flipXvalues = config->getTagBoolDefault("flipXvalues",false);
      bool flipYlabels = config->getTagBoolDefault("flipYlabels",false);
      bool flipYvalues = config->getTagBoolDefault("flipYvalues",false);
      if(autofix){
        if(down && status == DIAG_DOWN_OK){
          manager->info("using matrix as-is");
        } else if(down && status == DIAG_DOWN_FLIPPED){
          manager->info("flipping Y labels");
          flipYlabels = true;
        } else if(up && status == DIAG_UP_OK){
          manager->info("using matrix as-is");
        } else if(up && status == DIAG_UP_FLIPPED){
          manager->info("flipping Y labels");
          flipYlabels = true;
        } else if(down && status == DIAG_UP_OK){
          manager->info("flipping Y values & labels");
          flipYlabels = true;
          flipYvalues = true;
        } else if(down && status == DIAG_UP_FLIPPED){
          manager->info("flipping Y values");
          flipYvalues = true;
        } else if(up && status == DIAG_DOWN_OK){
          manager->info("flipping Y values & labels");
          flipYlabels = true;
          flipYvalues = true;
        } else if(up && status == DIAG_DOWN_FLIPPED){
          manager->info("flipping Y values");
          flipYvalues = true;
        } else {
          manager->error("matrix is broken, aborting!");
          return false;
        }
      }
      
      for(size_t i=0; i<n; ++i){
        int ix = flipXvalues?(n-i):(i+1);
        double xx = orig->GetBinContent(ix,ix);
        //XOR(up,!flipXvalues)?(i+1):(n-i));        
        for(size_t j=0; j<n; ++j){
          int iy = flipYvalues?(n-j):(j+1);
          double xy = orig->GetBinContent(ix,iy);
          double yy = orig->GetBinContent(iy,iy);
          double corr = xy /  (useCovariance && !showCovariance ? sqrt(xx * yy) : 1. );
          correlations->SetBinContent(i+1,j+1,corr);
        }
      }

      for(size_t i=0; i<n; ++i){
        int x = flipXlabels ? n-i : i+1;
        int y = flipYlabels ? n-i : i+1;
        correlations->GetXaxis()->SetBinLabel(i+1,orig->GetXaxis()->GetBinLabel(x));
        correlations->GetYaxis()->SetBinLabel(i+1,orig->GetYaxis()->GetBinLabel(y));
      }


      std::ofstream log;
      TString logfilename;
      bool writelog = config->getTagString("writeCorrelations.outputFile",logfilename);
      if(writelog){
        log.open(TQPathManager::getPathManager()->getTargetPath(logfilename).c_str());
        double threshold = config->getTagDoubleDefault("writeCorrelations.threshold",.60);
        for(size_t i=0; i<n; ++i){
          int x = down?(n-i):(i+1);
          TString xlabel = correlations->GetXaxis()->GetBinLabel(x);
          for(size_t j=i+1; j<n; ++j){
            int y = j+1;
            TString ylabel =  correlations->GetYaxis()->GetBinLabel(y);
            double corr = correlations->GetBinContent(x,y);
            //            std::cout << i << " " << j << " " << x << " " << y << " " << corr << std::endl;
            if(fabs(corr) > threshold){
              log << "corr(" << xlabel << "," << ylabel << ")=" << corr << std::endl;
            }
          }
          log.close();
        }
      }

      std::vector<TString> blacklist = config->getTagVString("blacklist");
      std::vector<TString> whitelist = config->getTagVString("whitelist");
      if(whitelist.empty()){
        whitelist.push_back("*");
      }

      // filter and reduce
      std::vector<TString> remove;
      std::vector<std::pair<std::string,double> > keep;
      double cutoff = config->getTagDoubleDefault("cutoff",0);
      if(!showCovariance && cutoff >=1){
        manager->error(TString::Format("cannot use cutoff %.2f > 1",cutoff));
        return false;
      }

      for(size_t i=0; i<n; ++i){
        TString label = correlations->GetXaxis()->GetBinLabel(down?n-i:i+1);
        // check whitelists and blacklists
        bool use = false;
        for(auto filter:whitelist){
          if(TQStringUtils::matches(label,filter)){
            use = true;
          }
        }
        for(auto antifilter:blacklist){
          if(TQStringUtils::matches(label,antifilter)){
            use = false;
          }
        }
        // check cutoff
        double maxcorr = 0;
        for(size_t j=0; j<n; ++j){
          if(i==j) continue;
          double corr = std::abs(correlations->GetBinContent(down?n-i:i+1,j+1));
          if(corr > maxcorr) maxcorr = corr;
        }
        if(maxcorr < cutoff) use = false;
        if(use) keep.push_back(std::make_pair<std::string,double>(label.Data(),double(maxcorr)));
        else remove.push_back(label);
      }
      // remove subleding entries as requested
      std::sort(keep.begin(), keep.end(), [](const std::pair<std::string,double> &x,
                                             const std::pair<std::string,double> &y)
      {
        return x.second > y.second;
      });
      int nMax;
      if(config->getTagInteger("nMax",nMax)){
        for(size_t i=keep.size()-1; i>=(size_t)nMax; --i){
          remove.push_back(keep[i].first.c_str());
        }
      }
      
      std::vector<TString> ordering = config->getTagVString("ordering");
     // actually make the histogram
      TH2* reduced = TQHistogramUtils::removeBins(correlations,remove);
      if(!ordering.empty()){TQHistogramUtils::reorderBins(reduced,ordering);}
      if(!reduced){
        manager->error(TString::Format("unable to trim correlation histogram by %d/%d items!",(int)(remove.size()),(int)n));
        return false;
      }

      reduced->SetDirectory(NULL);
      if(!showCovariance){
        reduced->SetMinimum(-1);
        reduced->SetMaximum(1);
      } else {
        double max = std::max(TQHistogramUtils::getMax(reduced),std::abs(TQHistogramUtils::getMin(reduced)));
        reduced->SetMinimum(-max);
        reduced->SetMaximum(max);        
      }

      TString outputfile = config->getTagStringDefault("outputFile","correlations.pdf");
      outputfile = TQPathManager::getPathManager()->getTargetPath(outputfile);
      TQUtils::ensureDirectoryForFile(outputfile);
      manager->info(TString::Format("plotting %dx%d correlation matrix (originally %dx%d) to '%s'",reduced->GetXaxis()->GetNbins(),reduced->GetYaxis()->GetNbins(),correlations->GetXaxis()->GetNbins(),correlations->GetYaxis()->GetNbins(),outputfile.Data()));
      if(outputfile.EndsWith(".tex")){
        if(!plotPGF(config,reduced,outputfile)) return false;
      } else {
        if(!plotROOT(config,reduced,outputfile)) return false;
      }

      TString outputfileCSV;
      TString outputfilePlain;
      //@tag:[outputFile.csv] write (reduced) correlation matrix to a csv file with path/name defined by this tag
      //@tag:[outputFile.plain] write (reduced) correlation matrix to a plain text file with path/name defined by this tag
      if (config->getTagString("outputFile.csv",outputfileCSV) + config->getTagString("outputFile.plain",outputfilePlain) ) {
        //if requested also generate a table instead of a plot
        TQTable correlationTable("correlations");
        correlationTable.expand(reduced->GetNbinsX()+1,reduced->GetNbinsY()+1);
        for (int x=1; x<reduced->GetNbinsX(); ++x) {
          correlationTable.setEntry(0,x,reduced->GetXaxis()->GetBinLabel(x));
          for (int y=1; y<reduced->GetNbinsY(); ++y) {
            if (x==1) correlationTable.setEntry(y,0,reduced->GetYaxis()->GetBinLabel(y));
            correlationTable.setEntryValue(y,x,reduced->GetBinContent(x,y));
          }

        }
        if (outputfileCSV.Length()>0) {
          TQUtils::ensureDirectoryForFile(TQPathManager::getPathManager()->getTargetPath(outputfileCSV));
          correlationTable.writeCSV(TQPathManager::getPathManager()->getTargetPath(outputfileCSV),config); }
        if (outputfilePlain.Length()>0) {
          TQUtils::ensureDirectoryForFile(TQPathManager::getPathManager()->getTargetPath(outputfilePlain));
          correlationTable.writePlain(TQPathManager::getPathManager()->getTargetPath(outputfilePlain),config); }
      }


      delete correlations;
      delete reduced;
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new PlotCorrelations(),"PlotCorrelations");
  }
}
