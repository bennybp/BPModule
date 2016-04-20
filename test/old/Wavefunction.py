#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr


def Run():
    try:

        tester = psr.testing.Tester("Testing Wavefunction")
        tester.PrintHeader()

        mol = psr.system.System()
        mol.AddAtom(1, [0.0, 0.0, 0.0] )
        mol.AddAtom(1, [1.0, 0.0, 0.0] )
        mol.AddAtom(8, [0.0, 1.0, 0.0] )


        bs = psr.system.CreateSimpleBasisSet("/home/ben/programming/BPModule/install/basis/sto-3g.gbs", mol);

        wfn = psr.datastore.Wavefunction()
        wfn.basis.Set(bs)
        psr.output.GlobalOutput("Basis object: {}\n".format(str(wfn.basis)))
        psr.output.GlobalOutput("NShell: {}\n".format(wfn.basis.NShell()))


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
