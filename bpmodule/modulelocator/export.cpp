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



/*! \brief Wraps GetModule so that it returns a type compatible with python
 *
 * This uses boost::shared_ptr, which boost::python automatically handles.
 * So we have to convert from a ScopedModule to a boost::shared_ptr.
 *
 * We return only a pointer to a ModuleBase. Python typing should
 * handle the rest
 *
 * \param [in] ms ModuleLocator object to get the module from
 * \param [in] key Key to get
 *
 * \return boost::shared_ptr containing a pointer to the new object
 */
boost::shared_ptr<ModuleBase> Wrap_GetModule(ModuleLocator * ms, const std::string & key)
{
    ScopedModule<ModuleBase> mod = ms->GetModule<ModuleBase>(key);
    std::function<void(ModuleBase *)> dfunc = mod.get_deleter();
    ModuleBase * ptr = mod.release();
    return boost::shared_ptr<ModuleBase>(ptr, dfunc);
}




////////////////////////////
// Main boost python part
////////////////////////////

BOOST_PYTHON_MODULE(modulelocator)
{
    class_<ModuleLocator, boost::noncopyable>("ModuleLocator")
    .def("Size", &ModuleLocator::Size)
    .def("Has", &ModuleLocator::Has)
    .def("SetOptions", static_cast<void(ModuleLocator::*)(const std::string &, const boost::python::dict &)>(&ModuleLocator::SetOptions))
    .def("GetKeys", &ModuleLocator::GetKeys)
    .def("KeyInfo", &ModuleLocator::KeyInfo)
    .def("PrintInfo", &ModuleLocator::PrintInfo)
    .def("TestAll", &ModuleLocator::TestAll)
    .def("GetModule", Wrap_GetModule);


    class_<CModuleLoader, boost::noncopyable>("CModuleLoader", init<ModuleLocator *>())
    .def("LoadSO", static_cast<void(CModuleLoader::*)(const std::string &, const boost::python::dict &)>(&CModuleLoader::LoadSO));

    class_<PyModuleLoader, boost::noncopyable>("PyModuleLoader", init<ModuleLocator *>())
    .def("LoadPyModule", static_cast<void(PyModuleLoader::*)(const std::string &, boost::python::object, const boost::python::dict &)>(&PyModuleLoader::LoadPyModule));

}


} // close namespace export_python
} // close namespace modulelocator
} // close namespace bpmodule

