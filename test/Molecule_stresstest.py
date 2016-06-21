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
        tester.print_header()

        molu = psr.system.AtomSetUniverse()

        time0 = perf_counter()
        for i in range(0, ntest):
            molu.insert(psr.system.CreateAtom(i, [ random.uniform(-1000, 1000), random.uniform(-1000, 1000), random.uniform(-1000, 1000) ], 6))
        mol = psr.system.System(molu, True)

        mol = ApplySingleBasis("Primary","sto-3g",mol)

        time1 = perf_counter()
        psr.output.global_output("Size: {}\n".format(mol.Size()))
        psr.output.global_output("     Time to fill: {}\n".format(time1-time0))


        time0 = perf_counter()
        mol2 = mol.Translate( [ 2.0, 3.0, 4.0 ] )
        time1 = perf_counter()
        psr.output.global_output("Time to translate: {}\n".format(time1-time0))

        time0 = perf_counter()
        mol2 = mol.Rotate( [ 0.0, 1.0, 0.0,
                             0.0, 0.0, 1.0,
                             1.0, 0.0, 0.0 ] )
        time1 = perf_counter()
        psr.output.global_output("   Time to rotate: {}\n".format(time1-time0))

        time0 = perf_counter()
        com = mol.CenterOfMass()
        time1 = perf_counter()
        psr.output.global_output(" Time to find COM: {}\n".format(time1-time0))
        psr.output.global_output("COM: {}\n".format(com.GetCoords()))

        time0 = perf_counter()
        h = mol.MyHash().String()
        time1 = perf_counter()
        psr.output.global_output("Time to find hash: {}\n".format(time1-time0))
        psr.output.global_output("Hash: {}\n".format(h))

        time0 = perf_counter()
        bs = mol.get_basis_set("Primary")
        time1 = perf_counter()
        psr.output.global_output("Time to get BS: {}\n".format(time1-time0))

        time0 = perf_counter()
        h = bs.MyHash().String()
        time1 = perf_counter()
        psr.output.global_output("Time to find BS hash: {}\n".format(time1-time0))
        psr.output.global_output("Basis set size: {}\n".format(bs.NFunctions()))
        psr.output.global_output("Hash: {}\n".format(h))

        tester.print_results() 


    except Exception as e:
      psr.output.global_output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      psr.output.global_error("\n")
      psr.output.global_error(str(e))
      psr.output.global_error("\n")




psr.initialize(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.finalize()
