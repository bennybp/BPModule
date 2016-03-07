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
from . import basisset
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



def Init(argv, out = "stdout", color = True, debug = False):
  """Initializes the BPModule core

  Initializes MPI, tensor libraries, etc, as well as the
  output streams.
  """

  if out == "stdout":
    output.SetOut_Stdout()
  else:
    output.SetOut_File(out)

  output.SetColor(color)
  output.SetDebug(debug) 

  # Set the command line
  util.SetCmdline(argv)

  # Initialize Parallel
  parallel.Init()

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
  output.Output("Finalizing tensors\n")
  tensor.Finalize()

  output.Output("Finalizing parallelization\n")
  parallel.Finalize()

  util.ClearCmdline()

