#!/usr/bin/env python

"""this submodule contains helper functions to handle the submission of batch jobs"""

import os.path
import argparse
import time
import sys
#importing individual components and classes so everything is available from the submissionHelpers name space
from SubmissionHelpers.task import task,taskStatus
from SubmissionHelpers.submissionControllerBase import *
from SubmissionHelpers.submissionControllers import *

from . import common

def configureDefaultArgumentParser(parser):
  """configure an ArgumentPraser with basic arguments already set"""
  parser.add_argument('--debug', action="store_const", const=True, default=False, help='print actions instead of performing them')
  parser.add_argument('--ntest', default=-1, type=int, help='submit only up to this number of (test) jobs. If value is <= 0 no limit is imposed.')
  parser.add_argument('--logpath', default="logs", help='directory where the log files should appear')
  parser.add_argument('--output', default="batchOutput", help='directory to write output to')
  parser.add_argument('--identifier', default=str(int(time.time())), help='identify this submission run')
  parser.add_argument('--ncores', default=1, type=int, help='how many cores to be used in local mode / how many cores (per job) to request on batch nodes')
  controllers = ["auto"]
  for ctrl in __inheritors(submissionControllerBase):
    if ctrl and ctrl._is_available() :
      name = ctrl.__name__.lower()
      if name.endswith("controller"): name = name[0:-10]
      controllers.append(name)
  parser.add_argument('--submit', default="auto", choices=controllers, type=str, help='submission type (submission binary) to use. "auto" means an automatic detection should be attempted')
  parser.add_argument('--verbose', '-v', action="store_const", const=True, default=False, help='provide the user with verbose output')
  parser.add_argument('--queue', default='', help='name of the queue to submit to')
  parser.add_argument('--memory', default=4000., type=float, help='memory to be requested per job (in MB)')
  parser.add_argument('--time', default=-1, help='request runtime different from the queues default value in minutes. If negative (=default) no time argument is to be applied')
  parser.add_argument('--account', default='', help='specify an account the used ressources should be charged on (only needed on a few clusters)')
  parser.add_argument('--checktimestamp', action="store_const", const=True, default=False, help='when checking if a certain processing step already has a valid output, also check that the timestamp of all known input files are older than the one of the output files')
  parser.add_argument('--voms', action="store_const", const=True, default=False, help='call "voms-proxy-init --voms atlas" in every job - requires passwordless .globus!')
  parser.add_argument('--checkmissing', action="store_const", const=True, default=False, help='do not submit/run any jobs but only check which output files are available and which are missing')
  parser.add_argument('--maxConcurrent', default=-1, type=int, help='If >0 will limit the number of jobs that can be queued/running/executed simultaneously. When the limit is reached the submission will pause untill the number of queued/running/executed jobs has reduced.')
  parser.add_argument('--wait', action="store_const", const=True, default=False, help='wait for all jobs to finish before exiting')
  analysisSetup = os.environ.get("ANALYSIS_SETUP")
  if not analysisSetup:
    analysisSetup = os.environ.get("CAF_SETUP")
  if analysisSetup:
    defaultsetup = [analysisSetup]
  else:
    defaultsetup = []
  parser.add_argument('--setup', nargs="+", help='path to one or more setup scripts', default=defaultsetup)
  parser.add_argument('--allowArgChanges', action="store_const", const=True, default=False, help='allow command line options to be modified at runtime, e.g., via modifier lines in job definition files')
  parser.add_argument('--pythonpath', help='PYTHONPATH variable to be used by the job (required when submitting without environment setup)')
  parser.add_argument('--pythonbin', help='path to python binary to be used by the job (required when submitting without environment setup)')
  parser.add_argument("--enable-default-library-setup",action="store_true",dest="defaultLibs",help="enable default library setup instructions",default=True)
  parser.add_argument("--disable-default-library-setup",action="store_false",dest="defaultLibs",help="disable default library setup instructions")
  parser.add_argument("--enable-default-setup",action="store_true",dest="doSetup",help="enable default setup instructions",default=True)
  parser.add_argument("--disable-default-setup",action="store_false",dest="doSetup",help="disable default setup instructions")    
  parser.add_argument("--k8s-image", default="busybox", help="Name of the Docker image to use in the Kubernetes Pod")
  parser.add_argument("--k8s-registry-secret", default="sh-k8s-registry-secret", help="Name of the Kubernetes Secret used to pull the Docker image")
  parser.add_argument("--k8s-volume-claim", default="ntuples-claim", help="Name of the Kubernetes PersistentVolumeClaim bound to a PersistentVolume holding the input data")
  parser.add_argument("--k8s-mount-path", default="/ntuples", help="Path inside the Docker container where the PersistentVolume is mounted")
  

def createDefaultArguments(inputArgs=""):
  from argparse import ArgumentParser
  parser = ArgumentParser()
  configureDefaultArgumentParser(parser)
  args = parser.parse_args(inputArgs)
  return args

def GeneralArgumentParser():
  """Create a pre-configured argument parser with some default options for handling a custom executable as well as config files"""
  from . import common
  parser = common.createEmptyArgumentParser("submissionHelper based job submission tool")
  parser.add_argument("executable",type=str,metavar="executable.py",help="executable to be submitted")
  common.configureBasicArgumentParser(parser)
  configureDefaultArgumentParser(parser)
  return parser

def MinimalArgumentParser(executable=None):
  """Create a pre-configured argument parser with some default options"""
  from . import common
  parser = common.createEmptyArgumentParser("submissionHelper based job submission tool")
  #parser.add_argument("--executable",type=str,metavar="executable.py",help="executable to be submitted",default=executable)
  #parser.add_argument("config",type=str,nargs="+",metavar="config.cfg",help="config to be submitted",default=[])
  configureDefaultArgumentParser(parser)
  return parser


def fractionOfOutputsPresent(t):
  # checks if all expected output files of a task are present. This is the default method to check if a task has finished successfully.
  # Arguments: task object
  # Returns: fraction of found output files w.r.t to all expected output files for this task.
  # Raises a TypeError (and returns -1) if argument is not of an expected type (task) or the tasks output member is not a list
  if not isinstance(t,task):
    raise TypeError("Argument provided is not a task object")
    return -1
  if not isinstance(t.output,list):
    raise TypeError("task.output member has unexpected data type, must be a list")
    return -1
  nExpected = len(t.output)
  nFound = 0.
  for f in t.output:
    if os.path.isfile(f):
      nFound += 1.
  if nExpected == 0:
    return 0
  return float(nExpected)/float(nFound)


def extractFinishedTasks(tasks, isTaskFinished=fractionOfOutputsPresent):
  #Arguments: list of task objects, isTaskFinished: function handle which takes a task object as argument
  #Returns: list of tasks considered as being finished and are removed from the list passed as the first argument.
  finishedTasks = []
  for t in tasks:
    if fractionOfOutputsPresent(t)>=1.:
      finishedTasks.append(t)
      t.status = "done"
      if t in tasks:
        tasks.remove(t)
  return finishedTasks

def extractRunningTasks(tasks, controller):
  # extracts tasks which are currently running or queued from a list of tasks
  # Arguments: list of task objects, submissionController
  # Returns: list of tasks which are currently running or queued. These are removed from the list of tasks passed as the first argument
  if not isinstance(controller,submissionControllerBase):
    raise TypeError("No valid submissionController provided")
  if not isinstance(tasks,list):
    tasks = [tasks] #transform into a list
  runningJobs = controller.getRunningJobs("")
  runningTasks = []
  for t in tasks:
    for j in runningJobs:
      if j['identifier'] == t.identifier:
        runningTasks.append(t)
        if 'info' in j:
          t.status = j['info']
        break
  for rt in runningTasks: #finally remove the runnig tasks from the full list
    tasks.remove(rt)
  return runningTasks
  #TODO

#additional utility functions and classes (mostly for internal use)
def rchop(thestring, ending):
  if thestring.endswith(ending):
    return thestring[:-len(ending)]
  return thestring

def findbin(name):
  from subprocess import Popen, PIPE, STDOUT
  p = Popen("type "+name, shell=True, stdout=PIPE, stdin=PIPE, stderr=PIPE, universal_newlines=True)
  retvals = rchop(p.communicate()[0].decode('ascii'),"\\n")
  s = str(retvals[0:-2]).split(" ")[-1].strip()
  return s

#possibly relevant functions for users
def adjustArgs(args,line):
  #parse a modifier line and adjust args object accordingly. Note: args will be modified ( one might want to use a copy of the original object created via 'import copy; clonedArgs = copy.copy(args) )!
  # Arguments : return value of argparse.parse_args()
  # Returns   : nothing
  if not args.allowArgChanges: return
  (key,val) = line.split("=",1)
  key = key.strip()
  val = val.strip()
  if hasattr(args,key):
    if isinstance(getattr(args,key) ,list): #special treatment for lists (->options)
      setattr(args,key, str(val).split())
    else:
      setattr(args,key, type(getattr(args,key))(val) ) #convert it to the right type
  else:
    print("[WARN] Cannot modify option '{:s}' on-the-fly, args has no such attribute! Ignoring".format(key))

def chunks(l, n):
  """Yield successive n-sized chunks from l."""
  return [ l[i:i + n] for i in xrange(0, len(l), n)]

#main steps for submitAna

def scanEnvironmentHelperRootCore(environment):
  try:
    rcsetup = os.path.join(os.path.dirname(os.environ.get("ROOTCOREBIN")),"rcSetup.sh")
  except:
    environment["RootCore"] = False
    return
  if not os.path.isfile(rcsetup):
    environment["RootCore"] = False
    return
  environment["RootCore"] = True
  environment["rcsetup"] = rcsetup
  environment["rcSetupPath"] = os.environ.get("ATLAS_LOCAL_RCSETUP_PATH")
  environment["unsetupROOTCore"] = ["source",os.path.join(environment["rcSetupPath"],"rcSetup.sh"),"-u"] if environment["rcSetupPath"] else []
  environment["unsetupROOT"] = ["export","ROOTSYS=''"]
  environment["setupROOTCore"] = ["source", rcsetup]

def scanEnvironmentHelperATLAS(environment):
  atlaspath = os.environ.get("ATLAS_LOCAL_ROOT_BASE")
  release = os.environ.get("ATLAS_RELEASE_BASE")
  if not release:
    environment["ATLAS"] = False
    return
  environment["ATLAS"] = True
  environment["atlaspath"] = atlaspath
  environment["setATLAS"] = ["export", "ATLAS_LOCAL_ROOT_BASE="+atlaspath ]
  asgCmake = os.environ.get("ATLAS_LOCAL_CMAKE_VERSION")
  environment["ATLAS_LOCAL_CMAKE_VERSION"] = asgCmake
  builddir = os.environ.get("ANALYSIS_BUILD_DIR")
  if not builddir:
    builddir = os.environ.get("CAF_BUILD_DIR")
  if not builddir:
    raise RuntimeError("ANALYSIS_BUILD_DIR environment variable is not set. Please verify that your analysis environment is correctly set up!")
  environment["BUILDDIR"] = builddir
  if os.path.isfile(os.path.join(builddir,".asetup.save")):
    environment["rel21"] = True
  else:
    environment["rel21"] = False
                    
  environment["setupATLAS"] = ["source", os.path.join("$ATLAS_LOCAL_ROOT_BASE","user","atlasLocalSetup.sh")]
  environment["rucio"] = ["lsetup", "rucio"]
  environment["voms"] = ["voms-proxy-init", "--voms", "atlas"]
  return True

def scanEnvironment(args):
  # Collect several environment variables and prepare commands to setup the analysis environment. Causes application to exit if problems are encountered
  # Arguments: return value of argparse.parse_args()
  # Returns: dictionary containing information about several environment variables and strings to set up the analysis environment (rcSetup, ...)
  # key               : value
  # string valued elements:
  # 'cwd'             : current working directory
  # 'rcsetup'         : command line to source RootCore setup script (rcSetup.sh) incl. path to it unless explicitly given in args
  # 'atlaspath'       : value of 'ATLAS_LOCAL_ROOT_BASE' environment variable
  # 'pythonpath'      : value of 'PYTHONPATH' environment variable unless explicitly given in args
  # 'pythonbin'       : path to python binary
  # 'shebang'         : "#!/usr/bin/env bash"
  # 'rcSetupPath'     : content of environment variable 'ATLAS_LOCAL_RCSETUP_PATH'
  # list valued elements
  # 'unsetupROOTCore' : command line elements to un-setup the rc environment
  # 'unsetupROOT'     : command line elements to clear ROOTSYS environment variable
  # 'setupROOTCore'   : command line elements to setup rc environment
  # 'setPYTHON'       : comment line elements to set PYTHONPATH to value corresponding to 'pythonpath' key
  # 'setATLAS'        : comment line elements to set ATLAS_LOCAL_ROOT_BASE to value corresponding to 'atlaspath' key
  # 'setupATLAS'      : command line elements to call atlasLocalSetup.sh (including path to it); a.k.a. "setupATLAS"
  environment = {}
  environment["cwd"] = os.getcwd()
  environment["shebang"] = "#!/usr/bin/env bash"

  # detect python
  if args.pythonpath:
    pythonpath = os.path.abspath(args.pythonpath)
  else:
    pythonpath = os.environ.get("PYTHONPATH")
  if not pythonpath:
    print("error: unable to read PYTHONPATH environment variable, please set with '--pythonpath' option")
    exit(0)
  environment["pythonpath"] = pythonpath
  if args.pythonbin:
    pythonbin = os.path.abspath(args.pythonbin)
  else:
    pythonbin = findbin("python")
  if not pythonbin:
    print("error: unable to locate correct 'python' binary, please set path with '--pythonbin' option")
    exit(0)

  # detect ROOT
  if "ROOTSYS" in os.environ:
    environment["setupROOT"] = ["source", os.path.join(os.environ.get("ROOTSYS"),"bin","thisroot.sh")]

  environment["setup_scripts"] = args.setup

  from SubmissionHelpers.common import findLibrarySetupScripts
  environment["library_setup_scripts"] = findLibrarySetupScripts()

  gccpath = findbin("g++")
  if gccpath:
    environment["g++"] = gccpath
    gccsetup = os.path.join(os.path.dirname(os.path.dirname(gccpath)),"setup.sh")
    if os.path.isfile(gccsetup):
      environment["setupG++"] = ["source",gccsetup]

  scanEnvironmentHelperATLAS(environment)
  scanEnvironmentHelperRootCore(environment)

  pythonsetup = os.path.join(os.path.dirname(os.path.dirname(pythonbin)),"Python-env.sh")
  if environment["ATLAS"]:
    atlaspython = os.environ.get("ATLAS_LOCAL_PYTHON_VERSION")
    if atlaspython == None:
      #protection against None. In principle this shouldn't be an issue as setting up an analysisRelease should set up an appropriate python version already
      environment["setPYTHON"] = []
    else:
      environment["setPYTHON"] = ["lsetup",'"python '+atlaspython+'"']
  elif os.path.isfile(pythonsetup):
    environment["setPYTHON"] = ["source",pythonsetup]
  else:
    environment["setPYTHON"] = ["export", "PYTHONPATH="+pythonpath ]

  return environment


def __inheritors(klass):
    subclasses = set()
    work = [klass]
    while work:
        parent = work.pop()
        for child in parent.__subclasses__():
            if child not in subclasses:
                subclasses.add(child)
                work.append(child)
    return subclasses

def guessSubmissionController(args=None):
  requestedName = args.submit if args else "auto"
  ctrlToUse = None
  ctrlPrio = None
  #first try to find a matching one by name (exception: 'auto' mode selected)
  if not (requestedName == "auto"):
    for ctrl in __inheritors(submissionControllerBase):
      if ( (ctrlPrio is None) or (ctrl.controllerPriority > ctrlPrio) ) and requestedName.lower() in ctrl.__name__.lower():
        ctrlToUse = ctrl
        ctrlPrio = ctrl.controllerPriority
  #return controller if we found one
  if ctrlToUse: return ctrlToUse()
  #then fall back to automatic detection
  ctrlToUse = None
  ctrlPrio = None
  for ctrl in __inheritors(submissionControllerBase):
    if ( (ctrlPrio is None) or (ctrl.controllerPriority > ctrlPrio) ) and ctrl._is_available() :
      #if this controller matches and has a higher priority than any previous match, set it as the current best match
      ctrlToUse = ctrl
      ctrlPrio = ctrl.controllerPriority

  if ctrlToUse is None:
    print("No submissionController seems to be available for the current system.")
    sys.exit(1)
  return ctrlToUse() #return an actual instance of the controller

def getSetupCommand(args):
  env = scanEnvironment(args)

  cmd = [
    [env['shebang']],
    ["cd", "/tmp"]
  ]
  if env.get("RootCore",False):
    cmd.append(env['unsetupROOTCore'])
    cmd.append(env['unsetupROOT'])

  if env.get("ATLAS",False):
    cmd.append(env['setATLAS'])
    cmd.append(env['setupATLAS'])

  if env.get("RootCore",False):
    cmd.append(env['setupROOTCore'])
    cmd.append(env['setPYTHON'])
  elif env.get("rel21",False):
    asetupdir = env["BUILDDIR"]
    if not os.path.isfile(os.path.join(asetupdir,".asetup.save")):
      basedir = os.path.dirname(asetupdir.rstrip("/"))
      find = ["find",basedir,'-name',".asetup.save"]
      output = common.shellexec(find,[],False).split()
      if len(output) < 1:
        raise RuntimeError("unable to locate .asetup.save!")
      asetupdir = os.path.dirname(output[0].rstrip("/"))
    cmd.append(["cd",asetupdir])
    cmd.append(["asetup","--restore"])
  else:
    if "setupG++" in env.keys():
      cmd.append(env["setupG++"])
    if "setupROOT" in env.keys():
      cmd.append(env["setupROOT"])
    if "setPYTHON" in env.keys():
      cmd.append(env['setPYTHON'])

  if not env.get("RootCore",False):
    for package in env["setup_scripts"]:
      cmd.append(["source",package])

  cmd.append(["cd", env['cwd']])

  if args.voms:
    cmd += [ env['rucio'], env['voms'] ]

  if args.defaultLibs:
    for script in env["library_setup_scripts"]:
      cmd.append([ "source" , script ])

  #TODO: clarify which format to use. For now, use a temporary hack to 'flatten' the setup commands
  cmd = [ " ".join(x) for x in cmd]
  return cmd

def makeSmartTaskList(args_, jobFile,config ,templateCommand, maxSampleCount=-1, maxSampleSize=-1, setup=[], idOffset=0, inputs=[], outputs=[]):
  """
  Creates a list of tasks based on a SampleFolder (determined from config argument, expected format is a TQFolder as obtained from common.getConfigOptions) taking boundary conditions from a job definition file (joblist) into account

  The payload of the created tasks is created from the templateCommand replacing
  occurances of the substrings '{restrict}' and '{identifier}' with the SampleFolder
  path(s) the job should be restricted to and a corresponding identifier which can
  be used in file names. Example:
  templateCommand = "analyze.py flatNtuple/config/master/analyze.cfg --retrict {restrict} --options outputFile:analyzed/toMerge/samples_unmerged_{identifier}.root"

  Additional replacements that can be used:
  -'{uniqueID}': a number which is unique within the list of tasks generated during a single call (starting at idOffset (=0 unless specified as argument to this function) and incremented for each subsequent task created)
  -'{options}' : if the args namespace has an attribute 'options' this placeholder is replaced by the concatenation of the options attribute of the args namespace. The individual elements are concatenated with single spaces ' '.

  The optional arguments 'inputs' and 'outputs' are expected to be lists of input (config) files each individual job reads and output files it is expected to produce. The same replacements as in the templateCommand case are applied (to each element of the lists).

  Optionally a list of setup instructions can be provided which will be set on each individual task. Also for the elements of the setup list above mentioned replacements are applied.

  The joblist file is expected to contain one SampleFolder path per line, tasks will be created within
  the domain of that path, possibly splitting into multiple tasks (TODO: use taskArrays instead) to
  respect the maxSampleCount and maxSampleSize arguments if they are > 0. maxSampleCount refers to a maximum number of
  input files, maxSampleSize to a maximum cumulated size of input files for a single task.

  The file size of a sample is read from tags '.init.filestamp.size' and (if the first is not found) '.xsp.fileSize', defaulting to 0.

  ATTENTION: this function assumes that whereever a wildcard is used in the job definition file, the corresponding parts of the SampleFolder are also marked as wildcarded with the tag 'wildcarded' set to true!

  """
  import QFramework
  import CommonAnalysisHelpers as CAH
  sf = CAH.common.loadSampleFolder(config)
  if not sf:
    QFramework.BREAK("unable to load sample folder '{:s}' to create smart job list - please check input path".format(inputFileName))
  # patch sample folder so that the same sample folder in the submitted job has the same format as the sample folder while creating the task list
  CAH.common.patchSampleFolder(config.getTagVStandardString("patches"), sf)
  #make a deep-ish copy of args_ (as it might be modified further down the line)
  args = argparse.Namespace(**vars(args_))

  retList = []
  allPathsWildcarded = [] #list to keep track of what contribution is already used somewhere to prevent double counting/running
  joblist = []
  with open(QFramework.TQPathManager.findFileFromEnvVar(jobFile, "CAFANALYSISSHARE")) as f:
    joblist_ = f.readlines()
    joblist = [x.strip() for x in joblist_]
  groupFriendsWildcarded = True # do we want to group samples using wildcards?
  for jobElement in joblist:
    #remove/skip comment lines:
    if jobElement == "" or jobElement[0]=="#": continue
    modline = jobElement.lstrip('!') #remove leading '!' character
    if len(modline) < len(jobElement): #we actually removed at least one leading '!'
      #hence, we adjust the current (local copy of the) argparse namespace
      if not args.allowArgChanges: continue
      (key,val) = modline.split("=",1)
      key = key.strip()
      val = val.strip()
      #we treat the following two cases differently as they are not part of the minimal argparser
      if key == "maxSampleCount":
        maxSampleCount = int(val)
      elif key == "maxSampleSize":
        maxSampleSize = float(val)
      elif key == "groupFriendsWildcarded":
        groupFriendsWildcarded = bool(int(val))
      else:
        adjustArgs(args,modline)
    else: #not a modline, so consider it as a job definition
      #TODO: remove
      #(label,restrict,downmerge) = jobElement
      localPaths = [[]] #sublists will contain all paths for one subjob

      sampleFolders = sf.getListOfSampleFolders(jobElement)
      if not sampleFolders:
        QFramework.BREAK("No matching sample folders found for expression '{:s}', exiting".format(jobElement))

      allSamplesForSubjob = []
      for thisSF in sampleFolders:
        allSamplesForSubjob = allSamplesForSubjob + [x for x in thisSF.getListOfSamples() if not x.hasSubSamples()] #this could, in theory, contain duplicates, we'll resovle this later by converting the list to a set
      #allSamplesForSubjob = list(set(allSamplesForSubjob)) #remove duplicates. Note: we still need another check later when dealing with wildcarded paths
      #the following lines efficiently remove duplicates whilst preserving the order. This is important since otherwise the creation of tasklists is not fully deterministic!!!
      alreadyAddedTemp = set()
      alreadyAddedTemp_add = alreadyAddedTemp.add #saves time w.r.t. resolution for each element
      allSamplesForSubjob = [x for x in allSamplesForSubjob if not (x in alreadyAddedTemp or alreadyAddedTemp_add(x))]
      localCount = 0
      localSize = 0.
      for sample in allSamplesForSubjob:
        if not sample: continue
        pathWildcarded = sample.getPathFriendsWildcarded(not groupFriendsWildcarded).Data().strip()
        if pathWildcarded in allPathsWildcarded: continue #check if the path is already used somewhere and skip it if needed to prevent double counting
        sampleSize = sample.getTagDoubleDefault(".init.filestamp.size",sample.getTagDoubleDefault(".xsp.fileSize",0.))
        if ( (maxSampleSize>0 and localSize+sampleSize>maxSampleSize) or (maxSampleCount>0 and localCount+1>maxSampleCount)  ): #check if we need to create a new batch of samples or we can add it to the current one
          if (len(localPaths[-1]) >0): #don't do anything if this is simply a single sample already exceeding the limit
            localPaths.append([])
            localSize = 0.
            localCount = 0

        allPathsWildcarded.append(pathWildcarded) #we are using this path now so we add it to the used paths
        localPaths[-1].append(pathWildcarded)
        localSize += sampleSize
        localCount += 1

      label=args.identifier+"_"+ ( jobElement.strip("/").replace("/","_").replace("?","X").replace(",","_") )
      #combine the sample paths into --restrict arguments
      nPart = 0

      try:
        options = args.options
      except:
        options = []
      options.append("prettyPrint=false")
      options.append("lineUpdates=false")

      optionstring = " ".join(options)

      for pList in localPaths:
        if len(pList)==0: continue
        restrict = ",".join(pList)

        if not isinstance(templateCommand,list):#turn into a list if this is not the case yet
          templateCommand = [templateCommand]
        partLabel = label+(".part"+str(nPart) if len(localPaths)>1 else "")
        payload = [x.format( identifier = partLabel,
                                          uniqueID = idOffset,
                                          restrict = restrict,
                                          options = optionstring )
                   for x in templateCommand] #note: this also ensures we have a copy of the payload list and don't run into troubles with the mutable nature of lists!
        theseOutputs = [x.format( identifier = partLabel,
                                          uniqueID = idOffset,
                                          restrict = restrict,
                                          options = optionstring )
                   for x in outputs] #note: this also ensures we have a copy of the payload list and don't run into troubles with the mutable nature of lists!
        theseInputs = [x.format( identifier = partLabel,
                                          uniqueID = idOffset,
                                          restrict = restrict,
                                          options = optionstring )
                   for x in inputs] #note: this also ensures we have a copy of the payload list and don't run into troubles with the mutable nature of lists!
        thisSetup = [x.format( identifier = partLabel,
                                          uniqueID = idOffset,
                                          restrict = restrict,
                                          options = optionstring )
                   for x in setup] #note: this also ensures we have a copy of the payload list and don't run into troubles with the mutable nature of lists!
        logFile = os.path.join(args.logpath,partLabel+".log")
        thisTask = task(partLabel,payload, setup=thisSetup, args=args, inputs=theseInputs, outputs=theseOutputs, logFile=logFile, errFile=logFile)
        retList.append(thisTask)
        #perform some increments
        nPart += 1 #id within this (sub)set of tasks
        idOffset += 1 #global id for full list of tasks
  return retList


def makeTaskList(args_, jobFile,config ,templateCommand, setup=[], idOffset=0, inputs=[], outputs=[]):
  """
  Creates a list of tasks based on a job definition file (joblist).
  One job will be scheduled for each line of the joblist file.
  Please refer to makeSmartTaskList for further documentation.
  """
  import QFramework
  import CommonAnalysisHelpers as CAH
  sf = CAH.common.loadSampleFolder(config)
  if not sf:
    QFramework.BREAK("unable to load sample folder '{:s}' to create smart job list - please check input path".format(inputFileName))
  #make a deep-ish copy of args_ (as it might be modified further down the line)
  args = argparse.Namespace(**vars(args_))

  retList = []
  joblist = []
  with open(QFramework.TQPathManager.findFileFromEnvVar(jobFile, "CAFANALYSISSHARE")) as f:
    joblist_ = f.readlines()
    joblist = [x.strip() for x in joblist_]
  for jobElement in joblist:
    #remove/skip comment lines:
    if jobElement == "" or jobElement[0]=="#": continue

    modline = jobElement.lstrip('!') #remove leading '!' character
    if len(modline) < len(jobElement): #we actually removed at least one leading '!'
      #hence, we adjust the current (local copy of the) argparse namespace
      if not args.allowArgChanges: continue
      (key,val) = modline.split("=",1)
      key = key.strip()
      val = val.strip()
      #we treat the following two cases differently as they are not part of the minimal argparser
      adjustArgs(args,modline)
    else: #not a modline, so consider it as a job definition
      localPaths = [[]] #sublists will contain all paths for one subjob

      label=args.identifier+"_"+ ( jobElement.strip("/").replace("/","_").replace("?","X").replace(",","_") )
      #combine the sample paths into --restrict arguments

      try:
        options = args.options
      except:
        options = []
      options.append("prettyPrint=false")
      options.append("lineUpdates=false")

      optionstring = " ".join(options)

      # Simply add the job
      pList = [jobElement]
      restrict = ",".join(pList)

      if not isinstance(templateCommand,list):#turn into a list if this is not the case yet
        templateCommand = [templateCommand]
      partLabel = label
      payload = [x.format( identifier = partLabel,
                                        uniqueID = idOffset,
                                        restrict = restrict,
                                        options = optionstring )
                 for x in templateCommand] #note: this also ensures we have a copy of the payload list and don't run into troubles with the mutable nature of lists!
      theseOutputs = [x.format( identifier = partLabel,
                                        uniqueID = idOffset,
                                        restrict = restrict,
                                        options = optionstring )
                 for x in outputs] #note: this also ensures we have a copy of the payload list and don't run into troubles with the mutable nature of lists!
      theseInputs = [x.format( identifier = partLabel,
                                        uniqueID = idOffset,
                                        restrict = restrict,
                                        options = optionstring )
                 for x in inputs] #note: this also ensures we have a copy of the payload list and don't run into troubles with the mutable nature of lists!
      thisSetup = [x.format( identifier = partLabel,
                                        uniqueID = idOffset,
                                        restrict = restrict,
                                        options = optionstring )
                 for x in setup] #note: this also ensures we have a copy of the payload list and don't run into troubles with the mutable nature of lists!
      logFile = os.path.join(args.logpath,partLabel+".log")
      thisTask = task(partLabel,payload, setup=thisSetup, args=args, inputs=theseInputs, outputs=theseOutputs, logFile=logFile, errFile=logFile)
      retList.append(thisTask)
      #perform some increments
      idOffset += 1 #global id for full list of tasks
  return retList
