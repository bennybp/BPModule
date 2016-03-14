import sys
import os
import importlib


##########################################
# Load all the core modules
##########################################

# Important! Symbols must be available
# globally! WILL NOT WORK WITHOUT
olddl = sys.getdlopenflags()
sys.setdlopenflags(os.RTLD_LAZY | os.RTLD_GLOBAL)


###############################################
# Some slight ordering of importing
# Despite using RTLD_LAZY, the
# PyInit_** functions may require some
# symbols to be loaded when they are called
###############################################

from . import exception
from . import util
from . import output
from . import parallel
from . import python
from . import math

from . import tensor
from . import datastore
from . import system
from . import modulebase
from . import modulemanager

from . import testing

# restore old DL flags
sys.setdlopenflags(olddl)



##########################################
# Load other python stuff from this directory
##########################################
from .ModuleCheck import *
from .ModuleAdministrator import *



##########################################
# Set some paths
##########################################
thispath = os.path.dirname(os.path.realpath(__file__))
toppath = os.path.realpath(os.path.join(thispath, "../../"))

bpmodule_paths = { "base": thispath,
                   "basis": [ os.path.join(toppath, "basis") ]
                 }


def Init(argv, out = "stdout", color = True, debug = False,nthreads=1):
  """Initializes the BPModule core

  Initializes MPI, tensor libraries, etc, as well as the
  output streams.
  """

  output.SetGlobalOut_Stdout()
  output.EnableColor(color)

  gout = output.GetGlobalOut()
  gout.EnableDebug(debug)

  # Set the command line
  util.SetCmdline(argv)

  # Initialize Parallel
  parallel.Init(nthreads)

  # Initialize tensor
  tensor.Init()


  # Set some info about the base module and
  # print it
  # Also, set some options (currently unused)
  for name,minfo in modulemanager.minfo.items():
      minfo["name"] = name
      minfo["key"] = "CORE"
      minfo["path"] = os.path.dirname(modulemanager.__file__) + "/"



def Finalize():
  output.GlobalOutput("Finalizing tensors\n")
  tensor.Finalize()

  output.GlobalOutput("Finalizing parallelization\n")
  parallel.Finalize()

  util.ClearCmdline()

