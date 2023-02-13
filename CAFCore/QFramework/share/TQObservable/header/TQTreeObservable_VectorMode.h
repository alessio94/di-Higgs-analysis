#include "QFramework/TQTreeObservable.h"


class ObsName : public TQTreeObservable {
protected:
  // Put here any data members your class might need and don't forget potential includes.
  // If you would like to create a vector observable
  // you should define the vectors here.
  // The "mutable" keyword ensures that this member 
  // can be changed also by const functions.
  /* example block for TTreeFormula method:
     TTreeFormula *fFormula = 0;
     don't forget to '#include "TTreeFormula.h"'
     mutable std::vector<double> myvector = std::vector<double>();
  */

  mutable Long64_t fCachedEntry;
  bool makeCache() const;
  
public:
  const std::vector<double>* getVector() const;
  virtual double getValue() const override;
  virtual double getValueAt(int index) const override;
  virtual int getNevaluations() const override;
  TQObservable::ObservableType fType = TQObservable::ObservableType::vector;
  inline virtual TQObservable::ObservableType getObservableType() const override {
    return fType;
  }
  virtual TObjArray* getBranchNames() const override;
