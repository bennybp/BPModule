#include <iostream>
#include <vector>

#include "BPModule/store/ModuleStore.h"
#include "BPModule/base/ModuleBase.h"

using namespace bpmodule;


class Test2Module : public ModuleBase
{
public:
    Test2Module(void) : ModuleBase(ModuleClass::MCLASS_TEST,
                                  ModuleType::MTYPE_TEST,
                                  "SIMPLETEST",
                                  "Yo-Yo Mama",
                                  "1.0",
                                  "Just a simple test module component")
    {
    }
                                  
};


class Test2Module2 : public ModuleBase
{
public:
    Test2Module2(void) : ModuleBase(ModuleClass::MCLASS_TEST,
                                   ModuleType::MTYPE_TEST,
                                   "SIMPLETESTTWO",
                                   "Yo-Yo Mama",
                                   "1.0",
                                   "Just a simple test module component 2")
    {
    }
                                  
};



extern "C" {

ModuleBase * NewTest2Module(void) { return new Test2Module(); }
ModuleBase * NewTest2Module2(void) { return new Test2Module2(); }


StorePairVec GetComponents(void)
{
    StorePairVec spv;
    spv.push_back(StorePair("TEST1", NewTest2Module));
    spv.push_back(StorePair("TEST2", NewTest2Module2));
    return spv;
}

}

