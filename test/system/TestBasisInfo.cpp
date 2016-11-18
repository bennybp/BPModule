#include "CXXTest.hpp"
#include <pulsar/system/System.hpp>
using namespace pulsar;
using BSI = BasisShellInfo;

TEST_SIMPLE(TestBasisInfo)
{

    ShellType cGTO = ShellType::CartesianGaussian;
    Tester tester("Testing BasisInfo ");

    std::vector<double> alpha({3.42525091, 0.62391373, 0.16885540}),
    c({0.15432897, 0.53532814, 0.44463454});
    
    BSI FakeD(cGTO, 2, 3, 1, alpha, c);
    BasisInfo BI;
    BI.description="My Description";
    BI.shells.push_back(FakeD);
    BasisInfo BI2(BI);
    std::vector<BSI> shells({FakeD});
    tester.test("Description is set","My Description",BI.description);
    tester.test("Shells are set",shells,BI2.shells);
    tester.test("Copy constructor works",BI,BI2);
    tester.test("Inequality works",false,BI!=BI2);
    
    tester.print_results();
}
