#!/usr/bin/env python3


import os
import sys
import traceback 


# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr
from pulsar.output import *
from pulsar.testing import *
from pulsar.system import *

from helper.SetOperations import test_set_operations
from helper.TestAtoms import nonane


def Run():
    try:
        tester = Tester("Testing System class - set operations")
        tester.print_header()

        atoms = list(nonane)

        a1 = Atom(atoms[0])
        a2 = Atom(atoms[0])
        a3 = Atom(atoms[0])
        a4 = Atom(atoms[0])
        a1[0] += 1.0e-10 # Slightly different coords
        a2[1] += 1.0e-10
        a3[2] += 1.0e-10
        a4.Z = 10    # Different Z
        badatoms = [ a1, a2, a3, a4 ]


        u = AtomSetUniverse()
        for a in atoms:
            u.insert(a)


        s0 = System(u, False)
        s1 = System(u, False)
        s2 = System(u, True)
        s3 = System(u, False)
        s9 = System(u, False)

        for it in atoms[:3]:
            s1.insert(it)

        for it in reversed(atoms):
            s3.insert(it)

        for it in atoms[3:]:
            s9.insert(it)

        ################################
        # Do basic testing of set stuff
        ################################
        test_set_operations(tester, System, atoms, badatoms,
                          True, s0, s1, s2, s3, s9)

        tester.print_results() 


    except Exception as e:
      print_global_output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      print_global_error("\n")
      print_global_error(str(e))
      print_global_error("\n")




psr.initialize(sys.argv, color = True, debug = True)
Run()
psr.finalize()
