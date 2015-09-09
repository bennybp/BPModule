#ifndef _GUARD_TEST_BASE_PYTHON_HPP_
#define _GUARD_TEST_BASE_PYTHON_HPP_

#include "bpmodule/modulebase/Test_Base.hpp"

#include <boost/python.hpp>


namespace bpmodule {
namespace modulebase {
namespace export_python {


class Test_Base_Wrap : public Test_Base, public boost::python::wrapper<Test_Base>
{
    public:
        Test_Base_Wrap(unsigned long id,
                       modulestore::ModuleStore & mstore,
                       const modulestore::ModuleInfo & minfo)
            : Test_Base(id, mstore, minfo)
        {}

        // expose protected member functions
        void ThrowException(const std::string & exwhat,
                            const boost::python::list & exinfo = boost::python::list())
        {
            ModuleBase::ThrowException(exwhat, PythonListToPairVec(exinfo));
        }

        modulestore::ModuleStore & MStore(void)
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

        virtual datastore::CalcData CalcTest(datastore::CalcData inputs)
        {
            return this->get_override("CalcData")(inputs);
        }


    private:
        static exception::GeneralException::ExceptionInfo
        PythonListToPairVec(const boost::python::list & exinfo)
        {
            int length = boost::python::extract<int>(exinfo.attr("__len__")());
            exception::GeneralException::ExceptionInfo inf;
            inf.reserve(length);

            for (int i = 0; i < length; i++)
                inf.push_back({
                                boost::python::extract<std::string>(exinfo[i][0]),
                                boost::python::extract<std::string>(exinfo[i][1]),
                              });

            return inf;
        }

};



} // close namespace export_python
} // close namespace modulebase
} // close namespace bpmodule

#endif
