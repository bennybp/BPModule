#!/usr/bin/env python3


import os
import sys
import traceback 


# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp
from bpmodule.output import *
from bpmodule.testing import *
from bpmodule.system import *

from helper.SetOperations import TestSetOperations
from helper.TestAtoms import nonane

def Run():
    try:
        tester = Tester("Testing System class - serialization")
        tester.PrintHeader()

        atoms = list(nonane)
        u = AtomSetUniverse()
        for a in atoms:
            u.Insert(a)

        s = System(u, True)
        s = ApplySingleBasis(ShellType.Gaussian, "primary", "sto-3g", s)

        tester.Test("System serialization", True, TestSerialization_System, s)

        i = 0
        for a in s:
            tester.Test("Atom serialization - {}".format(i), True,
                        TestSerialization_Atom, a)
            i += 1
            

        bs = s.GetBasisSet("primary")
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




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
