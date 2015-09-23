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
        bp.testing.PrintHeader("Testing construction of OptionMap objects with required options")

        #                  key               type                default,               required, Validator,          valid?
        testelements = [ ( "int_1",          "int",              None                  ,  True ,  None,               True ),
                         ( "int_2",          "int",              None                  ,  True ,  None,               True ),
                         ( "float_1",        "float",            None                  ,  True ,  None,               True ),
                         ( "bool_1",         "bool",             None                  ,  True ,  None,               True ),
                         ( "bool_2",         "bool",             None                  ,  True ,  None,               True ),
                         ( "string_1",       "str",              None                  ,  True ,  None,               True ),
                         ( "string_2",       "str",              None                  ,  True ,  None,               True ),
                         ( "dict_1",         "dict",             None                  ,  True ,  None,               False),
                         ( "tuple_1",        "tuple",            None                  ,  True ,  None,               False),
                         ( "set_1",          "set",              None                  ,  True ,  None,               False),
                         ( "list_i",         "listint",          None                  ,  True ,  None,               True ),
                         ( "list_f",         "listfloat",        None                  ,  True ,  None,               True ),
                         ( "list_s",         "liststr",          None                  ,  True ,  None,               True ),
                         ( "list_l",         "listlist",         None                  ,  True ,  None,               False),
                         ( "list_d",         "listdict",         None                  ,  True ,  None,               False),
                         ( "list_t",         "listtuple",        None                  ,  True ,  None,               False),
                         ( "list_set",       "listset",          None                  ,  True ,  None,               False),
                         ( "list_0",         "listempty",        None                  ,  True ,  None,               False),
                         ( "list_1",         "listhetero",       None                  ,  True ,  None,               False),
                         ( "list_2",         "listhetero",       None                  ,  True ,  None,               False),
                         ( "list_3",         "listhetero",       None                  ,  True ,  None,               False),
                         ( "def_int_1",      "int",              int(5)                ,  True ,  None,               False),
                         ( "def_int_2",      "int",              int(-5)               ,  True ,  None,               False),
                         ( "def_float_1",    "float",            float(5.0)            ,  True ,  None,               False),
                         ( "def_bool_1",     "bool",             True                  ,  True ,  None,               False),
                         ( "def_bool_2",     "bool",             False                 ,  True ,  None,               False),
                         ( "def_string_1",   "str",              "Hi"                  ,  True ,  None,               False),
                         ( "def_string_2",   "str",              ""                    ,  True ,  None,               False),
                         ( "def_dict_1",     "dict",             {"key" : int(5)}      ,  True ,  None,               False),
                         ( "def_tuple_1",    "tuple",            ("key", int(5))       ,  True ,  None,               False),
                         ( "def_set_1",      "set",              {"key", int(5)}       ,  True ,  None,               False),
                         ( "def_list_i",     "listint",          [ 5, 10, 15 ]         ,  True ,  None,               False),
                         ( "def_list_f",     "listfloat",        [ 5.0, 10.0, 15.0 ]   ,  True ,  None,               False),
                         ( "def_list_s",     "liststr",          [ "5", "10", "Hi" ]   ,  True ,  None,               False),
                         ( "def_list_l",     "listlist",         [ [ 5, 10, 15 ] ]     ,  True ,  None,               False),
                         ( "def_list_d",     "listdict",         [ {"key" : 5 } ]      ,  True ,  None,               False),
                         ( "def_list_t",     "listtuple",        [ ("key" , 5 ) ]      ,  True ,  None,               False),
                         ( "def_list_set",   "listset",          [ {"key" , 5 } ]      ,  True ,  None,               False),
                         ( "def_list_0",     "listempty",        [  ]                  ,  True ,  None,               False),
                         ( "def_list_1",     "listhetero",       [ 5, 10.1 ]           ,  True ,  None,               False),
                         ( "def_list_2",     "listhetero",       [ 5, "Hi" ]           ,  True ,  None,               False),
                         ( "def_list_3",     "listhetero",       [ "Hi", 5 ]           ,  True ,  None,               False),

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
