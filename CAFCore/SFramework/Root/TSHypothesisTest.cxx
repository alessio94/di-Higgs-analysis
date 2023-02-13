#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iterator>

#include "RooDataSet.h"
#include "RooCategory.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/ProofConfig.h"
#include "RooStats/ToyMCSampler.h"

#include "QFramework/TQIterator.h"

#include "SFramework/TSHypothesisTest.h"

#include "QFramework/TQLibrary.h"

#include "SFramework/TSUtils.h"

ClassImp(TSHypothesisTest)

//__________________________________________________________________________________|___________

TSHypothesisTest::Hypothesis::Hypothesis(const char* hname) :
  fName(hname)
{
  // do nothing
}

//__________________________________________________________________________________|___________

TSHypothesisTest::Hypothesis::Hypothesis(const char* hname, const char* pname, double val) :
  fName(hname)
{
  // add one parameter
  this->fParameters[pname]=val;
}

//__________________________________________________________________________________|___________

TSHypothesisTest::Hypothesis::Hypothesis(TQFolder* f){
  // constructor based on a TQFolder
  this->fName = f->getTagStringDefault("name",f->GetName());
  TQIterator pars(f->getListOfKeys("pars.*"), true);
  while(pars.hasNext()){
    TString tag = pars.readNext()->GetName();
    double val;
    if(!f->getTagDouble(tag,val)){
      throw std::runtime_error("unable to retrieve tag!");
    }
    TQStringUtils::removeLeading(tag,"pars.");
    this->addParameter(tag.Data(),val);
  }
}  

//__________________________________________________________________________________|___________

void TSHypothesisTest::Hypothesis::addParametersToList(const RooAbsCollection& allPars, RooAbsCollection& toList){
  // add all parameters in this hypothesis to the list
  for(auto p:this->fParameters){
    RooAbsArg* par = allPars.find(p.first.c_str());
    if(!par) throw std::runtime_error(TString::Format("unable to find parameter '%s' in list!",p.first.c_str()).Data());
    toList.add(*par);
  }
}

//__________________________________________________________________________________|___________

const char* TSHypothesisTest::Hypothesis::getParameterNameString() {
  // get a comma separated string of all parameter names
  std::stringstream ss;
  bool first = true;
  for(auto p:this->fParameters){
    ss << p.first;
    if(!first) ss << ",";
    first = false;
  }
  return ss.str().c_str();
}

//__________________________________________________________________________________|___________
    
void TSHypothesisTest::Hypothesis::addParameter(const char* pname, double val){
  // add a parameter to this hypothesis
  this->fParameters[pname]=val;
}

//__________________________________________________________________________________|___________

const char* TSHypothesisTest::Hypothesis::name() const {
  // get the name of this hypothesis
  return fName.c_str();
}

//__________________________________________________________________________________|___________

void TSHypothesisTest::Hypothesis::setParameters(RooAbsCollection& pars) const {
  // apply the parameter settings of this hypothesis
  RooFIter itr(pars.fwdIterator());
  TObject* obj = NULL;
  while((obj = itr.next())){
    RooRealVar* par = dynamic_cast<RooRealVar*>(obj);
    for(auto p:this->fParameters){
      if(p.first.compare(par->GetName()) == 0){
        par->setVal(p.second);
      }
    }
  }
}

//__________________________________________________________________________________|___________

void TSHypothesisTest::Hypothesis::setParametersConstant(RooAbsCollection& pars, bool setConstant) const {
  // control the const property of all parameters of this hypothesis
  RooFIter itr(pars.fwdIterator());
  TObject* obj = NULL;
  while((obj = itr.next())){
    RooRealVar* par = dynamic_cast<RooRealVar*>(obj);
    for(auto p:this->fParameters){
      if(p.first.compare(par->GetName()) == 0){
        par->setConstant(setConstant);
      }
    }
  }
}

//__________________________________________________________________________________|___________

TSHypothesisTest::TSHypothesisTest(RooWorkspace * ws, TQFolder* snapshots) : TSStatisticsCalculator("TSHypothesisTest",ws,snapshots) {
  // constructor
}


//__________________________________________________________________________________|___________

TSHypothesisTest::~TSHypothesisTest() {
  // default destructor
}

//__________________________________________________________________________________|___________

void TSHypothesisTest::runFit(TQFolder* results, RooDataSet* data, const TString& name, TQFolder* options){
  // run a fit of one hypotheses on one toy (or real) dataset
  bool store = options->getTagBoolDefault("storeResults",false);
  TQFolder* result = this->fitPdfToData(this->getPdf(),data,this->getNuisanceParameters(),options);
  
  if(!result) return;
  double minNll = result->getTagDoubleDefault("minNll",0);
  int status = result->getTagIntegerDefault("status",-1);
  
  results->setTagDouble(name+".minNll",minNll);
  results->setTagDouble(name+".status",status);
  
  if(store){
    result->setName(name);
    results->addFolder(result);
  } else {
    delete result;
  }
}


//__________________________________________________________________________________|___________

TQFolder* TSHypothesisTest::runFit(RooDataSet* data, TQFolder* options, RooAbsCollection& pois, const std::vector<const Hypothesis*>& hypotheses){
  // run a fit of all hypotheses on one toy (or real) dataset
  TString snapshot = options->getTagStringDefault("snapshot","SnSh_AllVars_Nominal");

  TQFolder* result = new TQFolder(data->GetName());

  for(const auto& h:hypotheses){
    this->fWorkspace->loadSnapshot(snapshot);
    h->setParameters(pois);
    TSUtils::setParametersConstant(&pois,true);
    runFit(result,data,h->name(),options);
  }
  
  this->fWorkspace->loadSnapshot(snapshot);
  TSUtils::setParametersConstant(&pois,false);
  runFit(result,data,"uncond",options);
  
  return result;
}

//__________________________________________________________________________________|___________

TQFolder * TSHypothesisTest::runCalculation(TQFolder * options){
  // perform the calculation
  if(!this->fModelConfig) return NULL;
  
  RooArgSet pois;
  std::vector<const Hypothesis*> hypotheses;
  TQFolder* result = new TQFolder("result");
  TQFolder* hypos = result->getFolder("hypotheses+");
  TQFolderIterator itr(options->getListOfFolders("?"));
  while(itr.hasNext()){
    TQFolder* f = itr.readNext();
    hypos->addFolder(f->copy());
    Hypothesis* h = new Hypothesis(f);
    h->addParametersToList(this->fWorkspace->allVars(),pois);
    hypotheses.push_back(h);
  }
  
  RooArgSet nuis (*(this->fModelConfig->GetNuisanceParameters()));
  RooArgSet globs(*(this->fModelConfig->GetGlobalObservables()));
  
  RooArgSet allVars;
  allVars.add(pois);
  allVars.add(nuis);
  allVars.add(globs);

  RooArgSet obs(*(this->fModelConfig->GetObservables()));
  
  RooCategory* indexCat = this->getIndexCategory();
  if(indexCat){
    indexCat->setIndex(0);    
    obs.add(*indexCat);
  }
  
  RooAbsPdf* pdf = this->getPdf();
  
  int nworkers = options->getTagIntegerDefault("nWorkers",8);
  RooStats::ProofConfig pc(*this->fWorkspace, nworkers, TString::Format("workers=%d",nworkers), false);
  RooStats::ToyMCSampler sampler;
  sampler.SetUseMultiGen(indexCat);
  sampler.SetPdf(*pdf);
  sampler.SetObservables(obs);
  sampler.SetNuisanceParameters(nuis);
  sampler.SetGlobalObservables(globs);
  sampler.SetParametersForTestStat(pois);
  sampler.SetProofConfig(&pc);
  
  TQFolder* dataresults = result->getFolder("data+");
  for(const auto& dsname:options->getTagVString("dataName")){
    RooDataSet* data = dynamic_cast<RooDataSet*>(this->fWorkspace->data(dsname));
    if(!data){
      this->error(TString::Format("unable to find dataset '%s', skipping",dsname.Data()));
      continue;
    }
    TQFolder* fitresult = this->runFit(data,options,pois,hypotheses);
    dataresults->addFolder(fitresult);
  }
  
  int nToys = options->getTagIntegerDefault("nToys",1000);
  bool saveToys = options->getTagBoolDefault("saveToys",false);

  TQFolder* toyresults = result->getFolder("toys+");
  for(auto h:hypotheses){
    TQFolder* f = new TQFolder(h->name());
    toyresults->addFolder(f);
  }
  
  for (int itoy = 0; itoy < nToys; ++itoy) {
    for(auto h:hypotheses){
      h->setParameters(pois);
      TSUtils::setParametersConstant(&pois,true);
      
      RooDataSet* toyData = dynamic_cast<RooDataSet*>(sampler.GenerateToyData(allVars));
      if(!toyData) throw std::runtime_error("unable to generate toy data!");
      toyData->SetName(TString::Format("toy_%d",itoy).Data());
      if(saveToys){
        fWorkspace->import(*toyData);
      }
      TQFolder* fitresult = this->runFit(toyData,options,pois,hypotheses);
      toyresults->getFolder(h->name())->addFolder(fitresult);
    }
  }
  
  for(auto h:hypotheses){
    delete h;
  }

  return result;
}


