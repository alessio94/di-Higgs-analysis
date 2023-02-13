
import os.path
import time
import subprocess
from SubmissionHelpers.submissionControllerBase import submissionControllerBase,getCmdAdditions
from SubmissionHelpers.common import mkdir_p, string_types
from SubmissionHelpers.task import task,taskStatus

class SGEController(submissionControllerBase):
  controllerPriority = -float('inf') #can be used to prioritize one controller over another if multiple ones seem to be available
  """
  Implementation of a submission controller using the slurm scheduler.
  """
  
  def _apply_base_command(self, args, thisTask, cmd_args, payload, setup):
    """
    Appends 'qsub' to the command line arguments list in-place and returns
    the original payload string. The method is called by __submitSingleTask()
    to build command line arguments and the payload.
    """
    cmd_args.append("qsub")
    cmd_args.append("-terse")
    cmd_args.extend(getCmdAdditions()) #add optionally some more arguments
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
        cmd_args.append( "-l" )
        cmd_args.append( "mem_free="+str(int(thisTask.memory))+"M" )
    return True
  
  
  def _apply_cpu_count(self, args, thisTask, cmd_args, payload, setup):
    """
    If the job specifies a cpu count requirement, append the cpus per task
    parameter to the list of command line arguments in-place. In any case
    return the original payload. The method is called by __submitSingleTask()
    to build command line arguments and the payload.
    """
    if thisTask.ncores is not None and thisTask.ncores>1:
        print ( "[WARNING] You requested more than 1 core on an SGE based system, this option might not work on your specific cluster out-of-the-box" )
        cmd_args.append( "-pe" )
        cmd_args.append( "local" ) #this name depends on the local system and determines, e.g., how the cores are scattered (single machine, multiple machines, ...). Here: using NAF2's 'local', i.e., all cores on one machine (likely to not work on other systems, please inform rgugel@cern.ch if this is an issue for you!)
        cmd_args.append( "{:d}".format(thisTask.ncores) )
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
      cmd_args.append( "-l" )
      cmd_args.append( "h_rt="+str(int(thisTask.time*60)) )
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
        cmd_args.append("-o")
        cmd_args.append("{:s}".format(thisTask.logFile))
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
    if thisTask.errFile:
      if (thisTask.errFile == thisTask.logFile) :
        cmd_args.append("-j")
        cmd_args.append("yes")
      else:
        mkdir_p(os.path.dirname(thisTask.errFile)) #ensure directory for log files exists
        cmd_args.append("-e")
        cmd_args.append("{:s}".format(thisTask.errFile))
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
      depString = ""
      hasActualDependency = False
      for dep in thisTask.dependencies:
        jid = dep.getJobID(args, self)
        if dep.statusCode == taskStatus.done:
          continue
        
        if dep.statusCode in [taskStatus.submitted,taskStatus.running]:
          if (jid is None):
            raise RuntimeError( "Failed to apply dependencies for task '{:s}': one of its dependencies ('{:s}') does not have a jobID assigned yet!".format(thisTask.identifier, dep.identifier) )
            return None
          depString += ","+jid
          hasActualDependency = True
        else:
          raise RuntimeError( "Failed to apply dependencies for task '{:s}': one of its dependencies ('{:s}') is in an unexpected state '{:s}' (statusCode {:d})!".format(thisTask.identifier, dep.identifier, taskStatus.names[dep.statusCode], dep.statusCode) )
      if hasActualDependency: 
        cmd_args.append( "-hold_jid" )
        cmd_args.append(depString)
      
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
    #check if there was an error reported:
    if len(stderr.strip()):
      raise RuntimeError("An error occured trying to submit task '{:s}', "
                         "qsub returned the following error:\n'{:s}'".format(thisTask.identifier,stderr))
      return False
    if status_code != 0:
      raise RuntimeError("An error occured trying to submit task '{:s}', "
                         "qsub returned non-zero status code:\n'{:d}'".format(thisTask.identifier,status_code))
      return False
    #if not, extract task ID
    
    jobID = stdout.strip() #assuming that torque's qsub only writes the plain jobID to stdout
    if len(jobID)<1:
      raise RuntimeError("Task '{:s}' seems to have been submitted successfully,"
                         "but no jobID could be extracted from slurms response"
                         "'{:s}'".format(thisTask.identifier,stdout) )
      return False
    
    thisTask.jobid = jobID
    thisTask.setStatusCode(taskStatus.submitted) #do not set by hand, this method also updates the timestamp of the task!
    
    return True
    
    raise NotImplementedError("Submission controllers must implement "
                              "the method _handle_submit_response().")
  
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
    p = subprocess.Popen("which qsub".split(), stdin=subprocess.PIPE,stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p.communicate("")
    if p.returncode: 
      return False
    #we found qsub, let's check that it *IS* SGE (SGE has qhost and qmon, torque doesn't)
    p = subprocess.Popen("which qhost".split(), stdin=subprocess.PIPE,stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p.communicate("")
    return p.returncode == 0 #returncode is 0 (success) if everything went fine (i.e. it *IS* SGE)! 
    
  
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
    
    #the code below isonly for reference, it's this function for slurm:
    
    import xml.dom.minidom
    from subprocess import Popen, PIPE, STDOUT
    cmd = 'qstat -xml -r'
    listtag = "job_info"
    jobtag = "job_list"
    nametag = "JB_name"
    idtag = "JB_job_number"
    statetag = "state"
    p = Popen(cmd,stdout=PIPE,shell=True)
    stdout,stderr = p.communicate()
    # if no job is running (output of qstat * gives empty string) no need to
    # further modify stdout --> return empty dict
    if not stdout: return {}
    dom=xml.dom.minidom.parseString(stdout)
    jobs=dom.getElementsByTagName(listtag)
    run=jobs[0]
    runjobs=run.getElementsByTagName(jobtag)
    
    self.queueStatusCache = {}
    
    for r in runjobs:
      try:
        job = {}
        
        jobname=str(r.getElementsByTagName(nametag)[0].childNodes[0].data)
        jobstate=str(r.getElementsByTagName(statetag)[0].childNodes[0].data)
        jobid=str(r.getElementsByTagName(idtag)[0].childNodes[0].data)
        
        job['jobid'] = jobid
        job['status'] = jobstate
        job['statusCode'] = taskStatus.running #SGE is mostly on it's way out, dissecting all its states in detail can be added if really needed....
        self.queueStatusCache[jobname] = job
      except Exception as e:
        pass
    
    self.queueStatusTimestamp = time.time()
    return True
    
  