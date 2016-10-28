from TestFxns import *
import math

tester=Tester("Testing the System class")
    
MyU, MyU2=psr.AtomSetUniverse(), psr.AtomSetUniverse()
H, H1 = psr.create_atom([0.0,0.0,0.0],1), psr.create_atom([0.0,0.0,0.89],1)
MyU.insert(H)
MyU.insert(H1)
    
#Constructors and assignment
Empty=psr.System(MyU,False)
H2=psr.System(MyU,True)
tester.test("System is empty",True,0,Empty.size)
tester.test("System is full",True,2,H2.size)
Empty2=psr.System(MyU,False)
tester.test("Different universes!=different systems",True,True,Empty.__eq__,Empty2)
H22=psr.System(H2)
tester.test("Copy constructor works",True,True,H2.__eq__,H22)

#At this point H2==H22={H,H1}
    
#Properties
q=psr.make_point_charge([0.0,0.0,-0.89],-1.0)
MyU.insert(q)
ChargedH2=psr.System(MyU,True)
tester.test("size works",True,2,H2.size)
tester.test("sum of mass works",True,2.01595,H22.get_sum_mass)
tester.test_value("mass is set correct",2.01595,H22.mass)
tester.test("sum of charge works (non-charged)",True,0,H2.get_sum_charge)
tester.test_value("charge is set correctly (non-charged)",0,H2.charge)
tester.test("sum charges works (charged)",True,-1.0,ChargedH2.get_sum_charge)
tester.test_value("charge is set correctly (charged)",-1.0,ChargedH2.charge)
tester.test_value("multiplicity is correct",1,H2.multiplicity)
tester.test("sum of n electrons is correct",True,2,H22.get_sum_n_electrons)
tester.test_value("n electrons is set correctly",2,H22.nelectrons)
tester.test_value("spaces are the same",psr.Space(),H22.space)
tester.test("count works",True,True,H22.count,H1)
tester.test("compare info works",True,True,H22.compare_info,H2)
    
#Access and filling
HAtoms=[i for i in H22]
corr_atoms=[H,H1]
tester.test_value("Iterators work",HAtoms,corr_atoms)
H26, H27 =psr.System(MyU,False), psr.System(MyU,False)
H26.insert(H)
H26.insert(H1)
H27.insert(q)
tester.test_value("Single element inserts work",H26,H22)
H28=psr.System(ChargedH2)
#At this point H2=H22={H,H1} | H28==ChargedH2={H,H1,q} H26={H,H1} H27={q}
    
#Set operations 
tester.test("intersection works",True,H2,ChargedH2.intersection,H26)
tester.test("intersection assignment",True,H2,ChargedH2.intersection_assign,H26)
tester.test("union works",True,H28,ChargedH2.set_union,H27)
tester.test("union assignment works",True,H28,ChargedH2.union_assign,H27)
tester.test("difference works",True,H26,ChargedH2.difference,H27)
tester.test("difference assign works",True,H26,ChargedH2.difference_assign,H27)
tester.test("complement works",True,H26,H27.complement)
    
#At this point H2=H22={H,H1}| ChargedH2==H26={H,H1} H27={q} H28={H,H1,q}
    
tester.test("subset equal",True,True,H2.is_subset_of,H22)
tester.test("subset true",True,True,H27.is_subset_of,H28)
tester.test("subset false",True,False,H28.is_subset_of,H27)
tester.test("superset equal",True,True,H2.is_superset_of,H22)
tester.test("superset true",True,True,H28.is_superset_of,H27)
tester.test("superset false",True,False,H27.is_superset_of,H28)
tester.test("proper subset equal",True,False,H2.is_proper_subset_of,H22)
tester.test("proper subset true",True,True,H27.is_proper_subset_of,H28)
tester.test("proper subset false",True,False,H28.is_proper_subset_of,H27)
tester.test("proper superset equal",True,False,H2.is_proper_superset_of,H22)
tester.test("proper superset true",True,True,H28.is_proper_superset_of,H27)
tester.test("proper superset false",True,False,H27.is_proper_superset_of,H28)
tester.test("inequality works",True,True,H26.__ne__,H28)
    
def part_fxn(atomi):
    return psr.is_point_charge(atomi)

tester.test("partition",True,H27,H28.partition,part_fxn)
MyU3=psr.AtomSetUniverse([psr.create_atom([1.0,0.0,0.0],1),
    psr.create_atom([1.0,0.0,0.89],1)])
    
def translate_by_1(atomi):
    temp=psr.Atom(atomi)
    temp[0]=1.0
    return temp

tester.test("transform",True,psr.System(MyU3,True),H22.transform,translate_by_1)
tester.test("get universe works",True,MyU,H27.get_universe)
tester.test("as universe works",True,MyU,H28.as_universe)

#Despite the manipulations and different universes, the following sets are equal

tester.test("Hash is correct 1",True,H2.my_hash(),H22.my_hash)
tester.test("Hash is correct 2",True,H2.my_hash(),ChargedH2.my_hash)
tester.test("Hash is correct 3",True,H2.my_hash(),H26.my_hash)
        
tester.print_results()
