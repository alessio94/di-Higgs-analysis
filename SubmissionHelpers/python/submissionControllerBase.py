#!/usr/bin/env python

import sys
import os.path
import os
import time
import errno
import subprocess
from .task import task,taskStatus
from .common import mkdir_p,concat,mergeDicts

inf = float("inf")

#How to use:
# -using an existing controller:
#    create an instance of the suitable controller. To submit tasks simply pass them as a list to myController.submitTasks(listOfTasks)
# -writing a new controller:
#    create a class derived from submissionControllerBase and override the methods of submissionControllerBase where indicated (i.e. methods raising a NotImplementedError)

def getCmdAdditions():
  """
  Checks if the environment variable `ANALYSIS_SUBMIT_CMD_ADD` is set and if so
  returns its contents as a list by splitting at whitespaces.
  If the env var is not set returns an empty list.
  """
  envVar = os.environ.get("ANALYSIS_SUBMIT_CMD_ADD")
  if not envVar:
    envVar = os.environ.get("CAF_SUBMIT_CMD_ADD")
  if isinstance(envVar,str):
    return envVar.split()
  return []


class submissionSummary:
  """
  Helper class to combine number of actually sumitted tasks and tasks that were
  expected to be subitted.

  Boolean interpretation corresponds to assessing if the number of expected and
  actually submitted tasks are identical and error member is not true

  Addition of another submissionSummary to this one corresponds to adding their

  """
  def __init__(self,nSubmitted=0,nExpected=0):
    self.nSub = nSubmitted
    self.nExp = nExpected
    self.error = False
    self.errString = ""

  #python2 bool interpretation:
  def __nonzero__(self):
    return (self.nSub==self.nExp) and (not self.error)
  #python3 bool interpretation:
  def __bool__(self):
    return (self.nSub==self.nExp) and (not self.error)

  def __add__(self,other):
    if not isinstance(other,submissionSummary):
      raise TypeError("cannot add submissionSummary and object of type '{:s}'".format(type(other).__str__()))
    sub = self.nSub + other.nSub
    exp = self.nExp + other.nSub
    newSubSum = submissionSummary(sub,exp)
    newSubSum.error = self.error or other.error
    newSubSum.errString = (self.errString+"\n"+other.errString).strip()
    return newSubSum


#abstract base class for all submissionControllers
class submissionControllerBase(object):
  debugJobID=-1
  controllerPriority = -inf #can be used to prioritize one controller over another if multiple ones seem to be available
  poll_delay = 60
  #######################################################################
  ########these methods should be overridden in the derived class:#######

  def _apply_base_command(self, args, task, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order to accommodate the initial submission
    command. For example in the case of Slurm, the method appends 'sbatch' to
    the command list. The method is called by _submitSingleTask() to build
    command line arguments and the payload..

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _apply_base_command().")

  def _apply_identifier(self, args, task, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order to pass the job identifier to the batch
    system. The method is called by _submitSingleTask() to build command line
    arguments and the payload.

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _apply_identifier().")

  def _apply_queue(self, args, task, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order to pass the queue selection to the batch
    system. The method is called by _submitSingleTask() to build command line
    arguments and the payload.

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _apply_queue().")

  def _apply_memory(self, args, task, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order to pass the memory requirements to the
    batch system. The method is called by _submitSingleTask() to build
    command line arguments and the payload.

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _apply_memory().")


  def _apply_cpu_count(self, args, task, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order to pass the cpu count requirement to the
    batch system. The method is called by _submitSingleTask() to build
    command line arguments and the payload.

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _apply_cpu_count().")

  def _apply_time_limit(self, args, task, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order to pass the execution time requirement to
    the batch system. The method is called by _submitSingleTask() to build
    command line arguments and the payload.

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _apply_time_limit().")

  def _apply_log_file(self, args, task, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order to pass the path of the log file to
    the batch system. The method is called by _submitSingleTask() to build
    command line arguments and the payload.

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _apply_log_file().")

  def _apply_error_file(self, args, task, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order to pass the path of the error file to
    the batch system. The method is called by _submitSingleTask() to build
    command line arguments and the payload.

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _apply_error_file().")

  def _apply_end_of_command(self, args, task, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order to pass the path of the error file to
    the batch system. The method is called by _submitSingleTask() to build
    command line arguments and the payload.
    """
    return True

  def _apply_dependencies(self, args, task, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order to pass dependencies to the batch system.
    The method is called by _submitSingleTask() to build command line
    arguments and the payload. The method is not called by
    _submitSingleTask() if _supports_simple_dependencies() returns False.

    Jobs in the dependency list without a job id are ignored.

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _apply_dependencies().")

  def _apply_account(self,args, task, cmd_args, payload, setup):
    """
    Modifies the given command line argument list in-place and returns the
    modified payload string in order announce the account to be used/charged
    to the batch system. The method is called by _submitSingleTask() to build
    command line arguments and the payload.

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _apply_account().")


  @staticmethod
  def _is_available():
    """
    This method returns True if the batch system corresponding to the particular
    controller appears to be available (the exact conditions can vary), typically
    this implies a check for the submission binary being available. If the
    corresponding batch system is not available, False is returned.

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _is_available().")


  def _supports_simple_dependencies(self):
    """
    This method returns True, if the batch system supports simple dependency
    management by specifying jobs by their id which must be completed before
    the dependent job can start.

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _supports_simple_dependencies().")
  def _updateQueueStatus(self):
    """
    This method polls the batch system for currently running jobs and updates the internal cache accordingly.
    See comments in __init__ for how this cache should be structured!
    Returns False in case of an error, True otherwise
    """
    raise NotImplementedError("Base class 'submissionControllerBase' does not implement this function, please override it in the derived class")

  ########################################################################

  def __init__(self):
    self.queueStatusCache = {} #cache to store the status of running/scheduled jobs on the batch system. To be filled/updated by _updateQueueStatus with a dict of dicts.
                               #the first level key is the identifier of the task, the second level keys are:
                               # 'status' (human readable string),
                               # 'statusCode' (one of taskStatus' members),
                               # 'jobid' (batch system side id as used to declare dependencies)
    self.queueStatusTimestamp = -1
    if not hasattr(self, 'workloadDir') or self.workloadDir == "":
      self.workloadDir = os.getcwd()+"/submissionWorkloadTemp"
    self.wrapperTemplate = """#!/bin/bash -l
cd {pwd}
function cleanup(){{{{
rm {{filesToDelete}}
rv=$?
rmdir {subfolder} 2>/dev/null
exit $rv
}}}}
trap "cleanup; exit 0" EXIT
trap "cleanup; echo 'The payload was requested to exit via SIGTERM, please check your ressource allocation';  exit \$?" TERM INT SIGTERM SIGCONT SIGKILL
source {{payloadScript}}
"""
    self.wrapperTemplate = self.wrapperTemplate.format(subfolder=self.workloadDir, pwd=os.getcwd())

  def _getWrapperScript(self, payloadScriptName, filesToDelete):
    if isinstance(filesToDelete, list):
      filesToDelete = " ".join(filesToDelete)
    if not isinstance(filesToDelete, str):
      raise Exception("The argument filesToDelete has to be a list or a string")

    wrapperScript = self.wrapperTemplate.format(payloadScript=payloadScriptName, filesToDelete=filesToDelete)
    return wrapperScript

  def _handle_submit_response(self, task, stdout, stderr, status_code):
    """
    Parses the given response strings and return code from the batch system,
    extracts the job id, stets the task to 'submitted' and assigns it the job
    id (task.jobid). The method returns True on success.

    If an error is detected, the method returns False and does not modify the
    task object.

    This method must be implemented be a concrete submission controller class.
    """
    raise NotImplementedError("Submission controllers must implement "
                              "the method _handle_submit_response().")


  def _should_prepend_setup(self):
    """
    Allows a submissionController to determine if the task.setup member is
    prepended to the (possibly modified) payload. Always returns True unless
    overwritten in non-abstract implementation.
    """
    return True

  #TODO: clearify if this should be "protected" or "private" (i.e. do we allow overwriting in derived classes?)
  def _submitSingleTask(self, args, thisTask, debug=False, verbose=False):
    """
    Sends a single task (or TaskArray) to the batch system.

    If args.debug or debug is True, the actual communication to the batch
    system is skipped. The method assigns toy job ids (less than zero) to the
    jobs in this case. If args.verbose or verbose is True, the method is
    verbose about what it is doing.

    The behavior of this method depends on whether the batch system supports
    dependencies as indicated by _supports_simple_dependencies().

     (i)  If the batch system is *ABLE* to handle dependencies, all dependencies
          must either be done or have a job id. Otherwise an exception is
          thrown.

     (ii) If the batch system is *NOT ABLE* to handle dependencies, all jobs
          are expected to be done. An exception is raised if this is not the
          case.

    In case of successful submission, the task is set to 'submitted' and is
    assigned a job id as returned by the batch system.

    The method returns True on success, False otherwise.
    """

    cmd_args = []
    payload = list(thisTask.payload)
    setup = list(thisTask.setup)

    self._apply_base_command(args, thisTask, cmd_args, payload, setup)
    self._apply_identifier(args, thisTask, cmd_args, payload, setup)
    self._apply_queue(args, thisTask, cmd_args, payload, setup)
    self._apply_memory(args, thisTask, cmd_args, payload, setup)
    self._apply_cpu_count(args, thisTask, cmd_args, payload, setup)
    self._apply_time_limit(args, thisTask, cmd_args, payload, setup)
    self._apply_account(args, thisTask, cmd_args, payload, setup)
    self._apply_log_file(args, thisTask, cmd_args, payload, setup)
    self._apply_error_file(args, thisTask, cmd_args, payload, setup)

    if self._supports_simple_dependencies():
        # The batch system has dependcy support:
        # Check that all dependencies are done or have a job id
        for dependency in thisTask.dependencies:
            if not (dependency.statusCode == taskStatus.done or\
                    not (dependency.getJobID(args, self) is None) ):
                raise Exception("Trying to submit a single task with "
                                "dependency support but a dependency task (%s)"
                                "neither is done nor has a job id." \
                                % dependency)

        self._apply_dependencies(args, thisTask, cmd_args, payload, setup)
    else:
        # The batch system does not have dependcy support:
        # Check that all dependencies are done
        for dependency in thisTask.dependencies:
            if not dependency.statusCode == taskStatus.done:
                raise Exception("Trying to submit a single task without "
                                "dependency support but a dependency task (%s)"
                                "is not done." % dependency)

    self._apply_end_of_command(args, thisTask, cmd_args, payload, setup)

    # Call submission command end pipe payload into stdin.
    if args.verbose or args.debug or verbose or debug:
      print("\nSubmission command:")
      print((" ".join(cmd_args)).replace(" ; ", "\n")+"\n")
      print("Setup+Payload:")
      print("\n".join(setup+payload))
      
    if args.debug or debug:
      thisTask.jobid = self.debugJobID
      self.debugJobID -= 1 #modify debug (fake) job ID
      thisTask.setStatusCode(taskStatus.submitted)
      return True
    else:
      print("Submitting task '{:s}'".format(thisTask.identifier))
      status_code, stdout, stderr = \
        self._execute_submission_command(thisTask, cmd_args, payload, setup)

      # Sets status and job id (->depends on non-abstract controller)
      return self._handle_submit_response(thisTask, stdout, stderr, status_code)

  def _execute_submission_command(self, thisTask, cmd_args, payload, setup):
    """
    Communicate the task to the batch system. By default this starts a
    subprocess and pipes the script into stdin.
    """
    process = subprocess.Popen(cmd_args, stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                               universal_newlines=True)

    (stdout, stderr) = process.communicate(("\n".join(setup+payload)))
    status_code = process.returncode

    return status_code, stdout, stderr

  def updateTaskStatus(self,args,thisTask):
    """
    updates and returns the statusCode member of thisTask and its dependencies. Note that this does not poll the batch system again but uses the internal cached status dict of the controller
    """
    initialTimestamp = thisTask.statusTimestamp #we might change the statusCode at some point, but we always want to refer to the timestamp at the time of calling this method

    if args.verbose or args.debug: print( "updating status for task '{:s}'".format(thisTask.identifier) )
    for t in thisTask.dependencies:
      if (self.updateTaskStatus(args,t) == taskStatus.failed):
        #if any of the dependencies are failed, we also consider this task to be failed.
        thisTask.setStatusCode(taskStatus.failed)
    if (thisTask.statusCode == taskStatus.done) or (thisTask.statusCode == taskStatus.tosubmit) or (thisTask.statusCode == taskStatus.failed):
      #these states are terminal states (or not to be changed here in case of 'tosubmit') and hence won't be changed. We simply return them
      return thisTask.statusCode
    if thisTask.statusCode == taskStatus.undefined:
      if args.verbose or args.debug:
        print("task '{:s}' is currently in undefined state, checking dependencies...".format(thisTask.identifier))
      #this task has never been asigned a stauts so far

      #if any dependency task is not done, then this task also needs to be submitted
      if not isinstance(thisTask.dependencies,list):
        raise TypeError("dependencies member of task '{:s}' is not a list".format(task.identifier))
      for dep in thisTask.dependencies:
        if not isinstance(dep,task):
          print("[WARNING] Found non-task object in list of dependencies of task '{:s}', ignoring!".format(thisTask.identifier))
          continue
        depStatus = dep.statusCode
        if not (depStatus == taskStatus.done):
          if depStatus == taskStatus.failed:
            thisTask.setStatusCode(taskStatus.failed)
            return thisTask.statusCode
          else:
            thisTask.setStatusCode(taskStatus.tosubmit)
          if args.verbose or args.debug:
            print("Some dependency was not done yet, hence this task was assigned the state '{:s}'".format(taskStatus.names[thisTask.statusCode]))
          #do not return just yet, the assignment of tosubmit might only be temporary as we should still check if the task might already be submitted/running!

      #if we reach this point we either have no dependencies or all dependencies are done or at least one is failed
      #if a task is still running, we don't consider it done just yet. All its output files may be present but they might still be (Simulation) Work In Progress ;)
    #if a task is not in a terminal state nor to be submitted we check if it's still running or already done (and update the stored status accordingly)

    if (initialTimestamp < self.queueStatusTimestamp) or (initialTimestamp < 0):#check that the cache is not outdated compare to this task's internal status. Otherwise we can't really tell if the task, e.g., has just been started, immedeadly started writing its output file but isn't done with it yet.
    #similarly we can't tell if the job has failed as the job has just been submitted and no output is written yet.
    #hence, we simply preserve the current statusCode
      if args.verbose or args.debug: print("checking task '{:s}' against the queue cache".format(thisTask.identifier))
      if (thisTask.identifier in self.queueStatusCache):
        if args.verbose or args.debug: print("task '{:s}' is found in queue cache".format(thisTask.identifier))
        info = self.queueStatusCache[thisTask.identifier]
        if "statusCode" in info:
          thisTask.setStatusCode(info["statusCode"])
        else: #fallback if status code wasn't properly set
          print("[WARNING] No valid statusCode was cached for task '{:s}' even though a corresponding, queued job was. Considering it as failed... (Please inform the author of the submissionController class corresponding to your batch system type!")
          thisTask.setStatusCode(taskStatus.failed)
        #if the task doesn't have a jobid assigned yet (this shouldn't happen if it's submitted, but there might have been a timeout during the submission in which case the job might still have been registered by the batch system but no jobid was returned by the submission call)
        if (thisTask.jobid is None) and "jobid" in info:
          thisTask.jobid = info["jobid"]
      elif len(thisTask.output) == 0 and \
            thisTask.statusCode == taskStatus.undefined:
        if args.debug or args.verbose: print( "Task '{:s}' has no outputs, marking it for submission.".format(thisTask.identifier) )
        thisTask.setStatusCode(taskStatus.tosubmit)

      elif len(thisTask.output) == 0 and \
            thisTask.statusCode in [taskStatus.submitted, taskStatus.running]:
        if args.debug or args.verbose: print( "Task '{:s}' has no outputs, it was previously submitted, considering it as done.".format(thisTask.identifier) )
        thisTask.setStatusCode(taskStatus.done)

      elif thisTask.outputsPresent(args) and \
            thisTask.statusCode != taskStatus.tosubmit: #we have no record that this task would be currently running or submitted. hence, if all output files are fine, we consider it done
        if args.debug or args.verbose: print( "All outputs of task '{:s}' are present, considering it as done.".format(thisTask.identifier) )
        thisTask.setStatusCode(taskStatus.done)


      elif (thisTask.statusCode == taskStatus.undefined):
        #job is not done yet
        if args.verbose or args.debug: print( "Task '{:s}' seems not to be done yet, assigning 'tosubmit' statusCode".format(thisTask.identifier) )
        thisTask.setStatusCode(taskStatus.tosubmit)
      elif thisTask.statusCode == taskStatus.tosubmit:
        if args.verbose or args.debug: print( "Task '{:s}' seems not to be done yet, is already in 'tosubmit' state".format(thisTask.identifier) )
        pass
      else:
        #if the task has already been submitted at some point (it's status is neither undefined not tosubmit) but we can neither find it in the list of still running tasks nor its outputs are complete and valid then the task must have failed:
        thisTask.setStatusCode(taskStatus.failed)



    #a safety check in case there is a stupid bug in the above logic...
    if thisTask.statusCode == taskStatus.undefined:
      raise RuntimeError("Failed to update status code of task '{:s}'. This should never happen!".format(thisTask.identifier))
    return thisTask.statusCode

  def _submitTasksInternal(self,args, thisTask, debug=False, verbose=False, nmax=inf ):
    """
    Arguments:
      -p0: arg parser namespace
      -p1: task object to be submitted (possibly including dependencies)
      -debug: bool, run in debug mode if True. Defaults to False
      -verbose: bool, verbose printout is True. Defaults to False
      -nmax: submit only up to this many individual tasks. Defaults to infinity
    Returns number of tasks submitted
    #TODO: change: Returns submissionSummary instance (members: nSub and nExp)
    """
    #create a new submissionSummary instance
    summary = submissionSummary(nSubmitted=0,nExpected=0)
    if thisTask.statusCode == taskStatus.done:
      if verbose or debug or args.debug or args.verbose: print( "Skipping task '{:s}' as its state 'done'".format(thisTask.identifier) )
      #The trivial case: this task is already done, so we don't even bother
      return summary
    if thisTask.statusCode in [taskStatus.submitted,taskStatus.running]:
      if verbose or debug or args.debug or args.verbose: print( "Skipping task '{:s}' as its state is '{:s}'".format(thisTask.identifier, taskStatus.names[thisTask.statusCode]) )
      #Another trivial case: this task is submitted/running, so we have nothing to do here
      return summary
    if not thisTask.statusCode in [taskStatus.tosubmit]:
      errMsg = "[WARNING] Refusing to submit task '{:s}' as its state is '{:s}' (expected: 'tosubmit')".format(thisTask.identifier, taskStatus.names[thisTask.statusCode])
      print( errMsg )
      #Another trivial case: this task is submitted/running, so we have nothing to do here
      summary.error = True
      summary.errString = errMsg
      summary.nExp += 1
      return summary

    for dep in thisTask.dependencies:
      #submit dependency tasks if they are still to be submitted
      if dep.statusCode == taskStatus.tosubmit:
        summary += self._submitTasksInternal(args,dep, debug, verbose, nmax-summary.nSub)
        if summary.error:
          return summary #return the current summary. There was an error further down the line, hence we don't even try to continue submitting stuff
      elif verbose or debug:
        print( "Skipping dependency '{:s}' as its state is '{:s}'".format(dep.identifier, taskStatus.names[dep.statusCode]) )
    if self._supports_simple_dependencies():
      #case: we have simple dependency support, hence we can try to submit right away
      #safety check: are all dependencies actually submitted or done?
      if nmax>summary.nSub:
        for dep in thisTask.dependencies:
          if not (dep.statusCode in [taskStatus.done, taskStatus.running, taskStatus.submitted]):
            if nmax > 0 and dep.statusCode == taskStatus.tosubmit and nmax == summary.nSub:
              # This is the case when the submit limit nmax was reached
              # somewhere above. In this case this is not an error
              pass
            else:
              errMsg = "[WARNING] Some dependency tasks of task '{:s}' are in an unexpected state, they should be running, submitted or done (is state: '{:s}').".format(thisTask.identifier,taskStatus.names[dep.statusCode])
              print( errMsg )
              summary.error = True
              summary.errString = errMsg
              summary.nExp += 1
              return summary

        if self._submitSingleTask(args, thisTask, debug, verbose):
          summary.nSub += 1
          summary.nExp += 1
          return summary
        else:
          summary.error = True
          summary.errString = "Failed to submit single task '{:s}'".format(thisTask.identifier)
          summary.nExp += 1
          return summary
      else:
        #we reached the max number of tasks to be submitted: stop.
        summary.nExp += 1
        return summary

    else:
      #case: no simple dependency support, we need to manage some more stuff ourselves
      for dep in thisTask.dependencies:
        if not dep.statusCode in [taskStatus.done]:
          #some dependency is not done yet, hence we delay the submission of this task
          if debug or verbose or args.debug or args.verbose:
            print("[INFO] Postponing execution of task '{:s}' as some of its dependencies are not yet done".format(thisTask.identifier))
          #add this task to the number of expected ones but not to the number of submitted ones
          summary.nExp += 1
          return summary
      if nmax>summary.nSub:
        # actually submit the task
        if self._submitSingleTask(args, thisTask, debug, verbose):
          summary.nSub += 1
          summary.nExp += 1
        else:#there was an issue submitting this task
          summary.error = True
          summary.errString = "Failed to submit single task '{:s}'".format(thisTask.identifier)
          summary.nExp += 1
          return summary
      else:
        #we reached the max number of tasks to be submitted: stop.
        summary.nExp += 1
        return summary

    return summary

  @staticmethod
  def __printTaskStatus(thisTask, stopAtStatusCodes=[taskStatus.done], indent=0, alreadyPrinted=set() ):
    """
    Prints the status of the provided task and recurses to dependency tasks if
    present and the provided tasks status is not in the list of statusCodes
    'stopAtStatusCodes'.

    Returns True if a line for thisTask has been printed and False if not (due
    to the task already having been printed at some point, i.e., it was in the
    alreadyPrinted set when this function was called)
    """
    #task.statusCode is the enumerator for our internal status, task.status can be any string
    #and depends mostly on the submissionController
    if thisTask in alreadyPrinted:
      return False
    print("  "*indent + "[{:s}] ".format(taskStatus.names[thisTask.statusCode]) + thisTask.identifier)
    alreadyPrinted.add(thisTask)
    if not (thisTask.statusCode in stopAtStatusCodes):
      allPrinted = True
      for dep in thisTask.dependencies:
        allPrinted = submissionControllerBase.__printTaskStatus(dep,stopAtStatusCodes = stopAtStatusCodes, indent = indent+1) and allPrinted
      if not allPrinted:
        print("  "*(indent+1) + "(...)")
    return True

  #main submission method. This is what the user mainly calls
  def submitTasks(self, args, tasks, debug=False, verbose=False):
    """
    Positional arguments: arg parser namespace, list of task objects
    Operates in debug (verbose) mode if debug (verbose) arguments are True or the arg parse namespace members of the same name are True
    If task dependencies are defined only terminal node tasks need to be provided, dependencies are automatically considered in this case
    It is the users responsibility to ensure there are no loops in the task dependency graph.
    This method differentiates cases where the non-abstract implementation of the submissionController has proper support for job dependencies (in which case the job dependency support of the underlying batch system is used) or there is no suitable support for job dependencies in which case this method will continue looping and take care of managing the dependencies itself.
    Aborts operation if args.ntest tasks have been submitted unless args.ntest<=0

    Raises exceptions in case of non-recoverable errors or illegal configurations.
    Raises exception in case input files of tasks are not found.
    If args.checktimestamp is True the following exceptions might be raised as well:
    Raises exception in case a task is already submitted but its input files
    or the output of a dependency task have changed after the time of submission of the task
    Returns: number of jobs submitted or negative value in case of errors.
    """
    allSubmitted = False
    allDone = False
    nSubmitted = 0
    nActive = 0 #number of active/pending/currently scheduled jobs
    maxNumberAttempts = 3
    secondsBetweenAttempts = 1
    nLimit = self._get_max_concurrent(args.maxConcurrent)
    while (not allSubmitted) or (not allDone and args.wait):
      allSubmitted = True
      allDone = True
      nFailedAttempts = 0
      while nFailedAttempts < maxNumberAttempts and not self._updateQueueStatus():
        nFailedAttempts += 1
        if not nFailedAttempts == maxNumberAttempts:
          print("Queue couldn't be updated. Trying {:d} more times.".format(maxNumberAttempts-nFailedAttempts))
          time.sleep(1)
      if nFailedAttempts == maxNumberAttempts:
        print("Queue couldn't be updated. Aborting submission.")
        return -1
      nActive = len(self.queueStatusCache)
      #first update all tasks status
      for t in tasks:
        if args.verbose or args.debug or verbose or debug: print( "Requesting task status update for task '{:s}'".format(t.identifier) )
        self.updateTaskStatus(args,t) #will automatically trickle down to dependencies
        if not (t.statusCode == taskStatus.done):
          allDone = False

      if not args.checkmissing:
        #then run the actual submission logic
        for t in tasks:
          nmax = min( ( max(0,nLimit-nActive) if nLimit>0 else inf )  ,  (args.ntest-nSubmitted if args.ntest>0 else inf) )
          summary = self._submitTasksInternal(args,t,debug,verbose, nmax=nmax )
          if not summary:
            #two options: there was an error or not all tasks have been submitted yet
            if summary.error:
              print ("There was an error during the submission, details follow below:" )
              print ( summary.errString )
              raise RuntimeError("Submission failed for task '{:s}' or one of its dependencies".format(t.identifier))
            else:
              allSubmitted = False
          nSubmitted += summary.nSub
          nActive += summary.nSub
          if not (args.ntest<1 or nSubmitted<args.ntest):
            print( "[INFO] Reached specified maximum number of jobs to be submitted, stopping." )
            return

        if not allSubmitted:#wait until next iteration
          print("Not all jobs have been submitted, yet. Will check again in %g seconds..." % self.poll_delay)
          time.sleep(self.poll_delay) #we need to wait a bit in order to prevent spamming the batch system with requests (this prevents pesky emails from your admins and contributes to keeping your colleagues in a good mood;)
        elif args.wait and (not allDone):
          print("All jobs have been submitted but not all jobs are done, yet. Will check again in %g seconds..." % self.poll_delay)
          time.sleep(self.poll_delay) #we need to wait a bit in order to prevent spamming the batch system with requests (this prevents pesky emails from your admins and contributes to keeping your colleagues in a good mood;)
      else: #checkmissing mode, no actual submission, only printing
        tskDone = [tsk for tsk in tasks if tsk.statusCode == taskStatus.done]
        if len(tskDone) > 0:
          print("The following (terminal node) tasks are 'done':")
          for t in tskDone:
            self.__printTaskStatus(t, indent=1) #will not recurse
        tskNotDone = [tsk for tsk in tasks if not (tsk.statusCode == taskStatus.done)]
        if len(tskNotDone) > 0:
          alreadyPrinted = set()
          print("The following tasks are not (yet) 'done':")
          for t in tskNotDone:
            self.__printTaskStatus(t, indent=1, alreadyPrinted=alreadyPrinted)

    if self._needs_empty_queue():
      self._wait(args, tasks)

    return allDone

  def _get_max_concurrent(self,suggestion):
    """
    Some submission controllers might want to mess with the number of
    simultaneously submitted processes
    """
    return suggestion
  
  def _needs_empty_queue(self):
    """
    Some submission controllers might need to wait until all jobs are done or
    killed before they return from submitTasks. Override _needs_empty_queue to
    trigger a wait loop.
    """
    return False

  def _wait(self, args, tasks):
    """
    Some submission controllers might need to wait until all jobs are done or
    killed before they return from submitTasks. Override _needs_empty_queue to
    trigger this logic.

    Wait until all jobs are done or killed.
    """
    while True:
      if not self._updateQueueStatus():
        print("Queue couldn't be updated. Trying again in %g seconds" % self.poll_delay)
      else:
        #also make sure tasks' statusCodes are updated
        for t in tasks:
          if args.debug: print( "Requesting task status update for task '{:s}'".format(t.identifier) )
          self.updateTaskStatus(args,t) #will automatically trickle down to dependencies

        if len(self.queueStatusCache) == 0:
            return

        print("Not all jobs are done (or killed), yet. Will check again in %g seconds..." % self.poll_delay)
      time.sleep(self.poll_delay)


  def __buildRemoteCommand(self, lines):
    # Arguments: a list of commands to be executed on the batch node. Each element becomes one line in the return value, potentially concatenating elements of the command if given as a (sub)list
    #returns the command to be executed on the batch node
    hasSubLists = len([True for l in lines if isinstance(l, list)]) > 0 #only go to advanced mode ifthere are sub-lists (in which case each sub-list will be one line/command to be executed on the node
    stdinlines = [ concat(pieces) for pieces in lines if len(pieces) > 0]
    s = "\n".join(stdinlines) if hasSubLists else " ".join(stdinlines)
    return s.encode('ascii')
