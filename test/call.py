#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, "/home/ben/programming/ambit/install/lib")

import bpmodule as bp


def Run():
    try:
      # Load the python modules
      #             supermodule      module name      key
      bp.LoadModule("testmodule1",   "testmodule1",   "TESTMOD1")
      bp.LoadModule("testpymodule1", "testpymodule1", "TESTPYMOD1")

      bp.SetOptions("TESTMOD1", { "double_opt_def": 1.111, "int_opt_def" : 55 })
      bp.CommitOptions()
      bp.DumpModuleInfo()
      bp.SanityCheck()


      b1 = bp.mst.GetModule_Test("TESTMOD1")
      b2 = bp.mst.GetModule_Test("TESTPYMOD1")

      b1.RunTest()
      bp.output.Output("\n")
      b2.RunTest()
      bp.output.Output("\n")

      bp.output.Output("\nDone testing\n")


    except Exception as e:
      bp.output.Output("Caught exception in main handler\n")
      #traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
