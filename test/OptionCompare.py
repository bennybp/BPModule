#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp


def Run():
    try:

        tester = bp.testing.Tester("Testing comparison of OptionMap objects")
        tester.PrintHeader()


        # create a list of dicts to test
        opt1 = { "test_opt1" : ( "int",   5,             False, None, "(no help)" ),
                 "test_opt2" : ( "str",   "SomeString",  False, None, "(no help)" ),
               }
        opt2 = { "test_opt1" : ( "int",   5,             False, None, "(no help)" ),
                 "test_opt2" : ( "str",   "SomeString",  False, None, "(no help)" ),
               }


        # Construct with some valid data
        opm1 = bp.datastore.OptionMap("nomodule", opt1, None)
        opm2 = bp.datastore.OptionMap("nomodule", opt2, None)
        s = "Testing equality"
        same = True
        tester.Test(s, True, bp.testing.PyTestBoolFunc, opm1.Compare, opm1)
        tester.Test(s, True, bp.testing.PyTestBoolFunc, opm2.Compare, opm2)
        tester.Test(s, same, bp.testing.PyTestBoolFunc, opm1.Compare, opm2)
        tester.Test(s, same, bp.testing.PyTestBoolFunc, opm2.Compare, opm1)


        opt2 = { "test_opt1" : ( "int",   10,            False, None, "(no help)" ),
                 "test_opt2" : ( "str",   "SomeString",  False, None, "(no help)" ),
               }

        opm1 = bp.datastore.OptionMap("nomodule", opt1, None)
        opm2 = bp.datastore.OptionMap("nomodule", opt2, None)
        same = False
        tester.Test(s, True, bp.testing.PyTestBoolFunc, opm1.Compare, opm1)
        tester.Test(s, True, bp.testing.PyTestBoolFunc, opm2.Compare, opm2)
        tester.Test(s, same, bp.testing.PyTestBoolFunc, opm1.Compare, opm2)
        tester.Test(s, same, bp.testing.PyTestBoolFunc, opm2.Compare, opm1)

        
        tester.Test(s, True, bp.testing.PyTestBoolFunc, opm1.CompareSelect, opm1, ["test_opt1"])
        tester.Test(s, True, bp.testing.PyTestBoolFunc, opm1.CompareSelect, opm1, ["test_opt2"])
        tester.Test(s, True, bp.testing.PyTestBoolFunc, opm2.CompareSelect, opm2, ["test_opt1"])
        tester.Test(s, True, bp.testing.PyTestBoolFunc, opm2.CompareSelect, opm2, ["test_opt2"])

        tester.Test(s, same, bp.testing.PyTestBoolFunc, opm1.CompareSelect, opm2, ["test_opt1"])
        tester.Test(s, same, bp.testing.PyTestBoolFunc, opm2.CompareSelect, opm1, ["test_opt1"])
        tester.Test(s, True, bp.testing.PyTestBoolFunc, opm1.CompareSelect, opm2, ["test_opt2"])
        tester.Test(s, True, bp.testing.PyTestBoolFunc, opm2.CompareSelect, opm1, ["test_opt2"])


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
