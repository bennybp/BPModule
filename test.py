#!/usr/bin/env python3

import gc
import argparse
import os
import importlib
import sys

sys.stdout = open('test.out', 'w')

sys.path.append("/home/ben/programming/BPModule/python")
import bpmodule as bp

bpcore = bp.loader.LoadModule("bpmodule_core", "/home/ben/programming/BPModule/build/BPModule/core")
t1 = bp.loader.LoadModule("test1", "/home/ben/programming/BPModule/build/BPModule/modules")

mst = bpcore.ModuleStore()

for key,value in t1.modinfo.minfo.items():
  mi = bp.utils.MakeInfo(value, bpcore)

  # find the full path of the so file
  fullpath = os.path.join(t1.modinfo.moddir, value["soname"]) + ".so" 
  if not os.path.isfile(fullpath):
    raise RuntimeError("Error - {} does not exist or is not a file!".format(fullpath))

  mst.LoadSO(key, fullpath, mi)
  #mst.Dump()


b1 = mst.GetModule_Test("TESTMOD1")
#b1.Help()
mst.Help("TESTMOD1")

# manually destroy all objects? then the store
# A hack for now
b1 = None
gc.collect()
