#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQTable.h"
#include "QFramework/TQPathManager.h"
#include "QFramework/TQSystematics.h"

#include "SFramework/TSModelBuilder.h"
#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"

#include <vector>
#include <algorithm>

namespace{
  template<class X,class Y> bool in(const std::vector<X>& v, const Y& e){
    return std::find(v.begin(), v.end(), e) != v.end();
  }
}

/*<cafdoc name=PlotSystematicsStatus>
  PlotSystematicsStatus
  ===========================
  
  Make a summary plot or table of all the systematics for all samples in all regions.

  Export a `RooFit` workspace to a file with a given name. This must be a `.root` file.
  
  Usage:
  ---------------

  If given a `.pdf` file ending, the plot will be made using `ROOT`. These are sometimes hard to read, so the text size usually needs to be adjusted.
  ```
  +PlotSystematicsStatus.legacy {
    +HWWRun2GGF{
	    <outputFile = "./workspaces/run2-ggf-$(fitLabel)/systematicsStatus.pdf", show="Percent", textFormat=".1f", style.textSize=0.01>
    }
    +HWWRun2GGFSimple{
      <outputFile = "./workspaces/run2-ggf-$(fitLabel)/systematicsStatusInitial.pdf", show="Percent", textFormat=".1f", style.textSize=0.01>
    }
  }
  ```

  A more readable alternative is to use TQTable to make `.html` or `.tex` files.
  ```
  +PlotSystematicsStatus.modern {
    +HWWRun2GGF{
	    <outputFile = "./workspaces/run2-ggf-$(fitLabel)/systematicsStatus.html", show="Percent">
    }
    +HWWRun2GGFSimple{
      <outputFile = "./workspaces/run2-ggf-$(fitLabel)/systematicsStatusInitial.html", show="Percent">
    }
  }
  ```

</cafdoc> */

namespace TSBaseActions {
  
  class PlotSystematicsStatus : public TSStatisticsManager::Action {
    
    bool matchesAny(const TString& f,const std::vector<TString>& filters) const {
      bool matched = false;
      for(const auto& x:filters){
        if(TQStringUtils::matches(f,x)){
          matched = true;
          break;
        }
      }
      return matched;
    }
    
    
    bool execute(TQFolder * config) const override {
      
      TString writeHists =    config->replaceInText(config->getTagStringDefault("writeHistograms"));
      TString sysType = config->getTagStringDefault("systematicType","OverallSys");
      
      std::vector<TString> sysFilter = config->getTagVString("systematicFilter");
      if(sysFilter.empty()){
        sysFilter.push_back("*");
      }

      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        return false;
      }

      double textsize = config->getTagDoubleDefault("style.textSize",0.08);

      std::map< std::pair< const std::string, const std::string>, std::map<const std::string, TQFolder* > > systematics;
      std::vector<std::string> channelList;
      std::vector<std::string> sampleList;
      std::vector<std::string> systematicList;

      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // collect all the folders
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      TString variationFilter = config->getTagStringDefault("variationFilter",sysType+".*");
      TString sampleFilter = config->getTagStringDefault("sampleFilter","*");
      TString channelFilter = config->getTagStringDefault("channelFilter","*");
      /* @tag: [variationFilter, sampleFilter, channelFilter] filter to decide what is plotted. For the variationFilter OverallSys or HistoSys needs to be included. */
      
      // collect all the folders
      TQFolderIterator channels(model->getListOfFolders("Channel."+channelFilter),true);
      while(channels.hasNext()){
        TQFolder* channel = channels.readNext();
        if(!channel) continue;
        TString channelname(channel->GetName());
        if(!TQStringUtils::removeLeadingText(channelname,"Channel.")) continue;
        //        channel->getTagString("Title",channelname);
        if(!in(channelList,channelname.Data())) channelList.push_back(channelname.Data());
        TQFolderIterator samples(channel->getListOfFolders("Sample."+sampleFilter),true);
        while(samples.hasNext()){
          TQFolder* sample = samples.readNext();
          if(!sample) continue;
          TString samplename(sample->GetName());
          if(!TQStringUtils::removeLeadingText(samplename,"Sample.")) continue;
          //          sample->getTagString("Title",samplename);
          if(!in(sampleList,samplename.Data())) sampleList.push_back(samplename.Data());
          auto coordinate = std::make_pair(channelname.Data(),samplename.Data());
          systematics[coordinate] = std::map<const std::string, TQFolder*>();
          TQFolderIterator goodSystematics(sample->getListOfFolders(variationFilter),true);
          while(goodSystematics.hasNext()){
            TQFolder* syst = goodSystematics.readNext();
            if(!syst) continue;
            TString systname(syst->GetName());
            if(!TQStringUtils::removeLeadingText(systname,sysType+".")) continue;
            if(!matchesAny(systname,sysFilter)) continue;
            systematics[coordinate][systname.Data()] = syst;
            if(!in(systematicList,systname.Data())) systematicList.push_back(systname.Data());
          }
          TQFolderIterator badSystematics(sample->getListOfFolders(TQFolder::concatPaths(".Dropped",variationFilter)),true);
          while(badSystematics.hasNext()){
            TQFolder* syst = badSystematics.readNext();
            if(!syst) continue;
            TString systname(syst->GetName());
            if(!TQStringUtils::removeLeadingText(systname,sysType+".")) continue;
            if(!matchesAny(systname,sysFilter)) continue;
            systematics[coordinate][systname.Data()] = syst;
            if(!in(systematicList,systname.Data())) systematicList.push_back(systname.Data());
          }
        }
      }

      std::sort(systematicList.begin(),systematicList.end());

      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // set styling
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
      TQTable tab;
      TQTable list;
      list.setTagBool("adjustColWidth",true);
      list.setEntry(0,0,"Systematic");
      list.setEntry(0,1,"Channel");
      list.setEntry(0,2,"Sample");
      list.setEntry(0,3,"Status");      
      
      // make the histograms
      std::map<TQSystematics::Status,int> colors; std::map<TQSystematics::Status,const char*> latexcolors;
      colors[TQSystematics::ACTIVE] = kGreen;               latexcolors[TQSystematics::ACTIVE] = "green";         
      colors[TQSystematics::ZERO] = kGray;                  latexcolors[TQSystematics::ZERO] = "gray";            
      colors[TQSystematics::DROPPED] = kYellow;             latexcolors[TQSystematics::DROPPED] = "yellow";       
      colors[TQSystematics::FIXED] = kBlue;                 latexcolors[TQSystematics::FIXED] = "blue";           
      colors[TQSystematics::OVERSHOOT] = kRed;              latexcolors[TQSystematics::OVERSHOOT] = "red";        
      colors[TQSystematics::ONESIDED] = kMagenta;           latexcolors[TQSystematics::ONESIDED] = "magenta";     
      colors[TQSystematics::SYMMETRIZED] = kCyan;           latexcolors[TQSystematics::SYMMETRIZED] = "cyan";     
      colors[TQSystematics::INSIGNIFICANT] = kOrange;;      latexcolors[TQSystematics::INSIGNIFICANT] = "orange";;


      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // prepare visualizations
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
      const size_t nSysts = systematicList.size();
      const size_t nRegions = channelList.size();
      const size_t nSamples = sampleList.size();
      size_t nLines = 0;
      for(size_t i=0; i<nRegions; ++i){
        for(size_t j=0; j<nSamples; j++){
          auto coordinate = std::make_pair(channelList[i],sampleList[j]);
          bool empty = true;
          for(auto syst:systematics[coordinate]){
            if(syst.second) empty=false;
          }
          if(!empty) nLines++;
          else systematics[coordinate].clear();
        }
      }
      bool verbose = config->getTagBoolDefault("verbose",false);

      if(verbose){
        std::stringstream ss;
        for(size_t i=0; i<nSysts; ++i){
          ss << systematicList[i] << ",";
        }
        manager->info("systematics: "+ss.str());
        ss.str("");
        for(size_t i=0; i<nRegions; ++i){
          ss << channelList[i] << ",";
        }
        manager->info("regions: "+ss.str());
        ss.str("");
        for(size_t i=0; i<nSamples; ++i){
          ss << sampleList[i] << ",";
        }
        manager->info("channels: "+ss.str());
        ss.str("");
      }
      gStyle->SetPaintTextFormat(config->getTagStringDefault("textFormat","d"));

      std::map<TQSystematics::Status,TH2*> histograms_2d;
      std::map<TQSystematics::Status,std::map<std::string,TH1*> > histograms_1d;
      for(auto it:colors){
        TQSystematics::Status status = it.first;
        TString id(TQSystematics::name(status));
        id.ReplaceAll("-","_");
        TH2* hist_2d = new TH2F(id.Data(),TQSystematics::name(status),nSysts,0,nSysts,nLines,0,nLines);
        hist_2d->SetDirectory(NULL);
        std::map<std::string,TH1*> thishists;
        for(const auto& s:sampleList){
          TH1* hist_1d = new TH1F(s.c_str(),s.c_str(),nRegions,0,nRegions);
          TSModelBuilder::applyStyle(model,s,hist_1d);
          TString title(hist_1d->GetTitle());
          title.ReplaceAll("$","");
          title.Append(" ("+s+")");
          hist_1d->SetTitle(title);
          if(hist_1d->GetFillStyle() > 1000) hist_1d->SetLineColor(hist_1d->GetFillColor());          
          hist_1d->SetDirectory(NULL);
          hist_1d->GetYaxis()->SetTitle("Number of '"+sysType+"' Nuisance Parameters with status '"+id+"'");
          int iregion=1;
          for(const auto& r:channelList){
            hist_1d->GetXaxis()->SetBinLabel(iregion,r.c_str());
            iregion++;
          }
          thishists[s] = hist_1d;
        }
        histograms_1d[status] = thishists;
        histograms_2d[status] = hist_2d;
      }
      TString show;
      bool doShow = config->getTagString("show",show);
      if(doShow){
        list.setEntry(0,4,show);
      }
      TH2* text = new TH2F("text","text",nSysts,0,nSysts,nLines,0,nLines);
      text->SetDirectory(NULL);
      size_t tabline = 0;
      size_t listline = 0;      
      for(size_t i=0; i<nRegions; ++i){
        for(size_t j=0; j<nSamples; ++j){
          auto coordinate = std::make_pair(channelList[i],sampleList[j]);
          if(systematics[coordinate].size() == 0) continue;
          tabline++;
          for(size_t k=0; k<nSysts; ++k){
            listline++;
            list.setEntry(listline,0,systematicList[k]);            
            list.setEntry(listline,1,channelList[i]);
            list.setEntry(listline,2,sampleList[j]);            
            int status;
            TQFolder* syst = systematics[coordinate][systematicList[k]];
            if(!syst){
              if(verbose) manager->warn(TString::Format("in Action 'PlotSystematicsStatus': unable to access systematic '%s' for sample '%s' in channel '%s'",systematicList[k].c_str(),sampleList[j].c_str(),channelList[i].c_str()));
              list.setEntry(listline,3,"lost");
              continue;
            }
            Int_t bin = text->GetBin(k+1,tabline);
            TQSystematics::Status actualStatus;
            if(syst->getTagInteger("Status",status)){
              actualStatus = (TQSystematics::Status)status;
            } else if(TQStringUtils::equal(syst->getBase()->GetName(),".Dropped")){
              actualStatus = TQSystematics::DROPPED;
            } else {
              actualStatus = TQSystematics::ACTIVE;
            }
            TH2* hist_2d = histograms_2d[actualStatus];
            if (hist_2d){
              hist_2d->SetBinContent(bin,1);
            }
            TH1* hist_1d = histograms_1d[actualStatus][sampleList[j]];
            if (hist_1d){
              hist_1d->Fill(channelList[i].c_str(),1);
            }
            list.setEntry(listline,3,TQSystematics::name(actualStatus));                            
            if(doShow){
              double x;
              TString s;
              if(syst->getTagDouble(show,x)){
                text->SetBinContent(bin,x);
                list.setEntryValue(listline,4,x);
                tab.setEntryValue(tabline,k+2,x);
              } else if(syst->getTagString(show,s)){
                tab.setEntry(tabline,k+2,s);
                list.setEntry(listline,4,s);                
              }
            }
            tab.setProperty(tabline,k+2,"tooltip",syst->exportTagsAsString().ReplaceAll("\"","'"));
            tab.setProperty(tabline,k+2,"status",actualStatus);
            tab.setProperty(tabline,k+2,"cellcolor",latexcolors[actualStatus]);
          }
        }
      }
      list.clearVlines();      
      tab.clearVlines();
      tab.setVline(2,2);
      tab.setHline(1,2);
      tab.setColAlign(0,"r");
      for(size_t i=0; i<nSysts; ++i){
        tab.setEntry(0,i+2,systematicList[i]);
        tab.setProperty(0,i+2,"rotate",90);
      }
      {
        int tabline = 0;
        for(size_t i=0; i<nRegions; ++i){
          int catline = tabline+1;
          tab.setEntry(catline,0,channelList[i]);
          for(size_t j=0; j<nSamples; ++j){
            auto coordinate = std::make_pair(channelList[i],sampleList[j]);
            if(systematics[coordinate].size() == 0) continue;
            tabline++;
            tab.setEntry(tabline,1,sampleList[j]);
          }
          tab.setProperty(catline,0,"multirow",tabline-catline);
          tab.setProperty(catline,0,"rotate",90);
          tab.setHline(catline,1);
        }
      }
      
      for(auto it:histograms_2d){
        auto i=it.first;
        auto hist=it.second;
        if (!hist) continue;
        hist->SetFillColor(colors[i]);
        for(size_t i=0; i<nSysts; ++i){
          hist->GetXaxis()->SetBinLabel(1+i,systematicList[i].c_str());
        }
        size_t line = 0;
        for(size_t i=0; i<nRegions; ++i){
          for(size_t j=0; j<nSamples; ++j){
            auto coordinate = std::make_pair(channelList[i],sampleList[j]);
            if(systematics[coordinate].size() == 0) continue;
            hist->GetYaxis()->SetBinLabel(line+1,(channelList[i]+" "+sampleList[j]).c_str());
            line++;
          }
        }
      }
      
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // write outputs
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      gStyle->SetOptStat(false);
      gStyle->SetOptTitle(false);
      
      if(writeHists.Length() > 0){
        TQUtils::ensureDirectory(writeHists);
        for(const auto& cat:histograms_1d){
          int nhists = 0;
          TCanvas* c = new TCanvas(TQSystematics::name(cat.first));
          c->SetRightMargin(0.2);
          c->cd();
          TLegend* leg = new TLegend(0.8,0.1,1.0,0.9);
          double max = 0;
          for(const auto& hist:cat.second){
            for(int i=0; i<hist.second->GetNbinsX(); ++i){
              hist.second->SetBinError(i+1,1e-9);
            }
            hist.second->SetMarkerColor(hist.second->GetLineColor());
            max = std::max(max,hist.second->GetMaximum());
          }
          for(const auto& hist:cat.second){
            if(hist.second->GetEntries() == 0) continue;
            if(nhists == 0){
              hist.second->SetMaximum(1.1*max);
              hist.second->SetMinimum(0);              
              hist.second->Draw("E");
            }
            else      hist.second->Draw("ESAME");
            leg->AddEntry(hist.second,hist.second->GetTitle(),"l");
            nhists++;
          }
          if(nhists > 0){
            leg->Draw();
            c->SaveAs(TQFolder::concatPaths(writeHists,TString(TQSystematics::name(cat.first))+".pdf"));
          }
          delete leg;
          delete c;
        }
      }

      std::vector<TString> filenames = config->getTagVString("outputFile");
      for(const auto& filename_orig:filenames){
        TString filename = config->replaceInText(filename_orig);
        if (filename.EndsWith(".root")) {
          TFile * file = TFile::Open(TQPathManager::getPathManager()->getTargetPath(filename).c_str(), "RECREATE");
          if (file) {
            if (file->IsOpen()) {
              for(auto it:histograms_2d){
                auto hist=it.second;
                file->Add(hist);
              }
              file->Write();
              file->Close();
            }
            delete file;
          } else {
            manager->error(TString::Format("-> Failed to write ROOT file '%s'", filename.Data()));
          }
        } else if(filename.EndsWith(".pdf")){
          // plot stuff
          double elementsize = 10;
          double marginsize = 400;
          double smallmarginsize = 10;
          double x = marginsize+elementsize*nSysts+smallmarginsize;
          double y = marginsize+elementsize*nLines+smallmarginsize;
          TCanvas* c = new TCanvas("SystematicsStatus","SystematicsStatus",x,y);
          c->SetFrameLineWidth(0.);
          c->SetFrameLineStyle(0);
          c->SetMargin(marginsize/x,smallmarginsize/x,marginsize/y,smallmarginsize/y);
          c->cd();
          bool first = true;
          for(auto it:histograms_2d){
            auto hist=it.second;
            if (!hist) continue;
            if(first){
              hist->SetLabelSize(textsize,"xy");
              hist->GetXaxis()->SetLabelOffset(0.2*elementsize/y);
              hist->GetXaxis()->SetTickSize(0.);
              hist->GetXaxis()->SetAxisColor(kWhite,0.);
              hist->GetYaxis()->SetLabelOffset(0.2*elementsize/x);
              hist->GetYaxis()->SetTickSize(0.);
              hist->GetYaxis()->SetAxisColor(kWhite,0.);
              hist->Draw("BOX");
            }
            else hist->Draw("SAMEBOX");
            first = false;
          }
          text->SetMarkerSize(20*textsize);
          text->Draw("SAMETEXT");
          c->Update();
          c->SaveAs(TQPathManager::getPathManager()->getTargetPath(filename).c_str());
          delete c;
        } else if(filename.EndsWith(".tex")){
          TString fname = TQPathManager::getPathManager()->getTargetPath(filename);
          manager->info(TString::Format("writing '%s'",fname.Data()));
          tab.writeLaTeX(TQPathManager::getPathManager()->getTargetPath(filename),"standalone=true");
        } else if(filename.EndsWith(".html")){
          TString fname = TQPathManager::getPathManager()->getTargetPath(filename);
          manager->info(TString::Format("writing '%s'",fname.Data()));
          tab.writeHTML(fname);
        } else if(filename.EndsWith(".txt")){
          TString fname = TQPathManager::getPathManager()->getTargetPath(filename);
          manager->info(TString::Format("writing '%s'",fname.Data()));
          list.writePlain(fname);
        } else if(filename.EndsWith(".csv")){
          TString fname = TQPathManager::getPathManager()->getTargetPath(filename);
          manager->info(TString::Format("writing '%s'",fname.Data()));
          list.writeCSV(fname);
        }        
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new PlotSystematicsStatus(),"PlotSystematicsStatus");
  }
}
    
