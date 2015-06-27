#include "BPModule/core/Output.hpp"
#include "BPModule/core/OptionMap.hpp"
#include "BPModule/core/ModuleInfo.hpp"
#include "BPModule/export_core/Python_stdconvert.hpp"

namespace out = bpmodule::output;
namespace bpy = boost::python;
using bpmodule::export_python::ConvertListToVec;

namespace bpmodule {

// the "options" element of the dict must be a list of tuples
void ModuleInfo::FromPythonDict(const bpy::dict & dictionary)
{
    try
    {
        key = bpy::extract<std::string>(dictionary["key"]);
        name = bpy::extract<std::string>(dictionary["name"]);
        type = bpy::extract<std::string>(dictionary["type"]);
        path = bpy::extract<std::string>(dictionary["path"]);
        version = bpy::extract<std::string>(dictionary["version"]);
        description = bpy::extract<std::string>(dictionary["description"]);
        authors = ConvertListToVec<std::string>(bpy::extract<bpy::list>(dictionary["authors"]));
        refs = ConvertListToVec<std::string>(bpy::extract<bpy::list>(dictionary["refs"]));

        OptionMap op;
        bpy::list olist = bpy::extract<bpy::list>(dictionary["options"]);
        options.InitFromList(olist);

        if(dictionary.has_key("soname"))
            soname = bpy::extract<std::string>(dictionary["soname"]);

        //if(dictionary.has_key("path"))

    }
    catch(...)
    {
        out::Error("HEREEEE\n");
    }
}


} // close namespace bpmodule

