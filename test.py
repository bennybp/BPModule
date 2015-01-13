#!/usr/bin/env python3


import argparse
import os
import importlib
import sys

oldldflag = sys.getdlopenflags()
sys.path.append("/home/ben/programming/BPModule/build/BPModule/core")
sys.path.append("/home/ben/programming/BPModule/build/BPModule/modules")

sys.setdlopenflags(os.RTLD_GLOBAL | os.RTLD_NOW)
import bpmodule_core as bp
sys.setdlopenflags(oldldflag)
  

def MakeInfo(minfo):
  op = bp.OptionMap()
  for o in minfo["options"]:
    op.Set(o[0], o[1], o[2])

  mi = bp.MakeInfo(value, op)
  return mi
  

sys.setdlopenflags(os.RTLD_GLOBAL | os.RTLD_NOW)
import test1 as t1
sys.setdlopenflags(oldldflag)


mst = bp.ModuleStore()


for key,value in t1.modinfo.minfo.items():
  mi = MakeInfo(value)

  # find the full path of the so file
  fullpath = os.path.join(t1.modinfo.moddir, value["soname"]) + ".so" 
  if not os.path.isfile(fullpath):
    raise RuntimeError("Error - {} does not exist or is not a file!".format(fullpath))

  mst.LoadSO(key, fullpath, mi)
  mst.Dump()

