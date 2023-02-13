#include "QFramework/TQSystematicsHandler.h"
#include "QFramework/TQHistogramUtils.h"
#include "QFramework/TQUtils.h"
#include "QFramework/TQIterator.h"

// #define _DEBUG_

#include "QFramework/TQLibrary.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
// TQSystematicsHandler:
//
// With the advent of the TQSystematicsHandler, it is now easily
// possible to include not only normalization, but also shape
// systematics into histograms produced by a plotter or add systematic
// uncertainties to the cutflow uncertainties.
//
////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(TQSystematicsHandler)

TQSystematicsHandler::TQSystematicsHandler(const TString& name) :
TQSystematicsManager(new TQFolder("histograms")),TQFolder(name),
  _config(new TQFolder("systematicsHandler"))
{
	// constructor with name
}

TQSystematicsHandler::TQSystematicsHandler(const TString& name, TQFolder* cfg) :
TQSystematicsManager(new TQFolder("histograms")),TQFolder(name),  
  _config(cfg)
{
	// constructor with name and config
}

TQFolder* TQSystematicsHandler::config(){
  return this->_config;
}

TQFolder* TQSystematicsHandler::addCut(const TString& id){
  // add a cut to the systematics handler
  if(!TQFolder::isValidName(id)){
    ERRORclass("'%s' is not a valid identifier!",id.Data());
    return NULL;
  }
  TQFolder* cut = config()->getFolder("Cuts+")->getFolder(id+"+");
  cut->setTagString("Cut",id);
  return cut;
}

TQFolder* TQSystematicsHandler::addSystematic(const TString& id, const TString& tags){
  // add a systematic to the systematics handler
  if(!TQFolder::isValidName(id)){
    ERRORclass("'%s' is not a valid identifier!",id.Data());
    return NULL;
  }
  TQFolder* sys = config()->getFolder("Systematics+")->getFolder(id+"+");
  sys->importTags(tags);
  return sys;
}

TQFolder* TQSystematicsHandler::addSystematic(const TString& id, const TString& upvar, const TString& dnvar){
  // add a systematic to the systematics handler
  if(!TQFolder::isValidName(id)){
    ERRORclass("'%s' is not a valid identifier!",id.Data());
    return NULL;
  }
  TQFolder* sys = config()->getFolder("Systematics+")->getFolder(id+"+");
  sys->setTagString("Up",upvar);
  sys->setTagString("Down",dnvar);  
  return sys;
}


TQFolder* TQSystematicsHandler::addVariation(const TString& id, const TString& tags){
  // add a variation to the systematics handler
  if(!TQFolder::isValidName(id)){
    ERRORclass("'%s' is not a valid identifier!",id.Data());
    return NULL;
  }
  TQFolder* var = config()->getFolder("Variations+")->getFolder(id+"+");
  var->importTags(tags);
  return var;
}

TQFolder* TQSystematicsHandler::addSample(const TString& id, const TString& path){
  // add a sample to the systematics handler
  if(!TQFolder::isValidName(id)){
    ERRORclass("'%s' is not a valid identifier!",id.Data());
    return NULL;
  }
  TQFolder* sample = config()->getFolder("Samples+")->getFolder(id+"+");
  sample->setTagString("Path",path);
  return sample;
}

void TQSystematicsHandler::collectHistograms(TQSampleDataReader* rd, TQFolder* variation){
  // collect the counters from a reader for one subpath of the type systname/file.root/
  TQFolder* cuts = this->config()->getFolder("Cuts");
  TQFolderIterator sampleItr(this->config()->getListOfFolders("Samples/?"));
  while(sampleItr.hasNext()){
    TQFolder* sampleConfig = sampleItr.readNext();
    TString sfolderpath = variation->replaceInText(sampleConfig->getTagStringDefault("Path",""));
    if(sfolderpath.Contains("$")){    
      ERROR("sample '%s' has unexpanded alias in path: '%s'. skipping.",sampleConfig->GetName(),sfolderpath.Data());
      continue;
    }
    TQIterator histoItr(rd->getListOfHistogramNames(sfolderpath),true);
    if(histoItr.getCollection()->GetSize() > 0){
      std::map<TString,std::vector<TString> > histoNames;
      while(histoItr.hasNext()){
        TString histo(histoItr.readNext()->GetName());
        TH1* h = rd->getHistogram(sfolderpath,histo);
        TString cutname=TQFolder::getPathHead(histo);
        if(cuts && !cuts->getFolder(cutname)) continue;
        if(histoNames.find(cutname) == histoNames.end()) histoNames[cutname] = std::vector<TString>();
        histoNames[cutname].push_back(histo);
        h->SetName(sampleConfig->getName()+"."+histo);
        this->storeVarHisto(h,variation->GetName(),cutname);
      }
      for(auto histo:histoNames){
        TQFolder* cut = this->getFolder(histo.first+"+");
        cut->setTagList("Shapes",histo.second);
      }
    }
  }
}

void TQSystematicsHandler::collectCounters(TQSampleDataReader* rd, TQFolder* variation){
  // collect the counters from a reader for one subpath of the type systname/file.root/
  TQFolder* cuts = this->config()->getFolder("Cuts");  
  TQFolderIterator sampleItr(this->config()->getListOfFolders("Samples/?"));
  while(sampleItr.hasNext()){
    TQFolder* sampleConfig = sampleItr.readNext();
    TString sfolderpath = variation->replaceInText(sampleConfig->getTagStringDefault("Path",""));
    if(sfolderpath.Contains("$")){
      ERROR("sample '%s' has unexpanded alias in path: '%s'. skipping.",sampleConfig->GetName(),sfolderpath.Data());
      continue;
    }    
    DEBUGclass("retrieving counters for sample '%s' from path '%s'",sampleConfig->GetName(),sfolderpath.Data());
    TQIterator cntItr(rd->getListOfCounterNames(sfolderpath),true);
    if(cntItr.getCollection()->GetSize() == 0){
      ERRORclass("unable to retrieve list of counters from sample '%s'",sfolderpath.Data());
    } else {
      while(cntItr.hasNext()){
        TString cutname(cntItr.readNext()->GetName());
        if(cuts && !cuts->getFolder(cutname)) continue;        
        TQCounter* cnt = rd->getCounter(sfolderpath,cutname);
        if(cnt->getRawCounter()>0){
          this->getFolder(cutname+"+")->setTagString("Cut",cutname);
          cnt->SetName(sampleConfig->getName()+".yield");      
          TH1* h = TQHistogramUtils::counterToHistogram(cnt);
          this->storeVarHisto(h,variation->GetName(),cutname);
        }
        delete cnt;
      }
    }
  }
}

TQSampleFolder* TQSystematicsHandler::getSampleFolder(const TString& name){
  return this->getSampleFolder(this->config()->getFolder("Variations")->getFolder(name));
}

TQSampleFolder* TQSystematicsHandler::getSampleFolder(TQFolder* var){
  // retrieve the sample folder for the variation given
  if(!var) return NULL;
  TString path = var->getTagStringDefault("~SampleFolder","");
  if(!path){
    ERROR("unable to find sample folder for variation '%s'",var->GetName());
  }

  TQSampleFolder* sf = NULL;
  auto input = _inputCache.find(path);
  if(input == _inputCache.end()){
    sf = TQSampleFolder::loadSampleFolder(path);
    _inputCache[path] = sf;
  } else {
    sf = input->second;
  }
  
  if(!sf){
    ERRORclass("unable to load sample folder from file '%s'",path.Data());
    return NULL;
  }
  return sf;
}

void TQSystematicsHandler::collectVariation(TQFolder* var){
  // collect counters and histograms for one systematic
  //@tag: [verbose] This object tag enables verbosity.
  if(this->getTagBoolDefault("verbose",false)){
    INFOclass("collecting variation '%s'",var->GetName());
  }
  TQSampleFolder* sf = this->getSampleFolder(var);
  TQSampleDataReader* rd = new TQSampleDataReader(sf);
  rd->setApplyStyles(false);
  this->collectCounters  (rd,var);
  this->collectHistograms(rd,var);
  delete rd;
}

void TQSystematicsHandler::collect(){
  // collect counters and histograms for all registered systematics
  DEBUGclass("starting collection of variations");
  TQFolderIterator itr(this->config()->getListOfFolders("Variations/?"),true);
  while(itr.hasNext()){
    this->collectVariation(itr.readNext());
  }
}

void TQSystematicsHandler::compute(){
  // compute the systematic uncertainties from the variations
  DEBUGclass("starting collection of variations");  
  TQFolderIterator cuts(this->getListOfFolders("?"));
  while(cuts.hasNext()){
    TQFolder* cut = cuts.readNext();
    DEBUGclass("computing for cut '%s'",cut->GetName());      
    cut->importTags(this->config()->getFolder("Cuts")->getFolder(cut->GetName()));
    TQFolderIterator samples(this->config()->getListOfFolders("Samples/?"),true);
    while(samples.hasNext()){
      TQFolder* sample = cut->getFolder(samples.readNext()->getName()+"+");
      sample->setTagString("Sample",sample->GetName());
      DEBUGclass("    for sample '%s'",sample->GetName());
      TQFolderIterator systematics(this->config()->getListOfFolders("Systematics/?"),true);
      while(systematics.hasNext()){
        TQFolder* sysConfig = systematics.readNext();    
        DEBUGclass("        for systematic '%s'",sysConfig->GetName());              
        TQFolder* sys = sample->getFolder(sysConfig->getName()+"+");
        sys->setTagString("Systematic",sysConfig->getName());
        sys->importTags(sysConfig);
        this->includeOverallSys(sysConfig,sys,cut->GetName(),sample->getName()+".yield");
        for(const auto& shape:cut->getTagVString("Shapes")){
          this->includeHistoSys(sysConfig,sys->getFolder(shape+"+"),cut->GetName(),sample->getName()+"."+shape);              
        }
      }
    }
  }
}

void TQSystematicsHandler::exportObjects(TQFolder* section,TQFolder* target, bool includeNormalizationInShape){
  // export the created objects for one cut
  if(!section || !target) return;

  // obtain the normalization
  TQFolderIterator systItr(section->getListOfFolders("?"),true);  
  double totalyield = 0.;
  std::vector<double> variations;
  while(systItr.hasNext()){
    TQFolder* syst = systItr.readNext();
    if(!syst) continue;
    double up = 0.;
    double dn = 0.;
    // division by 2 here because of double counting of up/down    
    if(syst->getTagDouble("High",up) && syst->getTagDouble("Low",dn)){
      if(TQUtils::isNum(up) && TQUtils::isNum(dn)){
        totalyield += pow(0.5*(up-dn),2);
      } else if(TQUtils::isNum(up)){
        totalyield += pow(up,2);
      } else if(TQUtils::isNum(dn)){
        totalyield += pow(dn,2);
      }
    }
  }
  double normsys = sqrt(fabs(totalyield));
  target->setTagDouble("yield",normsys);

  // obtain the shape(s)
  target->setTagBool("shapeIncludesNormalization",includeNormalizationInShape);
  std::vector<TString> shapes = section->getTagVString("~Shapes");
  for(const auto& shape:shapes){
    DEBUGclass("exporting histograms '%s' for '%s' to '%s'",shape.Data(),section->getPath().Data(),target->getPath().Data());
    TQFolderIterator systItr(section->getListOfFolders("?/"+shape),true);
    std::vector<TH1*> histos_up;
    std::vector<TH1*> histos_dn;    
    int n=0;
    while(systItr.hasNext()){
      TQFolder* syst = systItr.readNext();
      if(!syst) continue;
      TH1* h_up = this->getSysHisto(syst->getTagStringDefault("HistoRelHigh"));
      TH1* h_dn = this->getSysHisto(syst->getTagStringDefault("HistoRelLow"));
      if(!h_up || !h_dn){
        ERRORclass("unable to retrieve histograms for '%s'",syst->getTagStringDefault("~Systematic").Data());
        continue;
      }
      histos_up.push_back(h_up);
      histos_dn.push_back(h_dn);
      n++;
    }
    TH1* hist = NULL;
    for(size_t i=0; i<histos_up.size(); ++i){
      if(!hist){
        hist = TQHistogramUtils::copyHistogram(histos_up[i],shape);
        hist->Reset();
      } 
      for(int b=0; b<hist->GetNbinsX()+1; ++b){
        hist->AddBinContent(b,pow(0.5*(histos_up[i]->GetBinContent(b)-histos_dn[i]->GetBinContent(b)),2));
      }
    }
    if(hist){
      double normComp = includeNormalizationInShape ? normsys : 0;
      for(int i=0; i<hist->GetNbinsX()+1; ++i){
        hist->SetBinContent(i,sqrt(hist->GetBinContent(i) + pow(normComp,2)));
        hist->SetBinError(i,0);
      }
      hist->SetEntries(n);
      hist->SetMinimum(0);
      if(!target->addObject(hist)){
        WARNclass("unable to export histogram '%s'",hist->GetName());
      }
    }
  }
}

TQFolder* TQSystematicsHandler::exportSystematics(const TString& sample, bool includeNormalizationInShape){
  // create and export variation histograms and counters for all systematics
  TQFolder* systematics = new TQFolder(this->GetName());
  TQFolderIterator cuts(this->getListOfFolders(TQFolder::concatPaths("?",sample)),true);
  while(cuts.hasNext()){
    TQFolder* cut = cuts.readNext();
    if(!cut) continue;
    DEBUGclass("processing section '%s'",cut->getPath().Data());
    TQFolder* target = systematics->getFolder(cut->getTagStringDefault("~Cut")+"+");
    TQSystematicsHandler::exportObjects(cut,target, includeNormalizationInShape);
  }
  return systematics;
}

void TQSystematicsHandler::printSystematics(){
  // create and export variation histograms and counters for all systematics
  TQFolderIterator folders(this->getListOfFolders("?/?"),true);
  while(folders.hasNext()){
    TQFolder* folder = folders.readNext();
    if(!folder) continue;
    std::cout << folder->GetName() << std::endl;
  }
}

bool TQSystematicsHandler::writeFolderHook(TDirectory* dir, const TString&, int, bool){
  // redirect internal writing to another folder
  TQFolder* tmpfolder = this->copy();
  bool ok = tmpfolder->writeFolder(dir,-1,false);
  delete tmpfolder;
  return ok;
}

std::vector<TQFolder*> TQSystematicsHandler::getRanking(const TString& cutname, const TString& sample){
  // return the systematics ranking for one specific cut
  TQFolder* section = this->getFolder(TQFolder::concatPaths(cutname,sample));
  std::vector<TQFolder*> retval;
  if(!section){
    ERRORclass("unable to retrieve ranking for cut '%s' and sample '%s'",cutname.Data(),sample.Data());
    return retval;
  }
  TQFolderIterator systematics(section->getListOfFolders("?"),true);
  while(systematics.hasNext()){
    TQFolder* f = systematics.readNext();
    retval.push_back(f);
  }
  std::sort(retval.begin(),retval.end(),[](TQFolder* a, TQFolder* b) {
    return a->getTagDoubleDefault("Percent",0) > b->getTagDoubleDefault("Percent",0);
  });
  return retval;
}

TQTable* TQSystematicsHandler::getTable(const TString& sample, const TString& cutname){
  // write the systematics table
  std::vector<TQFolder*> ranking = this->getRanking(sample,cutname);
  if(ranking.size() == 0) return NULL;
  TQTable* table = new TQTable(sample+"-"+cutname);
  table->expand(ranking.size()+1,2);
  table->setEntry(0,0,"Systematic");
  table->setEntry(0,1,"\\% yield","latex");
  table->setEntry(0,1,"% yield","plain");
  table->setEntry(0,1,"% yield","html");
  int idx=0;
  for(auto* systematic:ranking){
    double yield = systematic->getTagDoubleDefault("Percent",0);
    size_t row = idx+1;
    table->setEntry(row,0,systematic->GetName(),"ascii");
    //@tag: [title.latex,title.html] This folder tag is used to set the LaTeX/HTML representations of the systematic's title.
    table->setEntry(row,0,systematic->getTagStringDefault("title.latex",systematic->GetName()),"latex");
    table->setEntry(row,0,systematic->getTagStringDefault("title.html",systematic->GetName()),"html");
    table->setEntryValue(row,1,yield);
    ++idx;
  }
  return table;
}

TQSystematicsHandler::~TQSystematicsHandler(){
  delete _config;
  for(auto sf:_inputCache){
    delete sf.second;
  }
}
