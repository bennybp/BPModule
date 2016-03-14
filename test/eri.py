#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp


def Run(mm):
    try:
        # Load the python modules
        #             supermodule      module name      key
        mm.LoadModule("LibERD",        "LibERD_ERI",   "ERI")
        mm.PrintInfo()
        mm.SanityCheck()
  
        atoms = [ bp.system.CreateAtom(0, [ 0.000000000000, -0.143225816552,  0.000000000000], 8),
                  bp.system.CreateAtom(1, [ 1.638036840407,  1.136548822547, -0.000000000000], 1),
                  bp.system.CreateAtom(2, [-1.638036840407,  1.136548822547, -0.000000000000], 1)
                ]  

        molu = bp.system.AtomSetUniverse()
        for a in atoms:
            molu.append(a)

        mol = bp.system.System(molu, True)
        mol = bp.system.ApplySingleBasis(bp.system.ShellType.Gaussian, "primary", "sto-3g", mol)
        bs = mol.GetBasisSet("primary")
        bs.Print()
  
  
         
        eri = mm.GetModule("ERI", 0)
        ubs = bp.datastore.MakeUIDPointer(bs)
        eri.SetBases(4, ubs, ubs, ubs, ubs)
        n = eri.Calculate(1, 0, 0, 0, 0)
        bp.output.GlobalOutput("Calculated {} integrals\n".format(n))
        bp.output.GlobalOutput(str(eri.GetBuf()))
        bp.output.GlobalOutput("\n")
      


    except Exception as e:
      bp.output.GlobalOutput("Caught exception in main handler\n")
      traceback.print_exc()



bp.Init(sys.argv, out = "stdout", color = True, debug = True)

with bp.ModuleAdministrator() as mm:
    Run(mm)

bp.Finalize()
