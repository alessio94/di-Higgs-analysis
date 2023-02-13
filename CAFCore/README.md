[![pipeline status](https://gitlab.cern.ch/atlas-caf/CAFCore/badges/master/pipeline.svg)](https://gitlab.cern.ch/atlas-caf/CAFCore/commits/master)

Core code for the Common Analysis Framework (CAF)
==================================================

This repository contains all of the code at the heart of the Common Analysis Framework. In order to conduct an analysis using CAF, please see the [CAFExample](https://gitlab.cern.ch/atlas-caf/CAFExample) repository to get started. The CAFCore code can also be cloned and compiled outside the context of a dependent project (see below).

Cloning the repository
----------------------

Starting with a fresh environment, navigate to a new working directory and run

```bash
git clone https://gitlab.cern.ch/atlas-caf/CAFCore.git
```

Building the code standalone using CMake
----------------------------------------

The most common reason for building CAFCore on it's own is the case in which the machine in question does not have access to cvmfs (e.g. user's laptop). Assuming CMake and ROOT are installed on the device, CAFCore has the capability of compiling in so-called "standalone" mode:

```bash
mkdir build
cd build
cmake ../CAFCore
make -j4
source ../CAFCore/cafsetup.sh
```


If cmake complains about non-existing BOOST header files, [download](https://www.boost.org/users/download/) and install boost. You may need to set BOOST_INDLUDE_DIR to the directory containing your boost headers, for example
```bash
export BOOST_INCLUDE_DIR="/usr/local/include/"
```
if your headers are in `/usr/local/include/boost/`.

On each subsequent login, simply navigate to the working directory and

```bash
source CAFCore/cafsetup.sh
```

Building the code in a CMake ASG environment
--------------------------------------------

```bash
mkdir build
cd build
setupATLAS
asetup AnalysisBase,21.2.34
cmake ../CAFCore
make -j4
source ../CAFCore/cafsetup.sh
```

On each subsequent login, navigate to the build directory and

```bash
setupATLAS
asetup --restore
source ../CAFCore/cafsetup.sh
```

Docker Images
-----------------------------------------------

Docker images can easily provide you the environment to run CAFCore even if you don't have ROOT or an ATLAS-ready system available. We provide images with compiled code for various usecases. If you have docker available, you can pull images with the command
```bash
docker pull gitlab-registry.cern.ch/atlas-caf/<IMAGE_NAME>:<IMAGE_TAG>
```
See below for an explanation of the available images. To run a container of the image, use
```bash
docker run --rm -it gitlab-registry.cern.ch/atlas-caf/<IMAGE_NAME>:<IMAGE_TAG>
```

The CAF environment will be set up automatically. If you want to disable this behaviour or see other options, execute the image with the `--help` option. For more information on docker, check out online tutorials, for example [this one](https://awesome-workshop.github.io/intro-to-docker/).

| Image name | Image tag | Content |
|---|---|---|
| cafcore|master | Latest master commit built on a specific version of AnalysisBase (check .gitlab-ci.yml to see the version). If you are new, you probably want to start with this. |
| cafcore|latest | Latest master commit built on the latest available AnalysisBase version |
| cafcore/standalone|master | Latest master commit built on an python3 image with ROOT installed. |
| cafcore/branch|branchName | Latest image of a feature branch (intended for debugging) |
| cafcore/branch-standalone|branchName | Latest image of a feature branch based on standalone (intended for debugging) |
| cafcore/debug| | Temporary storage for build images before they get a nice tag |
| cafcore/debug-standalone| | Temporary storage for standalone build images before they get a nice tag |
| cafcore/skopeo-helper| | Helper image to move and untag images in the CI |
| cafcore/standalone-base | | Base image for standalone compilation |
