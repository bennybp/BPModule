#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback

from helper.SetOperations import TestSetOperations

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp
from bpmodule.math import StringSetUniverse

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
        u.Insert(v)
    return u


def Run():
    try:
        Alphabet="abcdefghijklmnopqrstuvwxyz"
        BadAlphabet = "ABCDE"

        tester = bp.testing.Tester("Testing Universe and MathSet")
        tester.PrintHeader()


        ################################
        # Do basic testing of set stuff
        ################################
        u0 = StringSetUniverse()
        u1 = FillUniverse(Alphabet[:7])
        u2 = FillUniverse(Alphabet)
        u3 = FillUniverse(reversed(Alphabet))
        u9 = FillUniverse(Alphabet[7:])
        TestSetOperations(tester, StringSetUniverse, Alphabet, BadAlphabet,
                          False, u0, u1, u2, u3, u9)


        ###############################
        # Universe-specific stuff
        ###############################
        for idx in range(0, len(u1)):
            tester.TestValue("Element {} by idx - getitem".format(idx), u1[idx], Alphabet[idx])
            tester.TestValue("Element {} by idx - At".format(idx), u1.At(idx), Alphabet[idx])


        # Try inserting bad values
        tester.Test("Inserting 10", False, u1.Insert, 10)
        tester.Test("Inserting None", False, u1.Insert, None)


        # Modification of an element in a universe -- shouldn't be possible
        tester.Test("Modification of an element in universe", False, SetElement, u1, 0, "Z")


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
