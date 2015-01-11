#include <iostream>
#include <vector>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/modules/base/Test_Base.h"

using namespace bpmodule;


class TestModule3 : public Test_Base
{
public:
    TestModule3(ModuleStore * mstore, const std::string & filepath)
                   : Test_Base(
                     mstore,
                     filepath,
                     ModuleType::TEST,
                     "SIMPLETEST3",
                     "Yo-Yo Mama",
                     "3.0",
                     "Just a simple test module component")
    {
    }

    void RunTest(void)
    {
        std::cout << "I am TestModule 3. Running tests...\n";
    }

};

class TestModule4 : public Test_Base
{
public:
    TestModule4(ModuleStore * mstore, const std::string & filepath)
                   : Test_Base(
                     mstore,
                     filepath,
                     ModuleType::TEST,
                     "SIMPLETEST4",
                     "Yo-Yo Mama",
                     "4.0",
                     "Just a simple test module component")
    {
    }

    void RunTest(void)
    {
        std::cout << "I am TestModule 4. Running tests...\n";
    }

};


ModuleBase * NewTestModule3(ModuleStore * mstore, const std::string & filepath) { return new TestModule3(mstore, filepath); }
ModuleBase * NewTestModule4(ModuleStore * mstore, const std::string & filepath) { return new TestModule4(mstore, filepath); }


extern "C" {

StoreType GetComponents(void)
{
    StoreType st;
    st["TEST3"] = NewTestModule3;
    st["TEST4"] = NewTestModule4;
    return st;
}

}

