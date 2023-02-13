#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQSampleDataReader.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSSystematicsManager.h"

/*<cafdoc name=ImportSystematics>
  ImportSystematics
  ===========================
  
  Import a set of externally defined systematic variations into a
  model.  This can be used to add systematics obtained by external
  sources, e.g. theory systematics, generator variations, systematics
  evaluated on truth samples, etc.

  Usage:
  ---------------

  This action is most commonly used with the `$include` keyword, importing the systematics from external files. These can be `txt` or `root` files.

  ```
  +ImportSystematics {
    +HWWRun2GGF {
      # new run2 ggH systematics
      $include("./auxData/theorySystematics/ggF/ggH.txt");
    }
  }
  ```

  The aforementioned file `./auxData/theorySystematics/ggF/ggH.txt` would in this case be formatted like this:

  ```
  # any top-level folder will be added as a new systematic uncertainty
  +theo_QCD_scale {
    +SR_0j_DF_all {
      # the "Region" selector can be used to define on which regions
      # this systematic should act. If not given, the folder name
      # ("SR_0j_DF_all") is used as a region selector.
      <Region = "Sample.sigGGF*jet*">
      +ggf {
        # the "Sample" selector can be used to define on which samples
        # this systematic should act. If not given, the folder name
        # ("ggf") is used as a sample selector.
        <Sample = "Sample.sigGGF*jet*">
        # the most basic type of systematic is an overall systematic,
        # providing a "High" and "Low" variation as well as a nominal
        # "Val"
        <IsOverallSys = true>
        <Val = 1, High = 1.0014, Low = 0.99875> 
        # some additional information can be provided to enable some
        # of the more advanced plotting and pruning methods of the
        # framework, but are not needed if the aim is to just have
        # this systematic included in the fit "as-is"
        <HighErr = 0.00522428, LowErr = 0.00519054, Percent = 5.26967, Entries = 57697>
        # additionally, shape systematics ("HistoSys") can be added as histograms
        <IsHistoSys = true>
        # histograms are added with the names "HistoHigh", "HistoLow" and "HistoNominal" in the typical TQFolder syntax
        TH1F("HistoHigh", "sometitle", 50, 80, 130) << 57697@{27.7231+-0.604252,2.91253+-0.177788,...};
        TH1F("HistoLow",  "sometitle", 50, 80, 130) << ...;
        TH1F("HistoNominal",  "sometitle", 50, 80, 130) << ...;
      }
      +vbf {
        ...
      }
    }
    +SR_1j_DF_all {
      ...
    }
    ...
  }
  +someothersystematic {
    ...
  }
  ...
  ```

  Of course, all of this can also be added directly into the main config file like this
  +ImportSystematics {
    +HWWRun2GGF {
      # new run2 ggH systematics
      +theo_QCD_scale {
        ...
      }
    }
  }


  However, in practice, these files are rarely created by
  hand. Instead, a script like the following can be used to prepare
  them.  In this example we compare three different parton shower
  radiation samples with each other, where it is assumed that the
  corresponding histograms and counters are already provided as part
  of some sample folder.

  ```
  #!/usr/bin/env python2

  def extractSystematicUpDown(syst,reader,cut,basepath,nom,up,down,observable):
    # extract an up/down systematic
    # similar functions for single-sided systematics can be written easily

    # overall systematic
    cntnom  = reader.getCounter(TQFolder.concatPaths(basepath,nom ),cut)
    cntup   = reader.getCounter(TQFolder.concatPaths(basepath,up  ),cut)
    cntdown = reader.getCounter(TQFolder.concatPaths(basepath,down),cut)
    if not cntnom:
      raise(RuntimeError("unable to obtain nominal counter '{:s}'/'{:s}' for cut '{:s}'".format(basepath,nom,cut)))
    if not cntup:
      raise(RuntimeError("unable to obtain variation counter '{:s}'/'{:s}' for cut '{:s}'".format(basepath,up,cut)))
    if not cntdown:
      raise(RuntimeError("unable to obtain variation counter '{:s}'/'{:s}' for cut '{:s}'".format(basepath,down,cut)))
    if cntnom.getRawCounter() == 0 or cntup.getRawCounter() == 0 or cntdown.getRawCounter() == 0:
      return
    # these bits are required
    syst.setTagBool("IsOverallSys",True)
    syst.setTagDouble ("Val",     1.)
    syst.setTagDouble ("High",    cntup.getCounter()/cntnom.getCounter())
    syst.setTagDouble ("Low",     cntdown.getCounter()/cntnom.getCounter())
    # these are just for decoration
    syst.setTagDouble ("HighErr", m.sqrt( (cntup.getError()  /cntnom.getCounter())**2 + (cntnom.getError()/cntnom.getCounter())**2 * (cntup.getCounter()   / cntnom.getCounter())**2  ))
    syst.setTagDouble ("LowErr",  m.sqrt( (cntdown.getError()/cntnom.getCounter())**2 + (cntnom.getError()/cntnom.getCounter())**2 * (cntdown.getCounter() / cntnom.getCounter())**2  ))
    syst.setTagInteger("Entries", max(cntup.getRawCounter(),cntdown.getRawCounter()))
    syst.setTagDouble ("Percent", 100.*max((cntup.getCounter()-cntnom.getCounter())/cntnom.getCounter(),(cntdown.getCounter()-cntnom.getCounter())/cntnom.getCounter()))
  
    # shape systematic
    histnom = reader.getHistogram(TQFolder.concatPaths(basepath,nom ),TQFolder.concatPaths(cut,observable),"normalize=true")
    if histnom:
      syst.setTagBool("IsHistoSys",True)
      histup   = reader.getHistogram(TQFolder.concatPaths(basepath,up),  TQFolder.concatPaths(cut,observable),"normalize=true")
      histdown = reader.getHistogram(TQFolder.concatPaths(basepath,down),TQFolder.concatPaths(cut,observable),"normalize=true")
      histnom.SetName("HistoNominal")
      histup.SetName("HistoHigh")
      histdown.SetName("HistoLow")
      syst.addObject(histnom)
      syst.addObject(histup)
      syst.addObject(histdown)
    else:
      print("no shape histograms found!")
  
  def main(args):
    import QFramework
    samples = QFramework.TQSampleFolder.loadSampleFolder(args.inputfile)
    reader = QFramework.TQSampleDataReader(samples)
  
    basepath = "bkg/[em+me]/top/singletop/Wt"
  
    for cut in args.cutnames:
      outfolder = outfolder.getFolder(TQFolder.concatPaths("theo_Wt_PS_radiation",cut,"Wt")+"+"),
      extractSystematicUpDown(reader,cut,basepath,"[410015+410016]","[410103+410105]","[410104+410106]",args.observable)
  
    if args.outputfile.endswith("txt"):
      outfolder.exportToTextFile(args.outputfile,True)
    elif args.outputfile.endswith("root"):
      outfolder.writeToFile(args.outputfile)                     
  
  if __name__ == "__main__":
    # parse the CLI arguments
    import argparse
    parser = argparse.ArgumentParser(description='extract top generator systematics')
    parser.add_argument('--input', '-i', metavar='INPUT', type=str, dest="inputfile", help='ROOT file with input sample folder')
    parser.add_argument('--output', '-o', metavar='OUTPUT', type=str, dest="outputfile", required=True, help='output text file for the systematics')
    parser.add_argument('--cutnames', '-c', metavar='*CUT', type=str, dest="cutnames", nargs="+", default=["*SR","*CR"], help='select cuts to extract systematics at')
    parser.add_argument('--observable', metavar='obs', type=str, dest="observable", default="MTfit", help='name of the observable to be used for shapes')
    args = parser.parse_args()
  
    # setup ROOT
    import ROOT
    ROOT.gROOT.SetBatch(True)
    import QFramework
    QFramework.TQLibrary.getQLibrary().setApplicationName("Top Generator Systematics");
  
    # call the main function
    main(args);

  ```



</cafdoc> */
namespace TSBaseActions {

  class ImportSystematics : public TSStatisticsManager::Action {
    void normalize(TH1* h, TH1* nom, TH1* othernom) const{
      if(!TQHistogramUtils::checkConsistency(h,nom,true)){
        throw std::runtime_error("encountered inconsistent histograms: variation vs. nominal!");
      }
      if(othernom){
        if(!TQHistogramUtils::checkConsistency(h,othernom,true)){
          throw std::runtime_error("encountered inconsistent histograms: variation vs. alternate nominal!");
        }
        h->Divide(othernom);
        h->Multiply(nom);
      }
      TQHistogramUtils::normalize(h,nom);
    }

    bool execute(TQFolder * config) const override {

      TQFolder* model = models()->getFolder(config->GetName());
      if(!model){
        manager->error(TString::Format("no such model available: '%s'",config->GetName()));
        return false;
      }

      TQSampleDataReader helper;
      
      bool fixHoles = config->getTagDoubleDefault("fixHoles",false);
      double threshold = std::numeric_limits<double>::epsilon();

      TSSystematicsManager man(model->getFolder(".Histograms+"));
      
      TQFolderIterator itrsys(config->getListOfFolders("?"),true);
      while(itrsys.hasNext()){
        TQFolder* systematic = itrsys.readNext();
        if(!systematic) continue;
        model->getFolder(".Configuration/Systematics+")->getFolder(systematic->getName()+"+")->setTagBool("imported",true);
        TQFolderIterator itrreg(systematic->getListOfFolders("?"),true);
        bool isShape = false;
        bool isOverall = false;
        while(itrreg.hasNext()){
          TQFolder* region = itrreg.readNext();
          if(!region) continue;
          TString regExp(region->getTagStringDefault("~Channel","Channel."+region->getName()));
          TQFolderIterator itrsamp(region->getListOfFolders("?"),true);
          TCollection* targetChannels = model->getListOfFolders(regExp);
          if(!targetChannels || targetChannels->GetSize() == 0){
            manager->error(TString::Format("unable to get channel '%s'",regExp.Data()));
            continue;
          }
          TQFolderIterator itrregTarget(targetChannels,true);
          while(itrregTarget.hasNext()){
            TQFolder* targetChannel = itrregTarget.readNext();
            if(!targetChannel) continue;
            TString targetChannelName = targetChannel->getTagStringDefault("Channel","");
            itrsamp.reset();
            while(itrsamp.hasNext()){
              TQFolder* sample = itrsamp.readNext();
              if(!sample) continue;
              TString sampExp(sample->getTagStringDefault("~Sample","Sample."+sample->getName()));
              TCollection* targetSamples = targetChannel->getListOfFolders(sampExp);
              if(!targetSamples || targetSamples->GetSize() == 0){
                manager->error(TString::Format("unable to get sample '%s' for channel '%s'",sampExp.Data(),regExp.Data()));
                continue;
              }
              TQFolderIterator itrsampTarget(targetSamples,true);
              while(itrsampTarget.hasNext()){
                TQFolder* targetSample = itrsampTarget.readNext();
                if(!targetSample) continue;
                TString targetSampleName = targetSample->getTagStringDefault("Sample","");
                if(sample->getTagBoolDefault("~IsOverallSys",true)){
                  TQFolder* overallSys = targetSample->getFolder("OverallSys."+systematic->getName()+"+");
                  overallSys->importTags(sample);
                  isOverall = true;
                }
                if(sample->getTagBoolDefault("~IsHistoSys",false)){
                  TH1* nom = man.getVarHisto("Nominal", targetChannelName, targetSampleName);
                  TH1* high = dynamic_cast<TH1*>(sample->getObject("HistoHigh"));
                  TH1* low  = dynamic_cast<TH1*>(sample->getObject("HistoLow"));
                  TH1* othernom  = dynamic_cast<TH1*>(sample->getObject("HistoNominal"));
                  TH1* myothernom  = NULL;
                  bool ownothernom = false;
                  TH1* myhigh  = NULL;
                  TH1* mylow  = NULL;
                  TH1* myrelhigh  = NULL;
                  TH1* myrellow  = NULL;
                  if(nom && nom->GetNbinsX()>1 && high && low){
                    TQFolder* histoSys = targetSample->getFolder("HistoSys."+systematic->getName()+"+");
                    
                    bool doUp   = (man.getSysHisto(TQFolder::concatPaths(systematic->getName(),"Up",    targetChannelName, targetSampleName), false)==NULL);
                    bool doDown = (man.getSysHisto(TQFolder::concatPaths(systematic->getName(),"Down",  targetChannelName, targetSampleName), false)==NULL);
                    
                    if(doUp || doDown){
                      helper.processHistogramOptions(high,targetSample);
                      helper.processHistogramOptions(low,targetSample);
                      if(othernom) helper.processHistogramOptions(othernom,targetSample);

                      if(targetSample->getTagBoolDefault("~.Remapped",false)){
                        std::vector<int> binning = targetSample->getTagVInteger("~.Remapping.Bins");
                        if(doUp){
                          myhigh = TQHistogramUtils::getRebinned(high,binning,true);
                        }
                        if(doDown){
                          mylow = TQHistogramUtils::getRebinned(low,binning,true);
                        }
                        myothernom  = TQHistogramUtils::getRebinned(othernom,binning,true);
                        ownothernom = false;

                        std::vector<int> smoothedBinning = region->getTagVInteger("~Smoothing.SmoothBins");
                        if(!smoothedBinning.empty()) {
                          manager->info(TString::Format("smooth histogram for systematic/region '%s/%s'", systematic->GetName(), region->GetName()));
                          if(doUp)   myhigh = TQHistogramUtils::getSmoothedManually(myhigh,smoothedBinning);
                          if(doDown) mylow = TQHistogramUtils::getSmoothedManually(mylow,smoothedBinning);
                          myothernom  = TQHistogramUtils::getSmoothedManually(myothernom,smoothedBinning);
                          ownothernom = false;
                        }
                        
                      } else {
                        if(doUp){
                          myhigh = TQHistogramUtils::copyHistogram(high);
                        }
                        if(doDown){
                          mylow  = TQHistogramUtils::copyHistogram(low);
                        }
                        myothernom = othernom;
                      }

                      if(myothernom && fixHoles){
                        try {
                          int fixed = TQHistogramUtils::fixHoles1D(myhigh,threshold);
                          if(fixed > 0){
                            manager->warn(TString::Format("fixed %d holes in nominal histogram for variation '%s' in channel '%s' for sample '%s'",fixed,systematic->GetName(),targetChannelName.Data(),targetSampleName.Data()));
                          }
                        } catch(std::runtime_error& err){
                          // do nothing;
                        }
                      }
                      
                      if(doUp && myhigh){
                        if(fixHoles){
                          try {
                            int fixed = TQHistogramUtils::fixHoles1D(myhigh,threshold);
                            if(fixed > 0){
                              manager->warn(TString::Format("fixed %d holes in up-histogram for variation '%s' in channel '%s' for sample '%s'",fixed,systematic->GetName(),targetChannelName.Data(),targetSampleName.Data()));
                            }
                          } catch(std::runtime_error& err){
                            // do nothing;
                          }
                        }
                        normalize(myhigh,nom,myothernom);
                        myrelhigh = TQHistogramUtils::copyHistogram(myhigh);
                        myrelhigh->Divide(nom);

                        // rename hists
                        myhigh->SetName(targetSampleName);
                        myrelhigh->SetName(targetSampleName);
                       
                        histoSys->setTagString("HistoHigh",man.storeSysHisto(myhigh,systematic->getName(),"Up",  targetChannelName));
                        histoSys->setTagString("HistoRelHigh",man.storeSysHisto(myrelhigh,systematic->getName(),"RelUp",  targetChannelName));
                      }
                      if(doDown && mylow){
                        if(fixHoles){
                          try {
                            int fixed = TQHistogramUtils::fixHoles1D(mylow,threshold);
                            if(fixed > 0){
                              manager->warn(TString::Format("fixed %d holes in down-histogram for variation '%s' in channel '%s' for sample '%s'",fixed,systematic->GetName(),targetChannelName.Data(),targetSampleName.Data()));
                            }
                          } catch(std::runtime_error& err){
                            // do nothing;
                          }
                        }
                        normalize(mylow,nom,myothernom);
                        myrellow = TQHistogramUtils::copyHistogram(mylow);
                        myrellow->Divide(nom);
                        
                        // rename hists
                        mylow->SetName(targetSampleName);
                        myrellow->SetName(targetSampleName);
                        
                        histoSys->setTagString("HistoLow",man.storeSysHisto(mylow,systematic->getName(),"Down",  targetChannelName));
                        histoSys->setTagString("HistoRelLow",man.storeSysHisto(myrellow,systematic->getName(),"RelDown",  targetChannelName));                        
                      }
                      isShape = true;
                      if(ownothernom) delete myothernom;
                    }
                  } else {
                    manager->warn(TString::Format("no histograms found for variation '%s' in channel '%s' for sample '%s'",systematic->GetName(),targetChannelName.Data(),targetSampleName.Data()));
                  }
                }
              }
            }
          }
        }
        if(isShape && isOverall){
          manager->info(TString::Format("imported '%s' as overall & shape systematic",systematic->GetName()));
        } else if(isShape){
          manager->info(TString::Format("imported '%s' as shape systematic",systematic->GetName()));
        } else if(isOverall){
          manager->info(TString::Format("imported '%s' as overall systematic",systematic->GetName()));
        } else {
          manager->info(TString::Format("imported '%s' with mixed properties",systematic->GetName()));
        }
      }
      model->sortByNameRecursive();      
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new ImportSystematics(),"ImportSystematics");
  }
}
