from TestFxns import *
import math

def run_test():
    tester=PyTester("Testing the System class")
        
    MyU, MyU2=psr.AtomSetUniverse(), psr.AtomSetUniverse()
    H, H1 = psr.create_atom([0.0,0.0,0.0],1), psr.create_atom([0.0,0.0,0.89],1)
    MyU.insert(H)
    MyU.insert(H1)
        
    #Constructors and assignment
    Empty=psr.System(MyU,False)
    H2=psr.System(MyU,True)
    tester.test_function("System is empty",True,0,Empty.size)
    tester.test_function("System is full",True,2,H2.size)
    Empty2=psr.System(MyU,False)
    tester.test_function("Different universes!=different systems",True,True,Empty.__eq__,Empty2)
    H22=psr.System(H2)
    tester.test_function("Copy constructor works",True,True,H2.__eq__,H22)
    H99=psr.System(H22,True)
    tester.test_function("Copy and fill works",True,True,H99.__eq__,H22)

    #At this point H2==H22={H,H1}
        
    #Properties
    q=psr.make_point_charge([0.0,0.0,-0.89],-1.0)
    MyU.insert(q)
    ChargedH2=psr.System(MyU,True)
    tester.test_function("size works",True,2,H2.size)
    tester.test_function("sum of mass works",True,2.01595,H22.get_sum_mass)
    tester.test_equal("mass is set correct",2.01595,H22.mass)
    tester.test_function("sum of charge works (non-charged)",True,0,H2.get_sum_charge)
    tester.test_equal("charge is set correctly (non-charged)",0,H2.charge)
    tester.test_function("sum charges works (charged)",True,-1.0,ChargedH2.get_sum_charge)
    tester.test_equal("charge is set correctly (charged)",-1.0,ChargedH2.charge)
    tester.test_equal("multiplicity is correct",1,H2.multiplicity)
    tester.test_function("sum of n electrons is correct",True,2,H22.get_sum_n_electrons)
    tester.test_equal("n electrons is set correctly",2,H22.nelectrons)
    tester.test_equal("spaces are the same",psr.Space(),H22.space)
    tester.test_function("count works",True,True,H22.count,H1)
    tester.test_function("compare info works",True,True,H22.compare_info,H2)
        
    #Access and filling
    HAtoms=[i for i in H22]
    corr_atoms=[H,H1]
    tester.test_equal("Iterators work",HAtoms,corr_atoms)
    H26, H27 =psr.System(MyU,False), psr.System(MyU,False)
    H26.insert(H)
    H26.insert(H1)
    H27.insert(q)
    tester.test_equal("Single element inserts work",H26,H22)
    H28=psr.System(ChargedH2)
    #At this point H2=H22={H,H1} | H28==ChargedH2={H,H1,q} H26={H,H1} H27={q}
        
    #Set operations 
    tester.test_function("intersection works",True,H2,ChargedH2.intersection,H26)
    tester.test_function("intersection assignment",True,H2,ChargedH2.intersection_assign,H26)
    tester.test_function("union works",True,H28,ChargedH2.set_union,H27)
    tester.test_function("union assignment works",True,H28,ChargedH2.union_assign,H27)
    tester.test_function("difference works",True,H26,ChargedH2.difference,H27)
    tester.test_function("difference assign works",True,H26,ChargedH2.difference_assign,H27)
    tester.test_function("complement works",True,H26,H27.complement)
        
    #At this point H2=H22={H,H1}| ChargedH2==H26={H,H1} H27={q} H28={H,H1,q}
        
    tester.test_function("subset equal",True,True,H2.is_subset_of,H22)
    tester.test_function("subset true",True,True,H27.is_subset_of,H28)
    tester.test_function("subset false",True,False,H28.is_subset_of,H27)
    tester.test_function("superset equal",True,True,H2.is_superset_of,H22)
    tester.test_function("superset true",True,True,H28.is_superset_of,H27)
    tester.test_function("superset false",True,False,H27.is_superset_of,H28)
    tester.test_function("proper subset equal",True,False,H2.is_proper_subset_of,H22)
    tester.test_function("proper subset true",True,True,H27.is_proper_subset_of,H28)
    tester.test_function("proper subset false",True,False,H28.is_proper_subset_of,H27)
    tester.test_function("proper superset equal",True,False,H2.is_proper_superset_of,H22)
    tester.test_function("proper superset true",True,True,H28.is_proper_superset_of,H27)
    tester.test_function("proper superset false",True,False,H27.is_proper_superset_of,H28)
    tester.test_function("inequality works",True,True,H26.__ne__,H28)
        
    def part_fxn(atomi):
        return psr.is_point_charge(atomi)

    tester.test_function("partition",True,H27,H28.partition,part_fxn)
    MyU3=psr.AtomSetUniverse([psr.create_atom([1.0,0.0,0.0],1),
        psr.create_atom([1.0,0.0,0.89],1)])
        
    def translate_by_1(atomi):
        temp=psr.Atom(atomi)
        temp[0]=1.0
        return temp

    tester.test_function("transform",True,psr.System(MyU3,True),H22.transform,translate_by_1)
    tester.test_function("get universe works",True,MyU,H27.get_universe)
    tester.test_function("as universe works",True,MyU,H28.as_universe)

    #Despite the manipulations and different universes, the following sets are equal

    tester.test_function("Hash is correct 1",True,H2.my_hash(),H22.my_hash)
    tester.test_function("Hash is correct 2",True,H2.my_hash(),ChargedH2.my_hash)
    tester.test_function("Hash is correct 3",True,H2.my_hash(),H26.my_hash)

    H28.clear()
    tester.test_function("Clear works",True,0,H28.size)
            
            
    tester.print_results()
    return tester.nfailed()
