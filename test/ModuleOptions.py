#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, "/home/ben/programming/ambit/install/lib")

import bpmodule as bp


def Run():
    try:
      # Load the python modules
      #             supermodule       module name       key
      bp.LoadModule("test_options",   "test_int",     "TEST_INT")
      bp.LoadModule("test_options",   "test_float",   "TEST_FLOAT")
      bp.LoadModule("test_options",   "test_bool",    "TEST_BOOL")
      bp.LoadModule("test_options",   "test_str",     "TEST_STR")

      bp.LoadModule("test_options",   "test_list_int",     "TEST_LIST_INT")
      bp.LoadModule("test_options",   "test_list_float",   "TEST_LIST_FLOAT")
      bp.LoadModule("test_options",   "test_list_bool",    "TEST_LIST_BOOL")
      bp.LoadModule("test_options",   "test_list_str",     "TEST_LIST_STR")

      mod_int   = bp.mst.GetModule_Test("TEST_INT")
      mod_float = bp.mst.GetModule_Test("TEST_FLOAT")
      mod_bool  = bp.mst.GetModule_Test("TEST_BOOL")
      mod_str   = bp.mst.GetModule_Test("TEST_STR")

      mod_list_int   = bp.mst.GetModule_Test("TEST_LIST_INT")
      mod_list_float = bp.mst.GetModule_Test("TEST_LIST_FLOAT")
      mod_list_bool  = bp.mst.GetModule_Test("TEST_LIST_BOOL")
      mod_list_str   = bp.mst.GetModule_Test("TEST_LIST_STR")

      allmod = [ mod_int, mod_float, mod_bool, mod_str,
                 mod_list_int, mod_list_float, mod_list_bool, mod_list_str ]

      for m in allmod:
          bp.output.Output("Got module key %|1$-20| %|2$-20| v%3%\n", m.Key(), m.Name(), m.Version())


      mod_int.ChangeOption("int_opt_def", "Hi")

      bp.output.Output("\nDone testing\n")


    except Exception as e:
      bp.output.Output("Caught exception in main handler\n")
      traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
