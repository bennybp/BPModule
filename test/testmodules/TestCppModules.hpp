#pragma once

#include <pulsar/modulebase/TestModule.hpp>

class TestCppModule1 : public pulsar::TestModule
{
public:
    TestCppModule1(ID_t id) : TestModule(id) { }

    virtual void run_test_(void)
    {
        out.output("Inside TestCppModule1: id = %?\n", id());
    }

    virtual ~TestCppModule1() = default;
};

