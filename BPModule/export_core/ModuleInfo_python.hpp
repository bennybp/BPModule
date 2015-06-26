#ifndef MODULEINFO_PYTHON_H
#define MODULEINFO_PYTHON_H

#include "BPModule/core/ModuleInfo.hpp"

#include <boost/python.hpp>

namespace bpy = boost::python;


namespace bpmodule {
namespace export_python {

ModuleInfo DictToModuleInfo(const bpy::dict & dictionary);


} // close namespace export_python
} // close namespace bpmodule

#endif
