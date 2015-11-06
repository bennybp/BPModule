/*! \file
 *
 * \brief Python exports for output and printing functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/list.hpp>

#include "bpmodule/output/Output.hpp"


using namespace boost::python;



namespace bpmodule {
namespace output {
namespace export_python {


/*! \brief Wrap general output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Output(const std::string & fmt, const boost::python::list & args)
{
    OutputPy_(output::GetOut(), detail::OutputType::Output, fmt, args);
}



/*! \brief Wrap 'success' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Success(const std::string & fmt, const boost::python::list & args)
{
    OutputPy_(output::GetOut(), detail::OutputType::Success, fmt, args);
}



/*! \brief Wrap 'changed' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Changed(const std::string & fmt, const boost::python::list & args)
{
    OutputPy_(output::GetOut(), detail::OutputType::Changed, fmt, args);
}



/*! \brief Wrap 'warning' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Warning(const std::string & fmt, const boost::python::list & args)
{
    OutputPy_(output::GetOut(), detail::OutputType::Warning, fmt, args);
}



/*! \brief Wrap 'error' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Error(const std::string & fmt, const boost::python::list & args)
{
    OutputPy_(output::GetOut(), detail::OutputType::Error, fmt, args);
}



/*! \brief Wrap 'debug' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Debug(const std::string & fmt, const boost::python::list & args)
{
    OutputPy_(output::GetOut(), detail::OutputType::Debug, fmt, args);
}



////////////////////////////
// Main boost python part
////////////////////////////

BOOST_PYTHON_MODULE(output)
{
    def("SetOut_Stdout", SetOut_Stdout);
    def("SetOut_Stderr", SetOut_Stderr);
    def("SetOut_File", SetOut_File);
    def("SetColor", SetColor);
    def("SetDebug", SetDebug);
    def("Valid", Valid);
    def("Flush", Flush);

    // printing to output
    def("Output", Output_Wrap_Output);
    def("Success", Output_Wrap_Success);
    def("Warning", Output_Wrap_Warning);
    def("Error", Output_Wrap_Error);
    def("Changed", Output_Wrap_Changed);
    def("Debug", Output_Wrap_Debug);
}


} // close namespace export_python
} // close namespace output
} // close namespace bpmodule

