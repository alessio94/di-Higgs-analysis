#include "CAFbbll/BBLLBTaggingStudyAlgorithm.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// you can perform debug printouts with statements like this
// DEBUG("error number %d occurred",someInteger);
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

BBLLBTaggingStudyAlgorithm::BBLLBTaggingStudyAlgorithm()
    : BBLLObjectSelectionAlgorithm() { }

bool BBLLBTaggingStudyAlgorithm::initializeSingleton(const std::vector<TQSample*>& samples) {
    // initialize default stuff
    if (!BBLLObjectSelectionAlgorithm::initializeSingleton(samples)) {
        return false;
    }

    // initialize tools for b-tagging study

    // DL1r

    m_btagtool_dl1r_fixedeff70 =
        asg::AnaToolHandle<IBTaggingSelectionTool>("BTaggingSelectionTool/BTaggingSelectionTool_DL1r_FixedCutBEff_70");
    m_btagtool_dl1r_fixedeff70.setProperty("OutputLevel", MSG::WARNING).ignore(); // disable info messages
    m_btagtool_dl1r_fixedeff70.setProperty("FlvTagCutDefinitionsFileName", m_btagging_cdi_file.c_str()).ignore();
    m_btagtool_dl1r_fixedeff70.setProperty("JetAuthor", jet_container_name()).ignore();
    m_btagtool_dl1r_fixedeff70.setProperty("TaggerName", "DL1r").ignore();
    m_btagtool_dl1r_fixedeff70.setProperty("OperatingPoint", "FixedCutBEff_70").ignore();
    m_btagtool_dl1r_fixedeff70.setProperty("MinPt", 20000).ignore();
    if (!m_btagtool_dl1r_fixedeff70.retrieve().isSuccess()) {
        ERRORclass("Could not initalize BTaggingSelectionTool (dl1r, 70%)!");
        return false;
    }

    m_btagtool_dl1r_fixedeff77 =
        asg::AnaToolHandle<IBTaggingSelectionTool>("BTaggingSelectionTool/BTaggingSelectionTool_DL1r_FixedCutBEff_77");
    m_btagtool_dl1r_fixedeff77.setProperty("OutputLevel", MSG::WARNING).ignore(); // disable info messages
    m_btagtool_dl1r_fixedeff77.setProperty("FlvTagCutDefinitionsFileName", m_btagging_cdi_file.c_str()).ignore();
    m_btagtool_dl1r_fixedeff77.setProperty("JetAuthor", jet_container_name()).ignore();
    m_btagtool_dl1r_fixedeff77.setProperty("TaggerName", "DL1r").ignore();
    m_btagtool_dl1r_fixedeff77.setProperty("OperatingPoint", "FixedCutBEff_77").ignore();
    m_btagtool_dl1r_fixedeff77.setProperty("MinPt", 20000).ignore();
    if (!m_btagtool_dl1r_fixedeff77.retrieve().isSuccess()) {
        ERRORclass("Could not initalize BTaggingSelectionTool (dl1r, 77%)!");
        return false;
    }

    m_btagtool_dl1r_fixedeff85 =
        asg::AnaToolHandle<IBTaggingSelectionTool>("BTaggingSelectionTool/BTaggingSelectionTool_DL1r_FixedCutBEff_85");
    m_btagtool_dl1r_fixedeff85.setProperty("OutputLevel", MSG::WARNING).ignore(); // disable info messages
    m_btagtool_dl1r_fixedeff85.setProperty("FlvTagCutDefinitionsFileName", m_btagging_cdi_file.c_str()).ignore();
    m_btagtool_dl1r_fixedeff85.setProperty("JetAuthor", jet_container_name()).ignore();
    m_btagtool_dl1r_fixedeff85.setProperty("TaggerName", "DL1r").ignore();
    m_btagtool_dl1r_fixedeff85.setProperty("OperatingPoint", "FixedCutBEff_85").ignore();
    m_btagtool_dl1r_fixedeff85.setProperty("MinPt", 20000).ignore();
    if (!m_btagtool_dl1r_fixedeff85.retrieve().isSuccess()) {
        ERRORclass("Could not initalize BTaggingSelectionTool (dl1r, 85%)!");
        return false;
    }

    // continuous b-tagging

    // m_btagtool_dl1_continuous.setProperty("FlvTagCutDefinitionsFileName", m_btagging_cdi_file.c_str()).ignore();
    // m_btagtool_dl1_continuous.setProperty("JetAuthor", jet_container_name()).ignore();
    // m_btagtool_dl1_continuous.setProperty("TaggerName", "DL1").ignore();
    // m_btagtool_dl1_continuous.setProperty("OperatingPoint", "Continuous").ignore();
    // m_btagtool_dl1_continuous.setProperty("MinPt", 20000).ignore();
    // if (!m_btagtool_dl1_continuous.retrieve().isSuccess()) {
    //     ERRORclass("Could not initalize BTaggingSelectionTool (continuous)!");
    // }

    return true;
}

bool BBLLBTaggingStudyAlgorithm::select_jets(const xAOD::JetContainer& jets, const std::string& variation) {
    if (!BBLLObjectSelectionAlgorithm::select_jets(jets, variation)) {
        return false;
    }

    std::vector<const xAOD::Jet*> selected_bjets_DL1r_fixedeff70;
    std::vector<const xAOD::Jet*> selected_bjets_DL1r_fixedeff77;
    std::vector<const xAOD::Jet*> selected_bjets_DL1r_fixedeff85;

    for (const xAOD::Jet* jet : jets) {
        if (!Props::passOR.get(jet)) {
            continue;
        }

        double jet_pt = jet->pt();
        double jet_eta = jet->eta();
        double jet_dl1r_pb = jet->auxdataConst<double>("DL1r_pb");
        double jet_dl1r_pc = jet->auxdataConst<double>("DL1r_pc");
        double jet_dl1r_pu = jet->auxdataConst<double>("DL1r_pu");

        double tagweight;

        m_btagtool_dl1r_fixedeff70->getTaggerWeight(jet_dl1r_pb, jet_dl1r_pc, jet_dl1r_pu, tagweight).ignore();
        bool isBJet_DL1r_fixedeff70 = m_btagtool_dl1r_fixedeff70->accept(jet_pt, jet_eta, tagweight);
        jet->auxdecor<int>("CAFisBJet_DL1r_FixedCutBEff_70") = isBJet_DL1r_fixedeff70;
        jet->auxdecor<float>("CAFBJet_DL1r_FixedCutBEff_70_tagweight") = tagweight;
        if (isBJet_DL1r_fixedeff70) {
            selected_bjets_DL1r_fixedeff70.push_back(jet);
        }
        m_btagtool_dl1r_fixedeff77->getTaggerWeight(jet_dl1r_pb, jet_dl1r_pc, jet_dl1r_pu, tagweight).ignore();
        bool isBJet_DL1r_fixedeff77 = m_btagtool_dl1r_fixedeff77->accept(jet_pt, jet_eta, tagweight);
        jet->auxdecor<int>("CAFisBJet_DL1r_FixedCutBEff_77") = isBJet_DL1r_fixedeff77;
        jet->auxdecor<float>("CAFBJet_DL1r_FixedCutBEff_77_tagweight") = tagweight;
        if (isBJet_DL1r_fixedeff77) {
            selected_bjets_DL1r_fixedeff77.push_back(jet);
        }
        m_btagtool_dl1r_fixedeff85->getTaggerWeight(jet_dl1r_pb, jet_dl1r_pc, jet_dl1r_pu, tagweight).ignore();
        bool isBJet_DL1r_fixedeff85 = m_btagtool_dl1r_fixedeff85->accept(jet_pt, jet_eta, tagweight);
        jet->auxdecor<int>("CAFisBJet_DL1r_FixedCutBEff_85") = isBJet_DL1r_fixedeff85;
        jet->auxdecor<float>("CAFBJet_DL1r_FixedCutBEff_85_tagweight") = tagweight;
        if (isBJet_DL1r_fixedeff85) {
            selected_bjets_DL1r_fixedeff85.push_back(jet);
        }

        // continuous b-tagging
        // m_btagtool_dl1_continuous->getTaggerWeight(jet_dl1_pb, jet_dl1_pc, jet_dl1_pu, tagweight).ignore();
        // jet->auxdecor<int>("pcbt_dl1_quantile") = m_btagtool_dl1_continuous->getQuantile(jet_pt, jet_eta,
        // tagweight);
    }

    save_particles_to_store<xAOD::JetContainer>(
        selected_bjets_DL1r_fixedeff70, "SelectedCAFBJetsDL1rFixedEff70___" + variation);
    save_particles_to_store<xAOD::JetContainer>(
        selected_bjets_DL1r_fixedeff77, "SelectedCAFBJetsDL1rFixedEff77___" + variation);
    save_particles_to_store<xAOD::JetContainer>(
        selected_bjets_DL1r_fixedeff85, "SelectedCAFBJetsDL1rFixedEff85___" + variation);

    return true;
}
