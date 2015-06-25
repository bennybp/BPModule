#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/CModuleLoader.h"
#include "BPModule/core/PyModuleLoader.h"


// helpers and wrappers
#include "BPModule/export_core/ModulesWrap_python.h"
#include "BPModule/export_core/Python_stdconvert.h"
#include "BPModule/export_core/OptionMap_python.h"
#include "BPModule/export_core/ModuleInfo_python.h"
#include "BPModule/export_core/Output_python.h"

using namespace boost::python;

namespace bpmodule {
namespace export_python {


// the main exception translator
void TranslateException(const BPModuleException & ex)
{
    PyErr_SetString(PyExc_RuntimeError, ExceptionString(ex).c_str());
}



// wraps CModuleLoader::LoadSO so that it can take a dict for the ModuleInfo
bool Wrap_CModuleLoader_LoadSO(CModuleLoader * ml, const std::string & key, const boost::python::dict & d)
{
   return ml->LoadSO(key, DictToModuleInfo(d));
}

// wraps PyModuleLoader::AddPyModule so that it can take a dict for the ModuleInfo
bool Wrap_PyModuleLoader_AddPyModule(PyModuleLoader * ml,
                                     const std::string & key, boost::python::object func,
                                     const boost::python::dict & d)
{
   return ml->AddPyModule(key, func, DictToModuleInfo(d));
}


template<typename T>
boost::shared_ptr<T> Wrap_GetScopedModule(ModuleStore * ms, const std::string & key)
{
    T & mod = ms->GetModule<T>(key);
    std::function<void(ModuleBase *)> dfunc = std::bind(static_cast<void(ModuleStore::*)(ModuleBase *)>(&ModuleStore::Delete), ms, std::placeholders::_1);
    return boost::shared_ptr<T>(&mod, dfunc);
}


////////////////////////////
// Main boost python part
////////////////////////////
BOOST_PYTHON_MODULE(bpmodule_core)
{
    register_exception_translator<BPModuleException>(&TranslateException);

    // converting pairs
    to_python_converter<std::pair<int, int>, PairConverter<int, int>>();
    to_python_converter<std::pair<double, double>, PairConverter<double, double>>();
    to_python_converter<std::pair<std::string, std::string>, PairConverter<std::string, std::string>>();

    // converting vectors
    to_python_converter<std::vector<int>, VectorConverter<int>>();
    to_python_converter<std::vector<double>, VectorConverter<double>>();
    to_python_converter<std::vector<std::string>, VectorConverter<std::string>>();

    // convert map to dict
    to_python_converter<std::map<std::string, std::string>, MapConverter<std::map<std::string, std::string>>>();
    to_python_converter<std::unordered_map<std::string, std::string>, MapConverter<std::unordered_map<std::string, std::string>>>();

    // convert module info to dict
    to_python_converter<ModuleInfo, ModuleInfoConverter>();

    // convert optionmap to dict
    to_python_converter<OptionMap, OptionMapConverter>();

    // setting the output
    def("SetOut_Stdout", SetOut_Stdout);
    def("SetOut_File", SetOut_File);
    def("SetColor", SetColor);
    def("SetDebug", SetDebug);

    // printing to output
    def("Output", Output_Wrap_Output);
    def("Success", Output_Wrap_Success);
    def("Warning", Output_Wrap_Warning);
    def("Error", Output_Wrap_Error);
    def("Debug", Output_Wrap_Debug);

    class_<ModuleStore, boost::noncopyable>("ModuleStore")
           .def("Lock", &ModuleStore::Lock)
           .def("Size", &ModuleStore::Size)
           .def("Has", &ModuleStore::Has)
           .def("Delete", static_cast<void(ModuleStore::*)(unsigned long)>(&ModuleStore::Delete))
           .def("GetKeys", &ModuleStore::GetKeys)
           .def("ModuleInfoFromKey", &ModuleStore::ModuleInfoFromKey)
           .def("GetModule", &ModuleStore::GetModule<ModuleBase>, return_internal_reference<>())
           .def("GetModule_Test", &ModuleStore::GetModule<Test_Base>, return_internal_reference<>())
           .def("GetScopedModule", Wrap_GetScopedModule<ModuleBase>)
           .def("GetScopedModule_Test", Wrap_GetScopedModule<Test_Base>);


    class_<CModuleLoader, boost::noncopyable>("CModuleLoader", init<ModuleStore *>())
           .def("CloseHandles", &CModuleLoader::CloseHandles)
           .def("DeleteAll", &CModuleLoader::DeleteAll)
           .def("LoadSO", Wrap_CModuleLoader_LoadSO);

    class_<PyModuleLoader, boost::noncopyable>("PyModuleLoader", init<ModuleStore *>())
           .def("DeleteAll", &PyModuleLoader::DeleteAll)
           .def("AddPyModule", Wrap_PyModuleLoader_AddPyModule);

    ///////////////////////
    // Module Base classes
    ///////////////////////
    class_<ModuleBase, boost::noncopyable>("ModuleBase", init<unsigned long, ModuleStore &, const OptionMap &>())
           .def("MStore", &ModuleBase::MStore, return_internal_reference<>())
           .def("ID", &ModuleBase::ID)
           .def("Traits", &ModuleBase::Traits)
           .def("Options", &ModuleBase::Options);

    register_ptr_to_python<boost::shared_ptr<Test_Base>>();
    class_<Test_Base_Wrap, bases<ModuleBase>, boost::shared_ptr<Test_Base_Wrap>, boost::noncopyable>("Test_Base", init<unsigned long, ModuleStore &, boost::python::list>())
           .def("RunTest", pure_virtual(&Test_Base::RunTest))
           .def("RunCallTest", pure_virtual(&Test_Base::RunCallTest));

}


} // close namespace export_python
} // close namespace bpmodule

