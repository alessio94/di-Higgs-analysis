#import all classes implemented in the files in this module into the modules scope.
#This is needed such that we can automatically guess the right controller by iterating over 
#all subclasses of submissionControllerBase. For this we need to import the individual classes,
#not just their "files", i.e., we need 'SlurmController' and not just 'slurm.SlurmController'
#to be available.
from .slurm import *
from .slurmOnCedar import *
from .condor import *
from .torque import *
from .moab import *
from .sge import *
from .local import *
from .kubernetes import *
