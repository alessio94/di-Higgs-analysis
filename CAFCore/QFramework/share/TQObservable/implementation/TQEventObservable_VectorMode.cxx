
#define XAOD_STANDALONE 1
// put here any EDM includes you might need, e.g.
/* example block
#include "xAODParticleEvent/CompositeParticleContainer.h"
*/

//______________________________________________________________________________________________

bool ObsName::makeCache() const {
  // function that chaches vector that is later retrieved with function getVector()
  // called on every event for every cut and histogram
  
  DEBUGclass("entering function");

  // the TQEventObservable only works in an ASG RELEASE, hence
  // we encapsulate the implementation in an ifdef/ifndef block
  #ifndef HAS_XAOD
  #warning "using plain ROOT compilation scheme - please add an ASG Analysis Release in order to use this feature!"
  return std::numeric_limits<double>::quiet_NaN();
  #else
  // in the rest of this function, you should retrieve the data and calculate your return value
  // here is the place where most of your custom code should go
  // a couple of comments should guide you through the process
  // when writing your code, please keep in mind that this code can be executed several times on every event
  // make your code efficient. catch all possible problems. when in doubt, contact experts!

  if (this->getCurrentEntry() == this->fCachedEntry) {
    return true; //nothing to do here, return value already cached
  }

  // clear the old cache before recalculating it for new event
  /* example block
     myvector.clear();
  */

  // first, you can retrieve the data members you need with the 'retrieve' method:
  /* example block:
  // Retrieve CompositeParticle container
  const xAOD::CompositeParticleContainer *cand = 0;
  if(!this->fEvent->retrieve(cand, this->mContName.Data()).isSuccess()){
  DEBUGclass("failed to retrieve candidates!");
  return false;
  }
   */
  DEBUGclass("retrieved candidates! %s",this->mContName.Data());
  DEBUGclass(" cand->size(): %i",cand->size());
  
  // after you have retrieved your data members, you can proceed to calculate the return value
  // probably, you first have to retrieve an element from the container
  /* example block:
  const xAOD::CompositeParticle *Evt = cand->at(0);
  */

  // calculate your return value from container and add to vector
  /* example block:
     double pt = Evt->part(0)->pt();
     myvector.push_back(pt);
   */
  
  // bookmark cached entry
  this->fCachedEntry = this->getCurrentEntry();
  DEBUGclass("Created new set of cached values in instance '%s'",this->GetName());

  return true;
#endif
}

//______________________________________________________________________________________________

const std::vector<double>* ObsName::getVector() const {
  // Function to retrieve the cached vector
  /* example block
     return &myvector;
  */
  
  //this should never be executed, we just make the compiler calm down
  throw std::runtime_error("Illegal / unsupported mode selected!");
  return NULL;
}

//______________________________________________________________________________________________

double ObsName::getValue() const {
  // Value retrieval function, called on every event for every cut and histogram
  // Since this is a vector observable this should never be executed
  DEBUGclass("entering function getValue");
  throw std::runtime_error("Caught attempt to evaluate vector valued observable in scalar context");
  return std::numeric_limits<double>::quiet_NaN();
}

//______________________________________________________________________________________________

double ObsName::getValueAt(int index) const {
  // Function to return element of vector with specified index
  if (!this->makeCache()) {
    ERROR("Failed to obtain return values!");
    return std::numeric_limits<double>::quiet_NaN();
  }

  const std::vector<double>* vec = this->getVector();
	
  if ((int)vec->size() == 0) { // vector is empty, not even filled once
    throw std::runtime_error("Attempted to retrieve value of vector that is empty");
    return std::numeric_limits<double>::quiet_NaN(); 
  }
  if (index >= (int)vec->size()) {
    throw std::runtime_error("Attempted to retrieve value for out of bounds");
    return std::numeric_limits<double>::quiet_NaN();
  }
  return vec->at(index);
}

//______________________________________________________________________________________________

int ObsName::getNevaluations() const {
  // Function to return the size of the vector that is going to be retrieved
  
  if (!this->makeCache()) {
    ERROR("Failed to obtain return values!");
    return -1;
  }
  if (this->fType==TQObservable::ObservableType::scalar) return 1;
  const std::vector<double>* vec = this->getVector();
  if (vec) return vec->size();
  return -1;
}
