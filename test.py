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

# Create the module store and loaders
mst = bpcore.ModuleStore()
cml = bpcore.CModuleLoader(mst)

# Load the python modules
t1 = bp.loader.LoadCModule("testmodule1", modpath, cml)


# Test
try:
  b1 = mst.GetModule_Test("TESTMOD1")
  b2 = mst.GetModule_Test("TESTMOD1")
  b3 = mst.GetModule_Test("TESTMOD1")
  print("Module ID: {}  :  {}".format(b1.ID(), b1))
  print("Module ID: {}  :  {}".format(b2.ID(), b2))
  print("Module ID: {}  :  {}".format(b3.ID(), b3))

  mst.Delete(1)
 
  b1.RunTest()
 
except RuntimeError as e:
  print(e)
