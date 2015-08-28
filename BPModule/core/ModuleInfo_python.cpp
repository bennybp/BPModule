#include "BPModule/core/Output.hpp"
#include "BPModule/core/ModuleInfo.hpp"
#include "BPModule/python_helper/Python_stdconvert.hpp"

namespace out = bpmodule::output;
namespace bpy = boost::python;
using bpmodule::export_python::ConvertListToVec;

namespace bpmodule {

// the "options" element of the dict must be a list of tuples
ModuleInfo::ModuleInfo(const bpy::dict & dictionary)
{
    key = bpy::extract<std::string>(dictionary["key"]);
    name = bpy::extract<std::string>(dictionary["name"]);
    type = bpy::extract<std::string>(dictionary["type"]);
    path = bpy::extract<std::string>(dictionary["path"]);
    version = bpy::extract<std::string>(dictionary["version"]);
    description = bpy::extract<std::string>(dictionary["description"]);
    authors = ConvertListToVec<std::string>(bpy::extract<bpy::list>(dictionary["authors"]));
    refs = ConvertListToVec<std::string>(bpy::extract<bpy::list>(dictionary["refs"]));

    if(dictionary.has_key("passedoptions"))
    {
        OptionMap op;
        bpy::list olist = bpy::extract<bpy::list>(dictionary["passedoptions"]);
        options = OptionMap(olist);
    }

    if(dictionary.has_key("soname"))
        soname = bpy::extract<std::string>(dictionary["soname"]);
}


} // close namespace bpmodule

