/*! \file
 *
 * \brief Python exports for module base classes
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_internal_reference.hpp>
#include <boost/python/register_ptr_to_python.hpp>

#include "bpmodule/modulebase/All.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
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

    // CallFunc doesn't need to be exported 
    class_<ModuleBase, boost::noncopyable>("ModuleBase", no_init)
    .def("ID", &ModuleBase::ID)
    .def("Key", &ModuleBase::Key)
    .def("Name", &ModuleBase::Name)
    .def("Version", &ModuleBase::Version)
    .def("Print", &ModuleBase::Print)
    .def("IsPythonModule", &ModuleBase::IsPythonModule)
    .def("MLocator", static_cast<ModuleLocator &(ModuleBase::*)(void) const>(&ModuleBase::MLocator), return_internal_reference<>()) 
    .def("Options", static_cast<OptionMap &(ModuleBase::*)(void)>(&ModuleBase::Options), return_internal_reference<>())
    .def("Wfn", static_cast<datastore::Wavefunction &(ModuleBase::*)(void)>(&ModuleBase::Wfn), return_internal_reference<>())
    .def("CreateChildModule", &ModuleBase::CreateChildModulePy)
    .def("Cache", &ModuleBase::Cache, return_internal_reference<>())
    ;


    /////////////////////////
    // Test class
    /////////////////////////
    register_ptr_to_python<boost::shared_ptr<Test_Base>>();

    class_<Test_Base, bases<ModuleBase>, boost::noncopyable>("Test_Base", init<PyObject *, unsigned long>())
    .def("RunTest", &Test_Base::RunTest)
    .def("CallRunTest", &Test_Base::CallRunTest)
    .def("TestThrow", &Test_Base::TestThrow)
    .def("CallThrow", &Test_Base::CallThrow)
    ;


    /////////////////////////
    // One electron integral implementation
    /////////////////////////
    register_ptr_to_python<boost::shared_ptr<OneElectronIntegralIMPL>>();

    //! \todo Don't know about from Calculate to python
    class_<OneElectronIntegralIMPL, bases<ModuleBase>, boost::noncopyable>("OneElectronIntegralIMPL", init<PyObject *, unsigned long>())
    .def("SetBases", &OneElectronIntegralIMPL::SetBases)
    .def("Calculate", &OneElectronIntegralIMPL::Calculate)
    ;


    /////////////////////////
    // One electron integral builder
    /////////////////////////
    register_ptr_to_python<boost::shared_ptr<OneElectronIntegral>>();

    //! \todo Don't know about from Calculate to python
    class_<OneElectronIntegral, bases<ModuleBase>, boost::noncopyable>("OneElectronIntegral", init<PyObject *, unsigned long>())
    .def("SetBases", &OneElectronIntegral::SetBases)
    .def("Calculate", &OneElectronIntegral::Calculate)
    ;


    /////////////////////////
    // Two electron integral implementation
    /////////////////////////
    register_ptr_to_python<boost::shared_ptr<TwoElectronIntegralIMPL>>();

    class_<TwoElectronIntegralIMPL, bases<ModuleBase>, boost::noncopyable>("TwoElectronIntegralIMPL", init<PyObject *, unsigned long>())
    .def("SetBases", &TwoElectronIntegralIMPL::SetBases)
    .def("Calculate", &TwoElectronIntegralIMPL::Calculate)
    ;


    /////////////////////////
    // Two electron integral builder
    /////////////////////////
    register_ptr_to_python<boost::shared_ptr<TwoElectronIntegral>>();

    class_<TwoElectronIntegral, bases<ModuleBase>, boost::noncopyable>("TwoElectronIntegral", init<PyObject *, unsigned long>())
    .def("SetBases", &TwoElectronIntegral::SetBases)
    .def("Calculate", &TwoElectronIntegral::Calculate)
    ;

}


} // close namespace export_python
} // close namespace modulebase
} // close namespace bpmodule

