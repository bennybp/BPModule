#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, "/home/ben/programming/ambit/install/lib")

import bpmodule as bp


def PyTestGet(mod, key):
    mod.GetOption(key) 




def Run():
    try:

          # Load the python modules
          #             supermodule       module name       key
          bp.LoadModule("test_options",   "test_int",     "TEST_INT")
          bp.LoadModule("test_options",   "test_float",   "TEST_FLOAT")
          bp.LoadModule("test_options",   "test_bool",    "TEST_BOOL")
          bp.LoadModule("test_options",   "test_str",     "TEST_STR")

          bp.LoadModule("test_options",   "test_listint",     "TEST_LIST_INT")
          bp.LoadModule("test_options",   "test_listfloat",   "TEST_LIST_FLOAT")
          bp.LoadModule("test_options",   "test_listbool",    "TEST_LIST_BOOL")
          bp.LoadModule("test_options",   "test_liststr",     "TEST_LIST_STR")

          mod_int   = ( bp.mst.GetModule_Test("TEST_INT"),    "int"   )
          mod_float = ( bp.mst.GetModule_Test("TEST_FLOAT"),  "float" )
          mod_bool  = ( bp.mst.GetModule_Test("TEST_BOOL"),   "bool"  )
          mod_str   = ( bp.mst.GetModule_Test("TEST_STR"),    "str"   )

          mod_listint   = ( bp.mst.GetModule_Test("TEST_LIST_INT"),   "listint"   )
          mod_listfloat = ( bp.mst.GetModule_Test("TEST_LIST_FLOAT"), "listfloat" )
          mod_listbool  = ( bp.mst.GetModule_Test("TEST_LIST_BOOL"),  "listbool"  )
          mod_liststr   = ( bp.mst.GetModule_Test("TEST_LIST_STR"),   "liststr"   )

          allmod = [ mod_int, mod_float, mod_bool, mod_str,
                     mod_listint, mod_listfloat, mod_listbool, mod_liststr ]

          for m in allmod:
              bp.output.Output("Got module key %|1$-20| %|2$-20| v%3%\n", m[0].Key(), m[0].Name(), m[0].Version())


          bp.testing.Output("\n\n")
          bp.testing.PrintHeader("Testing construction of OptionMap objects")
          nfailed = 0
          ntest = 1

          for m in allmod:
              # None should be valid - I haven't specified required options
              desc = "Testing validity of {}".format(m[0].Key())
              nfailed += bp.testing.PyTestBoolFunc(ntest, desc, False, m[0].OptionsValid)              
              ntest += 1

          mod_int[0].ChangeOption(        "int_req",          5               )
          mod_float[0].ChangeOption(      "float_req",        5.0             )
          mod_bool[0].ChangeOption(       "bool_req",         False           )
          mod_str[0].ChangeOption(        "str_req",         "Hi"             )
          mod_listint[0].ChangeOption(    "listint_req",      [5, 10]         )
          mod_listfloat[0].ChangeOption(  "listfloat_req",    [5.0, 10]       )
          mod_listbool[0].ChangeOption(   "listbool_req",     [False, True]   )
          mod_liststr[0].ChangeOption(    "liststr_req",      ["Hi", "There"] )
          
              
          for m in allmod:
              # Should be valid now
              desc = "Retesting validity of {}".format(m[0].Key())
              nfailed += bp.testing.PyTestBoolFunc(ntest, desc, True, m[0].OptionsValid)              
              ntest += 1


          bp.testing.PrintResults(nfailed) 



    except Exception as e:
          bp.output.Output("Caught exception in main handler\n")
          traceback.print_exc()
          bp.output.Error("\n")
          bp.output.Error(str(e))
          bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
