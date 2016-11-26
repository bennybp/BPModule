import pulsar as psr

def run_test():
    tester = psr.PyTester("Testing Universe Python Interface")

    #Constructors and assignment
    U0, U1=psr.DoubleUniverse(),psr.DoubleUniverse()
    tester.test_equal("Default constructor",U0,U1)
    U2=psr.DoubleUniverse([1.0,2.0,3.0])
    U3=psr.DoubleUniverse([1.0,2.0,3.0])
    tester.test_equal("Variadic initializer",U2,U3);
    U4=psr.DoubleUniverse(U3);
    tester.test_equal("Copy constructor",U3,U4);
    U0=U2;
    tester.test_equal("Assignment",U0,U3);

    #Basic properties
    tester.test_return("Size",True,3,U0.size)
    tester.test_return("Count",True,True,U0.count,1.0)
    tester.test_return("Get index",True,2,U3.idx,3.0)
    tester.test_call("Get non-existant index",False,U3.idx,5.0)

    tester.test_return("Get hash U0",True,U0.my_hash(),U3.my_hash)
    tester.test_return("Get hash U3",True,U0.my_hash(),U4.my_hash)
        
    #Element access/modification
    tester.test_return("Subscript operator",True,3.0,U0.__getitem__,2)
    tester.test_call("Subscript operator (out of range)",False,U0.__getitem__,9)
    tester.test_return("at function",True,3.0,U0.at,2)
    tester.test_call("at function (out of range)",False,U0.at,9)
    U0.insert(4.0)
    tester.test_return("insert elements",True,U0,U3.insert,4.0)
    #U0=U3=1-4; U1=empty; U2=U4=1-3

    #Set operations
    U5=psr.DoubleUniverse([4.0,5.0,6.0])
    U8=psr.DoubleUniverse([1.0,2.0,3.0,4.0,5.0,6.0])
    tester.test_return("union assign",True,U8,U0.union_assign,U5)    
    tester.test_return("union",True,U8,U3.set_union,U5)
    U9=psr.DoubleUniverse([1.0,2.0])
    U10=psr.DoubleUniverse([1.0,2.0,15.0,16.0])
    tester.test_return("intersection assign",True,U9,U0.intersection_assign,U10)
    print(U10)
    tester.test_return("intersection",True,U9,U3.intersection,U10)
    U11=psr.DoubleUniverse([3.0,4.0,5.0,6.0])
    tester.test_return("difference",True,U11,U8.difference,U9)
    tester.test_return("difference assign",True,U11,U8.difference_assign,U9)
        
    #Comparison operators
    tester.test_return("not equal",True,True,U9.__ne__,U11)
    tester.test_return("superset equal",True,True,U11.is_superset_of,U8)
    tester.test_return("superset true",True,True,U10.is_superset_of,U9)
    tester.test_return("superset false",True,False,U9.is_superset_of,U10)
    tester.test_return("proper superset equal",True,False,U11.is_proper_superset_of,U8)
    tester.test_return("proper supserset true",True,True,U10.is_proper_superset_of,U9)
    tester.test_return("proper superset false",True,False,U9.is_proper_superset_of,U10)
    tester.test_return("subset equal",True,True,U11.is_subset_of,U8)
    tester.test_return("subset true",True,True,U9.is_subset_of,U10)
    tester.test_return("subset false",True,False,U10.is_subset_of,U9)
    tester.test_return("proper subset equal",True,False,U11.is_proper_subset_of,U8)
    tester.test_return("proper subset true",True,True,U9.is_proper_subset_of,U10)
    tester.test_return("proper subset false",True,False,U10.is_proper_subset_of,U9)

    tester.print_results()
    return tester.nfailed()
