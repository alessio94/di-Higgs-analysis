#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQPathManager.h"

#include <fstream>

#include "SFramework/TSStatisticsManager.h"

#include "TFile.h"

namespace TSBaseActions {

  class PrintResult : public TSStatisticsManager::Action {


    bool execute(TQFolder * config) const override {

      TQFolder* result = results()->getFolder(config->GetName());
      if(!result){
        manager->error(TString::Format("no such result available: '%s'",config->GetName()));
        return false;
      }

      TString filename;
      std::ofstream outfile;
      bool write = config->getTagString("writeToFile",filename);
      if(write){
        outfile.open(TQPathManager::getPathManager()->getTargetPath(filename).c_str());

        outfile << "\\documentclass{standalone}\n";
        outfile << "\\usepackage{tikz}\n";
        outfile << "\\usetikzlibrary{calc,arrows.meta,decorations.markings,math,patterns}\n";
        outfile << "\\begin{document}\n";
        outfile << "\\begin{tabular}{|l l|}\n";
      }
      
      double normalizeTo = config->getTagDoubleDefault("normalize",0.);
      
      
      TQFolderIterator pois(config->getListOfFolders("?"),true);
      while(pois.hasNext()){
        TQFolder* poi = pois.readNext();
        if(!poi) continue;
        TString name = poi->getTagStringDefault("POI",poi->GetName());

        
        TString valSource;
        if(!poi->getTagString("value",valSource)){
          manager->error(TString::Format("no source given for value of parameter '%s'",name.Data()));
          continue;
        }
            
        TQFolder* nominal = result->getFolder(valSource);
        if(!nominal){
          manager->error(TString::Format("invalid source given for value of parameter '%s': '%s'",name.Data(),valSource.Data()));
          continue;
        }   

        name.ReplaceAll("_"," ");

        std::stringstream ss;

        double value;
        if(!nominal->getTagDouble("val",value)){
          manager->error(TString::Format("unable to read value of parameter from '%s'",valSource.Data()));
          continue;
        }
        double normFactor = (normalizeTo>0.) && (value>0.) ? normalizeTo/value : 1.;
        double err = nominal->getTagDoubleDefault("err",0);
        
        ss << value*normFactor << "^{+" << fabs(nominal->getTagDoubleDefault("errHigh",err)*normFactor) << "}_{-" << fabs(nominal->getTagDoubleDefault("errLow",err)*normFactor) << "} (tot)";
        
        TCollection* uncSources = poi->getListOfKeys("uncert.*");
        if(uncSources && uncSources->GetEntries() > 0){
          ss << " = " << value*normFactor;
          TQIterator itr(uncSources,true);
          while(itr.hasNext()){
            TObject* uncSource = itr.readNext();
            if(!uncSource) continue;
            TString uncKey(uncSource->GetName());
            TString uncName(uncKey(7,uncKey.Length()-7));
            TString path;
            if(!poi->getTagString(uncKey,path)){
              manager->error(TString::Format("unable to read uncertainty source for '%s'",uncKey.Data()));
              continue;
            }
            TQFolder* unc = result->getFolder(path);
            if(!unc){
              manager->error(TString::Format("invalid source given for uncertainty '%s' on parameter '%s': '%s'",uncKey.Data(),name.Data(),path.Data()));
              continue;
            } 
            double high = unc->getTagDoubleDefault("High",0);
            double low = unc->getTagDoubleDefault("Low",0);
            ss << "^{+" << fabs(high*normFactor) << "}_{-" << fabs(low*normFactor) << "}" << "(" << uncName << ")";
          }
        }

        manager->info(TString::Format("Result for '%s': %s", name.Data(),ss.str().c_str()));
        if(write) outfile << "${" << name.Data() << "}$ & $" << ss.str().c_str() << "$\\tabularnewline\n";
        if(write) outfile << "\\[0.10cm]\n";
      }
      if(write){
        outfile << "\\end{tabular}\n";
        outfile << "\\end{document}\n";
        outfile.close();
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new PrintResult(),"PrintResults");
  }
}
    
