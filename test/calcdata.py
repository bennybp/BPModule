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

  bp.SetOptions("TESTMOD1", { "double_opt_def": 1.111, "int_opt_def" : 55 })
  bp.CommitOptions()
  bp.DumpModuleInfo()


  b1 = bp.mst.GetModule_Test("TESTMOD1")
  b2 = bp.mst.GetModule_Test("TESTPYMOD1")

  inp = bp.bpbase.CalcData()
  inp.Set("TEST_INPUT_1", 10.00)
  b1_r = b1.CalcTest(inp)
  bp.Output("B1 results: %1%\n", b1_r.GetCopy("TEST_RESULT_1"))

  b2_r = b2.CalcTest(inp)
  bp.Output("B2 results: %1%\n", b2_r.GetCopy("TEST_RESULT_1"))

  bp.Output("\nDone testing\n")


except Exception as e:
  bp.Output("Caught exception in main handler\n")
  #traceback.print_exc()
  bp.Error("\n")
  bp.Error(str(e))
  bp.Error("\n")


bp.Finalize()
