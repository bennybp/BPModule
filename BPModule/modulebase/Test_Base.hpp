#ifndef TEST_BASE_H
#define TEST_BASE_H

#include "BPModule/modulebase/ModuleBase.hpp"


namespace bpmodule {
namespace modulebase {

class Test_Base : public ModuleBase
{
    public:
        Test_Base(unsigned long id, 
                  modulestore::ModuleStore & mstore,
                  const modulestore::ModuleInfo & minfo);

        virtual void RunTest(void) = 0;
        virtual void CallRunTest(const std::string & other) = 0;
        virtual void Throw(void) = 0;
        virtual void CallThrow(const std::string & other) = 0;

        virtual datastore::CalcData CalcTest(datastore::CalcData inputs) = 0;
};

} // close namespace modulebase
} // close namespace bpmodule

#endif
