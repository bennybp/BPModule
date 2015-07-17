import sys
import os
import importlib

# Important! Symbols must be loaded now and available
# globally! WILL NOT WORK WITHOUT
sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
import bpmodule_core as bpcore

from .utils import *
from .exception import *
from .optioncheck import *
from .optionvalidate import *


mst = None
cml = None
pml = None
modmap = {}

def Init(output = "stdout", color = True, debug = False):
  global mst
  global cml
  global pml

  if output == "stdout":
    bpcore.SetOut_Stdout()
  else:
    bpcore.SetOut_File(output)

  bpcore.SetColor(color)
  bpcore.SetDebug(debug) 

  # Print info about the core module
  for key,minfo in bpcore.minfo.items():
      minfo["key"] = key
      minfo["path"] = os.path.dirname(bpcore.__file__) + "/"

      # merge the options
      defopt = minfo["options"]
      try:
        newopt = MergeAndCheckOptions(defopt, {}) # No options
      except BPModuleException as e:
        e.Append([ ("ModuleKey", minfo["key"]),
                   ("ModulePath", minfo["path"])
                ])
        raise e

      minfo["passedoptions"] = newopt
      PrintModuleInfo(key, minfo)


  # Create the various stores and loaders
  mst = bpcore.ModuleStore()
  cml = bpcore.CModuleLoader(mst)
  pml = bpcore.PyModuleLoader(mst)




def Finalize():
    Output("Deleting python modules\n")
    pml.DeleteAll()
    Output("Deleting C modules\n")
    cml.DeleteAll()
    Output("Closing C handles\n")

    cml.CloseHandles()
    Output("BPModule finalized\n")



def LoadModule(supermodule, key):
    Output("Importing %1% module from supermodule %2%\n", key, supermodule)

    try:
        olddl = sys.getdlopenflags()
        sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
        m = importlib.import_module(supermodule)
        sys.setdlopenflags(olddl)
    except Exception as e:
        raise BPModuleException(
                                "Unable to load supermodule {}".format(supermodule),
                                [
                                  ("Exception", str(e))
                                ]
                               )

    if not key in m.minfo:
        raise BPModuleException(
                                 "Supermodule doesn't have key!",
                                [
                                  ("Supermodule", supermodule),
                                  ("Key", key)
                                ]
                               )


    minfo = m.minfo[key]

    path = os.path.dirname(m.__file__) + "/"

    Output("\n")
    Output("Loading module %1% v%2%\n", minfo["name"], minfo["version"])

    # Copy the key to the dict
    minfo["key"] = key

    # set the path
    minfo["path"] = path


    if minfo["type"] == "c_module":
        cml.LoadSO(key, minfo)
    elif minfo["type"] == "python_module":
        pml.AddPyModule(key, m.CreateModule, minfo)
    Debug("Done importing module %1% from %2%\n", key, supermodule)
    Output("\n")

    modmap[key] = minfo;




def SetOptions(key, useropt):
    global mst

    if not key in modmap:
        raise BPModuleException(
                                 "Key not loaded, so I can't change the options!",
                                [
                                  ("Key", key)
                                ]
                               )

    minfo = modmap[key]
    defopt = minfo["options"]

    try:
      newopt = MergeAndCheckOptions(defopt, useropt)
    except BPModuleException as e:
      e.Append([ 
                 ("ModuleName", minfo["name"]),
                 ("ModuleKey", minfo["key"]),
                 ("ModulePath", minfo["path"])
              ])
      raise e
    
    # Add to the moduleinfo
    minfo["passedoptions"] = newopt

    mst.SetOptions(key, minfo["passedoptions"]);





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
                raise BPModuleException("Error setting default options", 
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

 
