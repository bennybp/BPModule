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
        bspath = os.path.join(thispath, "../", "basis", "sto-3g.gbs")
        bspath = os.path.realpath(bspath)
        bstype = bp.system.ShellType.Gaussian
        bsmap = bp.system.ReadBasisFile(bstype, bspath)

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

        for a in atoms:
            a.SetShells("primary", bsmap[a.GetZ()])

        molu = bp.system.AtomSetUniverse()
        for a in atoms:
            molu.append(a)

        mol = bp.system.Molecule(molu, True)
        bs = mol.GetBasisSet("primary")
        bs.Print()

        bs.Shrink()
        bs.Print()

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
