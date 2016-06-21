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
# Components remain part of pulsar_core namespace until
# imported by the individual sub-packages below
from . import pulsar_core

# Each of these imports its namespace from pulsar_core
# into the main pulsar namespace
from . import exception
from . import util
from . import output
from . import parallel
from . import python
from . import math

from . import system
from . import datastore
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

pulsar_paths = { "base": thispath,
                 "basis": [ os.path.join(toppath, "basis") ]
               }


def initialize(argv, out = "stdout", color = True, debug = False,nthreads=1):
  """Initializes the Pulsar core

  Initializes MPI, libraries, etc, as well as the
  output streams.
  """

  output.set_global_out_to_stdout()
  output.enable_color(color)

  gout = output.get_global_output()
  gout.enable_debug(debug)

  # Set the command line
  util.set_cmdline(argv)

  # Initialize Parallel
  parallel.initialize(nthreads)


def finalize():
  output.print_global_output("Finalizing parallelization\n")
  parallel.finalize()

  util.clear_cmdline()

