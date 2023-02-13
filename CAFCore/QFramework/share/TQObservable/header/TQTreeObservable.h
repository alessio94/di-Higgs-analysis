#include "QFramework/TQTreeObservable.h"

class ObsName : public TQTreeObservable {
protected:
  // Put here any data members your class might need and don't forget potential includes.
  /* example block for TTreeFormula method:
     TTreeFormula *fFormula = 0;
     don't forget to '#include "TTreeFormula.h"'
  */

 
public:
  virtual double getValue() const override;
  virtual TObjArray* getBranchNames() const override;
