import sys
import os
import importlib

# Important! Symbols must be loaded now and available
# globally! WILL NOT WORK WITHOUT
sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
import bpmodule_core as bpcore

from .utils import *


# Print info about the core module
#print(bpcore.minfo)
for key,minfo in bpcore.minfo.items():
  minfo["key"] = key
  minfo["path"] = os.path.dirname(bpcore.__file__) + "/"
  PrintModuleInfo(key, minfo)


# Create the various stores and loaders
mst = bpcore.ModuleStore()
cml = bpcore.CModuleLoader(mst)
pml = bpcore.PyModuleLoader(mst)




def Init():
    pass


def Finalize():
    print("Deleting python modules")
    pml.DeleteAll()
    print("Deleting C modules")
    cml.DeleteAll()
    print("Closing C handles")
    cml.CloseHandles()
    print("BPModule finalized")


def LoadModule(name):
    olddl = sys.getdlopenflags()
    sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
    m = importlib.import_module(name)
    sys.setdlopenflags(olddl)

    path = os.path.dirname(m.__file__) + "/"

    for key,minfo in m.minfo.items():
        # Copy the key to the dict 
        minfo["key"] = key

        # set the path for all
        minfo["path"] = path

        # Dump some info
        PrintModuleInfo(key, minfo)

        # Load & insert
        # skip core types and others
        if minfo["type"] == "c_module":
            cml.LoadSO(key, minfo)
        elif minfo["type"] == "python_module":
            pml.AddPyModule(key, m.CreateModule, minfo)

    return m


def LoadModules(names):
    mods = []
    for name in names:
        mods.append(LoadModule(name))
    return mods

