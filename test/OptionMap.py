#!/usr/bin/env python3


############################
# Testing of ModuleInfo
############################

import os
import sys
import argparse
import copy
import traceback 

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, "/home/ben/programming/ambit/install/lib")

import bpmodule as bp


def Run():
    try:
        bp.testing.PrintHeader("Testing conversions from Python to C++")

        #                  name             key,          value                   valid
        testelements = [ ( "int",           ( "int_1",     int(5)                ),  True ),
                         ( "int",           ( "int_2",     int(-5)               ),  True ),
                         ( "float",         ( "float_1",   float(5.0)            ),  True ),
                         ( "bool",          ( "bool_1",    True                  ),  True ),
                         ( "bool",          ( "bool_2",    False                 ),  True ),
                         ( "string",        ( "string_1",  "Hi"                  ),  True ),
                         ( "string",        ( "string_2",  ""                    ),  True ),
                         ( "dict",          ( "dict_1",    {"key" : int(5)}      ),  False),
                         ( "tuple",         ( "tuple_1",   ("key", int(5))       ),  False),
                         ( "set",           ( "set_1",     {"key", int(5)}       ),  False),
                         ( "list<int>",     ( "list_i",    [ 5, 10, 15 ]         ),  True),
                         ( "list<float>",   ( "list_f",    [ 5.0, 10.0, 15.0 ]   ),  True),
                         ( "list<string>",  ( "list_s",    [ "5", "10", "Hi" ]   ),  True),
                         ( "list<list>",    ( "list_l",    [ [ 5, 10, 15 ] ]     ),  False),
                         ( "list<dict>",    ( "list_d",    [ {"key" : 5 } ]      ),  False),
                         ( "list<tuple>",   ( "list_t",    [ ("key" , 5 ) ]      ),  False),
                         ( "list<set>",     ( "list_set",  [ {"key" , 5 } ]      ),  False),
                         ( "list<empty>",   ( "list_0",    [  ]                  ),  False),
                         ( "list<hetero>",  ( "list_1",    [ 5, 10.1 ]           ),  False),
                         ( "list<hetero>",  ( "list_2",    [ 5, "Hi" ]           ),  False),
                         ( "list<hetero>",  ( "list_3",    [ "Hi", 5 ]           ),  False),
                       ]

        nfailed = 0
        ntest = 1


        # Test single elements
        for d in testelements:
            s = "Test construction with {}".format(d[0])
            expected = d[2]
            lst = [ d[1] ]
            nfailed += bp.testing.TestOptionMap(ntest, s, expected, lst)
            ntest += 1

        # Test two elements
        # including testing for duplicate keys
        for d1 in testelements:
            for d2 in testelements:
                s = "Test construction with {} + {}".format(d1[0], d2[0])
                expected = (d1[2] and d2[2]) and (d1 != d2)
                lst = [ d1[1], d2[1] ]
                nfailed += bp.testing.TestOptionMap(ntest, s, expected, lst)
                ntest += 1
       
        # Test three elements
        # including testing for duplicate keys
        for d1 in testelements:
            for d2 in testelements:
                for d3 in testelements:
                    s = "Test construction with {} + {} + {}".format(d1[0], d2[0], d3[0])
                    expected = (d1[2] and d2[2] and d3[2]) and (d1 != d2 and d2 != d3 and d1 != d3)
                    lst = [ d1[1], d2[1], d3[1] ]
                    nfailed += bp.testing.TestOptionMap(ntest, s, expected, lst)
                    ntest += 1

        bp.testing.PrintResults(nfailed) 


    except Exception as e:
      bp.output.Output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
