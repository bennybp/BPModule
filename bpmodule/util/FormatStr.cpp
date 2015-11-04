/*! \file
 *
 * \brief Various string formatting functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/output/Output.hpp"


namespace bpmodule {
namespace util {


std::string FormatStrPy_(const std::string & fmt, const boost::python::list & args)
{
    std::stringstream ss;
    OutputPy_(ss, bpmodule::output::OutputType::Output, fmt, args);
    return ss.str();
}


} // close namespace util
} // close namespace bpmodule

