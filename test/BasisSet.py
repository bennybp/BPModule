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
        bspath = os.path.join(thispath, "../", "basis")
        bspath = os.path.realpath(bspath)
        bstype = bp.system.ShellType.Gaussian

        tester = bp.testing.Tester("Testing BasisSet class")
        tester.PrintHeader()

        molu = bp.system.AtomSetUniverse()
        molu.append(bp.system.CreateAtom(0, [ 0.000000000000,     0.000000000000,     0.000000000000], 1))
        molu.append(bp.system.CreateAtom(1, [ 1.000000000000,     0.000000000000,     0.000000000000], 1))
        molu.append(bp.system.CreateAtom(2, [ 0.000000000000,     1.000000000000,     0.000000000000], 8))

        mol = bp.system.Molecule(molu, True)
        molwithbs = bp.system.CreateSimpleBasisSet(os.path.join(bspath, "sto-3g.gbs"), mol, bstype, "primary");
        bs = molwithbs.GetBasisSet("primary")

        bp.output.Output("Number of shells: %1%\n", bs.NShell())
        for i in range(0, bs.NShell()):
           gs = bs.GetShell(i)
           print("ID={}  Cart={}   NPrim={}   {}".format(gs.ID(), gs.IsCartesian(), gs.NPrim(), gs.GetCoords()))
           for j in range(0, gs.NPrim()):
             print("      Prim {:<4} :  {:<10}    {:<10}".format(j, gs.Alpha(j), gs.Coef(j)))


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
