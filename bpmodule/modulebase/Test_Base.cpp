/*! \file
 *
 * \brief Base class for a test module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/shared_ptr.hpp>
#include <boost/python/object.hpp>

#include "bpmodule/modulebase/Test_Base.hpp"

using bpmodule::modulelocator::ModuleLocator;
using bpmodule::modulelocator::ModuleInfo;

namespace bpmodule {
namespace modulebase {

Test_Base::Test_Base(unsigned long id)
    : ModuleBase(id)
{ }

Test_Base::Test_Base(PyObject * self, unsigned long id)
    : ModuleBase(self, id)
{ }


void Test_Base::RunTest(void)
{
    ModuleBase::CallPyMethod<void>("RunTest");
}



void Test_Base::CallRunTest(const std::string & other)
{
    ModuleBase::CallPyMethod<void>("CallRunTest", other);
}



void Test_Base::TestThrow(void)
{
    ModuleBase::CallPyMethod<void>("TestThrow");
}



void Test_Base::CallThrow(const std::string & other)
{
    ModuleBase::CallPyMethod<void>("CallThrow", other);
}




////////////////////////////
// Private functions
////////////////////////////
boost::python::object Test_Base::MoveToPyObject_(std::function<void(modulebase::ModuleBase *)> deleter)
{
    boost::shared_ptr<Test_Base> me(this, deleter);
    return boost::python::object(me);
}



} // close namespace modulebase
} // close namespace bpmodule
