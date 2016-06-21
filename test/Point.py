#!/usr/bin/env python3

import os
import sys
import traceback 
import random

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr
from pulsar.output import *
from pulsar.testing import *
from pulsar.math import *


def Run():
    try:
        tester = Tester("Testing Math class - serialization")
        tester.print_header()

        ####################################
        # Serialization of Points
        ####################################
        pts = []
        for i in range(0, 15):
            pts.append(Point(random.uniform(-1.0e4, 1e4), random.uniform(-1.0e4, 1e4), random.uniform(-1.0e4, 1e4)))

        for i in range(0, len(pts)):
            p = pts[i]
            tester.test("Point serialization - {}".format(i), True,
                        TestSerialization_Point, p)


        tester.print_results() 


    except Exception as e:
      print_global_output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      print_global_error("\n")
      print_global_error(str(e))
      print_global_error("\n")




psr.initialize(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.finalize()
