#!/usr/bin/env python3

import sys
import importlib
import os
from . import utils

def LoadModule(name, path, modulestore):
  # store the path, etc. We will reset later
  oldpath = sys.path
  olddl = sys.getdlopenflags()

  sys.path.append(path)
  sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
  m = importlib.import_module(name)

  # reset the path, etc
  sys.path = oldpath
  sys.setdlopenflags(olddl)


  for key,minfo in m.minfo.items():

    if "soname" in minfo:
      fullpath = os.path.join(path, name, minfo["soname"]) + ".so"
    else:
      fullpath = path

    # Dump some info
    utils.PrintModuleInfo(key, fullpath, minfo)

    # if given a modulestore
    # (should only NOT be given when loading core)
    if modulestore and "soname" in minfo:
      # find the full path of the so file
      if not os.path.isfile(fullpath):
        raise RuntimeError("Error - {} does not exist or is not a file!".format(fullpath))

      # insert into the modulestore
      modulestore.LoadSO(key, fullpath, minfo)

  return m
  


def LoadCModule(name, path, cml):
  # store the path, etc. We will reset later
  oldpath = sys.path
  olddl = sys.getdlopenflags()

  sys.path.append(path)
  sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
  m = importlib.import_module(name)

  # reset the path, etc
  sys.path = oldpath
  sys.setdlopenflags(olddl)


  for key,minfo in m.minfo.items():

    fullpath = os.path.join(path, name, minfo["soname"]) + ".so"

    # Dump some info
    utils.PrintModuleInfo(key, fullpath, minfo)

    # find the full path of the so file
    if not os.path.isfile(fullpath):
      raise RuntimeError("Error - {} does not exist or is not a file!".format(fullpath))

    # load & insert into the modulestore
    cml.LoadSO(fullpath, key, minfo)


  return m
  

