#!/usr/bin/env python

"""
This module implements unit test for the local submission controller.
"""

import unittest
import tempfile
import os
import shutil
import time

from SubmissionHelpers.submit import MinimalArgumentParser
from SubmissionHelpers.submissionControllers.local \
    import LocalController
from SubmissionHelpers.task \
    import task, taskStatus


class ArgumentObject(dict):
    """
    Object to mimic the behavior of a name space.
    """
    defaults = MinimalArgumentParser().parse_args([]) #explicitly passing empty list to not use argv but only parse defaults. Note that this means the minimalArgParser must NOT have required arguments!
    def __getattr__(self, name):
        """
        If the requested attribute does not exist, try to find it in the
        dictionary.
        """
        if name in self:
          return self[name]
        elif hasattr(ArgumentObject.defaults,name):
          return getattr(ArgumentObject.defaults,name)
        else:
          raise AttributeError("Unknown argument requested: '{:s}'".format(name))

class LocalControllerTest(unittest.TestCase):
    """
    Implements test for the local submission controller. The tests use the
    high level methods in various different use cases an check if the jobs
    were executed.
    """

    def setUp(self):
        """
        Creates a new temporary directory. Jobs will create files inside the
        directory to indicate that the jobs ran.
        """
        self.temp_dir = tempfile.mkdtemp()
        self.log_dir = tempfile.mkdtemp()

    def tearDown(self):
        """
        Remove the temporary directory.
        """
        shutil.rmtree(self.temp_dir)
        shutil.rmtree(self.log_dir)

    @unittest.skip("")
    def test_dependency_indicatior(self):
        """
        The current implementation does not support dependencies. The
        indicator method should return False.
        """
        controller = LocalController()
        controller.poll_delay = 1
        self.assertFalse(controller._supports_simple_dependencies())

    @unittest.skip("")
    def test_single_job(self):
        """
        Check that a single job is executed. The job creates the file
        'job1.txt'. When the submit method returns, this file should exist.
        """
        output_1 = os.path.join(self.temp_dir, "job1.txt")
        log_1 = os.path.join(self.log_dir, "job1.log")
        task1 = task("Creates-job1.txt", "echo Hello > %s" % output_1,
                     setup="#!/usr/bin/env bash", outputs=[output_1],
                     logFile=log_1, errFile=log_1)

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=0, maxConcurrent=-1)

        controller = LocalController()
        controller.poll_delay = 1
        controller.submitTasks(args, [task1])

        controller.updateTaskStatus(args, task1)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertTrue(os.path.exists(output_1))


    @unittest.skip("")
    def test_independent_jobs(self):
        """
        Check that a two independent jobs are executed. The jobs creates the
        file 'job1.txt' and 'job2.txt'. When the submit method returns, both
        files should exist.
        """
        output_1 = os.path.join(self.temp_dir, "job1.txt")
        log_1 = os.path.join(self.log_dir, "job1.log")
        task1 = task("Creates-job1.txt", "echo Hello > %s" % output_1,
                     setup="#!/usr/bin/env bash", outputs=[output_1],
                     logFile=log_1, errFile=log_1)

        output_2 = os.path.join(self.temp_dir, "job2.txt")
        log_2 = os.path.join(self.log_dir, "job2.log")
        task2 = task("Creates-job2.txt", "echo Hello > %s" % output_2,
                     setup="#!/usr/bin/env bash", outputs=[output_2],
                     logFile=log_2, errFile=log_2)

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=0, maxConcurrent=-1)

        controller = LocalController()
        controller.poll_delay = 1
        controller.submitTasks(args, [task1, task2])

        controller.updateTaskStatus(args, task1)
        controller.updateTaskStatus(args, task2)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertTrue(os.path.exists(output_1))

        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertTrue(os.path.exists(output_2))

    @unittest.skip("")
    def test_linear_jobs(self):
        """
        Check that tree jobs in a linear dependency chain are executed on
        after another. Each job creates a which contains a sorted list of all
        existing files in the temporary directory. After the submit method
        returns, check that the files have the expected content to endure that
        the jobs ran in order.
        """
        output_1 = os.path.join(self.temp_dir, "job1.txt")
        log_1 = os.path.join(self.log_dir, "job1.log")
        task1 = task("Creates-job1.txt", "export A=\"$(ls %s | sort)\"; echo \"$A\" > %s" \
                     % (self.temp_dir, output_1),
                     setup="#!/usr/bin/env bash", outputs=[output_1],
                     logFile=log_1, errFile=log_1)

        output_2 = os.path.join(self.temp_dir, "job2.txt")
        log_2 = os.path.join(self.log_dir, "job2.log")
        task2 = task("Creates-job2.txt", "export A=\"$(ls %s | sort)\"; echo \"$A\" > %s" \
                     %  (self.temp_dir, output_2),
                     setup="#!/usr/bin/env bash", outputs=[output_2],
                     dependencies=[task1],
                     logFile=log_2, errFile=log_2)

        output_3 = os.path.join(self.temp_dir, "job3.txt")
        log_3 = os.path.join(self.log_dir, "job3.log")
        task3 = task("Creates-job3.txt", "export A=\"$(ls %s | sort)\"; echo \"$A\" > %s" \
                     %  (self.temp_dir, output_3),
                     setup="#!/usr/bin/env bash", outputs=[output_3],
                     dependencies=[task2],
                     logFile=log_3, errFile=log_3)

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=0, maxConcurrent=-1)

        controller = LocalController()
        controller.poll_delay = 1
        controller.submitTasks(args, [task3])

        controller.updateTaskStatus(args, task3)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertTrue(os.path.exists(output_1))
        with open(output_1) as file_1:
            self.assertEqual(file_1.read(), "\n")

        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertTrue(os.path.exists(output_2))
        with open(output_2) as file_2:
            self.assertEqual(file_2.read(), "job1.txt\n")

        self.assertEqual(task3.statusCode, taskStatus.done)
        self.assertTrue(os.path.exists(output_3))
        with open(output_3) as file_3:
            self.assertEqual(file_3.read(), "job1.txt\njob2.txt\n")

    @unittest.skip("")
    def test_diamond_pattern(self):
        """
        Check that a diamond shaped pattern is executed in order. Each jobs
        creates a file with a sorted list of all files in the temporary
        directory. By examining the content of the files, it is checked that
        the jobs are executed in order.
        """
        output_1 = os.path.join(self.temp_dir, "job1.txt")
        log_1 = os.path.join(self.log_dir, "job1.log")
        task1 = task("Creates-job1.txt", "export A=\"$(ls %s | sort)\"; echo \"$A\" > %s" \
                     %  (self.temp_dir, output_1),
                     setup="#!/usr/bin/env bash", outputs=[output_1],
                     logFile=log_1, errFile=log_1)

        output_2 = os.path.join(self.temp_dir, "job2.txt")
        log_2 = os.path.join(self.log_dir, "job2.log")
        task2 = task("Creates-job2.txt", "export A=\"$(ls %s | sort)\"; echo \"$A\" > %s" \
                     %  (self.temp_dir, output_2),
                     setup="#!/usr/bin/env bash", outputs=[output_2],
                     dependencies=[task1],
                     logFile=log_2, errFile=log_2)

        output_3 = os.path.join(self.temp_dir, "job3.txt")
        log_3 = os.path.join(self.log_dir, "job3.log")
        task3 = task("Creates-job3.txt", "export A=\"$(ls %s | sort)\"; echo \"$A\" > %s" \
                     %  (self.temp_dir, output_3),
                     setup="#!/usr/bin/env bash", outputs=[output_3],
                     dependencies=[task1],
                     logFile=log_3, errFile=log_3)

        output_4 = os.path.join(self.temp_dir, "job4.txt")
        log_4 = os.path.join(self.log_dir, "job4.log")
        task4 = task("Creates-job4.txt", "export A=\"$(ls %s | sort)\"; echo \"$A\" > %s" \
                     %  (self.temp_dir, output_4),
                     setup="#!/usr/bin/env bash", outputs=[output_4],
                     dependencies=[task2, task3],
                     logFile=log_4, errFile=log_4)

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=0, maxConcurrent=-1)

        controller = LocalController()
        controller.poll_delay = 1
        controller.submitTasks(args, [task4])

        controller.updateTaskStatus(args, task4)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertTrue(os.path.exists(output_1))
        with open(output_1) as file_1:
            self.assertEqual(file_1.read(), "\n")

        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertTrue(os.path.exists(output_2))
        with open(output_2) as file_2:
            self.assertIn(file_2.read(), ["job1.txt\n",
                                          "job1.txt\njob3.txt\n"])

        self.assertEqual(task3.statusCode, taskStatus.done)
        self.assertTrue(os.path.exists(output_3))
        with open(output_3) as file_3:
            self.assertIn(file_3.read(), ["job1.txt\n",
                                          "job1.txt\njob2.txt\n"])

        self.assertEqual(task4.statusCode, taskStatus.done)
        self.assertTrue(os.path.exists(output_4))
        with open(output_4) as file_4:
            self.assertEqual(file_4.read(),
                             "job1.txt\njob2.txt\njob3.txt\n")

    @unittest.skip("")
    def test_time_limit(self):
        """
        Check that a job which exceeds its time limit is stopped.
        """
        output_1 = os.path.join(self.temp_dir, "start.txt")
        output_2 = os.path.join(self.temp_dir, "end.txt")
        log_1 = os.path.join(self.log_dir, "job1.log")

        slow_payload = """
        echo "Job start" > %s
        sleep 10
        echo "Job start" > %s
        """ % (output_1, output_2)

        task1 = task("Creates-job1.txt", slow_payload,
                     setup="#!/usr/bin/env bash", time=3./60,
                     outputs=[output_1, output_2],
                     logFile=log_1, errFile=log_1)

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=0, maxConcurrent=-1)

        controller = LocalController()
        controller.poll_delay = 1

        start = time.time()
        controller.submitTasks(args, [task1])
        end = time.time()

        # Make sure that the job did not ran too long
        self.assertLess(end - start, 5)

        controller.updateTaskStatus(args, task1)

        self.assertEqual(task1.statusCode, taskStatus.failed)
        self.assertTrue(os.path.exists(output_1))
        self.assertFalse(os.path.exists(output_2))

        # Make sure that the file process did not continue running after it
        # should have been killed
        time.sleep(11)
        self.assertFalse(os.path.exists(output_2))

if __name__ == "__main__":
    unittest.main()
