#!/usr/bin/env python3

import os
import sys
import traceback 
import operator

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp
from bpmodule.output import *
from bpmodule.testing import *
from bpmodule.math import *

from helper.SetOperations import TestSetOperations


def SetElement(s, i, e):
    s[i] = e


def Run():
    try:
        Alphabet="abcdefghijklmnopqrstuvwxyz"
        BadAlphabet = "ABCDE"

        tester = Tester("Testing Universe and MathSet")
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
 
        ################################
        # Serialization
        ################################
        tester.Test("MathSet serialization - s0", True, TestSerialization_StringSet, s0)
        tester.Test("MathSet serialization - s1", True, TestSerialization_StringSet, s1)
        tester.Test("MathSet serialization - s2", True, TestSerialization_StringSet, s2)
        tester.Test("MathSet serialization - s3", True, TestSerialization_StringSet, s3)
        tester.Test("MathSet serialization - s9", True, TestSerialization_StringSet, s9)


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
      GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      GlobalError("\n")
      GlobalError(str(e))
      GlobalError("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
