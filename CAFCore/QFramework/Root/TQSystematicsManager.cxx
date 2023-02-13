#include "QFramework/TQSystematicsManager.h"
#include "QFramework/TQSystematics.h"
#include "QFramework/TQIterator.h"
#include "QFramework/TQHistogramUtils.h"

#include "TF1.h"
#include "TFitResult.h"
#include "TMath.h"

#define UNUSED(x) (void)(x)

#ifdef HAS_CommonSmoothingTool
#pragma message "enabling CommonSmoothingTools integration"
#include "SmoothSystematics/SmoothHist.h"
#else
#pragma message "disabling CommonSmoothingTools integration"
#endif

// #define _DEBUG_
#include "QFramework/TQLibrary.h"

//__________________________________________________________________________________|___________

bool TQSystematicsManager::isBlacklistedSys(TQFolder* sys, const std::vector<TString>&  systBlacklist, const std::vector<TString>& types){
  // return true if a systematic matches the blacklist filter
  bool isBlacklisted = false;
  for (auto& s: systBlacklist) {
    for(auto& type:types){
      if( TQStringUtils::matches(sys->getName(),type+"."+s)){
        isBlacklisted = true;
      }
    }
  }
  return isBlacklisted;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::includeOverallSys(TQFolder* sysConfig, TQFolder* sys, const TString& nameChannel, const TString& nameSample){
  // include a normalization systematic
  DEBUGclass("investigating systematic %s for channel %s in sample %s",sys->GetName(),nameChannel.Data(),nameSample.Data());    
  
  // get systematic + up/down variation names
  TString varUp = sysConfig->getTagStringDefault("Up","");
  TQFolder* computeFolder_Up = sysConfig->getFolder("Compute/Up");
  // get the variation histograms, compute if needed
  TH1* h_Up = varUp.IsNull() ? NULL : this->getVarHisto(varUp, nameChannel, nameSample,!computeFolder_Up);
  if(!h_Up && computeFolder_Up) {
    DEBUGclass("computing up variation from Compute folder");
    h_Up   = this->computeHistogramVariation(computeFolder_Up, nameChannel, nameSample);
    if (h_Up) storeVarHisto(h_Up, varUp, nameChannel);
  }
  // get systematic + up/down variation names
  TString varDown = sysConfig->getTagStringDefault("Down","");
  TQFolder* computeFolder_Down = sysConfig->getFolder("Compute/Down");
  // get the variation histograms, compute if needed
  TH1* h_Down = varDown.IsNull() ? NULL : this->getVarHisto(varDown, nameChannel, nameSample,!computeFolder_Down);
  if (!h_Down && computeFolder_Down) {
    DEBUGclass("computing down variation from Compute folder");    
    h_Down = this->computeHistogramVariation(computeFolder_Down, nameChannel, nameSample);
    if (h_Down) storeVarHisto(h_Down, varDown, nameChannel);
  } 

  // get nominal histogram integral
  const TH1* h_Nominal = this->getVarHisto("Nominal", nameChannel, nameSample);
  Double_t norm_Nominal = TQHistogramUtils::getIntegral(h_Nominal);
  
  // compute ratios
  int      nEntries       = sys->getTagIntegerDefault("Entries",0);
  Double_t ratio_Up	  = 1.;
  Double_t ratio_Down	  = 1.;
  Double_t ratio_Up_Err	  = 0.;
  Double_t ratio_Down_Err = 0.;

  if (h_Up || h_Down){
    if (norm_Nominal == 0.) {
      warn(TString::Format("includeSystematics('%s'): Normalization of Nominal histogram "
                           "of sample '%s' in channel '%s' is zero. Skipping ...",
                           sysConfig->GetName(), nameSample.Data(), nameChannel.Data()));
      return false;
    }
    if(h_Up){
      double norm_Up = TQHistogramUtils::getIntegralAndError(h_Up,ratio_Up_Err);
      ratio_Up = norm_Up / norm_Nominal;
      DEBUGclass("derived systematic Up effect %.4f from proper histogram",ratio_Up);
      ratio_Up_Err /= norm_Nominal;
      nEntries = std::max((int)h_Up->GetEntries(),nEntries);
    }
    if(h_Down){
      double norm_Down = TQHistogramUtils::getIntegralAndError(h_Down,ratio_Down_Err);
      ratio_Down = norm_Down / norm_Nominal;
      DEBUGclass("derived systematic Down effect %.4f from proper histogram",ratio_Down);
      ratio_Down_Err /= norm_Nominal;
      nEntries = std::max((int)h_Down->GetEntries(),nEntries);      
    }
    if(!h_Up){ // Then ratio_Down is available
      ratio_Up=2.-ratio_Down;
      DEBUGclass("derived systematic Up effect %.4f from symmetrization",ratio_Up);      
      ratio_Up_Err=ratio_Down_Err;
    }
    if(!h_Down){ // Then ratio_Up is available
      ratio_Down=2.-ratio_Up;
      DEBUGclass("derived systematic Down effect %.4f from symmetrization",ratio_Down);      
      ratio_Down_Err=ratio_Up_Err;
    }
  } else if(!sys->hasTagDouble("High") || !sys->hasTagDouble("Low")) {
    error(TString::Format("includeOverallSys(...): Systematic '%s' does not have variations assigned for sample '%s' in channel '%s'!",sysConfig->GetName(),nameSample.Data(),nameChannel.Data()));
    return false;
  }

  // magnify variation
  Double_t magnDefault	= sysConfig->getTagDoubleDefault("Magnification", 1.);
  Double_t magnOverallSys	= sysConfig->getTagDoubleDefault("OverallSysMagnification", magnDefault);
  if (magnOverallSys != 1.) {
    ratio_Up = TMath::Power(ratio_Up, magnOverallSys);
    ratio_Down = TMath::Power(ratio_Down, magnOverallSys);
  }

  // define OverallSys
  double low  = ratio_Down * sys->getTagDoubleDefault("Low", 1.);
  double high = ratio_Up   * sys->getTagDoubleDefault("High", 1.);
  double variation = std::max(fabs(low-1.),fabs(high-1.));  
  sys->setTagInteger("Status", TQSystematics::ACTIVE);
  sys->setTagInteger("Entries", nEntries);
  sys->setTagDouble("Low", low);
  sys->setTagDouble("High", high);
  sys->setTagDouble("LowErr", ratio_Down_Err);
  sys->setTagDouble("HighErr", ratio_Up_Err);
  sys->setTagDouble("Percent", variation*100);
                    
  // succesfully included OverallSys
  return true;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::includeHistoSys(TQFolder* sysConfig, TQFolder* sys, const TString& nameChannel, const TString& nameSample){
  // include a histogram systematic

  // get systematic + up/down variation names
  TString varUp = sysConfig->getTagStringDefault("Up","");
  TQFolder* computeFolder_Up = sysConfig->getFolder("Compute/Up");
  // get the variation histograms, compute if needed
  TH1* h_Up = varUp.IsNull() ? NULL : this->getVarHisto(varUp, nameChannel, nameSample,!computeFolder_Up);
  if(!h_Up && computeFolder_Up) {
    h_Up   = this->computeHistogramVariation(computeFolder_Up, nameChannel, nameSample);
    if (h_Up) storeVarHisto(h_Up, varUp, nameChannel);
  }
  // get systematic + up/down variation names
  TString varDown = sysConfig->getTagStringDefault("Down","");
  TQFolder* computeFolder_Down = sysConfig->getFolder("Compute/Down");
  // get the variation histograms, compute if needed
  TH1* h_Down = varDown.IsNull() ? NULL : this->getVarHisto(varDown, nameChannel, nameSample,!computeFolder_Down);
  if (!h_Down && computeFolder_Down) {
    h_Down = this->computeHistogramVariation(computeFolder_Down, nameChannel, nameSample);
    if (h_Down) storeVarHisto(h_Down, varDown, nameChannel);
  }

  if(!h_Up && !h_Down){
    ERRORclass("unable to include HistoSys '%s': no variation found!",sysConfig->GetName());
    return false;
  }
  
  // magnify variations
  Double_t magnDefault	= sysConfig->getTagDoubleDefault("Magnification", 1.);
  Double_t magnHistoSys	= sysConfig->getTagDoubleDefault("HistoSysMagnification", magnDefault);

  // get nominal histogram
  const TH1* h_Nominal = this->getVarHisto("Nominal", nameChannel, nameSample);
  // normalize and/or symmetrize into shape variations
  
  // store systematic variations
  TString sysName(sysConfig->GetName());
  
  TH1* shape_Up = normalizeShapeVariation(h_Up, h_Nominal, h_Down);
  if(!shape_Up){
    ERRORclass("failed to obtain normalized variation for 'Up'");
    return false;
  }  
  if (magnHistoSys != 1.) {
    magnifyShapeVariation(shape_Up, h_Nominal, magnHistoSys);
  }
  TH1* shape_RelUp = TQHistogramUtils::copyHistogram(shape_Up);
  shape_RelUp->Divide(h_Nominal);
  TString histoPathHigh = storeSysHisto(shape_Up,   sysName, "Up",   nameChannel);
  TString histoPathRelHigh = storeSysHisto(shape_RelUp, sysName, "RelUp", nameChannel);  
  sys->setTagString("HistoHigh", histoPathHigh);
  sys->setTagString("HistoRelHigh", histoPathRelHigh);

  TH1* shape_Down = normalizeShapeVariation(h_Down, h_Nominal, h_Up);
  if(!shape_Down){
    ERRORclass("failed to obtain normalized variation for 'Down'");
    return false;
  }
  if (magnHistoSys != 1.) {
    magnifyShapeVariation(shape_Down, h_Nominal, magnHistoSys);
  }
  TH1* shape_RelDown = TQHistogramUtils::copyHistogram(shape_Down);
  shape_RelDown->Divide(h_Nominal);
  TString histoPathLow = storeSysHisto(shape_Down,   sysName, "Down",   nameChannel);
  TString histoPathRelLow = storeSysHisto(shape_RelDown, sysName, "RelDown", nameChannel);      
  sys->setTagString("HistoLow", histoPathLow);
  sys->setTagString("HistoRelLow", histoPathRelLow);    
    
  if (magnHistoSys != 1.) {
    sys->setTagDouble("Magnification",magnHistoSys);
  }

  // successfully included HistoSys
  return true;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::processHistoSys_smoothVariation(TQTaggable* parameters, TQFolder* sys, TH1* nom, TH1* up, TH1* down){
  // process a systematic by smoothing it
  // return true if a smoothing has been performed, false otherwise

  //@tag: [histoSys.smoothingMethod] choose the smoothing method to be used. "default" (default) uses TH1::Smooth, any other will be forwarded to CommonSmoothingTool SmoothHist::Smooth (https://gitlab.cern.ch/atlas-phys/exot/CommonSystSmoothingTool) if available - check there for possible values of smoothing methods
  TString alg = parameters->getTagStringDefault("histoSys.smoothingMethod","default");
  
  //@tag: [histoSys.smoothDirectionAndSymmetrize] Can be set to 'Up' or 'Down' to configure the smoothing so it is applied only to this variation followed by a symmetrization of the smoothed variation. This is useful for 2-point systematics.
  TString smoothDirection = parameters->getTagStringDefault("histoSys.smoothDirectionAndSymmetrize", "");
  
  bool smoothedUp = false;
  bool smoothedDown = false;
  
  if (smoothDirection.IsNull()) {
    smoothedUp = smoothHist(alg, nom, up);
    smoothedDown = smoothHist(alg, nom, down);
  } else {
    if (smoothDirection.EqualTo("Up"))  {
      smoothedUp = smoothHist(alg, nom, up);
      TH1* inverted = TQHistogramUtils::invertShiftWithoutError(up, nom);
      for (int i = 0; i < inverted->GetNbinsX()+1; i++) {
        down->SetBinContent(i, inverted->GetBinContent(i));
        down->SetBinError(i, inverted->GetBinError(i));
      }
      smoothedDown = true;
    } else if (smoothDirection.EqualTo("Down")) {
      smoothedDown = smoothHist(alg, nom, down);
      TH1* inverted = TQHistogramUtils::invertShiftWithoutError(down, nom);
      for (int i = 0; i < inverted->GetNbinsX()+1; i++) {
        up->SetBinContent(i, inverted->GetBinContent(i));
        up->SetBinError(i, inverted->GetBinError(i));
      }
      smoothedUp = true;
    } else {
      warn(TString::Format("The smoothing direction '%s' you specified with the tag 'smoothDirectionAndSymmetrize' is not supported. Please choose either 'Up' or 'Down'. Skipping smoothing in this run!", smoothDirection.Data()));
    }
  }

  bool smoothed = smoothedUp && smoothedDown;
  if (!smoothed)  warn(TString::Format("You specified smoothing for systematic with name '%s' but something went wrong in the smoothing tool", sys->GetName()));
  if ((smoothedUp && !smoothedDown) || (!smoothedUp && smoothedDown)) warn("A strange error occured where the up/down variation was successfully smoothed but the down/up variation was not. Please check your input uncertainties");
  return smoothed;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::smoothHist(const TString& alg, TH1* nom, TH1* var){
  bool smoothed = false;
  if(alg=="default"){
    UNUSED(nom);
    UNUSED(var);
    // var->Smooth();
    ERRORclass("You configured smoothing with the default ROOT algorithm. This is not supported");
  } else {
#ifdef HAS_CommonSmoothingTool
    SmoothHist smoother;
    // Note, SmoothHist::Smooth manipulates histogram inplace.
    TH1* newHist = smoother.Smooth(nom,var,alg.Data(), SmoothedHistErrors::OriginalErrors);
    if(newHist != var){
      // combat temporary inconsistency of smoothing tool and set variation to new hist
      // for Kernel method where var is not manipulated inplace.
      if (alg.Contains("Kernel")) {
        var = newHist;
        smoothed = true;
      } else {
        ERRORclass("Smoother created new histogram, but pointer feedback is not implemented!");
      }
    } else {
      smoothed = true;
    }
#else
    ERRORclass("You configured smoothing algorithm '%s', but the CommonSmoothingTool is not available!", alg.Data());
    smoothed = false;
#endif
  }
  return smoothed;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::processHistoSys_systsToSymmetrizeMaxBins(TQTaggable* parameters, TQFolder* sys, TH1* nom, TH1* up, TH1* down, bool force){
  // Fix shape by symmetrizing maximum relative variation in each bin
  // alternative to systsToSymmetrizeMaxBins
  // return true if a symmetrization has been performed, false otherwise  
  
  //@tag: [histoSys.systsToSymmetrizeMaxBins,histoSys.systsToForceSymmetrizeMaxBins] List of systematics to symmetrize when a one-sided bin is present. The new shape will be determined by symmetrizing the larger variation in each bin. This could lead to amplification of statistically fluctuating templates so use with caution. The tag 'histoSys.systsToForceSymmetrizeMaxBins' can be used to force certain systematics to be symmetrized even if they don't exhibit one-sided bins (might be useful in case of largely asymmetric variations). 

  bool symmetrizedBins = false;
  if (up && down) {
    int nBinGlobal = TQHistogramUtils::getNbinsGlobal(nom);
            
    bool forceSymmetrization = force;
    bool continueSymmetrizing = true;
    int loopCounter = 0;
    while (continueSymmetrizing) {
      bool oneSidedBinPresent = false;
      for(int i=0; i<nBinGlobal; ++i){
        double binUp = up->GetBinContent(i);
        double binDo = down->GetBinContent(i);
        double binNom = nom->GetBinContent(i);
        // if one-sided 
        if ( ((binUp > binNom) && (binDo > binNom)) || ( (binUp < binNom) && (binDo < binNom) ) ) {
          symmetrizedBins = true;
          oneSidedBinPresent = true;
        }
      }
      if (oneSidedBinPresent || forceSymmetrization) {
        symmetrizedBins = true;
        for(int i=0; i<nBinGlobal; ++i){
          double binUp = up->GetBinContent(i);
          double binDo = down->GetBinContent(i);
          double binNom = nom->GetBinContent(i);
          double relUp = std::abs(1 - binUp/binNom);
          double relDo = std::abs(1 - binDo/binNom);
          if ( relUp > relDo) {
            down->SetBinContent(i, 2*binNom-binUp);
          } else {
            up->SetBinContent(i, 2*binNom-binDo);
          }
          if ( relUp > 0.3 || relDo > 0.3 ) {
            if (parameters->getTagIntegerDefault("histoSys.symmetrizeMaxBinsPrintWarnings", true)) {
              WARNclass(TString::Format("You might artifically increase your shape uncertainty: A large relative variation (> 0.3) was found and symmetrized in bin '%d' for shape syst '%s', sample '%s', region '%s'.",i, sys->GetName(),sys->getBase()? sys->getBase()->GetName() : "<none>", sys->getBase() && sys->getBase()->getBase() ? sys->getBase()->getBase()->GetName() : "<none>").Data());
            }
          }
        }
        TQHistogramUtils::normalize(up, nom);
        TQHistogramUtils::normalize(down, nom);
      } else {
        continueSymmetrizing = false;
      }
      if (forceSymmetrization) break;
      loopCounter++;
      // @tag: [histoSys.symmetrizeMaxBinsIterations] This tag determines how many iterations of "symmetrizing bins" and "normalizing the histograms after symmetrizing" we allow before terminating the loop and throwing a warning
      if (loopCounter > parameters->getTagIntegerDefault("histoSys.symmetrizeMaxBinsIterations", 2)) {
        WARNclass(TString::Format("Jumping out of loop to symmetrize maximum bins with one-sided variations in shape syst '%s', sample '%s', region '%s'",sys->GetName() ,sys->getBase()? sys->getBase()->GetName() : "<none>", sys->getBase() && sys->getBase()->getBase() ? sys->getBase()->getBase()->GetName() : "<none>").Data());
        break;
      }
    }
  }
  
  return symmetrizedBins;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::processHistoSys_checkCutoff(TQTaggable* parameters,  TH1* ratio){
  // Cut-off systs with artificially high bins
  // return true if histogram is found to violate the cutoff, false if it is found to be ok
  
  double cutOffHigh = parameters->getTagDoubleDefault("histoSys.cutOffHigh", 2);
  double cutOffLow = parameters->getTagDoubleDefault("histoSys.cutOffLow", 0.3);
  //@tag: [histoSys.cutOffHigh, histoSys.cutOffLow] The systematic is pruned if the variation / nominal ratio histogram has one or more bins above histoSys.cutOffHigh or below histoSys.cutOffLow.
  bool isCutOff = false;
  int nBinGlobal = TQHistogramUtils::getNbinsGlobal(ratio);
  for(int i=0; i<nBinGlobal; ++i){
    if (ratio->GetBinContent(i) > cutOffHigh || ratio->GetBinContent(i) < cutOffLow) {
      isCutOff = true;
    }
  }
  return isCutOff;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::processHistoSys_isFlat(TQTaggable* parameters, TQFolder* sys, TH1* ratio){
  // Check flatness of a shape histogram using a chi2 fit of a flat line
  // return true if histogram is found to be flat, false otherwise

  double threshold = 0.;
  bool doSomething = parameters->getTagDouble("histoSys.flatChi2pValMin",threshold);
  //@tag: [histoSys.flatChi2pValMin] If set both relative up and down variation (w.r.t. the nominal histogram) are tested for being compatible with a flat ratio by performing a Chi2 fit to a polinomial of 0th order. The histoSys is pruned unless the probability to encounter a Chi2 value lower than the one found in the fit of the relative variation exceeds the threshold determined by this tag. I both, up and down variation histograms are found the systematic is kept if one of the variations is deemed significant by the afforementioned criterium.  
  if(!doSomething) return false;
  //print a warning if the threshold is high enough to prune anything
  if (! (threshold<1.) ) warn("Found high threshold ( >= 1.0 ) for histoSys pruning based on Chi2 p-Value of a flat-ratio hypothesis fit");
  bool verbose = parameters->getTagBoolDefault("verbose",false);
  
  TF1 *f = new TF1("f","pol0",ratio->GetXaxis()->GetBinLowEdge(ratio->GetXaxis()->GetFirst()),ratio->GetXaxis()->GetBinUpEdge(ratio->GetXaxis()->GetLast()));
  
  f->SetParameter(0,1.); //set a reasonable start value

  bool isFlat = false;
  auto resultptr = ratio->Fit(f,"SNRQ"); //options: do *N*ot store function internally, use *R*ange of f, *Q*uiet
  auto result = resultptr.Get();
  int status = result ? result->Status() : -1;
  if(status != 0){
    std::cout << "fit failed with status " << status << std::endl;
    int nBinGlobal = TQHistogramUtils::getNbinsGlobal(ratio);
    for(int i=0; i<nBinGlobal; ++i){
      std::cout << "bin " << i << ": " << ratio->GetBinContent(i) << " +- " << ratio->GetBinError(i) << ", ";
    }
    isFlat=false;
    std::cout << std::endl;
  } else {
    if ( (1.-f->GetProb()> threshold) && f->GetNDF()>0 ) {
      if (verbose) std::cout<<TString::Format("Found significant shape: %.4f > %.4f for syst '%s', sample '%s', region '%s'",1.-f->GetProb(), threshold, sys->GetName(),sys->getBase()? sys->getBase()->GetName() : "<none>", sys->getBase() && sys->getBase()->getBase() ? sys->getBase()->getBase()->GetName() : "<none>").Data()<<std::endl;
      isFlat=false;
    } else if (f->GetNDF() < 1) {
      if(verbose) std::cout<<TString::Format("Not enough information to consider as shape systematic (histoSys), NDF after fit of pol0 is %d for syst '%s', sample '%s', region '%s'", f->GetNDF(), sys->GetName(),sys->getBase()? sys->getBase()->GetName() : "<none>", sys->getBase() && sys->getBase()->getBase() ? sys->getBase()->getBase()->GetName() : "<none>").Data()<<std::endl;
      isFlat=true;      
    } else {
      if(verbose) std::cout<<TString::Format("Shape seems insignificant: %.4f < %.4f for syst '%s', sample '%s', region '%s'",1.-f->GetProb(), threshold, sys->GetName(),sys->getBase()? sys->getBase()->GetName() : "<none>", sys->getBase() && sys->getBase()->getBase() ? sys->getBase()->getBase()->GetName() : "<none>").Data()<<std::endl;
      isFlat=true;
    }
  }
  
  delete f; //remove function from memory
  return isFlat;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::processHistoSys_systsToSymmetrizeBins(TQTaggable* parameters, TQFolder* sys, TH1* nom, TH1* up, TH1* down){
  // Fix shape by symmetrizing one-sided bins
  // return true if a symmetrization has been performed, false otherwise
  
  bool symmetrizedBins = false;
  
  double integral = TQHistogramUtils::getIntegral(nom);
  if (up && down) {
    int nBinGlobal = TQHistogramUtils::getNbinsGlobal(nom);
    bool continueSymmetrizing = true;
    int loopCounter = 0;
    while (continueSymmetrizing) {
      bool oneSidedBinPresent = false;
      for(int i=0; i<nBinGlobal; ++i){
        double binUp = up->GetBinContent(i);
        double binDo = down->GetBinContent(i);
        double binNom = nom->GetBinContent(i);
        // if one-sided 
        if ( ((binUp > binNom) && (binDo > binNom)) || ( (binUp < binNom) && (binDo < binNom) ) ) {
          symmetrizedBins = true;
          oneSidedBinPresent = true;
          // symmetrize smaller variation
          if ( (binUp > binDo && binUp > binNom) || (binUp < binDo && binUp < binNom) ) {
            down->SetBinContent(i,  2*binNom - binDo  ) ;
          } else {
            up->SetBinContent(i,  2*binNom - binUp  ) ;
          }
        }
      }
      if (oneSidedBinPresent) {
        symmetrizedBins = true;
        // normalizing variations after they have been massaged
        // only if norm is changed by > 0.01%
        double up_integral = TQHistogramUtils::getIntegral(up);
        double do_integral = TQHistogramUtils::getIntegral(down);
        if ( std::fabs(1 - up_integral/integral) > 0.0001 ) {
          TQHistogramUtils::normalize(up, nom);
        }
        if ( std::fabs(1 - do_integral/integral) > 0.0001 ) {
          TQHistogramUtils::normalize(down, nom);
        }
      } else {
        continueSymmetrizing = false;
      }
      loopCounter++;
      // @tag: [histoSys.symmetrizeBinsIterations] This tag determines how many iterations of "symmetrizing bins" and "normalizing the histograms after symmetrizing" we allow before terminating the loop and throwing a warning
      if (loopCounter > parameters->getTagIntegerDefault("histoSys.symmetrizeBinsIterations", 50)) {
        WARNclass(TString::Format("Jumping out of loop to symmetrize bins with one-sided variations in shape syst '%s', sample '%s', region '%s'",sys->GetName(),sys->getBase()? sys->getBase()->GetName() : "<none>", sys->getBase() && sys->getBase()->getBase() ? sys->getBase()->getBase()->GetName() : "<none>").Data());
        break;
      }
    }
  }
  return symmetrizedBins;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::processHistoSys(TQTaggable* parameters, TQFolder* sys){
  // returns true if the systematic is kept, false causes the systematic to be dropped
  
  if (!parameters) return true; //no parameters for operation given, hence, don't touch the systematic
  if (!sys) return true;


  // --------------- Get All Tags ------------------
  bool doSomething = false;
  
  // prune based on different criteria
  //@tag: [histoSys.prune] Controls if pruning criteria based on a chi2 test should be applied to HistoSys ("shape") systematics/components (Default: false). The stringency of the pruning can be controlled with the tag 'histoSys.flatChi2pValMin'
  bool pruneBasedOnFlatness = parameters->getTagBoolDefault("histoSys.prune",false);
  
  //@tag: [histoSys.pruneBasedOnCutOff] Controls if pruning criteria based on a cut off should be applied to HistoSys ("shape") systematics/components (Default: false). Cut-off values can be controlled with tags 'histoSys.cutOffHight' and 'histoSys.cutOffLow'. 
  bool pruneBasedOnCutOff = parameters->getTagBoolDefault("histoSys.pruneBasedOnCutOff",false);
  
  //@tag: [histoSys.symmetrizeBins] Controls if shapes with one-sided bins should be symmetrized. If the up and down variation have the same sign in a given bin, the smaller of the two will be symmetrized, i.e. the sign of the smaller variation is swapped. 
  bool symmetrizeBins = parameters->getTagBoolDefault("histoSys.symmetrizeBins",false);
  
  //@tag: [histoSys.symmetrizeMaxBins,histoSys.forceSymmetrizeMaxBins] Symmetrize shape when a one-sided bin (up and down variation going in the same direction) is present. The new shape is determined by symmetrizing the larger variation (i.e. setting the smaller of the variations to the mirror image of the larger) in each bin. Warning: This could lead to amplification of statistically fluctuating templates so it should be used with caution. The tag 'histoSys.forceSymmetrizeMaxBins' can be used to force certain systematics to be symmetrized even if they don't exhibit one-sided bins (might be useful in case of largely asymmetric variations). 
  bool symmetrizeMaxBins = parameters->getTagBoolDefault("histoSys.symmetrizeMaxBins",false);
  bool forceSymmetrizeMaxBins = parameters->getTagBoolDefault("histoSys.forceSymmetrizeMaxBins",false);

  //@tag: [histoSys.smooth] activate smoothing of systematic variation (default: false). Choose a smoothing method with tat 'histoSys.smoothingMethod'
  bool smooth = parameters->getTagBoolDefault("histoSys.smooth",false);

  if (pruneBasedOnFlatness ||
      pruneBasedOnCutOff ||
      symmetrizeMaxBins ||
      symmetrizeBins ||
      forceSymmetrizeMaxBins ||
      smooth) {
    doSomething = true;
  }
  
  bool verbose = parameters->getTagBoolDefault("verbose",false);

  TQSystematics::Status status = TQSystematics::ACTIVE;
  
  if (!doSomething){
    return true;
  } else  {

    // ----------- Get Histograms -----------
    TString pathUp, pathDown, pathNom;
    bool hasUp   = sys->getTagString("HistoHigh",pathUp);
    bool hasDown = sys->getTagString("HistoLow",pathDown);
    bool hasNom  = sys->getBase() ? sys->getBase()->getTagString("Histo",pathNom) : false;
    if (!hasNom) warn(TString::Format("Failed to obtain path to nominal histogram for systematic '%s' for sample '%s'!", sys->GetName(), sys->getBase()? sys->getBase()->GetName() : "<none>"));
    if (! (hasDown || hasUp)) {
      warn(TString::Format("Failed to obtain path to both, up and down variation histogram for systematic '%s' for sample '%s'!", sys->GetName(), sys->getBase()? sys->getBase()->GetName() : "<none>")); 
    } else if (hasNom && (hasUp || hasDown)) {
      //obtaining the raw objects from the folders, ownership remains with the folders
      TH1* up   = hasUp   ?   dynamic_cast<TH1*>(this->getHisto(pathUp)   ) : nullptr;
      TH1* down = hasDown ?   dynamic_cast<TH1*>(this->getHisto(pathDown) ) : nullptr;
      TH1* nom  =             dynamic_cast<TH1*>(this->getHisto(pathNom)  ) ;
      //check if histograms were obtained as expected

      if (!up && hasUp) { throw std::runtime_error(TString::Format("Found path for up variation histogram but histogram itself could not be found at expected location '%s' for histoSys '%s' of sample '%s'",pathUp.Data(), sys->GetName(), sys->getBase()? sys->getBase()->GetName() : "<none>" ).Data()); return false; }

      if (!down && hasDown) { throw std::runtime_error(TString::Format("Found path for down variation histogram but histogram itself could not be found at expected location '%s' for histoSys '%s' of sample '%s'",pathDown.Data(), sys->GetName(), sys->getBase()? sys->getBase()->GetName() : "<none>" ).Data()); return false; }

      // -------------- Process HistoSys --------------
      bool isFlat = up || down; // if we have at least one variation we assume the histoSys is flat (i.e. to be pruned) until we find something exceeding the threshold
      bool isCutOff = false;
      bool isSymmetrizedBins = false;
      bool isSmoothed = false;

      if(nom->GetNbinsX() > 1){
        
        // 1. smoothing
        if (smooth) {
          isSmoothed = processHistoSys_smoothVariation(parameters, sys, nom, up, down);
        }

        // 2. symmetrization
        if (symmetrizeBins) {
          isSymmetrizedBins = processHistoSys_systsToSymmetrizeBins(parameters, sys, nom, up,down);
        }
        if (symmetrizeMaxBins) {
          isSymmetrizedBins = processHistoSys_systsToSymmetrizeMaxBins(parameters, sys, nom, up, down);
        }
        if (forceSymmetrizeMaxBins) {
          isSymmetrizedBins = processHistoSys_systsToSymmetrizeMaxBins(parameters, sys, nom, up, down, true);
        }

        // 3. pruning
        for (auto var: {up,down}) {
          if (!var) continue;
          // check for flatness and cut off
          TH1* ratio = TQHistogramUtils::getRelativeVariationHistogram(var, nom, 1e-9);
          if(ratio){
            if(!TQHistogramUtils::isEmpty(ratio,0.001,1.)){
              if (pruneBasedOnFlatness && isFlat) {
                isFlat = processHistoSys_isFlat(parameters, sys, ratio);
              }
              if (pruneBasedOnCutOff && !isCutOff) {
                isCutOff = processHistoSys_checkCutoff(parameters, ratio);
              }
            } 
            delete ratio;
          } else {
            if(verbose) std::cout<<TString::Format("Failed to obtain ratio histogram for histoSys '%s' of sample '%s'", sys->GetName(), sys->getBase()? sys->getBase()->GetName() : "<none>" ).Data()<<std::endl;
          }          
        }
      }

      // ---------------- Set Status ----------------------
      if (isSymmetrizedBins) {
        status = TQSystematics::SYMMETRIZED;
        if(verbose) std::cout<<TString::Format("Shape had bin with up and down variation going in the same direction for syst '%s', sample '%s', region '%s'",sys->GetName(),sys->getBase()? sys->getBase()->GetName() : "<none>", sys->getBase() && sys->getBase()->getBase() ? sys->getBase()->getBase()->GetName() : "<none>").Data()<<std::endl;
      }
      if (isSmoothed) {
        status = TQSystematics::SMOOTHED;
        if (verbose) std::cout<<TString::Format("Shape has been smoothed for syst '%s', sample '%s', region '%s'",sys->GetName(),sys->getBase()? sys->getBase()->GetName() : "<none>", sys->getBase() && sys->getBase()->getBase() ? sys->getBase()->getBase()->GetName() : "<none>").Data()<<std::endl;
      }       
      // need to add condition whether "flatness pruning" was considered because the default of isFlat is true;
      if (isFlat && pruneBasedOnFlatness) {
        if (verbose) std::cout<<TString::Format("Shape has been pruned because no significant shape was found: syst '%s', sample '%s', region '%s'",sys->GetName(),sys->getBase()? sys->getBase()->GetName() : "<none>", sys->getBase() && sys->getBase()->getBase() ? sys->getBase()->getBase()->GetName() : "<none>").Data()<<std::endl;
        status = TQSystematics::INSIGNIFICANT;
      }
      if (isCutOff) {
        status = TQSystematics::OVERSHOOT;
        if(verbose) std::cout<<TString::Format("Shape seems artifical for syst '%s', sample '%s', region '%s'",sys->GetName(),sys->getBase()? sys->getBase()->GetName() : "<none>", sys->getBase() && sys->getBase()->getBase() ? sys->getBase()->getBase()->GetName() : "<none>").Data()<<std::endl;
      }
    }
  }
  
  //after all pruning variants are done, check the systematic status
  sys->setTagInteger("Status",status);
  if (status == TQSystematics::INSIGNIFICANT || status == TQSystematics::OVERSHOOT || status == TQSystematics::DROPPED) {
    return false;
  }

  return true;
}

//__________________________________________________________________________________|___________

namespace {
  inline double sgn(double x){
    return x<0?-1:+1;
  }
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::isProtectedSys(TQFolder* sys, TQTaggable* parameters, const TString& prefix) const {
  // checks if a systematic should be extempt from some treatment
  // 'select.tag'/'select.name' specifies whitelist(s) for systematics that some treatment should be applied to
  // 'except.tag'/'except.name' specifies blacklist(s) for systematics that some treatment should NOT be applied to
  TQTaggable paramLocal;
  paramLocal.importTagsWithoutPrefix(parameters,prefix);
  bool passedWhiteList = true; //if no whitelisting is present this will stay unchanged
  bool failedBlackList = false;
  //check for whitelisted tags
  std::vector<TString> whiteListTags = paramLocal.getTagVString("select.tag");
  if (paramLocal.hasTag("select.tag")) whiteListTags.push_back(paramLocal.getTagStringDefault("select.tag"));
  for (auto key:whiteListTags) {
    passedWhiteList = sys->getTagBoolDefault(key,false); //unless we find a match this will not pass selection criteria
    if (passedWhiteList) break;
  }
  if (!passedWhiteList) return true; //if we didn't match any entry on the white list (given it exists) this systematic is not selected, i.e., protected

  //check for white listed names
  std::vector<TString> whiteListNames = paramLocal.getTagVString("select.name");
  if (paramLocal.hasTag("select.name")) whiteListNames.push_back(paramLocal.getTagStringDefault("select.name"));
  for (auto pattern:whiteListNames) {
    passedWhiteList = TQStringUtils::matches(sys->GetName(),pattern); //unless we find a match this will not pass selection criteria
    if (passedWhiteList) break;
  }
  if (!passedWhiteList) return true; //if we didn't match any entry on the white list (given it exists) this systematic is not selected, i.e., protected

  //check for black listed tags
  std::vector<TString> blackListTags = paramLocal.getTagVString("except.tag");
  if (paramLocal.hasTag("except.tag")) whiteListTags.push_back(paramLocal.getTagStringDefault("except.tag"));
  for (auto key:blackListTags) {
    failedBlackList = sys->getTagBoolDefault(key,false); //unless we find a match this will not pass selection criteria
    if (failedBlackList) return true; //if this systematic is black listed we can already say that it's proteced from the treatment
  }

  //check for black listed names
  std::vector<TString> blackListNames = paramLocal.getTagVString("except.name");
  if (paramLocal.hasTag("except.name")) whiteListNames.push_back(paramLocal.getTagStringDefault("except.name"));
  for (auto pattern:blackListNames) {
    failedBlackList = TQStringUtils::matches(sys->GetName(),pattern); //unless we find a match this will not pass selection criteria
    if (failedBlackList) return true;
  }

  //if neither black lists are matched nor (existing) whitelist are failed this systematic is eligible for the treatment, i.e., not protected
  return false;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::processOverallSys(TQTaggable* parameters, TQFolder* sys){
  // process the overall systematics, applying pruning and symmetrization
  
  // retrieve some global parameters
  Double_t sysRelMin       = parameters->getTagDoubleDefault("SysRelMin", -1.);
  Double_t sysRelThreshold = parameters->getTagDoubleDefault("SysRelThreshold", -1.);
  Double_t sysRelCutoff    = parameters->getTagDoubleDefault("SysRelCutoff", -1.);
  Double_t sysRelCap       = parameters->getTagDoubleDefault("SysRelCap", -1.);
  Double_t sysRelZero      = parameters->getTagDoubleDefault("SysRelZero", 2*std::numeric_limits<double>::epsilon());
  Double_t sysSigMin       = parameters->getTagDoubleDefault("SysSigMin", -1.);
  Double_t sysAsymLimit    = -1.;
  bool hasSysAsymLimit     = parameters->getTagDouble("SysAsymLimit",sysAsymLimit);
  if (sysAsymLimit>=1.) throw std::runtime_error("Illegal value for tag 'SysAsymLimit', must be smaller than 1.0");

  double ratio_Down     = sys->getTagDoubleDefault("Low", 1);
  double ratio_Up       = sys->getTagDoubleDefault("High", 1);
  double ratio_Down_Err = sys->getTagDoubleDefault("LowErr", 0);
  double ratio_Up_Err   = sys->getTagDoubleDefault("HighErr", 0);

  const double signifUp   = fabs(ratio_Up-1.)/ratio_Up_Err;
  const double signifDown = fabs(ratio_Down-1.)/ratio_Down_Err;

  TString symmode = parameters->getTagStringDefault("SysSymmetrizationMode","envelope");
  bool verbose = parameters->getTagBoolDefault("verbose",
#ifdef _DEBUG_
                                               true
#else
                                               false
#endif
                                               );

  TString sysname,samplename,regionname;
  if(verbose){
    sysname    = sys->getTagStringDefault("~Systematic",sys->GetName());
    samplename = sys->getTagStringDefault("~Sample",sys->getBase() ? sys->getBase()->GetName() : "<none>");
    regionname = sys->getTagStringDefault("~Channel",sys->getBase() && sys->getBase()->getBase() ? sys->getBase()->getBase()->GetName() : "<none>");
  }

  Bool_t dropOverallSys = false;
  TQSystematics::Status status = TQSystematics::ACTIVE;
  
  // first process all fixes and then process the "drop" stati or symmetrize

  // @tag: [SysRelMin] Variations below a certain minimum (sysRelMin) are cast to 1.
  if (sysRelMin > 0. && TQHistogramUtils::isCloseToOneRel(ratio_Up, sysRelMin) && !isProtectedSys(sys,parameters,"SysRelMin.")) {
    if(verbose) std::cout<<"OverallSys '"<<sysname<<"' for sample '"<<samplename<<"' in region '"<<regionname<<"' is close to one for the up variation with sysRelMin threshold "<<sysRelMin<<" (up: "<<ratio_Up<<")"<<std::endl;
    ratio_Up = 1.;
    status = TQSystematics::FIXED;
  }
  if (sysRelMin > 0 && TQHistogramUtils::isCloseToOneRel(ratio_Down, sysRelMin) && !isProtectedSys(sys,parameters,"SysRelMin.") ) {
    if(verbose) std::cout<<"OverallSys '"<<sysname<<"' for sample '"<<samplename<<"' in region '"<<regionname<<"' is close to one for the down variation with sysRelMin threshold "<<sysRelMin<<" (down: "<<ratio_Down<<")"<<std::endl;
    ratio_Down = 1.;
    status = TQSystematics::FIXED;
  }
  // @tag: [SysRelCap] variations above a certain maximum (sysRelCap) are cast to maximum threshold.
  if ((sysRelCap > 0.)
      && !TQHistogramUtils::isCloseToOneRel(ratio_Up, sysRelCutoff)
      && !isProtectedSys(sys,parameters,"SysRelCap.")) {
    if(verbose) std::cout<<"OverallSys '"<<sysname<<"' for sample '"<<samplename<<"' in region '"<<regionname<<"' exceeds sysRelCap threshold "<<sysRelCap<<" (up: "<<ratio_Up<<")"<<std::endl;
    ratio_Up = ratio_Up > ratio_Down ? (1+sysRelCap) : (1/(1+sysRelCap));
    //if(verbose) std::cout<<"ratio_Up set to "<<ratio_Up<<" (ratio_Down="<<ratio_Down<<")"<<std::endl;
    status = TQSystematics::FIXED;
  }
  if ((sysRelCap > 0.)
      && !TQHistogramUtils::isCloseToOneRel(ratio_Down, sysRelCutoff)
      && !isProtectedSys(sys,parameters,"SysRelCap.")) {
    if(verbose) std::cout<<"OverallSys '"<<sysname<<"' for sample '"<<samplename<<"' in region '"<<regionname<<"' exceeds sysRelCap threshold "<<sysRelCap<<" (down: "<<ratio_Down<<")"<<std::endl;
    ratio_Down = ratio_Down > ratio_Up ? (1+sysRelCap) : (1/(1+sysRelCap));
    //if(verbose) std::cout<<"ratio_Down set to "<<ratio_Down<<" (ratio_Up="<<ratio_Up<<")"<<std::endl;
    status = TQSystematics::FIXED;
  }

  if((sysRelZero > 0.)
     && TQHistogramUtils::isCloseToOneRel(ratio_Up, sysRelZero)
     && TQHistogramUtils::isCloseToOneRel(ratio_Down, sysRelZero)
     && !isProtectedSys(sys,parameters,"SysRelZero.") ){
    // @tag: [SysRelZero] drop OverallSys if variation does NOT exceed a certain threshold (sysRelZero)
    if(verbose) std::cout<<"OverallSys '"<<sysname<<"' for sample '"<<samplename<<"' in region '"<<regionname<<"' is close to one with threshold "<<sysRelZero<<" (up: "<<ratio_Up<<", down: "<<ratio_Down<<")"<<std::endl;
    dropOverallSys = true;
    status = TQSystematics::ZERO;
  } else if((sysRelThreshold > 0.)
            && TQHistogramUtils::isCloseToOneRel(ratio_Up, sysRelThreshold)
            && TQHistogramUtils::isCloseToOneRel(ratio_Down, sysRelThreshold)
            && !isProtectedSys(sys,parameters,"SysRelThreshold.") ){
    // @tag: [SysRelThreshold] drop OverallSys if variation does NOT exceed a certain threshold (sysRelThreshold)
    if(verbose) std::cout<<"OverallSys '"<<sysname<<"' for sample '"<<samplename<<"' in region '"<<regionname<<"' is close to one with threshold "<<sysRelThreshold<<" (up: "<<ratio_Up<<", down: "<<ratio_Down<<")"<<std::endl;
    dropOverallSys = true;
    status = TQSystematics::DROPPED;
  } else if ((sysRelCutoff > 0.)
             && (!TQHistogramUtils::isCloseToOneRel(ratio_Up, sysRelCutoff)
                 || !TQHistogramUtils::isCloseToOneRel(ratio_Down, sysRelCutoff))
             && !isProtectedSys(sys,parameters,"SysRelCutoff.")) {
    // @tag: [SysRelCutoff] drop OverallSys if variation DOES exceed a certain threshold (sysRelCutoff)
    if(verbose) std::cout<<"OverallSys '"<<sysname<<"' for sample '"<<samplename<<"' in region '"<<regionname<<"' exceeds sysRelCutoff threshold "<<sysRelCutoff<<" (up: "<<ratio_Up<<", down: "<<ratio_Down<<")"<<std::endl;
    status = TQSystematics::OVERSHOOT;
    dropOverallSys = true;
  } else if ( sysSigMin>0.
              && ( ratio_Up_Err>0 && signifUp < sysSigMin )
              && ( ratio_Down_Err>0. && signifDown < sysSigMin )
              && !isProtectedSys(sys,parameters,"SysSigMin.")) {
    // @tag: [SysSigMin] ]drop OverallSys if error on variation DOES exceed a certain threshold (sysSigMin)
    if(verbose) std::cout<<"OverallSys '"<<sysname<<"' for sample '"<<samplename<<"' in region '"<<regionname<<"' has a variation uncertainty larger than SysSigMin  threshold "<<sysSigMin<<" (up_err: "<<ratio_Up_Err<<", down_err: "<<ratio_Down_Err<<")"<<std::endl;
    status = TQSystematics::INSIGNIFICANT;
    dropOverallSys = true;
  } else if (hasSysAsymLimit && !isProtectedSys(sys,parameters,"SysAsymLimit.")) {
    //symmetrize systematic if too asymmetric or even one-sided (only if limit is explicitly set!)
    double deltaUp = ratio_Up-1.;
    double deltaDown = ratio_Down-1.;
    double sign = sgn( signifUp > signifDown ? deltaUp : -deltaDown);//determine the sign of the systematic based on the most significant variation
    if ( (std::min( fabs(deltaUp/deltaDown), fabs(deltaDown/deltaUp)) < sysAsymLimit) || (deltaDown*deltaUp>0.) ){
      if(verbose) std::cout<<"OverallSys '"<<sysname<<"' for sample '"<<samplename<<"' in region '"<<regionname<<"' is being symmetrized with mode "<<symmode.Data()<<" (up: "<<ratio_Up<<", down: "<<ratio_Down<<")"<<std::endl;
      if(symmode.CompareTo("envelope",TString::kIgnoreCase) == 0){ //take the larger of the two variations, use it as a symmetric uncertainty
        // a.k.a. "Michael's method"
        double deltaSym = std::max(std::abs(deltaUp),std::abs(deltaDown));
        ratio_Up = 1 + sign*deltaSym;
        ratio_Down = 1 - sign*deltaSym;
        status = TQSystematics::SYMMETRIZED;
      } else if(symmode.CompareTo("drop",TString::kIgnoreCase) == 0){ //simply remove the too asymmetric (or one sided) systematic
        // drop all single sided
        dropOverallSys = true;
        status = TQSystematics::DROPPED;
      } else if(symmode.CompareTo("average",TString::kIgnoreCase) == 0){ //use the average deviation from unity
        // average
        double deltaSym = 0.5*(std::abs(deltaUp)+std::abs(deltaDown));
        ratio_Up = 1 + sign*deltaSym;
        ratio_Down = 1 - sign*deltaSym;
        status = TQSystematics::SYMMETRIZED;
      } else if(symmode.CompareTo("shift",TString::kIgnoreCase) == 0){ //shift up and down variations such that their avaerage is 1.0
        // a.k.a. "Monica's method"
        double mean = 0.5*(deltaUp+deltaDown);
        //"sign" is based on the more significant variation, here we only care about their nominal (original) value!
        ratio_Up = 1+deltaUp-mean;
        ratio_Down = 1+deltaDown-mean;
        status = TQSystematics::SYMMETRIZED;
      } else if(symmode.CompareTo("squaresum",TString::kIgnoreCase) == 0){
        // do we need this?
        double deltaSym = sqrt(std::pow(deltaUp,2.)+std::pow(deltaDown,2.));
        ratio_Up = 1 + sign*deltaSym;
        ratio_Down = 1 - sign*deltaSym;
        status = TQSystematics::SYMMETRIZED;
      } else {
        error(TString::Format("unknown symmetrization mode: '%s'",symmode.Data()));
      }
    }
  } else if(((ratio_Up < 1 && ratio_Down < 1) || (ratio_Up > 1 && ratio_Down > 1))){ //no protection here since only a label (enum) is asigned
    status = TQSystematics::ONESIDED;
  }

  if(verbose) std::cout<<"OverallSys '"<<sysname<<"' for sample '"<<samplename<<"' in region '"<<regionname<<"' done (up: "<<ratio_Up<<", down: "<<ratio_Down<<", status: "<<status<<")"<<std::endl;

  double variation = std::max(fabs(ratio_Up-1.),fabs(ratio_Down-1.));
  sys->setTagDouble("Low", ratio_Down);
  sys->setTagDouble("High", ratio_Up);
  sys->setTagDouble("LowErr", ratio_Down_Err);
  sys->setTagDouble("HighErr", ratio_Up_Err);
  sys->setTagInteger("Status", status);
  sys->setTagDouble("Percent", 100.*variation);
  return !dropOverallSys;
}


//__________________________________________________________________________________|___________

TH1 * TQSystematicsManager::getVarHisto(const TString& nameVariation, const TString& nameChannel, const TString& nameSample, bool verbose) {
  // attempt to obtain a histogram from the folder
  TH1* hist = dynamic_cast<TH1*>(this->repository()->getObject(nameSample,TQFolder::concatPaths(this->varPrefix, nameVariation, nameChannel)));
  if (verbose && !hist){
    error(TString::Format("getVarHisto(...): Failed to find histogram '%s' for variation '%s' in channel '%s'",nameSample.Data(),nameVariation.Data(),nameChannel.Data()));
  }
  return hist;  
}

//__________________________________________________________________________________|___________

TH1 * TQSystematicsManager::getVarHisto(const TString& path, bool verbose) {
  // attempt to obtain a histogram from the folder
  return this->getHisto(TQFolder::concatPaths(this->varPrefix, path),verbose);
}

//__________________________________________________________________________________|___________

TH1 * TQSystematicsManager::getSysHisto(const TString& path, bool verbose) {
  // attempt to obtain a histogram from the folder
  return this->getHisto(TQFolder::concatPaths(this->sysPrefix, path),verbose);
}

//__________________________________________________________________________________|___________

TH1 * TQSystematicsManager::getHisto(TString path, bool verbose) {
  // attempt to obtain a histogram from the folder  
  TString name = TQFolder::getPathTail(path);  
  TH1* hist = dynamic_cast<TH1*>(this->repository()->getObject(name,path));
  if (verbose && !hist){
    error(TString::Format("getHisto(...): Failed to find histogram '%s' in '%s'", name.Data(), path.Data()));
  }
  return hist;
}

//__________________________________________________________________________________|___________


bool TQSystematicsManager::isNegligibleSys(TQFolder * sys, TQTaggable* parameters){
  // Pruning based on contribution of sample in channel.
  // If a sample contributes less in the channel than 'threshold' percent of the specified total,
  // the systematic attached to that sample is pruned.
  if(!parameters) return false;
  
  bool prune = false;
  TQSystematics::Status status = TQSystematics::ACTIVE;

  //@tag: [pruneNegligibles.samples, pruneNegligibles.channelFilter] tags to specify which systematics for which samples in what channels should be considered for this contribution based pruning. 'pruneNegligibles.samples' are vectors of strings that correspond to filters samples to consider. The total contribution is calculated from summing up all the samples that match the 'sample' string. The channelFilter is a simple string that should match the channel names to consider (default is all channels). 
  //@tag: [pruneNegligibles.threshold] Threshold to apply for pruning. systematic is pruned if: "yields of single sample" < (threshold * "yields of all samples")
  
  // ----------  Preparation ----------
  double pruneThreshold = parameters->getTagDoubleDefault("pruneNegligibles.threshold", 0.01);
  std::vector<TString> samplesToPrune = parameters->getTagVString("pruneNegligibles.Samples");
  if (samplesToPrune.empty()) {
    ERRORclass("You configured pruning based on contribution but didn't provide any samples to be considered. Please specify a list of samples with the tag 'pruneNegligibles.Samples'");
  }
  TString channelFilter = parameters->getTagStringDefault("pruneNegligibles.channelFilter", "*");

  std::cout << "==>> Pruning negligibles for syst: " << sys->GetName() << std::endl;

  // systematic that is looped over
  TString sysName = sys->getTagStringDefault("Systematic",sys->GetName());
  TQStringUtils::removeLeadingText(sysName,"OverallSys.");
  TQStringUtils::removeLeadingText(sysName,"HistoSys.");
  
  // sample that is looped over
  TString sampleName = sys->getBase()? sys->getBase()->GetName() : "";
  if (sampleName.IsNull()) {
    WARNclass("Pruning based on contribution has been configured but sample name of sample that is currently looped over couldn't be found! Skipping...");
    return false;
  } else {
    sampleName.ReplaceAll("Sample.", "");
  }

  // channel that is looped over
  TString channelName = sys->getBase()->getBase() ? sys->getBase()->getBase()->GetName() : "";
  if (channelName.IsNull()) {
    WARNclass("Pruning based on contribution has been configured but region name of channel that is currently looped over couldn't be found! Skipping...");
    return false;
  } else {
    channelName.ReplaceAll("Channel.", "");
  }
  
  // check if channel that is looped over is configured for pruning
  if (!TQStringUtils::matches(channelName, channelFilter)) {
    return false;
  }

  for (auto sampleToPruneFilter : samplesToPrune) {
  
    // check if sample that is looped over is configured for pruning
    // prevents that syst is accidentally pruned based on contribution of completely independent samples
    if (!TQStringUtils::matches(sampleName, sampleToPruneFilter)) continue;
    

    // ----------  Get Histograms ----------
    // Get total yields based on sampleFilter
    double totalIntegral = 0;
    TQFolderIterator samplesToAddItr(sys->getBase()->getBase()->getListOfFolders("Sample."+sampleToPruneFilter));
    while(samplesToAddItr.hasNext()) {
      TQFolder* sample = samplesToAddItr.readNext();
      TString sampleToAddName = sample->GetName();
      sampleToAddName.ReplaceAll("Sample.", "");
      TString histPath = TQFolder::concatPaths("Nominal", channelName, sampleToAddName);
      TH1 *tmp_hist = dynamic_cast<TH1*>(this->repository()->getObject(histPath));
      if (tmp_hist) {
        totalIntegral += tmp_hist->Integral(0, tmp_hist->GetNbinsX()+1);
      } else {
        WARNclass("Couldn't find histogram with path '%s'", histPath.Data());
        continue;
      }
    }
    // Check contribution of currently looped over sample in region
    TString histPathSingle = TQFolder::concatPaths("Nominal", channelName, sampleName);
    TH1 *histSingle = dynamic_cast<TH1*>(this->repository()->getObject(histPathSingle));
    double singleIntegral;
    if (histSingle) {
      singleIntegral = histSingle->Integral(0, histSingle->GetNbinsX()+1);
    } else {
      WARNclass("Couldn't find histogram with path '%s'", histPathSingle.Data());
      continue;
    }

    if (totalIntegral > 0 && singleIntegral > 0) {
      if (singleIntegral < (pruneThreshold * totalIntegral) ) {
        // the channel that is looped over exhibits a negligible contribution from the current sample that is looped over and the systematic that is looped over is configured for pruning. So => prune this!
        status = TQSystematics::DROPPED;
        prune = true;
      }
      // }
    } 
  }

  sys->setTagInteger("Status", status);
  if (prune) {sys->setTagString("Message", "Pruned based on (negligible) contribution");} 
  return prune;
}


//__________________________________________________________________________________|___________

TH1* TQSystematicsManager::computeHistogramVariation(TQFolder* computeFolder, const TString& nameChannel, const TString& nameSample){
  // compute the variation based on the information in the computeFolder
  //@tag: [Mode] mode used for the computation of the Up/Down variations from the input variations. Possible values include: single,difference,minimum,maximum,envelope,stddev,hessian
  //@tag: [Variations] list of input variations to use for the computation of the Up/Down variations
  //@tag: [Baseline] baseline variation to be used (default: Nominal)
  //@tag: [StatError] stat error histogram to be used
  //@tag: [Direction] direction of this variation (+1 for Up, -1 for Down)
  //@tag: [Scale] scale up or down this variation by some arbitrary factor
  //@tag: [TransferRelative] transfer relative variation instead of absolute (default: true)
  /* @tag[Smoothing.SmoothBins]: A vector of integers specifying the bin boundaries to be used for
     calculating the relative variation between nominal and a variation. This relative variation
     is applied to the nominal histogram to get the smoothed variation.
     Example: Given a nominal histogram with 7 bins and the following tag applied Smoothing.SmoothBins={1, 2, 3, 5, 8} 
     means the relative variation would be calculated for the last 3 bins merged, that is, the sum of bin 5, 6, 7 
     for nominal (Nom_567) and the variation (Var_567) is taken and the smoothed variational histogram 
     bins will become:  Var_5 = Var_567/Nom_567*Nom_5, Var_6=Var_567/Nom_567*Nom_6, ... */
  
  TH1* hist = NULL;
  std::vector<TString> varList = computeFolder->getTagVString("Variations");
  const TH1* h_Nominal = this->getVarHisto("Nominal", nameChannel, nameSample);

  // mode  
  TString mode_Compute = computeFolder->getTagStringDefault("Mode","");

  // baseline histogram  
  TString varBaseline = computeFolder->getTagStringDefault("Baseline","Nominal");
  bool ownBaseline = false;
  if(varBaseline.IsNull()){
    ERRORclass("cannot compute variation '%s' without baseline!",computeFolder->getTagStringDefault("~Systematic","<unknown>").Data());
    return NULL;
  }
  const TH1* h_Baseline = this->getVarHisto(varBaseline, nameChannel, nameSample);
  DEBUGclass("obtained baseline histogram '%s' with integral '%g' for channel '%s' and sample '%s'",varBaseline.Data(),TQHistogramUtils::getIntegral(h_Baseline),nameChannel.Data(),nameSample.Data());

  std::vector<int>  smoothedBinning = computeFolder->getTagVInteger("~Smoothing.SmoothBins");
  if (!smoothedBinning.empty() && h_Baseline) {
    h_Baseline = TQHistogramUtils::getSmoothedManually(h_Baseline, smoothedBinning);
    ownBaseline = true;
  }

  // error histogram  
  TString nameError = computeFolder->getTagStringDefault("StatError","");
  TH1* h_StatError = nameError.IsNull() ? NULL : this->getVarHisto(nameError, nameChannel, nameSample);

  // +/- flip direction  
  int val_Direction = computeFolder->getTagIntegerDefault("Direction",1);
  bool relative = computeFolder->getTagBoolDefault("TransferRelative",true);
  if (abs(val_Direction)!=1) {
    WARNclass("Systematic variation direction must be +/-1, ignoring");
    val_Direction = 1;
  }
  // scale the computed variation by some amount (eg. 0.5 for alpha_s uncertainties)
  double val_Scale = fabs(computeFolder->getTagDoubleDefault("Scale",1.0));

  // perform computation  
  if (varList.size()) {
    TObjArray* hList_Var = new TObjArray();
    for (auto nameVar: varList) {
      if(nameVar.IsNull()){
        ERRORclass("unable to obtain variation without name!");
        continue;
      }
      TH1* h = this->getVarHisto(nameVar, nameChannel, nameSample);
      if (!smoothedBinning.empty() && h) {
        h = TQHistogramUtils::getSmoothedManually(h, smoothedBinning);
      }
      if(h){
        DEBUGclass("obtained variation histogram '%s' with integral '%g' for channel '%s' and sample '%s'",nameVar.Data(),TQHistogramUtils::getIntegral(h),nameChannel.Data(),nameSample.Data());      
        hList_Var->Add(h);
      } else {
        ERRORclass("failed to obtain variation histogram '%s' for channnel '%s' and sample '%s'",nameVar.Data(),nameChannel.Data(),nameSample.Data());
      }
    }
    
    // variation computer
    HistogramVariationComputer varsProcessor(h_Baseline);
    // check for consistency between variations & baseline
    for (int ivar=0 ; ivar<hList_Var->GetEntries() ; ++ivar) {
      TH1* h_Var = dynamic_cast<TH1*>(hList_Var->At(ivar));
      if(!TQHistogramUtils::checkConsistency(h_Var,h_Baseline)) {
        ERRORclass("Inconsistent histograms between variation and baseline");
        return NULL;
      }
      if(h_Nominal && !TQHistogramUtils::checkConsistency(h_Var,h_Nominal)) {
        ERRORclass("Inconsistent histograms between variation and nominal");
        return NULL;        
      }
    }
    bool computed = varsProcessor.computeVariation(hList_Var,mode_Compute);
    if (!computed) {
      ERRORclass("Failed to compute '%s'",mode_Compute.Data());
      return NULL;      
    }
    // manually use errors from an existing variation
    if (h_StatError) varsProcessor.useStatError(h_StatError,relative);
    // set result
    hist = varsProcessor.applyVariation(h_Nominal,val_Scale*val_Direction,relative);

    delete hList_Var;
  }
  
  if (ownBaseline) delete h_Baseline;
  
  return hist;
}

//__________________________________________________________________________________|___________

TH1* TQSystematicsManager::normalizeShapeVariation(const TH1* h_Var, const TH1* h_Nominal, const TH1* h_Sym) {
  // normalize histogram variation to get shape variation
  TH1* shape_Var = NULL;
  if (h_Var) {
    shape_Var = TQHistogramUtils::copyHistogram(h_Var);
    TQHistogramUtils::normalize(shape_Var, h_Nominal);
  } else if(h_Sym){ // symmetrize if needed
    shape_Var = TQHistogramUtils::copyHistogram(h_Sym);
    TQHistogramUtils::normalize(shape_Var, h_Nominal);
    TQHistogramUtils::addHistogramWithoutError(shape_Var, h_Nominal, -2.);
    TQHistogramUtils::scaleHistogram(shape_Var, -1.);
    TQHistogramUtils::ensureMinimumBinContent(shape_Var, 1e-12, true);
    TQHistogramUtils::normalize(shape_Var, h_Nominal);
  }
  return shape_Var;
}

//__________________________________________________________________________________|___________

void TQSystematicsManager::setSystematicPrefix(const TString& prefix){
  // set the prefix of all systematics histogram to use in the repository
  this->sysPrefix = prefix;
}

//__________________________________________________________________________________|___________

void TQSystematicsManager::setVariationPrefix(const TString& prefix){
  // set the prefix of all variations histogram to use in the repository  
  this->varPrefix = prefix;
}

//__________________________________________________________________________________|___________

TString TQSystematicsManager::storeSysHisto(TH1 * histo, const TString& nameVariation, const TString& direction, const TString& nameChannel) {
  // store a variation (input) histogram  
  return this->storeSysHisto(histo,TQFolder::concatPaths(nameVariation, direction, nameChannel));
}


//__________________________________________________________________________________|___________

TString TQSystematicsManager::storeVarHisto(TH1 * histo, const TString& nameVariation, const TString& nameChannel) {
  // store a variation (input) histogram
  return this->storeVarHisto(histo,TQFolder::concatPaths(nameVariation,nameChannel));
}

//__________________________________________________________________________________|___________

TString TQSystematicsManager::storeSysHisto(TH1 * histo, const TString& path) {
  // store a systematic (output) histogram  
  if (!histo) return "";  
  if (!this->repository()->addObject(histo, TQFolder::concatPaths(this->sysPrefix,path) + "+")) {
    error(TString::Format("storeShapeVariation(...): Failed to set histogram '%s' in path '%s'", histo->GetName(),path.Data()));
    return "";
  }
  return TQFolder::concatPaths(this->sysPrefix, path, histo->GetName());
}

//__________________________________________________________________________________|___________

TString TQSystematicsManager::storeVarHisto(TH1 * histo, const TString& path) {
  // store a variation (input) histogram  
  if (!histo) return "";
  if (!this->repository()->addObject(histo, TQFolder::concatPaths(this->varPrefix,path) + "+")) {
    error(TString::Format("storeHistogramVariation(...): Failed to set histogram '%s' in path '%s'", histo->GetName(), path.Data()));
    return "";
  }
  return TQFolder::concatPaths(this->varPrefix, path, histo->GetName());
}

//__________________________________________________________________________________|___________

void TQSystematicsManager::magnifyShapeVariation(TH1*& shape_Var,  const TH1 * h_Nominal, Double_t magn) {
  // magnify a variation by an exponent
  TH1 * h_ratio = TQHistogramUtils::copyHistogram(shape_Var);
  h_ratio->Divide(h_Nominal);
  TQHistogramUtils::power(h_ratio, magn);
  h_ratio->Multiply(h_Nominal);
  delete shape_Var;
  shape_Var = h_ratio;
  TQHistogramUtils::normalize(shape_Var, h_Nominal);
}



//__________________________________________________________________________________|___________

void TQSystematicsManager::info(TString message) {
  // print an info message
  std::cout << "SFramework/TQSystematicsManager: " << message.Data() << std::endl;
}


//__________________________________________________________________________________|___________

void TQSystematicsManager::error(TString message) {
  // print a red error message
  info(TQStringUtils::makeBoldRed(TString("ERROR: ") + message));
}


//__________________________________________________________________________________|___________

void TQSystematicsManager::warn(TString message) {
  // print a yellow warning message
  info(TQStringUtils::makeBoldYellow(TString("WARNING: ") + message));
}

//__________________________________________________________________________________|___________

TQSystematicsManager::HistogramVariationComputer::HistogramVariationComputer(const TH1* base) :
  fBaseline(base) {}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::HistogramVariationComputer::computeVariation(TObjArray* vars, TString mode) {
  mode.ReplaceAll(" ","");
  mode.ToLower();
  bool deltaComputed = false;
  if (mode.EqualTo("single")) deltaComputed = computeSingle(vars);
  else if (mode.EqualTo("difference")) deltaComputed = computeDifference(vars);
  else if (mode.EqualTo("minimum")) deltaComputed = computeMinimum(vars);
  else if (mode.EqualTo("maximum")) deltaComputed = computeMaximum(vars);
  else if (mode.EqualTo("envelope")) deltaComputed = computeEnvelope(vars);
  else if (mode.EqualTo("stddev")) deltaComputed = computeStdDev(vars);
  else if (mode.EqualTo("hessian")) deltaComputed = computeHessian(vars);
  // if computation failed, set delta to be 0.0,
  // i.e. return exactly nominal
  if (!deltaComputed) {
    if(!fBaseline){
      ERRORclass("cannot compute variation without baseline!");
      return false;
    }
    fDelta = TQHistogramUtils::copyHistogram(fBaseline);
    TQHistogramUtils::addHistogramWithoutError(fDelta,fBaseline,-1);
    fRelativeDelta = TQHistogramUtils::copyHistogram(fBaseline);
    TQHistogramUtils::divideHistogramWithoutError(fRelativeDelta,fBaseline);
  }
  return deltaComputed;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::HistogramVariationComputer::computeSingle(TObjArray* vars) {
  if (vars->GetEntries()!=1) {
    ERRORclass("Exactly 1 variation must be provided");
    return false;
  }
  if(!fBaseline){
    ERRORclass("cannot compute Single baseline!");
    return false;
  }    
  TH1* var = dynamic_cast<TH1*>(vars->At(0));
  // make deltas

  fDelta = TQHistogramUtils::copyHistogram(var);
  TQHistogramUtils::addHistogramWithoutError(fDelta,fBaseline,-1.0);
  fRelativeDelta = TQHistogramUtils::copyHistogram(fDelta);
  TQHistogramUtils::divideHistogramWithoutError(fRelativeDelta,fBaseline);
  // TODO: Might be useful here to add a flag whether we want to propagate errors or not 
  // TQHistogramUtils::addHistogram(fDelta,fBaseline,-1.0);
  // fRelativeDelta->Divide(fBaseline);
  return true;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::HistogramVariationComputer::computeDifference(TObjArray* vars) {
  if (vars->GetEntries()!=2) {
    ERRORclass("Exactly 2 variations must be provided");
    return false;
  }
  TH1* varA = dynamic_cast<TH1*>(vars->At(0));
  TH1* varB = dynamic_cast<TH1*>(vars->At(1));
  // make deltas
  fDelta = TQHistogramUtils::copyHistogram(varA);
  TQHistogramUtils::addHistogram(fDelta,varB,-1.0);
  fRelativeDelta = TQHistogramUtils::copyHistogram(fDelta);
  TQHistogramUtils::divideHistogramWithoutError(fRelativeDelta,fBaseline);
  return true;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::HistogramVariationComputer::computeMinimum(TObjArray* vars) {
  if(!fBaseline){
    ERRORclass("cannot compute Minimum without baseline!");
    return false;
  }
  fDelta = TQHistogramUtils::copyHistogram(fBaseline);
  int numBins = TQHistogramUtils::getNbinsGlobal(fBaseline);
  int maxBin = numBins+1;
  int numVars = vars->GetEntries();
  fDelta->Reset();
  for (int ibin=0 ; ibin<maxBin ; ++ibin) {
    double binDelta = fDelta->GetBinContent(ibin);
    double binError = fDelta->GetBinError(ibin);
    for (int ivar=0 ; ivar<numVars ; ++ivar) {
      TH1* thisVar = TQHistogramUtils::copyHistogram(dynamic_cast<TH1*>(vars->At(ivar)));
      double thisBinDelta = thisVar->GetBinContent(ibin)-fBaseline->GetBinContent(ibin);
      if (thisBinDelta < binDelta) {
        binDelta = thisBinDelta;
        binError = thisVar->GetBinError(ibin);
      }
      delete thisVar;
    }
    fDelta->SetBinContent(ibin,binDelta);
    fDelta->SetBinError(ibin,binError);
  }
  fRelativeDelta = TQHistogramUtils::copyHistogram(fDelta);
  TQHistogramUtils::divideHistogramWithoutError(fRelativeDelta,fBaseline);
  return true;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::HistogramVariationComputer::computeMaximum(TObjArray* vars) {
  if(!fBaseline){
    ERRORclass("cannot compute Maximum without baseline!");
    return false;
  }
  fDelta = TQHistogramUtils::copyHistogram(fBaseline);
  int numBins = TQHistogramUtils::getNbinsGlobal(fBaseline);
  int maxBin = numBins+1;
  int numVars = vars->GetEntries();
  fDelta->Reset();
  for (int ibin=0 ; ibin<maxBin ; ++ibin) {
    double binDelta = fDelta->GetBinContent(ibin);
    double binError = fDelta->GetBinError(ibin);
    for (int ivar=0 ; ivar<numVars ; ++ivar) {
      TH1* thisVar = TQHistogramUtils::copyHistogram(dynamic_cast<TH1*>(vars->At(ivar)));
      double thisBinDelta = thisVar->GetBinContent(ibin)-fBaseline->GetBinContent(ibin);
      if (thisBinDelta > binDelta) {
        binDelta = thisBinDelta;
        binError = thisVar->GetBinError(ibin);
      }
      delete thisVar;
    }
    fDelta->SetBinContent(ibin,binDelta);
    fDelta->SetBinError(ibin,binError);
  }
  fRelativeDelta = TQHistogramUtils::copyHistogram(fDelta);
  TQHistogramUtils::divideHistogramWithoutError(fRelativeDelta,fBaseline);
  return true;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::HistogramVariationComputer::computeEnvelope(TObjArray* vars) {
  if(!fBaseline){
    ERRORclass("cannot compute Envelope without baseline!");
    return false;
  }
  fDelta = TQHistogramUtils::copyHistogram(fBaseline);
  int numBins = TQHistogramUtils::getNbinsGlobal(fBaseline);
  int maxBin = numBins+1;
  int numVars = vars->GetEntries();
  fDelta->Reset();
  for (int ibin=0 ; ibin<maxBin ; ++ibin) {
    double binDelta = fDelta->GetBinContent(ibin);
    double binError = fDelta->GetBinError(ibin);
    for (int ivar=0 ; ivar<numVars ; ++ivar) {
      TH1* thisVar = TQHistogramUtils::copyHistogram(dynamic_cast<TH1*>(vars->At(ivar)));
      double thisBinDelta = fabs(thisVar->GetBinContent(ibin)-fBaseline->GetBinContent(ibin));
      if (thisBinDelta > binDelta) {
        binDelta = thisBinDelta;
        binError = thisVar->GetBinError(ibin);
      }
      delete thisVar;
    }
    fDelta->SetBinContent(ibin,binDelta);
    fDelta->SetBinError(ibin,binError);
  }
  fRelativeDelta = TQHistogramUtils::copyHistogram(fDelta);
  TQHistogramUtils::divideHistogramWithoutError(fRelativeDelta,fBaseline);
  return true;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::HistogramVariationComputer::computeStdDev(TObjArray* vars) {
  // +/- delta to apply to nominal
  if(!fBaseline){
    ERRORclass("cannot compute StdDev without baseline!");
    return false;
  }  
  fDelta = TQHistogramUtils::copyHistogram(fBaseline);
  fDelta->Reset();
  TH1* iDelta = TQHistogramUtils::copyHistogram(fBaseline);
  iDelta->Reset();
  int numVars = vars->GetEntries();
  for (int ivar=0 ; ivar<numVars ; ++ivar) {
    iDelta->Reset();
    TQHistogramUtils::addHistogram(iDelta,dynamic_cast<TH1*>(vars->At(ivar)));
    TQHistogramUtils::addHistogram(iDelta,fBaseline,-1.0);
    TQHistogramUtils::addHistogramInQuadrature(fDelta,iDelta);
  }
  TQHistogramUtils::scaleHistogram(fDelta,1.0/TMath::Sqrt(numVars));
  fRelativeDelta = TQHistogramUtils::copyHistogram(fDelta);
  TQHistogramUtils::divideHistogramWithoutError(fRelativeDelta,fBaseline);
  return true;
}

//__________________________________________________________________________________|___________

bool TQSystematicsManager::HistogramVariationComputer::computeHessian(TObjArray* vars) {
  if(!fBaseline){
    ERRORclass("cannot compute Hessian without baseline!");
    return false;
  }
  fDelta = TQHistogramUtils::copyHistogram(fBaseline);
  fDelta->Reset();
  TH1* iDelta = TQHistogramUtils::copyHistogram(fBaseline);
  iDelta->Reset();
  int numVars = vars->GetEntries();
  for (int ivar=0 ; ivar<numVars ; ++ivar) {
    iDelta->Reset();
    TQHistogramUtils::addHistogram(iDelta,dynamic_cast<TH1*>(vars->At(ivar)));
    TQHistogramUtils::addHistogram(iDelta,fBaseline,-1.0);
    TQHistogramUtils::addHistogramInQuadrature(fDelta,iDelta);
  }
  fRelativeDelta = TQHistogramUtils::copyHistogram(fDelta);
  TQHistogramUtils::divideHistogramWithoutError(fRelativeDelta,fBaseline);
  return true;
}

//__________________________________________________________________________________|___________

void TQSystematicsManager::HistogramVariationComputer::useStatError(TH1* error, bool relative) {
  TH1* tmpDelta = TQHistogramUtils::copyHistogram(error);
  if (relative) {
    // create unity histogram keeping relative errors
    TQHistogramUtils::divideHistogramWithoutError(tmpDelta,tmpDelta);
    // scale to get (1.0*rdel) +/- (rerr*rdel)
    TQHistogramUtils::multiplyHistogramWithoutError(tmpDelta,fRelativeDelta);
    // set result to be the newly-computed histogram taking errors
    fRelativeDelta = tmpDelta;
  } else {
    // create unity histogram keeping absolute errors
    TQHistogramUtils::addHistogramWithoutError(tmpDelta,tmpDelta,-1);
    // add to get (0.0 + delta) +/- error
    TQHistogramUtils::addHistogramWithoutError(tmpDelta,fDelta,+1);
    // set to be the new result
    fDelta = tmpDelta;
  }
}

//__________________________________________________________________________________|___________

TH1* TQSystematicsManager::HistogramVariationComputer::applyVariation(const TH1* nom, double c, bool relative) {
  if(!fBaseline){
    ERRORclass("cannot apply variation without baseline!");
    return NULL;
  }  
  if (!nom) nom = fBaseline;
  TH1* absDelta;
  if (relative) {
    absDelta = TQHistogramUtils::copyHistogram(fRelativeDelta);
    TQHistogramUtils::scaleHistogram(absDelta,c);    
    TQHistogramUtils::multiplyHistogramWithoutError(absDelta,nom);
    TQHistogramUtils::addHistogramWithoutError(absDelta,nom);
    // TODO: Might be useful to add a flag whether we want to propagate errors or not
    // absDelta->Multiply(nom);
    //TQHistogramUtils::addHistogram(absDelta,nom);
  } else {
    absDelta = TQHistogramUtils::copyHistogram(fDelta);
    TQHistogramUtils::scaleHistogram(absDelta,c);
    TQHistogramUtils::addHistogramWithoutError(absDelta,nom);
  }
  return absDelta;
}

//__________________________________________________________________________________|___________

void TQSystematicsManager::setRepository(TQFolder* histos){
  this->histos = histos;
}

//__________________________________________________________________________________|___________

TQFolder* TQSystematicsManager::repository() {
  if(this->histos){
    return this->histos;
  } else {
    throw std::runtime_error("TQSystematicsManager: used without initialized repository!");
  }
}
