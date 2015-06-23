#!/usr/bin/env python3

import sys
import importlib
import os
from . import utils

class ModuleStore:



  def __init__(self):

    # Maps name to all module info
    self.loaded_modules = {}



  def LoadModule(self, name, path):

    # store the path, etc. We will reset later
    oldpath = sys.path
    olddl = sys.getdlopenflags()

    sys.path.append(path)
    sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
    m = importlib.import_module(name)

    # reset the path, etc
    sys.path = oldpath
    sys.setdlopenflags(olddl)


    # todo check for duplicates
    for key, minfo in m.minfo.items():
      utils.PrintModuleInfo(key, path, minfo)
   
      self.loaded_modules[key] = m
                                   
    return m

 
  def CleanUp(self):
    self.loaded_modules = {}



  def Dump(self):
    print(self.loaded_modules)



  def CreateObject(self, key):
    if not key in loaded_modules:
      raise RuntimeError("Key {} not loaded".format(key))

    m = loaded_modules[key]
    return m.CreateObject(self, key, m.minfo)

