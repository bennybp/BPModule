#!/usr/bin/env python3


import os
import sys
import traceback 


# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr
from pulsar.output import *
from pulsar.testing import *
from pulsar.datastore import *

from helper.TestOptions import opttypemap, validvals

def Run():
    try:
        tester = Tester("Testing Datastore - serialization")
        tester.print_header()

        opt = OptionMap()
        tester.test("Serialization of empty OptionMap", True,
                    TestSerialization_OptionMap, opt)

        for k,v in validvals:
            # with default, not required
            opttype = opttypemap[k]
            opt.add_option(k, opttype, False, None, "No Help", v)

        tester.test("Serialization of full OptionMap", True,
                    TestSerialization_OptionMap, opt)


        opt = OptionMap()
        for k,v in validvals:
            # no default, required
            opttype = opttypemap[k]
            opt.add_option(k, opttype, True, None, "No Help", None)

        tester.test("Serialization of full OptionMap", True,
                    TestSerialization_OptionMap, opt)

        opt = OptionMap()
        for k,v in validvals:
            # no default, not required
            opttype = opttypemap[k]
            opt.add_option(k, opttype, False, None, "No Help", None)
        tester.test("Serialization of full OptionMap", True,
                    TestSerialization_OptionMap, opt)

        tester.print_results() 


    except Exception as e:
      print_global_output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      print_global_error("\n")
      print_global_error(str(e))
      print_global_error("\n")




psr.initialize(sys.argv, color = True, debug = True)
Run()
psr.finalize()
