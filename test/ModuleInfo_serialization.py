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
from pulsar.modulemanager import *

from helper.TestOptions import opttypemap, allvals

def Run():
    try:
        tester = Tester("Testing ModuleInfo - serialization")
        tester.PrintHeader()

        mi = ModuleInfo()
        tester.Test("Serialization of empty ModuleInfo", True,
                    TestSerialization_ModuleInfo, mi)

        mi.name = "Some name"
        tester.Test("Serialization of ModuleInfo - name", True, TestSerialization_ModuleInfo, mi)

        mi.type = "Some type"
        tester.Test("Serialization of ModuleInfo - type", True, TestSerialization_ModuleInfo, mi)

        mi.base = "Some base"
        tester.Test("Serialization of ModuleInfo - base", True, TestSerialization_ModuleInfo, mi)

        mi.path = "Some path"
        tester.Test("Serialization of ModuleInfo - path", True, TestSerialization_ModuleInfo, mi)

        mi.version = "Some version"
        tester.Test("Serialization of ModuleInfo - version", True, TestSerialization_ModuleInfo, mi)

        mi.description = "Some description"
        tester.Test("Serialization of ModuleInfo - description", True, TestSerialization_ModuleInfo, mi)

        mi.authors = ["Author 1", "Author 2"]
        tester.Test("Serialization of ModuleInfo - authors", True, TestSerialization_ModuleInfo, mi)

        mi.authors = ["Ref 1", "Ref 2"]
        tester.Test("Serialization of ModuleInfo - refs", True, TestSerialization_ModuleInfo, mi)
        

        for k,v in allvals:
            opttype = opttypemap[k]
            if not mi.options.HasKey(k): # Can't add duplicate keys
                mi.options.AddOption(k, opttype, True, None, "No help", None)

        tester.Test("Serialization of ModuleInfo - options", True, TestSerialization_ModuleInfo, mi)


        tester.PrintResults() 


    except Exception as e:
      GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      GlobalError("\n")
      GlobalError(str(e))
      GlobalError("\n")




psr.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.Finalize()
