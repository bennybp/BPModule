

def SetContainsOwnElements(tester, s, name):
    idx = 0
    for v in s:
        tester.Test("{} Contains {}".format(name, idx), True, s.Contains, v)
        tester.Test("{} Contains {} - via 'in'".format(name, idx), True, lambda u, el: el in u, s, v) 
        idx += 1

def SetContainsValidValue(tester, s, values, name):
    idx = 0
    for v in s:
        tester.TestValue("{} Contains valid value {}".format(name, idx), v in values, True)
        idx += 1

def SetContainsAllValues(tester, s, values, name):
    idx = 0
    for v in values:
        tester.TestValue("{} Contains valid value {}".format(name, idx), s.Contains(v), True)
        idx += 1

def SetDoesntContainValues(tester, s, badvalues, name):
    idx = 0
    for v in badvalues:
        tester.TestValue("{} Doesn't contain value {}".format(name, idx), s.Contains(v), False)
        idx += 1
        

def TestUnion(tester, Type, v, v2, vresult):
    tester.TestValue("Union", v.Union(v2) == vresult, True)
    tester.TestValue("Union - operator +", (v + v2) == vresult, True)
    tester.TestValue("Union", v2.Union(v) == vresult, True)
    tester.TestValue("Union - operator +", (v2 + v) == vresult, True)

    stmp = Type(v)
    stmp.UnionAssign(v2)
    tester.TestValue("Union assign", stmp == vresult, True)
    stmp = Type(v) 
    stmp += v2
    tester.TestValue("Union assign - operator +=", stmp == vresult, True)
    stmp = Type(v)
    tester.TestValue("Union assign - return value", stmp.UnionAssign(v2) == vresult, True)

    stmp = Type(v2)
    stmp.UnionAssign(v)
    tester.TestValue("Union assign", stmp == vresult, True)
    stmp = Type(v2) 
    stmp += v
    tester.TestValue("Union assign - operator +=", stmp == vresult, True)
    stmp = Type(v2)
    tester.TestValue("Union assign - return value", stmp.UnionAssign(v) == vresult, True)


def TestIntersection(tester, Type, v, v2, vresult):
    tester.TestValue("Intersection", v.Intersection(v2) == vresult, True)
    tester.TestValue("Intersection - operator /", (v / v2) == vresult, True)
    tester.TestValue("Intersection", v2.Intersection(v) == vresult, True)
    tester.TestValue("Intersection - operator /", (v2 / v) == vresult, True)

    stmp = Type(v)
    stmp.IntersectionAssign(v2)
    tester.TestValue("Intersection assign", stmp == vresult, True)
    stmp = Type(v) 
    stmp /= v2
    tester.TestValue("Intersection assign - operator /=", stmp == vresult, True)
    stmp = Type(v)
    tester.TestValue("Intersection assign - return value", stmp.IntersectionAssign(v2) == vresult, True)

    stmp = Type(v2)
    stmp.IntersectionAssign(v)
    tester.TestValue("Intersection assign", stmp == vresult, True)
    stmp = Type(v2) 
    stmp /= v
    tester.TestValue("Intersection assign - operator /=", stmp == vresult, True)
    stmp = Type(v2)
    tester.TestValue("Intersection assign - return value", stmp.IntersectionAssign(v) == vresult, True)


def TestDifference(tester, Type, v, v2, vresult, vresult2):
    tester.TestValue("Difference", v.Difference(v2) == vresult, True)
    tester.TestValue("Difference - operator -", (v - v2) == vresult, True)
    tester.TestValue("Difference", v2.Difference(v) == vresult2, True)
    tester.TestValue("Difference - operator -", (v2 - v) == vresult2, True)

    stmp = Type(v)
    stmp.DifferenceAssign(v2)
    tester.TestValue("Difference assign", stmp == vresult, True)
    stmp = Type(v) 
    stmp -= v2
    tester.TestValue("Difference assign - operator -=", stmp == vresult, True)
    stmp = Type(v)
    tester.TestValue("Difference assign - return value", stmp.DifferenceAssign(v2) == vresult, True)

    stmp = Type(v2)
    stmp.DifferenceAssign(v)
    tester.TestValue("Difference assign", stmp == vresult2, True)
    stmp = Type(v2) 
    stmp -= v
    tester.TestValue("Difference assign - operator -=", stmp == vresult2, True)
    stmp = Type(v2)
    tester.TestValue("Difference assign - return value", stmp.DifferenceAssign(v) == vresult2, True)


def TestSubsetSuperset(tester, v, v2, ispsubset, issubset, ispsuperset, issuperset):
    tester.TestValue("Proper Subset", ispsubset, v.IsProperSubsetOf(v2))
    tester.TestValue("Proper Subset - operator <", ispsubset, v < v2)
    tester.TestValue("Subset", issubset, v.IsSubsetOf(v2))
    tester.TestValue("Subset - operator <=", issubset, v <= v2)
    tester.TestValue("Proper Superset", ispsuperset, v.IsProperSupersetOf(v2))
    tester.TestValue("Proper Superset - operator >", ispsuperset, v > v2)
    tester.TestValue("Superset", issuperset, v.IsSupersetOf(v2))
    tester.TestValue("Superset - operator >=", issuperset, v >= v2)

    tester.TestValue("Proper Subset", ispsuperset, v2.IsProperSubsetOf(v))
    tester.TestValue("Proper Subset - operator <", ispsuperset, v2 < v)
    tester.TestValue("Subset", issuperset, v2.IsSubsetOf(v))
    tester.TestValue("Subset - operator <=", issuperset, v2 <= v)
    tester.TestValue("Proper Superset", ispsubset, v2.IsProperSupersetOf(v))
    tester.TestValue("Proper Superset - operator >", ispsubset, v2 > v)
    tester.TestValue("Superset", issubset, v2.IsSupersetOf(v))
    tester.TestValue("Superset - operator >=", issubset, v2 >= v)


def TestPermutations(tester, Type, s, name):
    stmp = Type(s)
    tester.TestValue("Copy construction - {}".format(name), stmp == s, True)

    for it in s:
        stmp.Insert(it)

    tester.TestValue("Size after re-insertion of all elements - {}".format(name), stmp.Size() == s.Size(), True)
    tester.TestValue("After re-insertion of all elements - {}".format(name), stmp == s, True)


    



# s0 = empty
# s1 = with some values
# s2 = with all values
# s3 = with all values, reversed
# s9 = complement of s1
def TestSetOperations(tester, Type, values, badvalues, hascomplement,
                      s0, s1, s2, s3, s9):

    SetContainsOwnElements(tester, s0, "s0")
    SetContainsOwnElements(tester, s1, "s1")
    SetContainsOwnElements(tester, s2, "s2")
    SetContainsOwnElements(tester, s9, "s9")

    SetContainsValidValue(tester, s0, values, "s0")
    SetContainsValidValue(tester, s1, values, "s1")
    SetContainsValidValue(tester, s2, values, "s2")
    SetContainsValidValue(tester, s9, values, "s9")

    SetDoesntContainValues(tester, s0, badvalues, "s0")
    SetDoesntContainValues(tester, s1, badvalues, "s1")
    SetDoesntContainValues(tester, s2, badvalues, "s2")
    SetDoesntContainValues(tester, s9, badvalues, "s9")

    TestPermutations(tester, Type, s1, "s1")

    tester.TestValue("Comparison - s0 == s0", s0 == s0, True)
    tester.TestValue("Comparison - s1 == s1", s1 == s1, True)
    tester.TestValue("Comparison - s0 == s1", s0 == s1, False)
    tester.TestValue("Comparison - s1 == s0", s1 == s0, False)
    tester.TestValue("Comparison - s0 != s0", s0 != s0, False)
    tester.TestValue("Comparison - s1 != s1", s1 != s1, False)
    tester.TestValue("Comparison - s0 != s1", s0 != s1, True)
    tester.TestValue("Comparison - s1 != s0", s1 != s0, True)
    tester.TestValue("Comparison - s3 == s2", s3 == s2, True)
    tester.TestValue("Comparison - s3 != s2", s3 != s2, False)
    tester.TestValue("Comparison - s2 == s3", s2 == s3, True)
    tester.TestValue("Comparison - s2 != s3", s2 != s3, False)

    # Self assignment
    stmp = Type(s0)
    tester.TestValue("Compare after self assignment", stmp == s0, True)
    stmp = Type(s1)
    tester.TestValue("Compare after self assignment", stmp == s1, True)

    # modify stmp
    for it in s2:
        stmp.Insert(it)
    tester.TestValue("Test deep copy aliasing", stmp == s1, False)
    

    tester.TestValue("Size - s0", s0.Size(), 0)
    tester.TestValue("Size - s2", s2.Size(), len(values))
    tester.TestValue("Size - s1+s9", s1.Size() + s9.Size(), len(values))
    tester.TestValue("Length operator - s0", len(s0), s0.Size())
    tester.TestValue("Length operator - s1", len(s1), s1.Size())
    tester.TestValue("Length operator - s2", len(s2), s2.Size())
    tester.TestValue("Length operator - s9", len(s9), s9.Size())

    ns0 = len(s0)
    ns1 = len(s1)
    ns2 = len(s2)
    ns9 = len(s9)

    tester.TestValue("s0 is empty", ns0, 0)
    tester.TestValue("length s1 + s9 = s2", ns1 + ns9, ns2)

    SetContainsAllValues(tester, s0, [], "s0")
    SetContainsAllValues(tester, s1, values[:ns1], "s0")
    SetContainsAllValues(tester, s2, values, "s2")
    SetContainsAllValues(tester, s9, values[ns1:], "s9")

    TestUnion(tester, Type, s0, s0, s0)
    TestUnion(tester, Type, s1, s1, s1)
    TestUnion(tester, Type, s1, s2, s2)
    TestUnion(tester, Type, s1, s9, s2)

    TestDifference(tester, Type, s0, s0, s0, s0)
    TestDifference(tester, Type, s0, s1, s0, s1)
    TestDifference(tester, Type, s1, s1, s0, s0)
    TestDifference(tester, Type, s1, s2, s0, s9)
    TestDifference(tester, Type, s9, s1, s9, s1)

    TestIntersection(tester, Type, s0, s0, s0)
    TestIntersection(tester, Type, s0, s1, s0)
    TestIntersection(tester, Type, s1, s1, s1)
    TestIntersection(tester, Type, s1, s2, s1)
    TestIntersection(tester, Type, s1, s9, s0)
    TestIntersection(tester, Type, s2, s9, s9)


    if hascomplement:
        stmp = s1.Complement()
        tester.TestValue("Complement", stmp == s9, True)
        stmp = s0.Complement()
        tester.TestValue("Complement of null set", stmp == s2, True)
        stmp = s2.Complement()
        tester.TestValue("Complement of full set", stmp == s0, True)


    #                                  ispsub, issub,  ispsup, issup
    TestSubsetSuperset(tester, s0, s0, False,  True,   False,  True)
    TestSubsetSuperset(tester, s0, s1, True,   True,   False,  False)
    TestSubsetSuperset(tester, s1, s1, False,  True,   False,  True)
    TestSubsetSuperset(tester, s1, s2, True,   True,   False,  False)
    TestSubsetSuperset(tester, s1, s2, True,   True,   False,  False)
    TestSubsetSuperset(tester, s2, s9, False,  False,  True,   True)

