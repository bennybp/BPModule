#!/usr/bin/env python3


import os
import sys
import argparse
import traceback

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr


def IsValid(t, d):
    if t == d:
        return True

    if t == "float" and d == "int":  # cast int to float
        return False

    if t == "listfloat" and d == "listint":  # cast int to float
        return False

    if d == "listempty":  # Empty list passed to a list type
      return t in [ "listint", "listfloat", "listbool", "liststr" ]

    return False




def Run(mm):
    try:

        tester = psr.testing.Tester("Testing construction of Modules and Options")
        tester.PrintHeader()


        # Load the python modules
        #             supermodule       module name       key
        mm.LoadModule("TestModules",   "TestOptions_int",     "TEST_INT")
        mm.LoadModule("TestModules",   "TestOptions_float",   "TEST_FLOAT")
        mm.LoadModule("TestModules",   "TestOptions_bool",    "TEST_BOOL")
        mm.LoadModule("TestModules",   "TestOptions_str",     "TEST_STR")

        mm.LoadModule("TestModules",   "TestOptions_listint",     "TEST_LIST_INT")
        mm.LoadModule("TestModules",   "TestOptions_listfloat",   "TEST_LIST_FLOAT")
        mm.LoadModule("TestModules",   "TestOptions_listbool",    "TEST_LIST_BOOL")
        mm.LoadModule("TestModules",   "TestOptions_liststr",     "TEST_LIST_STR")

        mod_int   = ( mm.GetModule("TEST_INT", 0),    "int"   )
        mod_float = ( mm.GetModule("TEST_FLOAT", 0),  "float" )
        mod_bool  = ( mm.GetModule("TEST_BOOL", 0),   "bool"  )
        mod_str   = ( mm.GetModule("TEST_STR", 0),    "str"   )

        mod_listint   = ( mm.GetModule("TEST_LIST_INT", 0),   "listint"   )
        mod_listfloat = ( mm.GetModule("TEST_LIST_FLOAT", 0), "listfloat" )
        mod_listbool  = ( mm.GetModule("TEST_LIST_BOOL", 0),  "listbool"  )
        mod_liststr   = ( mm.GetModule("TEST_LIST_STR", 0),   "liststr"   )

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
            psr.output.GlobalOutput("Got module key {:<20} {:<20} v{}\n".format(m[0].Key(), m[0].Name(), m[0].Version()))


        nfailed = 0
        ntest = 1

        for m in allmod:
            # None should be valid - I haven't specified required options
            desc = "Testing validity of {}".format(m[0].Key())
            tester.Test(desc, False, psr.testing.PyTestBoolFunc, m[0].Options().AllReqSet)

        for m in allmod:
            for d in testelements:
                if d[0] == m[1]:
                    desc = "Setting required option for {}".format(m[0].Key())
                    opt = d[0] + "_req"
                    tester.Test(desc, True, psr.testing.PyTestFunc, m[0].Options().Change, opt, d[1]) 

        for m in allmod:
            # Should be valid now
            desc = "Retesting validity of {}".format(m[0].Key())
            tester.Test(desc, True, psr.testing.PyTestBoolFunc, m[0].Options().AllReqSet)


        for m in allmod:
            for d in testelements:
                for o in "_opt", "_req", "_opt_def":
                    opt = m[1] + o
                    expected = IsValid(m[1], d[0])
                    desc = "Setting {} option for {} -> {}".format(opt, m[0].Key(), d[0])
                    tester.Test(desc, expected, psr.testing.PyTestFunc, m[0].Options().Change, opt, d[1])

        tester.PrintResults()



    except Exception as e:
          psr.output.GlobalOutput("Caught exception in main handler\n")
          traceback.print_exc()
          psr.output.GlobalError("\n")
          psr.output.GlobalError(str(e))
          psr.output.GlobalError("\n")




psr.Init(sys.argv, out = "stdout", color = True, debug = True)

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.Finalize()
