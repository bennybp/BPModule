#!/usr/bin/env python3


import os
import sys
import traceback 

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "../", "modules")
parent = os.path.join(os.path.dirname(thispath))
sys.path.insert(0, psrpath)
sys.path.insert(0, parent)

from pulsar.system import *
from pulsar.datastore import *
from pulsar.output import *

from helper.TestAtoms import water


def Run(mm):
    try:
        out = GetGlobalOut()

        # Load the python modules
        #             supermodule      module name      key
        mm.LoadModule("LibERD",        "LibERD_ERI",   "ERI")
        mm.Print(out)
        mm.SanityCheck()
  
        atoms = list(water)
        u = AtomSetUniverse()
        for a in atoms:
            u.Insert(a)
        s = System(u, True)

        s = ApplySingleBasis("Primary","sto-3g",s)

         
        eri = mm.GetModule("ERI", 0)
        eri.EnableDebug(True)
        iwfn = Wavefunction()
        iwfn.system = s
        eri.SetInitialWfn(iwfn)
        eri.SetBases("Primary", "Primary", "Primary", "Primary")
        n = eri.Calculate(1, 0, 0, 0, 0)
        GlobalOutput("Calculated {} integrals\n".format(n))
        GlobalOutput(str(eri.GetBuf()))
        GlobalOutput("\n")
      


    except Exception as e:
      GlobalOutput("Caught exception in main handler\n")
      traceback.print_exc()



psr.Init(sys.argv, out = "stdout", color = True, debug = True)

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.Finalize()
