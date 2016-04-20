/*! \file
 *
 * \brief Python exports for python helper functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/python/Pybind11.hpp"

namespace bpmodule {
namespace python {
namespace export_python {


PYBIND11_PLUGIN(python)
{
    pybind11::module m("python", "Some helper functions for python stuff");

    return m.ptr();
}


} // close namespace export_python
} // close namespace python
} // close namespace bpmodule

