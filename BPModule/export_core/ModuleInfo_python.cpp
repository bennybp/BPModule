#include "BPModule/core/Output.hpp"
#include "BPModule/export_core/OptionMap_python.hpp"
#include "BPModule/export_core/Python_stdconvert.hpp"
#include "BPModule/export_core/ModuleInfo_python.hpp"

namespace out = bpmodule::output;
namespace bpy = boost::python;

namespace bpmodule {
namespace export_python {

// the "options" element of the dict must be a list of tuples
ModuleInfo DictToModuleInfo(const bpy::dict & dictionary)
{
    ModuleInfo ret;
    try
    {
        ret.key = bpy::extract<std::string>(dictionary["key"]);
        ret.name = bpy::extract<std::string>(dictionary["name"]);
        ret.type = bpy::extract<std::string>(dictionary["type"]);
        ret.path = bpy::extract<std::string>(dictionary["path"]);
        ret.version = bpy::extract<std::string>(dictionary["version"]);
        ret.description = bpy::extract<std::string>(dictionary["description"]);
        ret.authors = ConvertListToVec<std::string>(bpy::extract<bpy::list>(dictionary["authors"]));
        ret.refs = ConvertListToVec<std::string>(bpy::extract<bpy::list>(dictionary["refs"]));

        OptionMap op;
        bpy::list olist = bpy::extract<bpy::list>(dictionary["options"]);
        ret.options = OptionMap_InitFromList_Helper(olist);

        if(dictionary.has_key("soname"))
            ret.soname = bpy::extract<std::string>(dictionary["soname"]);

        //if(dictionary.has_key("path"))

    }
    catch(...)
    {
        out::Error("HEREEEE\n");
    }

    return ret;
}


} // close namespace export_python
} // close namespace bpmodule

