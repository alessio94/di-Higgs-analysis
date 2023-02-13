import time
import os
import tempfile
import subprocess

from SubmissionHelpers.task import task, taskStatus
from SubmissionHelpers.submissionControllerBase import submissionControllerBase


class Process:
    """
    Object to store all attributes of a process in a single object.
    """

    def __init__(self, script, time_limit, job_id, stdout="/dev/null",
                 stderr="/dev/null"):
        """
        Start the process. The script argument is a string to be executed in a
        process. Shebangs are respected. If the shebang is missing, bash will
        be used. The lime_limit is the maximum runtime of the process. Please
        note that the process is not automatically terminated if the time
        limit is reached. The arguments stdout and stderr define the paths to
        which the standard streams are redirected.
        """
        if time_limit is None or time_limit < 0:
            self.end_time = float('inf')
        else:
            self.end_time = time.time() + float(time_limit) * 60

        self.job_id = job_id
        self.script = script
        self.stdout = stdout
        self.stderr = stderr

        parts = self.script.split("\n", 1)
        if len(parts) != 2 or not parts[0].startswith("#!"):
            # Assume it should be bash
            interpreter = "/usr/bin/env bash"
            content = self.script
        else:
            shebang, rest = parts
            interpreter = shebang[2:].strip()
            content = rest


        # This redirection via a file is required, because
        # subprocess.communicate is blocking.
        _, self.content_path = tempfile.mkstemp()
        with open(self.content_path, "w") as content_file:
            content_file.write(content)

        from SubmissionHelpers.common import mkdir_forFile
        mkdir_forFile(self.stdout)
        mkdir_forFile(self.stderr)
        
        with open(self.stdout, "w") as stdout, \
                open(self.stderr, "w") as stderr, \
                open(self.content_path, "r") as stdin:

            self.process = subprocess.Popen(interpreter.split(" "),
                                            stdin=stdin,
                                            stdout=stdout,
                                            stderr=stderr)

    def __del__(self):
        """
        Delete content file.
        """
        if os.path.exists(self.content_path):
            os.remove(self.content_path)
                                   
    def reached_limit(self):
        """
        Returns True if the process has reached its time limit.
        """
        return time.time() > self.end_time
        
    def kill(self):
        """
        Kills the process painfully (SIGKILL).
        """
        self.process.kill()

    def is_running(self):
        """
        Check if the job is still running.
        """
        return self.process.poll() is None
        

class LocalController(submissionControllerBase):
  controllerPriority = -float('inf') #can be used to prioritize one controller over another if multiple ones seem to be available
  poll_delay=5
  
  def __init__(self):
    """
    Initialize a local variables which store and count all threads.
    """
    self.processes = {}  # {identifier: process}
    self.previous_id = 0
    
  def _get_max_concurrent(self,suggestion):
    """
    Some submission controllers might want to mess with the number of
    simultaneously submitted processes
    """
    if suggestion > 0:
      return suggestion
    else:
      try:
        from os import cpu_count
      except ImportError:
        from multiprocessing import cpu_count  
      return cpu_count()-1
    
  def _apply_base_command(self, args, thisTask, cmd_args, payload, setup):
    # not applicable since _execute_submission_command is overridden
    return True
  
  def _apply_identifier(self, args, thisTask, cmd_args, payload, setup):
    # not applicable since _execute_submission_command is overridden
    return True
  
  def _apply_queue(self, args, thisTask, cmd_args, payload, setup):
    # not applicable
    return True
  
  def _apply_memory(self, args, thisTask, cmd_args, payload, setup):
    # not applicable
    return True
  
  def _apply_cpu_count(self, args, thisTask, cmd_args, payload, setup):
    # not applicable
    return True
  
  def _apply_time_limit(self, args, thisTask, cmd_args, payload, setup):
    # not applicable since _execute_submission_command is overridden
    return True
  
  def _apply_log_file(self, args, thisTask, cmd_args, payload, setup):
    # not applicable since _execute_submission_command is overridden
    return True
  
  def _apply_error_file(self, args, thisTask, cmd_args, payload, setup):
    # not applicable since _execute_submission_command is overridden
    return True

  def _apply_account(self,args, task, cmd_args, payload, setup):
    # not applicable since _execute_submission_command is overridden
    return True

  def _execute_submission_command(self, thisTask, cmd_args, payload, setup): 
    """
    Submitting a job locally means to start a thread which starts a
    subprocess. Returns the tuple (0, "id", "").
    """
    self.previous_id += 1

    job_id = self.previous_id
    job_process = Process("\n".join(setup + payload), time_limit=thisTask.time,
                          job_id=job_id, stderr=thisTask.errFile,
                          stdout=thisTask.logFile)

    self.processes[thisTask.identifier] = job_process

    return 0, str(self.previous_id), ""

  
  def _handle_submit_response(self, thisTask, stdout, stderr, status_code):
    """
    Parses the given response strings and return code from the batch system,
    extracts the job id, stets the task to 'submitted' and assigns it the job
    id. The method returns True on success. 

    If an error is detected, the method returns False and does not modify the
    task object. 

    This method must be implemented be a concrete submission controller class.
    """

    if status_code != 0:
        return False

    try:
        jobid = int(stdout)
    except ValueError as e:
        return False

    thisTask.jobid = jobid
    thisTask.setStatusCode(taskStatus.submitted)
    return True
    
  
  @staticmethod
  def _is_available():
    """
    The local controller is always available.
    """
    return True
    
  
  def _supports_simple_dependencies(self):
    """
    Returns always False because this implementation of the local controller
    does not handle dependencies.
    """
    return False
  
  def _updateQueueStatus(self):
    """
    This method polls the batch system for currently running jobs and updates the internal cache accordingly.
    See comments in __init__ for how this cache should be structured!
    Returns False in case of an error, True otherwise
    """
    self.queueStatusCache = {} #clear previous cache

    # Kill processes over time limit
    for identifier, process in self.processes.items():
        if process.reached_limit():
            process.kill()

        if process.reached_limit() or not process.is_running():
            # Can't delete while iterating
            self.processes[identifier] = None
            continue

    # Remove all timed out processes
    self.processes = {k: v for k, v in self.processes.items() \
                      if v is not None}

    # Build queue cache
    for identifier, process in self.processes.items():
        self.queueStatusCache[identifier] = \
                                {'jobid': process.job_id,
                                 'statusCode': taskStatus.running}

    self.queueStatusTimestamp = time.time()
    return True
    
  def _needs_empty_queue(self):
    """
    The local controller must wait for all jobs before submitTasks can exist.
    Otherwise we would loose all subprocesses on exist.
    """
    return True
