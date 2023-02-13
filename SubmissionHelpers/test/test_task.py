#!/usr/bin/env python

"""
This python module implements tests for the task class and small related
auxiliary classes, such as taskStatus. The tests in this module do not cover
submission related aspects.
"""

import unittest

from SubmissionHelpers.task \
    import task, taskStatus

class TaskTest(unittest.TestCase):
    """
    Implements test cases for the task class. The tests test static
    (not submission related) aspects of the task class.
    """

    @unittest.skip("")
    def test_string_sparse(self):
        """
        Check that a meaningful string representation is created when
        optional properties are omitted.
        """
        thisTask = task("mock task", "#!/...")
        try:
            self.assertTrue(len(str(thisTask)) > 10)
        except Exception as e:
            self.fail(e)

    @unittest.skip("")
    def test_string_full(self):
        """
        Check that a meaningful string representation is created when
        all properties are set.
        """
        depTask = task("mock task", "#!/...")
        thisTask = task("mock task", "#!/...", time=314,
               memory=4200, ncores=6, queue="atljak",
               setup="setupATLAS", inputs=["/dev/urandom"],
               outputs=["/dev/null"], logFile="/path/to/log",
               errFile="/path/to/err", account="higgs_user",
               dependencies=[depTask])
        try:
            self.assertTrue(len(str(thisTask)) > 100)
        except Exception as e:
            self.fail(e)

if __name__ == "__main__":
    unittest.main()
