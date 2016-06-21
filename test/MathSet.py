#!/usr/bin/env python3

import os
import sys
import traceback 
import operator

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr
from pulsar.output import *
from pulsar.testing import *
from pulsar.math import *

from helper.SetOperations import test_set_operations


def SetElement(s, i, e):
    s[i] = e


def Run():
    try:
        Alphabet="abcdefghijklmnopqrstuvwxyz"
        BadAlphabet = "ABCDE"

        tester = Tester("Testing Universe and MathSet")
        tester.print_header()

        u1 = StringSetUniverse()

        # Make a universe of the alphabet
        for i in list(Alphabet):
           u1.insert(i)


        # insert all
        s0 = StringSet(u1, False)
        s1 = StringSet(u1, False)
        s2 = StringSet(u1, True)
        s3 = StringSet(u1, False)
        s9 = StringSet(u1, False)

        for it in Alphabet[:7]:
            s1.insert(it)

        for it in reversed(Alphabet):
            s3.insert(it)

        for it in Alphabet[7:]:
            s9.insert(it)


        ################################
        # Do basic testing of set stuff
        ################################
        test_set_operations(tester, StringSet, Alphabet, BadAlphabet,
                          True, s0, s1, s2, s3, s9)
 
        ################################
        # Serialization
        ################################
        tester.test("MathSet serialization - s0", True, TestSerialization_StringSet, s0)
        tester.test("MathSet serialization - s1", True, TestSerialization_StringSet, s1)
        tester.test("MathSet serialization - s2", True, TestSerialization_StringSet, s2)
        tester.test("MathSet serialization - s3", True, TestSerialization_StringSet, s3)
        tester.test("MathSet serialization - s9", True, TestSerialization_StringSet, s9)


        ############################################################
        # Test failure of set operations with different universes
        ############################################################
        u2 = StringSetUniverse(u1) # Deep copies universe
        s1 = StringSet(u1, True) # Whole alphabet
        s2 = StringSet(u2, True) # Whole alphabet

        tester.test("Union, different universe", False, s1.set_union, s2)
        tester.test("Union, different universe - operator +", False, operator.add, s1, s2)
        tester.test("Difference, different universe", False, s1.difference, s2)
        tester.test("Difference, differenet universe - operator -", False, operator.sub, s1, s2)
        tester.test("intersection, different universe", False, s1.intersection, s2)
        tester.test("intersection, different universe - operator /", False, operator.truediv, s1, s2)

        tester.test("Union assign, different universe", False, s1.union_assign, s2)
        tester.test("Union assign, different universe - operator +=", False, operator.iadd, s1, s2)
        tester.test("Difference assign, different universe", False, s1.difference_assign, s2)
        tester.test("Difference assign, differenet universe - operator -=", False, operator.isub, s1, s2)
        tester.test("intersection assign, different universe", False, s1.intersection_assign, s2)
        tester.test("intersection assign, different universe - operator /=", False, operator.itruediv, s1, s2)


        # Lastly, use empty universes
        u3 = StringSetUniverse()
        s3 = StringSet(u3, True)
        s4 = s3.complement()
        tester.test_value("Complement without universe", s3 == s4, True)

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
