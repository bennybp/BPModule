#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "../", "modules")
sys.path.insert(0, psrpath)

import pulsar as psr


def Run(mm):
    try:
      out = psr.output.GetGlobalOut()

      # Load the python modules
      #             supermodule      module name      key
      mm.LoadModule("TestModules",   "TestExtLib",   "TESTEXTLIB")
      mm.Print(out)
      mm.SanityCheck()

      b1 = mm.GetModule("TESTEXTLIB", 0)

      b1.RunTest()
      psr.output.GlobalOutput("\n")

      psr.output.GlobalOutput("\nDone testing\n")

    except Exception as e:
      psr.output.GlobalOutput("Caught exception in main handler\n")
      traceback.print_exc()
      psr.output.GlobalError("\n")
      psr.output.GlobalError(str(e))
      psr.output.GlobalError("\n")



psr.Init(sys.argv, out = "stdout", color = True, debug = True)

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.Finalize()
