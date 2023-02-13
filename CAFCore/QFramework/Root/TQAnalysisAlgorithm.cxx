#include "QFramework/TQAnalysisAlgorithm.h"



#include <string>

//#define _DEBUG_


#include "QFramework/TQAnalysisJob.h"
#include "QFramework/TQCompiledCut.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQToken.h"
#include "QFramework/TQLibrary.h"
#include "QFramework/TQCut.h"
#include "QFramework/TQAlgorithm.h"
#include "TTree.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include <stdexcept>

using std::cout;
using std::endl;


ClassImp(TQAnalysisAlgorithm)

TQAnalysisAlgorithm::TQAnalysisAlgorithm(const TString& name, TQCut* myCut):
   fCut(myCut)
{
  // constructor
  this->SetName(name);


}

//__________________________________________________________________________________|___________

void TQAnalysisAlgorithm::setBaseCut(TQCut* myCut){
    this->fCut = myCut;
}
//__________________________________________________________________________________|___________

bool TQAnalysisAlgorithm::initializeSampleFolder(TQSampleFolder* s){
  // called for each sample folder
  if (!s) {
    return false;
  }

  fCut->initializeSampleFolder(s);

  return true;
}

//__________________________________________________________________________________|___________

bool TQAnalysisAlgorithm::finalizeSampleFolder(TQSampleFolder* s){
  // called for each sample folder  
  if (!s) {
    return false;
  }
  
  fCut->finalizeSampleFolder(s);
  return true;
}
//__________________________________________________________________________________|___________

bool TQAnalysisAlgorithm::initialize(TQSample* s){
  // called at the beginning of each sample
  if (!s) {
    return false;
  }
  
  TQToken* treeTok = s->getTreeToken();
  TTree* tree = nullptr;
  if (treeTok) tree = static_cast<TTree*>(treeTok->getContent());
  
  this->fSample = s;
  if (tree && tree->GetEntries()>0 && !fCut->initialize(s)) {
    ERRORclass("Failed to initialize cuts for sample with path '%s'",s->getPath().Data());
    //don't forget to hand back the token before returning!
    s->returnToken(treeTok);
    return false;
  }
  
  if (treeTok) s->returnToken(treeTok);
    
  this->fUseWeights= false;
  s->getTagBool("usemcweights", fUseWeights, true);
  return true;
}

//__________________________________________________________________________________|___________

bool TQAnalysisAlgorithm::finalize(){
  // called at the end of each sample

  fCut->finalize();
  return true;
}

//__________________________________________________________________________________|___________

bool TQAnalysisAlgorithm::execute(){
  // called at the beginning of each event

  // you probably don't need to do anything here
  return true;
}

//__________________________________________________________________________________|___________

bool TQAnalysisAlgorithm::cleanup(){
  // called at the end of each event

 fCut->analyse(1., fUseWeights);

  // TODO: think about how to handle event weights!

  return true;
}

//__________________________________________________________________________________|___________

TObjArray * TQAnalysisAlgorithm::getListOfBranches(){
  if (!fCut) return nullptr;
  return fCut->getListOfBranches();
}

//__________________________________________________________________________________|___________
TQAlgorithm * TQAnalysisAlgorithm::getClone() const {
  TQAnalysisAlgorithm* newAlg= new TQAnalysisAlgorithm(this->getName(),this->fCut->getClone());
  return newAlg;
}
