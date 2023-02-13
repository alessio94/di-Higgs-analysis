// This script reweights to different kappa lambda values using the HEFT re-weighting                              
// Author: laura.pereira.sanchez@cern.ch

#include<iostream>
#include<fstream>
#include "src/NLO_EFT.h"
#include "src/Root.h"

int main (int argc, char **argv){


    const char* inputfile = argv[1];
    const char* treename= argv[2];
    const char* branchname= argv[3];
    float scale = std::stof(argv[4]);  

    float kl = 1.0;
    float kt = 1.0;
    float c2 = 0.0;
    float c2g = 0.0;
    float cg = 0.0;

    TString SM_sample = "/eos/user/l/lapereir/HH/Signals/Truth/kl_1-tree.root";

    NLO_EFT w;
    root_code r;

    // ###################################                                                                           
    // ####  Re-weight Non-SM samples ####                                                        
    // ###################################   
    // Un-comment this part if you want to re-weight non-SM samples!

    /*
    std::cout << "input sample" << std::endl;
    r.Get1DHist(inputfile, branchname, treename, scale); // Non-SM sample    
    std::cout << "SM sample" << std::endl;
    r.Get1DHist(SM_sample, branchname, treename, scale); // SM sample, specify your sample!
    std::cout << "done" << std::endl;
   
    TFile * f_SM =  new TFile(SM_sample, "READ");
    TH1F* h_SM = (TH1F*) f_SM->Get("H_1D");
    */

    // #######################
    // ####  Read weights ####
    // #######################

    auto n = w.n;
    auto p = w.p;

    float **A_values;
    A_values = new float *[n];
    for (int i = 0; i < n; ++i)
      {
	A_values[i] = new float [p];
      }

    w.GetMatrix(A_values);

    std::vector<float> A_total;
    for (int p = 0; p < 23; p++){
      A_total.push_back(A_values[40][p]);
    }

    std::vector<float>A;

    // ################################
    // ####  Open file to reweight ####
    // ################################

    TFile * f =  new TFile(inputfile, "UPDATE");
    //TH1F* h_R = (TH1F*) f->Get("H_1D");  // Un-comment this part if you want to re-weight non-SM samples!    
    TTreeReader reader(treename, f);
    TTreeReaderValue<float> Truth_mHH (reader, branchname);

    TTree *tree = (TTree*)f->Get(treename); 
    float nentries = tree->GetEntries();
    float weight, mhh_in_GeV;

    // ###############################################
    // ####  Define the branches you want to add! ####
    // ###############################################
 
    float kl_00, kl_01, kl_02, kl_04, kl_06, kl_10, kl_m01, kl_m02, kl_m04, kl_m06, kl_m10;

    TBranch *branch00 = tree->Branch("HEFT_weight.kl_00",  &kl_00,"HEFT_weight.kl_00/F");
    TBranch *branch01 = tree->Branch("HEFT_weight.kl_01",  &kl_01,"HEFT_weight.kl_01/F");
    TBranch *branch02 = tree->Branch("HEFT_weight.kl_02",  &kl_02,"HEFT_weight.kl_02/F");
    TBranch *branch04 = tree->Branch("HEFT_weight.kl_04",  &kl_04,"HEFT_weight.kl_04/F");
    TBranch *branch06 = tree->Branch("HEFT_weight.kl_06",  &kl_06,"HEFT_weight.kl_06/F");
    TBranch *branch10 = tree->Branch("HEFT_weight.kl_10",  &kl_10,"HEFT_weight.kl_10/F");
    TBranch *branchm01 = tree->Branch("HEFT_weight.kl_m01",  &kl_m01,"HEFT_weight.kl_m01/F");
    TBranch *branchm02 = tree->Branch("HEFT_weight.kl_m02",  &kl_m02,"HEFT_weight.kl_m02/F");
    TBranch *branchm04 = tree->Branch("HEFT_weight.kl_m04",  &kl_m04,"HEFT_weight.kl_m04/F");
    TBranch *branchm06 = tree->Branch("HEFT_weight.kl_m06",  &kl_m06,"HEFT_weight.kl_m06/F");
    TBranch *branchm10 = tree->Branch("HEFT_weight.kl_m10",  &kl_m10,"HEFT_weight.kl_m10/F");

    //float Ni_R, Ni_SM, mhh_bin; //Re-weight Non-SM samples
    float Correct = 1.0;
    int i =1;
    float perc = 0.;

    while (reader.Next()) {
      //std::cout << i << std::endl;
      if (i % 50000 == 0){
	perc = i/nentries*100;
	std::cout << perc << "% "<<std::endl;
      }

      mhh_in_GeV = *Truth_mHH;
      mhh_in_GeV = mhh_in_GeV*scale;
     
      // ###################################                                                                        
      // ####  Re-weight Non-SM samples ####                                                                 
      // ###################################                                                            
      // Un-comment this part if you want to re-weight non-SM samples!                                                     
      /*
      mhh_bin = h_R->GetXaxis()->FindBin(mhh_in_GeV);
      Ni_R = h_R->GetBinContent(mhh_bin);
      Ni_SM = h_SM->GetBinContent(mhh_bin);
      Correct = Ni_SM/Ni_R;
      std::cout << Correct << std::endl;
      */
      // ###################################################################
      // ####  Calculate the proper weights for the selected couplings! ####
      // ###################################################################            

      int mhh_tab = w.GetMhhCenterValue(mhh_in_GeV);
      
      //std::cout << "Central values: " << mhh_tab << "," <<  std::endl;
      int l = w.GetLine(mhh_tab,A_values);
      //std::cout << "Line: " << l << std::endl;

      for (int p = 1; p < 24; p++){
	A.push_back(A_values[l][p]);
      }
      
      kl_00 = Correct*w.Rhh(c2,c2g,cg,0,kt,A)/(w.Rhh(c2,c2g,cg,0,kt,A_total));
      kl_01 = Correct*w.Rhh(c2,c2g,cg,1,kt,A)/(w.Rhh(c2,c2g,cg,1,kt,A_total));
      kl_02 = Correct*w.Rhh(0,0,0,2,1,A)/(w.Rhh(0,0,0,2,1,A_total));
      kl_04 = Correct*w.Rhh(0,0,0,4,1,A)/(w.Rhh(0,0,0,4,1,A_total));
      kl_06 = Correct*w.Rhh(0,0,0,6,1,A)/(w.Rhh(0,0,0,6,1,A_total));
      kl_10 = Correct*w.Rhh(0,0,0,10,1,A)/(w.Rhh(0,0,0,10,1,A_total));
      kl_m01 = Correct*w.Rhh(0,0,0,-1,1,A)/(w.Rhh(0,0,0,-1,1,A_total));
      kl_m02 = Correct*w.Rhh(0,0,0,-2,1,A)/(w.Rhh(0,0,0,-2,1,A_total));
      kl_m04 = Correct*w.Rhh(0,0,0,-4,1,A)/(w.Rhh(0,0,0,-4,1,A_total));
      kl_m06 = Correct*w.Rhh(0,0,0,-6,1,A)/(w.Rhh(0,0,0,-6,1,A_total));
      kl_m10 = Correct*w.Rhh(0,0,0,-10,1,A)/(w.Rhh(0,0,0,-10,1,A_total));

      A.clear();
      
      //std::cout << " m_hh = " << mhh_in_GeV << ", weight SM = " <<  kl_01 << std::endl;

      //Fill the selected branches
      branch00->Fill();
      branch01->Fill();
      branch02->Fill();
      branch04->Fill();
      branch06->Fill();
      branch10->Fill();
      branchm01->Fill();
      branchm02->Fill();
      branchm04->Fill();
      branchm06->Fill();
      branchm10->Fill();
      
      i+=1;
    }

    tree->SetDirectory(f);
    tree->GetCurrentFile()->Write("", TObject::kOverwrite); // save only the new version of the tree
    f->Close();

    return 0;
}
