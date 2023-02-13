#include "CAFbbll/MultiParticleObservable.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include <regex>

MultiParticleObservable::MultiParticleObservable() { DEBUGclass("Default constructor called"); }

MultiParticleObservable::~MultiParticleObservable() { DEBUGclass("Destructor called"); }

MultiParticleObservable::MultiParticleObservable(const TString& expression)
    : TQEventObservable(expression) {
    DEBUGclass("Constructor called with '%s'", expression.Data());
    SetName(TQObservable::makeObservableName(expression));
    setExpression(expression);
}

DEFINE_OBSERVABLE_FACTORY(MultiParticleObservable, TString expression) {
    // a factory for this observable type
    if (expression.BeginsWith("MP(")) {
        return new MultiParticleObservable(expression);
    }
    return NULL;
}

bool MultiParticleObservable::initializeSelf() {
    // initialize self - parse expression, get container names and variable mode
    return parse_expression(TQObservable::compileExpression(getExpression(), fSample));
}

bool MultiParticleObservable::finalizeSelf() {
    // cleanup
    m_quantity = UNKNOWN;
    m_constituents.clear();
    m_containers.clear();
    return true;
}

bool MultiParticleObservable::parse_expression(const TString& expr) {
    // parse the expression of the observable
    // the following examples are all valid
    // MP(MyContainer(0,1)).m
    // MP(MyContainer(0,1)).m()
    // MP(MyContainer1(0),MyContainer2(1)).pt
    // MP(MyContainer1(0,1),MyContainer2(1)).eta
    // MP(MyContainer1(0,1),MyContainer2(1),MyContainer3(0,1)).m
    DEBUGclass("Called with '%s'", expr.Data());

    // the parsing happens in two steps, in the first step the arguments of the observable (i.e. the MyContainer(0,...)
    // parts) and the quantity to obtain in the end are parsed for this the following regex is used:
    // MP\((\w+\(\d+(?:,\d+)*\))(?:,(\w+\(\d+(?:,\d+)*\)))*\)\.(\w+)(?:\(\))?
    // To see how this regex work use for example https://regex101.com/
    std::regex observable_regex(
        "MP\\((\\w+\\(\\d+(?:,\\d+)*\\))(?:,(\\w+\\(\\d+(?:,\\d+)*\\)))*\\)\\.(\\w+)(?:\\(\\))?");
    std::smatch observable_match;
    std::string observable_expression(expr.Data());
    DEBUGclass("Parsing whole expression...");
    if (!std::regex_match(observable_expression, observable_match, observable_regex)) {
        ERRORclass("Error while creating MultiParticleObservable, the expression '%s' is not in the right format!",
            expr.Data());
        return false;
    }
    // because of how the regex is constructed we know that there is at least one container and the quantity given
    if (observable_match.size() < 3) { // 0: first match, 1 - second last: container, last: quantity
        ERRORclass("Error while creating MultiParticleObservable, the size of sub-matches is lower than expected!");
        return false;
    }
    // get container arguments
    std::vector<std::string> container_expressions;
    for (size_t i = 1; i < observable_match.size() - 1; ++i) { // start with one since the first entry is the full match
        std::string container_expression = observable_match[i].str();
        if (container_expression.empty())
            continue; // filter out empty matches
        container_expressions.push_back(container_expression);
        DEBUGclass("Found argument '%s' ", container_expression.c_str());
    }
    // get quantity
    TString matched_quantity = observable_match[observable_match.size() - 1].str();
    DEBUGclass("Found quantity '%s'", matched_quantity.Data());

    // second step: parse the individual container definitions
    // the result is the name of the container and all the indices of the particles
    // for this the following regex is used:
    // (\w+)\((\d+)(?:,(\d+))*\)
    std::regex container_regex("(\\w+)\\((\\d+)(?:,(\\d+))*\\)");
    std::smatch container_match;
    for (const auto& container_expression : container_expressions) {
        if (!std::regex_match(container_expression, container_match, container_regex)) {
            ERRORclass(
                "Unexpected error while parsing container expression '%s', since this matched in the first step!",
                container_expression.c_str());
            return false;
        }
        if (container_match.size() < 3) { // 0: first match, 1: container name, second - last: index
            ERRORclass("Error while parsing container definition '%s', the size of sub-matches is lower than expected!",
                container_expression.c_str());
            return false;
        }
        TString container_name = container_match[1].str();
        DEBUGclass("Found container definition with name '%s'", container_name.Data());
        for (size_t i = 2; i < container_match.size(); ++i) {
            std::string index_str = container_match[i].str();
            if (index_str.empty())
                continue; // filter out empty matches
            int index = atoi(index_str.c_str());
            DEBUGclass("Found index '%d'", index);
            m_constituents.push_back({ container_name, index });
        }
    }

#ifdef _DEBUG_
    DEBUGclass("Overview of all constituents in this observable:");
    for (auto constituent : m_constituents) {
        DEBUGclass("Container: %s, Index: %d", constituent.container_name.Data(), constituent.index);
    }
#endif

    bool hasTwoConstituents = m_constituents.size() == 2;
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
                "The deltaR property can only be computed for exactly two partices, %d given!", m_constituents.size());
            return false;
        }
        m_quantity = DELTAR;
        DEBUGclass("Set quantity to DELTAR");
    } else if (matched_quantity == "deltaphi") {
        if (!hasTwoConstituents) {
            ERRORclass("The deltaPhi property can only be computed for exactly two partices, %d given!",
                m_constituents.size());
            return false;
        }
        m_quantity = DELTAPHI;
        DEBUGclass("Set quantity to DELTAPHI");
    } else if (matched_quantity == "deltaeta") {
        if (!hasTwoConstituents) {
            ERRORclass("The deltaEta property can only be computed for exactly two partices, %d given!",
                m_constituents.size());
            return false;
        }
        m_quantity = DELTAETA;
        DEBUGclass("Set quantity to DELTAETA");
    } else if (matched_quantity == "deltapt") {
        if (!hasTwoConstituents) {
            ERRORclass(
                "The deltaPt property can only be computed for exactly two partices, %d given!", m_constituents.size());
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

bool MultiParticleObservable::retrieve_containers() const {
    m_containers.clear();
    for (auto constituent : m_constituents) {
        if (m_containers.count(constituent.container_name))
            continue; // container already exists in map
        const xAOD::IParticleContainer* tmp_container;
        if (!fEvent->retrieve(tmp_container, constituent.container_name.Data())) {
            ERRORclass("Failed to retrieve container '%s'", constituent.container_name.Data());
            return false;
        }
        DEBUGclass("Successfully retrieved container '%s'", constituent.container_name.Data());
        m_containers[constituent.container_name] = tmp_container;
    }
    return true;
}

const xAOD::IParticle* MultiParticleObservable::getParticle(MultiParticleConstituent constituent) const {
    DEBUGclass("Trying to get access particle with index '%d' from container '%s'", constituent.index,
        constituent.container_name.Data());
    const xAOD::IParticleContainer* container = m_containers.at(constituent.container_name);
    if (constituent.index >= (int)container->size()) {
        DEBUGclass("Unable to access particle with index '%d' from container '%s', since there are only '%d' particles "
                   "stored!",
            constituent.index, constituent.container_name.Data(), container->size());
        DEBUGclass("The expression of this observable is '%s'", getExpression().Data());
        return nullptr;
    }
    return container->at(constituent.index);
}

double MultiParticleObservable::getValue() const {
    // value retrieval function, called on every event for every cut and histogram

#ifdef _DEBUG_
    if (!fEvent) {
        ERRORclass("Failed to access xAOD::TEvent member!");
        return false;
    }
#endif // _DEBUG_

    if (!retrieve_containers()) {
        ERRORclass("Error while retrieving containers! Returning 0.");
        return 0;
    }

    // check if delta quantity
    if (m_quantity == DELTAR || m_quantity == DELTAPHI || m_quantity == DELTAETA || m_quantity == DELTAPT) {
        DEBUGclass("Calculating 2 particle angular difference");
        // sanity checks were done in parse_expression, so if the quantity is DELTA* we have exactly two constituents
        const xAOD::IParticle* p0 = getParticle(m_constituents[0]);
        const xAOD::IParticle* p1 = getParticle(m_constituents[1]);

        if (!p0 || !p1) {
            return DEFAULT_VALUE;
        }

        TLorentzVector p4_0 = p0->p4();
        TLorentzVector p4_1 = p1->p4();

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

    // return quantity of whole particle system, no restriction on number of constituents

    // add up all four vectors in system
    TLorentzVector system_p4(0., 0., 0., 0.);
    double pt_scalar = 0; // for pt scalar version
    for (auto constituent : m_constituents) {
        const xAOD::IParticle* p = getParticle(constituent);
        if (!p) {
            return DEFAULT_VALUE;
        }
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
