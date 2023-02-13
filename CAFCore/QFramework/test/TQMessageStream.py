#!/usr/bin/env python

import unittest

import ROOT
import QFramework

class Test_TQMessageStream(unittest.TestCase):
    """
    Test the interplay of python strings and TStrings.
    """

    def test_messageCount(self):
        '''Make sure that info/eerror messages are counted correctly.
        '''
        MS = QFramework.TQMessageStream
        infoType = MS.INFO
        errorType = MS.ERROR

        # Retrieve initial count
        countInfo1 = MS.getGlobMessageCount(infoType)
        countError1 = MS.getGlobMessageCount(errorType)

        # Make sure these methods work
        MS.printGlobMessageCount(infoType)
        MS.printGlobMessageCount(errorType)

        # Make sure the printout didn't increase the message count
        countInfo2 = MS.getGlobMessageCount(infoType)
        countError2 = MS.getGlobMessageCount(errorType)
        self.assertEqual(countInfo1, countInfo2)
        self.assertEqual(countError1, countError2)

        # Print an info message through QFramework, TQLibrary and a
        # custom instance of TQMessageStream.
        QFramework.INFO("message1")
        QFramework.TQLibrary.msgStream.sendMessage(infoType, "message2")
        stream_cout = QFramework.TQMessageStream()
        stream_cout.sendMessage(infoType, "message3")

        # All three info messages should be counted.
        countInfo3 = MS.getGlobMessageCount(infoType)
        countError3 = MS.getGlobMessageCount(errorType)
        self.assertEqual(countInfo3-countInfo2, 3)
        self.assertEqual(countError3-countError2, 0)

        # Use a new stringstream that doesn't point to cout. Its
        # message should not be counted.
        stringstream = ROOT.std.stringstream()
        stream = QFramework.TQMessageStream(stringstream)
        stream.sendMessage(infoType, "message4")
        countInfo4 = MS.getGlobMessageCount(infoType)
        self.assertEqual(countInfo4, countInfo3)

        # It should be counted once the messages are written to cout.
        stream.writeMessagesToStdout()
        countInfo5 = MS.getGlobMessageCount(infoType)
        self.assertEqual(countInfo5 - countInfo4, 1)



if __name__ == "__main__":
    unittest.main()
