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

from helper.TestAtoms import water

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
        #             supermodule         module name           key
        mm.load_module("Integrals",  "NuclearRepulsion",   "NUC_REP")
        mm.print(out)
        mm.sanity_check()
  
        atoms = list(water)
        u = AtomSetUniverse()
        for a in atoms:
            u.insert(a)
        s = System(u, True)

        s = apply_single_basis("Primary","sto-3g",s)

         
        nr = mm.get_module("NUC_REP", 0)
        nr.enable_debug(True)

        tester = Tester("Testing basic System integrals")
        tester.print_header()

        ref_h2o = [ 8.80146556478736 ] 

        outbuf = array.array('d', [0]*64)
        nr.initialize(0, s)
        n = nr.calculate(outbuf) 

        tester.test_value("H2O Nuclear repulsion", True, CompareList(ref_h2o, list(outbuf[:n]), 1e-14))

        tester.print_results() 


    except Exception as e:
      print_global_output("Caught exception in main handler\n")
      traceback.print_exc()



psr.initialize(sys.argv, color = True, debug = True)

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.finalize()
