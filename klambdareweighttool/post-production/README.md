# Scripts to re-weight to the different couplings after ntuple production

## Installation
No installation needed, simply:

```
make
```

## Requirements

To use this scripts you must have haved the truth HH invariant mass (m_hh) in your ntuple as a branch (preferably in GeV)


e.g. Ideally to keep the compatibility with the DAOD level tool you should save the m_hh information as:

const xAOD::TruthParticle* h1 = // Get one of the Higgs from the truth record ;
const xAOD::TruthParticle* h2 = // Get otherHiggs from the truth record ;
float mhh = (h1->p4()+h2->p4()).M();


## Weights from kLambdaReweightTool

Modify ReWeight_klambda.py to add:
1) information about your ntuple (current example compatible with bbyy)
2) information on what kappa lambda values you want to re-weight to
```
python ReWeight_klambda.py
```

## Weights from HEFT 

Modify ReWeight_HEFT.py to add: 
1) information about your ntuple (current example compatible with bbyy)
2) information on what coupling values you want to re-weight to

```
python ReWeight_HEFT.py
```

Furthermore, a script to use the HEFT weights to simulaneously re-weight to the different kappa lamda values or some benchmark suggested by theorists are available:

```
./klambda_HEFT_reweight ntuple.root treename mhh_branch

./BM_HEFT_reweight ntuple.root treename mhh_branch

```