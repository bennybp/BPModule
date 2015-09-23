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
      #             supermodule       module name       key
      bp.LoadModule("test_options",   "testoptions1",   "TESTOPT1")

      b1 = bp.mst.GetModule_Test("TESTOPT1")
      bpmoutput.Output("Got module: %1% v%2%\n", b1.Version(), b1.Name())

      bp.output.Output("\nDone testing\n")


    except Exception as e:
      bp.output.Output("Caught exception in main handler\n")
      traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
