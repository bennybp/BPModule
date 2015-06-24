import sys
import os

from bppython import *


# for output:
# package args into a list then pass to bpcore
def Output(s, *args):
    bpcore.Output(s, list(args))


def Success(s, *args):
    bpcore.Success(s, list(args))

def Warning(s, *args):
    bpcore.Warning(s, list(args))

def Error(s, *args):
    bpcore.Error(s, list(args))

def Debug(s, *args):
    bpcore.Debug(s, list(args))



def PrintModuleInfo(key, minfo):

  print()
  print("  ++ Module: {}".format(key))
  print("            Name: {}".format(minfo["name"]))
  print("            Type: {}".format(minfo["type"]))
  print("            Path: {}".format(minfo["path"]))

  if "soname" in minfo:
      print("          SOName: {}".format(minfo["soname"]))

  print("         Version: {}".format(minfo["version"]))
  print("     Description: {}".format(minfo["description"]))

  print("         Authors: {}".format(len(minfo["authors"])))
  for aut in minfo["authors"]:
      print("                  {}".format(aut))

  print("      References: {}".format(len(minfo["refs"])))
  for ref in minfo["refs"]:
      print("                  {}".format(ref))

  print("         Options: {}".format(len(minfo["options"])))
  for opt in minfo["options"]:
      print("                  {:<12}  :  {:<12}  : {}".format(opt[0], opt[1], opt[2]))
  print()




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

