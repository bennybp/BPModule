#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")

import bpmodule as bp


def Run():
    try:

        tester = bp.testing.Tester("Testing construction of OptionMap objects")
        tester.PrintHeader()

        for i in range(1, 20):
            bp.output.Output("Z = %1%\n", i)
            bp.output.Output("    Name: %1%\n", bp.molecule.AtomicNameFromZ(i))
            bp.output.Output("  Symbol: %1%\n", bp.molecule.AtomicSymFromZ(i))
            bp.output.Output("    Mass: %1%\n", bp.molecule.AtomicMassFromZ(i))
            bp.output.Output("\n")

        tester.PrintResults() 


    except Exception as e:
      bp.output.Output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
