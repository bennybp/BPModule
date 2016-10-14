#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr


def Run():
    try:

        tester = psr.testing.Tester("Testing comparison of OptionMap objects")
        tester.PrintHeader()


        # create a list of dicts to test
        opt1 = { "test_opt1" : ( "int",   5,             False, None, "(no help)" ),
                 "test_opt2" : ( "str",   "SomeString",  False, None, "(no help)" ),
               }
        opt2 = { "test_opt1" : ( "int",   5,             False, None, "(no help)" ),
                 "test_opt2" : ( "str",   "SomeString",  False, None, "(no help)" ),
               }


        # Construct with some valid data
        opm1 = psr.datastore.OptionMap("nomodule", opt1, None)
        opm2 = psr.datastore.OptionMap("nomodule", opt2, None)
        s = "Testing equality"
        same = True
        tester.Test(s, True, psr.testing.PyTestBoolFunc, opm1.Compare, opm1)
        tester.Test(s, True, psr.testing.PyTestBoolFunc, opm2.Compare, opm2)
        tester.Test(s, same, psr.testing.PyTestBoolFunc, opm1.Compare, opm2)
        tester.Test(s, same, psr.testing.PyTestBoolFunc, opm2.Compare, opm1)


        opt2 = { "test_opt1" : ( "int",   10,            False, None, "(no help)" ),
                 "test_opt2" : ( "str",   "SomeString",  False, None, "(no help)" ),
               }

        opm1 = psr.datastore.OptionMap("nomodule", opt1, None)
        opm2 = psr.datastore.OptionMap("nomodule", opt2, None)
        same = False
        tester.Test(s, True, psr.testing.PyTestBoolFunc, opm1.Compare, opm1)
        tester.Test(s, True, psr.testing.PyTestBoolFunc, opm2.Compare, opm2)
        tester.Test(s, same, psr.testing.PyTestBoolFunc, opm1.Compare, opm2)
        tester.Test(s, same, psr.testing.PyTestBoolFunc, opm2.Compare, opm1)

        
        tester.Test(s, True, psr.testing.PyTestBoolFunc, opm1.CompareSelect, opm1, ["test_opt1"])
        tester.Test(s, True, psr.testing.PyTestBoolFunc, opm1.CompareSelect, opm1, ["test_opt2"])
        tester.Test(s, True, psr.testing.PyTestBoolFunc, opm2.CompareSelect, opm2, ["test_opt1"])
        tester.Test(s, True, psr.testing.PyTestBoolFunc, opm2.CompareSelect, opm2, ["test_opt2"])

        tester.Test(s, same, psr.testing.PyTestBoolFunc, opm1.CompareSelect, opm2, ["test_opt1"])
        tester.Test(s, same, psr.testing.PyTestBoolFunc, opm2.CompareSelect, opm1, ["test_opt1"])
        tester.Test(s, True, psr.testing.PyTestBoolFunc, opm1.CompareSelect, opm2, ["test_opt2"])
        tester.Test(s, True, psr.testing.PyTestBoolFunc, opm2.CompareSelect, opm1, ["test_opt2"])


        tester.PrintResults() 


    except Exception as e:
      psr.output.GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      psr.output.GlobalError("\n")
      psr.output.GlobalError(str(e))
      psr.output.GlobalError("\n")




psr.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.Finalize()
