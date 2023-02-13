#ifndef __BBLLBTAGGINGSTUDYALGORITHM__
#define __BBLLBTAGGINGSTUDYALGORITHM__

#include "CAFbbll/BBLLObjectSelectionAlgorithm.h"

class BBLLBTaggingStudyAlgorithm : public BBLLObjectSelectionAlgorithm {
private:
    asg::AnaToolHandle<IBTaggingSelectionTool> m_btagtool_dl1_fixedeff70;  //!
    asg::AnaToolHandle<IBTaggingSelectionTool> m_btagtool_dl1_fixedeff77;  //!
    asg::AnaToolHandle<IBTaggingSelectionTool> m_btagtool_dl1_fixedeff85;  //!
    asg::AnaToolHandle<IBTaggingSelectionTool> m_btagtool_dl1r_fixedeff70; //!
    asg::AnaToolHandle<IBTaggingSelectionTool> m_btagtool_dl1r_fixedeff77; //!
    asg::AnaToolHandle<IBTaggingSelectionTool> m_btagtool_dl1r_fixedeff85; //!
    // asg::AnaToolHandle<IBTaggingSelectionTool> m_btagtool_dl1_continuous; //!
protected:
    bool virtual select_jets(const xAOD::JetContainer&, const std::string& variation) override;

public:
    BBLLBTaggingStudyAlgorithm();
    ~BBLLBTaggingStudyAlgorithm() {};

    virtual bool initializeSingleton(const std::vector<TQSample*>& samples) override;

    ClassDefOverride(BBLLBTaggingStudyAlgorithm, 1);
};

#endif // not __BBLLBTAGGINGSTUDYALGORITHM__
