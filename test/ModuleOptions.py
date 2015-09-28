#!/usr/bin/env python3


import os
import sys
import argparse
import traceback

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, "/home/ben/programming/ambit/install/lib")

import bpmodule as bp


def IsValid(t, d):
    if t == d:
        return True

    if t == "float" and d == "int":  # cast int to float
        return True

    if t == "listfloat" and d == "listint":  # cast int to float
        return True

    if d == "listempty":  # Empty list passed to a list type
      return t in [ "listint", "listfloat", "listbool", "liststr" ]

    return False




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

        validtypes = ["int", "float", "bool", "str", "listint", "listfloat", "listbool", "liststr" ]


        #                  type                value
        testelements = [ ( "int",              int(5)                  ),
                       ( "int",              int(-5)                 ),
                       ( "float",            float(5.0)              ),
                       ( "bool",             True                    ),
                       ( "bool",             False                   ),
                       ( "str",              "Hi"                    ),
                       ( "str",              ""                      ),
                       ( "dict",             {"key" : int(5)}        ),
                       ( "tuple",            ("key", int(5))         ),
                       ( "set",              {"key", int(5)}         ),
                       ( "listint",          [ 5, 10, 15 ]           ),
                       ( "listfloat",        [ 5.0, 10.0, 15.0 ]     ),
                       ( "listbool",         [ False, False, True ]  ),
                       ( "liststr",          [ "5", "10", "Hi" ]     ),
                       ( "listlist",         [ [ 5, 10, 15 ] ]       ),
                       ( "listdict",         [ {"key" : 5 } ]        ),
                       ( "listtuple",        [ ("key" , 5 ) ]        ),
                       ( "listset",          [ {"key" , 5 } ]        ),
                       ( "listempty",        [  ]                    ),
                       ( "listhetero",       [ 5, 10.1, [ 4 ]]       ),
                       ( "listhetero",       [ 5, "Hi" ]             ),
                       ( "listhetero",       [ "Hi", 5 ]             ),
                     ]
                   



        for m in allmod:
            bp.output.Output("Got module key %|1$-20| %|2$-20| v%3%\n", m[0].Key(), m[0].Name(), m[0].Version())


        bp.testing.Output("\n\n")
        bp.testing.PrintHeader("Testing construction of OptionMap objects")
        nfailed = 0
        ntest = 1

        for m in allmod:
            # None should be valid - I haven't specified required options
            desc = "Testing validity of {}".format(m[0].Key())
            nfailed += bp.testing.PyTestBoolFunc(ntest, desc, False, m[0].Options().IsValid)
            ntest += 1

        for m in allmod:
            for d in testelements:
                if d[0] == m[1]:
                    desc = "Setting required option for {}".format(m[0].Key())
                    opt = d[0] + "_req"
                    nfailed += bp.testing.PyTestFunc(ntest, desc, True, m[0].Options().Change, opt, d[1]) 
                    ntest += 1

        for m in allmod:
            # Should be valid now
            desc = "Retesting validity of {}".format(m[0].Key())
            nfailed += bp.testing.PyTestBoolFunc(ntest, desc, True, m[0].Options().IsValid)
            ntest += 1


        for m in allmod:
            for d in testelements:
                for o in "_opt", "_req", "_opt_def":
                    opt = m[1] + o
                    expected = IsValid(m[1], d[0])
                    desc = "Setting {} option for {} -> {}".format(opt, m[0].Key(), d[0])
                    nfailed += bp.testing.PyTestFunc(ntest, desc, expected, m[0].Options().Change, opt, d[1]) 
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
