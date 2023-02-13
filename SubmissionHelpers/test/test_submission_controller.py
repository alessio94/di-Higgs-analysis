#!/usr/bin/env python

"""
This module implements unit tests for the abstract submission controller
class.
"""

import unittest
import os.path as op
import os
import subprocess
import json
import time
import threading
from tempfile import mkstemp, mkdtemp
import shutil
import inspect

from SubmissionHelpers.submit import MinimalArgumentParser
from SubmissionHelpers.submissionControllerBase \
    import submissionControllerBase
from SubmissionHelpers.task \
    import task, taskStatus


class MockDepController(submissionControllerBase):
    """
    This submission controller is used exclusively for testing purposes. It
    implement all the methods called by submitSingleTask(). Each method call
    is recorded in the internal _calls list. Each item is a tuple consisting of
    the called method name, the positional arguments and the keyword
    arguments.

    This class is inspired by magic mocks.
    """
    METHODS = ["_apply_identifier",
               "_apply_queue",
               "_apply_memory",
               "_apply_account",
               "_apply_cpu_count",
               "_apply_time_limit",
               "_apply_log_file",
               "_apply_error_file",
               "_apply_dependencies",
               "_handle_submit_response",]

    def _apply_base_command(self, args, thisTask, cmd_args, payload, setup):
        """
        This needs to be implemented manually, because we need to supply a
        base command.
        """
        self._record_method_call("_apply_base_command",
                                  args, thisTask, cmd_args, payload, setup)
        cmd_args += ["echo", "Hello World"]

    def _supports_simple_dependencies(self):
        """
        This mock controller supports dependencies.
        """
        return True

    @staticmethod    
    def _is_available():
        """
        This mock controller is always available.
        """
        return True

    def __init__(self, *args, **kwds):
        """
        Initialize the _calls variable and call the super constructor to
        create a new MockDepController.
        """
        self._calls = []
        super(MockDepController, self).__init__(*args, **kwds)

    def _record_method_call(self, method_name, *args, **kwds):
        """
        Constructs a tuple out of the three given arguments and appends them
        to the internal _calls variable.

        The method returns True. This is required because the
        _apply_xxx() methods return True.
        """
        self._calls.append((method_name, args, kwds))

        return True

    def __getattribute__(self, name):
        """
        Checks if the requests attribute is defined in the METHODS variable.
        If yes, returns a method which records the method call in this class.
        If it is not found, raise an exception.
        """
        METHODS = object.__getattribute__(self, "METHODS")
        if name in METHODS:
            return lambda *a, **k: self._record_method_call(name, *a, **k)
        else:
            return object.__getattribute__(self, name)

class MockNoDepController(MockDepController):
    """
    Mock controller version which does not support simple dependency handling.
    """
    def _supports_simple_dependencies(self):
        """
        This controller does not support dependencies.
        """
        return False

class MockNoDepControllerNoResponse(MockNoDepController):
    """
    Mock controller that cannot interact with the batch system.
    """
    def _updateQueueStatus(self):
        return False

class TempDirTests:

    def setUp(self):
        """
        Creates a temporary directory and changes into it.
        """
        self.orig_dir = os.getcwd()
        self.temp_dir = mkdtemp()
        base_dir = op.dirname(op.realpath(__file__))
        os.symlink(op.join(base_dir, "toy_controller.py"),
                        op.join(self.temp_dir, "toy_controller.py"))

        os.chdir(self.temp_dir)


    def tearDown(self):
        """
        Change back to original directory and remove temporary directory.
        """
        os.chdir(self.orig_dir)
        try:
            shutil.rmtree(self.temp_dir)
        except (IOError, OSError) as e:
            pass


class MockControllerTest(TempDirTests, unittest.TestCase):
    """
    Self tests of the MockDepController.
    """

    @unittest.skip("")
    def test_record_method_call(self):
        """
        Check that the arguments passed to _record_method_call() are stored in
        _calls.
        """
        controller = MockDepController()
        self.assertEqual(len(controller._calls), 0)

        controller._record_method_call("Hello", 2, 3, "World", debug="yes")

        self.assertEqual(len(controller._calls), 1)
        self.assertEqual(controller._calls[0],
                         ("Hello", (2, 3, "World"), {"debug": "yes"}))


        controller._record_method_call("submit", int)
        controller._record_method_call("empty")

        self.assertEqual(len(controller._calls), 3)
        self.assertEqual(controller._calls[0],
                         ("Hello", (2, 3, "World"), {"debug": "yes"}))
        self.assertEqual(controller._calls[1],
                         ("submit", (int, ), {}))
        self.assertEqual(controller._calls[2],
                         ("empty", (), {}))

    @unittest.skip("")
    def test_record_method_call_return(self):
        """
        Check that _record_method_call() returns an empty string.
        """
        controller = MockDepController()
        retval = controller._record_method_call("Hello", payload=["#!/..."],
                                                setup=["..."])
        self.assertTrue(retval)

    @unittest.skip("")
    def test_apply_queue(self):
        """
        Check that the method call to _apply_queue() is recorded.
        """
        controller = MockDepController()
        controller._apply_queue(None, "task", [], ["echo"], ["setupATLAS"])

        self.assertEqual(len(controller._calls), 1)
        self.assertEqual(controller._calls[0],
                         ("_apply_queue", (None, "task", [], ["echo"], ["setupATLAS"]), {}))

    @unittest.skip("")
    def test_apply_queue_return(self):
        """
        Check that _apply_queue returns an empty string.
        """
        controller = MockDepController()
        retval = controller._apply_queue(None, "task", [], ["echo"], ["setupATLAS"])

        self.assertTrue(retval)

    @unittest.skip("")
    def test_apply_identifier (self):
        """
        Check that the method call to _apply_identifier() is recorded.
        """
        controller = MockDepController()
        controller._apply_identifier(None, "task", [], ["echo"], ["setupATLAS"])

        self.assertEqual(len(controller._calls), 1)
        self.assertEqual(controller._calls[0],
                         ("_apply_identifier", (None, "task", [], ["echo"], ["setupATLAS"]), {}))

    @unittest.skip("")
    def test_dependecy_support(self):
        """
        Check that the MockDepController and MockNoDepController classes
        return the appropriate value when _supports_simple_dependencies() is
        called.
        """
        dep_controller = MockDepController()
        self.assertTrue(dep_controller._supports_simple_dependencies())

        nodep_controller = MockNoDepController()
        self.assertFalse(nodep_controller._supports_simple_dependencies())

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

class ArgumentObjectTest(unittest.TestCase):
    """
    Test the auxiliary ArgumentObject class.
    """

    @unittest.skip("")
    def test_creating(self):
        """
        Check that dict-like object can be created via the constructor.
        """
        args = ArgumentObject({4: 2, "hello": "world"})
        self.assertEqual(args[4], 2)
        self.assertEqual(args["hello"], "world")

    @unittest.skip("")
    def test_access(self):
        args = ArgumentObject()
        args["hello"] = "world"
        args["foo"] = 3

        self.assertEqual(args.hello, "world")
        self.assertEqual(args.foo, 3)
        self.assertRaises(AttributeError, getattr, args,  "bar")


class SubmissionControllerTest(TempDirTests, unittest.TestCase):
    """
    Test the behavior of the submission controller base class.
    """

    @unittest.skip("")
    def test_submitSingleTest_dep_none_exception(self):
        """
        Check that __submitSingleTask does not raise an exception
        if all the task has no dependencies.
        """
        thisTask = task("mock task", "#!/...")
        controller = MockDepController()
        args = ArgumentObject(verbose=False, debug=False)
        try:
            controller._submitSingleTask(args, thisTask)
        except Exception as e:
            # Please not this includes even name errors
            self.fail(e)

    @unittest.skip("")
    def test_submitSingleTest_dep_none_building(self):
        """
        Check that __submitSingleTask calles all the _apply_xxx() methods when
        submitting a task without dependencies.
        """
        thisTask = task("mock task", "#!/...")
        controller = MockDepController()
        args = ArgumentObject(verbose=False, debug=False)
        controller._submitSingleTask(args, thisTask)

        method_names = [c[0] for c in controller._calls]
        self.assertIn("_apply_base_command", method_names)
        self.assertIn("_apply_identifier", method_names)
        self.assertIn("_apply_queue", method_names)
        self.assertIn("_apply_account", method_names)
        self.assertIn("_apply_memory", method_names)
        self.assertIn("_apply_cpu_count", method_names)
        self.assertIn("_apply_time_limit", method_names)
        self.assertIn("_apply_log_file", method_names)
        self.assertIn("_apply_error_file", method_names)
        self.assertIn("_apply_dependencies", method_names)

        kwds = [c[2] for c in controller._calls]
        self.assertEqual(kwds, [{}] * 11)

        call_args = [c[1] for c in controller._calls]
        self.assertEqual(call_args,
                         [(args, thisTask, ["echo", "Hello World"], ["#!/..."], [])] * 10 + \
                         [(thisTask, b"Hello World\n", b"", 0)])

    @unittest.skip("")
    def test_submitSingleTest_dep_done(self):
        """
        Check that no exception is raised if all dependencies are done or have
        a job id.
        """
        dep1 = task("mock dep task 1", "#!/...")
        dep1.statusCode = taskStatus.done
        dep2 = task("mock dep task 2", "#!/...")
        dep2.jobid = 1234
        dep3 = task("mock dep task 3", "#!/...")
        dep3.statusCode = taskStatus.done
        dep3.jobid = 1235

        thisTask = task("mock task", "#!/...", dependencies=[dep1, dep2, dep3])
        controller = MockDepController()
        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False)

        try:
            controller._submitSingleTask(args, thisTask)
        except Exception as e:
            # Please not this includes even name errors
            self.fail(e)

    @unittest.skip("")
    def test_submitSingleTest_dep_not_done(self):
        """
        Check that an exception is raised if a dependency is not done and has
        no job id
        """
        dep1 = task("mock dep task 1", "#!/...")
        dep1.statusCode = taskStatus.done
        dep2 = task("mock dep task 2", "#!/...")
        dep3 = task("mock dep task 3", "#!/...")
        dep3.statusCode = taskStatus.done
        dep3.jobid = 1235

        thisTask = task("mock task", "#!/...", dependencies=[dep1, dep2, dep3])
        controller = MockDepController()
        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False)

        self.assertRaises(Exception, controller._submitSingleTask,
                          args, thisTask)


    @unittest.skip("")
    def test_submitSingleTest_nodep_none_exception(self):
        """
        Check that __submitSingleTask does not raise an exception
        if all the task has no dependencies.
        """
        thisTask = task("mock task", "#!/...")
        controller = MockNoDepController()
        args = ArgumentObject(verbose=False, debug=False)
        try:
            controller._submitSingleTask(args, thisTask)
        except Exception as e:
            # Please not this includes even name errors
            self.fail(e)

    @unittest.skip("")
    def test_submitSingleTest_nodep_none_building(self):
        """
        Check that __submitSingleTask calles all the _apply_xxx() methods when
        submitting a task without dependencies.
        """
        thisTask = task("mock task", "#!/...")
        controller = MockNoDepController()
        args = ArgumentObject(verbose=False, debug=False)
        controller._submitSingleTask(args, thisTask)

        method_names = [c[0] for c in controller._calls]
        self.assertIn("_apply_base_command", method_names)
        self.assertIn("_apply_identifier", method_names)
        self.assertIn("_apply_queue", method_names)
        self.assertIn("_apply_memory", method_names)
        self.assertIn("_apply_cpu_count", method_names)
        self.assertIn("_apply_account", method_names)
        self.assertIn("_apply_time_limit", method_names)
        self.assertIn("_apply_log_file", method_names)
        self.assertIn("_apply_error_file", method_names)

        kwds = [c[2] for c in controller._calls]
        self.assertEqual(kwds, [{}] * 10)

        call_args = [c[1] for c in controller._calls]
        self.assertEqual(call_args,
                         [(args, thisTask, ["echo", "Hello World"], ["#!/..."], [])] * 9 + \
                         [(thisTask, b"Hello World\n", b"", 0)])

    @unittest.skip("")
    def test_submitSingleTest_nodep_done(self):
        """
        Check that no exception is raised if all dependencies are done.
        """
        dep1 = task("mock dep task 1", "#!/...")
        dep1.statusCode = taskStatus.done
        dep2 = task("mock dep task 2", "#!/...")
        dep2.statusCode = taskStatus.done
        dep2.jobid = 1234
        dep1.statusCode = taskStatus.done
        dep3 = task("mock dep task 3", "#!/...")
        dep3.statusCode = taskStatus.done
        dep3.jobid = 1235

        thisTask = task("mock task", "#!/...", dependencies=[dep1, dep2, dep3])
        controller = MockNoDepController()
        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False)

        try:
            controller._submitSingleTask(args, thisTask)
        except Exception as e:
            self.fail(e)

    @unittest.skip("")
    def test_submitSingleTest_nodep_not_done_id(self):
        """
        Check that an exception is raised if all dependencies there are
        not-done dependencies with a job id.
        """
        dep1 = task("mock dep task 1", "#!/...")
        dep1.statusCode = taskStatus.done
        dep2 = task("mock dep task 2", "#!/...")
        dep2.jobid = 1234
        dep3 = task("mock dep task 3", "#!/...")
        dep3.statusCode = taskStatus.done
        dep3.jobid = 1235

        thisTask = task("mock task", "#!/...", dependencies=[dep1, dep2, dep3])
        controller = MockNoDepController()
        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False)

        self.assertRaises(Exception, controller._submitSingleTask, args, thisTask)

    @unittest.skip("")
    def test_submitSingleTest_nodep_not_done(self):
        """
        Check that an exception is raised if a dependency is not done and has
        no job id
        """
        dep1 = task("mock dep task 1", "#!/...")
        dep1.statusCode = taskStatus.done
        dep2 = task("mock dep task 2", "#!/...")
        dep3 = task("mock dep task 3", "#!/...")
        dep3.statusCode = taskStatus.done
        dep3.jobid = 1235

        thisTask = task("mock task", "#!/...", dependencies=[dep1, dep2, dep3])
        controller = MockNoDepController()
        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False)

        self.assertRaises(Exception, controller._submitSingleTask, args, thisTask)

class ToyNoDepController(MockDepController):
    """
    This somewhat more advances mock controller is able to update a fake
    queue. The controller uses the toy_controller.py script which writes
    submissions into a file including job name, end time (start time +
    duration) and dependencies. The controller assigns toy job ids. The queue
    is updated based on the current time. The job meta data file is deleted
    when the controller object is destroyed.
    """
    METHODS = ["_apply_queue",
               "_apply_memory",
               "_apply_cpu_count",
               "_apply_account",
               "_apply_log_file",
               "_apply_error_file",]

    def  __init__(self):
        """
        Creates the temporary queue file.
        """
        super(ToyNoDepController, self).__init__()
        _, self.queue_file = mkstemp()
        os.remove(self.queue_file)
        self.script_path = op.join(os.getcwd(), "toy_controller.py")

    def __del__(self):
        """
        Clean up queue file.
        """
        try:
            os.remove(self.queue_file)
        except OSError as e:
            pass

    def _apply_base_command(self, args, thisTask, cmd_args, payload, setup):
        self._record_method_call("_apply_base_command",
                                 args, thisTask, cmd_args, payload, setup)
        cmd_args += [self.script_path, "-f", self.queue_file, "submit"]
        return True


    def _apply_time_limit(self, args, thisTask, cmd_args, payload, setup):
        self._record_method_call("_apply_time_limit",
                                 args, thisTask, cmd_args, payload, setup)
        cmd_args.append("-t")
        cmd_args.append("%d" % (thisTask.time * 60))

        return True

    def _apply_identifier(self, args, thisTask, cmd_args, payload, setup):
        self._record_method_call("_apply_identifier",
                                 args, thisTask, cmd_args, payload, setup)
        cmd_args.append("-i")
        cmd_args.append(thisTask.identifier)

        return True

    def _updateQueueStatus(self):
        self._record_method_call("_updateQueueStatus")

        cmd_args = [self.script_path, "-f", self.queue_file, "queue"]
        process = subprocess.Popen(cmd_args, stdin=subprocess.PIPE,
                                   stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                   universal_newlines=True)
        (stdout, stderr) = process.communicate("...\n")
        status_code = process.returncode

        if status_code != 0 or stderr:
            raise Exception("Queue pull returned with status code %d" % \
                            status_code, stderr)

        self.queueStatusCache = {}

        queue = json.loads(stdout)
        for job in queue:
            if job["state"] == "running":
                status = "running"
                statusCode = taskStatus.running
            else:
                status = "submitted"
                statusCode = taskStatus.submitted

            # Add / overwrite job information in cache
            self.queueStatusCache[job["identifier"]] = {
                "status": status,
                "statusCode": statusCode,
                "jobid": job["job_id"]}

        self.queueStatusTimestamp = time.time()
        return True

    def _handle_submit_response(self, thisTask, stdout, stderr, status_code):
        if len(stderr) or status_code:
            raise Exception("Submission failed.")

        job_info = json.loads(stdout)

        thisTask.jobid = job_info["job_id"]
        thisTask.setStatusCode(taskStatus.submitted)

        return True

    def _supports_simple_dependencies(self):
        return False


class ToyDepController(ToyNoDepController):
    """
    Toy submission controller based on the ToyNoDepController. This submission
    controller uses toy_controller.py's ability to add dependencies.
    """

    def _supports_simple_dependencies(self):
        return True

    def _apply_dependencies(self, args, thisTask, cmd_args, payload, setup):
        self._record_method_call("_apply_dependencies",
                                 args, thisTask, cmd_args, payload, setup)
        for dependency in thisTask.dependencies:
            if dependency.statusCode == taskStatus.done:
                continue
            cmd_args.append("-d")
            cmd_args.append("%d" % dependency.jobid)

class DependencyTest(TempDirTests, unittest.TestCase):
    """
    Test the dependency handling of the submissionControllerBase class. This
    test case uses the ToyDepController implementation.
    """
    ###########################################################################
    ## File system dependencies / independent jobs
    @unittest.skip("")
    def test_single_job(self):
        """
        Test the submission behavior of a single task with a non-existing
        output.
        """
        self.assertFalse(op.exists("no_such_file"))
        thisTask = task("mock task 1", "#!/...", time=1.0/60,
                        outputs=["no_such_file"])
        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        controller.submitTasks(args, [thisTask])
        self.assertIn(thisTask.statusCode, [taskStatus.running,
                                            taskStatus.done,
                                            taskStatus.submitted])
        self.assertEqual(thisTask.jobid, 10042)

    @unittest.skip("")
    def test_single_job_no_output(self):
        """
        Test the submission behavior of a single task without any output
        specification. If a job does not list any output file, it should not
        be considered as 'done'
        """
        thisTask = task("mock task 1", "#!/...", time=1.0/60)
        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        controller.submitTasks(args, [thisTask])
        self.assertIn(thisTask.statusCode, [taskStatus.running,
                                            taskStatus.done,
                                            taskStatus.submitted])
        self.assertEqual(thisTask.jobid, 10042)

    @unittest.skip("")
    def test_two_independent_jobs(self):
        """
        Test the submission behavior of two independent jobs.
        """
        if op.exists("no_such_file_1"):
            os.remove("no_such_file_1")

        if op.exists("no_such_file_2"):
            os.remove("no_such_file_2")

        task1 = task("mock task 1", "#!/...", time=10.0/60,
                     outputs=["no_such_file_1"])
        task2 = task("mock task 2", "#!/...", time=10.0/60,
                     outputs=["no_such_file_2"])
        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        start = time.time()
        controller.submitTasks(args, [task1, task2])
        stop = time.time()

        # Make sure that main loop is not emulating dependencies
        self.assertLess(stop - start, 2)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task1)
        controller.updateTaskStatus(args, task2)

        self.assertEqual(task1.statusCode, taskStatus.running)
        self.assertEqual(task2.statusCode, taskStatus.running)

        self.assertEqual(set([task1.jobid, task2.jobid]), set([10042, 10043]))

    @unittest.skip("")
    def test_single_job_w_input_missing_output(self):
        """
        Check that a jobs are submitted when input files are defined and the
        output files are missing.
        """
        self.assertFalse(op.exists("no_such_file"))
        with open("a_file", "w") as f:
            f.write("Very important config file.")

        task1 = task("mock task 1", "#!/...", time=10.0/60,
                     outputs=["no_such_file"], inputs=["a_file"])
        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        controller.submitTasks(args, [task1])

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task1)

        self.assertEqual(task1.statusCode, taskStatus.running)
        self.assertEqual(task1.jobid, 10042)

    @unittest.skip("")
    def test_single_job_w_input_old_output_nots(self):
        """
        Check that a job is not submitted when input files are defined and the
        output file is existent but older than the input not
        checktimestamp=False.
        """
        with open("another_file", "w") as f:
            f.write("Very serious but outdated output.")

        time.sleep(2)

        with open("a_file", "w") as f:
            f.write("Very important config file.")

        task1 = task("mock task 1", "#!/...", time=10.0/60,
                     outputs=["another_file"], inputs=["a_file"])
        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        controller.submitTasks(args, [task1])

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task1.jobid, None)

    @unittest.skip("")
    def test_single_job_w_input_old_output(self):
        """
        Check that a job is submitted when input files are defined and the
        output file is existent but older than the input not
        checktimestamp=True.
        """
        with open("another_file", "w") as f:
            f.write("Very serious but outdated output.")

        time.sleep(1)

        with open("a_file", "w") as f:
            f.write("Very important config file.")

        task1 = task("mock task 1", "#!/...", time=10.0/60,
                     outputs=["another_file"], inputs=["a_file"])
        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        controller.submitTasks(args, [task1])

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task1)

        self.assertEqual(task1.statusCode, taskStatus.running)
        self.assertEqual(task1.jobid, 10042)

    @unittest.skip("")
    def test_single_job_w_input_new_output(self):
        """
        Check that a job is not submitted when input files are defined and the
        output file is existent and newer than the input and
        checktimestamp=True.
        """

        with open("a_file", "w") as f:
            f.write("Very important config file.")

        time.sleep(1)

        with open("another_file", "w") as f:
            f.write("Very serious and up-to-date output.")


        task1 = task("mock task 1", "#!/...", time=10.0/60,
                     outputs=["another_file"], inputs=["a_file"])
        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        controller.submitTasks(args, [task1])

        self.assertEqual(task1.statusCode, taskStatus.done)

    @unittest.skip("")
    def test_single_job_w_input_complex_output_nots(self):
        """
        Check that a job is submitted when input files are defined and the
        three output files are in the following states:
          - non existent
          - older than input
          - newer than input
        and checktimestamp=False.
        """
        with open("output_old", "w") as f:
            f.write("Very serious but outdated output.")

        time.sleep(1)

        with open("a_file", "w") as f:
            f.write("Very important config file.")

        time.sleep(1)

        with open("output_new", "w") as f:
            f.write("Very serious and up-to-date output.")

        self.assertFalse(op.exists("no_such_file"))


        task1 = task("mock task 1", "#!/...", time=10.0/60,
                     outputs=["output_old", "output_new", "no_such_file"],
                     inputs=["a_file"])
        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        controller.submitTasks(args, [task1])

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task1)

        self.assertEqual(task1.statusCode, taskStatus.running)
        self.assertEqual(task1.jobid, 10042)

    @unittest.skip("")
    def test_single_job_w_input_complex_output(self):
        """
        Check that a job is submitted when input files are defined and the
        three output files are in the following states:
          - non existent
          - older than input
          - newer than input
        and checktimestamp=False.
        """
        with open("output_old", "w") as f:
            f.write("Very serious but outdated output.")

        time.sleep(1)

        with open("a_file", "w") as f:
            f.write("Very important config file.")

        time.sleep(1)

        with open("output_new", "w") as f:
            f.write("Very serious and up-to-date output.")

        self.assertFalse(op.exists("no_such_file"))


        task1 = task("mock task 1", "#!/...", time=10.0/60,
                     outputs=["output_old", "output_new", "no_such_file"],
                     inputs=["a_file"])
        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        controller.submitTasks(args, [task1])

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task1)

        self.assertEqual(task1.statusCode, taskStatus.running)
        self.assertEqual(task1.jobid, 10042)

    @unittest.skip("")
    def test_single_job_w_input_output_overlap(self):
        """
        Check that a job is submitted when input there two output and two
        input files with the following modification time line:
            - input 1 (oldest)
            - output 1
            - input 2
            - output 2 (newest)
        For this test checktimestamp is True.
        """
        with open("a_file", "w") as f:
            f.write("Very important config file.")

        time.sleep(1)

        with open("output_old", "w") as f:
            f.write("Very serious but outdated output.")

        time.sleep(1)

        with open("b_file", "w") as f:
            f.write("Other very important config file.")

        time.sleep(1)

        with open("output_new", "w") as f:
            f.write("Very serious and up-to-date output.")

        task1 = task("mock task 1", "#!/...", time=10.0/60,
                     outputs=["output_old", "output_new"],
                     inputs=["a_file", "b_file"])
        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        controller.submitTasks(args, [task1])

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task1)

        self.assertEqual(task1.statusCode, taskStatus.running)
        self.assertEqual(task1.jobid, 10042)

    @unittest.skip("")
    def test_independent_jobs_w_input_complex_output(self):
        """
        Check that only one job is submitted when for the first job:
            input files are defined and the two output files are in the
            following states:
              - older than input
              - newer than input
        and the output of the other job are up-to-data.

        For this test checktimestamp is set to True.
        """
        with open("output_old", "w") as f:
            f.write("Very serious but outdated output.")

        time.sleep(1)

        with open("a_file", "w") as f:
            f.write("Very important config file.")
        with open("b_file", "w") as f:
            f.write("Very important config file of other job.")

        time.sleep(1)

        with open("output_new_a", "w") as f:
            f.write("Very serious and up-to-date output.")
        with open("output_new_b", "w") as f:
            f.write("Very serious and up-to-date output of other job.")

        task1 = task("mock task 1", "#!/...", time=10.0/60,
                     outputs=["output_old", "output_new_a"],
                     inputs=["a_file"])
        task2 = task("mock task 2", "#!/...", time=10.0/60,
                     outputs=["output_new_b"],
                     inputs=["b_file"])

        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        controller.submitTasks(args, [task1, task2])

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task1)

        self.assertEqual(task1.statusCode, taskStatus.running)
        self.assertEqual(task1.jobid, 10042)

        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertEqual(task2.jobid, None)

    ###########################################################################
    ## Real job dependencies, tree (chain) only

    @unittest.skip("")
    def test_dep_two_jobs(self):
        """
        Check that two jobs are submitted with dependencies.
        """
        if op.exists("no_such_file_1"):
            os.remove("no_such_file_1")
        if op.exists("no_such_file_2"):
            os.remove("no_such_file_2")

        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["no_such_file_1"])
        task2 = task("mock task 2", "#!/...", time=1030.0/60,
                     outputs=["no_such_file_2"], dependencies=[task1])

        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        self.create_output_file("no_such_file_1", 2)

        start = time.time()
        controller.submitTasks(args, [task2])
        stop = time.time()

        # Make sure that dependencies are not emulated
        self.assertLess(stop - start, 2)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task2)

        self.assertEqual(task1.statusCode, taskStatus.running)
        self.assertEqual(task1.jobid, 10042)
        self.assertEqual(task2.statusCode, taskStatus.submitted)
        self.assertEqual(task2.jobid, 10043)

        time.sleep(4)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task2)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task1.jobid, 10042)
        self.assertEqual(task2.statusCode, taskStatus.running)
        self.assertEqual(task2.jobid, 10043)

    @unittest.skip("")
    def test_dep_first_job_no_output(self):
        """
        Test the submission behavior when the first job has not inputs. The
        first one should be submitted, the second one should be submitted with
        the first one as a dependency.
        """
        self.assertFalse(op.exists("no_such_file"))
        task_1 = task("mock task 1", "#!/...", time=3.0/60)
        task_2 = task("mock task 2", "#!/...", time=3.0/60,
                      outputs=["no_such_file"], dependencies=[task_1])

        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False,
                              checkmissing=False, ntest=-1, maxConcurrent=-1)

        controller.submitTasks(args, [task_2])

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task_2)

        self.assertEqual(task_1.statusCode, taskStatus.running)
        self.assertEqual(task_2.statusCode, taskStatus.submitted)

        self.assertEqual(task_1.jobid, 10042)
        self.assertEqual(task_2.jobid, 10043)

        time.sleep(4)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task_2)

        self.assertEqual(task_1.statusCode, taskStatus.done)
        self.assertEqual(task_2.statusCode, taskStatus.running)

        self.assertEqual(task_1.jobid, 10042)
        self.assertEqual(task_2.jobid, 10043)

    @unittest.skip("")
    def test_dep_three_jobs_uxx(self):
        """
        Check that the last two jobs in a three-job dependency chain are
        submitted if the output of the center job and the last job are missing.

        (u = up-to-date, x = missing, o = outdated)
        """
        with open("output", "w") as f:
            f.write("Very serious output file file.")

        if op.exists("no_such_file_1"):
            os.remove("no_such_file_1")
        if op.exists("no_such_file_2"):
            os.remove("no_such_file_2")

        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["output"])
        task2 = task("mock task 2", "#!/...", time=3.0/60,
                     outputs=["no_such_file_1"], dependencies=[task1])
        task3 = task("mock task 3", "#!/...", time=3.0/60,
                     outputs=["no_such_file_2"], dependencies=[task2])

        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        self.create_output_file("no_such_file_1", 2)

        start = time.time()
        controller.submitTasks(args, [task3])
        stop = time.time()

        # Make sure that dependencies are not emulated
        self.assertLess(stop - start, 2)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task3)


        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task1.jobid, None)
        self.assertEqual(task2.statusCode, taskStatus.running)
        self.assertEqual(task2.jobid, 10042)
        self.assertEqual(task3.statusCode, taskStatus.submitted)
        self.assertEqual(task3.jobid, 10043)

        time.sleep(4)
        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task3)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertEqual(task3.statusCode, taskStatus.running)

    @unittest.skip("")
    def test_dep_three_jobs_uoo(self):
        """
        Check that the last two jobs in a three-job dependency chain are
        submitted if the output of the last two jobs are outdated.

        (u = up-to-date, x = missing, o = outdated)
        """
        with open("output_1", "w") as f:
            f.write("Very serious output file.")
        time.sleep(1)
        with open("output_2", "w") as f:
            f.write("Very serious output file.")
        time.sleep(1)
        with open("output_3", "w") as f:
            f.write("Very serious output file.")

        time.sleep(1)
        with open("a_file", "w") as f:
            f.write("Very important config file.")

        self.assertFalse(op.exists("no_such_file"))
        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["output_1"])
        task2 = task("mock task 2", "#!/...", time=3.0/60,
                     inputs=["a_file"],
                     outputs=["output_2"], dependencies=[task1])
        task3 = task("mock task 3", "#!/...", time=3.0/60,
                     inputs=["a_file"],
                     outputs=["output_3"], dependencies=[task2])

        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        self.create_output_file("output_2", 2)

        start = time.time()
        controller.submitTasks(args, [task3])
        stop = time.time()

        # Make sure that dependencies are not emulated
        self.assertLess(stop - start, 2)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task3)


        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task1.jobid, None)
        self.assertEqual(task2.statusCode, taskStatus.running)
        self.assertEqual(task2.jobid, 10042)
        self.assertEqual(task3.statusCode, taskStatus.submitted)
        self.assertEqual(task3.jobid, 10043)

        time.sleep(4)
        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task3)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertEqual(task3.statusCode, taskStatus.running)

    @unittest.skip("")
    def test_dep_three_jobs_uou(self):
        """
        Check that the last two jobs in a three-job dependency chain are
        submitted if the output of the last two jobs are outdated.

        (u = up-to-date, x = missing, o = outdated)
        """
        with open("output_1", "w") as f:
            f.write("Very serious output file.")
        time.sleep(1)
        with open("output_2", "w") as f:
            f.write("Very serious output file.")
        time.sleep(1)
        with open("output_3", "w") as f:
            f.write("Very serious output file.")

        time.sleep(1)
        with open("a_file", "w") as f:
            f.write("Very important config file.")

        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["output_1"])
        task2 = task("mock task 2", "#!/...", time=3.0/60,
                     inputs=["a_file"],
                     outputs=["output_2"], dependencies=[task1])
        task3 = task("mock task 3", "#!/...", time=3.0/60,
                     outputs=["output_3"], dependencies=[task2])

        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)


        self.create_output_file("output_2", 2)
        self.create_output_file("output_3", 5)

        start = time.time()
        controller.submitTasks(args, [task3])
        stop = time.time()

        # Make sure that dependencies are not emulated
        self.assertLess(stop - start, 2)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task3)


        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task1.jobid, None)
        self.assertEqual(task2.statusCode, taskStatus.running)
        self.assertEqual(task2.jobid, 10042)
        self.assertEqual(task3.statusCode, taskStatus.submitted)
        self.assertEqual(task3.jobid, 10043)

        time.sleep(4)
        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task3)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertEqual(task3.statusCode, taskStatus.running)


    ###########################################################################
    ## Diamond shaped task graph.
    #
    # The initial status of all jobs are indicated in the method name. The
    # following states abbreviations are used.
    #   u = up-to-date
    #   x = missing
    #   o = outdated
    #
    # The jobs are numbered as follows:
    #       .-- 2 <--.
    # 1 <--+          +-- 4
    #       `-- 3 <--`
    #
    # In this example, job 4 depends directly on job 2 and 3. Job does not
    # depend on any other job.
    #
    # A test case contains the string 'uuxo' if
    #  - the output of job 1 are up-to-date,
    #  - the output of job 2 are up-to-date,
    #  - the output of 3 is missing and
    #  - the output of job 4 is out dated.

#     # See issue 167
#    def test_dep_diamond_xxxx(self):
#         """
#         Check that all jobs are submitted, if all outputs are missing.
#         """
#         for no_such_file in ["no_such_file_%d" % i for i in [1, 2, 3, 4]]:
#             if op.exists(no_such_file):
#                 os.remove(no_such_file)
#
#         task1 = task("mock task 1", "#!/...", time=3.0/60,
#                      outputs=["no_such_file_1"])
#         task2 = task("mock task 2", "#!/...", time=3.0/60,
#                      outputs=["no_such_file_2"], dependencies=[task1])
#         task3 = task("mock task 3", "#!/...", time=3.0/60,
#                      outputs=["no_such_file_3"], dependencies=[task1])
#         task4 = task("mock task 4", "#!/...", time=3.0/60,
#                      outputs=["no_such_file_4"], dependencies=[task2, task3])
#
#         controller = ToyDepController()
#
#         args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
#                              checkmissing=False, ntest=-1, maxConcurrent=-1)
#
#         self.create_output_file("no_such_file_1", 2)
#         self.create_output_file("no_such_file_2", 5)
#         self.create_output_file("no_such_file_3", 5)
#         self.create_output_file("no_such_file_4", 8)
#
#         start = time.time()
#         controller.submitTasks(args, [task4])
#         stop = time.time()
#
#         self.assertEqual(task1.jobid, 10042)
#         self.assertEqual(task2.jobid, 10043)
#         self.assertEqual(task3.jobid, 10044)
#         self.assertEqual(task4.jobid, 10045)
#
#         # Make sure that dependencies are not emulated
#         self.assertLess(stop - start, 2)
#
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.statusCode, taskStatus.running)
#         self.assertEqual(task2.statusCode, taskStatus.submitted)
#         self.assertEqual(task3.statusCode, taskStatus.submitted)
#         self.assertEqual(task4.statusCode, taskStatus.submitted)
#
#         time.sleep(4)
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.running)
#         self.assertEqual(task3.statusCode, taskStatus.running)
#         self.assertEqual(task4.statusCode, taskStatus.submitted)
#
#         time.sleep(3)
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.done)
#         self.assertEqual(task3.statusCode, taskStatus.done)
#         self.assertEqual(task4.statusCode, taskStatus.running)

#     # See issue 167
#    def test_dep_diamond_uuuu(self):
#         """
#         Check that no job are submitted, if all outputs are up-to-date.
#         """
#         for i in range(0,4): #need individual files with correct ordering of timestamps
#           with open("output_{:d}".format(i+1), "w") as f:
#               f.write("m(Higgs) = 125")
#           time.sleep(1)
#
#         task1 = task("mock task 1", "#!/...", time=3.0/60,
#                      outputs=["output_1"])
#         task2 = task("mock task 2", "#!/...", time=3.0/60,
#                      outputs=["output_2"], dependencies=[task1])
#         task3 = task("mock task 3", "#!/...", time=3.0/60,
#                      outputs=["output_3"], dependencies=[task1])
#         task4 = task("mock task 4", "#!/...", time=3.0/60,
#                      outputs=["output_4"], dependencies=[task2, task3])
#
#         controller = ToyDepController()
#
#         args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
#                              checkmissing=False, ntest=-1, maxConcurrent=-1)
#
#         start = time.time()
#         controller.submitTasks(args, [task4])
#         stop = time.time()
#
#         # Make sure that dependencies are not emulated
#         self.assertLess(stop - start, 2)
#
#
#         self.assertEqual(task1.jobid, None)
#         self.assertEqual(task2.jobid, None)
#         self.assertEqual(task3.jobid, None)
#         self.assertEqual(task4.jobid, None)
#
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.done)
#         self.assertEqual(task3.statusCode, taskStatus.done)
#         self.assertEqual(task4.statusCode, taskStatus.done)
#
#    def test_dep_diamond_xuuu(self):
#         """
#         Check that all jobs are submitted if the initial job output has to be
#         created.
#         """
#         if op.exists("output_1"):
#             os.remove("output_1")
#
#         with open("output_2", "w") as f:
#             f.write("m(Higgs) = 125")
#         time.sleep(1)
#
#         with open("output_3", "w") as f:
#             f.write("m(Higgs) = 125")
#         time.sleep(1)
#
#         with open("output_4", "w") as f:
#             f.write("m(Higgs) = 125")
#
#         task1 = task("mock task 1", "#!/...", time=3.0/60,
#                      outputs=["output_1"])
#         task2 = task("mock task 2", "#!/...", time=3.0/60,
#                      outputs=["output_2"], dependencies=[task1])
#         task3 = task("mock task 3", "#!/...", time=3.0/60,
#                      outputs=["output_3"], dependencies=[task1])
#         task4 = task("mock task 4", "#!/...", time=3.0/60,
#                      outputs=["output_4"], dependencies=[task2, task3])
#
#         controller = ToyDepController()
#
#         args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
#                              checkmissing=False, ntest=-1, maxConcurrent=-1)
#
#         self.create_output_file("output_1", 2)
#         self.create_output_file("output_2", 5)
#         self.create_output_file("output_3", 5)
#         self.create_output_file("output_4", 8)
#
#
#         start = time.time()
#         controller.submitTasks(args, [task4])
#         stop = time.time()
#
#         # Make sure that dependencies are not emulated
#         self.assertLess(stop - start, 2)
#
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.jobid, 10042)
#         self.assertEqual(task2.jobid, 10043)
#         self.assertEqual(task3.jobid, 10044)
#         self.assertEqual(task4.jobid, 10045)
#
#         self.assertEqual(task1.statusCode, taskStatus.running)
#         self.assertEqual(task2.statusCode, taskStatus.submitted)
#         self.assertEqual(task3.statusCode, taskStatus.submitted)
#         self.assertEqual(task4.statusCode, taskStatus.submitted)
#
#         time.sleep(4)
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.running)
#         self.assertEqual(task3.statusCode, taskStatus.running)
#         self.assertEqual(task4.statusCode, taskStatus.submitted)
#
#         time.sleep(3)
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.done)
#         self.assertEqual(task3.statusCode, taskStatus.done)
#         self.assertEqual(task4.statusCode, taskStatus.running)

#     # See issue 167
#    def test_dep_diamond_ouuu(self):
#         """
#         check that all jobs are submitted if the initial job output is
#         outdated.
#         """
#         self.assertFalse(op.exists("no_such_file"))
#
#         with open("output_1", "w") as f:
#             f.write("m(higgs) = 125")
#
#         time.sleep(1)
#         with open("a_file", "w") as f:
#             f.write("master config.")
#
#         with open("output_2", "w") as f:
#             f.write("m(higgs) = 125")
#         with open("output_3", "w") as f:
#             f.write("m(higgs) = 125")
#         time.sleep(1)
#         with open("output_4", "w") as f:
#             f.write("m(higgs) = 125")
#
#         task1 = task("mock task 1", "#!/...", time=3.0/60,
#                      inputs=["a_file"],
#                      outputs=["output_1"])
#         task2 = task("mock task 2", "#!/...", time=3.0/60,
#                      outputs=["output_2"], dependencies=[task1])
#         task3 = task("mock task 3", "#!/...", time=3.0/60,
#                      outputs=["output_3"], dependencies=[task1])
#         task4 = task("mock task 4", "#!/...", time=3.0/60,
#                      outputs=["output_4"], dependencies=[task2, task3])
#
#         controller = ToyDepController()
#
#         args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
#                              checkmissing=False, ntest=-1, maxConcurrent=-1)
#
#
#         self.create_output_file("output_1", 2)
#         self.create_output_file("output_2", 5)
#         self.create_output_file("output_3", 5)
#         self.create_output_file("output_4", 8)
#
#         start = time.time()
#         controller.submitTasks(args, [task4])
#         stop = time.time()
#
#         # make sure that dependencies are not emulated
#         self.assertLess(stop - start, 3)
#
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.jobid, 10042)
#         self.assertEqual(task2.jobid, 10043)
#         self.assertEqual(task3.jobid, 10044)
#         self.assertEqual(task4.jobid, 10045)
#
#
#         self.assertEqual(task1.statusCode, taskStatus.running)
#         self.assertEqual(task2.statusCode, taskStatus.submitted)
#         self.assertEqual(task3.statusCode, taskStatus.submitted)
#         self.assertEqual(task4.statusCode, taskStatus.submitted)
#
#         time.sleep(4)
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.running)
#         self.assertEqual(task3.statusCode, taskStatus.running)
#         self.assertEqual(task4.statusCode, taskStatus.submitted)
#
#         time.sleep(3)
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.done)
#         self.assertEqual(task3.statusCode, taskStatus.done)
#         self.assertEqual(task4.statusCode, taskStatus.running)
#
#    def test_dep_diamond_uxuu(self):
#         """
#         Check that the jobs 2 and 4 are submitted if the output of job 2 is
#         missing.
#         """
#         if op.exists("output_2"):
#             os.remove("output_2")
#
#         with open("output_1", "w") as f:
#             f.write("m(higgs) = 125")
#         time.sleep(1)
#         with open("output_3", "w") as f:
#             f.write("m(higgs) = 125")
#         time.sleep(1)
#         with open("output_4", "w") as f:
#             f.write("m(higgs) = 125")
#
#         task1 = task("mock task 1", "#!/...", time=3.0/60,
#                      outputs=["output_1"])
#         task2 = task("mock task 2", "#!/...", time=3.0/60,
#                      outputs=["output_2"], dependencies=[task1])
#         task3 = task("mock task 3", "#!/...", time=3.0/60,
#                      outputs=["output_3"], dependencies=[task1])
#         task4 = task("mock task 4", "#!/...", time=3.0/60,
#                      outputs=["output_4"], dependencies=[task2, task3])
#
#         controller = ToyDepController()
#
#         args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
#                              checkmissing=False, ntest=-1, maxConcurrent=-1)
#
#         self.create_output_file("output_2", 2)
#         self.create_output_file("output_4", 5)
#
#         start = time.time()
#         controller.submitTasks(args, [task4])
#         stop = time.time()
#
#         # Make sure that dependencies are not emulated
#         self.assertLess(stop - start, 2)
#
#         self.assertEqual(task1.jobid, None)
#         self.assertEqual(task2.jobid, 10042)
#         self.assertEqual(task3.jobid, None)
#         self.assertEqual(task4.jobid, 10043)
#
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.running)
#         self.assertEqual(task3.statusCode, taskStatus.done)
#         self.assertEqual(task4.statusCode, taskStatus.submitted)
#
#         time.sleep(4)
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.done)
#         self.assertEqual(task3.statusCode, taskStatus.done)
#         self.assertEqual(task4.statusCode, taskStatus.running)

#     # See Issue 167
#    def test_dep_diamond_uouu(self):
#         """
#         Check that the jobs 2 and 4 are submitted if the output of job 2 is
#         out dated.
#         """
#         with open("output_1", "w") as f:
#             f.write("m(higgs) = 125")
#         time.sleep(1)
#         with open("output_2", "w") as f:
#             f.write("m(higgs) = 125")
#         with open("output_3", "w") as f:
#             f.write("m(higgs) = 125")
#         time.sleep(1)
#         with open("output_4", "w") as f:
#             f.write("m(higgs) = 125")
#
#         time.sleep(1)
#         with open("a_file", "w") as f:
#             f.write("master config.")
#
#         task1 = task("mock task 1", "#!/...", time=3.0/60,
#                      outputs=["output_1"])
#         task2 = task("mock task 2", "#!/...", time=3.0/60,
#                      inputs=["a_file"],
#                      outputs=["output_2"], dependencies=[task1])
#         task3 = task("mock task 3", "#!/...", time=3.0/60,
#                      outputs=["output_3"], dependencies=[task1])
#         task4 = task("mock task 4", "#!/...", time=3.0/60,
#                      outputs=["output_4"], dependencies=[task2, task3])
#
#         controller = ToyDepController()
#
#         args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
#                              checkmissing=False, ntest=-1, maxConcurrent=-1)
#
#         self.create_output_file("output_2", 2)
#         self.create_output_file("output_4", 5)
#
#         start = time.time()
#         controller.submitTasks(args, [task4])
#         stop = time.time()
#
#         # Make sure that dependencies are not emulated
#         self.assertLess(stop - start, 2)
#
#         self.assertEqual(task1.jobid, None)
#         self.assertEqual(task2.jobid, 10042)
#         self.assertEqual(task3.jobid, None)
#         self.assertEqual(task4.jobid, 10043)
#
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.running)
#         self.assertEqual(task3.statusCode, taskStatus.done)
#         self.assertEqual(task4.statusCode, taskStatus.submitted)
#
#         time.sleep(4)
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.done)
#         self.assertEqual(task3.statusCode, taskStatus.done)
#         self.assertEqual(task4.statusCode, taskStatus.running)

    ###########################################################################
    ## Real job dependencies, tree (chain) only with emulated dependencies

    @unittest.skip("")
    def test_nodep_two_jobs(self):
        """
        Check that two jobs are submitted with emulated dependencies.
        """
        if op.exists("no_such_file_1"):
            os.remove("no_such_file_1")
        if op.exists("no_such_file_2"):
            os.remove("no_such_file_2")

        self.assertFalse(op.exists("no_such_file"))
        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["no_such_file_1"])
        task2 = task("mock task 2", "#!/...", time=3.0/60,
                     outputs=["no_such_file_2"], dependencies=[task1])

        controller = ToyNoDepController()
        controller.poll_delay = 1

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        self.create_output_file("no_such_file_1", 2)

        start = time.time()
        controller.submitTasks(args, [task2])
        stop = time.time()

        # Make sure that dependencies are emulated
        self.assertGreater(stop - start, 3)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task2)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task1.jobid, 10042)
        self.assertEqual(task2.statusCode, taskStatus.running)
        self.assertEqual(task2.jobid, 10043)

    @unittest.skip("")
    def test_nodep_first_job_no_output(self):
        """
        Test the submission behavior when the first job has not inputs. The
        first one should be submitted, the second one should be submitted with
        the first one as a dependency.
        """
        self.assertFalse(op.exists("no_such_file"))
        task_1 = task("mock task 1", "#!/...", time=3.0/60)
        task_2 = task("mock task 2", "#!/...", time=3.0/60,
                      outputs=["no_such_file"], dependencies=[task_1])

        controller = ToyNoDepController()
        controller.poll_delay = 1

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=False,
                              checkmissing=False, ntest=-1, maxConcurrent=-1)

        start = time.time()
        controller.submitTasks(args, [task_2])
        stop = time.time()

        # Make sure that dependencies are emulated
        self.assertGreater(stop - start, 3)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task_2)

        self.assertEqual(task_1.statusCode, taskStatus.done)
        self.assertEqual(task_2.statusCode, taskStatus.running)

        self.assertEqual(task_1.jobid, 10042)
        self.assertEqual(task_2.jobid, 10043)


    @unittest.skip("")
    def test_nodep_two_quick_jobs_no_output(self):
        """
        Check that two jobs are submitted with emulated dependencies when the
        jobs finish before the queue is polled. The submission should fail if
        the missing output is not created.
        """
        self.assertFalse(op.exists("no_such_file"))
        task1 = task("mock task 1", "#!/...", time=1.0/60,
                     outputs=["no_such_file"])
        task2 = task("mock task 2", "#!/...", time=10.0/60,
                     outputs=["no_such_file"], dependencies=[task1])

        controller = ToyNoDepController()
        controller.poll_delay = 3

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        self.assertRaises(RuntimeError, controller.submitTasks, args,
                          [task2])

    def create_output_file(self, path, delay):
        """
        This method launches an external thread which creates the specified
        file after the predetermined delay.
        """
        timestamp = time.time() + delay

        def internal():
            """
            Creates the requested file at the given time stamp.
            """
            time.sleep(timestamp - time.time())
            with open(path, "w") as f:
                f.write("File created in a thread.")

        thread = threading.Thread(target=internal)
        thread.start()

    @unittest.skip("")
    def test_create_output_file(self):
        """
        Check that the requested output file does not exist before the
        requested time. However, check that file exists after the time stamp.
        """

        if op.exists("delayed_file"):
            os.remove("delayed_file")

        self.create_output_file("delayed_file", 1)

        time.sleep(0.8)
        self.assertFalse(op.exists("delayed_file"))

        time.sleep(0.4)
        self.assertTrue(op.exists("delayed_file"))

    @unittest.skip("")
    def test_nodep_two_quick_jobs_w_output(self):
        """
        Check that two jobs are submitted with emulated dependencies when the
        jobs finish before the queue is polled.

        This test case is a bit dirty. We need to create the output file,
        while task 1 is running, before the controller polls the queue. The
        file will be created in a separate thread after 2s.
        """
        self.assertFalse(op.exists("no_such_file"))
        if op.exists("no_such_file_tbg"):
            os.remove("no_such_file_tbg")
        task1 = task("mock task 1", "#!/...", time=1.0/60,
                     outputs=["no_such_file_tbg"])
        task2 = task("mock task 2", "#!/...", time=10.0/60,
                     outputs=["no_such_file"], dependencies=[task1])

        controller = ToyNoDepController()
        controller.poll_delay = 3

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        self.create_output_file("no_such_file_tbg", 2)

        start = time.time()
        controller.submitTasks(args, [task2])
        stop = time.time()

        # Make sure that dependencies are emulated
        self.assertGreater(stop - start, 3)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task2)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task1.jobid, 10042)
        self.assertEqual(task2.statusCode, taskStatus.running)
        self.assertEqual(task2.jobid, 10043)

    @unittest.skip("")
    def test_nodep_three_jobs_uxx(self):
        """
        Check that the last two jobs in a three-job dependency chain are
        submitted if the output of the center job and the last job are missing.

        (u = up-to-date, x = missing, o = outdated)
        """
        if op.exists("no_such_file_1"):
            os.remove("no_such_file_1")
        if op.exists("no_such_file_2"):
            os.remove("no_such_file_2")

        with open("output", "w") as f:
            f.write("Very serious output file file.")

        self.assertFalse(op.exists("no_such_file"))
        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["output"])
        task2 = task("mock task 2", "#!/...", time=3.0/60,
                     outputs=["no_such_file_1"], dependencies=[task1])
        task3 = task("mock task 3", "#!/...", time=3.0/60,
                     outputs=["no_such_file_2"], dependencies=[task2])

        controller = ToyNoDepController()
        controller.poll_delay = 1

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        self.create_output_file("no_such_file_1", 2)

        start = time.time()
        controller.submitTasks(args, [task3])
        stop = time.time()

        # Make sure that dependencies are emulated
        self.assertGreater(stop - start, 3)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task3)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task1.jobid, None)
        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertEqual(task2.jobid, 10042)
        self.assertEqual(task3.statusCode, taskStatus.running)
        self.assertEqual(task3.jobid, 10043)

    @unittest.skip("")
    def test_nodep_three_jobs_uoo(self):
        """
        Check that the last two jobs in a three-job dependency chain are
        submitted if the output of the last two jobs are outdated.

        (u = up-to-date, x = missing, o = outdated)
        """
        with open("output_1", "w") as f:
            f.write("Very serious output file.")
        time.sleep(1)
        with open("output_2", "w") as f:
            f.write("Very serious output file.")
        with open("output_3", "w") as f:
            f.write("Very serious output file.")

        time.sleep(1)
        with open("a_file", "w") as f:
            f.write("Very important config file.")

        self.assertFalse(op.exists("no_such_file"))
        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["output_1"])
        task2 = task("mock task 2", "#!/...", time=3.0/60,
                     inputs=["a_file"],
                     outputs=["output_2"], dependencies=[task1])
        task3 = task("mock task 3", "#!/...", time=3.0/60,
                     inputs=["a_file"],
                     outputs=["output_3"], dependencies=[task2])

        controller = ToyNoDepController()
        controller.poll_delay = 1

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        self.create_output_file("output_2", 2)

        start = time.time()
        controller.submitTasks(args, [task3])
        stop = time.time()

        # Make sure that dependencies are emulated
        self.assertLess(stop - start, 6)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task3)


        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task1.jobid, None)
        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertEqual(task2.jobid, 10042)
        self.assertEqual(task3.statusCode, taskStatus.running)
        self.assertEqual(task3.jobid, 10043)

    @unittest.skip("")
    def test_nodep_three_jobs_uou(self):
        """
        Check that the last two jobs in a three-job dependency chain are
        submitted if the output of the last two jobs are outdated.

        (u = up-to-date, x = missing, o = outdated)
        """
        with open("output_1", "w") as f:
            f.write("Very serious output file.")
        time.sleep(1)
        with open("output_2", "w") as f:
            f.write("Very serious output file.")
        with open("output_3", "w") as f:
            f.write("Very serious output file.")

        time.sleep(1)
        with open("a_file", "w") as f:
            f.write("Very important config file.")

        self.assertFalse(op.exists("no_such_file"))
        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["output_1"])
        task2 = task("mock task 2", "#!/...", time=3.0/60,
                     inputs=["a_file"],
                     outputs=["output_2"], dependencies=[task1])
        task3 = task("mock task 3", "#!/...", time=3.0/60,
                     outputs=["output_3"], dependencies=[task2])

        controller = ToyNoDepController()
        controller.poll_delay = 1

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        self.create_output_file("output_2", 2)
        self.create_output_file("output_3", 5)

        start = time.time()
        controller.submitTasks(args, [task3])
        stop = time.time()

        # Make sure that dependencies are emulated
        self.assertLess(stop - start, 6)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task3)


        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task1.jobid, None)
        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertEqual(task2.jobid, 10042)
        self.assertEqual(task3.statusCode, taskStatus.running)
        self.assertEqual(task3.jobid, 10043)

    ###########################################################################
    ## Diamond shaped task graph with emulated dependencies, see note above.

    @unittest.skip("")
    def test_nodep_diamond_xxxx(self):
        """
        Check that all jobs are submitted, if all outputs are missing.
        """
        for no_such_file in ["no_such_file_%d" % i for i in [1, 2, 3, 4]]:
            if op.exists(no_such_file):
                os.remove(no_such_file)

        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["no_such_file_1"])
        task2 = task("mock task 2", "#!/...", time=3.0/60,
                     outputs=["no_such_file_2"], dependencies=[task1])
        task3 = task("mock task 3", "#!/...", time=3.0/60,
                     outputs=["no_such_file_3"], dependencies=[task1])
        task4 = task("mock task 4", "#!/...", time=3.0/60,
                     outputs=["no_such_file_4"], dependencies=[task2, task3])

        controller = ToyNoDepController()
        controller.poll_delay = 1

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        self.create_output_file("no_such_file_1", 2)
        self.create_output_file("no_such_file_2", 5)
        self.create_output_file("no_such_file_3", 5)
        self.create_output_file("no_such_file_4", 28)

        start = time.time()
        controller.submitTasks(args, [task4])
        stop = time.time()

        # Make sure that dependencies are emulated
        self.assertGreater(stop - start, 6)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task4)

        self.assertEqual(task1.jobid, 10042)
        self.assertEqual(task2.jobid, 10043)
        self.assertEqual(task3.jobid, 10044)
        self.assertEqual(task4.jobid, 10045)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertEqual(task3.statusCode, taskStatus.done)
        self.assertEqual(task4.statusCode, taskStatus.running)

#     # See issue 167
#    def test_nodep_diamond_uuuu(self):
#         """
#         Check that no job are submitted, if all outputs are up-to-date.
#         """
#         for i in range(0,4): #need individual files with correct ordering of timestamps
#           with open("output_{:d}".format(i+1), "w") as f:
#               f.write("m(Higgs) = 125")
#           time.sleep(1)
#
#         task1 = task("mock task 1", "#!/...", time=3.0/60,
#                      outputs=["output_1"])
#         task2 = task("mock task 2", "#!/...", time=3.0/60,
#                      outputs=["output_2"], dependencies=[task1])
#         task3 = task("mock task 3", "#!/...", time=3.0/60,
#                      outputs=["output_3"], dependencies=[task1])
#         task4 = task("mock task 4", "#!/...", time=3.0/60,
#                      outputs=["output_4"], dependencies=[task2, task3])
#
#         controller = ToyNoDepController()
#         controller.poll_delay = 1
#
#         args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
#                              checkmissing=False, ntest=-1, maxConcurrent=-1)
#
#         start = time.time()
#         controller.submitTasks(args, [task4])
#         stop = time.time()
#
#         # Make sure that the controller did not wait for a job
#         self.assertLess(stop - start, 2)
#
#         self.assertEqual(task1.jobid, None)
#         self.assertEqual(task2.jobid, None)
#         self.assertEqual(task3.jobid, None)
#         self.assertEqual(task4.jobid, None)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.done)
#         self.assertEqual(task3.statusCode, taskStatus.done)
#         self.assertEqual(task4.statusCode, taskStatus.done)

    @unittest.skip("")
    def test_nodep_diamond_xuuu(self):
        """
        Check that all jobs are submitted if the initial job output has to be
        created.
        """
        if op.exists("output_1"):
            os.remove("output_1")

        with open("output_2", "w") as f:
            f.write("m(Higgs) = 125")
        with open("output_3", "w") as f:
            f.write("m(Higgs) = 125")
        time.sleep(1)
        with open("output_4", "w") as f:
            f.write("m(Higgs) = 125")

        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["output_1"])
        task2 = task("mock task 2", "#!/...", time=3.0/60,
                     outputs=["output_2"], dependencies=[task1])
        task3 = task("mock task 3", "#!/...", time=3.0/60,
                     outputs=["output_3"], dependencies=[task1])
        task4 = task("mock task 4", "#!/...", time=3.0/60,
                     outputs=["output_4"], dependencies=[task2, task3])

        controller = ToyNoDepController()
        controller.poll_delay = 1

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        self.create_output_file("output_1", 2)
        self.create_output_file("output_2", 5)
        self.create_output_file("output_3", 5)
        self.create_output_file("output_4", 8)

        start = time.time()
        controller.submitTasks(args, [task4])
        stop = time.time()

        # Make sure that dependencies are emulated
        self.assertGreater(stop - start, 6)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task4)

        self.assertEqual(task1.jobid, 10042)
        self.assertEqual(task2.jobid, 10043)
        self.assertEqual(task3.jobid, 10044)
        self.assertEqual(task4.jobid, 10045)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertEqual(task3.statusCode, taskStatus.done)
        self.assertEqual(task4.statusCode, taskStatus.running)


    @unittest.skip("")
    def test_nodep_diamond_ouuu(self):
        """
        check that all jobs are submitted if the initial job output is
        outdated.
        """
        with open("output_1", "w") as f:
            f.write("m(higgs) = 125")

        time.sleep(1)
        with open("a_file", "w") as f:
            f.write("master config.")

        with open("output_2", "w") as f:
            f.write("m(higgs) = 125")
        with open("output_3", "w") as f:
            f.write("m(higgs) = 125")
        time.sleep(1)
        with open("output_4", "w") as f:
            f.write("m(higgs) = 125")

        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     inputs=["a_file"],
                     outputs=["output_1"])
        task2 = task("mock task 2", "#!/...", time=3.0/60,
                     outputs=["output_2"], dependencies=[task1])
        task3 = task("mock task 3", "#!/...", time=3.0/60,
                     outputs=["output_3"], dependencies=[task1])
        task4 = task("mock task 4", "#!/...", time=3.0/60,
                     outputs=["output_4"], dependencies=[task2, task3])

        controller = ToyNoDepController()
        controller.poll_delay = 1

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        self.create_output_file("output_1", 2)
        self.create_output_file("output_2", 5)
        self.create_output_file("output_3", 5)
        self.create_output_file("output_4", 8)

        start = time.time()
        controller.submitTasks(args, [task4])
        stop = time.time()

        # Make sure that dependencies are emulated
        self.assertGreater(stop - start, 3)  # Waiting only for central jobs

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task4)

        self.assertEqual(task1.jobid, 10042)
        self.assertEqual(task2.jobid, 10043)
        self.assertEqual(task3.jobid, 10044)
        self.assertEqual(task4.jobid, 10045)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task2.statusCode, taskStatus.done)
        self.assertEqual(task3.statusCode, taskStatus.done)
        self.assertEqual(task4.statusCode, taskStatus.running)


#     See issue 167
#    def test_nodep_diamond_uxuu(self):
#         """
#         Check that the jobs 2 and 4 are submitted if the output of job 2 is
#         missing.
#         """
#         if op.exists("output_2"):
#             os.remove("output_2")
#
#         with open("output_1", "w") as f:
#             f.write("m(higgs) = 125")
#         time.sleep(1)
#         with open("output_3", "w") as f:
#             f.write("m(higgs) = 125")
#         time.sleep(1)
#         with open("output_4", "w") as f:
#             f.write("m(higgs) = 125")
#
#         task1 = task("mock task 1", "#!/...", time=3.0/60,
#                      outputs=["output_1"])
#         task2 = task("mock task 2", "#!/...", time=3.0/60,
#                      outputs=["output_2"], dependencies=[task1])
#         task3 = task("mock task 3", "#!/...", time=3.0/60,
#                      outputs=["output_3"], dependencies=[task1])
#         task4 = task("mock task 4", "#!/...", time=3.0/60,
#                      outputs=["output_4"], dependencies=[task2, task3])
#
#         controller = ToyNoDepController()
#         controller.poll_delay = 1
#
#         args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
#                              checkmissing=False, ntest=-1, maxConcurrent=-1)
#
#         self.create_output_file("output_2", 2)
#         self.create_output_file("output_4", 5)
#
#         start = time.time()
#         controller.submitTasks(args, [task4])
#         stop = time.time()
#
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         # Make sure that dependencies are emulated
#         self.assertGreater(stop - start, 3)
#
#         self.assertEqual(task1.jobid, None)
#         self.assertEqual(task2.jobid, 10042)
#         self.assertEqual(task3.jobid, None)
#         self.assertEqual(task4.jobid, 10043)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.done)
#         self.assertEqual(task3.statusCode, taskStatus.done)
#         self.assertEqual(task4.statusCode, taskStatus.running)

#     # See issue 167
#    def test_nodep_diamond_uouu(self):
#         """
#         Check that the jobs 2 and 4 are submitted if the output of job 2 is
#         out dated.
#         """
#         with open("output_1", "w") as f:
#             f.write("m(higgs) = 125")
#         time.sleep(1)
#         with open("output_2", "w") as f:
#             f.write("m(higgs) = 125")
#         with open("output_3", "w") as f:
#             f.write("m(higgs) = 125")
#         time.sleep(1)
#         with open("output_4", "w") as f:
#             f.write("m(higgs) = 125")
#
#         time.sleep(1)
#         with open("a_file", "w") as f:
#             f.write("master config.")
#
#         task1 = task("mock task 1", "#!/...", time=3.0/60,
#                      outputs=["output_1"])
#         task2 = task("mock task 2", "#!/...", time=3.0/60,
#                      inputs=["a_file"],
#                      outputs=["output_2"], dependencies=[task1])
#         task3 = task("mock task 3", "#!/...", time=3.0/60,
#                      outputs=["output_3"], dependencies=[task1])
#         task4 = task("mock task 4", "#!/...", time=3.0/60,
#                      outputs=["output_4"], dependencies=[task2, task3])
#
#         controller = ToyNoDepController()
#         controller.poll_delay = 1
#
#         args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
#                              checkmissing=False, ntest=-1, maxConcurrent=-1)
#
#         self.create_output_file("output_2", 2)
#         self.create_output_file("output_4", 5)
#
#         start = time.time()
#         controller.submitTasks(args, [task4])
#         stop = time.time()
#
#         controller._updateQueueStatus()
#         controller.updateTaskStatus(args, task4)
#
#         # Make sure that dependencies are emulated
#         self.assertGreater(stop - start, 3)
#
#         self.assertEqual(task1.jobid, None)
#         self.assertEqual(task2.jobid, 10042)
#         self.assertEqual(task3.jobid, None)
#         self.assertEqual(task4.jobid, 10043)
#
#         self.assertEqual(task1.statusCode, taskStatus.done)
#         self.assertEqual(task2.statusCode, taskStatus.done)
#         self.assertEqual(task3.statusCode, taskStatus.done)
#         self.assertEqual(task4.statusCode, taskStatus.running)

    ###########################################################################
    ## Other
    @unittest.skip("")
    def test_dep_ntest(self):
        """
        Check that the ntest flag is obeyed. The test layout is xxxx.
        """
        self.assertFalse(op.exists("no_such_file"))
        if op.exists("output_1"):
            os.remove("output_1")

        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["output_1"])
        task2 = task("mock task 2", "#!/...", time=3.0/60,
                     outputs=["no_such_file"], dependencies=[task1])
        task3 = task("mock task 3", "#!/...", time=3.0/60,
                     outputs=["no_such_file"], dependencies=[task1])
        task4 = task("mock task 4", "#!/...", time=3.0/60,
                     outputs=["no_such_file"], dependencies=[task2, task3])

        controller = ToyDepController()

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=1, maxConcurrent=-1)

        self.create_output_file("output_1", 2)

        start = time.time()
        controller.submitTasks(args, [task4])
        stop = time.time()

        time.sleep(4)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task4)

        # Make sure that dependencies are not emulated
        self.assertLess(stop - start, 3)

        self.assertEqual(task1.jobid, 10042)
        self.assertEqual(task2.jobid, None)
        self.assertEqual(task3.jobid, None)
        self.assertEqual(task4.jobid, None)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task2.statusCode, taskStatus.tosubmit)
        self.assertEqual(task3.statusCode, taskStatus.tosubmit)
        self.assertEqual(task4.statusCode, taskStatus.tosubmit)

    @unittest.skip("")
    def test_nodep_ntest(self):
        """
        Check that the ntest flag is obeyed. The test layout is xxxx.
        """
        self.assertFalse(op.exists("no_such_file"))
        if op.exists("output_1"):
            os.remove("output_1")

        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["output_1"])
        task2 = task("mock task 2", "#!/...", time=3.0/60,
                     outputs=["no_such_file"], dependencies=[task1])
        task3 = task("mock task 3", "#!/...", time=3.0/60,
                     outputs=["no_such_file"], dependencies=[task1])
        task4 = task("mock task 4", "#!/...", time=3.0/60,
                     outputs=["no_such_file"], dependencies=[task2, task3])

        controller = ToyNoDepController()
        controller.poll_delay = 1

        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=1, maxConcurrent=-1)

        self.create_output_file("output_1", 2)

        start = time.time()
        controller.submitTasks(args, [task4])
        stop = time.time()

        time.sleep(4)

        controller._updateQueueStatus()
        controller.updateTaskStatus(args, task4)

        # Make sure that dependencies are not emulated
        self.assertLess(stop - start, 3)

        self.assertEqual(task1.jobid, 10042)
        self.assertEqual(task2.jobid, None)
        self.assertEqual(task3.jobid, None)
        self.assertEqual(task4.jobid, None)

        self.assertEqual(task1.statusCode, taskStatus.done)
        self.assertEqual(task2.statusCode, taskStatus.tosubmit)
        self.assertEqual(task3.statusCode, taskStatus.tosubmit)
        self.assertEqual(task4.statusCode, taskStatus.tosubmit)

class MockNoDepControllerNoResponseTest(unittest.TestCase):
    @unittest.skip("")
    def test_no_response(self):

        task1 = task("mock task 1", "#!/...", time=3.0/60,
                     outputs=["output_1"])

        controller = MockNoDepControllerNoResponse()
        controller.poll_delay = 1
        args = ArgumentObject(verbose=False, debug=False, checktimestamp=True,
                             checkmissing=False, ntest=-1, maxConcurrent=-1)

        start = time.time()
        retVal = controller.submitTasks(args, [task1])
        stop = time.time()

        self.assertLess(retVal, 0)
        # self.assertLess(stop-start, 5)
        # self.assertGreater(stop-start, 2)


if __name__ == "__main__":
    unittest.main()
