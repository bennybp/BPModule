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

        tester = bp.testing.Tester("Testing Molecule class")
        tester.PrintHeader()

        mol = bp.molecule.Molecule()
        mol.AddAtom(1, (0.0, 0.0, 0.0) )
        mol.AddAtom(1, (1.0, 0.0, 0.0) )
        mol.AddAtom(1, (0.0, 1.0, 0.0) )
        mol.AddAtom(1, (0.0, 0.0, 1.0) )
        bp.output.Output("Number of atoms: %1%\n", mol.NAtoms())

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
