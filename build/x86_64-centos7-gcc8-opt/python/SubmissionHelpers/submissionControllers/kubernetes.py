import time
import os
import tempfile
import subprocess
import re

try:
    import kubernetes
    if not hasattr(kubernetes, "config"):
        # This is a weird import
        kubernetes = None
except ImportError:
    kubernetes = None

from SubmissionHelpers.task import task, taskStatus
from SubmissionHelpers.submissionControllerBase import submissionControllerBase



class KubernetesController(submissionControllerBase):
  controllerPriority = 1 #can be used to prioritize one controller over another if multiple ones seem to be available
  poll_delay=10
  
  def __init__(self):
    """
    Initialize a local variables which store and count all threads.
    """
    self.bclient = kubernetes.client.BatchV1Api()
    self.namespace = "default"
    self.next_job_id = 0
    
  def _apply_base_command(self, args, thisTask, cmd_args, payload, setup):
    # not applicable since _execute_submission_command is overridden
    self.docker_image = args.k8s_image
    self.registry_secret = args.k8s_registry_secret
    self.volume_claim = args.k8s_volume_claim
    self.mount_path = args.k8s_mount_path
    return True
  
  def _apply_identifier(self, args, thisTask, cmd_args, payload, setup):
    # not applicable since _execute_submission_command is overridden
    return True
  
  def _apply_queue(self, args, thisTask, cmd_args, payload, setup):
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
    job_id = self.next_job_id
    self.next_job_id += 1

    # command = "\n".join(setup + payload)
    command = "\n".join(payload)

    resources = {}
    if thisTask.memory:
        resources["memory"] = "%dM" % thisTask.memory
    if thisTask.ncores:
        resources["cpu"] = str(thisTask.ncores)
    resources = kubernetes.client.V1ResourceRequirements(requests=resources)

    volume_mounts=[kubernetes.client.V1VolumeMount(
        mount_path=self.mount_path,
        name="pvc-sh-vol",
    )]
    container = kubernetes.client.V1Container(
        name="submission-helper-payload",
        image=self.docker_image,
        image_pull_policy="Always",
        command=["bash", "-cl", command],
        resources=resources,
        volume_mounts=volume_mounts,
    )

    image_pull_secrets = [
        kubernetes.client.V1LocalObjectReference(name=self.registry_secret)
    ]

    volumes = [kubernetes.client.V1Volume(
        name="pvc-sh-vol",
        persistent_volume_claim=
            kubernetes.client.V1PersistentVolumeClaimVolumeSource(claim_name=self.volume_claim)
    )]

    pod_spec = kubernetes.client.V1PodSpec(
        restart_policy="OnFailure",
        containers=[container],
        volumes=volumes,
        image_pull_secrets=image_pull_secrets,
    )

    labels = {
        "app": "submission-helper",
        "identifier": thisTask.identifier,
    }
    template = kubernetes.client.V1PodTemplateSpec(
        metadata=kubernetes.client.V1ObjectMeta(labels=labels),
        spec=pod_spec,
    )

    # Fallback to 24h if time limit is negative
    timeout = thisTask.time * 60 if thisTask.time > 0 else 60 * 60 * 24
    spec = kubernetes.client.V1JobSpec(
        template=template,
        active_deadline_seconds=timeout,
        backoff_limit=3
    )

    job_name = "shj-%s-%05d" % (thisTask.identifier, job_id)
    job_name = re.sub("[^a-z0-9.-]", "-", job_name.lower())
    job = kubernetes.client.V1Job(
        api_version="batch/v1",
        kind="Job",
        metadata=kubernetes.client.V1ObjectMeta(name=job_name),
        spec=spec
    )
    
    api_response = self.bclient.create_namespaced_job(
        body=job,
        namespace=self.namespace,
    )

    return 0, job_name, ""
    
  
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

    thisTask.jobid = stdout
    thisTask.setStatusCode(taskStatus.submitted)
    return True
  
  @staticmethod
  def _is_available():
    """
    Check if the kubernetes controller is available
    """
    if not kubernetes:
        return False

    try:
        kubernetes.config.load_kube_config()
    except kubernetes.config.config_exception.ConfigException:
        return False
    except FileNotFoundError:
        return False    

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
    return_value = True

    try:
        api_jobs = self.bclient.list_namespaced_job(self.namespace)
    except kubernetes.client.exceptions.ApiException:
        raise RuntimeError("[submissionControllerKubernetes] Failed to get list of jobs via API")
        return False

    self.queueStatusCache = {} #clear previous cache
    for api_job in api_jobs.items:
        if "submission-helper" != api_job.metadata.labels.get("app"):
            # not our job
            continue
                
        identifier = api_job.metadata.labels.get("identifier")
        job = {
            'jobid': api_job.metadata.name,
            'status': "",
        }

        if api_job.status.failed:
            job["statusCode"] = taskStatus.failed
        elif api_job.status.active:
            job["statusCode"] = taskStatus.running
        elif api_job.status.succeeded:
            job["statusCode"] = taskStatus.done
        elif any(c.type == "Failed" for c in api_job.status.conditions):
            job["statusCode"] = taskStatus.failed
        else:
            #unknown status, warn the user an set status as undefined:
            print( "[WARNING] Encountered unknown status in squeue output for task with identifier '{:s}'/ jobid '{:s}'! Will consider its queue status as undefined!".format(identifier,job['jobid']) )
            job['statusCode'] = taskStatus.undefined
            return_value = False

        self.queueStatusCache[identifier] = job

    self.queueStatusTimestamp = time.time()
    return return_value
