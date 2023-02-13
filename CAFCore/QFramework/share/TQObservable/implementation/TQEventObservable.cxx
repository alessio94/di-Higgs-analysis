#define XAOD_STANDALONE 1
// put here any EDM includes you might need, e.g.
/* example block:
//#include "xAODParticleEvent/CompositeParticleContainer.h"
*/

//______________________________________________________________________________________________

double ObsName::getValue() const {
  // value retrieval function, called on every event for every cut and histogram
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
  
  // first, you can retrieve the data members you need with the 'retrieve' method
  // as arguments to this function, pass the member pointer to the container as well as the container name
    /* example block:
  // Retrieve CompositeParticle container
  const xAOD::CompositeParticleContainer *cand = 0;
  if(!this->fEvent->retrieve(cand, this->mContName.Data()).isSuccess()){
  DEBUGclass("failed to retrieve candidates!");
  return false;
  }
   */

  // after you have retrieved your data members, you can proceed to calculate the return value
  // probably, you first have to retrieve an element from the container
  /* example block:
  const xAOD::CompositeParticle *Evt = cand->at(0);
  */

  // in the end, you should calculate your return value 
  // of course, you can use other data members of your observable at any time
  /* example block:
     const double retval = Evt->part(0)->pt();
  */

  DEBUGclass("returning retval: %f",retval);
  return retval;
  #endif
}
