#ifndef MODULESWRAP_PYTHON_H
#define MODULESWRAP_PYTHON_H

#include "BPModule/exception/Exception.hpp"

// All the module base classes
#include "BPModule/modulebase/All.hpp"

#include <boost/python.hpp>

namespace bpy = boost::python;


namespace bpmodule {
namespace modulebase {
namespace export_python {


class Test_Base_Wrap : public Test_Base, public bpy::wrapper<Test_Base>
{
    public:
        Test_Base_Wrap(unsigned long id, ModuleStore & mstore, const ModuleInfo & minfo)
            : Test_Base(id, mstore, minfo)
        {}

        // expose protected member functions
        void ThrowException(const std::string & exwhat, const bpy::list & exinfo = bpy::list())
        {
            ModuleBase::ThrowException(exwhat, PythonListToPairVec(exinfo));
        }

        ModuleStore & MStore(void)
        {
            return ModuleBase::MStore();
        }

        // Provide overrides for protected member functions
        virtual void RunTest(void)
        {
            this->get_override("RunTest")();
        }

        virtual void CallRunTest(const std::string & other)
        {
            this->get_override("RunCallTest")(other);
        }

        virtual void Throw(void)
        {
            this->get_override("Throw")();
        }

        virtual void CallThrow(const std::string & other)
        {
            this->get_override("CallThrow")(other);
        }

        virtual CalcData CalcTest(CalcData inputs)
        {
            return this->get_override("CalcData")(inputs);
        }


    private:
        static BPModuleException::ExceptionInfo
        PythonListToPairVec(const bpy::list & exinfo)
        {
            int length = bpy::extract<int>(exinfo.attr("__len__")());
            BPModuleException::ExceptionInfo inf;
            inf.reserve(length);

            for (int i = 0; i < length; i++)
                inf.push_back({
                                bpy::extract<std::string>(exinfo[i][0]),
                                bpy::extract<std::string>(exinfo[i][1]),
                              });

            return inf;
        }

};



} // close namespace export_python
} // close namespace modulebase
} // close namespace bpmodule

#endif
