#include "CXXTest.hpp"
#include <pulsar/system/Atom.hpp>

using namespace pulsar;

TEST_SIMPLE(TestAtom){
    Tester tester("Testing the Atom class");
    
    Atom H=create_atom({0.0,0.0,0.0},1);
    Atom H2=create_atom({0.0,0.0,0.0},1,1);
    tester.test("create_atom works",H,H2);
    tester.test("correct Z",1,H.Z);
    tester.test("correct isotope",1,H.isotope);
    tester.test("correct mass",1.007975,H.mass);
    tester.test("correct isotope mass",1.0078250322,H.isotope_mass);
    tester.test("correct charge",0,H.charge);
    tester.test("correct multiplicity",2,H.multiplicity);
    tester.test("correct nelectrons",1,H.nelectrons);
    tester.test("correct covalent radius",0.5858150988919267,H.cov_radius);
    tester.test("correct vDW radius",2.267671350549394,H.vdw_radius);
    Atom H3(H2);
    tester.test("copy constructor works",H,H3);
    Atom H4(std::move(H3));
    tester.test("move constructor works",H,H4);
        
    Atom D=create_atom({0.0,0.0,0.0},1,2);
    tester.test("Isotopes work",2,D.isotope);
    tester.test("Isotopes are different",true,D!=H);
    
    D=H4;
    tester.test("assignment works",D,H);
    Atom U=create_atom({0.0,0.0,0.0},92);
    U=std::move(H4);
    tester.test("move assignment works",U,H);

    tester.test("hash works",H.my_hash(),U.my_hash());
    tester.test("hash works 1",H.my_hash(),H2.my_hash());
    tester.test("hash works 2",H.my_hash(),D.my_hash());

    
    Atom GH=make_ghost_atom(H2);
    tester.test("ghost works",true,is_ghost_atom(GH));
    Atom q=make_point_charge(H2,3.3),q2=make_point_charge(H2.get_coords(),3.3);
    tester.test("point charges work",true,is_point_charge(q));
    tester.test("point charges work 2",true,is_point_charge(q2));
    tester.test("is same point charge",q,q2);
    
    Atom Dm=make_dummy_atom(H),Dm2=make_dummy_atom(H.get_coords());
    tester.test("is dummy atom",true,is_dummy_atom(Dm));
    tester.test("is dummy atom 2",true,is_dummy_atom(Dm2));
    tester.test("is same dummy atom",Dm,Dm2);

    tester.print_results();
}
