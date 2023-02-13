#include "CAFxAODUtils/TQContainerSizeObservable.h"
#include <limits>

#include "QFramework/TQSample.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_

// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include "xAODBase/IParticleContainer.h"

/*@observable: [TQContainerSizeObservable] The TQContainerSizeObservable provides the size of a xAOD container. It is automatically created from expressions of the form
    'SIZE:nameOfContainer'
*/

ClassImp(TQContainerSizeObservable)

//______________________________________________________________________________________________

TQContainerSizeObservable::TQContainerSizeObservable(){
  // default constructor
  DEBUGclass("default constructor called");
}

//______________________________________________________________________________________________

TQContainerSizeObservable::~TQContainerSizeObservable(){
  // default destructor
  DEBUGclass("destructor called");
} 

//______________________________________________________________________________________________

double TQContainerSizeObservable::getValue() const {
  // value retrieval function, called on every event for every cut and histogram
  DEBUGclass("entering function");
  xAOD::IParticleContainer const * container = 0;
  this->retrieve(container, this->fExpression);
  return container->size();
}
//______________________________________________________________________________________________

TQContainerSizeObservable::TQContainerSizeObservable(const TString& expression):
TQEventObservable(expression)
{
  // constructor with expression argument
  DEBUGclass("constructor called with '%s'",expression.Data());
  // the predefined string member "expression" allows your observable to store an expression of your choice
  // this string will be the verbatim argument you passed to the constructor of your observable
  // you can use it to choose between different modes or pass configuration options to your observable
  this->SetName(TQObservable::makeObservableName(expression));
  this->setExpression(expression);
}

//______________________________________________________________________________________________

const TString& TQContainerSizeObservable::getExpression() const {
  // retrieve the expression associated with this observable
  return this->fExpression;
}

//______________________________________________________________________________________________

bool TQContainerSizeObservable::hasExpression() const {
  // check if this observable type knows expressions
  return true;
}

//______________________________________________________________________________________________

void TQContainerSizeObservable::setExpression(const TString& expr){
  // set the expression to a given string
  this->fExpression = expr;
}
//______________________________________________________________________________________________

DEFINE_OBSERVABLE_FACTORY(TQContainerSizeObservable,TString expr){
  // try to create an instance of this observable from the given expression
  // return the newly created observable upon success
  // or NULL upon failure

  // first, check if the expression fits your observable type
  // for example, you can grab all expressions that begin wth "TQContainerSizeObservable:"
  if(TQStringUtils::removeLeadingText(expr,"SIZE:")){
    // if this is the case, then we call the expression-constructor
    return new TQContainerSizeObservable(expr);
  }
  return NULL;
}

