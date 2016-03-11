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

def ApplyBasis(syst,bsname,bslabel="primary",bstype=bp.system.ShellType.Gaussian):
    return bp.system.ApplySingleBasis(bstype,bslabel,bsname,syst)

def Run(mm):
    try:
        mm.LoadModule("MBE","MBE","MBE")

        MyMod=mm.GetModule("MBE",0)
        mol=bp.system.MakeSystem("""
        0 1
        O .000000000000 -0.143225816552  0.000000000000
        H 1.638036840407  1.136548822547 -0.000000000000
        H -1.638036840407,  1.136548822547, -0.000000000000
        """)
        mol = ApplyBasis(mol,"sto-3g")
        wfn=bp.datastore.Wavefunction()
        wfn.system.Set(mol)
        MyMod.SetWfn(wfn)
        
        print(MyMod.Deriv(1))
      
    except Exception as e:
      bp.output.Output("Caught exception in main handler\n")
      traceback.print_exc()



bp.Init(sys.argv, out = "stdout", color = True, debug = True)

with bp.ModuleAdministrator() as mm:
    Run(mm)

bp.Finalize()
