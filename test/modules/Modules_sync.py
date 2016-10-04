#!/usr/bin/env python3


import os
import sys
import argparse
import traceback
import time

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "../", "modules")
sys.path.insert(0, psrpath)

import pulsar as psr


def Run(mm):
    try:
      out = psr.output.get_global_output()

      # Load the python modules
      #             supermodule      module name      key
      mm.load_module("TestModules",   "TestModule1",   "TESTMOD1")
      mm.load_module("TestModules",   "TestPyModule1", "TESTPYMOD1")

      mm.sanity_check()


      b1 = mm.get_module("TESTMOD1", 0)
      b2 = mm.get_module("TESTPYMOD1", 0)

      mm.start_cache_sync(10)

      mr = str(myrank)
      mr2 = str( (myrank+1) % nproc)
      b1.add_to_cache("TestCache1_" + mr, 0);
      b2.add_to_cache("TestCache1_Py_" + mr, 0);

      pol = psr.datastore.CacheData.DistributeGlobal
      b1.add_to_cache("TestDist1_" + mr, pol);
      b2.add_to_cache("TestDist1_Py_" + mr, pol);

      #time.sleep(1)
      b1.get_from_cache("TestCache1_" + mr, False);
      b2.get_from_cache("TestCache1_Py_" + mr, False);
      b1.get_from_cache("TestDist1_" + mr, False);
      b2.get_from_cache("TestDist1_Py_" + mr, False);

      # Get stuff from distributed cache
      b1.get_from_cache("TestDist1_" + mr2, False);
      b1.get_from_cache("TestDist1_" + mr2, True);

      #b2.get_from_cache("TestDist1_Py_" + mr2);

      #mm.print(out)

      mm.stop_cache_sync()

      psr.output.print_global_output("\nDone testing\n")


    except Exception as e:
      psr.output.print_global_output("Caught exception in main handler\n")
      traceback.print_exc()
      psr.output.print_global_error("\n")
      psr.output.print_global_error(str(e))
      psr.output.print_global_error("\n")



psr.initialize(sys.argv, color = True, debug = True,
               outbase = "psr_output")

myrank = psr.parallel.get_proc_id()
nproc = psr.parallel.get_nproc()
psr.print_global_output("My rank: {}\n".format(myrank))

cpio_global = psr.modulemanager.BDBCheckpointIO("/tmp/TestChk_global.dat")
cpio_local = psr.modulemanager.BDBCheckpointIO("/tmp/TestChk_local.dat.{}".format(myrank))
cp = psr.modulemanager.Checkpoint(cpio_local, cpio_global)

with psr.ModuleAdministrator() as mm:
    cp.load_local_cache(mm)
    cp.load_global_cache(mm)
    Run(mm)

psr.finalize()
