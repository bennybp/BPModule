#!/usr/bin/env python3

import gc
import argparse
import os
import importlib
import sys

basepath = "/home/ben/programming/BPModule/install"
modpath = os.path.join(basepath, "modules")

# import bpmodule python helpers
# the path should be the highest priority
sys.path.insert(0, basepath)
import bppython as bp








# Load the core module
bpcore = bp.loader.LoadModule("bpmodule_core", modpath, None)


# Set some outputs
bpcore.SetOut_Stdout()
#bpcore.SetOut_File("test.out")
bpcore.SetColor(True)
bpcore.SetDebug(True)

# Create the module store
mst = bpcore.ModuleStore()

# Load the python modules
t1 = bp.loader.LoadModule("testmodule1", modpath, mst)


#mst.Dump()

# Test
try:
  b1 = mst.GetModule_Test("TESTMOD1")
  print(b1)
  b1.RunTest()
 
except RuntimeError as e:
  print(e)


# manually destroy all objects? then the store
# A hack for now
b1 = None
gc.collect()
