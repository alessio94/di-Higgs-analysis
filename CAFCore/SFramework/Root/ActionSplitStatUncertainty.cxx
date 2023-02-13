#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQHistogramUtils.h"

#include "SFramework/TSStatisticsManager.h"

namespace TSBaseActions {

  class SplitStatUncertainty : public TSStatisticsManager::Action {

    bool execute(TQFolder * config) const override {

      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
      }
      //small helper vector used later
      std::vector<TString> updown; updown.push_back(TString("__1up"));updown.push_back(TString("__1down"));
      
      bool doHistOnly = true;
      bool doShape = true;
      
      TQFolderIterator itr(config->getListOfFolders());
      //loop over all sub-configs (one per process we want to split off)
      while(itr.hasNext()) {
        TQFolder* subconfig = itr.readNext();
        TString processName = "";
        if (!subconfig->getTagString("sample",processName)) {
          manager->error("No process given to process!");
          continue;
        }
        std::vector<TString> chanFilter = subconfig->getTagVString("channelFilter");
        if (chanFilter.size() < 1) chanFilter.push_back(subconfig->getTagStringDefault("channelFilter","*"));
        TList* channels = new TList(); channels->SetOwner(false);
        for (size_t i=0; i<chanFilter.size();i++) {
          TQStringUtils::ensureLeadingText(chanFilter[i],"Channel.");
          TList* chans = model->getListOfFolders(chanFilter[i]);
          channels->AddAll(chans);
          delete chans;
        }
        manager->info(TString::Format("Splitting statistical uncertainties for sample '%s'",processName.Data()).Data());
        channels->Print();
        std::cout<<"Process name: '"<<processName.Data()<<"'\n";
        TQStringUtils::ensureLeadingText(processName,"Sample.");
        //int binGlobal = 0; //"global" counter to create unique names for new systematics
        TQFolderIterator citr(channels);
        //loop over all requested channels
        while(citr.hasNext()) {
          TQFolder* thisChannel = citr.readNext();
          if (!thisChannel) {
            manager->error("Failed to retrieve channel folder");
            continue;
          }
          TString channelName = thisChannel->getName();
          manager->info(TString::Format("Processing channel '%s'",channelName.Data()).Data());
          TQStringUtils::removeLeadingText(channelName,"Channel.");
          TQFolder* thisSample = thisChannel->getFolder(processName);
          if (!thisSample) {
            manager->error("Failed to retrieve sample folder");
          }
          TString sampleName = thisSample->getName();
          TQStringUtils::removeLeadingText(sampleName,"Sample.");
          
          //get the nominal histogram folder
          TQFolder* nominal = model->getFolder(TQFolder::concatPaths(".Histograms/Nominal/",channelName));
          if (!nominal) {
            manager->error("Failed to obtain nominal histogram folder");
            return false;
          }
          TH1* hNominal = dynamic_cast<TH1*>(nominal->getObject(sampleName));
          double intNominal = hNominal->Integral();
          if (!hNominal) {
            manager->error(TString::Format("Failed to retrieve nominal histogram for sample '%s' in channel '%s'",sampleName.Data(),channelName.Data()).Data());
            continue;
          }
          int nBins = TQHistogramUtils::getNBins(hNominal);
          
          if (doShape) { //insert statistical uncertainties as 'shapeSys'
            TString systName = TString::Format("gamma%s_%s",sampleName.Data(),channelName.Data());
            TQFolder* variation = model->getFolder(TQFolder::concatPaths(".Histograms/Systematics",systName,"Shape",channelName)+"+");
            TH1* hShape = static_cast<TH1*>(hNominal->Clone());
            for (int bin=0; bin< nBins+1; bin++) {
              if (hNominal->GetBinContent(bin) > 0) {
                hShape->SetBinContent(bin, hNominal->GetBinError(bin)/hNominal->GetBinContent(bin));
                hShape->SetBinError(bin,0.);
              } else {
                hShape->SetBinContent(bin, 0.);
                hShape->SetBinError(bin,0.);
              }
            }
            hShape->SetDirectory(0);   variation->addObject(hShape);
            TQFolder* shapeSys = thisSample->getFolder("ShapeSys."+systName+"+");
            shapeSys->setTagString("Histo",TQFolder::concatPaths(TQFolder::concatPaths("Systematics",systName,"Shape",channelName),hShape->GetName()) );
            shapeSys->setTagBool("usePoisson",true); //use poisson constraint
            
            TQFolder* settings = model->getFolder(TQFolder::concatPaths("ParamSettings",systName)+"+");
            if (!settings) throw std::runtime_error("Failed to set constraint type");
            settings->setTag("ConstraintTerm","Poisson");
            
          } else { //insert statistical uncertainties as 'histoSys' (optional: normalize histoSys component and add separate, but correlated, 'overallSys')
            for (int bin=0; bin< nBins+1; bin++) {
              if (hNominal->GetBinContent(bin) < 2*std::numeric_limits<double>::epsilon() ) continue; //skip empty bins (meant for under/overflow bins)
              TString systName = TString::Format("gamma%s_%s_bin%d",sampleName.Data(),channelName.Data(),bin);
              //we copy the entire set of nominal histograms, not sure if this is needed:
              TQFolder* variation_up = model->getFolder(TQFolder::concatPaths(".Histograms/Systematics",systName,"Up",channelName)+"+");
              //nominal->copy();
              //if (!model->addFolder(variation_up,TQFolder::concatPaths(".Histograms/Systematics",systName,"Up")+"+")) {
              //  manager->error("Failed to add variation histogram folder");
              //  return false;
              //}
              TQFolder* variation_down = model->getFolder(TQFolder::concatPaths(".Histograms/Systematics",systName,"Down",channelName)+"+");
              //nominal->copy();
              //if (!model->addFolder(variation_down,TQFolder::concatPaths(".Histograms/Systematics",systName,"Down")+"+")) {
              //  manager->error("Failed to add variation histogram folder");
              //  return false;
              //}
              
              TH1* hUp = static_cast<TH1*>(hNominal->Clone());
              //dynamic_cast<TH1*>(variation_up->getObject(sampleName)); //we get a direct pointer to the internal object, don't delete it!
              TH1* hDown = static_cast<TH1*>(hNominal->Clone());
              //dynamic_cast<TH1*>(variation_down->getObject(sampleName)); //we get a direct pointer to the internal object, don't delete it!
              if (!hUp || !hDown) {
                throw std::runtime_error("Failed to clone nominal histogram");
              }
              hUp->SetDirectory(0);   variation_up->addObject(hUp);
              hDown->SetDirectory(0); variation_down->addObject(hDown);
              
              
              
              //modify the one bin that we are interested in right now, everything else stays at nominal values
              hUp->SetBinContent(bin,hNominal->GetBinContent(bin)+hNominal->GetBinError(bin));
              hDown->SetBinContent(bin,hNominal->GetBinContent(bin)-hNominal->GetBinError(bin));
              
              double intUp =  hUp->Integral();
              double intDown = hDown->Integral();
              if (! (intUp>0 && intDown>0) ) {
                manager->error("At least one variation creates a negative integral. This is prone to cause problems, please ensure your statistical uncertainties are smaller than the corresponding bin content.");
                return false;
              }
              //determine overallSys component
              double up = intNominal>0 ? (intUp-intNominal)/intNominal : 0.;
              double down =  intNominal>0 ? (intDown-intNominal)/intNominal : 0.;
              //revert normalization to nominal for histoSys component
              if (!doHistOnly) {
                hUp->Scale(intNominal/intUp);
                hUp->Scale(intNominal/intDown);
                //Store the overallSys part:
                TQFolder* overallSys = thisSample->getFolder("OverallSys."+systName+"+");
                overallSys->setTagDouble("Val",1.);
                overallSys->setTagDouble("High",up);
                overallSys->setTagDouble("Low",down);
              }
              
              //create a histoSys part:
              TQFolder* histoSys = thisSample->getFolder("HistoSys."+systName+"+");
              histoSys->setTagString("HistoHigh",TQFolder::concatPaths(TQFolder::concatPaths("Systematics",systName,"Up",channelName),hUp->GetName()) );
              histoSys->setTagString("HistoLow",TQFolder::concatPaths(TQFolder::concatPaths("Systematics",systName,"Down",channelName),hDown->GetName()));
              
              //set constraint type
              TQFolder* settings = model->getFolder(TQFolder::concatPaths("ParamSettings",systName)+"+");
              if (!settings) throw std::runtime_error("Failed to set constraint type");
              settings->setTag("ConstraintTerm","Poisson");
            }
          }
          thisSample->setTagBool("ActivateStatError",false); //we have now split it from the regular stat. uncertainty
          
          
        }
        
        
        
      }
      
          
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new SplitStatUncertainty(),"SplitStatUncertainty");
  }
}
