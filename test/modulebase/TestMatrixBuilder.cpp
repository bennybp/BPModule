#include <pulsar/testing/CppTester.hpp>
#include <pulsar/modulemanager/ModuleManager.hpp>
#include <pulsar/modulebase/MatrixBuilder.hpp>
#include <pulsar/math/EigenImpl.hpp>

using namespace std;
using namespace pulsar;

MatrixBuilder::ReturnType corr_answer = {make_shared<const EigenMatrixImpl>(Eigen::MatrixXd::Identity(3,3))};

class TestMatrix:public MatrixBuilder{
public:
    TestMatrix(ID_t id):MatrixBuilder(id){}
private:
    ReturnType calculate_(const std::string &,
                          unsigned int, const Wavefunction &,
                          const BasisSet &, const BasisSet &)
    {
        return {corr_answer};
    }
};

TEST_SIMPLE(TestMatrixBuilder){
    CppTester tester("Testing MatrixBuilder base class");

    auto mm=make_shared<ModuleManager>();
    mm->load_lambda_module<TestMatrix>("MatrixBuilder","test_module");

    auto builder=mm->get_module<MatrixBuilder>("test_module",0);
    string a_str="a string";
    unsigned int deriv=1;
    Wavefunction wf;
    BasisSet bs;
    tester.test_member_return("Can call calculate",true,corr_answer,
        &MatrixBuilder::calculate,&(*builder),a_str,deriv,wf,bs,bs);
    tester.print_results();
    return tester.nfailed();
}
