import os

# Specifications for bbyy samples

mhh_branch = "HGamTruthEventInfoAuxDyn.m_hh"
tree = "CollectionTree"
path = "/eos/user/l/lapereir/HH/Signals/H26/Modified/"
samples = [path+"mc16e.PowhegH7_HHbbyy_cHHH01d0.root"] #Specify list of samples to run
scale = "0.001" # mhh must be given in GeV! Hence you need to specify what scale to use to convert mhh_branch to GeV.

# What HEFT couplings do you want to re-weight to?

couplings = ["1.3 -1.6 0 0.3 -3.3", "2 1 0 0 0,","6 1 0 0 0" ] # Specify values for kl, kt, cg, c2g and c2. e.g for SM: "1 1 0 0 0" WARNING: Use spaces to separate the coupling values!!!

for sample in samples:
    for c in couplings:
        print("./HEFT_reweight "+sample+" "+tree+" "+mhh_branch+" "+scale+" "+c)
        os.system("./HEFT_reweight "+sample+" "+tree+" "+mhh_branch+" "+scale+" "+c)

