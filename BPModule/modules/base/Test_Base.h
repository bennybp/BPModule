#ifndef TEST_BASE_H
#define TEST_BASE_H

#include "BPModule/core/ModuleBase.h"

using namespace bpmodule;


namespace bpmodule {

class Test_Base : public ModuleBase
{
public:
  Test_Base(ModuleStore * mstore,
            const std::string & filepath,
            ModuleType mtype,
            const std::string & name, const std::string & authors, 
            const std::string & version, const std::string & description);

  virtual void RunTest(void) = 0;
};

}

#endif
