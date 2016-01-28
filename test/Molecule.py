#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp


def Run():
    try:

        tester = bp.testing.Tester("Testing Molecule class")
        tester.PrintHeader()

        mol = bp.system.Molecule()
        mol.AddAtom(1, [0.0, 0.0, 0.0] )
        mol.AddAtom(2, [1.0, 0.0, 0.0] )
        mol.AddAtom(3, [0.0, 1.0, 0.0] )
        mol.AddAtom(4, [0.0, 0.0, 1.0] )
        bp.output.Output("Number of atoms: %1%\n", mol.NAtoms())

        for i in range(0, mol.NAtoms()):
          a = mol.GetAtom(i)
          print("{:<4}  {:<4}  {:<15}  {:<20}   {}".format(i,
                                                             a.Symbol(),
                                                             a.Name(),
                                                             a.Mass(),
                                                             a.xyz))

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
