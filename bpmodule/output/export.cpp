/*! \file
 *
 * \brief Python exports for output and printing functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/output/Output.hpp"

#include <boost/python.hpp>

using namespace boost::python;



namespace bpmodule {
namespace output {
namespace export_python {


/*! \brief Wrap printing functions for use from python
 *
 * This function takes a boost::python list rather than the parameter pack
 *
 * \param [in] type The type of output
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap(output::OutputType type, const std::string & fmt, const boost::python::list & args)
{
    boost::format bfmt(fmt);

    int len = boost::python::extract<int>(args.attr("__len__")());

    for(int i = 0; i < len; i++)
    {
        std::string cl = boost::python::extract<std::string>(args[i].attr("__class__").attr("__name__"));

        if(cl == "int")
            bfmt % static_cast<int>(boost::python::extract<long>(args[i]));
        else if(cl == "float")
            bfmt % static_cast<double>(boost::python::extract<double>(args[i]));
        else if(cl == "str")
            bfmt % static_cast<std::string>(boost::python::extract<std::string>(args[i]));
        else //! \todo Throw exception when printing unknown python object?
            bfmt % static_cast<std::string>(boost::python::extract<std::string>(args[i].attr("__str__")()));
    }

    output::Output_(output::GetOut(), type, boost::str(bfmt));
}



/*! \brief Wrap general output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Output(const std::string & fmt, const boost::python::list & args)
{
    Output_Wrap(OutputType::Output, fmt, args);
}



/*! \brief Wrap 'success' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Success(const std::string & fmt, const boost::python::list & args)
{
    Output_Wrap(OutputType::Success, fmt, args);
}



/*! \brief Wrap 'changed' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Changed(const std::string & fmt, const boost::python::list & args)
{
    Output_Wrap(OutputType::Changed, fmt, args);
}



/*! \brief Wrap 'warning' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Warning(const std::string & fmt, const boost::python::list & args)
{
    Output_Wrap(OutputType::Warning, fmt, args);
}



/*! \brief Wrap 'error' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Error(const std::string & fmt, const boost::python::list & args)
{
    Output_Wrap(OutputType::Error, fmt, args);
}



/*! \brief Wrap 'debug' output for use from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output_Wrap_Debug(const std::string & fmt, const boost::python::list & args)
{
    Output_Wrap(OutputType::Debug, fmt, args);
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

