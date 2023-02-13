#include "Root.h"
#include <iostream>

void root_code::OpenFile(){
  std::cout << "opening file!" << std::endl;
}

void root_code::Get1DHist(TString file, TString truth_mhh, TString tree_name, float SCALE){

  TFile * F =  new TFile(file, "UPDATE");
  TTreeReader reader(tree_name, F);
  TTreeReaderValue<float> m_hh (reader, truth_mhh);

  float mhh;

  TH1F* h_1D = (TH1F*) F->Get("H_1D");
  if (!h_1D) {
    std::cout << "H_1D NLO reweighting histogram not yet found! Producing it now!" << std::endl;
    TH1F* h_1D = new TH1F("H_1D","H_1D",40,240,1040);
    while (reader.Next()) {
      mhh = *m_hh*SCALE;
      std::cout << "Filling 1D histogram!" << std::endl;
      h_1D->Fill(mhh);
    }

    std::cout << "DONE! Writting histo" << std::endl;
    h_1D->Write();
    std::cout << "Finished writting!" << std::endl;

    F->Write();
  } else {
    std::cout << "1D histogram already exists!" << std::endl;
  }
  F->Close();
}

float root_code::Get1DFrac(TH1F *h, int mhh_bin){
  float Frac = 0;
  float N = h->GetBinContent(mhh_bin);
  float Total = h->Integral();
  Frac = N/Total;

  return Frac;
}
