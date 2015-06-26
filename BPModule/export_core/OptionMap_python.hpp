#ifndef OPTIONMAP_PYTHON_H
#define OPTIONMAP_PYTHON_H

#include "BPModule/core/OptionMap.hpp"

#include <boost/python.hpp>

namespace bpy = boost::python;



namespace bpmodule {

namespace export_python {


std::unique_ptr<OptionPlaceholder> OptionPlaceholder_FromPython(const bpy::object & value);

bpy::object OptionMap_Get_Helper(const OptionMap * op, const std::string & key);
void OptionMap_Change_Helper(OptionMap * op, const std::string & key, const bpy::object & value);
void OptionMap_InitDefault_Helper(OptionMap * op, const std::string & key, const bpy::object & def, const std::string & help);

OptionMap OptionMap_InitFromList_Helper(const bpy::list & olist);


} // close namespace export_python
} // close namespace bpmodule

#endif
