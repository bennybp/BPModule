/* STO-3G For H is:
H 0
S 3 1.00
    3.42525091 0.15432897
    0.62391373 0.53532814
    0.16885540 0.44463454 
 */
#include "TestCXX.hpp"
#include <pulsar/system/System.hpp>
using std::bind;
using std::move;
using namespace std::placeholders;
using namespace pulsar;
using BSI = BasisShellInfo;

TEST_CLASS(TestBasisShellInfo)
{

    ShellType cGTO = ShellType::CartesianGaussian;
    Tester tester("Testing BasisSetInfo (BSI)");

    Atom H = create_atom({0.0, 0.0, 0.00}, 1), 
        H1 = create_atom({0.0, 0.0, 0.89}, 1);

    std::vector<double> alpha({3.42525091, 0.62391373, 0.16885540}),
    c({0.15432897, 0.53532814, 0.44463454}), alpha2;
    
    //Test BSI    
    bool threw = false;
    try {
        BSI s6(cGTO, 0, 3, 1, alpha2, c);
    }    catch (...) {
        threw = true;
    }
    tester.test("BSI : Constructor check passed", true, threw);
    
    BSI s1(cGTO, 0, 3, 1),
        s2(cGTO, 0, 3, 1, alpha, c), 
        s3(cGTO, 0, 3, 1, move(alpha), move(c));
    tester.test("BSI : Inequality works", true, s1 != s3);
    tester.test("BSI : Copying and moving primitive data works", s2, s3);
    BSI s4(s3), s5(move(s2));
    tester.test("BSI : Copy constructor works", s4, s3);
    tester.test("BSI : Move constructor works", s5, s2);
    s2 = s5;
    tester.test("BSI : Assignment operator works", s2, s5);
    s1 = std::move(s2);
    tester.test("BSI : Move assignment works", s1, s3);
    tester.print_results();
}
