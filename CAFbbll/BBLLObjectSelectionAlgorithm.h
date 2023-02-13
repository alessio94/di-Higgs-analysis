#ifndef __BBLLOBJECTSELECTIONALGORITHM__
#define __BBLLOBJECTSELECTIONALGORITHM__

// STL
#include <set>
#include <string>

// CAFCore
#include "QFramework/TQAlgorithm.h"
#include "QFramework/TQSample.h"
#include "QFramework/TQToken.h"

// ASG
#include "AsgTools/AnaToolHandle.h"
#include "FTagAnalysisInterfaces/IBTaggingSelectionTool.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

// CxAODTools
#include "CxAODTools/OverlapRegisterAccessor.h"

// CAFbbll
#include "CAFbbll/BBLLContainerRetrieval.h"

class BBLLObjectSelectionAlgorithm
    : public TQAlgorithm
    , public BBLLContainerRetrieval {
private:
    TQSample* m_sample = nullptr;

    TQToken* m_event_token = nullptr;
    xAOD::TEvent* m_event = nullptr; //!
    xAOD::TStore* m_store = nullptr; //!

    OverlapRegisterAccessor* m_overlapRegAcc = nullptr; //!
    std::set<std::string> m_variations;

protected:
    std::string m_electron_id = "";
    std::string m_electron_iso = "";
    std::string m_muon_iso = "";
    std::string m_btagging_cdi_file = "";
    asg::AnaToolHandle<IBTaggingSelectionTool> m_btagtool_default; //!

    virtual bool select_leptons(
        const xAOD::ElectronContainer&, const xAOD::MuonContainer&, const std::string& variation);
    virtual bool select_jets(const xAOD::JetContainer&, const std::string& variation);

    template <typename Particle> static bool compare_pt(Particle a, Particle b);
    template <typename Container>
    void save_particles_to_store(
        std::vector<const typename Container::base_value_type*>& particles, std::string container_name);

    template <typename Particle> void print_particle_info(Particle particle);
    template <typename Particle> void print_particle_vector_info(std::vector<Particle> particles, std::string name);

public:
    BBLLObjectSelectionAlgorithm();
    virtual ~BBLLObjectSelectionAlgorithm();

    inline virtual bool isSingleton() const override { return true; }

    virtual bool initializeSingleton(const std::vector<TQSample*>& samples) override;
    virtual bool finalize() override;
    virtual bool cleanup() override;
    virtual bool execute() override;

    void set_electron_id(const std::string& id);
    void set_electron_iso(const std::string& iso);
    void set_muon_iso(const std::string& iso);
    void set_jet_collection(const std::string& name, const std::string& btagging_version);
    void set_btagging_cdi_file(const std::string& cdi_file);
    void add_variation(const std::string& variation);

    ClassDefOverride(BBLLObjectSelectionAlgorithm, 1);
};

#endif // not __BBLLOBJECTSELECTIONALGORITHM__
