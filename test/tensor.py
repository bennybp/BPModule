#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

modpath = os.path.join("/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, modpath)

import bppython as bp


def Run():
    # Set some outputs
    bp.output.SetOut_Stdout()
    bp.output.SetColor(True)
    bp.output.SetDebug(True)


    # Test
    try:
      # Load the python modules
      bp.LoadModule("testtensor", "TESTTENSOR")

      bp.CommitOptions()
      bp.DumpModuleInfo()


      b1 = bp.mst.GetModule_Test("TESTTENSOR")

      inp = bp.datastore.CalcData()
      inp.Set("TEST_INPUT_1", 10.00)
      b1_r = b1.CalcTest(inp)
      bp.output.Output("B1 results: %1%\n", b1_r.GetCopy("TEST_RESULT_1"))

      bp.output.Output("Has OUTMAT: %1%\n", b1_r.Has("OUTMAT"))

      bp.output.Output("\nDone testing\n")


    except Exception as e:
      bp.output.Output("Caught exception in main handler\n")
      #traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv)
Run()
bp.Finalize()
