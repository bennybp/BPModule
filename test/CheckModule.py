#!/usr/bin/env python3

import os
import sys
from  importlib.machinery import SourceFileLoader

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")

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
        output.Error("Checking of supermodule %1% failed. See above for errors\n")
        allok = False
    

output.Output("\n\n")    
if allok:
    output.Success("All supermodules seem ok\n\n")
else:
    output.Error("***Problem detected. Fix it!***\n\n")

