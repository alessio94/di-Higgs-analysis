#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include "QFramework/TQLibrary.h"

#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"

#include <vector>
#include <algorithm>

namespace{
  template<class X,class Y> bool in(const std::vector<X>& v, const Y& e){
    return std::find(v.begin(), v.end(), e) != v.end();
  }
}

namespace TSBaseActions {
  
  class PlotSystematicsSignificance : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {
      if (!config) return false;
      TString filename = config->replaceInText(config->getTagStringDefault("outputFile"));
      bool verbose = config->getTagBoolDefault("verbose",false);
      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        models()->print();
        return false;
      }

      std::map< std::pair< const std::string, const std::string>, std::map<const std::string, TQFolder* > > systematics;
      std::vector<std::string> channelList;
      std::vector<std::string> sampleList;
      std::vector<std::string> systematicList;
      int nLines = 0;
      // collect all the folders
      TQFolderIterator channels(model->getListOfFolders("Channel.*"),true);
      while(channels.hasNext()){
        TQFolder* channel = channels.readNext();
        if(!channel) continue;
        TString channelname(channel->GetName());
        if(!TQStringUtils::removeLeadingText(channelname,"Channel.")) continue;
        //        channel->getTagString("Title",channelname);
        if(!in(channelList,channelname.Data())) channelList.push_back(channelname.Data());
        TQFolderIterator samples(channel->getListOfFolders("Sample.*"),true);
        while(samples.hasNext()){
          TQFolder* sample = samples.readNext();
          if(!sample) continue;
          TString samplename(sample->GetName());
          if(!TQStringUtils::removeLeadingText(samplename,"Sample.")) continue;
          //          sample->getTagString("Title",samplename);
          if(!in(sampleList,samplename.Data())) sampleList.push_back(samplename.Data());
          //check if there will be any entries:
          
          TList* goodSamples = config->getTagBoolDefault("showGoodSystematics",true) ? sample->getListOfFolders("OverallSys.*") : NULL;
          TList* badSamples = config->getTagBoolDefault("showBadSystematics",false) ? sample->getListOfFolders(".Dropped/OverallSys.*") : NULL;
          if (0 == (goodSamples ? goodSamples->GetSize() : 0) + (badSamples ? badSamples->GetSize() : 0) ) continue; //don't create an entry if there would be just an empty line 
          nLines++; //if there should be an entry in this line, we increment this counter
          auto coordinate = std::make_pair(channelname.Data(),samplename.Data());
          systematics[coordinate] = std::map<const std::string, TQFolder*>();
          //if (config->getTagBoolDefault("showGoodSystematics",true)) {
            TQFolderIterator goodSystematics(goodSamples,true);
            while(goodSystematics.hasNext()){
              TQFolder* syst = goodSystematics.readNext();
              if(!syst) continue;
              TString systname(syst->GetName());
              if(!TQStringUtils::removeLeadingText(systname,"OverallSys.")) continue;
              systematics[coordinate][systname.Data()] = syst;
              if(!in(systematicList,systname.Data())) systematicList.push_back(systname.Data());
            }
          //}
          //if (config->getTagBoolDefault("showBadSystematics",false)) {
            TQFolderIterator badSystematics(badSamples,true);
            while(badSystematics.hasNext()){
              TQFolder* syst = badSystematics.readNext();
              if(!syst) continue;
              TString systname(syst->GetName());
              if(!TQStringUtils::removeLeadingText(systname,"OverallSys.")) continue;
              systematics[coordinate][systname.Data()] = syst;
              if(!in(systematicList,systname.Data())) systematicList.push_back(systname.Data());
            }
          //}
        }
      }

      const size_t nSysts = systematicList.size();
      const size_t nRegions = channelList.size();
      const size_t nSamples = sampleList.size();

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
      
      gStyle->SetPaintTextFormat(config->getTagStringDefault("textFormat","d"));
      
      
      TH2* hDown = new TH2F("hDown","hDown",nSysts,0,nSysts,nLines,0,nLines);
      TH2* hUp = new TH2F("hUp","hUp",nSysts,0,nSysts,nLines,0,nLines);
      TH2* hMax = new TH2F("hMax","hMax",nSysts,0,nSysts,nLines,0,nLines);
      int line = 0;
      for(size_t i=0; i<nRegions; ++i){
        for(size_t j=0; j<nSamples; ++j){
          auto coordinate = std::make_pair(channelList[i],sampleList[j]);
          if (systematics.count(coordinate) == 0) continue; //this entry doesn't exist (we skipped it before)
          line++;
          hDown->GetYaxis()->SetBinLabel(line,(channelList[i]+" "+sampleList[j]).c_str());
          hUp->GetYaxis()->SetBinLabel(line,(channelList[i]+" "+sampleList[j]).c_str());
          hMax->GetYaxis()->SetBinLabel(line,(channelList[i]+" "+sampleList[j]).c_str());
          for(size_t k=0; k<nSysts; ++k){
            
            TQFolder* syst = systematics[coordinate][systematicList[k]];
            if(!syst){
              if(verbose)
                manager->warn(TString::Format("in Action 'PlotSystematicsSignificance': unable to access systematic '%s' for sample '%s' in channel '%s'",systematicList[k].c_str(),sampleList[j].c_str(),channelList[i].c_str()));
              continue;
            }
            Int_t bin = hDown->GetBin(k+1,line);
            double down = fabs((syst->getTagDoubleDefault("High",0.)-1) / syst->getTagDoubleDefault("HighErr",100.));
            double up = fabs((1-syst->getTagDoubleDefault("Low",0.)) / syst->getTagDoubleDefault("LowErr",100.));
            double max = std::max(down,up);
            bool doSomething = false;
            if (syst->getTagDoubleDefault("HighErr",100.) != 100.) {hDown->SetBinContent(bin,down); doSomething = true;}
            if (syst->getTagDoubleDefault("LowErr",100.) != 100.) {hUp->SetBinContent(bin,up); doSomething = true;}
            if (syst->getTagDoubleDefault("LowErr",100.) != 100. || syst->getTagDoubleDefault("HighErr",100.) != 100.) {hMax->SetBinContent(bin,max); doSomething = true;}
            if (doSomething && verbose) INFO("setting bin content: (%.2f/%.2f/%.2f), statErr: (%.2f/%.2f/%.2f)",up,down,max,syst->getTagDoubleDefault("HighErr",100.),syst->getTagDoubleDefault("LowErr",100.), std::max( syst->getTagDoubleDefault("HighErr",100.),syst->getTagDoubleDefault("HighErr",100.) ) );
            
            //TString show;
            /*if(config->getTagString("show",show)){
              double x;
              if(syst->getTagDouble(show,x)){
                text->SetBinContent(bin,x);
              }
            }*/
          }
        }
      }
      
      for(size_t i=0; i<nSysts; ++i){
        hDown->GetXaxis()->SetBinLabel(1+i,systematicList[i].c_str());
        hUp->GetXaxis()->SetBinLabel(1+i,systematicList[i].c_str());
        hMax->GetXaxis()->SetBinLabel(1+i,systematicList[i].c_str());
      }
      for(size_t i=0; i<nRegions; ++i){
        for(size_t j=0; j<nSamples; ++j){

        }
      }

      
      /* write file */
      if (filename.EndsWith(".root")) {
        TFile * file = TFile::Open(TQPathManager::getPathManager()->getTargetPath(filename).c_str(), "RECREATE");
        if (file) {
          if (file->IsOpen()) {
            file->Add(hDown);
            file->Add(hUp);
            file->Add(hMax);
            file->Write();
            file->Close();
          }
          delete file;
        } else {
          manager->error(TString::Format("-> Failed to write ROOT file '%s'", filename.Data()));
        }
      } else {
        size_t pos = TQStringUtils::findLastOf(filename,".");
        TString extension = filename(pos,filename.Length());
        filename = filename(0,pos-1); 
        // plot stuff
        double lMargin = config->getTagDoubleDefault("lMargin",0.20);
        double textSizeX = config->getTagDoubleDefault("textSizeX",config->getTagDoubleDefault("textSize",0.04));
        double textSizeY = config->getTagDoubleDefault("textSizeY",config->getTagDoubleDefault("textSize",0.04));
        int vSize = nRegions*nSamples*9;
        gStyle->SetOptStat(false);
        gStyle->SetOptTitle(false);
        TCanvas* c = new TCanvas("SystematicsSignificance","SystematicsSignificance");
        vSize = vSize > (int)c->GetWh() ? vSize : c->GetWh();
        c->SetCanvasSize( c->GetWw()*(1.+lMargin) , vSize);
        c->SetLeftMargin(lMargin);
        c->cd();
        hDown->SetMaximum(config->getTagDoubleDefault("max",2.));
        hDown->GetXaxis()->SetLabelSize(textSizeX);
        hDown->GetYaxis()->SetLabelSize(textSizeY);
        hDown->LabelsDeflate("X");
        hDown->LabelsDeflate("Y");
        hDown->LabelsOption("v");
        hDown->Draw("colz");
        c->Update();
        c->SaveAs(TQPathManager::getPathManager()->getTargetPath(filename+"_down"+extension).c_str());
        delete c;
        c = new TCanvas("SystematicsSignificance","SystematicsSignificance");
        c->SetCanvasSize( c->GetWw()*(1.+lMargin) , vSize);
        c->SetLeftMargin(lMargin);
        c->cd();
        hUp->SetMaximum(config->getTagDoubleDefault("max",2.));
        hUp->GetXaxis()->SetLabelSize(textSizeX);
        hUp->GetYaxis()->SetLabelSize(textSizeY);
        hUp->LabelsDeflate("X");
        hUp->LabelsDeflate("Y");
        hUp->LabelsOption("v");
        hUp->Draw("colz");
        c->Update();
        c->SaveAs(TQPathManager::getPathManager()->getTargetPath(filename+"_up"+extension).c_str());
        delete c;
        c = new TCanvas("SystematicsSignificance","SystematicsSignificance");
        c->SetCanvasSize( c->GetWw()*(1.+lMargin) , vSize);
        c->SetLeftMargin(lMargin);
        c->cd();
        hMax->SetMaximum(config->getTagDoubleDefault("max",2.));
        hMax->GetXaxis()->SetLabelSize(textSizeX);
        hMax->GetYaxis()->SetLabelSize(textSizeY);
        hMax->LabelsDeflate("X");
        hMax->LabelsDeflate("Y");
        hMax->LabelsOption("v");
        hMax->Draw("colz");
        c->Update();
        c->SaveAs(TQPathManager::getPathManager()->getTargetPath(filename+"_max"+extension).c_str());
        delete c;
      }
      delete hUp;
      delete hDown;
      delete hMax;
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new PlotSystematicsSignificance(),"PlotSystematicsSignificance");
  }
}
    
