#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr


def Run(mm):

    psr.output.GlobalOutput("\n\n----------------------------------------\n")
    isrun = psr.util.Memwatch_running()
    psr.output.GlobalOutput("Memwatch is running? {}\n".format(isrun))

    mem = psr.util.Memwatch_allocated()
    psr.output.GlobalOutput("Allocated memory so far: {}\n".format(mem))

    maxmem = psr.util.Memwatch_getlimit()
    psr.output.GlobalOutput("Maximum memory to be used: {}\n".format(maxmem))


    psr.util.Memwatch_setlimit(10485760)
    maxmem = psr.util.Memwatch_getlimit()
    psr.output.GlobalOutput("Maximum memory to be used: {}\n".format(maxmem))

    psr.output.GlobalOutput("----------------------------------------\n\n")



psr.Init(sys.argv, out = "stdout", color = True, debug = True)

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.Finalize()
