#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr


def Run():
    try:
        out = psr.output.GetGlobalOut()
        tester = psr.testing.Tester("Testing BasisSet class")
        tester.PrintHeader()

        atoms = [ psr.system.CreateAtom(0, [ 0.000000000000,     0.000000000000,     0.000000000000], 1),
                  psr.system.CreateAtom(1, [ 1.000000000000,     0.000000000000,     0.000000000000], 1),
                  psr.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  psr.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  psr.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  psr.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  psr.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  psr.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  psr.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  psr.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                  psr.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8),
                ]

        molu = psr.system.AtomSetUniverse()
        for a in atoms:
            molu.append(a)


        mol = psr.system.System(molu, True)
        mol = psr.system.ApplySingleBasis(psr.system.ShellType.Gaussian, "primary", "sto-3g", mol)
        bs = mol.GetBasisSet("primary")
        bs.Print(out)

        tester.PrintResults() 


    except Exception as e:
      psr.output.GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      psr.output.GlobalError("\n")
      psr.output.GlobalError(str(e))
      psr.output.GlobalError("\n")




psr.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.Finalize()
