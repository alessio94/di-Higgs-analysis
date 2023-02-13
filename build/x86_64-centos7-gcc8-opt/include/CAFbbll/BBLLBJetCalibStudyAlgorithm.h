#ifndef __BBLLBCALIBSTUDYALGORITHM__
#define __BBLLBCALIBSTUDYALGORITHM__

#include "CAFbbll/BBLLObjectSelectionAlgorithm.h"

class BBLLBJetCalibStudyAlgorithm : public BBLLObjectSelectionAlgorithm {
protected:
    bool virtual select_jets(const xAOD::JetContainer&, const std::string& variation) override;

public:
    BBLLBJetCalibStudyAlgorithm();
    ~BBLLBJetCalibStudyAlgorithm() {};
    ClassDefOverride(BBLLBJetCalibStudyAlgorithm, 1);
};

#endif // not __BBLLBCALIBSTUDYALGORITHM__
