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

from . import mangle
from . import exception
from . import python_helper
from . import output
from . import parallel
from . import math
from . import tensor
from . import datastore
from . import options
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
from .utils import *
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

  # Initialize Parallel
  parallel.InitParallel(argv)

  # Initialize tensor
  tensor.InitTensor(argv)


  # Set some info about the base module and
  # print it
  # Also, set some options (currently unused)
  for name,minfo in modulelocator.minfo.items():
      minfo["name"] = name
      minfo["key"] = "CORE"
      minfo["path"] = os.path.dirname(modulelocator.__file__) + "/"



def Finalize():
  output.Output("Finalizing tensors\n")
  tensor.FinalizeTensor()

  output.Output("Finalizing parallelization\n")
  parallel.FinalizeParallel()

