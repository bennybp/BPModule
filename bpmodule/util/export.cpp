/*! \file
 *
 * \brief Python exports for miscellaneous utilities
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/list.hpp>

#include "bpmodule/util/FormatString.hpp"


using namespace boost::python;


//! \todo export the rest?

namespace bpmodule {
namespace util {
namespace export_python {



////////////////////////////
// Main boost python part
////////////////////////////

BOOST_PYTHON_MODULE(util)
{
    // format string
    def("FormatString", FormatStringPy);
}


} // close namespace export_python
} // close namespace utilt
} // close namespace bpmodule

