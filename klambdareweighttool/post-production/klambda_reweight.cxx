#include<iostream>
#include<fstream>
#include<TROOT.h>
#include<TH1.h>
#include<TH2.h>
#include<TMath.h>
#include<TFile.h>
#include<TTree.h>
#include<TF1.h>
#include<TParameter.h>
#include "src/klambda_weight.h"
#include "src/Root.h"


// XS information has been extracted from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHXSWGHH#Latest_recommendations_for_gluon on October 28th following f(κλ) = 70.3874-50.4111*κλ+11.0595*κλ^2 in fb but divided by 1000 to save it in pb!.

int main(int argc, char **argv){

    const char* inputfile = argv[1];
    const char* treename= argv[2];
    const char* branchname= argv[3];
    TString target_kappa = argv[4];
    TString binsize = argv[5];
    float scale = std::stof(argv[6]);   
    float BR_hh = std::stof(argv[7]);
    float kl = std::stof(argv[8]);
    float XS_correction = std::stof(argv[9]);

    // Read Weights!
    ReadWeight_struct weight_vector;
      
    ReadWeight(binsize, target_kappa, &weight_vector);
          
    // Open target file!
    std::cout << inputfile << std::endl;
    TFile * f =  new TFile(inputfile, "UPDATE");    
    TTreeReader reader(treename, f);
    TTreeReaderValue<float>   Truth_mHH (reader, branchname);

    TTree *tree = (TTree*)f->Get(treename); 
    float nentries = tree->GetEntries();

    float weight, weight_err,mhh_in_GeV,perc,xs;

    TBranch *kappalambdaweight = tree->Branch("kappa_lambda_weight_"+binsize+"GeV_"+target_kappa, &weight, "kappa_lambda_weight_"+binsize+"GeV_"+target_kappa+"/F");
    TBranch  *kappalambdaweight_err = tree->Branch("kappa_lambda_weight_err_"+binsize+"GeV_"+target_kappa, &weight_err, "kappa_lambda_weight_err_"+binsize+"GeV_"+target_kappa+"/F");
    TBranch *XSBRFilter = tree->Branch("crossSectionBR_"+target_kappa, &xs, "crossSectionBR_"+target_kappa+"/F");

    // CrossSection*BranchingRatio = 2 * BR_h1 * BR_h2 * f(kl) * XS_correction / 1000 pb!

    xs = 2*BR_hh*(70.3874-50.4111*kl+11.0595*kl*kl)*XS_correction/1000;

    std::cout << "XS = " << xs << std::endl;

    int i = 0;

    while (reader.Next()) {
      if (i % 50000 == 0){
        perc = i/nentries*100;
	std::cout << perc << "% "<<std::endl;
      }

      mhh_in_GeV = *Truth_mHH;
      mhh_in_GeV = mhh_in_GeV*scale;


      std::pair <float,float> weight_pair = GetWeight(mhh_in_GeV, &weight_vector);

      weight = weight_pair.first;
      weight_err = weight_pair.second;

      //std::cout << " m_hh = " << mhh_in_GeV << ", weight = " <<  weight <<  ", weight_err = " << weight_err << std::endl;
      kappalambdaweight->Fill();
      kappalambdaweight_err->Fill();
      XSBRFilter->Fill();

      i++;
    }
    
    tree->SetDirectory(f);
    tree->GetCurrentFile()->Write("", TObject::kOverwrite); // save only the new version of the tree
    f->Close();

    return 0;
}

