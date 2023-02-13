## TTbarNNLOReweighter package

## About
The package provides corrections to kinematics of *top pT*, *top Y*, *ttbar Y* and *ttbar M* based on NNLO QCD + NLO EW differential predictions taken from: JHEP 10 (2017) 186, Top-pair production at the LHC through NNLO QCD and NLO EW

It also provides kinematics corrections to *dPhi(ll)* azimuthal difference of the two leptons based on NNLO QCD differential prediction taken from: arXiv:1901.05407, Higher order corrections to spin correlations in top quark pair production at the LHC.

Supported ATLAS MC samples for the NNLO QCD + NLO EW correction are:

PowhegPythia8, PowhegHerwig7.0, aMC@NLOPythia8, PowhegPythia8 (hdamp=3mt), Sherpa 2.2.1., PowhegHerwig7.1, aMC@NLO+Herwig7.1 

Supported ATLAS MC samples for the NNLO QCD correction to dPhi(ll): PowhegPythia8.

Corrections can be applied to any filtered sample produced from those generator+shower combinations. Can be applied to any MC16 campaign.

Latest presentation about the package: 

[Top Focus Group, 04.03.2019](https://indico.cern.ch/event/799800/contributions/3338026/attachments/1805099/2945902/NNLO_QCD_NLO_EW_ttbar_LSerkin.pdf)

[Top Cross-Section, 17.04.2019](https://indico.cern.ch/event/813936/contributions/3395466/attachments/1831592/2999881/NNLO_QCD_NLO_EW_correction.pdf)

[Top Working Group Workshop 2019](https://indico.cern.ch/event/795477/contributions/3413743/)

Analyses currently using the correction [not full list, **please let us know** if your analysis is using it and is not listed below]

[ttbar inclusive xs l+jets 13 TeV](https://glance.cern.ch/atlas/analysis/confnotes/details.php?id=11947)

[tt resonance 1+2 lepton](https://glance.cern.ch/atlas/analysis/analyses/details.php?id=1705)

[ST tW Run 2](https://glance.cern.ch/atlas/analysis/analyses/details.php?id=331)

[top width](https://glance.cern.ch/atlas/analysis/confnotes/details?id=11887)

[tt xs dilepton differential](https://glance.cern.ch/atlas/analysis/analyses/details?id=549)

## Top quark information
In order to use the correction, one needs to have the information of the top quarks already stored for each of the samples that are going to be used.
Truth particles at parton level after radiation (the last replica in the MC record for the top quarks) are used.
That means that no default 'status' of particles is used as those are different between showers.
Just as an example: in case of Pythia 8 showering, technically the last copy of the top particle has status 62.
For top pT correction, use top quark information (not anti-top), for simplicity.
For leptons from W bosons, take lepton from W decay coming from both (anti)top quarks (pdgid=11,13,15), should be status 23 in Pythia8 shower.

## Setup default top pT correction for Powheg+Pythia8 DSIDs 410470/471
First, initialise the tool as:

```
include "TTbarNNLOReweighter/TTbarNNLOReweighter.h"
[...]
// initialise
TTbarNNLOReweighter *myReweighter = new TTbarNNLOReweighter( MCchannelNumber );  // e.g. 410470, ...
myReweighter->Init();
[...]
```

Then, inside the event loop do:
```
weight *= myReweighter->GetTopPt_Powheg_Pythia8_Nominal ( truth_top_pt_inGeV );
```

The tool uses the values in GeV (not MeV!).

## Additional corrections available:
```
// alternative ttbar samples
  GetTopPt_Sherpa(float truth_top_pt); // derived from DSIDs 410424-427
  GetTopPt_Powheg_Herwig7(float truth_top_pt); // derived from DSIDs 410557/558/559
  GetTopPt_aMCNLO_Pythia8(float truth_top_pt); // derived from DSIDs 410464/465/466
  GetTopPt_Powheg_Pythia8_Hdamp(float truth_top_pt); // derived from DSIDs 410480/481/482

// alternative variables
   float GetTTbarM_Powheg_Pythia8_Nominal(float truth_TTbar_M); // Mass of the ttbar system
   float GetTTbarY_Powheg_Pythia8_Nominal(float truth_TTbar_Y); // Rapidity of the ttbar system
   float GetTopY_Powheg_Pythia8_Nominal(float truth_top_Y); // Rapidity of the top quark

// systematics on the default top pT correction for Powheg+Pythia8
   float GetTopPt_PDFvar_Powheg_Pythia8_Nominal(float truth_top_pt); // LUXqed PDF
   float GetTopPt_ScaleMin_Powheg_Pythia8_Nominal(float truth_top_pt); // min values of the scales
   float GetTopPt_ScaleMax_Powheg_Pythia8_Nominal(float truth_top_pt); // max values of the scales

// azimuthal difference of the two leptons
   float GetDeltaPhi(float truth_dPhi_ll); 

// Powheg+Pythia8 with different top masses settings (172 GeV and 173 GeV)
   float GetTopPt_Powheg_Pythia8_TopMass172GeV(float truth_top_pt);
   float GetTopPt_Powheg_Pythia8_TopMass173GeV(float truth_top_pt);

// new alternative samples: Powheg+Herwig713 and aMCNLO+Herwig713
   float GetTopPt_Powheg_Herwig713(float truth_top_pt); // derived from DSIDs 411233/411234
   float GetTopPt_aMCNLO_Herwig713(float truth_top_pt); // derived from DSIDs 412116/412117

// average top pT and antitop pT correction for Powheg+Pythia8
   float GetTopPt_Powheg_Pythia8_Nominal_Average(float truth_top_pt, float truth_antitop_pt);

```

## Text snippet for your note/paper:
Previous studies have seen improved agreement between data and prediction in tt events, particularly for the top-quark pT distribution, 
when comparing with NNLO calculations [cite me](https://arxiv.org/abs/1511.04716). Lately, top-quark pair differential calculations at NNLO QCD accuracy 
and including EW corrections became available [cite me](https://arxiv.org/abs/1705.04105). 
<*If applied by default*> Hence, a small improvement to the modelling are incorporated by correcting all tt samples to match their top quark pT distribution
to that predicted at NNLO in QCD and NLO EW accuracy.
<*If applied as uncertainty*> Hence, an uncertainty in the modeling of the top-quark pT distribution is evaluated by taking the full difference 
between applying and not applying the correction to match the predictions at NNLO in QCD and NLO EW accuracy.


## Question/Answers:
Q: How was this derived? 
A: NNLO QCD ( + NLO EW) differential prediction for several distributions were compared to our ATLAS MC predictions, and a correction based on that was derived at MC level

Q: Why my DSID of the sample is not there? 
A: Probably because we have not derived it yet. Feel free to ask if you need it.

Q: Why I see some normalisation change in my variables after applying the correction?
A: The correction is derived comparing normalised distributions of parton kinematics before any selection (full phase-space) between Powheg (and other ttbar generators) and the predicitons by the authors of the NNLO QCD + NLO EW calculation, and therefore fixes only the shapes. Normally, ttbar cross-section is normalised to NNLO in QCD including NNLL terms, and these cross-section calculations are different by <1%, however numerical precision and rebinning done by us due to our ATLAS-limited MC stats play their role, and due to a different phase-space of each analysis, it brings minor (<1.5%) normalisation differences.

Q: How do I assses a systematic uncertainty on this correction? 
A: In order to add the correction as a systematic uncertainty, just compare the nominal non-reweighted to nominal reweighted samples, and use that as a new nuisance in your fit. To avoid double counting, alternative tt distributions used to estimate uncertainties on the modelling have to be also reweighted and only then compared to the reweighted distribution obtained from the nominal prediction.

Q: Are we getting OTP or credit for maintaining and costantly updating this package?
A: Nop.. but from time to time we happen to find that you used it and it made your data/MC better or helped in answering approval/EB questions, and that makes us happy;)


## Team
Leonid Serkin, Jacopo Margo, Mohammed Faraj, Giancarlo Panizzo, Michele Pinamonti

Questions: lserkin@cern.ch

