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
      #             supermodule   module name   key
      bp.LoadModule("testtensor", "testtensor", "TESTTENSOR")

      bp.CommitOptions()
      bp.DumpModuleInfo()


      b1 = bp.mst.GetModule_Test("TESTTENSOR")

      inp = bp.datastore.CalcData()
      inp.Set("TEST_INPUT_1", 10.00)
      b1_r = b1.CalcTest(inp)
      bp.output.Output("B1 results: %1%\n", b1_r.GetCopy("TEST_RESULT_1"))

      bp.output.Output("Has OUTMAT: %1%\n", b1_r.Has("OUTMAT"))
      mat = b1_r.GetCopy("OUTMAT")
      print("MAT: {}".format(type(mat)))
      mat.printf()

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
