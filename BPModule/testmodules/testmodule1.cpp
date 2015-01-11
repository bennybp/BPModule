#include <iostream>
#include <vector>

#include "BPModule/store/ModuleStore.h"
#include "BPModule/base/ModuleBase.h"

using namespace bpmodule;


class TestModule1 : public ModuleBase
{
public:
    TestModule1(const std::string & filename) 
                   : ModuleBase(
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
    TestModule2(const std::string & filename) 
                   : ModuleBase(
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


ModuleBase * NewTestModule1(const std::string & filename) { return new TestModule1(filename); }
ModuleBase * NewTestModule2(const std::string & filename) { return new TestModule2(filename); }


extern "C" {

StoreType GetComponents(void)
{
    StoreType st;
    st["TEST1"] = NewTestModule1;
    st["TEST2"] = NewTestModule2;
    return st;
}

}

