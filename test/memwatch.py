#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp


def Run(mm):

    bp.output.Output("\n\n----------------------------------------\n")
    isrun = bp.util.Memwatch_running()
    bp.output.Output("Memwatch is running? %1%\n", isrun)

    mem = bp.util.Memwatch_allocated()
    bp.output.Output("Allocated memory so far: %1%\n", mem)

    maxmem = bp.util.Memwatch_getlimit()
    bp.output.Output("Maximum memory to be used: %1%\n", maxmem)


    bp.util.Memwatch_setlimit(10485760)
    maxmem = bp.util.Memwatch_getlimit()
    bp.output.Output("Maximum memory to be used: %1%\n", maxmem)

    bp.output.Output("----------------------------------------\n\n")



bp.Init(sys.argv, out = "stdout", color = True, debug = True)

with bp.ModuleAdministrator() as mm:
    Run(mm)

bp.Finalize()
