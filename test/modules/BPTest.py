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
from pulsar.modulemanager.ModuleTreePrinters import print_dot_tree

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

        out = get_global_output()

        # Load the python modules
        #             supermodule              module name    key
        mm.load_module("Methods",    "DIIS",                     "SCF_DIIS")
        mm.load_module("Methods",    "Damping",                  "SCF_DAMPING")
        mm.load_module("Methods",    "CoreGuess",                "IGUESS")
        mm.load_module("Methods",    "HFIterate",                "SCFITER")
        mm.load_module("Methods",    "BasicFockBuild",           "FOCKBUILD")
        mm.load_module("Integrals",  "NuclearRepulsion",         "NUC_REP")
        mm.load_module("Integrals",  "NuclearDipole",            "NUC_DIP")
        mm.load_module("Integrals",  "OSOverlap",                "AO_OVERLAP")
        mm.load_module("Integrals",  "OneElectronIntegralSum",   "AO_COREBUILD")
        mm.load_module("Integrals",  "OSKineticEnergy",          "AO_KINETIC")
        mm.load_module("Integrals",  "OSOneElectronPotential",   "AO_NUCEL")
        mm.load_module("Integrals",  "OneElectron_Eigen",        "ONEEL_CACHE")
        mm.load_module("SimintERI",  "SimintERI",                "AO_ERI")
        #mm.load_module("TwoElectronIntegrals",  "ReferenceERI",                "AO_ERI")


        mm.load_module("Integrals",  "OSDipole",                 "AO_DIPOLE")
        mm.load_module("Integrals",  "OneElectronProperty",      "PROP_DIPOLE")

        mm.change_option("ONEEL_CACHE", "CACHE_RESULTS", True)
        # Set the OneElectronPotential module to use the atom grid (ie, nuclear-electron attraction)
        mm.change_option("AO_NUCEL", "grid", "ATOMS")

        # SCF Iterations settings
        mm.change_option("SCFITER",  "KEY_AO_OVERLAP",      "AO_OVERLAP")
        mm.change_option("SCFITER",  "KEY_ONEEL_MAT",       "ONEEL_CACHE")

        # Fock builder
        mm.change_option("FOCKBUILD",  "KEY_AO_OVERLAP",    "AO_OVERLAP")
        mm.change_option("FOCKBUILD",  "KEY_AO_COREBUILD",  "AO_COREBUILD")
        mm.change_option("FOCKBUILD",  "KEY_AO_ERI",        "AO_ERI")
        mm.change_option("FOCKBUILD",  "KEY_ONEEL_MAT",     "ONEEL_CACHE")


        # Tell the core builder which modules to use
        mm.change_option("AO_COREBUILD", "KEY_AO_TERMS", [ "AO_KINETIC", "AO_NUCEL" ])

        # Tell the core guess which modules to use
        mm.change_option("IGUESS", "KEY_NUC_REPULSION", "NUC_REP")
        mm.change_option("IGUESS", "KEY_AO_OVERLAP",    "AO_OVERLAP")
        mm.change_option("IGUESS", "KEY_AO_COREBUILD",  "AO_COREBUILD")
        mm.change_option("IGUESS", "KEY_ONEEL_MAT",     "ONEEL_CACHE")

        # Tell the SCF which modules to use
        mm.change_option("SCF_DIIS", "KEY_INITIAL_GUESS", "IGUESS")
        mm.change_option("SCF_DIIS", "KEY_SCF_ITERATOR",  "SCFITER")
        mm.change_option("SCF_DIIS", "KEY_FOCK_BUILDER",  "FOCKBUILD")
        mm.change_option("SCF_DIIS", "KEY_AO_COREBUILD",  "AO_COREBUILD")
        mm.change_option("SCF_DIIS", "KEY_NUC_REPULSION", "NUC_REP")
        mm.change_option("SCF_DIIS", "KEY_AO_OVERLAP",    "AO_OVERLAP")
        mm.change_option("SCF_DIIS", "MAX_ITER", 1000)
        mm.change_option("SCF_DIIS", "EGY_TOLERANCE", 1e-10)
        mm.change_option("SCF_DIIS", "DENS_TOLERANCE", 1e-8)
        mm.change_option("SCF_DIIS", "KEY_ONEEL_MAT",     "ONEEL_CACHE")

        mm.change_option("SCF_DAMPING", "KEY_INITIAL_GUESS", "IGUESS")
        mm.change_option("SCF_DAMPING", "KEY_SCF_ITERATOR",  "SCFITER")
        mm.change_option("SCF_DAMPING", "KEY_FOCK_BUILDER",  "FOCKBUILD")
        mm.change_option("SCF_DAMPING", "KEY_AO_COREBUILD",  "AO_COREBUILD")
        mm.change_option("SCF_DAMPING", "KEY_NUC_REPULSION", "NUC_REP")
        mm.change_option("SCF_DAMPING", "MAX_ITER", 1000)
        mm.change_option("SCF_DAMPING", "EGY_TOLERANCE", 1e-10)
        mm.change_option("SCF_DAMPING", "DENS_TOLERANCE", 1e-8)
        mm.change_option("SCF_DAMPING", "KEY_ONEEL_MAT",     "ONEEL_CACHE")

        # Dipole calculator
        mm.change_option("PROP_DIPOLE", "KEY_ONEEL_MOD", "AO_DIPOLE");

        mm.enable_debug_all(True)
        mm.print(out)
        #mm.sanity_check()
  


        atoms = list(water2)
        u = AtomSetUniverse()
        for a in atoms:
            u.insert(a)
        s = System(u, True)

        s = apply_single_basis("Primary","sto-3g",s)

         
        nr = mm.get_module("SCF_DIIS", 0)
        iwfn = Wavefunction()
        iwfn.system = s

        ret = nr.energy(iwfn)
        newwfn,nrg = ret

        ndip = mm.get_module("NUC_DIP", 0)
        dipmod = mm.get_module("PROP_DIPOLE", 0)

        bs = s.get_basis_set("Primary")
        dip = dipmod.calculate(0, newwfn, bs, bs)

        nucdip = array.array('d', [0]*3)
        ndip.initialize(0, newwfn.system)
        n = ndip.calculate(nucdip)

        print("Final energy: {}".format(nrg))
        print("Electronic dipole moment: {} {} {}".format(dip[0], dip[1], dip[2]))
        print("Nuclear dipole moment: {} {} {}".format(nucdip[0], nucdip[1], nucdip[2]))
        print("Total dipole moment: {} {} {}".format(dip[0] + nucdip[0], dip[1] + nucdip[1], dip[2] + nucdip[2]))


        tester = Tester("Testing simple energy calculation")
        tester.print_header()


    except Exception as e:
      print_global_output("Caught exception in main handler\n")
      traceback.print_exc()



psr.initialize(sys.argv, out = "stdout", color = True, debug = True)

with psr.ModuleAdministrator() as mm:
    Run(mm)
mm.run_checkpoint(False)

#dotout = print_dot_tree(mm)
#with open("module_tree.dot", 'w') as f:
#  f.write(dotout)

psr.finalize()
