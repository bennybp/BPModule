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

from helper.TestAtoms import nonane

def Run():
    try:
        tester = Tester("Testing System - serialization")
        tester.print_header()

        atoms = list(nonane)
        u = AtomSetUniverse()
        for a in atoms:
            u.insert(a)

        s = System(u, True)
        s = apply_single_basis("primary", "sto-3g", s)

        tester.test("System serialization", True, TestSerialization_System, s)

        i = 0
        for a in s:
            tester.test("Atom serialization - {}".format(i), True,
                        TestSerialization_Atom, a)
            i += 1
            

        bs = s.get_basis_set("primary")
        tester.test("BasisSet serialization", True,
                    TestSerialization_BasisSet, bs)


        for i in range(0, bs.n_shell()):
            s = bs.shell_info(i)
            tester.test("ShellInfo serialization - {}".format(i), True,
                        TestSerialization_BasisShellInfo, s)

        tester.print_results() 


    except Exception as e:
      print_global_output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      print_global_error("\n")
      print_global_error(str(e))
      print_global_error("\n")




psr.initialize(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.finalize()
