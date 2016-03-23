#!/usr/bin/env python3


import os
import sys
import argparse
import operator
import traceback

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp
from bpmodule.math import StringSetUniverse, StringSet

def SetComp(set1, set2):
    return set1 == set2


def PrintSet(s):
    string = "{} : ".format(len(s))
    for it in s:
        string += str(it)
    print(string)

def SetElement(s, i, e):
    s[i] = e

def InsertElement(s, e):
    s.Insert(e)
    return True

def Run():
    try:
        Alphabet="abcdefghijklmnopqrstuvwxyz"
        AlphabetBegin="abcdefg"
        AlphabetEnd="hijklmnopqrstuvwxyz"

        tester = bp.testing.Tester("Testing Universe and MathSet")
        tester.PrintHeader()

        u0 = StringSetUniverse()
        u1 = StringSetUniverse()

        # Make a universe of the alphabet
        for i in list(Alphabet):
           u1.Insert(i)

        s0 = StringSet(u0, True)
        s1 = StringSet(u0, True)

        tester.Test("Empty self comparison", True, SetComp, s0, s0)
        tester.Test("Empty comparison", True, SetComp, s0, s1)

        s1 = StringSet(u1, False)
        tester.Test("Empty comparison 2", False, SetComp, s0, s1) # but different universe

        s1 = StringSet(u1, True)

        # Iteration, etc
        tester.TestValue("MathSet size", len(s0), 0)
        tester.TestValue("MathSet size", len(s1), len(Alphabet))
        tester.TestValue("MathSet size", s1.size(), len(Alphabet))


        idx = 0
        for it in s1:
            tester.TestValue("Element {} by iterator".format(idx), it, Alphabet[idx])
            tester.Test("MathSet contains {}".format(it), True, s1.Contains, it)
            tester.Test("MathSet contains {} - via 'in'".format(it), True, lambda u, el: el in u, s1, it)
            idx += 1


        # Test failures of contains, getitem, etc
        tester.Test("MathSet contains A", False, s1.Contains, "A")
        tester.Test("MathSet contains Z", False, s1.Contains, "Z")
        tester.Test("Inserting a", True, InsertElement, s1, "a")
        tester.Test("Inserting A", False, InsertElement, s1, "A") # Not part of universe
        tester.Test("Inserting 10", False, s1.Insert, 10)
        tester.Test("Inserting None", False, s1.Insert, None)
        tester.TestValue("MathSet size", s1.size(), len(Alphabet))


        # Insert all
        s1 = StringSet(u1, False)
        tester.TestValue("MathSet size", s1.size(), 0)
        tester.TestValue("MathSet size", len(s1), 0)
        for it in Alphabet:
            tester.Test("Inserting {}".format(it), True, InsertElement, s1, it)
        tester.TestValue("MathSet size", s1.size(), len(Alphabet))
     
        # Copying, etc
        tester.Test("Self comparison", True, SetComp, s1, s1)


        # Self assignment
        s1 = s1
        tester.Test("Compare after self assignment", True, SetComp, s1, s1)


        s1 = StringSet(u1, False)
        for it in AlphabetBegin:
            s1.Insert(it)

        # Modify s2
        s2 = StringSet(s1)
        tester.Test("Compare after deep copy", True, SetComp, s1, s2)

        s2.Insert("z")
        tester.Test("Un-aliasing of deep copy", False, SetComp, s1, s2)


        # Copy assignment
        s3 = s2
        tester.Test("Compare after assignment", True, SetComp, s2, s3)

        # Modify s3
        s3.Insert("q")
        tester.Test("Aliasing of (python) assignment", True, SetComp, s2, s3)

        # Modification of an element in a universe -- shouldn't be possible
        tester.Test("Modification of an element in universe", False, SetElement, s2, 0, "Z")


        # Subsets and supersets
        tester.TestValue("Self Proper Subset", False, s1.IsProperSubsetOf(s1))
        tester.TestValue("Self Proper Subset - operator <", False, s1 < s1)
        tester.TestValue("Self Subset", True, s1.IsSubsetOf(s1))
        tester.TestValue("Self Subset - operator <=", True, s1 <= s1)

        tester.TestValue("Self Proper Superset", False, s1.IsProperSupersetOf(s1))
        tester.TestValue("Self Proper Superset - operator >", False, s1 > s1)
        tester.TestValue("Self Superset", True, s1.IsSupersetOf(s1))
        tester.TestValue("Self Superset - operator <=", True, s1 >= s1)

        #s0 is empty, same universe
        #s1 only contains the beginning of the alphabet
        #s2 contains the whole alphabet, same universe object
        #s9 contains the end of the alphabet
        s0 = StringSet(u1, False)
        for it in Alphabet:
            s2.Insert(it)
        s9 = StringSet(u1, False)
        for it in AlphabetEnd:
            s9.Insert(it)


        # s2 is a proper superset of s1 and s1 is a proper subset of s2
        tester.TestValue("Proper Subset s1, s2", True, s1.IsProperSubsetOf(s2))
        tester.TestValue("Proper Subset s1, s2 - operator <", True, s1 < s2)
        tester.TestValue("Subset s1, s2", True, s1.IsSubsetOf(s2))
        tester.TestValue("Subset s1, s2 - operator <=", True, s1 <= s2)
        tester.TestValue("Proper Subset s2, s1", False, s2.IsProperSubsetOf(s1))
        tester.TestValue("Proper Subset s2, s1 - operator <", False, s2 < s1)
        tester.TestValue("Subset s2, s1", False, s2.IsSubsetOf(s1))
        tester.TestValue("Subset s2, s1 - operator <=", False, s2 <= s1)

        tester.TestValue("Proper Superset s1, s2", False, s1.IsProperSupersetOf(s2))
        tester.TestValue("Proper Superset s1, s2 - operator >", False, s1 > s2)
        tester.TestValue("Superset s1, s2", False, s1.IsSupersetOf(s2))
        tester.TestValue("Superset s1, s2 - operator >=", False, s1 >= s2)
        tester.TestValue("Proper Superset s2, s1", True, s2.IsProperSupersetOf(s1))
        tester.TestValue("Proper Superset s2, s1 - operator >", True, s2 > s1)
        tester.TestValue("Superset s2, s1", True, s2.IsSupersetOf(s1))
        tester.TestValue("Superset s2, s1 - operator >=", True, s2 >= s1)

        # Ordering of elements
        sAQ = StringSet(u1, False)
        sQA = StringSet(u1, False)
        sAQ.Insert("a").Insert("q").Insert("q")
        sQA.Insert("q").Insert("a").Insert("a")

        tester.TestValue("Inserting equivalent elements", len(sAQ), 2)
        tester.TestValue("Inserting equivalent elements", len(sQA), 2)
        sQA.Insert("q")
        sAQ.Insert("q")
        tester.TestValue("Inserting equivalent elements", len(sAQ), 2)
        tester.TestValue("Inserting equivalent elements", len(sQA), 2)
        
        tester.Test("Equal with different ordering", True, SetComp, sAQ, sQA)
        tester.Test("Equal with different ordering", True, SetComp, sQA, sAQ)

        # Set operations
        tester.Test("Self union", True, SetComp, s1.Union(s1), s1)
        tester.Test("Self union - operator +", True, SetComp, s1 + s1, s1)
        tester.Test("Self difference", True, SetComp, s1.Difference(s1), s0)
        tester.Test("Self difference - operator -", True, SetComp, s1 - s1, s0)
        tester.TestValue("Self difference", len(s1 - s1), 0)
        tester.Test("Self intersection", True, SetComp, s1.Intersection(s1), s1)
        tester.Test("Self intersection - operator /", True, SetComp, s1 / s1, s1)

        tester.Test("Union", True, SetComp, s1 + s2, s2)
        tester.Test("Difference", True, SetComp, s1 - s2, s0)
        tester.Test("Intersection", True, SetComp, s1 / s2, s1)

        tester.Test("Union", True, SetComp, s2.Union(s1), s2)
        tester.Test("Union - operator +", True, SetComp, s2 + s1, s2)
        tester.Test("Difference", True, SetComp, s2.Difference(s1), s9)
        tester.Test("Difference - operator -", True, SetComp, s2 - s1, s9)
        tester.Test("Intersection", True, SetComp, s2.Intersection(s1), s1)
        tester.Test("Intersection - operator /", True, SetComp, s2 / s1, s1)


        # We need to deep copy a few times
        s4 = StringSet(s1) 
        tester.Test("Self union assign - return value", True, SetComp, s4.UnionAssign(s4), s1)
        s4 = StringSet(s1)
        s4.UnionAssign(s4) 
        tester.Test("Self union assign", True, SetComp, s4, s1)
        s4 = StringSet(s1) 
        s4 += s4
        tester.Test("Self union assign - operator +=", True, SetComp, s4, s1)
        
        s4 = StringSet(s1) 
        tester.Test("Self difference assign - return value", True, SetComp, s4.DifferenceAssign(s4), s0)
        s4 = StringSet(s1)
        s4.DifferenceAssign(s4) 
        tester.Test("Self difference assign", True, SetComp, s4, s0)
        s4 = StringSet(s1) 
        s4 -= s4
        PrintSet(s4)
        tester.Test("Self difference assign - operator -=", True, SetComp, s4, s0)

        s4 = StringSet(s1) 
        tester.Test("Self intersection assign - return value", True, SetComp, s4.IntersectionAssign(s4), s1)
        s4 = StringSet(s1)
        s4.IntersectionAssign(s4) 
        tester.Test("Self intersection assign", True, SetComp, s4, s1)
        s4 = StringSet(s1) 
        s4 /= s4
        tester.Test("Self intersection assign - operator /=", True, SetComp, s4, s1)

        s4 = StringSet(s1) 
        tester.Test("Union assign - return value", True, SetComp, s4.UnionAssign(s2), s2)
        s4 = StringSet(s1)
        s4.UnionAssign(s2) 
        tester.Test("Union assign", True, SetComp, s4, s2)
        s4 = StringSet(s1) 
        s4 += s2
        tester.Test("Union assign - operator +=", True, SetComp, s4, s2)

        # s1 - s2 = null set
        s4 = StringSet(s1) 
        tester.Test("Difference assign - return value", True, SetComp, s4.DifferenceAssign(s2), s0)
        s4 = StringSet(s1)
        s4.DifferenceAssign(s2) 
        tester.Test("Difference assign", True, SetComp, s4, s0)
        s4 = StringSet(s1) 
        s4 -= s2
        tester.Test("Difference assign - operator -=", True, SetComp, s4, s0)

        # s2 - s1
        s4 = StringSet(s2) 
        tester.Test("Difference assign - return value", True, SetComp, s4.DifferenceAssign(s1), s9)
        s4 = StringSet(s2)
        s4.DifferenceAssign(s1) 
        tester.Test("Difference assign", True, SetComp, s4, s9)
        s4 = StringSet(s2) 
        s4 -= s1
        tester.Test("Difference assign - operator -=", True, SetComp, s4, s9)


        s4 = StringSet(s1) 
        tester.Test("Intersection assign - return value", True, SetComp, s4.IntersectionAssign(s2), s1)
        s4 = StringSet(s1)
        s4.IntersectionAssign(s2) 
        tester.Test("Intersection assign", True, SetComp, s4, s1)
        s4 = StringSet(s1) 
        s4 /= s2
        tester.Test("Intersection assign - operator /=", True, SetComp, s4, s1)


        # Testing the complement
        # s1 = beginning of alphabet, s9 would be its complement
        s4 = s1.Complement()
        tester.Test("Complement", True, SetComp, s4, s9)
        s4 = s0.Complement()
        tester.Test("Complement of null set", True, SetComp, s4, s2)
        s4 = s2.Complement()
        tester.Test("Complement of full set", True, SetComp, s4, s0)


        # Test failure of set operations with different universes
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
        tester.Test("Complement without universe", True, SetComp, s3, s4)

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
