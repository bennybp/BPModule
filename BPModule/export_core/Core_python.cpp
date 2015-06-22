#include "BPModule/core/OptionMap.h"
#include "BPModule/core/Output.h"
#include "BPModule/core/ModuleStore.h"

// All the module base classes
#include "BPModule/modulebase/All.h"

// python stuff
#include "BPModule/export_core/Python_stdconvert.h"
#include <boost/python.hpp>

using namespace boost::python;

namespace bpmodule {

// Some wrappers for OptionMap Get/Set member
void OptionMap_Set_Helper(OptionMap * op, const std::string & key, 
                          const boost::python::object & value,
                          const std::string & help)
{
    std::string cl = extract<std::string>(value.attr("__class__").attr("__name__"));

    if(cl == "int")
        op->Set(key, static_cast<int>(extract<int>(value)), help);
    else if(cl == "float")
        op->Set(key, static_cast<double>(extract<double>(value)), help);
    else if(cl == "str")
        op->Set(key, static_cast<std::string>(extract<std::string>(value)), help);
    else if(cl == "list")
    {
        // get type of first element
        boost::python::list lst = boost::python::extract<boost::python::list>(value);
        int length = extract<int>(lst.attr("__len__")());
        if(length == 0)
        {
            // plain string vector if empty
            std::vector<std::string> v;
            op->Set(key, std::vector<std::string>(), help);
        } 
        else
        {
            std::string cl2 = extract<std::string>(lst[0].attr("__class__").attr("__name__"));
            if(cl2 == "int")
                op->Set(key, ConvertListToVec<int>(lst), help);
            else if(cl2 == "float")
                op->Set(key, ConvertListToVec<double>(lst), help);
            else if(cl2 == "str")
                op->Set(key, ConvertListToVec<std::string>(lst), help);
            else
                Error("Unknown type %1% in list for key %2%\n", cl, key);
        }
    }
    else
        Error("Unknown type %1% for key %2%\n", cl, key);
}



boost::python::object OptionMap_Get_Helper(const OptionMap * op, const std::string & key)
{
    std::string type = op->GetType(key);

    if(type.size() == 0)
        return object();
    else if(type == typeid(int).name())
        return object(op->Get<int>(key));
    else if(type == typeid(double).name())
        return object(op->Get<double>(key));
    else if(type == typeid(std::string).name())
        return object(op->Get<std::string>(key));
    else if(type == typeid(std::vector<int>).name())
        return object(op->Get<std::vector<int>>(key));
    else if(type == typeid(std::vector<double>).name())
        return object(op->Get<std::vector<double>>(key));
    else if(type == typeid(std::vector<std::string>).name())
        return object(op->Get<std::vector<std::string>>(key));
    else
    {
        Error("Unable to deduce type: %1%\n", type);
        return object();
    }
}



// Convert lists <-> OptionMaps
OptionMap ListToOptionMap(const boost::python::list & olist)
{
    OptionMap op;
    int optlen = extract<int>(olist.attr("__len__")());

    for(int i = 0; i < optlen; i++)
    {
        std::string key = extract<std::string>(olist[i][0]);
        std::string help = extract<std::string>(olist[i][2]);
        OptionMap_Set_Helper(&op, key, olist[i][1], help); 
    }
    return op;
}



boost::python::list OptionMapToList(const OptionMap & op)
{
    boost::python::list lst;
    auto keys = op.GetKeys();

    for(auto & it : keys)
        lst.append(boost::python::make_tuple(it, OptionMap_Get_Helper(&op, it), op.GetHelp(it)));

    return lst;
}




// the "options" element of the dict must be a list of tuples
ModuleInfo DictToModuleInfo(const dict & dictionary)
{
    ModuleInfo ret;
    try {
        ret.name = extract<std::string>(dictionary["name"]);
        ret.soname = extract<std::string>(dictionary["soname"]);
        ret.version = extract<std::string>(dictionary["version"]);
        ret.description = extract<std::string>(dictionary["description"]);
        ret.authors = ConvertListToVec<std::string>(extract<list>(dictionary["authors"]));
        ret.refs = ConvertListToVec<std::string>(extract<list>(dictionary["refs"]));

        OptionMap op;
        boost::python::list olist = extract<boost::python::list>(dictionary["options"]);
        ret.options = ListToOptionMap(olist);
    }
    catch(...)
    {
        Error("HEREEEE\n");
    }   

    return ret;
}



dict ModuleInfoToDict(const ModuleInfo & mi)
{
    dict d;

    //simple ones first
    d["name"] = mi.name;
    d["soname"] = mi.soname;
    d["version"] = mi.version;
    d["description"] = mi.description;
    d["sopath"] = mi.sopath;

    // now lists of strings
    d["authors"] = ConvertVecToList(mi.authors);
    d["refs"] = ConvertVecToList(mi.refs);

    // now options
    d["options"] = OptionMapToList(mi.options); 

    return d;
}


// Converter for OptionMap
struct OptionMapConverter
{
    static PyObject* convert(const OptionMap & op)
    {
      boost::python::list lst = OptionMapToList(op);
      return boost::python::incref(lst.ptr());
    }
};



// Converter for ModuleInfo
struct ModuleInfoConverter
{
    static PyObject* convert(const ModuleInfo & m)
    {
      boost::python::dict d = ModuleInfoToDict(m);
      return boost::python::incref(d.ptr());
    }
};



// the main exception translator
void TranslateException(const BPModuleException & ex)
{
    PyErr_SetString(PyExc_RuntimeError, bpmodule::ExceptionString(ex).c_str());
}



// wraps ModuleStore::LoadSO so that it can take a dict for the ModuleInfo
bool Wrap_ModuleStore_LoadSO(ModuleStore * ms, const std::string & key,
                             const std::string & sopath, const boost::python::dict & d)
{
    return ms->LoadSO(key, sopath, DictToModuleInfo(d));
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
    def("SetOut_Stdout", bpmodule::SetOut_Stdout);
    def("SetOut_File", bpmodule::SetOut_File);
    def("SetColor", bpmodule::SetColor);
    def("SetDebug", bpmodule::SetDebug);

/*
    // commented since it is now converted to a list
    class_<OptionMap>("OptionMap")
        .def("Set", Wrap_OptionMap_Set)
        .def("Set", OptionMap_Set_Helper)
        .def("Get", OptionMap_Get_Helper)
        .def("Has", &OptionMap::Has)
        .def("GetType", &OptionMap::GetType)
        .def("GetHelp", &OptionMap::GetHelp)
        .def("GetAllHelp", &OptionMap::GetAllHelp)
        .def("GetKeys", &OptionMap::GetKeys)
        .def("Size", &OptionMap::Size);
*/
/*
    // commented since it is now converted to a dict
    class_<ModuleInfo>("ModuleInfo")
           .def_readwrite("name", &ModuleInfo::name)
           .def_readwrite("soname", &ModuleInfo::soname)
           .def_readwrite("version", &ModuleInfo::version)
           .def_readwrite("authors", &ModuleInfo::authors)
           .def_readwrite("description", &ModuleInfo::description)
           .def_readwrite("refs", &ModuleInfo::refs)
           .def_readwrite("options", &ModuleInfo::options);
*/

    class_<ModuleStore, boost::noncopyable>("ModuleStore")
           .def("LoadSO", Wrap_ModuleStore_LoadSO)
           .def("Lock", &ModuleStore::Lock)
           .def("Size", &ModuleStore::Size)
           .def("Has", &ModuleStore::Has)
           .def("GetKeys", &ModuleStore::GetKeys)
           .def("ModuleInfoFromKey", &ModuleStore::ModuleInfoFromKey)
           .def("GetModule", &ModuleStore::GetModule<ModuleBase>, return_internal_reference<>())
           .def("GetModule_Test", &ModuleStore::GetModule<Test_Base>, return_internal_reference<>());


    ///////////////////////
    // Module Base classes
    ///////////////////////
    class_<ModuleBase, boost::noncopyable>("ModuleBase", no_init)
           .def("Traits", &ModuleBase::Traits)
           .def("Options", &ModuleBase::Options);

    class_<Test_Base, bases<ModuleBase>, boost::noncopyable>("Test_Base", no_init)
           .def("RunTest", &Test_Base::RunTest);

}



} // close namespace bpmodule

