#ifndef OUTPUT_PYTHON_H
#define OUTPUT_PYTHON_H

#include "BPModule/core/Output.hpp"
#include <boost/python.hpp>

namespace bpy = boost::python;



namespace bpmodule {
namespace export_python {

void Output_Wrap(bpmodule::output::OutputType type, const std::string fmt, bpy::list args);

void Output_Wrap_Output(const std::string fmt, bpy::list args);

void Output_Wrap_Success(const std::string fmt, bpy::list args);

void Output_Wrap_Changed(const std::string fmt, bpy::list args);

void Output_Wrap_Warning(const std::string fmt, bpy::list args);

void Output_Wrap_Error(const std::string fmt, bpy::list args);

void Output_Wrap_Debug(const std::string fmt, bpy::list args);


} // close namespace export_python
} // close namespace bpmodule


#endif
