
import os.path
import time
import subprocess
from SubmissionHelpers.submissionControllerBase import submissionControllerBase,getCmdAdditions
from SubmissionHelpers.common import mkdir_p, string_types
from SubmissionHelpers.task import task,taskStatus

class SlurmController(submissionControllerBase):
  controllerPriority = 1 #can be used to prioritize one controller over another if multiple ones seem to be available
  """
  Implementation of a submission controller using the slurm scheduler.
  """

  def __init__(self):
    submissionControllerBase.__init__(self)

  def _apply_base_command(self, args, thisTask, cmd_args, payload, setup):
    """
    Appends 'sbatch' to the command line arguments list in-place and returns
    the original payload string. The method is called by __submitSingleTask()
    to build command line arguments and the payload.
    """
    cmd_args.append("sbatch")
    cmd_args.append("--parsable") #machine readable stdout
    cmd_args.extend(getCmdAdditions()) #add optionally some more arguments
    return True

  def _apply_identifier(self, args, thisTask, cmd_args, payload, setup):
    """
    Append the job-name argument to the command list argument list in-place and
    returns the original payload string. The method is called by
    __submitSingleTask() to build command line arguments and the payload.
    """
    cmd_args.append("-J")  # job-name
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
        cmd_args.append("-p")  # partition
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
        cmd_args.append( "--mem={:d}".format(int(thisTask.memory)) )
    return True


  def _apply_cpu_count(self, args, thisTask, cmd_args, payload, setup):
    """
    If the job specifies a cpu count requirement, append the cpus per task
    parameter to the list of command line arguments in-place. In any case
    return the original payload. The method is called by __submitSingleTask()
    to build command line arguments and the payload.
    """
    if thisTask.ncores is not None:
        cmd_args.append( "--cpus-per-task={:d}".format(thisTask.ncores) )
    return True



  def _apply_time_limit(self, args, thisTask, cmd_args, payload, setup):
    """
    If the job specifies a time limit, append the time parameter to the list
    of command line arguments in-place. In any case return the original
    payload. The method is called by __submitSingleTask() to build command
    line arguments and the payload.
    """
    if ( not (thisTask.time is None) ) and thisTask.time > 0:
      t = float(thisTask.time)
      #convert to h:m:s format
      timeString = "{h}:{m}:{s}".format(h = int(t/60.), m = int(t)-60*int(t/60.), s = int(60*(t-int(t))) )
      cmd_args.append("--time={:s}".format(timeString) )
    return True


  def _apply_log_file(self, args, thisTask, cmd_args, payload, setup):
    """
    If the job specifies a log file, prepend the log file parameter to the
    payload. The list of command line arguments is not modified. The method is
    called by __submitSingleTask() to build command line arguments and the
    payload.
    """
    if thisTask.logFile:
        mkdir_p(os.path.dirname(thisTask.logFile)) #ensure directory for log files exists
        cmd_args.append("--output={:s}".format(thisTask.logFile))
        return True
    else:
        return True

  def _apply_error_file(self, args, thisTask, cmd_args, payload, setup):
    """
    If the job specifies an error file, prepend the log error parameter to the
    payload. The list of command line arguments is not modified. The method is
    called by __submitSingleTask() to build command line arguments and the
    payload.
    """
    if thisTask.errFile and not (thisTask.errFile == thisTask.logFile) :
        mkdir_p(os.path.dirname(thisTask.errFile)) #ensure directory for log files exists
        cmd_args.append("--error={:s}".format(thisTask.errFile))
        return True
    else:
        return True

  def _apply_dependencies(self, args, thisTask, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order to pass dependencies to the batch system.
    The method is called by __submitSingleTask() to build command line
    arguments and the payload. The method is not called by
    __submitSingleTask() if _supports_simple_dependencies() returns False.

    Jobs in the dependency list without a job id are ignored.

    This method must be implemented be a concrete submission controller class.
    """
    if len(thisTask.dependencies) > 0:
      depString = "--dependency=afterok"
      hasActualDependency = False
      for dep in thisTask.dependencies:
        jid = dep.getJobID(args, self)
        if dep.statusCode == taskStatus.done:
          continue

        if dep.statusCode in [taskStatus.submitted,taskStatus.running]:
          if (jid is None):
            raise RuntimeError( "Failed to apply dependencies for task '{:s}': one of its dependencies ('{:s}') does not have a jobID assigned yet!".format(thisTask.identifier, dep.identifier) )
            return None
          depString += ":"+jid
          hasActualDependency = True
        else:
          raise RuntimeError( "Failed to apply dependencies for task '{:s}': one of its dependencies ('{:s}') is in an unexpected state '{:s}' (statusCode {:d})!".format(thisTask.identifier, dep.identifier, taskStatus.names[dep.statusCode], dep.statusCode) )
      if hasActualDependency: cmd_args.append(depString)

    return True

  def _apply_account(self,args, task, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order announce the account to be used/charged
    to the batch system. The method is called by _submitSingleTask() to build
    command line arguments and the payload.

    This method must be implemented be a concrete submission controller class.
    """
    accGroup = task.account
    if accGroup == None or (isinstance(accGroup,string_types) and len(accGroup)<1):
      accGroup = args.account #might be empty string
    if len(accGroup) > 0:
      cmd_args.append("--account={:s}".format(accGroup))
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
    #check if there was an error reported:
    if len(stderr.strip()):
      raise RuntimeError("An error occured trying to submit task '{:s}', "
                         "sbatch returned the following error:\n'{:s}'".format(thisTask.identifier,stderr))
      return False
    if status_code != 0:
      if not stdout.strip("\n") == "":
        raise RuntimeError("An error occured trying to submit task '{:s}', "
                           "sbatch returned non-zero status code:\n'{:d}'\n"
                           "Maybe the stdout message helps:\n{:s}\n".format(thisTask.identifier,status_code,stdout.strip("\n")))
      else:
        raise RuntimeError("An error occured trying to submit task '{:s}', "
                           "sbatch returned non-zero status code:\n'{:d}'".format(thisTask.identifier,status_code))
      return False
    #if not, extract task ID
    fragments = stdout.strip().split(";") #with --parsable flag output is jobID;clusterName
    if len(fragments)<1:
      raise RuntimeError("Task '{:s}' seems to have been submitted successfully,"
                         "but no jobID could be extracted from slurms response"
                         "'{:s}'".format(thisTask.identifier,stdout) )
      return False

    clusterName = ""
    if len(fragments)>1: clusterName = fragments[1]
    jobID = fragments[0]

    thisTask.jobid = jobID
    thisTask.setStatusCode(taskStatus.submitted) #do not set by hand, this method also updates the timestamp of the task!

    return True


  @staticmethod
  def _is_available():
    """
    This method returns True if the batch system corresponding to the particular
    controller appears to be available (the exact conditions can vary), typically
    this implies a check for the submission binary being available. If the
    corresponding batch system is not available, False is returned.

    This method must be implemented be a concrete submission controller class.

    Implementation for slurm: checks availability of "sbatch" binary using
    "which sbatch" and checking the return code of this call.
    """
    p = subprocess.Popen("which sbatch".split(), stdin=subprocess.PIPE,stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
    p.communicate("")
    return not p.returncode #returncode is 0 if everything went fine!


  def _supports_simple_dependencies(self):
    """
    Returns always True because slurm supports simple job dependencies.
    """
    return True

  def _updateQueueStatus(self):
    """
    This method polls the batch system for currently running jobs and updates the internal cache accordingly.
    See comments in __init__ for how this cache should be structured!
    Returns False in case of an error, True otherwise
    """
    from subprocess import Popen, PIPE
    cmd = 'squeue -u $(whoami) -h -o "%i %j %T %R"'
    p = Popen(cmd,stdout=PIPE,shell=True,universal_newlines=True)
    output, _ = p.communicate()
    returnValue = True
    self.queueStatusCache = {} #clear previous cache
    for line in output.split('\n'):
      line = line.strip()
      if len(line) < 1: continue #skip empty lines
      parts = line.split()
      if len(parts)>3:
        auxInfo = ""
        job = {}
        if parts[3].startswith("("): #we got a reason why the job isn't running yet
          auxInfo = ", reason: "+parts[3][1:-1]
        else:
          auxInfo = " at node "+parts[3]
        identifier = parts[1].strip()
        job['jobid'] = parts[0].strip()
        job['status'] = (parts[2] + auxInfo).strip()

        status = parts[2].strip()
        if status in ['RUNNING','COMPLETING','COMPLETED']: job['statusCode'] = taskStatus.running
        elif status in ['PENDING','SUSPENDED','CONFIGURING']: job['statusCode'] = taskStatus.submitted
        elif status in ['FAILED','CANCELED','TIMEOUT','PREEMPTED','NODE_FAIL','REVOKED','SPECIAL_EXIT','REQUEUE_HOLD']: job['statusCode'] = taskStatus.running
        else: #unknown status, warn the user an set status as undefined:
          print( "[WARNING] Encountered unknown status '{:s}' in squeue output for task with identifier '{:s}'/ jobid '{:s}'! Will consider its queue status as undefined!".format(status,identifier,job['jobid']) )
          job['statusCode'] = taskStatus.undefined
          returnValue = False

        self.queueStatusCache[identifier] = job
      else:
        raise RuntimeError( "[submissionControllerSlurm] Failed to update queue status cache: encountered unexpected line format in squeue output '{:s}'".format(line) )
        return False
    self.queueStatusTimestamp = time.time()
    return returnValue
