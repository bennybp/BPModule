#include <pulsar/testing/CppTester.hpp>
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
    std::shared_ptr<BSB> pFakeD=std::make_shared<BSI>(cGTO, 2, 3, 1, alpha, c),
                    pFakeD2=std::make_shared<BSI>(sGTO, 2, 3, 1, alpha, c);
    BSB* FakeD=pFakeD.get(), *FakeD2=pFakeD2.get();

    tester.test_member_return("get type works", true, cGTO, &BSB::get_type,FakeD);
    tester.test_member_return("get am works", true, 2, &BSB::am,FakeD2);
    tester.test_member_return("number of prims", true, 3, &BSB::n_primitives,FakeD2);
    tester.test_member_return("number of ceofs", true, 3, &BSB::n_coefficients,FakeD);
    tester.test_member_return("number of gen contract", true, 1,&BSB::n_general_contractions,FakeD2);
    tester.test_member_return("number of functions Cart", true, 6, &BSB::n_functions,FakeD);
    tester.test_member_return("number of functions Spherical", true,5, &BSB::n_functions,FakeD2);
    tester.test_member_return("is combined am", true, false, &BSB::is_combined_am,FakeD2);
    tester.test_member_return("get valid general am", true, 2, &BSB::general_am,FakeD,0);
    tester.test_member_call("get invalid general am", false, &BSB::general_am,FakeD,99);
    tester.test_member_return("general n fxns valid", true, 6, &BSB::general_n_functions,FakeD,0);
    tester.test_member_call("general n fxns invalid", false, &BSB::general_n_functions,FakeD,99);
    tester.test_member_return("get valid alpha", true, alpha[1], &BSB::alpha, FakeD,1);
    tester.test_member_return("get valid coef", true, c[1], &BSB::coef,FakeD2,0,1);
    tester.test_equal("get non-const alpha ptr", alpha[1], FakeD2->alpha_ptr()[1]);
    tester.test_equal("get const alpha ptr",  alpha[1],
            static_cast<const BSB*>(FakeD2)->alpha_ptr()[1]);

    tester.test_equal("get valid coef ptr", c[1], FakeD2->coef_ptr(0)[1]);
    tester.test_equal("get valid const coef ptr", c[1],static_cast<const BSB*>(FakeD)->coef_ptr(0)[1]);
    
    tester.test_equal("get all valid coef ptr", c[1], FakeD->all_coefs_ptr()[1]);
    tester.test_equal("get all valid const coef ptr", c[1],
            static_cast<const BSB*>(FakeD)->all_coefs_ptr()[1]);
    
    tester.test_member_return("get valid alpha safe", true, alpha[1], &BSB::get_alpha,FakeD2,1);
    tester.test_member_call("get invalid alpha safe", false, &BSB::get_alpha,FakeD2,99);
    tester.test_member_call("set valid alpha safe",true,&BSB::set_alpha,FakeD,0, 3.14);
    tester.test_member_return("set valid alpha safe right val", true,3.14, &BSB::get_alpha,FakeD,0);
    tester.test_member_call("set invalid alpha safe",false,&BSB::set_alpha,FakeD,99,3.14);
    tester.test_member_return("get valid coef safe", true, c[1], &BSB::get_coef,FakeD2,0, 1);
    tester.test_member_call("get invalid gen coef safe", false, &BSB::get_coef,FakeD2,99,1);
    tester.test_member_call("get invalid coef safe", false,  &BSB::get_coef,FakeD2,0, 99);
    tester.test_member_call("set valid coef safe",true, &BSB::set_coef,FakeD,0, 0, 3.14);
    tester.test_member_return("set valid coef safe right val", true, 3.14, &BSB::get_coef,FakeD,0,0);
    tester.test_member_call("set invalid gen coef safe",false,&BSB::set_coef,FakeD,99,1,3.14);
    tester.test_member_call("set invalid coef safe",false,&BSB::set_coef,FakeD,0,99,3.14);
    tester.test_member_call("safe set all alpha",true,&BSB::set_alphas,FakeD,alpha);
    tester.test_member_return("safe get all alpha",true,alpha,&BSB::get_alphas,FakeD);
    tester.test_member_call("safe set all coefs",true,&BSB::set_coefs,FakeD,0,c);
    tester.test_member_return("safe get/set all coef",true,c,&BSB::get_coefs,FakeD,0);
    tester.test_member_call("safe set all invalid coefs",false,&BSB::set_coefs,FakeD,99,c);
    tester.test_member_call("safe set all gen coefs",true,&BSB::set_all_coefs,FakeD2,c);
    tester.test_member_return("safe get/set all gen coefs",true,c,&BSB::get_all_coefs,FakeD2);
    tester.test_member_call("safe set all invalid coefs",false,&BSB::set_all_coefs,FakeD,c2);
    using setprim1=void(BSB::*)(size_t,double,double);
    using setprim2=void(BSB::*)(size_t,double,const std::vector<double>&);
    tester.test_member_call("set primitive works",true,
                 static_cast<setprim1>(&BSB::set_primitive),FakeD2,0,3.14,3.14);
    tester.test_member_return("set primitive works alpha",true,3.14,&BSB::get_alpha,FakeD2,0);
    tester.test_member_return("set primitive works coef",true,3.14,&BSB::get_coef,FakeD2,0,0);
    tester.test_member_call("safe invalid primitive",false,
                 static_cast<setprim1>(&BSB::set_primitive),FakeD,99,3.14,3.14);
    
    std::vector<double> pi({3.14});
    tester.test_member_call("set gen primitive works",true,
                   static_cast<setprim2>(&BSB::set_primitive),FakeD2,0,3.14,pi);
    tester.test_member_return("set gen primitive works alpha", true,3.14,
                              &BSB::get_alpha,FakeD2,0);
    tester.test_member_return("set gen primitive works coef", true, 3.14,
                              &BSB::get_coef,FakeD2,0,0);
    
    tester.test_member_call("safe gen primitive invalid prim",false,
                    static_cast<setprim2>(&BSB::set_primitive),FakeD,99,3.14,pi);
    tester.test_member_call("safe invalid gen primitive",false,
                    static_cast<setprim2>(&BSB::set_primitive),FakeD,0,3.14,c2);
    
//    *FakeD2=*FakeD;
//    tester.test_equal("hash",FakeD->my_hash(), FakeD2->my_hash());

    tester.print_results();
    return tester.nfailed();
}
