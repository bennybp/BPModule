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

        bs = bp.basisset.BasisSet()

        gs = bp.basisset.GaussianShell(0, False, 0.0, 0.0, 0.0)
        gs.AddPrimitive(0.50, 1.00)
        gs.AddPrimitive(0.25, 0.50)
        gs.AddPrimitive(0.10, 0.25)
        bs.AddShell(gs)

        gs = bs.Shell(0)

        bp.output.Output("Number of shells: %1%\n", bs.NShell())
        for i in range(0, bs.NShell()):
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
