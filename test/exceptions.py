#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

modpath = os.path.join("/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, modpath)

import bppython as bp

bp.Init(sys.argv)

# Set some outputs
bp.bpbase.SetOut_Stdout()
#bpbase.SetOut_File("test.out")
bp.bpbase.SetColor(True)
bp.bpbase.SetDebug(True)


# Test
try:
  # Load the python modules
  bp.LoadModule("testmodule1", "TESTMOD1")
  bp.LoadModule("testpymodule1", "TESTPYMOD1")

  bp.SetOptions("TESTMOD1", { "double_opt_def": 1.111 })
  bp.CommitOptions()
  bp.DumpModuleInfo()


  b1 = bp.mst.GetModule_Test("TESTMOD1")
  b2 = bp.mst.GetModule_Test("TESTPYMOD1")

  bp.Output("\n!!!Testing exceptions\n")
  try:
    b1.Throw()
  except Exception as e2:
    bp.Error(str(e2))

  bp.Output("\n")

  try:
    b2.Throw()
  except Exception as e2:
    bp.Error(str(e2))

  bp.Output("\n")

  try:
    b1.CallThrow("TESTMOD1")
  except Exception as e2:
    bp.Error(str(e2))

  bp.Output("\n")

  try:
    b2.CallThrow("TESTPYMOD1")
  except Exception as e2:
    bp.Error(str(e2))

  try:
    b2.CallThrow("TESTMOD1")
  except Exception as e2:
    bp.Error(str(e2))

  bp.Output("\n")

  try:
    b1.CallThrow("TESTMOD1")
  except Exception as e2:
    bp.Error(str(e2))

  bp.Output("\n")

  raise bp.BPModuleException("Test exception from input", [ ( "Hi2", "There2" ) ])


  bp.Output("\nDone testing\n")


except Exception as e:
  bp.Output("Caught exception in main handler\n")
  #traceback.print_exc()
  bp.Error("\n")
  bp.Error(str(e))
  bp.Error("\n")


bp.Finalize()
