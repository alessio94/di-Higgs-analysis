#include "CAFbbll/TQIParticleDefaultDecorationObservable.h"
#include <limits>
#include <sstream>
#include <stdexcept>

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
// #define _DEBUG_
#include "QFramework/TQLibrary.h"

/*@observable: [TQIParticleDefaultDecorationObservable] The TQIParticleDefaultDecorationObservable can be used to read
  the value of decorations of xAOD::IParticle objects. It is automatically created when encountering expressions like
  'IP?DD:nameOfContainer[index]:nameOm_decoration:default'. It works similar to the TQIParticleDecorationObservable but
  if the particle with the requested index does not exist the default value is returned.
  */

template <class Taux> TQIParticleDefaultDecorationObservable<Taux>::TQIParticleDefaultDecorationObservable() {
    DEBUGclass("Default constructor called");
}

template <class Taux> TQIParticleDefaultDecorationObservable<Taux>::~TQIParticleDefaultDecorationObservable() {
    DEBUGclass("Destructor called");
}

template <class Taux>
TQIParticleDefaultDecorationObservable<Taux>::TQIParticleDefaultDecorationObservable(const TString& expression)
    : TQEventObservable(expression) {
    // constructor with expression argument
    DEBUGclass("constructor called with '%s'", expression.Data());
    setExpression(expression);
    SetName(TQObservable::makeObservableName(getExpression()));
}

DEFINE_TEMPLATE_OBSERVABLE_FACTORY(TQIParticleDefaultDecorationObservable, Taux, TString expression) {
    // a factory for this observable type
    if (TQStringUtils::removeLeadingText(
            expression, TQIParticleDefaultDecorationObservable<Taux>::getExpressionPrefix() + ":")) {
        return new TQIParticleDefaultDecorationObservable<Taux>(expression);
    }
    return nullptr;
}

PREFIX_ALL_TEMPLATES(TQIParticleDefaultDecorationObservable, IP, DD)

INSTANTIATE_ALL_TEMPLATES(TQIParticleDefaultDecorationObservable)

template <class Taux> double TQIParticleDefaultDecorationObservable<Taux>::getValue() const {
    // value retrieval function, called on every event for every cut and histogram
    DEBUGclass("Entering function");
    // the following protective if-block should NEVER evaluate true
    // for performance reasons, we disable it per preprocessor directive unless in debugging mode
#ifdef _DEBUG_
    if (!fEvent) {
        ERRORclass("Failed to access xAOD::TEvent member!");
        abort();
        return 0;
    }
#endif

    retrieve(m_container, m_container_name.Data());

    if (m_container->size() <= m_container_index) {
        DEBUGclass("Unable to retrieve entry %d from container %s, using default value.", m_container_index,
            m_container_name.Data());
        return m_default_value;
    }

    DEBUGclass("Reading aux-data (object-level)");
    const xAOD::IParticle* pp = m_container->at(m_container_index);
    if (!m_deco_access->isAvailable(*pp)) {
        DEBUGclass("Unable to find decorator, using default value.");
        return m_default_value;
    } else {
        const double retval = (*(m_deco_access))(*pp);
        DEBUGclass("Got: %lf\n", retval);
        return retval;
    }
}

template <class Taux> TString TQIParticleDefaultDecorationObservable<Taux>::getActiveExpression() const {
    // retrieve the expression associated with this observable
    if (m_cached_expression.IsNull()) {
        m_cached_expression = TString::Format("%s:%s",
            TQIParticleDefaultDecorationObservable<Taux>::getExpressionPrefix().Data(), m_container_name.Data());
        m_cached_expression.Append(":");
        m_cached_expression.Append(m_decoration);
    }
    return m_cached_expression;
}

template <class Taux> bool TQIParticleDefaultDecorationObservable<Taux>::initializeSelf() {
    // initialize self - compile container name, construct accessor
    if (!parseExpression(TQObservable::compileExpression(m_expression, fSample))) {
        return false;
    }
    m_deco_access = new SG::AuxElement::Accessor<Taux>(m_decoration.Data());
    return true;
}

template <class Taux> bool TQIParticleDefaultDecorationObservable<Taux>::finalizeSelf() {
    // finalize self - delete accessor
    if (m_deco_access) {
        delete m_deco_access;
        m_deco_access = nullptr;
    }
    clearParsedExpression();
    return true;
}

template <class Taux> void TQIParticleDefaultDecorationObservable<Taux>::clearParsedExpression() {
    // clear the current expression
    m_container_name.Clear();
    m_decoration.Clear();
    m_cached_expression.Clear();
}

template <class Taux> bool TQIParticleDefaultDecorationObservable<Taux>::parseExpression(const TString& expr) {
    TString input(expr);
    // set the expression to a given string
    clearParsedExpression();

    if (!TQEventObservable::parseParticleReference(input, m_container_name, m_container_index)) {
        ERRORclass("unable to read particle reference from expression '%s'", expr.Data());
        return false;
    }

    if (TQStringUtils::removeLeading(input, ":") != 1) {
        ERRORclass("missing auxdata declaration in expression", expr.Data());
        return false;
    }
    // read m_decoration
    TString DecoTempString = "";
    if (TQStringUtils::readToken(input, DecoTempString, TQStringUtils::alphanumvarext) == 0) {
        ERRORclass("missing decoration declaration in expression", expr.Data());
    }
    m_decoration = DecoTempString;
    // get default value
    if (TQStringUtils::removeLeading(input, ":") != 1) {
        ERRORclass("missing auxdata declaration in expression", expr.Data());
        return false;
    }
    if (input.Length() < 1) {
        ERRORclass("missing auxdata declaration in expression", expr.Data());
        return false;
    }
    m_default_value = input.Atof();
    // show info
    DEBUGclass("Initialized TQIParticleDefaultDecorationObservable with : %s[%d]:%s:%lf", m_container_name.Data(),
        m_container_index, m_decoration.Data(), m_default_value);
    return true;
}
