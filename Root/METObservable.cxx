#include "CAFbbll/METObservable.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include "xAODMissingET/MissingETContainer.h"

METObservable::METObservable() { DEBUGclass("Default constructor called"); }

METObservable::~METObservable() { DEBUGclass("Destructor called"); }

METObservable::METObservable(const TString& name)
    : TQEventObservable(name) {
    DEBUGclass("Constructor called with '%s'", name.Data());

    if (name == TString("MET:sumet")) {
        m_mode = SUMET;
    } else if (name == TString("MET:met")) {
        m_mode = MET;
    } else if (name == TString("MET:metx")) {
        m_mode = METX;
    } else if (name == TString("MET:mety")) {
        m_mode = METY;
    } else if (name == TString("MET:metphi")) {
        m_mode = METPHI;
    } else if (name == TString("MET:soft_sumet")) {
        m_mode = SOFT_SUMET;
    } else if (name == TString("MET:soft_met")) {
        m_mode = SOFT_MET;
    } else if (name == TString("MET:soft_metx")) {
        m_mode = SOFT_METX;
    } else if (name == TString("MET:soft_mety")) {
        m_mode = SOFT_METY;
    } else if (name == TString("MET:metrho")) {
        m_mode = METRHO;
    } else if (name == TString("MET:metsig")) {
        m_mode = METSIG;
    } else if (name == TString("MET:metsig_soft")) {
        m_mode = METSIG_SOFT;
    } else if (name == TString("MET:metsig_hard")) {
        m_mode = METSIG_HARD;
    } else {
        ERRORclass("Unkown mode for MET observable: '%s'", name.Data());
        m_mode = UNKNOWN;
    }
}

double METObservable::getValue() const {
    if (!fSample) {
        ERRORclass("Failed to access TQSample member!");
        return false;
    }
    std::string variation = fSample->getTagStandardStringDefault("~p4Variation", "Nominal");

    const xAOD::MissingETContainer* met_container = nullptr;
    if (!get_xaod_met_container(fEvent, variation, met_container)) {
        return false;
    }

    xAOD::MissingET const* missingET = met_container->at(0);

    switch (m_mode) {

    case SUMET:
        return missingET->sumet();

    case MET:
        return missingET->met();

    case METX:
        return missingET->mpx();

    case METY:
        return missingET->mpy();

    case METPHI:
        return missingET->phi();

    case SOFT_SUMET:
        return missingET->auxdataConst<float>("soft_sumet");

    case SOFT_MET: {
        float soft_mpx2 = missingET->auxdataConst<float>("soft_mpx") * missingET->auxdataConst<float>("soft_mpx");
        float soft_mpy2 = missingET->auxdataConst<float>("soft_mpy") * missingET->auxdataConst<float>("soft_mpy");
        return sqrt(soft_mpx2 + soft_mpy2);
    }

    case SOFT_METX:
        return missingET->auxdataConst<float>("soft_mpx");

    case SOFT_METY:
        return missingET->auxdataConst<float>("soft_mpy");

    case METRHO:
        return missingET->auxdataConst<float>("metRho");

    case METSIG:
        return missingET->auxdataConst<float>("metSig");

    case METSIG_SOFT:
        return missingET->auxdataConst<float>("metSig_soft");

    case METSIG_HARD:
        return missingET->auxdataConst<float>("metSig_hard");

    default:
        return -1e11;
    }
}

void METObservable::set_met_collection(const std::string& jet_collection) {
    std::string met_container_name = "MET_Reference_AntiKt4" + jet_collection;
    set_met_container_name(met_container_name);
    DEBUGclass("Set met container name to '%s'", met_container_name.c_str());
}
