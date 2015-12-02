/*! \file
 *
 * \brief Python exports for module base classes
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/register_ptr_to_python.hpp>
#include <boost/python/pure_virtual.hpp>

#include "bpmodule/modulebase/All.hpp"
#include "bpmodule/modulelocator/ModuleLocator.hpp"


using bpmodule::modulelocator::ModuleLocator;
using bpmodule::modulelocator::ModuleInfo;
using bpmodule::options::OptionMap;

using namespace boost::python;



namespace bpmodule {
namespace modulebase {
namespace export_python {


BOOST_PYTHON_MODULE(modulebase)
{
    ///////////////////////
    // Module Base classes
    ///////////////////////
    // python should never derive from ModuleBase, so a constructor should not be needed
    register_ptr_to_python<boost::shared_ptr<ModuleBase>>();

    class_<ModuleBase, boost::noncopyable>("ModuleBase", no_init)
    .def("ID", &ModuleBase::ID)
    .def("Key", &ModuleBase::Key)
    .def("Name", &ModuleBase::Name)
    .def("Version", &ModuleBase::Version)
    .def("Print", &ModuleBase::Print)
    .def("IsPythonModule", &ModuleBase::IsPythonModule)
    .def("Options", static_cast<OptionMap &(ModuleBase::*)(void)>(&ModuleBase::Options), return_value_policy<reference_existing_object>());  // should be safe?


    /////////////////////////
    // Test class
    /////////////////////////
    register_ptr_to_python<boost::shared_ptr<Test_Base>>();

    class_<Test_Base, bases<ModuleBase>, boost::shared_ptr<Test_Base>, boost::noncopyable>("Test_Base", init<PyObject *, unsigned long>())
    .def("MLocator", static_cast<ModuleLocator &(Test_Base::*)(void)>(&Test_Base::MLocator), return_value_policy<reference_existing_object>()) 
    .def("RunTest", &Test_Base::RunTest)
    .def("CallRunTest", &Test_Base::CallRunTest)
    .def("Throw", &Test_Base::Throw)
    .def("CallThrow", &Test_Base::CallThrow);

}


} // close namespace export_python
} // close namespace modulebase
} // close namespace bpmodule

