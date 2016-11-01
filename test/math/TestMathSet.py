from TestFxns import *
    
tester=Tester("Testing MathSet and MathSet Iterator C++ Interface")


U1=psr.DoubleUniverse([1.0,2.0,3.0])
U2, U3=psr.DoubleUniverse([3.0,4.0,5.0]),psr.DoubleUniverse([3.0])

#Constructors
S1=psr.DoubleSet(U1,{2});S2=psr.DoubleSet(S1)
tester.test_value("Constructor 1 and copy constructor work",S1,S2)
U1.insert(4.0);
tester.test_value("Shallow copy of universe",S1,S2)
S3,S4=psr.DoubleSet(U2,True),psr.DoubleSet(U2,False)
tester.test("Constructor 2",True,False,S3.__eq__,S4)
B2,B3=psr.DoubleSet(S1,True),psr.DoubleSet(U1,True)
tester.test("Copy and fill works",True,True,B2.__eq__,B3)
S5=psr.DoubleSet(U2,{0,1,2})
tester.test_value("Fill constructor works",S3,S5);
S8=psr.DoubleSet(S2.clone())
U1.insert(5.0)
tester.test("Clone is not aliased",True,False,S8.__eq__,S2)    
    
#Access and manipulation
tester.test("Get universe works",True,U1,S1.get_universe)
tester.test("As universe",True,U3,S1.as_universe)
tester.test("Size works",True,1,S8.size)
tester.test("count element false",True,False,S2.count,15.0)
tester.test("count element true",True,True,S3.count,3.0)
tester.test("count index false",True,False,S2.count_idx,15)
tester.test("count index true",True,True,S3.count_idx,0)
vals=[3.0,4.0,5.0]
itercheck=[i for i in S5]
tester.test_value("iterators work",vals,itercheck)
tester.test("idx valid",True,0,S3.idx,3.0)
tester.test("idx invalid",False,2,S8.idx,55.0)
S11=psr.DoubleSet(U1,{2,3})
tester.test("insert by valid elem",True,S11,S1.insert,4.0)
tester.test("insert by invalid elem",False,S11,S1.insert,55.0)
tester.test("insert by valid index",True,S11,S2.insert_idx,3)
tester.test("insert by invalid index",False,S11,S2.insert_idx,99)

S9=psr.DoubleSet(U1,{1,2,3})
S10=psr.DoubleSet(U1,{1,2,4})
S12=psr.DoubleSet(U1,{1,2})
S13=psr.DoubleSet(U1,{3})
tester.test("union",True,S9,S1.set_union,S12)
tester.test("union fail",False,S9,S1.set_union,S4)
tester.test("union assign",True,S9,S1.union_assign,S12)
tester.test("union assign fail",False,S9,S1.union_assign,S4)
tester.test("intersection",True,S12,S1.intersection,S10)
tester.test("intersection fail",False,S2,S1.intersection,S4)
tester.test("intersection assign",True,S12,S1.intersection_assign,S10)
tester.test("intersection assign fail",False,S2,S1.intersection_assign,S4)
tester.test("difference",True,S13,S2.difference,S12)
tester.test("difference fail",False,S13,S2.difference,S4)
tester.test("difference assign",True,S13,S2.difference_assign,S12)
tester.test("difference assign fail",False,S13,S2.difference_assign,S4)
S14=psr.DoubleSet(U1,True)
S14-=S2
tester.test("complement",True,S14,S2.complement);

#Set comparisons
tester.test("subset equal",True,True,S2.is_subset_of,S13)
tester.test("subset true",True,True,S2.is_subset_of,S9)
tester.test("subset false",True,False,S9.is_subset_of,S2)
tester.test("proper subset equal",True,False,S2.is_proper_subset_of,S13)
tester.test("proper subset true",True,True,S2.is_proper_subset_of,S9)
tester.test("proper subset false",True,False,S9.is_proper_subset_of,S2)
tester.test("superset equal",True,True,S2.is_superset_of,S13)
tester.test("superset true",True,True,S9.is_superset_of,S2)
tester.test("superset false",True,False,S2.is_superset_of,S9)
tester.test("proper superset equal",True,False,S2.is_proper_superset_of,S13)
tester.test("proper superset true",True,True,S9.is_proper_superset_of,S2)
tester.test("proper superset false",True,False,S2.is_proper_superset_of,S9)
tester.test("not equal",True,True,S2.__ne__,S14)
    
#Manipulations    
transresults=[4.0,6.0]
def transfxn(in_val):
     return 2.0*in_val
NewS=S1.transform(transfxn)
tresults2=[i for i in NewS]
tester.test_value("transform works",transresults,tresults2)
def partfxn(in_val):
    return in_val==2.0    
NewS2=S1.partition(partfxn)
partresults=[2.0]
presults2=[i for i in NewS2]
tester.test_value("partition works",partresults,presults2)

tester.test("hash works check 1",True,S2.my_hash(),S13.my_hash)

S2.clear()
tester.test("clear works",True,0,S2.size)
tester.print_results()
