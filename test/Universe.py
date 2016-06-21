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


def SetComp(set1, set2):
    if len(set1) != len(set2):
        return False
    for i in range(0, len(set1)):
        if set1[i] != set2[i]:
            return False
    return True

def SetComp2(set1, set2):
    return set1 == set2


def PrintSet(s):
    string = "{} : ".format(len(s))
    for it in s:
        string += str(it)
    print(string)

def SetElement(s, i, e):
    s[i] = e


def FillUniverse(values):
    u = StringSetUniverse()
    for v in values:
        u.insert(v)
    return u


def Run():
    try:
        Alphabet="abcdefghijklmnopqrstuvwxyz"
        BadAlphabet = "ABCDE"

        tester = Tester("Testing Universe and MathSet")
        tester.print_header()


        ################################
        # Do basic testing of set stuff
        ################################
        u0 = StringSetUniverse()
        u1 = FillUniverse(Alphabet[:7])
        u2 = FillUniverse(Alphabet)
        u3 = FillUniverse(reversed(Alphabet))
        u9 = FillUniverse(Alphabet[7:])
        test_set_operations(tester, StringSetUniverse, Alphabet, BadAlphabet,
                          False, u0, u1, u2, u3, u9)

        ###############################
        # Serialization
        ###############################
        tester.test("Universe serialization - u0", True, TestSerialization_StringSetUniverse, u0)
        tester.test("Universe serialization - u1", True, TestSerialization_StringSetUniverse, u1)
        tester.test("Universe serialization - u2", True, TestSerialization_StringSetUniverse, u2)
        tester.test("Universe serialization - u3", True, TestSerialization_StringSetUniverse, u3)
        tester.test("Universe serialization - u9", True, TestSerialization_StringSetUniverse, u9)

        ###############################
        # Universe-specific stuff
        ###############################
        for idx in range(0, len(u1)):
            tester.test_value("Element {} by idx - getitem".format(idx), u1[idx], Alphabet[idx])
            tester.test_value("Element {} by idx - At".format(idx), u1.at(idx), Alphabet[idx])


        # Try inserting bad values
        tester.test("inserting 10", False, u1.insert, 10)
        tester.test("inserting None", False, u1.insert, None)


        # Modification of an element in a universe -- shouldn't be possible
        tester.test("Modification of an element in universe", False, SetElement, u1, 0, "Z")


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
