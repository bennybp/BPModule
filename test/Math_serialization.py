#!/usr/bin/env python3


import os
import sys
import random
import traceback 

from helper.SetOperations import TestSetOperations
from helper.TestAtoms import nonane

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp
from bpmodule.math import StringSetUniverse, StringSet


def Run():
    try:
        out = bp.output.GetGlobalOut()

        tester = bp.testing.Tester("Testing Math class - serialization")
        tester.PrintHeader()

        ####################################
        # Serialization of Points
        ####################################
        pts = []
        for i in range(0, 15):
            pts.append(bp.math.Point(random.uniform(-1.0e4, 1e4), random.uniform(-1.0e4, 1e4), random.uniform(-1.0e4, 1e4)))

        for i in range(0, len(pts)):
            p = pts[i]
            tester.Test("Point serialization - {}".format(i), True,
                        bp.math.TestSerialization_Point, p)


        ###########################################
        # Serialization of Universe and MathSet
        ###########################################
        Alphabet="abcdefghijklmnopqrstuvwxyz"

        u0 = StringSetUniverse()
        u1 = StringSetUniverse()

        # Make a universe of the alphabet
        for i in list(Alphabet):
           u1.Insert(i)

        s0 = StringSet(u1, False)
        s1 = StringSet(u1, False)
        s2 = StringSet(u1, True)
        s1.Insert("a")
        s1.Insert("b")
        s1.Insert("c")

        tester.Test("Universe serialization - u0", True, bp.math.TestSerialization_StringSetUniverse, u0)
        tester.Test("Universe serialization - u1", True, bp.math.TestSerialization_StringSetUniverse, u1)
        tester.Test("MathSet serialization - s0", True, bp.math.TestSerialization_StringSet, s0)
        tester.Test("MathSet serialization - s1", True, bp.math.TestSerialization_StringSet, s1)
        tester.Test("MathSet serialization - s2", True, bp.math.TestSerialization_StringSet, s2)


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
