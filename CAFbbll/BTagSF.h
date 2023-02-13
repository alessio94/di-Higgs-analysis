#ifndef __BTAGSF__
#define __BTAGSF__

#include "CAFbbll/BBLLContainerRetrieval.h"
#include "CAFxAODUtils/TQEventObservable.h"

#include <AsgTools/AnaToolHandle.h>
#include <FTagAnalysisInterfaces/IBTaggingEfficiencyTool.h>

class BTagSF
    : public TQEventObservable
    , public BBLLContainerRetrieval {
private:
    TString m_tagger = "";
    TString m_operating_point = "";

    std::string m_is_bjet_auxdata_string = "";
    std::string m_btagging_cdi_file = "";

    asg::AnaToolHandle<IBTaggingEfficiencyTool> m_btag_sf_tool; //!

    std::string m_p4_variation = "";
    std::string m_weight_variation = "";
    std::string m_variation = "";

protected:
    bool virtual initializeSelf() override;

public:
    BTagSF();
    BTagSF(const TString& name);
    virtual ~BTagSF();

    virtual double getValue() const override;

    void set_jet_collection(std::string name, std::string btagging_version);
    void set_btagging_cdi_file(std::string cdi_file);

    ClassDefOverride(BTagSF, 1);
};
#endif // not __BTAGSF__
