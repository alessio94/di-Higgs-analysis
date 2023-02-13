/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef TTBARNNLOREWEIGHTER_H_
#define TTBARNNLOREWEIGHTER_H_

#include <string>
#include "TH1.h"
#include "TFile.h"


class TTbarNNLOReweighter {

  public:
    TTbarNNLOReweighter(int sampleID=0,std::string dirName="");
    ~TTbarNNLOReweighter();
    void SetInputDirectory(std::string dirName);
    void SetInputFile(std::string fileName); // to specify a non-default file name
    void SetSampleID(int sampleID);
    void Init();

// variables stored in: top pt in GeV, top M in GeV, top Y (rapidity) no units
//
// default top pT correction for Powheg+Pythia8
   float GetTopPt_Powheg_Pythia8_Nominal(float truth_top_pt);

// alternative ttbar samples
   float GetTopPt_Sherpa(float truth_top_pt); // derived from DSIDs 410424-427
   float GetTopPt_Powheg_Herwig7(float truth_top_pt); // derived from DSIDs 410557/558/559 
   float GetTopPt_aMCNLO_Pythia8(float truth_top_pt); // derived from DSIDs 410464/465/466
   float GetTopPt_Powheg_Pythia8_Hdamp(float truth_top_pt); // derived from DSIDs 410480/481/482

// alternative variables
   float GetTTbarM_Powheg_Pythia8_Nominal(float truth_TTbar_M); // Mass of the ttbar system
   float GetTTbarY_Powheg_Pythia8_Nominal(float truth_TTbar_Y); // Rapidity of the ttbar system
   float GetTopY_Powheg_Pythia8_Nominal(float truth_top_Y); // Rapidity of the top quark

// systematics on the default top pT correction for Powheg+Pythia8
   float GetTopPt_PDFvar_Powheg_Pythia8_Nominal(float truth_top_pt); // LUXqed PDF 
   float GetTopPt_ScaleMin_Powheg_Pythia8_Nominal(float truth_top_pt); // min values of the scales 
   float GetTopPt_ScaleMax_Powheg_Pythia8_Nominal(float truth_top_pt); // max values of the scales 

// Higher order corrections to spin correlations in top quark pair production at the LHC
   float GetDeltaPhi(float truth_dPhi_ll); // azimuthal difference of the two leptons devided by Pi, values between 0 and 1
// Calculated in PowpPyt8 as:   
// float deltaPhi = TMath::Abs(LeptonfromWfromTop.Phi() - LeptonfromWfromATop.Phi() );
// if(deltaPhi > TMath::Pi()) deltaPhi = TMath::TwoPi() - deltaPhi;
// dphi_lep1_lep2 = deltaPhi / TMath::Pi();
// LeptonfromWfrom(A)Top== Lepton from W decay coming from (anti)top quark; can be pdgid=11,13,15; normally should be status 23 in Pythia8

// top pT correction for Powheg+Pythia8 with different top masses (172 GeV and 173 GeV)
   float GetTopPt_Powheg_Pythia8_TopMass172GeV(float truth_top_pt);
   float GetTopPt_Powheg_Pythia8_TopMass173GeV(float truth_top_pt);

// new alternative samples
   float GetTopPt_Powheg_Herwig713(float truth_top_pt); // derived from DSIDs 411233/411234
   float GetTopPt_aMCNLO_Herwig713(float truth_top_pt); // derived from DSIDs 412116/412117 

// average top pT and antitop pT correction for Powheg+Pythia8
   float GetTopPt_Powheg_Pythia8_Nominal_Average(float truth_top_pt, float truth_antitop_pt);


  private:
    int m_sampleID;
    std::string m_weightsDirName;
    std::string m_weightsFileName;
    TFile *m_Weights_file;

//NOMINAL: 410470(NON_ALLHAD), 410471(ALLHAD) 
    TH1* m_Hist_TopPt_Powheg_Pythia8_Nominal;
    TH1* m_Hist_TTbarM_Powheg_Pythia8_Nominal;
    TH1* m_Hist_TTbarY_Powheg_Pythia8_Nominal;
    TH1* m_Hist_TopY_Powheg_Pythia8_Nominal;
    TH1* m_Hist_TopPt_PDFvar_Powheg_Pythia8_Nominal;//PDF_Variation
    TH1* m_Hist_TopPt_ScaleMin_Powheg_Pythia8_Nominal;//Scale_Min
    TH1* m_Hist_TopPt_ScaleMax_Powheg_Pythia8_Nominal;//Scale_Max

//aMC@NLO+Pythia8: 410464(Single_lep), 410465(Dilep), 410466(ALLHAD)
    TH1* m_Hist_TopPt_aMCNLO_Pythia8;

//Powheg+Herwig7: 410557(Single_lep + jets), 410558(Dilep), 410558(ALLHAD)
    TH1* m_Hist_TopPt_Powheg_Herwig7;

//Powheg_Pythia8_Hdamp: 410480(Single_lep), 410481(Dilep), 410482(ALLHAD)
    TH1* m_Hist_TopPt_Powheg_Pythia8_Hdamp;

//Sherpa: 410249(ALLHAD), 410250(Singlelep(+) + jets),410251(Singlelep(-) + jets), 410252(Dilep)
    TH1* m_Hist_TopPt_Sherpa;

// NOMINAL: 410472(DILEPTON)
    TH1* m_Hist_dPhi_ll;

// Powheg_Pythia8 with different top masses: 411049 and 411057 for 173 GeV and 411046 and 411054 for 172 GeV
    TH1* m_Hist_TopPt_411049_411057_mass173GeV;
    TH1* m_Hist_TopPt_411046_411054_mass172GeV;

// new alternative Powheg_Herwig713 and aMCNLO_Herwig713
    TH1* m_Hist_TopPt_Powheg_Herwig713;
    TH1* m_Hist_TopPt_aMCNLO_Herwig713;

};

#endif
