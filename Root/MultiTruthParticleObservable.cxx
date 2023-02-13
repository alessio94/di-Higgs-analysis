#include "CAFbbll/MultiTruthParticleObservable.h"

#include <algorithm>
#include <regex>

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#ifdef _DEBUG_
#include <iomanip> // std::setw
#endif

MultiTruthParticleObservable::MultiTruthParticleObservable() { DEBUGclass("Default constructor called"); }

MultiTruthParticleObservable::~MultiTruthParticleObservable() { DEBUGclass("Destructor called"); }

MultiTruthParticleObservable::MultiTruthParticleObservable(const TString& expression)
    : TQEventObservable(expression) {
    DEBUGclass("Constructor called with '%s'", expression.Data());
    SetName(TQObservable::makeObservableName(expression));
    setExpression(expression);
}

DEFINE_OBSERVABLE_FACTORY(MultiTruthParticleObservable, TString expression) {
    // a factory for this observable type
    if (expression.BeginsWith("MTP(")) {
        return new MultiTruthParticleObservable(expression);
    }
    return nullptr;
}

bool MultiTruthParticleObservable::initializeSelf() {
    // initialize self - parse expression: get pdg id, particle indices, and variable mode
    return parse_expression(TQObservable::compileExpression(m_expression, fSample));
}

bool MultiTruthParticleObservable::finalizeSelf() {
    // cleanup
    m_pdg_ids.clear();
    m_indices.clear();
    m_quantity = UNKNOWN;
    return true;
}

std::vector<int> MultiTruthParticleObservable::parse_int_list(const std::string& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<int> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(std::stoi(item));
    }
    return tokens;
}

bool MultiTruthParticleObservable::parse_expression(const TString& expr) {
    // Parse the expression of the observable.
    // The observable works in the following way:
    // MTP(pdgid1,pdgid2,...)[index1,index2,...].quantity()
    // One can define which truth particles are selected based on the pdg id.
    // These particles are then ordered by pt and they can be selected by index.
    // The four vectors of all selected particles are added up and the requested quantity is computed.
    // For exactly two truth particles one can also get back for example the DeltaR or DeltaPt.
    // The following examples are all valid:
    // MTP(25)[0].pt
    // MTP(25)[0].pt()
    // MTP(11,13)[0].pt
    // MTP(11,13)[0,1].m
    // MTP(11,13)[0,1].DeltaR
    DEBUGclass("Called with '%s'", expr.Data());

    // For the parsing the following regex is used
    // MTP\((\d+(?:,\d+)*)\)\[(\d+(?:,\d+)*)\]\.(\w+)(?:\(\))?
    // It has 3 capturing groups: list of pdg ids, list of indices, quantity
    // To see how this regex work use for example https://regex101.com/
    std::regex observable_regex("MTP\\((\\d+(?:,\\d+)*)\\)\\[(\\d+(?:,\\d+)*)\\]\\.(\\w+)(?:\\(\\))?");
    std::smatch observable_match;
    std::string observable_expression(expr.Data());
    DEBUGclass("Parsing whole expression...");
    if (!std::regex_match(observable_expression, observable_match, observable_regex)) {
        ERRORclass("Error while creating MultiTruthParticleObservable, the expression '%s' is not in the right format!",
            expr.Data());
        return false;
    }
    // There are 3 capturing groups. The full match is also included and has index 0.In total we expect 4 matches.
    if (observable_match.size() != 4) {
        ERRORclass("Error while creating MultiTruthParticleObservable, could not match all capturing groups!");
        return false;
    }
    // parse the list of pdg ids
    DEBUGclass("Found the following list of pdg ids: '%s'", observable_match[1].str().c_str());
    m_pdg_ids = parse_int_list(observable_match[1].str(), ',');
    std::sort(m_pdg_ids.begin(), m_pdg_ids.end());

    // parse the list of particle indices
    DEBUGclass("Found the following list of particle indices: '%s'", observable_match[2].str().c_str());
    m_indices = parse_int_list(observable_match[2].str(), ',');

    // get quantity
    TString matched_quantity = observable_match[3].str();
    DEBUGclass("Found quantity '%s'", matched_quantity.Data());

    bool hasTwoConstituents = m_indices.size() == 2;
    matched_quantity.ToLower();
    if (matched_quantity == "pt") {
        m_quantity = PT;
        DEBUGclass("Set quantity to PT");
    } else if (matched_quantity == "eta") {
        m_quantity = ETA;
        DEBUGclass("Set quantity to ETA");
    } else if (matched_quantity == "phi") {
        m_quantity = PHI;
        DEBUGclass("Set quantity to PHI");
    } else if (matched_quantity == "e") {
        m_quantity = E;
        DEBUGclass("Set quantity to E");
    } else if (matched_quantity == "m") {
        m_quantity = M;
        DEBUGclass("Set quantity to M");
    } else if (matched_quantity == "deltar") {
        if (!hasTwoConstituents) {
            ERRORclass(
                "The deltaR property can only be computed for exactly two partices, %d given!", m_indices.size());
            return false;
        }
        m_quantity = DELTAR;
        DEBUGclass("Set quantity to DELTAR");
    } else if (matched_quantity == "deltaphi") {
        if (!hasTwoConstituents) {
            ERRORclass(
                "The deltaPhi property can only be computed for exactly two partices, %d given!", m_indices.size());
            return false;
        }
        m_quantity = DELTAPHI;
        DEBUGclass("Set quantity to DELTAPHI");
    } else if (matched_quantity == "deltaeta") {
        if (!hasTwoConstituents) {
            ERRORclass(
                "The deltaEta property can only be computed for exactly two partices, %d given!", m_indices.size());
            return false;
        }
        m_quantity = DELTAETA;
        DEBUGclass("Set quantity to DELTAETA");
    } else if (matched_quantity == "deltapt") {
        if (!hasTwoConstituents) {
            ERRORclass(
                "The deltaPt property can only be computed for exactly two partices, %d given!", m_indices.size());
            return false;
        }
        m_quantity = DELTAPT;
        DEBUGclass("Set quantity to DELTAPT");
    } else if (matched_quantity == "ptscalar") {
        m_quantity = PTSCALAR;
        DEBUGclass("Set quantity to PTSCALAR");
    } else {
        ERRORclass("Quantity '%s' not recognized!", matched_quantity.Data());
        return false;
    }

    return true;
}

bool MultiTruthParticleObservable::compare_pt(const xAOD::TruthParticle* a, const xAOD::TruthParticle* b) {
    return (a->pt() > b->pt());
}

double MultiTruthParticleObservable::getValue() const {
    // value retrieval function, called on every event for every cut and histogram

#ifdef _DEBUG_
    if (!fEvent) {
        ERRORclass("Failed to access xAOD::TEvent member!");
        return false;
    }
#endif // _DEBUG_

    bool isMC = false;
    this->fSample->getTag("~isMC", isMC);
    if (!isMC) { // truth information only available for MC and not for data
        return 0;
    }

    xAOD::TruthParticleContainer const* truth_particles = nullptr;
    if (!this->fEvent->retrieve(truth_particles, "TruthParticles___Nominal").isSuccess()) {
        ERRORclass("Failed to retrieve TruthParticles___Nominal container!");
        return false;
    }

    std::vector<const xAOD::TruthParticle*> filtered_truth_particles;
    for (const xAOD::TruthParticle* p : *truth_particles) {
        // check if current pdg is is in list of pdg ids
        if (std::find(m_pdg_ids.begin(), m_pdg_ids.end(), p->absPdgId()) != m_pdg_ids.end()) {
            filtered_truth_particles.emplace_back(p);
        }
    }
    // sort by pt
    std::sort(filtered_truth_particles.begin(), filtered_truth_particles.end(), compare_pt);

#ifdef _DEBUG_
    std::cout << std::endl;
    for (const xAOD::TruthParticle* truth_particle : filtered_truth_particles) {
        std::cout << "pt=" << std::setw(8) << truth_particle->pt() << "  eta=" << std::setw(10) << truth_particle->eta()
                  << "  phi=" << std::setw(10) << truth_particle->phi() << "  type=" << truth_particle->pdgId()
                  << "  status=" << truth_particle->status() << std::endl
                  << std::endl;
    }
#endif // _DEBUG_

    // check if the size of selected truth particles is bigger than the largest selected index
    int max_index = *std::max_element(m_indices.begin(), m_indices.end());
    int n_truth_particles = filtered_truth_particles.size();
    if (max_index >= n_truth_particles) {
        ERRORclass(
            "You want to access the truth particle with index '%d', but there are only '%d' truth particles selected!",
            max_index, n_truth_particles);
        return 0;
    }

    // check if delta quantity
    if (m_quantity == DELTAR || m_quantity == DELTAPHI || m_quantity == DELTAETA || m_quantity == DELTAPT) {
        DEBUGclass("Calculating DELTA quantity");

        TLorentzVector p4_0 = filtered_truth_particles.at(m_indices.at(0))->p4();
        TLorentzVector p4_1 = filtered_truth_particles.at(m_indices.at(1))->p4();

        double retval = 0;
        switch (m_quantity) {
        case DELTAR:
            retval = p4_0.DeltaR(p4_1);
            break;
        case DELTAPHI:
            retval = p4_0.DeltaPhi(p4_1);
            break;
        case DELTAETA:
            retval = p4_0.Eta() - p4_1.Eta();
            break;
        case DELTAPT:
            retval = p4_0.Pt() - p4_1.Pt();
            break;
        default:
            ERRORclass("Encountered unexpected quantity! Returnin 0.");
        }
        DEBUGclass("Returning %f", retval);
        return retval;
    }

    // return quantity of whole particle system

    // add up all four vectors in system
    TLorentzVector system_p4(0., 0., 0., 0.);
    double pt_scalar = 0; // for pt scalar version
    for (int index : m_indices) {
        const xAOD::TruthParticle* p = filtered_truth_particles.at(index);
        system_p4 += p->p4();
        if (m_quantity == PTSCALAR) {
            pt_scalar += p->pt();
        }
    }

    double retval = 0.;
    switch (m_quantity) {
    case PT:
        retval = system_p4.Pt();
        break;
    case ETA:
        retval = system_p4.Eta();
        break;
    case PHI:
        retval = system_p4.Phi();
        break;
    case E:
        retval = system_p4.E();
        break;
    case M:
        retval = system_p4.M();
        break;
    case PTSCALAR:
        retval = pt_scalar;
        break;
    default:
        ERRORclass("Encountered unexpected quantity! Returnin 0.");
    }

    DEBUGclass("Returning %f", retval);
    return retval;
}
