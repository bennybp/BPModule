/*! \file
 *
 * \brief Python exports for module storage and database
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/dict.hpp>

// Various components
#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/modulelocator/CModuleLoader.hpp"
#include "bpmodule/modulelocator/PyModuleLoader.hpp"


using bpmodule::modulebase::ModuleBase;
using namespace boost::python;


namespace bpmodule {
namespace modulelocator {
namespace export_python {


////////////////////////////
// Main boost python part
////////////////////////////

BOOST_PYTHON_MODULE(modulelocator)
{
    //////////////////////
    // ModuleInfo
    //////////////////////
    // for now, we won't allow initialization from python
    class_<ModuleInfo>("ModuleInfo", no_init)
    .def("Print", &ModuleInfo::Print)
    .def_readonly("key", &ModuleInfo::key)
    .def_readonly("name", &ModuleInfo::name)
    .def_readonly("type", &ModuleInfo::type)
    .def_readonly("path", &ModuleInfo::path)
    .def_readonly("soname", &ModuleInfo::soname)
    .def_readonly("version", &ModuleInfo::version)
    .def_readonly("description", &ModuleInfo::description)
    .add_property("authors", make_getter(&ModuleInfo::authors, return_value_policy<return_by_value>()))
    .add_property("refs", make_getter(&ModuleInfo::refs, return_value_policy<return_by_value>()))
    .add_property("options", make_getter(&ModuleInfo::options, return_value_policy<return_by_value>()))
    ;
     


    //////////////////////////
    // Main ModuleLocator and
    // module loaders
    //////////////////////////
    class_<ModuleLocator, boost::noncopyable>("ModuleLocator")
    .def("Size", &ModuleLocator::Size)
    .def("GetKeys", &ModuleLocator::GetKeys)
    .def("KeyInfo", &ModuleLocator::KeyInfo)
    .def("PrintInfo", &ModuleLocator::PrintInfo)
    .def("Has", &ModuleLocator::Has)
    .def("SetOptions", static_cast<void(ModuleLocator::*)(const std::string &, const boost::python::dict &)>(&ModuleLocator::SetOptions))
    .def("TestAll", &ModuleLocator::TestAll)
    .def("GetModule", &ModuleLocator::GetModulePy)
    ;


    class_<CModuleLoader, boost::noncopyable>("CModuleLoader", init<ModuleLocator *>())
    .def("LoadSO", static_cast<void(CModuleLoader::*)(const std::string &, const boost::python::dict &)>(&CModuleLoader::LoadSO));

    class_<PyModuleLoader, boost::noncopyable>("PyModuleLoader", init<ModuleLocator *>())
    .def("LoadPyModule", static_cast<void(PyModuleLoader::*)(const std::string &, boost::python::object, const boost::python::dict &)>(&PyModuleLoader::LoadPyModule));

}


} // close namespace export_python
} // close namespace modulelocator
} // close namespace bpmodule

