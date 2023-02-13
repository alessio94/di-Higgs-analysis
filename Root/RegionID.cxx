#include "CAFbbll/RegionID.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicVariation.h"
#include "xAODJet/JetContainer.h"

RegionID::RegionID() { DEBUGclass("Default constructor called"); }

RegionID::~RegionID() { DEBUGclass("Destructor called"); }

RegionID::RegionID(const TString& name)
    : TQEventObservable(name) { }

bool RegionID::initializeSelf() {
    DEBUGclass("initializeSelf called");
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }

    std::shared_ptr<TQTaggable> aliases = TQTaggable::getGlobalTaggable("aliases");

    m_mll_obs =
        TQEventObservable::getObservable(aliases->replaceInTextRecursive("MP($(LeptonContainer)(0,1)).M()"), fSample);
    if (!m_mll_obs->initialize(fSample)) {
        return false;
    }

    m_mbb_obs =
        TQEventObservable::getObservable(aliases->replaceInTextRecursive("MP($(BJetContainer)(0,1)).M()"), fSample);
    if (!m_mbb_obs->initialize(fSample)) {
        return false;
    }

    m_nbjets_obs = TQEventObservable::getObservable(aliases->replaceInTextRecursive("SIZE:$(BJetContainer)"), fSample);
    if (!m_nbjets_obs->initialize(fSample)) {
        return false;
    }

    return true;
}

bool RegionID::finalizeSelf() {
    DEBUGclass("finalizeSelf called");

    if (m_mll_obs) {
        m_mll_obs->finalize();
        m_mll_obs = nullptr;
    }

    if (m_mbb_obs) {
        m_mbb_obs->finalize();
        m_mbb_obs = nullptr;
    }

    if (m_nbjets_obs) {
        m_nbjets_obs->finalize();
        m_nbjets_obs = nullptr;
    }

    return true;
}

void RegionID::fillOutputVector() const {
    const Long64_t entry = getCurrentEntry();
    if (entry != m_cached_entry) { // new event, fill vector with region indices
        m_cached_entry = entry;
        m_cached_values.clear();

        bool isSF = true;
        fSample->getTag("~isSF", isSF);
        int nbjets = m_nbjets_obs->getValue();
        double mll = m_mll_obs->getValue() / 1000.;

        if (nbjets == 2) {
            double mbb = m_mbb_obs->getValue() / 1000.;

            if (isSF && mll > 15 && mll < 75) { // SR1 SF
                m_cached_values.push_back(SR1_2TAG);
            }

            if (!isSF && mll > 15 && mll < 110) { // SR1 DF
                m_cached_values.push_back(SR1_2TAG);
            }

            if (isSF && mll >= 75 && mll < 110) { // SR2 && Zll CR
                if (mbb > 40 && mbb < 210) {      // SR2
                    m_cached_values.push_back(SR2_2TAG);
                } else { // Zll CR
                    m_cached_values.push_back(ZLLCR_2TAG);
                }
            }

            if (mll >= 110) { // Top CR
                m_cached_values.push_back(TOPCR_2TAG);
            }
        } else if (nbjets == 1) {

            if (isSF && mll > 15 && mll < 75) { // SR1 SF
                m_cached_values.push_back(SR1_1TAG);
            }

            if (!isSF && mll > 15 && mll < 110) { // SR1 DF
                m_cached_values.push_back(SR1_1TAG);
            }

            if (isSF && mll >= 75 && mll < 110) { // SR2 && Zll CR
                m_cached_values.push_back(SR2_1TAG);
            }

            if (mll >= 110) { // Top CR
                m_cached_values.push_back(TOPCR_1TAG);
            }
        }
    }
}

int RegionID::getNevaluations() const {
    fillOutputVector();
    return m_cached_values.size();
}

double RegionID::getValue() const {
    throw std::runtime_error("Caught attempt to perform scalar evaluation on RegionID observable");
}

double RegionID::getValueAt(int index) const {
    DEBUGclass("entering function");
    fillOutputVector();
    return m_cached_values.at(index);
}
