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


mst = None
cml = None
pml = None


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



def LoadModule(name, useropt = {}):
    Output(Line('*'))
    Output("Importing module %1%\n", name)

    olddl = sys.getdlopenflags()
    sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
    m = importlib.import_module(name)
    sys.setdlopenflags(olddl)

    path = os.path.dirname(m.__file__) + "/"




    for key,minfo in m.minfo.items():
        Output("\n")
        Output(Line('-'))
        Output("Loading module %1% v%2%\n", minfo["name"], minfo["version"])
        Output(Line('-'))

        # Copy the key to the dict
        minfo["key"] = key

        # set the path for all
        minfo["path"] = path

        # merge the options
        if key in useropt:  #the user specified options for this key
          user = useropt[key]
        else:
          user = {}
   
        defopt = minfo["options"]
        try:
          newopt = MergeAndCheckOptions(defopt, user)
        except BPModuleException as e:
          e.Append([ 
                     ("ModuleName", minfo["name"]),
                     ("ModuleKey", minfo["key"]),
                     ("ModulePath", minfo["path"])
                  ])
          raise e
                   
        # Add to the moduleinfo
        minfo["passedoptions"] = newopt

        # Dump some info
        PrintModuleInfo(key, minfo)

        # Load & insert
        # skip core types and others
        if minfo["type"] == "c_module":
            cml.LoadSO(key, minfo)
        elif minfo["type"] == "python_module":
            pml.AddPyModule(key, m.CreateModule, minfo)

    Debug("Done importing module %1%\n", name)
    Output("\n")
    return m

