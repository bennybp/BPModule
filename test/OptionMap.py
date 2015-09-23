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

        #                  key               type                default,               required, Validator,          valid?
        testelements = [ ( "int_1",          "int",              int(5)                ,  False,  None,               True ),
                         ( "int_2",          "int",              int(-5)               ,  False,  None,               True ),
                         ( "float_1",        "float",            float(5.0)            ,  False,  None,               True ),
                         ( "bool_1",         "bool",             True                  ,  False,  None,               True ),
                         ( "bool_2",         "bool",             False                 ,  False,  None,               True ),
                         ( "string_1",       "str",              "Hi"                  ,  False,  None,               True ),
                         ( "string_2",       "str",              ""                    ,  False,  None,               True ),
                         ( "dict_1",         "dict",             {"key" : int(5)}      ,  False,  None,               False),
                         ( "tuple_1",        "tuple",            ("key", int(5))       ,  False,  None,               False),
                         ( "set_1",          "set",              {"key", int(5)}       ,  False,  None,               False),
                         ( "list_i",         "listint",          [ 5, 10, 15 ]         ,  False,  None,               True ),
                         ( "list_f",         "listfloat",        [ 5.0, 10.0, 15.0 ]   ,  False,  None,               True ),
                         ( "list_s",         "liststr",          [ "5", "10", "Hi" ]   ,  False,  None,               True ),
                         ( "list_l",         "listlist",         [ [ 5, 10, 15 ] ]     ,  False,  None,               False),
                         ( "list_d",         "listdict",         [ {"key" : 5 } ]      ,  False,  None,               False),
                         ( "list_t",         "listtuple",        [ ("key" , 5 ) ]      ,  False,  None,               False),
                         ( "list_set",       "listset",          [ {"key" , 5 } ]      ,  False,  None,               False),
                         ( "list_0",         "listempty",        [  ]                  ,  False,  None,               False),
                         ( "list_1",         "listhetero",       [ 5, 10.1 ]           ,  False,  None,               False),
                         ( "list_2",         "listhetero",       [ 5, "Hi" ]           ,  False,  None,               False),
                         ( "list_3",         "listhetero",       [ "Hi", 5 ]           ,  False,  None,               False),
                       ]

        nfailed = 0
        ntest = 1


        # Test single elements
        for d1 in testelements:
            s = "Test construction with {}".format(d1[0])
            expected = d1[5]
            opt = { d1[0] : ( d1[1], d1[2], d1[3], d1[4], "(no help)" ) }
            nfailed += bp.testing.TestOptionMap(ntest, s, expected, opt)
            ntest += 1

        # Test two elements
        # Duplicate keys aren't tested since the python dict
        # has unique keys
        for d1 in testelements:
            for d2 in testelements:
                s = "Test construction with {} + {}".format(d1[0], d2[0])
                expected = (d1[5] and d2[5])
                opt = { d1[0] : ( d1[1], d1[2], d1[3], d1[4], "(no help)" ),  
                        d2[0] : ( d2[1], d2[2], d2[3], d2[4], "(no help)" ) }
                nfailed += bp.testing.TestOptionMap(ntest, s, expected, opt)
                ntest += 1
      
        # Test three elements
        # Duplicate keys aren't tested since the python dict
        # has unique keys
        for d1 in testelements:
            for d2 in testelements:
                for d3 in testelements:
                    s = "Test construction with {} + {}".format(d1[0], d2[0])
                    expected = (d1[5] and d2[5] and d3[5])
                    opt = { d1[0] : ( d1[1], d1[2], d1[3], d1[4], "(no help)" ),  
                            d2[0] : ( d2[1], d2[2], d2[3], d2[4], "(no help)" ),
                            d3[0] : ( d3[1], d3[2], d3[3], d2[4], "(no help)" ) }
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
