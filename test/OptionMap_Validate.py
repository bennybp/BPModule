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

        tester = bp.testing.Tester("Testing validation of options")
        tester.PrintHeader()


        # Test range validator
        s = "Construction with RangeCheck"
        opt = { "test_opt" : ( "int", 105, False, bp.options.RangeCheck(0, 100), "(no help)" ) } 
        tester.Test(s, True, bp.testing.PyTestFunc, bp.options.OptionMap, opt)

        #
        #for d1 in testelements:
        #    s = "Test construction with {}".format(d1[0])
        #    expected = (d1[0] in validtypes)
        #    opt = { d1[0] : ( d1[0], d1[1], False, None, "(no help)" ) }
        #    tester.Test(s, expected, bp.testing.PyTestFunc, bp.options.OptionMap, opt)


        tester.PrintResults() 


    except Exception as e:
      bp.output.Output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
