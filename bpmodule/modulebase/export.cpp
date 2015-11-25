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
#include "bpmodule/modulebase/All_python.hpp"
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
    class_<ModuleBase, boost::noncopyable>("ModuleBase", no_init)
//    .def("MLocator", &ModuleBase::MLocator, return_internal_reference<>())
    .def("ID", &ModuleBase::ID)
    .def("Key", &ModuleBase::Key)
    .def("Name", &ModuleBase::Name)
    .def("Version", &ModuleBase::Version)
    .def("Print", &ModuleBase::Print)
    .def("Options", static_cast<OptionMap &(ModuleBase::*)(void)>(&ModuleBase::Options), return_value_policy<reference_existing_object>());  // should be safe?



    register_ptr_to_python<boost::shared_ptr<Test_Base>>();
    class_<Test_Base_Wrap, bases<ModuleBase>, boost::shared_ptr<Test_Base_Wrap>, boost::noncopyable>("Test_Base", init<unsigned long>())
    .def("MLocator", &Test_Base_Wrap::MLocator, return_value_policy<reference_existing_object>()) 
    .def("RunTest", pure_virtual(&Test_Base::RunTest))
    .def("CallRunTest", pure_virtual(&Test_Base::CallRunTest))
    .def("Throw", pure_virtual(&Test_Base::Throw))
    .def("CallThrow", pure_virtual(&Test_Base::CallThrow));

}


} // close namespace export_python
} // close namespace modulebase
} // close namespace bpmodule

