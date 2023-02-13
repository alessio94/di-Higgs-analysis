#!/usr/bin/env python

import os.path
import sys
import time
from .common import isstr

class taskStatus:
  #enum class for (internal) task status:
  # undefined : default, not checked yet
  # running   : task is currently being executed
  # submitted : task is known to the batch system but not yet started (or actual execution state unknown)
  # tosubmit  : task is not yet known to the batch system but is to be submitted
  # done      : task is completed
  # failed    : task was submitted or running at some point. It is set to broken when it is neither still known to the batch system nor are all output files present or if any dependency is considered as failed
  undefined, running, submitted, tosubmit, done, failed = range(6) #adjust range argument according to number of enums!
  names = {}
#this is on purpose outside the definition of the taskStatus class!
taskStatus.names = { getattr(taskStatus,name):name for name in dir(taskStatus) if isinstance(getattr(taskStatus,name),int) and (not name.startswith('__')) }

#helper object storing properties and status of a job ('task')
class task:
  def setSetup(self,setup):
    if isstr(setup):
      self.setup = str(setup).split("\n")
    elif isinstance(setup,list):
      self.setup = setup
    else:
      raise TypeError("Cannot set setup for task '{:s}': Invalid data type (must be string or list)".format(self.identifier))

  def setPayload(self,pl):
    if isstr(pl):
      self.payload = str(pl).split("\n")
    elif isinstance(pl,list):
      self.payload = pl
    else:
      raise TypeError("Cannot set payload for task '{:s}': Invalid data type (must be string or list)".format(self.identifier))


  def setOutput(self,outputs):
    #set the output files the job is expected to produce. Can be passed either as a single string separated by spaces or a
    if isstr(outputs):
      self.output = str(outputs).split(" ,")
    elif isinstance(outputs,list):
      self.output = list(map(str,outputs))
    else:
      raise TypeError("Cannot set expected outputs for task '{:s}': Invalid data type (must be string or list)".format(self.identifier))

  def setInputs(self,inputs):
    #set the input files required to run this task. Can be passed either as a single string separated by spaces or a list
    if isstr(inputs):
      self.input = str(inputs).split(" ")
    elif isinstance(inputs,list):
      self.input = inputs
    else:
      raise TypeError("Cannot set required inputs for task '{:s}': Invalid data type (must be string or list)".format(self.identifier))

  def addDependency(self, other):
    #add a task or a list of tasks to the list of dependencies unless it is already in. Returns True is all dependencies were added, False otherwise.

    #in case we got a list, we iterate over its elements and call this function recursively
    if isinstance(other,list):
      retVal = True
      for o in other:
        #note: we might have to skip o if it is a list here to prevent infinite recursions. For now, blame the user if that happens (s)he passes such an object complex.
        if not self.addDependency(o):
          retVal = False
      return retVal
    #treatment of single task instances:
    if not isinstance(other,task):
      raise TypeError("Cannot add object to list of dependencies as it is not a task!")
      return False
    if other in self.dependencies:
      return False
    else:
      self.dependencies.append(other)

  def removeDependency(self, other):
    #remove a task from the list of dependencies. Returns True is an element was removed, False otherwise
    if (isinstance(other,task) or isstr(other)) and other in self.dependencies:
      self.dependencies.remove(other)
      return True
    else:
      return False

  def setStatusCode(self, status):
    if not isinstance(status,int):
      raise TypeError("Failed to set status code for task '{:s}': invalid type".format(str(self.identifier)))
    self.statusCode = status
    self.statusTimestamp = time.time()

  def getNewestOutputTimestamp(self):
    """
    Checks the timestamp of all specified/expected output files of this task and returns the latest timestamp, i.e., the timestamp of the newest file.
    Returns 0 if no output files are specified (->all done at beginning of epoch)
    Returns int("inf") if an output file can not be found (->file is yet to be produced)
    """
    #TODO: is the int("inf") return value what we want? one could also argue that some intermediate file has been deleted to free some disk space for subsequent steps
    if not isinstance(self.output,list):
      raise TypeError("task.output member of task '{:s}' has unexpected data type, must be a list".format(self.identifier))
      return False
    outTimestamp = 0
    for f in self.output:
      if os.path.isfile(i):
        outTimestamp = max(outTimestamp, os.path.getmtime(i)) #get timestamp of newest input file
      else: #file could not be found, hence specify its timestamp as "far in the future"
        outTimestamp = int("inf")
    return outTimestamp

  def outputsPresent(self, args):
    # checks if all expected output files of a task are present. To check if a task has finished successfully combine 't.isDone(args) and not t.isRunning(args)' .
    # Arguments: arg parser result. Requires "checktimestamp" member on arg parser result.
    # Returns True if all expected output files are present, False otherwise
    # Raises a TypeError (and returns False) if the tasks input and/or output member is not a list or an expected input file could not be found
    if args.debug or args.verbose:
      print("Checking for presence of outputs for task '{:s}'".format(self.identifier))
    if not isinstance(self.output,list):
      raise TypeError("task.output member of task '{:s}' has unexpected data type, must be a list".format(self.identifier))
      return False
    outTimestamp = 0
    inTimestamp = -1
    #if we should take timestamps of files into account, we need to get the timestamp of our input files:
    if args.checktimestamp:
      if not isinstance(self.input,list):
        raise TypeError("task.input member of task '{:s}' has unecpected data type, must be a list".format(self.identifier))
      for i in self.input:
        if os.path.isfile(i):
          inTimestamp = max(inTimestamp, os.path.getmtime(i)) #get timestamp of newest input file
        else: #a required input file is not present, this is a fatal error
          sys.exit("Task '{:s}' expects input file '{:s}' which could not be found! Please note: if a task requires the output of some previous step then the task for that step should be added as a dependency, (expected) input files should only list external files that are not produced by another task (configuration files, weight files,...)".format(self.identifier,i)) #effectively raises SystemExit exception
          return False
      #if dependencies are already done they might have already re-run but this task hasn't. Hence, we also need to check the timestamps of dependencies outputs and compare them to our outputs (essentially treating outputs from dependencies like an input).
      for dep in self.dependencies:
        for out in dep.output:
          if os.path.isfile(out): #if the file doesn't exist it's up to the other task to consider itself as not-done
            inTimestamp = max(inTimestamp, os.path.getmtime(out))
    for f in self.output:
      if args.verbose or args.debug: print("Checking for presence of file '{:s}'".format(f))
      if os.path.isfile(f) and ( (not args.checktimestamp) or (os.path.getmtime(f) > inTimestamp) ): #TODO: >= or > ?
        pass
      elif (args.verbose or args.debug) and os.path.isfile(f) and args.checktimestamp and (os.path.getmtime(f) == inTimestamp):
        print("task '{:s}': modification time of output file '{:s}' is identical to timestamp of input or dependency output. Dependencies are printed below".format(self.identifier, f))
        for dep in self.dependencies:
          print(str(dep))
      else:
        if (args.verbose or args.debug):
          if not os.path.isfile(f):
            print("Considering outputs of task '{:s}' as not being complete due to missing file '{:s}'".format(self.identifier,f))
          else:
            print("Considering outputs of task '{:s}' as not being completed due to timestamp requirement regarding file '{:s}' (timestamp: {:f}, newest input/dependency file has {:f}) ".format(self.identifier,f, float(os.path.getmtime(f)), float(inTimestamp)))
        #return false uppon the first file that doesn't match our requirements
        return False
    #or return true is all output files are ok.
    if (args.verbose or args.debug): print("All outputs for task '{:s}' are present and up-to-date".format(self.identifier))
    return True

  def getJobID(self, args, controller):
    """
      Positional arguments: submissionController
      Returns the ID of this taks if available, else returns None.
      ID is taken from self.jobid if not None, else the task's status is updated
      from the cached information of the submissionController, possibly yielding
      a not-None jobid which is then returned. If also via the
      submissionController no jobid could be found, None is returned.

      Raises TypeError is argument provided is not a submissionController.
    """
    #do not import this earlier, as the class wouldn't be defined yet at the top of this file
    from .submissionControllerBase import submissionControllerBase

    if not isinstance(controller, submissionControllerBase):
      raise TypeError("[task.getJobID] invalid argument type, must be a submissionController instance")
    if self.statusTimestamp < controller.queueStatusTimestamp or self.statusTimestamp < 0: #TODO: clarify: should we always update the state of the job or only if no jobid is assigned yet?
      controller.updateTaskStatus(args, self) #this will also set the jobid if available in the queue cache
    return self.jobid

  def __init__(self, identifier, payload, args=None, time=None, memory=None, ncores=None, queue=None,
               setup=None, inputs=None, outputs=None, logFile=None,
               errFile=None, account=None, dependencies=None, logPath="logs"):
    """
    Create a new Task object.

    The argument names coincide with the corresponding task attribute.

      identifier:   String identifying the job. If possible the batch system
                    will use this as the jobs name.
      payload:      String used as bash script to be executed. Internally this
                    is managed as a list of line strings.
      args:         Argument object from argparse. Contains general methods.
                    If this argument is given, its values for time, memory,
                    ncores and queue are used as a fallback/default.
      memory:       Memory requirement of the task in MB.
      time:         Time limit in minutes.
      setup:        String used as bash script to setup the job including
                    shebang
      inputs:       List of file paths which this job uses as input
      outputs:      List of file paths which this job creates
      logFile:      Path to the log file. If omitted, identifier is used to
                    construct a log file path.
      errFile:      Path to the stderr log file. If omitted, identifier is
                    used to construct a log file path.
      account:      Account used to submit the jobs/ressources used to be
                    charged on.
      dependencies: List of task objects which have to be 'done' before this
                    task can be executed.

    """
    #ctor, taking return value of argparse.parse_args(), and identifier (string), and optionally setup and payload instructions and expected output files as well as log (stdout) / error (stderr) file paths
    # setup instructions, payload instructions and expected output files should be passed as lists or space separated strings (each element is considered as one output file or one argument in the payload/setup instructions

    try:
      from QFramework import TQPathManager
      pathManager = TQPathManager.getPathManager()
    except ImportError:
      pathManager = None

    # initialize arguments with empty lists
    if setup is None: setup = []
    if inputs is None: inputs = []
    if outputs is None: outputs = []
    if dependencies is None: dependencies = []

    if args:
    # options expected to be defined on argparse object: time (str), memory (float, in MB), nCores (int), queue (str)
      self.time = args.time if time is None else time # time limit of the task
      self.memory = float(args.memory) if memory is None else memory# memory in MB
      self.ncores = int(args.ncores) if ncores is None else ncores# number of CPU cores
      self.queue = str(args.queue) if queue is None else str(queue)# queue the task should be submitted to
      self.account = str(args.account) if account is None else str(account) #can be used to use a different account on the batch system (e.g. in case of different quotas on different accounts)
    else:
      self.time = time
      self.memory = memory
      self.ncores = ncores
      self.queue = queue
      self.account = account
    self.identifier = str(identifier) # identifier of this task
    self.logFile = str(logFile) if logFile and len(str(logFile))>0 else logPath+"/"+str(identifier)+".log"
    if pathManager: self.logFile = pathManager.getTargetPath(self.logFile)
    self.errFile = str(errFile) if errFile and len(str(errFile))>0 else logPath+"/"+str(identifier)+".stderr"
    if pathManager: self.errFile = pathManager.getTargetPath(self.errFile)
    self.setup=[]
    self.setSetup(setup)
    self.payload=[]
    self.setPayload(payload)
    self.output=[] #output files the task is expected to produce
    self.input=[] #list of input files required for task. Note: this does NOT include files produced by dependency tasks!
    self.setOutput(outputs)
    self.setInputs(inputs)
    self.dependencies = dependencies #a list of tasks which are dependencies for this task to start execution (might not be respected by all controllers, especially submissionControllerLocal!)
    self.info = self.identifier #suposedly used to store arbitrary additional information
    self.status = "" #store the status of the task when, e.g., polling the batch system. Intended to be user readable, do not rely on the precise value!
    self.statusCode = taskStatus.undefined #internal status code. To be used to test/store the technical status of the task
    self.statusTimestamp = -1 #timestamp of the last status update
    self.jobid = None #job ID according to the backend batch system

  def __hash__(self):
    return hash(self.identifier)

  def __eq__(self,other):
    #compare if this object is equal to another: Two tasks are considered equal if their identifier is equal. As an alternative, a task can simply be compared to a string which is compared to this tasks identifier
    if isinstance(other,task):
      return self.identifier == other.identifier
    else: #if other is a string or comparable to a string. All other cases should yield an error
      return self.identifier == other

  def __str__(self):
    #compile a string representation of this object (for pretty printing)
    rep = "Task '{:s}'\n".format(self.identifier)
    if self.time is not None:
        rep += "  time    : {:d} min\n".format(int(self.time))
    if self.memory is not None:
        rep += "  memory  : {:.2f} MB\n".format(self.memory)
    if self.ncores is not None:
        rep += "  nCores  : {:d}\n".format(self.ncores)
    if self.queue is not None:
        rep += "  queue   : {:s}\n".format(repr(self.queue))

    rep += "  logFile : {:s}\n".format(repr(self.logFile))
    rep += "  errFile : {:s}\n".format(repr(self.errFile))
    rep += "  setup   : {:s}\n".format(repr(self.setup))
    rep += "  payload : {:s}\n".format(repr(self.payload))
    rep += "  input   : {:s}\n".format(repr(self.input))
    rep += "  output  : {:s}\n".format(repr(self.output))
    rep += "  info    : {:s}\n".format(repr(self.info))
    rep += "  jobid   : {:s}\n".format(str(self.jobid) or "")
    #if (not self.account is None) and len(str(self.account)) > 0:
    rep += "  account : {:s}\n".format(repr(self.account))
    rep += "  status  : '{:d} ({:s})'\n".format(self.statusCode,taskStatus.names[self.statusCode] if self.statusCode in taskStatus.names else "<invalid status code>")

    return rep
