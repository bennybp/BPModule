#include "BPModule/export_core/Output_python.h"

namespace bpmodule {
namespace export_python {

// Wrap printing
void Output_Wrap(OutputType type, const std::string fmt, boost::python::list args)
{
    boost::format bfmt(fmt);

    int len = boost::python::extract<int>(args.attr("__len__")());

    for(int i = 0; i < len; i++)
    {
        std::string cl = boost::python::extract<std::string>(args[i].attr("__class__").attr("__name__"));

        if(cl == "int")
            bfmt % static_cast<int>(boost::python::extract<int>(args[i]));
        else if(cl == "float")
            bfmt % static_cast<double>(boost::python::extract<double>(args[i]));
        else if(cl == "str")
            bfmt % static_cast<std::string>(boost::python::extract<std::string>(args[i]));
        else // throw?
            bfmt % static_cast<std::string>(boost::python::extract<std::string>(args[i].attr("__str__")()));
    }

    Output_(GetOut(), type, boost::str(bfmt));
}



void Output_Wrap_Output(const std::string fmt, boost::python::list args)
{
    Output_Wrap(bpmodule::OutputType::Output, fmt, args);
}



void Output_Wrap_Success(const std::string fmt, boost::python::list args)
{
    Output_Wrap(bpmodule::OutputType::Success, fmt, args);
}



void Output_Wrap_Warning(const std::string fmt, boost::python::list args)
{
    Output_Wrap(bpmodule::OutputType::Warning, fmt, args);
}



void Output_Wrap_Error(const std::string fmt, boost::python::list args)
{
    Output_Wrap(bpmodule::OutputType::Error, fmt, args);
}



void Output_Wrap_Debug(const std::string fmt, boost::python::list args)
{
    Output_Wrap(bpmodule::OutputType::Debug, fmt, args);
}


} // close namespace export_python
} // close namespace bpmodule

