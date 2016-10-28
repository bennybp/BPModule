#include "TestCXX.hpp"
#include <pulsar/system/System.hpp>
using namespace pulsar;
using shared_universe=std::shared_ptr<const AtomSetUniverse>;

TEST_CLASS(TestSystem){
    Tester tester("Testing the System class");
    
    AtomSetUniverse MyU,MyU2;
    Atom H=create_atom({0.0,0.0,0.0},1),H1=create_atom({0.0,0.0,0.89},1);
    MyU.insert(H);
    MyU.insert(H1);
    MyU2=MyU;
    
    //Constructors and assignment
    System Empty(MyU,false),H2(MyU,true),H25(MyU,false),H26(MyU,false);
    tester.test("System is empty",0,Empty.size());
    tester.test("System is full",2,H2.size());
    System Empty2(MyU,false);
    tester.test("Different universes!=different systems",true,Empty==Empty2);
    System H22(std::move(MyU2),true);
    tester.test("Move universe constructor works",2,H22.size());
    System H23(H22);
    tester.test("Copy constructor works",true,H23==H22);
    System H24(std::move(H22));
    tester.test("Move constructor works",true,H23==H24);
    H2=H24;
    tester.test("Assignment works",true,H24==H2);
    H22=std::move(H23);
    tester.test("Move assignment works",true,H24==H22);
    //At this point H2==H22==H24 and H23 is invalid (and they are H2 molecule)
    
    //Properties
    Atom q=make_point_charge({0.0,0.0,-0.89},-1.0);
    MyU.insert(q);
    System ChargedH2(MyU,true);
    tester.test("size works",2,H2.size());
    tester.test("sum of mass works",2.01595,H24.get_sum_mass());
    tester.test("mass is set correct",2.01595,H24.mass);
    tester.test("sum of charge works (non-charged)",0,H22.get_sum_charge());
    tester.test("charge is set correctly (non-charged)",0,H22.charge);
    tester.test("sum charges works (charged)",-1.0,ChargedH2.get_sum_charge());
    tester.test("charge is set correctly (charged)",-1.0,ChargedH2.charge);
    tester.test("multiplicity is correct",1,H2.multiplicity);
    tester.test("sum of n electrons is correct",2,H24.get_sum_n_electrons());
    tester.test("n electrons is set correctly",2,H24.nelectrons);
    tester.test("spaces are the same",Space(),H22.space);
    tester.test("count works",true,H22.count(H1));
    tester.test("compare info works",true,H22.compare_info(H24));
    
    //Access and filling
    std::vector<Atom> HAtoms(H2.begin(),H2.end()),corr_atoms({H,H1});
    tester.test("Iterators work",HAtoms,corr_atoms);
    H26.insert(H);H26.insert(H26.end(),H1);
    tester.test("Single element inserts work",H26,H24);
    H25.insert(HAtoms.begin(),HAtoms.end());
    tester.test("range insert works",H25,H22);
    Atom q2=q;
    System H27(MyU,false),H28(MyU,false);
    H27.insert(HAtoms.begin(),HAtoms.end());
    System H210(H27);
    H27.insert(std::move(q));
    tester.test("move insert works",H27,ChargedH2);
    H28.insert(H28.end(),std::move(q2));
    tester.test("other move insert works",1,H28.size());
    System H29(H28);
    
    //At this point:
    //Original U: H2=H22==H24==H25==H26={H,H1}
    //New U: H210={H,H1}    H27==ChargedH2={H,H1,q}  H28==H29=={q}
    
    //Set operations 
    tester.test("intersection works",H28,H27.intersection(H28));
    tester.test("intersection assignment",H28,H27.intersection_assign(H28));
    tester.test("union works",ChargedH2,H27.set_union(H210));
    tester.test("union assignment works",ChargedH2,H27.union_assign(H210));
    tester.test("difference works",H210,H27.difference(H29));
    tester.test("difference assign works",H210,H27.difference_assign(H29));
    tester.test("complement works",H210,H29.complement());
    
    //At this point:
    //Original U: H2=H22==H24==H25==H26={H,H1}
    //New U: H27==H210={H,H1}    ChargedH2={H,H1,q}  H28==H29=={q}
    
    tester.test("subset equal",true,H27.is_subset_of(H210));
    tester.test("subset true",true,H27.is_subset_of(ChargedH2));
    tester.test("subset false",false,H27.is_subset_of(H28));
    tester.test("superset equal",true,H27.is_superset_of(H210));
    tester.test("superset true",true,ChargedH2.is_superset_of(H27));
    tester.test("superset false",false,H28.is_superset_of(H27));
    tester.test("proper subset equal",false,H27.is_proper_subset_of(H210));
    tester.test("proper subset true",true,H27.is_proper_subset_of(ChargedH2));
    tester.test("proper subset false",false,H27.is_proper_subset_of(H28));
    tester.test("proper superset equal",false,H27.is_proper_superset_of(H210));
    tester.test("proper superset true",true,ChargedH2.is_proper_superset_of(H27));
    tester.test("proper superset false",false,H210.is_proper_superset_of(H27));
    tester.test("inequality works",true,H22!=H28);
    tester.test("equality across universes works",H22,H27);
    
    tester.test("partition",H28,ChargedH2.partition([](const Atom& a){
        return is_point_charge(a);}));
    AtomSetUniverse MyU3(create_atom({1.0,0.0,0.0},1),create_atom({1.0,0.0,0.89},1));
    tester.test("transform",System(MyU3,true),H22.transform([](const Atom& a){
        Atom temp(a);temp[0]=1.0;return temp;
    }));
    tester.test("get universe works",MyU,*H27.get_universe());
    tester.test("as universe works",MyU,ChargedH2.as_universe());

    //Despite the manipulations and different universes, the following sets are
    //all equal
    tester.test("Hash is correct 1",H22.my_hash(),H24.my_hash());
    tester.test("Hash is correct 2",H22.my_hash(),H2.my_hash());
    tester.test("Hash is correct 3",H22.my_hash(),H27.my_hash());
    tester.test("Hash is correct 4",H22.my_hash(),H210.my_hash());
        
    tester.print_results();
    
}
