#ifndef _GUARD_TESTMODULE1_HPP_
#define _GUARD_TESTMODULE1_HPP_

#include <bpmodule/modulebase/Test_Base.hpp>

class TestModule1 : public bpmodule::modulebase::Test_Base
{
public:
    TestModule1(unsigned long id);

    virtual void RunTest(void);

    virtual void CallRunTest(const std::string & other);

    virtual void Throw(void);

    virtual void CallThrow(const std::string & other);

    virtual ~TestModule1();

};


#endif
