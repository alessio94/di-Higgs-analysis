#include<iostream>
#include<fstream>
#include<TROOT.h>
#include<TError.h>
#include<TH1.h>
#include<TH2.h>
#include<TMath.h>
#include<TFile.h>
#include<TTree.h>
#include<TF1.h>
#include<TParameter.h>

void hhReweightAnalysisLvl(const char* inputfile="", const char* branchname="truth_mHH", TString target_kappa="5p0", TString binsize="10",TString start_kappa="01d0",float scale=1./1000., bool overflowbin=false){

    TFile *f_in = TFile::Open(inputfile, "update");
    TString m_reweightFile;
    if(overflowbin==false){
        m_reweightFile = "../data/weight-mHH-from-cHHHp"+start_kappa+"-to-cHHHpx_"+binsize+"GeV_Jul28.root";
    } else {
        m_reweightFile = "../data/weight-mHH-from-cHHHp"+start_kappa+"-to-cHHHpx_"+binsize+"GeV_Sept7_overflowfix.root";
    }
    TString target = "reweight_mHH_1p0_to_"+target_kappa;

    std::vector<float> m_mHHUpperBinEdge;
    std::vector<float> m_kFactor;
    std::vector<float> m_kFactorErr;

    TFile* inFile = new TFile(m_reweightFile,"READ");
    TH1* mhh_hist = (TH1*)inFile->Get(target);

    //
    // Underflow + all bins
    //

    for(int iBin =0; iBin < mhh_hist->GetNbinsX()+1; ++iBin){
      m_mHHUpperBinEdge.push_back(mhh_hist->GetBinLowEdge(iBin)+mhh_hist->GetBinWidth(iBin));
      m_kFactor        .push_back(mhh_hist->GetBinContent(iBin)        );
      m_kFactorErr     .push_back(mhh_hist->GetBinError  (iBin));
    }

    //
    // Overflow
    //
    m_mHHUpperBinEdge.push_back(1e9);
    m_kFactor        .push_back(mhh_hist->GetBinContent(mhh_hist->GetNbinsX()+1));
    m_kFactorErr     .push_back(mhh_hist->GetBinError(mhh_hist->GetNbinsX()+1));

    unsigned int m_nMHHBins = m_mHHUpperBinEdge.size();

    inFile->Close();

    TIter next(f_in->GetListOfKeys());
    TKey *key;
    TTree *tree;
    // loop over all trees in the input file
    while ((key=(TKey*)next())) {
        if (strcmp(key->GetClassName(), "TTree")) continue;
        TString tree_name = key->GetName();
        Info("hhReweightAnalysisLvl", "Doing tree %s", tree_name.Data());
        tree = (TTree*)f_in->Get(tree_name);

        Long64_t nentries = tree->GetEntries();
        float truth_mHH = -999;

        float weight;
        float weight_err;

        auto kappalambdaweight = tree->Branch("kappa_lambda_weight_"+binsize+"GeV_"+target_kappa, &weight, "kappa_lambda_weight_"+binsize+"GeV_"+target_kappa+"/F");
        auto kappalambdaweight_err = tree->Branch("kappa_lambda_weight_err_"+binsize+"GeV_"+target_kappa, &weight_err, "kappa_lambda_weight_err_"+binsize+"GeV_"+target_kappa+"/F");
        if (start_kappa != "01d0"){
            kappalambdaweight = tree->Branch("kappa_lambda_from"+start_kappa+"weight_"+binsize+"GeV_"+target_kappa, &weight, "kappa_lambda_from"+start_kappa+"weight_"+binsize+"GeV_"+target_kappa+"/F");
            kappalambdaweight_err = tree->Branch("kappa_lambda_from"+start_kappa+"weight_err_"+binsize+"GeV_"+target_kappa, &weight_err, "kappa_lambda_from"+start_kappa+"weight_err_"+binsize+"GeV_"+target_kappa+"/F");
        }
        for (Long64_t i = 0; i < nentries; i++) {
            bool done = false;
            tree->GetEntry(i);
            tree->SetBranchAddress(branchname,&truth_mHH);
            float mhh_in_GeV = truth_mHH*scale;
            if (mhh_in_GeV < 0) {
                weight = 1;
                weight_err = 1;
                done = true;
            } else {
                for(unsigned int iBin = 0; iBin < m_nMHHBins; ++iBin){

                    if(mhh_in_GeV < m_mHHUpperBinEdge.at(iBin) && !done){
                        weight= m_kFactor.at(iBin);
                        weight_err = m_kFactorErr.at(iBin);
                        done = true;
                    }
                }
            }
            if (!done) {
                weight = m_kFactor.back();
                weight_err = m_kFactorErr.back();
            }

            kappalambdaweight->Fill();
            kappalambdaweight_err->Fill();
        }
        tree->SetDirectory(f_in);
        tree->GetCurrentFile()->Write("", TObject::kOverwrite); // save only the new version of the tree
    }
    f_in->Close();
}
