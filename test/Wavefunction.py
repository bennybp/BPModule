#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp


def Run():
    try:

        tester = bp.testing.Tester("Testing Wavefunction")
        tester.PrintHeader()

        mol = bp.molecule.Molecule()
        mol.AddAtom(1, [0.0, 0.0, 0.0] )
        mol.AddAtom(1, [1.0, 0.0, 0.0] )
        mol.AddAtom(8, [0.0, 1.0, 0.0] )


        bs = bp.basisset.SimpleCreator("/home/ben/programming/BPModule/install/basis/sto-3g.gbs", mol);

        wfn = bp.datastore.Wavefunction()
        wfn.basis.Set(bs)
        bp.output.Output("Basis object: %1%\n", str(wfn.basis))
        bp.output.Output("NShell: %1%\n", wfn.basis.NShell())


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