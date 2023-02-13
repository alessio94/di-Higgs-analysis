#inFile = open("hh4b-02-03-03_MiniNTuple.txt","r")

inputData = [
    ["c10_M300",250,900,
     "mc15_13TeV.301488.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M300.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M300",250,900,
     "mc15_13TeV.301508.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M300.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M400",250,900,
     "mc15_13TeV.301489.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M400.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M400",250,900,
     "mc15_13TeV.301509.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M400.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M500", 300,900,
     "mc15_13TeV.301490.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M500.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M500", 300, 900,
     "mc15_13TeV.301510.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M500.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M600",350, 1100,
     "mc15_13TeV.301491.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M600.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M600",350, 1100,
     "mc15_13TeV.301511.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M600.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M700",350, 1300,
     "mc15_13TeV.301492.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M700.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M700",350, 1300,
     "mc15_13TeV.301512.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M700.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M800",400, 1500,
     "mc15_13TeV.301493.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M800.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M800",400, 1500,
     "mc15_13TeV.301513.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M800.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M900",500, 1600,
     "mc15_13TeV.301494.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M900.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M900",500, 1600,
     "mc15_13TeV.301514.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M900.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M1000",500, 1700,
     "mc15_13TeV.301495.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M1000.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M1000",500, 1700,
     "mc15_13TeV.301515.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M1000.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M1100",600, 1900,
     "mc15_13TeV.301496.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M1100.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M1100",600, 1900,
     "mc15_13TeV.301516.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M1100.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M1200",600, 2000,
     "mc15_13TeV.301497.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M1200.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M1200",600, 2000,
     "mc15_13TeV.301517.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M1200.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M1300",600, 2200,
     "mc15_13TeV.301498.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M1300.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M1300",600, 2200,
     "mc15_13TeV.301518.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M1300.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M1400",700, 2300,
     "mc15_13TeV.301499.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M1400.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M1400",700, 2300,
     "mc15_13TeV.301519.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M1400.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M1500",700, 2500,
     "mc15_13TeV.301500.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M1500.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M1500",700, 2500,
     "mc15_13TeV.301520.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M1500.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M1600",700, 3000,
     "mc15_13TeV.301501.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M1600.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M1600",700, 3000,
     "mc15_13TeV.301521.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M1600.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M1800",700, 3500,
     "mc15_13TeV.301502.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M1800.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M1800",700, 3500,
     "mc15_13TeV.301522.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M1800.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M2000",800, 3500,
     "mc15_13TeV.301503.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M2000.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M2000",800, 3500,
     "mc15_13TeV.301523.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M2000.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M2250",700, 4000,
     "mc15_13TeV.301504.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M2250.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M2250",700, 4000,
     "mc15_13TeV.301524.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M2250.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    
    ["c10_M2500",700, 4000,
     "mc15_13TeV.301505.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M2500.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M2500",700, 4000,
     "mc15_13TeV.301525.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M2500.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["c10_M3000",700, 5000,
     "mc15_13TeV.301507.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M3000.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],
    ["c20_M3000",700, 5000,
     "mc15_13TeV.301527.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M3000.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2952"
     ],

    ["hh_4b", 250,930,
     "mc15_13TeV.342619.aMcAtNloHerwigppEvtGen_UEEE5_CTEQ6L1_CT10ME_hh_4b.merge.DAOD_EXOT8.e4419_s2608_r7772_r7676_p2952"
     ],
    ]


for thisInputData in inputData:
    
    outName = thisInputData[0]
    massMin = float(thisInputData[1])
    massMax = float(thisInputData[2])
    dsName  = thisInputData[3]

    cmd = "python scripts/procDAOD.py"
    cmd += " -i "+dsName
    cmd += " -o ./data/DAOD"+outName+".root"
    cmd += " --massMin "+str(massMin)
    cmd += " --massMax "+str(massMax)
    cmd += " &"
    print cmd


#python hhTruthWeightTools/scripts/procXhhMiniNtuple.py -i group.phys-exotics.mc15_13TeV.301490.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M500.hh4b-02-03-04_MiniNTuple.root  -o c10_M500.root  --massMin 200 --massMax 800 &
#python hhTruthWeightTools/scripts/procXhhMiniNtuple.py -i group.phys-exotics.mc15_13TeV.301510.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M500.hh4b-02-03-04_MiniNTuple.root   -o c20_M500.root  --massMin 200 --massMax 800 &

#python scripts/procHists.py -t data/c10_M500.root -r data/c20_M500.root -o data/RSG_m500_c20_to_c10.root
