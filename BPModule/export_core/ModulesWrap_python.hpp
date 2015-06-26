#ifndef MODULESWRAP_PYTHON_H
#define MODULESWRAP_PYTHON_H

// All the module base classes
#include "BPModule/modulebase/All.hpp"
#include "BPModule/export_core/ModuleInfo_python.hpp"

#include <boost/python.hpp>

namespace bpmodule {
namespace export_python {

class Test_Base_Wrap : public Test_Base, public boost::python::wrapper<Test_Base>
{
    public:
        Test_Base_Wrap(unsigned long id, ModuleStore & mstore, const ModuleInfo & minfo)
            : Test_Base(id, mstore, minfo)
        {}

        virtual void RunTest(void)
        {
            this->get_override("RunTest")();
        }

        virtual void RunCallTest(const std::string & other)
        {
            this->get_override("RunCallTest")(other);
        }
};


} // close namespace export_python
} // close namespace bpmodule

#endif
