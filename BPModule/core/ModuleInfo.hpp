#ifndef MODULEINFO_H
#define MODULEINFO_H

#include <string>
#include <vector>

#include "BPModule/python_helper/BoostPython_fwd.hpp"
#include "BPModule/core/OptionMap.hpp"

namespace bpy = boost::python;

namespace bpmodule {


struct ModuleInfo
{
    std::string key;
    std::string name;
    std::string type;
    std::string path;
    std::string soname;
    std::string version;
    std::vector<std::string> authors;
    std::string description;
    std::vector<std::string> refs;
    OptionMap options;

    ModuleInfo(void) = default;
    ModuleInfo(const ModuleInfo & rhs) = default;
    ModuleInfo(ModuleInfo && rhs) = default;
    ~ModuleInfo(void) = default;

    // for python
    ModuleInfo(const bpy::dict & dictionary);
};


} // close namespace bpmodule


#endif
