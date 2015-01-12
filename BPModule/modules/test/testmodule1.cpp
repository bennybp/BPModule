#include <iostream>
#include <vector>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/modules/base/Test_Base.h"
#include "BPModule/modules/base/Parallel_Base.h"

using namespace bpmodule;


class TestModule1 : public Test_Base
{
public:
    TestModule1(ModuleStore * mstore, const OptionMap & options)
                   : Test_Base(mstore, options)
    {
    }

    void RunTest(void)
    {
        std::cout << "I am TestModule 1. Running tests...\n";
    }

};

ModuleBase * NewTestModule1(ModuleStore * mstore, const OptionMap & options)
{ return new TestModule1(mstore, options); }


extern "C" {

StoreType GetComponents(void)
{
    StoreType st;

    OptionMap testops;
    testops.Set("DOUBLE", static_cast<double>(2.0));
    testops.Set("STRING", std::string("Some string"));
    testops.Set("CSTRING", "Some C string");

    st.insert(StoreType::value_type(
                    "TEST1",
                  { 
                    "Test1Module",
                    ModuleClass::TEST,
                    ModuleType::TEST,
                    "Y. O. Mama",
                    "0.1a",
                    "Just a simple test module",
                    "No refs",
                    NewTestModule1,
                    testops
                  }));
                   
    return st;
}

}

