#include <pulsar/testing/CppTester.hpp>
#include <pulsar/modulemanager/ModuleManager.hpp>
#include <pulsar/modulebase/FourCenterIntegral.hpp>

using namespace std;
using namespace pulsar;

std::vector<double> corr_answer={1.1,2.2,3.3};

class Test4CInt:public FourCenterIntegral{
public:
    Test4CInt(ID_t id):FourCenterIntegral(id){}
private:
    void initialize_(unsigned int,
                     const Wavefunction &,
                     const BasisSet &,
                     const BasisSet &,
                     const BasisSet &,
                     const BasisSet &)
    {

    }

    HashType my_hash_(unsigned int,
                      const Wavefunction &,
                      const BasisSet &,
                      const BasisSet &,
                      const BasisSet &,
                      const BasisSet &){
        return "";
    }

    const double* calculate_(uint64_t, uint64_t, uint64_t, uint64_t)
    {
        return corr_answer.data();
    }
};

TEST_SIMPLE(TestFourCenterIntegral){
    CppTester tester("Testing FourCenterIntegral base class");

    auto mm=make_shared<ModuleManager>();
    mm->load_lambda_module<Test4CInt>("FourCenterIntegral","test_module");
    auto builder=mm->get_module<Test4CInt>("test_module",0);

    unsigned int deriv=1;
    Wavefunction wf;
    BasisSet bs;

    FourCenterIntegral* ints=&(*builder);
    tester.test_member_call("Can_call initialize",true,
                            &FourCenterIntegral::initialize,ints,deriv,wf,bs,bs,bs,bs);

    const double* corr=corr_answer.data();

    tester.test_member_return("Can call calculate",true,corr,
        &FourCenterIntegral::calculate,ints,deriv,deriv,deriv,deriv);
    tester.print_results();
    return tester.nfailed();
}
