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
        tester.PrintHeader()

        atoms = list(nonane)
        u = AtomSetUniverse()
        for a in atoms:
            u.Insert(a)

        s = System(u, True)
        s = ApplySingleBasis("primary", "sto-3g", s)

        tester.Test("System serialization", True, TestSerialization_System, s)

        i = 0
        for a in s:
            tester.Test("Atom serialization - {}".format(i), True,
                        TestSerialization_Atom, a)
            i += 1
            

        bs = s.GetBasisSet("primary")
        tester.Test("BasisSet serialization", True,
                    TestSerialization_BasisSet, bs)


        for i in range(0, bs.NShell()):
            s = bs.ShellInfo(i)
            tester.Test("ShellInfo serialization - {}".format(i), True,
                        TestSerialization_BasisShellInfo, s)

        tester.PrintResults() 


    except Exception as e:
      GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      GlobalError("\n")
      GlobalError(str(e))
      GlobalError("\n")




psr.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.Finalize()
