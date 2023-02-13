import ROOT
def histFromFile(name,inputDataFileName,nBins=34,min=250.,max=930.):


    hmhh = ROOT.TH1F(name,"mhh", nBins, min, max)

    data = getData(inputDataFileName)

    for d in data:
        mass    = d[0]
        mhh     = d[1]
        mhhErr  = d[2]
        
        hmhh.SetBinContent(hmhh.FindBin(mass),mhh)
        hmhh.SetBinError  (hmhh.FindBin(mass),mhhErr)
        
    return hmhh


def getData(inputDataFileName):

    inFile = open(inputDataFileName,"r")
    data = []


    for line in inFile:
        words = line.split()
        if not len(words): continue
        if line[0] == "#": continue

        mass    = float(words[0])
        mhh     = float(words[1])
        mhhErr  = float(words[2])
        data.append((mass,mhh,mhhErr))
    inFile.close()
    return data


def makeRatio(target,source):
    if not len(target) == len(source):
        print "ERROR",len(target),len(source)
        return

    kFactors = {}

    for itr, tData in enumerate(target):

        print "mass",tData[0],"\t",tData[1]/source[itr][1]
        kFactors[tData[0]] = tData[1]/source[itr][1]
        

    return kFactors


def getKFactors(target,source, binHalfWidth=10):
    if not len(target) == len(source):
        print "ERROR",len(target),len(source)
        return

    binUpperEdges = []
    kFactors      = []

    for itr, tData in enumerate(target):

        print "mass",tData[0]+10,"\t",tData[1]/source[itr][1]
        binUpperEdges.append(tData[0]+binHalfWidth)
        kFactors.append(tData[1]/source[itr][1])
        
    return binUpperEdges,kFactors


def gethhWeight(mhh, binUpperEdges,kFactors):

    for iBin, upperEdge in enumerate(binUpperEdges):
        if mhh < upperEdge:
            return kFactors[iBin]
        #if abs(mhh - binCenter) < binWidth:
        #    return kfactorDict[binCenter]

    return kFactors[-1]
