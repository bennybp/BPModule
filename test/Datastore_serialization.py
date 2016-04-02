#!/usr/bin/env python3


import os
import sys
import traceback 


# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp
from bpmodule.output import *
from bpmodule.testing import *
from bpmodule.datastore import *

from helper.TestOptions import opttypemap, validvals

def Run():
    try:
        tester = Tester("Testing Datastore - serialization")
        tester.PrintHeader()

        opt = OptionMap()
        tester.Test("Serialization of empty OptionMap", True,
                    TestSerialization_OptionMap, opt)

        for k,v in validvals:
            # with default, not required
            opttype = opttypemap[k]
            opt.AddOption(k, opttype, False, None, "No Help", v)

        tester.Test("Serialization of full OptionMap", True,
                    TestSerialization_OptionMap, opt)


        opt = OptionMap()
        for k,v in validvals:
            # no default, required
            opttype = opttypemap[k]
            opt.AddOption(k, opttype, True, None, "No Help", None)

        tester.Test("Serialization of full OptionMap", True,
                    TestSerialization_OptionMap, opt)

        opt = OptionMap()
        for k,v in validvals:
            # no default, not required
            opttype = opttypemap[k]
            opt.AddOption(k, opttype, False, None, "No Help", None)
        tester.Test("Serialization of full OptionMap", True,
                    TestSerialization_OptionMap, opt)

        tester.PrintResults() 


    except Exception as e:
      GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      GlobalError("\n")
      GlobalError(str(e))
      GlobalError("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
