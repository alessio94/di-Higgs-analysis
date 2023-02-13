

for mPt in [300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2250,2500,3000]:
    cmd = "python "
    cmd += "scripts/procHists.py "
    cmd += "-t data/c10_M"+str(mPt)+".root "
    cmd += "-r data/c20_M"+str(mPt)+".root "
    cmd += "-o data/RSG_m"+str(mPt)+"_c20_to_c10.root "
    cmd += "& "
    print cmd
