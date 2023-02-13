#include "CAFxAODUtils/TQEventObservable.h"

class ObsName : public TQEventObservable {
protected:
  // Put here data members you wish to use. 
  // If you would like to create a vector observable
  // you should define the vectors here.
  // The "mutable" keyword ensures that this member 
  // can be changed also by const functions.
  // You also might want to add a variable
  // for the name of the xAOD container to be retrieved that
  // will be initialized in the initialize function.
  /* example block
     mutable std::vector<double> myvector = std::vector<double>();
     TString mContName = "";
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
