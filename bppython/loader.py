#!/usr/bin/env python3

import sys
import importlib
import os


def LoadModule(name, path):
  # store the path, etc. We will reset later
  oldpath = sys.path
  olddl = sys.getdlopenflags()

  sys.path.append(path)
  sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
  m = importlib.import_module(name)

  # reset the path, etc
  sys.path = oldpath
  sys.setdlopenflags(olddl)
  return m
  
