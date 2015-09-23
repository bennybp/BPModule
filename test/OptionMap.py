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
        bp.testing.PrintHeader("Testing construction of OptionMap objects")

        validtypes = ["int", "float", "bool", "str", "listint", "listfloat", "listbool", "liststr" ]

        #                  key               type                default,              
        testelements = [ ( "int_1",          "int",              int(5)                  ),
                         ( "int_2",          "int",              int(-5)                 ),
                         ( "float_1",        "float",            float(5.0)              ),
                         ( "bool_1",         "bool",             True                    ),
                         ( "bool_2",         "bool",             False                   ),
                         ( "string_1",       "str",              "Hi"                    ),
                         ( "string_2",       "str",              ""                      ),
                         ( "dict_1",         "dict",             {"key" : int(5)}        ),
                         ( "tuple_1",        "tuple",            ("key", int(5))         ),
                         ( "set_1",          "set",              {"key", int(5)}         ),
                         ( "list_i",         "listint",          [ 5, 10, 15 ]           ),
                         ( "list_f",         "listfloat",        [ 5.0, 10.0, 15.0 ]     ),
                         ( "list_b",         "listbool",         [ False, False, True ]  ),
                         ( "list_s",         "liststr",          [ "5", "10", "Hi" ]     ),
                         ( "list_l",         "listlist",         [ [ 5, 10, 15 ] ]       ),
                         ( "list_d",         "listdict",         [ {"key" : 5 } ]        ),
                         ( "list_t",         "listtuple",        [ ("key" , 5 ) ]        ),
                         ( "list_set",       "listset",          [ {"key" , 5 } ]        ),
                         ( "list_0",         "listempty",        [  ]                    ),
                         ( "list_1",         "listhetero",       [ 5, 10.1 ]             ),
                         ( "list_2",         "listhetero",       [ 5, "Hi" ]             ),
                         ( "list_3",         "listhetero",       [ "Hi", 5 ]             ),
                       ]

        nfailed = 0
        ntest = 1


        # Test single element, no validator, not required, with default
        for d1 in testelements:
            s = "Test construction with {}".format(d1[0])
            expected = (d1[1] in validtypes)
            opt = { d1[0] : ( d1[1], d1[2], False, None, "(no help)" ) }
            nfailed += bp.testing.TestOptionMap(ntest, s, expected, opt)
            ntest += 1


        # Test two elements, no validator, not required, with default
        for d1 in testelements:
            for d2 in testelements:
                s = "Test construction with {} + {}".format(d1[0], d2[0])
                expected = (d1[1] in validtypes) and (d2[1] in validtypes)
                opt = { d1[0] : ( d1[1], d1[2], False, None, "(no help)" ),  
                        d2[0] : ( d2[1], d2[2], False, None, "(no help)" ) }
                nfailed += bp.testing.TestOptionMap(ntest, s, expected, opt)
                ntest += 1


        # Test single element, no validator, not required, no default
        for d1 in testelements:
            s = "Test construction with {}".format(d1[0])
            expected = (d1[1] in validtypes)
            opt = { d1[0] : ( d1[1], None, False, None, "(no help)" ) }
            nfailed += bp.testing.TestOptionMap(ntest, s, expected, opt)
            ntest += 1


        # Test two elements, no validator, not required, no default
        for d1 in testelements:
            for d2 in testelements:
                s = "Test construction with {} + {}".format(d1[0], d2[0])
                expected = (d1[1] in validtypes) and (d2[1] in validtypes)
                opt = { d1[0] : ( d1[1], None, False, None, "(no help)" ),  
                        d2[0] : ( d2[1], None, False, None, "(no help)" ) }
                nfailed += bp.testing.TestOptionMap(ntest, s, expected, opt)
                ntest += 1


      
        # Test single element, no validator, required, but with default specified
        # (should always be false)
        for d1 in testelements:
            s = "Test construction with {}".format(d1[0])
            expected = False
            opt = { d1[0] : ( d1[1], d1[2], True, None, "(no help)" ) }
            nfailed += bp.testing.TestOptionMap(ntest, s, expected, opt)
            ntest += 1


        # Test two elements, no validator, required, but with default specified
        # (should always be false)
        for d1 in testelements:
            for d2 in testelements:
                s = "Test construction with {} + {}".format(d1[0], d2[0])
                expected = False
                opt = { d1[0] : ( d1[1], d1[2], True, None, "(no help)" ),  
                        d2[0] : ( d2[1], d2[2], True, None, "(no help)" ) }
                nfailed += bp.testing.TestOptionMap(ntest, s, expected, opt)
                ntest += 1


        # Test single element, no validator, required, no default specified
        for d1 in testelements:
            s = "Test construction with {}".format(d1[0])
            expected = (d1[1] in validtypes)
            opt = { d1[0] : ( d1[1], None, True, None, "(no help)" ) }
            nfailed += bp.testing.TestOptionMap(ntest, s, expected, opt)
            ntest += 1


        # Test two elements, no validator, required, no default specified
        for d1 in testelements:
            for d2 in testelements:
                s = "Test construction with {} + {}".format(d1[0], d2[0])
                expected = (d1[1] in validtypes) and (d2[1] in validtypes)
                opt = { d1[0] : ( d1[1], None, True, None, "(no help)" ),  
                        d2[0] : ( d2[1], None, True, None, "(no help)" ) }
                nfailed += bp.testing.TestOptionMap(ntest, s, expected, opt)
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
