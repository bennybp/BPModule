#include <pulsar/testing/CppTester.hpp>
#include <pulsar/modulemanager/ModuleManager.hpp>
#include <pulsar/modulebase/ThreeCenterIntegral.hpp>

using namespace std;
using namespace pulsar;

std::vector<double> corr_answer={1.1,2.2,3.3};

class Test3CInt:public ThreeCenterIntegral{
public:
    Test3CInt(ID_t id):ThreeCenterIntegral(id){}
private:
    void initialize_(unsigned int,
                     const Wavefunction &,
                     const BasisSet &,
                     const BasisSet &,
                     const BasisSet &)
    {

    }

    HashType my_hash_(unsigned int,
                      const Wavefunction &,
                      const BasisSet &,
                      const BasisSet &,
                      const BasisSet &){
        return "";
    }

    const double* calculate_(uint64_t, uint64_t, uint64_t)
    {
        return corr_answer.data();
    }
};

TEST_SIMPLE(TestThreeCenterIntegral){
    CppTester tester("Testing ThreeCenterIntegral base class");

    auto mm=make_shared<ModuleManager>();
    mm->load_lambda_module<Test3CInt>("ThreeCenterIntegral","test_module");
    auto builder=mm->get_module<Test3CInt>("test_module",0);

    unsigned int deriv=1;
    Wavefunction wf;
    BasisSet bs;

    ThreeCenterIntegral* ints=&(*builder);
    tester.test_member_call("Can_call initialize",true,
                            &ThreeCenterIntegral::initialize,ints,deriv,wf,bs,bs,bs);

    const double* corr=corr_answer.data();

    tester.test_member_return("Can call calculate",true,corr,
        &ThreeCenterIntegral::calculate,ints,deriv,deriv,deriv);
    tester.print_results();
    return tester.nfailed();
}
