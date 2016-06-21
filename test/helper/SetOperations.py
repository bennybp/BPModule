

def set_contains_own_elements(tester, s, name):
    idx = 0
    for v in s:
        tester.test("{} count {}".format(name, idx), True, s.count, v)
        tester.test("{} count {} - via 'in'".format(name, idx), True, lambda u, el: el in u, s, v) 
        idx += 1

def set_contains_valid_value(tester, s, values, name):
    idx = 0
    for v in s:
        tester.test_value("{} count valid value {}".format(name, idx), v in values, True)
        idx += 1

def set_contains_all_values(tester, s, values, name):
    idx = 0
    for v in values:
        tester.test_value("{} count valid value {}".format(name, idx), s.count(v), True)
        idx += 1

def set_doesnt_contain_values(tester, s, badvalues, name):
    idx = 0
    for v in badvalues:
        tester.test_value("{} Doesn't contain value {}".format(name, idx), s.count(v), False)
        idx += 1
        

def test_union(tester, Type, v, v2, vresult):
    tester.test_value("Union", v.set_union(v2) == vresult, True)
    tester.test_value("Union - operator +", (v + v2) == vresult, True)
    tester.test_value("Union", v2.set_union(v) == vresult, True)
    tester.test_value("Union - operator +", (v2 + v) == vresult, True)

    stmp = Type(v)
    stmp.union_assign(v2)
    tester.test_value("Union assign", stmp == vresult, True)
    stmp = Type(v) 
    stmp += v2
    tester.test_value("Union assign - operator +=", stmp == vresult, True)
    stmp = Type(v)
    tester.test_value("Union assign - return value", stmp.union_assign(v2) == vresult, True)

    stmp = Type(v2)
    stmp.union_assign(v)
    tester.test_value("Union assign", stmp == vresult, True)
    stmp = Type(v2) 
    stmp += v
    tester.test_value("Union assign - operator +=", stmp == vresult, True)
    stmp = Type(v2)
    tester.test_value("Union assign - return value", stmp.union_assign(v) == vresult, True)


def test_intersection(tester, Type, v, v2, vresult):
    tester.test_value("Intersection", v.intersection(v2) == vresult, True)
    tester.test_value("Intersection - operator /", (v / v2) == vresult, True)
    tester.test_value("Intersection", v2.intersection(v) == vresult, True)
    tester.test_value("Intersection - operator /", (v2 / v) == vresult, True)

    stmp = Type(v)
    stmp.intersection_assign(v2)
    tester.test_value("Intersection assign", stmp == vresult, True)
    stmp = Type(v) 
    stmp /= v2
    tester.test_value("Intersection assign - operator /=", stmp == vresult, True)
    stmp = Type(v)
    tester.test_value("Intersection assign - return value", stmp.intersection_assign(v2) == vresult, True)

    stmp = Type(v2)
    stmp.intersection_assign(v)
    tester.test_value("Intersection assign", stmp == vresult, True)
    stmp = Type(v2) 
    stmp /= v
    tester.test_value("Intersection assign - operator /=", stmp == vresult, True)
    stmp = Type(v2)
    tester.test_value("Intersection assign - return value", stmp.intersection_assign(v) == vresult, True)


def test_difference(tester, Type, v, v2, vresult, vresult2):
    tester.test_value("Difference", v.difference(v2) == vresult, True)
    tester.test_value("Difference - operator -", (v - v2) == vresult, True)
    tester.test_value("Difference", v2.difference(v) == vresult2, True)
    tester.test_value("Difference - operator -", (v2 - v) == vresult2, True)

    stmp = Type(v)
    stmp.difference_assign(v2)
    tester.test_value("Difference assign", stmp == vresult, True)
    stmp = Type(v) 
    stmp -= v2
    tester.test_value("Difference assign - operator -=", stmp == vresult, True)
    stmp = Type(v)
    tester.test_value("Difference assign - return value", stmp.difference_assign(v2) == vresult, True)

    stmp = Type(v2)
    stmp.difference_assign(v)
    tester.test_value("Difference assign", stmp == vresult2, True)
    stmp = Type(v2) 
    stmp -= v
    tester.test_value("Difference assign - operator -=", stmp == vresult2, True)
    stmp = Type(v2)
    tester.test_value("Difference assign - return value", stmp.difference_assign(v) == vresult2, True)


def test_subset_superset(tester, v, v2, ispsubset, issubset, ispsuperset, issuperset):
    tester.test_value("Proper Subset", ispsubset, v.is_proper_subset_of(v2))
    tester.test_value("Proper Subset - operator <", ispsubset, v < v2)
    tester.test_value("Subset", issubset, v.is_subset_of(v2))
    tester.test_value("Subset - operator <=", issubset, v <= v2)
    tester.test_value("Proper Superset", ispsuperset, v.is_proper_superset_of(v2))
    tester.test_value("Proper Superset - operator >", ispsuperset, v > v2)
    tester.test_value("Superset", issuperset, v.is_superset_of(v2))
    tester.test_value("Superset - operator >=", issuperset, v >= v2)

    tester.test_value("Proper Subset", ispsuperset, v2.is_proper_subset_of(v))
    tester.test_value("Proper Subset - operator <", ispsuperset, v2 < v)
    tester.test_value("Subset", issuperset, v2.is_subset_of(v))
    tester.test_value("Subset - operator <=", issuperset, v2 <= v)
    tester.test_value("Proper Superset", ispsubset, v2.is_proper_superset_of(v))
    tester.test_value("Proper Superset - operator >", ispsubset, v2 > v)
    tester.test_value("Superset", issubset, v2.is_superset_of(v))
    tester.test_value("Superset - operator >=", issubset, v2 >= v)


def test_permutations(tester, Type, s, name):
    stmp = Type(s)
    tester.test_value("Copy construction - {}".format(name), stmp == s, True)

    for it in s:
        stmp.insert(it)

    tester.test_value("Size after re-insertion of all elements - {}".format(name), stmp.size() == s.size(), True)
    tester.test_value("After re-insertion of all elements - {}".format(name), stmp == s, True)


    



# s0 = empty
# s1 = with some values
# s2 = with all values
# s3 = with all values, reversed
# s9 = complement of s1
def test_set_operations(tester, Type, values, badvalues, hascomplement,
                      s0, s1, s2, s3, s9):

    set_contains_own_elements(tester, s0, "s0")
    set_contains_own_elements(tester, s1, "s1")
    set_contains_own_elements(tester, s2, "s2")
    set_contains_own_elements(tester, s9, "s9")

    set_contains_valid_value(tester, s0, values, "s0")
    set_contains_valid_value(tester, s1, values, "s1")
    set_contains_valid_value(tester, s2, values, "s2")
    set_contains_valid_value(tester, s9, values, "s9")

    set_doesnt_contain_values(tester, s0, badvalues, "s0")
    set_doesnt_contain_values(tester, s1, badvalues, "s1")
    set_doesnt_contain_values(tester, s2, badvalues, "s2")
    set_doesnt_contain_values(tester, s9, badvalues, "s9")

    test_permutations(tester, Type, s1, "s1")

    tester.test_value("Comparison - s0 == s0", s0 == s0, True)
    tester.test_value("Comparison - s1 == s1", s1 == s1, True)
    tester.test_value("Comparison - s0 == s1", s0 == s1, False)
    tester.test_value("Comparison - s1 == s0", s1 == s0, False)
    tester.test_value("Comparison - s0 != s0", s0 != s0, False)
    tester.test_value("Comparison - s1 != s1", s1 != s1, False)
    tester.test_value("Comparison - s0 != s1", s0 != s1, True)
    tester.test_value("Comparison - s1 != s0", s1 != s0, True)
    tester.test_value("Comparison - s3 == s2", s3 == s2, True)
    tester.test_value("Comparison - s3 != s2", s3 != s2, False)
    tester.test_value("Comparison - s2 == s3", s2 == s3, True)
    tester.test_value("Comparison - s2 != s3", s2 != s3, False)

    # Self assignment
    stmp = Type(s0)
    tester.test_value("Compare after self assignment", stmp == s0, True)
    stmp = Type(s1)
    tester.test_value("Compare after self assignment", stmp == s1, True)

    # modify stmp
    for it in s2:
        stmp.insert(it)
    tester.test_value("Test deep copy aliasing", stmp == s1, False)
    

    tester.test_value("Size - s0", s0.size(), 0)
    tester.test_value("Size - s2", s2.size(), len(values))
    tester.test_value("Size - s1+s9", s1.size() + s9.size(), len(values))
    tester.test_value("Length operator - s0", len(s0), s0.size())
    tester.test_value("Length operator - s1", len(s1), s1.size())
    tester.test_value("Length operator - s2", len(s2), s2.size())
    tester.test_value("Length operator - s9", len(s9), s9.size())

    ns0 = len(s0)
    ns1 = len(s1)
    ns2 = len(s2)
    ns9 = len(s9)

    tester.test_value("s0 is empty", ns0, 0)
    tester.test_value("length s1 + s9 = s2", ns1 + ns9, ns2)

    set_contains_all_values(tester, s0, [], "s0")
    set_contains_all_values(tester, s1, values[:ns1], "s0")
    set_contains_all_values(tester, s2, values, "s2")
    set_contains_all_values(tester, s9, values[ns1:], "s9")

    test_union(tester, Type, s0, s0, s0)
    test_union(tester, Type, s1, s1, s1)
    test_union(tester, Type, s1, s2, s2)
    test_union(tester, Type, s1, s9, s2)

    test_difference(tester, Type, s0, s0, s0, s0)
    test_difference(tester, Type, s0, s1, s0, s1)
    test_difference(tester, Type, s1, s1, s0, s0)
    test_difference(tester, Type, s1, s2, s0, s9)
    test_difference(tester, Type, s9, s1, s9, s1)

    test_intersection(tester, Type, s0, s0, s0)
    test_intersection(tester, Type, s0, s1, s0)
    test_intersection(tester, Type, s1, s1, s1)
    test_intersection(tester, Type, s1, s2, s1)
    test_intersection(tester, Type, s1, s9, s0)
    test_intersection(tester, Type, s2, s9, s9)


    if hascomplement:
        stmp = s1.complement()
        tester.test_value("Complement", stmp == s9, True)
        stmp = s0.complement()
        tester.test_value("Complement of null set", stmp == s2, True)
        stmp = s2.complement()
        tester.test_value("Complement of full set", stmp == s0, True)


    #                                  ispsub, issub,  ispsup, issup
    test_subset_superset(tester, s0, s0, False,  True,   False,  True)
    test_subset_superset(tester, s0, s1, True,   True,   False,  False)
    test_subset_superset(tester, s1, s1, False,  True,   False,  True)
    test_subset_superset(tester, s1, s2, True,   True,   False,  False)
    test_subset_superset(tester, s1, s2, True,   True,   False,  False)
    test_subset_superset(tester, s2, s9, False,  False,  True,   True)

