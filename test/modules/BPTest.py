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
from pulsar.modulemanager.ModuleTreePrinters import PrintDotTree

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
        mm.LoadModule("Methods",               "DIIS",                     "SCF_DIIS")
        mm.LoadModule("Methods",               "Damping",                  "SCF_DAMPING")
        mm.LoadModule("Methods",               "CoreGuess",                "IGUESS")
        mm.LoadModule("Methods",               "HFIterate",                "SCFITER")
        mm.LoadModule("Methods",               "BasicFockBuild",           "FOCKBUILD")
        mm.LoadModule("SystemIntegrals",       "NuclearRepulsion",         "NUC_REP")
        mm.LoadModule("SystemIntegrals",       "NuclearDipole",            "NUC_DIP")
        mm.LoadModule("OneElectronIntegrals",  "Overlap",                  "AO_OVERLAP")
        mm.LoadModule("OneElectronIntegrals",  "CoreBuild",                "AO_COREBUILD")
        mm.LoadModule("OneElectronIntegrals",  "KineticEnergy",            "AO_KINETIC")
        mm.LoadModule("OneElectronIntegrals",  "OneElectronPotential",     "AO_NUCEL")
        mm.LoadModule("SimintERI",            "SimintERI",                "AO_ERI")
        #mm.LoadModule("TwoElectronIntegrals",  "ReferenceERI",                "AO_ERI")

        mm.LoadModule("OneElectronIntegrals",  "Dipole",                   "AO_DIPOLE")
        mm.LoadModule("OneElectronIntegrals",  "OneElectronProperty",      "PROP_DIPOLE")


        # Set the OneElectronPotential module to use the atom grid (ie, nuclear-electron attraction)
        mm.ChangeOption("AO_NUCEL", "grid", "ATOMS")

        # SCF Iterations settings
        mm.ChangeOption("SCFITER",  "KEY_AO_OVERLAP",      "AO_OVERLAP")

        # Fock builder
        mm.ChangeOption("FOCKBUILD",  "KEY_AO_OVERLAP",    "AO_OVERLAP")
        mm.ChangeOption("FOCKBUILD",  "KEY_AO_COREBUILD",  "AO_COREBUILD")
        mm.ChangeOption("FOCKBUILD",  "KEY_AO_ERI",        "AO_ERI")


        # Tell the core builder which modules to use
        mm.ChangeOption("AO_COREBUILD", "KEY_AO_KINETIC", "AO_KINETIC")
        mm.ChangeOption("AO_COREBUILD", "KEY_AO_NUCATT",  "AO_NUCEL")

        # Tell the core guess which modules to use
        mm.ChangeOption("IGUESS", "KEY_NUC_REPULSION", "NUC_REP")
        mm.ChangeOption("IGUESS", "KEY_AO_OVERLAP",    "AO_OVERLAP")
        mm.ChangeOption("IGUESS", "KEY_AO_COREBUILD",  "AO_COREBUILD")

        # Tell the SCF which modules to use
        mm.ChangeOption("SCF_DIIS", "KEY_INITIAL_GUESS", "IGUESS")
        mm.ChangeOption("SCF_DIIS", "KEY_SCF_ITERATOR",  "SCFITER")
        mm.ChangeOption("SCF_DIIS", "KEY_FOCK_BUILDER",  "FOCKBUILD")
        mm.ChangeOption("SCF_DIIS", "KEY_AO_COREBUILD",  "AO_COREBUILD")
        mm.ChangeOption("SCF_DIIS", "KEY_NUC_REPULSION", "NUC_REP")
        mm.ChangeOption("SCF_DIIS", "KEY_AO_OVERLAP",    "AO_OVERLAP")
        mm.ChangeOption("SCF_DIIS", "MAX_ITER", 1000)
        mm.ChangeOption("SCF_DIIS", "E_TOLERANCE", 1e-10)
        mm.ChangeOption("SCF_DIIS", "DENS_TOLERANCE", 1e-8)

        mm.ChangeOption("SCF_DAMPING", "KEY_INITIAL_GUESS", "IGUESS")
        mm.ChangeOption("SCF_DAMPING", "KEY_SCF_ITERATOR",  "SCFITER")
        mm.ChangeOption("SCF_DAMPING", "KEY_FOCK_BUILDER",  "FOCKBUILD")
        mm.ChangeOption("SCF_DAMPING", "KEY_AO_COREBUILD",  "AO_COREBUILD")
        mm.ChangeOption("SCF_DAMPING", "KEY_NUC_REPULSION", "NUC_REP")
        mm.ChangeOption("SCF_DAMPING", "MAX_ITER", 1000)
        mm.ChangeOption("SCF_DAMPING", "E_TOLERANCE", 1e-10)
        mm.ChangeOption("SCF_DAMPING", "DENS_TOLERANCE", 1e-8)

        # Dipole calculator
        mm.ChangeOption("PROP_DIPOLE", "KEY_ONEEL_MOD", "AO_DIPOLE");

        mm.Print(out)
        #mm.SanityCheck()
  


        atoms = list(water2)
        u = AtomSetUniverse()
        for a in atoms:
            u.Insert(a)
        s = System(u, True)

        s = ApplySingleBasis("Primary","sto-3g",s)

         
        nr = mm.GetModule("SCF_DIIS", 0)
        nr.EnableDebug(True)
        iwfn = Wavefunction()
        iwfn.system = s

        ret = nr.Energy(iwfn)
        newwfn,nrg = ret

        ndip = mm.GetModule("NUC_DIP", 0)
        dipmod = mm.GetModule("PROP_DIPOLE", 0)

        bs = s.GetBasisSet("Primary")
        dip = dipmod.Calculate(newwfn, bs, bs)

        nucdip = array.array('d', [0]*3)
        n = ndip.Calculate(0, newwfn.system, nucdip)

        print("Final energy: {}".format(nrg))
        print("Electronic dipole moment: {} {} {}".format(dip[0], dip[1], dip[2]))
        print("Nuclear dipole moment: {} {} {}".format(nucdip[0], nucdip[1], nucdip[2]))
        print("Total dipole moment: {} {} {}".format(dip[0] + nucdip[0], dip[1] + nucdip[1], dip[2] + nucdip[2]))


        tester = Tester("Testing simple energy calculation")
        tester.PrintHeader()


    except Exception as e:
      GlobalOutput("Caught exception in main handler\n")
      traceback.print_exc()



psr.Init(sys.argv, out = "stdout", color = True, debug = True)

with psr.ModuleAdministrator() as mm:
    Run(mm)

dotout = PrintDotTree(mm)
with open("module_tree.dot", 'w') as f:
  f.write(dotout)

psr.Finalize()
