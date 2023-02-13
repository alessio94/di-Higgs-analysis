// This script reweights to different kappa lambda values using the HEFT re-weighting                              
// Author: laura.pereira.sanchez@cern.ch

#include<iostream>
#include<fstream>
#include "src/NLO_EFT.h"
#include "src/Root.h"
#include <cmath>

int main (int argc, char **argv){

    const char* inputfile = argv[1];
    const char* treename= argv[2];
    const char* branchname= argv[3];
    float scale = std::stof(argv[4]);  
    float kl = std::stof(argv[5]);
    float kt = std::stof(argv[6]);
    float c2 = std::stof(argv[7]);
    float c2g = std::stof(argv[8]);
    float cg = std::stof(argv[9]);

    TString name;
    const char *c_names[5] =  {"kl","kt","c2","c2g","cg"};  

    for (int i=5; i < 10; i++){            
      TString c = c_names[i-5];
      name+=c+"_"+argv[i];
      if (i != 9) name+="_";

    }

    //TString name = "kl_"+std::to_string(kl)+"_kt_"+std::to_string(kt)+"_c2_"+std::to_string(c2)+"_c2g_"+std::to_string(c2g)+"_cg_"+std::to_string(cg);

    std::cout << "Running couplings: " << name << std::endl;

    NLO_EFT w;
    root_code r;

    // ###################################                                                                           
    // ####  Re-weight Non-SM samples ####                                                        
    // ###################################   
    // Un-comment this part if you want to re-weight non-SM samples!

    /*
    r.Get1DHist(inputfile, branchname, tree_name, scale); // Non-SM sample
    r.Get1DHist("mc16e_hh_yybb_plus_lambda01_p3705.root", branchname, tree_name, scale); // SM sample, specify your sample!
    
    TFile * f_SM =  new TFile(Path+"mc16e_hh_yybb_plus_lambda01_p3705.root", "READ");
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
 
    TBranch *new_branch = tree->Branch("HEFT_weight."+name, &weight,"HEFT_weight."+name+"/F");
    //float Ni_R, Ni_SM, mhh_bin; //Re-weight Non-SM samples
    float Correct = 1.0;
    int i =1;
    float perc = 0.;

    while (reader.Next()) {
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
      
      weight = Correct*w.Rhh(c2,c2g,cg,kl,kt,A)/(w.Rhh(c2,c2g,cg,kl,kt,A_total));

      A.clear();
      
      //std::cout << " m_hh = " << mhh_in_GeV << ", weight  = " <<  weight << std::endl;

      //Fill the selected branches
      new_branch->Fill();

      i+=1;
    }

    tree->SetDirectory(f);
    tree->GetCurrentFile()->Write("", TObject::kOverwrite); // save only the new version of the tree
    f->Close();

    return 0;
}
