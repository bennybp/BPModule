#include <pulsar/testing/CppTest.hpp>
#include <pulsar/system/System.hpp>
using std::bind;
using namespace std::placeholders;
using namespace pulsar;
using BSI = BasisShellInfo;
using BSB = BasisShellBase;

TEST_SIMPLE(TestBasisShellBase)
{

    ShellType cGTO = ShellType::CartesianGaussian;
    ShellType sGTO = ShellType::SphericalGaussian;
    CppTester tester("Testing BasisShellBase (BSB)");

    std::vector<double> alpha({3.42525091, 0.62391373, 0.16885540}),
    c({0.15432897, 0.53532814, 0.44463454}),c2;
    
    //Use tested BSI to test BSB class
    BSI FakeD(cGTO, 2, 3, 1, alpha, c), 
       FakeD2(sGTO, 2, 3, 1, alpha, c);

    TEST_FXN("get type works", true, cGTO, FakeD.get_type());
    TEST_FXN("get am works", true, 2, FakeD2.am());
    TEST_FXN("number of prims", true, 3, FakeD2.n_primitives());
    TEST_FXN("number of ceofs", true, 3, FakeD.n_coefficients());
    TEST_FXN("number of gen contract", true, 1,FakeD2.n_general_contractions());
    TEST_FXN("number of functions Cart", true, 6, FakeD.n_functions());
    TEST_FXN("number of functions Spherical", true,5, FakeD2.n_functions());
    TEST_FXN("is combined am", true, false, FakeD2.is_combined_am());
    TEST_FXN("get valid general am", true, 2, FakeD.general_am(0));
    TEST_FXN("get invalid general am", false, 3, FakeD.general_am(99));
    TEST_FXN("general n fxns valid", true, 6, FakeD.general_n_functions(0));
    TEST_FXN("general n fxns invalid", false, 6, FakeD.general_n_functions(99));
    TEST_FXN("get valid alpha", true, alpha[1], FakeD.alpha(1));
    TEST_FXN("get invalid alpha", false, 0.4, FakeD.alpha(99));
    TEST_FXN("get valid coef", true, c[1], FakeD2.coef(0, 1));
    TEST_FXN("get invalid coef", false, 0.2, FakeD2.coef(0, 99));
    TEST_FXN("get invalid gen coef", false, 0.2, FakeD2.coef(99, 1));
    TEST_FXN("get non-const alpha ptr", true,alpha[1], FakeD.alpha_ptr()[1]);
    TEST_FXN("get const alpha ptr", true, alpha[1],
            static_cast<const BSI*>(&FakeD2)->alpha_ptr()[1]);
     

    TEST_FXN("get valid coef ptr", true, c[1], FakeD2.coef_ptr(0)[1]);
    TEST_FXN("get invalid coef ptr", false, nullptr, FakeD2.coef_ptr(99));
    TEST_FXN("get valid const coef ptr", true, c[1], 
            static_cast<const BSI*>(&FakeD)->coef_ptr(0)[1]);
    TEST_FXN("get invalid const coef ptr", false, nullptr, 
            static_cast<const BSI*>(&FakeD)->coef_ptr(99));
    
    TEST_FXN("get all valid coef ptr", true, c[1], FakeD.all_coefs_ptr()[1]);
    TEST_FXN("get all valid const coef ptr", true,c[1], 
            static_cast<const BSI*>(&FakeD)->all_coefs_ptr()[1]);
    
    TEST_FXN("get valid alpha safe", true, alpha[1], FakeD2.get_alpha(1));
    TEST_FXN("get invalid alpha safe", false, 0.0, FakeD2.get_alpha(99));
    TEST_VOID("set valid alpha safe",true,FakeD.set_alpha(0, 3.14));
    TEST_FXN("set valid alpha safe right val", true,3.14, FakeD.get_alpha(0));
    TEST_VOID("set invalid alpha safe",false,FakeD.set_alpha(99,3.14));
    TEST_FXN("get valid coef safe", true, c[1], FakeD2.get_coef(0, 1));
    TEST_FXN("get invalid gen coef safe", false, 0.0, FakeD2.get_coef(99,1));
    TEST_FXN("get invalid coef safe", false, 0.0, FakeD2.get_coef(0, 99));
    TEST_VOID("set valid coef safe",true,FakeD.set_coef(0, 0, 3.14));
    TEST_FXN("set valid coef safe right val", true, 3.14, FakeD.get_coef(0,0));
    TEST_VOID("set invalid gen coef safe",false,FakeD.set_coef(99,1,3.14));
    TEST_VOID("set invalid coef safe",false,FakeD.set_coef(0,99,3.14));
    TEST_VOID("safe set all alpha",true,FakeD.set_alphas(alpha));
    TEST_FXN("safe get all alpha",true,alpha,FakeD.get_alphas());
    TEST_VOID("safe set all coefs",true,FakeD.set_coefs(0,c));
    TEST_FXN("safe get/set all coef",true,c,FakeD.get_coefs(0));
    TEST_VOID("safe set all invalid coefs",false,FakeD.set_coefs(99,c));
    TEST_VOID("safe set all gen coefs",true,FakeD2.set_all_coefs(c));
    TEST_FXN("safe get/set all gen coefs",true,c,FakeD2.get_all_coefs());
    TEST_VOID("safe set all invalid coefs",false,FakeD.set_all_coefs(c2));
    TEST_VOID("set primitive works",true,FakeD2.set_primitive(0,3.14,3.14));
    TEST_FXN("set primitive works alpha",true,3.14,FakeD2.get_alpha(0));
    TEST_FXN("set primitive works coef",true,3.14,FakeD2.get_coef(0,0));
    TEST_VOID("safe invalid primitive",false,FakeD.set_primitive(99,3.14,3.14));
    
    std::vector<double> pi({3.14});
    TEST_VOID("set gen primitive works",true,FakeD2.set_primitive(0,3.14,pi));
    TEST_FXN("set gen primitive works alpha", true,3.14, FakeD2.get_alpha(0));
    TEST_FXN("set gen primitive works coef", true, 3.14, FakeD2.get_coef(0,0));
    
    TEST_VOID("safe gen primitive invalid prim",false,FakeD.set_primitive(99,3.14,pi));
    TEST_VOID("safe invalid gen primitive",false,FakeD.set_primitive(0,3.14,c2));
    
    FakeD2=FakeD;
    TEST_FXN("hash", true, FakeD.my_hash(), FakeD2.my_hash());

    tester.print_results();
    return tester.nfailed();
}
