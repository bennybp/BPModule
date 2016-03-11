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

      # Load the python modules
      #             supermodule      module name      key
      mm.LoadModule("TestModules",   "TestModule1",   "TESTMOD1")
      mm.LoadModule("TestModules",   "TestPyModule1", "TESTPYMOD1")

      mm.ChangeOption("TESTMOD1", "double_opt_def", 1.111)
      mm.ChangeOption("TESTMOD1", "int_opt_def", 55 )
      mm.PrintInfo()
      mm.SanityCheck()


      b1 = mm.GetModule("TESTMOD1", 0)
      b2 = mm.GetModule("TESTPYMOD1", 0)

      b1.RunTest()
      bp.output.GlobalOutput("\n")
      b2.RunTest()
      bp.output.GlobalOutput("\n")
    
      b3 = mm.GetModule("TESTMOD1", 0)
      b4 = mm.GetModule("TESTPYMOD1", 0)

      b3.RunTest()
      bp.output.GlobalOutput("\n")
      b4.RunTest()
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
