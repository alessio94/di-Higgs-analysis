
import os.path
import time
import subprocess
from SubmissionHelpers.submissionControllerBase import submissionControllerBase,getCmdAdditions
from SubmissionHelpers.common import mkdir_p
from SubmissionHelpers.task import task,taskStatus
from SubmissionHelpers.submissionControllers.slurm import SlurmController


class SlurmControllerOnCedar(SlurmController):
  controllerPriority = 2 # choose 2 to prioritize over slurmController
  """
  Implementation of a submission controller using the slurm scheduler.
  """

  def __init__(self):
    # Slurm doesn't allow to execute submission scripts from home.
    # Try to put the files on scratch.
    import os
    if not hasattr(self, 'workloadDir') or self.workloadDir == "":
      
      username = ""
      buildDir = ""
      if "USER" in os.environ:
        username = os.environ["USER"]
      if "ANALYSIS_BUILD_DIR" in os.environ:
        buildDir = os.environ["ANALYSIS_BUILD_DIR"]
      elif "CAF_BUILD_DIR" in os.onviron:
        buildDir = os.environ["CAF_BUILD_DIR"]
      if not username == "":
        self.workloadDir = "/scratch/" + username
        if not os.path.isdir(self.workloadDir):
          if not buildDir == "":
            self.workloadDir = buildDir
          else:
            self.workloadDir = "."
      self.workloadDir = self.workloadDir + "/submissionWorkloadTemp"

    SlurmController.__init__(self)
    return

  def _apply_base_command(self, args, thisTask, cmd_args, payload, setup):
    """
    Appends 'sbatch' to the command line arguments list in-place and returns
    the original payload string. The method is called by __submitSingleTask()
    to build command line arguments and the payload.
    """

    #not human readable but at least a safe file name which is unique as long as the tasks identifier is unique:
    import hashlib
    hash_object = hashlib.md5(thisTask.identifier.encode("ascii"))
    identifier = hash_object.hexdigest()
    self.payloadScriptName = os.path.join( self.workloadDir, identifier )
    self.submitScriptName = self.payloadScriptName + "_submitScript.sh"
    self.wrapperScriptName = self.payloadScriptName + "_wrapper.sh"
    self.payloadScriptName += ".sh"
    filesToDelete = [self.submitScriptName, self.wrapperScriptName, self.payloadScriptName]

    self.wrapperScript = self._getWrapperScript(self.submitScriptName, filesToDelete)

    # automatically write the submit script for container submission
    writeSubmissionScript = ["batchScript", self.payloadScriptName, "-O", self.submitScriptName, ";"]
    cmd_args += writeSubmissionScript

    cmd_args.append("sbatch")
    cmd_args.append("--parsable") #machine readable stdout
    cmd_args.append("--export=NONE")
    cmd_args.extend(getCmdAdditions()) #add optionally some more arguments
    return True

  def _apply_end_of_command(self, args, task, cmd_args, payload, setup):
    """
    The submission command sbatch needs the submit file as last argument.
    It is appended in this method.
    """
    cmd_args.append(self.wrapperScriptName)
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

  def _execute_submission_command(self, thisTask, cmd_args, payload, setup):
    """
    Communicate the task to the batch system.
    """
    setupAndPayload = "\n".join(setup+payload)

    mkdir_p(self.workloadDir)
    with open(self.payloadScriptName, "w") as f:
      f.write(setupAndPayload)
    with open(self.wrapperScriptName, "w") as f:
      f.write(self.wrapperScript)
    os.chmod(self.payloadScriptName, 0o0744)
    os.chmod(self.wrapperScriptName, 0o0744)

    # The following line converts commands separated by ";" into a list of commands
    commandList = [l.split("<tmp>") for l in "<tmp>".join(cmd_args).split("<tmp>;<tmp>")]
    for command in commandList:
      process = subprocess.Popen(command, stdin=subprocess.PIPE,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)

      (stdout, stderr) = process.communicate("")
      status_code = process.returncode

    # only return the stdout and stderr of the last command (i.e. the submission command)
    return status_code, stdout, stderr

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
    (stdout, stderr) = p.communicate("")

    success = not p.returncode #returncode is 0 if everything went fine!
    return success and stdout.strip("\n") == "/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/wrappers/containers/slurm/sbatch"
