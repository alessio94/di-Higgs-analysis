# Submission Helpers

Analysis frameworks within ATLAS are often used by team members who
are scattered around the globe with local computing resources.
To ease the development of software that is able to submit analysis jobs to a variety of different
clusters, the repository SubmissionHelpers provides a uniform abstraction layer for
jobs and job schedulers of a cluster.

The package enables the user to define a
directed, acyclic graph (DAG) of jobs that is submitted to the cluster. The
graph specifies dependencies between the jobs such that a dependent job can only
start once its dependency has completed successfully.
The submission code emulates this behavior if a job scheduler of a
local cluster does not have native support for inter-job dependencies.

Furthermore, inspired by `make`, the DAG can be annotated with input and output
file information. If a job's output files
are newer than an input file, the job's artifacts are considered to be
up-to-date, and the submission of the job can is skipped. This means repeated
invocations will only submit missing pieces of the dependency graph, which saves
resources and makes validating the output much simpler.

The SubmissionHelper package currently supports the following job schedulers:
 - HTCondor
 - MOAB
 - SGE
 - SlurmOnCedar
 - Slurm
 - Torque
 - [Kubernetes](Kubernetes.md)

## Installation

### CMake standalone installation 
You can build the library in a standalone `build` directory using CMake

```console
mkdir build
cd build
cmake ..
make
```

To make the library available in your environment, run
```console
source setup.sh  # in the build directory
```

### CAF Integration

To integrate the SubmissionHelpers into a CAF project, add this repository as a
submodule of your analysis repository. Additionally, you need to add it to your
main `CMakeLists.txt` as shown in
[CAFExample](https://gitlab.cern.ch/atlas-caf/CAFExample/-/blob/master/CMakeLists.txt#L183).

## Quickstart

```python
#!/usr/bin/env python

from SubmissionHelpers import submit
import argparse

def submit_hello_world_job(args):
    """
    Creates a 'Hello World' job and sends it to the available submission
    controller.
    """
    task = submit.task("hello_world", "sleep 60; touch hello_world.txt",
                       memory=args.memory,
                       ncores=args.ncores,
                       time=args.time,
                       setup=submit.getSetupCommand(args),
                       outputs=["hello_world.txt"])

    controller = submit.guessSubmissionController(args)
    controller.submitTasks(args, [task])

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Script to submit a 'Hello World' job")
    submit.configureDefaultArgumentParser(parser)
    args = parser.parse_args()

    submit_hello_world_job(args)

```

## Tutorial

A more comples example of showing the use of job dependencies is located in the
[CAFExample/Tutorial](https://gitlab.cern.ch/atlas-caf/CAFExample/-/tree/master/Tutorial/submission-paper)
directory.
