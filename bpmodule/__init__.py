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

# Standalone (don't depend on other core modules)
from . import exception


# TODO - can we get some order that will work with RTLD_NOW?
# Probably not - there are circular dependencies
# (modulelocator and modulebase, for example)
from . import python_helper
from . import mangle
from . import util
from . import output
from . import parallel
from . import datastore
from . import math
from . import tensor
from . import modulebase
from . import modulelocator
from . import molecule
from . import basisset

from . import testing

# restore old DL flags
sys.setdlopenflags(olddl)



##########################################
# Load other python stuff from this directory
##########################################
from .modulecheck import *
from .ModuleManager import *



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
  parallel.InitParallel()

  # Initialize tensor
  tensor.Init()


  # Set some info about the base module and
  # print it
  # Also, set some options (currently unused)
  for name,minfo in modulelocator.minfo.items():
      minfo["name"] = name
      minfo["key"] = "CORE"
      minfo["path"] = os.path.dirname(modulelocator.__file__) + "/"



def Finalize():
  output.Output("Finalizing tensors\n")
  tensor.Finalize()

  output.Output("Finalizing parallelization\n")
  parallel.FinalizeParallel()

  util.ClearCmdline()

