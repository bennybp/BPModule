#include <pulsar/testing/CppTester.hpp>
#include <pulsar/modulebase/SystemFragmenter.hpp>

using namespace pulsar;

TEST_SIMPLE(TestNMerInfo){
    CppTester tester("Testing NMerInfo.");

    AtomSetUniverse MyU;
    Atom H=create_atom({0.0,0.0,0.0},1),H1=create_atom({0.0,0.0,0.89},1);
    MyU.insert(H);
    MyU.insert(H1);
    System H2(MyU,true);
    SNType SN({1,2,3});

    NMerInfo NI1,NI3;
    NI1.sn=SN;
    NI1.nmer=H2;
    NI1.weight=2.0;

    tester.test_equal("Serial number set",NI1.sn,SN);
    tester.test_equal("System is set",NI1.nmer,H2);
    tester.test_double("Weight is set",NI1.weight,2.0);
    NMerInfo NI2(NI1);
    tester.test_equal("Serial number is copied",NI2.sn,SN);
    tester.test_equal("System is copied",NI2.nmer,H2);
    tester.test_double("Weight is copied",NI2.weight,2.0);
    tester.test_equal("Equality works",NI1,NI2);
    tester.test_not_equal("Inequlaity works",NI1,NI3);

    tester.print_results();
    return tester.nfailed();

}
