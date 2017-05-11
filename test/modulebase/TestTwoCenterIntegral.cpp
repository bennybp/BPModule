#include <pulsar/testing/CppTester.hpp>
#include <pulsar/modulemanager/ModuleManager.hpp>
#include <pulsar/modulebase/TwoCenterIntegral.hpp>

using namespace std;
using namespace pulsar;

std::vector<double> corr_answer={1.1,2.2,3.3};

class Test2CInt:public TwoCenterIntegral{
public:
    Test2CInt(ID_t id):TwoCenterIntegral(id){}
private:
    void initialize_(unsigned int,
                     const Wavefunction &,
                     const BasisSet &,
                     const BasisSet &)
    {

    }

    const double* calculate_(uint64_t, uint64_t)
    {
        return corr_answer.data();
    }
};

TEST_SIMPLE(TestTwoCenterIntegral){
    CppTester tester("Testing TwoCenterIntegral base class");

    auto mm=make_shared<ModuleManager>();
    mm->load_lambda_module<Test2CInt>("TwoCenterIntegral","test_module");
    auto builder=mm->get_module<Test2CInt>("test_module",0);

    unsigned int deriv=1;
    Wavefunction wf;
    BasisSet bs;

    TwoCenterIntegral* ints=&(*builder);
    tester.test_member_call("Can_call initialize",true,
                            &TwoCenterIntegral::initialize,ints,deriv,wf,bs,bs);

    const double* corr=corr_answer.data();

    tester.test_member_return("Can call calculate",true,corr,
        &TwoCenterIntegral::calculate,ints,deriv,deriv);
    tester.print_results();
    return tester.nfailed();
}
