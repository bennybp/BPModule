#include "BPModule/core/ModuleStore.hpp"
#include "BPModule/core/CModuleLoader.hpp"
#include "BPModule/core/PyModuleLoader.hpp"


// helpers and wrappers
#include "BPModule/export_core/ModulesWrap_python.hpp"
#include "BPModule/export_core/Python_stdconvert.hpp"
#include "BPModule/export_core/Output_python.hpp"
#include "BPModule/export_core/Exception_python.hpp"

using namespace boost::python;
namespace bpy = boost::python;



namespace bpmodule {
namespace export_python {


// wraps CModuleLoader::LoadSO so that it can take a dict for the ModuleInfo
void Wrap_CModuleLoader_LoadSO(CModuleLoader * ml, const std::string & key, const bpy::dict & minfo)
{
    // dictionary is converted to ModuleInfo via constructor
    return ml->LoadSO(key, minfo);
}

// wraps PyModuleLoader::AddPyModule so that it can take a dict for the ModuleInfo
void Wrap_PyModuleLoader_AddPyModule(PyModuleLoader * ml,
                                     const std::string & key, bpy::object func,
                                     const bpy::dict & minfo)
{
    // dictionary is converted to ModuleInfo via constructor
    return ml->AddPyModule(key, func, minfo);
}


void Wrap_ModuleStore_SetOptions(ModuleStore * mst, const std::string & key, bpy::list & opt)
{
    // OptionMap has a conversion constructor
    mst->SetOptions(key, opt);
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

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ThrowException_overloads, ThrowException, 1, 2)


BOOST_PYTHON_MODULE(bpmodule_core)
{
    // set the translator for exceptions
    register_exception_translator<BPModuleException>(&TranslateException);

    // converting pairs
    to_python_converter<std::pair<int, int>, PairConverter<int, int>>();
    to_python_converter<std::pair<long, long>, PairConverter<long, long>>();
    to_python_converter<std::pair<double, double>, PairConverter<double, double>>();
    to_python_converter<std::pair<std::string, std::string>, PairConverter<std::string, std::string>>();

    // converting vectors
    to_python_converter<std::vector<int>, VectorConverter<int>>();
    to_python_converter<std::vector<long>, VectorConverter<long>>();
    to_python_converter<std::vector<double>, VectorConverter<double>>();
    to_python_converter<std::vector<std::string>, VectorConverter<std::string>>();

    // convert map to dict
    to_python_converter<std::map<std::string, std::string>, MapConverter<std::map<std::string, std::string>>>();
    to_python_converter<std::unordered_map<std::string, std::string>, MapConverter<std::unordered_map<std::string, std::string>>>();

    // setting the output
    def("SetOut_Stdout", &output::SetOut_Stdout);
    def("SetOut_Stderr", &output::SetOut_Stderr);
    def("SetOut_File", &output::SetOut_File);
    def("SetColor", &output::SetColor);
    def("SetDebug", &output::SetDebug);
    def("Valid", &output::Valid);
    def("Flush", &output::Flush);

    // printing to output
    def("Output", Output_Wrap_Output);
    def("Success", Output_Wrap_Success);
    def("Warning", Output_Wrap_Warning);
    def("Error", Output_Wrap_Error);
    def("Changed", Output_Wrap_Changed);
    def("Debug", Output_Wrap_Debug);


    // This is only needed because we pass through python
    // No need to do declare all the members, etc 
    class_<ModuleInfo, boost::noncopyable>("ModuleInfo");


    class_<ModuleStore, boost::noncopyable>("ModuleStore")
    .def("Lock", &ModuleStore::Lock)
    .def("Size", &ModuleStore::Size)
    .def("Has", &ModuleStore::Has)
    .def("Delete", static_cast<void(ModuleStore::*)(unsigned long)>(&ModuleStore::Delete))
    .def("SetOptions", Wrap_ModuleStore_SetOptions)
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
    // python should never derive from ModuleBase, so a constructor should not be needed
    class_<ModuleBase, boost::noncopyable>("ModuleBase", no_init)
//    .def("MStore", &ModuleBase::MStore, return_internal_reference<>())
    .def("ID", &ModuleBase::ID)
    .def("Key", &ModuleBase::Key, return_value_policy<copy_const_reference>())
    .def("Name", &ModuleBase::Name, return_value_policy<copy_const_reference>())
    .def("Version", &ModuleBase::Version, return_value_policy<copy_const_reference>())
    .def("GetOption", &ModuleBase::GetOption<bpy::object>);

    register_ptr_to_python<boost::shared_ptr<Test_Base>>();
    class_<Test_Base_Wrap, bases<ModuleBase>, boost::shared_ptr<Test_Base_Wrap>, boost::noncopyable>("Test_Base", init<unsigned long, ModuleStore &, const ModuleInfo &>())
    .def("MStore", &Test_Base_Wrap::MStore, return_internal_reference<>())
    .def("ThrowException", &Test_Base_Wrap::ThrowException, ThrowException_overloads())
    .def("RunTest", pure_virtual(&Test_Base::RunTest))
    .def("CallRunTest", pure_virtual(&Test_Base::CallRunTest))
    .def("Throw", pure_virtual(&Test_Base::Throw))
    .def("CallThrow", pure_virtual(&Test_Base::CallThrow));

}


} // close namespace export_python
} // close namespace bpmodule

