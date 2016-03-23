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


def Run():
    try:
        Alphabet="abcdefghijklmnopqrstuvwxyz"

        tester = bp.testing.Tester("Testing Universe and MathSet")
        tester.PrintHeader()

        u0 = StringSetUniverse()
        u1 = StringSetUniverse()
        tester.Test("Empty self comparison", True, SetComp, u0, u0)
        tester.Test("Empty self comparison", True, SetComp2, u0, u0)
        tester.Test("Empty comparison", True, SetComp, u0, u1)
        tester.Test("Empty comparison", True, SetComp2, u0, u1)

        # Make a universe of the alphabet
        for i in list(Alphabet):
           u1.Insert(i)


        # Iteration, etc
        tester.TestValue("Universe size", len(u0), 0)
        tester.TestValue("Universe size", len(u1), len(Alphabet))
        tester.TestValue("Universe size", u1.size(), len(Alphabet))

        for idx in range(0, len(u1)):
            tester.TestValue("Element {} by idx - getitem".format(idx), u1[idx], Alphabet[idx])
            tester.TestValue("Element {} by idx - at".format(idx), u1.at(idx), Alphabet[idx])

        idx = 0
        for it in u1:
            tester.TestValue("Element {} by iterator".format(idx), it, Alphabet[idx])
            tester.Test("Universe contains {}".format(it), True, u1.Contains, it)
            tester.Test("Universe contains {} - via 'in'".format(it), True, lambda u, el: el in u, u1, it)
            idx += 1


        # Test failures of contains, getitem, etc
        tester.Test("Universe contains A", False, u1.Contains, "A")
        tester.Test("Universe contains Z", False, u1.Contains, "Z")
        tester.Test("Getting idx 26", False, u1.at, 26)
        tester.Test("Getting idx 26 via []A", False, u1.__getitem__, 26)
        tester.Test("Inserting 10", False, u1.Insert, 10)
        tester.Test("Inserting None", False, u1.Insert, None)


        # Copying, etc
        tester.Test("Self comparison", True, SetComp, u1, u1)
        tester.Test("Self comparison", True, SetComp2, u1, u1)

        u2 = StringSetUniverse(u1)
        tester.Test("Compare after deep copy", True, SetComp2, u1, u2)

        # Self assignment
        u1 = u1
        tester.Test("Compare after self assignment", True, SetComp2, u1, u1)


        # Modify u2
        u2.Insert("A")
        tester.Test("Un-aliasing of deep copy", False, SetComp2, u1, u2)

        # Copy assignment
        u3 = u2
        tester.Test("Compare after assignment", True, SetComp, u2, u3)
        tester.Test("Compare after assignment", True, SetComp2, u2, u3)

        # Modify u3
        u3.Insert("Q")
        tester.Test("Aliasing of (python) assignment", True, SetComp2, u2, u3)

        # Modification of an element in a universe -- shouldn't be possible
        tester.Test("Modification of an element in universe", False, SetElement, u2, 0, "Z")


        # Subsets and supersets
        tester.TestValue("Self Proper Subset", False, u1.IsProperSubsetOf(u1))
        tester.TestValue("Self Proper Subset - operator <", False, u1 < u1)
        tester.TestValue("Self Subset", True, u1.IsSubsetOf(u1))
        tester.TestValue("Self Subset - operator <=", True, u1 <= u1)

        tester.TestValue("Self Proper Superset", False, u1.IsProperSupersetOf(u1))
        tester.TestValue("Self Proper Superset - operator >", False, u1 > u1)
        tester.TestValue("Self Superset", True, u1.IsSupersetOf(u1))
        tester.TestValue("Self Superset - operator <=", True, u1 >= u1)

        # u2 is a proper superset of u1 and u1 is a proper subset of u2
        tester.TestValue("Proper Subset u1, u2", True, u1.IsProperSubsetOf(u2))
        tester.TestValue("Proper Subset u1, u2 - operator <", True, u1 < u2)
        tester.TestValue("Subset u1, u2", True, u1.IsSubsetOf(u2))
        tester.TestValue("Subset u1, u2 - operator <=", True, u1 <= u2)
        tester.TestValue("Proper Subset u2, u1", False, u2.IsProperSubsetOf(u1))
        tester.TestValue("Proper Subset u2, u1 - operator <", False, u2 < u1)
        tester.TestValue("Subset u2, u1", False, u2.IsSubsetOf(u1))
        tester.TestValue("Subset u2, u1 - operator <=", False, u2 <= u1)

        tester.TestValue("Proper Superset u1, u2", False, u1.IsProperSupersetOf(u2))
        tester.TestValue("Proper Superset u1, u2 - operator >", False, u1 > u2)
        tester.TestValue("Superset u1, u2", False, u1.IsSupersetOf(u2))
        tester.TestValue("Superset u1, u2 - operator >=", False, u1 >= u2)
        tester.TestValue("Proper Superset u2, u1", True, u2.IsProperSupersetOf(u1))
        tester.TestValue("Proper Superset u2, u1 - operator >", True, u2 > u1)
        tester.TestValue("Superset u2, u1", True, u2.IsSupersetOf(u1))
        tester.TestValue("Superset u2, u1 - operator >=", True, u2 >= u1)

        # Ordering of elements
        uAQ = StringSetUniverse()
        uQA = StringSetUniverse()
        uAQ.Insert("A").Insert("Q").Insert("Q")
        uQA.Insert("Q").Insert("A").Insert("Q")

        tester.TestValue("Inserting equivalent elements", len(uAQ), 2)
        tester.TestValue("Inserting equivalent elements", len(uQA), 2)
        uQA.Insert("Q")
        uAQ.Insert("Q")
        tester.TestValue("Inserting equivalent elements", len(uAQ), 2)
        tester.TestValue("Inserting equivalent elements", len(uQA), 2)
        
        tester.Test("Equal with different ordering", True, SetComp2, uAQ, uQA)
        tester.Test("Equal with different ordering", True, SetComp2, uQA, uAQ)

        # Set operations
        tester.Test("Self union", True, SetComp2, u1.Union(u1), u1)
        tester.Test("Self union - operator +", True, SetComp2, u1 + u1, u1)
        tester.Test("Self difference", True, SetComp2, u1.Difference(u1), u0)
        tester.Test("Self difference - operator -", True, SetComp2, u1 - u1, u0)
        tester.TestValue("Self difference", len(u1 - u1), 0)
        tester.Test("Self intersection", True, SetComp2, u1.Intersection(u1), u1)
        tester.Test("Self intersection - operator /", True, SetComp2, u1 / u1, u1)

        tester.Test("Union", True, SetComp2, u1 + u2, u2)
        tester.Test("Difference", True, SetComp2, u1 - u2, u0)
        tester.Test("Intersection", True, SetComp2, u1 / u2, u1)

        tester.Test("Union", True, SetComp2, u2.Union(u1), u2)
        tester.Test("Union - operator +", True, SetComp2, u2 + u1, u2)
        tester.Test("Difference", True, SetComp2, u2.Difference(u1), uAQ)
        tester.Test("Difference - operator -", True, SetComp2, u2 - u1, uAQ)
        tester.Test("Difference - operator -", True, SetComp2, u2 - u1, uQA)
        tester.Test("Intersection", True, SetComp2, u2.Intersection(u1), u1)
        tester.Test("Intersection - operator /", True, SetComp2, u2 / u1, u1)

        
        # We need to deep copy a few times
        u4 = StringSetUniverse(u1) 
        tester.Test("Self union assign - return value", True, SetComp2, u4.UnionAssign(u4), u1)
        u4 = StringSetUniverse(u1)
        u4.UnionAssign(u4) 
        tester.Test("Self union assign", True, SetComp2, u4, u1)
        u4 = StringSetUniverse(u1) 
        u4 += u4
        tester.Test("Self union assign - operator +=", True, SetComp2, u4, u1)
        
        u4 = StringSetUniverse(u1) 
        tester.Test("Self difference assign - return value", True, SetComp2, u4.DifferenceAssign(u4), u0)
        u4 = StringSetUniverse(u1)
        u4.DifferenceAssign(u4) 
        tester.Test("Self difference assign", True, SetComp2, u4, u0)
        u4 = StringSetUniverse(u1) 
        u4 -= u4
        tester.Test("Self difference assign - operator -=", True, SetComp2, u4, u0)

        u4 = StringSetUniverse(u1) 
        tester.Test("Self intersection assign - return value", True, SetComp2, u4.IntersectionAssign(u4), u1)
        u4 = StringSetUniverse(u1)
        u4.IntersectionAssign(u4) 
        tester.Test("Self intersection assign", True, SetComp2, u4, u1)
        u4 = StringSetUniverse(u1) 
        u4 /= u4
        tester.Test("Self intersection assign - operator /=", True, SetComp2, u4, u1)


        u4 = StringSetUniverse(u1) 
        tester.Test("Union assign - return value", True, SetComp2, u4.UnionAssign(u2), u2)
        u4 = StringSetUniverse(u1)
        u4.UnionAssign(u2) 
        tester.Test("Union assign", True, SetComp2, u4, u2)
        u4 = StringSetUniverse(u1) 
        u4 += u2
        tester.Test("Union assign - operator +=", True, SetComp2, u4, u2)

        # u1 - u2 = null set
        u4 = StringSetUniverse(u1) 
        tester.Test("Difference assign - return value", True, SetComp2, u4.DifferenceAssign(u2), u0)
        u4 = StringSetUniverse(u1)
        u4.DifferenceAssign(u2) 
        tester.Test("Difference assign", True, SetComp2, u4, u0)
        u4 = StringSetUniverse(u1) 
        u4 -= u2
        tester.Test("Difference assign - operator -=", True, SetComp2, u4, u0)

        # u2 - u1
        u4 = StringSetUniverse(u2) 
        tester.Test("Difference assign - return value", True, SetComp2, u4.DifferenceAssign(u1), uAQ)
        u4 = StringSetUniverse(u2)
        u4.DifferenceAssign(u1) 
        tester.Test("Difference assign", True, SetComp2, u4, uAQ)
        u4 = StringSetUniverse(u2) 
        u4 -= u1
        tester.Test("Difference assign - operator -=", True, SetComp2, u4, uAQ)


        u4 = StringSetUniverse(u1) 
        tester.Test("Intersection assign - return value", True, SetComp2, u4.IntersectionAssign(u2), u1)
        u4 = StringSetUniverse(u1)
        u4.IntersectionAssign(u2) 
        tester.Test("Intersection assign", True, SetComp2, u4, u1)
        u4 = StringSetUniverse(u1) 
        u4 /= u2
        tester.Test("Intersection assign - operator /=", True, SetComp2, u4, u1)





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
