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
        mm.LoadModule("Methods",               "BPTest",                   "SCF")
        mm.LoadModule("Methods",               "CoreGuess",                "IGUESS")
        mm.LoadModule("Methods",               "HFIterate",                "SCFITER")
        mm.LoadModule("SystemIntegrals",       "NuclearRepulsion",         "NUC_REP")
        mm.LoadModule("OneElectronIntegrals",  "Overlap",                  "AO_OVERLAP")
        mm.LoadModule("OneElectronIntegrals",  "CoreBuild",                "AO_COREBUILD")
        mm.LoadModule("OneElectronIntegrals",  "KineticEnergy",            "AO_KINETIC")
        mm.LoadModule("OneElectronIntegrals",  "OneElectronPotential",     "AO_NUCEL")
        mm.LoadModule("TwoElectronIntegrals",  "ReferenceERI",             "AO_ERI")

        # Set the OneElectronPotential module to use the atom grid (ie, nuclear-electron attraction)
        mm.ChangeOption("AO_NUCEL", "grid", "ATOMS")

        # SCF Iterations settings
        mm.ChangeOption("SCFITER",  "KEY_AO_COREBUILD",    "AO_COREBUILD")
        mm.ChangeOption("SCFITER",  "KEY_AO_OVERLAP",      "AO_OVERLAP")
        mm.ChangeOption("SCFITER",  "KEY_AO_ERI",          "AO_ERI")
        mm.ChangeOption("SCFITER",  "KEY_NUC_REPULSION",   "NUC_REP")


        # Tell the core builder which modules to use
        mm.ChangeOption("AO_COREBUILD", "KEY_AO_KINETIC", "AO_KINETIC")
        mm.ChangeOption("AO_COREBUILD", "KEY_AO_NUCATT",  "AO_NUCEL")

        # Tell the core guess which modules to use
        mm.ChangeOption("IGUESS", "KEY_NUC_REPULSION", "NUC_REP")
        mm.ChangeOption("IGUESS", "KEY_AO_OVERLAP",    "AO_OVERLAP")
        mm.ChangeOption("IGUESS", "KEY_AO_COREBUILD",  "AO_COREBUILD")

        # Tell the SCF which modules to use
        mm.ChangeOption("SCF", "KEY_INITIAL_GUESS", "IGUESS")
        mm.ChangeOption("SCF", "KEY_SCF_ITERATOR",  "SCFITER")



        mm.Print(out)
        mm.SanityCheck()
  


        atoms = list(water2)
        u = AtomSetUniverse()
        for a in atoms:
            u.Insert(a)
        s = System(u, True)

        s = ApplySingleBasis("Primary","sto-3g",s)

         
        nr = mm.GetModule("SCF", 0)
        nr.EnableDebug(True)
        iwfn = Wavefunction()
        iwfn.system = s

        a = nr.Energy(iwfn)

        tester = Tester("Testing simple energy calculation")
        tester.PrintHeader()


    except Exception as e:
      GlobalOutput("Caught exception in main handler\n")
      traceback.print_exc()



psr.Init(sys.argv, out = "stdout", color = True, debug = True)

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.Finalize()