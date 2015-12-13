/*! \file
 *
 * \brief Python exports for output and printing functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/output/Output.hpp"


namespace bpmodule {
namespace output {
namespace export_python {


/*! \brief Wrap general output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Output(const std::string & fmt, const pybind11::list & args)
{
    OutputPy_(output::GetOut(), detail::OutputType::Output, fmt, args);
}



/*! \brief Wrap 'success' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Success(const std::string & fmt, const pybind11::list & args)
{
    OutputPy_(output::GetOut(), detail::OutputType::Success, fmt, args);
}



/*! \brief Wrap 'changed' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Changed(const std::string & fmt, const pybind11::list & args)
{
    OutputPy_(output::GetOut(), detail::OutputType::Changed, fmt, args);
}



/*! \brief Wrap 'warning' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Warning(const std::string & fmt, const pybind11::list & args)
{
    OutputPy_(output::GetOut(), detail::OutputType::Warning, fmt, args);
}



/*! \brief Wrap 'error' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Error(const std::string & fmt, const pybind11::list & args)
{
    OutputPy_(output::GetOut(), detail::OutputType::Error, fmt, args);
}



/*! \brief Wrap 'debug' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Debug(const std::string & fmt, const pybind11::list & args)
{
    OutputPy_(output::GetOut(), detail::OutputType::Debug, fmt, args);
}



////////////////////////////
// Main boost python part
////////////////////////////

PYBIND11_PLUGIN(output)
{
    pybind11::module m("output", "Output functionality");

    m.def("SetOut_Stdout", SetOut_Stdout);
    m.def("SetOut_Stderr", SetOut_Stderr);
    m.def("SetOut_File", SetOut_File);
    m.def("SetColor", SetColor);
    m.def("SetDebug", SetDebug);
    m.def("Valid", Valid);
    m.def("Flush", Flush);

    // printing to output
    m.def("Output_", Output_Wrap_Output);
    m.def("Success_", Output_Wrap_Success);
    m.def("Warning_", Output_Wrap_Warning);
    m.def("Error_", Output_Wrap_Error);
    m.def("Changed_", Output_Wrap_Changed);
    m.def("Debug_", Output_Wrap_Debug);

    return m.ptr();
}


} // close namespace export_python
} // close namespace output
} // close namespace bpmodule

