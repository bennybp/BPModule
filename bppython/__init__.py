import sys
import os
import importlib

# Important! Symbols must be loaded now and available
# globally! WILL NOT WORK WITHOUT
sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)

##########################################
# Load all the core modules
##########################################
# Note for bpmodule_exception
# Just need to load exception class and register translator
# Otherwise, we use the python exception in exception.py
import bpmodule_python_helper as python_helper
import bpmodule_output as output
import bpmodule_exception as exception_
import bpmodule_parallel as parallel
import bpmodule_tensor as tensor
import bpmodule_datastore as datastore
import bpmodule_modulebase as modulebase
import bpmodule_modulestore as modulestore

##########################################
# Load other python stuff from this directory
##########################################
from .utils import *
#from .exception import *
from .optioncheck import *
from .optionvalidate import *


# Main module store and module loaders
mst = None
cml = None
pml = None
modmap = {}



# Initializes the BPModule core
def Init(argv, out = "stdout", color = True, debug = False):
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


  # Print info about the base module
  for key,minfo in modulestore.minfo.items():
      minfo["key"] = key
      minfo["path"] = os.path.dirname(modulestore.__file__) + "/"

      # merge the options
      defopt = minfo["options"]
      try:
        newopt = MergeAndCheckOptions(defopt, {}) # No options
      except exception.GeneralException as e:
        e.Append([ ("ModuleKey", minfo["key"]),
                   ("ModulePath", minfo["path"])
                ])
        raise e

      minfo["passedoptions"] = newopt
      PrintModuleInfo(key, minfo)


  # Create the various stores and loaders
  mst = modulestore.ModuleStore()
  cml = modulestore.CModuleLoader(mst)
  pml = modulestore.PyModuleLoader(mst)




def Finalize():
    output.Output("Deleting python modules\n")
    pml.UnloadAll()
    output.Output("Deleting C modules\n")
    cml.UnloadAll()
    output.Output("Closing C handles\n")

    cml.CloseHandles()
    output.Output("BPModule finalized\n")

    tensor.FinalizeTensor()
    parallel.FinalizeParallel()





def LoadModule(supermodule, key):
    output.Output("Importing %1% module from supermodule %2%\n", key, supermodule)

    try:
        olddl = sys.getdlopenflags()
        sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
        m = importlib.import_module(supermodule)
        sys.setdlopenflags(olddl)
    except Exception as e:
        raise exception.GeneralException(
                                "Unable to load supermodule {}".format(supermodule),
                                [
                                  ("Exception", str(e))
                                ]
                               )

    if not key in m.minfo:
        raise exception.GeneralException(
                                 "Supermodule doesn't have key!",
                                [
                                  ("Supermodule", supermodule),
                                  ("Key", key)
                                ]
                               )


    minfo = m.minfo[key]

    path = os.path.dirname(m.__file__) + "/"

    output.Output("\n")
    output.Output("Loading module %1% v%2%\n", minfo["name"], minfo["version"])

    # Copy the key to the dict
    minfo["key"] = key

    # set the path
    minfo["path"] = path


    if minfo["type"] == "c_module":
        cml.LoadSO(key, minfo)
    elif minfo["type"] == "python_module":
        pml.AddPyModule(key, m.CreateModule, minfo)
    output.Debug("Done importing module %1% from %2%\n", key, supermodule)
    output.Output("\n")

    modmap[key] = minfo;



# Set the options for a module under a given key
def SetOptions(key, useropt):
    global mst

    if not key in modmap:
        raise exception.GeneralException(
                                 "Key not loaded, so I can't change the options!",
                                [
                                  ("Key", key)
                                ]
                               )

    minfo = modmap[key]
    defopt = minfo["options"]

    try:
      newopt = MergeAndCheckOptions(defopt, useropt)
    except exception.GeneralException as e:
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
                raise exception.GeneralException("Error setting default options", 
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
        PrintModuleInfo(k, m)

 
