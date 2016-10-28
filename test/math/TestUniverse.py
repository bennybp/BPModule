from TestFxns import *


tester = Tester("Testing Universe Python Interface")

#Constructors and assignment
U0=psr.math.DoubleUniverse()
U1=psr.math.DoubleUniverse()
tester.test_value("Default constructor",U0,U1)
U2=psr.math.DoubleUniverse([1.0,2.0,3.0])
U3=psr.math.DoubleUniverse([1.0,2.0,3.0])
tester.test_value("Variadic initializer",U2,U3);
U4=psr.math.DoubleUniverse(U3);
tester.test_value("Copy constructor",U3,U4);
U0=U2;
tester.test_value("Assignment",U0,U3);

#Basic properties
tester.test("Size",True,3,U0.size)
tester.test("Count",True,True,U0.count,1.0)
tester.test("Get index",True,2,U3.idx,3.0)
tester.test("Get non-existant index",False,2,U3.idx,5.0)

tester.test("Get hash U0",True,U0.my_hash(),U3.my_hash)
tester.test("Get hash U3",True,U0.my_hash(),U4.my_hash)
    
#Element access/modification
tester.test("Subscript operator",True,3.0,U0.__getitem__,2)
tester.test("Subscript operator (out of range)",False,3.0,U0.__getitem__,9)
tester.test("at function",True,3.0,U0.at,2)
tester.test("at function (out of range)",False,3.0,U0.at,9)
U0.insert(4.0)
tester.test("insert elements",True,U0,U3.insert,4.0)
#U0=U3=1-4; U1=empty; U2=U4=1-3

#Set operations
U5=psr.math.DoubleUniverse([4.0,5.0,6.0])
U8=psr.math.DoubleUniverse([1.0,2.0,3.0,4.0,5.0,6.0])
tester.test("union assign",True,U8,U0.union_assign,U5)    
tester.test("union",True,U8,U3.set_union,U5)
U9=psr.math.DoubleUniverse([1.0,2.0])
U10=psr.math.DoubleUniverse([1.0,2.0,15.0,16.0])
tester.test("intersection assign",True,U9,U0.intersection_assign,U10)
print(U10)
tester.test("intersection",True,U9,U3.intersection,U10)
U11=psr.math.DoubleUniverse([3.0,4.0,5.0,6.0])
tester.test("difference",True,U11,U8.difference,U9)
tester.test("difference assign",True,U11,U8.difference_assign,U9)
    
#Comparison operators
tester.test("not equal",True,True,U9.__ne__,U11)
tester.test("superset equal",True,True,U11.is_superset_of,U8)
tester.test("superset true",True,True,U10.is_superset_of,U9)
tester.test("superset false",True,False,U9.is_superset_of,U10)
tester.test("proper superset equal",True,False,U11.is_proper_superset_of,U8)
tester.test("proper supserset true",True,True,U10.is_proper_superset_of,U9)
tester.test("proper superset false",True,False,U9.is_proper_superset_of,U10)
tester.test("subset equal",True,True,U11.is_subset_of,U8)
tester.test("subset true",True,True,U9.is_subset_of,U10)
tester.test("subset false",True,False,U10.is_subset_of,U9)
tester.test("proper subset equal",True,False,U11.is_proper_subset_of,U8)
tester.test("proper subset true",True,True,U9.is_proper_subset_of,U10)
tester.test("proper subset false",True,False,U10.is_proper_subset_of,U9)

tester.print_results()

psr.finalize()
