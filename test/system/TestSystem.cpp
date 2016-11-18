#include "CppTest.hpp"
#include <pulsar/system/System.hpp>
using namespace pulsar;
using shared_universe=std::shared_ptr<const AtomSetUniverse>;

TEST_SIMPLE(TestSystem){
    CppTester tester("Testing the System class");
    
    AtomSetUniverse MyU,MyU2;
    Atom H=create_atom({0.0,0.0,0.0},1),H1=create_atom({0.0,0.0,0.89},1);
    MyU.insert(H);
    MyU.insert(H1);
    MyU2=MyU;
    
    //Constructors and assignment
    System Empty(MyU,false),H2(MyU,true),H25(MyU,false),H26(MyU,false);
    tester.test_equal("System is empty",0,Empty.size());
    tester.test_equal("System is full",2,H2.size());
    System Empty2(MyU,false);
    tester.test_equal("Different universes!=different systems",true,Empty==Empty2);
    System H22(std::move(MyU2),true);
    tester.test_equal("Move universe constructor works",2,H22.size());
    System H23(H22);
    tester.test_equal("Copy constructor works",true,H23==H22);
    System E2(H23,true);
    tester.test_equal("Copy atoms and fill works",true,E2==H23);
    System H24(std::move(H22));
    tester.test_equal("Move constructor works",true,H23==H24);
    H2=H24;
    tester.test_equal("Assignment works",true,H24==H2);
    H22=std::move(H23);
    tester.test_equal("Move assignment works",true,H24==H22);
    //At this point H2==H22==H24 and H23 is invalid (and they are H2 molecule)
    
    //Properties
    Atom q=make_point_charge({0.0,0.0,-0.89},-1.0);
    MyU.insert(q);
    System ChargedH2(MyU,true);
    tester.test_equal("size works",2,H2.size());
    tester.test_equal("sum of mass works",2.01595,H24.get_sum_mass());
    tester.test_equal("mass is set correct",2.01595,H24.mass);
    tester.test_equal("sum of charge works (non-charged)",0,H22.get_sum_charge());
    tester.test_equal("charge is set correctly (non-charged)",0,H22.charge);
    tester.test_equal("sum charges works (charged)",-1.0,ChargedH2.get_sum_charge());
    tester.test_equal("charge is set correctly (charged)",-1.0,ChargedH2.charge);
    tester.test_equal("multiplicity is correct",1,H2.multiplicity);
    tester.test_equal("sum of n electrons is correct",2,H24.get_sum_n_electrons());
    tester.test_equal("n electrons is set correctly",2,H24.nelectrons);
    tester.test_equal("spaces are the same",Space(),H22.space);
    tester.test_equal("count works",true,H22.count(H1));
    tester.test_equal("compare info works",true,H22.compare_info(H24));
    
    //Access and filling
    std::vector<Atom> HAtoms(H2.begin(),H2.end()),corr_atoms({H,H1});
    tester.test_equal("Iterators work",HAtoms,corr_atoms);
    H26.insert(H);H26.insert(H26.end(),H1);
    tester.test_equal("Single element inserts work",H26,H24);
    H25.insert(HAtoms.begin(),HAtoms.end());
    tester.test_equal("range insert works",H25,H22);
    Atom q2=q;
    System H27(MyU,false),H28(MyU,false);
    H27.insert(HAtoms.begin(),HAtoms.end());
    System H210(H27);
    H27.insert(std::move(q));
    tester.test_equal("move insert works",H27,ChargedH2);
    H28.insert(H28.end(),std::move(q2));
    tester.test_equal("other move insert works",1,H28.size());
    System H29(H28);
    
    //At this point:
    //Original U: H2=H22==H24==H25==H26={H,H1}
    //New U: H210={H,H1}    H27==ChargedH2={H,H1,q}  H28==H29=={q}
    
    //Set operations 
    tester.test_equal("intersection works",H28,H27.intersection(H28));
    tester.test_equal("intersection assignment",H28,H27.intersection_assign(H28));
    tester.test_equal("union works",ChargedH2,H27.set_union(H210));
    tester.test_equal("union assignment works",ChargedH2,H27.union_assign(H210));
    tester.test_equal("difference works",H210,H27.difference(H29));
    tester.test_equal("difference assign works",H210,H27.difference_assign(H29));
    tester.test_equal("complement works",H210,H29.complement());
    
    //At this point:
    //Original U: H2=H22==H24==H25==H26={H,H1}
    //New U: H27==H210={H,H1}    ChargedH2={H,H1,q}  H28==H29=={q}
    
    tester.test_equal("subset equal",true,H27.is_subset_of(H210));
    tester.test_equal("subset true",true,H27.is_subset_of(ChargedH2));
    tester.test_equal("subset false",false,H27.is_subset_of(H28));
    tester.test_equal("superset equal",true,H27.is_superset_of(H210));
    tester.test_equal("superset true",true,ChargedH2.is_superset_of(H27));
    tester.test_equal("superset false",false,H28.is_superset_of(H27));
    tester.test_equal("proper subset equal",false,H27.is_proper_subset_of(H210));
    tester.test_equal("proper subset true",true,H27.is_proper_subset_of(ChargedH2));
    tester.test_equal("proper subset false",false,H27.is_proper_subset_of(H28));
    tester.test_equal("proper superset equal",false,H27.is_proper_superset_of(H210));
    tester.test_equal("proper superset true",true,ChargedH2.is_proper_superset_of(H27));
    tester.test_equal("proper superset false",false,H210.is_proper_superset_of(H27));
    tester.test_equal("inequality works",true,H22!=H28);
    tester.test_equal("equality across universes works",H22,H27);
    
    tester.test_equal("partition",H28,ChargedH2.partition([](const Atom& a){
        return is_point_charge(a);}));
    AtomSetUniverse MyU3(create_atom({1.0,0.0,0.0},1),create_atom({1.0,0.0,0.89},1));
    tester.test_equal("transform",System(MyU3,true),H22.transform([](const Atom& a){
        Atom temp(a);temp[0]=1.0;return temp;
    }));
    tester.test_equal("get universe works",MyU,*H27.get_universe());
    tester.test_equal("as universe works",MyU,ChargedH2.as_universe());

    //Despite the manipulations and different universes, the following sets are
    //all equal
    tester.test_equal("Hash is correct 1",H22.my_hash(),H24.my_hash());
    tester.test_equal("Hash is correct 2",H22.my_hash(),H2.my_hash());
    tester.test_equal("Hash is correct 3",H22.my_hash(),H27.my_hash());
    tester.test_equal("Hash is correct 4",H22.my_hash(),H210.my_hash());

    H27.clear();
    tester.test_equal("Clear works",0,H27.size());

    tester.print_results();
    return tester.nfailed();
    
}
