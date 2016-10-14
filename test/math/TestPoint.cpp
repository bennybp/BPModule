/*! \file
 *
 * \brief  Add me
 * \author Ryan Richard (ryanmrichard1<at>gmail.com)
 */

#include<pulsar/testing/Tester.hpp>
#include<pulsar/modulebase/Test_Base.hpp>
#include<cstdlib>  //For exit success

using pulsar::testing::Tester;
using pulsar::modulebase::Test_Base;

class TestPoint : public Test_Base {
protected:
        virtual void run_test_(void){}
        virtual void call_run_test_(const std::string & other){}
        virtual void call_run_test2_(const std::string & other1,
        const std::string & other2){}
        virtual void test_throw_(void){}
        virtual void call_throw_(const std::string & other){}
public:
    using Test_Base::Test_Base;
};


using pulsar::modulemanager::ModuleCreationFuncs;

extern "C" {

ModuleCreationFuncs insert_supermodule(void){
    ModuleCreationFuncs cf;
    cf.add_cpp_creator<TestPoint>("TestPoint");
    return cf;
}
}
