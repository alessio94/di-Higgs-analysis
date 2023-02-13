"""
This script is used to install SubmissionHelpers and all its dependencies. Run

    python setup.py install
or
    pip install .

to install the package.
"""

from setuptools import setup

def load_long_description(filename):
    """
    Loads the given file and returns its content.
    """
    with open(filename) as readme_file:
        content = readme_file.read()
        return content

setup(name='SubmissionHelpers',
      version='1.0.0',  # Also change in module and CMakeLists
      packages=["SubmissionHelpers",
                "SubmissionHelpers.test",
                "SubmissionHelpers.submissionControllers"],
      package_dir={"SubmissionHelpers": "python/",
                   "SubmissionHelpers.test": "test/"},
      scripts=['share/submit'],
      description="The package enables the user to define a directed, acyclic "
				  "graph (DAG) of jobs that is submitted to a variety of cluster",
      long_description=load_long_description("README.md"),
      long_description_content_type='text/markdown',
      url="https://gitlab.cern.ch/atlas-caf/SubmissionHelpers",
      author="ATLAS - CAF Developers",
      classifiers=[
        "Intended Audience :: Science/Research",
        "Topic :: Scientific/Engineering :: Physics",
        "Topic :: System :: Distributed Computing",
      ])
