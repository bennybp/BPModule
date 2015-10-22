#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")

import bpmodule as bp



def Run():
    try:

        tester = bp.testing.Tester("Testing validation of options")
        tester.PrintHeader()

        tests = [
                  ( "int range [0,100]", "int",      bp.options.RangeCheck(0, 100, includemin=True,  includemax=True),  [5, 10, 15, 0, 100], [-1, 101, 1000, -1000] ),
                  ( "int range (0_100]", "int",      bp.options.RangeCheck(0, 100, includemin=False, includemax=True),  [5, 10 ,15, 100],    [-1, 101, 1000, -1000, 0] ),
                  ( "int range [0_100)", "int",      bp.options.RangeCheck(0, 100, includemin=True,  includemax=False), [5, 10, 15, 0],      [-1, 101, 1000, -1000, 100] ),
                  ( "int range (0,100)", "int",      bp.options.RangeCheck(0, 100, includemin=False, includemax=False), [5, 10, 15],         [-1, 101, 1000, -1000, 0, 100] ),
                  ( "float range [0,100]", "float",  bp.options.RangeCheck(0, 100, includemin=True,  includemax=True),  [1.0e-20, 5.0, 10.0, 15.0, 99.999, 0.0, 100.0], [-1.0, -1e-20, 101.0, 100.0001, 1000.0, -1000.0] ),
                  ( "float range (0_100]", "float",  bp.options.RangeCheck(0, 100, includemin=False, includemax=True),  [1.0e-20, 5.0, 10.0, 15.0, 99.999, 100.0],      [-1.0, -1e-20, 101.0, 100.0001, 1000.0, -1000.0, 0.0] ),
                  ( "float range [0_100)", "float",  bp.options.RangeCheck(0, 100, includemin=True,  includemax=False), [1.0e-20, 5.0, 10.0, 15.0, 99.999, 0.0],        [-1.0, -1e-20, 101.0, 100.0001, 1000.0, -1000.0, 100.0] ),
                  ( "float range (0,100)", "float",  bp.options.RangeCheck(0, 100, includemin=False, includemax=False), [1.0e-20, 5.0, 10.0, 15.0, 99.999],             [-1.0, -1e-20, 101.0, 100.0001, 1000.0, -1000.0, 0.0, 100.0] ),
                  ( "int inlist 1", "int",           bp.options.InList([ -5, 5, 10, 15, 20, 25 ]),  [ -5, 5, 10, 15, 20, 25 ], [-4, 4, 9, 14, 19, 26, 29] ),
                  ( "float inlist 1", "float",       bp.options.InList([ -5.0, 5.0, 10.0, 15.0, 20.0, 25.0 ]),  [ -5.0, 5.0, 10.0, 15.0, 20.0, 25.0 ], [-4.0, 4.0, 9.0, 14.0, 19.0, 26.0, 29.0] ),
                  ( "str inlist 1", "str",           bp.options.InList([ "str1", "Str2", "sTr3" ]),  [ "str1", "Str2", "sTr3", "STR1", "STR2", "STR3", "str2", "str3"], ["no", "str4", "str5"] ),
                ]


        # Test validator with default values
        for v in tests:
    
            for valid in v[3]:
                s = "Construction with \"{}\" -> {}".format(v[0], valid)
                opt = { "test_opt" : ( v[1], valid, False, v[2], "(no help)" ) } 
                tester.Test(s, True, bp.testing.PyTestFunc, bp.options.OptionMap, opt, None)

            for invalid in v[4]:
                s = "Construction with \"{}\" -> {}".format(v[0], invalid)
                opt = { "test_opt" : ( v[1], invalid, False, v[2], "(no help)" ) } 
                tester.Test(s, False, bp.testing.PyTestFunc, bp.options.OptionMap, opt, None)

        # Test validator with changing values
        for v in tests:
    
            for val in v[3]:
                opt = { "test_opt" : ( v[1], val, False, v[2], "(no help)" ) } 
                opm = bp.options.OptionMap(opt, None)
                # test for validity?

                for valid in v[3]:
                    s = "Changing -> {}".format(valid)
                    opt = { "test_opt" : valid }
                    tester.Test(s, True, bp.testing.PyTestFunc, opm.Change, "test_opt", valid)
                
                for invalid in v[4]:
                    s = "Changing -> {}".format(invalid)
                    opt = { "test_opt" : ( v[1], invalid, False, v[2], "(no help)" ) } 
                    tester.Test(s, False, bp.testing.PyTestFunc, opm.Change, "test_opt", invalid)

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
