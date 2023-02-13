#!/usr/bin/env python

import unittest
import os
from ROOT import TString
from QFramework import TQPythonTest,TQFolder,TQPathManager,TQLibrary,TQUtils
from CommonAnalysisHelpers import analyze

os.environ['CAFCOREDIR'] = os.path.join(str(TQLibrary.getQLibrary().getTQPATH().Data()),"..")

dummyFileName = "dummy.py"
dummyScriptPath = TQUtils.readEnvVarValue("TQPATH") + "/../CommonAnalysisHelpers/share/algorithms/" + dummyFileName

class analyzeTest(TQPythonTest):
    def setUp(self):
        self.removeDummyScript = False
        if not os.path.exists(dummyScriptPath):
            self.removeDummyScript = True
            with open(dummyScriptPath, "w") as f:
                f.write('# This file can safely be removed. In fact, you should not even see it. The fact\n# that you do, probably points to an error in a plotting test. Please remove it\n# and hope it does not appear again.\n\ndef addAlgorithms(visitor,config):\n\treturn True\n\n# Here could be your algorithm')
        return

    def tearDown(self):
        if self.removeDummyScript:
            os.remove(dummyScriptPath)
        return

    @unittest.skip("")
    def test_findAlgorithmsInDefaultLocation(self):
        from QFramework import TQAnalysisSampleVisitor
        config = TQFolder.newFolder("config");
        config.setTagString("algorithms.snippets", dummyFileName)
        visitor = TQAnalysisSampleVisitor.TQAnalysisSampleVisitor()
        analyze.bookAlgorithms(config, visitor)
        return


if __name__ == "__main__":
    unittest.main()
