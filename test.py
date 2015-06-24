#!/usr/bin/env python3


import os
import sys
import argparse

modpath = os.path.join("/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, modpath)

import bppython as bp


# Set some outputs
bp.bpcore.SetOut_Stdout()
#bpcore.SetOut_File("test.out")
bp.bpcore.SetColor(True)
bp.bpcore.SetDebug(True)

# Create the module store and loaders
mst = bp.bpcore.ModuleStore()
cml = bp.bpcore.CModuleLoader(mst)
pml = bp.bpcore.PyModuleLoader(mst)

# Load the python modules
t1 = bp.loader.LoadCModule("testmodule1", cml)
t2 = bp.loader.LoadPyModule("testpymodule1", pml)

# Test
try:
  b1 = mst.GetModule_Test("TESTMOD1")
  b2 = mst.GetModule_Test("TESTPYMOD1")

  b1.RunTest()
  print()
  b1.RunCallTest("TESTMOD1")
  print()
  b1.RunCallTest("TESTPYMOD1")
  print()

  b2.RunTest()
  print()
  b2.RunCallTest("TESTMOD1")
  print()
  b2.RunCallTest("TESTPYMOD1")
  print()

  print("\nDone testing\n")
 
except RuntimeError as e:
  print(e)


pml.DeleteAll()
cml.DeleteAll()
cml.CloseHandles()


