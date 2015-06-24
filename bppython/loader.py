#!/usr/bin/env python3

import sys
import importlib
import os
from . import utils

def LoadCoreModule(name):
  # store the path, etc. We will reset later
  olddl = sys.getdlopenflags()
  sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
  m = importlib.import_module(name)
  sys.setdlopenflags(olddl)

  path = os.path.dirname(m.__file__)

  for key,minfo in m.minfo.items():
    # Dump some info
    utils.PrintModuleInfo(key, path, minfo)
  return m
  


def LoadCModule(name, cml):
  olddl = sys.getdlopenflags()
  sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
  m = importlib.import_module(name)
  sys.setdlopenflags(olddl)

  path = os.path.dirname(m.__file__)

  for key,minfo in m.minfo.items():

    sopath = os.path.join(path, minfo["soname"]) + ".so"

    # Dump some info
    utils.PrintModuleInfo(key, path, minfo)

    # find the full path of the so file
    if not os.path.isfile(sopath):
      raise RuntimeError("Error - {} does not exist or is not a file!".format(sopath))

    # load & insert into the modulestore
    cml.LoadSO(sopath, key, minfo)


  return m
  

def LoadPyModule(name, pml):
  olddl = sys.getdlopenflags()
  sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
  m = importlib.import_module(name)
  sys.setdlopenflags(olddl)

  path = os.path.dirname(m.__file__)

  for key,minfo in m.minfo.items():
    # Dump some info
    utils.PrintModuleInfo(key, path, minfo)

    # load & insert into the modulestore
    pml.AddPyModule(path, key, m.CreateModule, minfo)


  return m
