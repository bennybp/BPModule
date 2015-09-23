/*! \file
 *
 * \brief Python exports for module base classes
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/modulebase/All.hpp"
#include "bpmodule/modulebase/All_python.hpp"
#include "bpmodule/modulestore/ModuleStore.hpp"

// included from above
//#include <boost/python.hpp>

using bpmodule::modulestore::ModuleStore;
using bpmodule::modulestore::ModuleInfo;
using namespace boost::python;



namespace bpmodule {
namespace modulebase {
namespace export_python {

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ThrowException_overloads, ThrowException, 1, 2)

BOOST_PYTHON_MODULE(modulebase)
{
    ///////////////////////
    // Module Base classes
    ///////////////////////
    // python should never derive from ModuleBase, so a constructor should not be needed
    class_<ModuleBase, boost::noncopyable>("ModuleBase", no_init)
//    .def("MStore", &ModuleBase::MStore, return_internal_reference<>())
    .def("ID", &ModuleBase::ID)
    .def("Key", &ModuleBase::Key, return_value_policy<copy_const_reference>())
    .def("Name", &ModuleBase::Name, return_value_policy<copy_const_reference>())
    .def("Version", &ModuleBase::Version, return_value_policy<copy_const_reference>())
    .def("HasOption", &ModuleBase::HasOption)
    .def("ResetOption", &ModuleBase::ResetOption)
    .def("GetOption", &ModuleBase::GetOptionPy)
    .def("ChangeOption", &ModuleBase::ChangeOptionPy);



    register_ptr_to_python<boost::shared_ptr<Test_Base>>();
    class_<Test_Base_Wrap, bases<ModuleBase>, boost::shared_ptr<Test_Base_Wrap>, boost::noncopyable>("Test_Base", init<unsigned long, ModuleStore &, const ModuleInfo &>())
    .def("MStore", &Test_Base_Wrap::MStore, return_internal_reference<>())
    .def("ThrowException", &Test_Base_Wrap::ThrowException, ThrowException_overloads())
    .def("RunTest", pure_virtual(&Test_Base::RunTest))
    .def("CallRunTest", pure_virtual(&Test_Base::CallRunTest))
    .def("Throw", pure_virtual(&Test_Base::Throw))
    .def("CallThrow", pure_virtual(&Test_Base::CallThrow))
    .def("CalcTest", pure_virtual(&Test_Base::CalcTest), return_value_policy<return_by_value>());

}


} // close namespace export_python
} // close namespace modulebase
} // close namespace bpmodule

