#include "BPModule/core/Exception.hpp"
#include "BPModule/export_core/Output_python.hpp"

namespace bpy = boost::python;



namespace bpmodule {
namespace export_python {

// Wrap printing
void Output_Wrap(output::OutputType type, const std::string fmt, bpy::list args)
{
    boost::format bfmt(fmt);

    int len = bpy::extract<int>(args.attr("__len__")());

    for(int i = 0; i < len; i++)
    {
        std::string cl = bpy::extract<std::string>(args[i].attr("__class__").attr("__name__"));

        if(cl == "int")
            bfmt % static_cast<int>(bpy::extract<long>(args[i]));
        else if(cl == "float")
            bfmt % static_cast<double>(bpy::extract<double>(args[i]));
        else if(cl == "str")
            bfmt % static_cast<std::string>(bpy::extract<std::string>(args[i]));
        else // throw?
            bfmt % static_cast<std::string>(bpy::extract<std::string>(args[i].attr("__str__")()));
    }

    output::Output_(output::GetOut(), type, boost::str(bfmt));
}



void Output_Wrap_Output(const std::string fmt, bpy::list args)
{
    Output_Wrap(bpmodule::output::OutputType::Output, fmt, args);
}



void Output_Wrap_Success(const std::string fmt, bpy::list args)
{
    Output_Wrap(bpmodule::output::OutputType::Success, fmt, args);
}



void Output_Wrap_Changed(const std::string fmt, bpy::list args)
{
    Output_Wrap(bpmodule::output::OutputType::Changed, fmt, args);
}



void Output_Wrap_Warning(const std::string fmt, bpy::list args)
{
    Output_Wrap(bpmodule::output::OutputType::Warning, fmt, args);
}



void Output_Wrap_Error(const std::string fmt, bpy::list args)
{
    Output_Wrap(bpmodule::output::OutputType::Error, fmt, args);
}



void Output_Wrap_Debug(const std::string fmt, bpy::list args)
{
    Output_Wrap(bpmodule::output::OutputType::Debug, fmt, args);
}


} // close namespace export_python
} // close namespace bpmodule

