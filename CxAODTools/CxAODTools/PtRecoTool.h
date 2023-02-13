// Dear emacs, this is -*-c++-*-
#ifndef CxAODTools_PtRecoTool_H
#define CxAODTools_PtRecoTool_H

#include "EventLoop/StatusCode.h"

#include <iostream>
#include <string>
#include <map>
#include "TFile.h"
#include "TH1F.h"

class TFile;
class TH1F;

class PtRecoTool {

public:
  PtRecoTool();
  ~PtRecoTool();
  EL::StatusCode initialize(bool debug);
  float getFactorFromTool(float jetPt, const std::string& name, std::string jetCollection);

protected:
  bool m_debug;
  std::map<std::string,TH1F*> m_map_name_TH1F_EM;
  std::map<std::string,TH1F*> m_map_name_TH1F_PF;
  EL::StatusCode initializePtRecoHistograms();
};

#endif // ifndef CxAODTools_PtRecoTool_H
