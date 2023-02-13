
import os.path
import os
import subprocess
import json
import time
from SubmissionHelpers.submissionControllerBase import submissionControllerBase,getCmdAdditions
from SubmissionHelpers.common import mkdir_p, string_types
from SubmissionHelpers.task import task,taskStatus

class CondorController(submissionControllerBase):
  controllerPriority = 0 #can be used to prioritize one controller over another if multiple ones seem to be available
  """
  Implementation of a submission controller using the htcondor scheduler.
  """

  def _apply_base_command(self, args, thisTask, cmd_args, payload, setup):
    """
    Appends 'sbatch' to the command line arguments list in-place and returns
    the original payload string. The method is called by __submitSingleTask()
    to build command line arguments and the payload.
    """
    cmd_args.append("condor_submit")
    cmd_args.append("--terse") #machine readable stdout
    cmd_args.extend(getCmdAdditions()) #add optionally some more arguments
    cmd_args.append("-") #read from stdin
    cmd_args.append("-queue") #this replaces adding a queue statement in the "payload"
    cmd_args.append("1") #number of copies of the job to submit
    #prepare payload scripts. Note: this should be the last argument

    mkdir_p(self.workloadDir)
    #not human readable but at least a safe file name which is unique as long as the tasks identifier is unique:
    import hashlib
    hash_object = hashlib.md5(thisTask.identifier.encode("ascii"))
    identifier = hash_object.hexdigest()
    payloadScriptName = os.path.join( self.workloadDir, identifier )

    wrapperScriptName = payloadScriptName+"_wrapper.sh"
    payloadScriptName += ".sh"
    with open(payloadScriptName, "w") as f:
      f.write("\n".join(thisTask.setup+thisTask.payload))
    with open(wrapperScriptName, "w") as f:
      f.write(self._getWrapperScript(payloadScriptName, [payloadScriptName, wrapperScriptName]))

    setup[:] = [] #the setup instructions are written to a file for htcondor, hence, don't pipe them to the submission binary!
    #create pseudo-payload, that is what is fed to condor_submit via stdin
    payload[:] = []
    payload.append("executable = /bin/bash")
    payload.append("arguments = {wrapper}".format(wrapper=os.path.abspath(wrapperScriptName)))
    # don't copy the bash binary from the submission node (maybe we could actually use this to copy/spool our payload and delete it right away?    
    payload.append("transfer_executable = false") 
    payload.append("requirements = TARGET.OpSysMajorVer == 7")
    #payload.append("initialdir = {:s}".format(os.getcwd()))
    if 'X509_USER_PROXY' in os.environ and os.path.isfile(os.environ['X509_USER_PROXY']) and os.system('voms-proxy-info -e &> /dev/null') == 0:
        payload.append("x509userproxy = $ENV(X509_USER_PROXY)")
    return True

  def _apply_identifier(self, args, thisTask, cmd_args, payload, setup):
    """
    Append the job-name argument to the command list argument list in-place and
    returns the original payload string. The method is called by
    __submitSingleTask() to build command line arguments and the payload.

    """
    payload.append("JobBatchName = {identifier}".format(identifier=thisTask.identifier))
    return True

  def _apply_queue(self, args, thisTask, cmd_args, payload, setup):
    """
    If the job's partition is not None, append the partition parameter to the
    list of command line arguments in-place. In any case, return the original
    payload. The method is called by __submitSingleTask() to build command line
    arguments and the payload.

    This method is a dummy due to the largely different behavior of HTCondor
    (HTCondor doesnot know queues unless it in turn submits to certain other
    systems)
    """
    return True

  def _apply_memory(self, args, thisTask, cmd_args, payload, setup):
    """
    If the job specifies a memory requirement, append the memory parameter to
    the list of command line arguments in-place. In any case, return the
    original payload. The method is called by __submitSingleTask() to build
    command line arguments and the payload.

    """
    payload.append("request_memory = {:d}".format(int(thisTask.memory))) #in MiB
    return True


  def _apply_cpu_count(self, args, thisTask, cmd_args, payload, setup):
    """
    If the job specifies a cpu count requirement, append the cpus per task
    parameter to the list of command line arguments in-place. In any case
    return the original payload. The method is called by __submitSingleTask()
    to build command line arguments and the payload.

    """
    payload.append("request_cpus = {:d}".format(int(thisTask.ncores)))
    return True



  def _apply_time_limit(self, args, thisTask, cmd_args, payload, setup):
    """
    If the job specifies a time limit, append the time parameter to the list
    of command line arguments in-place. In any case return the original
    payload. The method is called by __submitSingleTask() to build command
    line arguments and the payload.

    Note: this method is non-trivial. HTCondor by default doesnot know time
    limits. The option which configures this is therefore specific to the cluster
    you are using! Examples are "+MaxRuntime=<timeInSeconds>" for lxplus or
    "+RequestRuntime=<timeInSeconds>" for NAF2(DESY). The particular name of the
    option to use can be specified via the environment variable 
    'CAF_CONDOR_RUNTIME_OPTION' and defaults to the option name to be used at
    lxplus ('+MaxRuntime')
    """
    if ( not (thisTask.time is None) ) and thisTask.time > 0:
      timeInSeconds = thisTask.time
      try:
        timeInSeconds = 60*int(timeInSeconds)
      except:
        raise ValueError("Invalid runtime specified for task '{:s}', must be in minutes (single number)".format(thisTask.identifier))
        return []
      
      
      runtimeOptionName = os.getenv("ANALYSIS_CONDOR_RUNTIME_OPTION",os.getenv('CAF_CONDOR_RUNTIME_OPTION', "+MaxRuntime"))
      if len(runtimeOptionName) > 0:
        payload.append("{name} = {time}".format(name=runtimeOptionName, time=timeInSeconds)) #option as documented for CERNs HTCondor setup
        #formerly this used to have a couple variants which were always applied, however, newer HTCondor versions choke on additional options leaving jobs in idle state forever since no matching nodes can be found...
        #payload.append("+MaxRuntime = {time}".format(time=timeInSeconds)) #option as documented for CERNs HTCondor setup
        #payload.append("+RequestRuntime = {time}".format(time=timeInSeconds)) #option as documented for NAF2(DESY)'s HTCondor setup
      #this one should be safe...
      payload.append("MaxJobRetirementTime = {time}".format(time=timeInSeconds)) #"normal" HTCondor option, recommended to be used by Sheffield's documentation

    return True


  def _apply_log_file(self, args, thisTask, cmd_args, payload, setup):
    """
    If the job specifies a log file, prepend the log file parameter to the
    payload. The list of command line arguments is not modified. The method is
    called by __submitSingleTask() to build command line arguments and the
    payload.

    """
    #this part takes care of the "technical" log file (not the stdout one) as without
    #this option lxbatch will cause an error!
    logFile = ""
    if thisTask.logFile:
      logFile = thisTask.logFile+".condorlog"
    else:
      import hashlib
      hash_object = hashlib.md5(thisTask.identifier.encode("ascii"))
      identifier = hash_object.hexdigest()
      logFile = "logs/{id}".format( identifier+".condorlog" )
    mkdir_p(os.path.dirname(logFile)) #ensure directory for log files exists
    payload.append("Log = {logFile}".format(logFile=logFile))

    if thisTask.logFile:
      payload.append("Output = {logFile}".format(logFile=thisTask.logFile))
    return True


  def _apply_error_file(self, args, thisTask, cmd_args, payload, setup):
    """
    If the job specifies an error file, prepend the log error parameter to the
    payload. The list of command line arguments is not modified. The method is
    called by __submitSingleTask() to build command line arguments and the
    payload.

    """
    if thisTask.errFile:
      suffix = ""
      if thisTask.errFile == thisTask.logFile:
        suffix = ".err" #htcondor wil mess up log files when log and err file are identical
      payload.append("Error = {errFile}".format(errFile=thisTask.errFile+suffix))
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

    This method is a dummy due to the largely different behavior of HTCondor
    """
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
      accountingKeyName = os.getenv("ANALYSIS_CONDOR_ACCOUNTING_OPTION",os.getenv('CAF_CONDOR_ACCOUNTING_OPTION', "+AccountingGroup"))
      payload.append("{:s} = \"{:s}\"".format(accountingKeyName,accGroup))
    return True

  def _should_prepend_setup(self):
    """
    Allows a submissionController to determine if the task.setup member is
    prepended to the (possibly modified) payload. Always returns True unless
    overwritten in non-abstract implementation.
    """
    return False

  def _handle_submit_response(self, thisTask, stdout, stderr, status_code):
    """
    Parses the given response strings and return code from the batch system,
    extracts the job id, stets the task to 'submitted' and assigns it the job
    id. The method returns True on success.

    If an error is detected, the method returns False and does not modify the
    task object.

    This method must be implemented be a concrete submission controller class.
    """

    #print "task: {ident}".format(ident=thisTask.identifier)
    #print "stdout:"
    #print stdout
    #print "stderr:"
    #print stderr
    #print "statuscode:"
    #print status_code
    #return True

    if len(stderr.strip()):
      raise RuntimeError("An error occured trying to submit task '{:s}', "
                         "condor_submit returned the following error:\n'{:s}'".format(thisTask.identifier,stderr))
      return False
    if status_code != 0:
      raise RuntimeError("An error occured trying to submit task '{:s}', "
                         "condor_submit returned non-zero status code:\n'{:d}'".format(thisTask.identifier,status_code))
      return False
    #if not, extract task ID
    #NOTE: with -terse condor_submit reports only an ID range in the form
    # 'first - last' where first and last are of the form 'clusterID.index' with
    # index being an integer starting at 0
    fragments = stdout.strip().split("-") #with --parsable flag output is jobID;clusterName
    if len(fragments)<1:
      raise RuntimeError("Task '{:s}' seems to have been submitted successfully,"
                         "but no jobID could be extracted from condors response"
                         "'{:s}'".format(thisTask.identifier,stdout) )
      return False

    jobID = fragments[0].strip()

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

    Implementation for condor_submit: checks availability of "condor_submit" binary
    using "which condor_submit" and checking the return code of this call.
    """
    p = subprocess.Popen("which condor_submit".split(), stdin=subprocess.PIPE,stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p.communicate("")
    return not p.returncode #returncode is 0 if everything went fine!


  def _supports_simple_dependencies(self):
    """
    Returns always False because htcondor_submit does not support simple job
    dependencies (at least not the way we would like them).
    """
    return False

  def _updateQueueStatus(self):
    """
    This method polls the batch system for currently running jobs and updates the internal cache accordingly.
    See comments in __init__ for how this cache should be structured!
    Returns False in case of an error, True otherwise
    """
    from subprocess import Popen, PIPE
    cmd = 'condor_q $(whoami) -json -attributes JobBatchName,ClusterId,JobStatus'
    p = Popen(cmd,stdout=PIPE,shell=True,universal_newlines=True)
    output, _ = p.communicate()
    self.queueStatusCache = {} #clear previous cache
    if len(output) < 1:
      self.queueStatusTimestamp = time.time()
      return True
    try:
      asJson = json.loads(output)
    except:
      print("Failed to get valid response from condor_q, considering current queue to be empty")
      self.queueStatusTimestamp = time.time()
      return False

    for job in asJson:
      if not isinstance(job,dict):
        print("[WARNING/_updateQueueStatus] Unexpected object in json output from condor_q!")
        print(job)
        continue
      if not (u'ClusterId' in job and u'JobStatus' in job and u'JobBatchName' in job) :
        print("[ERROR] obtained incomplete information for a job:")
        print(job)
        continue
      myJob = {}
      myJob['jobid'] = job[u'ClusterId']
      myJob['status'] = job[u'JobStatus']
      myJobName = str(job[u'JobBatchName'])

      #easier handling of condor status code
      cStatus = myJob['status']
      #translate into our status codes
      #see: http://pages.cs.wisc.edu/~adesmet/status.html
      if cStatus in [0,1,5]:
        myJob['statusCode'] = taskStatus.submitted
      elif cStatus in [2,4]:
        myJob['statusCode'] = taskStatus.running
      elif cStatus in [3,6,5]:
        myJob['statusCode'] = taskStatus.failed
      else:
        print( "[WARNING] Encountered unknown status '{:d}' in condor_q output for task with identifier '{:s}'/ jobid '{:s}'! Will consider its queue status as undefined!".format(cStatus,myJobName,str(job['jobid'])) )
        myJob['statusCode'] = taskStatus.undefined
      self.queueStatusCache[myJobName] = myJob
    self.queueStatusTimestamp = time.time() #update the time stamp for the current cache
    return True
