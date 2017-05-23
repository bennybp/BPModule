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

# Import the main so file
from .pulsar_core import *

# Import other python files
from .datastore.OptionValidators import *
from .exception.PulsarException import *
from .modulebase.base_options import *
from .modulemanager.ModuleTreePrinters import *
from .modulemanager.ModuleCheck import *
from .modulemanager.ModuleAdministrator import *
from .system.BasisSetParsers import *
from .system.ApplyBasisSet import *
from .system.MakeSystem import *
from .testing.PyTester import *

#Used to be in utl/__init__.py
def line(char, n = 80):
    return char * n + "\n" 

# restore old DL flags
sys.setdlopenflags(olddl)

##########################################
# Load other python stuff from this directory
##########################################



##########################################
# Set some paths
##########################################
thispath = os.path.dirname(os.path.realpath(__file__))
toppath = os.path.realpath(os.path.join(thispath, "../../"))

pulsar_paths = { "base": thispath,
                 "basis": [ os.path.join(toppath, "basis") ],
                 "shared_scratch": [ "/tmp" ],
                 "local_scratch": [ "/tmp" ],
               }


def initialize(argv,
               outpath = os.getcwd(),
               outbase = "psr_output",
               color = True,
               debug = False,
               use_stdout = True,
               nthreads = 1):

  """Initializes the Pulsar core

  Initializes MPI, libraries, etc, as well as the
  output streams.
  """

  # Output depends on MPI being initialized, so we do that first
  pulsar_core.parallel_initialize(nthreads)

  create_global_output(outpath, outbase, use_stdout)
  enable_color(color)

  gout = get_global_output()
  gout.enable_debug(debug)

  # Print startup info
  memwatch_running = pulsar_core.memwatch_running()
  if memwatch_running:
      s = "Running"
  else:
      s = "NOT Running"

  pulsar_core.print_global_output("Memwatch status: {}\n".format(s))


def finalize():
  pulsar_core.print_global_output("Finalizing pulsar core\n")
  # nothing else to do. All the rest handled in the SO file constructor

