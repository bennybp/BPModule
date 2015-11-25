#include <bpmodule/modulebase/ModuleBase.hpp>
#include <bpmodule/exception/ModuleCreateException.hpp>

#include "TestModule1.hpp"
#include "TestExtLib.hpp"

using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::ModuleCreateException;


extern "C" {

ModuleBase * CreateModule(const std::string & name, unsigned long id)
{
    if(name == "TestModule1")
        return new TestModule1(id);
    else if(name == "TestExtLib")
        return new TestExtLib(id);
    else
        throw ModuleCreateException("Unknown module", name);
}



}

