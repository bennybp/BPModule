#include <vector>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/ModuleBase.h"

using namespace bpmodule;


class TestModule1 : public ModuleBase
{
public:
    TestModule1(ModuleStore * mstore, const std::string & filename)
                   : ModuleBase(
                     mstore,
                     filename,
                     ModuleClass::MCLASS_TEST,
                     ModuleType::MTYPE_TEST,
                     "SIMPLETEST1",
                     "Yo-Yo Mama",
                     "1.0",
                     "Just a simple test module component")
    {
    }

};

class TestModule2 : public ModuleBase
{
public:
    TestModule2(ModuleStore * mstore, const std::string & filename)
                   : ModuleBase(
                     mstore,
                     filename,
                     ModuleClass::MCLASS_TEST,
                     ModuleType::MTYPE_TEST,
                     "SIMPLETEST2",
                     "Yo-Yo Mama",
                     "2.0",
                     "Just a simple test module component")
    {
    }

};


ModuleBase * NewTestModule1(ModuleStore * mstore, const std::string & filename) { return new TestModule1(mstore, filename); }
ModuleBase * NewTestModule2(ModuleStore * mstore, const std::string & filename) { return new TestModule2(mstore, filename); }


extern "C" {

StoreType GetComponents(void)
{
    StoreType st;
    st["TEST1"] = NewTestModule1;
    st["TEST2"] = NewTestModule2;
    return st;
}

}

