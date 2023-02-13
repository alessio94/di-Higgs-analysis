import os

def rucioMakeList(dsname,tempfname,grepName = "MWT2_UC_LOCALGROUPDISK"):
    l = os.popen("rucio list-file-replicas --protocol root "+dsname+" | grep "+grepName)
    print "rucio list-file-replicas --protocol root "+dsname+" "
    f = open(tempfname,"w")

    processedFiles = []

    for line in l:
        words = line.split()
        globalName = words[-2]
        #line = line.split("MWT2_UC_LOCALGROUPDISK: ")[-1]
        #line = line.split()[0]
        localFileName = globalName.split("/")[-1]


        if localFileName not in processedFiles:
            processedFiles.append(localFileName)
            f.write(globalName+"\n")
            print globalName
            print "\t",localFileName
        
        #print(line)
    f.close()
