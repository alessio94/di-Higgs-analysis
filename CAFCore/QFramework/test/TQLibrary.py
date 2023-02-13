#!/usr/bin/env python

import unittest
import QFramework
from QFramework import TQLibrary

class TQLibraryTest(unittest.TestCase):

    def setUp(self):
        return
    def tearDown(self):
        return

    def getErrorFormat(self, message):
        return '\x1b[1;31mERROR\x1b[0m\x1b[1m: \x1b[0m\x1b[1m' + message + '\x1b[0m\n'
    def getWarnFormat(self, message):
        return '\x1b[1;33mWARNING\x1b[0m\x1b[1m: \x1b[0m\x1b[1m' + message + '\x1b[0m\n'

    def testCapturingBasics(self):
        """
        Test TQLibrary basic return values when capturing stdout and stderr.
        """

        for stream in ["out", "err"]:

            eval("TQLibrary.allowCapturing_std" + stream + "(False)")

            # Returns -1 if stdout not allowed to be captured.
            out = eval("TQLibrary.captureStd" + stream + "()")
            self.assertEqual(out, -1)

            # If capturing is not on, it returns "".
            out = eval("TQLibrary.readCapturedStd" + stream + "()")
            self.assertEqual(out, "")

            # Does not get reset, instead -3 is returned.
            out = eval("TQLibrary.resetCapturedStd" + stream + "()")
            self.assertEqual(out, -3)
        return

    def testCapturing(self):
        """
        Test TQLibrary capturing of stdout and stderr.
        """

        for stream in ["Stdout", "StdoutStderr"]:

            if not stream == "StdoutStderr":
                eval("TQLibrary.allowCapturing_std" + stream[3:] + "()")
            else:
                TQLibrary.allowCapturing()
            # Returns 0 if stdout not captured or redirected yet.
            out = eval("TQLibrary.capture" + stream + "()")
            self.assertEqual(out, 0)

            # Returns -3 if already captured.
            out = eval("TQLibrary.capture" + stream + "()")
            self.assertEqual(out, -3)

            QFramework.ERROR("test")

            # The error message is correctly captured.
            out = eval("TQLibrary.readCaptured" + stream + "()")
            self.assertEqual(out, self.getErrorFormat("test"))

            QFramework.WARN("anotherTest")

            # The warning message is correctly captured. It gets added to the previous one.
            out = eval("TQLibrary.readCaptured" + stream + "()")
            self.assertEqual(out, self.getErrorFormat("test") + self.getWarnFormat("anotherTest"))

            eval("TQLibrary.resetCaptured" + stream + "()")
            QFramework.ERROR("t3st")

            # After resetting the captured text, only one error message is there.
            out = eval("TQLibrary.readCaptured" + stream + "()")
            self.assertEqual(out, self.getErrorFormat("t3st"))

            # Reading and resetting at the same time.
            out = eval("TQLibrary.readAndResetCaptured" + stream + "()")
            self.assertEqual(out, self.getErrorFormat("t3st"))

            # Nothing there, because it has been reset.
            out = eval("TQLibrary.readCaptured" + stream + "()")
            self.assertEqual(out, "")

            QFramework.ERROR("bla")
            
            if not stream == "StdoutStderr":
                eval("TQLibrary.restore_std" + stream[3:] + "()")
            else:
                TQLibrary.restore()

            # Returns "" if output is not currently captured.
            out = eval("TQLibrary.readCaptured" + stream + "()")
            self.assertEqual(out, "")

            # Capturing works just like before.
            out = eval("TQLibrary.capture" + stream + "()")
            self.assertEqual(out, 0)

            QFramework.WARN("anotherTest")

            # The warning message is captured again.
            out = eval("TQLibrary.readCaptured" + stream + "()")
            self.assertEqual(out, self.getWarnFormat("anotherTest"))

            if not stream == "StdoutStderr":
                eval("TQLibrary.restore_std" + stream[3:] + "()")
            else:
                TQLibrary.restore()
        
        return

if __name__ == "__main__":
    unittest.main()
