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

        tester = bp.testing.Tester("Testing molecule / atomic data")
        tester.PrintHeader()

        for i in range(1, 20):
            bp.output.GlobalOutput("Z = {}\n".format(i))
            bp.output.GlobalOutput("      Name: {}\n".format(bp.system.AtomicNameFromZ(i)))
            bp.output.GlobalOutput("    Symbol: {}\n".format(bp.system.AtomicSymFromZ(i)))
            bp.output.GlobalOutput("      Mass: {}\n".format(bp.system.AtomicMassFromZ(i)))
            ad = bp.system.AtomicInfoFromZ(i)

            bp.output.GlobalOutput("  Isotopes: {}\n".format(len(ad.isotopes)))

            for iso in ad.isotopes:
                bp.output.GlobalOutput("        Number: {}\n".format(iso.isonum))
                bp.output.GlobalOutput("                Mass: {}\n".format(iso.mass))
                bp.output.GlobalOutput("           Abundance: {}\n".format(iso.abund))
                bp.output.GlobalOutput("\n")

            bp.output.GlobalOutput("\n")




        tester.PrintResults() 


    except Exception as e:
      bp.output.GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.GlobalError("\n")
      bp.output.GlobalError(str(e))
      bp.output.GlobalError("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
