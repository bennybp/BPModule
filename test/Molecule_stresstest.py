#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 
from time import perf_counter
import random

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp


onemillion = 1000000
ntest = 5*onemillion


def Run():
    try:

        tester = bp.testing.Tester("Testing Molecule class")
        tester.PrintHeader()

        molu = bp.system.AtomSetUniverse()

        time0 = perf_counter()
        for i in range(0, ntest):
            molu.append(bp.system.CreateAtom(0, [ random.uniform(-1000, 1000), random.uniform(-1000, 1000), random.uniform(-1000, 1000) ], 6))
        mol = bp.system.Molecule(molu, True)
        time1 = perf_counter()
        bp.output.Output("NAtoms: {}\n".format(mol.NAtoms()))
        bp.output.Output("     Time to fill: {}\n".format(time1-time0))


        time0 = perf_counter()
        mol2 = mol.Translate( [ 2.0, 3.0, 4.0 ] )
        time1 = perf_counter()
        bp.output.Output("Time to translate: {}\n".format(time1-time0))

        time0 = perf_counter()
        mol2 = mol.Rotate( [ 0.0, 1.0, 0.0,
                             0.0, 0.0, 1.0,
                             1.0, 0.0, 0.0 ] )
        time1 = perf_counter()
        bp.output.Output("   Time to rotate: {}\n".format(time1-time0))

        time0 = perf_counter()
        com = mol.CenterOfMass()
        time1 = perf_counter()
        bp.output.Output(" Time to find COM: {}\n".format(time1-time0))
        bp.output.Output("COM: {}".format(com.GetCoords()))


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
