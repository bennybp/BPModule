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

        tester = bp.testing.Tester("Testing BasisSet class")
        tester.PrintHeader()

        mol = bp.molecule.Molecule()
        mol.AddAtom(1, [0.0, 0.0, 0.0] )
        mol.AddAtom(1, [1.0, 0.0, 0.0] )
        mol.AddAtom(8, [0.0, 1.0, 0.0] )


        bs = bp.basisset.SimpleCreator("/home/ben/programming/BPModule/install/basis/sto-3g.gbs", mol);

        bp.output.Output("Number of shells: %1%\n", bs.NShell())
        for i in range(0, bs.NShell()):
           gs = bs.Shell(i)
           print("ID={}  Cart={}   NPrim={}   {}".format(gs.ID(), gs.IsCartesian(), gs.NPrim(), gs.Coordinates()))
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
