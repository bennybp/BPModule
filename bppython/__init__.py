import sys
import os
import importlib
from .utils import *

sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
import bpmodule_core as bpcore


# Print info about the core module
print(bpcore.minfo)
for key,minfo in bpcore.minfo.items():
  minfo["path"] = os.path.dirname(bpcore.__file__) + "/"
  PrintModuleInfo(key, minfo)


# Create the various stores and loaders
mst = bpcore.ModuleStore()
cml = bpcore.CModuleLoader(mst)
pml = bpcore.PyModuleLoader(mst)




def Init():
    pass


def Finalize():
    pml.DeleteAll()
    cml.DeleteAll()
    cml.CloseHandles()


def LoadModule(name):
    olddl = sys.getdlopenflags()
    sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
    m = importlib.import_module(name)
    sys.setdlopenflags(olddl)

    path = os.path.dirname(m.__file__) + "/"

    for key,minfo in m.minfo.items():
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
