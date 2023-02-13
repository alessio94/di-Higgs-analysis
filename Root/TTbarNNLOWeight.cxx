#include "CAFbbll/TTbarNNLOWeight.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include "xAODEventInfo/EventInfo.h"

TTbarNNLOWeight::TTbarNNLOWeight() { DEBUGclass("Default constructor called"); }

TTbarNNLOWeight::~TTbarNNLOWeight() { DEBUGclass("Destructor called"); }

TTbarNNLOWeight::TTbarNNLOWeight(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());
}

TTbarSampleType dsid_to_sample_type(int dsid) {
    if (dsid >= 410470 && dsid <= 410472) {
        return TTbarSampleType::Powheg_Pythia8_Nominal;
    }
    if (dsid >= 410424 && dsid <= 410427) {
        return TTbarSampleType::Sherpa;
    }
    if (dsid >= 410557 && dsid <= 410559) {
        return TTbarSampleType::Powheg_Herwig7;
    }
    if (dsid >= 410464 && dsid <= 410466) {
        return TTbarSampleType::aMCNLO_Pythia8;
    }
    if (dsid >= 410480 && dsid <= 410482) {
        return TTbarSampleType::Powheg_Pythia8_Hdamp;
    }
    return TTbarSampleType::None;
}

bool TTbarNNLOWeight::initializeSelf() {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    bool isMC = false;
    fSample->getTag("~isMC", isMC);

    if (!isMC)
        return true;

    int dsid = 0;
    fSample->getTag("~dsid", dsid);
    m_ttbar_sample_type = dsid_to_sample_type(dsid);

    DEBUGclass("dsid: %d", dsid);

    if (m_ttbar_sample_type != TTbarSampleType::None) {
        if (m_data_path.empty()) {
            ERRORclass("data_path not set!");
            return false;
        }
        m_ttbar_nnlo_reweighter = new TTbarNNLOReweighter(dsid, m_data_path);
        m_ttbar_nnlo_reweighter->Init();
    }

    return true;
}

bool TTbarNNLOWeight::finalizeSelf() {
    m_ttbar_sample_type = TTbarSampleType::None;
    if (m_ttbar_nnlo_reweighter) {
        delete m_ttbar_nnlo_reweighter;
        m_ttbar_nnlo_reweighter = nullptr;
    }
    return true;
}

void TTbarNNLOWeight::set_data_path(const std::string& data_path) {
    DEBUGclass("Set data_path to %s", data_path.c_str());
    m_data_path = data_path;
}

double TTbarNNLOWeight::getValue() const {
    if (m_ttbar_sample_type == TTbarSampleType::None)
        return 1;

    // TODO handle weight variation
    std::string variation = "Nominal";
    const xAOD::EventInfo* event_info = nullptr;
    if (!get_xaod_eventinfo(fEvent, variation, event_info)) {
        return false;
    }

    float ttbar_truth_top_pt_gev = event_info->auxdataConst<float>("TTbarNNLOReweighterTopPt") / 1000;
    DEBUGclass("Truth top pt = %.3f [GeV]", ttbar_truth_top_pt_gev);
    float weight = 1.0;
    switch (m_ttbar_sample_type) {
    case TTbarSampleType::Powheg_Pythia8_Nominal:
        weight = m_ttbar_nnlo_reweighter->GetTopPt_Powheg_Pythia8_Nominal(ttbar_truth_top_pt_gev);
        break;
    case TTbarSampleType::Sherpa:
        weight = m_ttbar_nnlo_reweighter->GetTopPt_Sherpa(ttbar_truth_top_pt_gev);
        break;
    case TTbarSampleType::Powheg_Herwig7:
        weight = m_ttbar_nnlo_reweighter->GetTopPt_Powheg_Herwig7(ttbar_truth_top_pt_gev);
        break;
    case TTbarSampleType::aMCNLO_Pythia8:
        weight = m_ttbar_nnlo_reweighter->GetTopPt_aMCNLO_Pythia8(ttbar_truth_top_pt_gev);
        break;
    case TTbarSampleType::Powheg_Pythia8_Hdamp:
        weight = m_ttbar_nnlo_reweighter->GetTopPt_Powheg_Pythia8_Hdamp(ttbar_truth_top_pt_gev);
        break;
    case TTbarSampleType::None:
        // how did we get here?
        break;
    }

    DEBUGclass("[%s] weight=%f", variation.c_str(), weight);
    return weight;
}
