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
from pulsar.datastore import *
from pulsar.system import *
from pulsar.math import *

from helper.TestOptions import opttypemap, validvals
from helper.TestAtoms import nonane

def Run():
    try:
        tester = Tester("Testing Wavefunction - serialization")
        tester.PrintHeader()

        w = Wavefunction()
        tester.Test("Serialization of empty wavefunction", True,
                    TestSerialization_Wavefunction, w)

        atoms = list(nonane)
        u = AtomSetUniverse()
        for a in atoms:
            u.Insert(a)

        s = System(u, True)
        s = ApplySingleBasis("primary", "sto-3g", s)


        w.system = s
        tester.Test("Serialization with system", True,
                    TestSerialization_Wavefunction, w)

        w.cmat = IrrepSpinMatrixD()
        w.cmat.Set(Irrep.A1, 0, SimpleMatrixD(2, 2, [1, 2, 3, 4]))
        tester.Test("Serialization with cmat", True,
                    TestSerialization_Wavefunction, w)

        w.epsilon = IrrepSpinVectorD()
        w.cmat.Set(Irrep.E2, 0, SimpleVectorD(2, [1, 2]))
        tester.Test("Serialization with epsilon", True,
                    TestSerialization_Wavefunction, w)
 

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
