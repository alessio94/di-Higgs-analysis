#ifndef hhTruthWeightTools_hhWeightTool_H
#define hhTruthWeightTools_hhWeightTool_H

#include <vector>

#include "AsgTools/AsgTool.h"

#include "xAODTruth/TruthParticle.h"

namespace xAOD {

  class hhWeightTool : public asg::AsgTool {
      
  public:
    
    //****************************************************************************** 
    hhWeightTool(const std::string& name);
    ~hhWeightTool() {};
    
    StatusCode initialize();
    StatusCode finalize();
    
    float       getWeight(float mhh);
    float       getWeightHigh(float mhh);
    float       getWeightLow(float mhh);
    float       getWeightErr(float mhh);
    float       getWeight(const xAOD::TruthParticleContainer* truth_particles,bool initial=true, bool oneset=false);
    float       getWeightHigh(const xAOD::TruthParticleContainer* truth_particles,bool initial=true, bool oneset=false);
    float       getWeightLow(const xAOD::TruthParticleContainer* truth_particles,bool initial=true, bool oneset=false);
    float       getWeightErr(const xAOD::TruthParticleContainer* truth_particles,bool initial=true, bool oneset=false);
    float       getMHH(const xAOD::TruthParticleContainer* truth_particles,bool initial=true, bool oneset=false);

  private:
    
    void addMHHBin(float upperMhhBinEdge, float kFactor, float kFactorError);
    void addMHHBin(float upperMhhBinEdge, float kFactor);

    void loadFile(std::string fileName, std::string histName);
    void loadTxtFile(std::string fileName, float kl,float kt,float cg,float c2g,float c2);
    
  private:

    int m_verbosity;
    std::string m_reweightFile;
    std::string m_target;
    float kl;
    float kt;
    float cg;
    float c2g;
    float c2;

    // In GeV 
    std::vector<float> m_mHHUpperBinEdge;
    std::vector<float> m_kFactor;
    std::vector<float> m_kFactorErr;
    unsigned int       m_nMHHBins;
  };

}//namespace

#endif
