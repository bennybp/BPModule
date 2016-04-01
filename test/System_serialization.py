#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

from helper.SetOperations import TestSetOperations
from helper.TestAtoms import nonane

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp
from bpmodule.system import System, AtomSetUniverse, Atom


def Run():
    try:
        out = bp.output.GetGlobalOut()

        tester = bp.testing.Tester("Testing System class - serialization")
        tester.PrintHeader()

        atoms = list(nonane)
        u = AtomSetUniverse()
        for a in atoms:
            u.Insert(a)

        s = System(u, True)
        s = bp.system.ApplySingleBasis(bp.system.ShellType.Gaussian, "primary", "sto-3g", s)

        i = 0
        for a in s:
            tester.Test("Atom serialization - {}".format(i), True,
                        bp.system.TestSerialization_Atom, a)
            i += 1
            

        bs = s.GetBasisSet("primary")
        for i in range(0, bs.NShell()):
            s = bs.ShellInfo(i)
            tester.Test("ShellInfo serialization - {}".format(i), True,
                        bp.system.TestSerialization_BasisShellInfo, s)

        tester.PrintResults() 


    except Exception as e:
      bp.output.GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.GlobalError("\n")
      bp.output.GlobalError(str(e))
      bp.output.GlobalError("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
