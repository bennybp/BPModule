#!/usr/bin/env python3

import os
import sys
from  importlib.machinery import SourceFileLoader

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

from bpmodule import output, CheckSupermodule
from bpmodule.exception import PyGeneralException


output.SetOut_Stdout()
output.SetDebug(True)
output.SetColor(True)
paths = sys.argv[1:]
output.Output("\n")

allok = True
for supermodule in paths:

    try:
        # will raise exception with a problem
        CheckSupermodule(supermodule)
        output.Success("    Successfully checked supermodule %1%\n\n", supermodule)

    except PyGeneralException as e:
        output.Error("Checking of supermodule %1% failed. See above for errors\n\n", supermodule)
        allok = False
    

output.Output("\n\n")    
if allok:
    output.Success("All supermodules seem ok\n")
else:
    output.Error("***Problem detected. Fix it!***\n")
output.Output("\n\n")

