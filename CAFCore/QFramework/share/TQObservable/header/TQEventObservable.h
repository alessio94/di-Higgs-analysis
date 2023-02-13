#include "CAFxAODUtils/TQEventObservable.h"

class ObsName : public TQEventObservable {
protected:
  // Put here data members you wish to use.
  // For example you might want to add a variable
  // for the name of the xAOD container to be retrieved that
  // will be initialized in the initialize function.
  /* example block:
     TString mContName = ""; 
  */

public:
  virtual double getValue() const override;
