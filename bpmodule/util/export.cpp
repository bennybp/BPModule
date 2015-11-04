/*! \file
 *
 * \brief Python exports for output and printing functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/list.hpp>

#include "bpmodule/util/FormatStr.hpp"

//! \todo export the rest?

using namespace boost::python;



namespace bpmodule {
namespace util {
namespace export_python {


////////////////////////////
// Main boost python part
////////////////////////////

BOOST_PYTHON_MODULE(util)
{
    // format string
    def("FormatStr", FormatStrPy_);
}


} // close namespace export_python
} // close namespace output
} // close namespace bpmodule

