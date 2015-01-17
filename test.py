#!/usr/bin/env python3

import gc
import argparse
import os
import importlib
import sys

sys.path.append("/home/ben/programming/BPModule/python")
import bpmodule as bp

bpcore = bp.loader.LoadModule("bpmodule_core", "/home/ben/programming/BPModule/build/BPModule/core")

bpcore.SetOut_Stdout()
#bpcore.SetOut_File("test.out")
bpcore.SetColor(True)
bpcore.SetDebug(True)


t1 = bp.loader.LoadModule("test1", "/home/ben/programming/BPModule/build/BPModule/modules")

mst = bpcore.ModuleStore()

for key,minfo in t1.modinfo.minfo.items():
  # find the full path of the so file
  fullpath = os.path.join(t1.modinfo.moddir, minfo["soname"]) + ".so" 
  if not os.path.isfile(fullpath):
    raise RuntimeError("Error - {} does not exist or is not a file!".format(fullpath))

  print(key)
  print(fullpath)
  print(minfo)
  mst.LoadSO(key, fullpath, minfo)
  #mst.Dump()


try:
  b1 = mst.GetModule_Test("TESTMOD1")
  #b1.RunTest()
  #print(mst.ModuleInfoFromKey("TESTMOD1"))
  #print(mst.KeyFromID(0))
  #print(mst.GetKeys())
  opt = b1.Options()
  inf = b1.Info()
  print(opt)
  print(inf)
 
except RuntimeError as e:
  print(e)


# manually destroy all objects? then the store
# A hack for now
b1 = None
gc.collect()
