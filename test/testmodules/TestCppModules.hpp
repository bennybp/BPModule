#pragma once

#include <pulsar/modulebase/TestModule.hpp>

using namespace pulsar;

class TestCppModule1 : public TestModule
{
public:
    TestCppModule1(ID_t id) : TestModule(id) { }

    virtual void run_test_(void)
    {
        out.output("Inside TestCppModule1: id = %?\n", id());
    }

    virtual ~TestCppModule1() = default;
};


class TestCppModule2 : public TestModule
{
public:
    TestCppModule2(ID_t id) : TestModule(id) { }

    virtual void run_test_(void)
    {
        PulsarException ex(format_string("TestCppModule2 throwing exception"));
        ex.append_info("moduleid", id());
        throw ex;
    }

    virtual ~TestCppModule2() = default;
};

