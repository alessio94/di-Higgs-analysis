#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
#include "TH1.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include <utility> 

//class klambda_weight{
// public:
struct ReadWeight_struct{
  std::vector<float> m_mHHUpperBinEdge;
  std::vector<float> m_kFactor;
  std::vector<float> m_kFactorErr;
  unsigned int m_nMHHBins;
};
 
void ReadWeight(TString binsize, TString target_kappa,ReadWeight_struct *add_weight_vectors);

std::pair<float,float> GetWeight(float mhh_in_GeV,ReadWeight_struct *add_weight_vectors);

//ReadWeight_struct weight_vector;
//};

