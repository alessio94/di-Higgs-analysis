#!/usr/bin/env python

import os
import unittest

from ROOT import TString

# Importing QFramework patches TStrings. If QFramework is not imported, weird
# things can happen. Consider the following examples. (Have to be executed in
# two separate python shells.)
#
# >>> from ROOT import TString
# >>> "Hello" + TString("World")
# raises an exception
#
# >>> from ROOT import TString
# >>> TString("other")  # this has some side effects
# >>> "Hello" + TString("World")
# prints "WorldHello"
import QFramework

class TStringTest(unittest.TestCase):
    """
    Test the interplay of python strings and TStrings.
    """
    
    def test_pystr_TString(self):
        """
        Add a python string and a TString
        """
        s = "1Hello " + TString("1World")
        self.assertIsInstance(s, str)
        self.assertEqual(s, "1Hello 1World")

    def test_TString_pystr(self):
        """
        Add a python string and a TString
        """
        s = TString("2Hello ") + "2World"
        self.assertIsInstance(s, TString)
        self.assertEqual(s, "2Hello 2World")

    def test_TString_TString(self):
        """
        Add a TStringa nd a TString
        """
        s = TString("3Hello ") + TString("3World")
        self.assertIsInstance(s, TString)
        self.assertEqual(s, "3Hello 3World")
  
if __name__ == "__main__":
    unittest.main()
