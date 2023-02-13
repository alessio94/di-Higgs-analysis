#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSContourScanner.h"
#include "SFramework/TSUtils.h"

#include <RooMinimizer.h>
#include <Math/Minimizer.h>

//__________________________________________________________________________________|___________

TSContourScanner::TSContourScanner(RooWorkspace * ws, TQFolder* snapshots) : TSStatisticsCalculator("TSContourScanner",ws,snapshots) {
  // default constructor  
}


//__________________________________________________________________________________|___________

TSContourScanner::~TSContourScanner() {
  // default destructor
}

//__________________________________________________________________________________|___________

void TSContourScanner::info(TString message) {
  std::cout << "SFramework/TSContourScanner: " << message.Data() << std::endl;
}

//__________________________________________________________________________________|___________

TQFolder * TSContourScanner::runCalculation(TQFolder * options){

  // expect valid inputs
  if (!fWorkspace || !fModelConfig || !options){
    return NULL;
  }

  // dataset name
  const TString datasetName = options->getTagStringDefault("datasetName", "obsData");

  TString logfile;
  // bool redirect = options->getTagString("fit.logToFile",logfile); // BW: hashed (unused variable)
  TQTaggable fitOptions;
  fitOptions.importTagsWithoutPrefix(*options,"fit.");
  fitOptions.setTagBool("adjustPOI",false);
  fitOptions.setTagString("datasetName",datasetName);

  // iterate of parameters to scan
  TQFolderIterator itr(options->getListOfFolders("?"));
  std::vector<TQFolder*> params;
  std::vector<TString> parnames;
  while(itr.hasNext()){
    TQFolder* par = itr.readNext();
    if(!par) continue;
    TString parname= par->getTagStringDefault("name",par->GetName());	
    params.push_back(par);
    parnames.push_back(parname);
    RooRealVar* v = fWorkspace->var(parname);
    if(!v){
      error(TString::Format("unable to retrieve parameter '%s' from workspace!",parname.Data()));
      return NULL;
    } else {
      v->setConstant(false);
    }
  }

  // load nominal snapshot to start from
  TString snapshotInitial = options->getTagStringDefault("snapshot.initial",options->getTagStringDefault("snapshot",TString::Format("SnSh_AllVars_Unconditional_%s", datasetName.Data())));
  fitOptions.setTagString("snapshot",snapshotInitial);
  if(!this->loadSnapshot(&fitOptions)){
    error(TString::Format("unable to load snapshot '%s'",snapshotInitial.Data()));
    return NULL;
  } else {
    info(TString::Format("using snapshot '%s'",snapshotInitial.Data()));
  }

  RooArgSet allVars(fWorkspace->allVars());
  const size_t npar(parnames.size());
  
  info("refitting unconditional minimum");
  this->setParametersConstFloat(&allVars,&fitOptions);
  
  for(size_t ipar=0; ipar<npar; ++ipar){
    auto p = parnames[ipar];
    fWorkspace->var(p)->setConstant(false);
  }
  
  this->setup(datasetName,&fitOptions);

  TString var1name,var2name;
  if(!options->getTagString("x",var1name)){
    throw std::runtime_error("unable to retrieve variable 'x'");
  }
  if(!options->getTagString("y",var2name)){
    throw std::runtime_error("unable to retrieve variable 'y'");
  }
  
  RooRealVar* var1 = fWorkspace->var(var1name);
  RooRealVar* var2 = fWorkspace->var(var2name);
  
  int npoints = options->getTagIntegerDefault("n",50);
  TQIterator itrthrsh(options->getListOfKeys("thresholds.*"),true);
  std::map<TString,double> thresholds;
  while(itrthrsh.hasNext()){
    TString key(itrthrsh.readNext()->GetName());
    double val;
    if(options->getTagDouble(key,val)){
      TQStringUtils::removeLeadingText(key,"thresholds.");
      thresholds[key] = val;
    }
  }
  if(thresholds.size() == 0){
    thresholds["1sigma"]= 2.296; // 1sigma
    thresholds["2sigma"]= 6.180; // 2sigma
    thresholds["3sigma"]=11.829; // 3sigma
    thresholds["4sigma"]=19.334; // 4sigma
    thresholds["5sigma"]=28.744; // 5sigma
  }
    
  TQFolder* result = this->contour(&fitOptions, var1,var2,npoints,thresholds);
  if(result){
    result->setTagString("content","contour");
    result->setTagInteger("nDim",2);  
    result->setTagString("x",var1name);
    result->setTagString("y",var2name);  
  }
  return result;
}

//__________________________________________________________________________________|___________

int TSContourScanner::getVarIndex(RooArgList* list, RooRealVar* v){
  // Verify that both variables are floating parameters of PDF
  int index= list->index(v);
  if(index < 0) {
    throw std::runtime_error(TString::Format("TSContourScanner::contour: %s is not a parameter of the Nll!",v->GetName()).Data());
  }
  return index;
}

//__________________________________________________________________________________|___________

TQFolder * TSContourScanner::contour(TQTaggable* options, RooRealVar* var1, RooRealVar* var2, unsigned int npoints, const std::map<TString,double>& thresholds){
  // slightly modified copy of RooMinimizer::contour
  RooMinimizer minimizer(*(this->fNll));

  TString minType = options->getTagStringDefault("minimizerType", ROOT::Math::MinimizerOptions::DefaultMinimizerType());
  TString minAlgo = options->getTagStringDefault("minimizerAlgo", ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo());
  minimizer.minimize(minType,minAlgo);

  double min1 = var1->getVal();
  double min2 = var2->getVal();  
  
  this->resetOffset();

  double val = this->fNll->getVal();
  if(!TQUtils::isNum(val)){
    error("Nll value is non-numeric!");
    return NULL;
  }
  
  minimizer.minos(RooArgSet(*var1,*var2));

  info("determined minimum and errors:");
  info(TString::Format("  %s = %g +%g -%g",var1->GetName(),var1->getVal(),fabs(var1->getErrorHi()),fabs(var1->getErrorLo())));
  info(TString::Format("  %s = %g +%g -%g",var2->GetName(),var2->getVal(),fabs(var2->getErrorHi()),fabs(var2->getErrorLo())));

  /// BW: hashed the following 5 lines (unused variables)
  //double epsilon = 1e-9;
  // double var1up = (fabs(var1->getErrorHi()) < epsilon) ? var1->getMax()-var1->getVal() : var1->getErrorHi();
  // double var1dn = (fabs(var1->getErrorLo()) < epsilon) ? var2->getVal()-var2->getMin() : var1->getErrorLo();
  // double var2up = (fabs(var2->getErrorHi()) < epsilon) ? var1->getMax()-var1->getVal() : var2->getErrorHi();
  // double var2dn = (fabs(var2->getErrorLo()) < epsilon) ? var2->getVal()-var2->getMin() : var2->getErrorLo();
  
  if(!minimizer.fitter()) throw std::runtime_error("internal RooFit error: cannot access fitter!");
  ROOT::Math::Minimizer* mini = minimizer.fitter()->GetMinimizer();
  if(!mini) throw std::runtime_error("internal RooFit error: cannot access minimizer!");

  var1->setConstant(true);
  var2->setConstant(true);
  RooArgList* params = TSUtils::getFunction(minimizer)->GetFloatParamList() ;
  RooArgList* paramSave = (RooArgList*) params->snapshot() ;
  int index1 = getVarIndex(TSUtils::getFunction(minimizer)->GetFloatParamList(),var1);
  int index2 = getVarIndex(TSUtils::getFunction(minimizer)->GetFloatParamList(),var2);
  
  TQFolder* result = new TQFolder("Contours");
  result->setTagDouble("xmin",min1);
  result->setTagDouble("ymin",min2);  
  
  // remember our original value of ERRDEF  
  Double_t errdef= mini->ErrorDef();

  for(auto it:thresholds){
    *params = *paramSave;
    info("minimum and errors after correction");  
    info(TString::Format("  %s = %g +%g -%g",var1->GetName(),var1->getVal(),fabs(var1->getErrorHi()),fabs(var1->getErrorLo())));
    info(TString::Format("  %s = %g +%g -%g",var2->GetName(),var2->getVal(),fabs(var2->getErrorHi()),fabs(var2->getErrorLo())));

    TString key = it.first;
    double val = it.second;
    info(TString::Format("calculating contour for %s at %g with %d points",key.Data(),val,npoints));
    
    if(val < 0){
      throw std::runtime_error("cannot calculate contour for negative Nll value!");
    }
    // set the value corresponding to the threshold contour
    mini->SetErrorDef(val*errdef);
   
    // calculate and draw the contour
    Double_t *xcoor = new Double_t[npoints+1];
    Double_t *ycoor = new Double_t[npoints+1];

    var1->Print();
    var2->Print();
    bool ret = mini->Contour(index1,index2,npoints,xcoor,ycoor);
   
    if (!ret) {
      warn(TString::Format("TSContourScanner::contour::Minuit did not return a contour graph %s for %f",key.Data(),val).Data());
    } else {
      xcoor[npoints] = xcoor[0];
      ycoor[npoints] = ycoor[0];
      
      TQFolder* cnt = result->getFolder(TString::Format("contour_%s+",key.Data())) ;
      cnt->setTagInteger("n",npoints);  
      cnt->setTagString("label",key);
      cnt->setTagDouble("threshold",val);      
      
      for(size_t i=0; i<npoints; ++i){
        TQFolder* p = cnt->getFolder(TString::Format("p.%d+",(int)(i)));
        p->setTagDouble("x",xcoor[i]);
        p->setTagDouble("y",ycoor[i]);
      }
    }
    
    delete [] xcoor;
    delete [] ycoor;
  }
  
  
  // restore the original ERRDEF
  mini->SetErrorDef(errdef);
  
  // restore parameter values
  *params = *paramSave ;
  delete paramSave ;
  
  return result ;
}
