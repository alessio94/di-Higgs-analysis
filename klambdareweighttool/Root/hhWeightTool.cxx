/**********************************************************************
 * AsgTool: hhWeightTool
 *
 * Authors:
 *      John Alison <john.alison@cern.ch>
 *
 * Description:
 *      Base tool for hh reweigting
 **********************************************************************/
#include "hhTruthWeightTools/hhWeightTool.h"
#include <AsgTools/MessageCheck.h>
#include "PathResolver/PathResolver.h"

#include "TFile.h"
#include "TH1.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

//**********************************************************************

using namespace xAOD;


hhWeightTool::hhWeightTool( const std::string& name )
  : AsgTool(name)
{
  declareProperty("Verbosity",            m_verbosity=0);

  //
  // Reweight file for SMhh
  //      "hhTruthWeightTools/data/SMhh_mhh_kfactor.root"
  // See (arXiv:1604.06447, arXiv:1608.04798) for details
  // 
  declareProperty("ReweightFile",         m_reweightFile="hhTruthWeightTools/weight-mHH-from-cHHHp01d0-to-cHHHpx_10GeV_Jul28.root");
  declareProperty("TargetKappaLambda", m_target="5p0");
  // For HEFT
  declareProperty("kl", kl=1.0);
  declareProperty("kt", kt=1.0);
  declareProperty("cg", cg=0.0);
  declareProperty("c2g", c2g=0.0);
  declareProperty("c2", c2=0.0);
}



//**********************************************************************

StatusCode hhWeightTool::initialize() {
  
  ATH_MSG_INFO( "Initialising tool " << name() );

  if(m_reweightFile.compare(m_reweightFile.length()-4,4,"root") == 0){
    ATH_MSG_INFO( "Root file: assuming this is kappa lambda reweighting" );
    loadFile(PathResolverFindCalibFile(m_reweightFile),"reweight_mHH_1p0_to_"+m_target);
  } else if (m_reweightFile.compare(m_reweightFile.length()-3,3,"txt") == 0){
    ATH_MSG_INFO( "Text file: assuming this is HEFT reweighting" );
    loadTxtFile(PathResolverFindCalibFile(m_reweightFile),kl,kt,cg,c2g,c2);
  } else {
    std::cout << "Error! Unrecognized weight file type" << std::endl;
    return StatusCode::FAILURE;   
  }

  return StatusCode::SUCCESS;
}


float hhWeightTool::getMHH(const xAOD::TruthParticleContainer* truth_particles, bool initial, bool oneset) {

    xAOD::IParticle::FourMom_t hh{};
    int hh_count = 0;
    for (const xAOD::TruthParticle* p : *truth_particles) {
      int nChildren = p->nChildren();
      int nParents = p->nParents();
      int parent_pdg = 0;
      if (nParents > 0) {
        parent_pdg = p->parent(0)->pdgId();
      }
      int status = p->status();

      if(oneset && p->isHiggs()){
        hh += p->p4();
        hh_count += 1;
      } else if (initial && p->isHiggs() && ((status ==22 || status == 11) && (nParents ==0 or parent_pdg!=25) && nChildren<2)) {
        hh += p->p4();
        hh_count += 1;
      } else if ((!initial) && p->isHiggs() && ((status ==62 || status == 11) && nChildren==2)) {
        hh += p->p4();
        hh_count += 1;
      }
    }
    if(hh_count!=2) {
        std::cout << "ERROR: Found " << hh_count << " truth higgs" << std::endl;
    }
    float b_truth_mHH = hh.M();
    return b_truth_mHH;
}

float hhWeightTool::getWeight(float mhh){
 
  float mhh_in_GeV = mhh*0.001;
  
  for(unsigned int iBin = 0; iBin < m_nMHHBins; ++iBin){

    if(mhh_in_GeV < m_mHHUpperBinEdge.at(iBin))
      return m_kFactor.at(iBin);
  }

  return m_kFactor.back();
}


float hhWeightTool::getWeight(const xAOD::TruthParticleContainer* truth_particles,bool initial, bool oneset) {

    float mhh = getMHH(truth_particles,initial,oneset);

    float mhh_in_GeV = mhh*0.001;

    for(unsigned int iBin = 0; iBin < m_nMHHBins; ++iBin){

    if(mhh_in_GeV < m_mHHUpperBinEdge.at(iBin))
      return m_kFactor.at(iBin);
    }
    return m_kFactor.back();
}

float hhWeightTool::getWeightHigh(float mhh){

  float mhh_in_GeV = mhh*0.001;

  for(unsigned int iBin = 0; iBin < m_nMHHBins; ++iBin){

    if(mhh_in_GeV < m_mHHUpperBinEdge.at(iBin))
      return m_kFactor.at(iBin)+m_kFactorErr.at(iBin);
  }

  return m_kFactor.back()+m_kFactorErr.back();
}

float hhWeightTool::getWeightHigh(const xAOD::TruthParticleContainer* truth_particles,bool initial, bool oneset) {

    float mhh = getMHH(truth_particles,initial,oneset);

    float mhh_in_GeV = mhh*0.001;

    for(unsigned int iBin = 0; iBin < m_nMHHBins; ++iBin){

    if(mhh_in_GeV < m_mHHUpperBinEdge.at(iBin))
      return m_kFactor.at(iBin)+m_kFactorErr.at(iBin);
    }
    return m_kFactor.back()+m_kFactorErr.back();
}

float hhWeightTool::getWeightLow(float mhh){

  float mhh_in_GeV = mhh*0.001;

  for(unsigned int iBin = 0; iBin < m_nMHHBins; ++iBin){

    if(mhh_in_GeV < m_mHHUpperBinEdge.at(iBin))
      return m_kFactor.at(iBin)-m_kFactorErr.at(iBin);
  }

  return m_kFactor.back()-m_kFactorErr.back();
}

float hhWeightTool::getWeightLow(const xAOD::TruthParticleContainer* truth_particles,bool initial, bool oneset) {

    float mhh = getMHH(truth_particles,initial,oneset);

    float mhh_in_GeV = mhh*0.001;

    for(unsigned int iBin = 0; iBin < m_nMHHBins; ++iBin){

    if(mhh_in_GeV < m_mHHUpperBinEdge.at(iBin))
      return m_kFactor.at(iBin)-m_kFactorErr.at(iBin);
    }
    return m_kFactor.back()-m_kFactorErr.back();
}

float hhWeightTool::getWeightErr(float mhh){

  float mhh_in_GeV = mhh*0.001;

  for(unsigned int iBin = 0; iBin < m_nMHHBins; ++iBin){

    if(mhh_in_GeV < m_mHHUpperBinEdge.at(iBin))
      return m_kFactorErr.at(iBin);
  }

  return m_kFactorErr.back();
}

float hhWeightTool::getWeightErr(const xAOD::TruthParticleContainer* truth_particles,bool initial, bool oneset) {

    float mhh = getMHH(truth_particles,initial,oneset);

    float mhh_in_GeV = mhh*0.001;

    for(unsigned int iBin = 0; iBin < m_nMHHBins; ++iBin){

    if(mhh_in_GeV < m_mHHUpperBinEdge.at(iBin))
      return m_kFactorErr.at(iBin);
    }
    return m_kFactorErr.back();
}

//!==========================================================================
StatusCode hhWeightTool::finalize() {
  
  ATH_MSG_INFO( "Finalising tool " << name() );
  
  return StatusCode::SUCCESS;
}

void hhWeightTool::addMHHBin(float upperMhhBinEdge, float kFactor, float kFactorError){
 
  m_mHHUpperBinEdge.push_back(upperMhhBinEdge);
  m_kFactor        .push_back(kFactor        );
  m_kFactorErr     .push_back(kFactorError   );
}

void hhWeightTool::addMHHBin(float upperMhhBinEdge, float kFactor){

  m_mHHUpperBinEdge.push_back(upperMhhBinEdge);
  m_kFactor        .push_back(kFactor        );
}

void hhWeightTool::loadFile(std::string fileName, std::string histName){
 
  TFile* inFile = new TFile(fileName.c_str(),"READ");
  TH1* mhh_hist = (TH1*)inFile->Get(histName.c_str());

  //
  // Underflow + all bins
  //
  for(int iBin =0; iBin < mhh_hist->GetNbinsX()+1; ++iBin){
    addMHHBin((mhh_hist->GetBinLowEdge(iBin)+mhh_hist->GetBinWidth(iBin)),  
	      mhh_hist->GetBinContent(iBin),
	      mhh_hist->GetBinError  (iBin));
  }

  //
  // Overflow
  //
  addMHHBin(1e9,
	    mhh_hist->GetBinContent(mhh_hist->GetNbinsX()+1),
	    mhh_hist->GetBinError  (mhh_hist->GetNbinsX()+1));

  m_nMHHBins = m_mHHUpperBinEdge.size();

  inFile->Close();
  return;
}

void hhWeightTool::loadTxtFile(std::string fileName,float kl,float kt,float cg,float c2g,float c2){


  std::string line;
  std::ifstream myfile (fileName);
  //float mHH_upper = 0;
  float Matrix [41][24];

  int a = 0; // line index
    
  if (myfile.is_open())
    {
      while ( getline (myfile,line) )
      {
        std::string temp = "";
        int b = 0; // column index
        for(u_int i=0; i < line.size(); i++){
            if (!isblank(line[i])){
                std::string d(1, line[i]);
                temp.append(d);
            } else {
                Matrix[a][b]= std::stof(temp); //convert string to float
                temp = "";
                b+=1;
            }
        }
        a+=1;
      }
      myfile.close();

      float weight_den = Matrix[40][0]*pow(kt,4) + Matrix[40][1]*pow(c2,2) + (Matrix[40][2]*pow(kt,2) + Matrix[40][3]*pow(cg,2))*pow(kl,2) + Matrix[40][4]*pow(c2g,2) + ( Matrix[40][5]*c2 + Matrix[40][6]*kt*kl )*pow(kt,2)  + (Matrix[40][7]*kt*kl + Matrix[40][8]*cg*kl )*c2 + Matrix[40][9]*c2*c2g  + (Matrix[40][10]*cg*kl + Matrix[40][11]*c2g)*pow(kt,2)+ (Matrix[40][12]*kl*cg + Matrix[40][13]*c2g )*kt*kl + Matrix[40][14]*cg*c2g*kl + Matrix[40][15]*cg*pow(kt,3) + Matrix[40][16]*c2*kt*cg + Matrix[40][17]*kt*kl*pow(cg,2)+ Matrix[40][18]*kt*cg*c2g + Matrix[40][19]*pow(kt,2)*pow(cg,2) + Matrix[40][20]*c2*pow(cg,2) + Matrix[40][21]*kl*pow(cg,3) + Matrix[40][22]*c2g*pow(cg,2);
      float weight;
      for (u_int line = 0; line < 40; line++){
        // Lines (besides last) start with mHH value
        float mHH_upper = Matrix[line][0]+10.0;
        float weight_nom = Matrix[line][1]*pow(kt,4) + Matrix[line][2]*pow(c2,2) + (Matrix[line][3]*pow(kt,2) + Matrix[line][4]*pow(cg,2))*pow(kl,2) + Matrix[line][5]*pow(c2g,2) + ( Matrix[line][6]*c2 + Matrix[line][7]*kt*kl )*pow(kt,2)  + (Matrix[line][8]*kt*kl + Matrix[line][9]*cg*kl )*c2 + Matrix[line][10]*c2*c2g  + (Matrix[line][11]*cg*kl + Matrix[line][12]*c2g)*pow(kt,2)+ (Matrix[line][13]*kl*cg + Matrix[line][14]*c2g )*kt*kl + Matrix[line][15]*cg*c2g*kl + Matrix[line][16]*cg*pow(kt,3) + Matrix[line][17]*c2*kt*cg + Matrix[line][18]*kt*kl*pow(cg,2)+ Matrix[line][19]*kt*cg*c2g + Matrix[line][20]*pow(kt,2)*pow(cg,2) + Matrix[line][21]*c2*pow(cg,2) + Matrix[line][22]*kl*pow(cg,3) + Matrix[line][23]*c2g*pow(cg,2);
        weight = weight_nom/weight_den;
        addMHHBin(mHH_upper,weight);       

      }
      //Overflow
      addMHHBin(1e9,weight);
      m_nMHHBins = m_mHHUpperBinEdge.size();
    }
  else std::cout << "Unable to open file"; 
  return;
}
