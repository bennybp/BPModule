#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp


def Run(mm):
    try:
      out = bp.output.GetGlobalOut()

      # Load the python modules
      #             supermodule      module name      key
      mm.LoadModule("TestModules",   "TestExtLib",   "TESTEXTLIB")
      mm.Print(out)
      mm.SanityCheck()

      b1 = mm.GetModule("TESTEXTLIB", 0)

      b1.RunTest()
      bp.output.GlobalOutput("\n")

      bp.output.GlobalOutput("\nDone testing\n")

    except Exception as e:
      bp.output.GlobalOutput("Caught exception in main handler\n")
      traceback.print_exc()
      bp.output.GlobalError("\n")
      bp.output.GlobalError(str(e))
      bp.output.GlobalError("\n")



bp.Init(sys.argv, out = "stdout", color = True, debug = True)

with bp.ModuleAdministrator() as mm:
    Run(mm)

bp.Finalize()
