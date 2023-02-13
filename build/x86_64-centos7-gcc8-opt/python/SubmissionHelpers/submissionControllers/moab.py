import hashlib
import os
import stat
import subprocess
import time

from SubmissionHelpers.submissionControllerBase import submissionControllerBase, getCmdAdditions
from SubmissionHelpers.common import mkdir_p, string_types
from SubmissionHelpers.task import taskStatus


class MoabController(submissionControllerBase):
    controllerPriority = 1  # choose 1 to prioritize over TorqueController
    """
  Implementation of a submission controller using the moab scheduler.
  Developed for and tested on the NEMO cluster in Freiburg: https://wiki.bwhpc.de/e/Batch_Jobs
  """

    def _apply_base_command(self, args, thisTask, cmd_args, payload, setup):
        """
        Appends 'msub' to the command line arguments.
        A payload and wrapper script are build, because moab needs a bash script as payload.
        The original setup and payload commands are replaced by the wrapper script.
        The method is called by __submitSingleTask() to build command line arguments and the payload.
        """
        cmd_args.append("msub")
        cmd_args.extend(getCmdAdditions())  # add optionally some more arguments

        mkdir_p(self.workloadDir)
        # not human readable but at least a safe file name which is unique as long as the tasks identifier is unique:
        hash_object = hashlib.md5(thisTask.identifier.encode("ascii"))
        identifier = hash_object.hexdigest()
        payloadScriptName = os.path.join(self.workloadDir, identifier)

        wrapperScriptName = payloadScriptName + "_wrapper.sh"
        payloadScriptName += ".sh"
        with open(payloadScriptName, "w") as f:
            f.write("\n".join(thisTask.setup + thisTask.payload))
            mode = os.fstat(f.fileno()).st_mode
            mode |= stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH
            os.fchmod(f.fileno(), stat.S_IMODE(mode))
        with open(wrapperScriptName, "w") as f:
            f.write(self._getWrapperScript(payloadScriptName, [payloadScriptName, wrapperScriptName]))
            mode = os.fstat(f.fileno()).st_mode
            mode |= stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH
            os.fchmod(f.fileno(), stat.S_IMODE(mode))

        # the setup instructions are written to a file for htcondor, hence, don't pipe them to the submission binary!
        setup[:] = []
        # create pseudo-payload, that is what is fed to condor_submit via stdin
        payload[:] = []
        payload.append(os.path.abspath(wrapperScriptName))

        return True

    def _apply_identifier(self, args, thisTask, cmd_args, payload, setup):
        """
        Append the job-name argument to the command list argument list in-place and
        returns the original payload string. The method is called by
        __submitSingleTask() to build command line arguments and the payload.
        """
        cmd_args.append("-N")  # job-name
        cmd_args.append(thisTask.identifier)
        return True

    def _apply_queue(self, args, thisTask, cmd_args, payload, setup):
        """
        If the job's partition is not None, append the partition parameter to the
        list of command line arguments in-place. In any case, return the original
        payload. The method is called by __submitSingleTask() to build command line
        arguments and the payload.
        """
        if not (thisTask.queue is None or thisTask.queue == ""):
            cmd_args.append("-q")  # partition
            cmd_args.append(thisTask.queue)
        return True

    def _apply_memory(self, args, thisTask, cmd_args, payload, setup):
        """
        If the job specifies a memory requirement, append the memory parameter to
        the list of command line arguments in-place. In any case, return the
        original payload. The method is called by __submitSingleTask() to build
        command line arguments and the payload.
        """
        if not (thisTask.memory is None):
            cmd_args.append("-l")
            cmd_args.append("pmem=" + str(int(thisTask.memory)) + "mb")
        return True

    def _apply_cpu_count(self, args, thisTask, cmd_args, payload, setup):
        """
        If the job specifies a cpu count requirement, append the cpus per task
        parameter to the list of command line arguments in-place. In any case
        return the original payload. The method is called by __submitSingleTask()
        to build command line arguments and the payload.
        """
        if thisTask.ncores is not None:
            cmd_args.append("-l")
            cmd_args.append("nodes=1:ppn={:d}".format(thisTask.ncores))
        return True

    def _apply_time_limit(self, args, thisTask, cmd_args, payload, setup):
        """
        If the job specifies a time limit, append the time parameter to the list
        of command line arguments in-place. In any case return the original
        payload. The method is called by __submitSingleTask() to build command
        line arguments and the payload.
        """
        if (not (thisTask.time is None)) and thisTask.time > 0:
            cmd_args.append("-l")
            cmd_args.append("walltime=" + str(int(thisTask.time) * 60))
        return True

    def _apply_log_file(self, args, thisTask, cmd_args, payload, setup):
        """
        If the job specifies a log file, prepend the log file parameter to the
        payload. The list of command line arguments is not modified. The method is
        called by __submitSingleTask() to build command line arguments and the
        payload.
        """
        if thisTask.logFile:
            mkdir_p(os.path.dirname(thisTask.logFile))  # ensure directory for log files exists
            cmd_args.append("-o")
            cmd_args.append("{:s}".format(thisTask.logFile))
            return True
        else:
            return True

    def _apply_error_file(self, args, task, cmd_args, payload, setup):
        """
        If the job specifies an error file, prepend the log error parameter to the
        payload. The list of command line arguments is not modified. The method is
        called by __submitSingleTask() to build command line arguments and the
        payload.
        """
        if task.errFile:
            if task.errFile == task.logFile:
                cmd_args.append("-j")
                cmd_args.append("oe")
            else:
                mkdir_p(os.path.dirname(task.errFile))  # ensure directory for log files exists
                cmd_args.append("-e")
                cmd_args.append("{:s}".format(task.errFile))
            return True
        else:
            return True

    def _apply_dependencies(self, args, task, cmd_args, payload, setup):
        """
        Modifies the given command line argument list in-place and returns the
        modified payload string in order to pass dependencies to the batch system.
        The method is called by __submitSingleTask() to build command line
        arguments and the payload. The method is not called by
        __submitSingleTask() if _supports_simple_dependencies() returns False.

        Jobs in the dependency list without a job id are ignored.

        This method must be implemented be a concrete submission controller class.
        """
        if len(task.dependencies) > 0:
            depString = "afterok"
            hasActualDependency = False
            for dep in task.dependencies:
                jid = dep.getJobID(args, self)
                if dep.statusCode == taskStatus.done:
                    continue

                if dep.statusCode in [taskStatus.submitted, taskStatus.running]:
                    if jid is None:
                        raise RuntimeError(
                            "Failed to apply dependencies for task '{:s}': one of its dependencies ('{:s}') does not have a jobID assigned yet!".format(
                                task.identifier, dep.identifier
                            )
                        )
                        return None
                    depString += ":" + jid
                    hasActualDependency = True
                else:
                    raise RuntimeError(
                        "Failed to apply dependencies for task '{:s}': one of its dependencies ('{:s}') is in an unexpected state '{:s}' (statusCode {:d})!".format(
                            task.identifier, dep.identifier, taskStatus.names[dep.statusCode], dep.statusCode
                        )
                    )
            if hasActualDependency:
                cmd_args.append("-l depend={:s}".format(depString))

        return True

    def _apply_account(self, args, task, cmd_args, payload, setup):
        """
        Modifies the given command line argument list in-place and returns the
        modified payload string in order announce the account to be used/charged
        to the batch system. The method is called by _submitSingleTask() to build
        command line arguments and the payload.

        This method must be implemented be a concrete submission controller class.
        """
        accGroup = task.account
        if accGroup is None or (isinstance(accGroup, string_types) and len(accGroup) < 1):
            accGroup = args.account  # might be empty string
        if len(accGroup) > 0:
            cmd_args.append("-A")
            cmd_args.append(accGroup)
        return True

    def _handle_submit_response(self, thisTask, stdout, stderr, status_code):
        """
        Parses the given response strings and return code from the batch system,
        extracts the job id, stets the task to 'submitted' and assigns it the job
        id. The method returns True on success.

        If an error is detected, the method returns False and does not modify the
        task object.

        This method must be implemented be a concrete submission controller class.
        """
        # check if there was an error reported:
        if len(stderr.strip()):
            raise RuntimeError(
                "An error occured trying to submit task '{:s}', "
                "msub returned the following error:\n'{:s}'".format(thisTask.identifier, stderr)
            )
            return False
        if status_code != 0:
            raise RuntimeError(
                "An error occured trying to submit task '{:s}', "
                "msub returned non-zero status code:\n'{:d}'".format(thisTask.identifier, status_code)
            )
            return False
        # if not, extract task ID
        jobID = stdout.strip()  # assuming that moabs's msub only writes the plain jobID to stdout
        if len(jobID) < 1:
            raise RuntimeError(
                "Task '{:s}' seems to have been submitted successfully,"
                "but no jobID could be extracted from moabs response"
                "'{:s}'".format(thisTask.identifier, stdout)
            )
            return False

        thisTask.jobid = jobID
        thisTask.setStatusCode(
            taskStatus.submitted
        )  # do not set by hand, this method also updates the timestamp of the task!

        return True

    @staticmethod
    def _is_available():
        """
        This method returns True if the batch system corresponding to the particular
        controller appears to be available (the exact conditions can vary), typically
        this implies a check for the submission binary being available. If the
        corresponding batch system is not available, False is returned.

        This method must be implemented be a concrete submission controller class.

        Implementation for moab: checks availability of "msub" binary using
        "which msub" and checking the return code of this call.
        """
        p = subprocess.Popen(
            "which msub".split(), stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE
        )
        p.communicate("")
        return not p.returncode  # returncode is 0 if everything went fine!

    def _supports_simple_dependencies(self):
        """
        Returns always True because moab does support simple job dependencies.
        """
        return True

    def _updateQueueStatus(self):
        """
        This method polls the batch system for currently running jobs and updates the internal cache accordingly.
        See comments in __init__ for how this cache should be structured!
        Returns False in case of an error, True otherwise
        """
        data = self._showqxml()
        if data is None:  # check if we got a valid xml object.
            # We get None from _showqxml() if the queue polling returned an empty string!
            self.queueStatusCache = {}
            self.queueStatusTimeStamp = time.time()
            return True

        self.queueStatusCache = {}  # clear previous cache

        for queue in data.iter("queue"):
            for job in queue.iter("job"):
                job_id = job.attrib["JobID"]
                job_name = job.attrib["JobName"]
                moab_job_state = job.attrib["State"]
                if moab_job_state == "Idle":
                    job_state = taskStatus.submitted
                elif moab_job_state == "Running":
                    job_state = taskStatus.running
                elif moab_job_state == "Deferred":
                    job_state = taskStatus.tosubmit
                else:
                    print("[WARNING] Could not parse job state '{0:s}'".format(moab_job_state))
                    job_state = taskStatus.undefined
                self.queueStatusCache[job_name] = {"jobid": job_id, "status": moab_job_state, "statusCode": job_state}

        self.queueStatusTimestamp = time.time()
        return True

    def _showqxml(self):
        """
        executes the 'showq --xml' command and reads the xml contents, returning
        an xml object if the output of 'showq --xml' is non-empty. If it is empty,
        None is returned
        """
        import xml.etree.ElementTree as ET
        from subprocess import Popen, PIPE

        cmd = "showq -u $(whoami) --xml"
        p = Popen(cmd, stdout=PIPE, shell=True)
        stdout, stderr = p.communicate()
        if len(stdout) < 1:
            return None
        return ET.fromstring(stdout)

    def _getStr(self, elem, tag):
        return str(elem.getElementsByTagName(tag)[0].childNodes[0].data)
