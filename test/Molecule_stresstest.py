#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 
from time import perf_counter
import random

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr
from pulsar.system import *


onemillion = 1000000
ntest = 10000


def Run():
    try:

        tester = psr.testing.Tester("Testing System class")
        tester.PrintHeader()

        molu = psr.system.AtomSetUniverse()

        time0 = perf_counter()
        for i in range(0, ntest):
            molu.Insert(psr.system.CreateAtom(i, [ random.uniform(-1000, 1000), random.uniform(-1000, 1000), random.uniform(-1000, 1000) ], 6))
        mol = psr.system.System(molu, True)

        mol = ApplySingleBasis("Primary","sto-3g",mol)

        time1 = perf_counter()
        psr.output.GlobalOutput("Size: {}\n".format(mol.Size()))
        psr.output.GlobalOutput("     Time to fill: {}\n".format(time1-time0))


        time0 = perf_counter()
        mol2 = mol.Translate( [ 2.0, 3.0, 4.0 ] )
        time1 = perf_counter()
        psr.output.GlobalOutput("Time to translate: {}\n".format(time1-time0))

        time0 = perf_counter()
        mol2 = mol.Rotate( [ 0.0, 1.0, 0.0,
                             0.0, 0.0, 1.0,
                             1.0, 0.0, 0.0 ] )
        time1 = perf_counter()
        psr.output.GlobalOutput("   Time to rotate: {}\n".format(time1-time0))

        time0 = perf_counter()
        com = mol.CenterOfMass()
        time1 = perf_counter()
        psr.output.GlobalOutput(" Time to find COM: {}\n".format(time1-time0))
        psr.output.GlobalOutput("COM: {}\n".format(com.GetCoords()))

        time0 = perf_counter()
        h = mol.MyHash().String()
        time1 = perf_counter()
        psr.output.GlobalOutput("Time to find hash: {}\n".format(time1-time0))
        psr.output.GlobalOutput("Hash: {}\n".format(h))

        time0 = perf_counter()
        bs = mol.GetBasisSet("Primary")
        time1 = perf_counter()
        psr.output.GlobalOutput("Time to get BS: {}\n".format(time1-time0))

        time0 = perf_counter()
        h = bs.MyHash().String()
        time1 = perf_counter()
        psr.output.GlobalOutput("Time to find BS hash: {}\n".format(time1-time0))
        psr.output.GlobalOutput("Basis set size: {}\n".format(bs.NFunctions()))
        psr.output.GlobalOutput("Hash: {}\n".format(h))

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
