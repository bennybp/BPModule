/*! \file
 *
 * \brief Python exports for module storage and database
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/dict.hpp>

// Various components
#include "bpmodule/modulebase/All_python.hpp"
#include "bpmodule/modulelocator/CModuleLoader.hpp"
#include "bpmodule/modulelocator/PyModuleLoader.hpp"


using bpmodule::modulebase::ModuleBase;
using bpmodule::modulebase::Test_Base;
using namespace boost::python;


namespace bpmodule {
namespace modulelocator {
namespace export_python {



/*! \brief Wraps GetModule so that it returns a type compatible with python
 *
 * This uses boost::shared_ptr, which boost::python automatically handles
 *
 * Putting this in the ModuleLocator source and headers would have required including boost for
 * basically every file in the project.
 *
 * \tparam T Type of module to get
 *
 * \param [in] ms ModuleLocator object to get the module from
 * \param [in] key Key to get
 *
 * \return boost::shared_ptr containing a pointer to the new object
 */
template<typename T>
static
boost::shared_ptr<T> Wrap_GetModule(ModuleLocator * ms, const std::string & key)
{
    ScopedModule<T> mod = ms->GetModule<T>(key);
    std::function<void(ModuleBase *)> dfunc = mod.get_deleter();
    T * ptr = mod.release();
    return boost::shared_ptr<T>(ptr, dfunc);
}




////////////////////////////
// Main boost python part
////////////////////////////

BOOST_PYTHON_MODULE(modulelocator)
{
    // This is only needed because we pass through python
    // No need to do declare all the members, etc
    class_<ModuleInfo, boost::noncopyable>("ModuleInfo");


    class_<ModuleLocator, boost::noncopyable>("ModuleLocator")
    .def("Size", &ModuleLocator::Size)
    .def("Has", &ModuleLocator::Has)
    .def("SetOptions", static_cast<void(ModuleLocator::*)(const std::string &, const boost::python::dict &)>(&ModuleLocator::SetOptions))
    .def("GetKeys", &ModuleLocator::GetKeys)
    .def("KeyInfo", &ModuleLocator::KeyInfo)
    .def("PrintInfo", &ModuleLocator::PrintInfo)
    .def("TestAll", &ModuleLocator::TestAll)
    .def("GetModule", Wrap_GetModule<ModuleBase>)
    .def("GetModule_Test", Wrap_GetModule<Test_Base>);


    class_<CModuleLoader, boost::noncopyable>("CModuleLoader", init<ModuleLocator *>())
    .def("LoadSO", static_cast<void(CModuleLoader::*)(const std::string &, const boost::python::dict &)>(&CModuleLoader::LoadSO));

    class_<PyModuleLoader, boost::noncopyable>("PyModuleLoader", init<ModuleLocator *>())
    .def("LoadPyModule", static_cast<void(PyModuleLoader::*)(const std::string &, boost::python::object, const boost::python::dict &)>(&PyModuleLoader::LoadPyModule));

}


} // close namespace export_python
} // close namespace modulelocator
} // close namespace bpmodule

