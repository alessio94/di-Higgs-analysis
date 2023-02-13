python hhTruthWeightTools/scripts/procXhhMiniNtuple.py -i group.phys-exotics.mc15_13TeV.301490.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M500.hh4b-02-03-04_MiniNTuple.root  -o c10_M500.root  --massMin 200 --massMax 800 &
python hhTruthWeightTools/scripts/procXhhMiniNtuple.py -i group.phys-exotics.mc15_13TeV.301510.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M500.hh4b-02-03-04_MiniNTuple.root   -o c20_M500.root  --massMin 200 --massMax 800 &



#python scripts/procHists.py -t data/c10_M500.root -r data/c20_M500.root -o data/RSG_m500_c20_to_c10.root