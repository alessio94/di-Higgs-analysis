# Kubernetes support

Submitting jobs to a Kubernetes cluster is still experimental. Feedback is
highly welcome.

## Prerequisites
In order to use the Kubernetes controller you need
 - A running Kubernetes cluster,
 - The [kubernetes](https://pypi.org/project/kubernetes/) Python client
   library installed,
 - Access to the cluster configured in a standard location, e.g,
   `~/.kube/config`,
 - A Docker image with your executable published in a (private) registry,
 - The input data on a PersistentVolume mountable via a PersistentVolumeClaim
   in the cluster.

## Usage
The Kubernetes controller add a couple of mandatory commandline arguments.
 - `--k8s-image`: Public identifier of the Docker image for your job. The image
   should be self-contained.  Example: `gitlab-registry.cern.ch/fsauerbu/caf-k8s/cafexample`

   When the payload is started, the usual setup
   code is *not* executed. The environment must be set up in the image itself or in
   a `.bashrc`. The payload is executed with `bash -cl` to load the
   environment from `.bashrc`. For example, to build a suitable image based on
   CAFExample, use:

   ```
   # Dockerfile
   FROM atlas/analysisbase:21.2.171
   ADD CAFExample /caf/CAFExample
   WORKDIR /caf/build
   RUN source /release_setup.sh &&  \
       sudo chown -R atlas /caf && \
       cmake ../CAFExample && \
       make -j4
   RUN echo "source /release_setup.sh && source /caf/build/setupAnalysis.sh" >> /home/atlas/.bashrc
   ```

   In the case of CAFExample, you need to make sure that the input and output
   paths in your config files match the `--k8s-mount-path` option.

 - `--k8s-registry-secret`: The name of the Kubernetes secret to access the
   Docker registry. For example, create a secret named
   `sh-k8s-registry-secret` with

   ```
   kubectl create secret docker-registry sh-k8s-registry-secret \
      --docker-server="gitlab-registry.cern.ch" \
      --docker-username="fsauerbu" \
      --docker-password="YOUR_PASSWORD"
   ```

   It is generally good practice, to generate dedicated GitLab access tokens for this
   task. (Default: `sh-k8s-registry-secret`)

 - `--k8s-volume-claim`: Name of the PersistentVolumeClaim bound to the
   PersistentVolume holding the input data (e.g. collection of root file).
   The PersistentVolumeClaim is used to mount the input data in the
   execution Pod. If you have an appropriate PersistentVolume, you can create
   the volume claim with the following Kubernetes object (`kubectl apply -f
   pvc.yaml`):

   ```
   # pvc.yaml
   apiVersion: v1
   kind: PersistentVolumeClaim
   metadata:
     name: ntuples-claim
   spec:
     accessModes:
       - ReadWriteMany
     storageClassName: ""  # For manually created PersistentVolume 
     resources:
       requests:
         # Requirement on the PersistentVolume, change if more storage is
         # required
         storage: 1Gi
   ```

   (Default: `ntuples-claim`)

 - `--k8s-mount-path`: Path inside the Docker container where the
   PersistentVolume bound via the PersistentVolumeClaim is mounted.
   (Default: `/ntuples`)


## Useful commands
 - View status of Jobs: `kubectl get jobs -l app=submission-helper`
 - View execution Pods: `kubectl get pods -l app=submission-helper`
 - View logs of Pod: `kubectl logs -f <POD_NAME>`
 - Cancel all Jobs: `kubectl delete jobs -l app=submission-helper`
