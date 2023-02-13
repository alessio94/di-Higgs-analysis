#include "CxAODTools/PtRecoTool.h"

PtRecoTool::PtRecoTool():
  m_debug(false)
{
}

PtRecoTool::~PtRecoTool()
{
}

EL::StatusCode PtRecoTool::initializePtRecoHistograms() {
  if (m_debug) std::cout<<"Start PtRecoTool::initializePtRecoHistograms()"<<std::endl;

  // scope defined to read root files from BJetCalibrationTool for ptReco correction
  {
    TFile *fileEMTopo = new TFile("$WorkDir_DIR/data/CxAODTools/AntiKt4EMTopo_PtReco_Correction.root","read");
    TFile *filePFlow = new TFile("$WorkDir_DIR/data/CxAODTools/AntiKt4EMPFlow_PtReco_Correction.root","read");

    std::vector<std::string> slds = {"Hadronic", "SemiLeptonic", "Inclusive"};
    for(const auto& sld : slds) {
      std::string nameTH1F = "Correction_" + sld + "_ttbar_mean";
      m_map_name_TH1F_EM[nameTH1F] = (TH1F*)fileEMTopo->Get(nameTH1F.c_str());
      m_map_name_TH1F_PF[nameTH1F] = (TH1F*)filePFlow->Get(nameTH1F.c_str());
      m_map_name_TH1F_EM[nameTH1F]->SetDirectory(0);
      m_map_name_TH1F_PF[nameTH1F]->SetDirectory(0);
    } //end loop over slds
    fileEMTopo->Close();
    filePFlow->Close();
  } //end scope

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PtRecoTool::initialize(bool debug)
{
  m_debug=debug;

  return initializePtRecoHistograms();
}

float PtRecoTool::getFactorFromTool(float jetPt, const std::string& name, std::string jetCollection)
{
   float factor = 0.0;

   if (jetCollection == "AntiKt4EMTopo" && m_map_name_TH1F_EM[name]) {
     factor = m_map_name_TH1F_EM[name]->Interpolate(log(jetPt));
  }
   else if (jetCollection == "AntiKt4EMPFlow" && m_map_name_TH1F_PF[name]) {
     factor = m_map_name_TH1F_PF[name]->Interpolate(log(jetPt));
   }
   else {
     std::cout << "PtRecoTool::getFactor() PtReco histogram not found for name=" << name << ". Will ABORT!!!" << std::endl;
     assert(false);
   }
   if (m_debug) {
     std::cout << "PtRecoTool::getFactor name=" << name << " jetPt[GeV]=" << jetPt << " factor=" << factor << std::endl;
   }
   return factor;
}
