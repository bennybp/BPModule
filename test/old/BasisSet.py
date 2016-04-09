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


def Run():
    try:
        out = bp.output.GetGlobalOut()
        tester = bp.testing.Tester("Testing BasisSet class")
        tester.PrintHeader()

        atoms = [ bp.system.CreateAtom(0, [ 0.000000000000,     0.000000000000,     0.000000000000], 1),
                  bp.system.CreateAtom(1, [ 1.000000000000,     0.000000000000,     0.000000000000], 1),
                  bp.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  bp.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  bp.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  bp.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  bp.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  bp.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  bp.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  bp.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  bp.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                ]

        molu = bp.system.AtomSetUniverse()
        for a in atoms:
            molu.append(a)


        mol = bp.system.System(molu, True)
        mol = bp.system.ApplySingleBasis(bp.system.ShellType.Gaussian, "primary", "sto-3g", mol)
        bs = mol.GetBasisSet("primary")
        bs.Print(out)

        tester.PrintResults() 


    except Exception as e:
      bp.output.GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.GlobalError("\n")
      bp.output.GlobalError(str(e))
      bp.output.GlobalError("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()