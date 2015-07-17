#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

modpath = os.path.join("/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, modpath)

import bppython as bp

bp.Init()

# Set some outputs
bp.bpcore.SetOut_Stdout()
#bpcore.SetOut_File("test.out")
bp.bpcore.SetColor(True)
bp.bpcore.SetDebug(True)




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

  b1.RunTest()
  bp.Output("\n")
  b2.RunTest()
  bp.Output("\n")

  bp.Output("\nDone testing\n")


except Exception as e:
  bp.Output("Caught exception in main handler\n")
  #traceback.print_exc()
  bp.Error("\n")
  bp.Error(str(e))
  bp.Error("\n")


bp.Finalize()
