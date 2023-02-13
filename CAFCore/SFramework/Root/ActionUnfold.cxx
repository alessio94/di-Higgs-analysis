#include "RooStats/ModelConfig.h"

#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooSimultaneous.h"
#include "RooProdPdf.h"
#include "RooRealSumPdf.h"
#include "RooFitResult.h"

// #define _DEBUG_
#include "QFramework/TQLibrary.h"
#include "QFramework/TQFolder.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQUtils.h"

#include "SFramework/TSStatisticsManager.h"
#include "SFramework/TSUtils.h"

#include "TFile.h"
#include "TDecompSVD.h"

/*<cafdoc name=EditWorkspace>
  Unfold
  ===========================

  Implement Unfolding in a workspace.

  Usage:
  ---------------
  ```
  +Unfold {
    + Unfolded {
      <SystematicsThreshold=0.005>
      <MigrationThreshold=1e-6>
      <mu=false> # decide if you want your POI to be the signal strength (true, default) or the cross-section (false)
      + Measured {
        <workspace="Reco">
      }
      + Migrations {
        <workspace="Truth">
        <region="SR_2D">
      }
      + Truth {
        <workspace="Truth">
        <region="FR_1D">
      }
      + Reconstructed {
        <workspace="Truth">
        <region="SR">
        <obs="obs_x_SR_1D">
      }   
      # option: regularize
      +Regularization {
        <method="curvature"> # Most regularization terms use the curvature of some reference quantity
        <reference="correctionfactor"> 
        # <reference="POI">  # just use the curvature of whatever POI you chose
        # <reference="signalstrength">  # This regularization method corresponds to SVD unfolding
        # <reference="excess">  # This regularization method corresponds to the default settings of TUnfold
        <tau=0.3>
        }
    }
  }
  ```
</cafdoc> */

namespace {
  TMatrixD snapshotMatrix(RooAbsReal* func,RooRealVar* obs1, RooRealVar* obs2) {
    int nbins1 = obs1->getBinning().numBins();
    int nbins2 = obs2->getBinning().numBins();      
    TMatrixD matrix(nbins1,nbins2);
    for(int i=0; i<nbins1; ++i){
      obs1->setVal(obs1->getBinning().binCenter(i));
      for(int j=0; j<nbins2; ++j){
        obs2->setVal(obs2->getBinning().binCenter(j));          
        matrix(i,j) = func->getVal();
      }
    }
    return matrix;
  }
    
  TVectorD snapshotVector(RooAbsReal* func, RooRealVar* obs1) {
    int nbins1 = obs1->getBinning().numBins();
    TVectorD vector(nbins1);
    for(int i=0; i<nbins1; ++i){
      obs1->setVal(obs1->getBinning().binCenter(i));
      vector(i) = func->getVal();
    }
    return vector;
  }

  TMatrixDSym poissonCovariances(const TVectorD& eventcounts) {
    TMatrixDSym matrix(eventcounts.GetNrows());
    matrix *= 0;
    for(int i=0; i<eventcounts.GetNrows(); ++i){
      matrix(i,i) = eventcounts[i];
    }
    return matrix;
  }
  TMatrixDSym eventcountCovariances(const TVectorD& eventcounts, const TMatrixD& covariances) {
    TMatrixDSym matrix(eventcounts.GetNrows());
    matrix *= 0;
    for(int i=0; i<covariances.GetNrows(); ++i){
      for(int j=0; j<covariances.GetNrows(); ++j){
        matrix(i,j) = eventcounts[i] * eventcounts[j] * covariances(i,j);
      }
    }
    return matrix;
  }  
    
  void sanitizeNaN(TMatrixD& m,double eps){
    for(int i=0; i<m.GetNrows(); ++i){
      for(int j=0; j<m.GetNcols(); ++j){
        if(std::isnan(m(i,j))) m(i,j) = eps;
      }
    }
  }
  
  void FillCurvatureMatrix( TMatrixD& tCurv, TMatrixD& tC, int fDdim = 0, Double_t eps = 0.00001 ) {
    Int_t ndim = tCurv.GetNrows();

    // Init
    tCurv *= 0;
    tC    *= 0;

    if (fDdim == 0) for (Int_t i=0; i<ndim; i++) tC(i,i) = 1;
    else if (fDdim == 1) {
      for (Int_t i=0; i<ndim; i++) {
        if (i < ndim-1) tC(i,i+1) = 1.0;
        tC(i,i) = 1.0;
      }
    }
    else if (fDdim == 2) {
      for (Int_t i=0; i<ndim; i++) {
        if (i > 0)      tC(i,i-1) = 1.0;
        if (i < ndim-1) tC(i,i+1) = 1.0;
        tC(i,i) = -2.0;
      }
      tC(0,0) = -1.0;
      tC(ndim-1,ndim-1) = -1.0;
    }
    else if (fDdim == 3) {
      for (Int_t i=1; i<ndim-2; i++) {
        tC(i,i-1) =  1.0;
        tC(i,i)   = -3.0;
        tC(i,i+1) =  3.0;
        tC(i,i+2) = -1.0;
      }
    }
    else if (fDdim==4) {
      for (Int_t i=0; i<ndim; i++) {
        if (i > 0)      tC(i,i-1) = -4.0;
        if (i < ndim-1) tC(i,i+1) = -4.0;
        if (i > 1)      tC(i,i-2) =  1.0;
        if (i < ndim-2) tC(i,i+2) =  1.0;
        tC(i,i) = 6.0;
      }
      tC(0,0) = 2.0;
      tC(ndim-1,ndim-1) = 2.0;
      tC(0,1) = -3.0;
      tC(ndim-2,ndim-1) = -3.0;
      tC(1,0) = -3.0;
      tC(ndim-1,ndim-2) = -3.0;
      tC(1,1) =  6.0;
      tC(ndim-2,ndim-2) =  6.0;
    }
    else if (fDdim == 5) {
      for (Int_t i=2; i < ndim-3; i++) {
        tC(i,i-2) = 1.0;
        tC(i,i-1) = -5.0;
        tC(i,i)   = 10.0;
        tC(i,i+1) = -10.0;
        tC(i,i+2) = 5.0;
        tC(i,i+3) = -1.0;
      }
    }
    else if (fDdim == 6) {
      for (Int_t i = 3; i < ndim - 3; i++) {
        tC(i,i-3) = 1.0;
        tC(i,i-2) = -6.0;
        tC(i,i-1) = 15.0;
        tC(i,i)   = -20.0;
        tC(i,i+1) = 15.0;
        tC(i,i+2) = -6.0;
        tC(i,i+3) = 1.0;
      }
    }

    // Add epsilon to avoid singularities
    for (Int_t i=0; i<ndim; i++) tC(i,i) = tC(i,i) + eps;

    //Get curvature matrix
    for (Int_t i=0; i<ndim; i++) {
      for (Int_t j=0; j<ndim; j++) {
        for (Int_t k=0; k<ndim; k++) {
          tCurv(i,j) = tCurv(i,j) + tC(k,i)*tC(k,j);
        }
      }
    }

  }
   
  double getSVDtau(TMatrixD mA, TMatrixD mB, int kreg) {
    int dim = mA.GetNrows();
      
    // Create vectors and matrices
    TMatrixD mCurv(dim, dim), mC(dim, dim);
    Double_t eps = 1e-12;
      
    ::sanitizeNaN(mB,eps);
      
    // Fill and invert the second derivative matrix
    FillCurvatureMatrix( mCurv, mC );
      
    // Inversion of mC by help of SVD
    TDecompSVD CSVD(mC);
    TMatrixD CUort(CSVD.GetU());
    TMatrixD CVort(CSVD.GetV());
    TVectorD CSV  (CSVD.GetSig());
      
    TMatrixD CSVM(dim, dim);
    for (Int_t i=0; i<dim; i++) CSVM(i,i) = 1/CSV(i);
      
    CUort.Transpose( CUort );
    TMatrixD mCinv((CVort*CSVM)*CUort);
      
    //Rescale using the data covariance matrix
    TDecompSVD BSVD( mB );   
    TMatrixD QT(BSVD.GetU());
    QT.Transpose(QT);
    TVectorD B2SV(BSVD.GetSig());
    TVectorD BSV(B2SV);
      
    for(int i=0; i<dim; i++){
      BSV(i) = TMath::Sqrt(B2SV(i));
    }
      
    TMatrixD mAtmp(dim,dim);
    mAtmp *= 0;
    for(int i=0; i<dim; i++){
      for(int j=0; j<dim; j++){
        for(int m=0; m<dim; m++){
          if(BSV(i)){
            mAtmp(i,j) += QT(i,m)*mA(m,j)/BSV(i);
          }
        }
      }
    }
      
    mA = mAtmp;
      
    // Singular value decomposition and matrix operations
    TDecompSVD ASVD( mA*mCinv );
    TVectorD ASV (ASVD.GetSig());

    // Damping coefficient
    Int_t k = kreg-1; 
    return k < ASV.GetNrows() ? ASV(k) : 0.;
  }
}



namespace TSBaseActions {

  class Unfold : public TSStatisticsManager::Action {
    RooAbsReal* obtainModel(TQFolder* config, RooWorkspace* defaultWS) const {
      TString pdfname;
      if(config->getTagString("region",pdfname)){
        pdfname.Append("_model");
      }
      config->getTagString("function",pdfname);
      
      if(pdfname.Length() == 0){
        throw std::runtime_error("function not set!");
      }
      TString wsname;
      if(config->getTagString("workspace",wsname)){
        defaultWS = dynamic_cast<RooWorkspace*>(workspaces()->getObject(wsname));
        if(!defaultWS){
          throw std::runtime_error(TString::Format("unable to obtain workspace '%s'!",wsname.Data()));
        }
      }
      RooAbsReal* pdf = defaultWS->function(pdfname);
      if(!pdf){
        throw std::runtime_error(TString::Format("unable to obtain function '%s' for '%s'!",pdfname.Data(),config->GetName()));
      }
      if(pdf->InheritsFrom(RooAbsPdf::Class())){
        if(pdf->InheritsFrom(RooRealSumPdf::Class())){
          RooRealSumPdf* sumpdf = (RooRealSumPdf*)(pdf);
          if(sumpdf->funcList().getSize() == 1){
            return (RooAbsReal*)(sumpdf->funcList().at(0));
          } else {
            throw std::runtime_error(TString::Format("found %d signal templates in %s: unfolding to multiple signal samples not supported yet!",sumpdf->funcList().getSize(),sumpdf->GetName()));
          }
        } else {
          throw std::runtime_error(TString::Format("'%s' is of type '%s' (unsupported Pdf type)",pdf->GetName(),pdf->ClassName()).Data());
        }
      } else {
        return pdf;
      }
    }

    RooRealVar* obtainObservable(RooAbsArg* func, const TString& name) const {
      for(auto s:func->serverMIterator()){
        if(TQStringUtils::equal(s->GetName(),name)){
          RooRealVar* v = dynamic_cast<RooRealVar*>(s);
          if(v) return v;
        } else {
          RooRealVar* var = obtainObservable(s,name);
          if(var) return var;
        }
      }
      return NULL;
    }

    bool checkReplacements(RooAbsArg* pdf, const RooArgList& poilist) const {
      int nerror=0;
      for(auto* oldmu:poilist){
        if(pdf->dependsOn(*oldmu)){
          manager->error(TString::Format("unfolding failed, '%s' still depends on '%s'!",pdf->GetName(),oldmu->GetName()));
          nerror++;
        }
      }
      if(nerror>0) return false;
      return true;
    }

    TString buildEdit(RooAbsPdf* pdf, const TString& suffix, const std::map<TString,TString>& replacements) const {
      RooArgSet params(*pdf->getParameters((RooArgSet*)0));
      std::stringstream edit;
      edit << "EDIT::" << pdf->GetName() << suffix << "(" << pdf->GetName();
      int n = 0;
      for(auto s:replacements){
        if(params.find(s.first)){
          edit << "," << s.first << "=" << s.second;
          n++;
        }
      }
      edit << ")";
      //      std::cout << edit.str() << std::endl;
      if(n==0) return "";
      return edit.str().c_str();
    }

    template<class T, class U> std::vector<RooAbsReal*> applyBinaryOperator(RooWorkspace* ws,const char* name,const char* op,const std::vector<U*>& first,const std::vector<T*>& second) const {
      if(first.size() != second.size()){
        throw std::runtime_error("unable to apply binary operator with unequal number of bins!");
      }
      std::vector<RooAbsReal*> results;
      for(size_t i=0; i<first.size(); ++i){
        std::stringstream expr;
        expr << "RooFormulaVar::" << name << "_bin" << i;
        expr << "('" << first[i]->GetName() << op << second[i]->GetName() << "',{" << first[i]->GetName() << "," << second[i]->GetName() << "})";
        RooAbsReal* result = (RooAbsReal*)(ws->factory(expr.str().c_str()));
        results.push_back(result);
      }
      return results;
    }

    RooAbsReal* createRegularizationStrength(const TString& name, int idx, RooWorkspace* ws,TQTaggable* regularize,const TMatrixD& response,const TMatrixD& datacov) const {
      double tauval;
      if(idx < 0){
	if(!regularize->getTagDouble("tau",tauval)){
	  int k;
	  if(regularize->getTagInteger("k",k)){
	    tauval = ::getSVDtau(response,datacov,k);
	  } else {
	    throw std::runtime_error("unable to create regularization term with neither 'tau' nor 'k' defined!");
	  }
	}
      } else {
	auto tauvals = regularize->getTagVDouble("tau");
	if((int)(tauvals.size()) > idx){
	  tauval = tauvals.at(idx);
	} else {
	  throw std::runtime_error("when creating multiple independent regularization strength terms, you need to explicitly give the values of tau for each of them as <tau = {1.,2.,...}>");
	}
      }
      std::stringstream tauex;
      tauex << name << "[" << tauval << "]";
      RooAbsReal* tau = (RooAbsReal*)(ws->factory(tauex.str().c_str()));
      return tau;
    }
    
    template<class T> RooAbsPdf* createTikhonovDifferential(RooWorkspace* ws,std::vector<RooAbsReal*> taus,const std::vector<T*>& bins,const std::vector<int>& boundaries) const {
      bool first = true;
      size_t regionidx = 0;
      std::stringstream formula;
      std::vector<std::string> components;
      bool split = (taus.size() > 1);
      bool skip = false;
      for(size_t i=1; i<bins.size()-1; ++i){
	if(regionidx >= taus.size()){
	  throw std::runtime_error(TString::Format("region index '%d' out of bounds!",(int)regionidx).Data());
	}
	RooAbsReal* tau = taus[regionidx];
	if(first && (regionidx==0 || (skip && split))){
	  formula << "-" << tau->GetName() << "*(";
	}
	skip = false;
        for(auto j:boundaries){ // skip entries that span boundaries
          if((int)i==(int)j || (int)(i+1)==(int)j) skip=true;
        }	
	if(split){
	  if(skip){
	    first = true;
	    ++regionidx;
	    formula << ")";
	    components.push_back(formula.str());
	    formula.str("");
	  }
	}
	if(skip){
	  i++;
	  continue;
	}
        if(!first) formula << "+";
        formula << "pow(";
//        formula << "(" << bins[i-1]->GetName() << " - " << bins[i]->GetName() << ")";
//        formula << "-";
//        formula << "(" << bins[i]->GetName() << " - " << bins[i+1]->GetName() << ")";
        formula << bins[i-1]->GetName() << "-2*" << bins[i]->GetName() << "+" << bins[i+1]->GetName();
        formula << ",2)";
        first = false;
      }
      formula << ")";
      components.push_back(formula.str());
      
      std::stringstream expr;
      expr << "RooGenericPdf::regularization('exp(";
      first = true;
      for(size_t i=0; i<components.size(); ++i){
	expr << components[i];
	first = false;
      }
      expr << ")',{";
      first = true;
      for(size_t i=0; i<components.size(); ++i){
	if(!first) expr << ",";
	expr << taus[i]->GetName();
	first = false;
      }
      for(auto arg:bins){
        expr << "," << arg->GetName();
      }      
      expr << "})";
      manager->info(TString::Format("using regularization '%s'",expr.str().c_str()));
      RooAbsPdf* regularization =  (RooAbsPdf*)(ws->factory(expr.str().c_str()));
      regularization->SetTitle("Tikhonov regularization term");
      return regularization;
    }

    RooWorkspace* unfold(TQFolder* config, RooWorkspace* oldws) const {

      // collect all the configuration information
      DEBUG("retrieving objects");
      RooStats::ModelConfig* mc = dynamic_cast<RooStats::ModelConfig*>(oldws->obj(config->getTagStringDefault("modelConfig","ModelConfig")));
     
      bool use_mu = config->getTagBoolDefault("mu",true);
      TQFolder* regularize = config->getFolder("Regularization");

      TQFolder* migrationConfig = config->getFolder("Migrations");
      if(!migrationConfig){
        throw std::runtime_error(TString::Format("unable to find configuration folder 'Migrations' in '%s'!",config->GetName()));
      }
      TQFolder* recoConfig      = config->getFolder("Reconstructed");      
      if(!recoConfig){
        throw std::runtime_error(TString::Format("unable to find configuration folder 'Reconstructed' in '%s'!",config->GetName()));
      }
      TQFolder* truthConfig     = config->getFolder("Truth");            
      if(!truthConfig){
        throw std::runtime_error(TString::Format("unable to find configuration folder 'Truth' in '%s'!",config->GetName()));
      }
      
      RooAbsReal* migration = obtainModel(migrationConfig,oldws);
      RooAbsReal* reco      = obtainModel(recoConfig,oldws);
      RooAbsReal* truth     = obtainModel(truthConfig,oldws);

      RooRealVar* truthObs_truth = obtainObservable(truth,    truthConfig    ->getTagStringDefault("obs"     ,"obs_x_"+truthConfig    ->getTagStringDefault("region","truth")));
      RooRealVar* recoObs_reco   = obtainObservable(reco,     recoConfig     ->getTagStringDefault("obs"     ,"obs_x_"+recoConfig     ->getTagStringDefault("region","reco" )));      
      RooRealVar* truthObs_mig   = obtainObservable(migration,migrationConfig->getTagStringDefault("truthobs","obs_x_"+migrationConfig->getTagStringDefault("region","mig")));
      RooRealVar* recoObs_mig    = obtainObservable(migration,migrationConfig->getTagStringDefault("recoobs" ,"obs_y_"+migrationConfig->getTagStringDefault("region","mig" )));
      if(!truthObs_truth){
        throw std::runtime_error("unable to find truth observable in fiducial region!");
      }
      if(!truthObs_mig){
        throw std::runtime_error("unable to find truth observable in migration matrix!");        
      }
      if(!recoObs_mig){
        throw std::runtime_error("unable to find reco observable in migration matrix!");        
      }
      if(!recoObs_reco){
        throw std::runtime_error("unable to find reco observable in signal region!");        
      }

      double prune = config->getTagDoubleDefault("SystematicsThreshold",0.);
      RooArgList nplist;
      bool includeSystematics = config->getTagDoubleDefault("IncludeSystematics",true);
      if(includeSystematics) nplist.add(*(mc->GetNuisanceParameters()));
      RooArgList obslist(*(mc->GetObservables()));      
      RooArgList oldpoilist(*(mc->GetParametersOfInterest()));

      DEBUG("transposing Pdfs");
      double simplify = config->getTagDoubleDefault("MigrationThreshold",0.);      
      auto migrationSlices = TSUtils::slice(migration,{recoObs_mig,truthObs_mig},nplist,prune);
      auto recoSlices      = TSUtils::slice(reco,{recoObs_reco},nplist,prune);      
      auto truthSlices     = TSUtils::slice(truth,{truthObs_truth},nplist,prune);
      if(migrationSlices.size() != recoSlices.size() * truthSlices.size()){
        throw std::runtime_error("failed to slice Pdfs!");
      }
      for(auto& slice:migrationSlices) oldws->import(*slice.second,RooFit::RecycleConflictNodes());
      for(auto& slice:recoSlices)      oldws->import(*slice.second,RooFit::RecycleConflictNodes());
      for(auto& slice:truthSlices)     oldws->import(*slice.second,RooFit::RecycleConflictNodes());
      

      DEBUG("creating truth POIs");
      // create the new truth POIs
      TString name = config->getTagStringDefault("name",recoConfig->getTagStringDefault("region","SR"));
      size_t nt = truthObs_mig->getBinning().numBins();
      std::vector<RooRealVar*> newpois;
      RooArgList newpoilist;
      for(size_t j=0; j<nt; ++j){
        RooRealVar* poi = NULL;
        if(use_mu){
          TString poiexpr = TString::Format("mu_%s_bin_%d[1,0,50]",name.Data(),(int)j+1);
          poi = (RooRealVar*)(oldws->factory(poiexpr.Data()));
        } else {
          auto truthSlice = truthSlices[{truthObs_truth->getBinning().binCenter(j)}];
          double xsval = truthSlice->getVal();
          TString poiexpr = TString::Format("xs_%s_bin_%d[%g,0,%g]",name.Data(),(int)j+1,xsval,50*xsval);
          poi = (RooRealVar*)(oldws->factory(poiexpr.Data()));
        }
        newpois.push_back(poi);
        newpoilist.add(*poi);
      }

      // create the helper objects
      std::vector<RooAbsReal*> recomus;
      std::map<TString,TString> replacements;
      size_t nm(recoObs_mig->getBinning().numBins());
      std::vector<RooAbsReal*> truthmus;
      std::vector<RooAbsReal*> truthmus_bare;
      std::vector<RooAbsReal*> truthyieldpredictions;        
      for(size_t j=0; j<nt; ++j){ // loop over truth bins
        RooAbsReal* truthmu = NULL;
        RooAbsReal* truthmu_bare = NULL;        
        double truthCoord = truthObs_mig->getBinning().binCenter(j);
        RooAbsReal* truthSlice = truthSlices[{truthCoord}];
        if(!truthSlice) throw std::runtime_error(TString::Format("unable to find truth slice for coordinate %g!",truthCoord).Data());
        truthyieldpredictions.push_back(truthSlice);
        RooRealVar* poi = newpois[j];
        if(!poi) throw std::runtime_error(TString::Format("unable to obtain old POI %d",(int)j).Data());        
        if(use_mu){
          truthmu = poi;
          truthmu_bare = poi;          
        } else {
          TString expr(TString::Format("RooFormulaVar::mu_%s_bin_%d_unfolded('%s/%s',{%s,%s})",
                                       name.Data(),(int)j+1,
                                       poi->GetName(),
                                       truthSlice->GetName(),
                                       poi->GetName(),
                                       truthSlice->GetName()));
          truthmu = (RooAbsReal*)oldws->factory(expr.Data());
          if(!truthmu) throw std::runtime_error(TString::Format("unable to create truth parameter for bin %d from expression %s",(int)j,expr.Data()).Data());
          TString expr_bare(TString::Format("RooFormulaVar::mu_%s_bin_%d_unfolded_bare('%s/%g',{%s})",
                                            name.Data(),(int)j+1,
                                            poi->GetName(),
                                            truthSlice->getVal(),
                                            poi->GetName()));
          truthmu_bare = (RooAbsReal*)oldws->factory(expr_bare.Data());
          if(!truthmu_bare) throw std::runtime_error(TString::Format("unable to create bare truth parameter for bin %d from expression %s",(int)j,expr_bare.Data()).Data());          
        }
        truthmus.push_back(truthmu);
        truthmus_bare.push_back(truthmu_bare);        
      }
      
      DEBUG("adding unfolded objects");
      if((int)nm != (int)oldpoilist.size()){
        throw std::runtime_error("dimensionality mismatch!");
      }
      for(size_t i=0; i<nm; ++i){ // loop over reco bins
        RooAbsReal* oldmu = (RooAbsReal*)(oldpoilist.at(i));
        DEBUG("attempting to replace %s",oldmu->GetName());
        double recoCoord = recoObs_mig->getBinning().binCenter(i);
        
          // evtyield[j] = (truth[i] * probability[i][j]) + fakes[j]
          // probability[i][j] = migrations[i][j] / truth[i]
          // evtyield[j] = sum(i,poi[i]/truth[i] * migrations[i][j])  + fakes[j]
          // fakes[j] = reco[j] - sum(i,migrations[i][j])
          // mu[j] = evtyield[j] / reco[j]
      
        RooArgList args;
        RooAbsReal* recoSlice = recoSlices[{recoCoord}];
        if(!recoSlice) throw std::runtime_error(TString::Format("unable to find reco slice for coordinate %g!",recoCoord).Data());
        args.add(*recoSlice);
        std::stringstream signalevents;
        std::stringstream fakes;
        fakes << recoSlice->GetName();
        bool first = true;
        for(size_t j=0; j<nt; ++j){ // loop over truth bins          
          double truthCoord = truthObs_mig->getBinning().binCenter(j);
          RooAbsReal* migSlice = migrationSlices[{recoCoord,truthCoord}];
          if(!migSlice) throw std::runtime_error(TString::Format("unable to find migration slice for coordinate %g/%g!",recoCoord,truthCoord).Data());
          RooAbsReal* truthmu = truthmus[j];
          if(!truthmu) throw std::runtime_error(TString::Format("unable to find truth parameter for bin %d!",(int)j).Data());
          DEBUG("  adding contribution of %s",truthmu->GetName());          
          if(migSlice->getVal() > simplify){
            args.add(*truthmu);
            if(!first){
              signalevents << "+";
            }
            signalevents << "(" << truthmu->GetName() << "*" << migSlice->GetName() << ")";
            args.add(*migSlice);
            fakes << "-" << migSlice->GetName();
            first = false;
          }
        }
        TString formula = TString::Format("(%s + %s) / %s",signalevents.str().c_str(),fakes.str().c_str(),recoSlice->GetName());
        RooFormulaVar* recomu = new RooFormulaVar(TString::Format("mu_%s_bin_%d_folded",name.Data(),(int)i+1).Data(),formula.Data(),args);
        if(!checkReplacements(recomu,oldpoilist)) return NULL;
        if(oldws->import(*recomu,RooFit::RecycleConflictNodes())){
          manager->error(TString::Format("unable to import '%s'",recomu->GetName()));
          return NULL;
        }
        recomus.push_back(recomu);
        manager->info(TString::Format("replacing '%s' with '%s'",oldmu->GetName(),recomu->GetName()));
        replacements[oldmu->GetName()] = recomu->GetName();
      }

      // create regularization
      DEBUG("defining regularization");
      RooAbsPdf* penalty = NULL;
      if(regularize){
        TMatrixD response = snapshotMatrix(migration,recoObs_mig,truthObs_mig);
        TVectorD asimovReco = snapshotVector(reco,recoObs_reco);
        TMatrixDSym datacov(recomus.size());
        TString preResult;
        bool includeSystematicsInRegularization = regularize->getTagBoolDefault("includeSystematics",false);
        if(regularize->getTagString("covariances",preResult)){
          TQFolder* result = results()->getFolder(preResult);
          RooFitResult* res = TSUtils::convertFitResults(result,&oldpoilist);
          if(res){
            datacov = eventcountCovariances(asimovReco,res->covarianceMatrix());
          }
          delete res;
        } else {
          datacov = poissonCovariances(asimovReco);
        }
        TString method = regularize->getTagStringDefault("method","curvature");
        if(method == "curvature"){
          TString reference = regularize->getTagStringDefault("reference","correctionfactor");
          std::vector<int> boundaries = regularize->getTagVInteger("boundaries");
          bool separateTaus = regularize->getTagBoolDefault("boundaries.differentStrengths",false);
	  std::vector<RooAbsReal*> taus;
	  if(boundaries.size()==0 || !separateTaus){
	    RooAbsReal* tau = this->createRegularizationStrength("tikhonov_tau",-1,oldws,regularize,response,datacov);
	    taus.push_back(tau);
	    manager->info(TString::Format("using regularization strength %.3f",tau->getVal()));
	  } else {
	    for(size_t i=0; i<=boundaries.size(); ++i){
	      RooAbsReal* tau = this->createRegularizationStrength(TString::Format("tikhonov_tau_region%d",(int)i),(int)i,oldws,regularize,response,datacov);
	      manager->info(TString::Format("using regularization strength %.3f for region %d",tau->getVal(),(int)i));	      
	      taus.push_back(tau);
	    }
	  }
          if(reference == "POI"){
            penalty = createTikhonovDifferential(oldws,taus,newpois,boundaries);            
          } else if(reference == "correctionfactor"){
            auto corrfs =  this->applyBinaryOperator(oldws,"correctionfactor","/",recomus,includeSystematicsInRegularization ? truthmus : truthmus_bare);
            penalty = createTikhonovDifferential(oldws,taus,corrfs,boundaries);
          } else if(reference == "signalstrength"){
            penalty = createTikhonovDifferential(oldws,taus,includeSystematicsInRegularization ? truthmus : truthmus_bare,boundaries);
          } else if(reference == "excess"){
            if(use_mu){
              auto yields =  this->applyBinaryOperator(oldws,"truth_yield","*",newpois,truthyieldpredictions);
              auto excesses =  this->applyBinaryOperator(oldws,"truth_yield_excess","-",yields,truthyieldpredictions);
              penalty = createTikhonovDifferential(oldws,taus,excesses,boundaries);              
            } else {
              auto excesses =  this->applyBinaryOperator(oldws,"truth_yield_excess","-",newpois,truthyieldpredictions);                            
              penalty = createTikhonovDifferential(oldws,taus,excesses,boundaries);
            }
          } else {
            manager->error(TString::Format("unknown reference quantity '%s'",reference.Data()));
          }
        } else {
          manager->error(TString::Format("unknown regularization method '%s'",method.Data()));
          return NULL;
        }
      }
      
      // create the unfolded pdf
      DEBUG("creating unfolded Pdf");      
      RooAbsPdf* oldpdf = mc->GetPdf();
      if(oldpdf->InheritsFrom(RooSimultaneous::Class())){
        RooSimultaneous* sim = (RooSimultaneous*)(oldpdf);
        auto& channelCat = sim->indexCat();
        TIterator* iter = channelCat.typeIterator() ;
        RooCatType* tt = NULL;
        RooArgList newcats;
        while((tt=(RooCatType*) iter->Next())) {
          TString catname(tt->GetName());
          RooAbsPdf* catpdf = sim->getPdf(catname);
          if(!catpdf->InheritsFrom(RooProdPdf::Class())){
            throw std::runtime_error("pdf is not a prodPdf!");
          }
          RooArgSet elements;
          RooAbsPdf* mainpdf = TSUtils::stripConstraints((RooProdPdf*)catpdf,obslist,elements);
          RooAbsPdf* newmainpdf = (RooAbsPdf*)(oldws->factory(buildEdit(mainpdf,"_folded",replacements)));
          RooAbsPdf* newcatpdf = NULL;          
          if(!newmainpdf){
            newcatpdf = catpdf;
          } else {
            TString newcatname = TString::Format("%s_folded",catpdf->GetName());
            if(penalty){
              elements.add(*penalty);
              newcatname.Append("_regularized");
            }
            elements.add(*newmainpdf);
            newcatpdf = new RooProdPdf(newcatname.Data(),newcatname.Data(),elements);
            if(!checkReplacements(newcatpdf,oldpoilist)){
              return NULL;
            }
          }
          newcats.add(*newcatpdf);
        }
        TString newsimname = TString::Format("%s_folded",sim->GetName());
        RooSimultaneous* newsim = new RooSimultaneous(newsimname.Data(),newsimname.Data(),newcats,*const_cast<RooAbsCategoryLValue*>(&channelCat));
        bool error = oldws->import(*newsim,RooFit::RecycleConflictNodes(true));
        if(error) throw std::runtime_error(TString::Format("error importing pdf '%s'",newsim->GetName()).Data());
        RooAbsPdf* pdf = oldws->pdf(newsim->GetName());
        if(!pdf) throw std::runtime_error(TString::Format("unable to find new simPdf name '%s'",newsim->GetName()).Data());
        if(!checkReplacements(pdf,oldpoilist)) return NULL;        
        mc->SetPdf(*pdf);
      } else {
        RooAbsPdf* pdf = (RooAbsPdf*)oldws->factory(buildEdit(oldpdf,"_folded",replacements));
        if(!checkReplacements(pdf,oldpoilist)) return NULL;        
        // apply penalty term
        if(penalty){
          DEBUG("applying regularization");      
          manager->info(TString::Format("adding regularization penalty term '%s'",penalty->GetTitle()));
          TString fullpdfname = TString::Format("%s_regularized",pdf->GetName());
          RooProdPdf* fullpdf = new RooProdPdf(fullpdfname.Data(),fullpdfname.Data(),*pdf,*penalty);
          oldws->import(*fullpdf,RooFit::RecycleConflictNodes());
          RooAbsPdf* thepdf = (RooAbsPdf*)(oldws->pdf(fullpdfname.Data()));
          if(!thepdf){
            manager->error("unable to import product pdf!");
            return 0;
          }
          mc->SetPdf(*thepdf);
        } else {
          mc->SetPdf(*oldws->pdf(pdf->GetName()));
        }       
      }
      
      // do cleanup
      DEBUG("performing cleanup");      
      mc->SetParametersOfInterest(newpoilist);
      for(auto slice:migrationSlices){
        delete slice.second;
      }
      for(auto slice:truthSlices){
        delete slice.second;
      }
      for(auto slice:recoSlices){
        delete slice.second;
      }            
      return TSUtils::makeCleanWorkspace(oldws,config->GetName(),mc->GetName(),true,false);
    }
    
    bool execute(TQFolder * config) const override {
      TQFolder* source = config->getFolder("Measured");
      if(!source){
        manager->error(TString::Format("unable to find configuration folder 'Measured' in '%s'!",config->GetName()));        
        return false;
      }
      TString wsname;
      if(!source->getTagString("workspace",wsname)){
        manager->error(TString::Format("unable to unfold '%s': no 'workspace' given in 'Measured' configuration!",config->GetName()));
        return false;
      }
      RooWorkspace * workspace = dynamic_cast<RooWorkspace*>(workspaces()->getObject(wsname));
      if(!workspace){
        manager->error(TString::Format("unable to unfold '%s': no such workspace!",config->GetName()));
        return false;
      }
      RooWorkspace copyws(*workspace);
      RooWorkspace* unfolded = unfold(config,&copyws);
      if(unfolded){
        if(unfolded != workspace){
          if(TQStringUtils::equal(workspace->GetName(),unfolded->GetName())){
            workspaces()->Remove(workspace);
            delete workspace;
          }
          workspaces()->addObject(unfolded);
        }
      } else {
        manager->error(TString::Format("error unfolding '%s'!",config->GetName()));
        return false;
      }
      return true;
    }
  };
  namespace {
    bool available = TSStatisticsManager::registerAction(new Unfold(),"Unfold");
  }
}
