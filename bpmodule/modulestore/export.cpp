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
#include "bpmodule/modulestore/CModuleLoader.hpp"
#include "bpmodule/modulestore/PyModuleLoader.hpp"


using bpmodule::modulebase::ModuleBase;
using bpmodule::modulebase::Test_Base;
using namespace boost::python;


namespace bpmodule {
namespace modulestore {
namespace export_python {



/*! \brief Wraps GetModule so that it returns a type compatible with python
 *
 * This uses boost::shared_ptr, which boost::python automatically handles
 *
 * Putting this in the ModuleStore source and headers would have required including boost for
 * basically every file in the project.
 *
 * \tparam T Type of module to get
 *
 * \param [in] ms ModuleStore object to get the module from
 * \param [in] key Key to get
 *
 * \return boost::shared_ptr containing a pointer to the new object
 */
template<typename T>
static
boost::shared_ptr<T> Wrap_GetModule(ModuleStore * ms, const std::string & key)
{
    ScopedModule<T> mod = ms->GetModule<T>(key);
    std::function<void(ModuleBase *)> dfunc = mod.get_deleter();
    T * ptr = mod.release();
    return boost::shared_ptr<T>(ptr, dfunc);
}




////////////////////////////
// Main boost python part
////////////////////////////

BOOST_PYTHON_MODULE(modulestore)
{
    // This is only needed because we pass through python
    // No need to do declare all the members, etc
    class_<ModuleInfo, boost::noncopyable>("ModuleInfo");


    class_<ModuleStore, boost::noncopyable>("ModuleStore")
    .def("Size", &ModuleStore::Size)
    .def("Has", &ModuleStore::Has)
    .def("SetOptions", static_cast<void(ModuleStore::*)(const std::string &, const boost::python::dict &)>(&ModuleStore::SetOptions))
    .def("GetKeys", &ModuleStore::GetKeys)
    .def("KeyInfo", &ModuleStore::KeyInfo)
    .def("PrintInfo", &ModuleStore::PrintInfo)
    .def("TestAll", &ModuleStore::TestAll)
    .def("GetModule", Wrap_GetModule<ModuleBase>)
    .def("GetModule_Test", Wrap_GetModule<Test_Base>);


    class_<CModuleLoader, boost::noncopyable>("CModuleLoader", init<ModuleStore *>())
    .def("LoadSO", static_cast<void(CModuleLoader::*)(const std::string &, const boost::python::dict &)>(&CModuleLoader::LoadSO));

    class_<PyModuleLoader, boost::noncopyable>("PyModuleLoader", init<ModuleStore *>())
    .def("LoadPyModule", static_cast<void(PyModuleLoader::*)(const std::string &, boost::python::object, const boost::python::dict &)>(&PyModuleLoader::LoadPyModule));

}


} // close namespace export_python
} // close namespace modulestore
} // close namespace bpmodule

