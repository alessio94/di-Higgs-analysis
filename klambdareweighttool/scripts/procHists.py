import optparse
parser = optparse.OptionParser()
parser.add_option('-r', '--refernceFileName',     dest="refFileName",         default=None, help="Run in loop mode")
parser.add_option('--refXS',     dest="refXS",  default=None, help="Run in loop mode")
parser.add_option('-t', '--targetFileName',       dest="tarFileName",         default=None, help="Run in loop mode")
parser.add_option('--tarXS',     dest="tarXS",  default=None, help="Run in loop mode")
parser.add_option('-o', '--outFileName',          dest="outfileName",        default=None, help="Run in loop mode")
o, a = parser.parse_args()


import ROOT

numFile = ROOT.TFile(o.tarFileName, "READ")
denFile = ROOT.TFile(o.refFileName, "READ")

h_num = numFile.Get("mhh")
if o.refXS: 
    ref_events = h_num.GetEntries()
    h_num.Scale(float(o.refXS)/ref_events)

h_den = denFile.Get("mhh")
if o.tarXS:
    tar_events = h_den.GetEntries()
    h_den.Scale(float(o.tarXS)/tar_events)

h_ratio = h_num.Clone("mhh_kfactor")
#h_ratio.Sumw2()
h_ratio.Divide(h_den)

outFile = ROOT.TFile(o.outfileName,"RECREATE")
outFile.cd()
h_ratio.Write()
outFile.Close()

