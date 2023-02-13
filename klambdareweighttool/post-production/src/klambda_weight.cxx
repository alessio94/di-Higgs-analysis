#include <iostream>
#include <fstream>
#include <cmath>
#include "klambda_weight.h"
#include <vector>


// Function to get Matrix of values from txt file                                                                                                                                 
/*struct ReadWeight_result{
  std::vector<float> m_mHHUpperBinEdge;
  std::vector<float> m_kFactor;
  std::vector<float> m_kFactorErr;
  unsigned int m_nMHHBins;
  }*/


void ReadWeight(TString binsize, TString target_kappa, ReadWeight_struct* add_weight_vectors){

  TString m_reweightFile = "../data/weight-mHH-from-cHHHp01d0-to-cHHHpx_"+binsize+"GeV_Jul28.root";
  std::cout << m_reweightFile << std::endl;
  TString target = "reweight_mHH_1p0_to_"+target_kappa;

  TFile* WeightFile = new TFile(m_reweightFile,"READ");
  TH1* mhh_hist = (TH1*)WeightFile->Get(target);

  //                                                                                                                                                                            
  // Underflow + all bins                                                                                                                                                       
  //                                                                                                                                                                            
  for(int iBin =0; iBin < mhh_hist->GetNbinsX()+1; ++iBin){
    add_weight_vectors->m_mHHUpperBinEdge.push_back(mhh_hist->GetBinLowEdge(iBin)+mhh_hist->GetBinWidth(iBin));
    add_weight_vectors->m_kFactor.push_back(mhh_hist->GetBinContent(iBin) );
    add_weight_vectors->m_kFactorErr.push_back(mhh_hist->GetBinError  (iBin));
  }
  
  //                                                                                                                                                                            
  // Overflow                                                                                                                                                                   
  //                                                                                                                                                                            
  add_weight_vectors->m_mHHUpperBinEdge.push_back(1e9);
  add_weight_vectors->m_kFactor.push_back(mhh_hist->GetBinContent(mhh_hist->GetNbinsX()+1));
  add_weight_vectors->m_kFactorErr.push_back(mhh_hist->GetBinError(mhh_hist->GetNbinsX()+1));
  
  add_weight_vectors->m_nMHHBins = add_weight_vectors->m_mHHUpperBinEdge.size();

  WeightFile->Close();

}

std::pair<float,float> GetWeight(float mhh_in_GeV, ReadWeight_struct *add_weight_vectors){

  bool done = false;
  float weight, weight_err;

  for(unsigned int iBin = 0; iBin < add_weight_vectors->m_nMHHBins; ++iBin){

    if(mhh_in_GeV < add_weight_vectors->m_mHHUpperBinEdge.at(iBin) && !done){
      weight= add_weight_vectors->m_kFactor.at(iBin);
      weight_err = add_weight_vectors->m_kFactorErr.at(iBin);
      done = true;
    }
  }
  if (!done) {
    weight = add_weight_vectors->m_kFactor.back();
    weight_err = add_weight_vectors->m_kFactorErr.back();
  }

  std::pair<float,float> weight_pair;
  weight_pair.first = weight;
  weight_pair.second = weight_err;

  return weight_pair;
}
