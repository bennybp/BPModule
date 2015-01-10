#include <iostream>
#include <vector>

#include "BPModule/store/ModuleStore.h"
#include "BPModule/base/ModuleBase.h"

using namespace bpmodule;


class TestModule : public ModuleBase
{
public:
    TestModule(void) : ModuleBase(ModuleClass::MCLASS_TEST,
                                  ModuleType::MTYPE_TEST,
                                  "SIMPLETEST",
                                  "Yo-Yo Mama",
                                  "1.0",
                                  "Just a simple test module component")
    {
    }
                                  
};


class TestModule2 : public ModuleBase
{
public:
    TestModule2(void) : ModuleBase(ModuleClass::MCLASS_TEST,
                                   ModuleType::MTYPE_TEST,
                                   "SIMPLETESTTWO",
                                   "Yo-Yo Mama",
                                   "1.0",
                                   "Just a simple test module component 2")
    {
    }
                                  
};



extern "C" {

ModuleBase * NewTestModule(void) { return new TestModule(); }
ModuleBase * NewTestModule2(void) { return new TestModule2(); }


StorePairVec GetComponents(void)
{
    StorePairVec spv;
    spv.push_back(StorePair("TEST3", NewTestModule));
    spv.push_back(StorePair("TEST4", NewTestModule2));
    return spv;
}

}

