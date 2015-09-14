import sys
import os
import importlib

# Important! Symbols must be loaded now and available
# globally! WILL NOT WORK WITHOUT
olddl = sys.getdlopenflags()
sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)

##########################################
# Load all the core modules
##########################################
from . import python_helper
from . import output
from . import exception
from . import parallel
from . import tensor
from . import datastore
from . import modulebase
from . import modulestore
from . import molecule
from . import basisset

from . import testing

##########################################
# Load other python stuff from this directory
##########################################
from .utils import *
from .optioncheck import *
from .optionvalidate import *


##########################################
# Load ambit
##########################################
import ambit

sys.setdlopenflags(olddl)



# Main module store and module loaders
mst = None
cml = None
pml = None
modmap = {}



def Init(argv, out = "stdout", color = True, debug = False):
  """Initializes the BPModule core

  Initializes MPI, tensor libraries, etc, as well as the
  output streams.
  """
  global mst
  global cml
  global pml

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
  for name,minfo in modulestore.minfo.items():
      minfo["name"] = name
      minfo["key"] = "CORE"
      minfo["path"] = os.path.dirname(modulestore.__file__) + "/"

      # merge the options
      defopt = minfo["options"]
      try:
        newopt = MergeAndCheckOptions(defopt, {}) # No options
      except exception.PyGeneralException as e:
        e.Append([ ("ModuleKey", minfo["key"]),
                   ("ModuleName", minfo["name"]),
                   ("ModulePath", minfo["path"])
                ])
        raise e

      minfo["passedoptions"] = newopt
      PrintModuleInfo(minfo)


  # Create the various stores and loaders
  mst = modulestore.ModuleStore()
  cml = modulestore.CModuleLoader(mst)
  pml = modulestore.PyModuleLoader(mst)




def Finalize():
  global mst
  global cml
  global pml

  output.Output("Deleting python modules\n")
  del pml

  output.Output("Deleting C modules\n")
  output.Output("Closing C handles\n")
  del cml

  output.Output("Clearing module store\n")
  del mst

  output.Output("Finalizing tensors\n")
  tensor.FinalizeTensor()

  output.Output("Finalizing parallelization\n")
  parallel.FinalizeParallel()





def LoadModule(supermodule, name, key):
    output.Output("Importing %1% module from supermodule %2% for key %3%\n", name, supermodule, key)

    try:
        # Don't use global from now on
        olddl = sys.getdlopenflags()
        sys.setdlopenflags(os.RTLD_NOW)
        m = importlib.import_module(supermodule)
        sys.setdlopenflags(olddl)
    except Exception as e:
        raise exception.PyGeneralException(
                                "Unable to load supermodule {}".format(supermodule),
                                [
                                  ("Exception", str(e))
                                ]
                               )

    if not name in m.minfo:
        raise exception.PyGeneralException(
                                 "Supermodule doesn't have module!",
                                [
                                  ("Supermodule", supermodule),
                                  ("Name", name)
                                ]
                               )


    minfo = m.minfo[name]

    path = os.path.dirname(m.__file__) + "/"

    output.Output("\n")
    output.Output("Loading module %1% v%2%\n", name, minfo["version"])

    # Copy the key and name to the dict
    minfo["key"] = key
    minfo["name"] = name

    # set the path
    minfo["path"] = path


    if minfo["type"] == "c_module":
        cml.LoadSO(key, minfo)
    elif minfo["type"] == "python_module":
        pml.LoadPyModule(key, m.CreateModule, minfo)
    output.Debug("Done importing module %1% from %2%\n", key, supermodule)
    output.Output("\n")

    modmap[key] = minfo;



# Set the options for a module under a given key
def SetOptions(key, useropt):
    global mst

    if not key in modmap:
        raise exception.PyGeneralException(
                                 "Key not loaded, so I can't change the options!",
                                [
                                  ("Key", key)
                                ]
                               )

    minfo = modmap[key]
    defopt = minfo["options"]

    try:
      newopt = MergeAndCheckOptions(defopt, useropt)
    except exception.PyGeneralException as e:
      e.Append([ 
                 ("ModuleName", minfo["name"]),
                 ("ModuleKey", minfo["key"]),
                 ("ModulePath", minfo["path"])
              ])
      raise e
    
    # Add to the moduleinfo
    minfo["passedoptions"] = newopt

    mst.SetOptions(key, minfo["passedoptions"]);




# Go through all modules and if options aren't set,
# use defaults (if possible)
def CommitOptions():
    err = False

    for k,m in modmap.items():
        # if passedoptions are there already, then they have been set
        if not "passedoptions" in m:
            # the passed options are the defaults.
            # Check if any are required. If so, bail out
            for ok,ov in m["options"].items():
                if ov[1] == True:
                    Error("Option \"%1%\" for key \"%2\" is not specified, but is required", ok, k)
                    err = True

            if err:
                raise exception.PyGeneralException("Error setting default options", 
                                      [ 
                                         ("ModuleName", m["name"]),
                                         ("ModuleKey", m["key"]),
                                         ("ModulePath", m["path"])
                                      ])

            # create the passed options
            passedopt = {}
            for ok,ov in m["options"].items():
                if HasDefault(ov[0]):
                  passedopt[ok] = ov[0]

            SetOptions(k, passedopt)
           



def DumpModuleInfo():
    for k,m in modmap.items():
        # Dump some info
        PrintModuleInfo(m)



def SanityCheck():
    global mst
    mst.TestAll() 
