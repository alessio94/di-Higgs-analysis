#!/usr/bin/env python

import os
import shutil
import unittest
import subprocess
import ROOT

# storage location
sl = "TQPathManagerOutputTest"
# storage location plus slash
slp = sl+"/"
# storage location absolute
pwd = ROOT.gSystem.pwd()
pwdp = pwd+"/"
sla = pwd+"/"+sl
# storage location absolute plus slash
slap = sla+"/"

CAFOutputDir_before = ""
CAFAnalysisShare_before = ""

class TQPathManagerTest(unittest.TestCase):

    def setUp(self):
        # create directories and files
        if os.path.exists(sla) and os.path.isdir(sla):
            shutil.rmtree(sla)

        listOfDirs  = ["dir1", "dir2", "dir3", sl]
        listOfFiles = ["dir1/testFile", "dir2/testFile", "dir3/anotherTestFile", slp+"testFile", slp+"testFile1", "testFile"]

        listOfDirs  = [slap+x for x in listOfDirs]
        listOfFiles = [slap+x for x in listOfFiles]

        for dir in listOfDirs:
            os.makedirs(dir)
        for file in listOfFiles:
            f = open(file,"w")
            f.close()

        os.environ["TEST"] = slap+"dir1:"+sla
        if "CAFOUTPUTDIR" in os.environ:
            CAFOutputDir_before = os.environ["CAFOUTPUTDIR"]
            del os.environ["CAFOUTPUTDIR"]
        else:
            CAFOutputDir_before = ""
        if "CAFANALYSISSHARE" in os.environ:
            CAFAnalysisShare_before = os.environ["CAFANALYSISSHARE"]
        else:
            CAFAnalysisShare_before = ""
        os.environ["CAFANALYSISSHARE"] = sla
        return

    def tearDown(self):
        # delete files
        if os.path.exists(sla) and os.path.isdir(sla):
            shutil.rmtree(sla)
        del os.environ["TEST"]
        if CAFOutputDir_before == "":
            if "CAFOUTPUTDIR" in os.environ:
                del os.environ["CAFOUTPUTDIR"]
        else:
            os.environ["CAFOUTPUTDIR"] = CAFOutputDir_before
        if CAFAnalysisShare_before == "":
            if "CAFANALYSISSHARE" in os.environ:
                del os.environ["CAFANALYSISSHARE"]
        else:
            os.environ["CAFANALYSISSHARE"] = CAFAnalysisShare_before
        return

    def getLocalDir(self):
        if "USER" in os.environ:
            return "/tmp/"+os.environ["USER"]+"/"
        else:
            return "/tmp/CAF_temporary/"

    def testStaticMethods(self):
        """
        Test the static methods related to reading environment variables.
        """
        from QFramework import TQPathManager

        os.environ["CAFOUTPUTDIR"] = sla

        # Absolute path. The second argument should not make any difference.
        out = TQPathManager.findFileFromList(slap+"testFile", "/some/path/that/is/ignored")
        self.assertEqual(out, slap+"testFile")

        # Absolute path. The second argument should not make any difference.
        out = TQPathManager.findFileFromList(slap+"testFile", slap+"dir1")
        self.assertEqual(out, slap+"testFile")

        # Absolute path. Even though testFile exists in dir1, the absolute path does not exist.
        out = TQPathManager.findFileFromList(slap+"notExistingFile", slap+"dir1")
        self.assertEqual(out, "")

        # Relative path found in list. Order matters. First file is found first.
        out = TQPathManager.findFileFromList("testFile", slap+"dir1:"+slap+"dir2")
        self.assertEqual(out, slap+"dir1/testFile")

        # Relative path found in list. Swapped dir1 and dir2. Nothing found in "random/path".
        out = TQPathManager.findFileFromList("testFile", slap+"random/path:"+slap+"dir2:"+slap+"dir1")
        self.assertEqual(out, slap+"dir2/testFile")

        # Searching for sub-paths works
        out = TQPathManager.findFileFromList("dir1/testFile", sla)
        self.assertEqual(out, slap+"dir1/testFile")

        # Relative path found locally (local match preferred over match in passed list)
        out = TQPathManager.findFileFromList(slp+"testFile", slap)
        self.assertEqual(out, slap+"testFile")

        # Execution dir is ignored (no local match)
        out = TQPathManager.findFileFromListWithoutExecDir(slp+"testFile", slap)
        self.assertEqual(out, slap+slp+"testFile")

        # Relative path found in environment variable
        out = TQPathManager.findFileFromEnvVar("testFile", "TEST")
        self.assertEqual(out, slap+"dir1/testFile")
        return


    def pathManagerLocalTestSub(self, useSingleton, targetDir, localDir, setLocalDir):
        from QFramework import TQPathManager

        if useSingleton:
            pm = TQPathManager.getPathManager()
            pm.setUseTemporaryStorage(True)
        else:
            pm = TQPathManager(True)

        if setLocalDir:
            out = pm.setLocalDirectory(localDir)
            self.assertTrue(out)
            self.assertTrue(os.path.exists(localDir) and os.path.isdir(sla))

        out = pm.getUseTemporaryStorage()
        self.assertTrue(out)

        # getTargetPath and getLocalPath should return different paths
        targetPath = pm.getTargetPath("targetPathName.root")
        self.assertEqual(targetPath, targetDir+"targetPathName.root")
        localPath  = pm.getLocalPath("localPathName.root")
        #todo use localDir here:
        self.assertEqual(localPath[0:5], "/tmp/")


        localPath2 = pm.getLocalPath("localPathName2.root")
        self.assertNotEqual(localPath, localPath2)
        localPath3 = pm.getLocalPath("localPathName3.root")
        localPath4 = pm.getLocalPath("localPathName4.root")
        localPath5 = pm.getLocalPath("subdir/localPathName5.root")

        # requesting the same file again, should return the same local file
        localPath2_2 = pm.getLocalPath("localPathName2.root")
        self.assertEqual(localPath2, localPath2_2)

        # create some files
        with open(targetPath, 'w') as f:
            f.write("text in targetPathName.root\n")
        with open(localPath, 'w') as f:
            f.write("text in localPathName.root\n")
        with open(localPath2, 'w') as f:
            f.write("text in localPathName2.root\n")
        with open(localPath4, 'w') as f:
            f.write("text in localPathName4.root\n")
        with open(localPath5, 'w') as f:
            f.write("text in subdir/localPathName5.root\n")

        # delete a file
        out = pm.deleteLocalFile("localPathName4.root")
        self.assertTrue(out)

        # test if list of files is stored correctly
        targetPaths = ["targetPathName.root", "localPathName.root", "localPathName2.root", "localPathName3.root", "subdir/localPathName5.root"]
        targetPaths = [targetDir+x for x in targetPaths]
        out = pm.getAllTargetPaths()
        # this test can fail if pathManagerTargetTest is changed.
        self.assertEqual(len(out), len(targetPaths))
        print("A warning associated with \"localPathName3.root\" is expected.")
        return

    def pathManagerLocalTest(self, useSingleton, targetDir, localDir, setLocalDir):
        if useSingleton:
            print("WARNING: Because of the way that testing is implemented in python, you cannot test getLocalPath for singletons.")
        self.pathManagerLocalTestSub(useSingleton, targetDir, localDir, setLocalDir)

        # clean up
        if setLocalDir and os.path.exists(localDir):
            os.rmdir(localDir)

        # Check if all files are copied in destructor
        self.assertTrue (os.path.isfile(targetDir+"targetPathName.root"))
        os.remove(targetDir+"targetPathName.root")
        self.assertTrue (os.path.isfile(targetDir+"localPathName.root"))
        os.remove(targetDir+"localPathName.root")
        self.assertTrue (os.path.isfile(targetDir+"localPathName2.root"))
        os.remove(targetDir+"localPathName2.root")
        self.assertFalse(os.path.isfile(targetDir+"localPathName3.root"))
        self.assertFalse(os.path.isfile(targetDir+"localPathName4.root"))
        self.assertTrue (os.path.isfile(targetDir+"subdir/localPathName5.root"))
        os.remove(targetDir+"subdir/localPathName5.root")
        os.rmdir(targetDir+"subdir")

        return

    def pathManagerTargetTest(self, useSingleton, targetDir):
        """
        Test if correct path names are created by TQPathManager.
        """
        from QFramework import TQPathManager

        if useSingleton:
            pm = TQPathManager.getPathManager()
            pm.setUseTemporaryStorage(False)
        else:
            pm = TQPathManager(False)

        out = pm.getUseTemporaryStorage()
        self.assertFalse(out)

        # getTargetPath and getLocalPath should return the same
        out = pm.getTargetPath("targetPathName.root")
        self.assertEqual(out, targetDir+"targetPathName.root")
        out = pm.getLocalPath("targetPathName.root")
        self.assertEqual(out, targetDir+"targetPathName.root")
        return

    def testPathManagerCAFOutputDir(self):
        """
        Test TQPathManager when environment variable CAFOUTPUTDIR is set.
        """
        useSingleton = False
        os.environ["CAFOUTPUTDIR"] = sla
        localDir = self.getLocalDir()
        self.pathManagerTargetTest(useSingleton, slap)
        self.pathManagerLocalTest(useSingleton, slap, localDir, False)

    def testPathManagerPWD(self):
        useSingleton = False
        localDir = self.getLocalDir()
        self.pathManagerTargetTest(useSingleton, pwdp)
        self.pathManagerLocalTest(useSingleton, pwdp, localDir, False)

    def testSingletonCAFOutputDir(self):
        useSingleton = True
        os.environ["CAFOUTPUTDIR"] = sla
        localDir = self.getLocalDir()
        self.pathManagerTargetTest(useSingleton, slap)
    #     # does not work because of python implementation:
    #     # self.pathManagerLocalTest(useSingleton, slap, localDir, False)

    def testSingletonPWD(self):
        useSingleton = True
        localDir = self.getLocalDir()
        self.pathManagerTargetTest(useSingleton, pwdp)
        # does not work because of python implementation:
        # self.pathManagerLocalTest(useSingleton, pwdp, localDir, False)

    def testSingletonCAFOutputDir_PathChange(self):
        useSingleton = False
        os.environ["CAFOUTPUTDIR"] = sla
        localDir = self.getLocalDir()+"test/"
        self.pathManagerLocalTest(useSingleton, slap, localDir, True)

    # def testSingletonCAFOutputDir_PathChange(self):
    #     useSingleton = True
    #     os.environ["CAFOUTPUTDIR"] = sla
    #     localDir = "/tmp/"+os.environ["USER"]+"/test/"
    #     self.pathManagerLocalTest(useSingleton, slap, localDir, True)

    def testCAHFindConfigPath(self):
        '''
        CommonAnalysisHelpers has a method to help users to find their config
        files. This function is tested here.
        '''
        from QFramework import TQPathManager
        from CommonAnalysisHelpers import common
        testFile = "dir1/testFile"
        labels = ":objectinside>>::label"

        # pass absolute file
        out = common.findConfigPath(slap + testFile)
        self.assertEqual(slap + testFile, out)

        # pass relative file
        out = common.findConfigPath(slp + testFile)
        self.assertEqual(slap + testFile, out)

        # pass relative file with object and label names appended
        # (these names should be ignored internally)
        out = common.findConfigPath(slp + testFile + labels)
        self.assertEqual(slap + testFile + labels, out)

        # the file testFile1 does not exist in slap, but only in
        # slap + slp + testFile1. The first case (finding relative to
        # pwd) should be ignored
        out = common.findConfigPath(slp + "testFile1")
        out1 = TQPathManager.findFileFromEnvVar(slp + "testFile1", "CAFANALYSISSHARE")
        self.assertEqual(out, out1)

    def testRegisterLocalPath(self):
        from QFramework import TQPathManager

        os.environ["CAFOUTPUTDIR"] = sla
        pm = TQPathManager.getPathManager()
        outLocal1 = pm.getLocalPath("testFile")
        self.assertNotEqual(outLocal1, slap + "testFile")
        outTarget1 = pm.getTargetPath("testFile")
        self.assertEqual(outTarget1, slap + "testFile")
        pm.registerLocalPath("testFile")
        outLocal2 = pm.getLocalPath("testFile")
        self.assertEqual(outLocal2, outLocal1)
        outLocal3 = pm.getTargetPath("testFile")
        self.assertEqual(outLocal3, outLocal1)
        outTarget_new = pm.getTargetPath("testFile_new")
        self.assertEqual(outTarget_new, slap + "testFile_new")
        pm.unregisterLocalPath("testFile")
        outTarget2 = pm.getTargetPath("testFile")
        self.assertEqual(outTarget2, outTarget1)
        pm.deleteLocalFile("testFile")

if __name__ == "__main__":
    unittest.main()
