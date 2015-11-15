/*! \file
 *
 * \brief Base class for a test module (python wrapper)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_TEST_BASE_PYTHON_HPP_
#define _GUARD_TEST_BASE_PYTHON_HPP_

#include <boost/python/wrapper.hpp>
#include <boost/python/list.hpp>

#include "bpmodule/modulebase/Test_Base.hpp"



namespace bpmodule {
namespace modulebase {
namespace export_python {


/*! \brief Python wrapper for bpmodule::modulebase::Test_Base
 */
class Test_Base_Wrap : public Test_Base, public boost::python::wrapper<Test_Base>
{
    public:
        Test_Base_Wrap(unsigned long id)
            : Test_Base(id)
        { }

        modulelocator::ModuleLocator & MLocator(void)
        {
            return ModuleBase::MLocator();
        }


        virtual void RunTest(void)
        {
            this->get_override("RunTest")();
        }

        virtual void CallRunTest(const std::string & other)
        {
            this->get_override("CallRunTest")(other);
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
            // prevents some compiler warnings
            const auto ret = this->get_override("CalcTest")(inputs);
            return static_cast<datastore::CalcData>(ret);
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
