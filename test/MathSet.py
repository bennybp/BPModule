#!/usr/bin/env python3


import os
import sys
import argparse
import operator
import traceback

from helper.SetOperations import TestSetOperations

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp
from bpmodule.math import StringSetUniverse, StringSet



def SetElement(s, i, e):
    s[i] = e


def Run():
    try:
        Alphabet="abcdefghijklmnopqrstuvwxyz"
        BadAlphabet = "ABCDE"

        tester = bp.testing.Tester("Testing Universe and MathSet")
        tester.PrintHeader()

        u1 = StringSetUniverse()

        # Make a universe of the alphabet
        for i in list(Alphabet):
           u1.Insert(i)


        # Insert all
        s0 = StringSet(u1, False)
        s1 = StringSet(u1, False)
        s2 = StringSet(u1, True)
        s3 = StringSet(u1, False)
        s9 = StringSet(u1, False)

        for it in Alphabet[:7]:
            s1.Insert(it)

        for it in reversed(Alphabet):
            s3.Insert(it)

        for it in Alphabet[7:]:
            s9.Insert(it)


        ################################
        # Do basic testing of set stuff
        ################################
        TestSetOperations(tester, StringSet, Alphabet, BadAlphabet,
                          True, s0, s1, s2, s3, s9)
 


        ############################################################
        # Test failure of set operations with different universes
        ############################################################
        u2 = StringSetUniverse(u1) # Deep copies universe
        s1 = StringSet(u1, True) # Whole alphabet
        s2 = StringSet(u2, True) # Whole alphabet

        tester.Test("Union, different universe", False, s1.Union, s2)
        tester.Test("Union, different universe - operator +", False, operator.add, s1, s2)
        tester.Test("Difference, different universe", False, s1.Difference, s2)
        tester.Test("Difference, differenet universe - operator -", False, operator.sub, s1, s2)
        tester.Test("Intersection, different universe", False, s1.Intersection, s2)
        tester.Test("Intersection, different universe - operator /", False, operator.truediv, s1, s2)

        tester.Test("Union assign, different universe", False, s1.UnionAssign, s2)
        tester.Test("Union assign, different universe - operator +=", False, operator.iadd, s1, s2)
        tester.Test("Difference assign, different universe", False, s1.DifferenceAssign, s2)
        tester.Test("Difference assign, differenet universe - operator -=", False, operator.isub, s1, s2)
        tester.Test("Intersection assign, different universe", False, s1.IntersectionAssign, s2)
        tester.Test("Intersection assign, different universe - operator /=", False, operator.itruediv, s1, s2)


        # Lastly, use empty universes
        u3 = StringSetUniverse()
        s3 = StringSet(u3, True)
        s4 = s3.Complement()
        tester.TestValue("Complement without universe", s3 == s4, True)

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
