#!/usr/bin/env python3


import os
import sys
import traceback
import array

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "../", "modules")
parent = os.path.join(os.path.dirname(thispath))
sys.path.insert(0, psrpath)
sys.path.insert(0, parent)

from pulsar.testing import *
from pulsar.system import *
from pulsar.datastore import *
from pulsar.output import *

from helper.TestAtoms import water2

def CompareList(lst1, lst2, tol):
  if len(lst1) != len(lst2):
    return False

  else:
    for i in range(0, len(lst1)):
        if abs(lst1[i]-lst2[i]) > tol:
            return False

  return True

def Run(mm):
    try:

        out = GetGlobalOut()

        # Load the python modules
        #             supermodule              module name    key
        mm.LoadModule("OneElectronIntegrals",  "OneElectronPotential",     "AO_NUCEL")
        mm.ChangeOption("AO_NUCEL", "grid", "ATOMS")
        mm.Print(out)
        mm.SanityCheck()
  
        atoms = list(water2)
        u = AtomSetUniverse()
        for a in atoms:
            u.Insert(a)
        s = System(u, True)

        s = ApplySingleBasis("Primary","sto-3g",s)
        wfn = Wavefunction()
        wfn.system = s
        bs = s.GetBasisSet("Primary")

         
        nr = mm.GetModule("AO_NUCEL", 0)
        nr.EnableDebug(True)
        nr.Initialize(0, wfn, bs, bs)

        tester = Tester("Testing Nuclear Electron Attraction integrals")
        tester.PrintHeader()

        outbuf = array.array('d', [0]*64)
        n = nr.Calculate(0, 0, outbuf) 
        print("Calculated {} integrals".format(n))
        print(outbuf[:n])

        #tester.TestValue("H2O Nuclear repulsion", True, CompareList(ref_h2o, list(outbuf[:n]), 1e-14))

        tester.PrintResults() 


    except Exception as e:
      GlobalOutput("Caught exception in main handler\n")
      traceback.print_exc()



psr.Init(sys.argv, out = "stdout", color = True, debug = True)

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.Finalize()
