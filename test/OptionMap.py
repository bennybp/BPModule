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
        bp.testing.PrintHeader("Testing construction of OptionMap objects")

        validtypes = ["int", "float", "bool", "str", "listint", "listfloat", "listbool", "liststr" ]

        #                  type                default,              
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

        alltypes = sorted(set( x[0] for x in testelements ))


        nfailed = 0
        ntest = 1


        # Test single element, no validator, not required, with default
        for d1 in testelements:
            s = "Test construction with {}".format(d1[0])
            expected = (d1[0] in validtypes)
            opt = { d1[0] : ( d1[0], d1[1], False, None, "(no help)" ) }
            nfailed += bp.testing.PyTestFunc(ntest, s, expected, bp.datastore.OptionMap, opt)
            ntest += 1



        # Test two elements, no validator, not required, with default
        for d1 in testelements:
            for d2 in testelements:
                s = "Test construction with {} + {}".format(d1[0], d2[0])
                expected = (d1[0] in validtypes) and (d2[0] in validtypes)
                opt = { d1[0] : ( d1[0], d1[1], False, None, "(no help)" ),  
                        d2[0] : ( d2[0], d2[1], False, None, "(no help)" ) }
                nfailed += bp.testing.PyTestFunc(ntest, s, expected, bp.datastore.OptionMap, opt)
                ntest += 1



        # Test single element, no validator, not required, no default
        for d1 in testelements:
            s = "Test construction with {}".format(d1[0])
            expected = (d1[0] in validtypes)
            opt = { d1[0] : ( d1[0], None, False, None, "(no help)" ) }
            nfailed += bp.testing.PyTestFunc(ntest, s, expected, bp.datastore.OptionMap, opt)
            ntest += 1



        # Test two elements, no validator, not required, no default
        for d1 in testelements:
            for d2 in testelements:
                s = "Test construction with {} + {}".format(d1[0], d2[0])
                expected = (d1[0] in validtypes) and (d2[0] in validtypes)
                opt = { d1[0] : ( d1[0], None, False, None, "(no help)" ),  
                        d2[0] : ( d2[0], None, False, None, "(no help)" ) }
                nfailed += bp.testing.PyTestFunc(ntest, s, expected, bp.datastore.OptionMap, opt)
                ntest += 1


      
        # Test single element, no validator, required, but with default specified
        # (should always be false)
        for d1 in testelements:
            s = "Test construction with {}".format(d1[0])
            expected = False
            opt = { d1[0] : ( d1[0], d1[1], True, None, "(no help)" ) }
            nfailed += bp.testing.PyTestFunc(ntest, s, expected, bp.datastore.OptionMap, opt)
            ntest += 1



        # Test two elements, no validator, required, but with default specified
        # (should always be false)
        for d1 in testelements:
            for d2 in testelements:
                s = "Test construction with {} + {}".format(d1[0], d2[0])
                expected = False
                opt = { d1[0] : ( d1[0], d1[1], True, None, "(no help)" ),  
                        d2[0] : ( d2[0], d2[1], True, None, "(no help)" ) }
                nfailed += bp.testing.PyTestFunc(ntest, s, expected, bp.datastore.OptionMap, opt)
                ntest += 1



        # Test single element, no validator, required, no default specified
        for d1 in testelements:
            s = "Test construction with {}".format(d1[0])
            expected = (d1[0] in validtypes)
            opt = { d1[0] : ( d1[0], None, True, None, "(no help)" ) }
            nfailed += bp.testing.PyTestFunc(ntest, s, expected, bp.datastore.OptionMap, opt)
            ntest += 1



        # Test two elements, no validator, required, no default specified
        for d1 in testelements:
            for d2 in testelements:
                s = "Test construction with {} + {}".format(d1[0], d2[0])
                expected = (d1[0] in validtypes) and (d2[0] in validtypes)
                opt = { d1[0] : ( d1[0], None, True, None, "(no help)" ),  
                        d2[0] : ( d2[0], None, True, None, "(no help)" ) }
                nfailed += bp.testing.PyTestFunc(ntest, s, expected, bp.datastore.OptionMap, opt)
                ntest += 1



        # Test single element, with varying types, not required
        for t1 in alltypes:
            for d1 in testelements:
                s = "Test construction with {} -> {}".format(t1, d1[0])
                expected = (t1 in validtypes) and (IsValid(t1, d1[0]))
                opt = { d1[0] : ( t1, d1[1], False, None, "(no help)" ) }  
                nfailed += bp.testing.PyTestFunc(ntest, s, expected, bp.datastore.OptionMap, opt)
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
