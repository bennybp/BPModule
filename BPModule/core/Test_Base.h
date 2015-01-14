#ifndef TEST_BASE_H
#define TEST_BASE_H

#include "BPModule/core/ModuleBase.h"

using namespace bpmodule;


namespace bpmodule {

class Test_Base : public ModuleBase
{
public:
  Test_Base(long id, ModuleStore * mstore, const OptionMap & options);

  virtual void RunTest(void) = 0;
};

}

#endif
