#include <vector>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/ModuleBase.h"

using namespace bpmodule;


class TestModule3 : public ModuleBase
{
public:
    TestModule3(ModuleStore * mstore, const std::string & filename)
                   : ModuleBase(
                     mstore,
                     filename,
                     ModuleClass::MCLASS_TEST,
                     ModuleType::MTYPE_TEST,
                     "SIMPLETEST3",
                     "Yo-Yo Mama",
                     "3.0",
                     "Just a simple test module component")
    {
    }

};

class TestModule4 : public ModuleBase
{
public:
    TestModule4(ModuleStore * mstore, const std::string & filename)
                   : ModuleBase(
                     mstore,
                     filename,
                     ModuleClass::MCLASS_TEST,
                     ModuleType::MTYPE_TEST,
                     "SIMPLETEST4",
                     "Yo-Yo Mama",
                     "4.0",
                     "Just a simple test module component")
    {
    }

};


ModuleBase * NewTestModule3(ModuleStore * mstore, const std::string & filename) { return new TestModule3(mstore, filename); }
ModuleBase * NewTestModule4(ModuleStore * mstore, const std::string & filename) { return new TestModule4(mstore, filename); }


extern "C" {

StoreType GetComponents(void)
{
    StoreType st;
    st["TEST3"] = NewTestModule3;
    st["TEST4"] = NewTestModule4;
    return st;
}

}

