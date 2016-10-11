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

        tester = psr.testing.Tester("Testing molecule / atomic data")
        tester.PrintHeader()

        for i in range(1, 20):
            psr.output.GlobalOutput("Z = {}\n".format(i))
            psr.output.GlobalOutput("      Name: {}\n".format(psr.system.AtomicNameFromZ(i)))
            psr.output.GlobalOutput("    Symbol: {}\n".format(psr.system.AtomicSymFromZ(i)))
            psr.output.GlobalOutput("      Mass: {}\n".format(psr.system.AtomicMassFromZ(i)))
            ad = psr.system.AtomicInfoFromZ(i)

            psr.output.GlobalOutput("  Isotopes: {}\n".format(len(ad.isotopes)))

            for iso in ad.isotopes:
                psr.output.GlobalOutput("        Number: {}\n".format(iso.isonum))
                psr.output.GlobalOutput("                Mass: {}\n".format(iso.mass))
                psr.output.GlobalOutput("           Abundance: {}\n".format(iso.abund))
                psr.output.GlobalOutput("\n")

            psr.output.GlobalOutput("\n")




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
