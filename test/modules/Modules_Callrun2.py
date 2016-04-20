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
      mm.LoadModule("TestModules",   "TestModule1",   "TESTMOD1")
      mm.LoadModule("TestModules",   "TestPyModule1", "TESTPYMOD1")

      mm.ChangeOption("TESTMOD1", "double_opt_def", 1.111)
      mm.ChangeOption("TESTMOD1", "int_opt_def", 55 )
      mm.Print(out)
      mm.SanityCheck()


      b1 = mm.GetModule("TESTMOD1", 0)
      b2 = mm.GetModule("TESTPYMOD1", 0)

      for b in [b1, b2]:
        for other1 in ["TESTMOD1", "TESTPYMOD1"]:
            for other2 in ["TESTMOD1", "TESTPYMOD1"]:
                b.CallRunTest2(other1, other2)
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
