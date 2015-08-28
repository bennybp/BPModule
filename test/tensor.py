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
    bp.bpbase.SetOut_Stdout()
    #bpbase.SetOut_File("test.out")
    bp.bpbase.SetColor(True)
    bp.bpbase.SetDebug(True)


    # Test
    try:
      # Load the python modules
      bp.LoadModule("testtensor", "TESTTENSOR")

      bp.CommitOptions()
      bp.DumpModuleInfo()


      b1 = bp.mst.GetModule_Test("TESTTENSOR")

      inp = bp.bpbase.CalcData()
      inp.Set("TEST_INPUT_1", 10.00)
      b1_r = b1.CalcTest(inp)
      bp.Output("B1 results: %1%\n", b1_r.GetCopy("TEST_RESULT_1"))

      bp.Output("Has OUTMAT: %1%\n", b1_r.Has("OUTMAT"))

      bp.Output("\nDone testing\n")


    except Exception as e:
      bp.Output("Caught exception in main handler\n")
      #traceback.print_exc()
      bp.Error("\n")
      bp.Error(str(e))
      bp.Error("\n")




bp.Init(sys.argv)
Run()
bp.Finalize()
