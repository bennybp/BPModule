#!/usr/bin/env python3

import sys
import importlib
import os


def LoadModule(name, path):
  oldpath = sys.path
  olddl = sys.getdlopenflags()

  sys.path.append(path)
  sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)
  m = importlib.import_module(name)
  sys.path = oldpath
  sys.setdlopenflags(olddl)
  return m
  
