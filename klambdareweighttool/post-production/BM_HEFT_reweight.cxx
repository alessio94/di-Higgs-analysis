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
 
    float BM_SM, BM_1, BM_2, BM_3, BM_4, BM_5, BM_6, BM_7;
    float CMS_BM_1, CMS_BM_2, CMS_BM_3, CMS_BM_4, CMS_BM_5, CMS_BM_6, CMS_BM_7, CMS_BM_8, CMS_BM_9, CMS_BM_10, CMS_BM_11, CMS_BM_12;

    TBranch *branch0 = tree->Branch("HEFT_weight.BM_SM", &BM_SM,"HEFT_weight.BM_SM/F");
    TBranch *branch1 = tree->Branch("HEFT_weight.BM_1",  &BM_1,"HEFT_weight.BM_1/F");
    TBranch *branch2 = tree->Branch("HEFT_weight.BM_2",  &BM_2,"HEFT_weight.BM_2/F");
    TBranch *branch3 = tree->Branch("HEFT_weight.BM_3",  &BM_3,"HEFT_weight.BM_3/F");
    TBranch *branch4 = tree->Branch("HEFT_weight.BM_4",  &BM_4,"HEFT_weight.BM_4/F");
    TBranch *branch5 = tree->Branch("HEFT_weight.BM_5",  &BM_5,"HEFT_weight.BM_5/F");
    TBranch *branch6 = tree->Branch("HEFT_weight.BM_6",  &BM_6,"HEFT_weight.BM_6/F");
    TBranch *branch7 = tree->Branch("HEFT_weight.BM_7",  &BM_7,"HEFT_weight.BM_7/F");

    TBranch *branch8 = tree->Branch("HEFT_weight.CMS_BM_1",  &CMS_BM_1,"HEFT_weight.CMS_BM_1/F");
    TBranch *branch9 = tree->Branch("HEFT_weight.CMS_BM_2",  &CMS_BM_2,"HEFT_weight.CMS_BM_2/F");
    TBranch *branch10 = tree->Branch("HEFT_weight.CMS_BM_3",  &CMS_BM_3,"HEFT_weight.CMS_BM_3/F");
    TBranch *branch11 = tree->Branch("HEFT_weight.CMS_BM_4",  &CMS_BM_4,"HEFT_weight.CMS_BM_4/F");
    TBranch *branch12 = tree->Branch("HEFT_weight.CMS_BM_5",  &CMS_BM_5,"HEFT_weight.CMS_BM_5/F");
    TBranch *branch13 = tree->Branch("HEFT_weight.CMS_BM_6",  &CMS_BM_6,"HEFT_weight.CMS_BM_6/F");
    TBranch *branch14 = tree->Branch("HEFT_weight.CMS_BM_7",  &CMS_BM_7,"HEFT_weight.CMS_BM_7/F");
    TBranch *branch15 = tree->Branch("HEFT_weight.CMS_BM_8",  &CMS_BM_8,"HEFT_weight.CMS_BM_8/F");
    TBranch *branch16 = tree->Branch("HEFT_weight.CMS_BM_9",  &CMS_BM_9,"HEFT_weight.CMS_BM_9/F");
    TBranch *branch17 = tree->Branch("HEFT_weight.CMS_BM_10",  &CMS_BM_10,"HEFT_weight.CMS_BM_10/F");
    TBranch *branch18 = tree->Branch("HEFT_weight.CMS_BM_11",  &CMS_BM_11,"HEFT_weight.CMS_BM_11/F");
    TBranch *branch19 = tree->Branch("HEFT_weight.CMS_BM_12",  &CMS_BM_12,"HEFT_weight.CMS_BM_12/F");

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
 
      //Rhh(c2, -1/3*c2g, 2/3*cg, kl, kt, std::vector<float>A)    // Different names depending on the publication -> Translation is needed and it depends whether you use A coefficients from NLO or LO publication. We use NLO so we need to translate the benchmarks to reproduce the those from CMS!

      //Rhh(ctt, cgghh, cggh, chhh, ct, std::vector<float>A)   // weights from  NLO publications are used and hence the coupling convention of their paper is the default
      
      BM_SM = Correct*w.Rhh(0,0,0,1,1,A)/w.Rhh(0,0,0,1,1,A_total);
      BM_1 = Correct*w.Rhh(-0.3333, 0.3333, 0.5, 3.94, 0.94,A)/w.Rhh(-0.3333, 0.3333, 0.5, 3.94, 0.94,A_total);
      BM_2 = Correct*w.Rhh(0.3333,-0.3333,0,6.84,0.61,A)/w.Rhh(0.3333,-0.3333,0,6.84,0.61,A_total);
      BM_3 = Correct*w.Rhh(-0.3333,0.5,0.5,2.21,1.05,A)/w.Rhh(-0.3333,0.5,0.5,2.21,1.05,A_total);
      BM_4 = Correct*w.Rhh(0.3333,0.1667,-0.5,2.79,0.61,A)/w.Rhh(0.3333,0.1667,-0.5,2.79,0.61,A_total);
      BM_5 = Correct*w.Rhh(-0.3333,-0.5,0.1667,3.95,1.17,A)/w.Rhh(-0.3333,-0.5,0.1667,3.95,1.17,A_total);
      BM_6 = Correct*w.Rhh(0.3333,0.3333,-0.5,5.68,0.83,A)/w.Rhh(0.3333,0.3333,-0.5,5.68,0.83,A_total);
      BM_7 = Correct*w.Rhh(1,-0.1667,0.1667,-0.1,0.94,A)/w.Rhh(1,-0.1667,0.1667,-0.1,0.94,A_total); 


      CMS_BM_1 = Correct*w.Rhh(-1, 0.0, 0.0, 7.5, 1.0, A)/w.Rhh(-1, 0.0, 0.0, 7.5, 1.0, A_total);
      CMS_BM_2 = Correct*w.Rhh(0.5, -0.2, -0.53, 1.0, 1.0, A)/w.Rhh(0.5, -0.2, -0.53, 1.0, 1.0, A_total);
      CMS_BM_3 = Correct*w.Rhh(-1.5, 0.26, 0.0, 1.0, 1.0, A)/w.Rhh(-1.5, 0.26, 0.0, 1.0, 1.0, A_total);
      CMS_BM_4 = Correct*w.Rhh(-3.0, 0.0, 0.0, -3.5, 1.5, A)/w.Rhh(-3.0, 0.0, 0.0, -3.5, 1.5, A_total);
      CMS_BM_5 = Correct*w.Rhh(0.0, 0.33, 0.53, 1.0, 1.0, A)/w.Rhh(0.0, 0.33, 0.53, 1.0, 1.0, A_total);
      CMS_BM_6 = Correct*w.Rhh(0.0, 0.66, 0.13, 2.4, 1.0, A)/w.Rhh(0.0, 0.66, 0.13, 2.4, 1.0, A_total);
      CMS_BM_7 = Correct*w.Rhh(0.0, 0.66, 0.13, 5.0, 1.0, A)/w.Rhh(0.0, 0.66, 0.13, 5.0, 1.0, A_total);
      CMS_BM_8 = Correct*w.Rhh(0.0, -0.33, -0.66, 15.0, 1.0, A)/w.Rhh(0.0, -0.33, -0.66, 15.0, 1.0, A_total);
      CMS_BM_9 = Correct*w.Rhh(1.0, -0.2, -0.4, 1.0, 1.0, A)/w.Rhh(1.0, -0.2, -0.4, 1.0, 1.0, A_total);
      CMS_BM_10 = Correct*w.Rhh(-1.0, 0.0, 0.0, 10.0, 1.5, A)/w.Rhh(-1.0, 0.0, 0.0, 10.0, 1.5, A_total);
      CMS_BM_11 = Correct*w.Rhh(0.0, 0.33, 0.66, 2.4, 1.0, A)/w.Rhh(0.0,  0.33, 0.66, 2.4, 1.0, A_total);
      CMS_BM_12 = Correct*w.Rhh(1.0, 0.0, 0.0, 15.0, 1.0, A)/w.Rhh(1.0, 0.0, 0.0, 15.0, 1.0, A_total);

            
      A.clear();
      
      //std::cout << " m_hh = " << mhh_in_GeV << ", weight SM = " <<  BM_01 << std::endl;

      //Fill the selected branches
      branch0->Fill();
      branch1->Fill();
      branch2->Fill();
      branch3->Fill();
      branch4->Fill();
      branch5->Fill();
      branch6->Fill();
      branch7->Fill();

      branch8->Fill();
      branch9->Fill();
      branch10->Fill();
      branch11->Fill();
      branch12->Fill();
      branch13->Fill();
      branch14->Fill();
      branch15->Fill();
      branch16->Fill();
      branch17->Fill();
      branch18->Fill();
      branch19->Fill();



      i+=1;
    }

    tree->SetDirectory(f);
    tree->GetCurrentFile()->Write("", TObject::kOverwrite); // save only the new version of the tree
    f->Close();

    return 0;
}
