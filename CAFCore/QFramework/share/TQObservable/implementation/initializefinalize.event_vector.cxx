//______________________________________________________________________________________________

bool ObsName::initializeSelf(){
  // initialize this observable
  // called once per sample (input file) so that the observable knows the name of the event candidate
  // will be EventEM or EventME in the case of DF analysis (depending on the current channel)
  /* example block
     TString ContName = "";
     if(!this->fSample->getTagString("~cand",ContName)) return false;
     this->mContName = "Event"+ContName;
   */

  DEBUGclass("initializing");
  
  fCachedEntry = -1;
  return true;
}

//______________________________________________________________________________________________

bool ObsName::finalizeSelf(){
  // finalize this observable
  // remember to undo anything you did in initializeSelf() !
  /* example block
     myvector.clear();
  */
  
  DEBUGclass("finalizing");
  return true;
}
