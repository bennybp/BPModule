#!/usr/bin/env python3

import os
import sys
import argparse
import traceback 

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
bspath = os.path.join(os.path.dirname(thispath), "basis")
sys.path.insert(0, bppath)

import bpmodule as bp


def Run(mm):
    try:
        mm.LoadModule("MBE","MBE","MBE")
        # bunch more of these

        MyMod=mm.GetModule("MBE",0)
        wfn=bp.datastore.Wavefunction()
        molu = bp.system.AtomSetUniverse()
        molu.append(bp.system.CreateAtom(0, [ 0.000000000000, -0.143225816552,  0.000000000000], 8))
        molu.append(bp.system.CreateAtom(1, [ 1.638036840407,  1.136548822547, -0.000000000000], 1))
        molu.append(bp.system.CreateAtom(2, [-1.638036840407,  1.136548822547, -0.000000000000], 1))
        mol = bp.system.Molecule(molu, True)
        bstype = bp.basisset.ShellType.Gaussian
        molwithbs = bp.basisset.SimpleCreator(os.path.join(bspath, "sto-3g.gbs"), mol, bstype, "primary");
        bs = molwithbs.GetBasisSet("primary")

        wfn.system.Set(molwithbs)
        MyMod.SetWfn(wfn)
        
        print(MyMod.Deriv(1))
      
    except Exception as e:
      bp.output.Output("Caught exception in main handler\n")
      traceback.print_exc()



bp.Init(sys.argv, out = "stdout", color = True, debug = True)

with bp.ModuleAdministrator() as mm:
    Run(mm)

bp.Finalize()
