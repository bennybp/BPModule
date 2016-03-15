#!/usr/bin/env python3

import os
import sys
from  importlib.machinery import SourceFileLoader

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

from bpmodule import GlobalOutput CheckSupermodule
from bpmodule.exception import PyGeneralException


GlobalOutputSetOut_Stdout()
GlobalOutputSetDebug(True)
GlobalOutputSetColor(True)
paths = sys.argv[1:]
output.GlobalOutput("\n")

allok = True
for supermodule in paths:

    try:
        # will raise exception with a problem
        CheckSupermodule(supermodule)
        GlobalOutputSuccess("    Successfully checked supermodule {}\n\n".format(supermodule))

    except PyGeneralException as e:
        output.GlobalError("Checking of supermodule {} failed. See above for errors\n\n".format(supermodule))
        allok = False
    

output.GlobalOutput("\n\n")    
if allok:
    GlobalOutputSuccess("All supermodules seem ok\n")
else:
    output.GlobalError("***Problem detected. Fix it!***\n")
output.GlobalOutput("\n\n")

