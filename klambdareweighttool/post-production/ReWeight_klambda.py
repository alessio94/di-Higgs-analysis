import os
from kl_dict import Dict

# Specifications for bbyy samples                                                                                     
mhh_branch = "HGamTruthEventInfoAuxDyn.m_hh"
tree = "CollectionTree"
path = "/eos/user/l/lapereir/HH/Signals/H26/diphotonsel/"
samples = [path+"mc16a.PowhegH7_HHbbyy_cHHH01d0.root",path+"mc16d.PowhegH7_HHbbyy_cHHH01d0.root",path+"mc16e.PowhegH7_HHbbyy_cHHH01d0.root"]

scale = "0.001" # mhh must be given in GeV! Hence you need to specify what scale to use to convert mhh_branch to GeV. 

#Branching ratio info can be found at https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR
BR_H_bb = 0.5809
BR_H_yy = 0.002270

BR = str(BR_H_bb*BR_H_yy) #for HH->bbyy

#Cross section correction: e.g. for bbyy we need to use XS(mh=125.09)/XS(mh=125) but if your analysis uses XS(mh=125) you should probably set it to 1.
XS_correction = str(31.02/31.0358) # XS(mh=125.09)/XS(mh=125 -> corrected to match MxAOD XS) for bbyy

# What kappa lambda do you want to re-weight to and with which binning?       
 
kl_values = ["0p0","0p2","0p4","0p6","0p8","1p2","1p4","1p6","1p8","2p0","2p2","2p4","2p6","2p8","3p0","3p2","3p4","3p6","3p8","4p0","4p2","4p4","4p6","4p8","5p0","5p2","5p4","5p6","5p8","6p0","6p2","6p4","6p6","6p8","7p0","7p2","7p4","7p6","7p8","8p0","8p2","8p4","8p6","8p8","9p0","9p2","9p4","9p6","9p8","10p0","10p2","10p4","10p6","10p8","11p0","11p2","11p4","11p6","11p8","12p0","12p2","12p4","12p6","12p8","13p0","n0p2","n0p4","n0p6","n0p8","n1p0","n1p2","n1p4","n1p6","n1p8","n2p0","n2p2","n2p4","n2p6","n2p8","n3p0","n3p2","n3p4","n3p6","n3p8","n4p0","n4p2","n4p4","n4p6","n4p8","n5p0","n5p2","n5p4","n5p6","n5p8","n6p0","n6p2","n6p4","n6p6","n6p8","n7p0","n7p2","n7p4","n7p6","n7p8","n8p0","n8p2","n8p4","n8p6","n8p8","n9p0","n9p2","n9p4","n9p6","n9p8","n10p0","n10p2","n10p4","n10p6","n10p8","n11p0","n11p2","n11p4","n11p6","n11p8","n12p0","n12p2","n12p4","n12p6","n12p8","n13p0"]
 #Speciy the kappa lambda values you want to reweight to. e.g. 6p0 -> kl= 6.0, n3p2 -> kl = -3.2 etc the SM option "1p0" does not exist! So don't try to re-weight to SM.

bin_width = ["10"] # weights have been saved for different binning options: 5, 10 or 20 GeV.


for sample in samples:
    print("Sample = ", sample)
    for kl in kl_values:
        kl_value = Dict[kl]
        for width in bin_width:
            print("./klambda_reweight "+sample+" "+tree+" "+mhh_branch+" "+kl+" "+width+" "+scale+" "+BR+" "+kl_value+" "+XS_correction)
            os.system("./klambda_reweight "+sample+" "+tree+" "+mhh_branch+" "+kl+" "+width+" "+scale+" "+BR+" "+kl_value+" "+XS_correction)
            
