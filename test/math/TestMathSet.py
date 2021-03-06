import pulsar as psr
    
def run_test():
    tester=psr.PyTester("Testing MathSet and MathSet Iterator C++ Interface")
    
    U1=psr.DoubleUniverse([1.0,2.0,3.0])
    U2, U3=psr.DoubleUniverse([3.0,4.0,5.0]),psr.DoubleUniverse([3.0])
    U4=psr.DoubleUniverse([9.0,51.0,100.0])
    #Constructors
    S1=psr.DoubleSet(U1,{2});S2=psr.DoubleSet(S1)
    tester.test_equal("Constructor 1 and copy constructor work",S1,S2)
    U1.insert(4.0);
    tester.test_equal("Shallow copy of universe",S1,S2)
    S3,S4=psr.DoubleSet(U2,True),psr.DoubleSet(U2,False)
    tester.test_return("Constructor 2",True,False,S3.__eq__,S4)
    B2,B3=psr.DoubleSet(S1,True),psr.DoubleSet(U1,True)
    tester.test_return("Copy and fill works",True,True,B2.__eq__,B3)
    S5=psr.DoubleSet(U2,{0,1,2})
    tester.test_equal("Fill constructor works",S3,S5);
    S8=psr.DoubleSet(S2.clone())
    U1.insert(5.0)
    tester.test_return("Clone is not aliased",True,False,S8.__eq__,S2)    
        
    #Access and manipulation
    tester.test_return("Get universe works",True,U1,S1.get_universe)
    tester.test_return("As universe",True,U3,S1.as_universe)
    tester.test_return("Size works",True,1,S8.size)
    tester.test_return("count element false",True,False,S2.count,15.0)
    tester.test_return("count element true",True,True,S3.count,3.0)
    tester.test_return("count index false",True,False,S2.count_idx,15)
    tester.test_return("count index true",True,True,S3.count_idx,0)
    vals=[3.0,4.0,5.0]
    itercheck=[i for i in S5]
    tester.test_equal("iterators work",vals,itercheck)
    tester.test_return("idx valid",True,0,S3.idx,3.0)
    tester.test_call("idx invalid",False,S8.idx,55.0)
    S11=psr.DoubleSet(U1,{2,3})
    tester.test_return("insert by valid elem",True,S11,S1.insert,4.0)
    tester.test_call("insert by invalid elem",False,S1.insert,55.0)
    tester.test_return("insert by valid index",True,S11,S2.insert_idx,3)
    tester.test_call("insert by invalid index",False,S2.insert_idx,99)
    
    S9=psr.DoubleSet(U1,{1,2,3})
    S10=psr.DoubleSet(U1,{1,2,4})
    S12=psr.DoubleSet(U1,{1,2})
    S13=psr.DoubleSet(U1,{3})
    S99=psr.DoubleSet(U4,{1})
    
    tester.test_return("union",True,S9,S1.set_union,S12)
    tester.test_call("union fail",False,S1.set_union,S99)
    tester.test_return("union assign",True,S9,S1.union_assign,S12)
    tester.test_call("union assign fail",False,S1.union_assign,S99)
    tester.test_return("intersection",True,S12,S1.intersection,S10)
    tester.test_call("intersection fail",False,S1.intersection,S99)
    tester.test_return("intersection assign",True,S12,S1.intersection_assign,S10)
    tester.test_call("intersection assign fail",False,S1.intersection_assign,S99)
    tester.test_return("difference",True,S13,S2.difference,S12)
    tester.test_call("difference fail",False,S2.difference,S99)
    tester.test_return("difference assign",True,S13,S2.difference_assign,S12)
    tester.test_call("difference assign fail",False,S2.difference_assign,S99)
    S14=psr.DoubleSet(U1,True)
    S14-=S2
    tester.test_return("complement",True,S14,S2.complement);
    
    #Set comparisons
    tester.test_return("subset equal",True,True,S2.is_subset_of,S13)
    tester.test_return("subset true",True,True,S2.is_subset_of,S9)
    tester.test_return("subset false",True,False,S9.is_subset_of,S2)
    tester.test_return("proper subset equal",True,False,S2.is_proper_subset_of,S13)
    tester.test_return("proper subset true",True,True,S2.is_proper_subset_of,S9)
    tester.test_return("proper subset false",True,False,S9.is_proper_subset_of,S2)
    tester.test_return("superset equal",True,True,S2.is_superset_of,S13)
    tester.test_return("superset true",True,True,S9.is_superset_of,S2)
    tester.test_return("superset false",True,False,S2.is_superset_of,S9)
    tester.test_return("proper superset equal",True,False,S2.is_proper_superset_of,S13)
    tester.test_return("proper superset true",True,True,S9.is_proper_superset_of,S2)
    tester.test_return("proper superset false",True,False,S2.is_proper_superset_of,S9)
    tester.test_return("not equal",True,True,S2.__ne__,S14)
        
    #Manipulations    
    transresults=[4.0,6.0]
    def transfxn(in_val):
         return 2.0*in_val
    NewS=S1.transform(transfxn)
    tresults2=[i for i in NewS]
    tester.test_equal("transform works",transresults,tresults2)
    def partfxn(in_val):
        return in_val==2.0    
    NewS2=S1.partition(partfxn)
    partresults=[2.0]
    presults2=[i for i in NewS2]
    tester.test_equal("partition works",partresults,presults2)
    
    tester.test_return("hash works check 1",True,S2.my_hash(),S13.my_hash)
    
    S2.clear()
    tester.test_return("clear works",True,0,S2.size)

    tester.print_results()
    return tester.nfailed()
