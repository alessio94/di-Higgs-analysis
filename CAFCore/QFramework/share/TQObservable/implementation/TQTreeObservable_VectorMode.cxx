
//______________________________________________________________________________________________

TObjArray* ObsName::getBranchNames() const {
  // retrieve the list of branch names 
  // ownership of the list belongs to the caller of the function
  DEBUGclass("retrieving branch names");
  TObjArray* bnames = new TObjArray();
  bnames->SetOwner(false);

  // add the branch names needed by your observable here, e.g.
  // bnames->Add(new TObjString("someBranch"));
  
  return bnames;
}

//______________________________________________________________________________________________

bool ObsName::makeCache() const {
  // function that chaches vector that is later retrieved with function getVector()
  // called on every event for every cut and histogram

  // in the rest of this function, you should retrieve the data and calculate your return value
  // here is the place where most of your custom code should go
  // a couple of comments should guide you through the process
  // when writing your code, please keep in mind that this code can be executed several times on every event
  // make your code efficient. catch all possible problems. when in doubt, contact experts!
  
  // here, you should calculate your return value
  // of course, you can use other data members of your observable at any time
  
  /* example block for TTreeFormula method:
     double value1 = this->fFormula->EvalInstance();
     myvector.push_back(value1);
  */

  /* example block for TTree::SetBranchAddress method:
     double value1 = this->fBranch1 + this->fBranch2;
     myvector.push_back(value1);
   */

  // in the end, bookmark cached entry
  this->fCachedEntry = this->getCurrentEntry();
  DEBUGclass("Created new set of cached values in instance '%s'",this->GetName());

  return true;
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
